//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include "View.hpp"
#include "NotebookPage.hpp"

#include <Helmet/Workbench/I_Workbench.hpp>
#include <Helmet/Workbench/I_Entity.hpp>

#include <wx/treectrl.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Workbench;

class WorkspaceView
:   public View
,   public NotebookPage
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<I_NotebookPage>                   pNotebookPage_type;
    typedef boost::shared_ptr<Helmet::Workbench::I_Workbench>   pWorkbench_type;
    typedef boost::shared_ptr<Helmet::Workbench::I_Entity>      pEntity_type;
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

    /// @name ProjectView implementation
    /// @{
public:
    wxTreeItemId getItemByName(const std::string& _name);

    void onBeginDrag(wxTreeEvent& _event);
    void onBeginRightDrag(wxTreeEvent& _event);
    void onEndDrag(wxTreeEvent& _event);
    void onBeginLabelEdit(wxTreeEvent& _event);
    void onEndLabelEdit(wxTreeEvent& _event);
    void onDeleteItem(wxTreeEvent& _event);
    void onGetInfo(wxTreeEvent& _event);
    void onSetInfo(wxTreeEvent& _event);
    void onItemExpanded(wxTreeEvent& _event);
    void onItemExpanding(wxTreeEvent& _event);
    void onItemCollapsed(wxTreeEvent& _event);
    void onItemCollapsing(wxTreeEvent& _event);
    void onSelectionChanged(wxTreeEvent& _event);
    void onSelectionChanging(wxTreeEvent& _event);
    void onTreeKeyDown(wxTreeEvent& _event);
    void onItemActivated(wxTreeEvent& _event);
    void onItemStateClick(wxTreeEvent& _event);
    void onContextMenu(wxContextMenuEvent& _event);
    void onItemMenu(wxTreeEvent& _event);
    void onItemRightClick(wxTreeEvent& _event);
    void onRightMouseDown(wxMouseEvent& _event);
    void onRightMouseUp(wxMouseEvent& _event);
    void onRightMouseDoubleClick(wxMouseEvent& _event);
    /// @}

    /// @name Internal Structures
    /// @{
public:
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class WorkspaceItemData
    :   public wxTreeItemData
    ,   public wxObject
    {
        /// @name Types
        /// @{
    public:
        typedef boost::shared_ptr<Helmet::Workbench::I_Entity>  pEntity_type;
        /// @}

        /// @name ProjectTreeItemData implementation
        /// @{
    public:
        pEntity_type getEntity();
        /// @}

        /// @name 'Structors
        /// @{
    public:
        explicit WorkspaceItemData(pEntity_type _pEntity);
        virtual ~WorkspaceItemData();
        /// @}

        /// @name Member Variables
        /// @{
    private:
        pEntity_type    m_pEntity;
        /// @}
    };
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    /// @name 'Structors
    /// @{
public:
    WorkspaceView(Workbench& _workbench,
                  Notebook& _parent,
                  const std::string& _name);
    virtual ~WorkspaceView();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Workbench&                          m_workbench;
    wxSizer*                            m_pSizer;
    wxTreeCtrl*                         m_pTreeCtrl;
    wxImageList*                        m_pImageList;
    pConnection_type                    m_pConnection;
    /// @}

};  // class ProjectView

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
