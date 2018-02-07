//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include "View.hpp"
#include "NotebookPage.hpp"

#include <wx/sizer.h>
#include <wx/textctrl.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class LogView
:   public View
,   public NotebookPage
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Observer implementation
    /// @{
public:
    void update(Helmet::Core::Event::I_Event& _event) override;
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

    /// @name LogView implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
    LogView(Notebook& _parent,
            const std::string& _name);
    virtual ~LogView();
    /// @}

    /// @name Member Variables
    /// @{
private:
    wxSizer*            m_pSizer;
    wxTextCtrl*         m_pTextCtrl;
    /// @}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
