/*
 * -----------------------------------------------------------------------
 *  Name:   F3DRenderable.cpp
 *  Desc:   本文件用于实现引擎的可渲染对象。
 *  Author: Yish
 *  Date:   2011/1/12
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DRenderable.h"
#include "F3DRenderer.h"
#include "F3DEngine.h"

F_IMPLEMENT_RTTI(F3DRenderable, F3DComponent)

/** 构造函数
*/
F3DRenderable::F3DRenderable(void)
    : m_RenderType(RT_TRIANGLELIST), m_pMaterial(NULL), m_nPriorityQueue(0)
{
    m_Bounds.Set( F3DVector3::ZERO,0.0f );
}

/** 析构函数
*/
F3DRenderable::~F3DRenderable(void)
{
}

/** 将该渲染对象添加到渲染队列
*/
void F3DRenderable::AddToRenderQueue(void)
{
    // 获取引擎的渲染器
	F3DRenderer* pRenderer = F3DEngine::GetInstance().GetRenderer();
	pRenderer->AddRenderable( this, m_nPriorityQueue );
}

/** 获取渲染对象与相机的距离
@Param 当前相机的位置
*/
float F3DRenderable::GetSqrCameraDist( const F3DVector3& vCamPos ) const
{
    float fSqrLen = (m_Bounds.m_vCenter - vCamPos).GetSqrLength();
    return fSqrLen - m_Bounds.m_fRadius*m_Bounds.m_fRadius;
}