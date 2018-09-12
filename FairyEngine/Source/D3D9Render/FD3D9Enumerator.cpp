/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9Enumerator.cpp
 *  Desc:   ���ļ�Ϊģ��ʵ����һ���豸ö������
 *  Author: Yish
 *  Date:   2011/7/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FD3D9Enumerator.h"

/** ���캯��
@Param Direct3D����ָ��
*/
FD3D9Enumerator::FD3D9Enumerator( LPDIRECT3D9 pD3D ) : m_pD3D(pD3D)
{
}

/** ��������
*/
FD3D9Enumerator::~FD3D9Enumerator(void)
{
    DeleteAllAdapters();
}

/** ö�ٵ�ǰ���õ��������б�
*/
bool FD3D9Enumerator::EnumAdapters(void)
{
    UINT nAdapterCount;
    FD3D9Adapter* pAdapter;
    D3DADAPTER_IDENTIFIER9 identifier;
    D3DDISPLAYMODE desktopMode;

    // ����ɵ��������б�
    DeleteAllAdapters();

    // ��ȡ��ǰ�����Կ�����
    nAdapterCount = m_pD3D->GetAdapterCount();

    for( UINT i=0;i<nAdapterCount;i++ )
    {
        pAdapter = new FD3D9Adapter( i );

        // ��ȡ������������
        m_pD3D->GetAdapterIdentifier( i, 0, &identifier );
        pAdapter->SetAdapterIdentifier( identifier );

        // ��ȡ��ǰ��������������ʾģʽ
        m_pD3D->GetAdapterDisplayMode( i, &desktopMode );
        pAdapter->SetDesktopMode( desktopMode );

        // ����µ�����������
        m_Adapters.push_back( pAdapter );
    }

    if( m_Adapters.size() == 0 )
	{
        FLOG_ERROR("FD3D9Enumerator::EnumAdapters, Can't find the valid video adapter!");
		return false;
	}

	return true;
}

/** ���������ʾ������
*/
void FD3D9Enumerator::DeleteAllAdapters(void)
{
    AdapterList::iterator it = m_Adapters.begin();
    while( it != m_Adapters.end() )
    {
        F_SAFE_DELETE( *it );
        ++it;
    }

    m_Adapters.clear();
}