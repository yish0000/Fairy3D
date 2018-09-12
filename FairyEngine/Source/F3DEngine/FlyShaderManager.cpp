/*
 * -----------------------------------------------------------------------
 *  Name:   FlyShaderManager.cpp
 *  Desc:   本文件为引擎实现了一个着色器管理器。
 *  Author: Yish
 *  Date:   2011/5/21
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FlyPrecompile.h"

#include "FlyShaderManager.h"


// 全局唯一的实例
FlyShaderManager* FlyShaderManager::s_pInstance = NULL;

/** 构造函数
*/
FlyShaderManager::FlyShaderManager(void)
{
    s_pInstance = this;
}

/** 析构函数
*/
FlyShaderManager::~FlyShaderManager(void)
{
}