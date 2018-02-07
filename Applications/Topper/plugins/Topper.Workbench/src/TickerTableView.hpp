//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include "View.hpp"
#include "NotebookPage.hpp"

#include <wx/grid.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Workbench;

class TickerTableView
:   public View
,   public NotebookPage
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Observer implementation
    /// @{
public:
    pConnection_type getConnection() const override;
    void setConnection(pConnection_type _pConnection) override;
    /// @}

    /// @name I_NotebookPage interface
    /// @{
public:
    void activate() override;
    void deactivate() override;
    /// @}

    /// @name I_View implementation
    /// @{
public:
    const Helmet::Workbench::I_Model* getModel() const override;
    /// @}

    /// @name View implementation
    /// @{
public:
    void modelUpdate(Helmet::Core::Event::I_Event& _event) override;
    /// @}

    /// @name TickerTableView implementation
    /// @{
public:
    void onCellLeftClick(wxGridEvent& _event);
    void onCellRightClick(wxGridEvent& _event);
    void onCellLeftDoubleClick(wxGridEvent& _event);
    void onCellRightDoubleClick(wxGridEvent& _event);
    void onLabelLeftClick(wxGridEvent& _event);
    void onLabelRightClick(wxGridEvent& _event);
    void onLabelLeftDoubleClick(wxGridEvent& _event);
    void onLabelRightDoubleClick(wxGridEvent& _event);
    void onRowSize(wxGridSizeEvent& _event);
    void onColSize(wxGridSizeEvent& _event);
    void onColAutoSize(wxGridSizeEvent& _event);
    void onRangeSelect(wxGridRangeSelectEvent& _event);
    void onCellChanging(wxGridEvent& _event);
    void onCellChanged(wxGridEvent& _event);
    void onSelectCell(wxGridEvent& _event);
    void onEditorShown(wxGridEvent& _event);
    void onEditorHidden(wxGridEvent& _event);
    void onEditorCreated(wxGridEditorCreatedEvent& _event);
    void onCellBeginDrag(wxGridEvent& _event);
    void onColMove(wxGridEvent& _event);
    void onColSort(wxGridEvent& _event);
    void onTabbing(wxGridEvent& _event);
    /// @}

    /// @name 'Structors
    /// @{
public:
    TickerTableView(Workbench& _workbench,
                    Notebook& _parent,
                    const std::string& _name);
    virtual ~TickerTableView();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Workbench&          m_workbench;
    wxSizer*            m_pSizer;
    wxGrid*             m_pGrid;
    pConnection_type    m_pConnection;
    /// @}

};  // class TickerTableView

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
