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
class I_Endpoint;
class I_ResourceLocation;

/// @brief Fully Qualified Resource Location
///
///
class HELMET_ENTERPRISE_DLL_LINK I_FullyQualifiedResourceLocation
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef std::shared_ptr<I_Endpoint>    pEndpoint_type;
    typedef std::shared_ptr<I_ResourceLocation>        pResourceLocation_type;
    /// @}

    /// @name I_FullyQualifiedResourceLocation interface.
    /// @{
public:
    /// Get the endpoint.
    virtual pEndpoint_type getEndpoint() const = 0;

    /// Get the relative resource location.
    virtual pResourceLocation_type getRelativeLocation() const = 0;

    /// Get the location as a URL string.
    virtual const std::string toString() const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_FullyQualifiedResourceLocation();
    virtual ~I_FullyQualifiedResourceLocation();
    /// @}

};  // interface I_FullyQualifiedResourceLocation

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
