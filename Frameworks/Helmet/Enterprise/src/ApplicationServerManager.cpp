//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "ApplicationServerManager.hpp"

#include <Helmet/Core/Thread/MutexFactory.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServerManager::ApplicationServerManager()
:   m_pResourceLocationGuard(Core::Thread::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServerManager::~ApplicationServerManager()
{
    Core::Thread::MutexFactory::destroy(m_pResourceLocationGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServerManager::pProtocolService_type
ApplicationServerManager::createProtocolService(I_ApplicationServer& _server, const std::string& _type)
{
    return pProtocolService_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServerManager::pApplicationService_type
ApplicationServerManager::createApplicationService(I_ApplicationServer& _server, const std::string& _type)
{
    return pApplicationService_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServerManager::pResourceLocation_type
ApplicationServerManager::createLocation(const std::string& _location)
{
    return pResourceLocation_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServerManager::destroyLocation(wpResourceLocation_type _pLocation)
{

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/*
 * //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
//  Tony Richards trichards@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "ApplicationServerManager.hpp"
#include "../I_ApplicationService.hpp"
#include "../I_ProtocolService.hpp"
#include "../I_ApplicationServiceFactory.hpp"
#include "../I_ProtocolServiceFactory.hpp"

#include "ResourceLocation.hpp"

#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Plugins/I_ExtensionRegistry.hpp>
#include <Zen/Core/Plugins/I_ExtensionPoint.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServerManager::ApplicationServerManager()
:   m_pResourceLocationGuard(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServerManager::~ApplicationServerManager()
{
    Threading::MutexFactory::destroy(m_pResourceLocationGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServerManager::pProtocolService_type
ApplicationServerManager::createProtocolService(I_ApplicationServer& _server, const std::string& _type)
{
    Plugins::I_ExtensionPoint::pExtension_type pExtension =
        Plugins::I_ExtensionRegistry::getSingleton()
            .getExtensionPoint(I_ProtocolService::getNamespace(), I_ProtocolService::getExtensionPointName())
                ->getExtension(_type);

    if (pExtension)
    {
        I_ProtocolServiceFactory* pFactory =
            dynamic_cast<I_ProtocolServiceFactory*>(&pExtension->getClassFactory());

        if (pFactory)
        {
            return pFactory->create(_server, _type);
        }
    }

    return pProtocolService_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServerManager::pApplicationService_type
ApplicationServerManager::createApplicationService(I_ApplicationServer& _server, const std::string& _type)
{
    Plugins::I_ExtensionPoint::pExtension_type pExtension =
        Plugins::I_ExtensionRegistry::getSingleton()
            .getExtensionPoint(I_ApplicationService::getNamespace(), I_ApplicationService::getExtensionPointName())
                ->getExtension(_type);

    if (pExtension)
    {
        I_ApplicationServiceFactory* pFactory =
            dynamic_cast<I_ApplicationServiceFactory*>(&pExtension->getClassFactory());

        if (pFactory)
        {
            return pFactory->create(_server, _type);
        }
    }

    return pApplicationService_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServerManager::pResourceLocation_type
ApplicationServerManager::createLocation(const std::string& _location)
{
    // Lock for consistency
    Threading::CriticalSection lock(m_pResourceLocationGuard);

    // Look to see if the resource location already exists
    ResourceLocations_type::iterator iter = m_resourceLocations.find(_location);
    if (iter != m_resourceLocations.end())
    {
        if (!iter->second.expired())
        {
            return iter->second.lock();
        }
        else
        {
            m_resourceLocations.erase(iter);
        }
    }

    // Wasn't found, so make one.
    pResourceLocation_type pLocation = pResourceLocation_type(new ResourceLocation(_location), boost::bind(&ApplicationServerManager::destroyLocation, this, _1));

    m_resourceLocations[_location] = pLocation.getWeak();
    return pLocation;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServerManager::destroyLocation(wpResourceLocation_type _pLocation)
{
    ResourceLocation* pLocation = dynamic_cast<ResourceLocation*>(_pLocation.get());
    if (pLocation != NULL)
    {
        {
            Threading::CriticalSection lock(m_pResourceLocationGuard);
            m_resourceLocations.erase(pLocation->toString());
        }

        delete pLocation;
    }
    else
    {
        // TODO Error, wrong type!
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
 */