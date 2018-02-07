//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "Thread.hpp"

#include <Helmet/Core/Thread/I_Runnable.hpp>
#include <Helmet/Core/Thread/CriticalSection.hpp>

#include <boost/thread.hpp>

#include <stdexcept>
#include <sstream>
#include <map>

#include <unistd.h>
#include <sys/errno.h>
#include <assert.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Thread {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Thread::Thread(I_Runnable *const _pRunnable)
:   m_pRunnable(_pRunnable)
,   m_nativeThread()
,   m_threadId()
,   m_isStarted(false)
,   m_isJoined(false) 
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Thread::~Thread()
{
    if (m_isStarted) {
        stop(); // might or might not be helpful
        join();
    }

    delete m_pRunnable;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Thread::start()
{
    if (!m_isStarted) {
        if (m_pRunnable != NULL) {
            m_nativeThread = boost::thread(ThreadFunction, this);
            auto threadId = m_nativeThread.get_id();
            m_threadId.m_pNativeThreadId = new NativeThreadId(threadId);
            m_isStarted = true;
        } else {
            throw std::runtime_error("Thread::Start(): Missing the required I_Runnable.");
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Thread::stop()
{
    if (m_isStarted && (m_pRunnable != NULL)) {
        m_pRunnable->stop();  // not all I_Runnables honor the Stop request, some just run to completion
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Thread::join()
{
    if (m_isStarted && !m_isJoined) {
        m_nativeThread.join();
        m_isJoined = true;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Thread::ThreadId
Thread::getCurrentThreadId()
{
    return ThreadId(new NativeThreadId(boost::this_thread::get_id()));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const I_Thread::ThreadId &
Thread::getThreadId() const
{
    return m_threadId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
//DWORD WINAPI
void *
(Thread::ThreadFunction(void *_pThis))
{
    auto *const pThis = static_cast <Thread *> (_pThis);

    try {
        pThis->m_pRunnable->run();
    }
    catch (...) {
    }

    return nullptr;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Thread::NativeThreadId::operator==(const I_Thread::ThreadId::I_NativeThreadId &_id) const {
    const NativeThreadId *const pNativeThreadId = static_cast<const NativeThreadId *>(&_id);
    return (pNativeThreadId != NULL) &&
            (m_nativeThreadId == pNativeThreadId->m_nativeThreadId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Thread::NativeThreadId::operator!=(const I_Thread::ThreadId::I_NativeThreadId &_id) const {
    const NativeThreadId *const pNativeThreadId = static_cast<const NativeThreadId *>(&_id);
    return (pNativeThreadId == NULL) ||
            (m_nativeThreadId != pNativeThreadId->m_nativeThreadId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Thread::NativeThreadId::operator<(const I_Thread::ThreadId::I_NativeThreadId &_id) const {
    const NativeThreadId *const pNativeThreadId = dynamic_cast<const NativeThreadId *>(&_id);
    assert(pNativeThreadId != NULL);
    return (m_nativeThreadId < pNativeThreadId->m_nativeThreadId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Thread::ThreadId::I_NativeThreadId *
Thread::NativeThreadId::clone() const {
    return new NativeThreadId(m_nativeThreadId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::string
Thread::NativeThreadId::toString() const {
    std::ostringstream oStream;
    oStream << std::hex << std::uppercase << "0x" << m_nativeThreadId;
    return oStream.str();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Thread
}   // namespace Core
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
