/*
 * ------------------------------------------------------------------------
 *  Name:   FNDiagramIO.h
 *  Desc:   本文件定义了网络库的数据报IO组件。
 *  Author: Yish
 *  Date:   2012/3/4
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FNET_DIAGRAMIO_H__
#define __FNET_DIAGRAMIO_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FNPollIO.h"
#include "FNSession.h"

///////////////////////////////////////////////////////////////////////////


namespace FNET {

    /** 基于数据报的客户端IO组件
    */
    class _FNET_EXPORT DiagramClientIO : public ConnectedIO
    {
    protected:
        SockAddr peer;

    public:
        DiagramClientIO( SOCKET sk,Session* s,const SockAddr& ad );
        virtual ~DiagramClientIO();

        void PollIn();
        int UpdateEvent();
    };

    /** 基于数据报的服务器IO组件
    */
    class _FNET_EXPORT DiagramServerIO : public ConnectedIO
    {
        struct addr_compare
        {
            bool operator () ( const SockAddr& sa1,const SockAddr& sa2 ) const
            {
                const struct sockaddr_in* s1 = sa1;
                const struct sockaddr_in* s2 = sa2;

                return s1->sin_addr.s_addr < s2->sin_addr.s_addr ||
                    (s1->sin_addr.s_addr == s2->sin_addr.s_addr && s1->sin_port < s2->sin_port);
            }
        };

        typedef std::map<SockAddr,Session*,addr_compare> SessionMap;

    public:
        DiagramServerIO( SOCKET sk,Session* s );
        virtual ~DiagramServerIO();

        void PollIn();
        int UpdateEvent();

    protected:
        SessionMap sessions;
    };
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FNET_DIAGRAMIO_H__