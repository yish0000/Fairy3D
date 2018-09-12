/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTerrainWater.h
 *  Desc:   本文件定义了地形系统的水面效果。
 *  Author: Yish
 *  Date:   2012/3/17
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_TERRAINWATER_H__
#define __F3D_TERRAINWATER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DSceneObject.h"

///////////////////////////////////////////////////////////////////////////


/** 水面效果
*/
class FAIRY_API F3DTerrainWater : public F3DSceneObject
{
public:

	enum
	{
		TECH_SIMPLE,		// Simple water surface.
		TECH_EXPENSIVE,		// Expensive water surface. (Reflection)
	};

public:
    F3DTerrainWater();
    virtual ~F3DTerrainWater() {}

protected:
	int m_iTechType;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_TERRAINWATER_H__