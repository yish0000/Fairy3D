/*
 * ------------------------------------------------------------------------
 *  Name:   FThreadPool.h
 *  Desc:   This file define the thread pool for engine.
 *  Author: Yish
 *  Date:   2014/3/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_THREADPOOL_H__
#define __FAIRY_THREADPOOL_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FThread.h"

///////////////////////////////////////////////////////////////////////////

// Mask of the thread task.
enum
{
	THREADTASK_MASK_DEFAULT = 1,
	THREADTASK_MASK_ALL = 0xffffffff,
};

/** Task running in the pool
*/
class FThreadTask : public FGeneralAlloc
{
public:
    FThreadTask(uint32 nMask, int iPriority = 0) : m_nMask(nMask), m_iPriority(iPriority), m_bAutoDelete(true) {}
	virtual ~FThreadTask() {}

	// Do task.
	virtual void Run() = 0;
    
    // Get the priority of the task.
    int GetPriority() const { return m_iPriority; }
    // Set the priority of the task.
    void SetPriority(int iPriority) { m_iPriority = iPriority; }
    
    // Delete the thread task automatic ?
    bool IsAutoDelete() const { return m_bAutoDelete; }
    void SetAutoDelete(bool bAutoDelete) { m_bAutoDelete = bAutoDelete; }

	// Get the mask of the task.
	uint32 GetMask() const { return m_nMask; }

protected:
	uint32 m_nMask;
	int m_iPriority;
    bool m_bAutoDelete;
};

/** Thread pool
*/
class FThreadPool : public FGeneralAlloc
{
    enum
    {
		STATE_CREATED,
        STATE_RUNNING,
        STATE_WILLSTOP,
        STATE_STOPPED,
    };

	struct TaskOperator
	{
		bool operator() (FThreadTask* pTask1, FThreadTask* pTask2)
		{
			return pTask1->GetPriority() > pTask2->GetPriority();
		}
	};
    
    typedef std::priority_queue<FThreadTask*, std::vector<FThreadTask*>, TaskOperator> TaskQueue;
    
    friend int _ThreadTaskProc(void* pParam);
    
public:
	FThreadPool();
	virtual ~FThreadPool();
    
    // Create a thread pool.
    static FThreadPool* Create(int iThreadNum);

	// Start the thread pool.
	bool Init(int iThreadNum);
	// Shutdown the thread pool.
	void Shutdown();
    // Wait for the end of the thread pool.
    void Wait();

	// Add a specified task
	bool AddTask(FThreadTask* pTask, int iPriority = 0);
	// Pause the execution
	void Pause();
	// Resume the execution
	void Resume();
	// Cancel some thread tasks.
	void CancelTask(uint32 nMask);
    // Get the count of task.
    size_t GetTaskCount() const;

    // Get the state of the thread pool.
    int GetState() const;
    // Is the thread pool paused ?
    bool IsPaused() const;
    // Get the count of threads.
    int GetThreadCount() const { return m_iThreadCount; }
    
protected:
	volatile int m_iState;
    volatile bool m_bPaused;
    volatile int m_iThreadCount;
    TaskQueue m_Tasks;
    FThreadMutex* m_pMutexTasks;
    FThreadMutex* m_pMutexState;
    FThreadSemaphore* m_pSemaphore;
    FThreadCond* m_pCondStop;
    
    // Run the tasks.
    int Run();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_THREADPOOL_H__