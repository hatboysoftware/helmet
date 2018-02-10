//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "Application.hpp"

#include <Helmet/Core/Plugin/I_Environment.hpp>
#include <Helmet/Core/Plugin/I_PluginManager.hpp>

#include <Helmet/Enterprise/I_ApplicationServer.hpp>
#include <Helmet/Enterprise/I_ResourceLocation.hpp>

#include <Helmet/Blockchain/I_BlockchainPlugin.hpp>
#include <Helmet/Blockchain/I_BlockchainNode.hpp>

#include <Helmet/Workbench/I_WorkbenchPlugin.hpp>
#include <Helmet/Workbench/I_Workbench.hpp>
#include <Helmet/Workbench/I_Frame.hpp>

#include <boost/log/trivial.hpp>
#include <boost/filesystem.hpp>

#include <wx/config.h>
#include <wx/msgdlg.h>

DECLARE_APP(Topper::Application)
IMPLEMENT_APP(Topper::Application)

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Application::Application()
:   m_localeGenerator()
,   m_locale(m_localeGenerator.generate(""))
,   m_applicationServer(Helmet::Enterprise::I_ApplicationServer::getInstance("Topper::ApplicationServer"))
,   m_pBlockchainNode(nullptr)
,   m_pWorkbench(nullptr)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Application::~Application() = default;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Application::OnInit()
{
    std::unique_ptr<wxConfig> pConfig(new wxConfig(wxT("Topper")));

    wxString lastWorkspace;

    if (pConfig->HasEntry(wxT("LastWorkspace")))
    {
        pConfig->Read(wxT("LastWorkspace"), &lastWorkspace);
    }

    auto pEnvironment = Helmet::Core::Plugin::I_Environment::create();
    pEnvironment->writeConfigurationField("plugin-path", "./plugins");
    auto pPluginManager = Helmet::Core::Plugin::I_PluginManager::create(pEnvironment);
    {
        auto workbenchId = "Topper.Workbench";
        auto pPlugin = pPluginManager->getPlugin(workbenchId);
        auto pWorkbenchPlugin =
                boost::static_pointer_cast<Helmet::Workbench::I_WorkbenchPlugin>(pPlugin);
        m_pWorkbench = pWorkbenchPlugin->getWorkbench(pEnvironment);
    }

    {
        auto blockchainId = "Topper.Transaction.Blockchain";
        auto pPlugin = pPluginManager->getPlugin(blockchainId);
        auto pBlockchainPlugin =
                boost::static_pointer_cast<Helmet::Blockchain::I_BlockchainPlugin>(pPlugin);
        m_pBlockchainNode = pBlockchainPlugin->getNode(m_applicationServer, "Topper.Transaction.Blockchain");
    }

    m_applicationServer.installApplication(m_pBlockchainNode, Helmet::Enterprise::I_ResourceLocation::getLocation("blockchain"));

    m_pWorkbench->getMainFrame().show(true);

    m_applicationServer.start();

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
Application::OnExit()
{
    m_applicationServer.stop();
    return 0;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
Application::OnRun()
{
    try
    {
        return wxApp::OnRun();
    }
    catch (std::exception& _ex)
    {
        BOOST_LOG_TRIVIAL(fatal) << "Application::OnRun(): Exception! : " << _ex.what() << std::endl;
    }

    return 1;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Application::OnExceptionInMainLoop()
{
    auto exit = false;
    try
    {
        throw;
    }
    catch (const std::exception& e)
    {
        wxMessageDialog exceptionDialog(
                nullptr,
                e.what(),
                wxT("Runtime Exception..."),
                wxOK | wxSTAY_ON_TOP);
        BOOST_LOG_TRIVIAL(error) << e.what();
    }

    return !exit;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
