/*
 * ------------------------------------------------------------------------
 *  Name:   FNSession.h
 *  Desc:   本文件为网络库定义了一个会话对象。
 *  Author: Yish
 *  Date:   2012/5/17
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FNET_SESSION_H__
#define __FNET_SESSION_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FNBase.h"
#include "FNOctets.h"
#include "FNSecure.h"
#include "FNThread.h"
#include "FNPollIO.h"

///////////////////////////////////////////////////////////////////////////


namespace FNET {

    /** Socket地址的封装
    */
    class _FNET_EXPORT SockAddr
    {
    private:
        Octets addr;

    public:
        SockAddr() {}
        template <typename T> SockAddr( const T& ad ) : addr(&ad,sizeof(ad)) {}
        SockAddr( const SockAddr& rhs ) : addr(rhs.addr) {}
        int GetLen() const { return (int)addr.size(); }
        template <typename T> operator T* () { addr.resize(sizeof(T)); return (T*)addr.begin(); }
        template <typename T> operator const T* () { return (const T*)addr.begin(); }
        operator const sockaddr* () const { return (const sockaddr*)addr.begin(); }
        operator const sockaddr_in* () const { return (const sockaddr_in*)addr.begin(); }
        operator sockaddr_in* () { addr.resize(sizeof(sockaddr_in)); return (sockaddr_in*)addr.begin(); }
    };

    /** 网络会话基类
    */
    class _FNET_EXPORT Session
    {
        friend class ConnectedIO;
        friend class StreamIO;
        friend class DiagramClientIO;
        friend class DiagramServerIO;

        enum { DEFAULTIOBUF = 8192 };

    protected:
        bool closing;
        Octets ibuffer;
        Octets obuffer;
        Octets isecbuf;
        Secure* isec;
        Secure* osec;

        Mutex locker;

    public:
        Session();
        Session( const Session& rhs );
        virtual ~Session();

        bool LoadConfig();
        virtual void Close() { closing = true; }
        virtual AString Identification() const = 0;
        virtual void OnRecv() = 0;
        virtual void OnSend() = 0;
        virtual bool NoMoreData() const = 0;
        virtual void OnOpen() {}
        virtual void OnClose() {}
        virtual void OnAbort() {}
        virtual Session* Clone() const = 0;
        virtual void OnCheckAddress( SockAddr& addr ) const {}

    protected:
        Octets& Input();
        bool Output( Octets& data );
        void SetISecure( ENetSecure type,const Octets& key );
        void SetOSecure( ENetSecure type,const Octets& key );
    };

    /** 建立连接后的IO
    */
    class _FNET_EXPORT ConnectedIO : public PollIO
    {
    protected:
        Session* session;
        Octets* ibuf;
        Octets* obuf;
        ConnectedIO( SOCKET sk,Session* s ) : PollIO(sk),session(s),ibuf(&s->ibuffer),obuf(&s->obuffer) {}
        ~ConnectedIO() { closesocket(sock); }
    };
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FNET_SESSION_H__