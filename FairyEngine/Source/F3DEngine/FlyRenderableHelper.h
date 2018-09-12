//------------------------------------------------------------------------
// Name: FlyRenderableHelper.h
// Desc: This file define some helper renderable objects. Such as: 
//       Bounding box, ...
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_RENDERABLE_HELPER_H__
#define __FLY_RENDERABLE_HELPER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyRenderable.h"

#include "FlyGraphicBuffer.h"
//////////////////////////////////////////////////////////////////////////


class FlyBBoxHelper : public FlyRenderable
{
protected:
    FlyAabb m_BBox;     // Bounding box of the object.
    FLYCOLOR m_WireClr; // Color of the wireframe.

    FlyGraphicBuffer* m_pVB;    // Pointer to the vertex buffer.
    FlyGraphicBuffer* m_pIB;    // Pointer to the index buffer.

public:
    // Constructor and destructor.
    FlyBBoxHelper( const FlyAabb& box,const FLYCOLOR& clr );
    ~FlyBBoxHelper(void);

    // Render the bounding box.
    void Render(void);

    // Update the bounding box.
    void UpdateBBox( const FlyAabb& box );
    void SetWireColor( const FLYCOLOR& clr );

    const FlyAabb& GetBoundingBox(void) const;
    float GetSqrCameraDist( const FlyVector& vCamPos ) const;
    void GetWorldMatrices( FlyMatrix* pMatrices ) const;
    UINT GetNumWorldMatrices(void) const;

    UINT GetLightCount(void) const;
    FlyLightObject* GetLightObject( UINT nIndex );

    UINT GetNumVerts(void) const;
    UINT GetNumIndis(void) const;
    UINT GetNumPrims(void) const;

    FlyGraphicBuffer* GetVertexBuffer(void);
    FlyGraphicBuffer* GetIndexBuffer(void);
};

//////////////////////////////////////////////////////////////////////////

class FlySkyboxHelper : public FlyRenderable
{
protected:
    float m_fSize;              // Size of the skybox.
    FlyMatrix m_mWorld;         // Current world matrix.

    FlyTexture* m_pFront;       // Front texture of the skybox.
    FlyTexture* m_pBack;        // Back texture of the skybox.
    FlyTexture* m_pLeft;        // Left texture of the skybox.
    FlyTexture* m_pRight;       // Right texture of the skybox.
    FlyTexture* m_pTop;         // Top texture of the skybox.
    FlyTexture* m_pBottom;      // Bottom texture of the skybox.

    UINT m_nNumVerts;           // Count of the vertices.
    UINT m_nNumIndis;           // Count of the indices.
    UINT m_nNumPrims;           // Count of the primitives.

    FlyGraphicBuffer* m_pVB;    // Pointer to the vertex buffer.

public:
    // Constructor and destructor.
    FlySkyboxHelper(void);
    ~FlySkyboxHelper(void);

    HRESULT BuildSkybox( float fSize );
    void SetSkyTexture( CUBEFACE Face,const char* cTexture );
    void SetCameraPos( const FlyVector& vCamPos );

    // Render the skybox for engine.
    void Render(void);

    UINT GetNumVerts(void) const;
    UINT GetNumIndis(void) const;
    UINT GetNumPrims(void) const;

    // Get the boundingBox of the object.
    const FlyAabb& GetBoundingBox(void) const;

    float GetSqrCameraDist( const FlyVector& vCamPos ) const;
    void GetWorldMatrices( FlyMatrix* pMatrices ) const;
    UINT GetNumWorldMatrices(void) const;

    UINT GetLightCount(void) const;
    FlyLightObject* GetLightObject( UINT nIndex );
};

//////////////////////////////////////////////////////////////////////////

class FlyShadowHelper : public FlyRenderable
{
protected:
    UINT m_nNumVerts;           // Count of the vertices.
    UINT m_nNumIndis;           // Count of the indices.
    UINT m_nNumPrims;           // Count of the primitives.

    FlyGraphicBuffer* m_pVB;    // Pointer to the vertex buffer.

public:
    // Constructor and destructor.
    FlyShadowHelper(void);
    ~FlyShadowHelper(void);

    // Override the function, Add me to surfaceQueue.
    void AddToRenderQueue(void);

    // Render a big rectangle which cover with the whole screen.
    void Render(void);

    UINT GetNumVerts(void) const;
    UINT GetNumIndis(void) const;
    UINT GetNumPrims(void) const;

    // Get the local boundingBox of the object.
    const FlyAabb& GetBoundingBox(void) const;

    float GetSqrCameraDist( const FlyVector& vCamPos ) const;
    void GetWorldMatrices( FlyMatrix* pMatrices ) const;
    UINT GetNumWorldMatrices(void) const;

    UINT GetLightCount(void) const;
    FlyLightObject* GetLightObject( UINT nIndex );
};

//////////////////////////////////////////////////////////////////////////

// class _FLY_EXPORT FlyLensFlareHelper : public FlyRenderable
// {
//     struct sLensFlareSpot
//     {
//         DWORD dwColor;
//         FlyTexture* pTexture;
//         float fSize;
//         float fLinePos;
//     };
//     typedef std::vector<sLensFlareSpot> LensFlareList;
// 
// public:
//     // Constructor and destructor.
//     FlyLensFlareHelper(void);
//     ~FlyLensFlareHelper(void);
// };

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_RENDERABLE_HELPER_H__