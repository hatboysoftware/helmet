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

#include <Helmet/Enterprise/I_ResourceLocation.hpp>

#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>

namespace boost {
    namespace archive {
        class polymorphic_iarchive;
        class polymorphic_oarchive;
    } // namespace archive
} // namespace boost

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_MessageHeader;
class I_MessageType;
class I_Endpoint;

/// I_Message interface.
class HELMET_ENTERPRISE_DLL_LINK I_Message
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef std::shared_ptr<I_Endpoint>                 pEndpoint_type;
    typedef std::shared_ptr<I_ResourceLocation>         pResourceLocation_type;
    typedef std::shared_ptr<I_MessageHeader>            pMessageHeader_type;
    typedef std::shared_ptr<I_MessageType>              pMessageType_type;
    /// @}

    /// @name I_Message interface.
    /// @{
public:
    /// Get the source endpoint of this message
    /// @return The destination endpoint, or invalid() if this message originated from
    ///     the current process.
    virtual pEndpoint_type getSourceEndpoint() = 0;

    /// Get the destination endpiont of this message
    /// @return The destination endpoint, or invalid() if this message is destined for
    ///     the current process.
    virtual pEndpoint_type getDestinationEndpoint() = 0;

    /// Get the source resource of this message.
    virtual pResourceLocation_type getSourceLocation() = 0;

    /// Get the destination resource of this message.
    virtual pResourceLocation_type getDestinationLocation() = 0;

    /// Get the message header.
    virtual pMessageHeader_type getMessageHeader() const = 0;

    /// Get the message Id of this message.
    virtual boost::uint64_t getMessageId() const = 0;

    /// This method returns the message type.
    virtual pMessageType_type getMessageType() const = 0;

    /// Deserialize this message from an input archive.
    /// The protocol adapter deserializes the header, creates the appropriate message
    /// using the message registry, and then deserializes the rest of the message using
    /// this method.
    virtual void serialize(pMessageHeader_type _pHeader, boost::archive::polymorphic_iarchive& _archive, const int _version) = 0;

    /// Serialize this message to an output archive.
    /// The protocol adapter serializes the header and then call this
    /// method to serialize the rest of the message.
    virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Message();
    virtual ~I_Message();
    /// @}

};  // interface I_Message

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
