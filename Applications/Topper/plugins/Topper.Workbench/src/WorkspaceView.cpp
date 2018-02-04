//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "WorkspaceView.hpp"
#include "WorkspaceModel.hpp"
#include "Workbench.hpp"
#include "Notebook.hpp"

#include <Helmet/Core/Event/I_Connection.hpp>
#include <Helmet/Core/Event/I_Event.hpp>

#include <wx/menu.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorkspaceView::WorkspaceView(Workbench& _workbench,
                             Notebook& _parent,
                             const std::string& _name)
:   View(_name)
,   NotebookPage(_name, _parent)
,   m_workbench(_workbench)
,   m_pSizer(new wxBoxSizer(wxHORIZONTAL))
,   m_pTreeCtrl(new wxTreeCtrl(&_parent,
                               wxID_ANY,
                               wxDefaultPosition,
                               wxDefaultSize,
                               wxTR_HAS_BUTTONS | wxTR_NO_LINES | wxTR_FULL_ROW_HIGHLIGHT | wxTR_ROW_LINES))
,   m_pImageList(new wxImageList(15, 15))
,   m_pConnection(nullptr)
{
    m_pImageList->Add(wxBitmap("resource/ui/folder.png", wxBITMAP_TYPE_PNG));
    m_pImageList->Add(wxBitmap("resource/ui/folder_selected.png", wxBITMAP_TYPE_PNG));
    m_pTreeCtrl->AssignImageList(m_pImageList);
    m_pSizer->Add(m_pTreeCtrl, 1, wxEXPAND);
    _parent.SetSizer(m_pSizer);

    _parent.Bind(wxEVT_TREE_BEGIN_DRAG, &WorkspaceView::onBeginDrag, this);
    _parent.Bind(wxEVT_TREE_BEGIN_RDRAG, &WorkspaceView::onBeginRightDrag, this);
    _parent.Bind(wxEVT_TREE_END_DRAG, &WorkspaceView::onEndDrag, this);
    _parent.Bind(wxEVT_TREE_BEGIN_LABEL_EDIT, &WorkspaceView::onBeginLabelEdit, this);
    _parent.Bind(wxEVT_TREE_END_LABEL_EDIT, &WorkspaceView::onEndLabelEdit, this);
    _parent.Bind(wxEVT_TREE_DELETE_ITEM, &WorkspaceView::onDeleteItem, this);
    _parent.Bind(wxEVT_TREE_GET_INFO, &WorkspaceView::onGetInfo, this);
    _parent.Bind(wxEVT_TREE_SET_INFO, &WorkspaceView::onSetInfo, this);
    _parent.Bind(wxEVT_TREE_ITEM_EXPANDED, &WorkspaceView::onItemExpanded, this);
    _parent.Bind(wxEVT_TREE_ITEM_EXPANDING, &WorkspaceView::onItemExpanding, this);
    _parent.Bind(wxEVT_TREE_ITEM_COLLAPSED, &WorkspaceView::onItemCollapsed, this);
    _parent.Bind(wxEVT_TREE_ITEM_COLLAPSING, &WorkspaceView::onItemCollapsing, this);
    _parent.Bind(wxEVT_TREE_SEL_CHANGED, &WorkspaceView::onSelectionChanged, this);
    _parent.Bind(wxEVT_TREE_SEL_CHANGING, &WorkspaceView::onSelectionChanging, this);
    _parent.Bind(wxEVT_TREE_KEY_DOWN, &WorkspaceView::onTreeKeyDown, this);
    _parent.Bind(wxEVT_TREE_ITEM_ACTIVATED, &WorkspaceView::onItemActivated, this);
    _parent.Bind(wxEVT_TREE_STATE_IMAGE_CLICK, &WorkspaceView::onItemStateClick, this);
    _parent.Bind(wxEVT_CONTEXT_MENU, &WorkspaceView::onContextMenu, this);
    _parent.Bind(wxEVT_TREE_ITEM_MENU, &WorkspaceView::onItemMenu, this);
    _parent.Bind(wxEVT_TREE_ITEM_RIGHT_CLICK, &WorkspaceView::onItemRightClick, this);
    _parent.Bind(wxEVT_RIGHT_DOWN, &WorkspaceView::onRightMouseDown, this);
    _parent.Bind(wxEVT_RIGHT_UP, &WorkspaceView::onRightMouseUp, this);
    _parent.Bind(wxEVT_RIGHT_DCLICK, &WorkspaceView::onRightMouseDoubleClick, this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorkspaceView::~WorkspaceView()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::modelUpdate(Helmet::Core::Event::I_Event& _event)
{
//    class EntityVisitor
//    :   public Helmet::Core::Utility::I_Visitor<const boost::shared_ptr<Helmet::Workbench::I_Entity>>
//    {
//    public:
//        void begin() override
//        {
//        }
//
//        bool visit(Object_type& _object) override
//        {
//            auto root = m_treeCtrl.AppendItem(m_root,
//                                              _object->getName(),
//                                              0,   // Set image
//                                              1,   // Set Selected image
//                                              new WorkspaceItemData(_object));
//
//            m_treeCtrl.SetItemImage(&m_root, 0, wxTreeItemIcon_Normal);
//            m_treeCtrl.SetItemImage(&m_root, 1, wxTreeItemIcon_Selected);
//            m_treeCtrl.SetItemImage(&m_root, 0, wxTreeItemIcon_Expanded);
//            m_treeCtrl.SetItemImage(&m_root, 1, wxTreeItemIcon_SelectedExpanded);
//
//            EntityVisitor visitor(root, m_treeCtrl);
//            _object->getChildEntities(visitor);
//
//            return false;
//        }
//
//        void end() override {}
//
//        EntityVisitor(wxTreeItemId& _root,
//                      wxTreeCtrl& _treeCtrl)
//                :   m_root(_root)
//                ,   m_treeCtrl(_treeCtrl)
//        {}
//
//    private:
//        wxTreeItemId&   m_root;
//        wxTreeCtrl&     m_treeCtrl;
//    };
//
//    auto& source = _event.getSource();
//    auto pWorkspaceModel = dynamic_cast<const WorkspaceModel*>(&source);
//    if (pWorkspaceModel != nullptr)
//    {
//        auto pRoot = pWorkspaceModel->getRootEntity();
//
//        m_pTreeCtrl->DeleteAllItems();
//        auto root = m_pTreeCtrl->AddRoot(pRoot->getName(),
//                                         0,     // Set image
//                                         1,     // Set Selected image
//                                         new WorkspaceItemData(pRoot));
//
//        EntityVisitor visitor(root, *m_pTreeCtrl);
//        pRoot->getChildEntities(visitor);
//
//        m_pTreeCtrl->ExpandAll();
//    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
wxTreeItemId
WorkspaceView::getItemByName(const std::string &name)
{
    std::stack<wxTreeItemId> items;
    if (m_pTreeCtrl->GetRootItem().IsOk())
        items.push(m_pTreeCtrl->GetRootItem());

    while (!items.empty()) {
        wxTreeItemId next = items.top();
        items.pop();

        if (next != m_pTreeCtrl->GetRootItem() && m_pTreeCtrl->GetItemText(next) == name)
            return next;

        wxTreeItemIdValue cookie;
        wxTreeItemId nextChild = m_pTreeCtrl->GetFirstChild(next, cookie);
        while (nextChild.IsOk()) {
            items.push(nextChild);
            nextChild = m_pTreeCtrl->GetNextSibling(nextChild);
        }
    }

    return wxTreeItemId();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Helmet::Core::Event::I_Observer::pConnection_type
WorkspaceView::getConnection() const
{
    return View::getConnection();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorkspaceView::setConnection(pConnection_type _pConnection)
{
    auto& source = _pConnection->getSource();
    auto pWorkspaceModel = dynamic_cast<const WorkspaceModel*>(&source);
    if (pWorkspaceModel != nullptr)
    {
        View::setConnection(_pConnection);
        return;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorkspaceView::activate()
{
    NotebookPage::activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorkspaceView::deactivate()
{
    NotebookPage::deactivate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Helmet::Workbench::I_Model*
WorkspaceView::getModel() const
{
    return View::getModel();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onBeginDrag(wxTreeEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onBeginRightDrag(wxTreeEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onEndDrag(wxTreeEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onBeginLabelEdit(wxTreeEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onEndLabelEdit(wxTreeEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onDeleteItem(wxTreeEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onGetInfo(wxTreeEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onSetInfo(wxTreeEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onItemExpanded(wxTreeEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onItemExpanding(wxTreeEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onItemCollapsed(wxTreeEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onItemCollapsing(wxTreeEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onSelectionChanged(wxTreeEvent& _event)
{
//    if (getConnection().get() != nullptr)
//    {
//        auto& source = getConnection()->getSource();
//        auto pWorkspaceModel = dynamic_cast<const WorkspaceModel*>(&source);
//        if (pWorkspaceModel != nullptr)
//        {
//            auto item = _event.GetItem();
////            auto pData = dynamic_cast<WorkspaceItemData*>(m_pTreeCtrl->GetItemData(item));
////            auto pMutableModel = const_cast<WorkspaceModel*>(pWorkspaceModel);
////            pMutableModel->setPropertyByName("focusEntity", pData->getEntity()->getName());
//        }
//    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onSelectionChanging(wxTreeEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onTreeKeyDown(wxTreeEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onItemActivated(wxTreeEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onItemStateClick(wxTreeEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onContextMenu(wxContextMenuEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onItemMenu(wxTreeEvent& _event)
{
    class ActionVisitor
    :   public Helmet::Core::Utility::I_Visitor<Helmet::Core::Event::I_Action>
    {
    public:
        void begin() override
        {
            m_menu.AppendSeparator();
            m_menu.SetClientData(&m_pEntity);
        }

        bool visit(Object_type& _object) override
        {
            auto id = m_parent.NewControlId();
            m_menu.Append(id, _object.getName(), _object.getDescription());
            auto handler = [=] (Object_type* _pObj, wxEvent& _event)
            {
                boost::any context(&_event);
                _pObj->execute(context);
            };

            using namespace std::placeholders;
            auto f = std::bind(handler, &_object, std::placeholders::_1);
            m_menu.Bind(wxEVT_MENU, f, id, wxID_ANY);

            return false;
        }

        void end() override
        {
            m_parent.PopupMenu(&m_menu);
        }

    public:
        ActionVisitor(wxWindow& _parent,
                      const std::string& _name,
                      WorkspaceItemData* _pData)
                :   m_parent(_parent)
                ,   m_menu(_name)
                ,   m_pData(_pData)
                ,   m_pEntity(_pData->getEntity())
        {
        }

    private:
        wxWindow&                   m_parent;
        wxMenu                      m_menu;
        WorkspaceItemData*          m_pData;
        pEntity_type                m_pEntity;
    };

    auto pData = dynamic_cast<WorkspaceItemData*>(m_pTreeCtrl->GetItemData(_event.GetItem()));
    if (pData != nullptr)
    {
        auto pEntity = pData->getEntity();
        {
            ActionVisitor visitor(*GetParent(), pEntity->getName(), pData);
            pEntity->getActions(visitor);
        }

        // TODO Create a deleted state to check for entity
        if (pEntity->getParent().getChildEntityByName(pEntity->getName()).get() == nullptr)
        {
            m_pTreeCtrl->Delete(_event.GetItem());
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onItemRightClick(wxTreeEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onRightMouseDown(wxMouseEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onRightMouseUp(wxMouseEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceView::onRightMouseDoubleClick(wxMouseEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorkspaceView::WorkspaceItemData::WorkspaceItemData(pEntity_type _pEntity)
        :   m_pEntity(_pEntity)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorkspaceView::WorkspaceItemData::~WorkspaceItemData()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorkspaceView::WorkspaceItemData::pEntity_type
WorkspaceView::WorkspaceItemData::getEntity()
{
    return m_pEntity;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TMPAST
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
