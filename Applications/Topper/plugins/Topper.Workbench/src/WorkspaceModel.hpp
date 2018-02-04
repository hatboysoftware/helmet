//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include "Model.hpp"

#include <Helmet/Core/Utility/I_Visitor.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class WorkspaceModel
:   public Model
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<Helmet::Workbench::I_Model>   pModel_type;
    /// @}

    /// @name I_Model implementation
    /// @{
public:
    const std::string& getType() const override;
    const std::string& getName() const override;
    void setName(const std::string& _name) override;
    /// @}

    /// @name WorkspaceModel implementation
    /// @{
public:
    const boost::filesystem::path& getPath() const;
    void setPath(const boost::filesystem::path& _path);
    bool createWorkspace(const boost::filesystem::path& _workspacePath);
    bool loadWorkspace(const boost::filesystem::path& _workspacePath);
    bool saveWorkspace(const boost::filesystem::path& _workspacePath);
    pModel_type getSubModel(const std::string& _name) const;
    void getSubModels(Helmet::Core::Utility::I_Visitor<pModel_type>& _visitor);
private:
    bool loadModels();
    bool saveModels();
    /// @}

    /// @name 'Structors
    /// @{
public:
             WorkspaceModel(pEnvironment_type _pEnvironment);
    virtual ~WorkspaceModel();
    /// @}

    /// @name Member Variables
    /// @{
private:
    boost::filesystem::path             m_path;
    std::map<std::string, pModel_type>  m_subModels;
    /// @}

};  // class WorkspaceModel

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
