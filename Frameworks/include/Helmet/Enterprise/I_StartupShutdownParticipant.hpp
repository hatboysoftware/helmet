//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Enterprise/Configuration.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
    namespace Core {
        namespace Thread {
            class I_Condition;
            class ThreadPool;
        }   // namespace Thread
    }   // namespace Core
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class HELMET_ENTERPRISE_DLL_LINK I_StartupShutdownParticipant
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_StartupShutdownParticipant interface
    /// @{
public:
    /// Prepare the service for starting.  If the service cannot start
    /// immediately, the service should return a condition variable that
    /// will be asserted once the service is prepared to start.  If startup
    /// takes a long time (i.e. more than a millisecond) then the service
    /// should start a thread and return a condition variable.
    /// @param _threadPool Thread Pool that the service should use if it requires
    ///         a thread pool.  Threads in the pool can also be used for service
    ///         initialization, but MUST NOT be used for tasks that need to
    ///         execute for the duration of the application.
    /// @return I_Condition that is asserted once the service is prepared to
    ///             start.  If NULL is returned then the service is already prepared
    ///             to start immediately.
    virtual Core::Thread::I_Condition* prepareToStart(Core::Thread::ThreadPool& _threadPool) = 0;

    /// Start the service.  The service should not return from this method until
    /// the service is fully started.
    virtual void start() = 0;

    /// Prepare the service for stopping.  If a service can stop immediately then
    /// it should return NULL, otherwise it should return a condition variable that
    /// will be asserted once the service is prepared to stop.
    /// @return I_Condition that is asserted once the service is ready to stop.
    ///             If NULL is returned then the service is already prepared to stop.
    virtual Core::Thread::I_Condition* prepareToStop() = 0;

    /// Stop the service.  The service should not return from this method until
    /// the service is fully stopped.
    virtual void stop() = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_StartupShutdownParticipant();
    virtual ~I_StartupShutdownParticipant();
    /// @}

};  // interface I_StartupShutdownParticipant

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
