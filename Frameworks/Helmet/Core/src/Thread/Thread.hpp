//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indieHelmet.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Core/Thread/I_Thread.hpp>

#include <boost/thread.hpp>

#include <sstream>
#include <stdexcept>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Thread {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Runnable;

class Thread
:   public I_Thread
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Thread implementation
    /// @{
public:
    virtual void start();

    virtual void stop();

    virtual void join();

    static ThreadId getCurrentThreadId();

    virtual const ThreadId &getThreadId() const;
    /// @}

    /// @name Inner classes
    /// @{
public:
    class NativeThreadId
    :   public I_Thread::ThreadId::I_NativeThreadId
    {
        /// @name Friend declarations
        /// @{
    private:
        friend class Thread;
        /// @}

        /// @name NativeThreadId_posix implementation
        /// @{
    public:
        virtual bool operator==(const I_NativeThreadId &) const;

        virtual bool operator!=(const I_NativeThreadId &) const;

        virtual bool operator<(const I_NativeThreadId &_otherId) const;

        virtual I_NativeThreadId *clone() const;

        virtual std::string toString() const;
        /// @}

        /// @name 'Structors
        /// @{
    public:
        NativeThreadId(const boost::thread::id _id)
        :   m_nativeThreadId(_id)
        {
        }

        virtual ~NativeThreadId() = default;
        /// @}

        /// @name Member variables
        /// @{
    private:
        const boost::thread::id m_nativeThreadId;
        /// @}

    };  // class NativeThreadId_posix
    /// @}

    /// @name 'Structors
    /// @{
public:
             Thread(I_Runnable *const _runnable);
    virtual ~Thread();
    /// @}

    /// @name Additional implementation
    /// @{
private:
    static void *( ThreadFunction(void *));
    /// @}

    /// @name Member Variables
    /// @{
private:
    I_Runnable*     m_pRunnable;
    boost::thread   m_nativeThread;
    ThreadId        m_threadId;
    volatile bool   m_isStarted;
    volatile bool   m_isJoined;
    /// @}

};    // interface Thread_posix

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Thread
}   // namespace Core
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
