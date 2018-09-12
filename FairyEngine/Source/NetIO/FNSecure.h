/*
 * ------------------------------------------------------------------------
 *  Name:   FNSecure.h
 *  Desc:   本文件定义了网络数据安全的相关类。
 *  Author: Yish
 *  Date:   2012/3/4
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FNET_SECURE_H__
#define __FNET_SECURE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FNBase.h"
#include "FNOctets.h"

///////////////////////////////////////////////////////////////////////////


namespace FNET {

    /** 网络数据加密方案
    */
    enum ENetSecure
    {
        SECURE_NULL,
        SECURE_RANDOM,
        SECURE_MD5,
    };

    /** 数据加密基类
    */
    class _FNET_EXPORT Secure
    {
    public:
        typedef std::map<ENetSecure,const Secure*> SecureMap;

    protected:
        ENetSecure type;
        static SecureMap map;

        Secure() {}
        Secure( const Secure& rhs ) : type(rhs.type) {}
        Secure( ENetSecure t ) : type(t)
        {
            if( map.find(type) == map.end() )
                map.insert( std::make_pair(type,this) );
        }

    public:
        virtual ~Secure() {}
        virtual void SetParameter( const Octets& o ) {}
        virtual void GetParameter( Octets& o ) {}
        virtual Octets& Update( Octets& o ) = 0;
        virtual Octets& Final( Octets& o ) { return o; }
        virtual Secure* Clone() const = 0;

        static Secure* Create( ENetSecure type )
        {
            SecureMap::const_iterator it = map.find(type);
            if( it == map.end() )
                it = map.find( SECURE_NULL );
            return it->second->Clone();
        }
    };

    /** 不加密方案
    */
    class _FNET_EXPORT NullSecure : public Secure
    {
    public:
        NullSecure() {}
        NullSecure( ENetSecure t ) : Secure(t) {}
        NullSecure( const NullSecure& rhs ) : Secure(rhs.type) {}
        Secure* Clone() const { return new NullSecure(*this); }
        Octets& Update( Octets& o ) { return o; }
    };

    /** 填充随机数
    */
    class _FNET_EXPORT RandomSecure : public Secure
    {
    public:
        RandomSecure() {}
        RandomSecure( ENetSecure t ) : Secure(t) {}
        RandomSecure( const RandomSecure& rhs ) : Secure(rhs) {}
        Secure* Clone() const { return new RandomSecure(*this); }
        Octets& Update( Octets& o );

        static bool Init();
    };

    /** MD5哈希
    */
    class _FNET_EXPORT MD5Secure : public Secure
    {
    public:
        MD5Secure();
        ~MD5Secure() {};
    };
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FNET_SECURE_H__