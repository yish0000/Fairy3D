/*
 * ------------------------------------------------------------------------
 *  Name:   FMacThread.cpp
 *  Desc:   Thread on MacOS or IOS platform.
 *  Author: Yish
 *  Date:   2014/3/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FMacThread.h"
#include "FLogManager.h"
#include <sys/time.h>

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FMacThreadAtomic
//  
///////////////////////////////////////////////////////////////////////////

int FMacThreadAtomic::Fetch_Add(int iNum)
{
	return __sync_fetch_and_add(&m_iValue, iNum);
}

int FMacThreadAtomic::Fetch_Set(int iNum)
{
	return __sync_lock_test_and_set(&m_iValue, iNum);
}

int FMacThreadAtomic::Fetch_CompareSet(int iNum, int iComparand)
{
	return __sync_val_compare_and_swap(&m_iValue, iComparand, iNum);
}

int FMacThreadAtomic::Fetch_Increment()
{
	return __sync_add_and_fetch(&m_iValue, 1);
}

int FMacThreadAtomic::Fetch_Decrement()
{
	return __sync_sub_and_fetch(&m_iValue, 1);
}

void FMacThreadAtomic::Reset()
{
	__sync_lock_release(&m_iValue);
}

FThreadAtomic* FThreadAtomic::Create(int iValue)
{
	return new FMacThreadAtomic(iValue);
}

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FMacThreadMutex
//  
///////////////////////////////////////////////////////////////////////////

FMacThreadMutex::FMacThreadMutex()
{
	pthread_mutexattr_t ma;
	pthread_mutexattr_init(&ma);
	pthread_mutexattr_settype(&ma, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m_mutex, &ma);
	pthread_mutexattr_destroy(&ma);
}

FMacThreadMutex::~FMacThreadMutex()
{
    pthread_mutex_destroy(&m_mutex);
}

void FMacThreadMutex::Lock()
{
    pthread_mutex_lock(&m_mutex);
}

void FMacThreadMutex::Unlock()
{
    pthread_mutex_unlock(&m_mutex);
}

FThreadMutex* FThreadMutex::Create()
{
	return new FMacThreadMutex();
}

///////////////////////////////////////////////////////////////////////////
//
//  Implement class FMacThreadCond
//
///////////////////////////////////////////////////////////////////////////

FMacThreadCond::FMacThreadCond()
{
    pthread_cond_init(&m_cond, NULL);
}

FMacThreadCond::~FMacThreadCond()
{
    pthread_cond_destroy(&m_cond);
}

void FMacThreadCond::Wait(FThreadMutex* pMutex, int nTimeout)
{
    FMacThreadMutex* pMacMutex = dynamic_cast<FMacThreadMutex*>(pMutex);
    if( !pMacMutex )
    {
        FASSERT(!"pMutex is not a FMacThreadMutex!");
        return;
    }
    
    if (nTimeout <= 0)
    {
        pthread_cond_wait(&m_cond, &pMacMutex->m_mutex);
    }
    else
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        
        uint64 nowInMicroSeconds = (int64)tv.tv_sec * 1000000 + tv.tv_usec;
        nowInMicroSeconds += nTimeout * 1000;
        struct timespec spec;
        spec.tv_sec = nowInMicroSeconds / 1000000;
        spec.tv_nsec = (nowInMicroSeconds - 1000000 * spec.tv_sec) * 1000;
        
        int retcode = pthread_cond_timedwait(&m_cond, &pMacMutex->m_mutex, &spec);
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
        if( retcode != 0 && retcode != 10060 )
#else
        if( retcode != 0 && retcode != ETIMEDOUT )
#endif
        {
            FLOG_ERRORF("FMacThreadCond::Wait, pthread_cond_timedwait failed (retcode=%d)", retcode);
        }
    }
}

void FMacThreadCond::Signal()
{
    pthread_cond_signal(&m_cond);
}

void FMacThreadCond::Broadcast()
{
    pthread_cond_broadcast(&m_cond);
}

///////////////////////////////////////////////////////////////////////////
//
//  Implement class FMacThreadSemaphore
//
///////////////////////////////////////////////////////////////////////////

FMacThreadSemaphore::FMacThreadSemaphore(int initValue)
{
    if( 0 != sem_init(&m_sem, 0, initValue) )
    {
        FLOG_ERROR("FMacThreadSemaphore::FMacThreadSemaphore, sem_init failed!");
    }
}

FMacThreadSemaphore::~FMacThreadSemaphore()
{
    if( 0 != sem_close(&m_sem) )
    {
        FLOG_ERROR("FMacThreadSemaphore::~FMacThreadSemaphore, sem_close failed!");
    }
}

void FMacThreadSemaphore::Wait()
{
    if( 0 != sem_wait(&m_sem) )
    {
        FLOG_ERROR("FMacThreadSemaphore::Wait, sem_wait failed!");
    }
}

void FMacThreadSemaphore::NotifyOne()
{
    if( 0 != sem_post(&m_sem) )
    {
        FLOG_ERROR("FMacThreadSemaphore::NotifyOne, sem_post failed!");
    }
}

void FMacThreadSemaphore::Notify(int n)
{
	for (int i = 0; i < n; i++)
	{
		if (0 != sem_post(&m_sem))
		{
			FLOG_ERROR("FMacThreadSemaphore::Notify, sem_post failed!");
		}
	}
}

FThreadSemaphore* FThreadSemaphore::Create(int initValue)
{
	return new FMacThreadSemaphore(initValue);
}

///////////////////////////////////////////////////////////////////////////
//
//  Implement class FMacThread
//
///////////////////////////////////////////////////////////////////////////

// Thread proc function.
void* _macThreadProc(void* pArgs)
{
    FMacThread* pThread = (FMacThread*)pArgs;
    return pThread->Run();
}

// Get the id of current thread.
void* FThread::GetCurrentThreadID()
{
    return (void*)pthread_self();
}

// Create a thread object.
FThread* FThread::Create(LPFNFTHREADWORKFUNC pWorkFunc, void *pParam, bool bDeleteAfterComplete, EThreadPriority priority)
{
    FMacThread* pThread = new FMacThread();
    if( !pThread )
        return NULL;
    if( !pThread->Init(pWorkFunc, pParam, bDeleteAfterComplete, priority) )
        return NULL;
    return pThread;
}

/** Constructor.
 */
