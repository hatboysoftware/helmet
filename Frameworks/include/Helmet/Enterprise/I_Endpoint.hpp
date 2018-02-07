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
class I_ProtocolService;
class I_Address;

/// Basic Network Endpoint
class HELMET_ENTERPRISE_DLL_LINK I_Endpoint
{
    /// @name Types
    /// @{
public:
    typedef std::weak_ptr<I_ProtocolService>  wpProtocolService_type;
    typedef std::shared_ptr<I_Address>  pAddress_type;
    /// @}

    /// @name I_Endpoint interface
    /// @{
public:
    /// Get the protocol adapter that is associated with this endpoint.
    virtual wpProtocolService_type getProtocolAdapter() const = 0;

    /// Get the string representation of this endpoint.
    virtual const std::string& toString() const = 0;

    /// Get the network address that is associated with this endpoint.
    virtual const I_Address& getAddress() const = 0;

    /// @return true if this endpoint is within the same process
    ///     as the calling method.
    virtual bool isLocal() const = 0;

    /// Operators
    virtual bool operator==(const I_Endpoint& _otherEndpoint) const = 0;
    virtual bool operator!=(const I_Endpoint& _otherEndpoint) const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Endpoint();
    virtual ~I_Endpoint();
    /// @}

};  // interface I_Endpoint

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
