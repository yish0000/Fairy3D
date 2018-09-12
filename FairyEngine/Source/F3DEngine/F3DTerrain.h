/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTerrain.h
 *  Desc:   本文件定义了引擎的地形类。
 *  Author: Yish
 *  Date:   2011/11/22
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_TERRAIN_H__
#define __F3D_TERRAIN_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


/** 地形类
@remarks
    每个地形由多个地形块构成，从而实现大规模室外场景的无缝拼接。
*/
class _F3D_EXPORT F3DTerrain
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

    typedef std::vector<F3DTexture*> TextureList;

protected:
    AString m_sName;            // Name of the terrain level.
    UINT m_nTerrainSize;        // Size of the terrain (in vertices).
    short* m_pHeightData;       // To store the height data.
    UINT m_nTileSize;           // Size per tile (in vertices).
    float m_fHeightScale;       // Height scale for the terrain.
    float m_fLengthPerUnit;     // Lengt per unit for terrain.
    TileInfo* m_pTiles;         // Info of each tile.

    TextureList m_Textures;     // Texture list of the terrain.
    F3DTexture* m_pAlphamap;    // Alphamap for the terrain.
    F3DTexture* m_pLightmap;    // Lightmap for the terrain.

public:
    F3DTerrain(void);
    virtual ~F3DTerrain(void);

    // Initialize the level data, Call this function when user want to 
    // generate the level data by himself.
    bool Initialize( UINT nTerrainSize,UINT nTileSize,UINT nAlphaMapSize,
                     UINT nLightMapSize );

    // Destroy the level, clean the memory.
    void DestroyLevel(void);

    // Load and save the map data.
    HRESULT LoadTerrainMap( const char* cFilename );
    HRESULT SaveTerrainMap( const char* cFilename );

    // Load and save the heightmap from RAW.
    HRESULT LoadHeightMap( const char* cRAWFile,int nSize );
    HRESULT SaveHeightMap( const char* cRAWFile );

    HRESULT LoadAlphaMap( const char* filename );
    HRESULT SaveAlphaMap( const char* filename );
    HRESULT LoadLightMap( const char* filename );
    HRESULT SaveLightMap( const char* filename );

    F3DTexture* CreateTexture( const char* filename );
    size_t GetNumTextures(void) const { return m_Textures.size(); }
    F3DTexture* GetTexture( size_t nIndex );

    short* GetHeightData(void) { return m_pHeightData; }
    const short* GetHeightData(void) const { return m_pHeightData; }
    short GetHeightValue( UINT x,UINT z ) const;
    float GetScaledHeightValue( float x,float z ) const;
    void GetNormalAt( float x,float z,F3DVector* pOut ) const;
    const TileInfo* GetTileInfo( UINT x,UINT z ) const;

    UINT GetTerrainSize(void) const { return m_nTerrainSize; }
    UINT GetTileSize(void) const { return m_nTileSize; }
    float GetHeightScale(void) const { return m_fHeightScale; }
    float GetLengthPerUnit(void) const { return m_fLengthPerUnit; }

    void SetLengthPerUnit( float fLength ) { m_fLengthPerUnit = fLength; }
    void SetHeightScale( float fScale ) { m_fHeightScale = fScale; }
    void SetHeightValue( UINT x,UINT z,short nHeight );

    F3DTexture* GetAlphaMap(void) { return m_pAlphamap; }
    F3DTexture* GetLightMap(void) { return m_pLightmap; }

    const AString& GetName(void) const { return m_sName; }
    void SetName( const AString& name ) { m_sName = name; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_TERRAIN_H__