FMacThread::FMacThread() : m_hThread(NULL)
{
}

/** Destructor.
 */
FMacThread::~FMacThread()
{
}

/** Initialize the thread.
 */
bool FMacThread::Init(LPFNFTHREADWORKFUNC pWorkFunc, void *pParam, bool bDelAfterComplete, EThreadPriority priority)
{
    if( !pWorkFunc )
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
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (0 != pthread_create(&m_hThread, &attr, _macThreadProc, this))
    {
        FLOG_ERROR("FMacThread::Init, pthread_create failed");
        pthread_attr_destroy(&attr);
        return false;
    }
    pthread_attr_destroy(&attr);
    
    // Set the priority.
    if( priority != PRIORITY_NORMAL )
        SetPriority(priority);
    
    // Start the thread routine.
    Resume();
    return true;
}

/** Change the priority of the thread.
 */
void FMacThread::SetPriority(EThreadPriority priority)
{
    if( m_Priority != priority )
    {
        // Get min and max priority of current thread's policy
        sched_param param;
        int policy;
        pthread_getschedparam(m_hThread, &policy, &param);
        
        int min = sched_get_priority_min(policy);
        int max = sched_get_priority_max(policy);
        
        // Set more proper priority value
        int threadPriority = min + (max - min) / 2;
        switch (priority)
        {
        case PRIORITY_HIGH: threadPriority = min + (max - min) * 3 / 4;	break;
        case PRIORITY_LOW:  threadPriority = min + (max - min) / 4;		break;
        default: break;
        }
        
        param.sched_priority = threadPriority;
        pthread_setschedparam(m_hThread, policy, &param);
        
        m_Priority = priority;
    }
}