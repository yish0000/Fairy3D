//------------------------------------------------------------------------
// Name: FlyD3DRenderer.h
// Desc: This file is to implement the interface FlyRenderer with
//       the Direct3D platform.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_D3DRENDERER_H__
#define __FLY_D3DRENDERER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyRenderer.h"
#include "FlyUtil.h"
#include "FlyGraphicBuffer.h"

#include "FlyD3DCommon.h"
#include "FlyD3DRenderDevice.h"
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

class FlyD3DRenderer : public FlyRenderer
{
public:
    struct D3DVertexFormat
    {
        DWORD dwFVF;        // Direct3D FVF.
        UINT nStrideSize;   // Size of the vertex struct.

        // Vertex format when use shader.
        LPDIRECT3DVERTEXDECLARATION9 pD3DDecl;
    };

    // Static buffer.
    struct D3DStaticBuffer
    {
        DWORD dwFVF;
        UINT nStrideSize;
        LPDIRECT3DVERTEXDECLARATION9 pD3DDecl;

        RENDERTYPE RenderType;
        FlyRenderMaterial* pMaterial;
        UINT nNumVerts;
        UINT nNumIndis;
        FlyGraphicBuffer* pVB;
        FlyGraphicBuffer* pIB;
    };

    struct DefaultObjCompare
    {
        bool operator() ( FlyRenderable* rhs1,FlyRenderable* rhs2 ) const;
    };

    struct AlphaObjCompare
    {
        bool operator() ( FlyRenderable* rhs1,FlyRenderable* rhs2 ) const;
    };

    typedef std::vector<D3DVertexFormat> VFormatList;
    typedef std::vector<D3DStaticBuffer> StaticBufferList;
    typedef std::vector<FlyRenderable*> RenderQueue;

protected:
    LPDIRECT3DDEVICE9 m_pd3dDevice; // Pointer to the Direct3D device.
    FlyD3DRenderDevice* m_pRDevice; // Pointer to the Render device.

    VFormatList m_VFormats;         // Vertex format list.

    RenderQueue m_DefaultQueue;     // A queue to render the renderable objects.
    RenderQueue m_AlphaQueue;       // A queue to render the transparent objects.
    RenderQueue m_LayoutQueue;      // A queue to render the 2D object on the screen.

    UINT m_nNumRenderedVerts;       // Count of vertices rendered in this frame.
    UINT m_nNumRenderedIndis;       // Count of indices rendered in this frame.
    UINT m_nNumRenderedPrims;       // Count of primitives rendered in this frame.
    UINT m_nNumRenderedObjects;     // Count of objects rendered in this frame.

    StaticBufferList m_StaticBuffers; // Static buffer list.

    FlyRenderMaterial* m_pMaterial; // Current render material for the renderable.
    RENDERTYPE m_RenderType;        // Current render type for the renderable.

    FlyRenderMaterial* m_pDefaultMaterial; // Default material for the renderer.
    FlyRenderAutoParameter m_AutoParameters; // Auto-update parameters.

    // Build the vertex declaration.
    HRESULT BuildVertexDeclaration( const FlyVertexType& vertexType,IDirect3DVertexDeclaration9** ppDecl );
    DWORD BuildVertexFVF( const FlyVertexType& vertexType );

public:
    // Constructor and destructor.
    FlyD3DRenderer( FlyD3DRenderDevice* pRDevice );
    ~FlyD3DRenderer(void);

    // Init the renderer, Execute when create the
    // renderer by the device, User don't run this function.
    HRESULT InitRenderer(void);
    void Release(void);

    // Add the user-defined vertex format.
    HRESULT RegisterVertexType( const FlyVertexType& Type );

    // Add a renderable object to the renderer.
    HRESULT AddRenderable( FlyRenderable* pObject );

    // Add a screen 2D object to queue. (Such as font, UI elements).
    HRESULT AddLayoutObject( FlyRenderable* pObject );

    // Add a static buffer to renderer.
    HRESULT AddStaticBuffer( VERTEXTYPE nVType,INDEXFORMAT indexFmt,
        UINT nNumVerts,UINT nNumIndis,const void* pVerts,const void* pIndis,
        FlyRenderMaterial* pMaterial,RENDERTYPE RType,UINT* pnID );

    // Reset the static buffer.
    void ClearStaticBuffer(void);

    // Render the static buffer.
    HRESULT RenderStaticBuffer( UINT nBufferIndex );

    // Render the geometry directly.
    HRESULT RenderBuffer( VERTEXTYPE nVType,UINT nNumVerts,UINT nNumIndis,
        FlyGraphicBuffer* pVB,FlyGraphicBuffer* pIB );
    HRESULT RenderBuffer( VERTEXTYPE nVType,UINT nOffset,UINT nNumVerts,
        FlyGraphicBuffer* pVB );
    HRESULT RenderBuffer( VERTEXTYPE nVType,UINT nVOffset,UINT nIOffset,
        UINT nNumVerts,UINT nNumIndis,FlyGraphicBuffer* pVB,
        FlyGraphicBuffer* pIB );

    // Flush the render queue.
    HRESULT FlushRenderQueue(void);

    // Flush the screen surface objects.
    HRESULT FlushLayoutQueue(void);

    // Set and get the render states for the renderer.
    RENDERTYPE GetRenderType(void) const;
    FlyRenderMaterial* GetRenderMaterial(void) const;
    void SetRenderType( RENDERTYPE Type );
    void SetRenderMaterial( FlyRenderMaterial* pMaterial );

    // Get the size of specified vertex type.
    UINT GetVertexSize( VERTEXTYPE vertexType ) const;

    // Reset the count of rendered objects.
    void ResetRenderedCount(void);

    UINT GetNumRenderedVerts(void) const;
    UINT GetNumRenderedIndis(void) const;
    UINT GetNumRenderedPrims(void) const;
    UINT GetNumRenderedObjects(void) const;

    FlyRenderAutoParameter* GetAutoParameters(void) { return &m_AutoParameters; }
    const FlyRenderMaterial* GetDefaultMaterial(void) const { return m_pDefaultMaterial; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_D3DRENDERER_H__