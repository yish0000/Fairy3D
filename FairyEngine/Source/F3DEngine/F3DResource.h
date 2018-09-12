/*
 * ------------------------------------------------------------------------
 *  Name:   F3DResource.h
 *  Desc:   ���ļ��������������Դ������ࡣ
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


/** �������
*/
template <class TAG> class F3DHandle
{
private:
    uint32 m_handle;

public:
    F3DHandle( uint32 idx,uint32 uid ) : m_handle(idx | (uid << 16)) {}

    // ��ȡ�����Ӧ��Դ������
    uint32 GetIndex() const { return m_handle & 0xffff; }

    // ��ȡ��Ӧ��Դ��ID
    uint32 GetUniqueID() const { return m_handle >> 16; }

    // ����Ƿ�Ϊ��
    bool IsValid() const { return m_handle != NULL; }

    operator uint32 () const { return m_handle; }
    bool operator == ( const F3DHandle& rhs ) const { return m_handle == rhs.m_handle; }
    bool operator != ( const F3DHandle& rhs ) const { return m_handle != rhs.m_handle; }
};

/** ���������
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

    // Ϊ��Դ����һ�����
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
            m_nFreeIndex = nNextFreeIndex;  // �ƶ�����һ�����о��
            return inst.m_handle;
        }
    }

    // ���������ʵ��ָ��
    TYPE* Dereference( const F3DHandle<TYPE>& handle )
    {
        FASSERT( handle.IsValid() );
        int index = handle.GetIndex();

        if( index > m_Handles.size() || m_Handles[index].m_handle != handle )
        {
            // ��Ч�ľ��
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
            // ��Ч�ľ��
            FASSERT(0);
            return NULL;
        }
        
        return m_Handles[index].pInst;
    }

    // ����ָ�����
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
        m_Handles[index].m_handle = F3DHandle<TYPE>(m_nFreeIndex, 0);   // ָ����һ�����о��

        m_nFreeIndex = index;
        return true;
    }

protected:
    uint32 m_nNextUniqueID;
    uint32 m_nFreeIndex;
    InstanceArray m_Handles;
};

///////////////////////////////////////////////////////////////////////////

/** ��Դ����
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

    // ���¼�����Դ
    virtual bool Reload() = 0;

    // ������Դ
    virtual void Dispose() = 0;

    // ��ȡ��Դ��ռ�ռ�Ĵ�С
    virtual size_t GetSize() const = 0;

    // ��Դ�Ƿ�����
    virtual bool IsDisposed() const = 0;

    // ��Դ���ȼ�
    int GetPriority() const { return m_iPriority; }
    void SetPriority( int iPriority ) { m_iPriority = iPriority; }

    // ���ü������
    uint32 GetRefCount() const { return m_nRefCount; }
    bool IsLocked() const { return (m_nRefCount > 0); }

    // ��Դ�����ʵ�ʱ��
    time_t GetLastAccess() const { return m_LastAccess; }
    void SetLastAccess( time_t acessTime ) { m_LastAccess = acessTime; }

    // �������ȼ�����Ĳ�����
    bool operator < ( const F3DResource& res );

protected:
    int m_iPriority;        // ��Դ�����ȼ�
    uint32 m_nRefCount;     // ��Դ�����ô���
    time_t m_LastAccess;    // ��Դ��һ�α����ʵ�ʱ��
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_RESOURCE_H__