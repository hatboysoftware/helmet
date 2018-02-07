//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Workbench/Configuration.hpp>

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Workbench {
class I_MenuItem;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class HELMET_WORKBENCH_DLL_LINK I_Menu
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name Internal structures
    /// @{
public:
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    struct HELMET_WORKBENCH_DLL_LINK I_MenuItemVisitor
    {
        /// @name I_PageVisitor interface
        /// @{
    public:
        virtual void begin() = 0;
        virtual void visit(I_MenuItem& _frame) = 0;
        virtual void end() = 0;
        /// @}

        /// @name 'Structors
        /// @{
    protected:
                 I_MenuItemVisitor();
        virtual ~I_MenuItemVisitor();
        /// @}

    };  // interface I_MenuItemVisitor
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    /// @name I_Menu interface
    /// @{
public:
    virtual const std::string& getTitle() const = 0;
    virtual void setTitle(const std::string& _title) = 0;
    virtual void addMenuItem(I_MenuItem& _menuItem) = 0;
    virtual void removeMenuItem(I_MenuItem& _menuItem) = 0;
    virtual void getMenuItems(I_MenuItemVisitor& _visitor) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Menu();
    virtual ~I_Menu();
    /// @}

};  // interface I_Menu

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
