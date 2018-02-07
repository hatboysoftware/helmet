//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indieHelmet.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once 

#include <Helmet/Core/Thread/ConditionFactory.hpp>
#include <Helmet/Core/Thread/MutexFactory.hpp>
#include <Helmet/Core/Thread/CriticalSection.hpp>
#include <Helmet/Core/Thread/I_Condition.hpp>

#include <list>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Thread {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// @brief Thread Safe First In First Out Queue
/// 
/// Usage Pattern:
///     Multi-threaded producer, multi-threaded consumer
///     Used as a first in first out queue with push() for individual items 
///     and lists of items, as well as pop() for individual items and lists
///     of items.  pop() blocks on an empty queue and wakes up after an item
///     becomes available.
///     
/// @param value_type MUST be copyable and assignable and have a default constructor
template<typename _Value_type = void *>
class SynchronizedQueue
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef _Value_type value_type;
    typedef std::list<value_type> queue_type;
    typedef typename queue_type::size_type size_type;
    typedef typename queue_type::iterator iterator;
    /// @}

    /// @name SynchronizedQueue implementation
    /// @{
public:
    /// Starting to close the queue:
    ///     enable user's optional m_pQueueIsEmpty assertConditionion.
    ///     push() is still enabled.
    void closing();

    /// same as closing(), AND disables any subsequent push()
    void close();

    /// returns true iff _Input was accepted
    bool push(const value_type &_input);

    /// side effect: _input will be empty upon return.
    /// _input MUST be local to this thread
    /// @return true iff _input was accepted
    bool push(queue_type &_input);

    /// side-effect: the interval [_begin,_end) is removed from _rInput.
    bool push(queue_type &_input, iterator const _begin, iterator const _end);

    /// side-effect: none.
    bool push(iterator const _begin, iterator const _end);

    /// takes 1 item from the front of the queue, assigns to _output
    void pop(value_type &_output);

    /// takes 1 item from the front of the queue, appends to end of _output
    void pop(queue_type &_output);

    /// @brief Pop All
    /// takes everything presently in the queue, REPLACES previous content of _output
    void popAll(queue_type &_output);
    /// @}

    /// @name Debugging Support
    /// If you do not know why these methods should only be used for debugging
    /// then please don't use them at all.
    /// @{
public:
    /// For debugging use only.
    /// If you don't know why that is so, then don't use this function at all.
    const value_type &front() const;

    /// For debugging use only.
    /// If you don't know why that is so, then don't use this function at all.
    value_type &front();

    /// For debugging use only.
    /// If you don't know why that is so, then don't use this function at all.
    const value_type &back() const;

    /// For debugging use only.
    /// If you don't know why that is so, then don't use this function at all.
    value_type &back();

    /// For debugging use only.
    /// If you don't know why that is so, then don't use this function at all.
    bool isEmpty() const;

    /// For debugging use only.
    /// If you don't know why that is so, then don't use this function at all.
    size_type size() const;
    /// @}

    /// @name 'Structors
    /// @{
public:
    SynchronizedQueue(I_Condition *const _pQueueIsEmpty = 0,
                      const bool _assertConditionEmptyOnlyAfterClose = false,
                      const bool _ThereIsOnlyOneConsumer = false);

    virtual ~SynchronizedQueue();
    /// @}

    /// @name Member Variables
    /// @{
private:
    I_Condition *m_pQueueIsNotEmpty;     // assertConditioning !m_Queue.empty()
    I_Condition *m_pQueueIsEmpty;        // assertConditioning m_Queue.empty(); optional, managed by the queue for the user (e.g. as part of shutdown logic), but not used by the queue itself.
    I_Condition *m_pQueueIsEmpty_closed; // ... value of above, to be used only after close()
    I_Mutex *m_pConsumerLock;        // for serializing consumers
    I_Mutex *m_pConsistencyLock;
    queue_type m_Queue;
    volatile bool m_AcceptsNewInput;      // true if push() is enabled, false if the queue has been closed.
    /// @}

};  // template class SynchronizedQueue <_Value_type>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename _Value_type>
inline
SynchronizedQueue<_Value_type>::SynchronizedQueue(I_Condition *const _pQueueIsEmpty /*=0*/,
                                                  const bool _AssertEmptyOnlyAfterClose /*=false*/,
                                                  const bool _ThereIsOnlyOneConsumer /*=false*/)
:   m_pQueueIsNotEmpty(ConditionFactory::create())    // initially false (reset); not-not-empty == empty
,   m_pQueueIsEmpty(_AssertEmptyOnlyAfterClose ? 0
        : _pQueueIsEmpty)           // if owner only Require()s his assertion AFTER close() or closing(), then it's more efficient to set _AssertEmptyOnlyAfterClose
