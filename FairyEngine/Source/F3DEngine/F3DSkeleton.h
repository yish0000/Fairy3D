/*
 * ------------------------------------------------------------------------
 *  Name:   F3DSkeleton.h
 *  Desc:   本文件定义了引擎的骨骼系统。
 *  Author: Yish
 *  Date:   2013/1/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_SKELETON_H__
#define __F3D_SKELETON_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//// Pre-define classes

class F3DBone;

///////////////////////////////////////////////////////////////////////////

/** 骨架系统
*/
class FAIRY_API F3DSkeleton : public FGeneralAlloc
{
public:
	F3DSkeleton();
	virtual ~F3DSkeleton() {}

	virtual void Update( uint32 nDeltaTime ) {}

protected:
	std::vector<F3DBone*> m_Bones;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_SKELETON_H__