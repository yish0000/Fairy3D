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