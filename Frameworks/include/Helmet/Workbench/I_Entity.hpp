//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Workbench/Configuration.hpp>

#include <Helmet/Core/Utility/I_Visitor.hpp>
#include <Helmet/Core/Event/I_Observable.hpp>
#include <Helmet/Core/Event/I_Action.hpp>

#include <boost/function.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Model;
class I_Action;

class HELMET_WORKBENCH_DLL_LINK I_Entity
:   public Helmet::Core::Event::I_Observable
,   public boost::enable_shared_from_this<I_Entity>
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<I_Entity>     pEntity_type;
    /// @}

    /// @name I_Entity interface
    /// @{
public:
    virtual const std::string& getType() const = 0;
    virtual const std::string& getName() const = 0;
    virtual void setName(const std::string& _name) = 0;
    virtual void setActive() = 0;
    virtual void setInactive() = 0;
    virtual bool isActive() const = 0;
    virtual void select() = 0;
    virtual void deselect() = 0;
    virtual bool isSelected() const = 0;
    virtual pEntity_type createChildEntity(const std::string& _name) = 0;
    virtual void addChildEntity(pEntity_type _pEntity) = 0;
    virtual void removeChildEntity(pEntity_type _pEntity) = 0;
    virtual const pEntity_type getChildEntityByName(const std::string& _name) const = 0;
    virtual void getChildEntities(Core::Utility::I_Visitor<const pEntity_type>& _visitor) const = 0;
    virtual const I_Model& getParentModel() const = 0;
    virtual I_Entity& getParent() = 0;
    virtual bool isVisible() = 0;
    virtual void save() const = 0;
    virtual void getActions(Core::Utility::I_Visitor<Core::Event::I_Action>& _visitor) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Entity();
    virtual ~I_Entity();
    /// @}

};	// interface I_Entity

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Workbench
}	// namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
