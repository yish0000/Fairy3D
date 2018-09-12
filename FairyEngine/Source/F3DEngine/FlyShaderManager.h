/*
 * ------------------------------------------------------------------------
 *  Name:   F3DShaderManager.h
 *  Desc:   本文件为引擎定义了一个着色器程序管理器。
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


/** 着色器程序管理器
*/
class _F3D_EXPORT FlyShaderManager : public GeneralAlloc
{
public:
    ~FlyShaderManager(void);

    // 获取该类的实例
    static FlyShaderManager* InstancePtr(void) { return s_pInstance; }
    static FlyShaderManager& Instance(void) { FASSERT(s_pInstance); return *s_pInstance; }

private:
    // 私有构造函数
    FlyShaderManager(void);

    // 单个实例
    static FlyShaderManager* s_pInstance;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_SHADERMANAGER_H__