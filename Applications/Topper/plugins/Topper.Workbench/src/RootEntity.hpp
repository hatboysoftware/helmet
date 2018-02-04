//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include "Entity.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class RootEntity
:	public Entity
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Cleanable implementation
    /// @{
public:
    void setDirty() const override;
    /// @}

    /// @name Entity implementation
    /// @{
public:
    const std::string& getType() const override;
    const I_Entity& getRoot() const;
    const Helmet::Workbench::I_Model& getParentModel() const override;
    bool isVisible() override;
    /// @}

    /// @name RootEntity implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
    RootEntity(Helmet::Workbench::I_Model& _parent,
               const std::string& _name,
               const std::string& _entityInfo);
    virtual ~RootEntity();
    /// @}

    /// @name Member Variables
    /// @{
private:
    static const std::string    sm_type;
    Helmet::Workbench::I_Model& m_parent;
    /// @}

};	// class RootEntity

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
