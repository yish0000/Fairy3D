//------------------------------------------------------------------------
// Name: FlyParticleRenderUnit.h
// Desc: This file define a class which is the basic render unit to 
//       render the particles.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_PARTICLE_RENDERUNIT_H__
#define __FLY_PARTICLE_RENDERUNIT_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyRenderable.h"

#include "FlyGraphicBuffer.h"
//////////////////////////////////////////////////////////////////////////


/*
* This class is the basic unit to render the particles.
* Inherit from the class FlyRenderable.
*/
class FlyParticleRenderUnit : public FlyRenderable
{
protected:
    FlyParticleController* m_pParent; // Pointer to the controller.

    UINT m_nNumParticles;       // Current number of the particles.
    UINT m_nMaxParticles;       // Max number of the particles.

    UINT m_nNumVerts;           // Number of the vertices.
    UINT m_nNumIndis;           // Number of the indices.
    UINT m_nNumPrims;           // Number of the primitives.
    FlyAabb m_BBox;             // Local boundingbox of the unit.

    FlyGraphicBuffer* m_pVB;    // Buffer to store vertex data.
    FlyGraphicBuffer* m_pIB;    // Buffer to store index data.

public:
    // Constructor and destructor.
    FlyParticleRenderUnit( FlyParticleController* pParent,const char* cMatName );
    ~FlyParticleRenderUnit(void);

    // Add the new particle to buffer.
    HRESULT AddParticleData( VERTEXL* pVert,WORD* pIndis );
    void ClearParticles(void);
    UINT GetNumParticles(void) const;

    HRESULT SetMaxParticles( UINT nMaxParticles );
    UINT GetMaxParticles(void) const;

    // Set the texture for render unit.
    void SetTexture( FlyTexture* pTexture );
    void SetBoundingBox( const FlyAabb& box );

    // Render the particles.
    void Render(void);

    UINT GetNumVerts(void) const;
    UINT GetNumIndis(void) const;
    UINT GetNumPrims(void) const;

    // Get the local boundingBox.
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

#endif  //#ifndef __FLY_PARTICLE_RENDERUNIT_H__