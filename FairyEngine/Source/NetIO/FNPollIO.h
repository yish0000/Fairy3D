/*
 * ------------------------------------------------------------------------
 *  Name:   FNPollIO.h
 *  Desc:   本文件定义了网络库IO组件的基类。
 *  Author: Yish
 *  Date:   2012/3/4
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FNET_POLLIO_H__
#define __FNET_POLLIO_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FNBase.h"
#include "FNThread.h"
#include "FIniFile.h"

///////////////////////////////////////////////////////////////////////////


namespace FNET {

    /** IO基类
    */
    class _FNET_EXPORT PollIO
    {
        friend class NetIO;

    public:
        enum IOEvent
        {
            IO_POLLIN = 1,
            IO_POLLOUT = 2,
            IO_POLLERR = 4,
        };

    public:
        virtual ~PollIO() {}
        virtual int UpdateEvent() = 0;
        virtual void PollIn() {}
        virtual void PollOut() {}
        virtual void PollErr() {}

    protected:
        SOCKET sock;

        PollIO() : sock(0) {}
        PollIO( SOCKET s );
    };

    /** IO控制器
    */
    class _FNET_EXPORT PollControl : public PollIO
    {
    public:
        PollControl( SOCKET sk ) : PollIO(sk) { writer = sk; }
        ~PollControl();

        int UpdateEvent();
        void PollIn();

        static bool Init();
        static void WakeUp();

    private:
        static SOCKET writer;
    };

    /** NetIO管理器
    @remarks
        用于控制网络库，这是一个单件
    */
    class _FNET_EXPORT NetIO
    {
    public:
        typedef std::map<SOCKET,PollIO*> IOMap;
        typedef std::vector<PollIO*> IOSet;

    protected:
        fd_set rfds, wfds, efds;

        IOMap iomap;
        IOSet ioset;
        IOMap ionew;
        Mutex locker_ionew;
        Mutex locker_poll;
        bool wakeup_scope;
        bool wakeup_flag;

    public:
        NetIO() : wakeup_scope(false),wakeup_flag(false) {}
        virtual ~NetIO() {}

        bool Init( const char* szCfgFile,int iThreadCount=1 );
        void Shutdown();
        void Close();
        void Poll( int iTimeout );

        // 唤醒网络线程
        void WakeUp();

        Mutex& LockerNew() { return locker_ionew; }
        IOMap& GetIOMap() { return iomap; }
        IOSet& GetIOSet() { return ioset; }
        IOMap& GetIONew() { return ionew; }

        // 获取实例对象
        static NetIO& GetInstance();

        // 获取网络库的配置文件
        static FIniFile& GetConfig();

        // 网络库的错误信息
        static AString errmsg;

        // PollIO的任务线程
        class _FNET_EXPORT Task : public Thread
        {
        public:
            Task() {}

            // 工作
            void Run();

            // 获取任务线程的实例
            static Task* GetInstance();
        };

    private:
        // 更新IO对象的事件
        void UpdateEvent( const IOMap::value_type iopair );

        // IO对象触发事件
        void TriggerEvent( PollIO* io );
    };
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FNET_POLLIO_H__