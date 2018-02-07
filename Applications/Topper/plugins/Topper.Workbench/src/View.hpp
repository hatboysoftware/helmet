//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Workbench/I_View.hpp>

#include <boost/function.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class View
:   public Helmet::Workbench::I_View
{
    /// @name Types
    /// @{
public:
    typedef boost::function<void(Helmet::Core::Event::I_Event&)>  EventHandler_type;
    /// @}

    /// @name I_Observer implementation
    /// @{
public:
    const std::string& getName() const override;
    void update(Helmet::Core::Event::I_Event& _event) override;
    pConnection_type getConnection() const override;
    void setConnection(pConnection_type _pConnection) override;
    /// @}

    /// @name I_View implementation
    /// @{
public:
    const Helmet::Workbench::I_Model* getModel() const override;
    /// @}

    /// @name View implementation
    /// @{
public:
    virtual void modelUpdate(Helmet::Core::Event::I_Event& _event);
    void registerEventHandler(const std::string& _type,
                              EventHandler_type _handler);
    void unregisterEventHandler(const std::string& _type);
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit View(const std::string& _name);
    virtual ~View();
    /// @}

    /// @name Member Variables
    /// @{
private:
    std::string                                 m_name;
    pConnection_type                            m_pConnection;
    std::map<std::string, EventHandler_type>    m_eventHandlers;
    /// @}

};  // class View

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
