/*
 * ------------------------------------------------------------------------
 *  Name:   FNThread.h
 *  Desc:   实现了网络库的线程池部分。
 *  Author: Yish
 *  Date:   2012/3/4
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FNET_THREAD_H__
#define __FNET_THREAD_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FNBase.h"

///////////////////////////////////////////////////////////////////////////

namespace FNET {

    /** 线程对象
    */
    class _FNET_EXPORT Thread
    {
    public:
        Thread( int iPriority=0 ) : m_iPriority() {};
        virtual ~Thread() {};

        virtual void Run() = 0;

        // 线程的优先级
        int GetPriority() const { return m_iPriority; }
        void SetPriority( int iPriority ) { m_iPriority = iPriority; }

    protected:
        int m_iPriority;    // 线程优先级
    };

    /** 互斥器
    */
    class _FNET_EXPORT Mutex
    {
    public:
        Mutex();
        ~Mutex();

		void Lock();
		void Unlock();

        // 代码块加锁
        class _FNET_EXPORT ScopeLock
        {
        public:
            ScopeLock( Mutex& m ) : m_pMutex(&m) { m_pMutex->Lock(); };
            ~ScopeLock() { m_pMutex->Unlock(); };

        private:
            Mutex* m_pMutex;
        };

    private:
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
        CRITICAL_SECTION m_cs;
#endif
    };

    /** 条件
    */
    class _FNET_EXPORT Condition
    {
    public:
        Condition();
        ~Condition();

		void Wait( Mutex& mutex );
		void NotifyOne();

	private:
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
		HANDLE handle;
#endif
    };

    /** 信号量
    */
    class _FNET_EXPORT Semaphore
    {
    public:
        Semaphore();
        ~Semaphore();

		void Wait();
		void NotifyOne();
		void Notify( int n );

	private:
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
		HANDLE handle;
#endif
    };

    /** 线程池
    */
    class _FNET_EXPORT ThreadPool
    {
		enum STATE
		{
			STATE_NORMAL,
			STATE_QUIT,
			STATE_QUITATONCE,
		};

		typedef std::deque<Thread*> TaskQueue;

    public:
        ThreadPool();
        ~ThreadPool();

		// 添加任务
		bool AddTask( Thread* pTask );

		// 开启指定数目的线程
		void Run( int iCount );

		// 关闭线程池
		void Shutdown();

		// 获取任务的个数
		int GetTaskCount() const;

		// 获取线程的个数
		int GetThreadCount() const { return m_iThreadCount; }

		// 获取线程池
		static ThreadPool& Instance();

	private:
		int m_iThreadCount;
		int m_iState;
		TaskQueue m_Tasks;
		mutable Mutex m_MutexTasks;
		mutable Semaphore m_CondTasks;

		// 运行指定线程的函数
		static void __cdecl RunThread( void* param );
    };
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FNET_THREAD_H__