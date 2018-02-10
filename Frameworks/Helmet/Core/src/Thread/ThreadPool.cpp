//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include <Helmet/Core/Thread/ThreadPool.hpp>
#include <Helmet/Core/Thread/ThreadFactory.hpp>
#include <Helmet/Core/Exception/Exception.hpp>

#include "Mutex.hpp"
#include "Thread.hpp"

#include <Helmet/Core/Thread/I_Thread.hpp>

#include <iostream>
#include <stdexcept>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Thread {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ThreadPool::ThreadPool(unsigned const _numThreads,
                       I_Condition *const _pQueueIsEmpty,
                       bool const _waitForEmptyQueue,
                       bool const _startNow)
:   m_threadPool()
,   m_numberOfNotRunningThreads(_numThreads)
,   m_threadsStarted(false)
,   m_numberOfThreads(_numThreads)
,   m_pAllThreadsAreRunning(ConditionFactory::create())
,   m_requestQueue(_pQueueIsEmpty, true, _numThreads == 1)
,   m_amPreparingToStop(false)
,   m_amStopping(false)
,   m_amStoppingAsSoonAsPossible(false)
,   m_waitForEmptyQueue(_waitForEmptyQueue)
,   m_pSpinLock(MutexFactory::create())
{
    m_threadPool.reserve(_numThreads);

    if (_startNow) {
        start();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ThreadPool::~ThreadPool()
{
    // volatile, and visible to all Threads in this pool
    m_amStoppingAsSoonAsPossible = !m_waitForEmptyQueue;

    for (size_t i = m_threadPool.size(); i > 0; --i) {
        // wake up a blocked Thread (if any), tell it to quit now
        m_requestQueue.push(0);
    }

    for (std::vector<pThread_type>::const_iterator ppThread = m_threadPool.begin();
         ppThread != m_threadPool.end(); ++ppThread) {
        (*ppThread)->join();

        delete dynamic_cast<Thread*>(*ppThread);
    }

    // Now that the threads are all shut down, drain any remaining Tasks from the queue
    while (!m_requestQueue.isEmpty()) {
        Task *pTask;
        m_requestQueue.pop(pTask);
        if (pTask) pTask->dispose();
    }

    MutexFactory::destroy(m_pSpinLock);
    ConditionFactory::destroy(m_pAllThreadsAreRunning);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ThreadPool::isStarted()
{
    return m_threadsStarted;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ThreadPool::start()
{
    // Guard while checking to see if the threads are already started
    {
        CriticalSection guard(m_pSpinLock);
        if (m_threadsStarted) {
            /// If they're already started, simply return
            return;
        } else {
            // They're not started, but we're about to start them
            m_threadsStarted = true;
        }
    }

    // Start the threads.
    for (unsigned i = m_numberOfThreads; i > 0; --i) {
        ThreadPoolRunnable *const pRunnable = new ThreadPoolRunnable(*this);
        pThread_type pThread(ThreadFactory::create(pRunnable));
        m_threadPool.push_back(pThread);
        pThread->start();
    }

    // Wait for the threads to completely start
    requireStarted();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ThreadPool::requireStarted()
{
    // Wait for the threads to completely start
    m_pAllThreadsAreRunning->requireCondition();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ThreadPool::prepareToStop()
{
    // subsequent PushRequest()s will be ignored (and will return 'false').
    m_amPreparingToStop = true;

    // enables optional requestQueue assertion.  Still accepts push()s.
    m_requestQueue.closing();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ThreadPool::stop()
{
    if (m_amStopping) return;

    m_amPreparingToStop = true;                           // subsequent PushRequest()s will be ignored (and will return 'false').
    m_amStopping = true;
    if (!m_waitForEmptyQueue) {
        m_amStoppingAsSoonAsPossible = true;            // volatile, and visible to all Threads in this pool
    }

    for (size_t i = m_threadPool.size(); i > 0; --i) {
        m_requestQueue.push(
                0);                         // wake up a blocked Thread (if any), tell it to quit now
    }
    m_requestQueue.closing();                           // enables optional RequestQueue assertion.  Still accepts push()s.

    for (std::vector<pThread_type>::const_iterator ppThread = m_threadPool.begin();
         ppThread != m_threadPool.end(); ++ppThread) {
        (*ppThread)->join();
    }

    // subsequent m_requestQueue.push()s will be ignored.  Don't use until threads are done.
    m_requestQueue.close();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ThreadPool::TaskPool::give(PooledTask &_task)
{
    CriticalSection cs(m_pSpinLock);

    m_unusedTasks.push_front(_task);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ThreadPool::PooledTask &
ThreadPool::TaskPool::take()
{
    PooledTask *pTask;

    while (true) {
        if (m_amStopping) {
            HELMET_EXCEPTION(
                    "Helmet::Threading::ThreadPool::TaskPool::take() : The TaskPool is currently being stopped.");
        }

        // Check to see if the collection is empty.
        {
            CriticalSection cs(m_pSpinLock);

            // If the unused tasks collection is not empty, pop a task off of the list
            // TR - size() is linear time, so use empty() whenever possible
            //     Use size() only if you really want the size.
            //isEmpty = (0 == m_unusedTasks.size());
            if (!m_unusedTasks.empty()) {
                // Do this here while still locked, otherwise we could have a race
                // condition.
                pTask = &(m_unusedTasks.front());
                m_unusedTasks.pop_front();
                return *pTask;
            }
        }

        // The unused tasks is empty, so create a new task
        pTask = m_pooledTaskAllocator.create(*this);
        if (pTask != NULL) {
            return *pTask;
        }

        // Circle back around and try it again :)
        // Don't sleep.  Leave it to the allocator to decide what's appropriate
        //Helmet::Threading::I_Thread::sleepForMilliseconds(1);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ThreadPool::TaskPool::prepareToStop()
{
    // subsequent request to create new tasks will be ignored.
    m_amPreparingToStop = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ThreadPool::TaskPool::stop()
{
    if (m_amStopping) return;

    // subsequent request to create new tasks will be ignored.
    m_amPreparingToStop = true;
    m_amStopping = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void ThreadPool::ThreadPoolRunnable::run() throw() {
    try {
        // ThreadPool startup
        {
            bool readyToRockAndRoll;
            {
                CriticalSection cs(m_pool.m_pSpinLock);
                readyToRockAndRoll = (0 == --m_pool.m_numberOfNotRunningThreads);
            }
            if (readyToRockAndRoll) {
                m_pool.m_pAllThreadsAreRunning->assertCondition();
            }
        }

        // ThreadPool inner loop
        std::list<Task *> taskList;
        while (!m_pool.m_amStoppingAsSoonAsPossible)    // volatile
        {
            taskList.clear();
            m_pool.popRequest(taskList);
            for (std::list<Task *>::iterator ppTask = taskList.begin(); ppTask != taskList.end(); ++ppTask) {
                Task *const pTask = *ppTask;
                if (pTask) {
                    if (!m_pool.m_amStoppingAsSoonAsPossible) {
                        try {
                            pTask->call();
                        }
                        catch (...) {
#if DEBUG > 0
                            Mutex mutex(m_pool.m_spinLock);
                            CriticalSection cs (mutex);
                            std::cerr << "ThreadPool::ThreadPoolRunnable::run(): pTask->call() failed and threw an unknown exception." << std::endl;
                            std::cerr << "ThreadPool::ThreadPoolRunnable::run(): Queue has "<<m_pool.m_requestQueue.size()<<" unprocessed tasks." << std::endl;
                            std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ran "<<m_useCount<<" tasks." << std::endl;
                            std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ID is "<<getThread()->getThreadId().toString()<<"." << std::endl;
#endif // DEBUG
                        }
                    } else {
#if DEBUG >= 40
                        Mutex mutex(m_pool.m_spinLock);
                        CriticalSection cs (mutex);
                        std::cout << "ThreadPool::ThreadPoolRunnable::run(): quitting, even though there is an unprocessed task." << std::endl;
#endif // DEBUG
                    }
                    try {
                        pTask->dispose();       // *this Pool owns the Task object, and now gets rid of it
                    }
#if DEBUG > 0
                    catch (const std::exception& _oops)
                    {
                        Mutex mutex(m_pool.m_spinLock);
                        CriticalSection cs (mutex);
                        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): pTask->Dispose() failed and threw a std::exception, description='"<<_oops.what()<<"'" << std::endl;
                        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): Queue has "<<m_pool.m_requestQueue.size()<<" unprocessed tasks." << std::endl;
                        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ran "<<m_useCount<<" tasks." << std::endl;
                        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ID is "<<getThread()->getThreadId().toString()<<"." << std::endl;
                    }
#endif // DEBUG
                    catch (...) {
#if DEBUG > 0
                        Mutex mutex(m_pool.m_spinLock);
                        CriticalSection cs (mutex);
                        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): pTask->Dispose() failed and threw an unknown exception." << std::endl;
                        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): Queue has "<<m_pool.m_requestQueue.size()<<" unprocessed tasks." << std::endl;
                        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ran "<<m_useCount<<" tasks." << std::endl;
                        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ID is "<<getThread()->getThreadId().toString()<<"." << std::endl;
#endif // DEBUG
                    }
                } else                        // convention: null pointer-to-Task stops this worker thread (only)
                {
#if DEBUG >= 40
                    {
                        Mutex mutex(m_pool.m_spinLock);
                        CriticalSection cs (mutex);
                        std::cout << "ThreadPool::ThreadPoolRunnable::run (): Null task, worker exits normally." << std::endl;
                    }
#endif // DEBUG
                    // Return the remaining pointers-to-tasks (presumably all NULLs) back to the queue, for the other threads.
                    // Note that we cannot use m_pool.PushRequest() any longer.
                    m_pool.m_requestQueue.push(taskList, ++ppTask, taskList.end());

                    return;                 // Exit the thread function!
                }
            }
        }
#if DEBUG >= 40
        {
            Mutex mutex(m_pool.m_spinLock);
            CriticalSection cs (mutex);
            std::cout << "ThreadPool::ThreadPoolRunnable::run (): Pool is shutting down, worker exits normally." << std::endl;
        }
#endif  // DEBUG
    }
#if DEBUG > 0
        catch (const std::exception& _oops)
        {
            // TBD -- should log the exception, or at least inform the pool
            Mutex mutex(m_pool.m_spinLock);
            CriticalSection cs (mutex);
            std::cerr << "ThreadPool::ThreadPoolRunnable::run(): std::exception, description='"<<_oops.what()<<"'" << std::endl;
            std::cerr << "ThreadPool::ThreadPoolRunnable::run(): Queue has "<<m_pool.m_requestQueue.size()<<" unprocessed tasks." << std::endl;
            std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ran "<<m_useCount<<" tasks." << std::endl;
            std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ID is "<<getThread()->getThreadId().toString()<<"." << std::endl;
        }
#endif // DEBUG
    catch (...) {
        // TBD -- should log the exception, or at least inform the pool
#if DEBUG > 0
        Mutex mutex(m_pool.m_spinLock);
        CriticalSection cs (mutex);
        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): Queue has "<<m_pool.m_requestQueue.size()<<" unprocessed tasks." << std::endl;
        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ran "<<m_useCount<<" tasks." << std::endl;
#endif // DEBUG
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void ThreadPool::ThreadPoolRunnable::stop()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ThreadPool::ThreadPoolRunnable::ThreadPoolRunnable(ThreadPool &_pool)
:   m_pool(_pool)
#ifdef DEBUG
,   m_useCount(0)
,   m_pThread (0)
#endif // DEBUG
{}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ThreadPool::ThreadPoolRunnable::~ThreadPoolRunnable()
{
#if DEBUG >= 40
    Mutex mutex(m_pool.m_spinLock);
    CriticalSection cs (mutex);
    std::cout << "ThreadPool::ThreadPoolRunnable::~ThreadPoolRunnable(): Use count for this Thread is "<<m_useCount << std::endl;
#endif // DEBUG
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ThreadPool::pThread_type
ThreadPool::ThreadPoolRunnable::getThread() const
{
    return m_pThread;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ThreadPool::ThreadPoolRunnable::setThread(pThread_type _pThread)
{
    m_pThread = _pThread;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Thread
}   // namespace Core
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~