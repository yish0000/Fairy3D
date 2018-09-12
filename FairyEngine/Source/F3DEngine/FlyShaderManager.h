/*
 * ------------------------------------------------------------------------
 *  Name:   F3DShaderManager.h
 *  Desc:   ���ļ�Ϊ���涨����һ����ɫ�������������
 *  Author: Yish
 *  Date:   2011/5/21
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_SHADERMANAGER_H__
#define __F3D_SHADERMANAGER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DBase.h"

///////////////////////////////////////////////////////////////////////////


/** ��ɫ�����������
*/
class _F3D_EXPORT FlyShaderManager : public GeneralAlloc
{
public:
    ~FlyShaderManager(void);

    // ��ȡ�����ʵ��
    static FlyShaderManager* InstancePtr(void) { return s_pInstance; }
    static FlyShaderManager& Instance(void) { FASSERT(s_pInstance); return *s_pInstance; }

private:
    // ˽�й��캯��
    FlyShaderManager(void);

    // ����ʵ��
    static FlyShaderManager* s_pInstance;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_SHADERMANAGER_H__