/*
 * ------------------------------------------------------------------------
 *  Name:   FNProtocol.cpp
 *  Desc:   本文件实现了网络库的协议对象。
 *  Author: Yish
 *  Date:   2012/2/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FNProtocol.h"
#include "FCompress.h"
#include "FStrUtil.h"


#pragma warning(disable:4355)

namespace FNET {

    // 定义静态变量
    NetSession::ID NetSession::session_id = 0;

    ///////////////////////////////////////////////////////////////////////
    // 
    // 网络协议基类的实现
    // 
    ///////////////////////////////////////////////////////////////////////

    /** 构造函数
    */
    Protocol::Protocol( Type t ) : type(t)
    {
        if( !GetStub(type) )
            GetMap().insert( std::make_pair(type,this) );
    }

    /** 获取协议类型映射
    */
    Protocol::ProtocolMap& Protocol::GetMap()
    {
        static Protocol::ProtocolMap map;
        return map;
    }

    /** 根据类型获取协议的存根
    */
    Protocol* Protocol::GetStub( Protocol::Type type )
    {
        ProtocolMap::iterator it = GetMap().find( type );
        return it == GetMap().end() ? NULL : it->second;
    }

    /** 创建指定类型的协议
    */
    Protocol* Protocol::Create( Protocol::Type type )
    {
        Protocol* stub = GetStub( type );
        return stub->Clone();
    }

    /** 将协议包压入数据流
    */
    void Protocol::Encode( OctetsStream& os ) const
    {
        os << CompactUINT(type) << (OctetsStream() << *this);
    }

    /** 响应协议包的到来
    */
    void Protocol::Notify( NetManager* mgr,ID sid )
    {
        mgr->OnRecvProtocol( sid,this );
    }

    /** 处理协议的线程工作函数
    */
    void Protocol::Task::Run()
    {
        try
        {
            protocol->Notify( manager,sid );
        }
        catch( Exception& )
        {
            manager->Close( sid );
        }

        delete this;
    }

    /** 分派收到的协议
    */
    void Protocol::Task::Dispatch( NetManager* mgr,ID sid,Protocol* p )
    {
        Task* task = new Task( mgr,sid,p );
        ThreadPool::Instance().AddTask( task );
        NetIO::GetInstance().WakeUp();
    }

    /** 从数据流中解析协议
    */
    Protocol* Protocol::Decode( OctetsStream* is )
    {
        NetSession::Stream* pis = dynamic_cast<NetSession::Stream*>(is);
        if( !pis || pis->eos() ) return NULL;

        Protocol* protocol = NULL;
        Protocol::Type type = 0;

        try
        {
            size_t sz = 0;
            if( pis->check_policy )
            {
                (*pis) >> OctetsStream::BEGIN >> CompactUINT(type) >> CompactUINT(sz) >> OctetsStream::ROLLBACK;
                if( !pis->session->StatePolicy(type) || !pis->session->manager->InputPolicy(type,sz) )
                {
                    NetIO::errmsg = AStringUtil::Format( "Policy violation: type=%d size=%d\n",type,sz );
                    throw Exception();
                }

                pis->check_policy = false;
            }

            NetSession::Stream data(pis->session);
            (*pis) >> OctetsStream::BEGIN >> CompactUINT(type) >> data >> OctetsStream::COMMIT;
            if( (protocol = Protocol::Create(type)) )
                data >> *protocol;
            pis->check_policy = true;
        }
        catch( Exception& )
        {
            if( protocol )
            {
                delete protocol;
                NetIO::errmsg = AStringUtil::Format( "Decode error %d\n",type );
                throw Exception();
            }

            (*pis) >> OctetsStream::ROLLBACK;
        }

        return protocol;
    }

    ///////////////////////////////////////////////////////////////////////
    // 
    // 网络会话的实现
    // 
    ///////////////////////////////////////////////////////////////////////

    /** 构造函数
    */
    NetSession::NetSession( NetManager* mgr ) : sid(session_id++),isc(this),is(this),
        manager(mgr),state(mgr->GetInitState())
    {
    }

    /** 赋值构造
    */
    NetSession::NetSession( const NetSession& rhs ) : Session(rhs),isc(this),is(this),
        sid(session_id++),manager(rhs.manager),state(rhs.state)
    {
    }

    /** 响应会话的打开
    */
    void NetSession::OnOpen()
    {
        timer.Reset();
        manager->AddSession( sid,this );
    }

    /** 响应会话的关闭
    */
    void NetSession::OnClose()
    {
        manager->DelSession( sid,this );
    }

    /** 响应会话的退出
    */
    void NetSession::OnAbort()
    {
        manager->AbortSession( sid,this );
    }

    /** 响应检查地址
    */
    void NetSession::OnCheckAddress( SockAddr& sa )
    {
        manager->OnCheckAddress( sa );
    }

    /** 获取该会话在配置文件中的标识
    */
    AString NetSession::Identification() const
    {
        return manager->Identification();
    }

    /** 关闭网络会话
    */
    void NetSession::Close()
    {
        NetIO::errmsg = "Active Close\n";
        Session::Close();
    }

    /** 响应发送数据
    */
    void NetSession::OnSend()
    {
        if( state->TimePolicy(timer.Elapse()) )
        {
            if( manager->compress )
            {
                Compress();
                if( osc.size() )
                {
                    for( ;osc.size()&&Output(osc.front());osc.pop() );
                    timer.Reset();
                }
            }
            else
            {
                if( os.size() && Output(os.front()) )
                {
                    os.pop_front();
                    timer.Reset();
                }
            }
        }
        else
        {
            NetIO::errmsg = "Local timeout\n";
            Session::Close();
        }
    }

    /** 响应接收到数据
    */
    void NetSession::OnRecv()
    {
        timer.Reset();
        Octets& input = Input();

        if( manager->compress )
        {
            isc.insert( isc.end(),input.begin(),input.end() );
            Decompress();
        }
        else
        {
            is.insert( is.end(),input.begin(),input.end() );
        }

        input.clear();

        // 解析协议
        try
        {
            for( Protocol* p; (p = Protocol::Decode(&is)); 
                Protocol::Task::Dispatch(manager,sid,p) );
        }
        catch( Exception& )
        {
            NetIO::errmsg = "Decode protocol error\n";
            Session::Close();
        }
    }

    /** 压缩数据
    */
    bool NetSession::Compress()
    {
        if( !os.size() ) return false;

        OctetsStream ss;    // 存储源数据
        OctetsStream cs;    // 存储压缩后的数据
        OctetsStream temp;

		bool bRet = false;
		FCompressor* pComp = FCompressor::Create(FCompressor::COMPRESS_ZLIB);

        for( ;os.size(); )
        {
            ss.insert( ss.end(),os.front().begin(),os.front().size() );
            os.pop_front();
        }

        temp.reserve( pComp->CompressBound((uint32)ss.size()) );
        uint32 dstLen = (uint32)temp.capcity();
        if( pComp->Compress((fbyte*)ss.begin(),(uint32)ss.size(),(fbyte*)(temp.begin()),dstLen) )
        {
            temp.resize( dstLen );
            uint32 src_len = (uint32)ss.size();
            cs << CompactUINT( src_len );
            cs << temp;
            osc.push( cs );
			bRet = true;
        }
        else
        {
            NetIO::errmsg = "Compress failed!\n";
        }

		delete pComp;
        return true;
    }

    /** 解压数据
    */
    void NetSession::Decompress()
    {
        try
        {
			FCompressor* pComp = FCompressor::Create(FCompressor::COMPRESS_ZLIB);
            uint32 sz_org, dst_len;
            OctetsStream data_c, data_o;
            isc >> OctetsStream::BEGIN >> CompactUINT(sz_org) >> data_c >> OctetsStream::COMMIT;
            dst_len = sz_org;
            data_o.reserve( dst_len );
            if( pComp->Decompress((fbyte*)data_c.begin(),(uint32)data_c.size(),(fbyte*)data_o.begin(),dst_len) )
            {
                data_o.resize( dst_len );
                is.insert( is.end(),data_o.begin(),data_o.size() );
            }
			delete pComp;
        }
        catch( ... )
        {
            isc >> OctetsStream::ROLLBACK;
        }
    }

    /** 发送数据
    @Param (urg): 是否插入队列前面
    */
    bool NetSession::Send( const Protocol* p,bool urg )
    {
        Mutex::ScopeLock l(locker);
        OctetsStream ps;
        p->Encode( ps );
        if( urg )
            os.push_front( ps );
        else
            os.push_back( ps );
        NetIO::GetInstance().WakeUp();
        return true;
    }

    /** 改变当前会话的状态
    */
    void NetSession::ChangeState( State* st )
    {
        Mutex::ScopeLock l(locker);
        timer.Reset();
        state = st;
    }

    ///////////////////////////////////////////////////////////////////////
    // 会话状态

    NetSession::State::State( Protocol::Type* first,size_t len,int t/* =0 */ )
        : set(first,first+len),timeout(t)
    {
    }

    NetSession::State::State( Protocol::Type* first,Protocol::Type* last,int t/* =0 */ )
        : set(first,last),timeout(t)
    {
    }

    bool NetSession::State::TypePolicy( Protocol::Type type ) const
    {
        return set.find(type) != set.end();
    }

    bool NetSession::State::TimePolicy( int t ) const
    {
        return timeout <= 0 || t < timeout;
    }

    ///////////////////////////////////////////////////////////////////////
    // 
    // 实现网络会话管理器
    // 
    ///////////////////////////////////////////////////////////////////////

    /** 构造函数
    */
    NetManager::NetManager()
    {
        compress = false;
    }

    /** 添加一个会话
    */
    void NetManager::AddSession( NetSession::ID sid,NetSession* ss )
    {
        Mutex::ScopeLock l(locker_map);
        map.insert( std::make_pair(sid,ss) );
        OnAddSession( sid );
    }

    /** 删除一个会话
    */
    void NetManager::DelSession( NetSession::ID sid,NetSession* ss )
    {
        OnDelSession( sid );

        Mutex::ScopeLock l(locker_map);
        map.erase( sid );
    }

    /** 退出一个会话
    */
    void NetManager::AbortSession( NetSession::ID sid,NetSession* ss )
    {
        OnAbortSession( sid );
    }

    /** 关闭指定的会话
    */
    bool NetManager::Close( NetSession::ID sid )
    {
        Mutex::ScopeLock l(locker_map);
        NetSession* session = GetSession( sid );
        if( session ) session->Close();
        return (session != NULL);
    }

    /** 获取指定的会话
    */
    NetSession* NetManager::GetSession( NetSession::ID sid )
    {
        NetSession::SessionMap::iterator it = map.find( sid );
        return it == map.end() ? NULL : it->second;
    }

    /** 设置网络会话的输入密钥
    */
    bool NetManager::SetISecure( NetSession::ID sid,ENetSecure secure,const Octets& key )
    {
        Mutex::ScopeLock l(locker_map);
        NetSession* session = GetSession( sid );
        if( session ) session->SetISecure( secure,key );
        return (session != NULL);
    }

    /** 设置网络会话的输出密钥
    */
    bool NetManager::SetOSecure( NetSession::ID sid,ENetSecure secure,const Octets& key )
    {
        Mutex::ScopeLock l(locker_map);
        NetSession* session = GetSession( sid );
        if( session ) session->SetOSecure( secure,key );
        return (session != NULL);
    }

    /** 改变网络会话的状态
    */
    bool NetManager::ChangeState( NetSession::ID sid,NetSession::State* state )
    {
        Mutex::ScopeLock l(locker_map);
        NetSession* session = GetSession( sid );
        if( session ) session->ChangeState( state );
        return (session != NULL);
    }

    /** 发送协议到指定的会话
    */
    bool NetManager::Send( NetSession::ID sid,const Protocol* p,bool urg/* =false */ )
    {
        bool ret = false;
        Mutex::ScopeLock l(locker_map);
        NetSession* session = GetSession( sid );
        if( session ) ret = session->Send( p,urg );
        return ret;
    }

    /** 发送协议到指定的会话
    */
    bool NetManager::Send( NetSession::ID sid,const Protocol& p,bool urg/* =false */ )
    {
        return Send( sid,&p,urg );
    }

    /** 检测协议优先级策略
    */
    int NetManager::PriorPolicy( Protocol::Type type ) const
    {
        return Protocol::GetStub(type)->PriorPolicy();
    }

    /** 检测协议长度是否合法
    */
    bool NetManager::InputPolicy( Protocol::Type type,size_t sz ) const
    {
        return Protocol::GetStub(type)->SizePolicy(sz);
    }

    ///////////////////////////////////////////////////////////////////////
    // 协议簇

    /** 赋值构造函数
    */
    ProtocolBinder::ProtocolBinder( const ProtocolBinder& rhs ) : Protocol(rhs)
    {
        Mutex::ScopeLock l(rhs.locker_vector);
        ProtocolVector::const_iterator it;
        for( it=rhs.pv.begin();it!=rhs.pv.end();++it )
        {
            pv.push_back( (*it)->Clone() );
        }
    }

    /** 析构函数
    */
    ProtocolBinder::~ProtocolBinder()
    {
        Mutex::ScopeLock l(locker_vector);
        ProtocolVector::iterator it;
        for( it=pv.begin();it!=pv.end();++it )
            delete (*it);
    }

    /** 绑定协议
    */
    ProtocolBinder& ProtocolBinder::Bind( const Protocol& p )
    {
        Mutex::ScopeLock l(locker_vector);
        pv.push_back( p.Clone() );
        return *this;
    }

    /** 绑定协议
    */
    ProtocolBinder& ProtocolBinder::Bind( const Protocol* p )
    {
        Mutex::ScopeLock l(locker_vector);
        pv.push_back( p->Clone() );
        return *this;
    }

    /** 装载到数据流
    */
    OctetsStream& ProtocolBinder::marshal( OctetsStream& os ) const
    {
        Mutex::ScopeLock l(locker_vector);
        for( ProtocolVector::const_iterator it=pv.begin();it!=pv.end();++it ) (*it)->Encode( os );
        return os;
    }

    /** 从数据流中解析
    */
    OctetsStream& ProtocolBinder::unmarshal( OctetsStream& is )
    {
        NetSession::Stream* pis = dynamic_cast<NetSession::Stream*>(&is);
        Mutex::ScopeLock l(locker_vector);
        for( Protocol* p; (p = Protocol::Decode(pis)); pv.push_back(p) );
        return is;
    }

    /** 响应协议簇的到来
    */
    void ProtocolBinder::Notify( NetManager* mgr,ID sid )
    {
        Mutex::ScopeLock l(locker_vector);
        for( ProtocolVector::iterator it=pv.begin();it!=pv.end();++it )
        {
            try
            {
                mgr->OnRecvProtocol( sid,*it );
            }
            catch( Exception& )
            {
                mgr->Close( sid );
            }
        }

        pv.clear();
        delete this;
    }

    ///////////////////////////////////////////////////////////////////////
    // 压缩后的协议簇

    /** 构造函数
    */
    CompressBinder::CompressBinder( const CompressBinder& rhs ) : Protocol(rhs)
    {
        Mutex::ScopeLock l(rhs.locker_vector);
        for( ProtocolVector::const_iterator it=rhs.pv.begin();it!=rhs.pv.end();++it )
        {
            pv.push_back( (*it)->Clone() );
        }
    }

    /** 析构函数
    */
    CompressBinder::~CompressBinder()
    {
        Mutex::ScopeLock l(locker_vector);
        pv.clear();
    }

    /** 绑定一个协议
    */
    CompressBinder& CompressBinder::Bind( const Protocol* p )
    {
        Mutex::ScopeLock l(locker_vector);
        OctetsStream o;
        p->Encode( o );
        os_src.push_byte( (char*)o.begin(),o.size() );
        return *this;
    }

    /** 绑定一个协议
    */
    CompressBinder& CompressBinder::Bind( const Protocol& p )
    {
        return Bind( &p );
    }

    /** 装载数据
    */
    OctetsStream& CompressBinder::marshal( OctetsStream& os ) const
    {
        Octets os_c;
		std::auto_ptr<FCompressor> pComp = FCompressor::Create(FCompressor::COMPRESS_ZLIB);
        os_c.reserve( pComp->CompressBound((uint32)os_src.size()) );
        uint32 len_c = (uint32)os_c.capcity();
        if( pComp->Compress((fbyte*)os_src.begin(),(uint32)os_src.size(),
            (fbyte*)os_c.begin(),len_c) && len_c < os_src.size() )
        {
            os_c.resize( len_c );
            os << CompactUINT((uint32)os_src.size()) << CompactUINT((uint32)os_c.size());
            os.insert( os.end(),os_c.begin(),os_c.end() );
            return os;
        }

        os << CompactUINT((uint32)os_src.size()) << CompactUINT((uint32)os_src.size());
        os.insert( os.end(),os_src.begin(),os_src.end() );
        return os;
    }

    /** 从数据流中取数据
    */
    OctetsStream& CompressBinder::unmarshal( OctetsStream& is )
    {
        NetSession::Stream& iis = (NetSession::Stream&)is;

        uint32 len_src, len_c;
        iis >> CompactUINT(len_src) >> OctetsStream::BEGIN >> CompactUINT(len_c);
        if( len_c < len_src )
        {
            iis >> OctetsStream::ROLLBACK;

            OctetsStream os_c;
            NetSession::Stream is_src(iis.session);
            os_c.reserve( len_c + 8 );
            iis >> os_c;
            is_src.reserve( len_src );

			FCompressor* pComp = FCompressor::Create(FCompressor::COMPRESS_ZLIB);
            if( pComp->Decompress((fbyte*)os_c.begin(),(uint32)os_c.size(),
                (fbyte*)is_src.begin(),len_src) )
            {
                is_src.resize( len_src );

                Protocol* p;
                Mutex::ScopeLock l(locker_vector);
                while( !is_src.eos() )
                {
                    p = Protocol::Decode( &is_src );
                    if( !p ) break;
                    pv.push_back( p );
                }
            }
            else
            {
                NetIO::errmsg = "CompressBinder::unmarshal, Decompress failed\n";
            }
			delete pComp;
        }
        else
        {
            iis >> OctetsStream::COMMIT;

            Protocol* p;
            Mutex::ScopeLock l(locker_vector);
            while( !is.eos() )
            {
                p = Protocol::Decode( &iis );
                if( !p ) break;
                pv.push_back( p );
            }
        }

        return is;
    }

    /** 处理到来的协议
    */
    void CompressBinder::Process( NetManager* mgr,ID sid )
    {
        Mutex::ScopeLock l(locker_vector);
        for( ProtocolVector::const_iterator it=pv.begin();it!=pv.end();++it )
        {
            try
            {
                (*it)->Process( mgr,sid );
            }
            catch( Exception& )
            {
                mgr->Close( sid );
            }
        }
    }

    /** 响应协议的到来
    */
    void CompressBinder::Notify( NetManager* mgr,ID sid )
    {
        {
            Mutex::ScopeLock l(locker_vector);
            for( ProtocolVector::const_iterator it=pv.begin();it!=pv.end();++it )
            {
                try
                {
                    mgr->OnRecvProtocol( sid,*it );
                }
                catch( Exception& )
                {
                    mgr->Close( sid );
                }
            }
        }

        delete this;
    }
}