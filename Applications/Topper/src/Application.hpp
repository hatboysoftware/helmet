//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <wx/app.h>

#include <boost/locale.hpp>
#include <Helmet/Workbench/I_Workbench.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
    namespace Blockchain {
        class I_BlockchainNode;
    }   // namespace Blockchain
    namespace Enterprise {
        class I_ApplicationServer;
        class I_ProtocolService;
    }   // namespace Enterprise
    namespace Workbench {
        class I_Workbench;
    }   // namespace Workbench
}   // namespace Helmet
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
    boost::locale::generator                    m_localeGenerator;
    std::locale                                 m_locale;

    Helmet::Enterprise::I_ApplicationServer&    m_applicationServer;

    typedef boost::shared_ptr<Helmet::Blockchain::I_BlockchainNode> pBlockchainNode_type;
    pBlockchainNode_type                        m_pBlockchainNode;

    typedef boost::shared_ptr<Helmet::Enterprise::I_ProtocolService> pProtocolService_type;
    pProtocolService_type                       m_pProtocolService;

    typedef boost::shared_ptr<Helmet::Workbench::I_Workbench>       pWorkbench_type;
    pWorkbench_type                             m_pWorkbench;
    /// @}

};	// class Application

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
