//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include "Configuration.hpp"

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ApplicationServer;

/// Helper container class that sets up an application server.
/// Use this implementation if you wish, but it's not required.
class HELMET_ENTERPRISE_DLL_LINK I_Container
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Container interface
    /// @{
public:
    /// Initialize the container.
    /// @return true if the container was able to load the
    ///     configuration file and install all of the application
    ///     services and protocol services.
    /// Parameters:
    ///     --config -c : configuration xml file.
    ///     --lang -l   : scripting plugin to load.
    ///     --script -s : boot script.
    ///
    virtual bool init(int _argc, const char* _argv[]) = 0;

    /// Run the container.
    virtual int run() = 0;

    /// Stop the container.
    /// @note The container will need to be reinitialized prior to use after this is called.
    virtual void stop() = 0;

    virtual I_ApplicationServer& getApplicationServer() = 0;
    /// @}

    /// @name Static methods
    /// @{
public:
    /// Get the singleton container.
    static I_Container& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Container();
    virtual ~I_Container();
    /// @}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
