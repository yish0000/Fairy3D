/*
 * ------------------------------------------------------------------------
 *  Name:   FNHelperIO.cpp
 *  Desc:   本文件实现了两种主动连接，被动连接的IO。
 *  Author: Yish
 *  Date:   2012/5/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FNHelperIO.h"
#include "FNStreamIO.h"
#include "FNDiagramIO.h"


namespace FNET {

    /** 主动连接IO构造函数
    */
    ActiveIO::ActiveIO( SOCKET sk,const SockAddr& addr,Session* s,Type t )
        : PollIO(sk),type(t),closing(t == STREAM ? CONNECTING : CONNECTED),session(s),sa(addr)
    {
        session->LoadConfig();
        if( type != DIAGRAM_BROADCAST )
            connect( sock,sa,sa.GetLen() );
        NetIO::GetInstance().WakeUp();
    }

    /** 析构函数
    */
    ActiveIO::~ActiveIO()
    {
        if( type == STREAM )
        {
            if( closing == CONNECTED )
            {
                Mutex::ScopeLock l(NetIO::GetInstance().LockerNew());
                new StreamIO( sock,session );

                NetIO::GetInstance().WakeUp();
            }
            else
            {
                session->OnAbort();
                delete session;
                closesocket( sock );
            }
        }
        else
        {
            Mutex::ScopeLock l(NetIO::GetInstance().LockerNew());
            new DiagramClientIO( sock,session,sa );
        }
    }

    /** 更新事件
    */
    int ActiveIO::UpdateEvent()
    {
        return (closing != CONNECTING) ? -1 : (IO_POLLOUT | IO_POLLERR);
    }

    /** 向外发送数据
    */
    void ActiveIO::PollOut()
    {
        Close( CONNECTED );
    }

    /** 触发错误
    */
    void ActiveIO::PollErr()
    {
        Close( ABORTING );
    }

    /** 打开一个主动连接的IO
    */
    ActiveIO* ActiveIO::Open( Session* ss )
    {
        SOCKET s;
        SockAddr sa;
        Type t;

        FIniFile& conf = NetIO::GetConfig();
        AString section = ss->Identification();
        AString type = conf.GetSetting( section,"type" );
        int optval = 1;

        if( !_stricmp(type.c_str(),"tcp") )
        {
            t = STREAM;
            struct sockaddr_in* addr = sa;
            memset( addr,0,sizeof(*addr) );
            s = socket( AF_INET,SOCK_STREAM,IPPROTO_TCP );
            addr->sin_family = AF_INET;
            addr->sin_addr.s_addr = inet_addr(conf.GetSetting(section,"address").c_str());
            addr->sin_port = htons(atoi(conf.GetSetting(section,"port").c_str()));
            ss->OnCheckAddress(sa);

            setsockopt( s,SOL_SOCKET,SO_KEEPALIVE,(char*)&optval,sizeof(optval) );
            optval = atoi(conf.GetSetting(section,"so_sndbuf").c_str());
            if( optval ) setsockopt( s,SOL_SOCKET,SO_SNDBUF,(char*)&optval,sizeof(optval) );
            optval = atoi(conf.GetSetting(section,"so_rcvbuf").c_str());
            if( optval ) setsockopt( s,SOL_SOCKET,SO_RCVBUF,(char*)&optval,sizeof(optval) );
            optval = atoi(conf.GetSetting(section,"tcp_nodelay").c_str());
            if( optval ) setsockopt( s,IPPROTO_TCP,TCP_NODELAY,(char*)&optval,sizeof(optval) );
        }
        else if( !_stricmp(type.c_str(),"udp") )
        {
            struct sockaddr_in* addr = sa;
            memset( addr,0,sizeof(*addr) );
            s = socket( AF_INET,SOCK_DGRAM,IPPROTO_UDP );
            addr->sin_family = AF_INET;
            addr->sin_addr.s_addr = inet_addr(conf.GetSetting(section,"address").c_str());
            addr->sin_port = htons(atoi(conf.GetSetting(section,"port").c_str()));
            ss->OnCheckAddress(sa);

            optval = atoi(conf.GetSetting(section,"so_broadcast").c_str());
            if( optval )
            {
                setsockopt( s,SOL_SOCKET,SO_BROADCAST,(char*)&optval,sizeof(optval) );
                t = DIAGRAM_BROADCAST;
            }
            else t = DIAGRAM;
            optval = atoi(conf.GetSetting(section,"so_sndbuf").c_str());
            if( optval ) setsockopt( s,SOL_SOCKET,SO_SNDBUF,(char*)&optval,sizeof(optval) );
            optval = atoi(conf.GetSetting(section,"so_rcvbuf").c_str());
            if( optval ) setsockopt( s,SOL_SOCKET,SO_RCVBUF,(char*)&optval,sizeof(optval) );
        }
        else
            s = INVALID_SOCKET;

        Mutex::ScopeLock l(NetIO::GetInstance().LockerNew());
        return s != INVALID_SOCKET ? new ActiveIO(s,sa,ss,t) : NULL;
    }

    /** 打开一个主动连接的IO
    */
    ActiveIO* ActiveIO::Open( Session* ss,const char* host,ushort port )
    {
        SOCKET s;
        SockAddr sa;
        Type t = STREAM;
        struct sockaddr_in* addr = sa;
        memset( addr,0,sizeof(*addr) );

        s = socket( AF_INET,SOCK_STREAM,IPPROTO_TCP );
        addr->sin_family = AF_INET;
        addr->sin_addr.s_addr = inet_addr(host);
        addr->sin_port = htons(port);
        ss->OnCheckAddress( sa );

        int optval = 1;
        setsockopt( s,SOL_SOCKET,SO_KEEPALIVE,(char*)&optval,sizeof(optval) );
        optval = 8192;
        setsockopt( s,SOL_SOCKET,SO_SNDBUF,(char*)&optval,sizeof(optval) );
        optval = 8192;
        setsockopt( s,SOL_SOCKET,SO_RCVBUF,(char*)&optval,sizeof(optval) );
        if( optval ) setsockopt( s,IPPROTO_TCP,TCP_NODELAY,(char*)&optval,sizeof(optval) );

        Mutex::ScopeLock l(NetIO::GetInstance().LockerNew());
        return s != INVALID_SOCKET ? new ActiveIO(s,sa,ss,t) : NULL;
    }

    /** 被动连接IO的构造函数
    */
    PassiveIO::PassiveIO( SOCKET sk,Session* s,Type t ) : PollIO(sk),session(s),type(t),closing(false)
    {
        session->LoadConfig();
    }

    /** 被动连接IO的析构函数
    */
    PassiveIO::~PassiveIO()
    {
        if( type == STREAM )
        {
            delete session;
            closesocket( sock );
        }
        else
        {
            Mutex::ScopeLock l(NetIO::GetInstance().LockerNew());
            new DiagramServerIO( sock,session );
            NetIO::GetInstance().WakeUp();
        }
    }

    /** 更新事件
    */
    int PassiveIO::UpdateEvent()
    {
        return closing ? -1 : IO_POLLIN;
    }

    /** 接收外部数据
    */
    void PassiveIO::PollIn()
    {
        if( type == STREAM )
        {
            SOCKET s = accept( sock,NULL,NULL );
            if( s != INVALID_SOCKET )
            {
                Mutex::ScopeLock l(NetIO::GetInstance().LockerNew());
                new StreamIO( s,session->Clone() );
            }
        }
        else
        {
            Close();
        }
    }

    /** 打开一个被动连接IO
    */
    PassiveIO* PassiveIO::Open( Session* ss )
    {
        SockAddr sa;
        FIniFile& conf = NetIO::GetConfig();
        AString section = ss->Identification();
        AString type = conf.GetSetting( section,"type" );

        SOCKET s;
        Type t;
        char optval = 1;
        if( !_stricmp(type.c_str(),"tcp") )
        {
            t = STREAM;
            s = socket( AF_INET,SOCK_STREAM,IPPROTO_TCP );
            struct sockaddr_in* addr = sa;
            memset( addr,0,sizeof(*addr) );
            addr->sin_family = AF_INET;
            if( (addr->sin_addr.s_addr = inet_addr(conf.GetSetting(section,"address").c_str()))
                == INADDR_NONE )
                addr->sin_addr.s_addr = INADDR_ANY;
            addr->sin_port = htons(atoi(conf.GetSetting(section,"port").c_str()));
            ss->OnCheckAddress(sa);

            setsockopt( s,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval) );
            setsockopt( s,SOL_SOCKET,SO_KEEPALIVE,&optval,sizeof(optval) );

            optval = atoi(conf.GetSetting(section,"so_sndbuf").c_str());
            if( optval ) setsockopt( s,SOL_SOCKET,SO_SNDBUF,&optval,sizeof(optval) );
            optval = atoi(conf.GetSetting(section,"so_rcvbuf").c_str());
            if( optval ) setsockopt( s,SOL_SOCKET,SO_RCVBUF,&optval,sizeof(optval) );
            optval = atoi(conf.GetSetting(section,"tcp_nodelay").c_str());
            if( optval ) setsockopt( s,IPPROTO_TCP,TCP_NODELAY,&optval,sizeof(optval) );

            bind( s,sa,sa.GetLen() );
            optval = atoi(conf.GetSetting(section,"listen_backlog").c_str());
            listen( s,optval ? optval : SOMAXCONN );
        }
        else if( !_stricmp(type.c_str(),"udp") )
        {
            t = DIAGRAM;
            s = socket( AF_INET,SOCK_DGRAM,IPPROTO_TCP );
            struct sockaddr_in* addr = sa;
            memset( addr,0,sizeof(*addr) );
            addr->sin_family = AF_INET;
            if( (addr->sin_addr.s_addr = inet_addr(conf.GetSetting(section,"address").c_str()))
                == INADDR_NONE )
                addr->sin_addr.s_addr = INADDR_ANY;
            addr->sin_port = htons(atoi(conf.GetSetting(section,"port").c_str()));
            ss->OnCheckAddress(sa);

            setsockopt( s,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval) );
            optval = atoi(conf.GetSetting(section,"so_broadcast").c_str());
            if( optval ) setsockopt( s,SOL_SOCKET,SO_BROADCAST,&optval,sizeof(optval) );
            optval = atoi(conf.GetSetting(section,"so_sndbuf").c_str());
            if( optval ) setsockopt( s,SOL_SOCKET,SO_SNDBUF,&optval,sizeof(optval) );
            optval = atoi(conf.GetSetting(section,"so_rcvbuf").c_str());
            if( optval ) setsockopt( s,SOL_SOCKET,SO_RCVBUF,&optval,sizeof(optval) );

            bind( s,sa,sa.GetLen() );
        }
        else
            s = INVALID_SOCKET;

        Mutex::ScopeLock l(NetIO::GetInstance().LockerNew());
        return s == INVALID_SOCKET ? NULL : new PassiveIO( s,ss,t );
    }
}