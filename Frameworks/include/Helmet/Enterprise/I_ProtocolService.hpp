//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include "Configuration.hpp"

#include <memory>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Endpoint;
class I_ApplicationServer;
class I_Message;
class I_MessageRegistry;
class I_MessageHeader;
class I_Event; // TODO Where does this belong?

class HELMET_ENTERPRISE_DLL_LINK I_ProtocolService
{
    /// @name Types
    /// @{
public:
    typedef std::shared_ptr<I_Message>                  pMessage_type;
    typedef std::shared_ptr<I_MessageHeader>            pMessageHeader_type;
    typedef std::shared_ptr<I_Endpoint>                 pEndpoint_type;
    typedef std::shared_ptr<I_MessageRegistry>          pMessageRegistry_type;
    /// @}

    /// @name I_ProtocolService interface.
    /// @{
public:
    /// Get the application server which is hosting this protocol service.
    /// This must be the same reference that was passed to the factory create
    /// method.
    /// @see I_ProtocolServiceFactory
    virtual I_ApplicationServer& getApplicationServer() = 0;

    /// Get the endpoint for this protocol adapter.
    virtual pEndpoint_type getEndpoint() = 0;

    /// Resolve the endpoint for an address and port specified by strings.
    /// If the resolver and the operating system supports it, _port can also
    /// contain service names such as "http".
    virtual pEndpoint_type resolveEndpoint(const std::string& _address, const std::string& _port) = 0;

    /// Send a message to an endpoint, potentially establishing the connection first.
    virtual void sendTo(pMessage_type _pMessage, pEndpoint_type _pEndpoint) = 0;

    /// Disconnect from an endpoint.
    virtual void disconnect(pEndpoint_type _pEndpoint) = 0;

    /// Get the "Connected" event.
    /// This event is fired when a connection is established.
    /// The payload for the event is the pEndpoint_type of the
    /// destination endpoint that was disconnected.
    virtual Event::I_Event& getConnectedEvent() = 0;

    /// Get the "Disconnected" event.
    /// This event is fired when a connection is established.
    /// The payload for the event is the pEndpoint_type of the
    /// destination endpoint that was disconnected.
    virtual Event::I_Event& getDisconnectedEvent() = 0;
    /// @}

    /// @name Static methods
    /// @{
public:
    static const std::string& getNamespace();
    static const std::string& getExtensionPointName();
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ProtocolService();
    virtual ~I_ProtocolService();
    /// @}

};  // interface I_ProtocolService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
