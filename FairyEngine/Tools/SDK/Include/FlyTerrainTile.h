//------------------------------------------------------------------------
// Name: FlyTerrainTile.h
// Desc: This file define a basic renderUnit for the terrain.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_TERRAIN_TILE_H__
#define __FLY_TERRAIN_TILE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlySceneObject.h"
#include "FlyRenderable.h"

#include "FlyGraphicBuffer.h"
#include "FlyRenderableHelper.h"
//////////////////////////////////////////////////////////////////////////


class FlyTerrainLevel;
class FlyTerrainSceneManager;
class FlyTerrainRenderUnit;

class _FLY_EXPORT FlyTerrainTile : public FlySceneObject
{
    friend class FlyTerrainRenderUnit;
    friend class FlyTerrainSceneManager;

public:
    enum Neighbor { NB_NORTH,NB_SOUTH,NB_EAST,NB_WEST };

    enum StitchMode
    {
        STITCH_NORTH_SHIFT = 0,
        STITCH_SOUTH_SHIFT = 8,
        STITCH_WEST_SHIFT = 16,
        STITCH_EAST_SHIFT = 24,

        STITCH_NORTH = 128 << STITCH_NORTH_SHIFT,
        STITCH_SOUTH = 128 << STITCH_SOUTH_SHIFT,
        STITCH_WEST = 128 << STITCH_WEST_SHIFT,
        STITCH_EAST = 128 << STITCH_EAST_SHIFT,
    };

protected:
    FlyTerrainSceneManager* m_pManager; // Pointer to the sceneManager.
    FlyTerrainLevel* m_pLevel;          // Pointer to the terrain level.

    FlyTerrainTile* m_pNeighbors[4];    // Four neighbors of the tile.
    FlyAabb m_Bounds;                   // BoundingBox of the tile.
    int m_nLodLevel;                    // Current lod level.

    FlyGraphicBuffer* m_pVB;            // Pointer to the vertex buffer.
    FlyTexture* m_pMainTexture;         // Pointer to the main texture.
    float m_fMainScale;                 // Main texture's scale of the tile.
    FlyTexture* m_pDetail[4];           // Detail texture array.
    float m_fDetailScale[4];            // Detail texture's scale of the tile.

    FlyTerrainRenderUnit* m_pUnit;      // RenderUnit of the tile.

    void GetIndexBuffer( FlyGraphicBuffer** ppIB,UINT* pNumIndices );
    void GenerateListIndexData( UINT nStitchFlags,FlyGraphicBuffer** ppIB,UINT* pNumIndices );
    void GenerateStripIndexData( UINT nStitchFlags,FlyGraphicBuffer** ppIB,UINT* pNumIndices );

    UINT StitchEdge( Neighbor nb,int hiLod,int loLod,bool omitFirst,
                     bool omitLast,WORD** ppIndex );

public:
    // Constructor and destructor.
    FlyTerrainTile(void);
    ~FlyTerrainTile(void);

    // Initialize the data for the tile.
    HRESULT Initialize( int startx,int startz,const short* pHeightData );

    // Override the virtual functions.
    void NotifyCamera( FlyCameraBase* pCamera );
    void UpdateObject(void);
    void RenderObject(void);

    // Get the boundingBox of the tile.
    const FlyAabb& GetLocalBoundingBox(void) const;

    // Test the intersect.
    bool IntersectSegment( const FlyVector& vStart,const FlyVector& vEnd,FlyVector* pOut );

    void SetNeighbor( Neighbor nb,FlyTerrainTile* pTile ) { m_pNeighbors[nb] = pTile; }
    FlyTerrainTile* GetNeighbor( Neighbor nb ) const { return m_pNeighbors[nb]; }
    UINT GetNumNeighbors(void) const;

    int GetCurrentLodLevel(void) const { return m_nLodLevel; }

    FlyTexture* GetMainTexture(void) { return m_pMainTexture; }
    void SetMainTexture( FlyTexture* pTexture );
    float GetMainScale(void) const { return m_fMainScale; }
    void SetMainScale( float fScale );

    int GetNumDetailLayers(void) const;
    FlyTexture* GetDetailTexture( int nIndex );
    float GetDetailScale( int nIndex ) const;
    void SetDetailTexture( int nIndex,FlyTexture* pTexture );
    void SetDetailScale( int nIndex,float fScale );

    FlyGraphicBuffer* GetVertexBuffer(void) { return m_pVB; }
    FlyTerrainRenderUnit* GetRenderUnit(void) { return m_pUnit; }

    void SetSceneManager( FlyTerrainSceneManager* pManager ) { m_pManager = pManager; }
    void SetTerrainLevel( FlyTerrainLevel* pLevel ) { m_pLevel = pLevel; }

    FlyTerrainSceneManager* GetSceneManager(void) const { return m_pManager; }
    FlyTerrainLevel* GetTerrainLevel(void) const { return m_pLevel; }
};

//////////////////////////////////////////////////////////////////////////

class FlyTerrainRenderUnit : public FlyRenderable
{
    friend class FlyTerrainTile;

protected:
    FlyTerrainTile* m_pParent;  // Pointer to the terrain tile.

    UINT m_nNumVerts;           // Count of the vertices.
    UINT m_nNumIndis;           // Count of the indices.
    UINT m_nNumPrims;           // Count of the primitives.
    FlyAabb m_BBox;             // BoundingBox of the object.

    FlyGraphicBuffer* m_pVB;    // Pointer to the vertex buffer.
    FlyGraphicBuffer* m_pIB;    // Pointer to the index buffer.

public:
    // Constructor and destructor.
    FlyTerrainRenderUnit( FlyTerrainTile* pParent );
    ~FlyTerrainRenderUnit(void);

    // Render the terrain tile.
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

#endif  //#ifndef __FLY_TERRAIN_TILE_H__