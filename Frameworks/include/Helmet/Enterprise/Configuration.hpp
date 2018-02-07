//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#ifdef  _MSC_VER
#    pragma once
#    pragma inline_depth(255)
#    pragma inline_recursion(off)
#    pragma warning(disable:4251)   // 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
#    pragma warning(disable:4275)   // non – DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
#ifdef VLD
#include <vld.h>
#endif // VLD
#endif  // _MSC_VER

#ifdef  _MSC_VER
#    ifndef HOST_WIN32              // this probably should be done elsewhere ...
#        ifdef _WIN32
#            define HOST_WIN32
#        endif
#    endif
#    ifndef HOST_WIN64
#        ifdef _WIN64
#            define HOST_WIN64
#        endif
#    endif
#endif  // _MSC_VER

// Microsoft C++ compiler
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#    ifdef  HELMET_ENTERPRISE_EXPORTS
#        define HELMET_ENTERPRISE_DLL_LINK __declspec(dllexport)
#    else
#        define HELMET_ENTERPRISE_DLL_LINK __declspec(dllimport)
#    endif
#endif  // _MSC_VER

// GNU C++ compiler
#if defined(__GNUG__) && !defined(__INTEL_COMPILER)
#    if 0 // (__GNUC__ >= 4) && !defined(HOST_CYGWIN) && !defined(HOST_SOLARIS)
#        ifdef  HELMET_ENTERPRISE_EXPORTS
#            define HELMET_ENTERPRISE_DLL_LINK __attribute__ ((visibility ("default")))
#        else
#            define HELMET_ENTERPRISE_DLL_LINK __attribute__ ((visibility ("hidden")))
#        endif
#    else
#        ifdef  HELMET_ENTERPRISE_EXPORTS
#            define HELMET_ENTERPRISE_DLL_LINK // __declspec(dllexport)
#        else
#            define HELMET_ENTERPRISE_DLL_LINK // __declspec(dllimport)
#        endif
#    endif
#endif  // __GNUG__

// Sun C++ compiler
#if defined(__SUNPRO_CC)
#    ifdef  HELMET_ENTERPRISE_EXPORTS
#        define HELMET_ENTERPRISE_DLL_LINK   // ?
#    else
#        define HELMET_ENTERPRISE_DLL_LINK   // ?
#    endif
#endif  // __SUNPRO_CC

// Intel C++ compiler
#if defined(__INTEL_COMPILER)
#    ifdef  HELMET_ENTERPRISE_EXPORTS
#        define HELMET_ENTERPRISE_DLL_LINK __declspec(dllexport)
#    else
#        define HELMET_ENTERPRISE_DLL_LINK __declspec(dllimport)
#    endif
#endif  // __INTEL_COMPILER

#ifndef HELMET_ENTERPRISE_DLL_LINK
#    define HELMET_ENTERPRISE_DLL_LINK
#endif  // ! HELMET_ENTERPRISE_DLL_LINK
