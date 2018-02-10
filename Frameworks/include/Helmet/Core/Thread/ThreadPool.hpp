//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Core/Configuration.hpp>

#include <Helmet/Core/Thread/I_Callable.hpp>
#include <Helmet/Core/Thread/I_Runnable.hpp>
#include <Helmet/Core/Thread/SynchronizedQueue.hpp>

#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>
#include <boost/intrusive/slist.hpp>
#include <boost/thread/mutex.hpp>

#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Thread {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Thread;

class I_Condition;

/// @note This implementation is exposed directly instead of going through
///         a public interface for performance reasons.
class HELMET_CORE_DLL_LINK ThreadPool
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef I_Thread *pThread_type;

    /// @Our basic unit of work is a Task
    /// All clients will subclass Task and implement its call() method.
    /// The pool owns the pushed Tasks, and will first call() and then dispose()
    /// each of them.
    class Task
    :   public I_Callable<>
    {
        /// @name Task interface / implementation
        /// @{
    public:
        /// Clients might choose instead to recycle his Tasks
        virtual void dispose() { delete this; }
        /// @}

        /// @name 'Structors
        /// @{
    protected:
        inline
        Task() {}

    public:
        virtual ~Task() {}
        /// @}

    };  // class Task

    /// @name Forward Declaration
    /// @{
    class TaskPool;
    /// @}

    class PooledTask
    :   public Task
    ,   public boost::intrusive::slist_base_hook<>
    ,   public boost::noncopyable
    {
        /// @name Task interface / implementation
        /// @{
    public:
        /// Recycle the task
        virtual void dispose() { m_parent.give(*this); }
        /// @}

        /// @name 'Structors
        /// @{
    protected:
        inline
        PooledTask(TaskPool &_parent) : m_parent(_parent) {}

    public:
        virtual ~PooledTask() {}
        /// @}

        /// @name Member variables
        /// @{
    private:
        TaskPool &m_parent;
        /// @}

    };  // class PooledTask

    class I_PooledTaskAllocator
    :   public boost::noncopyable
    {
        /// @name I_PooledTaskAllocator interface
        /// @{
    public:
        virtual PooledTask *create(TaskPool &_parent) = 0;

        virtual void destroy(PooledTask *_pTask) = 0;
        /// @}

        /// @name 'Structors
        /// @{
    protected:
                 I_PooledTaskAllocator() = default;
        virtual ~I_PooledTaskAllocator() = default;
        /// @}

    };  // interface I_PooledTaskAllocator

    class HELMET_CORE_DLL_LINK TaskPool
    :   public boost::noncopyable
    {
        /// @name TaskPool interface / implementation
        /// @{
    public:

        /// Take a PooledTask from the TaskPool
        PooledTask &take();

        /// Give a PooledTask back to the TaskPool
        void give(PooledTask &_task);

        /// Prepare the TaskPool for stopping.
        /// No more new PooledTasks can be created.
        /// @todo TR - This currently does not do anything, but should it?
        void prepareToStop();

        /// Stop the TaskPool.
        /// No more PooledTasks can be taken.
        void stop();

        /// @}

        /// @name 'Structors
        /// @{
    public:
        inline
        TaskPool(I_PooledTaskAllocator& _pooledTaskAllocator)
        :   m_pooledTaskAllocator (_pooledTaskAllocator)
        ,   m_amPreparingToStop(false)
        ,   m_amStopping(false)
        ,   m_pSpinLock(MutexFactory::create())
        {
        }

        virtual ~TaskPool()
        {
            {
                CriticalSection lock(m_pSpinLock);
                PooledTask *pTask;
                while (!m_unusedTasks.empty()) {
                    pTask = &(m_unusedTasks.front());
                    m_unusedTasks.pop_front();
                    m_pooledTaskAllocator.destroy(pTask);
                }
            }

            MutexFactory::destroy(m_pSpinLock);
        }
        /// @}

        /// @name Member variables
        /// @{
    private:
        /// PooledTaskAllocator object (PooledTask Factory/Flyweight)
        I_PooledTaskAllocator &m_pooledTaskAllocator;

        /// Unused/Inactive PooledTask list
        boost::intrusive::slist<PooledTask> m_unusedTasks;

        /// Blocks submission of new PooledTask requests
        volatile bool m_amPreparingToStop;

        volatile bool m_amStopping;

        mutable I_Mutex* m_pSpinLock;
        /// @}

    };  // class TaskPool

