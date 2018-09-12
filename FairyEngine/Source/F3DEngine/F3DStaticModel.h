/*
 * ------------------------------------------------------------------------
 *  Name:   F3DStaticModel.h
 *  Desc:   本文件定义了一个场景里静态模型的对象。
 *  Author: Yish
 *  Date:   2013/1/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_STATICMODEL_H__
#define __F3D_STATICMODEL_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


/** 静态模型
@remarks
	这种模型，有预计算的光照贴图，而不用参与动态光照计算
*/
class FAIRY_API F3DStaticModel
{
public:
	F3DStaticModel();
	virtual ~F3DStaticModel() {}
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_STATICMODEL_H__