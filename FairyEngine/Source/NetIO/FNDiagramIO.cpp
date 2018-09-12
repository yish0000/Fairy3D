/*
 * ------------------------------------------------------------------------
 *  Name:   FNDiagramIO.cpp
 *  Desc:   本文件实现了网络库中基于数据包的IO组件。
 *  Author: Yish
 *  Date:   2012/5/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FNDiagramIO.h"


namespace FNET {

    /** DiagramClientIO构造函数
    */
    DiagramClientIO::DiagramClientIO( SOCKET sk,Session* s,const SockAddr& ad )
        : ConnectedIO(sk,s),peer(ad)
    {
        session->OnOpen();
    }

    /** DiagramClientIO析构函数
    */
    DiagramClientIO::~DiagramClientIO()
    {
        session->OnClose();
        delete session;
    }

    /** 数据报客户端更新事件
    */
    int DiagramClientIO::UpdateEvent()
    {
        Mutex::ScopeLock l(session->locker);
        if( ibuf->size() )
            session->OnRecv();
        session->OnSend();
        if( obuf->size() )
        {
            sendto( sock,(char*)obuf->begin(),(int)obuf->size(),0,peer,peer.GetLen() );
            obuf->clear();
        }

        return session->closing ? -1 : IO_POLLIN;
    }

    /** 收取数据
    */
    void DiagramClientIO::PollIn()
    {
        int recv_len;
        if( (recv_len = recv(sock,(char*)ibuf->end(),(int)(ibuf->capcity()-ibuf->size()),0)) > 0 )
        {
            ibuf->resize( ibuf->size() + recv_len );
            return;
        }
    }

    /** 数据报服务器IO
    */
    DiagramServerIO::DiagramServerIO( SOCKET sk,Session* s ) : ConnectedIO(sk,s)
    {
    }

    /** DiagramServerIO析构函数
    */
    DiagramServerIO::~DiagramServerIO()
    {
        SessionMap::iterator it;
        for( it=sessions.begin();it!=sessions.end();++it )
        {
            Session* ss = it->second;
            ss->OnClose();
            delete ss;
        }

        delete session;
    }

    /** 数据报服务器更新事件
    */
    int DiagramServerIO::UpdateEvent()
    {
        SessionMap::iterator it;
        for( it=sessions.begin();it!=sessions.end();++it )
        {
            const SockAddr& sa = it->first;
            Session* ss = it->second;
            Octets* ibuffer = &ss->ibuffer;
            Octets* obuffer = &ss->obuffer;

            Mutex::ScopeLock l(ss->locker);
            if( ibuffer->size() )
                ss->OnRecv();
            ss->OnSend();
            if( obuffer->size() )
            {
                sendto( sock,(char*)obuffer->begin(),(int)obuffer->size(),0,sa,sa.GetLen() );
                obuffer->clear();
            }

            if( ss->closing )
            {
                sessions.erase( it );
                ss->OnClose();
                delete ss;
            }
        }

        session->OnSend();
        return session->closing ? -1 : IO_POLLIN;
    }

    /** 收取外部数据
    */
    void DiagramServerIO::PollIn()
    {
        int len;
        int recv_len;
        struct sockaddr_in addr;
        while( (recv_len = recvfrom(sock,(char*)ibuf->begin(),(int)ibuf->capcity(),0,
            (struct sockaddr*)&addr,&len)) > 0 )
        {
            SessionMap::iterator it = sessions.find(addr);
            if( it == sessions.end() )
            {
                Session* ss = session->Clone();
                it = sessions.insert(std::make_pair(addr,ss)).first;
                ss->OnOpen();
            }

            Octets& i = it->second->ibuffer;
            if( i.size() + recv_len < i.capcity() )
                i.insert( i.end(),ibuf->begin(),recv_len );
        }
    }
}