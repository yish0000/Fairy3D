/*
 * ------------------------------------------------------------------------
 *  Name:   F3DResource.h
 *  Desc:   本文件定义了引擎的资源对象基类。
 *  Author: Yish
 *  Date:   2012/8/24
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_RESOURCE_H__
#define __F3D_RESOURCE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FObject.h"
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////

/** Base class for all the resources.
*/
class FAIRY_API F3DResource : public FObject
{
	F_DECLARE_RTTI(F3DResource)

public:

    enum
    {
        RES_PRIORITY_LOW,
        RES_PRIORITY_NORMAL,
        RES_PRIORITY_HIGH,
    };

public:
    F3DResource();
    virtual ~F3DResource();

    // 重新加载资源
    virtual bool Reload() = 0;

    // 销毁资源
    virtual void Dispose() = 0;

    // 获取资源所占空间的大小
    virtual size_t GetSize() const = 0;

    // 资源是否被销毁
    virtual bool IsDisposed() const = 0;

    // 资源优先级
    int GetPriority() const { return m_iPriority; }
    void SetPriority( int iPriority ) { m_iPriority = iPriority; }

    // 引用计数相关
    uint32 GetRefCount() const { return m_nRefCount; }
    bool IsLocked() const { return (m_nRefCount > 0); }

    // 资源被访问的时间
    time_t GetLastAccess() const { return m_LastAccess; }
    void SetLastAccess( time_t acessTime ) { m_LastAccess = acessTime; }

    // 用于优先级排序的操作符
    bool operator < ( const F3DResource& res );

protected:
    int m_iPriority;        // 资源的优先级
    uint32 m_nRefCount;     // 资源的引用次数
    time_t m_LastAccess;    // 资源上一次被访问的时间
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_RESOURCE_H__