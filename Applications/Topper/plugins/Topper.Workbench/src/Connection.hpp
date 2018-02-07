//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Core/Event/I_Connection.hpp>
#include <Helmet/Core/Event/I_Observer.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Connection
:   public Helmet::Core::Event::I_Connection
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Resource<T> implementation
    /// @{
public:
    const Helmet::Core::Utility::I_Handle<boost::signals2::connection>& getHandle() const override;
    /// @}

    /// @name I_Connection implementation
    /// @{
public:
    const Helmet::Core::Event::I_Observable& getSource() const override;
    /// @}

    /// @name Internal Structures
    /// @{
public:
    class Handle
    :   public Helmet::Core::Utility::I_Handle<boost::signals2::connection>
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name I_Handle<T> implementation
        /// @{
    public:
        const boost::signals2::connection& getValue() const override;
        /// @}

        /// @name 'Structors
        /// @{
    public:
        explicit Handle(boost::signals2::connection& _connection);
        virtual ~Handle();
        /// @}

        /// @name Member Variables
        /// @{
    private:
        boost::signals2::connection     m_value;
        /// @}

    };  // class Handle
    /// @}

    /// @name 'Structors
    /// @{
public:
    Connection(boost::signals2::connection& _connection,
               const Helmet::Core::Event::I_Observable& _source);
    virtual ~Connection();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Handle                                      m_handle;
    const Helmet::Core::Event::I_Observable&    m_source;
    /// @}

};  // class Connection

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
