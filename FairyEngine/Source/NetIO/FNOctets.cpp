/*
 * ------------------------------------------------------------------------
 *  Name:   FNOctets.cpp
 *  Desc:   ���ļ�ʵ���������İ�λ�����
 *  Author: Yish
 *  Date:   2012/5/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FNOctets.h"


namespace FNET {

    /** Ϊ��λ������ڴ�
    */
    Octets& Octets::reserve( size_t sz )
    {
        if( sz > m_nCaps )
        {
            for( m_nCaps=2;sz>>=1;m_nCaps<<=1 );
            sz = (char*)m_pHigh - (char*)m_pBase;
            char* pBase = (char*)F_MALLOC( m_nCaps );
            memcpy( pBase,m_pBase,sz );
            F_FREE( m_pBase );
            m_pBase = pBase;
            m_pHigh = (char*)m_pBase + sz;
        }

        return *this;
    }

    /** ���ð�λ��Ĵ�С
    */
    Octets& Octets::resize( size_t sz )
    {
        reserve( sz );
        m_pHigh = (char*)m_pBase + sz;
        return *this;
    }

    /** ��ָ�����ݸ������λ��
    */
    Octets& Octets::replace( const void* data,size_t sz )
    {
        reserve( sz );
        memmove( m_pBase,data,sz );
        m_pHigh = (char*)m_pBase + sz;
        return *this;
    }

    /** ����ָ��������
    */
    Octets& Octets::insert( void* pos,const void* data,size_t sz )
    {
        reserve( sz + (char*)m_pHigh - (char*)m_pBase );

        if( pos )
        {
            memmove( (char*)pos+sz,pos,(char*)m_pHigh-(char*)pos );
            memmove( pos,data,sz );
            m_pHigh = (char*)m_pHigh + sz;
        }
        else
        {
            memmove( m_pBase,data,sz );
            m_pHigh = (char*)m_pBase + sz;
        }

        return *this;
    }

    /** ����ָ��������
    */
    Octets& Octets::insert( void* pos,const void* f,const void* t )
    {
        return insert(pos,f,(size_t)((char*)t-(char*)f));
    }

    /** ���ָ�������ݶ�
    */
    Octets& Octets::erase( void* f,void* t )
    {
        if( t != f )
        {
            memmove( f,t,(char*)m_pHigh-(char*)t );
            m_pHigh = (char*)m_pHigh - ((char*)t - (char*)f);
        }

        return *this;
    }

    /** ����������λ��
    */
    Octets& Octets::swap( Octets& rhs )
    {
        std::swap( m_pBase,rhs.m_pBase );
        std::swap( m_pHigh,rhs.m_pHigh );
        std::swap( m_nCaps,rhs.m_nCaps );
        return *this;
    }
}