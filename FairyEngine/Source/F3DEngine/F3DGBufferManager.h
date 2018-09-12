/*
 * -----------------------------------------------------------------------
 *  Name:   FlyGBufferManager.h
 *  Desc:   本文件为引擎定义了一个图形缓冲区管理器。
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


/** 图形缓冲区管理器
*/
class _FLY_EXPORT FlyGBufferManager : public GeneralAlloc
{
public:
    // 析构函数
    ~FlyGBufferManager(void);

    // 获取全局唯一的实例
    static FlyGBufferManager& Instance(void) { FASSERT(s_pInstance); return *s_pInstance; }
    static FlyGBufferManager* InstancePtr(void) { return s_pInstance; }

private:
    // 私有的构造函数
    FlyGBufferManager(void);

    // 全局唯一的实例
    static FlyGBufferManager* s_pInstance;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_GBUFFERMANAGER_H__