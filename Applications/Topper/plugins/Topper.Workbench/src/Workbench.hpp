//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Workbench/I_Workbench.hpp>

#include <boost/enable_shared_from_this.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
class TopFrame;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Workbench
:   public Helmet::Workbench::I_Workbench
,	public boost::enable_shared_from_this<Workbench>
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Workbench implementation
    /// @{
public:
    virtual Helmet::Workbench::I_Frame& getMainFrame();
    /// @}

    /// @name Workbench implementation
    /// @{
public:
    void initialize();
    /// @}

    /// @name Static Methods
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit Workbench(pEnvironment_type _pEnvironment);
    virtual ~Workbench();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pEnvironment_type                       m_pEnvironment;
    pEnvironment_type                       m_pLocalEnvironment;
    TopFrame*                               m_pTopFrame;
    /// @}

};  // class Workbench

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
