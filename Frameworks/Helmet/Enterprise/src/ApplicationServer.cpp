//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "ApplicationServer.hpp"

#include <Helmet/Enterprise/I_ApplicationService.hpp>
#include <Helmet/Enterprise/I_ResourceLocation.hpp>
#include <Helmet/Enterprise/I_ProtocolService.hpp>
#include <Helmet/Enterprise/I_Request.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::ApplicationServer()
:   m_sharedThreadPool(16, nullptr, true, true)
,   m_installQueue(1, nullptr, true, false)
,   m_shutdownQueue(1, nullptr, true, false)
,   m_pApplicationGuard(Core::Thread::MutexFactory::create())
,   m_applicationServices()
,   m_pProtocolGuard(Core::Thread::MutexFactory::create())
,   m_protocolServices()
,   m_pStartCondition(Core::Thread::ConditionFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::~ApplicationServer()
{
    Core::Thread::MutexFactory::destroy(m_pProtocolGuard);
    Core::Thread::MutexFactory::destroy(m_pApplicationGuard);
    Core::Thread::ConditionFactory::destroy(m_pStartCondition);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Core::Thread::I_Condition*
ApplicationServer::start()
{
    /// @name Internal Structures
    /// @{
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class InstallationCompleteTask
    :   public Core::Thread::ThreadPool::Task
    {
        /// @name Internal Structures
        /// @{
    private:
        //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
        class PrepareToStartCompleteTask
        :   public Core::Thread::ThreadPool::Task
        {
            /// @name Internal Structures
            /// @{
        private:
            //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
            class StartCompleteTask
            :   public Core::Thread::ThreadPool::Task
            {
                /// @name Task implementation
                /// @{
            public:
                void call() override
                {
                    m_condition.assertCondition();
                }
                /// @}

                /// @name 'Structors
                /// @{
            public:
                explicit StartCompleteTask(Core::Thread::I_Condition& _condition)
                :   m_condition(_condition)
                {
                }
                /// @}

                /// @name Member Variables
                /// @{
            private:
                Core::Thread::I_Condition&  m_condition;
                /// @}
            };
            //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
            /// @}

            /// @name Task implementation
            /// @{
        public:
            void call() override
            {
                auto* pTask = new StartCompleteTask(m_condition);
                m_installQueue.pushRequest(pTask);
            }
            /// @}

            /// @name 'Structors
            /// @{
        public:
            PrepareToStartCompleteTask(Core::Thread::I_Condition& _condition,
                                       Core::Thread::ThreadPool& _installQueue)
            :   m_condition(_condition)
            ,   m_installQueue(_installQueue)
            {
            }
            /// @}

            /// @name Member Variables
            /// @{
        private:
            Core::Thread::I_Condition&  m_condition;
            Core::Thread::ThreadPool&   m_installQueue;
            /// @}
        };
        //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
        /// @}

        /// @name Task implementation
        /// @{
    public:
        void call() override
        {
            auto* pTask = new PrepareToStartCompleteTask(m_condition, m_installQueue);
            m_installQueue.pushRequest(pTask);
        }
        /// @}

        /// @name 'Structors
        /// @{
    public:
        InstallationCompleteTask(Core::Thread::I_Condition& _condition,
                                 Core::Thread::ThreadPool& _installQueue)
        :   m_condition(_condition)
        ,   m_installQueue(_installQueue)
        {
        }
        /// @}

        /// @name Member Variables
        /// @{
    private:
        Core::Thread::I_Condition&  m_condition;
        Core::Thread::ThreadPool&   m_installQueue;
        /// @}
    };
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    // Create the installation complete task and pass the condition into it.
    auto* pTask = new InstallationCompleteTask(*m_pStartCondition, m_installQueue);

    // Push the task onto the end of the queue
    m_installQueue.pushRequest(pTask);

    // Start the installation queue so that protocols and services will be
    // installed.
    m_installQueue.start();

    return m_pStartCondition;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::stop()
{
    /// @name Internal Structures
    /// @{
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class InstallationQueueEmptyTask
    :   public Core::Thread::ThreadPool::Task
    {
        /// @name Task implementation
        /// @{
    public:
        void call() override
        {
            m_condition.assertCondition();
        }
        /// @}

        /// @name 'Structors
        /// @{
    public:
        explicit InstallationQueueEmptyTask(Core::Thread::I_Condition& _condition)
        :   m_condition(_condition)
        {
        }
        /// @}

        /// @name Member Variables
        /// @{
    private:
        Core::Thread::I_Condition&  m_condition;
        /// @}
    };
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class ShutdownCompleteTask
    :   public Core::Thread::ThreadPool::Task
    {
        /// @name Task implementation
        /// @{
    public:
        void call() override
        {
            m_condition.assertCondition();
        }
        /// @}

        /// @name 'Structors
        /// @{
    public:
        explicit ShutdownCompleteTask(Core::Thread::I_Condition& _condition)
        :   m_condition(_condition)
        {
        }
        /// @}

        /// @name Member Variables
        /// @{
    private:
        Core::Thread::I_Condition&  m_condition;
        /// @}
    };
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    // Create a condition to know when the installQueue is empty.
    auto* pInstallerCondition = Core::Thread::ConditionFactory::create(false);

    // Create the installation complete task and pass the condition into it.
    auto* pInstallerTask = new InstallationQueueEmptyTask(*pInstallerCondition);

    // Push the task onto the end of the queue
    m_installQueue.pushRequest(pInstallerTask);

    // Prepare to stop the installation queue so that no more protocols or
    // services will be installed (at least not until the app server is
    // restarted, if ever).
    m_installQueue.prepareToStop();

    // Wait for the install queue to be emptied.
    pInstallerCondition->requireCondition();

    // Destroy the condition
    Core::Thread::ConditionFactory::destroy(pInstallerCondition);

    // Stop the install queue
    m_installQueue.stop();

    // Create a condition to know when the shutdownQueue is finished.
    auto* pShutdownCondition = Core::Thread::ConditionFactory::create(false);

    // Create the shutdown complete task and pass the condition into it.
    auto* pShutdownTask = new ShutdownCompleteTask(*pShutdownCondition);

    // Push the task onto the end of the queue
    m_shutdownQueue.pushRequest(pShutdownTask);

    // Start the shutdown queue
    m_shutdownQueue.start();

    // Wait on the shutdown queue to completely start
    m_shutdownQueue.requireStarted();

    // Prepare to stop the shutdown queue so that no more tasks will
    // be pushed onto it.
    m_shutdownQueue.prepareToStop();

    // Wait for the shutdown queue to be emptied.
    pShutdownCondition->requireCondition();

    // Destroy the condition
    Core::Thread::ConditionFactory::destroy(pShutdownCondition);

    // Stop the shutdown queue
    m_shutdownQueue.stop();

    // TODO Empty service collections
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::installApplication(pApplicationService_type _pApplicationService,
                                      pResourceLocation_type _pRootLocation)
{
    /// @name Internal Structures
    /// @{
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class InstallApplicationServiceTask
    :   public Core::Thread::ThreadPool::Task
    {
        /// @name Internal Structures
        /// @{
        //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
        class PrepareToStartApplicationServiceTask
        :   public Core::Thread::ThreadPool::Task
        {
            /// @name Internal Structures
            /// @{
        private:
            //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
            class StartApplicationServiceTask
            :   public Core::Thread::ThreadPool::Task
            {
                /// @name Task implementation
                /// @{
            public:
                void call() override
                {
                    m_pApplicationService->start();
                }
                /// @}

                /// @name 'Structors
                /// @{
            public:
                StartApplicationServiceTask(pApplicationService_type _pApplicationService)
                :   m_pApplicationService(_pApplicationService)
                {
                }
                /// @}

                /// @name Member Variables
                /// @{
            private:
                pApplicationService_type    m_pApplicationService;
                /// @}
            };
            //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
            class StopApplicationServiceTask
            :   public Core::Thread::ThreadPool::Task{
                /// @name Task implementation
                /// @{
            public:
                void call() override
                {
                    m_pApplicationService->stop();
                }
                /// @}

                /// @name 'Structors
                /// @{
            public:
                StopApplicationServiceTask(pApplicationService_type _pApplicationService)
                :   m_pApplicationService(_pApplicationService)
                {
                }
                /// @}

                /// @name Member Variables
                /// @{
            private:
                pApplicationService_type    m_pApplicationService;
                /// @}
            };
            //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
            /// @}

            /// @name Task implementation
            /// @{
        public:
            void call() override
            {
                auto* pCondition = m_pApplicationService->prepareToStart(m_server.getSharedThreadPool());
                if (pCondition != NULL)
                {
                    pCondition->requireCondition();
                }

                auto* pStartTask = new StartApplicationServiceTask(m_pApplicationService);
                auto* pStopTask = new StopApplicationServiceTask(m_pApplicationService);
                m_installQueue.pushRequest(pStartTask);
                m_shutdownQueue.pushRequest(pStopTask);
            }
            /// @}

            /// @name 'Structors
            /// @{
        public:
            PrepareToStartApplicationServiceTask(ApplicationServer& _server,
                                                 pApplicationService_type _pApplicationService,
                                                 Core::Thread::ThreadPool& _installQueue,
                                                 Core::Thread::ThreadPool& _shutdownQueue)
            :   m_server(_server)
            ,   m_pApplicationService(_pApplicationService)
            ,   m_installQueue(_installQueue)
            ,   m_shutdownQueue(_shutdownQueue)
            {
            }
            /// @}

            /// @name Member Variables
            /// @{
        private:
            ApplicationServer&          m_server;
            pApplicationService_type    m_pApplicationService;
            Core::Thread::ThreadPool&   m_installQueue;
            Core::Thread::ThreadPool&   m_shutdownQueue;
            /// @}
        };
        //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
        class PrepareToStopApplicationServiceTask
        :   public Core::Thread::ThreadPool::Task
        {
            /// @name Task implementation
            /// @{
        public:
            void call() override {
                auto* pCondition = m_pApplicationService->prepareToStop();
                if (pCondition != nullptr)
                {
                    pCondition->requireCondition();
                }
            }
            /// @}

            /// @name 'Structors
            /// @{
        public:
            PrepareToStopApplicationServiceTask(pApplicationService_type _pApplicationService)
            :   m_pApplicationService(_pApplicationService)
            {
            }
            /// @}

            /// @name Member Variables
            /// @{
        private:
            pApplicationService_type    m_pApplicationService;
            /// @}
        };
        //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
        /// @}

        /// @name Task implementation
        /// @{
    public:
        void call() override
        {
            m_server.handleInstallApplication(m_pApplicationService, m_pRootLocation);
            auto* pStartTask = new PrepareToStartApplicationServiceTask(
                    m_server,
                    m_pApplicationService,
                    m_installQueue,
                    m_shutdownQueue
            );

            auto* pStopTask = new PrepareToStopApplicationServiceTask(
                    m_pApplicationService
            );

            m_installQueue.pushRequest(pStartTask);
            m_shutdownQueue.pushRequest(pStopTask);
            // TODO set up application install handler.
        }
        /// @}

        /// @name 'Structors
        /// @{
    public:
        InstallApplicationServiceTask(ApplicationServer& _server,
                                      pApplicationService_type _pApplicationService,
                                      pResourceLocation_type _pRootLocation,
                                      Core::Thread::ThreadPool& _installQueue,
                                      Core::Thread::ThreadPool& _shutdownQueue)
        :   m_server(_server)
        ,   m_pApplicationService(_pApplicationService)
        ,   m_pRootLocation(_pRootLocation)
        ,   m_installQueue(_installQueue)
        ,   m_shutdownQueue(_shutdownQueue)
        {
        }
        /// @}

        /// @name Member Variables
        /// @{
    private:
        ApplicationServer&          m_server;
        pApplicationService_type    m_pApplicationService;
        pResourceLocation_type      m_pRootLocation;
        Core::Thread::ThreadPool&   m_installQueue;
        Core::Thread::ThreadPool&   m_shutdownQueue;
        /// @}
    };
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    auto* pTask = new InstallApplicationServiceTask(
            *this,
            _pApplicationService,
            _pRootLocation,
            m_installQueue,
            m_shutdownQueue
    );

    m_installQueue.pushRequest(pTask);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::installProtocol(pProtocolService_type _pProtocolService)
{
    /// @name Internal Structures
    /// @{
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class InstallProtocolServiceTask
    :   public Core::Thread::ThreadPool::Task
    {
        /// @name Internal Structures
        /// @{
    private:
        //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
        class PrepareToStartProtocolServiceTask
        :   public Core::Thread::ThreadPool::Task
        {
            /// @name Internal Structures
            /// @{
        private:
            //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
            class StartProtocolServiceTask
            :   public Core::Thread::ThreadPool::Task
            {
                /// @name Task implementation
                /// @{
            public:
                void call() override
                {
                    m_pProtocolService->start();
                }
                /// @}

                /// @name 'Structors
                /// @{
            public:
                StartProtocolServiceTask(pProtocolService_type _pProtocolService)
                :   m_pProtocolService(_pProtocolService)
                {
                }
                /// @}

                /// @name Member Variables
                /// @{
            private:
                pProtocolService_type   m_pProtocolService;
                /// @}
            };
            //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
            class StopProtocolServiceTask
            :   public Core::Thread::ThreadPool::Task
            {
                /// @name Task implementation
                /// @{
            public:
                void call() override
                {
                    m_pProtocolService->stop();
                }
                /// @}

                /// @name 'Structors
                /// @{
            public:
                StopProtocolServiceTask(pProtocolService_type _pProtocolService)
                :   m_pProtocolService(_pProtocolService)
                        {
                        }
                /// @}

                /// @name Member Variables
                /// @{
            private:
                pProtocolService_type   m_pProtocolService;
                /// @}
            };
            //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
            /// @}

            /// @name Task implementation
            /// @{
        public:
            void call() override
            {
                auto* pCondition = m_pProtocolService->prepareToStart(m_server.getSharedThreadPool());
                if (pCondition != nullptr)
                {
                    pCondition->requireCondition();
                }

                auto* pStartTask = new StartProtocolServiceTask(m_pProtocolService);
                auto* pStopTask = new StopProtocolServiceTask(m_pProtocolService);
                m_installQueue.pushRequest(pStartTask);
                m_shutdownQueue.pushRequest(pStopTask);
            }
            /// @}

            /// @name 'Structors
            /// @{
        public:
            PrepareToStartProtocolServiceTask(ApplicationServer& _server,
                                              pProtocolService_type _pProtocolService,
                                              Core::Thread::ThreadPool& _installQueue,
                                              Core::Thread::ThreadPool& _shutdownQueue)
            :   m_server(_server)
            ,   m_pProtocolService(_pProtocolService)
            ,   m_installQueue(_installQueue)
            ,   m_shutdownQueue(_shutdownQueue)
            {
            }
            /// @}

            /// @name Member Variables
            /// @{
        private:
            ApplicationServer&          m_server;
            pProtocolService_type       m_pProtocolService;
            Core::Thread::ThreadPool&   m_installQueue;
            Core::Thread::ThreadPool&   m_shutdownQueue;
            /// @}
        };
        //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
        class PrepareToStopProtocolServiceTask
        :   public Core::Thread::ThreadPool::Task
        {
            /// @name Task implementation
            /// @{
        public:
            void call() override
            {
                auto* pCondition = m_pProtocolService->prepareToStop();
                if (pCondition != nullptr)
                {
                    pCondition->requireCondition();
                }
            }
            /// @}

            /// @name 'Structors
            /// @{
        public:
            PrepareToStopProtocolServiceTask(pProtocolService_type _pProtocolService)
            :   m_pProtocolService(_pProtocolService)
            {
            }
            /// @}

            /// @name Member Variables
            /// @{
        private:
            pProtocolService_type   m_pProtocolService;
            /// @}
        };
        //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
        /// @}

        /// @name Task implementation
        /// @{
    public:
        void call() override
        {
            m_server.handleInstallProtocol(m_pProtocolService);
            auto* pStartTask = new PrepareToStartProtocolServiceTask(
                    m_server,
                    m_pProtocolService,
                    m_installQueue,
                    m_shutdownQueue);

            auto* pStopTask =
                    new PrepareToStopProtocolServiceTask(m_pProtocolService);

            m_installQueue.pushRequest(pStartTask);
            m_shutdownQueue.pushRequest(pStopTask);
        }
        /// @}

        /// @name 'Structors
        /// @{
    public:
        InstallProtocolServiceTask(ApplicationServer& _server,
                                   pProtocolService_type _pProtocolService,
                                   Core::Thread::ThreadPool& _installQueue,
                                   Core::Thread::ThreadPool& _shutdownQueue)
        :   m_server(_server)
        ,   m_pProtocolService(_pProtocolService)
        ,   m_installQueue(_installQueue)
        ,   m_shutdownQueue(_shutdownQueue)
        {
        }
        /// @}

        /// @name Member Variables
        /// @{
    private:
        ApplicationServer&          m_server;
        pProtocolService_type       m_pProtocolService;
        Core::Thread::ThreadPool&   m_installQueue;
        Core::Thread::ThreadPool&   m_shutdownQueue;
        /// @}
    };
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    /// Schedule the installation of the protocol service.
    auto* pTask = new InstallProtocolServiceTask(
            *this,
            _pProtocolService,
            m_installQueue,
            m_shutdownQueue
    );

    m_installQueue.pushRequest(pTask);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::pApplicationService_type
ApplicationServer::getApplication(pResourceLocation_type _pServiceLocation) const
{
    m_pStartCondition->requireCondition();

    Core::Thread::CriticalSection lock(m_pApplicationGuard);

    ApplicationServices_type::const_iterator iter = m_applicationServices.find(_pServiceLocation);

    if (iter != m_applicationServices.end())
    {
        return iter->second;
    }
    else
    {
        return pApplicationService_type();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::pProtocolService_type
ApplicationServer::getProtocol(const std::string &_protocolName)
{
    m_pStartCondition->requireCondition();

    Core::Thread::CriticalSection lock(m_pProtocolGuard);

    ProtocolServices_type::iterator iter = m_protocolServices.find(_protocolName);

    if (iter != m_protocolServices.end())
    {
        return iter->second;
    }
    else
    {
        return pProtocolService_type();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::handleMessage(pMessage_type _pMessage)
{
    m_sharedThreadPool.pushRequest(_pMessage->sendMessageTask());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::handleRequest(pRequest_type _pRequest,
                                 pResponseHandler_type _pResponseHandler)
{
    m_sharedThreadPool.pushRequest(_pRequest->sendRequestTask(_pResponseHandler));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::handleInstallApplication(pApplicationService_type _pApplicationService,
                                            pResourceLocation_type _pRootLocation)
{
    Core::Thread::CriticalSection lock(m_pApplicationGuard);

    m_applicationServices[_pRootLocation] = _pApplicationService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::handleInstallProtocol(pProtocolService_type _pProtocolService)
{
    Core::Thread::CriticalSection lock(m_pProtocolGuard);
    m_protocolServices[_pProtocolService->getName()] = _pProtocolService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/*
 * //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2016 Raymond A. Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
// Copyright (C)        2009 Joshua Cassity
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "ApplicationServer.hpp"

#include "ResourceLocation.hpp"
#include "NumericTypeMessageRegistry.hpp"

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/ConditionFactory.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/I_Condition.hpp>
#include <Zen/Core/Threading/I_Thread.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Scripting/I_ScriptableService.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>
#include <Zen/Enterprise/Networking/I_Address.hpp>

#include <Zen/Enterprise/Database/I_DatabaseManager.hpp>
#include <Zen/Enterprise/Database/I_DatabaseService.hpp>
#include <Zen/Enterprise/Database/I_DatabaseConnection.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void destroy(Memory::managed_weak_ptr<I_MessageRegistry> _pMessageRegistry)
{
    NumericTypeMessageRegistry* pMessageRegistry = dynamic_cast<NumericTypeMessageRegistry*>(_pMessageRegistry.get());

    if (pMessageRegistry)
    {
        delete pMessageRegistry;
    }
    else
    {
        // TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::ApplicationServer()
:   m_pEventService(Event::I_EventManager::getSingleton().create("eventService"))
,   m_sharedThreadPool(16, NULL, true, true)
,   m_installQueue(1, NULL, true, false)
,   m_shutdownQueue(1, NULL, true, false)
,   m_pProtocolGuard(Threading::MutexFactory::create())
,   m_pApplicationGuard(Threading::MutexFactory::create())
,   m_pStartCondition(Threading::ConditionFactory::create())
,   m_pMessageRegistry_type(new NumericTypeMessageRegistry(), &destroy)
,   m_databaseConnectionsMap()
,   m_pDatabaseConnectionsMapMutex(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::~ApplicationServer()
{
    Threading::MutexFactory::destroy(m_pDatabaseConnectionsMapMutex);
    Threading::MutexFactory::destroy(m_pProtocolGuard);
    Threading::MutexFactory::destroy(m_pApplicationGuard);
    Threading::ConditionFactory::destroy(m_pStartCondition);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
ApplicationServer::start()
{
    class InstallationCompleteTask
    :   public Threading::ThreadPool::Task
    {
    private:
        class PrepareToStartCompleteTask
        :   public Threading::ThreadPool::Task
        {
        private:
            class StartCompleteTask
            :   public Threading::ThreadPool::Task
            {
            public:
                virtual void call()
                {
                    m_condition.assertCondition();
                }

                StartCompleteTask(Zen::Threading::I_Condition& _condition)
                :   m_condition(_condition)
                {
                }
            private:
                Zen::Threading::I_Condition&    m_condition;
            };

        public:
            virtual void call()
            {
                StartCompleteTask* pTask = new StartCompleteTask(m_condition);
                m_installQueue.pushRequest(pTask);
            }

            PrepareToStartCompleteTask(Zen::Threading::I_Condition& _condition, Zen::Threading::ThreadPool& _installQueue)
            :   m_condition(_condition)
            ,   m_installQueue(_installQueue)
            {
            }
        private:
            Zen::Threading::I_Condition&    m_condition;
            Zen::Threading::ThreadPool&     m_installQueue;
        };
    public:
        virtual void call()
        {
            PrepareToStartCompleteTask* pTask = new PrepareToStartCompleteTask(m_condition, m_installQueue);
            m_installQueue.pushRequest(pTask);
        }

        InstallationCompleteTask(Zen::Threading::I_Condition& _condition, Zen::Threading::ThreadPool& _installQueue)
        :   m_condition(_condition)
        ,   m_installQueue(_installQueue)
        {
        }
    private:
        Zen::Threading::I_Condition&    m_condition;
        Zen::Threading::ThreadPool&     m_installQueue;
    };

    // Create the installation complete task and pass the condition into it.
    InstallationCompleteTask* pTask = new InstallationCompleteTask(*m_pStartCondition, m_installQueue);

    // Push the task onto the end of the queue
    m_installQueue.pushRequest(pTask);

    // Start the installation queue so that protocols and services will be
    // installed.
    m_installQueue.start();

    return m_pStartCondition;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::stop()
{
    class InstallationQueueEmptyTask
    :   public Threading::ThreadPool::Task
    {
    public:
        virtual void call()
        {
            m_condition.assertCondition();
        }

        InstallationQueueEmptyTask(Zen::Threading::I_Condition& _condition)
        :   m_condition(_condition)
        {
        }
    private:
        Zen::Threading::I_Condition&    m_condition;
    };

    class ShutdownCompleteTask
    :   public Threading::ThreadPool::Task
    {
    public:
        virtual void call()
        {
            m_condition.assertCondition();
        }

        ShutdownCompleteTask(Zen::Threading::I_Condition& _condition)
        :   m_condition(_condition)
        {
        }
    private:
        Zen::Threading::I_Condition&    m_condition;
    };

    // Create an I_Condition to know when the installQueue is empty...
    Zen::Threading::I_Condition* pInstallerCondition = Zen::Threading::ConditionFactory::create(false);

    // Create the installation complete task and pass the condition into it.
    InstallationQueueEmptyTask* pInstallerTask = new InstallationQueueEmptyTask(*pInstallerCondition);

    // Push the task onto the end of the queue
    m_installQueue.pushRequest(pInstallerTask);

    // Prepare to stop the installation queue so that no more protocols or services will
    // be installed (at least not until the app server is restarted, if ever).
    m_installQueue.prepareToStop();

    // Wait for the install queue to be emptied
    pInstallerCondition->requireCondition();

    // Destroy the condition
    Zen::Threading::ConditionFactory::destroy(pInstallerCondition);

    // Stop the install queue
    m_installQueue.stop();

    // Create an I_Condition to know when the shutdownQueue is finished.
    Zen::Threading::I_Condition* pShutdownCondition = Zen::Threading::ConditionFactory::create(false);

    // Create the shutdown complete task and pass the condition into it.
    ShutdownCompleteTask* pShutdownTask = new ShutdownCompleteTask(*pShutdownCondition);

    // Push the task onto the end of the queue
    m_shutdownQueue.pushRequest(pShutdownTask);

    // Start the shutdown queue
    m_shutdownQueue.start();

    // Wait on the shutdown queue to completely start
    m_shutdownQueue.requireStarted();

    // Prepare to stop the shutdown queue so that no more tasks will be
    // pushed onto it.
    m_shutdownQueue.prepareToStop();

    // Wait for the shutdown queue to be emptied.
    pShutdownCondition->requireCondition();

    // Destroy the condition
    Zen::Threading::ConditionFactory::destroy(pShutdownCondition);

    // Stop the shutdown queue
    m_shutdownQueue.stop();

    // Empty service collections
    {
        Threading::CriticalSection lock(m_pProtocolGuard);
        m_protocolServices.empty();
    }

    {
        Threading::CriticalSection lock(m_pApplicationGuard);
        m_applicationServices.empty();
    }

    m_pEventService.reset();
    m_pScriptEngine.reset();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::registerDefaultScriptEngine(pScriptEngine_type _pEngine)
{
    // TODO Register with all of the already-installed app services.

    m_pScriptEngine = _pEngine;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::pScriptEngine_type
ApplicationServer::getDefaultScriptEngine()
{
    return m_pScriptEngine;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::pEventService_type
ApplicationServer::getEventService()
{
    return m_pEventService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::installProtocols(pConfig_type _pProtocolsConfig)
{
    class ConfigVisitor
    : public Zen::Plugins::I_ConfigurationElement::I_ConfigurationElementVisitor
    {
    public:
        virtual void begin()
        {
        }

        virtual void visit(const Zen::Plugins::I_ConfigurationElement& _element)
        {
            Zen::Enterprise::I_ApplicationServerManager& manager =
                Zen::Enterprise::I_ApplicationServerManager::getSingleton();

            Zen::Enterprise::I_ApplicationServerManager::pProtocolService_type
                pProtocolService = manager.createProtocolService(*m_pAppServer, _element.getAttribute("type"));

            if (pProtocolService.isValid())
            {
                pProtocolService->setConfiguration(_element);
                m_pAppServer->installProtocol(pProtocolService, _element.getAttribute("name"));
            }
        }

        virtual void end()
        {
        }

        ConfigVisitor(I_ApplicationServer* _pAppServer)
        :   m_pAppServer(_pAppServer)
        {
        }
    private:
        I_ApplicationServer*        m_pAppServer;
    };

    ConfigVisitor visitor(this);
    _pProtocolsConfig->getChildren("protocol", visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::installProtocol(pProtocolService_type _pProtocolService, const std::string& _protocolName)
{
    class InstallProtocolServiceTask
    :   public Threading::ThreadPool::Task
    {
    private:
        class PrepareToStartProtocolServiceTask
        :   public Threading::ThreadPool::Task
        {
        private:
            class StartProtocolServiceTask
            :   public Threading::ThreadPool::Task
            {
            public:
                virtual void call()
                {
                    m_pProtocolService->start();
                }

                StartProtocolServiceTask(pProtocolService_type _pProtocolService)
                :   m_pProtocolService(_pProtocolService)
                {
                }
            private:
                pProtocolService_type   m_pProtocolService;
            };
            class StopProtocolServiceTask
            :   public Threading::ThreadPool::Task
            {
            public:
                virtual void call()
                {
                    m_pProtocolService->stop();
                }

                StopProtocolServiceTask(pProtocolService_type _pProtocolService)
                :   m_pProtocolService(_pProtocolService)
                {
                }
            private:
                pProtocolService_type   m_pProtocolService;
            };
        public:
            virtual void call()
            {
                Threading::I_Condition* pCondition = m_pProtocolService->prepareToStart(m_server.getSharedThreadPool());
                if( pCondition != NULL )
                {
                    pCondition->requireCondition();
                }

                StartProtocolServiceTask* pStartTask = new StartProtocolServiceTask(m_pProtocolService);
                StopProtocolServiceTask* pStopTask = new StopProtocolServiceTask(m_pProtocolService);
                m_installQueue.pushRequest(pStartTask);
                m_shutdownQueue.pushRequest(pStopTask);
            }

            PrepareToStartProtocolServiceTask(ApplicationServer& _server, pProtocolService_type _pProtocolService, Threading::ThreadPool& _installQueue, Threading::ThreadPool& _shutdownQueue)
            :   m_server(_server)
            ,   m_pProtocolService(_pProtocolService)
            ,   m_installQueue(_installQueue)
            ,   m_shutdownQueue(_shutdownQueue)
            {
            }
        private:
            ApplicationServer&          m_server;
            pProtocolService_type       m_pProtocolService;
            Threading::ThreadPool&      m_installQueue;
            Threading::ThreadPool&      m_shutdownQueue;
        };
        class PrepareToStopProtocolServiceTask
        :   public Threading::ThreadPool::Task
        {
        public:
            virtual void call()
            {
                Threading::I_Condition* pCondition = m_pProtocolService->prepareToStop();
                if( pCondition != NULL )
                {
                    pCondition->requireCondition();
                }
            }

            PrepareToStopProtocolServiceTask(pProtocolService_type _pProtocolService)
            :   m_pProtocolService(_pProtocolService)
            {
            }
        private:
            pProtocolService_type       m_pProtocolService;
        };
    public:
        virtual void call()
        {
            m_server.handleInstallProtocol(m_pProtocolService, m_protocolName);
            PrepareToStartProtocolServiceTask* pStartTask = new PrepareToStartProtocolServiceTask(m_server,m_pProtocolService, m_installQueue, m_shutdownQueue);
            PrepareToStopProtocolServiceTask* pStopTask = new PrepareToStopProtocolServiceTask(m_pProtocolService);
            m_installQueue.pushRequest(pStartTask);
            m_shutdownQueue.pushRequest(pStopTask);
        }

        InstallProtocolServiceTask(ApplicationServer& _server, pProtocolService_type _pProtocolService, const std::string& _protocolName, Threading::ThreadPool& _installQueue, Threading::ThreadPool& _shutdownQueue)
        :   m_server(_server)
        ,   m_pProtocolService(_pProtocolService)
        ,   m_protocolName(_protocolName)
        ,   m_installQueue(_installQueue)
        ,   m_shutdownQueue(_shutdownQueue)
        {
        }
    private:
        ApplicationServer&      m_server;
        pProtocolService_type   m_pProtocolService;
        std::string             m_protocolName;
        Threading::ThreadPool&  m_installQueue;
        Threading::ThreadPool&  m_shutdownQueue;
    };

    // Schedule the installation of the protocol.
    InstallProtocolServiceTask* pTask = new InstallProtocolServiceTask(*this, _pProtocolService, _protocolName,m_installQueue, m_shutdownQueue);
    m_installQueue.pushRequest(pTask);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::pProtocolService_type
ApplicationServer::getProtocol(const std::string& _protocolName)
{
    Threading::CriticalSection lock(m_pProtocolGuard);

    ProtocolServices_type::iterator iter = m_protocolServices.find(_protocolName);

    if (iter != m_protocolServices.end())
    {
        return iter->second;
    }
    else
    {
        // TODO Throw an exception?
        return pProtocolService_type();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::installApplications(pConfig_type _pAppServicesConfig)
{
    class ConfigVisitor
    :   public Zen::Plugins::I_ConfigurationElement::I_ConfigurationElementVisitor
    {

    public:
        virtual void begin() {}

        virtual void visit(const Zen::Plugins::I_ConfigurationElement& _element)
        {
            Zen::Enterprise::I_ApplicationServerManager& manager =
                Zen::Enterprise::I_ApplicationServerManager::getSingleton();

            Zen::Enterprise::I_ApplicationServerManager::pApplicationService_type
                pApplicationService = manager.createApplicationService(*m_pAppServer, _element.getAttribute("type"));

            if( pApplicationService.isValid() )
            {
                pApplicationService->setConfiguration(_element);
                m_pAppServer->installApplication(pApplicationService, manager.createLocation(_element.getAttribute("location")));
            }
        }

        virtual void end() {}

        ConfigVisitor(ApplicationServer* _pAppServer)
        :   m_pAppServer(_pAppServer)
        {
        }

    private:
        ApplicationServer*        m_pAppServer;
    };

    ConfigVisitor visitor(this);
	_pAppServicesConfig->getChildren("application", visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::installApplication(pApplicationService_type _pApplicationService, pResourceLocation_type _pRootLocation)
{
    class InstallApplicationServiceTask
    :   public Threading::ThreadPool::Task
    {
        class PrepareToStartApplicationServiceTask
        :   public Threading::ThreadPool::Task
        {
        private:
            class StartApplicationServiceTask
            :   public Threading::ThreadPool::Task
            {
            public:
                virtual void call()
                {
                    m_pApplicationService->start();
                }

                StartApplicationServiceTask(pApplicationService_type _pApplicationService)
                :   m_pApplicationService(_pApplicationService)
                {
                }
            private:
                pApplicationService_type   m_pApplicationService;
            };
            class StopApplicationServiceTask
            :   public Threading::ThreadPool::Task
            {
            public:
                virtual void call()
                {
                    m_pApplicationService->stop();
                }

                StopApplicationServiceTask(pApplicationService_type _pApplicationService)
                :   m_pApplicationService(_pApplicationService)
                {
                }
            private:
                pApplicationService_type    m_pApplicationService;
            };
        public:
            virtual void call()
            {
                Threading::I_Condition* pCondition = m_pApplicationService->prepareToStart(m_server.getSharedThreadPool());
                if( pCondition != NULL )
                {
                    pCondition->requireCondition();
                }

                StartApplicationServiceTask* pStartTask = new StartApplicationServiceTask(m_pApplicationService);
                StopApplicationServiceTask* pStopTask = new StopApplicationServiceTask(m_pApplicationService);
                m_installQueue.pushRequest(pStartTask);
                m_shutdownQueue.pushRequest(pStopTask);
            }

            PrepareToStartApplicationServiceTask(ApplicationServer& _server, pApplicationService_type _pApplicationService, Threading::ThreadPool& _installQueue, Threading::ThreadPool& _shutdownQueue)
            :   m_server(_server)
            ,   m_pApplicationService(_pApplicationService)
            ,   m_installQueue(_installQueue)
            ,   m_shutdownQueue(_shutdownQueue)
            {
            }
        private:
            ApplicationServer&          m_server;
            pApplicationService_type    m_pApplicationService;
            Threading::ThreadPool&      m_installQueue;
            Threading::ThreadPool&      m_shutdownQueue;
        };

        class PrepareToStopApplicationServiceTask
        :   public Threading::ThreadPool::Task
        {
        public:
            virtual void call()
            {
                Threading::I_Condition* pCondition = m_pApplicationService->prepareToStop();
                if( pCondition != NULL )
                {
                    pCondition->requireCondition();
                }
            }

            PrepareToStopApplicationServiceTask(pApplicationService_type _pApplicationService)
            :   m_pApplicationService(_pApplicationService)
            {
            }
        private:
            pApplicationService_type    m_pApplicationService;
        };
    public:
        virtual void call()
        {
            // TODO What about the resource location?
            m_server.handleInstallApplication(m_pApplicationService, m_pRootLocation);
            PrepareToStartApplicationServiceTask* pStartTask = new PrepareToStartApplicationServiceTask(m_server, m_pApplicationService, m_installQueue, m_shutdownQueue);
            PrepareToStopApplicationServiceTask* pStopTask = new PrepareToStopApplicationServiceTask(m_pApplicationService);
            m_installQueue.pushRequest(pStartTask);
            m_shutdownQueue.pushRequest(pStopTask);
        }

        InstallApplicationServiceTask(ApplicationServer& _server, pApplicationService_type _pApplicationService, pResourceLocation_type _pRootLocation, Threading::ThreadPool& _installQueue, Threading::ThreadPool& _shutdownQueue)
        :   m_server(_server)
        ,   m_pApplicationService(_pApplicationService)
        ,   m_pRootLocation(_pRootLocation)
        ,   m_installQueue(_installQueue)
        ,   m_shutdownQueue(_shutdownQueue)
        {
        }
    private:
        ApplicationServer&          m_server;
        pApplicationService_type    m_pApplicationService;
        pResourceLocation_type      m_pRootLocation;
        Threading::ThreadPool&      m_installQueue;
        Threading::ThreadPool&      m_shutdownQueue;
    };

    // Schedule the installation of the application.
    InstallApplicationServiceTask* pTask = new InstallApplicationServiceTask(*this, _pApplicationService, _pRootLocation, m_installQueue, m_shutdownQueue);
    m_installQueue.pushRequest(pTask);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::configureApplication(pApplicationService_type _pApplicationService, pConfig_type _pConfig)
{
    class ConfigureApplicationServiceTask
    :   public Threading::ThreadPool::Task
    {
    public:
        virtual void call()
        {
            m_server.handleConfigureApplication(m_pApplicationService, m_pConfig);
        }

        ConfigureApplicationServiceTask(ApplicationServer& _server, pApplicationService_type _pApplicationService, pConfig_type _pConfig)
        :   m_server(_server)
        ,   m_pApplicationService(_pApplicationService)
        ,   m_pConfig(_pConfig)
        {
        }

    private:
        ApplicationServer&          m_server;
        pApplicationService_type    m_pApplicationService;
        pConfig_type                m_pConfig;
    };

    // Schedule the configuration of the application.
    ConfigureApplicationServiceTask* pTask = new ConfigureApplicationServiceTask(*this, _pApplicationService, _pConfig);
    m_installQueue.pushRequest(pTask);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::pApplicationService_type
ApplicationServer::getApplication(pResourceLocation_type _pServiceLocation) const
{
    m_pStartCondition->requireCondition();

    Threading::CriticalSection lock(m_pApplicationGuard);

    ApplicationServices_type::const_iterator iter = m_applicationServices.find(_pServiceLocation);

    if( iter != m_applicationServices.end() )
    {
        return iter->second;
    }
    else
    {
        // TODO Throw an exception?
        return pApplicationService_type();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::pApplicationService_type
ApplicationServer::getApplication(const std::string& _serviceLocation) const
{
    return getApplication(
        Zen::Enterprise::I_ApplicationServerManager::getSingleton()
            .createLocation(_serviceLocation)
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::getResourceLocations(I_ResourceLocationVisitor& _visitor) const
{
    _visitor.begin();

    ApplicationServices_type::const_iterator iter = m_applicationServices.begin();
    while( iter != m_applicationServices.end() )
    {
        _visitor.visit(iter->first, iter->second);
        iter++;
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::pMessageRegistry_type
ApplicationServer::getMessageRegistry()
{
    return m_pMessageRegistry_type;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::handleMessage(pMessage_type _pMessage)
{
    // Local and remote messages are handled here.

    ResourceLocation* pDestination = dynamic_cast<ResourceLocation*>(_pMessage->getDestinationLocation().get());

    if (pDestination != NULL)
    {
        if (isLocalDestination(_pMessage->getDestinationEndpoint()))
        {
            // This is a message received by a protocol adapter or was sent
            // from a local application service to another local application service.

            class HandleMessageTask
            :   public Threading::ThreadPool::Task
            {
            public:
                virtual void call()
                {
                    if( m_pApplicationService.isValid() )
                    {
                        m_pApplicationService->handleMessage(m_pMessage);
                    }
                }

                HandleMessageTask(pApplicationService_type _pApplicationService, pMessage_type _pMessage)
                :   m_pApplicationService(_pApplicationService)
                ,   m_pMessage(_pMessage)
                {
                }

                virtual ~HandleMessageTask() {}
            private:
                pApplicationService_type    m_pApplicationService;
                pMessage_type               m_pMessage;
            };

            // TODO Use a TaskPool
            HandleMessageTask* pTask = new HandleMessageTask(pDestination->getApplicationService(), _pMessage);

            m_sharedThreadPool.pushRequest(pTask);
        }
        else
        {
            // This is an outbound message.
            // This is an outbound request.  Send it.
            // TODO Does this end up being asynchronous?  If not then we
            // probably should handle it in a worker thread / outbound queue.
            _pMessage->getDestinationEndpoint()->getProtocolAdapter().lock()->sendTo
            (
                _pMessage,
                _pMessage->getDestinationEndpoint()
            );
        }
    }
    else
    {
        throw Utility::runtime_exception("ApplicationServer::handleMessage(): Error, invalid destination.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::handleRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    ResourceLocation* pDestination = dynamic_cast<ResourceLocation*>(_pRequest->getDestinationLocation().get());

    if (pDestination != NULL)
    {
        // TODO: If _pRequest destination is not this server then the request is destined for
        // another destination and it should be sent instead of dispatched.

        if (isLocalDestination(_pRequest->getDestinationEndpoint()))
        {
            // All requests that are destined for a local service go through
            // this logic. All requests coming from an external source are
            // processed by ApplicationServer::handleMessage().

            class HandleRequestTask
            :   public Threading::ThreadPool::Task
            {
            public:
                virtual void call()
                {
                    if( m_pApplicationService.isValid() )
                    {
                        m_pApplicationService->handleRequest(m_pRequest, m_pResponseHandler);
                    }
                }

                HandleRequestTask(pApplicationService_type _pApplicationService, pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
                :   m_pApplicationService(_pApplicationService)
                ,   m_pRequest(_pRequest)
                ,   m_pResponseHandler(_pResponseHandler)
                {
                }

                virtual ~HandleRequestTask() {}
            private:
                pApplicationService_type    m_pApplicationService;
                pRequest_type               m_pRequest;
                pResponseHandler_type       m_pResponseHandler;
            };

            // TODO Use a TaskPool
            HandleRequestTask* pTask = new HandleRequestTask(pDestination->getApplicationService(), _pRequest, _pResponseHandler);

            m_sharedThreadPool.pushRequest(pTask);
        }
        else
        {
            // This is an outbound request.  Send it.
            // TODO Does this end up being asynchronous?  If not then we
            // probably should handle it in a worker thread / outbound queue.
            _pRequest->getDestinationEndpoint()->getProtocolAdapter().lock()->sendTo
            (
                _pRequest,
                _pRequest->getDestinationEndpoint()
            );
        }
    }
    else
    {
        throw Utility::runtime_exception("ApplicationServer::handleRequest(): Error, invalid destination.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::handleSessionEvent(pSessionEvent_type _pSessionEvent)
{
    throw Utility::runtime_exception("ApplicationServer::handleSessionEvent(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::pDatabaseConnection_type
ApplicationServer::getDatabaseConnection(const std::string& _database)
{
    /// Do we need to guard this?
    Zen::Threading::I_Thread::ThreadId _threadId = Threading::I_Thread::getCurrentThreadId();

    {
        Threading::CriticalSection guard(m_pDatabaseConnectionsMapMutex);

        DatabaseConnectionsMap_type::iterator iter = m_databaseConnectionsMap.find(_database);
        if( iter != m_databaseConnectionsMap.end() )
        {
            return iter->second->getConnection(_threadId);
        }
        else
        {
            return pDatabaseConnection_type();
            // TODO Error?
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::handleInstallProtocol(pProtocolService_type _pProtocolService, const std::string& _protocolName)
{
    Threading::CriticalSection lock(m_pProtocolGuard);

    m_protocolServices[_protocolName] = _pProtocolService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::handleInstallApplication(pApplicationService_type _pApplicationService, pResourceLocation_type _pRootLocation)
{
    Threading::CriticalSection lock(m_pApplicationGuard);

    m_applicationServices[_pRootLocation] = _pApplicationService;

    // TODO Get the root resource location and set the application service
    ResourceLocation* pRoot = dynamic_cast<ResourceLocation*>(_pRootLocation.get());
    pRoot->setApplicationService(_pApplicationService);

    if (m_pScriptEngine.isValid())
    {
        Scripting::I_ScriptableService* const pScriptable = dynamic_cast<Scripting::I_ScriptableService*>(_pApplicationService.get());

        if (pScriptable)
        {
            pScriptable->registerScriptEngine(m_pScriptEngine);
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::handleConfigureApplication(pApplicationService_type _pApplicationService, pConfig_type _pConfig)
{
    _pApplicationService->setConfiguration(*_pConfig);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::DatabaseConnections::DatabaseConnections(pDatabaseService_type _pService,
                                                            config_type _connectionConfig)
:   m_pDatabaseService(_pService)
,   m_connectionConfig(_connectionConfig)
,   m_databaseConnections()
,   m_databaseConnectionsMutex(Zen::Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::DatabaseConnections::~DatabaseConnections()
{
    Zen::Threading::MutexFactory::destroy(m_databaseConnectionsMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::DatabaseConnections::pDatabaseConnection_type
ApplicationServer::DatabaseConnections::getConnection(Zen::Threading::I_Thread::ThreadId& _threadId)
{
    Zen::Threading::CriticalSection guard(m_databaseConnectionsMutex);

    DatabaseConnections_type::iterator iter = m_databaseConnections.find(_threadId);
    if( iter != m_databaseConnections.end() )
    {
        return iter->second;
    }
    else
    {
        pDatabaseConnection_type pDatabaseConnection = m_pDatabaseService->connect(_threadId.toString(), m_connectionConfig);
        m_databaseConnections[_threadId] = pDatabaseConnection;
        return pDatabaseConnection;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::installDatabaseConnections(pConfig_type _pDatabasesConfig)
{
    class DatabaseVisitor
    :   public Zen::Plugins::I_ConfigurationElement::I_ConfigurationElementVisitor
    {
        typedef std::map<std::string, std::string>      config_type;

        class DBConfigVisitor
        :   public Zen::Plugins::I_ConfigurationElement::I_ConfigurationElementVisitor
        {
        public:
            virtual void begin() {}

            virtual void visit(const Zen::Plugins::I_ConfigurationElement& _element)
            {
                // TODO Instead of getAttributeValue("value") there should
                // be some way to get the text inside of an element.
                m_config[_element.getName()] = _element.getAttribute("value");
            }

            virtual void end() {}

            DBConfigVisitor(config_type& _config)
            :   m_config(_config)
            {
            }

        private:
            config_type&    m_config;

        };  // class DBConfigVisitor

    public:
        virtual void begin() {}

        virtual void visit(const Zen::Plugins::I_ConfigurationElement& _element)
        {
            config_type config;

            DBConfigVisitor visitor(config);
            _element.getChildren(visitor);

            m_pAppServer->createDatabaseEntry(_element.getAttribute("name"),
                _element.getAttribute("type"), config);
        }

        virtual void end() {}

        DatabaseVisitor(ApplicationServer* _pAppServer)
        :   m_pAppServer(_pAppServer)
        {
        }

    private:
        ApplicationServer*        m_pAppServer;

    };  // class DatabaseVisitor

    DatabaseVisitor visitor(this);
    _pDatabasesConfig->getChildren("database", visitor);

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::createDatabaseEntry(const std::string& _connectionName, const std::string& _databaseType, config_type& _config)
{
    Threading::CriticalSection guard(m_pDatabaseConnectionsMapMutex);

    DatabaseConnectionsMap_type::iterator iter = m_databaseConnectionsMap.find(_connectionName);
    if( iter == m_databaseConnectionsMap.end() )
    {
        typedef std::shared_ptr<Zen::Database::I_DatabaseService>  pDatabaseService_type;
        pDatabaseService_type pDatabaseService =
            Zen::Database::I_DatabaseManager::getSingleton().createDatabaseService(
                _databaseType,
                _config
            );

        DatabaseConnections* pRaw = new DatabaseConnections(pDatabaseService, _config);
        pDatabaseConnections_type pDatabaseConnections(pRaw);
        m_databaseConnectionsMap[_connectionName] = pDatabaseConnections;
    }
    else
    {
        // TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ApplicationServer::isLocalDestination(pEndpoint_type _pDestination)
{
    // TODO Since all endpoints must be constructed via a protocol adapter, first
    // check the protocol adapter associated with the endpoint.
    // TODO Only return true if this is true, otherwise consult the
    // other protocol adapters.  We don't want to do this now because
    // we're testing a situation where we want to forcibly serialize and
    // send over the wire within a single application.  If we check other
    // protocol adapters then we'll find out that this is indeed a local
    // destination.
    //return _pDestination->getProtcolAdapter()->isLocalDestination();

    // An invalid pointer here means that it is definitely local.
    return (!_pDestination.isValid()) || _pDestination->isLocal();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

 */