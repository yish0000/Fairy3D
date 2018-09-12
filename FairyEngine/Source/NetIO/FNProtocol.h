/*
 * ------------------------------------------------------------------------
 *  Name:   FNProtocol.h
 *  Desc:   本文件定义了网络库的协议对象基类。
 *  Author: Yish
 *  Date:   2012/2/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FNET_PROTOCOL_H__
#define __FNET_PROTOCOL_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FNBase.h"
#include "FNMarshal.h"
#include "FNThread.h"
#include "FNSession.h"
#include "FNTimer.h"

///////////////////////////////////////////////////////////////////////////


namespace FNET {

    class Protocol;

    /** 协议对象
    */
    class _FNET_EXPORT Protocol : public Marshal
    {
        friend class Task;
        friend class NetManager;
        friend class NetSession;
        friend class ProtocolBinder;
        friend class CompressBinder;

    public:
        typedef unsigned int Type;
        typedef unsigned int ID;    // Session ID
        typedef std::map<Type,Protocol*> ProtocolMap;

    protected:
        Type type;

    public:
        Protocol() {}
        Protocol( Type t );
        Protocol( const Protocol& rhs ) : type(rhs.type) {}
        virtual ~Protocol() {}

        virtual Protocol* Clone() const = 0;
        virtual int PriorPolicy() const { return 0; }
        virtual bool SizePolicy( size_t sz ) const { return true; }
        virtual Type GetType() const { return type; }

        virtual void Process( NetManager* mgr,ID sid ) {}
        virtual void Notify( NetManager* mgr,ID sid );

        static Protocol* GetStub( Type type );
        static Protocol* Create( Type type );

    private:
        static ProtocolMap& GetMap();
        void Encode( OctetsStream& os ) const;
        static Protocol* Decode( OctetsStream* is );

        /** 处理协议的线程
        */
        class _FNET_EXPORT Task : public Thread
        {
        protected:
            NetManager* manager;
            ID sid;
            Protocol* protocol;

        public:
            Task( NetManager* mgr,ID id,Protocol* p )
                : manager(mgr),sid(id),protocol(p) {}
            ~Task() {}

            void Run();

            // 分派一个任务，处理到来的协议
            static void Dispatch( NetManager* mgr,ID sid,Protocol* p );
        };
    };

    /** 网络会话
    */
    class _FNET_EXPORT NetSession : public Session
    {
        friend class Protocol;
        friend class NetManager;

    public:
        typedef unsigned int ID;
        typedef std::map<ID,NetSession*> SessionMap;

        // 数据流
        struct _FNET_EXPORT Stream : public OctetsStream
        {
            NetSession* session;
            mutable bool check_policy;
            Stream( NetSession* ss ) : session(ss),check_policy(true) {}
        };

        // 状态，记录需要过滤的情况
        class _FNET_EXPORT State
        {
            typedef std::set<Protocol::Type> ProtocolSet;

        protected:
            ProtocolSet set;
            int timeout;

        public:
            State( Protocol::Type* first,size_t len,int t=0 );
            State( Protocol::Type* first,Protocol::Type* last,int t=0 );
            bool TypePolicy( Protocol::Type type ) const;
            bool TimePolicy( int t ) const;
        };

    protected:
        ID sid;
        Stream isc, is;
        std::queue<OctetsStream> osc;   // 压缩后的待输出数据
        std::deque<OctetsStream> os;    // 未压缩的待输出数据
        NetManager* manager;
        const State* state;
        Timer timer;

        static ID session_id;

    public:
        NetSession( NetManager* mgr );
        NetSession( const NetSession& rhs );
        virtual ~NetSession() {}

        bool Send( const Protocol* p,bool urg );
        void Close();
        void ChangeState( State* st );

        void OnOpen();
        void OnClose();
        void OnAbort();
        void OnSend();
        void OnRecv();

        void OnCheckAddress( SockAddr& sa );
        bool NoMoreData() const { return os.empty(); }
        bool StatePolicy( Protocol::Type type ) const { return state->TypePolicy(type); }
        AString Identification() const;
        ID GetSID() const { return sid; }

        NetSession* Clone() const { return new NetSession(*this); }

    private:
        bool Compress();
        void Decompress();
    };

    /** 网络管理器
    */
    class _FNET_EXPORT NetManager
    {
        friend class Protocol;
        friend class NetSession;

    protected:
        NetSession::SessionMap map;
        bool compress;
        Mutex locker_map;

    public:
        NetManager();
        virtual ~NetManager() {}

        void AddSession( NetSession::ID sid,NetSession* ss );
        void DelSession( NetSession::ID sid,NetSession* ss );
        void AbortSession( NetSession::ID sid,NetSession* ss );
        NetSession* GetSession( NetSession::ID sid );
        bool Close( NetSession::ID sid );

        bool SetISecure( NetSession::ID sid,ENetSecure secure,const Octets& key );
        bool SetOSecure( NetSession::ID sid,ENetSecure secure,const Octets& key );
        bool ChangeState( NetSession::ID sid,NetSession::State* state );

        bool Send( NetSession::ID sid,const Protocol* p,bool urg=false );
        bool Send( NetSession::ID sid,const Protocol& p,bool urg=false );

        virtual void OnAddSession( NetSession::ID sid ) = 0;
        virtual void OnDelSession( NetSession::ID sid ) = 0;
        virtual void OnAbortSession( NetSession::ID sid ) {}
        virtual void OnRecvProtocol( NetSession::ID sid,Protocol* p ) {}

        virtual AString Identification() const = 0;
        virtual void OnCheckAddress( SockAddr& sa ) const {}
        virtual NetSession::State* GetInitState() = 0;
        virtual int PriorPolicy( Protocol::Type type ) const;
        virtual bool InputPolicy( Protocol::Type type,size_t sz ) const;

        // 是否压缩数据
        bool IsCompress() const { return compress; }
    };

    /** 协议簇
    */
    class _FNET_EXPORT ProtocolBinder : public Protocol
    {
        typedef std::vector<Protocol*> ProtocolVector;

    protected:
        ProtocolVector pv;
        mutable Mutex locker_vector;

    public:
        explicit ProtocolBinder( Type type ) : Protocol(type) {}
        ProtocolBinder( const ProtocolBinder& rhs );
        virtual ~ProtocolBinder();

        // 绑定协议
        ProtocolBinder& Bind( const Protocol* p );
        ProtocolBinder& Bind( const Protocol& p );

        OctetsStream& marshal( OctetsStream& os ) const;
        OctetsStream& unmarshal( OctetsStream& is );

        // 处理该协议簇
        void Notify( NetManager* mgr,ID sid );
    };

    /** 压缩后的协议簇
    */
    class _FNET_EXPORT CompressBinder : public Protocol
    {
        typedef std::vector<Protocol*> ProtocolVector;

    protected:
        ProtocolVector pv;
        mutable Mutex locker_vector;
        OctetsStream os_src;

    public:
        explicit CompressBinder( Type type ) : Protocol(type) {}
        CompressBinder( const CompressBinder& rhs );
        virtual ~CompressBinder();

        OctetsStream& marshal( OctetsStream& os ) const;
        OctetsStream& unmarshal( OctetsStream& is );

        void Process( NetManager* mgr,ID sid );
        void Notify( NetManager* mgr,ID sid );

        CompressBinder& Bind( const Protocol& p );
        CompressBinder& Bind( const Protocol* p );

        int PriorPolicy() const { return 1; }
        bool SizePolicy( size_t sz ) const { return true; }
    };
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FNET_PROTOCOL_H__