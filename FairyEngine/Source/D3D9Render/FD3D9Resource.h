/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9Resource.h
 *  Desc:   本文件为渲染设备模块定义了一个D3D资源接口，用于在设备丢失时
 *          控制资源的释放和恢复。
 *  Author: Yish
 *  Date:   2011/7/6
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FD3D9_RESOURCE_H__
#define __FD3D9_RESOURCE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FD3D9Common.h"

///////////////////////////////////////////////////////////////////////////


/** D3D资源接口
@remarks
    每个Direct3D资源类都必须继承这个类，当渲染设备丢失时，使资源释放，
    设备恢复时恢复资源。
*/
class FD3D9Resource
{
public:
    // 构造函数和析构函数
    FD3D9Resource(void);
    virtual ~FD3D9Resource(void);

    // 当设备丢失时调用此函数
    virtual void OnDeviceLost(void) = 0;

    // 当恢复设备时调用此函数
    virtual void OnDeviceReset(void) = 0;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FD3D9_RESOURCE_H__