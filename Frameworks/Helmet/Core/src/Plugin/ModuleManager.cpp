//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "ModuleManager.hpp"
#include "ModuleService.hpp"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/log/trivial.hpp>

#include <sstream>

#include <stddef.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Plugin {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ModuleManager::ModuleManager()
:   m_pModuleService()
,   m_modulePaths()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ModuleManager::~ModuleManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static boost::shared_ptr<I_ModuleService> sm_pModuleService;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ModuleManager::pService_type
ModuleManager::getService()
{
    if (sm_pModuleService.get() == nullptr)
    {
        sm_pModuleService.reset(new ModuleService());
    }
    return sm_pModuleService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ModuleManager::addPath(const boost::filesystem::path& _modulePath)
{
    m_modulePaths.push_back(_modulePath);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ModuleManager::dropPath(const boost::filesystem::path& _modulePath)
{
    ModulePaths_type::iterator iter;

    iter = m_modulePaths.begin();
    while( (*iter != _modulePath) &&
           (iter  != m_modulePaths.end()) )
    {
        ++iter;
    }

    if( iter != m_modulePaths.end() )
        m_modulePaths.erase(iter);
    else
    {
        std::stringstream stream;
        stream << "Path " << _modulePath << " is not in the list of module search paths.";
        BOOST_LOG_TRIVIAL(error) << stream.str();

        throw std::runtime_error(stream.str());
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ModuleManager::findPath(const std::string _moduleName, boost::filesystem::path &_modulePath)
{
    auto modulePathFound = false;
    boost::filesystem::path modulePath;
    ModulePaths_type::iterator iter;

    if( m_modulePaths.empty() )
        return false;

    // Set the extension
    std::stringstream moduleName;

#if   defined _WIN32 || HOST_CYGWIN
    moduleName << _moduleName;
#ifdef _DEBUG
#    ifdef USE_DEBUG_POSTFIX
    moduleName << "_d";
#    endif // USE_DEBUG_POSTFIX
#endif
    moduleName << ".dll";
#elif defined __APPLE__
    moduleName << "lib" << _moduleName << ".dylib";
#else
    moduleName << "lib" << _moduleName << ".so";
#endif

    iter = m_modulePaths.begin();
    do
    {
        // Create the fully qualified path
        modulePath = *iter / boost::filesystem::path(moduleName.str());

        // Check if the path is valid
        modulePathFound = boost::filesystem::exists(modulePath);

        BOOST_LOG_TRIVIAL(info) << "Attempting " << modulePath.string();

        // Iterate path iterator
        ++iter;

    }
    while( iter != m_modulePaths.end() &&
           !modulePathFound );

    if( modulePathFound )
    {
        _modulePath = modulePath;
        return true;
    }
    else
    {
        _modulePath = std::string("");
        return false;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugin
}   // namespace Core
}	// namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
