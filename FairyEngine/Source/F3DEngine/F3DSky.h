/*
 * ------------------------------------------------------------------------
 *  Name:   F3DSky.h
 *  Desc:   This file define all types of sky for engine.
 *  Author: Yish
 *  Date:   2015/2/22
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_SKY_H__
#define __F3D_SKY_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DSceneObject.h"

///////////////////////////////////////////////////////////////////////////

/** Base class of sky.
*/
class FAIRY_API F3DSkyBase : public F3DSceneObject
{
public:

	enum
	{
		SKY_CUBE,
		SKY_DOME,
	};

public:
	F3DSkyBase(const AString& name, F3DSceneManager* pOwner);
	virtual ~F3DSkyBase();
};

/** Cube sky box.
*/
class FAIRY_API F3DSkyBox : public F3DSkyBase
{
public:
	F3DSkyBox(const AString& name, F3DSceneManager* pOwner);
	virtual ~F3DSkyBox();
};

/** Sky box like a dome.
*/
class FAIRY_API F3DSkyDome : public F3DSkyBase
{
public:
	F3DSkyDome(const AString& name, F3DSceneManager* pOwner);
	virtual ~F3DSkyDome();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_SKY_H__