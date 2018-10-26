/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRenderer.h
 *  Desc:   ���ļ�Ϊ���涨����һ����Ⱦ�����ࡣ
 *  Author: Yish
 *  Date:   2010/12/19
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_RENDERER_H__
#define __F3D_RENDERER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DVertexDecl.h"

#include "F3DGraphicBuffer.h"
#include "F3DRenderable.h"

//#include "F3DRenderMaterial.h"
//#include "F3DLightObject.h"

//#include "F3DRenderAutoParameter.h"

///////////////////////////////////////////////////////////////////////////

// Priority of the render queue.
enum
{
	RENDERQUEUE_ERROR = -1,
	RENDERQUEUE_BACKGROUND,				// Background...
	RENDERQUEUE_ENVIRONMENT,			// Sky, sun, moon, star ...
	RENDERQUEUE_SHADOW,					// Shadow
	RENDERQUEUE_DEFAULT,				// Opacity elements.
	RENDERQUEUE_ALPHATEST,				// Translucent elements with alpha test.
	RENDERQUEUE_TRANSLUCENT,			// Translucent elements.
	RENDERQUEUE_WATER,					// Water surface.
	RENDERQUEUE_2D_TRANSLUCENT,			// GFX...
	RENDERQUEUE_3D_OVERLAY,				// 3D overlay elements.
	RENDERQUEUE_POSTPROCESS,			// Post process for 3d scene.
	RENDERQUEUE_2D_IMAGE_OPAQUE,		// For 2d sprite...
	RENDERQUEUE_2D_IMAGE_TRANSLUCENT,	// For 2d translucent sprite...
	RENDERQUEUE_UI,						// UI elements.
	RENDERQUEUE_2D_OVERLAY,				// 2D overlay elements.

	MAX_NUM_RENDERQUEUE,				// Count of the render queues.
};

class F3DSceneManager;

/** Renderer
@remarks
    ����������������������Ⱦ����������Ⱦ������б�Ҫ������
@note
    ����������Ⱦ�豸�Զ��������û����𴴽���
*/
class FAIRY_API F3DRenderer : public FGeneralAlloc
{
    typedef std::vector<F3DRenderable*> RenderQueue;

protected:
    F3DRenderDevice* m_pDevice;             // ��Ⱦ�豸��ָ��
    RenderQueue m_RenderQueues[MAX_NUM_RENDERQUEUE];    // ��Ⱦ�����б�

    uint32 m_nNumRenderedVerts;             // ��֡����Ⱦ�Ķ�����
    uint32 m_nNumRenderedIndis;             // ��֡����Ⱦ��������
    uint32 m_nNumRenderedPrims;             // ��֡����Ⱦ��ͼԪ��
    uint32 m_nNumRenderedObjects;           // ��֡����Ⱦ�Ķ�����

    ERenderType m_RenderType;               // ��ǰ����Ⱦ����
//     F3DRenderMaterial* m_pMaterial;         // ��ǰ�Ĳ���
//     F3DRenderMaterial* m_pDefaultMaterial;  // ����Ⱦʱ����Ϊ��ʱ��ʹ�õ�ȱʡ����

public:
    F3DRenderer();
    virtual ~F3DRenderer() {}

    // ���һ����Ⱦ���󵽶���
    virtual void AddRenderable( F3DRenderable* pObject, uint32 nQueueID=RENDERQUEUE_DEFAULT ) {}

    // ע��һ���µĶ�������
    virtual void RegisterVertexDecl( const F3DVertexDecl& decl ) = 0;

	// Render the specified scene.
	virtual void Render(F3DViewport* pViewport, F3DSceneManager* pScene) {}
	void Render(F3DRenderTarget* pTarget, F3DSceneManager* pScene) {}
	void Render(F3DSceneManager* pScene) {}

    // ��Ⱦָ��������
    virtual void RenderBuffer( EVertexType nVType,uint32 nVOffset,uint32 nIOffset,
        uint32 nNumVerts,uint32 nNumIndis,F3DGraphicBuffer* pVB,F3DGraphicBuffer* pIB ) = 0;

    // ��Ⱦָ��������
    virtual void RenderBuffer( EVertexType VType, uint32 nNumVerts, uint32 nNumIndis, F3DGraphicBuffer* pVB, F3DGraphicBuffer* pIB ) {}
	virtual void RenderBuffer( EVertexType VType, uint32 nVOffset, uint32 nNumVerts, F3DGraphicBuffer* pVB ) {}
	virtual void RenderBuffer( EVertexType VType, uint32 nNumVerts, F3DGraphicBuffer* pVB ) {}

    // �ͷ�������Ⱦ����
    virtual void FlushRenderQueue(void) {}

    // �ͷ�ָ������Ⱦ����
    virtual void FlushRenderQueue( uint32 nQueueID ) {}

    // ��ȡ��Ⱦ����ǰ����Ⱦ����
    ERenderType GetRenderType(void) const { return m_RenderType; }

    // ��ȡ��Ⱦ����ǰ�Ĳ���
    //const F3DRenderMaterial* GetRenderMaterial(void) const { return m_pMaterial; }

    // ������Ⱦ������Ⱦ����
    void SetRenderType( ERenderType Type ) { m_RenderType = Type; }

    // ������Ⱦ���Ĳ���
    //void SetRenderMaterial( F3DRenderMaterial* pMaterial ) { m_pMaterial = pMaterial; }

    uint32 GetNumRenderedVerts(void) const { return m_nNumRenderedVerts; }
    uint32 GetNumRenderedIndis(void) const { return m_nNumRenderedIndis; }
    uint32 GetNumRenderedPrims(void) const { return m_nNumRenderedPrims; }
    uint32 GetNumRenderedObjects(void) const { return m_nNumRenderedObjects; }

    // Get the size of specified vertex type.
    //virtual UINT GetVertexSize( FVertexType Type ) const = 0;

    //virtual F3DRenderAutoParameter* GetAutoParameters(void) = 0;

protected:
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_RENDERER_H__