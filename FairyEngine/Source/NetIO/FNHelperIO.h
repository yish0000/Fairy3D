/*
 * ------------------------------------------------------------------------
 *  Name:   FNHelperIO.h
 *  Desc:   本文件定义了两种主动连接，被动连接的IO。
 *  Author: Yish
 *  Date:   2012/5/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FNET_HELPERIO_H__
#define __FNET_HELPERIO_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FNBase.h"
#include "FNPollIO.h"
#include "FNSession.h"

///////////////////////////////////////////////////////////////////////////


namespace FNET {

    /** 主动连接的IO
    */
    class _FNET_EXPORT ActiveIO : public PollIO
    {
        enum Type
        {
            STREAM,
            DIAGRAM,
            DIAGRAM_BROADCAST
        };

        enum Status
        {
            CONNECTING,
            CONNECTED,
            ABORTING,
        };

    protected:
        Type type;
        int closing;
        Session* session;
        SockAddr sa;

    public:
        ActiveIO( SOCKET sk,const SockAddr& addr,Session* s,Type t );
        ~ActiveIO();

        int UpdateEvent();
        void PollOut();
        void PollErr();

        void Close( int code ) { closing = code; }
        const Session* GetSession() { return session; }
        SOCKET GetSocket() { return sock; }

        static ActiveIO* Open( Session* ss );
        static ActiveIO* Open( Session* ss,const char* host,ushort port );
    };

    /** 被动连接的IO
    */
    class _FNET_EXPORT PassiveIO : public PollIO
    {
        enum Type { STREAM,DIAGRAM };

    protected:
        Session* session;
        Type type;
        bool closing;

    public:
        PassiveIO( SOCKET sk,Session* s,Type t );
        virtual ~PassiveIO();

        int UpdateEvent();
        void PollIn();
        void Close() { closing = true; }

        static PassiveIO* Open( Session* ss );
    };
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FNET_HELPERIO_H__