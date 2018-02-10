//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Enterprise/I_ProtocolService.hpp>

#include <boost/enable_shared_from_this.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ProtocolService
:   public Helmet::Enterprise::I_ProtocolService
,	public boost::enable_shared_from_this<ProtocolService>
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_StartupShutdownParticipant implementation
    /// @{
public:
    Helmet::Core::Thread::I_Condition* prepareToStart(Helmet::Core::Thread::ThreadPool& _threadPool) override;
    void start() override;
    Helmet::Core::Thread::I_Condition* prepareToStop() override;
    void stop() override;
    /// @}

    /// @name I_ProtocolService implementation
    /// @{
public:
    const std::string& getName() const override;
    Enterprise::I_ApplicationServer& getApplicationServer() override;
    pEndpoint_type getEndpoint() override;
    pEndpoint_type resolveEndpoint(const std::string& _address, const std::string& _port) override;
    void sendTo(pMessage_type _pMessage, pEndpoint_type _pEndpoint) override;
    void disconnect(pEndpoint_type _pEndpoint) override;
//    Event::I_Event& getConnectedEvent() override;
//    Event::I_Event& getDisconnectedEvent() override;
    /// @}

    /// @name ProtocolService implementation
    /// @{
public:
    /// @}

    /// @name Static Methods
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit ProtocolService(Helmet::Enterprise::I_ApplicationServer& _applicationServer,
                            const std::string& _name);
    virtual ~ProtocolService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Helmet::Enterprise::I_ApplicationServer&    m_applicationServer;
    std::string                                 m_name;
    /// @}

};  // class ProtocolService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
