//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include "Frame.hpp"

#include <Helmet/Workbench/I_NotebookPage.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Notebook;

class NotebookPage
:   public Helmet::Workbench::I_NotebookPage
,   public Frame
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_NotebookPage implementation
    /// @{
public:
    void activate() override;
    void deactivate() override;
    Helmet::Workbench::I_Notebook& getNotebook() override;
    /// @}

    /// @name NotebookPage implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
    explicit NotebookPage(const std::string& _name,
                          Notebook& _parent);
    ~NotebookPage() override;
    /// @}

    /// @name Member Variables
    /// @{
private:
    bool        m_active;
    Notebook&   m_notebook;
    /// @}

};  // class NotebookPage

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
