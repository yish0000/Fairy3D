/*
 * ------------------------------------------------------------------------
 *  Name:   F3DMeshBase.h
 *  Desc:   本文件定义了引擎的网格基类。
 *  Author: Yish
 *  Date:   2013/1/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_MESHBASE_H__
#define __F3D_MESHBASE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


/** 网格基类
*/
class FAIRY_API F3DMeshBase : public FGeneralAlloc
{
public:
	F3DMeshBase();
	virtual ~F3DMeshBase() {}


};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_MESHBASE_H__