//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include "../Configuration.hpp"

#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>

#ifdef HOST_POSIX
#include <dlfcn.h>
#elif __APPLE__
#include <dlfcn.h>
#endif //HOST_POSIX

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Plugin {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

// Forward declarations
class I_Plugin;

/// Base class for a loadable problem module
/// A plugin DLL should implement one of these
class HELMET_CORE_DLL_LINK I_Module
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<I_Plugin>    pPlugin_type;

#ifdef HOST_WIN32
    typedef I_Module& (*proc_ptr_type)();
#elif HOST_POSIX
    typedef I_Module& (*QUERY_MODULE_FUNCTION_PTR)();
#elif __APPLE__
    typedef I_Module& (*QUERY_MODULE_FUNCTION_PTR)();
#else
    #error Unsupported platform in I_Module.hpp
#endif
    /// @}

    /// @name I_Module interface
    /// @{
public:
    /// Gets the name of the plugin provided by this module.
    /// @return The name of the plugin provided by this module.
    virtual const std::string& getName() const = 0;

    /// Sets the root path of the plugin provided by this module.
    /// @param[in] _path The root path of the plugin provided by this module.
    virtual void setPath(const boost::filesystem::path& _path) = 0;

    /// Gets the root path of the plugin provided by this module.
    /// @return The root path of the plugin provided by this module.
    virtual const boost::filesystem::path& getPath() const = 0;

    /// Returns a plugin.
    /// For now, Plugins are all considered singletons. The problem should
    /// normally not be created until getPlugin is called, in case the plugin
    /// isn't required.
    /// @return The plugin instance.
    virtual pPlugin_type getPlugin() = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Module();
    virtual ~I_Module();
    /// @}

};  // interface I_Module

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugin
}   // namespace Core
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
