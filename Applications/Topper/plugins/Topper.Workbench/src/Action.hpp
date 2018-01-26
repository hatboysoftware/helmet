//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Core/Event/I_Action.hpp>

#include <boost/any.hpp>
#include <boost/function.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Action
:   public Helmet::Core::Event::I_Action
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Action implementation
    /// @{
public:
    const std::string& getName() const override;
    const std::string& getDescription() const override;
    void execute(const boost::any _context) override;
    bool enabled() const override;
    /// @}

    /// @name Action implementation
    /// @{
public:
    void enable(bool _enabled);
    /// @}

    /// @name 'Structors
    /// @{
public:
    Action();
    Action(const std::string& _name,
           const std::string& _description,
           boost::function<void(const boost::any&)> _handler,
           bool _enabled);
    virtual ~Action();
    /// @}

    /// @name Member Variables
    /// @{
private:
    std::string                                 m_name;
    std::string                                 m_description;
    boost::function<void(const boost::any&)>    m_handler;
    bool                                        m_enabled;
    /// @}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
