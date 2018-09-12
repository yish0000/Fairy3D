/*
 * ------------------------------------------------------------------------
 *  Name:   FNRPC.cpp
 *  Desc:   本文件实现了网络库的RPC功能。
 *  Author: Yish
 *  Date:   2012/5/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FNRPC.h"


namespace FNET {

    Mutex Rpc::XID::locker_xid;
    uint32 Rpc::XID::xid_stamp = 0;
    Mutex Rpc::locker_map;
    Rpc::HouseKeeper Rpc::housekeeper;
    Rpc::RpcMap Rpc::map;

    ///////////////////////////////////////////////////////////////////////
    // XID的实现

    Rpc::XID& Rpc::XID::operator = ( const Rpc::XID& rhs )
    {
        if( &rhs != this )
            id = rhs.id;
        return *this;
    }

    bool Rpc::XID::operator < ( const Rpc::XID& rhs ) const
    {
        return count < rhs.count;
    }

    OctetsStream& Rpc::XID::marshal( OctetsStream& os ) const
    {
        return os << id;
    }

    OctetsStream& Rpc::XID::unmarshal( OctetsStream& is )
    {
        return is >> id;
    }

    void Rpc::XID::SetRequest()
    {
        is_request = 1;
        Mutex::ScopeLock l(locker_xid);
        count = xid_stamp++;
    }

    void Rpc::XID::ClrRequest()
    {
        is_request = 0;
    }

    ///////////////////////////////////////////////////////////////////////
    // RPC超时监听

    void Rpc::HouseKeeper::Update()
    {
        Mutex::ScopeLock l(locker_map);
        for( RpcMap::iterator it=map.begin();it!=map.end();++it )
        {
            Rpc* rpc = it->second;
            if( !rpc->TimePolicy(rpc->timer.Elapse()) )
            {
                rpc->OnTimeout();
                delete rpc;
                map.erase( it );
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////
    // RPC的实现

    Rpc::Rpc( Type type,RpcData* arg,RpcData* res ) : Protocol(type),argument(arg),result(res)
    {
    }

    Rpc::Rpc( const Rpc& rhs ) : Protocol(rhs),argument(rhs.argument->Clone()),
        result(rhs.result->Clone())
    {
    }

    Rpc::~Rpc()
    {
        delete argument;
        delete result;
    }

    OctetsStream& Rpc::marshal( OctetsStream& os ) const
    {
        return os << xid << (xid.IsRequest() ? *argument : *result);
    }

    OctetsStream& Rpc::unmarshal( OctetsStream& is )
    {
        is >> xid;
        if( xid.IsRequest() )
            return is >> *argument;
        Mutex::ScopeLock l(locker_map);
        RpcMap::iterator it = map.find( xid );
        return it == map.end() ? is : is >> *(it->second->result);
    }

    /** 响应RPC的到来
    */
    void Rpc::Notify( NetManager* mgr,ID sid )
    {
        Rpc* rpc;

        if( !xid.IsRequest() )
        {
            {
                Mutex::ScopeLock l(locker_map);
                RpcMap::iterator it = map.find( xid );
                if( it == map.end() ) return;
                rpc = it->second;
                map.erase( it );
            }

            mgr->OnRecvProtocol( sid,rpc );
            delete this;
        }
        else
        {
            xid.ClrRequest();
            mgr->OnRecvProtocol( sid,this );
        }
    }

    /** 调用指定类型的RPC
    */
    Rpc* Rpc::Call( Type type,const RpcData* arg )
    {
        Rpc* rpc = (Rpc*)Rpc::Create( type );
        rpc->xid.SetRequest();
        *rpc->argument = *arg;
        rpc->timer.Reset();

        Mutex::ScopeLock l(locker_map);
        RpcMap::iterator it = map.find( rpc->xid );
        if( it != map.end() )
        {
            delete it->second;
            it->second = rpc;
        }
        else
        {
            map.insert( std::make_pair(rpc->xid,rpc) );
        }

        return rpc;
    }

    /** 调用指定类型的RPC
    */
    Rpc* Rpc::Call( Type type,const RpcData& arg )
    {
        return Call( type,&arg );
    }
}