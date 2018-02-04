//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Workbench/I_MenuBar.hpp>

#include <wx/menu.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
    namespace Workbench {
        class I_Frame;
    }   // namespace Workbench
}   // namespace Helmet
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class MenuBar
:   public Helmet::Workbench::I_MenuBar
,   public wxMenuBar
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_MenuBar implementation
    /// @{
public:
    /// @}

    /// @name MenuBar implementation
    /// @{
private:
    void addMenu(Helmet::Workbench::I_Menu& _menu) override;
    void removeMenu(Helmet::Workbench::I_Menu& _menu) override;
    void getMenus(I_MenuVisitor& _visitor) override;
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit MenuBar();
    ~MenuBar() override;
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// @}

};  // class MenuBar

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
