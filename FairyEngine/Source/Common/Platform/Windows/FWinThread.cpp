/*
 * ------------------------------------------------------------------------
 *  Name:   FWinThread.cpp
 *  Desc:   本文件为引擎实现了WINDOWS平台下线程相关函数。
 *  Author: Yish
 *  Date:   2014/3/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FWinThread.h"
#include "FLogManager.h"

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FWinThreadAtomic
//  
///////////////////////////////////////////////////////////////////////////

int FWinThreadAtomic::Fetch_Add(int iNum)
{
	return (int)InterlockedExchangeAdd((LPLONG)&m_iValue, (LONG)iNum);
}

int FWinThreadAtomic::Fetch_Set(int iNum)
{
	return (int)InterlockedExchange((LPLONG)&m_iValue, (LONG)iNum);
}

int FWinThreadAtomic::Fetch_CompareSet(int iNum, int iComparand)
{
	return (int)InterlockedCompareExchange((LPLONG)&m_iValue, (LONG)iNum, (LONG)iComparand);
}

int FWinThreadAtomic::Fetch_Increment()
{
	return (int)InterlockedIncrement((LPLONG)&m_iValue);
}

int FWinThreadAtomic::Fetch_Decrement()
{
	return (int)InterlockedDecrement((LPLONG)&m_iValue);
}

void FWinThreadAtomic::Reset()
{
	::InterlockedExchange((LPLONG)&m_iValue, 0);
}

FThreadAtomic* FThreadAtomic::Create(int iValue)
{
	return new FWinThreadAtomic(iValue);
}

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FWinThreadMutex
//  
///////////////////////////////////////////////////////////////////////////

FWinThreadMutex::FWinThreadMutex()
{
	::InitializeCriticalSection(&m_cs);
}

FWinThreadMutex::~FWinThreadMutex()
{
	::DeleteCriticalSection(&m_cs);
}

void FWinThreadMutex::Lock()
{
	::EnterCriticalSection(&m_cs);
}

void FWinThreadMutex::Unlock()
{
	::LeaveCriticalSection(&m_cs);
}

FThreadMutex* FThreadMutex::Create()
{
	return new FWinThreadMutex();
}

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FWinThreadCond
//  
///////////////////////////////////////////////////////////////////////////

FWinThreadCond::FWinThreadCond()
{
	m_hEvent = ::CreateEventA(NULL, FALSE, FALSE, NULL);
	FASSERT(m_hEvent && "m_hEvent == null!");
}

FWinThreadCond::~FWinThreadCond()
{
	if (m_hEvent)
	{
		::CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}
}

void FWinThreadCond::Wait(FThreadMutex* pMutex, int nTimeout)
{
	FASSERT(pMutex && "pMutex == null!");
	FASSERT(m_hEvent && "m_hEvent == null!");

	pMutex->Unlock();
	::WaitForSingleObject(m_hEvent, nTimeout > 0 ? nTimeout : INFINITE);
	pMutex->Lock();
}

void FWinThreadCond::Signal()
{
	FASSERT(m_hEvent && "m_hEvent == null!");
	::SetEvent(m_hEvent);
}

void FWinThreadCond::Broadcast()
{
	FASSERT(m_hEvent && "m_hEvent == null!");
	FASSERT(!"FWinThreadCond::Broadcast, NOT Implemented!");
	FLOG_ERROR("FWinThreadCond::Broadcast, This function has not been implemented!");
	//::PulseEvent(m_hEvent);
}

FThreadCond* FThreadCond::Create()
{
	return new FWinThreadCond();
}

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FWinThreadSemaphore
//  
///////////////////////////////////////////////////////////////////////////

FWinThreadSemaphore::FWinThreadSemaphore(int initValue)
{
	m_hSem = ::CreateSemaphoreA(NULL, initValue, 0x7fff, NULL);
	FASSERT(m_hSem && "m_hSem == null!");
}

FWinThreadSemaphore::~FWinThreadSemaphore()
{
	if (m_hSem)
	{
		::CloseHandle(m_hSem);
		m_hSem = NULL;
	}
}

void FWinThreadSemaphore::Wait()
{
	::WaitForSingleObject(m_hSem, INFINITE);
}

void FWinThreadSemaphore::NotifyOne()
{
	::ReleaseSemaphore(m_hSem, 1, NULL);
}

void FWinThreadSemaphore::Notify(int n)
{
	::ReleaseSemaphore(m_hSem, n, NULL);
}

FThreadSemaphore* FThreadSemaphore::Create(int initValue)
{
	return new FWinThreadSemaphore(initValue);
}

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FWinThread
//  
///////////////////////////////////////////////////////////////////////////

// Thread proc for windows platform.
DWORD WINAPI _winThreadProc(LPVOID lpParameter)
{
	FWinThread* pThread = (FWinThread*)lpParameter;
	return (DWORD)pThread->Run();
}

// Get the current thread's id.
void* FThread::GetCurrentThreadID()
{
	return ::GetCurrentThread();
}

// Create a thread object.
FThread* FThread::Create(LPFNFTHREADWORKFUNC pWorkFunc, void *pParam, bool bDeleteAfterComplete, EThreadPriority priority)
{
	FWinThread* pThread = new FWinThread();
	if (!pThread)
		return NULL;
	if (!pThread->Init(pWorkFunc, pParam, bDeleteAfterComplete, priority))
		return NULL;
	return pThread;
}

/** Constructor.
*/
FWinThread::FWinThread() : m_hThread(NULL)
{
}

/** Destructor.
*/
FWinThread::~FWinThread()
{
	if (m_hThread)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}

/** Initialize the thread.
*/
bool FWinThread::Init(LPFNFTHREADWORKFUNC pWorkFunc, void *pParam, bool bDelAfterComplete, EThreadPriority priority)
{
	if (!pWorkFunc)
	{
		FASSERT(!"pWorkFunc must not be null!");
		return false;
	}

	m_pWorkFunc = pWorkFunc;
	m_pParam = pParam;
	m_State = STATE_RUNNING;
	m_bDeleteAfterComplete = bDelAfterComplete;
	m_bSuspend = true;

	// Create the thread.
	m_hThread = ::CreateThread(NULL, 0, _winThreadProc, this, 0, 0);
	if (!m_hThread)
	{
		FLOG_ERROR("FWinThread::Init, Failed to create the thread!");
		return false;
	}

	// Set the priority.
	if (priority != PRIORITY_NORMAL)
		SetPriority(priority);

	// Start the thread routine.
	Resume();
	return true;
}

/** Change the priority of the thread.
*/
void FWinThread::SetPriority(EThreadPriority priority)
{
	if (m_hThread && m_Priority != priority)
	{
		int iThreadPriority = THREAD_PRIORITY_NORMAL;
		switch (priority)
		{
		case PRIORITY_LOW: iThreadPriority = THREAD_PRIORITY_BELOW_NORMAL; break;
		case PRIORITY_HIGH: iThreadPriority = THREAD_PRIORITY_ABOVE_NORMAL; break;
		default: break;
		}

		::SetThreadPriority(m_hThread, iThreadPriority);
		m_Priority = priority;
	}
}