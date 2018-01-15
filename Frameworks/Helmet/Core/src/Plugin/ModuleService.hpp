//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Core/Plugin/I_ModuleService.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Plugin {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ModuleInfo;

class ModuleService
:   public I_ModuleService
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, pModule_type>         ModuleNameIdx_type;
    typedef boost::shared_ptr<I_ModuleInfo>             pModuleInfo_type;
    typedef std::map<pModule_type, pModuleInfo_type>    Modules_type;
    /// @}

    /// @name I_ModuleService implementation
    /// @{
public:
    virtual pModule_type load(const std::string& _moduleName) override;
    virtual void unload(pModule_type _pModule) override;
    /// @}

    /// @name 'Structors
    /// @{
public:
    ModuleService();
    virtual ~ModuleService();
    /// @}

    /// @name Member variables
    /// @{
private:
    ModuleNameIdx_type      m_moduleIdx;
    Modules_type            m_modules;
    boost::mutex            m_moduleMutex;
    /// @}

};  // interface ModuleService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugin
}   // namespace Core
}	// namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
