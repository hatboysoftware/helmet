//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include "Configuration.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_NetworkAddressFactory;

/// Abstract Network Address Factory
class HELMET_ENTERPRISE_DLL_LINK I_AbstractNetworkAddressFactory
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name Factory methods
    /// @{
public:
    static I_AbstractNetworkAddressFactory& getSingleton();
    /// @}

    /// @name I_AbstractNetworkAddressFactory interface
    /// @{
public:
    /// Register a Network Address Factory
    virtual void registerFactory(I_NetworkAddressFactory& _networkAddressFactory) = 0;

    /// Unregister a Network Address Factory
    virtual void unregisterFactory(I_NetworkAddressFactory& _networkAddressFactory) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_AbstractNetworkAddressFactory() = default;
    virtual ~I_AbstractNetworkAddressFactory() = default;
    /// @}

};  // interface I_AbstractNetworkAddressFactory

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
