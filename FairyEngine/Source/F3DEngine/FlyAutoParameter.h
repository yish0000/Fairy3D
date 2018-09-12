/*
 * -----------------------------------------------------------------------
 *  Name:   FlyAutoParameter.h
 *  Desc:   本文件为引擎定义了一个着色器自动更新参数对象。
 *  Author: Yish
 *  Date:   2011/5/20
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __FLY_AUTOPARAMETER_H__
#define __FLY_AUTOPARAMETER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyGfxBasis.h"

//////////////////////////////////////////////////////////////////////////


/** 自动更新参数类型
*/
enum EAutoParameterType
{
    APT_NONE,
    APT_WORLDMATRIX,
    APT_VIEWMATRIX,
    APT_PROJECTIONMATRIX,
    APT_WORLDVIEWPROJMATRIX,
    APT_WORLDMATRIXARRAY,
    APT_AMBIENTCOLOR,
};

/** 自动更新参数
*/
class _FLY_EXPORT FlyAutoParameter : public GfxResourceAlloc
{
public:
    FlyAutoParameter(void);
    virtual ~FlyAutoParameter(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_AUTOPARAMETER_H__