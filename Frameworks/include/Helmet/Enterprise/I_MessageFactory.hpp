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

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class I_Endpoint;
class I_Message;
class I_MessageHeader;

class HELMET_ENTERPRISE_DLL_LINK I_MessageFactory
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef std::shared_ptr<I_Message>                      pMessage_type;
    typedef Helmet::Memory::managed_weak_ptr<I_Message>        wpMessage_type;
    typedef std::shared_ptr<I_Endpoint>                     pEndpoint_type;
    typedef std::shared_ptr<I_MessageHeader>                pMessageHeader_type;
    /// @}

    /// @name I_MessageFactory interface
    /// @{
public:
    /// Create a message.
    virtual pMessage_type create(pMessageHeader_type _pMessageHeader, pEndpoint_type _pSourceEndpoint, pEndpoint_type _pDestinationEndpoint) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_MessageFactory();
    virtual ~I_MessageFactory();
    /// @}

};  // interface I_MessageFactory

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
