/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTerrainBlock.h
 *  Desc:   本文件定义了一个引擎中的地形块对象。
 *  Author: Yish
 *  Date:   2012/2/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_TERRAINBLOCK_H__
#define __F3D_TERRAINBLOCK_H__


//// HEADERS OF THE FILE //////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


/** 地形块对象
@remarks
    无缝地图按块划分，是资源加载的最小单位。
*/
class _F3D_EXPORT F3DTerrainBlock
{
public:
    F3DTerrainBlock();
    virtual ~F3DTerrainBlock();
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_TERRAINBLOCK_H__