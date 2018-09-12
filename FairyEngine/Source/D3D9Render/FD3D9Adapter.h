/*
 * ------------------------------------------------------------------------
 *  Name:   FD3DAdapter.h
 *  Desc:   本文件为引擎定义了一个D3D适配器对象。
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

/** D3D9适配器
*/
class FD3D9Adapter
{
    typedef std::vector<D3DDISPLAYMODE> DisplayModeList;
    typedef std::pair<D3DMULTISAMPLE_TYPE, DWORD> MultiSampleMode;
    typedef std::vector<MultiSampleMode> MultiSampleModeList;

protected:
    uint32 m_nAdapter;                      // 适配器的ID
    D3DADAPTER_IDENTIFIER9 m_Identifier;    // D3D适配器的标识符
    D3DDISPLAYMODE m_DesktopMode;           // 桌面的显示模式

    DisplayModeList m_DisplayModes;         // 显示模式列表
    MultiSampleModeList m_MSQuality;        // 可用的抗锯齿模式

public:
    // 构造函数
    FD3D9Adapter( uint32 nAdapterID );

    // 枚举该适配器的显示模式
    void EnumVideoMode( LPDIRECT3D9 pD3D,D3DFORMAT adapterFmt );

    // 枚举可用的抗锯齿模式
    void EnumAntiAliasing( LPDIRECT3D9 pD3D,D3DFORMAT surfaceFmt );

    // 获取适配器ID
    uint32 GetAdapterID(void) const { return m_nAdapter; }

    // 获取适配器的描述
    const D3DADAPTER_IDENTIFIER9& GetDescription(void) const { return m_Identifier; }

    // 获取当前桌面显示模式
    const D3DDISPLAYMODE& GetDesktopMode(void) const { return m_DesktopMode; }

    // 获取显示模式的数目
    size_t GetVideoModeCount(void) const { return m_DisplayModes.size(); }

    // 获取显示模式
    const D3DDISPLAYMODE& GetVideoMode( size_t nIndex ) const
    {
        FASSERT( nIndex < m_DisplayModes.size() );
        return m_DisplayModes[nIndex];
    }

    // 获取抗锯齿模式的数目
    size_t GetMultiSampleCount(void) const { return m_MSQuality.size(); }

    // 获取指定的抗锯齿模式
    void GetMultiSample( size_t nIndex,D3DMULTISAMPLE_TYPE& msType,DWORD& dwQuality ) const
    {
        FASSERT( nIndex < m_MSQuality.size() );
        msType = m_MSQuality[nIndex].first;
        dwQuality = m_MSQuality[nIndex].second;
    }

    // 设置当前的桌面显示模式
    void SetDesktopMode( const D3DDISPLAYMODE& mode );

    // 设置适配器的描述
    void SetAdapterIdentifier( const D3DADAPTER_IDENTIFIER9& identifier );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FD3D9_ADAPTER_H__