private:

    class ThreadPoolRunnable
            : public I_Runnable, boost::noncopyable {
        /// @name I_Runnable implementation
        /// @{
    public:
        virtual void run() throw();

        virtual void stop();
        /// @}

        /// @name ThreadPoolRunnable implementation
        /// @{
    public:
        pThread_type getThread() const;

        void setThread(pThread_type _pThread);
        /// @}

        /// @name 'Structors
        /// @{
    public:
        ThreadPoolRunnable(ThreadPool &_pool);

        virtual ~ThreadPoolRunnable();
        /// @}

        /// @name Member Variables
        /// @{
    private:
        ThreadPool &m_pool;
        unsigned long m_useCount;
        pThread_type m_pThread;
        /// @}
    };  // class ThreadPoolRunnable
    /// @}

    /// @name ThreadPool implementation
    /// @{
public:

    /// @return true if start() has been called, either directly or
    ///     indirectly.  A true return value does not mean that the
    ///     threads have fully started and that the queue is ready for
    ///     use.
    /// @see requireStarted() if you need to block until the pool is
    ///     completely started.
    bool isStarted();

    /// Start the threads in the thread pool.
    ///
    /// If the threads have already been started, this method
    /// will simply return with no state changes.
    ///
    /// If the threads have not already been started, this
    /// method will start the threads and not return until the
    /// threads have been completely started.
    ///
    /// @note On Windows OS's, this method MUST NOT be called
    /// before WinMain AND DLLMain is called.  i.e. DO NOT
    /// call this method from a static initializer.
    void start();

    /// Require the thread pool to be completely started.  This method
    /// will block until that condition has been met.
    void requireStarted();

    /// Prepare the queue for stopping.
    /// No more new items can be pushed onto the queue.
    void prepareToStop();

    /// Stop the queue.
    /// No more Task objects can be popped off the queue
    void stop();

    /// @note do NOT push null pointers-to-Tasks!
    bool pushRequest(Task *const _pInput);

    /// @note do NOT push null pointers-to-Tasks!
    bool pushRequest(std::list<Task *> &_input);

    /// @note do NOT push null pointers-to-Tasks!
    bool pushRequest(std::list<Task *> &_input, std::list<Task *>::iterator _begin, std::list<Task *>::iterator _end);

    /// @note do NOT push null pointers-to-Tasks!
    bool pushRequest(std::list<Task *>::iterator _begin, std::list<Task *>::iterator _end);

    size_t getCurrentQueueSize() const;

    size_t getNumberOfThreads() const;

    I_Condition &allThreadsAreRunning() const;

protected:

    void popRequest(Task *&_output);

    void popRequest(std::list<Task *> &_output);

    /// @}

    /// @name 'Structors
    /// @{
public:

    ThreadPool(unsigned const _numThreads = 1,
               I_Condition *const _pQueueIsEmpty = 0,
               bool const _waitForEmptyQueue = true,
               bool const _startNow = true);

    virtual ~ThreadPool();
    /// @}

    /// @name Member Variables
    /// @{
private:
    std::vector<pThread_type> m_threadPool;
    unsigned m_numberOfNotRunningThreads;
    /// True if the threads have been started (or are being started)
    bool m_threadsStarted;
    /// Number of threads in this thread pool
    unsigned m_numberOfThreads;
    I_Condition *m_pAllThreadsAreRunning;
    SynchronizedQueue<Task *> m_requestQueue;

    /// Blocks submission of new work requests
    volatile bool m_amPreparingToStop;

    volatile bool m_amStopping;

    /// Cancels unfinished work requests, and stops the pool threads
    volatile bool m_amStoppingAsSoonAsPossible;

    /// Destructor watis for all work requests to finish
    const bool m_waitForEmptyQueue;

    mutable I_Mutex* m_pSpinLock;

    friend class ThreadPoolRunnable;
    /// }

};  // class ThreadPool

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
bool
ThreadPool::pushRequest(Task *const _pInput) {
    return m_amPreparingToStop ? false : m_requestQueue.push(_pInput);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
bool
ThreadPool::pushRequest(std::list<Task *> &_input) {
    return m_amPreparingToStop ? false : m_requestQueue.push(_input);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
bool
ThreadPool::pushRequest(std::list<Task *> &_input, std::list<Task *>::iterator _begin,
                        std::list<Task *>::iterator _end) {
    return m_amPreparingToStop ? false : m_requestQueue.push(_input, _begin, _end);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
bool
ThreadPool::pushRequest(std::list<Task *>::iterator _begin, std::list<Task *>::iterator _end) {
    return m_amPreparingToStop ? false : m_requestQueue.push(_begin, _end);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
size_t
ThreadPool::getCurrentQueueSize() const {
    return m_requestQueue.size();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
size_t
ThreadPool::getNumberOfThreads() const {
    return m_threadPool.size();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
ThreadPool::popRequest(Task *&_output) {
    m_requestQueue.pop(_output);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
ThreadPool::popRequest(std::list<Task *> &_output) {
    m_requestQueue.popAll(_output);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
I_Condition &
ThreadPool::allThreadsAreRunning() const {
    return *m_pAllThreadsAreRunning;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Thread
}   // namespace Core
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
