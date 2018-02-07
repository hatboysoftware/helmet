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

#include <memory>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ProtocolService;
class I_ApplicationService;
class I_ResourceLocation;
class I_ApplicationServer;

class HELMET_ENTERPRISE_DLL_LINK I_ApplicationServerManager
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef std::shared_ptr<I_ProtocolService>          pProtocolService_type;
    typedef std::shared_ptr<I_ApplicationService>       pApplicationService_type;
    typedef std::shared_ptr<I_ResourceLocation>         pResourceLocation_type;
    /// @}

    /// @name I_ApplicationServerManager interface.
    /// @{
public:
    /// Create a protocol service of the specified type.
    virtual pProtocolService_type createProtocolService(I_ApplicationServer& _server, const std::string& _type) = 0;

    /// Create an Application Service
    virtual pApplicationService_type createApplicationService(I_ApplicationServer& _server, const std::string& _type) = 0;

    /// Create a resource location
    virtual pResourceLocation_type createLocation(const std::string& _location) = 0;
    /// @}

    /// @name Static Instance
    /// @{
public:
    /// Get the singleton
    /// @return Reference to the singleton instance.
    static I_ApplicationServerManager& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ApplicationServerManager() = default;
    virtual ~I_ApplicationServerManager() = default;
    /// @}

};  // interface I_ApplicationServerManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
