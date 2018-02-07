//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "TickerTableView.hpp"
#include "TickerDataModel.hpp"
#include "Workbench.hpp"
#include "Notebook.hpp"

#include <Helmet/Core/Event/I_Connection.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TickerTableView::TickerTableView(Workbench &_workbench,
                                 Notebook &_parent,
                                 const std::string &_name)
:   View(_name)
,   NotebookPage(_name, _parent)
,   m_workbench(_workbench)
,   m_pSizer(new wxBoxSizer(wxHORIZONTAL))
,   m_pGrid(new wxGrid(&_parent,
                       wxID_ANY,
                       wxDefaultPosition,
                       wxDefaultSize))
,   m_pConnection(nullptr)
{
    m_pSizer->Add(m_pGrid, 1, wxEXPAND);
    _parent.SetSizer(m_pSizer);
    _parent.Layout();

    _parent.Bind(wxEVT_GRID_CELL_LEFT_CLICK, &TickerTableView::onCellLeftClick, this);
    _parent.Bind(wxEVT_GRID_CELL_RIGHT_CLICK, &TickerTableView::onCellRightClick, this);
    _parent.Bind(wxEVT_GRID_CELL_LEFT_DCLICK, &TickerTableView::onCellLeftDoubleClick, this);
    _parent.Bind(wxEVT_GRID_CELL_RIGHT_DCLICK, &TickerTableView::onCellRightDoubleClick, this);
    _parent.Bind(wxEVT_GRID_LABEL_LEFT_CLICK, &TickerTableView::onLabelLeftClick, this);
    _parent.Bind(wxEVT_GRID_LABEL_RIGHT_CLICK, &TickerTableView::onLabelRightClick, this);
    _parent.Bind(wxEVT_GRID_LABEL_LEFT_DCLICK, &TickerTableView::onLabelLeftDoubleClick, this);
    _parent.Bind(wxEVT_GRID_LABEL_RIGHT_DCLICK, &TickerTableView::onLabelRightDoubleClick, this);
    _parent.Bind(wxEVT_GRID_ROW_SIZE, &TickerTableView::onRowSize, this);
    _parent.Bind(wxEVT_GRID_COL_SIZE, &TickerTableView::onColSize, this);
    _parent.Bind(wxEVT_GRID_COL_AUTO_SIZE, &TickerTableView::onColAutoSize, this);
    _parent.Bind(wxEVT_GRID_RANGE_SELECT, &TickerTableView::onRangeSelect, this);
    _parent.Bind(wxEVT_GRID_CELL_CHANGING, &TickerTableView::onCellChanging, this);
    _parent.Bind(wxEVT_GRID_CELL_CHANGED, &TickerTableView::onCellChanged, this);
    _parent.Bind(wxEVT_GRID_SELECT_CELL, &TickerTableView::onSelectCell, this);
    _parent.Bind(wxEVT_GRID_EDITOR_SHOWN, &TickerTableView::onEditorShown, this);
    _parent.Bind(wxEVT_GRID_EDITOR_HIDDEN, &TickerTableView::onEditorHidden, this);
    _parent.Bind(wxEVT_GRID_EDITOR_CREATED, &TickerTableView::onEditorCreated, this);
    _parent.Bind(wxEVT_GRID_CELL_BEGIN_DRAG, &TickerTableView::onCellBeginDrag, this);
    _parent.Bind(wxEVT_GRID_COL_MOVE, &TickerTableView::onColMove, this);
    _parent.Bind(wxEVT_GRID_COL_SORT, &TickerTableView::onColSort, this);
    _parent.Bind(wxEVT_GRID_TABBING, &TickerTableView::onTabbing, this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TickerTableView::~TickerTableView() = default;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Helmet::Core::Event::I_Observer::pConnection_type
TickerTableView::getConnection() const
{
    return View::getConnection();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::setConnection(pConnection_type _pConnection)
{
    auto& source = _pConnection->getSource();
    auto pModel = dynamic_cast<const TickerDataModel*>(&source);
    if (pModel != nullptr)
    {
        View::setConnection(_pConnection);
        return;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::activate()
{
    NotebookPage::activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::deactivate()
{
    NotebookPage::deactivate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Helmet::Workbench::I_Model*
TickerTableView::getModel() const
{
    return View::getModel();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::modelUpdate(Helmet::Core::Event::I_Event& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onCellLeftClick(wxGridEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onCellRightClick(wxGridEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onCellLeftDoubleClick(wxGridEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onCellRightDoubleClick(wxGridEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onLabelLeftClick(wxGridEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onLabelRightClick(wxGridEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onLabelLeftDoubleClick(wxGridEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onLabelRightDoubleClick(wxGridEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onRowSize(wxGridSizeEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onColSize(wxGridSizeEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onColAutoSize(wxGridSizeEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onRangeSelect(wxGridRangeSelectEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onCellChanging(wxGridEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onCellChanged(wxGridEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onSelectCell(wxGridEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onEditorShown(wxGridEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onEditorHidden(wxGridEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onEditorCreated(wxGridEditorCreatedEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onCellBeginDrag(wxGridEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onColMove(wxGridEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onColSort(wxGridEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerTableView::onTabbing(wxGridEvent &_event) {

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
