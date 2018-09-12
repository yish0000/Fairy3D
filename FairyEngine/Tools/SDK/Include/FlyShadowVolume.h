//------------------------------------------------------------------------
// Name: FlyShadowVolume.h
// Desc: This file define a shadow volume for the sceneObject.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_SHADOWVOLUME_H__
#define __FLY_SHADOWVOLUME_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

#include "FlyGraphicBuffer.h"
#include "FlyRenderable.h"
//////////////////////////////////////////////////////////////////////////


class FlyShadowVolumeRenderable;

class _FLY_EXPORT FlyShadowVolume
{
public:
    struct _Vertex { float x,y,z; };

protected:
    UINT m_nNumVerts;       // Count of the vertices.
    BYTE* m_pVerts;         // Pointer to the vertex data.
    int m_nStrideSize;      // Size of the vertex struct.
    UINT m_nNumIndis;       // Count of the indices.
    WORD* m_pIndis;         // Pointer to the index data.

    UINT m_nNumTriangles;   // Count of the triangles.

    WORD* m_pEdges;
    UINT m_nNumEdges;

    // Used for rendering the shadow.
    _Vertex* m_pVolume;
    UINT m_nNumVolVerts;

    FlyVector m_vLight;     // Position of the light.
    FlyMatrix m_mWorld;     // World matrix of the shadowVolume.

    FlyShadowVolumeRenderable* m_pRObject;

    void AddEdge( WORD v0,WORD v1 );
    _Vertex Extrude( _Vertex* pVert,const FlyVector& vc );

public:
    // Constructor and destructor.
    FlyShadowVolume(void);
    ~FlyShadowVolume(void);

    HRESULT BuildShadowVolume( UINT nNumVerts,UINT nNumIndis,const void* pVerts,
                               const WORD* pIndis,int nStrideSize );
    void UpdateShadowVolume( const FlyVector& vLight );

    const FlyMatrix& GetWorldMatrix(void) const { return m_mWorld; }
    void SetWorldMatrix( const FlyMatrix& mWorld ) { m_mWorld = mWorld; }

    FlyShadowVolumeRenderable* GetShadowVolumeRenderable(void) { return m_pRObject; }
};

//////////////////////////////////////////////////////////////////////////

class FlyShadowVolumeRenderable : public FlyRenderable
{
protected:
    FlyShadowVolume* m_pParent; // Pointer to the shadowVolume.

    UINT m_nNumVerts;           // Count of the vertices.
    UINT m_nNumIndis;           // Count of the indices.
    UINT m_nNumPrims;           // Count of the primitives.

    FlyAabb m_BBox;             // BoundingBox of the object.
    FlyGraphicBuffer* m_pVB;    // Pointer to the vertex buffer.

public:
    // Constructor and destructor.
    FlyShadowVolumeRenderable( FlyShadowVolume* pParent );
    ~FlyShadowVolumeRenderable(void);

    // Build the vertex buffer for shadowVolume.
    HRESULT BuildVertexBuffer( UINT nNumVerts,UINT nStrideSize );

    // Send the vertices data to buffer.
    void SendVertexData( const BYTE* pVerts,UINT nNumVerts );

    // Render the shadow volume to stencil buffer.
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

    FlyGraphicBuffer* GetVertexBuffer(void);
    FlyGraphicBuffer* GetIndexBuffer(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_SHADOWVOLUME_H__