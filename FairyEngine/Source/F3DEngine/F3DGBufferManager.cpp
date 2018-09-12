/*
 * -----------------------------------------------------------------------
 *  Name:   FlyGBufferManager.cpp
 *  Desc:   本文件为引擎实现了一个图形缓冲区管理器。
 *  Author: Yish
 *  Date:   2011/7/4
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FlyPrecompile.h"

#include "FlyGBufferManager.h"


// 全局唯一的实例指针
FlyGBufferManager* FlyGBufferManager::s_pInstance = NULL;