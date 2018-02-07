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
#include <memory>

#include <Helmet/Enterprise/I_MessageRegistry.hpp>
#include <Helmet/Enterprise/I_MessageType.hpp>

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Message Registry that uses I_MessageType with a numeric
/// uint32_t implementation.
class HELMET_ENTERPRISE_DLL_LINK I_NumericTypeMessageRegistry
:   public I_MessageRegistry
{
    /// @name Types
    /// @{
public:
    typedef std::shared_ptr<I_MessageType>         pMessageType_type;
    typedef Helmet::Memory::managed_weak_ptr<I_MessageType>    wpMessageType_type;
    typedef std::shared_ptr<I_MessageRegistry>     pMessageRegistry_type;
    /// @}

    /// @name I_NumericTypeMessageRegistry interface.
    /// @{
public:
    virtual pMessageType_type getMessageType(boost::uint64_t _type) = 0;
    /// @}

    /// @name Inner types
    /// @{
public:
    class I_NumericType
    :   public I_MessageType
    {
    public:
        virtual boost::uint64_t getType() const = 0;
    };  // class NumericType

    /// @}
    /// @name 'Structors
    /// @{
protected:
             I_NumericTypeMessageRegistry();
    virtual ~I_NumericTypeMessageRegistry();
    /// @}

};  // interface I_NumericTypeMessageRegistry

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
