//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Enterprise/I_ApplicationService.hpp>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <memory>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Enterprise {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// @brief Resource Location
///
///
class HELMET_ENTERPRISE_DLL_LINK I_ResourceLocation
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<I_ApplicationService> pApplicationService_type;
    typedef boost::shared_ptr<I_ResourceLocation>   pResourceLocation_type;
    /// @}

    /// @name I_ResourceLocation interface.
    /// @{
public:
    /// Get the location as a string.
    virtual const std::string& toString() const = 0;

    /// Operators
    virtual bool operator==(const I_ResourceLocation& _otherLocation) const = 0;
    virtual bool operator!=(const I_ResourceLocation& _otherLocation) const = 0;
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name Static methods
    /// @{
public:
    static pResourceLocation_type getLocation(const std::string& _location);
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ResourceLocation();
    virtual ~I_ResourceLocation();
    /// @}

};  // interface I_ResourceLocation

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Enterprise
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
