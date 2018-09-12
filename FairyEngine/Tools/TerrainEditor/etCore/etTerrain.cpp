//------------------------------------------------------------------------
// Name: etTerrain.cpp
// Desc: This file is to implement the class etTerrain.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etTerrain.h"

#include "etCoreManager.h"
#include "etSceneManager.h"

#include "FlyStringUtil.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etTerrain::etTerrain(void)
{
    m_sName = "Untitled Map";

    m_nTerrainSize = 0;
    m_pHeightData = NULL;
    m_nTileSize = 0;
    m_fHeightScale = 1.0f;
    m_fLengthPerUnit = 1.0f;
    m_nNumTiles = 0;
    m_pTerrainTiles = NULL;
    m_pTileSelected = NULL;

    m_nMaxLodLevel = 0;
    m_fLodSqrDist = NULL;

    // Create the default texture.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();
    m_pNoTexture = pResMgr->Load2DTexture( "Texture\\notexture.bmp",0 );
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etTerrain::~etTerrain(void)
{
    SAFE_DELETE_ARRAY( m_pHeightData );
    SAFE_DELETE_ARRAY( m_pTerrainTiles );
    SAFE_DELETE_ARRAY( m_fLodSqrDist );

    // Destroy all the indexBuffers.
    LodIndexData::iterator itr;
    for( itr=m_IndexData.begin();itr!=m_IndexData.end();itr++ )
    {
        IndexBufferMap bufferMap = *itr;

        IndexBufferMap::iterator buf;
        for( buf=bufferMap.begin();buf!=bufferMap.end();buf++ )
            SAFE_DELETE( buf->second.pIB );
    }

    // Destroy all the textures.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    TextureList::iterator tex;
    for( tex=m_Textures.begin();tex!=m_Textures.end();tex++ )
        pResMgr->DeleteTexture( *tex );

    // Destroy the default texture.
    pResMgr->DeleteTexture( m_pNoTexture );
}


//------------------------------------------------------------------------
// Destroy the terrain level, clean the memory.
//------------------------------------------------------------------------
void etTerrain::DestroyLevel(void)
{
    SAFE_DELETE_ARRAY( m_pHeightData );
    SAFE_DELETE_ARRAY( m_pTerrainTiles );
    SAFE_DELETE_ARRAY( m_fLodSqrDist );

    // Destroy all the indexBuffers.
    LodIndexData::iterator itr;
    for( itr=m_IndexData.begin();itr!=m_IndexData.end();itr++ )
    {
        IndexBufferMap bufferMap = *itr;

        IndexBufferMap::iterator buf;
        for( buf=bufferMap.begin();buf!=bufferMap.end();buf++ )
            SAFE_DELETE( buf->second.pIB );
    }
    m_IndexData.clear();

    // Destroy the textures.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    TextureList::iterator tex;
    for( tex=m_Textures.begin();tex!=m_Textures.end();tex++ )
        pResMgr->DeleteTexture( *tex );

    m_Textures.clear();
    m_DetailLayer.DestroyLayer();
    m_LightLayer.DestroyLayer();

    m_nTerrainSize = 0;
    m_nTileSize = 0;
    m_fHeightScale = 1.0f;
    m_fLengthPerUnit = 1.0f;
    m_nNumTiles = 0;
    m_nMaxLodLevel = 0;
    m_sName = "Untitled Map";
}


//------------------------------------------------------------------------
// Intialize the terrain data, Call this function when user want to 
// generate the level data by themself.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Size of the whole terrain.
//      UINT:       Size of the each tile.
//      UINT:       Size of the alphamap.
//      float:      Initial height of the terrain.
//------------------------------------------------------------------------
HRESULT etTerrain::Initialize( UINT nTerrainSize,UINT nTileSize,UINT nAlphaMapSize,
                               float fInitHeight )
{
    HRESULT hr;

    // Destroy the old data.
    DestroyLevel();

    m_nTerrainSize = nTerrainSize;
    m_nTileSize = nTileSize;

    // Create a buffer to store height data.
    m_pHeightData = new float[(nTerrainSize+1)*(nTerrainSize+1)];
    if( !m_pHeightData ) return FLY_OUTOFMEMORY;

    for( int i=0;i<(nTerrainSize+1)*(nTerrainSize+1);i++ )
        m_pHeightData[i] = fInitHeight;

    // Initialize the detail layer.
    hr = m_DetailLayer.Initialize( nAlphaMapSize );
    if( FAILED(hr) ) return hr;

    // Initialize the light layer.
    hr = m_LightLayer.Initialize( 256 );
    if( FAILED(hr) ) return hr;

    // Initialize the tiles.
    UINT nNumTiles = m_nTerrainSize / m_nTileSize;
    m_nNumTiles = nNumTiles * nNumTiles;
    m_pTerrainTiles = new etTile[nNumTiles*nNumTiles];
    for( int j=0;j<nNumTiles;j++ )
    {
        for( int i=0;i<nNumTiles;i++ )
        {
            char cTemp[40];

            etTile* pTile = &m_pTerrainTiles[j*nNumTiles+i];
            sprintf( cTemp,"Tile(%d,%d)",i,j );
            pTile->SetName( cTemp );
            pTile->SetSceneManager( etCoreManager::Instance().GetSceneManager() );
            pTile->SetCreator( NULL );
            pTile->SetTerrainLevel( this );

            // Initialize the terrain tile.
            pTile->Initialize( m_nTileSize*i,m_nTileSize*j,m_pHeightData );

            if( i - 1 >= 0 )
                pTile->SetNeighbor( etTile::NB_WEST,&m_pTerrainTiles[j*nNumTiles+(i-1)] );
            else
                pTile->SetNeighbor( etTile::NB_WEST,NULL );

            if( j - 1 >= 0 )
                pTile->SetNeighbor( etTile::NB_SOUTH,&m_pTerrainTiles[(j-1)*nNumTiles+i] );
            else
                pTile->SetNeighbor( etTile::NB_SOUTH,NULL );

            if( i + 1 < nNumTiles )
                pTile->SetNeighbor( etTile::NB_EAST,&m_pTerrainTiles[j*nNumTiles+(i+1)] );
            else
                pTile->SetNeighbor( etTile::NB_EAST,NULL );

            if( j + 1 < nNumTiles )
                pTile->SetNeighbor( etTile::NB_NORTH,&m_pTerrainTiles[(j+1)*nNumTiles+i] );
            else
                pTile->SetNeighbor( etTile::NB_NORTH,NULL );

            pTile->SetMainTexture( m_pNoTexture );
            pTile->SetMainScale( 1.0f );
            pTile->SetDetailTexture( 0,NULL );
            pTile->SetDetailTexture( 1,NULL );
            pTile->SetDetailTexture( 2,NULL );
            pTile->SetDetailTexture( 3,NULL );
            pTile->SetDetailScale( 0,1.0f );
            pTile->SetDetailScale( 1,1.0f );
            pTile->SetDetailScale( 2,1.0f );
            pTile->SetDetailScale( 3,1.0f );
        }
    }

    // Calculate the lod square distance.
    CalcLodSqrDistance();

    // Prepare the indices for the terrain tiles.
    // Initialize the indices for terrain.
    etSceneManager* pManager = etCoreManager::Instance().GetSceneManager();
    for( j=0;j<m_nMaxLodLevel;j++ )
        m_IndexData.push_back( IndexBufferMap() );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Calculate the lod square distance.
//------------------------------------------------------------------------
void etTerrain::CalcLodSqrDistance(void)
{
    // Calculate the max lod level.
    m_nMaxLodLevel = 0;

    UINT nTemp = m_nTileSize;
    while( nTemp != 0 )
    {
        nTemp = nTemp / 2;
        m_nMaxLodLevel++;
    }

    m_fLodSqrDist = new float[m_nMaxLodLevel];
    m_fLodSqrDist[0] = 0.0f;
    m_fLodSqrDist[1] = 64.0f * 64.0f;

    for( int i=2;i<m_nMaxLodLevel;i++ )
        m_fLodSqrDist[i] = m_fLodSqrDist[i-1] * 4.0f;
}


//------------------------------------------------------------------------
// Load the terrain data from the map file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the map file.
//------------------------------------------------------------------------
HRESULT etTerrain::LoadTerrainMap( const char* cFilename )
{
    HRESULT hr;
    char cIdentifier[4];
    DWORD dwVersion;
    std::string str;
    UINT nSize;

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    // Load the map file.
    FILE* pFile = fopen( cFilename,"rb" );
    if( !pFile ) return FLY_FILENOTFOUND;

    cIdentifier[0] = fgetc( pFile );
    cIdentifier[1] = fgetc( pFile );
    cIdentifier[2] = fgetc( pFile );
    cIdentifier[3] = fgetc( pFile );
    fread( &dwVersion,sizeof(DWORD),1,pFile );

    if( strcmp(cIdentifier,"MAP") != 0 ||
        dwVersion != FLY_VERSION(2,0) )
    {
        fclose( pFile );
        return FLY_INVALIDVERSION;
    }

    // Destroy the old data.
    DestroyLevel();

    // 1. Read the terrain info.
    fread( &m_nTerrainSize,sizeof(UINT),1,pFile );
    fread( &m_nTileSize,sizeof(UINT),1,pFile );
    fread( &m_fHeightScale,sizeof(float),1,pFile );
    fread( &m_fLengthPerUnit,sizeof(float),1,pFile );

    // Initialize the terrain.
    hr = Initialize( m_nTerrainSize,m_nTileSize,256,256 );
    if( FAILED(hr) )
    {
        fclose( pFile );
        return hr;
    }

    // 2. Read the height data.
    short* pHeightData = new short[(m_nTerrainSize+1)*(m_nTerrainSize+1)];
    if( !pHeightData ) return FLY_OUTOFMEMORY;
    fread( pHeightData,sizeof(short),(m_nTerrainSize+1)*(m_nTerrainSize+1),pFile );
    for( int y=0;y<(m_nTerrainSize+1);y++ )
    {
        for( int x=0;x<(m_nTerrainSize+1);x++ )
            m_pHeightData[y*(m_nTerrainSize+1)+x] = pHeightData[y*(m_nTerrainSize+1)+x];
    }
    delete[] pHeightData;

    // 3. Read the alphaMap data.
    fread( &nSize,sizeof(UINT),1,pFile );
    hr = m_DetailLayer.Initialize( nSize );
    if( FAILED(hr) ) return hr;
    fread( m_DetailLayer.GetTempBuffer(),nSize*nSize,4,pFile );

    // 4. Read the lightMap data.
    fread( &nSize,sizeof(UINT),1,pFile );
    hr = m_LightLayer.Initialize( nSize );
    if( FAILED(hr) ) return hr;
    BYTE* pTemp = new BYTE[nSize*nSize*3];
    fread( pTemp,nSize*nSize,3,pFile );
    DWORD* pLightmap = static_cast<DWORD*>(m_LightLayer.GetTempBuffer());
    for( y=0;y<nSize;y++ )
    {
        for( int x=0;x<nSize;x++ )
        {
            pLightmap[y*nSize+x] = MAKE3DCOLOR( 255,pTemp[(y*nSize+x)*3+0],
                pTemp[(y*nSize+x)*3+1],pTemp[(y*nSize+x)*3+2] );
        }
    }
    delete[] pTemp;

    // 5. Read the textures.
    fread( &nSize,sizeof(UINT),1,pFile );
    for( int i=0;i<nSize;i++ )
    {
        size_t nLen;
        fread( &nLen,sizeof(size_t),1,pFile );
        str.resize( nLen,'\0' );
        fread( &str[0],nLen,1,pFile );
        AddTexture( str.c_str() );
    }

    // 6. Read the terrain tiles.
    fread( &nSize,sizeof(int),1,pFile );
    for( i=0;i<nSize;i++ )
    {
        int nTextureID;
        float fScale;
        etTile* pTile = &m_pTerrainTiles[i];

        fread( &nTextureID,sizeof(int),1,pFile );
        pTile->SetMainTexture( GetTexture(nTextureID) );
        fread( &fScale,sizeof(float),1,pFile );
        pTile->SetMainScale( fScale );

        fread( &nTextureID,sizeof(int),1,pFile );
        pTile->SetDetailTexture( 0,GetTexture(nTextureID) );
        fread( &nTextureID,sizeof(int),1,pFile );
        pTile->SetDetailTexture( 1,GetTexture(nTextureID) );
        fread( &nTextureID,sizeof(int),1,pFile );
        pTile->SetDetailTexture( 2,GetTexture(nTextureID) );
        fread( &nTextureID,sizeof(int),1,pFile );
        pTile->SetDetailTexture( 3,GetTexture(nTextureID) );
        fread( &fScale,sizeof(float),1,pFile );
        pTile->SetDetailScale( 0,fScale );
        fread( &fScale,sizeof(float),1,pFile );
        pTile->SetDetailScale( 1,fScale );
        fread( &fScale,sizeof(float),1,pFile );
        pTile->SetDetailScale( 2,fScale );
        fread( &fScale,sizeof(float),1,pFile );
        pTile->SetDetailScale( 3,fScale );
    }

    fclose( pFile );
    return FLY_OK;
}


//------------------------------------------------------------------------
// Save the terrain level data to the specified map file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the terrain map file.
//------------------------------------------------------------------------
HRESULT etTerrain::SaveTerrainMap( const char* cFilename )
{
    DWORD dwVersion;
    UINT nSize;

    // Create a new map file.
    FILE* pFile = fopen( cFilename,"wb" );
    if( !pFile ) return FLY_FAIL;

    fputc( 'M',pFile );
    fputc( 'A',pFile );
    fputc( 'P',pFile );
    fputc( '\0',pFile );
    dwVersion = FLY_VERSION( 2,0 );
    fwrite( &dwVersion,sizeof(DWORD),1,pFile );

    // 1. Write the terrain info.
    fwrite( &m_nTerrainSize,sizeof(UINT),1,pFile );
    fwrite( &m_nTileSize,sizeof(UINT),1,pFile );
    fwrite( &m_fHeightScale,sizeof(float),1,pFile );
    fwrite( &m_fLengthPerUnit,sizeof(float),1,pFile );

    // 2. Write the height data.
    short* pHeightData = new short[(m_nTerrainSize+1)*(m_nTerrainSize+1)];
    if( !pHeightData )
    {
        fclose( pFile );
        return FLY_OUTOFMEMORY;
    }
    for( int y=0;y<(m_nTerrainSize+1);y++ )
    {
        for( int x=0;x<(m_nTerrainSize+1);x++ )
            pHeightData[y*(m_nTerrainSize+1)+x] = (short)m_pHeightData[y*(m_nTerrainSize+1)+x];
    }
    fwrite( pHeightData,sizeof(short),(m_nTerrainSize+1)*(m_nTerrainSize+1),pFile );
    delete[] pHeightData;

    // 3. Write the alphamap data.
    nSize = m_DetailLayer.GetAlphamapSize();
    fwrite( &nSize,sizeof(UINT),1,pFile );
    fwrite( m_DetailLayer.GetTempBuffer(),nSize*nSize,4,pFile );

    // 4. Write the lightmap data.
    nSize = m_LightLayer.GetLightmapSize();
    fwrite( &nSize,sizeof(UINT),1,pFile );
    BYTE* pTemp = new BYTE[nSize*nSize*3];
    for( y=0;y<nSize;y++ )
    {
        for( int x=0;x<nSize;x++ )
        {
            DWORD* pBuffer = m_LightLayer.GetTempBuffer();
            pTemp[(y*nSize+x)+0] = (pBuffer[y*nSize+x] & 0x00ff0000) >> 16;
            pTemp[(y*nSize+x)+1] = (pBuffer[y*nSize+x] & 0x0000ff00) >> 8;
            pTemp[(y*nSize+x)+2] = (pBuffer[y*nSize+x] & 0x000000ff);
        }
    }
    fwrite( pTemp,nSize*nSize,3,pFile );
    delete[] pTemp;

    // 5. Write the texture infos.
    nSize = m_Textures.size();
    fwrite( &nSize,sizeof(UINT),1,pFile );
    for( y=0;y<nSize;y++ )
    {
        std::string texture;
        texture = FlyStringUtil::GetNameFromPath( m_Textures[y]->GetName() );
        nSize = texture.size();
        fwrite( &nSize,sizeof(UINT),1,pFile );
        fwrite( texture.c_str(),nSize,1,pFile );
    }

    // 6. Write the tile data.
    fwrite( &m_nNumTiles,sizeof(UINT),1,pFile );
    for( int i=0;i<m_nNumTiles;i++ )
    {
        int nTextureID;
        float fScale;

        nTextureID = GetTextureID(m_pTerrainTiles[i].GetMainTexture());
        fwrite( &nTextureID,sizeof(int),1,pFile );
        fScale = m_pTerrainTiles[i].GetMainScale();
        fwrite( &fScale,sizeof(float),1,pFile );

        nTextureID = GetTextureID(m_pTerrainTiles[i].GetDetailTexture(0));
        fwrite( &nTextureID,sizeof(int),1,pFile );
        fScale = m_pTerrainTiles[i].GetDetailScale(0);
        fwrite( &fScale,sizeof(float),1,pFile );
        nTextureID = GetTextureID(m_pTerrainTiles[i].GetDetailTexture(1));
        fwrite( &nTextureID,sizeof(int),1,pFile );
        fScale = m_pTerrainTiles[i].GetDetailScale(1);
        fwrite( &fScale,sizeof(float),1,pFile );
        nTextureID = GetTextureID(m_pTerrainTiles[i].GetDetailTexture(2));
        fwrite( &nTextureID,sizeof(int),1,pFile );
        fScale = m_pTerrainTiles[i].GetDetailScale(2);
        fwrite( &fScale,sizeof(float),1,pFile );
        nTextureID = GetTextureID(m_pTerrainTiles[i].GetDetailTexture(3));
        fwrite( &nTextureID,sizeof(int),1,pFile );
        fScale = m_pTerrainTiles[i].GetDetailScale(3);
        fwrite( &fScale,sizeof(float),1,pFile );
    }

    fclose( pFile );
    return FLY_OK;
}


//------------------------------------------------------------------------
// Load the heightmap from the RAW file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the heightmap.
//      int:            Size of the heightmap.
//------------------------------------------------------------------------
HRESULT etTerrain::LoadHeightMap( const char* cFilename,int nSize )
{
    if( nSize != m_nTerrainSize )
        return FLY_INVALIDPARAM;

    FILE* pFile = fopen( cFilename,"rb" );
    if( !pFile ) return FLY_FILENOTFOUND;

    BYTE* pBuffer = new BYTE[(nSize+1)*(nSize+1)];
    if( !pBuffer )
    {
        fclose( pFile );
        return FLY_OUTOFMEMORY;
    }

    fread( pBuffer,nSize+1,nSize+1,pFile );
    fclose( pFile );

    for( int j=0;j<nSize+1;j++ )
    {
        for( int i=0;i<nSize+1;i++ )
            m_pHeightData[j*(nSize+1)+i] = (float)pBuffer[j*(nSize+1)+i];
    }

    delete[] pBuffer;
    return FLY_OK;
}


//------------------------------------------------------------------------
// Save the heightmap to the RAW file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the heightmap.
//------------------------------------------------------------------------
HRESULT etTerrain::SaveHeightMap( const char* cFilename )
{
    BYTE* pBuffer;

    FILE* pFile = fopen( cFilename,"wb" );
    if( !pFile ) return FLY_FAIL;

    pBuffer = new BYTE[(m_nTerrainSize+1)*(m_nTerrainSize+1)];
    if( !pBuffer )
    {
        fclose( pFile );
        return FLY_OUTOFMEMORY;
    }

    for( int j=0;j<m_nTerrainSize+1;j++ )
    {
        for( int i=0;i<m_nTerrainSize+1;i++ )
        {
            float h = m_pHeightData[j*(m_nTerrainSize+1)+i];

            if( h > 255 ) h = 255;
            if( h < 0 ) h = 0;
            pBuffer[j*(m_nTerrainSize+1)+i] = (BYTE)h;
        }
    }

    fwrite( pBuffer,m_nTerrainSize+1,m_nTerrainSize+1,pFile );
    fclose( pFile );

    delete[] pBuffer;
    return FLY_OK;
}


//------------------------------------------------------------------------
// Add a new texture for the terrain level.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the texture file.
//------------------------------------------------------------------------
HRESULT etTerrain::AddTexture( const char* cFilename )
{
    FlyTexture* pTexture;

    // Check whether the texture existed.
    TextureList::iterator itr;
    for( itr=m_Textures.begin();itr!=m_Textures.end();itr++ )
    {
        if( strcmp((*itr)->GetName(),cFilename) == 0 )
            return ET_TEXTUREEXISTED;
    }

    // Get the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();
    pTexture = pResMgr->Load2DTexture( cFilename,0 );
    if( !pTexture ) return ET_TEXTURELOADFAIL;

    m_Textures.push_back( pTexture );
    return FLY_OK;
}


//------------------------------------------------------------------------
// Delete the specified texture for the terrain.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        The index of the texture.
//------------------------------------------------------------------------
void etTerrain::DeleteTexture( UINT nIndex )
{
    FlyTexture* pTexture;

    if( nIndex >= m_Textures.size() )
        return;

    // Get the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();
    pTexture = m_Textures[nIndex];

    for( int i=0;i<m_nNumTiles;i++ )
    {
        if( m_pTerrainTiles[i].GetMainTexture() == pTexture )
            m_pTerrainTiles[i].SetMainTexture( m_pNoTexture );
        if( m_pTerrainTiles[i].GetDetailTexture(0) == pTexture )
            m_pTerrainTiles[i].SetDetailTexture( 0,NULL );
        if( m_pTerrainTiles[i].GetDetailTexture(1) == pTexture )
            m_pTerrainTiles[i].SetDetailTexture( 1,NULL );
        if( m_pTerrainTiles[i].GetDetailTexture(2) == pTexture )
            m_pTerrainTiles[i].SetDetailTexture( 2,NULL );
        if( m_pTerrainTiles[i].GetDetailTexture(3) == pTexture )
            m_pTerrainTiles[i].SetDetailTexture( 3,NULL );

        // Sort the detail layers.
        m_pTerrainTiles[i].SortDetailLayers();
    }

    // Destroy the specified texture.
    pResMgr->DeleteTexture( pTexture );
    m_Textures.erase( m_Textures.begin() + nIndex );
}


//------------------------------------------------------------------------
// Get the count of the textures.
//------------------------------------------------------------------------
UINT etTerrain::GetNumTextures(void) const
{
    return m_Textures.size();
}


//------------------------------------------------------------------------
// Get the specified texture by its id.
//------------------------------------------------------------------------
FlyTexture* etTerrain::GetTexture( UINT nTextureID )
{
    if( nTextureID >= m_Textures.size() )
        return NULL;

    return m_Textures[nTextureID];
}


//------------------------------------------------------------------------
// Get the specified texture id by its pointer.
//------------------------------------------------------------------------
UINT etTerrain::GetTextureID( FlyTexture* pTexture ) const
{
    bool bFound = false;

    for( UINT i=0;i<m_Textures.size();i++ )
    {
        if( pTexture == m_Textures[i] )
        {
            bFound = true;
            break;
        }
    }

    return bFound ? i : -1;
}


//------------------------------------------------------------------------
// Get the height value of specified position.
//------------------------------------------------------------------------
float etTerrain::GetHeightValue( int x,int z ) const
{
    if( x < 0 || x >= (m_nTerrainSize+1) ||
        z < 0 || z >= (m_nTerrainSize+1) )
        return -1.0f;

    return m_pHeightData[z*(m_nTerrainSize+1)+x];
}


//------------------------------------------------------------------------
// Get the scaled height value of specified position.
//------------------------------------------------------------------------
float etTerrain::GetScaledHeightValue( float x,float z ) const
{
    int i1,j1,i2,j2;
    float f1,f2,f3,f4;
    float f12,f34;

    float fSize = m_nTerrainSize * m_fLengthPerUnit;

    // Calculate the nearest coords.
    i1 = (int)(x / m_fLengthPerUnit);
    i2 = i1 + 1;
    j1 = (int)(z / m_fLengthPerUnit);
    j2 = j1 + 1;

    if( x < 0 || x > fSize || z < 0 || z > fSize )
        return -1.0f;
    else
    {
        // If on the right line.
        if( fabs(x-fSize) <= 0.0f )
        {
            // If on the top line. (So it's the right_top point.)
            // Return the right_top height.
            if( fabs(z-fSize) <= 0.0f )
            {
                return m_pHeightData[j1*(m_nTerrainSize+1)+i1] * m_fHeightScale;
            }

            // If just on the right line.
            // not the right_top point.
            else
            {
                f1 = m_pHeightData[j1*(m_nTerrainSize+1)+i1] * m_fHeightScale;
                f2 = m_pHeightData[j2*(m_nTerrainSize+1)+i1] * m_fHeightScale;
                return f1 + (f2-f1) * (z/m_fLengthPerUnit-j1);
            }
        }

        // Just on the top line.
        else if( fabs(z-fSize) <= 0.0f )
        {
            f1 = m_pHeightData[j1*(m_nTerrainSize+1)+i1] * m_fHeightScale;
            f2 = m_pHeightData[j1*(m_nTerrainSize+1)+i2] * m_fHeightScale;
            return f1 + (f2-f1) * (x/m_fLengthPerUnit-i1);
        }

        // Inner of the terrain.
        else
        {
            f1 = m_pHeightData[j1*(m_nTerrainSize+1)+i1] * m_fHeightScale;
            f2 = m_pHeightData[j1*(m_nTerrainSize+1)+i2] * m_fHeightScale;
            f3 = m_pHeightData[j2*(m_nTerrainSize+1)+i1] * m_fHeightScale;
            f4 = m_pHeightData[j2*(m_nTerrainSize+1)+i2] * m_fHeightScale;
            f12 = f1 + (f2-f1) * (x/m_fLengthPerUnit-i1);
            f34 = f3 + (f4-f1) * (x/m_fLengthPerUnit-i1);
            return f12 + (f34-f12) * (z/m_fLengthPerUnit-j1);
        }
    }
}


//------------------------------------------------------------------------
// Get the normal of the specified position.
//------------------------------------------------------------------------
void etTerrain::GetNormalAt( int x,int z,FlyVector* pOut ) const
{
    FlyVector vHere,vRight,vUp,vOut;

    assert( x>=0 && x<m_nTerrainSize &&
            z>=0 && z<m_nTerrainSize &&
            "X or z is out of the terrain size!" );

    vHere.x = x * m_fLengthPerUnit;
    vHere.y = m_pHeightData[z*(m_nTerrainSize+1)+x]*m_fHeightScale;
    vHere.z = z * m_fLengthPerUnit;

    vRight.x = (x + 1) * m_fLengthPerUnit;
    vRight.y = m_pHeightData[z*(m_nTerrainSize+1)+x+1]*m_fHeightScale;
    vRight.z = z * m_fLengthPerUnit;

    vUp.x = x * m_fLengthPerUnit;
    vUp.y = m_pHeightData[(z+1)*(m_nTerrainSize+1)+x]*m_fHeightScale;
    vUp.z = (z + 1) * m_fLengthPerUnit;

    vRight = vRight - vHere;
    vUp = vUp - vHere;
    vRight.Normalize();
    vUp.Normalize();

    vOut.Cross( vUp,vRight );
    *pOut = vOut;
}


//------------------------------------------------------------------------
// Set the height value for the terrain.
//------------------------------------------------------------------------
void etTerrain::SetHeightValue( int x,int z,float fHeight )
{
    if( x < 0 || x >= (m_nTerrainSize+1) ||
        z < 0 || z >= (m_nTerrainSize+1) )
        return;

    m_pHeightData[z*(m_nTerrainSize+1)+x] = fHeight;
}


//------------------------------------------------------------------------
// Get the name of the terrain level.
//------------------------------------------------------------------------
const char* etTerrain::GetName(void) const
{
    return m_sName.c_str();
}

//------------------------------------------------------------------------
// Set the name for the terrain level.
//------------------------------------------------------------------------
void etTerrain::SetName( const char* cName )
{
    m_sName = cName;
}


//------------------------------------------------------------------------
// Test the intersecting with the segment.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   Start point of the segment.
//      const FlyVector&:   End point of the segment.
//      FlyVector*:         To store the intersecting point.
//------------------------------------------------------------------------
bool etTerrain::IntersectSegment( const FlyVector& vStart,const FlyVector& vEnd,
                                  FlyVector* pOut )
{
    FlyVector vDir = vEnd - vStart;
    FlyVector vRay = vStart;

    // 1. Special case. When the segment perpendicular 
    // to the plane.
    if( vDir.x == 0 && vDir.z == 0 )
    {
        float h = GetScaledHeightValue( vRay.x,vRay.z );

        if( (vStart.y <= h && vEnd.y >= h) ||
            (vStart.y >= h && vEnd.y <= h) )
        {
            if( pOut )
            {
                *pOut = vRay;
                pOut->y = h;
            }

            return true;
        }
        else
        {
            if( pOut ) *pOut = FlyVector( -1.0f,-1.0f,-1.0f );

            return false;
        }
    }

    vDir.Normalize();
    vRay += vDir;

    while( true )
    {
        float h = GetScaledHeightValue( vRay.x,vRay.z );

        if( vDir.y < 0 )
        {
            if( vRay.y <= h )
            {
                if( pOut ) *pOut = vRay;

                return true;
            }
            else
                vRay += vDir;

            if( vRay.y - vEnd.y < 1.0f )
                break;
        }
        else
        {
            if( vRay.y >= h )
            {
                if( pOut ) *pOut = vRay;

                return true;
            }
            else
                vRay += vDir;

            if( vRay.y - vEnd.y > -1.0f )
                break;
        }
    }

    return false;
}


//------------------------------------------------------------------------
// Add an indexBuffer for tiles to the list.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyGraphicBuffer*:  Pointer to the indexBuffer.
//      UINT:               Count of the indices.
//      int:                LodLevel of the indexBuffer.
//      UINT:               Stitch flag.
//------------------------------------------------------------------------
void etTerrain::AddIndexBuffer( FlyGraphicBuffer* pIB,UINT nNumIndices,int nLodLevel,
                                UINT nStitchFlags )
{
    IndexBuffer buffer;
    buffer.pIB = pIB;
    buffer.nNumIndices = nNumIndices;

    m_IndexData[nLodLevel].insert( IndexBufferMap::value_type(
        nStitchFlags,buffer) );
}


//------------------------------------------------------------------------
// Get the specified indexBuffer for the tile.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:                LodLevel of the indexBuffer.
//      UINT:               Stitch flag.
//      FlyGraphicBuffer**: To store the indexBuffer's pointer.
//      UINT*:              To store the count of indices.
//------------------------------------------------------------------------
void etTerrain::GetIndexBuffer( int nLodLevel,UINT nStitchFlags,
                                FlyGraphicBuffer** ppIB,UINT* pNumIndices )
{
    IndexBufferMap::iterator itr;
    itr = m_IndexData[nLodLevel].find( nStitchFlags );

    if( itr != m_IndexData[nLodLevel].end() )
    {
        *ppIB = itr->second.pIB;
        *pNumIndices = itr->second.nNumIndices;
    }
    else
    {
        *ppIB = NULL;
        *pNumIndices = 0;
    }
}


//------------------------------------------------------------------------
// Get the terrain tile at the specified point.
//------------------------------------------------------------------------
etTile* etTerrain::GetTerrainTileAt( const FlyVector& vPoint )
{
    if( vPoint.x < 0.0f || 
        vPoint.x >= m_nTerrainSize*m_fLengthPerUnit ||
        vPoint.z < 0.0f ||
        vPoint.z >= m_nTerrainSize*m_fLengthPerUnit )
    {
        return NULL;
    }

    int nX = (int)(vPoint.x / m_fLengthPerUnit);
    int nZ = (int)(vPoint.z / m_fLengthPerUnit);
    int nTileX = nX / m_nTileSize;
    int nTileZ = nZ / m_nTileSize;

    int nSize = m_nTerrainSize / m_nTileSize;
    if( nTileX == nSize ) nTileX--;
    if( nTileZ == nSize ) nTileZ--;

    return &m_pTerrainTiles[nTileZ*nSize+nTileX];
}


//------------------------------------------------------------------------
// Select the specified terrain tile.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      X- coordinate of the brush's position.
//      float:      Z- coordinate of the brush's position.
//------------------------------------------------------------------------
void etTerrain::SelectTile( float fX,float fZ )
{
    int nX = (int)(fX / m_fLengthPerUnit);
    int nZ = (int)(fZ / m_fLengthPerUnit);

    int nTileX = nX / m_nTileSize;
    int nTileZ = nZ / m_nTileSize;
    int nSize = m_nTerrainSize / m_nTileSize;

    if( nTileX < 0 || nTileX >= nSize ||
        nTileZ < 0 || nTileZ >= nSize )
        return;

    if( m_pTileSelected ) m_pTileSelected->SetSelected( false );
    m_pTileSelected = &m_pTerrainTiles[nTileZ*nSize+nTileX];
    m_pTileSelected->SetSelected( true );
}