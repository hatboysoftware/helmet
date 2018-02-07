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

#include <Helmet/Core/Thread/I_Mutex.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Core {
namespace Thread {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class CriticalSection
:   public ::boost::noncopyable {
    /// @name Types
    /// @{
public:
    /// @}

    /// @name Implementation
    /// @{
public:
    void suspend();

    void resume();
    /// @}

    /// @name 'Structors
    /// @{
public:
    CriticalSection(I_Mutex &_Mutex);

    CriticalSection(I_Mutex *_pMutex);

    /// Constructor that uses requires_synchronization trait
    CriticalSection(I_Mutex &_Mutex, const boost::true_type &);

    /// Constructor that uses requires_synchronization trait
    CriticalSection(I_Mutex *_pMutex, const boost::true_type &);

    /// Constructor that uses requires_synchronization trait
    CriticalSection(I_Mutex &_Mutex, const boost::false_type &);

    /// Constructor that uses requires_synchronization trait
    CriticalSection(I_Mutex *_pMutex, const boost::false_type &);

    virtual ~CriticalSection();
    /// @}

    /// @name Member variables
    /// @{
private:
    I_Mutex *m_pMutex;

    friend class CriticalSection_ExemptionZone;
    /// @}

};    // class CriticalSection

class HELMET_CORE_DLL_LINK CriticalSection_ExemptionZone
:   public ::boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name Implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             CriticalSection_ExemptionZone(CriticalSection& _enclosingCritSec);
    virtual ~CriticalSection_ExemptionZone();
    /// @}

    /// @name Member variables
    /// @{
private:
    CriticalSection &m_enclosingCritSec;
    /// @}

};    // class CriticalSection_ExemptionZone

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection::CriticalSection(I_Mutex &_Mutex)
:   m_pMutex(&_Mutex)
{
    m_pMutex->acquire();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection::CriticalSection(I_Mutex *_pMutex)
:   m_pMutex(_pMutex)
{
    if (m_pMutex) m_pMutex->acquire();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection::CriticalSection(I_Mutex &_Mutex, const boost::true_type &)
:   m_pMutex(&_Mutex)
{
    m_pMutex->acquire();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection::CriticalSection(I_Mutex *_pMutex, const boost::true_type &)
:   m_pMutex(_pMutex)
{
    if (m_pMutex) m_pMutex->acquire();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection::CriticalSection(I_Mutex &_Mutex, const boost::false_type &)
:   m_pMutex(NULL)
{
    // Not required to do anything
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection::CriticalSection(I_Mutex *_pMutex, const boost::false_type &)
:   m_pMutex(NULL)
{
    // Not required to do anything
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection::~CriticalSection()
{
    if (m_pMutex) m_pMutex->release();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
CriticalSection::suspend()
{
    if (m_pMutex) m_pMutex->suspend();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
CriticalSection::resume()
{
    if (m_pMutex) m_pMutex->resume();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection_ExemptionZone::CriticalSection_ExemptionZone(CriticalSection &_enclosingCritSec)
:   m_enclosingCritSec(_enclosingCritSec)
{
    // suspend the original critical section
    m_enclosingCritSec.suspend();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CriticalSection_ExemptionZone::~CriticalSection_ExemptionZone()
{
    // resume the original critical section
    m_enclosingCritSec.resume();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Thread
}   // namespace Core
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
