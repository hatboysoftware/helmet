//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "WorkspaceModel.hpp"
#include "LogModel.hpp"
#include "EntityFactory.hpp"

#include <Helmet/Core/Utility/Deleter.hpp>
#include <Helmet/Core/Utility/Utility.hpp>

#include <boost/log/sources/logger.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorkspaceModel::WorkspaceModel(pEnvironment_type _pEnvironment)
:   Model("Empty Workspace", _pEnvironment, "")
,   m_subModels()
{
    m_subModels["console"] = pModel_type(new LogModel(std::cout, _pEnvironment),
                                         Helmet::Core::Utility::Deleter<
                                                 I_Model,
                                                 LogModel>());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorkspaceModel::~WorkspaceModel()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string& WorkspaceModel::getType() const
{
    static std::string sm_type("workspace");

    return sm_type;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string& WorkspaceModel::getName() const
{
    return Model::getName();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void WorkspaceModel::setName(const std::string& _name)
{
    Model::setName(_name);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const boost::filesystem::path&
WorkspaceModel::getPath() const
{
    return m_path;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorkspaceModel::setPath(const boost::filesystem::path& _path)
{
    m_path = _path;
    setDirty();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
WorkspaceModel::createWorkspace(const boost::filesystem::path& _workspacePath)
{
    auto workspaceTemplatesPath = (boost::filesystem::current_path() / "plugins/Topper.Workbench/template/workspace").normalize();
    if (boost::filesystem::exists(workspaceTemplatesPath) &&
        !boost::filesystem::exists(_workspacePath))
    {
        Helmet::Core::Utility::copyDir(workspaceTemplatesPath, _workspacePath);
        return true;
    }

    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
WorkspaceModel::loadWorkspace(const boost::filesystem::path& _workspacePath)
{
    auto workspaceDefinitionPath = _workspacePath / "workspace.json";

    if (getEnvironment()->loadConfiguration(workspaceDefinitionPath))
    {
        getEnvironment()->writeSessionInfoField("workspacePath", _workspacePath.string());
        loadWorkspace(_workspacePath);
        setDirty();
        return true;
    }

    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
WorkspaceModel::saveWorkspace(const boost::filesystem::path& _workspacePath)
{
    clean();
    auto workspaceDefinitionPath = _workspacePath / "workspace.json";
    auto workspaceScenePath = _workspacePath / "entities.json";
    auto workspacePropertiesPath = _workspacePath / "properties.json";

//    bool success = true;
//    for (auto model : m_subModels)
//    {
////        success &= model.second->save();
//    }

    return getEnvironment()->saveConfiguration(workspaceDefinitionPath);
    //m_pPropertiesEnvironment->saveConfiguration(workspacePropertiesPath);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorkspaceModel::pModel_type
WorkspaceModel::getSubModel(const std::string& _name) const
{
    auto iter = m_subModels.find(_name);
    if (iter != m_subModels.end())
    {
        return iter->second;
    }

    return pModel_type(nullptr);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorkspaceModel::getSubModels(Helmet::Core::Utility::I_Visitor<pModel_type>& _visitor)
{
    _visitor.begin();

    auto exit = false;
    for (auto iter = m_subModels.begin(); !exit && iter != m_subModels.end(); ++iter)
    {
        exit = _visitor.visit(iter->second);
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
