/*
 * ------------------------------------------------------------------------
 *  Name:   FNStreamIO.cpp
 *  Desc:   本文件实现了网络库基于连接的IO组件。
 *  Author: Yish
 *  Date:   2012/5/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FNStreamIO.h"
#include "FNPollIO.h"
#include "FStrUtil.h"


namespace FNET {

    /** 构造函数
    */
    StreamIO::StreamIO( SOCKET sk,Session* s ) : ConnectedIO(sk,s)
    {
        session->OnOpen();
    }

    /** 析构函数
    */
    StreamIO::~StreamIO()
    {
        session->OnClose();
        delete session;
    }

    /** 更新IO的事件
    */
    int StreamIO::UpdateEvent()
    {
        int iEvent = 0;

        Mutex::ScopeLock l(session->locker);
        if( ibuf->size() )
            session->OnRecv();

        if( !session->closing )
        {
            session->OnSend();
            if( obuf->size() || !session->NoMoreData() )
                iEvent = IO_POLLOUT;
        }
        else
            return -1;

        if( ibuf->size() < ibuf->capcity() )
            iEvent |= IO_POLLIN;

        return iEvent;
    }

    /** 从外部接收数据
    */
    void StreamIO::PollIn()
    {
        int recv_len;
        if( (recv_len = recv(sock,(char*)ibuf->end(),(int)(ibuf->capcity()-ibuf->size()),0)) > 0 )
        {
            ibuf->resize( ibuf->size() + recv_len );
            return;
        }

        int err = WSAGetLastError();
        if( recv_len != SOCKET_ERROR || err != WSAEWOULDBLOCK )
        {
            obuf->clear();
            NetIO::errmsg = AStringUtil::Format( "PollIn: errno=%d\n",err );
            session->closing = true;
        }
    }

    /** 向外部发送数据
    */
    void StreamIO::PollOut()
    {
        int send_len;
        if( (send_len = send(sock,(char*)obuf->begin(),(int)obuf->size(),0)) > 0 )
        {
            obuf->erase( obuf->begin(),(char*)obuf->begin()+send_len );
            return;
        }

        int err = WSAGetLastError();
        if( send_len != SOCKET_ERROR || err != WSAEWOULDBLOCK )
        {
            obuf->clear();
            NetIO::errmsg = AStringUtil::Format( "PollOut: errno=%d\n",err );
            session->closing = true;
        }
    }
}