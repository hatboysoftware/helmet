//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Enterprise/I_ApplicationServer.hpp>

#include <Helmet/Core/Thread/ThreadPool.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class ApplicationServer
:   public I_ApplicationServer
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_ApplicationServer implementation
    /// @{
public:
    Core::Thread::I_Condition* start() override;
    void stop() override;
    void installApplication(pApplicationService_type _pApplicationService,
                            pResourceLocation_type _pRootLocation) override;
    void installProtocol(pProtocolService_type _pProtocolService) override;
    pApplicationService_type getApplication(pResourceLocation_type _pServiceLocation) const override;
    pProtocolService_type getProtocol(const std::string& _protocolName) override;
    void handleMessage(pMessage_type _pMessage) override;
    void handleRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler) override;
    /// @}

    /// @name ApplicationServer implementation
    /// @{
public:
    void handleInstallApplication(pApplicationService_type _pApplicationService,
                                  pResourceLocation_type _pRootLocation);
    void handleInstallProtocol(pProtocolService_type _pProtocolService);
    Core::Thread::ThreadPool& getSharedThreadPool() { return m_sharedThreadPool; }
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             ApplicationServer();
    virtual ~ApplicationServer();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// ThreadPool that's shared among all of the services and protocols.
    Core::Thread::ThreadPool    m_sharedThreadPool;

    /// Queue of items that require installation
    /// plus the thread that is used to perform
    /// the installation.
    Core::Thread::ThreadPool    m_installQueue;

    /// Queue of the items that require shutdown
    /// plus the thread that is used to perform
    /// the shutdown.
    Core::Thread::ThreadPool    m_shutdownQueue;

    /// Guard for application services consistency
    Core::Thread::I_Mutex*      m_pApplicationGuard;

    /// Collection of installed application services
    typedef std::map<pResourceLocation_type, pApplicationService_type>  ApplicationServices_type;
    ApplicationServices_type    m_applicationServices;

    /// Guard for protocol services consistency
    Core::Thread::I_Mutex*      m_pProtocolGuard;

    /// Collection of installed protocol services
    typedef std::map<std::string, pProtocolService_type>    ProtocolServices_type;
    ProtocolServices_type       m_protocolServices;

    Core::Thread::I_Condition*  m_pStartCondition;
    /// @}

};  // class ApplicationServer

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
#ifndef ZEN_ENTERPRISE_APPLICATION_SERVER_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPLICATION_SERVER_HPP_INCLUDED

#include <Zen/Core/Threading/I_Thread.hpp>
#include <Zen/Core/Threading/ThreadPool.hpp>
#include <Zen/Core/Event/I_EventService.hpp>
#include <Zen/Core/Plugins/I_Configuration.hpp>

#include <map>
#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Threading {
        class I_Condition;
    }   // namespace Threading
    namespace Database {
        class I_DatabaseService;
        class I_DatabaseConnection;
    }   // namespace Database
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class ApplicationServer
:   public I_ApplicationServer
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, pProtocolService_type>                ProtocolServices_type;
    typedef std::map<pResourceLocation_type, pApplicationService_type>  ApplicationServices_type;
    /// @}

    /// @name Enums
    /// @{
public:
    enum ServerStates
    {
        INITIALIZED,
        STARTING,
        STARTED,
        STOPPING,
        STOPPED
    };
    /// @}

    /// @name I_ApplicationServer implementation
    /// @{
public:
    virtual Zen::Threading::I_Condition* start();
    virtual void stop();
    virtual void registerDefaultScriptEngine(pScriptEngine_type _pEngine);
    virtual pScriptEngine_type getDefaultScriptEngine();
    virtual pEventService_type getEventService();
    virtual void installProtocols(pConfig_type _pProtocolsConfig);
    virtual void installProtocol(pProtocolService_type _pProtocolService, const std::string& _protocolName);
    virtual pProtocolService_type getProtocol(const std::string& _protocolName);
    virtual void installApplications(pConfig_type _pAppServicesConfig);
    virtual void installApplication(pApplicationService_type _pApplicationService, pResourceLocation_type _pRootLocation);
    virtual void configureApplication(pApplicationService_type _pApplicationService, pConfig_type _pConfig);
    virtual pApplicationService_type getApplication(pResourceLocation_type _pServiceLocation) const;
    virtual pApplicationService_type getApplication(const std::string& _serviceLocation) const;
    virtual void getResourceLocations(I_ResourceLocationVisitor& _visitor) const;
    virtual pMessageRegistry_type getMessageRegistry();
    virtual void handleMessage(pMessage_type _pMessage);
    virtual void handleRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);
    virtual void handleSessionEvent(pSessionEvent_type _pSessionEvent);
    virtual void installDatabaseConnections(pConfig_type _pDatabasesConfig);
    void createDatabaseEntry(const std::string& _connectionName, const std::string& _databaseType, config_type& _config);
    virtual pDatabaseConnection_type getDatabaseConnection(const std::string& _connectionName);
    /// @}

    /// @name ApplicationServer implementation
    /// @{
