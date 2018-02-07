//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "Notebook.hpp"

#include <Helmet/Core/Exception/Exception.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Notebook::Notebook(wxWindow& _parent, const std::string& _name)
:   wxAuiNotebook(
    &_parent,
    wxID_ANY,
    wxDefaultPosition,
    wxDefaultSize,
    wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxAUI_NB_WINDOWLIST_BUTTON | wxAUI_NB_TAB_MOVE | wxNO_BORDER
)
{
    SetName(_name);
    Bind(wxEVT_AUINOTEBOOK_PAGE_CHANGED, &Notebook::onPageChanged, this, wxID_ANY);
    Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSED, &Notebook::onPageClose, this, wxID_ANY);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Notebook::~Notebook() = default;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Notebook::setName(const std::string& _name)
{
    SetName(_name);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string
Notebook::getName() const
{
    return std::string(GetName().mb_str());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Notebook::addChildFrame(I_Frame& _frame)
{
    HELMET_EXCEPTION("Should never be called.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Notebook::removeChildFrame(I_Frame& _frame)
{
    HELMET_EXCEPTION("Should never be called.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Notebook::getChildFrames(I_FrameVisitor& _visitor)
{
    _visitor.begin();

    for(auto i = 0; i < GetPageCount(); ++i)
    {
        auto pPage = dynamic_cast<NotebookPage*>(GetPage(i));
        if (pPage != nullptr)
        {
            _visitor.visit(*pPage);
        }
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Notebook::show(bool _visible)
{
    Show(_visible);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Notebook::handleEvent(pEvent_type _pEvent)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Helmet::Workbench::I_NotebookPage&
Notebook::addPage(Helmet::Workbench::I_NotebookPage& _page)
{
    auto pPage = dynamic_cast<NotebookPage*>(&_page);
    if (pPage == nullptr)
    {
        HELMET_EXCEPTION("Invalid dynamic cast exception");
    }

    AddPage(pPage,
            //wxString::FromUTF8(pPage->getName().c_str()),
                         pPage->getName(),
                         false);
    return *pPage;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Notebook::showPage(int _index)
{
    if (_index >= 0 && _index < GetPageCount())
    {
        auto pCurrentPage = dynamic_cast<NotebookPage*>(GetCurrentPage());
        if (pCurrentPage != nullptr)
        {
            pCurrentPage->deactivate();
        }

        SetSelection(_index);

        auto pPage = dynamic_cast<NotebookPage*>(GetPage(_index));
        if (pPage != nullptr)
        {
            pPage->activate();
        }
    }

    HELMET_EXCEPTION("Index out of range.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void Notebook::showPage(Helmet::Workbench::I_NotebookPage& _page)
{
    auto pPage = dynamic_cast<NotebookPage*>(&_page);
    if (pPage == nullptr)
    {
        HELMET_EXCEPTION("Invalid dynamic cast exception");
    }

    auto pageIndex = GetPageIndex(pPage);
    if (pageIndex != wxNOT_FOUND)
    {
        showPage(pageIndex);
    }
    else
    {
        auto pageCount = GetPageCount();
        addPage(_page);
        showPage(pageCount);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Notebook::removePage(int _index)
{
    if (_index >= 0 && _index < GetPageCount())
    {
        if (GetPageIndex(GetCurrentPage()) == _index)
        {
            auto pCurrentPage = dynamic_cast<NotebookPage*>(GetCurrentPage());
            if (pCurrentPage != nullptr)
            {
                pCurrentPage->deactivate();
            }
        }

        RemovePage(_index);
        showPage(0);
        return;
    }

    HELMET_EXCEPTION("Index out of range.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Notebook::removePage(Helmet::Workbench::I_NotebookPage& _page)
{
    auto pPage = dynamic_cast<NotebookPage*>(&_page);
    if (pPage == nullptr)
    {
        HELMET_EXCEPTION("Invalid dynamic cast exception");
    }

    auto pageIndex = GetPageIndex(pPage);
    if (pageIndex != wxNOT_FOUND)
    {
        removePage(pageIndex);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Notebook::getPages(Helmet::Core::Utility::I_Visitor<Helmet::Workbench::I_NotebookPage>& _visitor)
{
    _visitor.begin();

    auto exit = false;
    for (auto i = 0;
         !exit && i < GetPageCount();
         ++i)
    {
        auto pPage = dynamic_cast<NotebookPage*>(GetPage(i));
        if (pPage != nullptr)
        {
            exit = _visitor.visit(*pPage);
        }
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void Notebook::onPageChanged(wxAuiNotebookEvent& _event)
{
    auto oldSelection = _event.GetOldSelection();
    auto newSelection = _event.GetSelection();
    if (oldSelection >= 0 && oldSelection < GetPageCount())
    {
        auto pOldPage = dynamic_cast<NotebookPage*>(GetPage(oldSelection));
        if (pOldPage != nullptr)
        {
            pOldPage->deactivate();
        }
    }

    if (newSelection >= 0 && newSelection < GetPageCount())
    {
        auto pNewPage = dynamic_cast<NotebookPage*>(GetPage(newSelection));
        if (pNewPage != nullptr)
        {
            pNewPage->activate();
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void Notebook::onPageClose(wxAuiNotebookEvent& _event)
{
    auto selection = _event.GetSelection();
    auto pPage = dynamic_cast<NotebookPage*>(GetPage(selection));
    if (pPage != nullptr)
    {
        pPage->deactivate();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
