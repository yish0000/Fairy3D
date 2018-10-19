/*
 * ------------------------------------------------------------------------
 *  Name:   F3DComponent.h
 *  Desc:   Base class for all the components.
 *  Author: Yish
 *  Date:   2018/10/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Shi. All rights Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_COMPONENT_H__
#define __F3D_COMPONENT_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FObject.h"

///////////////////////////////////////////////////////////////////////////

class FAIRY_API F3DComponent : public FObject
{
	F_DECLARE_RTTI(F3DComponent)

public:
	F3DComponent();
	virtual ~F3DComponent();


};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_COMPONENT_H__