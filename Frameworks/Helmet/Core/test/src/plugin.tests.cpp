//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#define BOOST_TEST_MODULE HelmetCorePlugin_Test
#include <boost/test/unit_test.hpp>

#include <Helmet/Core/Plugin/I_ModuleManager.hpp>
#include <Helmet/Core/Plugin/I_ModuleService.hpp>

#include <Helmet/Core/Plugin/I_Plugin.hpp>
#include <Helmet/Core/Plugin/I_Module.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class TestPlugin
:   public Helmet::Core::Plugin::I_Plugin
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Plugin implementation
    /// @{
public:
    const std::string& getName() const override;
    /// @}

    /// @name TestPlugin implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             TestPlugin();
    virtual ~TestPlugin();
    /// @}

    /// @name Member Variables
    /// @{
private:
    const std::string m_name;
    /// @}

};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TestPlugin::TestPlugin()
:   m_name("test")
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TestPlugin::~TestPlugin()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
TestPlugin::getName() const
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class TestModule
:   public Helmet::Core::Plugin::I_Module
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Module implementation
    /// @{
public:
    virtual const std::string& getName() const override;
    virtual void setPath(const boost::filesystem::path& _path) override;
    virtual const boost::filesystem::path& getPath();
    virtual pPlugin_type getPlugin() override;
    /// @}

    /// @name TestModule implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// @}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BOOST_AUTO_TEST_CASE(simple_plugin_example)
{
    try
    {
        return;
//        // Get the module
//        // getModule is a function defined indirectly by IMPLEMENT_SINGLE_ZEN_PLUGIN macro
//        auto& module = getModule();
//
//        auto moduleService = Zen::Plugin::I_ModuleManager::getSingleton().getService(
//                Zen::Plugin::I_ModuleManager::STATIC);
//
//        moduleService->install(moduleName, module);
//
//        auto& extensionPoint = TestExtensionPoint::getSingleton();
//        auto extension = extensionPoint.create("test");
        // extension->test();
    } catch(std::exception& ex) {
        std::cout << ex.what() << std::endl;
    } catch(...) {
        std::cout << "Unknown exception thrown" << std::endl;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
