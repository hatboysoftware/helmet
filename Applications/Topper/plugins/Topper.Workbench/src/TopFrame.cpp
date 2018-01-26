//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "TopFrame.hpp"
#include "Workbench.hpp"
#include "MenuBar.hpp"
#include "StatusBar.hpp"
#include "Notebook.hpp"

#include <Helmet/Core/Plugin/I_Environment.hpp>
#include <Helmet/Workbench/I_Event.hpp>

#include <boost/property_tree/ptree.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TopFrame::TopFrame(pWorkbench_type _pWorkbench, const std::string& _pTitle, int _xpos, int _ypos, int _width, int _height)
:   wxFrame(
        nullptr,
        wxID_ANY,
        wxString::FromUTF8(_pTitle.c_str()),
        wxPoint(_xpos, _ypos),
        wxSize(_width, _height))
,   m_name(_pTitle)
,	m_mgr(this)
,	m_pWorkbench(std::move(_pWorkbench))
,	m_pFrames()
{
    m_mgr.Update();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TopFrame::~TopFrame()
{
    m_mgr.UnInit();

    for (auto frame : m_pFrames)
        frame.reset();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TopFrame::setName(const std::string& _name)
{
    m_name = _name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string
TopFrame::getName() const
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TopFrame::addChildFrame(I_Frame& _frame)
{
    auto pFrame = dynamic_cast<wxFrame*>(&_frame);
    AddChild(pFrame);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TopFrame::removeChildFrame(I_Frame& _frame)
{
    auto pFrame = dynamic_cast<wxFrame*>(&_frame);
    RemoveChild(pFrame);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TopFrame::getChildFrames(I_FrameVisitor& _visitor)
{
    _visitor.begin();

    for (const auto &pFrame: m_pFrames)
    {
        _visitor.visit(*pFrame);
    }

    _visitor.end();
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void TopFrame::show(bool _visible) {
    Show(_visible);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TopFrame::handleEvent(pEvent_type _pEvent)
{
    if (_pEvent.get() != nullptr)
    {
        if (_pEvent->getId() == "Exit")
        {
            Close(true);
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Helmet::Workbench::I_MenuBar&
TopFrame::getMenuBar()
{
    auto pMenuBar = dynamic_cast<MenuBar*>(GetMenuBar());
    if (pMenuBar != nullptr)
    {
        return *pMenuBar;
    }

    pMenuBar = new MenuBar();
    SetMenuBar(pMenuBar);
    return *pMenuBar;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Helmet::Workbench::I_StatusBar&
TopFrame::getStatusBar()
{
    auto pStatusBar = dynamic_cast<StatusBar*>(GetStatusBar());
    if (pStatusBar != nullptr)
    {
        return *pStatusBar;
    }

    pStatusBar = new StatusBar(CreateStatusBar(4));
    SetStatusBar(pStatusBar->getConcreteStatusBar());
    return *pStatusBar;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Notebook&
TopFrame::getBottomNotebook()
{
    auto pNotebook = new Notebook(*this, "bottom");
    wxAuiPaneInfo paneInfo;
    paneInfo.Name(wxString::FromUTF8("bottom"));
    paneInfo.Caption(wxString::FromUTF8("Bottom Dock"));
    paneInfo.CloseButton(true);
    paneInfo.MinimizeButton(true);
    paneInfo.MaximizeButton(true);
    paneInfo.PinButton(true);
    paneInfo.Bottom();

    m_mgr.AddPane(pNotebook, paneInfo);

    m_mgr.Update();

    return *pNotebook;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Notebook&
TopFrame::getLeftNotebook()
{
    auto pNotebook = new Notebook(*this, "left");
    wxAuiPaneInfo paneInfo;
    paneInfo.Name(wxString::FromUTF8("left"));
    paneInfo.Caption(wxString::FromUTF8("Left Dock"));
    paneInfo.CloseButton(true);
    paneInfo.MinimizeButton(true);
    paneInfo.MaximizeButton(true);
    paneInfo.PinButton(true);
    paneInfo.Left();

    m_mgr.AddPane(pNotebook, paneInfo);

    m_mgr.Update();

    return *pNotebook;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Notebook&
TopFrame::getCenterNotebook()
{
    auto pNotebook = new Notebook(*this, "center");
    wxAuiPaneInfo paneInfo;
    paneInfo.Name(wxString::FromUTF8("center"));
    paneInfo.Caption(wxString::FromUTF8("Center Dock"));
    paneInfo.CloseButton(true);
    paneInfo.MinimizeButton(true);
    paneInfo.MaximizeButton(true);
    paneInfo.PinButton(true);
    paneInfo.Center();

    m_mgr.AddPane(pNotebook, paneInfo);

    m_mgr.Update();

    return *pNotebook;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/*
 *     auto pNotebook = new Notebook(*this, _title);
    wxAuiPaneInfo panelInfo;
    panelInfo.Name(wxString::FromUTF8(_title.c_str()));
    panelInfo.Caption(wxString::FromUTF8(_caption.c_str()));
    panelInfo.CloseButton(_closeButton == "true");
    panelInfo.MinimizeButton(_minimizeButton == "true");
    panelInfo.MaximizeButton(_maximizeButton == "true");
    panelInfo.PinButton(_pinButton == "true");
    if (_toolbar == "true")
    {
        panelInfo = panelInfo.ToolbarPane();
    }

    if (_dock == "left")
    {
        panelInfo.Left();
    }
    else if (_dock == "top")
    {
        panelInfo.Top();
    }
    else if (_dock == "right")
    {
        panelInfo.Right();
    }
    else if (_dock == "bottom")
    {
        panelInfo.Bottom();
    }
    else if (_dock == "center")
    {
        panelInfo.Center();
    }
    else
    {
        panelInfo.Center();
    }

    m_mgr.AddPane(
        pNotebook->getWindow(),
        panelInfo);

    m_mgr.Update();

    auto pManagedNotebook = pNotebook_type(pNotebook,
                                           Hyperion::Core::Utility::Deleter<
                                                Hyperion::Workbench::I_Notebook,
                                                Notebook>());
    m_pFrames.push_back(pManagedNotebook);
    return pManagedNotebook;

 */