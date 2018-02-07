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

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_NetworkAddress;
class I_NetworkConnection;
class I_NetworkConnectionListener;
class I_NetworkReadReadyListener;
class I_NetworkWriteReadyListener;

/// Basic network service interface
class HELMET_ENTERPRISE_DLL_LINK I_NetworkService
{
    /// @name Types
    /// @{
public:
    typedef std::shared_ptr<I_NetworkConnection>     pNetworkConnection_type;
    /// @}

    /// @name I_NetworkService interface
    /// @{
public:
    /// Connect to the specified network address
    ///
    /// A valid I_NetworkConnection is always returned if _address is valid, but
    /// it's possible that the returned object is not connected.  If the
    /// I_NetworkService implementation implements an asynchronous connect method
    /// then the logical / physical connection might occur at a later time.
    ///
    /// @return Network connection or NULL if _address is invalid.
    /// @see I_NetworkConnection
    virtual pNetworkConnection_type connect(const I_NetworkAddress& _address) = 0;

    /// Listens on the specified network address
    virtual void listen(const I_NetworkAddress& _address) = 0;

    /// Registers a network connection listener
    virtual void registerConnectionListener(I_NetworkConnectionListener& _listener) = 0;

    /// Unregisters a network connection listener
    virtual void unregisterConnectionListener(I_NetworkConnectionListener& _listener) = 0;

    /// Registers the one Read Ready Listener
    /// If a listener has already been registered then it will be superceeded.
    /// If NULL is specified for _pListener then the previous listener will be
    //  unregistered.
    virtual void setReadReadyListener(I_NetworkReadReadyListener* const _pListener) = 0;

    /// Registers the one Write Ready Listener
    /// If a listener has already been registered then it will be superceeded.
    /// If NULL is specified for _pListener then the previous listener will be
    ///  unregistered.
    virtual void setWriteReadyListener(I_NetworkWriteReadyListener* const _pListener) = 0;

    /// At the network level, a write must be requested before writing.
    /// I_NetworkWriteReadyListener::onWriteReady will indicate that the connection
    /// is ready for writing.
    virtual void requestWrite(pNetworkConnection_type const _pConnection) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_NetworkService();
    virtual ~I_NetworkService();
    /// @}

};  // interface I_NetworkService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
