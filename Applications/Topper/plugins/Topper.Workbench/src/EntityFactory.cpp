//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "EntityFactory.hpp"

#include "RootEntity.hpp"

#include <Helmet/Core/Exception/Exception.hpp>
#include <Helmet/Core/Utility/Deleter.hpp>
#include <Helmet/Workbench/I_Model.hpp>

#include <boost/assign.hpp>
#include <boost/property_tree/ptree.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
EntityFactory::EntityFactory()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
EntityFactory::~EntityFactory()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
EntityFactory::pEntity_type
EntityFactory::createEntity(Helmet::Workbench::I_Model& _parent,
                            const std::string& _name,
                            const std::string& _entityInfoPath)
{
    auto& entityInfo = (*_parent.getEnvironment())[_entityInfoPath == "." ? _name : _entityInfoPath + "." + _name];
    auto entityType = entityInfo.get_child("type").data();
    if (entityType == "root")
    {
        auto pEntity = pEntity_type(new RootEntity(_parent, _name, _entityInfoPath),
                                    Helmet::Core::Utility::Deleter<
                                            Helmet::Workbench::I_Entity,
                                            Entity>());

        auto children = entityInfo.get_child("children");
        for (auto child: children)
        {
            auto pChild = pEntity->createChildEntity(child.first);
        }

        return pEntity;
    }

    HELMET_EXCEPTION("Initial entity must be of type 'root'");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
