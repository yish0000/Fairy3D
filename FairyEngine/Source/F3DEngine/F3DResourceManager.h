/*
 * ------------------------------------------------------------------------
 *  Name:   F3DResourceManager.h
 *  Desc:   本文件实现了引擎的资源管理器基类。
 *  Author: Yish
 *  Date:   2012/8/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_RESOURCEMANAGER_H__
#define __F3D_RESOURCEMANAGER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DResource.h"

///////////////////////////////////////////////////////////////////////////


/** 资源管理器
*/
class FAIRY_API F3DResourceManager
{
public:
    virtual ~F3DResourceManager() {}

    static F3DResourceManager& GetInstance() { FASSERT(s_pInst); return *s_pInst; }
    static F3DResourceManager* GetInstancePtr() { return s_pInst; }

private:
    F3DResourceManager();

    static F3DResourceManager* s_pInst;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_RESOURCEMANAGER_H__