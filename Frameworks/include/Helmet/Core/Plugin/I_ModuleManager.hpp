//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include "../Configuration.hpp"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Plugin {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ModuleService;

class HELMET_CORE_DLL_LINK I_ModuleManager
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<I_ModuleService>  pService_type;
    /// @}

    /// @name I_ModuleManager interface
    /// @{
public:
    /// Load a module service
    virtual pService_type getService() = 0;

    /// Add a module search path
    virtual void addPath(const boost::filesystem::path& _modulePath) = 0;

    /// Drop a module search path
    virtual void dropPath(const boost::filesystem::path& _modulePath) = 0;

    /// Find module path if one exists
    virtual bool findPath(const std::string _moduleName, boost::filesystem::path &_modulePath) = 0;
    /// @}

    /// @name Static Instance
    /// @{
public:
    static I_ModuleManager& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ModuleManager();
    virtual ~I_ModuleManager();
    /// @}

};  // interface I_ModuleManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugin
}   // namespace Core
}	// namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
