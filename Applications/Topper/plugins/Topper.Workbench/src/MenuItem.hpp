//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Workbench/I_MenuItem.hpp>

#include <wx/menu.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
    namespace Workbench {
        class I_Menu;
    }   // namespace Workbench
}   // namespace Helmet
namespace Topper {
class Menu;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class MenuItem
:   public Helmet::Workbench::I_MenuItem
,   public wxMenuItem
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_MenuBar implementation
    /// @{
public:
    void bind(Helmet::Workbench::I_Frame& _target, const std::string& _eventId) const override;
    Helmet::Workbench::I_Menu* getSubmenu() override;
    /// @}

    /// @name Menu implementation
    /// @{
private:
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit MenuItem(int _itemId,
                      const std::string& _name,
                      const std::string& _help,
                      bool _isCheckable,
                      Menu* _pMenu);
    virtual ~MenuItem();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// @}

};  // class MenuItem

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
