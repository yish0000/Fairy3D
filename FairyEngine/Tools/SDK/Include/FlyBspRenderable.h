//------------------------------------------------------------------------
// Name: FlyBspRenderable.h
// Desc: This file define a renderable object to render the bsp level.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_BSP_RENDERABLE_H__
#define __FLY_BSP_RENDERABLE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyRenderable.h"
#include "FlyGraphicBuffer.h"

//////////////////////////////////////////////////////////////////////////


/*
* A renderable object to render the bsp level.
*/
class FlyBspRenderable : public FlyRenderable
{
protected:
    UINT m_nMaxNumVerts;        // Max count of the vertices.
    UINT m_nMaxNumIndis;        // Max count of the indices.

    UINT m_nNumVerts;           // Count of the vertices.
    UINT m_nNumIndis;           // Count of the indices.
    UINT m_nNumPrims;           // Count of the primitives.

    FlyAabb m_BBox;             // Local boundingBox of the object.

    FlyGraphicBuffer* m_pVB;    // Pointer to the vertex buffer.
    FlyGraphicBuffer* m_pIB;    // Pointer to the index buffer.

public:
    // Constructor and destructor.
    FlyBspRenderable( const char* cMatName );
    ~FlyBspRenderable(void);

    // Create the graphic buffer for me.
    HRESULT CreateGraphicBuffer(void);

    // Add a bsp face's data to this object.
    HRESULT AddFaceData( UINT nNumVerts,UINT nNumIndis,const void* pVerts,
                         const WORD* pIndis,const FlyAabb& box );

    // Clear the geometry data.
    void ClearData(void);

    // Render the object.
    void Render(void);

    UINT GetNumVerts(void) const;
    UINT GetNumIndis(void) const;
    UINT GetNumPrims(void) const;

    UINT GetMaxNumVerts(void) const;
    UINT GetMaxNumIndis(void) const;
    void SetMaxNumVerts( UINT nMaxNumVerts );
    void SetMaxNumIndis( UINT nMaxNumIndis );

    float GetSqrCameraDist( const FlyVector& vCamPos ) const;
    const FlyAabb& GetBoundingBox(void) const;
    void GetWorldMatrices( FlyMatrix* pMatrices ) const;
    UINT GetNumWorldMatrices(void) const;

    UINT GetLightCount(void) const;
    FlyLightObject* GetLightObject( UINT nIndex );

    FlyTexture* GetTexture(void);
    FlyTexture* GetLightmap(void);
    void SetTexture( FlyTexture* pTexture );
    void SetLightmap( FlyTexture* pTexture );

    FlyGraphicBuffer* GetVertexBuffer(void);
    FlyGraphicBuffer* GetIndexBuffer(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_BSP_RENDERABLE_H__