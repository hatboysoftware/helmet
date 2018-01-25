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

#include <string>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Plugin {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class HELMET_CORE_DLL_LINK I_ModuleInfo
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
#if defined _WIN32
    typedef HMODULE     ModuleHandle_type;
#else
    typedef void*       ModuleHandle_type;
#endif // _WIN32
    /// @}

    /// @name I_ModuleInfo interface
    /// @{
public:
    virtual void setName(const std::string _name) = 0;
    virtual std::string getName() const = 0;

    virtual void setHandle(const ModuleHandle_type _handle) = 0;
    virtual ModuleHandle_type getHandle() const = 0;

    virtual unsigned incrementReferences() = 0;
    virtual unsigned decrementReferences() = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ModuleInfo();
    virtual ~I_ModuleInfo();
    /// @}

};  // interface I_ModuleInfo

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugin
}   // namespace Core
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
