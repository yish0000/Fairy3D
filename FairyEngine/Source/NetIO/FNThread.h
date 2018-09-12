/*
 * ------------------------------------------------------------------------
 *  Name:   FNThread.h
 *  Desc:   ʵ�����������̳߳ز��֡�
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

    /** �̶߳���
    */
    class _FNET_EXPORT Thread
    {
    public:
        Thread( int iPriority=0 ) : m_iPriority() {};
        virtual ~Thread() {};

        virtual void Run() = 0;

        // �̵߳����ȼ�
        int GetPriority() const { return m_iPriority; }
        void SetPriority( int iPriority ) { m_iPriority = iPriority; }

    protected:
        int m_iPriority;    // �߳����ȼ�
    };

    /** ������
    */
    class _FNET_EXPORT Mutex
    {
    public:
        Mutex();
        ~Mutex();

		void Lock();
		void Unlock();

        // ��������
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

    /** ����
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

    /** �ź���
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

    /** �̳߳�
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

		// �������
		bool AddTask( Thread* pTask );

		// ����ָ����Ŀ���߳�
		void Run( int iCount );

		// �ر��̳߳�
		void Shutdown();

		// ��ȡ����ĸ���
		int GetTaskCount() const;

		// ��ȡ�̵߳ĸ���
		int GetThreadCount() const { return m_iThreadCount; }

		// ��ȡ�̳߳�
		static ThreadPool& Instance();

	private:
		int m_iThreadCount;
		int m_iState;
		TaskQueue m_Tasks;
		mutable Mutex m_MutexTasks;
		mutable Semaphore m_CondTasks;

		// ����ָ���̵߳ĺ���
		static void __cdecl RunThread( void* param );
    };
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FNET_THREAD_H__