//------------------------------------------------------------------------
// Name: FlyTerrainLevel.cpp
// Desc: This file is to implement the class FlyTerrainLevel.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyTerrainLevel.h"

#include "FlyStringUtil.h"
#include "FlyKernel.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyTerrainLevel::FlyTerrainLevel(void)
{
    m_nTerrainSize = 0;
    m_pHeightData = NULL;
    m_nTileSize = 0;
    m_fHeightScale = 1.0f;
    m_fLengthPerUnit = 1.0f;

    m_pAlphamap = NULL;
    m_pLightmap = NULL;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyTerrainLevel::~FlyTerrainLevel(void)
{
    SAFE_DELETE_ARRAY( m_pHeightData );

    // Get the pointer to the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    pResMgr->DeleteTexture( m_pAlphamap );
    pResMgr->DeleteTexture( m_pLightmap );

    // Destroy all the textures.
    TextureList::iterator itr;
    for( itr=m_Textures.begin();itr!=m_Textures.end();itr++ )
        pResMgr->DeleteTexture( *itr );
}


//------------------------------------------------------------------------
// Initialize the level data, Call this function when user want to 
// generate the level data by himself.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Size of the terrain (In vertices).
//      UINT:       Size of the tile (In vertices).
//      UINT:       Size of the alphamap.
//      UINT:       Size of the lightmap.
//------------------------------------------------------------------------
HRESULT FlyTerrainLevel::Initialize( UINT nTerrainSize,UINT nTileSize,UINT nAlphaMapSize,
                                     UINT nLightMapSize )
{
    HRESULT hr;
    char cTemp[80];

    m_nTerrainSize = nTerrainSize;
    m_nTileSize = nTileSize;

    // Create a buffer to store height data.
    m_pHeightData = new short[(nTerrainSize+1)*(nTerrainSize+1)];
    if( !m_pHeightData ) return FLY_OUTOFMEMORY;

    // Get the pointer to the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    // Create an alphamap for terrain.
    sprintf( cTemp,"%s:Alphamap",m_sName.c_str() );
    m_pAlphamap = pResMgr->AddNullTexture( cTemp );
    hr = m_pAlphamap->Create2DTexture( nAlphaMapSize,nAlphaMapSize,TU_DEFAULT,PFT_A8R8G8B8,1 );
    if( FAILED(hr) )
    {
        pResMgr->DeleteTexture( m_pAlphamap );
        return hr;
    }

    // Create an lightmap for terrain.
    sprintf( cTemp,"%s:Lightmap",m_sName.c_str() );
    m_pLightmap = pResMgr->AddNullTexture( cTemp );
    hr = m_pLightmap->Create2DTexture( nLightMapSize,nLightMapSize,TU_DEFAULT,PFT_X8R8G8B8,1 );
    if( FAILED(hr) )
    {
        pResMgr->DeleteTexture( m_pLightmap );
        return hr;
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Destroy the level, clean the memory.
//------------------------------------------------------------------------
void FlyTerrainLevel::DestroyLevel(void)
{
    SAFE_DELETE_ARRAY( m_pHeightData );

    // Get the pointer to the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    pResMgr->DeleteTexture( m_pAlphamap );
    pResMgr->DeleteTexture( m_pLightmap );

    // Destroy all the textures.
    TextureList::iterator itr;
    for( itr=m_Textures.begin();itr!=m_Textures.end();itr++ )
        pResMgr->DeleteTexture( *itr );

    m_nTerrainSize = 0;
    m_nTileSize = 0;
    m_pHeightData = NULL;
    m_fHeightScale = 1.0f;
    m_fLengthPerUnit = 1.0f;
}


//------------------------------------------------------------------------
// Load the terrain level from the map file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the terrain map file.
//------------------------------------------------------------------------
HRESULT FlyTerrainLevel::LoadTerrainMap( const char* cFilename )
{
    std::string str;
    char cIdentifier[4];
    DWORD dwVersion;
    HRESULT hr;

    FILE* pFile = fopen( cFilename,"rb" );
    if( !pFile ) return FLY_FILENOTFOUND;

    cIdentifier[0] = fgetc( pFile );
    cIdentifier[1] = fgetc( pFile );
    cIdentifier[2] = fgetc( pFile );
    cIdentifier[3] = fgetc( pFile );
    fread( &dwVersion,sizeof(DWORD),1,pFile );

    if( strcmp(cIdentifier,"MAP") != 0 ||
        dwVersion != FLY_VERSION(2,0) )
        return FLY_INVALIDVERSION;

    // Destroy the old data.
    DestroyLevel();

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    // 1. Read the terrain info.
    fread( &m_nTerrainSize,sizeof(UINT),1,pFile );
    fread( &m_nTileSize,sizeof(UINT),1,pFile );
    fread( &m_fHeightScale,sizeof(float),1,pFile );
    fread( &m_fLengthPerUnit,sizeof(float),1,pFile );

    // 2. Read the height data.
    m_pHeightData = new short[(m_nTerrainSize+1)*(m_nTerrainSize+1)];
    if( !m_pHeightData ) return FLY_OUTOFMEMORY;
    fread( m_pHeightData,sizeof(short),(m_nTerrainSize+1)*(m_nTerrainSize+1),pFile );

    // 3. Read the alphaMap data.
    void* pBits;
    int nPitch,nSize;
    fread( &nSize,sizeof(int),1,pFile );
    str = m_sName + ":Alphamap";
    m_pAlphamap = pResMgr->AddNullTexture( str.c_str() );
    hr = m_pAlphamap->Create2DTexture( nSize,nSize,TU_DEFAULT,PFT_A8R8G8B8,1 );
    if( FAILED(hr) ) return hr;
    m_pAlphamap->Access2DMap( 0,TMA_WRITEONLY,&pBits,&nPitch );
    fread( pBits,nSize*nSize,4,pFile );
    m_pAlphamap->Unaccess2DMap( 0 );

    // 4. Read the lightMap data.
    str = m_sName + ":Lightmap";
    m_pLightmap = pResMgr->AddNullTexture( str.c_str() );
    hr = m_pLightmap->Create2DTexture( nSize,nSize,TU_DEFAULT,PFT_X8R8G8B8,1 );
    if( FAILED(hr) ) return hr;
    fread( &nSize,sizeof(int),1,pFile );
    BYTE* pTemp = new BYTE[nSize*nSize*3];
    if( !pTemp ) return FLY_OUTOFMEMORY;
    fread( pTemp,3,nSize*nSize,pFile );
    m_pLightmap->Access2DMap( 0,TMA_WRITEONLY,&pBits,&nPitch );
    for( int y=0;y<nSize;y++ )
    {
        for( int x=0;x<nSize;x++ )
        {
            static_cast<DWORD*>(pBits)[y*nSize+x] = MAKE3DCOLOR( 
                255,pTemp[(y*nSize+x)*3+0],pTemp[(y*nSize+x)*3+1],
                pTemp[(y*nSize+x)*3+2]);
        }
    }
    m_pLightmap->Unaccess2DMap( 0 );
    delete[] pTemp;

    // 5. Read the textures.
    fread( &nSize,sizeof(int),1,pFile );
    for( int i=0;i<nSize;i++ )
    {
        size_t nLen;
        fread( &nLen,sizeof(size_t),1,pFile );
        str.resize( nLen,'\0' );
        fread( &str[0],nLen,1,pFile );

        std::string realPath;
        realPath = FlyStringUtil::GetDirFromPath( cFilename );
        realPath += str;
        CreateTexture( realPath.c_str() );
    }

    // 6. Read the tile data.
    fread( &nSize,sizeof(int),1,pFile );
    m_pTiles = new TileInfo[nSize];
    if( !m_pTiles ) return FLY_OUTOFMEMORY;
    for( int i=0;i<nSize;i++ )
        fread( &m_pTiles[i],sizeof(TileInfo),1,pFile );

    fclose( pFile );
    return FLY_OK;
}


//------------------------------------------------------------------------
// Save the terrain level data to the specified map file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the terrain map file.
//------------------------------------------------------------------------
HRESULT FlyTerrainLevel::SaveTerrainMap( const char* cFilename )
{
    DWORD dwVersion;

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
    fwrite( m_pHeightData,sizeof(short),(m_nTerrainSize+1)*(m_nTerrainSize+1),pFile );

    // 3. Write the alphamap data.
    size_t nSize;
    int nPitch;
    void* pBits;
    nSize = m_pAlphamap->GetWidth( 0 );
    fwrite( &nSize,sizeof(int),1,pFile );
    m_pAlphamap->Access2DMap( 0,TMA_READONLY,&pBits,&nPitch );
    fwrite( pBits,nSize*nSize,4,pFile );
    m_pAlphamap->Unaccess2DMap( 0 );

    // 4. Write the lightmap data.
    nSize = m_pLightmap->GetWidth( 0 );
    m_pLightmap->Access2DMap( 0,TMA_READONLY,&pBits,&nPitch );
    BYTE* pTemp = new BYTE[nSize*nSize*3];
    for( size_t y=0;y<nSize;y++ )
    {
        for( size_t x=0;x<nSize;x++ )
        {
            DWORD dw = static_cast<DWORD*>(pBits)[y*nSize+x];
            pTemp[(y*nSize+x)+0] = (dw & 0x00ff0000) >> 16;
            pTemp[(y*nSize+x)+1] = (dw & 0x0000ff00) >> 8;
            pTemp[(y*nSize+x)+2] = (dw & 0x000000ff);
        }
    }
    m_pLightmap->Unaccess2DMap( 0 );
    fwrite( &nSize,sizeof(int),1,pFile );
    fwrite( pTemp,3,nSize*nSize,pFile );
    delete[] pTemp;

    // Write the texture data.
    nSize = m_Textures.size();
    fwrite( &nSize,sizeof(int),1,pFile );
    for( size_t i=0;i<nSize;i++ )
    {
        std::string path;
        std::string texture;
        path = FlyStringUtil::GetDirFromPath( m_sName.c_str() );
        texture = m_Textures[i]->GetName();
        size_t nIndex = texture.size() - path.size();
        texture[nIndex] = '\0';
        strrev( &texture[0] );
        nIndex = texture.size();
        fwrite( &nIndex,sizeof(size_t),1,pFile );
        fwrite( texture.c_str(),nIndex,1,pFile );
    }

    // Write the tile data.
    nSize = m_nTerrainSize / m_nTileSize;
    nSize *= nSize;
    fwrite( &nSize,sizeof(int),1,pFile );
    fwrite( m_pTiles,sizeof(TileInfo),nSize,pFile );

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
HRESULT FlyTerrainLevel::LoadHeightMap( const char* cFilename,int nSize )
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
            m_pHeightData[j*(nSize+1)+i] = pBuffer[j*(nSize+1)+i];
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
HRESULT FlyTerrainLevel::SaveHeightMap( const char* cFilename )
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

    for( UINT j=0;j<m_nTerrainSize+1;j++ )
    {
        for( UINT i=0;i<m_nTerrainSize+1;i++ )
        {
            short h = m_pHeightData[j*(m_nTerrainSize+1)+i];

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
// Load the alphamap data from the image.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the image file.
//------------------------------------------------------------------------
HRESULT FlyTerrainLevel::LoadAlphaMap( const char* cFilename )
{
    // Get the pointer to the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    m_pAlphamap->Load2DTexture( cFilename,1 );
    if( !m_pAlphamap ) return FLY_FAIL;

    if( m_pAlphamap->GetFormat() != PFT_A8R8G8B8 )
    {
        pResMgr->DeleteTexture( m_pAlphamap );
        m_pAlphamap = NULL;
        return FLY_UNKNOWNFORMAT;
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Save the alphamap to the image file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the image file.
//------------------------------------------------------------------------
HRESULT FlyTerrainLevel::SaveAlphaMap( const char* cFilename )
{
    if( m_pAlphamap )
        return m_pAlphamap->Save2DTexture( cFilename,FIMAGE_BMP );
    else
        return FLY_FAIL;
}


//------------------------------------------------------------------------
// Load the lightmap from the image file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the image file.
//------------------------------------------------------------------------
HRESULT FlyTerrainLevel::LoadLightMap( const char* cFilename )
{
    // Get the pointer to the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    m_pLightmap->Load2DTexture( cFilename,1 );
    if( !m_pLightmap ) return FLY_FAIL;

    if( m_pLightmap->GetFormat() != PFT_X8R8G8B8 )
    {
        pResMgr->DeleteTexture( m_pLightmap );
        return FLY_UNKNOWNFORMAT;
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Save the lightmap to the image file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the image file.
//------------------------------------------------------------------------
HRESULT FlyTerrainLevel::SaveLightMap( const char* cFilename )
{
    if( m_pLightmap )
        return m_pLightmap->Save2DTexture( cFilename,FIMAGE_BMP );
    else
        return FLY_FAIL;
}


//------------------------------------------------------------------------
// Create a new texture for the terrain level.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the texture file.
//------------------------------------------------------------------------
FlyTexture* FlyTerrainLevel::CreateTexture( const char* cFilename )
{
    FlyTexture* pTexture;

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    pTexture = pResMgr->Load2DTexture( cFilename,0 );
    m_Textures.push_back( pTexture );
    return pTexture;
}


//------------------------------------------------------------------------
// Get the height value of specified position.
//------------------------------------------------------------------------
short FlyTerrainLevel::GetHeightValue( UINT x,UINT z ) const
{
    if( x >= (m_nTerrainSize+1) ||
        z >= (m_nTerrainSize+1) )
    {
        return -1;
    }

    return m_pHeightData[z*(m_nTerrainSize+1)+x];
}


//------------------------------------------------------------------------
// Get the scaled height value of specified position.
//------------------------------------------------------------------------
float FlyTerrainLevel::GetScaledHeightValue( float x,float z ) const
{
    int i1,j1,i2,j2;
    float f1,f2,f3,f4;
    float f12,f34;

    float fExp = 0.0001f;
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
        if( fabs(x-fSize) <= fExp )
        {
            // If on the top line. (So it's the right_top point.)
            // Return the right_top height.
            if( fabs(z-fSize) <= fExp )
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
        else if( fabs(z-fSize) <= fExp )
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
void FlyTerrainLevel::GetNormalAt( float x,float z,FlyVector* pOut ) const
{
    FlyVector vHere,vRight,vUp,vOut;

    vHere.x = x;
    vHere.y = GetHeightValue( x,z );
    vHere.z = z;

    vRight.x = x + 1;
    vRight.y = GetHeightValue( x+1,z );
    vRight.z = z;

    vUp.x = x;
    vUp.y = GetHeightValue( x,z+1 );
    vUp.z = z + 1;

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
void FlyTerrainLevel::SetHeightValue( UINT x,UINT z,short nHeight )
{
    if( x >= (m_nTerrainSize+1) ||
        z >= (m_nTerrainSize+1) )
        return;

    m_pHeightData[z*(m_nTerrainSize+1)+x] = nHeight;
}


//------------------------------------------------------------------------
// Get the specified texture.
// ----------------------------------------------------------------------
// Param -> IN:
//      size_t:     Index of the texture.
//------------------------------------------------------------------------
FlyTexture* FlyTerrainLevel::GetTexture( size_t nIndex )
{
    if( nIndex >= m_Textures.size() )
        return NULL;

    return m_Textures[nIndex];
}


//------------------------------------------------------------------------
// Get the specified tile info.
//------------------------------------------------------------------------
const FlyTerrainLevel::TileInfo* FlyTerrainLevel::GetTileInfo( UINT x,UINT z ) const
{
    UINT nNumTiles = m_nTerrainSize / m_nTileSize;

    if( x >= nNumTiles ||
        z >= nNumTiles )
        return NULL;

    return &m_pTiles[z*nNumTiles+x];
}


//------------------------------------------------------------------------
// Get the name of the terrain level.
//------------------------------------------------------------------------
const char* FlyTerrainLevel::GetName(void)
{
    return m_sName.c_str();
}

//------------------------------------------------------------------------
// Set the name for the terrain level.
//------------------------------------------------------------------------
void FlyTerrainLevel::SetName( const char* cName )
{
    m_sName = cName;
}