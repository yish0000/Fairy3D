/*
 * ------------------------------------------------------------------------
 *  Name:   F3DFlatPainter.h
 *  Desc:   本文件定义了一个渲染容器，用于渲染一些零碎的面片。
 *  Author: Yish
 *  Date:   2012/12/22
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_FLATPAINTER_H__
#define __F3D_FLATPAINTER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


/** 面片渲染容器
*/
class FAIRY_API F3DFlatPainter : public FGeneralAlloc
{
public:
	F3DFlatPainter();
	virtual ~F3DFlatPainter() {}

    // 添加一系列3D几何图形
    bool Add3DGeometry(const F3DVector3* pVerts, int iNumVerts, const ushort* pIndices,
        int iNumIndices, uint32 nColor);

    // 添加一个盒子
    bool AddBox(const F3DVector3& vPos, const F3DVector3& vDir, const F3DVector3& vRight,
        const F3DVector3& vCenter, const F3DVector3& vExtents, uint32 nColor);

    // 添加一个球
    bool AddSphere(const F3DVector3& vCenter, float fRadius, uint32 nColor);

    // 添加一个AABB包围盒
    bool AddAABB(const F3DAABB& aabb, uint32 nColor);

    // 添加一个OBB包围盒
    bool AddOBB(const F3DOBB& obb, uint32 nColor);

    ///////////////////////////////////////////////////////////////////////////

    // 添加一系列2D几何图形
    bool Add2DGeometry(const F3DVector3* pVerts, int iNumVerts, const ushort* pIndices,
        int iNumIndices, uint32 nColor);

    // 添加一个矩形
    bool AddRectangle(const F2DRECT& rect, uint32 nColor);

    ///////////////////////////////////////////////////////////////////////////

protected:
	F3DEngine* m_pEngine;
	F3DRenderDevice* m_pDevice;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_FLATPAINTER_H__