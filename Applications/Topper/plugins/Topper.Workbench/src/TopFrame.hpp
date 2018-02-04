//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Workbench/I_Frame.hpp>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

#include <list>

#include <wx/aui/aui.h>

namespace boost{
    namespace filesystem{
        class path;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
    namespace Workbench {
        class I_Workbench;
        class I_MenuBar;
        class I_StatusBar;
        class I_Notebook;
    }   // namespace Workbench
}   // namespace Helmet
namespace Topper {
class Workbench;
class Notebook;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class TopFrame
:   public Helmet::Workbench::I_Frame
,   public wxFrame
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<Helmet::Workbench::I_Frame>       pFrame_type;
    typedef boost::shared_ptr<Helmet::Workbench::I_Workbench>   pWorkbench_type;
    typedef std::list<pFrame_type>                              pFrames_type;
    /// @}

    /// @name I_Frame implementation
    /// @{
public:
    void setName(const std::string& _name) override;
    const std::string getName() const override;
    void addChildFrame(I_Frame& _frame) override;
    void removeChildFrame(I_Frame& _frame) override;
    void getChildFrames(I_FrameVisitor& _visitor) override;
    void show(bool _visible) override;
    void handleEvent(pEvent_type _pEvent) override;
    /// @}

    /// @name TopFrame implementation
    /// @{
public:
    Helmet::Workbench::I_MenuBar& getMenuBar();
    Helmet::Workbench::I_StatusBar& getStatusBar();
    Notebook& getBottomNotebook();
    Notebook& getLeftNotebook();
    Notebook& getCenterNotebook();
    /// @}

    /// @name Event Handlers
    /// @{
private:
    /// @}

    /// @name 'Structors
    /// @{
public:
     TopFrame(pWorkbench_type _pWorkbench, const std::string& _pTitle, int _xpos, int _ypos, int _width, int _height);
    ~TopFrame() override;
    /// @}

    /// @name Member Variables
    /// @{
private:
    std::string                 m_name;
    wxAuiManager                m_mgr;
    pWorkbench_type             m_pWorkbench;
    pFrames_type                m_pFrames;
    /// @}

};  // class TopFrame

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
