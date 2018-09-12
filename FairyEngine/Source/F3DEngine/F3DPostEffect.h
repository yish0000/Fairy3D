/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPostEffect.h
 *  Desc:   本文件定义了一个后期处理特效的基类。
 *  Author: Yish
 *  Date:   2012/4/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_POSTEFFECT_H__
#define __F3D_POSTEFFECT_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


/** 后期特效基类
@remarks
*/
class FAIRY_API F3DPostEffect : public FGeneralAlloc
{
public:
    F3DPostEffect();
    virtual ~F3DPostEffect() {};
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_POSTEFFECT_H__