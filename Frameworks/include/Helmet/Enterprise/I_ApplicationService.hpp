//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Enterprise/I_StartupShutdownParticipant.hpp>

#include <memory>

#include <Helmet/Enterprise/I_RequestHandler.hpp>
#include <Helmet/Enterprise/I_Endpoint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ApplicationServer;
class I_Endpoint;
class I_Message;
class I_MessageType;
class I_ResourceLocation;

class HELMET_ENTERPRISE_DLL_LINK I_ApplicationService
:   public I_StartupShutdownParticipant
,   public I_RequestHandler
{
    /// @name Types
    /// @{
public:
    typedef std::shared_ptr<I_Message>          pMessage_type;
    typedef std::shared_ptr<I_MessageType>      pMessageType_type;
    typedef std::shared_ptr<I_Endpoint>         pEndpoint_type;
    typedef std::shared_ptr<I_ResourceLocation> pResourceLocation_type;
    /// @}

    /// @name I_ApplicationService interface.
    /// @{
public:
    /// Get the application server which is hosting this application service.
    /// This must be the same reference that was passed to the factory create
    /// method.
    /// @see I_ApplicationServiceFactory
    virtual I_ApplicationServer& getApplicationServer() = 0;

    /// Get the application service name.
    virtual const std::string& getServiceName() const = 0;

    /// Get the application service endpoint.
    virtual pEndpoint_type getServiceEndpoint() = 0;

    /// Get the application service resource location.
    virtual pResourceLocation_type getServiceLocation() = 0;

    /// Handle a message
    /// Messages are one-way notifications and are not expected to return a result.
    virtual void handleMessage(pMessage_type _pMessage) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ApplicationService();
    virtual ~I_ApplicationService();
    /// @}

};  // interface I_ApplicationService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
