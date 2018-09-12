//------------------------------------------------------------------------
// Name: FlyRenderer.h
// Desc: This file define an interface to render the geometries.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_RENDERER_H__
#define __FLY_RENDERER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyVertexType.h"

#include "FlyGraphicBuffer.h"
#include "FlyRenderable.h"
#include "FlyLightObject.h"

#include "FlyRenderAutoParameter.h"
//////////////////////////////////////////////////////////////////////////


// Renderer of the engine.
class FlyRenderer
{
public:
    // Constructor and destructor.
    FlyRenderer(void) { /* Nothing to do! */ };
    virtual ~FlyRenderer(void) { /* Nothing to do! */ };

    // Initialize the renderer.
    virtual HRESULT InitRenderer(void) = 0;
    virtual void Release(void) = 0;

    // Add a renderable object to the renderer.
    virtual HRESULT AddRenderable( FlyRenderable* pObject ) = 0;

    // Add a screen 2D object to the queue. (Such as font, UI elements...)
    virtual HRESULT AddLayoutObject( FlyRenderable* pObject ) = 0;

    // Register the user-defined vertex format.
    virtual HRESULT RegisterVertexType( const FlyVertexType& Type ) = 0;

    // Add a static buffer to renderer.
    virtual HRESULT AddStaticBuffer( VERTEXTYPE nVType,INDEXFORMAT indexFmt,UINT nNumVerts,
                                     UINT nNumIndis,const void* pVerts,const void* pIndis,
                                     FlyRenderMaterial* pMaterial,RENDERTYPE RType,
                                     UINT* pnID ) = 0;

    // Clear all the static buffers.
    virtual void ClearStaticBuffer(void) = 0;

    // Render the static buffer.
    virtual HRESULT RenderStaticBuffer( UINT nBufferIndex ) = 0;

    // Render the geometry directly.
    virtual HRESULT RenderBuffer( VERTEXTYPE nVType,UINT nNumVerts,UINT nNumIndis,
        FlyGraphicBuffer* pVB,FlyGraphicBuffer* pIB ) = 0;
    virtual HRESULT RenderBuffer( VERTEXTYPE nVType,UINT nOffset,UINT nNumVerts,
        FlyGraphicBuffer* pVB ) = 0;
    virtual HRESULT RenderBuffer( VERTEXTYPE nVType,UINT nVOffset,UINT nIOffset,
        UINT nNumVerts,UINT nNumIndis,FlyGraphicBuffer* pVB,FlyGraphicBuffer* pIB ) = 0;

    // Flush all the Renderable objects.
    virtual HRESULT FlushRenderQueue(void) = 0;

    // Flush all the surface objects.
    virtual HRESULT FlushLayoutQueue(void) = 0;

    // Set and get the render states of the renderer.
    virtual RENDERTYPE GetRenderType(void) const = 0;
    virtual FlyRenderMaterial* GetRenderMaterial(void) const = 0;
    virtual void SetRenderType( RENDERTYPE Type ) = 0;
    virtual void SetRenderMaterial( FlyRenderMaterial* pMaterial ) = 0;

    // Get the size of specified vertex type.
    virtual UINT GetVertexSize( VERTEXTYPE vertexType ) const = 0;

    virtual void ResetRenderedCount(void) = 0;
    virtual UINT GetNumRenderedVerts(void) const = 0;
    virtual UINT GetNumRenderedIndis(void) const = 0;
    virtual UINT GetNumRenderedPrims(void) const = 0;
    virtual UINT GetNumRenderedObjects(void) const = 0;

    virtual FlyRenderAutoParameter* GetAutoParameters(void) = 0;
    virtual const FlyRenderMaterial* GetDefaultMaterial(void) const = 0;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_RENDERER_H__