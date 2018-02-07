//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Core/Exception/Exception.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Exception {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class HELMET_CORE_DLL_LINK NotImplementedException
:   public Exception
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name std::exception implementation
    /// @{
public:
    /// @}

    /// @name Exception implementation
    /// @{
public:
    /// @}

    /// @name NotImplementedException implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
    NotImplementedException(const std::string& _function,
                            const std::string& _file,
                            const std::string& _line);
    virtual ~NotImplementedException() = default;
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// @}

};  // class NotImplementedException

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Exception
}   // namespace Core
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define HELMET_NOT_IMPLEMENTED_EXCEPTION() \
throw Helmet::Core::Exception::NotImplementedException(TOSTRING(__FUNCTION__),\
                                                     __FILE__,\
                                                     TOSTRING(__LINE__))
