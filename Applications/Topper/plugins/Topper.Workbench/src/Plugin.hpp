//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Workbench/I_WorkbenchPlugin.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
    namespace Core {
        namespace Plugin {
            class I_Module;
        }   //  namespace Plugin
    }   //  namespace Core
}   // namespace Helmet
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Plugin
:   public Helmet::Workbench::I_WorkbenchPlugin
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Solution implementation
    /// @{
public:
    virtual const std::string& getName() const override;
    virtual pWorkbench_type getWorkbench(pEnvironment_type _pEnvironment) const override;
    /// @}

    /// @name Solution implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             Plugin(Helmet::Core::Plugin::I_Module* _pModule);
    virtual ~Plugin();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Helmet::Core::Plugin::I_Module* m_pModule;
    /// @}

};  // class Solution

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
