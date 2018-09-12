//------------------------------------------------------------------------
// Name: etLightLayer.cpp
// Desc: This file is to implement the class etLightmap.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etLightLayer.h"
#include "etCoreManager.h"
#include "etSceneManager.h"

#include "FlyImageCodec.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etLightLayer::etLightLayer(void)
{
    m_vDirection.Set( 1.0f,-1.0f,1.0f );
    m_cAmbient = FLYCOLOR::Black();
    m_cDiffuse = FLYCOLOR::White();
    m_nMapSize = 0;
    m_pTempData = NULL;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etLightLayer::~etLightLayer(void)
{
    SAFE_DELETE_ARRAY( m_pTempData );

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    if( m_pTexture ) pResMgr->DeleteTexture( m_pTexture );
}


//------------------------------------------------------------------------
// Destroy the light layer.
//------------------------------------------------------------------------
void etLightLayer::DestroyLayer(void)
{
    SAFE_DELETE_ARRAY( m_pTempData );

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    if( m_pTexture ) pResMgr->DeleteTexture( m_pTexture );

    m_nMapSize = 0;
    m_pTexture = NULL;
}


//------------------------------------------------------------------------
// Initialize the lightmap for the terrain.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:   Size of the lightmap.
//------------------------------------------------------------------------
HRESULT etLightLayer::Initialize( UINT nMapSize )
{
    HRESULT hr;
    void* pBits;
    int nPitch;

    // Clean the old data.
    DestroyLayer();

    // Create the temp buffer.
    m_pTempData = new DWORD[nMapSize*nMapSize];
    if( !m_pTempData ) return FLY_OUTOFMEMORY;

    m_nMapSize = nMapSize;
    memset( m_pTempData,0,nMapSize*nMapSize*sizeof(DWORD) );

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    m_pTexture = pResMgr->AddNullTexture( "et-LightLayer" );
    if( !m_pTexture ) return FLY_OUTOFMEMORY;

    // Create the texture for detail layer.
    hr = m_pTexture->Create2DTexture( nMapSize,nMapSize,TU_DEFAULT,PFT_X8R8G8B8,1 );
    if( FAILED(hr) ) return hr;
    m_pTexture->Access2DMap( 0,TMA_WRITEONLY,&pBits,&nPitch );
    memcpy( pBits,m_pTempData,nMapSize*nMapSize*4 );
    m_pTexture->Unaccess2DMap( 0 );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Load the light data from the image file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the image file.
//------------------------------------------------------------------------
HRESULT etLightLayer::ImportLightmap( const char* cFilename )
{
    HRESULT hr;
    FlyImageCodec codec;
    FLYIMAGEDATA data;
    void* pBits;
    int nPitch;

    // Load the image file.
    hr = codec.LoadImage( cFilename,&data );
    if( FAILED(hr) ) return hr;

    if( data.Format != PFT_R8G8B8 ||
        data.nWidth != data.nHeight )
    {
        SAFE_DELETE_ARRAY( data.pData );
        return FLY_INVALIDFILE;
    }

    if( m_nMapSize != data.nWidth )
    {
        m_nMapSize = data.nWidth;
        SAFE_DELETE_ARRAY( m_pTempData );

        m_pTempData = new DWORD[m_nMapSize*m_nMapSize];
        if( !m_pTempData )
        {
            SAFE_DELETE_ARRAY( data.pData );
            return FLY_OUTOFMEMORY;
        }

        // Rebuild the texture.
        hr = m_pTexture->Create2DTexture( m_nMapSize,m_nMapSize,TU_DEFAULT,
            PFT_X8R8G8B8,1 );
        {
            SAFE_DELETE_ARRAY( data.pData );
            return hr;
        }
    }

    // Copy the texture data to temp buffer.
    for( int j=0;j<m_nMapSize;j++ )
    {
        for( int i=0;i<m_nMapSize;i++ )
        {
            UCHAR r = data.pData[(j*m_nMapSize+i)+0];
            UCHAR g = data.pData[(j*m_nMapSize+i)+1];
            UCHAR b = data.pData[(j*m_nMapSize+i)+2];
            m_pTempData[j*m_nMapSize+i] = MAKE3DCOLOR( r,g,b,255 );
        }
    }

    // Send the temp data to texture.
    m_pTexture->Access2DMap( 0,TMA_WRITEONLY,&pBits,&nPitch );
    memcpy( pBits,m_pTempData,m_nMapSize*m_nMapSize*sizeof(DWORD) );
    m_pTexture->Unaccess2DMap( 0 );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Save the light data to the image file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the image file.
//------------------------------------------------------------------------
HRESULT etLightLayer::ExportLightmap( const char* cFilename )
{
    return m_pTexture->Save2DTexture( cFilename,FIMAGE_BMP );
}


//------------------------------------------------------------------------
// Generate the lighting data for the editor.
//------------------------------------------------------------------------
void etLightLayer::GenerateLightData(void)
{
    FLYCOLOR clr;
    FlyVector v1,v2,v3;
    FlyVector vNormal;

    // Get the pointer to the terrain.
    etTerrain* pTerrain = etCoreManager::Instance().GetSceneManager()
        ->GetTerrainLevel();
    float fScale = (float)pTerrain->GetTerrainSize() / m_nMapSize;

    FlyVector vLightDir = m_vDirection;
    vLightDir.Normalize();
    for( int y=0;y<m_nMapSize+1;y++ )
    {
        for( int x=0;x<m_nMapSize+1;x++ )
        {
            v1.Set( (float)x*fScale,pTerrain->GetScaledHeightValue((float)x*fScale,(float)y*fScale),
                (float)y*fScale );
            v2.Set( (float)(x+1)*fScale,pTerrain->GetScaledHeightValue((float)(x+1)*fScale,(float)y*fScale),
                (float)y*fScale );
            v3.Set( (float)x*fScale,pTerrain->GetScaledHeightValue((float)x*fScale,(float)(y+1)*fScale),
                (float)(y+1)*fScale );
            vNormal.Cross( v2-v1,v3-v1 );
            vNormal.Normalize();
            clr = m_cAmbient + m_cDiffuse*max(0,vLightDir*vNormal);
            m_pTempData[y*m_nMapSize+x] = clr.GetRGBAValue();
        }
    }

    // Send the temp data to texture.
    m_pTexture->Access2DMap( 0,TMA_WRITEONLY,&pBits,&nPitch );
    memcpy( pBits,m_pTempData,m_nMapSize*m_nMapSize*sizeof(DWORD) );
    m_pTexture->Unaccess2DMap( 0 );
}