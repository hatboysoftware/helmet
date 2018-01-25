//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include <boost/function.hpp>
#include "MenuItem.hpp"
#include "Menu.hpp"
#include "TopFrame.hpp"
#include "Event.hpp"

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
MenuItem::MenuItem(int _itemId,
                   const std::string& _name,
                   const std::string& _help,
                   bool _isCheckable,
                   Menu* _pMenu)
:   wxMenuItem(
        nullptr,
        _itemId,
        wxString::FromUTF8(_name.c_str()),
        wxString::FromUTF8(_name.c_str()),
        _isCheckable ? wxITEM_CHECK : wxITEM_NORMAL,
        _pMenu
    )
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
MenuItem::~MenuItem()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
MenuItem::bind(Helmet::Workbench::I_Frame& _target, const std::string& _eventId) const
{
    auto pParent = dynamic_cast<TopFrame*>(&_target);
    if (pParent != nullptr)
    {
        typedef boost::shared_ptr<Helmet::Workbench::I_Event>  pEvent_type;
        boost::function<void(wxEvent&)> handler(
                boost::bind(&Helmet::Workbench::I_Frame::handleEvent,
                            &_target,
                            pEvent_type(new Event(_eventId))
                )
        );

        pParent->Bind(wxEVT_COMMAND_MENU_SELECTED, handler, GetId());
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Helmet::Workbench::I_Menu*
MenuItem::getSubmenu()
{
    return dynamic_cast<Menu*>(GetMenu());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
