//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
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
namespace Strategy {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Strategy;

class HELMET_TRADING_DLL_LINK I_StrategyPlugin
:   public Core::Plugin::I_Plugin
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<Helmet::Core::Plugin::I_Environment>  pEnvironment_type;
    typedef boost::shared_ptr<I_Strategy>                           pStrategy_type;
    /// @}

    /// @name I_StrategyPlugin interface
    /// @{
public:
    virtual pStrategy_type getStrategy(pEnvironment_type _pEnvironment) const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_StrategyPlugin();
    virtual ~I_StrategyPlugin();
    /// @}

};  // interface I_StrategyPlugin

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Strategy
}   // namespace Trading
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
