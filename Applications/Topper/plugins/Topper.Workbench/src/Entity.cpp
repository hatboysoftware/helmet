//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "Entity.hpp"
#include "WorkspaceView.hpp"
#include "Connection.hpp"
#include "Model.hpp"

#include <Helmet/Core/Utility/Deleter.hpp>
#include <Helmet/Core/Exception/NotImplementedException.hpp>

#include <boost/assign/list_of.hpp>

#include <wx/artprov.h>
#include <wx/event.h>

#include <boost/log/trivial.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string Entity::sm_type = "entity";

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void Entity::deleteEntity(boost::any _context)
{
    auto& parent = getParent();
    if (&parent == this)
    {
        HELMET_EXCEPTION("Root entity cannot be deleted");
    }

    dynamic_cast<Entity&>(parent).deleteFromTree(shared_from_this());
    parent.removeChildEntity(shared_from_this());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void Entity::deleteFromTree(pEntity_type _pEntity) const
{
    auto& environment = (*getParentModel().getEnvironment());
    auto& entityInfo = environment[m_entityInfoPath + ".children"];
    entityInfo.erase(_pEntity->getName());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Entity::Entity(I_Entity& _parent,
               const std::string& _name,
               const std::string& _entityInfoPath)
        :   m_name(_name)
        ,   m_dirty(true)
        ,   m_signal()
        ,   m_active(false)
        ,   m_selected(false)
        ,   m_children()
        ,   m_parent(_parent)
        ,   m_actions()
        ,   m_handlers()
        ,   m_entityInfoPath(_entityInfoPath == "." ? _name : _entityInfoPath + ".children." + _name)
        ,   m_deleteable(false)
{
    m_actions["delete"] = Action("Delete", "Delete entity from the scene", boost::bind(&Entity::deleteEntity, this, _1), false);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Entity::~Entity()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Entity::isDirty() const
{
    return m_dirty;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Entity::setDirty() const
{
    m_dirty = true;
    clean();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Entity::clean() const
{
    for (auto child : m_children)
    {
        child.second->clean();
    }

    if (m_dirty)
    {
        auto evt = Model::Event(*this);
        signal(evt);
        getParentModel().signal(evt);

        m_dirty = false;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Entity::subscribe(Helmet::Core::Event::I_Observer& _observer) const
{
    typedef boost::signals2::signal<void(Helmet::Core::Event::I_Event&)> Signal_type;

    Signal_type::slot_type slot(&Helmet::Core::Event::I_Observer::update, &_observer, _1);
    auto connection = m_signal.connect(slot);

    Helmet::Core::Event::I_Observer::pConnection_type pConnection(
            new Connection(connection, *this),
            Helmet::Core::Utility::Deleter<
                    Helmet::Core::Event::I_Connection,
                    Connection>());

    _observer.setConnection(pConnection);

    BOOST_LOG_TRIVIAL(trace) << "[" << getParentModel().getName() << ":" << getName() << "] : Subscribing [" << _observer.getName() << "]";
    auto evt = Model::Event(*this);
    _observer.update(evt);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Entity::signal(Helmet::Core::Event::I_Event& _event) const
{
    BOOST_LOG_TRIVIAL(trace) << "[" << getParentModel().getName() << ":" << getName() << "] : Issuing "<< _event.getType() << " update event.";
    m_signal(_event);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Entity::getType() const
{
    return sm_type;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Entity::getName() const
{
    return m_name;
    //return m_rootProperty.getValue();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Entity::setName(const std::string& _name)
{
//    m_rootProperty.setValue(_name);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void Entity::setActive()
{
    m_active = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void Entity::setInactive()
{
    m_active = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool Entity::isActive() const
{
    return m_active;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void Entity::select()
{
    m_selected = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void Entity::deselect()
{
    m_selected = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool Entity::isSelected() const
{
    return m_selected;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Helmet::Workbench::I_Entity::pEntity_type
Entity::createChildEntity(const std::string& _name)
{
    auto& environment = (*getParentModel().getEnvironment());
    auto path = m_entityInfoPath + ".children." + _name;
    auto& entityInfo = environment[path];

    auto entityType = environment[path + "." + "type"].data();
    auto iter = m_handlers.find(entityType);
    if (iter != m_handlers.end())
    {
        auto pEntity = iter->second(_name);
//        auto properties = environment[path + ".properties"];
//        for (auto entityProperty : properties)
//        {
//            pEntity->registerProperty(entityProperty.first);
//        }
//
//        pEntity->setDirty();

        auto children = environment[path + ".children"];
        for (auto child : children)
        {
            auto pChild = pEntity->createChildEntity(child.first);
        }

        pEntity->setDirty();
        addChildEntity(pEntity);
        return pEntity;
    }

    HELMET_EXCEPTION("Child entity type not supported by current entity.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Entity::addChildEntity(pEntity_type _pEntity)
{
    if (_pEntity.get() == nullptr)
    {
        HELMET_EXCEPTION("_pEntity cannot be null");
    }

    if (m_children.find(_pEntity->getName()) != m_children.end())
    {
        HELMET_EXCEPTION("_pEntity is already a child of this entity");
    }

    m_children[_pEntity->getName()] = _pEntity;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Entity::removeChildEntity(pEntity_type _pEntity)
{
    if (_pEntity.get() == nullptr)
    {
        HELMET_EXCEPTION("_pEntity cannot be null");
    }

    auto iterator = m_children.find(_pEntity->getName());
    if (iterator != m_children.end())
    {
        m_children.erase(iterator);
    }
    else
    {
        HELMET_EXCEPTION("_pEntity is not an existing child of this entity");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Helmet::Workbench::I_Entity::pEntity_type
Entity::getChildEntityByName(const std::string& _name) const
{
    auto iterator = m_children.find(_name);
    if (iterator == m_children.end())
    {
        for (auto child : m_children)
        {
            auto pEntity = child.second->getChildEntityByName(_name);
            if (pEntity.get() != nullptr)
            {
                return pEntity;
            }
        }

        return pEntity_type(nullptr);
    }

    return iterator->second;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Entity::getChildEntities(Helmet::Core::Utility::I_Visitor<const pEntity_type>& _visitor) const
{
    _visitor.begin();
    auto exit = false;
    for (auto iter = m_children.cbegin(); !exit && iter != m_children.cend(); ++iter)
    {
        exit = _visitor.visit(iter->second);
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Helmet::Workbench::I_Model&
Entity::getParentModel() const
{
    HELMET_NOT_IMPLEMENTED_EXCEPTION();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Helmet::Workbench::I_Entity&
Entity::getParent()
{
    return m_parent;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void Entity::save() const
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Entity::getActions(Helmet::Core::Utility::I_Visitor<Helmet::Core::Event::I_Action>& _visitor)
{
    _visitor.begin();

    auto exit = false;
    for (auto iter = m_actions.begin();
         !exit && iter != m_actions.end();
         ++iter)
    {
        exit = _visitor.visit(iter->second);
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