public:
    /// Install a protocol service.
    ///
    /// This is called by an InstallProtocolServiceTask that is queued into m_installQueue
    ///
    /// @note Internal use only
    void handleInstallProtocol(pProtocolService_type _pProtocolService, const std::string& _protocolName);

    /// Install an application service.
    ///
    /// This is called by an InstallApplicationServiceTask that is queued into m_installQueue
    ///
    /// @note Internal use only
    void handleInstallApplication(pApplicationService_type _pApplicationService, pResourceLocation_type _pRootLocation);

    void handleConfigureApplication(pApplicationService_type _pApplicationService, pConfig_type _pConfig);

    Threading::ThreadPool& getSharedThreadPool() { return m_sharedThreadPool; }

    /// @return true if _location is a local destination.
    bool isLocalDestination(pEndpoint_type _pDestination);
    /// @}

    /// @name Inner classes
    /// @{
private:
    class DatabaseConnections
    {
        /// @name Types
        /// @{
    public:
        typedef std::shared_ptr<Zen::Database::I_DatabaseService>              pDatabaseService_type;
        typedef std::map<std::string,std::string>                                       config_type;
        typedef std::shared_ptr<Zen::Database::I_DatabaseConnection>           pDatabaseConnection_type;
        typedef std::map<Zen::Threading::I_Thread::ThreadId,pDatabaseConnection_type>   DatabaseConnections_type;
        /// @}

        /// @name DatabaseConnections implementation
        /// @{
    public:
        pDatabaseConnection_type getConnection(Zen::Threading::I_Thread::ThreadId& _threadId);
        /// @}

        /// @name 'Structors
        /// @{
                 DatabaseConnections(pDatabaseService_type _pService, config_type _connectionConfig);
                ~DatabaseConnections();
        /// @}

        /// @name Member Variables
        /// @{
    private:
        pDatabaseService_type           m_pDatabaseService;
        config_type                     m_connectionConfig;
        DatabaseConnections_type        m_databaseConnections;
        Zen::Threading::I_Mutex*        m_databaseConnectionsMutex;
        /// @}

    };  // class DatabaseConnections
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             ApplicationServer();
    virtual ~ApplicationServer();
    /// @}

    /// @name Member Variables
    /// @{
private:
    ServerStates                m_currentState;

    /// Default script engine.
    pScriptEngine_type          m_pScriptEngine;

    /// Primary event service.
    pEventService_type          m_pEventService;

    /// ThreadPool that's shared among all of the services and protocols.
    Threading::ThreadPool       m_sharedThreadPool;

    /// Queue of items that require installation
    /// plus the thread that is used to perform
    /// the installation.
    Threading::ThreadPool       m_installQueue;

    /// Queue of items that require shutdown
    /// plus the thread that is used to perform
    /// the shutdown.
    Threading::ThreadPool       m_shutdownQueue;

    /// Guard for m_protocolServices consistency
    Threading::I_Mutex*         m_pProtocolGuard;

    /// Collection of installed protocol services
    ProtocolServices_type       m_protocolServices;

    /// Guard for the m_applicatinoServices consistency
    Threading::I_Mutex*         m_pApplicationGuard;

    /// Collection of installed application services
    ApplicationServices_type    m_applicationServices;

    Threading::I_Condition*     m_pStartCondition;

    pMessageRegistry_type       m_pMessageRegistry_type;

    typedef std::shared_ptr<DatabaseConnections>       pDatabaseConnections_type;
    typedef std::map<std::string, pDatabaseConnections_type>    DatabaseConnectionsMap_type;
    DatabaseConnectionsMap_type m_databaseConnectionsMap;
    Threading::I_Mutex*         m_pDatabaseConnectionsMapMutex;
    /// @}

};  // interface I_ApplicationServer

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPLICATION_SERVER_HPP_INCLUDED
 */