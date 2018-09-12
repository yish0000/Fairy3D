/*
 * ------------------------------------------------------------------------
 *  Name:   FD3DAdapter.h
 *  Desc:   ���ļ�Ϊ���涨����һ��D3D����������
 *  Author: Yish
 *  Date:   2011/1/10
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FD3D9_ADAPTER_H__
#define __FD3D9_ADAPTER_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FD3D9Common.h"

///////////////////////////////////////////////////////////////////////////

/** D3D9������
*/
class FD3D9Adapter
{
    typedef std::vector<D3DDISPLAYMODE> DisplayModeList;
    typedef std::pair<D3DMULTISAMPLE_TYPE, DWORD> MultiSampleMode;
    typedef std::vector<MultiSampleMode> MultiSampleModeList;

protected:
    uint32 m_nAdapter;                      // ��������ID
    D3DADAPTER_IDENTIFIER9 m_Identifier;    // D3D�������ı�ʶ��
    D3DDISPLAYMODE m_DesktopMode;           // �������ʾģʽ

    DisplayModeList m_DisplayModes;         // ��ʾģʽ�б�
    MultiSampleModeList m_MSQuality;        // ���õĿ����ģʽ

public:
    // ���캯��
    FD3D9Adapter( uint32 nAdapterID );

    // ö�ٸ�����������ʾģʽ
    void EnumVideoMode( LPDIRECT3D9 pD3D,D3DFORMAT adapterFmt );

    // ö�ٿ��õĿ����ģʽ
    void EnumAntiAliasing( LPDIRECT3D9 pD3D,D3DFORMAT surfaceFmt );

    // ��ȡ������ID
    uint32 GetAdapterID(void) const { return m_nAdapter; }

    // ��ȡ������������
    const D3DADAPTER_IDENTIFIER9& GetDescription(void) const { return m_Identifier; }

    // ��ȡ��ǰ������ʾģʽ
    const D3DDISPLAYMODE& GetDesktopMode(void) const { return m_DesktopMode; }

    // ��ȡ��ʾģʽ����Ŀ
    size_t GetVideoModeCount(void) const { return m_DisplayModes.size(); }

    // ��ȡ��ʾģʽ
    const D3DDISPLAYMODE& GetVideoMode( size_t nIndex ) const
    {
        FASSERT( nIndex < m_DisplayModes.size() );
        return m_DisplayModes[nIndex];
    }

    // ��ȡ�����ģʽ����Ŀ
    size_t GetMultiSampleCount(void) const { return m_MSQuality.size(); }

    // ��ȡָ���Ŀ����ģʽ
    void GetMultiSample( size_t nIndex,D3DMULTISAMPLE_TYPE& msType,DWORD& dwQuality ) const
    {
        FASSERT( nIndex < m_MSQuality.size() );
        msType = m_MSQuality[nIndex].first;
        dwQuality = m_MSQuality[nIndex].second;
    }

    // ���õ�ǰ��������ʾģʽ
    void SetDesktopMode( const D3DDISPLAYMODE& mode );

    // ����������������
    void SetAdapterIdentifier( const D3DADAPTER_IDENTIFIER9& identifier );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FD3D9_ADAPTER_H__