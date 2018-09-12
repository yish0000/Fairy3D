/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9Enumerator.cpp
 *  Desc:   本文件为模块实现了一个设备枚举器。
 *  Author: Yish
 *  Date:   2011/7/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FD3D9Enumerator.h"

/** 构造函数
@Param Direct3D对象指针
*/
FD3D9Enumerator::FD3D9Enumerator( LPDIRECT3D9 pD3D ) : m_pD3D(pD3D)
{
}

/** 析构函数
*/
FD3D9Enumerator::~FD3D9Enumerator(void)
{
    DeleteAllAdapters();
}

/** 枚举当前可用的适配器列表
*/
bool FD3D9Enumerator::EnumAdapters(void)
{
    UINT nAdapterCount;
    FD3D9Adapter* pAdapter;
    D3DADAPTER_IDENTIFIER9 identifier;
    D3DDISPLAYMODE desktopMode;

    // 清除旧的适配器列表
    DeleteAllAdapters();

    // 获取当前机器显卡个数
    nAdapterCount = m_pD3D->GetAdapterCount();

    for( UINT i=0;i<nAdapterCount;i++ )
    {
        pAdapter = new FD3D9Adapter( i );

        // 获取适配器的描述
        m_pD3D->GetAdapterIdentifier( i, 0, &identifier );
        pAdapter->SetAdapterIdentifier( identifier );

        // 获取当前适配器的桌面显示模式
        m_pD3D->GetAdapterDisplayMode( i, &desktopMode );
        pAdapter->SetDesktopMode( desktopMode );

        // 添加新的适配器对象
        m_Adapters.push_back( pAdapter );
    }

    if( m_Adapters.size() == 0 )
	{
        FLOG_ERROR("FD3D9Enumerator::EnumAdapters, Can't find the valid video adapter!");
		return false;
	}

	return true;
}

/** 清除所有显示适配器
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