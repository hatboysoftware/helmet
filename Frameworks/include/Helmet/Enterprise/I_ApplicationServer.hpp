//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include "Configuration.hpp"

#include <boost/noncopyable.hpp>
#include <boost/thread/condition.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
    namespace Core {
        namespace Thread {
            class I_Condition;
        }   // namespace Thread
    }   // namespace Core
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Base Application Server.
class HELMET_ENTERPRISE_DLL_LINK I_ApplicationServer
:   boost::noncopyable
{
    /// @name Forward declarations
    /// @{
public:
    /// @}

    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_ApplicationServer interface.
    /// @{
public:
    virtual Core::Thread::I_Condition* start() = 0;
    virtual void stop() = 0;
    /// @}

    /// @name Inner classes
    /// @{
public:
    /// @}

    /// @name Static Instance
    /// @{
public:
    /// Get an application server by name.
    ///
    /// The lifetime of an application server is from the time that getInstance()
    /// returns to the time that I_ApplicationServer::stop() is called and the 
    /// application server is fully stopped.
    static I_ApplicationServer& getInstance(const std::string& _instanceName);
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
    I_ApplicationServer() = default;
public:
    virtual ~I_ApplicationServer() = default;
    /// @}

};  // interface I_ApplicationServer

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
