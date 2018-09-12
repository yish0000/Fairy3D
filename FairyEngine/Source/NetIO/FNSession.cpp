/*
 * ------------------------------------------------------------------------
 *  Name:   FNSession.cpp
 *  Desc:   本文件实现了网络库的会话对象。
 *  Author: Yish
 *  Date:   2012/5/17
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FNSession.h"


namespace FNET {

    /** 构造函数
    */
    Session::Session() : closing(false),ibuffer(DEFAULTIOBUF),obuffer(DEFAULTIOBUF),
        isec(Secure::Create(SECURE_NULL)),osec(Secure::Create(SECURE_NULL))
    {
    }

    /** 构造函数
    */
    Session::Session( const Session& rhs ) : closing(false),ibuffer(rhs.ibuffer.capcity()),
        obuffer(rhs.obuffer.capcity()),isec(rhs.isec->Clone()),osec(rhs.osec->Clone())
    {
    }

    /** 析构函数
    */
    Session::~Session()
    {
        F_SAFE_DELETE( isec );
        F_SAFE_DELETE( osec );
    }

    /** 设置输入安全方案
    */
    void Session::SetISecure( ENetSecure type,const Octets& key )
    {
        Mutex::ScopeLock l(locker);
        delete isec;
        isec = Secure::Create( type );
        isec->SetParameter( key );
    }

    /** 设置输出安全方案
    */
    void Session::SetOSecure( ENetSecure type,const Octets& key )
    {
        Mutex::ScopeLock l(locker);
        delete osec;
        osec = Secure::Create( type );
        osec->SetParameter( key );
    }

    /** 将输入的数据进行加密
    */
    Octets& Session::Input()
    {
        isec->Update( ibuffer );
        isecbuf.insert( isecbuf.end(),ibuffer.begin(),ibuffer.end() );
        ibuffer.clear();
        return isecbuf;
    }

    /** 将输出的数据进行加密
    */
    bool Session::Output( Octets& data )
    {
        if( data.size() + obuffer.size() > obuffer.capcity() )
            return false;
        osec->Update( data );
        obuffer.insert( obuffer.end(),data.begin(),data.end() );
        return true;
    }

    /** 加载配置信息
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