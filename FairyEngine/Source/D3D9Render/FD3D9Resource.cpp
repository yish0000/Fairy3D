/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9Resource.cpp
 *  Desc:   本文件为渲染设备模块实现了D3D资源接口。
 *  Author: Yish
 *  Date:   2011/7/6
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FD3D9Resource.h"
#include "FD3D9RenderDevice.h"


extern FD3D9RenderDevice* g_pDevice;

/** 构造函数
*/
FD3D9Resource::FD3D9Resource(void)
{
    g_pDevice->AddUnmanagedResource( this );
}

/** 析构函数
*/
FD3D9Resource::~FD3D9Resource(void)
{
    g_pDevice->RemoveUnmanagedResource( this );
}