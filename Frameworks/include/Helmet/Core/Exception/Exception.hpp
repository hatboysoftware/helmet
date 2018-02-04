//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Core/Configuration.hpp>

#include <exception>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Exception {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class HELMET_CORE_DLL_LINK Exception
:   public std::exception
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name std::exception implementation
    /// @{
public:
    const char* what() const _NOEXCEPT override;
    /// @}

    /// @name Exception implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
    Exception(const std::string& _message,
              const std::string& _function,
              const std::string& _file,
              const std::string& _line);
    virtual inline ~Exception() = default;
    /// @}

    /// @name Member Variables
    /// @{
private:
    const std::string       m_message;
    const std::string       m_function;
    const std::string       m_file;
    const std::string       m_line;
    /// @}

};  // interface I_Exception

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Exception
}   // namespace Core
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define HELMET_EXCEPTION(message) \
    throw Helmet::Core::Exception::Exception(message,\
                                             TOSTRING(__FUNCTION__),\
                                             __FILE__,\
                                             TOSTRING(__LINE__))
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
