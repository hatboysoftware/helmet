//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "ApplicationServer.hpp"

#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static std::map<std::string, I_ApplicationServer*>   sm_servers;
static boost::mutex* sm_pServerGuard = new boost::mutex();
namespace {
    class gc
    {
    public:
        ~gc()
        {
            {
                boost::lock_guard<boost::mutex> lock(*sm_pServerGuard);

                while (!sm_servers.empty())
                {
                    delete sm_servers.begin()->second;
                    sm_servers.erase(sm_servers.begin());
                }
            }

            delete sm_pServerGuard;
        }
    };

    static gc sm_garbageCollection;
}   // namespace
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ApplicationServer&
I_ApplicationServer::getInstance(const std::string& _instanceName)
{
    boost::lock_guard<boost::mutex> lock(*sm_pServerGuard);

    std::map<std::string, I_ApplicationServer*>::iterator iter = sm_servers.find(_instanceName);
    if (iter == sm_servers.end())
    {
        I_ApplicationServer* pServer = new ApplicationServer();
        sm_servers[_instanceName] = pServer;
        return *pServer;
    }
    else
    {
        return *iter->second;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
