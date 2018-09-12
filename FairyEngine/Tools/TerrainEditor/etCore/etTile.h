//------------------------------------------------------------------------
// Name: etTile.h
// Desc: This file define a class to store the terrain tile data.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_TILE_H__
#define __ET_TILE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etCommon.h"
#include "etFloatRect.h"

#include "FlyGraphicBuffer.h"
#include "FlySceneObject.h"
#include "FlyRenderable.h"
//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etTile : public FlySceneObject
{
    friend class etTileRenderable;
    friend class etSceneManager;

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
    etSceneManager* m_pManager;     // Pointer to the sceneManager.
    etTerrain* m_pLevel;            // Pointer to the level.

    int m_nStartX;                  // Start position to read height data.
    int m_nStartZ;                  // End position to read height data.
    etFloatRect m_BRect;            // BoundingRect of the tile.
    etTile* m_pNeighbors[4];        // Four neighbors of this tile.
    FlyAabb m_Bounds;               // BoundingBox of this tile.
    int m_nLodLevel;                // Current lod level.

    FlyGraphicBuffer* m_pVB;        // Pointer to the vertex buffer.
    FlyTexture* m_pMainTexture;     // Pointer to the main texture.
    float m_fMainScale;             // Main texture's scale of this tile.
    FlyTexture* m_pDetails[4];      // Four detail textures.
    float m_fDetailScale[4];        // Scale of the detail textures.

    bool m_bModified;                   // Is the tile modified ?
    etTileRenderable* m_pRenderable;    // Pointer to the Renderable object.
    etTileSelected* m_pSelected;        // Object to render when the tile is selected.
    bool m_bSeleced;                    // Is the tile selected ?

    void GetIndexBuffer( FlyGraphicBuffer** ppIB,UINT* pNumIndices );
    void GenerateListIndexData( UINT nStitchFlags,FlyGraphicBuffer** ppIB,UINT* pNumIndices );
    UINT StitchEdge( Neighbor nb,int hiLod,int loLod,bool omitFirst,
        bool omitLast,WORD** ppIndex );

public:
    // Constructor and destructor.
    etTile(void);
    ~etTile(void);

    // Initialize the data for the tile.
    HRESULT Initialize( int startx,int startz,const float* pHeightData );

    // Update the height data for the tile.
    void UpdateHeightData(void);

    // Override the virtual functions.
    void NotifyCamera( FlyCameraBase* pCamera );
    void UpdateObject(void);
    void RenderObject(void);

    // Get the local BoundingBox of the tile.
    const FlyAabb& GetLocalBoundingBox(void) const;

    // Get the current lod level.
    int GetCurrentLodLevel(void) const { return m_nLodLevel; }

    // Test the intersect.
    bool IntersectSegment( const FlyVector& vStart,const FlyVector& vEnd,FlyVector* pOut );

    UINT GetNumNeighbors(void) const;
    void SetNeighbor( Neighbor nb,etTile* pTile ) { m_pNeighbors[nb] = pTile; }
    etTile* GetNeighbor( Neighbor nb ) const { return m_pNeighbors[nb]; }

    FlyTexture* GetMainTexture(void) { return m_pMainTexture; }
    void SetMainTexture( FlyTexture* pTexture );
    float GetMainScale(void) const { return m_fMainScale; }
    void SetMainScale( float fScale );

    UINT GetNumDetailLayers(void) const;
    FlyTexture* GetDetailTexture( int nIndex );
    float GetDetailScale( int nIndex ) const;
    void SetDetailTexture( int nIndex,FlyTexture* pTexture );
    void SetDetailScale( int nIndex,float fScale );

    FlyGraphicBuffer* GetVertexBuffer(void) { return m_pVB; }
    etTileRenderable* GetRenderable(void) { return m_pRenderable; }

    void SetSceneManager( etSceneManager* pManager ) { m_pManager = pManager; }
    void SetTerrainLevel( etTerrain* pLevel ) { m_pLevel = pLevel; }

    // Set the modify flag.
    void NeedUpdateHeight(void) { m_bModified = true; }

    // Sort the detail layers for the tile.
    void SortDetailLayers(void);

    bool IsSelected(void) const { return m_bSeleced; }
    void SetSelected( bool bSelected ) { m_bSeleced = bSelected; }

    // Get the boundingRect.
    const etFloatRect& GetBoundingRect(void) const { return m_BRect; }

    etSceneManager* GetSceneManager(void) const { return m_pManager; }
    etTerrain* GetTerrainLevel(void) const { return m_pLevel; }
};

//////////////////////////////////////////////////////////////////////////

class _ET_EXPORT etTileRenderable : public FlyRenderable
{
    friend class etTile;

protected:
    etTile* m_pParent;          // Pointer to the tile.
    FlyAabb m_BBox;             // BoundingBox of the object.

    UINT m_nNumVerts;           // Count of the vertices.
    UINT m_nNumIndis;           // Count of the indices.
    UINT m_nNumPrims;           // Count of the primitives.

    FlyGraphicBuffer* m_pVB;    // Pointer to the vertex buffer.
    FlyGraphicBuffer* m_pIB;    // Pointer to the index buffer.

public:
    // Constructor and destructor.
    etTileRenderable( etTile* pParent );
    ~etTileRenderable(void);

    // Render the terrain tile.
    void Render(void);

    UINT GetNumVerts(void) const;
    UINT GetNumIndis(void) const;
    UINT GetNumPrims(void) const;

    // Get the local boundingBox of the tile.
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

class _ET_EXPORT etTileSelected : public FlyRenderable
{
    friend class etTile;

protected:
    etTile* m_pParent;          // Pointer to the tile.

    UINT m_nNumVerts;           // Count of the vertices.
    UINT m_nNumIndis;           // Count of the indices.
    UINT m_nNumPrims;           // Count of the primitives.

    FlyGraphicBuffer* m_pVB;    // Pointer to the vertex buffer.

public:
    // Constructor and destructor.
    etTileSelected( etTile* pParent );
    ~etTileSelected(void);

    // Build the geometry data.
    void BuildGeometry(void);

    // Render the terrain tile.
    void Render(void);

    UINT GetNumVerts(void) const;
    UINT GetNumIndis(void) const;
    UINT GetNumPrims(void) const;

    // Get the local boundingBox of the tile.
    const FlyAabb& GetBoundingBox(void) const;

    float GetSqrCameraDist( const FlyVector& vCamPos ) const;
    void GetWorldMatrices( FlyMatrix* pMatrices ) const;
    UINT GetNumWorldMatrices(void) const;

    UINT GetLightCount(void) const;
    FlyLightObject* GetLightObject( UINT nIndex );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_TILE_H__