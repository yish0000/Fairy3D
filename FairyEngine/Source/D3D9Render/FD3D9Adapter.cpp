/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9Adapter.cpp
 *  Desc:   ���ļ�Ϊ��Ⱦ�豸ģ��ʵ����һ��D3D9����������
 *  Author: Yish
 *  Date:   2011/7/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FD3D9Adapter.h"
#include "FD3D9Util.h"

/** ����Ϊ��ʾģʽ����
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

/** ���캯��
*/
FD3D9Adapter::FD3D9Adapter( uint32 nAdapterID ) : m_nAdapter(nAdapterID)
{
    memset( &m_DesktopMode,0,sizeof(D3DDISPLAYMODE) );
    memset( &m_Identifier,0,sizeof(D3DADAPTER_IDENTIFIER9) );
}

/** ö������������ʾģʽ
*/
void FD3D9Adapter::EnumVideoMode( LPDIRECT3D9 pD3D,D3DFORMAT adapterFmt )
{
    UINT nModeCount;
    D3DDISPLAYMODE mode;

    // ���������������ʾģʽ�б�
    m_DisplayModes.clear();

    // ��ȡ��ʾģʽ�ĸ���
    nModeCount = pD3D->GetAdapterModeCount( m_nAdapter,adapterFmt );

    for( UINT j=0;j<nModeCount;j++ )
    {
        if( FAILED(pD3D->EnumAdapterModes(m_nAdapter,adapterFmt,j,&mode)) )
            continue;

        // ����16λ���µ���ɫ��ʽ
        if( mode.Width < 640 || mode.Height < 400 ||
            FD3D9Util::GetFormatBits(mode.Format) < 16 )
        {
            continue;
        }

        m_DisplayModes.push_back( mode );
    }

    // ����ʾģʽ��������
    std::sort( m_DisplayModes.begin(),m_DisplayModes.end() );
}

/** ö�ٿ��õĿ����ģʽ
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