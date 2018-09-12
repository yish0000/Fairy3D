/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9Enumerator.h
 *  Desc:   ���ļ�Ϊģ�鶨����һ���豸ö������
 *  Author: Yish
 *  Date:   2011/7/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FD3D9_ENUMERATOR_H__
#define __FD3D9_ENUMERATOR_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FD3D9Adapter.h"

///////////////////////////////////////////////////////////////////////////


/** �豸��Ⱦö��
@remarks
    ����ö��������õ��������б�
*/
class FD3D9Enumerator
{
    typedef std::vector<FD3D9Adapter*> AdapterList;

protected:
    LPDIRECT3D9 m_pD3D;         // Direct3D����ָ��
    AdapterList m_Adapters;     // ��ʾ�������б�

public:
    // ���캯��/��������
    FD3D9Enumerator( LPDIRECT3D9 pD3D );
    ~FD3D9Enumerator(void);

    // ö�ٵ�ǰ���õ�������
    bool EnumAdapters(void);

    // ���������ʾ������
    void DeleteAllAdapters(void);

    // ��ȡ�Կ��ĸ���
    size_t GetAdapterCount(void) const { return m_Adapters.size(); }

    // ��ȡָ������ʾ����������
    FD3D9Adapter* GetAdapter( size_t nIndex )
    {
        FASSERT( nIndex < m_Adapters.size() );
        return m_Adapters[nIndex];
    }

    // ��ȡDirect3D����ָ��
    LPDIRECT3D9 GetD3DPtr(void) const { return m_pD3D; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FD3D9_ENUMERATOR_H__