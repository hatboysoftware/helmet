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

namespace boost {
    namespace archive {
        class polymorphic_iarchive;
    } // namespace archive
} // namespace boost

#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_MessageType;
class I_MessageHeader;

class HELMET_ENTERPRISE_DLL_LINK I_MessageRegistry
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef std::shared_ptr<I_MessageType>              pMessageType_type;
    typedef std::shared_ptr<I_MessageHeader>            pMessageHeader_type;
    typedef std::shared_ptr<I_MessageRegistry>          pMessageRegistry_type;
    /// @}

    /// @name I_MessageRegistry interface.
    /// @{
public:
    /// Register a message type.
    virtual void registerMessageType(pMessageType_type _pMessageType) = 0;

    /// Unregister a message type.
    virtual void unregisterMessageType(pMessageType_type _pMessageType) = 0;

    /// Get the message header.
    /// The implementation should use the archive to deserialize the application level
    /// message header.
    virtual pMessageHeader_type getMessageHeader(boost::archive::polymorphic_iarchive& _archive) const = 0;

    /// Create a message header for the given message type.
    virtual pMessageHeader_type createMessageHeader(pMessageType_type _pMessageType, boost::uint64_t _messageId, boost::uint64_t _requestId, const std::string& _srcLocation, const std::string& _destLocation) const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_MessageRegistry();
    virtual ~I_MessageRegistry();
    /// @}

};  // interface I_MessageRegistry

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
