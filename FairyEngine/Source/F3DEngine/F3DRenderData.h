/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRenderData.h
 *  Desc:   This file define the class which stores the data for rendering.
 *  Author: Yish
 *  Date:   2015/3/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_RENDERDATA_H__
#define __F3D_RENDERDATA_H__

//// HEADER FILES /////////////////////////////////////////////////////////
#include "F3DGraphicBuffer.h"

///////////////////////////////////////////////////////////////////////////

/** Primitive data to be rendered.
*/
class F3DRenderData : public FGeneralAlloc
{
public:

	typedef std::vector<F3DVertexBuffer*> VertexBufferStream;

public:
	F3DRenderData();

	//

protected:
	ERenderType m_RenderType;
	F3DVertexDecl m_VertDesc;
	VertexBufferStream m_pVBStreams;
	F3DIndexBuffer* m_pIB;
	uint32 m_nFirstIdx;
	uint32 m_nPrimCount;
	uint32 m_nMinVertIdx;
	uint32 m_nMaxVertIdx;
	F3DMaterial* m_pMaterial;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_RENDERDATA_H__