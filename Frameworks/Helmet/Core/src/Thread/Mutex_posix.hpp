//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include "../I_Mutex.hpp"

// C++
#include <string>

#include <boost/thread/mutex.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Mutex
:   public I_Mutex
{
    friend class Condition;

    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Mutex implementation
    /// @{
public:
    virtual void acquire();
    virtual void release();
    /// @}

    /// @name Helper Functions
    /// @{
public:
    static const std::string& getNameOfError(const int _errNo);
    /// @}

    /// @name 'Structors
    /// @{
public:
             Mutex();
    virtual ~Mutex();
    /// @}

    /// @name Accessor functions
    /// @{
protected:
    inline boost::mutex&
    getNativeMutex()
    {
        return m_mutex;
    }

    inline const boost::mutex&
    getNativeMutex() const
    {
        return m_mutex;
    }
    /// @}

    /// @name Member variables
    /// @{
private:
    boost::mutex    m_mutex;
    /// @}

};  // class Mutex_posix

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
