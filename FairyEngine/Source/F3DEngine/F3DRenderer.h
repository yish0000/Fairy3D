/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRenderer.h
 *  Desc:   本文件为引擎定义了一个渲染器基类。
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
    本类用于完成整个引擎的渲染工作，对渲染对象进行必要的排序。
@note
    本对象由渲染设备自动创建，用户请勿创建。
*/
class FAIRY_API F3DRenderer : public FGeneralAlloc
{
    typedef std::vector<F3DRenderable*> RenderQueue;

protected:
    F3DRenderDevice* m_pDevice;             // 渲染设备的指针
    RenderQueue m_RenderQueues[MAX_NUM_RENDERQUEUE];    // 渲染队列列表

    uint32 m_nNumRenderedVerts;             // 本帧所渲染的顶点数
    uint32 m_nNumRenderedIndis;             // 本帧所渲染的索引数
    uint32 m_nNumRenderedPrims;             // 本帧所渲染的图元数
    uint32 m_nNumRenderedObjects;           // 本帧所渲染的对象数

    ERenderType m_RenderType;               // 当前的渲染类型
//     F3DRenderMaterial* m_pMaterial;         // 当前的材质
//     F3DRenderMaterial* m_pDefaultMaterial;  // 当渲染时材质为空时所使用的缺省材质

public:
    F3DRenderer();
    virtual ~F3DRenderer() {}

    // 添加一个渲染对象到队列
    virtual void AddRenderable( F3DRenderable* pObject, uint32 nQueueID=RENDERQUEUE_DEFAULT ) {}

    // 注册一个新的顶点类型
    virtual void RegisterVertexDecl( const F3DVertexDecl& decl ) = 0;

	// Render the specified scene.
	virtual void Render(F3DViewport* pViewport, F3DSceneManager* pScene) {}
	void Render(F3DRenderTarget* pTarget, F3DSceneManager* pScene) {}
	void Render(F3DSceneManager* pScene) {}

    // 渲染指定的数据
    virtual void RenderBuffer( EVertexType nVType,uint32 nVOffset,uint32 nIOffset,
        uint32 nNumVerts,uint32 nNumIndis,F3DGraphicBuffer* pVB,F3DGraphicBuffer* pIB ) = 0;

    // 渲染指定的数据
    virtual void RenderBuffer( EVertexType VType, uint32 nNumVerts, uint32 nNumIndis, F3DGraphicBuffer* pVB, F3DGraphicBuffer* pIB ) {}
	virtual void RenderBuffer( EVertexType VType, uint32 nVOffset, uint32 nNumVerts, F3DGraphicBuffer* pVB ) {}
	virtual void RenderBuffer( EVertexType VType, uint32 nNumVerts, F3DGraphicBuffer* pVB ) {}

    // 释放所有渲染队列
    virtual void FlushRenderQueue(void) {}

    // 释放指定的渲染队列
    virtual void FlushRenderQueue( uint32 nQueueID ) {}

    // 获取渲染器当前的渲染类型
    ERenderType GetRenderType(void) const { return m_RenderType; }

    // 获取渲染器当前的材质
    //const F3DRenderMaterial* GetRenderMaterial(void) const { return m_pMaterial; }

    // 设置渲染器的渲染类型
    void SetRenderType( ERenderType Type ) { m_RenderType = Type; }

    // 设置渲染器的材质
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