,   m_pQueueIsEmpty_closed(_pQueueIsEmpty)
,   m_pConsumerLock(_ThereIsOnlyOneConsumer ? 0
        : MutexFactory::create()) // if there is only 1 thread doing pop(), then it's more efficient to set _ThereIsOnlyOneConsumer
,   m_pConsistencyLock(MutexFactory::create())
,   m_Queue()
,   m_AcceptsNewInput(true)
{
    if (_pQueueIsEmpty) {
        if (_AssertEmptyOnlyAfterClose) {
            _pQueueIsEmpty->retractCondition();
        } else {
            _pQueueIsEmpty->assertCondition();
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename _Value_type>
inline
SynchronizedQueue<_Value_type>::~SynchronizedQueue()
{
    ConditionFactory::destroy(m_pQueueIsNotEmpty);
    MutexFactory::destroy(m_pConsumerLock);
    MutexFactory::destroy(m_pConsistencyLock);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename _Value_type>
inline
void
SynchronizedQueue<_Value_type>::closing()
{
    CriticalSection ForConsistency(*m_pConsistencyLock);
    if ((0 == m_pQueueIsEmpty) && (0 != (m_pQueueIsEmpty = m_pQueueIsEmpty_closed))) {
        if (m_Queue.empty()) {
            m_pQueueIsEmpty->assertCondition();
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename _Value_type>
inline
void
SynchronizedQueue<_Value_type>::close()
{
    CriticalSection ForConsistency(*m_pConsistencyLock);
    if ((0 == m_pQueueIsEmpty) && (0 != (m_pQueueIsEmpty = m_pQueueIsEmpty_closed))) {
        if (m_Queue.empty()) {
            m_pQueueIsEmpty->assertCondition();
        }
    }
    m_AcceptsNewInput = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename _Value_type>
inline
bool
SynchronizedQueue<_Value_type>::push(const value_type &_Input)
{
    CriticalSection ForConsistency(*m_pConsistencyLock);
    if (m_AcceptsNewInput) {
        if (m_Queue.empty())                                        // only on the empty-to-not-empty transition
        {
            m_pQueueIsNotEmpty->assertCondition();
            if (m_pQueueIsEmpty) m_pQueueIsEmpty->retractCondition();
        }
        m_Queue.push_back(_Input);
        return true;
    } else {
        // _Input was ignored (not queued)
        return false;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename _Value_type>
inline
bool
SynchronizedQueue<_Value_type>::push(queue_type &_rInput)
{
    CriticalSection ForConsistency(*m_pConsistencyLock);
    if (m_AcceptsNewInput) {
        // Note:  _rInput will be cleared.
        if (!_rInput.empty()) {
            if (m_Queue.empty())                                    // only on the empty-to-not-empty transition
            {
                m_pQueueIsNotEmpty->assertCondition();
                if (m_pQueueIsEmpty) m_pQueueIsEmpty->retractCondition();
            }
            m_Queue.splice(m_Queue.end(), _rInput);                 // _rInput is clear()ed by the splice()
        }
        return true;
    } else {
        // _rInput was ignored (not queued)
        return false;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename _Value_type>
inline
bool
SynchronizedQueue<_Value_type>::push(queue_type &_rInput,
                                     iterator const _begin,
                                     iterator const _end)
{                                                                   // _rInput MUST be local to this thread
    CriticalSection ForConsistency(*m_pConsistencyLock);
    if (m_AcceptsNewInput) {
        // Note:  _rInput is modified:  all push()ed values are removed from it.
        if (_begin != _end) {
            if (m_Queue.empty())                                    // only on the empty-to-not-empty transition
            {
                m_pQueueIsNotEmpty->assertCondition();
                if (m_pQueueIsEmpty) m_pQueueIsEmpty->retractCondition();
            }
            m_Queue.splice(m_Queue.end(), _rInput, _begin, _end);
        }
        return true;
    } else {
        // _rInput was ignored (not queued)
        return false;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename _Value_type>
inline
bool
SynchronizedQueue<_Value_type>::push(iterator const _begin, iterator const _end)
{
    CriticalSection ForConsistency(*m_pConsistencyLock);
    if (m_AcceptsNewInput) {
        // Note:  this variant has no side effects, the input list is unchanged.
        if (_begin != _end) {
            if (m_Queue.empty())                                    // only on the empty-to-not-empty transition
            {
                m_pQueueIsNotEmpty->assertCondition();
                if (m_pQueueIsEmpty) m_pQueueIsEmpty->retractCondition();
            }
            m_Queue.insert(m_Queue.end(), _begin, _end);            // makes a copy of [_begin, _end)
        }
        return true;
    } else {
        // input collection was ignored (not queued)
        return false;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename _Value_type>
inline
void
SynchronizedQueue<_Value_type>::pop(value_type &_rOutput)
{                                                                   // _rOutput MUST be local to this thread
    CriticalSection ToSerializePop(m_pConsumerLock);
    CriticalSection ForConsistency(*m_pConsistencyLock);

    if (m_Queue.empty()) {
        CriticalSection_ExemptionZone ToAllowPush(ForConsistency);
        m_pQueueIsNotEmpty->requireCondition();
    }

    // The std::queue<> API is inherently single-threaded.
    // For multi-threaded code, the front() and pop_front() need to be an atomic combination,
    // i.e. 1 API call (like we do it), not 2 (like STL):
    _rOutput = m_Queue.front();
    m_Queue.pop_front();

    if (m_Queue.empty())                                            // only on the not-empty-to-empty transition
    {
        m_pQueueIsNotEmpty->retractCondition();
        if (m_pQueueIsEmpty) m_pQueueIsEmpty->assertCondition();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename _Value_type>
inline
void
SynchronizedQueue<_Value_type>::pop(queue_type &_rOutput)
{                                                                   // _rOutput MUST be local to this thread
    CriticalSection ToSerializePop(m_pConsumerLock);
    CriticalSection ForConsistency(*m_pConsistencyLock);

    if (m_Queue.empty()) {
        CriticalSection_ExemptionZone ToAllowPush(ForConsistency);
        m_pQueueIsNotEmpty->requireCondition();
    }

    // The std::queue<> API is inherently single-threaded.
    // For multi-threaded code, the splice() together with the end() and begin() iterators need to be an atomic combination,
    // i.e. 1 API call (like we do it), not 3 (like STL):
    _rOutput.splice(_rOutput.end(), m_Queue, m_Queue.begin());

    if (m_Queue.empty()) {
        m_pQueueIsNotEmpty->retractCondition();
        if (m_pQueueIsEmpty) m_pQueueIsEmpty->assertCondition();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename _Value_type>
inline
void
SynchronizedQueue<_Value_type>::popAll(queue_type &_rOutput)
{                                                                   // _rOutput MUST be local to this thread
    _rOutput.clear();                                               // may fire dtors; do this BEFORE taking locks

    CriticalSection ToSerializePop(m_pConsumerLock);
    CriticalSection ForConsistency(*m_pConsistencyLock);

    if (m_Queue.empty()) {
        CriticalSection_ExemptionZone ToAllowPush(ForConsistency);
        m_pQueueIsNotEmpty->requireCondition();
    }

    // The std::queue<> API is inherently single-threaded.
    // For multi-threaded code, the swap() and clear() need to be an atomic combination,
    // i.e. 1 API call (like we do it), not 2 (like STL):
    _rOutput.swap(m_Queue);

    // assertCondition(m_Queue.empty());
    m_pQueueIsNotEmpty->retractCondition();
    if (m_pQueueIsEmpty) m_pQueueIsEmpty->assertCondition();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename _Value_type>
inline
const typename SynchronizedQueue<_Value_type>::value_type &
SynchronizedQueue<_Value_type>::front() const
{
    CriticalSection ForConsistency(*m_pConsistencyLock);
    return m_Queue.front();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename _Value_type>
inline
typename SynchronizedQueue<_Value_type>::value_type &
SynchronizedQueue<_Value_type>::front()
{
    CriticalSection ForConsistency(*m_pConsistencyLock);
    return m_Queue.front();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename _Value_type>
inline
const typename SynchronizedQueue<_Value_type>::value_type &
SynchronizedQueue<_Value_type>::back() const
{
    CriticalSection ForConsistency(*m_pConsistencyLock);
    return m_Queue.back();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename _Value_type>
inline
typename SynchronizedQueue<_Value_type>::value_type &
SynchronizedQueue<_Value_type>::back()
{
    CriticalSection ForConsistency(*m_pConsistencyLock);
    return m_Queue.back();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename _Value_type>
inline
bool
SynchronizedQueue<_Value_type>::isEmpty() const
{
    return m_Queue.empty();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename _Value_type>
inline
typename SynchronizedQueue<_Value_type>::size_type
SynchronizedQueue<_Value_type>::size() const
{
    return m_Queue.size();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Thread
}   // namespace Core
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
