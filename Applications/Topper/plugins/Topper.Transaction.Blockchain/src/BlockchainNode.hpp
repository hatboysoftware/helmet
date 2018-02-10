//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Blockchain/I_BlockchainNode.hpp>

#include <boost/enable_shared_from_this.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class BlockchainNode
:   public Helmet::Blockchain::I_BlockchainNode
,	public boost::enable_shared_from_this<BlockchainNode>
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

    /// @name I_RequestHandler implementation
    /// @{
public:
    void handleRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler) override;
    /// @}

    /// @name I_ApplicationService interface
    /// @{
public:
    Helmet::Enterprise::I_ApplicationServer& getApplicationServer() override;
    const std::string& getServiceName() const override;
    pEndpoint_type getServiceEndpoint() override;
    pResourceLocation_type getServiceLocation() override;
    void handleMessage(pMessage_type _pMessage) override;
    /// @}

    /// @name I_BlockchainNode implementation
    /// @{
public:
    /// @}

    /// @name BlockchainNode implementation
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
    explicit BlockchainNode(Helmet::Enterprise::I_ApplicationServer& _applicationServer,
                            const std::string& _name);
    virtual ~BlockchainNode();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Helmet::Enterprise::I_ApplicationServer&    m_applicationServer;
    std::string                                 m_name;
    /// @}

};  // class BlockchainNode

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
