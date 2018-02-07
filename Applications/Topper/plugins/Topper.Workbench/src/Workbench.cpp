//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "Workbench.hpp"
#include "TopFrame.hpp"
#include "Menu.hpp"
#include "MenuItem.hpp"
#include "LogView.hpp"
#include "LogModel.hpp"
#include "Notebook.hpp"
#include "TickerTableView.hpp"
#include "TickerChartView.hpp"
#include "TickerDataModel.hpp"
#include "WorkspaceView.hpp"


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

    auto& bottomNotebook = m_pTopFrame->getBottomNotebook();
    auto logView = new LogView(bottomNotebook, "Console Log");
    auto logModel = new LogModel(std::cout, m_pEnvironment);
    logModel->subscribe(*logView);
    bottomNotebook.addPage(*logView);

    auto& leftNotebook = m_pTopFrame->getLeftNotebook();


    auto& centerNotebook = m_pTopFrame->getCenterNotebook();
    auto tickerTableView = new TickerTableView(*this, centerNotebook, "Ticker Data");
    auto tickerChartView = new TickerChartView(*this, centerNotebook, "Ticker Data");
    auto tickerDataModel = new TickerDataModel(m_pEnvironment);
    tickerDataModel->subscribe(*tickerTableView);
    tickerDataModel->subscribe(*tickerChartView);
    centerNotebook.addPage(*tickerTableView);
    centerNotebook.addPage(*tickerChartView);

    m_pTopFrame->show(true);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
