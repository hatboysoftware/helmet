//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Enterprise/I_ApplicationServerManager.hpp>
#include <Helmet/Core/Thread/I_Mutex.hpp>

#include <map>
#include <memory>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ResourceLocation;

class ApplicationServerManager
:   public I_ApplicationServerManager
{
    /// @name Types
    /// @{
public:
    typedef std::shared_ptr<I_ProtocolService>          pProtocolService_type;
    typedef std::shared_ptr<I_ApplicationService>       pApplicationService_type;
    typedef std::shared_ptr<I_ResourceLocation>         pResourceLocation_type;
    typedef std::weak_ptr<I_ResourceLocation>           wpResourceLocation_type;
    /// @}

    /// @name I_ApplicationServerManager implementation.
    /// @{
public:
    virtual pProtocolService_type createProtocolService(I_ApplicationServer& _server, const std::string& _type);
    virtual pApplicationService_type createApplicationService(I_ApplicationServer& _server, const std::string& _type);
    virtual pResourceLocation_type createLocation(const std::string& _location);
    /// @}

    /// @name ApplicationServerManager implementation
    /// @{
private:
    void destroyLocation(wpResourceLocation_type _location);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class I_ApplicationServerManager;
             ApplicationServerManager();
    virtual ~ApplicationServerManager();
    /// @}

    /// @name Member Variables
    /// @{
private:
    typedef std::map<std::string, wpResourceLocation_type>   ResourceLocations_type;

    /// Guard for m_resourceLocations
    Core::Thread::I_Mutex*  m_pResourceLocationGuard;
    /// Map of resource locations from the string representation to the actual object
    ResourceLocations_type  m_resourceLocations;
    /// @}

};  // interface ApplicationServerManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
