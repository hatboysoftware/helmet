//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Workbench/I_StatusBar.hpp>

#include <wx/statusbr.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
    namespace Workbench {
        class I_Frame;
    }   // namespace Workbench
}   // namespace Helmet
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class StatusBar
:   public Helmet::Workbench::I_StatusBar
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_StatusBar implementation
    /// @{
public:
    /// @}

    /// @name StatusBar implementation
    /// @{
public:
    wxStatusBar* getConcreteStatusBar();
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit StatusBar(wxStatusBar* _pStatusBar);
    ~StatusBar() override;
    /// @}

    /// @name Member Variables
    /// @{
private:
    wxStatusBar*    m_pStatusBar;
    /// @}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
