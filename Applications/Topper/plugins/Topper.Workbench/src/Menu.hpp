//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Workbench/I_Menu.hpp>
#include <Helmet/Workbench/I_MenuBar.hpp>

#include <wx/menu.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Menu
:   public Helmet::Workbench::I_Menu
,   public wxMenu
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Menu implementation
    /// @{
public:
    const std::string& getTitle() const override;
    void setTitle(const std::string& _title) override;
    void addMenuItem(Helmet::Workbench::I_MenuItem& _menuItem) override;
    void removeMenuItem(Helmet::Workbench::I_MenuItem& _menuItem) override;
    void getMenuItems(I_MenuItemVisitor& _visitor) override;
    /// @}

    /// @name Menu implementation
    /// @{
private:
    /// @}

    /// @name 'Structors
    /// @{
public:
     Menu(const std::string& _title);
    ~Menu() override;
    /// @}

    /// @name Member Variables
    /// @{
private:
    std::string     m_title;
    /// @}

};  // class Menu

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
