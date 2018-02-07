//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include "Action.hpp"

#include <Helmet/Core/Event/I_Event.hpp>

#include <Helmet/Workbench/I_Entity.hpp>

#include <boost/function.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/signals2/signal.hpp>

#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Entity
:	public virtual Helmet::Workbench::I_Entity
{
    /// @name Types
    /// @{
public:
    typedef boost::function<pEntity_type(
            const std::string&)>                Functor_type;
    typedef std::map<std::string, Functor_type> HandlerMap_type;
    typedef std::map<std::string, Action>       ActionMap_type;
    typedef std::map<std::string, pEntity_type> pEntityMap_type;
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
    void subscribe(Helmet::Core::Event::I_Observer& _observer) const override;
    void signal(Helmet::Core::Event::I_Event& _event) const override;
    /// @}

    /// @name I_Entity implementation
    /// @{
public:
    const std::string& getType() const override;
    const std::string& getName() const override;
    void setName(const std::string& _name) override;
    void setActive() override;
    void setInactive() override;
    bool isActive() const override;
    void select() override;
    void deselect() override;
    bool isSelected() const override;
    pEntity_type createChildEntity(const std::string& _name) override;
    void addChildEntity(pEntity_type _pEntity) override;
    void removeChildEntity(pEntity_type _pEntity) override;
    const pEntity_type getChildEntityByName(const std::string& _name) const override;
    void getChildEntities(Helmet::Core::Utility::I_Visitor<const pEntity_type>& _visitor) const override;
    const Helmet::Workbench::I_Model& getParentModel() const override;
    I_Entity& getParent() override;
    void save() const override;
    void getActions(Helmet::Core::Utility::I_Visitor<Helmet::Core::Event::I_Action>& _visitor) override;
    /// @}

    /// @name Entity implementation
    /// @{
public:
protected:
    void deleteEntity(boost::any _context);
    void deleteFromTree(pEntity_type _pEntity) const;
    /// @}

    /// @name 'Structors
    /// @{
public:
    Entity(I_Entity& _parent,
           const std::string& _name,
           const std::string& _entityInfoPath);
    virtual ~Entity();
    /// @}

    /// @name Member Variables
    /// @{
private:
    std::string                         m_name;
    static const std::string		    sm_type;
    typedef boost::signals2::signal<
            void(Helmet::Core::Event::I_Event& _type)
    >   Signal_type;

    mutable bool                        m_dirty;
    mutable Signal_type                 m_signal;
    mutable bool                        m_active;
    mutable bool                        m_selected;

    pEntityMap_type                     m_children;

protected:
    I_Entity&                           m_parent;
    ActionMap_type                      m_actions;
    HandlerMap_type                     m_handlers;
    std::string                         m_entityInfoPath;
    bool                                m_createable;
    bool                                m_deleteable;
    bool                                m_hideable;
    /// @}

};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
