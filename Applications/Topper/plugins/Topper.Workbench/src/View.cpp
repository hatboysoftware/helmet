//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "View.hpp"

#include <Helmet/Core/Event/I_Connection.hpp>
#include <Helmet/Core/Event/I_Event.hpp>
#include <Helmet/Core/Event/I_Observable.hpp>

#include <Helmet/Workbench/I_Model.hpp>
#include <Helmet/Core/Exception/Exception.hpp>

#include <boost/log/trivial.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
View::View(const std::string& _name)
        :   m_name(_name)
{
    registerEventHandler("model", boost::bind(&View::modelUpdate, this, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
View::~View()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
View::getName() const
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void View::update(Helmet::Core::Event::I_Event& _event)
{
    BOOST_LOG_TRIVIAL(trace) << "Handling update event : [" << getName() << ":" << _event.getSource().getName() << ":" << _event.getType() << "]";
    auto handler = m_eventHandlers.find(_event.getType());
    if (handler != m_eventHandlers.end())
    {
        m_eventHandlers[_event.getType()](_event);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Helmet::Core::Event::I_Observer::pConnection_type
View::getConnection() const
{
    return m_pConnection;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void View::setConnection(pConnection_type _pConnection)
{
    m_pConnection = _pConnection;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Helmet::Workbench::I_Model*
View::getModel() const
{
    if (getConnection().get() != nullptr)
    {
        auto& source = getConnection()->getSource();
        return dynamic_cast<const Helmet::Workbench::I_Model*>(&source);
    }

    return nullptr;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
View::modelUpdate(Helmet::Core::Event::I_Event& _event)
{
    // TODO Debug log.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
View::registerEventHandler(const std::string& _type, EventHandler_type _handler)
{
    auto handler = m_eventHandlers.find(_type);
    if (handler == m_eventHandlers.end())
    {
        m_eventHandlers[_type] = _handler;
        return;
    }

    // TODO Boost log warning.
    HELMET_EXCEPTION("Handler already registered with view");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
View::unregisterEventHandler(const std::string& _type)
{
    auto handler = m_eventHandlers.find(_type);
    if (handler != m_eventHandlers.end())
    {
        m_eventHandlers.erase(handler);
        return;
    }

    // TODO Boost log warning.
    HELMET_EXCEPTION("Handler is not registered with view");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
