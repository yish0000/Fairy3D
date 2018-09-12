//------------------------------------------------------------------------
// Name: etBrush.h
// Desc: This file 
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_BRUSH_H__
#define __ET_BRUSH_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etCommon.h"

#include "FlySceneObject.h"
#include "FlyGraphicBuffer.h"
#include "FlyRenderable.h"
//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etBrush : public FlySceneObject
{
protected:
    etTerrain* m_pTerrain;  // Pointer to the terrain.

    TOOLTYPE m_BrushType;   // Type of the brush.
    TOOLSHAPE m_BrushShape; // Shape of the brush.

    float m_fX;             // X- coordinate of the position.
    float m_fZ;             // Z- coordinate of the position.
    float m_fInner;         // Inner radius of the brush.
    float m_fOuter;         // Outer radius of the brush.
    float m_fStrength;      // Strength of the brush.

    UINT m_nCurLayer;       // Current alpha layer.

    // Object to render the brush.
    etBrushRenderable* m_pRenderable;
    bool m_bBrushChanged;

public:
    // Constructor and destructor.
    etBrush(void);
    ~etBrush(void);

    // Set the active terrain for brush to modify.
    void AttachTerrain( etTerrain* pTerrain );

    void ModifyTerrain(void);
    void PaintTerrain(void);

    void UpdateObject(void);
    void RenderObject(void);

    // Get the local boundingBox.
    const FlyAabb& GetLocalBoundingBox(void) const;

    TOOLTYPE GetBrushType(void) const { return m_BrushType; }
    TOOLSHAPE GetBrushShape(void) const { return m_BrushShape; }
    float GetInnerRadius(void) const { return m_fInner; }
    float GetOuterRadius(void) const { return m_fOuter; }
    float GetStrength(void) const { return m_fStrength; }
    float GetPositionX(void) const { return m_fX; }
    float GetPositionZ(void) const { return m_fZ; }
    UINT GetCurrentLayer(void) const { return m_nCurLayer; }

    void SetBrushType( TOOLTYPE Type ) { m_BrushType = Type; }
    void SetStrength( float fStrength ) { m_fStrength = fStrength; }
    void SetBrushShape( TOOLSHAPE Shape );
    void SetInnerRadius( float fRadius );
    void SetOuterRadius( float fRadius );
    void SetPosition( float fX,float fZ );
    void SetCurrentLayer( UINT nLayer ) { m_nCurLayer = nLayer; }

    // Get the pointer to the terrain.
    etTerrain* GetTerrain(void) { return m_pTerrain; }

protected:
    void ModifyTerrainRS(void);
    void ModifyTerrainCS(void);
    void PaintTerrainRS( UINT nDetailLayer );
    void PaintTerrainCS( UINT nDetailLayer );

    void SmoothTerrain( int x,int z );
    void BlendTerrain( int x,int z,UINT nDetailLayer );
};

//////////////////////////////////////////////////////////////////////////

class _ET_EXPORT etBrushRenderable : public FlyRenderable
{
protected:
    etBrush* m_pParent;         // Pointer to the brush.
    FlyAabb m_BBox;             // BoundingBox of the brush.

    UINT m_nNumVerts;           // Count of the vertices.
    UINT m_nNumIndis;           // Count of the indices.
    UINT m_nNumPrims;           // Count of the primitives.

    FlyGraphicBuffer* m_pInnerVB;
    FlyGraphicBuffer* m_pOuterVB;

public:
    // Constructor and destructor.
    etBrushRenderable( etBrush* pBrush );
    ~etBrushRenderable(void);

    // Build the geometry data for brush.
    HRESULT BuildBrushData( TOOLSHAPE Shape,float fInner,float fOuter,
        float fX,float fZ );

    // Render the object.
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

#endif  //#ifndef __ET_BRUSH_H__