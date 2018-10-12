/*
 * ------------------------------------------------------------------------
 *  Name:   FThread.h
 *  Desc:   本文件定义了线程对象的封装。
 *  Author: Yish
 *  Date:   2011/11/19
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_THREAD_H__
#define __FAIRY_THREAD_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

/** Atomic number.
*/
class FThreadAtomic
{
public:
    virtual ~FThreadAtomic() {}

	virtual void Reset() = 0;

	virtual int Fetch_Add(int iNum) = 0;
	virtual int Fetch_Set(int iNum) = 0;
	virtual int Fetch_CompareSet(int iNum, int iComparand) = 0;

	virtual int Fetch_Increment() = 0;
	virtual int Fetch_Decrement() = 0;

	virtual int GetValue() const = 0;

	static FThreadAtomic* Create(int iValue);
};

/** Mutex object.
*/
class FThreadMutex
{
public:
    virtual ~FThreadMutex() {}
    
	virtual void Lock() = 0;
	virtual void Unlock() = 0;

	static FThreadMutex* Create();
};

/** Spin lock object.
*/
class FThreadSpin
{
public:
	FThreadSpin();
	virtual ~FThreadSpin();

	void Lock();
	void Unlock();

protected:
	FThreadAtomic* m_pAtomic;
};

/** Lock the scope with specified mutex.
*/
class FScopedLock
{
public:
	FScopedLock(FThreadMutex* pMutex)
		: m_pMutex(pMutex)
	{
        FASSERT(m_pMutex);
		m_pMutex->Lock();
	}

	~FScopedLock()
	{
		m_pMutex->Unlock();
	}

protected:
	FThreadMutex* m_pMutex;
};

/** Lock the scope with the specified spin lock.
*/
class FScopedSpinLock
{
public:
	FScopedSpinLock(FThreadSpin* pSpinLock)
		: m_pSpinLock(pSpinLock)
	{
		FASSERT(m_pSpinLock);
		m_pSpinLock->Lock();
	}

	~FScopedSpinLock()
	{
		m_pSpinLock->Unlock();
	}

protected:
	FThreadSpin* m_pSpinLock;
};

/** Condition object.
*/
class FThreadCond
{
public:
    virtual ~FThreadCond() {}
    
	virtual void Wait(FThreadMutex* pMutex, int nTimeout = -1) = 0;
	virtual void Signal() = 0;
	virtual void Broadcast() = 0;	// FIXME,  Invalid for windows platform.

	static FThreadCond* Create();
};

/** Semaphore object.
 */
class FThreadSemaphore
{
public:
    virtual ~FThreadSemaphore() {}
    
    virtual void Wait() = 0;
    virtual void NotifyOne() = 0;
    virtual void Notify(int n) = 0;
    
    static FThreadSemaphore* Create(int initValue = 0);
};

// Work function of the thread.
typedef int (*LPFNFTHREADWORKFUNC) (void* pParam);

/** Thread object.
*/
class FThread
{
public:
    
    enum EThreadState
    {
        STATE_CREATED = 0,
        STATE_RUNNING,
        STATE_STOPPING,
        STATE_STOPPED,
    };
    
    enum EThreadResult
    {
        RESULT_COMPLETE = 0,
        RESULT_CONTINUE,
    };
    
    enum EThreadPriority
    {
        PRIORITY_NORMAL,
        PRIORITY_LOW,
        PRIORITY_HIGH,
    };
    
public:
    FThread();
    virtual ~FThread();
    
    // Create the thread
    static FThread* Create(LPFNFTHREADWORKFUNC pWorkFunc, void* pParam, bool bDeleteAfterComplete = true, EThreadPriority iPriority = PRIORITY_NORMAL);
    
	// Suspend the thread.
	virtual void Suspend();
	// Resume the thread.
	virtual void Resume();
    // Terminate the thread.
    virtual void Terminate();
    // Wait the thread stop.
    virtual void Wait();

    // Get the thread state.
    EThreadState GetState() const;
	// Is the thread running ?
    bool IsRunning() const;
    // Is the thread suspending ?
    bool IsSuspending() const;
    
    // Get the thread id.
    virtual void* GetThreadID() const = 0;
    // Set the priority
    virtual void SetPriority(EThreadPriority priority) = 0;
    
    // Is main thread.
    bool IsMainThread() const { return (GetThreadID() == s_mainThreadID); }
    // Is active thread.
    bool IsActiveThread() const { return (GetThreadID() == GetCurrentThreadID()); }
    
    // Get the priority
    EThreadPriority GetPriority() const { return m_Priority; }
    
    // Record the main thread ID.
    static void RecordMainThreadID();
    // Get the current thread ID.
    static void* GetCurrentThreadID();
    // Current thread is the main thread ?
    static bool IsInMainThread();

protected:
    bool m_bDeleteAfterComplete;        // Delete the thread object after complete the work ?
    EThreadState m_State;               // Current state of the thread.
    LPFNFTHREADWORKFUNC m_pWorkFunc;    // Work function of the thread.
    void* m_pParam;                     // Parameter of the work function.
    EThreadPriority m_Priority;         // Priority of the thread.
    volatile bool m_bSuspend;           // Whether the current thread is suspended

    FThreadMutex* m_pMutexState;        // Lock the some member variables.
    FThreadMutex* m_pMutexSuspend;      // Mutex object for suspend flag.
    FThreadCond* m_pCondStop;           // Wait the thread stopped.
    FThreadCond* m_pCondSuspend;        // Condition for the suspend flag.
    
    static void* s_mainThreadID;        // Id of the main thread.
    
    // Run the work function.
    void* Run();
    // Will the thread stop ?
    bool WillStop();
    // The thread has stopped!
    void DoStop();
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_THREAD_H__