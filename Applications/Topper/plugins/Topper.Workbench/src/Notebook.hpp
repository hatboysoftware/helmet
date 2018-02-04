//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Workbench/I_Notebook.hpp>

#include <wx/aui/aui.h>
#include <vector>
#include "NotebookPage.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Notebook
:   public Helmet::Workbench::I_Notebook
,   public wxAuiNotebook
{
    /// @name Types
    /// @{
public:
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

    /// @name I_Notebook implementation
    /// @{
public:
    Helmet::Workbench::I_NotebookPage& addPage(Helmet::Workbench::I_NotebookPage& _page) override;
    void showPage(int _index) override;
    void showPage(Helmet::Workbench::I_NotebookPage& _page) override;
    void removePage(int _index) override;
    void removePage(Helmet::Workbench::I_NotebookPage& _page) override;
    void getPages(Helmet::Core::Utility::I_Visitor<Helmet::Workbench::I_NotebookPage>& _visitor) override;
    /// @}

    /// @name Notebook implementation
    /// @{
public:
    void onPageChanged(wxAuiNotebookEvent& _event);
    void onPageClose(wxAuiNotebookEvent& _event);
    /// @}

    /// @name 'Structors
    /// @{
public:
    Notebook(wxWindow& _parent, const std::string& _name);
    ~Notebook() override;
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// @}

};  // class Notebook

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
