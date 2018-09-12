/*
 * ------------------------------------------------------------------------
 *  Name:   F3DWirePainter.h
 *  Desc:   本文件定义了一个渲染容器，用于批量渲染一些线类几何体。
 *  Author: Yish
 *  Date:   2012/12/22
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_WIREPAINTER_H__
#define __F3D_WIREPAINTER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


/** 线框渲染容器
*/
class FAIRY_API F3DWirePainter : public FGeneralAlloc
{

public:
	F3DWirePainter();
	virtual ~F3DWirePainter() {}

	bool AddLine( const F3DVector3& v1, const F3DVector3& v2 );
	bool AddRectangle( const F3DVector3& vCenter, float fWidth, float fHeight, float fRotate[3] );
	bool AddTriangle( const F3DVector3& v1, const F3DVector3& v2, const F3DVector3& v3 );
	bool AddCircle( const F3DVector3& vCenter, float fRadius, float fRotate[3] );
	bool AddAABB( const F3DAABB& aabb );
	bool AddFrustum( const F3DFrustum& frustum );

	void FlushGeometry();

protected:

};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_WIREPAINTER_H__