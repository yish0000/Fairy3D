/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9Enumerator.h
 *  Desc:   本文件为模块定义了一个设备枚举器。
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


/** 设备渲染枚举
@remarks
    用于枚举引擎可用的适配器列表
*/
class FD3D9Enumerator
{
    typedef std::vector<FD3D9Adapter*> AdapterList;

protected:
    LPDIRECT3D9 m_pD3D;         // Direct3D对象指针
    AdapterList m_Adapters;     // 显示适配器列表

public:
    // 构造函数/析构函数
    FD3D9Enumerator( LPDIRECT3D9 pD3D );
    ~FD3D9Enumerator(void);

    // 枚举当前可用的适配器
    bool EnumAdapters(void);

    // 清除所有显示适配器
    void DeleteAllAdapters(void);

    // 获取显卡的个数
    size_t GetAdapterCount(void) const { return m_Adapters.size(); }

    // 获取指定的显示适配器对象
    FD3D9Adapter* GetAdapter( size_t nIndex )
    {
        FASSERT( nIndex < m_Adapters.size() );
        return m_Adapters[nIndex];
    }

    // 获取Direct3D对象指针
    LPDIRECT3D9 GetD3DPtr(void) const { return m_pD3D; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FD3D9_ENUMERATOR_H__