//------------------------------------------------------------------------
// Name: etDetailLayer.cpp
// Desc: This file is to implement the class etDetailLayer.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etDetailLayer.h"

#include "FlyKernel.h"
#include "FlyImageCodec.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etDetailLayer::etDetailLayer(void)
{
    m_nMapSize = 0;
    m_pTempData = NULL;
    m_pTexture = NULL;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etDetailLayer::~etDetailLayer(void)
{
    SAFE_DELETE_ARRAY( m_pTempData );

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    if( m_pTexture ) pResMgr->DeleteTexture( m_pTexture );
}


//------------------------------------------------------------------------
// Initialize the detail layer.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:           Size of the alphamap.
//------------------------------------------------------------------------
HRESULT etDetailLayer::Initialize( UINT nMapSize )
{
    HRESULT hr;
    void* pBits;
    int nPitch;

    // Clean the old data.
    DestroyLayer();

    // Create the temp buffer.
    m_pTempData = new UCHAR[nMapSize*nMapSize*4];
    if( !m_pTempData ) return FLY_OUTOFMEMORY;

    m_nMapSize = nMapSize;
    memset( m_pTempData,0,nMapSize*nMapSize*4 );

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    m_pTexture = pResMgr->AddNullTexture( "et-DetailLayer" );
    if( !m_pTexture ) return FLY_OUTOFMEMORY;

    // Create the texture for detail layer.
    hr = m_pTexture->Create2DTexture( nMapSize,nMapSize,TU_DEFAULT,PFT_A8R8G8B8,1 );
    if( FAILED(hr) ) return hr;
    m_pTexture->Access2DMap( 0,TMA_WRITEONLY,&pBits,&nPitch );
    memcpy( pBits,m_pTempData,nMapSize*nMapSize*4 );
    m_pTexture->Unaccess2DMap( 0 );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Destroy the detail layer, clean the memory.
//------------------------------------------------------------------------
void etDetailLayer::DestroyLayer(void)
{
    SAFE_DELETE_ARRAY( m_pTempData );

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    if( m_pTexture ) pResMgr->DeleteTexture( m_pTexture );

    m_nMapSize = 0;
    m_pTexture = NULL;
}


//------------------------------------------------------------------------
// Load the data from an image file. (BMP File)
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the image file.
//------------------------------------------------------------------------
HRESULT etDetailLayer::LoadLayerFromImage( const char* cFilename )
{
    HRESULT hr;
    FlyImageCodec codec;
    FLYIMAGEDATA data;
    void* pBits;
    int nPitch;

    // Load the image file.
    hr = codec.LoadImage( cFilename,&data );
    if( FAILED(hr) ) return hr;

    if( data.Format != PFT_A8R8G8B8 ||
        data.nWidth != data.nHeight )
    {
        SAFE_DELETE_ARRAY( data.pData );
        return FLY_INVALIDFILE;
    }

    if( m_nMapSize != data.nWidth )
    {
        m_nMapSize = data.nWidth;
        SAFE_DELETE_ARRAY( m_pTempData );

        m_pTempData = new UCHAR[m_nMapSize*m_nMapSize*4];
        if( !m_pTempData )
        {
            SAFE_DELETE_ARRAY( data.pData );
            return FLY_OUTOFMEMORY;
        }

        // Rebuild the texture.
        hr = m_pTexture->Create2DTexture( m_nMapSize,m_nMapSize,TU_DEFAULT,
            PFT_A8R8G8B8,1 );
        if( FAILED(hr) )
        {
            SAFE_DELETE_ARRAY( data.pData );
            return hr;
        }
    }

    // Copy the texture data to temp buffer.
    memcpy( m_pTempData,data.pData,m_nMapSize*m_nMapSize*4 );

    // Send the temp data to texture.
    m_pTexture->Access2DMap( 0,TMA_WRITEONLY,&pBits,&nPitch );
    memcpy( pBits,m_pTempData,m_nMapSize*m_nMapSize*4 );
    m_pTexture->Unaccess2DMap( 0 );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Save the data to an image file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the image file.
//------------------------------------------------------------------------
HRESULT etDetailLayer::SaveLayerToImage( const char* cFilename )
{
    return m_pTexture->Save2DTexture( cFilename,FIMAGE_BMP );
}


//------------------------------------------------------------------------
// Get alpha value at the specified position.
//------------------------------------------------------------------------
UCHAR etDetailLayer::GetAlphaValue( int x,int y,int nLayer )
{
    if( x < 0 || x >= m_nMapSize ||
        y < 0 || y >= m_nMapSize ||
        nLayer < 0 || nLayer > 3 )
        return 0;
    else
        return m_pTempData[(y*m_nMapSize+x)*4+nLayer];
}


//------------------------------------------------------------------------
// Set the alpha value at the specified position.
//------------------------------------------------------------------------
void etDetailLayer::SetAlphaValue( int x,int y,int nDetail,UCHAR ucValue )
{
    if( x < 0 || x >= m_nMapSize ||
        y < 0 || y >= m_nMapSize ||
        nDetail < 0 || nDetail > 3 )
        return;

    m_pTempData[(y*m_nMapSize+x)*4+nDetail] = ucValue;
}


//------------------------------------------------------------------------
// Update the texture data for the detail layer.
//------------------------------------------------------------------------
void etDetailLayer::UpdateTextureData(void)
{
    int nPitch;
    void* pBits;

    if( m_bModified )
    {
        m_pTexture->Access2DMap( 0,TMA_WRITEONLY,&pBits,&nPitch );
        memcpy( pBits,m_pTempData,m_nMapSize*m_nMapSize*4 );
        m_pTexture->Unaccess2DMap( 0 );

        m_bModified = false;
    }
}