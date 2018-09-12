/*
 * ------------------------------------------------------------------------
 *  Name:   FWinThread.h
 *  Desc:   本文件为引擎封装了WINDOWS平台下的线程相关函数。
 *  Author: Yish
 *  Date:   2014/3/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_WINTHREAD_H__
#define __FAIRY_WINTHREAD_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FThread.h"
#include <windows.h>

///////////////////////////////////////////////////////////////////////////

/** Atomic number.
*/
class FWinThreadAtomic : public FThreadAtomic
{
public:
	FWinThreadAtomic(int iValue)
		: m_iValue(iValue)
	{
	}

	void Reset();

	int GetValue() const { return m_iValue; }

	int Fetch_Add(int iNum);
	int Fetch_Set(int iNum);
	int Fetch_CompareSet(int iNum, int iComparand);

	int Fetch_Increment();
	int Fetch_Decrement();

protected:
	__declspec(align(4)) volatile int m_iValue;
};

/** Mutex object
*/
class FWinThreadMutex : public FThreadMutex
{
public:
	FWinThreadMutex();
	~FWinThreadMutex();

	void Lock();
	void Unlock();

protected:
	CRITICAL_SECTION m_cs;
};

/** Condition object.
*/
class FWinThreadCond : public FThreadCond
{
public:
	FWinThreadCond();
	~FWinThreadCond();

	void Wait(FThreadMutex* pMutex, int nTimeout /* = -1 */);
	void Signal();
	void Broadcast();

protected:
	HANDLE m_hEvent;
};

/** Semaphore object.
*/
class FWinThreadSemaphore : public FThreadSemaphore
{
public:
	FWinThreadSemaphore(int initValue);
	~FWinThreadSemaphore();

	void Wait();
	void NotifyOne();
	void Notify(int n);

protected:
	HANDLE m_hSem;
};

/** Thread object.
*/
class FWinThread : public FThread
{
	friend DWORD WINAPI _winThreadProc(LPVOID lpParameter);

public:
	FWinThread();
	virtual ~FWinThread();

	// Initialize the thread object.
	bool Init(LPFNFTHREADWORKFUNC pWorkFunc, void* pParam, bool bDelAfterComplete, EThreadPriority priority);

	// Set the priority
	virtual void SetPriority(EThreadPriority priority);
	// Get the thread id.
	virtual void* GetThreadID() const { return (void*)m_hThread; }

protected:
	HANDLE m_hThread;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_WINTHREAD_H__