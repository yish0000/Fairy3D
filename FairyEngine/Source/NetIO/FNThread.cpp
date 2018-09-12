/*
 * ------------------------------------------------------------------------
 *  Name:   FNThread.cpp
 *  Desc:   本文件实现了网络库的线程池机制。
 *  Author: Yish
 *  Date:   2012/3/10
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FNThread.h"
#include "FLogManager.h"
#include <process.h>


namespace FNET {

#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	Mutex::Mutex()
	{
		InitializeCriticalSection( &m_cs );
	}

	Mutex::~Mutex()
	{
		DeleteCriticalSection( &m_cs );
	}

	void Mutex::Lock()
	{
		EnterCriticalSection( &m_cs );
	}

	void Mutex::Unlock()
	{
		LeaveCriticalSection( &m_cs );
	}

	Condition::Condition()
	{
		handle = CreateEvent( NULL,TRUE,TRUE,NULL );
	}

	Condition::~Condition()
	{
		CloseHandle( handle );
	}

	void Condition::Wait( Mutex& mutex )
	{
		mutex.Unlock();
		WaitForSingleObject( handle,INFINITE );
		mutex.Lock();
		ResetEvent( handle );
	}

	void Condition::NotifyOne()
	{
		SetEvent( handle );
	}

	Semaphore::Semaphore()
	{
		handle = CreateSemaphore( NULL,0,0x7fff,NULL );
	}

	Semaphore::~Semaphore()
	{
		CloseHandle( handle );
	}

	void Semaphore::Wait()
	{
		WaitForSingleObject( handle,INFINITE );
	}

	void Semaphore::NotifyOne()
	{
		ReleaseSemaphore( handle,1,NULL );
	}

	void Semaphore::Notify( int n )
	{
		ReleaseSemaphore( handle,n,NULL );
	}
#endif

	// 获取线程池
	ThreadPool& ThreadPool::Instance()
	{
		static ThreadPool pool;
		return pool;
	}

	// 构造函数
	ThreadPool::ThreadPool()
	{
		m_iThreadCount = 0;
		m_iState = STATE_NORMAL;
	}

	// 析构函数、
	ThreadPool::~ThreadPool()
	{
	}

	// 添加任务到线程池
	bool ThreadPool::AddTask( Thread* pTask )
	{
		if( m_iState != STATE_NORMAL )
			return false;

		Mutex::ScopeLock lock(m_MutexTasks);
		m_Tasks.push_back( pTask );
		m_CondTasks.NotifyOne();
		return true;
	}

	// 关闭线程池
	void ThreadPool::Shutdown()
	{
		m_iState = STATE_QUIT;
		m_CondTasks.Notify( (int)m_Tasks.size() );
	}

	// 开启指定数目的线程
	void ThreadPool::Run( int iCount )
	{
		m_iThreadCount = iCount;
		{
			Mutex::ScopeLock lock(m_MutexTasks);
			for( int i=0;i<iCount;++i )
			{
				_beginthread( &ThreadPool::RunThread,0,NULL );
			}
		}
	}

	// 获取任务的数目
	int ThreadPool::GetTaskCount() const
	{
		Mutex::ScopeLock lock(m_MutexTasks);
		return (int)m_Tasks.size();
	}

	// 线程函数
	void __cdecl ThreadPool::RunThread( void* param )
	{
		ThreadPool& pool = ThreadPool::Instance();

		try
		{
			while(1)
			{
				Thread* pTask = NULL;
				{
					pool.m_CondTasks.Wait();
					pool.m_MutexTasks.Lock();
					if( pool.m_iState == STATE_QUIT )
					{
						pool.m_MutexTasks.Unlock();
						break;
					}

					if( pool.m_Tasks.empty() )
					{
						pool.m_MutexTasks.Unlock();
						continue;
					}

					pTask = pool.m_Tasks.front();
					pool.m_Tasks.pop_front();
					pool.m_MutexTasks.Unlock();
				}

				if( pTask )
					pTask->Run();
				else
					break;
			}

			pool.m_iThreadCount--;
		}
		catch(...)
		{
			FASSERT(0);
			FLOG_ERROR( "Exception occurred in FNetIO!" );
			exit(-1);
		}
	}
}