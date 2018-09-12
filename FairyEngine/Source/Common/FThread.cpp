/*
 * ------------------------------------------------------------------------
 *  Name:   FThread.cpp
 *  Desc:   本文件实现了引擎中线程对象的封装。
 *  Author: Yish
 *  Date:   2011/11/19
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FThread.h"
#include "FSysAPI.h"

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FThreadSpin
//  
///////////////////////////////////////////////////////////////////////////

/** Constructor.
*/
FThreadSpin::FThreadSpin()
{
	m_pAtomic = FThreadAtomic::Create(0);
}

/** Destructor.
*/
FThreadSpin::~FThreadSpin()
{
	F_SAFE_DELETE(m_pAtomic);
}

/** Lock.
*/
void FThreadSpin::Lock()
{
	FASSERT(m_pAtomic && "m_pAtomic == null");

#ifdef FAIRY_DEBUG
	if( m_pAtomic->Fetch_Set(1) == 1 )
	{
		int i;
		for(i=0; i<128 && m_pAtomic->Fetch_Set(1) == 1; i++)
			FSysAPI::Sleep(0);

		if (i >= 128)
		{
			for (i = 0; i < 64 && m_pAtomic->Fetch_Set(1) == 1; i++)
				FSysAPI::Sleep(50);

			// Wait too long time!!!
			if (i >= 64)
				FASSERT(0);
		}
	}
#else
	while (m_pAtomic->Fetch_Set(1) == 1)
		FSysAPI::Sleep(0);
#endif
}

/** Unlock.
*/
void FThreadSpin::Unlock()
{
	FASSERT(m_pAtomic && "m_pAtomic == null");
	m_pAtomic->Reset();
}

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FThread
//  
///////////////////////////////////////////////////////////////////////////

// ID of the main thread.
void* FThread::s_mainThreadID = NULL;

// Constructor.
FThread::FThread()
    : m_State(STATE_CREATED), m_pWorkFunc(NULL), m_pParam(NULL), m_Priority(PRIORITY_NORMAL)
    , m_bDeleteAfterComplete(true)
{
    m_pMutexState = FThreadMutex::Create();
    m_pMutexSuspend = FThreadMutex::Create();
    m_pCondStop = FThreadCond::Create();
    m_pCondSuspend = FThreadCond::Create();
}

// Destructor
FThread::~FThread()
{
    F_SAFE_DELETE(m_pMutexState);
    F_SAFE_DELETE(m_pMutexSuspend);
    F_SAFE_DELETE(m_pCondStop);
    F_SAFE_DELETE(m_pCondSuspend);
}

// Get the state of the thread.s
FThread::EThreadState FThread::GetState() const
{
    FScopedLock lock(m_pMutexState);
    return m_State;
}

// Is the thread running ?
bool FThread::IsRunning() const
{
    FScopedLock lock(m_pMutexState);
    return (m_State == STATE_RUNNING);
}

// Record the ID of main thread.
void FThread::RecordMainThreadID()
{
    s_mainThreadID = GetCurrentThreadID();
}

// Current thread is the main thread ?
bool FThread::IsInMainThread()
{
    return GetCurrentThreadID() == s_mainThreadID;
}

// Will the thread stop ?
bool FThread::WillStop()
{
    FScopedLock lock(m_pMutexState);
    return (m_State == STATE_STOPPING);
}

// Run the work function.
void* FThread::Run()
{
    while(1)
    {
        // Suspend the thread ?
        m_pMutexSuspend->Lock();
        while(m_bSuspend)
        {
            m_pCondSuspend->Wait(m_pMutexSuspend);
        }
        m_pMutexSuspend->Unlock();
        
        if( WillStop() )
            break;
        
        int ret = m_pWorkFunc(m_pParam);
        if( ret == RESULT_COMPLETE )
            break;
    }
    
    DoStop();
    
    // Delete the thread object.
    if( m_bDeleteAfterComplete )
    {
        FSysAPI::Sleep(10);
        delete this;
    }
    
    return NULL;
}

// Suspend the thread.
void FThread::Suspend()
{
    if( !IsRunning() )
        return;
    
    FScopedLock lock(m_pMutexSuspend);
    if( !m_bSuspend )
    {
        m_bSuspend = true;
    }
}

// Resume the thread.
void FThread::Resume()
{
    if( !IsRunning() )
        return;
    
    FScopedLock lock(m_pMutexSuspend);
    if( m_bSuspend )
    {
        m_bSuspend = false;
        m_pCondSuspend->Signal();
    }
}

// Is the thread suspending ?
bool FThread::IsSuspending() const
{
    FScopedLock lock(m_pMutexSuspend);
    return m_bSuspend;
}

// The thread has stopped.
void FThread::DoStop()
{
    FScopedLock lock(m_pMutexState);
    m_State = STATE_STOPPED;
    m_pCondStop->Signal();
}

/** Terminate the thread.
 */
void FThread::Terminate()
{
    FScopedLock lock(m_pMutexState);
    if( m_State != STATE_STOPPED )
        m_State = STATE_STOPPING;
    
    if( IsSuspending() )
        Resume();
}

/** Wait for the end of the thread.
 */
void FThread::Wait()
{
    FScopedLock lock(m_pMutexState);
    if( m_State != STATE_STOPPED )
    {
        m_pCondStop->Wait(m_pMutexState);
    }
}