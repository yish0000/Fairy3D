/*
 * ------------------------------------------------------------------------
 *  Name:   FNStreamIO.h
 *  Desc:   本文件定义了网络库的TCP连接的IO组件。
 *  Author: Yish
 *  Date:   2012/3/4
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FNET_STREAMIO_H__
#define __FNET_STREAMIO_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FNPollIO.h"
#include "FNSession.h"

///////////////////////////////////////////////////////////////////////////


namespace FNET {

    /** 基于连接的IO组件
    */
    class _FNET_EXPORT StreamIO : public ConnectedIO
    {
    public:
        StreamIO( SOCKET sk,Session* s );
        virtual ~StreamIO();

    protected:
        int UpdateEvent();
        void PollIn();
        void PollOut();
    };
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FNET_STREAMIO_H__