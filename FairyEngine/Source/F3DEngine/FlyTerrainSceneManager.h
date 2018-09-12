//------------------------------------------------------------------------
// Name: FlyTerrainSceneManager.h
// Desc: This file define a sceneManager to render the terrain for 
//       engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_TERRAIN_SCENEMANAGER_H__
#define __FLY_TERRAIN_SCENEMANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyOctreeSceneManager.h"

#include "FlyTerrainLevel.h"
#include "FlyTerrainTile.h"
//////////////////////////////////////////////////////////////////////////


class _FLY_EXPORT FlyTerrainSceneManager : public FlyOctreeSceneManager
{
    friend class FlyTerrainTile;

    typedef std::vector<FlyTerrainTile*> TerrainTileList;

    struct IndexBuffer
    {
        UINT nNumIndices;       // Count of the indices.
        FlyGraphicBuffer* pIB;  // Pointer to the index buffer.
    };
    typedef std::map<UINT,IndexBuffer> IndexBufferMap;
    typedef std::vector<IndexBufferMap> LevelIndexData;

protected:
    FlyTerrainLevel* m_pLevel;          // Pointer to the terrain level.
    FlyTerrainTile* m_pTerrainTiles;    // Terrain tile array.
    FlyRenderEffect* m_pRenderEffect;   // Pointer to the renderEffect.
    LevelIndexData m_IndexData;         // Index data of the level.

    UINT m_nMaxLodLevel;                // Max lod level for the terrain.
    float* m_fLodSqrDist;               // To store the lod square distance array.
    int m_nRenderQuality;               // Render Quality of the terrain.
    UINT m_nMaxPixelError;              // Max pixel error.
    bool m_bUseTriStrips;               // Use the TriStrip mode to render.
    bool m_bOpenLight;                  // Open the light for terrain ?

    // Build the terrain tiles for manager.
    HRESULT BuildTerrainTiles(void);

    // Calculate the lod square distance.
    void CalcLodSqrDistance(void);

public:
    // Constructor and destructor.
    FlyTerrainSceneManager(void);
    ~FlyTerrainSceneManager(void);

    // Load the terrain geometry data.
    HRESULT LoadWorldGeometry( const char* cFilename );

    // Clear the scene data.
    void ClearScene(void);

    // Test the collistion in the 3D world.
    bool TestCollision( const FlyRay& ray,float fL,float& fDist );

    // Add a new indexBuffer for the tile.
    void AddIndexBuffer( FlyGraphicBuffer* pIB,UINT nNumIndices,int nLodLevel,DWORD dwStitchFlags );

    // Get the indexBuffer for tile.
    void GetIndexBuffer( int nLodLevel,UINT dwStitchFlags,FlyGraphicBuffer** ppIB,UINT* pNumIndices );

    // Get the height value.
    float GetHeightValue( const FlyVector& vPostion ) const;

    FlyTerrainTile* GetTerrainTile( const FlyVector& vPoint );
    FlyRenderEffect* GetRenderEffect(void) const { return m_pRenderEffect; }

    bool IsUseTriStripMode(void) const { return m_bUseTriStrips; }
    UINT GetMaxPixelError(void) const { return m_nMaxPixelError; }
    bool IsLightOpen(void) const { return m_bOpenLight; }

    void UseTriStripMode( bool bStripMode ) { m_bUseTriStrips = bStripMode; }
    void SetMaxPixelError( UINT nMaxError ) { m_nMaxPixelError = nMaxError; }

    UINT GetMaxLodLevel(void) const { return m_nMaxLodLevel; }
    float GetLodSqrDist( int nLodLevel ) const { return m_fLodSqrDist[nLodLevel]; }

    void SetLightMode( bool bOpenLight );

    void SetWireMode( bool bWireMode );

    // A static variable to store the shader text.
    static const std::string s_sEffectText;
};

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlyTerrainSceneFactory : public FlySceneFactory
{
public:
    // Constructor and destructor.
    FlyTerrainSceneFactory(void) { m_sTypename = "terrain"; }
    ~FlyTerrainSceneFactory(void) { /* Nothing to do! */ };

    FlySceneManager* CreateInstance( const char* cName );
    void DestroyInstance( FlySceneManager* pManager );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_TERRAIN_SCENEMANAGER_H__