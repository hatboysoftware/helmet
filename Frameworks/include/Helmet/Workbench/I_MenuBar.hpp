//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Workbench/Configuration.hpp>

#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Workbench {
class I_Menu;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class HELMET_WORKBENCH_DLL_LINK I_MenuBar
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
    struct HELMET_WORKBENCH_DLL_LINK I_MenuVisitor
    {
        /// @name I_MenuVisitor interface
        /// @{
    public:
        virtual void begin() = 0;
        virtual void visit(I_Menu& _menu) = 0;
        virtual void end() = 0;
        /// @}

        /// @name 'Structors
        /// @{
    protected:
                 I_MenuVisitor();
        virtual ~I_MenuVisitor();
        /// @}

    };  // interface I_MenuVisitor
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    /// @name I_MenuBar interface
    /// @{
public:
    virtual void addMenu(I_Menu& _menu) = 0;
    virtual void removeMenu(I_Menu& _menu) = 0;
    virtual void getMenus(I_MenuVisitor& _visitor) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_MenuBar();
    virtual ~I_MenuBar();
    /// @}

};  // interface I_MenuBar

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
