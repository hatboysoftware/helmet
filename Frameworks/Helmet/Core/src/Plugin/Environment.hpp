//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Core/Plugin/I_Environment.hpp>

#include <boost/property_tree/ptree.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Plugin {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Environment
:   public I_Environment
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name Environment implementation
    /// @{
public:
    bool loadConfiguration(const boost::filesystem::path& _path) override;
    bool loadConfiguration(std::istream& _input) override;
    bool saveConfiguration(const boost::filesystem::path& _path) override;
    bool saveConfiguration(std::ostream& _output) override;
    void writeConfigurationField(const std::string& _key, const std::string& _value) override;
    bool loadSessionInfo(const boost::filesystem::path& _path) override;
    bool loadSessionInfo(std::istream& _input) override;
    bool saveSessionInfo(const boost::filesystem::path& _path) override;
    bool saveSessionInfo(std::ostream& _output) override;
    void writeSessionInfoField(const std::string& _key, const std::string& _value) override;
    boost::property_tree::ptree& operator[](const std::string& _key) override;
    boost::property_tree::ptree& getConfiguration() override;
    boost::property_tree::ptree& getSession() override;
    /// @}

    /// @name 'Structors
    /// @{
public:
             Environment();
    virtual ~Environment();
    /// @}

    /// @name Member Variables
    /// @{
private:
    boost::property_tree::ptree     m_configuration;
    boost::property_tree::ptree     m_sessionInfo;
    /// @}

};  // class Environment

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugin
}   // namespace Core
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
