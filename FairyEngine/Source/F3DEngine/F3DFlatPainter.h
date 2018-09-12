/*
 * ------------------------------------------------------------------------
 *  Name:   F3DFlatPainter.h
 *  Desc:   ���ļ�������һ����Ⱦ������������ȾһЩ�������Ƭ��
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


/** ��Ƭ��Ⱦ����
*/
class FAIRY_API F3DFlatPainter : public FGeneralAlloc
{
public:
	F3DFlatPainter();
	virtual ~F3DFlatPainter() {}

    // ���һϵ��3D����ͼ��
    bool Add3DGeometry(const F3DVector3* pVerts, int iNumVerts, const ushort* pIndices,
        int iNumIndices, uint32 nColor);

    // ���һ������
    bool AddBox(const F3DVector3& vPos, const F3DVector3& vDir, const F3DVector3& vRight,
        const F3DVector3& vCenter, const F3DVector3& vExtents, uint32 nColor);

    // ���һ����
    bool AddSphere(const F3DVector3& vCenter, float fRadius, uint32 nColor);

    // ���һ��AABB��Χ��
    bool AddAABB(const F3DAABB& aabb, uint32 nColor);

    // ���һ��OBB��Χ��
    bool AddOBB(const F3DOBB& obb, uint32 nColor);

    ///////////////////////////////////////////////////////////////////////////

    // ���һϵ��2D����ͼ��
    bool Add2DGeometry(const F3DVector3* pVerts, int iNumVerts, const ushort* pIndices,
        int iNumIndices, uint32 nColor);

    // ���һ������
    bool AddRectangle(const F2DRECT& rect, uint32 nColor);

    ///////////////////////////////////////////////////////////////////////////

protected:
	F3DEngine* m_pEngine;
	F3DRenderDevice* m_pDevice;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_FLATPAINTER_H__