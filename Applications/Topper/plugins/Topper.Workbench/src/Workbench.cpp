//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "Workbench.hpp"
#include "TopFrame.hpp"
#include "Menu.hpp"
#include "MenuItem.hpp"

#include <Helmet/Core/Plugin/I_Environment.hpp>
#include <Helmet/Workbench/I_MenuBar.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Workbench::Workbench(pEnvironment_type _pEnvironment)
:   m_pTopFrame(nullptr)
,   m_pEnvironment(std::move(_pEnvironment))
,   m_pLocalEnvironment(Helmet::Core::Plugin::I_Environment::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Workbench::~Workbench()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Helmet::Workbench::I_Frame &Workbench::getMainFrame()
{
    if (m_pTopFrame == nullptr)
    {
        initialize();
        assert(m_pTopFrame != nullptr);
    }

    return *m_pTopFrame;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void Workbench::initialize()
{
    int displayWidth, displayHeight;
    wxDisplaySize(&displayWidth, &displayHeight);
    m_pTopFrame = new TopFrame(
            shared_from_this(),
            "Topper Trading Workbench",
            100,
            100,
            640,
            480);

    auto& menuBar = m_pTopFrame->getMenuBar();
    auto fileMenu = new Menu("File");
    auto openWorkspaceMenuItem = new MenuItem(100, "Open Workspace...", "O", false, nullptr);
    openWorkspaceMenuItem->bind(*m_pTopFrame, "open_workspace");
    fileMenu->addMenuItem(*openWorkspaceMenuItem);
    menuBar.addMenu(*fileMenu);

    auto& statusBar = m_pTopFrame->getStatusBar();

    m_pTopFrame->show(true);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
