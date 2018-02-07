//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "PluginManager.hpp"

#include <Helmet/Core/Plugin/I_ModuleManager.hpp>
#include <Helmet/Core/Plugin/I_ModuleService.hpp>
#include <Helmet/Core/Plugin/I_Module.hpp>

#include <boost/log/trivial.hpp>
#include <boost/property_tree/ptree.hpp>

#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Plugin {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PluginManager::PluginManager(I_Environment::pEnvironment_type pEnvironment)
:   m_pEnvironment(pEnvironment)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PluginManager::~PluginManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_PluginManager::pPlugin_type
PluginManager::getPlugin(const std::string& plugin)
{
    auto pluginPath = (*m_pEnvironment)["plugin-path"].get_value<boost::filesystem::path>();
    pluginPath = boost::filesystem::absolute(pluginPath) / plugin;
    if (!boost::filesystem::exists(pluginPath))
    {
        std::stringstream stream;
        stream << "Plugin path \"" << pluginPath << "\" does not exist.";
        BOOST_LOG_TRIVIAL(error) << stream.str();
        BOOST_THROW_EXCEPTION(std::runtime_error(stream.str()));
    }

    auto modulePath = pluginPath;// / "bin";

    I_ModuleManager::getSingleton().addPath(modulePath);

    auto pModule =
            I_ModuleManager::getSingleton()
                    .getService()
                    ->load(plugin);

    if (pModule != nullptr)
    {
        pModule->setPath(pluginPath);
        return pModule->getPlugin();
    }

    return pPlugin_type(nullptr);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugin
}   // namespace Core
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
