//------------------------------------------------------------------------
// Name: FlyTerrainLevel.h
// Desc: This file define a class to store the terrain level data.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_TERRAIN_LEVEL_H__
#define __FLY_TERRAIN_LEVEL_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


class _FLY_EXPORT FlyTerrainLevel
{
    friend class FlyTerrainSceneManager;

public:
    struct TileInfo
    {
        UINT nMainTexture;
        float fMainScale;
        UINT nDetailTexture[4];
        float fDetailScale[4];
    };

    typedef std::vector<FlyTexture*> TextureList;

protected:
    std::string m_sName;        // Name of the terrain level.
    UINT m_nTerrainSize;        // Size of the terrain (in vertices).
    short* m_pHeightData;       // To store the height data.
    UINT m_nTileSize;           // Size per tile (in vertices).
    float m_fHeightScale;       // Height scale for the terrain.
    float m_fLengthPerUnit;     // Lengt per unit for terrain.
    TileInfo* m_pTiles;         // Info of each tile.

    TextureList m_Textures;     // Texture list of the terrain.
    FlyTexture* m_pAlphamap;    // Alphamap for the terrain.
    FlyTexture* m_pLightmap;    // Lightmap for the terrain.

public:
    // Constructor and destructor.
    FlyTerrainLevel(void);
    ~FlyTerrainLevel(void);

    // Initialize the level data, Call this function when user want to 
    // generate the level data by himself.
    HRESULT Initialize( UINT nTerrainSize,UINT nTileSize,UINT nAlphaMapSize,
                        UINT nLightMapSize );

    // Destroy the level, clean the memory.
    void DestroyLevel(void);

    // Load and save the map data.
    HRESULT LoadTerrainMap( const char* cFilename );
    HRESULT SaveTerrainMap( const char* cFilename );

    // Load and save the heightmap from RAW.
    HRESULT LoadHeightMap( const char* cRAWFile,int nSize );
    HRESULT SaveHeightMap( const char* cRAWFile );

    HRESULT LoadAlphaMap( const char* cFilename );
    HRESULT SaveAlphaMap( const char* cFilename );
    HRESULT LoadLightMap( const char* cFilename );
    HRESULT SaveLightMap( const char* cFilename );

    FlyTexture* CreateTexture( const char* cFilename );
    UINT GetNumTextures(void) const { return m_Textures.size(); }
    FlyTexture* GetTexture( int nIndex );

    short* GetHeightData(void) { return m_pHeightData; }
    const short* GetHeightData(void) const { return m_pHeightData; }
    short GetHeightValue( int x,int z ) const;
    float GetScaledHeightValue( float x,float z ) const;
    void GetNormalAt( float x,float z,FlyVector* pOut ) const;
    const TileInfo* GetTileInfo( int x,int z ) const;

    UINT GetTerrainSize(void) const { return m_nTerrainSize; }
    UINT GetTileSize(void) const { return m_nTileSize; }
    float GetHeightScale(void) const { return m_fHeightScale; }
    float GetLengthPerUnit(void) const { return m_fLengthPerUnit; }

    void SetLengthPerUnit( float fLength ) { m_fLengthPerUnit = fLength; }
    void SetHeightScale( float fScale ) { m_fHeightScale = fScale; }
    void SetHeightValue( int x,int z,short nHeight );

    FlyTexture* GetAlphaMap(void) { return m_pAlphamap; }
    FlyTexture* GetLightMap(void) { return m_pLightmap; }

    const char* GetName(void);
    void SetName( const char* cName );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_TERRAIN_LEVEL_H__