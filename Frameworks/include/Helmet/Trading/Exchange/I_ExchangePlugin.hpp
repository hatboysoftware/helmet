//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Trading/Configuration.hpp>

#include <Helmet/Core/Plugin/I_Plugin.hpp>

#include <boost/smart_ptr/shared_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
    namespace Plugin {
        class I_Environment;
    }   // namespace Plugin
}   // namespace Core
namespace Trading {
namespace Exchange {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Exchange;

class HELMET_TRADING_DLL_LINK I_ExchangePlugin
:   public Core::Plugin::I_Plugin
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<Helmet::Core::Plugin::I_Environment>  pEnvironment_type;
    typedef boost::shared_ptr<I_Exchange>                           pExchange_type;
    /// @}

    /// @name I_ExchangePlugin interface
    /// @{
public:
    virtual pExchange_type getExchange(pEnvironment_type _pEnvironment) const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ExchangePlugin();
    virtual ~I_ExchangePlugin();
    /// @}

};  // interface I_ExchangePlugin

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Exchange
}   // namespace Trading
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
