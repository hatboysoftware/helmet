//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <wx/app.h>

#include <boost/locale.hpp>
#include <Helmet/Workbench/I_Workbench.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Top level wxWidgets Application object.
class Application
:	public wxApp
{
    /// @name wxApp overrides
    /// @{
public:
    virtual bool OnInit() override;
    virtual int OnExit() override;
    virtual int OnRun() override;
    virtual bool OnExceptionInMainLoop() override;
    /// @}

    /// @name Application implementation
    /// @{
protected:
    /// @}

    /// @name 'Structors
    /// @{
public:
             Application();
    virtual ~Application();
    /// @}

    /// @name Member Variables
    /// @{
private:
    boost::locale::generator    m_localeGenerator;
    std::locale                 m_locale;
    typedef boost::shared_ptr<Helmet::Workbench::I_Workbench>   pWorkbench_type;
    pWorkbench_type             m_pWorkbench;
    /// @}

};	// class Application

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
