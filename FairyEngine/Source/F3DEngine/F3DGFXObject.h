/*
 * ------------------------------------------------------------------------
 *  Name:   F3DGFXObject.h
 *  Desc:   This file define the class for GFX object.
 *  Author: Yish
 *  Date:   2015/2/17
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_GFX_OBJECT_H__
#define __F3D_GFX_OBJECT_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"
#include "F3DGFXElement.h"

///////////////////////////////////////////////////////////////////////////


class FAIRY_API F3DGFXObject : public FGeneralAlloc
{
public:

	typedef std::vector<F3DGFXElement*> GFXElementList;

public:
	F3DGFXObject() {}
	virtual ~F3DGFXObject() {}

	// Load the GFX data from file.
	virtual bool Load(const char* filename) { return true; }
	// Save the GFX data to the specified file.
	virtual void Save(const char* filename) {}

protected:
	GFXElementList m_Elements;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_GFX_OBJECT_H__