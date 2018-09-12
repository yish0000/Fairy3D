/*
 * ------------------------------------------------------------------------
 *  Name:   FNPollIO.h
 *  Desc:   ���ļ������������IO����Ļ��ࡣ
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

    /** IO����
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

    /** IO������
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

    /** NetIO������
    @remarks
        ���ڿ�������⣬����һ������
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

        // ���������߳�
        void WakeUp();

        Mutex& LockerNew() { return locker_ionew; }
        IOMap& GetIOMap() { return iomap; }
        IOSet& GetIOSet() { return ioset; }
        IOMap& GetIONew() { return ionew; }

        // ��ȡʵ������
        static NetIO& GetInstance();

        // ��ȡ�����������ļ�
        static FIniFile& GetConfig();

        // �����Ĵ�����Ϣ
        static AString errmsg;

        // PollIO�������߳�
        class _FNET_EXPORT Task : public Thread
        {
        public:
            Task() {}

            // ����
            void Run();

            // ��ȡ�����̵߳�ʵ��
            static Task* GetInstance();
        };

    private:
        // ����IO������¼�
        void UpdateEvent( const IOMap::value_type iopair );

        // IO���󴥷��¼�
        void TriggerEvent( PollIO* io );
    };
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FNET_POLLIO_H__