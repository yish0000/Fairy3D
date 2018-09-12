//------------------------------------------------------------------------
// Name: etTerrain.h
// Desc: This file define a class to store the terrain data.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_TERRAIN_H__
#define __ET_TERRAIN_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etCommon.h"

#include "etDetailLayer.h"
#include "etLightLayer.h"
#include "etTile.h"
//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etTerrain
{
    friend class etSceneManager;

    struct IndexBuffer
    {
        UINT nNumIndices;
        FlyGraphicBuffer* pIB;
    };

    typedef std::map<UINT,IndexBuffer> IndexBufferMap;
    typedef std::vector<IndexBufferMap> LodIndexData;
    typedef std::vector<FlyTexture*> TextureList;

protected:
    std::string m_sName;            // Name of the terrain level.

    UINT m_nTerrainSize;            // Size of the terrain.
    float* m_pHeightData;           // To store the height data.
    UINT m_nTileSize;               // Size of the tile.
    float m_fHeightScale;           // Height scale of the terrain.
    float m_fLengthPerUnit;         // Length scale of the terrain.

    UINT m_nNumTiles;               // Count of tiles.
    etTile* m_pTerrainTiles;        // Terrain tile array.
    etTile* m_pTileSelected;        // Pointer to the tile selected.

    LodIndexData m_IndexData;       // Index data for the tiles.

    FlyTexture* m_pNoTexture;       // Default texture.
    TextureList m_Textures;         // Textures used for terrain.
    etDetailLayer m_DetailLayer;    // Detail layer of the terrain.
    etLightLayer m_LightLayer;      // Light layer of the terrain.

    UINT m_nMaxLodLevel;            // Max lod level.
    float* m_fLodSqrDist;           // Lod Square distance.

    // Calculate the lod square distance.
    void CalcLodSqrDistance(void);

public:
    // Constructor and destructor.
    etTerrain(void);
    ~etTerrain(void);

    // Initialize the level data, Call this function when user want to 
    // generate the level data by himself.
    HRESULT Initialize( UINT nTerrainSize,UINT nTileSize,UINT nAlphaMapSize,
        float fInitHeight );

    // Destroy the level, clean the memory.
    void DestroyLevel(void);

    const char* GetName(void) const;
    void SetName( const char* cName );

    // Load and save the map data.
    HRESULT LoadTerrainMap( const char* cFilename );
    HRESULT SaveTerrainMap( const char* cFilename );

    // Load and save the heightmap from RAW.
    HRESULT LoadHeightMap( const char* cRAWFile,int nSize );
    HRESULT SaveHeightMap( const char* cRAWFile );

    HRESULT AddTexture( const char* cFilename );
    void DeleteTexture( UINT nIndex );
    UINT GetNumTextures(void) const;
    FlyTexture* GetTexture( UINT nTextureID );
    FlyTexture* GetDefaultTexture(void) { return m_pNoTexture; }
    UINT GetTextureID( FlyTexture* pTexture ) const;

    float* GetHeightData(void) { return m_pHeightData; }
    const float* GetHeightData(void) const { return m_pHeightData; }
    float GetHeightValue( int x,int z ) const;
    float GetScaledHeightValue( float x,float z ) const;
    void GetNormalAt( int x,int z,FlyVector* pOut ) const;

    UINT GetTerrainSize(void) const { return m_nTerrainSize; }
    UINT GetTileSize(void) const { return m_nTileSize; }
    float GetHeightScale(void) const { return m_fHeightScale; }
    float GetLengthPerUnit(void) const { return m_fLengthPerUnit; }

    void SetLengthPerUnit( float fLength ) { m_fLengthPerUnit = fLength; }
    void SetHeightScale( float fScale ) { m_fHeightScale = fScale; }
    void SetHeightValue( int x,int z,float fHeight );

    etDetailLayer* GetDetailLayer(void) { return &m_DetailLayer; }
    etLightLayer* GetLightLayer(void) { return &m_LightLayer; }

    // Test the intersecting with the segment.
    bool IntersectSegment( const FlyVector& vStart,const FlyVector& vEnd,
        FlyVector* pOut );

    // Select the tile.
    void SelectTile( float fX,float fZ );

    // Get the tile at the specified position.
    etTile* GetTerrainTileAt( const FlyVector& vPoint );
    UINT GetNumTiles(void) const { return m_nNumTiles; }
    etTile* GetTerrainTile( UINT nIndex ) { return &m_pTerrainTiles[nIndex]; }
    etTile* GetSelectedTile(void) { return m_pTileSelected; }

    // Add a new indexBuffer for the tile.
    void AddIndexBuffer( FlyGraphicBuffer* pIB,UINT nNumIndices,int nLodLevel,
        UINT nStitchFlags );

    // Get the indexBuffer for tile.
    void GetIndexBuffer( int nLodLevel,UINT nStitchFlags,
        FlyGraphicBuffer** ppIB,UINT* pNumIndices );

    UINT GetMaxLodLevel(void) const { return m_nMaxLodLevel; }
    float GetLodSqrDist( int nLodLevel ) const { return m_fLodSqrDist[nLodLevel]; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_TERRAIN_H__