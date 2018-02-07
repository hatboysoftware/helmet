//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indieHelmet.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Core/Configuration.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Thread {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class HELMET_CORE_DLL_LINK I_Thread
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    struct ThreadId;
    /// @}

    /// @name I_Thread interface
    /// @{
public:

    /// Start this thread
    virtual void start() = 0;

    /// Stop this thread, or at least request it to stop because it is not
    /// guaranteed to actually stop.
    virtual void stop() = 0;

    /// Block until this I_Thread has terminated.
    virtual void join() = 0;

    /// Returns a copy of running (kernel-)thread's id, rendered in this
    /// object's id format (so that you can safely compare it to
    /// getThreadId()).  Always valid.
    static ThreadId getCurrentThreadId();

    /// Returns a copy of this object's id. Not valid until after start().
    virtual const ThreadId &getThreadId() const = 0;
    /// @}

    /// @name Static Methods
    /// @{
public:

    static void sleepForMilliseconds(unsigned const _milliseconds); // current thread will sleep
    /// @}

    /// @name Inner Classes
    /// @{
public:

    struct ThreadId {
        /// @name Inner Interfaces
        /// @{
    public:
        class I_NativeThreadId {
            /// @name Types
            /// @{
        public:
            /// @}

            /// @name I_NativeThreadId interface
            /// @{
        public:
            virtual bool operator==(const I_NativeThreadId &_otherId) const = 0;

            virtual bool operator!=(const I_NativeThreadId &_otherId) const = 0;

            virtual bool operator<(const I_NativeThreadId &_otherId) const = 0;

            virtual I_NativeThreadId *clone() const = 0;

            virtual std::string toString() const = 0;
            /// @}

            /// @name 'Structors
            /// @{
        public:
                     I_NativeThreadId() = default;
            virtual ~I_NativeThreadId() = default;
            /// @}

        };  // class I_NativeThreadId
        /// @}

        /// @name ThreadId implementation
        /// @{
    public:
        ThreadId &operator=(const ThreadId &_otherId) {
            delete m_pNativeThreadId;
            m_pNativeThreadId = _otherId.m_pNativeThreadId->clone();
            return *this;
        }

        bool operator==(const ThreadId &_otherId) const {
            return (m_pNativeThreadId == _otherId.m_pNativeThreadId) ||
                   (m_pNativeThreadId && _otherId.m_pNativeThreadId &&
                    (*m_pNativeThreadId == *_otherId.m_pNativeThreadId));
        }

        bool operator!=(const ThreadId &_otherId) const {
            return (m_pNativeThreadId != _otherId.m_pNativeThreadId) &&
                   (!m_pNativeThreadId || !_otherId.m_pNativeThreadId ||
                    (*m_pNativeThreadId != *_otherId.m_pNativeThreadId));
        }

        bool operator<(const ThreadId &_otherId) const {
            return (m_pNativeThreadId < _otherId.m_pNativeThreadId);
        }

        operator std::string() const { return m_pNativeThreadId ? m_pNativeThreadId->toString() : ""; }

        std::string toString() const { return m_pNativeThreadId ? m_pNativeThreadId->toString() : ""; }
        /// @}

        /// @name 'Structors
        /// @{
    public:
        explicit ThreadId(I_NativeThreadId *const _pId = NULL) : m_pNativeThreadId(_pId) {}

        ThreadId(const ThreadId &_otherId) : m_pNativeThreadId(_otherId.m_pNativeThreadId->clone()) {}

        ~ThreadId() { delete m_pNativeThreadId; }
        /// @}

        /// @name Member variables
        /// @{
    public:
        I_NativeThreadId *m_pNativeThreadId;
        /// @}

    };  // struct ThreadId
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Thread();
    virtual ~I_Thread();
    /// @}

};    // interface I_Thread

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Thread
}   // namespace Core
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
