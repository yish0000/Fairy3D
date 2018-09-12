/*
 * -----------------------------------------------------------------------
 *  Name:   FlyGBufferManager.h
 *  Desc:   ���ļ�Ϊ���涨����һ��ͼ�λ�������������
 *  Author: Yish
 *  Date:   2011/7/4
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __FLY_GBUFFERMANAGER_H__
#define __FLY_GBUFFERMANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyGraphicBuffer.h"

//////////////////////////////////////////////////////////////////////////


/** ͼ�λ�����������
*/
class _FLY_EXPORT FlyGBufferManager : public GeneralAlloc
{
public:
    // ��������
    ~FlyGBufferManager(void);

    // ��ȡȫ��Ψһ��ʵ��
    static FlyGBufferManager& Instance(void) { FASSERT(s_pInstance); return *s_pInstance; }
    static FlyGBufferManager* InstancePtr(void) { return s_pInstance; }

private:
    // ˽�еĹ��캯��
    FlyGBufferManager(void);

    // ȫ��Ψһ��ʵ��
    static FlyGBufferManager* s_pInstance;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_GBUFFERMANAGER_H__