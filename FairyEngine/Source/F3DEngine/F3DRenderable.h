/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRenderable.h
 *  Desc:   This file define the base class of renderable objects.
 *  Author: Yish
 *  Date:   2011/1/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_RENDERABLE_H__
#define __F3D_RENDERABLE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"
#include "F3DComponent.h"
#include "F3DMaterial.h"

///////////////////////////////////////////////////////////////////////////

/** Base class of Renderable objects.
@remarks
    ����������м�����Ⱦ����С��Ԫ
*/
class FAIRY_API F3DRenderable : public F3DComponent
{
	F_DECLARE_RTTI(F3DRenderable)

protected:
    uint32 m_nPriorityQueue;        // ����Ⱦ��������ȼ�
    F3DSphere m_Bounds;             // ��Ⱦ����İ�Χ��
    ERenderType m_RenderType;       // ����Ⱦ�������Ⱦ����
    F3DMaterial* m_pMaterial;       // ����Ⱦ����Ĳ���

public:
    F3DRenderable();
    virtual ~F3DRenderable();

    // Add me to the render queue.
    virtual void AddToRenderQueue(void);

    // ��Ⱦ�ö���
    virtual void Render(void) = 0;

    // ��ȡ��Ⱦ����Ķ��㣬������ͼԪ��Ŀ
    virtual uint32 GetNumVerts(void) const = 0;
    virtual uint32 GetNumIndis(void) const = 0;
    virtual uint32 GetNumPrims(void) const = 0;

    // ��ȡ��Ⱦ������������
    virtual uint32 GetWorldMatrices( F3DMatrix4* pMatrices ) const = 0;

    // ��ȡ��Ⱦ����İ�Χ��
    const F3DSphere& GetBoundingSphere(void) const { return m_Bounds; }

    // ��ȡ��Ⱦ����������ľ���
    float GetSqrCameraDist( const F3DVector3& vCamPos ) const;

    // ������Ⱦ�������Ⱦ����
    void SetRenderType( ERenderType Type ) { m_RenderType = Type; }

    // ������Ⱦ����Ĳ���
    void SetMaterial( F3DMaterial* pMaterial ) { m_pMaterial = pMaterial; }

    // ������Ⱦ��������ȼ�
    void SetRenderPriority( uint32 nPriority ) { m_nPriorityQueue = nPriority; }

    // ��ȡ��Ⱦ�������Ⱦ����
    ERenderType GetRenderType(void) const { return m_RenderType; }

    // ��ȡ��Ⱦ����Ĳ���
    F3DMaterial* GetMaterial(void) const { return m_pMaterial; }

    // ��ȡ��Ⱦ��������ȼ�
    uint32 GetRenderPriority(void) const { return m_nPriorityQueue; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_RENDERABLE_H__