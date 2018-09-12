/*
 * ------------------------------------------------------------------------
 *  Name:   FUIRenderer.h
 *  Desc:   本文件定义了UI子系统的渲染器基类，用户可以派生该类以实现自己
 *          的渲染器。
 *  Author: Yish
 *  Date:   2010/8/15
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_RENDERER_H__
#define __FUI_RENDERER_H__

//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FUIBase.h"

//////////////////////////////////////////////////////////////////////////

/** UI模块的渲染器基类
@remarks
@note
*/
class FAIRY_API FUIRenderer : public FGeneralAlloc
{
public:
    FUIRenderer();
    virtual ~FUIRenderer() {};
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FUI_RENDERER_H__