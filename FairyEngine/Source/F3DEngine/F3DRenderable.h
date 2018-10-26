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
    这是引擎进行几何渲染的最小单元
*/
class FAIRY_API F3DRenderable : public F3DComponent
{
	F_DECLARE_RTTI(F3DRenderable)

protected:
    uint32 m_nPriorityQueue;        // 该渲染对象的优先级
    F3DSphere m_Bounds;             // 渲染对象的包围球
    ERenderType m_RenderType;       // 可渲染对象的渲染类型
    F3DMaterial* m_pMaterial;       // 该渲染对象的材质

public:
    F3DRenderable();
    virtual ~F3DRenderable();

    // Add me to the render queue.
    virtual void AddToRenderQueue(void);

    // 渲染该对象
    virtual void Render(void) = 0;

    // 获取渲染对象的顶点，索引，图元数目
    virtual uint32 GetNumVerts(void) const = 0;
    virtual uint32 GetNumIndis(void) const = 0;
    virtual uint32 GetNumPrims(void) const = 0;

    // 获取渲染对象的世界矩阵
    virtual uint32 GetWorldMatrices( F3DMatrix4* pMatrices ) const = 0;

    // 获取渲染对象的包围球
    const F3DSphere& GetBoundingSphere(void) const { return m_Bounds; }

    // 获取渲染对象与相机的距离
    float GetSqrCameraDist( const F3DVector3& vCamPos ) const;

    // 设置渲染对象的渲染类型
    void SetRenderType( ERenderType Type ) { m_RenderType = Type; }

    // 设置渲染对象的材质
    void SetMaterial( F3DMaterial* pMaterial ) { m_pMaterial = pMaterial; }

    // 设置渲染对象的优先级
    void SetRenderPriority( uint32 nPriority ) { m_nPriorityQueue = nPriority; }

    // 获取渲染对象的渲染类型
    ERenderType GetRenderType(void) const { return m_RenderType; }

    // 获取渲染对象的材质
    F3DMaterial* GetMaterial(void) const { return m_pMaterial; }

    // 获取渲染对象的优先级
    uint32 GetRenderPriority(void) const { return m_nPriorityQueue; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_RENDERABLE_H__