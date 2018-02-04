//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Core/Configuration.hpp>

#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Plugin {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class HELMET_CORE_DLL_LINK I_Environment
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<I_Environment>    pEnvironment_type;
    /// @}

    /// @name I_Environment interface
    /// @{
public:
    /// Loads the environment configuration from a file path.
    /// @param[in] _path Configuration file path.
    /// @returns True if successful, False otherwise.
    virtual bool loadConfiguration(const boost::filesystem::path& _path) = 0;

    /// Loads the environment configuratoin from an input stream.
    /// @param[in] _input Configuration input stream.
    /// @returns True if successful, False otherwise.
    virtual bool loadConfiguration(std::istream& _input) = 0;

    /// Saves the configuration to a file path.
    /// @param[in] _path Session info file path.
    /// @returns True if successful, False otherwise.
    virtual bool saveConfiguration(const boost::filesystem::path& _path) = 0;

    /// Saves the configuration to an output stream.
    /// @param[in] _output Session info output stream.
    /// @returns True if successful, False otherwise.
    virtual bool saveConfiguration(std::ostream& _output) = 0;

    /// Writes configuration field to environment.
    /// @param[in] _key Key to session field.
    /// @param[in] _value Value of sessoin field.
    virtual void writeConfigurationField(const std::string& _key, const std::string& _value) = 0;

    /// Loads the session info from a file path.
    /// @param[in] _path Session info file path.
    /// @returns True if successful, False otherwise.
    virtual bool loadSessionInfo(const boost::filesystem::path& _path) = 0;

    /// Loads the session info from an input stream.
    /// @param[in] _input Session info input stream.
    /// @returns True if successful, False otherwise.
    virtual bool loadSessionInfo(std::istream& _input) = 0;

    /// Saves the session info to a file path.
    /// @param[in] _path Session info file path.
    /// @returns True if successful, False otherwise.
    virtual bool saveSessionInfo(const boost::filesystem::path& _path) = 0;

    /// Saves the session info to an output stream.
    /// @param[in] _output Session info output stream.
    /// @returns True if successful, False otherwise.
    virtual bool saveSessionInfo(std::ostream& _output) = 0;

    /// Writes session info field to environment.
    /// @param[in] _key Key to session field.
    /// @param[in] _value Value of sessoin field.
    virtual void writeSessionInfoField(const std::string& _key, const std::string& _value) = 0;

    /// Gets an environment variable (configuration and session)
    virtual boost::property_tree::ptree& operator[](const std::string& _key) = 0;

    /// Gets environment configuration
    virtual boost::property_tree::ptree& getConfiguration() = 0;

    /// Gets environment session
    virtual boost::property_tree::ptree& getSession() = 0;
    /// @}

    /// @name Static Methods
    /// @{
public:
    static pEnvironment_type create();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Environment();
    virtual ~I_Environment();
    /// @}

};  // interface I_Environment

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugin
}   // namespace Core
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
