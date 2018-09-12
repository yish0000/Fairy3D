/*
 * ------------------------------------------------------------------------
 *  Name:   FNRPC.h
 *  Desc:   本文件定义了网络库的RPG机制。
 *  Author: Yish
 *  Date:   2012/3/4
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FNET_RPC_H__
#define __FNET_RPC_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FNProtocol.h"

///////////////////////////////////////////////////////////////////////////


namespace FNET {

    /** 远程过程调用数据
    */
    class _FNET_EXPORT RpcData : public Marshal
    {
    public:
        virtual ~RpcData() {}
        virtual RpcData* Clone() const = 0;
        virtual RpcData& operator = ( const RpcData& rhs ) = 0;
    };

    /** 远程过程调用
    */
    class _FNET_EXPORT Rpc : public Protocol
    {
    public:

        // XID
        class _FNET_EXPORT XID : public Marshal
        {
        protected:
            union
            {
                struct
                {
                    uint32 count : 31;
                    uint32 is_request : 1;
                };

                uint32 id;
            };

            static uint32 xid_stamp;
            static Mutex locker_xid;

        public:
            XID() : id(0) {}
            XID& operator = ( const XID& rhs );
            bool operator < ( const XID& rhs ) const;

            OctetsStream& marshal( OctetsStream& os ) const;
            OctetsStream& unmarshal( OctetsStream& is );

            void ClrRequest();
            void SetRequest();
            bool IsRequest() const { return is_request; }
        };

        typedef std::map<XID,Rpc*> RpcMap;

        // RPC超时监听
        class _FNET_EXPORT HouseKeeper : public Timer::Observer
        {
        public:
            HouseKeeper() { Timer::Attach(this); }
            void Update();
        };

    public:
        Rpc( Type type,RpcData* arg,RpcData* res );
        Rpc( const Rpc& rhs );
        virtual ~Rpc();

        OctetsStream& marshal( OctetsStream& os ) const;
        OctetsStream& unmarshal( OctetsStream& is );

        void Notify( NetManager* mgr,ID sid );

        virtual void Server( RpcData* arg,RpcData* res ) {}
        virtual void Client( RpcData* arg,RpcData* res ) {}
        virtual void OnTimeout() {}

        virtual bool TimePolicy( int timeout ) const { return timeout < 5; }

        RpcData* GetArgument() const { return argument; }
        RpcData* GetResult() const { return result; }

        static Rpc* Call( Type type,const RpcData* arg );
        static Rpc* Call( Type type,const RpcData& arg );

    protected:
        XID xid;
        RpcData* argument;
        RpcData* result;
        Timer timer;

        static RpcMap map;
        static HouseKeeper housekeeper;
        static Mutex locker_map;
    };
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FNET_RPC_H__