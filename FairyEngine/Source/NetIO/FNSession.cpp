/*
 * ------------------------------------------------------------------------
 *  Name:   FNSession.cpp
 *  Desc:   ���ļ�ʵ���������ĻỰ����
 *  Author: Yish
 *  Date:   2012/5/17
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FNSession.h"


namespace FNET {

    /** ���캯��
    */
    Session::Session() : closing(false),ibuffer(DEFAULTIOBUF),obuffer(DEFAULTIOBUF),
        isec(Secure::Create(SECURE_NULL)),osec(Secure::Create(SECURE_NULL))
    {
    }

    /** ���캯��
    */
    Session::Session( const Session& rhs ) : closing(false),ibuffer(rhs.ibuffer.capcity()),
        obuffer(rhs.obuffer.capcity()),isec(rhs.isec->Clone()),osec(rhs.osec->Clone())
    {
    }

    /** ��������
    */
    Session::~Session()
    {
        F_SAFE_DELETE( isec );
        F_SAFE_DELETE( osec );
    }

    /** �������밲ȫ����
    */
    void Session::SetISecure( ENetSecure type,const Octets& key )
    {
        Mutex::ScopeLock l(locker);
        delete isec;
        isec = Secure::Create( type );
        isec->SetParameter( key );
    }

    /** ���������ȫ����
    */
    void Session::SetOSecure( ENetSecure type,const Octets& key )
    {
        Mutex::ScopeLock l(locker);
        delete osec;
        osec = Secure::Create( type );
        osec->SetParameter( key );
    }

    /** ����������ݽ��м���
    */
    Octets& Session::Input()
    {
        isec->Update( ibuffer );
        isecbuf.insert( isecbuf.end(),ibuffer.begin(),ibuffer.end() );
        ibuffer.clear();
        return isecbuf;
    }

    /** ����������ݽ��м���
    */
    bool Session::Output( Octets& data )
    {
        if( data.size() + obuffer.size() > obuffer.capcity() )
            return false;
        osec->Update( data );
        obuffer.insert( obuffer.end(),data.begin(),data.end() );
        return true;
    }

    /** ����������Ϣ
    */
    bool Session::LoadConfig()
    {
        size_t sz;
        FIniFile& conf = NetIO::GetConfig();
        AString section = Identification();

        if( (sz = atoi(conf.GetSetting(section,"ibuffermax").c_str())) ) ibuffer.reserve( sz );
        if( (sz = atoi(conf.GetSetting(section,"obuffermax").c_str())) ) obuffer.reserve( sz );

        if( (sz = atoi(conf.GetSetting(section,"isec").c_str())) )
        {
            AString key = conf.GetSetting( section,"iseckey" );
            SetISecure( (ENetSecure)sz,Octets(&key[0],key.size()) );
        }

        if( (sz = atoi(conf.GetSetting(section,"osec").c_str())) )
        {
            AString key = conf.GetSetting( section,"oseckey" );
            SetOSecure( (ENetSecure)sz,Octets(&key[0],key.size()) );
        }

        return true;
    }
}