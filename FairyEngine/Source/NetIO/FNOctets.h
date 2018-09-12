/*
 * ------------------------------------------------------------------------
 *  Name:   FNOctets.h
 *  Desc:   本文件定义了一个网络库的八位组对象。
 *  Author: Yish
 *  Date:   2012/2/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FNET_OCTETS_H__
#define __FNET_OCTETS_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FNBase.h"

///////////////////////////////////////////////////////////////////////////


namespace FNET {

    /** 八位组对象
    */
    class _FNET_EXPORT Octets
    {
    public:
        Octets() : m_pBase(NULL),m_pHigh(NULL),m_nCaps(0) {};
        Octets( size_t sz ) : m_pBase(NULL),m_pHigh(NULL),m_nCaps(0) { reserve(sz); }
        Octets( const void* data,size_t sz ) : m_pBase(NULL),m_pHigh(NULL),m_nCaps(0) { replace(data,sz); }
        Octets( const void* f,const void* t ) : m_pBase(NULL),m_pHigh(NULL),m_nCaps(0) { replace(f,(char*)t-(char*)f); }
        Octets( const Octets& x ) : m_pBase(NULL),m_pHigh(NULL),m_nCaps(0) { if(x.size()) replace(x.begin(),x.size()); }
        virtual ~Octets() { F_FREE(m_pBase); }

        Octets& operator = ( const Octets& x ) { return replace(x.begin(),x.size()); }
        bool operator == ( const Octets& x ) const { return x.size()==size() && !memcmp(x.m_pBase,m_pBase,size()); }
        bool operator != ( const Octets& x ) const { return !operator==(x); }

        void* begin() { return m_pBase; }
        void* end() { return m_pHigh; }
        const void* begin() const { return m_pBase; }
        const void* end() const { return m_pHigh; }
        bool empty() const { return m_pHigh == m_pBase; }
        Octets& clear() { m_pHigh = m_pBase; return *this; }
        size_t size() const { return (size_t)((char*)m_pHigh - (char*)m_pBase); }
        size_t capcity() const { return m_nCaps; }
        Octets& reserve( size_t sz );
        Octets& resize( size_t sz );
        Octets& insert( void* pos,const void* data,size_t sz );
        Octets& insert( void* pos,const void* f,const void* t );
        Octets& replace( const void* data,size_t sz );
        Octets& erase( void* f,void* t );
        Octets& swap( Octets& rhs );

    protected:
        void* m_pBase;
        void* m_pHigh;
        size_t m_nCaps;
    };
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FNET_OCTETS_H__