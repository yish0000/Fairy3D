/*
 * -----------------------------------------------------------------------
 *  Name:   F3DRenderable.cpp
 *  Desc:   ���ļ�����ʵ������Ŀ���Ⱦ����
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

/** ���캯��
*/
F3DRenderable::F3DRenderable(void)
    : m_RenderType(RT_TRIANGLELIST), m_pMaterial(NULL), m_nPriorityQueue(0)
{
    m_Bounds.Set( F3DVector3::ZERO,0.0f );
}

/** ��������
*/
F3DRenderable::~F3DRenderable(void)
{
}

/** ������Ⱦ������ӵ���Ⱦ����
*/
void F3DRenderable::AddToRenderQueue(void)
{
    // ��ȡ�������Ⱦ��
	F3DRenderer* pRenderer = F3DEngine::GetInstance().GetRenderer();
	pRenderer->AddRenderable( this, m_nPriorityQueue );
}

/** ��ȡ��Ⱦ����������ľ���
@Param ��ǰ�����λ��
*/
float F3DRenderable::GetSqrCameraDist( const F3DVector3& vCamPos ) const
{
    float fSqrLen = (m_Bounds.m_vCenter - vCamPos).GetSqrLength();
    return fSqrLen - m_Bounds.m_fRadius*m_Bounds.m_fRadius;
}