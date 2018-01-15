//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Core/Plugin/I_ModuleManager.hpp>

#include <boost/shared_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Plugin {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Concrete implementation of I_ModuleManager
class ModuleManager
:   public I_ModuleManager
{
    /// @name Types
    /// @{
public:
    typedef std::list<boost::filesystem::path>    ModulePaths_type;
    /// @}

    /// @name I_ModuleManager implementation
    /// @{
public:
    virtual pService_type getService() override;
    virtual void addPath(const boost::filesystem::path& _modulePath) override;
    virtual void dropPath(const boost::filesystem::path& _modulePath) override;
    virtual bool findPath(const std::string _moduleName, boost::filesystem::path &_modulePath) override;
    /// @}

    /// @name ModuleManager implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             ModuleManager();
    virtual ~ModuleManager();
    /// @}

    /// @name Member variables
    /// @{
private:

    /// Module service
    pService_type       m_pModuleService;

    /// Module search paths
    ModulePaths_type    m_modulePaths;
    /// @}

};  // interface ModuleManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugin
}   // namespace Core
}	// namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
