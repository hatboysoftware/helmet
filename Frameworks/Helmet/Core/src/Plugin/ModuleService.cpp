//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "ModuleService.hpp"
#include "ModuleInfo.hpp"

#include <Helmet/Core/Plugin/I_Module.hpp>
#include <Helmet/Core/Plugin/I_ModuleManager.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/log/trivial.hpp>
#include <boost/thread/locks.hpp>

#include <sstream>

#include <stddef.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Plugin {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ModuleService::ModuleService()
:   m_moduleIdx()
,   m_modules()
,   m_moduleMutex()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ModuleService::~ModuleService() = default;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ModuleService::pModule_type
ModuleService::load(const std::string& _moduleName)
{
    boost::lock_guard<boost::mutex> guard(m_moduleMutex);

    auto iter = m_moduleIdx.find(_moduleName);
    if(iter != m_moduleIdx.end())
    {
        // Increment the reference count
        m_modules[iter->second]->incrementReferences();

        // Return the
        return iter->second;
    }

    pModuleInfo_type pModuleInfo(new ModuleInfo);

    auto pModuleManager = &I_ModuleManager::getSingleton();

    boost::filesystem::path modulePath;
    if(!pModuleManager->findPath(_moduleName, modulePath))
    {
        std::stringstream stream;
        stream << "Module " << _moduleName << " not found in defined module search paths.";
        BOOST_LOG_TRIVIAL(error) << stream.str();

        throw std::runtime_error(stream.str());
    }

    pModuleInfo->setName(_moduleName);

#ifdef _WIN32
    auto hModule = LoadLibraryA(modulePath.string().c_str());
#else
    BOOST_LOG_TRIVIAL(info) << "dlopen " << modulePath.string().c_str();
    I_ModuleInfo::ModuleHandle_type hModule = dlopen(modulePath.string().c_str(), RTLD_NOW | RTLD_GLOBAL);
#endif // _WIN32

    if (hModule == nullptr)
    {
        std::stringstream stream;
        stream << "Error loading module " << modulePath.string()
               #ifndef _WIN32
               << dlerror()
#else
            << ": The module is probably missing dependencies not on the path.  Use depends.exe to figure it out."
#endif
                ;

        BOOST_LOG_TRIVIAL(error) << stream.str();
        throw std::runtime_error(stream.str());
    }

    pModuleInfo->setHandle(hModule);

    // Get the "getModule" procedure
#if   defined _WIN32
    auto proc = GetProcAddress(hModule, "getModule");
#else
    void* proc = dlsym(hModule, "getModule");
#endif

    // Check to make sure the procedure exists
    if (proc)
    {
        // Convert the procedure type to the assumed type
#ifdef _WIN32
        auto pRealProc = reinterpret_cast<I_Module::proc_ptr_type>(proc);
#else
        I_Module::QUERY_MODULE_FUNCTION_PTR pRealProc = reinterpret_cast<I_Module::QUERY_MODULE_FUNCTION_PTR>(proc);
#endif

        // Execute the procedure to get the I_Module for this module.
        auto pModule = &(pRealProc());

        // Set the reference count to 1
        pModuleInfo->incrementReferences();

        // Put it in the index
        m_moduleIdx[_moduleName] = pModule;

        // Put it in the cache
        m_modules[pModule] = pModuleInfo;

        // And return it.
        return pModule;
    }
    else
    {
        BOOST_LOG_TRIVIAL(error) << "Error getting procedure address in module " << modulePath.string();

        // Not found, so return NULL
        return nullptr;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ModuleService::unload(pModule_type _pModule)
{
    boost::lock_guard<boost::mutex> guard(m_moduleMutex);

    // Use _pModule to get the module info
    auto pModuleInfo = m_modules[_pModule];

    // Decrement the reference
    // If the count is zero, get the handle and unload the library
    if( pModuleInfo->decrementReferences() == 0 )
    {
        auto moduleName = pModuleInfo->getName();

        // Remove module from the index
        auto moduleIndexIterator = m_moduleIdx.find(moduleName);
        m_moduleIdx.erase(moduleIndexIterator);

        // Physically unload module
        auto hModule = pModuleInfo->getHandle();

#ifdef _WIN32
        if( !FreeLibrary(hModule) )
#else
        if( dlclose(hModule) )
#endif // _WIN32
        {
            std::stringstream stream;
            stream << "Error unloading module " << moduleName;
            BOOST_LOG_TRIVIAL(error) << stream.str();
            throw std::runtime_error(stream.str());
        }

        // Remove module from the cache
        auto moduleIterator = m_modules.find(_pModule);
        m_modules.erase(moduleIterator);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugin
}   // namespace Core
}	// namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
