/*
 * ------------------------------------------------------------------------
 *  Name:   F3DResource.h
 *  Desc:   This file defines the base class of all the resources.
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

    // Reload the 
    virtual bool Reload() = 0;

    // 销毁资源
    virtual void Dispose() = 0;

    // 获取资源所占空间的大小
    virtual size_t GetSize() const = 0;

    // 资源是否被销毁
    virtual bool IsDisposed() const = 0;

    // Get the priority of the resource.
    int GetPriority() const { return m_iPriority; }
	// Set the priority of the resource.
    void SetPriority( int iPriority ) { m_iPriority = iPriority; }

    // Get the reference count.
    uint32 GetRefCount() const { return m_nRefCount; }
	// Is this resource dead ?
    bool IsDead() const { return (m_nRefCount == 0); }

    // Get the last access time.
    time_t GetLastAccess() const { return m_LastAccess; }
	// Modify the last access time.
	void SetLastAccess(time_t acessTime) { m_LastAccess = acessTime; }

    // 用于优先级排序的操作符
    bool operator < ( const F3DResource& res );

protected:
    int m_iPriority;        // Priority of the resource.
    uint32 m_nRefCount;     // Reference count of the resource.
    time_t m_LastAccess;    // Last access time.
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_RESOURCE_H__