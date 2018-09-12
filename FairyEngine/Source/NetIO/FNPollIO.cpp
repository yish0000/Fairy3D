/*
 * ------------------------------------------------------------------------
 *  Name:   FNPollIO.cpp
 *  Desc:   ���ļ�ʵ������������ѯIO���ࡣ
 *  Author: Yish
 *  Date:   2012/3/5
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FNPollIO.h"
#include "FNTimer.h"


namespace FNET {

    SOCKET PollControl::writer;
    AString NetIO::errmsg;

    /** PollIO���캯��
    */
    PollIO::PollIO( SOCKET s ) : sock(s)
    {
        ulong ul = 1;
        ioctlsocket( sock,FIONBIO,&ul );
        NetIO::GetInstance().GetIONew()[sock] = this;
    }

    PollControl::~PollControl()
    {
        closesocket( sock );
    }

    int PollControl::UpdateEvent()
    {
        return IO_POLLIN;
    }

    void PollControl::PollIn()
    {
        for( char buf[256]; recv(sock,buf,256,0) == 256; );
    }

    bool PollControl::Init()
    {
        ulong ul = 1;
        SOCKET s = socket( AF_INET,SOCK_DGRAM,0 );

        struct sockaddr_in sin;
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = inet_addr("127.0.0.1");
        sin.sin_port = htons(0);

        bind( s,(LPSOCKADDR)&sin,sizeof(sin) );
        struct sockaddr addr;
        int len = sizeof(struct sockaddr);
        getsockname( s,&addr,&len );

        connect( s,(LPSOCKADDR)&addr,len );
        ioctlsocket( s,FIONBIO,&ul );

        return (new PollControl(s)) ? true : false;
    }

    void PollControl::WakeUp()
    {
        send( writer,"",1,0 );
    }

    /** ��ȡNetIO��Ψһʵ��
    */
    NetIO& NetIO::GetInstance()
    {
        static NetIO io;
        return io;
    }

    /** ��ȡ�����������ļ�
    */
    FIniFile& NetIO::GetConfig()
    {
        static FIniFile file;
        return file;
    }

    /** ��ȡ�����̵߳�ʵ��
    */
    NetIO::Task* NetIO::Task::GetInstance()
    {
        static NetIO::Task task;
        return &task;
    }

    /** ��ʼ��NetIO
    */
    bool NetIO::Init( const char* szCfgFile,int thread_count )
    {
        FD_ZERO( &rfds );
        FD_ZERO( &wfds );
        FD_ZERO( &efds );

        // ���������ļ�
        if( !GetConfig().LoadFile(szCfgFile) )
            return false;

        // ��ʼ�������
        PollControl::Init();
        ThreadPool::Instance().AddTask( NetIO::Task::GetInstance() );
        ThreadPool::Instance().Run(thread_count);      // Ϊ����⿪��ָ����Ŀ���߳�
        return true;
    }

    /** ֹͣ�����
    */
    void NetIO::Shutdown()
    {
        Mutex::ScopeLock l(locker_poll);
        for( IOMap::const_iterator it=iomap.begin();it!=iomap.end();++it )
        {
            if( it->second )
                delete it->second;
        }
        iomap.clear();
    }

    /** ���¸���IO������¼�
    */
    void NetIO::UpdateEvent( const IOMap::value_type iopair )
    {
        PollIO* io = iopair.second;

        if( !io ) return;

        int iEvent = io->UpdateEvent();
        if( iEvent == -1 )
        {
            iomap[io->sock] = NULL;
            delete io;
            return;
        }

        if( iEvent )
        {
            if( iEvent & PollIO::IO_POLLIN )  FD_SET( io->sock,&rfds );
            if( iEvent & PollIO::IO_POLLOUT ) FD_SET( io->sock,&wfds );
            if( iEvent & PollIO::IO_POLLERR ) FD_SET( io->sock,&efds );
            ioset.push_back( io );
        }
    }

    /** ����IO������¼�
    */
    void NetIO::TriggerEvent( PollIO* io )
    {
        if( FD_ISSET(io->sock,&rfds) ) io->PollIn();
        if( FD_ISSET(io->sock,&wfds) ) io->PollOut();
        if( FD_ISSET(io->sock,&efds) ) io->PollErr();
    }

    /** ��ѯ����IO����
    */
    void NetIO::Poll( int iTimeout )
    {
        Mutex::ScopeLock l(locker_poll);
        {
            Mutex::ScopeLock ll(locker_ionew);
            for( IOMap::const_iterator it=ionew.begin();it!=ionew.end();++it )
                iomap[it->first] = it->second;
            ionew.clear();
        }

        ioset.clear();
        wakeup_flag = false;
        for( IOMap::iterator it=iomap.begin();it!=iomap.end();++it )
            UpdateEvent( *it );
        wakeup_scope = true;

        if( wakeup_flag )
        {
            wakeup_scope = false;
            wakeup_flag = false;
            iTimeout = 0;
        }

        int iEvents;
        if( iTimeout < 0 )
            iEvents = select( 0,&rfds,&wfds,&efds,NULL );
        else
        {
            struct timeval tv;
            tv.tv_sec = iTimeout / 1000;
            tv.tv_usec = (iTimeout - (tv.tv_sec * 1000)) * 1000;
            iEvents = select( 0,&rfds,&wfds,&efds,&tv );
        }

        wakeup_scope = false;
        if( iEvents > 0 )
        {
            std::vector<PollIO*>::iterator it;
            for( it=ioset.begin();it!=ioset.end();++it )
                TriggerEvent( *it );
        }

        FD_ZERO( &rfds );
        FD_ZERO( &wfds );
        FD_ZERO( &efds );
    }

    /** �ر������
    */
    void NetIO::Close()
    {
        ThreadPool::Instance().Shutdown();
        WakeUp();
    }

    /** �����������߳�
    */
    void NetIO::WakeUp()
    {
        if( wakeup_scope )
            PollControl::WakeUp();
        else
            wakeup_flag = true;
    }

    /** NetIO�������߳�
    */
    void NetIO::Task::Run()
    {
        NetIO& io = NetIO::GetInstance();

        io.Poll(1000);
        Timer::Update();

        if( !ThreadPool::Instance().AddTask(this) )
            io.Shutdown();
    }
}