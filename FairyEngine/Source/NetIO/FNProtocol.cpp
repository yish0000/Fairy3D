/*
 * ------------------------------------------------------------------------
 *  Name:   FNProtocol.cpp
 *  Desc:   ���ļ�ʵ����������Э�����
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

    // ���徲̬����
    NetSession::ID NetSession::session_id = 0;

    ///////////////////////////////////////////////////////////////////////
    // 
    // ����Э������ʵ��
    // 
    ///////////////////////////////////////////////////////////////////////

    /** ���캯��
    */
    Protocol::Protocol( Type t ) : type(t)
    {
        if( !GetStub(type) )
            GetMap().insert( std::make_pair(type,this) );
    }

    /** ��ȡЭ������ӳ��
    */
    Protocol::ProtocolMap& Protocol::GetMap()
    {
        static Protocol::ProtocolMap map;
        return map;
    }

    /** �������ͻ�ȡЭ��Ĵ��
    */
    Protocol* Protocol::GetStub( Protocol::Type type )
    {
        ProtocolMap::iterator it = GetMap().find( type );
        return it == GetMap().end() ? NULL : it->second;
    }

    /** ����ָ�����͵�Э��
    */
    Protocol* Protocol::Create( Protocol::Type type )
    {
        Protocol* stub = GetStub( type );
        return stub->Clone();
    }

    /** ��Э���ѹ��������
    */
    void Protocol::Encode( OctetsStream& os ) const
    {
        os << CompactUINT(type) << (OctetsStream() << *this);
    }

    /** ��ӦЭ����ĵ���
    */
    void Protocol::Notify( NetManager* mgr,ID sid )
    {
        mgr->OnRecvProtocol( sid,this );
    }

    /** ����Э����̹߳�������
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

    /** �����յ���Э��
    */
    void Protocol::Task::Dispatch( NetManager* mgr,ID sid,Protocol* p )
    {
        Task* task = new Task( mgr,sid,p );
        ThreadPool::Instance().AddTask( task );
        NetIO::GetInstance().WakeUp();
    }

    /** ���������н���Э��
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
    // ����Ự��ʵ��
    // 
    ///////////////////////////////////////////////////////////////////////

    /** ���캯��
    */
    NetSession::NetSession( NetManager* mgr ) : sid(session_id++),isc(this),is(this),
        manager(mgr),state(mgr->GetInitState())
    {
    }

    /** ��ֵ����
    */
    NetSession::NetSession( const NetSession& rhs ) : Session(rhs),isc(this),is(this),
        sid(session_id++),manager(rhs.manager),state(rhs.state)
    {
    }

    /** ��Ӧ�Ự�Ĵ�
    */
    void NetSession::OnOpen()
    {
        timer.Reset();
        manager->AddSession( sid,this );
    }

    /** ��Ӧ�Ự�Ĺر�
    */
    void NetSession::OnClose()
    {
        manager->DelSession( sid,this );
    }

    /** ��Ӧ�Ự���˳�
    */
    void NetSession::OnAbort()
    {
        manager->AbortSession( sid,this );
    }

    /** ��Ӧ����ַ
    */
    void NetSession::OnCheckAddress( SockAddr& sa )
    {
        manager->OnCheckAddress( sa );
    }

    /** ��ȡ�ûỰ�������ļ��еı�ʶ
    */
    AString NetSession::Identification() const
    {
        return manager->Identification();
    }

    /** �ر�����Ự
    */
    void NetSession::Close()
    {
        NetIO::errmsg = "Active Close\n";
        Session::Close();
    }

    /** ��Ӧ��������
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

    /** ��Ӧ���յ�����
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

        // ����Э��
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

    /** ѹ������
    */
    bool NetSession::Compress()
    {
        if( !os.size() ) return false;

        OctetsStream ss;    // �洢Դ����
        OctetsStream cs;    // �洢ѹ���������
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

    /** ��ѹ����
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

    /** ��������
    @Param (urg): �Ƿ�������ǰ��
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

    /** �ı䵱ǰ�Ự��״̬
    */
    void NetSession::ChangeState( State* st )
    {
        Mutex::ScopeLock l(locker);
        timer.Reset();
        state = st;
    }

    ///////////////////////////////////////////////////////////////////////
    // �Ự״̬

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
    // ʵ������Ự������
    // 
    ///////////////////////////////////////////////////////////////////////

    /** ���캯��
    */
    NetManager::NetManager()
    {
        compress = false;
    }

    /** ���һ���Ự
    */
    void NetManager::AddSession( NetSession::ID sid,NetSession* ss )
    {
        Mutex::ScopeLock l(locker_map);
        map.insert( std::make_pair(sid,ss) );
        OnAddSession( sid );
    }

    /** ɾ��һ���Ự
    */
    void NetManager::DelSession( NetSession::ID sid,NetSession* ss )
    {
        OnDelSession( sid );

        Mutex::ScopeLock l(locker_map);
        map.erase( sid );
    }

    /** �˳�һ���Ự
    */
    void NetManager::AbortSession( NetSession::ID sid,NetSession* ss )
    {
        OnAbortSession( sid );
    }

    /** �ر�ָ���ĻỰ
    */
    bool NetManager::Close( NetSession::ID sid )
    {
        Mutex::ScopeLock l(locker_map);
        NetSession* session = GetSession( sid );
        if( session ) session->Close();
        return (session != NULL);
    }

    /** ��ȡָ���ĻỰ
    */
    NetSession* NetManager::GetSession( NetSession::ID sid )
    {
        NetSession::SessionMap::iterator it = map.find( sid );
        return it == map.end() ? NULL : it->second;
    }

    /** ��������Ự��������Կ
    */
    bool NetManager::SetISecure( NetSession::ID sid,ENetSecure secure,const Octets& key )
    {
        Mutex::ScopeLock l(locker_map);
        NetSession* session = GetSession( sid );
        if( session ) session->SetISecure( secure,key );
        return (session != NULL);
    }

    /** ��������Ự�������Կ
    */
    bool NetManager::SetOSecure( NetSession::ID sid,ENetSecure secure,const Octets& key )
    {
        Mutex::ScopeLock l(locker_map);
        NetSession* session = GetSession( sid );
        if( session ) session->SetOSecure( secure,key );
        return (session != NULL);
    }

    /** �ı�����Ự��״̬
    */
    bool NetManager::ChangeState( NetSession::ID sid,NetSession::State* state )
    {
        Mutex::ScopeLock l(locker_map);
        NetSession* session = GetSession( sid );
        if( session ) session->ChangeState( state );
        return (session != NULL);
    }

    /** ����Э�鵽ָ���ĻỰ
    */
    bool NetManager::Send( NetSession::ID sid,const Protocol* p,bool urg/* =false */ )
    {
        bool ret = false;
        Mutex::ScopeLock l(locker_map);
        NetSession* session = GetSession( sid );
        if( session ) ret = session->Send( p,urg );
        return ret;
    }

    /** ����Э�鵽ָ���ĻỰ
    */
    bool NetManager::Send( NetSession::ID sid,const Protocol& p,bool urg/* =false */ )
    {
        return Send( sid,&p,urg );
    }

    /** ���Э�����ȼ�����
    */
    int NetManager::PriorPolicy( Protocol::Type type ) const
    {
        return Protocol::GetStub(type)->PriorPolicy();
    }

    /** ���Э�鳤���Ƿ�Ϸ�
    */
    bool NetManager::InputPolicy( Protocol::Type type,size_t sz ) const
    {
        return Protocol::GetStub(type)->SizePolicy(sz);
    }

    ///////////////////////////////////////////////////////////////////////
    // Э���

    /** ��ֵ���캯��
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

    /** ��������
    */
    ProtocolBinder::~ProtocolBinder()
    {
        Mutex::ScopeLock l(locker_vector);
        ProtocolVector::iterator it;
        for( it=pv.begin();it!=pv.end();++it )
            delete (*it);
    }

    /** ��Э��
    */
    ProtocolBinder& ProtocolBinder::Bind( const Protocol& p )
    {
        Mutex::ScopeLock l(locker_vector);
        pv.push_back( p.Clone() );
        return *this;
    }

    /** ��Э��
    */
    ProtocolBinder& ProtocolBinder::Bind( const Protocol* p )
    {
        Mutex::ScopeLock l(locker_vector);
        pv.push_back( p->Clone() );
        return *this;
    }

    /** װ�ص�������
    */
    OctetsStream& ProtocolBinder::marshal( OctetsStream& os ) const
    {
        Mutex::ScopeLock l(locker_vector);
        for( ProtocolVector::const_iterator it=pv.begin();it!=pv.end();++it ) (*it)->Encode( os );
        return os;
    }

    /** ���������н���
    */
    OctetsStream& ProtocolBinder::unmarshal( OctetsStream& is )
    {
        NetSession::Stream* pis = dynamic_cast<NetSession::Stream*>(&is);
        Mutex::ScopeLock l(locker_vector);
        for( Protocol* p; (p = Protocol::Decode(pis)); pv.push_back(p) );
        return is;
    }

    /** ��ӦЭ��صĵ���
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
    // ѹ�����Э���

    /** ���캯��
    */
    CompressBinder::CompressBinder( const CompressBinder& rhs ) : Protocol(rhs)
    {
        Mutex::ScopeLock l(rhs.locker_vector);
        for( ProtocolVector::const_iterator it=rhs.pv.begin();it!=rhs.pv.end();++it )
        {
            pv.push_back( (*it)->Clone() );
        }
    }

    /** ��������
    */
    CompressBinder::~CompressBinder()
    {
        Mutex::ScopeLock l(locker_vector);
        pv.clear();
    }

    /** ��һ��Э��
    */
    CompressBinder& CompressBinder::Bind( const Protocol* p )
    {
        Mutex::ScopeLock l(locker_vector);
        OctetsStream o;
        p->Encode( o );
        os_src.push_byte( (char*)o.begin(),o.size() );
        return *this;
    }

    /** ��һ��Э��
    */
    CompressBinder& CompressBinder::Bind( const Protocol& p )
    {
        return Bind( &p );
    }

    /** װ������
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

    /** ����������ȡ����
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

    /** ��������Э��
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

    /** ��ӦЭ��ĵ���
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