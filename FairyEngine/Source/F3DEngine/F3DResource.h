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
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


/** 句柄对象
*/
template <class TAG> class F3DHandle
{
private:
    uint32 m_handle;

public:
    F3DHandle( uint32 idx,uint32 uid ) : m_handle(idx | (uid << 16)) {}

    // 获取句柄对应资源的索引
    uint32 GetIndex() const { return m_handle & 0xffff; }

    // 获取对应资源的ID
    uint32 GetUniqueID() const { return m_handle >> 16; }

    // 句柄是否为空
    bool IsValid() const { return m_handle != NULL; }

    operator uint32 () const { return m_handle; }
    bool operator == ( const F3DHandle& rhs ) const { return m_handle == rhs.m_handle; }
    bool operator != ( const F3DHandle& rhs ) const { return m_handle != rhs.m_handle; }
};

/** 句柄管理器
*/
template <class TYPE> class F3DHandleManager
{
public:

    struct _INSTANCE
    {
        F3DHandle<TYPE> m_handle;
        TYPE* pInst;

        _INSTANCE() : m_handle(0,0),pInst(0) {}
    };

    typedef std::vector<_INSTANCE> InstanceArray;

public:
    F3DHandleManager() : m_nNextUniqueID(1),m_nFreeIndex(0) {}

    // 为资源分配一个句柄
    F3DHandle<TYPE> Create( TYPE* pInst )
    {
        if( !pInst )
            return F3DHandle<TYPE>(0, 0);

        if( m_nFreeIndex >= m_Handles.size() )
        {
            _INSTANCE newHandle;
            newHandle.pInst = pInst;
            newHandle.m_handle = F3DHandle<TYPE>(m_Handles.size(), m_nNextUniqueID++);
            m_Handles.push_back(newHandle);
            m_nFreeIndex++;
            return newHandle.m_handle;
        }
        else
        {
            _INSTANCE& inst = m_Handles[m_nFreeIndex];
            uint32 nNextFreeIndex = inst.m_handle.GetIndex();
            inst.pInst = pInst;
            inst.m_handle = F3DHandle<TYPE>(m_nFreeIndex, m_nNextUniqueID++);
            m_nFreeIndex = nNextFreeIndex;  // 移动到下一个空闲句柄
            return inst.m_handle;
        }
    }

    // 解析句柄到实例指针
    TYPE* Dereference( const F3DHandle<TYPE>& handle )
    {
        FASSERT( handle.IsValid() );
        int index = handle.GetIndex();

        if( index > m_Handles.size() || m_Handles[index].m_handle != handle )
        {
            // 无效的句柄
            FASSERT(0);
            return NULL;
        }

        return m_Handles[index].pInst;
    }

    const TYPE* Dereference( const F3DHandle<TYPE>& handle ) const
    {
        FASSERT( handle.IsValid() );
        int index = handle.GetIndex();
        
        if( index > m_Handles.size() || m_Handles[index].m_handle != handle )
        {
            // 无效的句柄
            FASSERT(0);
            return NULL;
        }
        
        return m_Handles[index].pInst;
    }

    // 销毁指定句柄
    bool Destroy( const F3DHandle<TYPE>& handle )
    {
        FASSERT( handle.IsValid() );
        int index = handle.GetIndex();

        if( index > m_Handles.size() || m_Handles[index].m_handle != handle )
        {
            FASSERT(0);
            return false;
        }

        m_Handles[index].pInst = NULL;
        m_Handles[index].m_handle = F3DHandle<TYPE>(m_nFreeIndex, 0);   // 指向上一个空闲句柄

        m_nFreeIndex = index;
        return true;
    }

protected:
    uint32 m_nNextUniqueID;
    uint32 m_nFreeIndex;
    InstanceArray m_Handles;
};

///////////////////////////////////////////////////////////////////////////

/** 资源对象
*/
class FAIRY_API F3DResource : public FGeneralAlloc
{
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