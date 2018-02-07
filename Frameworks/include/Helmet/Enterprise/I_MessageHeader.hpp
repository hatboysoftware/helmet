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

#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>

namespace boost {
    namespace archive {
        class polymorphic_oarchive;
    } // namespace archive
} // namespace boost

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_MessageType;

class HELMET_ENTERPRISE_DLL_LINK I_MessageHeader
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef std::shared_ptr<I_MessageType>          pMessageType_type;
    /// @}

    /// @name I_MessageHeader interface.
    /// @{
public:
    /// Get the type of this message.
    virtual pMessageType_type getMessageType() const = 0;

    /// Get the message id of this message.
    virtual boost::uint64_t getMessageId() const = 0;

    /// Get the associated request id of this message.
    /// This value is 0 if this message is a request a message, and
    /// non-zero if this message is a response.  This value
    /// is the message Id of the request that resulted in the
    /// response.
    virtual boost::uint64_t getRequestId() const = 0;

    /// Get the source resource location of this message.
    virtual const std::string& getSourceLocation() const = 0;

    /// Get the destination resource location of this message.
    virtual const std::string& getDestinationLocation() const = 0;

    /// Serialize the message header.
    /// Deserialization is done in I_MessageRegistry::getMessageHeader().
    virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_MessageHeader();
    virtual ~I_MessageHeader();
    /// @}

};  // interface I_MessageHeader

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
