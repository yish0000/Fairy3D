/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9Adapter.cpp
 *  Desc:   本文件为渲染设备模块实现了一个D3D9适配器对象。
 *  Author: Yish
 *  Date:   2011/7/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FD3D9Adapter.h"
#include "FD3D9Util.h"

/** 用于为显示模式排序
*/
static bool operator < ( const D3DDISPLAYMODE& lhs, const D3DDISPLAYMODE& rhs )
{
    if( lhs.Width < rhs.Height )
    {
        return true;
    }
    else
    {
        if( lhs.Width == rhs.Width )
        {
            if( lhs.Height < rhs.Height )
            {
                return true;
            }
            else
            {
                if( lhs.Height == rhs.Height )
                {
                    if( FD3D9Util::GetFormatBits(lhs.Format) < 
                        FD3D9Util::GetFormatBits(rhs.Format) )
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

/** 构造函数
*/
FD3D9Adapter::FD3D9Adapter( uint32 nAdapterID ) : m_nAdapter(nAdapterID)
{
    memset( &m_DesktopMode,0,sizeof(D3DDISPLAYMODE) );
    memset( &m_Identifier,0,sizeof(D3DADAPTER_IDENTIFIER9) );
}

/** 枚举适配器的显示模式
*/
void FD3D9Adapter::EnumVideoMode( LPDIRECT3D9 pD3D,D3DFORMAT adapterFmt )
{
    UINT nModeCount;
    D3DDISPLAYMODE mode;

    // 清除该适配器的显示模式列表
    m_DisplayModes.clear();

    // 获取显示模式的个数
    nModeCount = pD3D->GetAdapterModeCount( m_nAdapter,adapterFmt );

    for( UINT j=0;j<nModeCount;j++ )
    {
        if( FAILED(pD3D->EnumAdapterModes(m_nAdapter,adapterFmt,j,&mode)) )
            continue;

        // 忽略16位以下的颜色格式
        if( mode.Width < 640 || mode.Height < 400 ||
            FD3D9Util::GetFormatBits(mode.Format) < 16 )
        {
            continue;
        }

        m_DisplayModes.push_back( mode );
    }

    // 将显示模式进行排序
    std::sort( m_DisplayModes.begin(),m_DisplayModes.end() );
}

/** 枚举可用的抗锯齿模式
*/
void FD3D9Adapter::EnumAntiAliasing( LPDIRECT3D9 pD3D,D3DFORMAT surfaceFmt )
{
    m_MSQuality.clear();

    DWORD dwLevel = 0;
    if( SUCCEEDED(pD3D->CheckDeviceMultiSampleType(m_nAdapter,D3DDEVTYPE_HAL,surfaceFmt,
        TRUE,D3DMULTISAMPLE_NONMASKABLE,&dwLevel)) )
    {
        if( dwLevel > 0 )
        {
            for( DWORD i=0;i<dwLevel;i++ )
                m_MSQuality.push_back( std::make_pair(D3DMULTISAMPLE_NONMASKABLE,i) );
        }
    }

    for( int ms=D3DMULTISAMPLE_2_SAMPLES;
         ms<D3DMULTISAMPLE_16_SAMPLES+1;ms++ )
    {
        if( SUCCEEDED(pD3D->CheckDeviceMultiSampleType(m_nAdapter,D3DDEVTYPE_HAL,surfaceFmt,
            TRUE,(D3DMULTISAMPLE_TYPE)ms,&dwLevel)) )
        {
            m_MSQuality.push_back( std::make_pair((D3DMULTISAMPLE_TYPE)ms,0) );
        }
    }
}