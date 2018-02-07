//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Core/Event/I_Event.hpp>

#include <Helmet/Workbench/I_Model.hpp>

#include <boost/signals2/signal.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Model
:   public virtual Helmet::Workbench::I_Model
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Cleanable implementation
    /// @{
public:
    bool isDirty() const override;
    void setDirty() const override;
    void clean() const override;
    /// @}

    /// @name I_Observable implementation
    /// @{
public:
    const std::string& getName() const override;
    void subscribe(Helmet::Core::Event::I_Observer& _observer) const override;
    void signal(Helmet::Core::Event::I_Event& _event) const override;
    /// @}

    /// @name I_Model implementation
    /// @{
public:
    void setName(const std::string& _name) override;
    pEnvironment_type getEnvironment() const override;
    /// @}

    /// @name Model implementation
    /// @{
public:
    /// @}

    /// @name Internal Structures
    /// @{
public:
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class Event
    :   public Helmet::Core::Event::I_Event
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name I_Event implementation
        /// @{
    public:
        const std::string& getType() const override;
        const I_Observable& getSource() const override;
        /// @}

        /// @name Event implementation
        /// @{
    public:
        /// @}

        /// @name 'Structors
        /// @{
    public:
        explicit Event(const I_Observable& _source);
        virtual ~Event();
        /// @}

        /// @name Member Variables
        /// @{
    private:
        const I_Observable& m_source;
        /// @}
    };
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    /// @name 'Structors
    /// @{
public:
    Model(const std::string& _name,
          pEnvironment_type _pEnvironment,
          const std::string& _modelInfoPath);
    virtual ~Model();
    /// @}

    /// @name Member Variables
    /// @{
private:
    typedef boost::signals2::signal<
            void(Helmet::Core::Event::I_Event& _type)
    >   Signal_type;

    std::string                     m_name;
    mutable Signal_type             m_signal;
    mutable bool                    m_dirty;
    pEnvironment_type               m_pEnvironment;
protected:
    std::string                     m_modelInfoPath;
    /// @}

};  // class Model

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
