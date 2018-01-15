//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "Environment.hpp"

#include <boost/log/trivial.hpp>
#include <boost/property_tree/json_parser.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Plugin {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Environment::Environment()
:   m_configuration()
,   m_sessionInfo()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Environment::~Environment()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Environment::loadConfiguration(const boost::filesystem::path& _path)
{
    if (boost::filesystem::exists(_path))
    {
        std::ifstream fileStream;
        fileStream.open(_path.string(), std::ios_base::in | std::ios_base::binary);
        if (loadConfiguration(fileStream))
        {
            BOOST_LOG_TRIVIAL(info) << "Loaded configuration from " << _path;
            return true;
        }
    }

    BOOST_LOG_TRIVIAL(error) << "Configuration file " << _path << " does not exist.";
    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Environment::loadConfiguration(std::istream& _input)
{
    try
    {
        std::stringstream json;
        json << _input.rdbuf();
        read_json(json, m_configuration);
        return true;
    }
    catch (std::exception& _ex)
    {
        BOOST_LOG_TRIVIAL(error) << "Environment::loadConfiguration() : " << _ex.what();
    }

    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Environment::saveConfiguration(const boost::filesystem::path& _path)
{
    std::ofstream fileStream;
    fileStream.open(_path.string(), std::ios_base::out | std::ios_base::trunc);
    if (saveConfiguration(fileStream))
    {
        BOOST_LOG_TRIVIAL(info) << "Saved configuration to " << _path;
        return true;
    }

    BOOST_LOG_TRIVIAL(error) << "Error saving configuration to " << _path;
    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Environment::saveConfiguration(std::ostream& _output)
{
    try
    {
        write_json(_output, m_configuration);
        return true;
    }
    catch (std::exception& _ex)
    {
        BOOST_LOG_TRIVIAL(error) << "Environment::saveConfiguration() : " << _ex.what();
    }

    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Environment::writeConfigurationField(const std::string& _key, const std::string& _value)
{
    m_configuration.put(_key, _value);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Environment::loadSessionInfo(const boost::filesystem::path& _path)
{
    if (boost::filesystem::exists(_path))
    {
        std::ifstream fileStream;
        fileStream.open(_path.string(), std::ios_base::in | std::ios_base::binary);
        if (loadSessionInfo(fileStream))
        {
            BOOST_LOG_TRIVIAL(info) << "Loaded session info from " << _path;
            return true;
        }
    }

    BOOST_LOG_TRIVIAL(error) << "Session info file " << _path << " does not exist.";
    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Environment::loadSessionInfo(std::istream& _input)
{
    try
    {
        std::stringstream json;
        json << _input.rdbuf();
        read_json(json, m_sessionInfo);
        return true;
    }
    catch (std::exception& _ex)
    {
        BOOST_LOG_TRIVIAL(error) << "Environment::loadSessionInfo() : " << _ex.what();
    }

    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Environment::saveSessionInfo(const boost::filesystem::path& _path)
{
    std::ofstream fileStream;
    fileStream.open(_path.string(), std::ios_base::out | std::ios_base::trunc);
    if (saveSessionInfo(fileStream))
    {
        BOOST_LOG_TRIVIAL(info) << "Saved session info to " << _path;
        return true;
    }

    BOOST_LOG_TRIVIAL(error) << "Error saving session info to " << _path;
    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Environment::saveSessionInfo(std::ostream& _output)
{
    try
    {
        write_json(_output, m_sessionInfo);
        return true;
    }
    catch (std::exception& _ex)
    {
        BOOST_LOG_TRIVIAL(error) << "Environment::saveSessionInfo() : " << _ex.what();
    }

    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Environment::writeSessionInfoField(const std::string& _key, const std::string& _value)
{
    m_sessionInfo.put(_key, _value);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<class Ptree>
inline Ptree &empty_ptree()
{
    static Ptree pt;
    return pt;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::property_tree::ptree&
Environment::operator[](const std::string& _key)
{
    if (m_configuration.get_child_optional(_key).is_initialized())
    {
        return m_configuration.get_child(_key);
    }
    else
    if (m_sessionInfo.get_child_optional(_key).is_initialized())
    {
        return m_sessionInfo.get_child(_key);
    }

    return empty_ptree<boost::property_tree::ptree>();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::property_tree::ptree&
Environment::getConfiguration()
{
    return m_configuration;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::property_tree::ptree&
Environment::getSession()
{
    return m_sessionInfo;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugin
}   // namespace Core
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
