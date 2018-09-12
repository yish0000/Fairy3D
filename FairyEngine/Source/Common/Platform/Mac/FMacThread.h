/*
 * ------------------------------------------------------------------------
 *  Name:   FMacThread.h
 *  Desc:   Thread on MacOS or IOS platform.
 *  Author: Yish
 *  Date:   2014/3/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_MACTHREAD_H__
#define __FAIRY_MACTHREAD_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FThread.h"
#include <pthread.h>
#include <semaphore.h>

///////////////////////////////////////////////////////////////////////////

/** Atomic number.
*/
class FMacThreadAtomic : public FThreadAtomic
{
public:
	FMacThreadAtomic(int iValue)
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
	volatile int m_iValue;
};

/** Mutex object
*/
class FMacThreadMutex : public FThreadMutex
{
    friend class FMacThreadCond;
    
public:
	FMacThreadMutex();
	~FMacThreadMutex();

	void Lock();
	void Unlock();

protected:
	pthread_mutex_t	m_mutex;
};

/** Condition object
 */
class FMacThreadCond : public FThreadCond
{
public:
    FMacThreadCond();
    virtual ~FMacThreadCond();
    
    void Wait(FThreadMutex* pMutex, int nTimeout);
    void Signal();
    void Broadcast();
    
protected:
    pthread_cond_t m_cond;
};

/** Semaphore object.
 */
class FMacThreadSemaphore : public FThreadSemaphore
{
public:
    FMacThreadSemaphore(int initValue);
    virtual ~FMacThreadSemaphore();
    
    void Wait();
    void NotifyOne();
    void Notify(int n);
    
protected:
    sem_t m_sem;
};

/** Thread object.
 */
class FMacThread : public FThread
{
    friend void* _macThreadProc(void* pArgs);
    
public:
    FMacThread();
    virtual ~FMacThread();

    // Initialize the thread object.
    bool Init(LPFNFTHREADWORKFUNC pWorkFunc, void* pParam, bool bDelAfterComplete, EThreadPriority priority);
    
    // Set the priority
    virtual void SetPriority(EThreadPriority priority);
    // Get the thread id.
	virtual void* GetThreadID() const { return (void*)m_hThread; }
    
protected:
    pthread_t m_hThread;    
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_MACTHREAD_H__