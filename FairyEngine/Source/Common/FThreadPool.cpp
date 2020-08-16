/*
 * ------------------------------------------------------------------------
 *  Name:   FThreadPool.cpp
 *  Desc:   本文件为引擎定义了一个线程池机制。
 *  Author: Yish
 *  Date:   2014/3/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Studio. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FThreadPool.h"
#include "FLogManager.h"

/** Create the thread pool.
*/
FThreadPool* FThreadPool::Create(int iThreadNum)
{
	FThreadPool* pPool = new FThreadPool();
	if (!pPool)
		return NULL;
	if (!pPool->Init(iThreadNum))
		return NULL;
	return pPool;
}

/** Constructor.
*/
FThreadPool::FThreadPool()
{
    m_iThreadCount = 0;
	m_iState = STATE_CREATED;
	m_bPaused = false;
	m_pMutexTasks = FThreadMutex::Create();
    m_pMutexState = FThreadMutex::Create();
	m_pSemaphore = FThreadSemaphore::Create();
    m_pCondStop = FThreadCond::Create();
}

/** Destructor.
*/
FThreadPool::~FThreadPool()
{
	F_SAFE_DELETE(m_pMutexTasks);
    F_SAFE_DELETE(m_pMutexState);
	F_SAFE_DELETE(m_pSemaphore);
    F_SAFE_DELETE(m_pCondStop);
    
    // Destroy the remained tasks.
    while(!m_Tasks.empty())
    {
        FThreadTask* pTask = m_Tasks.top();
        m_Tasks.pop();
        F_SAFE_DELETE(pTask);
    }
}

/** Thread function of the pool.
*/
int _ThreadTaskProc(void* pParam)
{
    FThreadPool* pPool = (FThreadPool*)pParam;
    return pPool->Run();
}

/** Start the thread pool.
*/
bool FThreadPool::Init(int iThreadNum)
{
	if (iThreadNum <= 0)
	{
		FLOG_ERROR("FThreadPool::Init, count of thread must be greater than 0!");
		return false;
	}
    
    m_iState = STATE_RUNNING;

	for (int i = 0; i < iThreadNum; i++)
	{
		if (!FThread::Create(_ThreadTaskProc, this))
		{
            Shutdown();
			FLOG_ERROR("FThreadPool::Init, failed to create the thread!");
			return false;
		}
        
        m_iThreadCount++;
	}

	return true;
}

/** Shutdown the thread pool.
 */
void FThreadPool::Shutdown()
{
    FScopedLock lock(m_pMutexState);
    m_iState = STATE_WILLSTOP;
    m_pSemaphore->Notify(m_iThreadCount);
}

/** Add a task with a priority value.
 */
bool FThreadPool::AddTask(FThreadTask *pTask, int iPriority)
{
    if( !pTask )
    {
		FASSERT(!"pTask == null");
        FLOG_ERROR("FThreadPool::AddTask, pointer to the task must not be null!");
        return false;
    }
    
    {
        FScopedLock lock(m_pMutexState);
        if( m_iState != STATE_RUNNING )
        {
            FLOG_ERROR("FThreadPool::AddTask, thread pool is not running now!");
            return false;
        }
    }
    
    if( iPriority != 0 )
        pTask->SetPriority(iPriority);
    
    FScopedLock lock(m_pMutexTasks);
    m_Tasks.push(pTask);
    m_pSemaphore->NotifyOne();
    return true;
}

/** Get the count of the task.
 */
size_t FThreadPool::GetTaskCount() const
{
    FScopedLock lock(m_pMutexTasks);
    return m_Tasks.size();
}

/** Get the state of thread pool.
 */
int FThreadPool::GetState() const
{
    FScopedLock lock(m_pMutexState);
    return m_iState;
}

/** Is the thread pool paused ?
 */
bool FThreadPool::IsPaused() const
{
    FScopedLock lock(m_pMutexState);
    return m_bPaused;
}

/** Wait the end of the thread pool.
 */
void FThreadPool::Wait()
{
    FScopedLock lock(m_pMutexState);
    if( m_iState != STATE_STOPPED )
    {
        m_pCondStop->Wait(m_pMutexState);
    }
}

/** Pause the thread pool.
 */
void FThreadPool::Pause()
{
    FScopedLock lock(m_pMutexState);
    m_bPaused = true;
}

/** Resume the thread pool.
 */
void FThreadPool::Resume()
{
    FScopedLock lock(m_pMutexState);
    m_bPaused = false;
    
    // Wake up all the threads.
    m_pSemaphore->Notify((int)m_Tasks.size());
}

/** Cancel the tasks which have the specified mask.
 */
void FThreadPool::CancelTask(uint32 nMask)
{
    // Pause the thread pool.
    Pause();
    
    {
        FScopedLock lock(m_pMutexTasks);
        std::vector<FThreadTask*> backupArray;
        while(!m_Tasks.empty())
        {
            FThreadTask* pTask = m_Tasks.top();
            m_Tasks.pop();
            if( pTask->GetMask() & nMask )
            {
                F_SAFE_DELETE(pTask);
            }
            else
            {
                backupArray.push_back(pTask);
            }
        }
        
        // Restore the remained tasks.
        std::vector<FThreadTask*>::iterator it = backupArray.begin();
        for(; it != backupArray.end(); ++it)
        {
            m_Tasks.push(*it);
        }
    }
    
    // Resume the thread pool.
    Resume();
}

/** Do the tasks.
 */
int FThreadPool::Run()
{
    while(1)
    {
        FThreadTask* pTask = NULL;
        {
            m_pSemaphore->Wait();
            m_pMutexTasks->Lock();
            m_pMutexState->Lock();
            if( m_iState == STATE_WILLSTOP )
            {
                m_pMutexState->Unlock();
                m_pMutexTasks->Unlock();
                break;
            }
            
            if( m_bPaused )
            {
                m_pMutexState->Unlock();
                m_pMutexTasks->Unlock();
                continue;
            }
            
            m_pMutexState->Unlock();
            
            if( m_Tasks.empty() )
            {
                m_pMutexTasks->Unlock();
                continue;
            }
            
            pTask = m_Tasks.top();
            m_Tasks.pop();
            m_pMutexTasks->Unlock();
        }
        
		if (pTask)
		{
			pTask->Run();

			if (pTask->IsAutoDelete())
				delete pTask;
		}
    }
    
    --m_iThreadCount;
    if( m_iThreadCount == 0 )
    {
        FScopedLock lock(m_pMutexState);
        m_iState = STATE_STOPPED;
        m_pCondStop->Signal();
    }
    
    return FThread::RESULT_COMPLETE;
}