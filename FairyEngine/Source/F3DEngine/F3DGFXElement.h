/*
 * ------------------------------------------------------------------------
 *  Name:   F3DGFXElement.h
 *  Desc:   This file define the base class of GFX elements.
 *  Author: Yish
 *  Date:   2015/2/17
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_GFX_ELEMENT_H__
#define __F3D_GFX_ELEMENT_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


class FAIRY_API F3DGFXElement : public FGeneralAlloc
{
public:
	F3DGFXElement() {}
	virtual ~F3DGFXElement() {}

	virtual bool Init() { return true; }
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_GFX_ELEMENT_H__