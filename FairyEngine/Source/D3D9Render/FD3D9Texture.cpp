//------------------------------------------------------------------------
// Name: FlyD3DTexture.cpp
// Desc: This file is to implement the class FlyD3DTexture.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyD3DTexture.h"
#include "FlyD3DUtil.h"

#include "FlyStringUtil.h"
#include "FlyImage.h"
#include "FlyImageCodec.h"
#include "FlyLogManager.h"


//------------------------------------------------------------------------
// Constructor of the class
// ----------------------------------------------------------------------
// Param -> IN:
//      LPDIRECT3DDEVICE9:      Pointer to the Direct3D device.
//------------------------------------------------------------------------
FlyD3DTexture::FlyD3DTexture( LPDIRECT3DDEVICE9 pd3dDevice )
{
    m_pd3dDevice    = pd3dDevice;
    m_pBaseTexture  = NULL;

    m_Type          = TT_2D;
    m_Usage         = TU_DEFAULT;
    m_Format        = PFT_A8R8G8B8;
    m_nMipLevels    = 0;
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
FlyD3DTexture::~FlyD3DTexture(void)
{
    LPDIRECT3DTEXTURE9 pTexture;
    LPDIRECT3DVOLUMETEXTURE9 pVolTexture;
    LPDIRECT3DCUBETEXTURE9 pCubeTexture;

    switch( m_Type )
    {
    case TT_2D:
        pTexture = (LPDIRECT3DTEXTURE9)m_pBaseTexture;
        SAFE_RELEASE( pTexture );
        break;
    case TT_3D:
        pVolTexture = (LPDIRECT3DVOLUMETEXTURE9)m_pBaseTexture;
        SAFE_RELEASE( pVolTexture );
        break;
    case TT_CUBEMAP:
        pCubeTexture = (LPDIRECT3DCUBETEXTURE9)m_pBaseTexture;
        SAFE_RELEASE( pCubeTexture );
        break;
    default: assert( false );
    }
}


//------------------------------------------------------------------------
// Reset the content for the texture.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::ResetContent(void)
{
    LPDIRECT3DTEXTURE9 pTexture;
    LPDIRECT3DVOLUMETEXTURE9 pVolTexture;
    LPDIRECT3DCUBETEXTURE9 pCubeTexture;

    switch( m_Type )
    {
    case TT_2D:
        pTexture = (LPDIRECT3DTEXTURE9)m_pBaseTexture;
        SAFE_RELEASE( pTexture );
        break;
    case TT_3D:
        pVolTexture = (LPDIRECT3DVOLUMETEXTURE9)m_pBaseTexture;
        SAFE_RELEASE( pVolTexture );
        break;
    case TT_CUBEMAP:
        pCubeTexture = (LPDIRECT3DCUBETEXTURE9)m_pBaseTexture;
        SAFE_RELEASE( pCubeTexture );
        break;
    default: assert( false );
    }

    m_Type          = TT_2D;
    m_Usage         = TU_DEFAULT;
    m_Format        = PFT_A8R8G8B8;
    m_nMipLevels    = 0;
    m_sName         = "";

    return FLY_OK;
}


//------------------------------------------------------------------------
// Create the 2D texture for the engine.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:           Width of the texture.
//      UINT:           Height of the texture.
//      TEXTUREUSAGE:   Usage of the texture.
//      PIXELFORMAT:    Pixel format of the texture.
//      UINT:           Number of mipmaps, If set to 0, System will calc
//                      all the mipmaps.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::Create2DTexture( UINT nWidth,UINT nHeight,TEXTUREUSAGE Usage,
                                        PIXELFORMAT Format,UINT nMipLevels )
{
    HRESULT hr;
    D3DFORMAT d3dFmt;
    DWORD dwUsage;
    LPDIRECT3DTEXTURE9 p2DTexture;

    if( m_pBaseTexture )
    {
        switch( m_Type )
        {
        case TT_2D:
            ((LPDIRECT3DTEXTURE9)m_pBaseTexture)->Release();
            break;
        case TT_3D:
            ((LPDIRECT3DVOLUMETEXTURE9)m_pBaseTexture)->Release();
            break;
        case TT_CUBEMAP:
            ((LPDIRECT3DCUBETEXTURE9)m_pBaseTexture)->Release();
            break;
        default: assert( false );
        }
    }

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    m_Type = TT_2D;
    m_Usage = Usage;
    m_Format = Format;

    d3dFmt = FlyD3DUtil::FormatToD3DFormat( Format );

    if( Usage == TU_DEFAULT )
    {
        dwUsage = 0;

        // Create 2D the texture
        hr = m_pd3dDevice->CreateTexture( nWidth,nHeight,nMipLevels,dwUsage,
            d3dFmt,D3DPOOL_MANAGED,&p2DTexture,
            NULL );
        if( FAILED(hr) )
        {
            logMgr.WriteError( 0,"Create texture error, Create2DTexture() failed!" );
            return hr;
        }
    }
    else if( Usage == TU_RENDERTARGET )
    {
        dwUsage = D3DUSAGE_RENDERTARGET;

        // Create texture target.
        hr = m_pd3dDevice->CreateTexture( nWidth,nHeight,1,dwUsage,
            d3dFmt,D3DPOOL_DEFAULT,&p2DTexture,
            NULL );
        if( FAILED(hr) )
        {
            logMgr.WriteError( 0,"Create texture error, Create2DTexture() failed!" );
            return hr;
        }
    }

    // Store the mipmap levels.
    m_nMipLevels = p2DTexture->GetLevelCount();

    // Convert to base interface.
    m_pBaseTexture = (LPDIRECT3DBASETEXTURE9)p2DTexture;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Create the 3D volume texture for the engine.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:           Width of the texture.
//      UINT:           Height of the texture.
//      UINT:           Depth of the texture.
//      TEXTUREUSAGE:   Usage of the texture.
//      PIXELFORMAT:    Pixel format of the texture.
//      UINT:           Number of mipmaps, If set to 0, System will calc
//                      all the mipmaps.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::Create3DTexture( UINT nWidth,UINT nHeight,UINT nDepth,TEXTUREUSAGE Usage,
                                        PIXELFORMAT Format,UINT nMipLevels )
{
    HRESULT hr;
    D3DFORMAT d3dFmt;
    LPDIRECT3DVOLUMETEXTURE9 p3DTexture;

    if( m_pBaseTexture )
    {
        switch( m_Type )
        {
        case TT_2D:
            ((LPDIRECT3DTEXTURE9)m_pBaseTexture)->Release();
            break;
        case TT_3D:
            ((LPDIRECT3DVOLUMETEXTURE9)m_pBaseTexture)->Release();
            break;
        case TT_CUBEMAP:
            ((LPDIRECT3DCUBETEXTURE9)m_pBaseTexture)->Release();
            break;
        default: assert( false );
        }
    }

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    // 3D texture can't be a renderTarget.
    if( Usage == TU_RENDERTARGET )
    {
        logMgr.WriteError( 0,"3D texture can't be a renderTarget, Create3DTexture() failed!" );
        return FLY_INVALIDPARAM;
    }

    m_Type = TT_3D;
    m_Usage = Usage;
    m_Format = Format;

    d3dFmt = FlyD3DUtil::FormatToD3DFormat( Format );

    // Create the 3D texture.
    hr = m_pd3dDevice->CreateVolumeTexture( nWidth,nHeight,nDepth,nMipLevels,0,d3dFmt,
                                            D3DPOOL_MANAGED,&p3DTexture,NULL );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Create volume texture error, Create3DTexture() failed!" );
        return hr;
    }

    // Store the mipmaps count
    m_nMipLevels = p3DTexture->GetLevelCount();

    // Convert to base interface.
    m_pBaseTexture = (LPDIRECT3DBASETEXTURE9)p3DTexture;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Create the cube texture for the engine.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:           Edge length of the cube texture.
//      TEXTUREUSAGE:   Usage of the texture.
//      PIXELFORMAT:    Pixel format of the texture.
//      UINT:           Number of mipmaps, If set to 0, System will calc
//                      all the mipmaps.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::CreateCubeTexture( UINT nEdgeLength,TEXTUREUSAGE Usage,
                                          PIXELFORMAT Format,UINT nMipLevels )
{
    HRESULT hr;
    D3DFORMAT d3dFmt;
    LPDIRECT3DCUBETEXTURE9 pCubeTexture;

    if( m_pBaseTexture )
    {
        switch( m_Type )
        {
        case TT_2D:
            ((LPDIRECT3DTEXTURE9)m_pBaseTexture)->Release();
            break;
        case TT_3D:
            ((LPDIRECT3DVOLUMETEXTURE9)m_pBaseTexture)->Release();
            break;
        case TT_CUBEMAP:
            ((LPDIRECT3DCUBETEXTURE9)m_pBaseTexture)->Release();
            break;
        default: assert( false );
        }
    }

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    // Cube texture can't be a renderTarget.
    if( Usage == TU_RENDERTARGET )
    {
        logMgr.WriteError( 0,"Cube texture can't be a renderTarget, CreateCubeTexture() failed!" );
        return FLY_INVALIDPARAM;
    }

    m_Type = TT_CUBEMAP;
    m_Usage = Usage;
    m_Format = Format;

    d3dFmt = FlyD3DUtil::FormatToD3DFormat( Format );

    // Create the cube texture.
    hr = m_pd3dDevice->CreateCubeTexture( nEdgeLength,nMipLevels,0,d3dFmt,
                                          D3DPOOL_MANAGED,&pCubeTexture,NULL );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Create cube texture error, CreateCubeTexture() failed!" );
        return hr;
    }

    // Store the mipmaps count.
    m_nMipLevels = pCubeTexture->GetLevelCount();

    // Convert to the base interface.
    m_pBaseTexture = (LPDIRECT3DBASETEXTURE9)pCubeTexture;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Build the lod data for the texture object.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::AutoBuildLodData(void)
{
    if( m_nMipLevels <= 1 ) return FLY_FAIL;

    for( UINT i=1;i<m_nMipLevels;i++ )
        BuildSubMipmaps( i );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Load the 2D texture from the file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the image file.
//      UINT:           Number of mipmaps, If set to 0, System will calc
//                      all the mipmaps.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::Load2DTexture( const char* cFilename,UINT nMipLevels )
{
    HRESULT hr;
    FLYIMAGEDATA data;
    FlyImageCodec codec;
    LPDIRECT3DTEXTURE9 pTexture;
    D3DFORMAT d3dFmt;
    DWORD* pMemory;
    D3DLOCKED_RECT d3dRect;
    int LineWidth;

    // Reset the content.
    ResetContent();

    // Get the log manager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    // 1. Special case, Load from the DDS file.
    char* cExt = (char*)FlyStringUtil::GetFileExt( cFilename );
    if( cExt && strcmp(strupr(cExt),"DDS") == 0 )
    {
        D3DSURFACE_DESC desc;

        hr = D3DXCreateTextureFromFileEx( m_pd3dDevice,cFilename,D3DX_DEFAULT,D3DX_DEFAULT,
                                          nMipLevels,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,
                                          D3DX_DEFAULT,0,NULL,NULL,&pTexture );
        if( FAILED(hr) )
        {
            logMgr.WriteError( 0,"Create the texture from DDS error, Load2DTexture() failed!" );
            return hr;
        }

        pTexture->GetLevelDesc( 0,&desc );

        // Check the valid format.
        if( desc.Format != D3DFMT_X8R8G8B8 && desc.Format != D3DFMT_X8B8G8R8 &&
            desc.Format != D3DFMT_A8R8G8B8 && desc.Format != D3DFMT_A8B8G8R8 &&
            desc.Format != D3DFMT_R5G6B5 )
        {
            logMgr.WriteError( 0,"Invalid DDS format file for us! Load2DTexture() failed!" );

            // Release the texture.
            pTexture->Release();
            return FLY_UNKNOWNFORMAT;
        }

        // Store the attribute of the texture.
        m_sName = cFilename;
        m_Format = FlyD3DUtil::D3DFormatToFormat( desc.Format );
        m_nMipLevels = pTexture->GetLevelCount();
        m_Type = TT_2D;
        m_Usage = TU_DEFAULT;

        // Convert to the base interface.
        m_pBaseTexture = (LPDIRECT3DBASETEXTURE9)pTexture;

        return FLY_OK;
    }

    // 2. The other type of image, Use our codec to load!
    hr = codec.LoadImage( cFilename,&data );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Load the image \"%s\" failed!",cFilename );
        return hr;
    }

    m_sName = cFilename;
    m_Type = TT_2D;
    m_Usage = TU_DEFAULT;

    switch( data.Format )
    {
    case PFT_R8G8B8:
    case PFT_B8G8R8:
        m_Format = PFT_X8R8G8B8;
        break;
    case PFT_R8G8B8A8:
    case PFT_A8R8G8B8:
    case PFT_B8G8R8A8:
    case PFT_A8B8G8R8:
        m_Format = PFT_A8R8G8B8;
        break;
    default:
        logMgr.WriteError( 0,"Invalid texture format for us! Load2DTexture() failed!" );
        SAFE_DELETE_ARRAY( data.pData );
        return FLY_UNKNOWNFORMAT;
    }

    d3dFmt = FlyD3DUtil::FormatToD3DFormat( m_Format );

    // Create the direct3D texture object.
    hr = m_pd3dDevice->CreateTexture( data.nWidth,data.nHeight,nMipLevels,0,d3dFmt,
                                      D3DPOOL_MANAGED,&pTexture,NULL );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Create texture error, Load2DTexture() failed!" );
        return hr;
    }

    // Write the pixel data.
    if( FAILED(pTexture->LockRect(0,&d3dRect,NULL,0)) )
    {
        logMgr.WriteError( 0,"Lock the texture failed, Load2DTexture() failed!" );
        return FLY_BUFFERLOCK;
    }

    LineWidth = d3dRect.Pitch >> 2;    // 32 bit
    pMemory = (DWORD*)d3dRect.pBits;

    // Copy each pixel according their formats.
    for( int cy=0;cy<data.nHeight;cy++ )
    {
        for( int cx=0;cx<data.nWidth;cx++ )
        {
            DWORD Color;
            int n = cy*data.nWidth + cx;

            switch( data.Format )
            {
            case PFT_R8G8B8:    // 24 bit
                Color = MAKE3DCOLOR( data.pData[n*3+2],data.pData[n*3+1],data.pData[n*3+0],255 );
                break;
            case PFT_B8G8R8:    // 24 bit
                Color = MAKE3DCOLOR( data.pData[n*3+0],data.pData[n*3+1],data.pData[n*3+2],255 );
                break;
            case PFT_A8R8G8B8:  // 32 bit
                Color = MAKE3DCOLOR( data.pData[n*4+2],data.pData[n*4+1],data.pData[n*4+0],data.pData[n*4+3] );
                break;
            case PFT_A8B8G8R8:  // 32 bit
                Color = MAKE3DCOLOR( data.pData[n*4+0],data.pData[n*4+1],data.pData[n*4+2],data.pData[n*4+3] );
                break;
            case PFT_R8G8B8A8:  // 32 bit
                Color = MAKE3DCOLOR( data.pData[n*4+3],data.pData[n*4+2],data.pData[n*4+1],data.pData[n*4+0] );
                break;
            case PFT_B8G8R8A8:  // 32 bit
                Color = MAKE3DCOLOR( data.pData[n*4+1],data.pData[n*4+2],data.pData[n*4+3],data.pData[n*4+0] );
                break;
            default: Color = 0xffffffff; break;
            }

            pMemory[cy*LineWidth+cx] = Color;
        }
    }

    pTexture->UnlockRect( 0 );

    // Clean the buffer.
    delete[] data.pData;

    // Convert to the base interface.
    m_pBaseTexture = (LPDIRECT3DBASETEXTURE9)pTexture;

    // Get the mipmaps count.
    m_nMipLevels = m_pBaseTexture->GetLevelCount();

    // Create the sub mipmaps.
    for( UINT i=1;i<m_nMipLevels;i++ )
    {
        BuildSubMipmaps( i );
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Save the 2D texture to a image file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the image file.
//      FLYIMAGETYPE:   Which type of image to save.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::Save2DTexture( const char* cFilename,FLYIMAGETYPE Type ) const
{
    FlyImage image;
    HRESULT hr;
    LPDIRECT3DTEXTURE9 pTexture;
    D3DLOCKED_RECT rect;
    UCHAR* pPixelDatas;

    if( m_Type != TT_2D ) return FLY_FAIL;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    pTexture = (LPDIRECT3DTEXTURE9)m_pBaseTexture;

    if( Type == FIMAGE_DDS )
    {
        LPDIRECT3DSURFACE9 pSurface;

        hr = pTexture->GetSurfaceLevel( 0,&pSurface );
        if( FAILED(hr) )
        {
            logMgr.WriteError( 0,"Get the surface for the texture error, Save2DTexture() failed!" );
            return hr;
        }

        // Save the surface.
        hr = D3DXSaveSurfaceToFile( cFilename,D3DXIFF_DDS,pSurface,NULL,NULL );
        if( FAILED(hr) )
        {
            logMgr.WriteError( 0,"Save the surface error, Save2DTexture() failed!" );
            return hr;
        }

        pSurface->Release();
    }
    else
    {
        // Lock the texture's rect, copy its pixels data.
        hr = pTexture->LockRect( 0,&rect,NULL,0 );
        if( FAILED(hr) )
        {
            logMgr.WriteError( 0,"Lock the texture's data error, Save2DTexture() failed!" );
            return hr;
        }

        pPixelDatas = (UCHAR*)rect.pBits;
        image.LoadImageFromMemory( pPixelDatas,m_Format,GetWidth(0),GetHeight(0) );

        switch( Type )
        {
        case FIMAGE_BMP:
            if( m_Format == PFT_A8R8G8B8 )
                image.SaveImageToBitmap( cFilename,true );
            else
                image.SaveImageToBitmap( cFilename,false );
            break;
        case FIMAGE_JPG:
            if( m_Format == PFT_A8R8G8B8 )
                image.SaveImageToJPEG( cFilename,true );
            else
                image.SaveImageToJPEG( cFilename,false );
            break;
        case FIMAGE_TEX:
            if( m_Format == PFT_A8R8G8B8 )
                image.SaveImageToFile( cFilename,true );
            else
                image.SaveImageToFile( cFilename,false );
            break;
        case FIMAGE_PNG:
            if( m_Format == PFT_A8R8G8B8 )
                image.SaveImageToPNG( cFilename,true );
            else
                image.SaveImageToPNG( cFilename,false );
            break;
        case FIMAGE_TGA:
            if( m_Format == PFT_A8R8G8B8 )
                image.SaveImageToTGA( cFilename,true );
            else
                image.SaveImageToTGA( cFilename,false );
            break;
        }

        // Unlock the texture.
        pTexture->UnlockRect( 0 );
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Build the mipmaps for the 2D texture.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        The level of the mipmap to build.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::BuildSubMipmaps( int nLevel )
{
    HRESULT hr;
    LPDIRECT3DTEXTURE9 pTexture;
    IDirect3DSurface9* pLevel1;
    IDirect3DSurface9* pLevel2;

    pTexture = (LPDIRECT3DTEXTURE9)m_pBaseTexture;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    // Get the higher level.
    hr = pTexture->GetSurfaceLevel( nLevel-1,&pLevel1 );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Get the surface error, BuildSubMipmaps() failed!" );
        return hr;
    }

    // Get the current level.
    hr = pTexture->GetSurfaceLevel( nLevel,&pLevel2 );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Get the surface error, BuildSubMipmaps() failed!" );
        return hr;
    }

    // Convert the higher level to current level.
    hr = D3DXLoadSurfaceFromSurface( pLevel2,NULL,NULL,pLevel1,NULL,NULL,
                                     D3DX_DEFAULT,0 );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Convert the surface error, BuildSubMipmaps() failed!" );
        return hr;
    }

    pLevel1->Release();
    pLevel2->Release();

    return FLY_OK;
}


//------------------------------------------------------------------------
// Load the 3D texture from the file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the 3D texture file.
//      UINT:           Number of mipmaps, If set to 0, System will calc
//                      all the mipmaps.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::Load3DTexture( const char* cFilename,UINT nMipLevels )
{
    HRESULT hr;
    LPDIRECT3DVOLUMETEXTURE9 pTexture;
    D3DVOLUME_DESC desc;

    // Reset the content.
    ResetContent();

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    // Create the volume texture.
    hr = D3DXCreateVolumeTextureFromFileEx( m_pd3dDevice,cFilename,D3DX_DEFAULT,
        D3DX_DEFAULT,D3DX_DEFAULT,nMipLevels,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,
        D3DX_DEFAULT,0,NULL,NULL,&pTexture );

    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Create volume texture error, Load3DTexture() failed!" );
        return hr;
    }

    pTexture->GetLevelDesc( 0,&desc );

    // Check the valid format.
    if( desc.Format != D3DFMT_X8R8G8B8 && desc.Format != D3DFMT_X8B8G8R8 &&
        desc.Format != D3DFMT_A8R8G8B8 && desc.Format != D3DFMT_A8B8G8R8 )
    {
        logMgr.WriteError( 0,"Invalid DDS format for us, Create3DTexture() failed!" );

        // Release the texture.
        pTexture->Release();
        return FLY_UNKNOWNFORMAT;
    }

    // Store the attribute of the texture.
    m_sName = cFilename;
    m_Format = FlyD3DUtil::D3DFormatToFormat( desc.Format );
    m_nMipLevels = pTexture->GetLevelCount();
    m_Type = TT_3D;
    m_Usage = TU_DEFAULT;

    // Convert to the base interface.
    m_pBaseTexture = (LPDIRECT3DBASETEXTURE9)pTexture;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Save the 3D texture to the DDS file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to store the 3D texture.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::Save3DTexture( const char* cFilename ) const
{
    HRESULT hr;
    LPDIRECT3DVOLUMETEXTURE9 pTexture;
    LPDIRECT3DVOLUME9 pVolume;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    pTexture = (LPDIRECT3DVOLUMETEXTURE9)m_pBaseTexture;

    // Get the volume at 0 level.
    hr = pTexture->GetVolumeLevel( 0,&pVolume );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Get the volume error, Save3DTexture() failed!" );
        return hr;
    }

    // Save the volume to DDS file.
    hr = D3DXSaveVolumeToFile( cFilename,D3DXIFF_DDS,pVolume,NULL,NULL );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Save the volume to DDS file error, Save3DTexture() failed!" );
        return hr;
    }

    // Release the volume.
    pVolume->Release();

    return FLY_OK;
}


//------------------------------------------------------------------------
// Load the cube texture from the file. (Only for DDS format)
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the cube texture file.
//      UINT:           Number of mipmaps, If set to 0, System will calc
//                      all the mipmaps.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::LoadCubeTexture( const char* cFilename,UINT nMipLevels )
{
    HRESULT hr;
    LPDIRECT3DCUBETEXTURE9 pTexture;
    D3DSURFACE_DESC desc;

    // Reset the content.
    ResetContent();

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    // Create the cube texture.
    hr = D3DXCreateCubeTextureFromFileEx( m_pd3dDevice,cFilename,D3DX_DEFAULT,
        nMipLevels,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,
        0,NULL,NULL,&pTexture );

    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Create cube texture error, LoadCubeTexture() failed!" );
        return hr;
    }

    pTexture->GetLevelDesc( 0,&desc );

    // Check the valid surface format.
    if( desc.Format != D3DFMT_X8R8G8B8 && desc.Format != D3DFMT_X8B8G8R8 &&
        desc.Format != D3DFMT_A8R8G8B8 && desc.Format != D3DFMT_A8B8G8R8 )
    {
        logMgr.WriteError( 0,"Invalid DDS format for us, CreateCubeTexture() failed!" );

        // Release the texture.
        pTexture->Release();
        return FLY_UNKNOWNFORMAT;
    }

    // Get the attributes of the texture.
    m_sName = cFilename;
    m_Format = FlyD3DUtil::D3DFormatToFormat( desc.Format );
    m_nMipLevels = pTexture->GetLevelCount();
    m_Type = TT_CUBEMAP;
    m_Usage = TU_DEFAULT;

    // Convert to the base interface.
    m_pBaseTexture = (LPDIRECT3DBASETEXTURE9)pTexture;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Save one of the face for the cube texture.
// ----------------------------------------------------------------------
// Param -> IN:
//      CUBEFACE:       Which Cube face to save ?
//      const char*:    Filename for the image file.
//      FLYIMAGETYPE:   Which type of image to save ?
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::SaveCubeTexture( CUBEFACE Face,const char* cFilename,FLYIMAGETYPE Type ) const
{
    FlyImage image;
    HRESULT hr;
    LPDIRECT3DCUBETEXTURE9 pTexture;
    D3DLOCKED_RECT rect;
    UCHAR* pPixelDatas;

    if( m_Type != TT_CUBEMAP ) return FLY_FAIL;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    pTexture = (LPDIRECT3DCUBETEXTURE9)m_pBaseTexture;

    if( Type == FIMAGE_DDS )
    {
        LPDIRECT3DSURFACE9 pSurface;

        hr = pTexture->GetCubeMapSurface( (D3DCUBEMAP_FACES)Face,0,&pSurface );
        if( FAILED(hr) )
        {
            logMgr.WriteError( 0,"Get the surface for the cube texture error, SaveCubeTexture() failed!" );
            return hr;
        }

        // Save the surface.
        hr = D3DXSaveSurfaceToFile( cFilename,D3DXIFF_DDS,pSurface,NULL,NULL );
        if( FAILED(hr) )
        {
            logMgr.WriteError( 0,"Save the surface error, SaveCubeTexture() failed!" );
            return hr;
        }

        pSurface->Release();
    }
    else
    {
        // Lock the texture's rect, copy its pixels data.
        hr = pTexture->LockRect( (D3DCUBEMAP_FACES)Face,0,&rect,NULL,0 );
        if( FAILED(hr) )
        {
            logMgr.WriteError( 0,"Lock the cube texture's data error, SaveCubeTexture() failed!" );
            return hr;
        }

        pPixelDatas = (UCHAR*)rect.pBits;
        image.LoadImageFromMemory( pPixelDatas,m_Format,GetWidth(0),GetHeight(0) );

        switch( Type )
        {
        case FIMAGE_BMP:
            if( m_Format == PFT_A8R8G8B8 || m_Format == PFT_A8B8G8R8 )
                image.SaveImageToBitmap( cFilename,true );
            else
                image.SaveImageToBitmap( cFilename,false );
            break;
        case FIMAGE_JPG:
            if( m_Format == PFT_A8R8G8B8 || m_Format == PFT_A8B8G8R8 )
                image.SaveImageToJPEG( cFilename,true );
            else
                image.SaveImageToJPEG( cFilename,false );
            break;
        case FIMAGE_TEX:
            if( m_Format == PFT_A8R8G8B8 || m_Format == PFT_A8B8G8R8 )
                image.SaveImageToFile( cFilename,true );
            else
                image.SaveImageToFile( cFilename,false );
            break;
        case FIMAGE_PNG:
            if( m_Format == PFT_A8R8G8B8 || m_Format == PFT_A8B8G8R8 )
                image.SaveImageToPNG( cFilename,true );
            else
                image.SaveImageToPNG( cFilename,false );
            break;
        case FIMAGE_TGA:
            if( m_Format == PFT_A8R8G8B8 || m_Format == PFT_A8B8G8R8 )
                image.SaveImageToTGA( cFilename,true );
            else
                image.SaveImageToTGA( cFilename,false );
            break;
        }

        // Unlock the texture.
        pTexture->UnlockRect( (D3DCUBEMAP_FACES)Face,0 );
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Get the width of texture in one level.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Which level's width to get.
//------------------------------------------------------------------------
UINT FlyD3DTexture::GetWidth( UINT nLevel ) const
{
    LPDIRECT3DTEXTURE9 p2DTexture;
    LPDIRECT3DVOLUMETEXTURE9 p3DTexture;
    LPDIRECT3DCUBETEXTURE9 pCubeTexture;

    D3DSURFACE_DESC surface;
    D3DVOLUME_DESC volume;

    if( nLevel >= m_pBaseTexture->GetLevelCount() )
        return 0;

    switch( m_Type )
    {
    case TT_2D:
        memset( &surface,0,sizeof(D3DSURFACE_DESC) );
        p2DTexture = (LPDIRECT3DTEXTURE9)m_pBaseTexture;
        p2DTexture->GetLevelDesc( nLevel,&surface );
        return surface.Width;
    case TT_3D:
        memset( &volume,0,sizeof(D3DVOLUME_DESC) );
        p3DTexture = (LPDIRECT3DVOLUMETEXTURE9)m_pBaseTexture;
        p3DTexture->GetLevelDesc( nLevel,&volume );
        return volume.Width;
    case TT_CUBEMAP:
        memset( &surface,0,sizeof(D3DSURFACE_DESC) );
        pCubeTexture = (LPDIRECT3DCUBETEXTURE9)m_pBaseTexture;
        pCubeTexture->GetLevelDesc( nLevel,&surface );
        return surface.Width;
    }

    return 0;
}


//------------------------------------------------------------------------
// Get the height of the texture in one level.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:   Which level's height to get.
//------------------------------------------------------------------------
UINT FlyD3DTexture::GetHeight( UINT nLevel ) const
{
    LPDIRECT3DTEXTURE9 p2DTexture;
    LPDIRECT3DVOLUMETEXTURE9 p3DTexture;
    LPDIRECT3DCUBETEXTURE9 pCubeTexture;

    D3DSURFACE_DESC surface;
    D3DVOLUME_DESC volume;

    if( nLevel >= m_pBaseTexture->GetLevelCount() )
        return 0;

    switch( m_Type )
    {
    case TT_2D:
        memset( &surface,0,sizeof(D3DSURFACE_DESC) );
        p2DTexture = (LPDIRECT3DTEXTURE9)m_pBaseTexture;
        p2DTexture->GetLevelDesc( nLevel,&surface );
        return surface.Height;
    case TT_3D:
        memset( &volume,0,sizeof(D3DVOLUME_DESC) );
        p3DTexture = (LPDIRECT3DVOLUMETEXTURE9)m_pBaseTexture;
        p3DTexture->GetLevelDesc( nLevel,&volume );
        return volume.Height;
    case TT_CUBEMAP:
        memset( &surface,0,sizeof(D3DSURFACE_DESC) );
        pCubeTexture = (LPDIRECT3DCUBETEXTURE9)m_pBaseTexture;
        pCubeTexture->GetLevelDesc( nLevel,&surface );
        return surface.Height;
    }

    return 0;
}


//------------------------------------------------------------------------
// Get the depth of the texture, Only for the 3D texture.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Which level's depth to get.
//------------------------------------------------------------------------
UINT FlyD3DTexture::GetDepth( UINT nLevel ) const
{
    D3DVOLUME_DESC desc;
    LPDIRECT3DVOLUMETEXTURE9 p3DTexture;

    if( nLevel >= m_pBaseTexture->GetLevelCount() )
        return 0;

    if( m_Type == TT_3D )
    {
        p3DTexture = (LPDIRECT3DVOLUMETEXTURE9)m_pBaseTexture;
        p3DTexture->GetLevelDesc( nLevel,&desc );
        return desc.Depth;
    }

    return 0;
}


//------------------------------------------------------------------------
// Does the texture has alpha channel ?
//------------------------------------------------------------------------
bool FlyD3DTexture::HasAlphaChannel(void) const
{
    switch( m_Format )
    {
    case PFT_A8R8G8B8:
    case PFT_A8B8G8R8:
    case PFT_R8G8B8A8:
    case PFT_B8G8R8A8:
        return true;
        break;
    default: return false;
    }
}


//------------------------------------------------------------------------
// Get the name of the texture.
//------------------------------------------------------------------------
const char* FlyD3DTexture::GetName(void) const
{
    return m_sName.c_str();
}


//------------------------------------------------------------------------
// Set the name for the texture.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the texture object.
//------------------------------------------------------------------------
void FlyD3DTexture::SetName( const char* cName )
{
    m_sName = cName;
}


//------------------------------------------------------------------------
// Get the texture type.
//------------------------------------------------------------------------
TEXTURETYPE FlyD3DTexture::GetTextureType(void) const
{
    return m_Type;
}


//------------------------------------------------------------------------
// Get the Usage for the texture.
//------------------------------------------------------------------------
TEXTUREUSAGE FlyD3DTexture::GetUsage(void) const
{
    return m_Usage;
}


//------------------------------------------------------------------------
// Get the pixel format for the texture.
//------------------------------------------------------------------------
PIXELFORMAT FlyD3DTexture::GetFormat(void) const
{
    return m_Format;
}


//------------------------------------------------------------------------
// Get the count of the mipmaps.
//------------------------------------------------------------------------
UINT FlyD3DTexture::GetMipmapCount(void) const
{
    return m_nMipLevels;
}


//------------------------------------------------------------------------
// Convert the normal vector to the RGBA color.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyVector*:     Normal of the point.
//      float:          Height of the point.
//------------------------------------------------------------------------
DWORD FlyD3DTexture::VectorToRGBA( FlyVector* pv,float fHeight )
{
    DWORD r = (DWORD)( 127.0f * pv->x + 128.0f );
    DWORD g = (DWORD)( 127.0f * pv->y + 128.0f );
    DWORD b = (DWORD)( 127.0f * pv->z + 128.0f );
    DWORD a = (DWORD)( 255.0f * fHeight );

    return (DWORD)((a << 24) | (r << 16) | (g << 8) | b);
}


//------------------------------------------------------------------------
// Set the alpha key for the given RGB color.
// ----------------------------------------------------------------------
// Param -> IN:
//      UCHAR:      Red value of the color.
//      UCHAR:      Green value of the color.
//      UCHAR:      Blue value of the color.
//      UCHAR:      Alpha value to set.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::SetAlphaKey( UCHAR r,UCHAR g,UCHAR b,UCHAR alpha )
{
    DWORD dwKey;
    D3DLOCKED_RECT d3dRect;
    HRESULT hr;
    LPDIRECT3DTEXTURE9 pTexture;
    D3DSURFACE_DESC desc;

    if( m_Type != TT_2D ) return FLY_FAIL;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    if( m_Format != PFT_A8B8G8R8 && m_Format != PFT_A8R8G8B8 )
    {
        logMgr.WriteError( 0,"Format of texture have no alpha value! SetAlphaKey() failed!" );
        return FLY_FAIL;
    }

    // Calculate alpha key
    dwKey = MAKE3DCOLOR( r,g,b,alpha );

    pTexture = (LPDIRECT3DTEXTURE9)m_pBaseTexture;

    // Loop through all the mip maps.
    for( UINT m=0;m<m_nMipLevels;m++ )
    {
        pTexture->GetLevelDesc( m,&desc );

        // Lock the texture datas.
        hr = pTexture->LockRect( m,&d3dRect,NULL,0 );
        if( FAILED(hr) )
        {
            logMgr.WriteError( 0,"Lock the texture datas error, SetAlphaKey() failed!" );
            return hr;
        }

        // Overwrite all key pixels with new color.
        for( UINT y=0;y<desc.Height;y++ )
        {
            for( UINT x=0;x<desc.Width;x++ )
            {
                UCHAR uR,uG,uB,uA;

                uR = (UCHAR)((((DWORD*)d3dRect.pBits)[y*desc.Width+x] & 0x00ff0000) >> 16);
                uG = (UCHAR)((((DWORD*)d3dRect.pBits)[y*desc.Width+x] & 0x0000ff00) >> 8);
                uB = (UCHAR)(((DWORD*)d3dRect.pBits)[y*desc.Width+x] & 0x000000ff);
                uA = (UCHAR)((((DWORD*)d3dRect.pBits)[y*desc.Width+x] & 0xff000000) >> 24);

                if( uR == r && uG == g && uB == b && alpha < uA )
                    ((DWORD*)d3dRect.pBits)[y*desc.Width+x] = dwKey;
            }
        }

        pTexture->UnlockRect( m );
    }

	return FLY_OK;
}


//------------------------------------------------------------------------
// Set the whole texture's transparency value.
// ----------------------------------------------------------------------
// Param -> IN:
//      UCHAR:      New alpha value to set.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::SetTransparency( UCHAR alpha )
{
    D3DLOCKED_RECT d3dRect;
    HRESULT hr;
    LPDIRECT3DTEXTURE9 pTexture;
    D3DSURFACE_DESC desc;

    if( m_Type != TT_2D ) return FLY_FAIL;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    if( m_Format != PFT_A8B8G8R8 && m_Format != PFT_A8R8G8B8 )
    {
        logMgr.WriteError( 0,"Format of texture have no alpha value! SetAlphaKey() failed!" );
        return FLY_FAIL;
    }

    pTexture = (LPDIRECT3DTEXTURE9)m_pBaseTexture;

    for( DWORD m=0;m<pTexture->GetLevelCount();m++ )
    {
        pTexture->GetLevelDesc( m,&desc );

        hr = pTexture->LockRect( m,&d3dRect,NULL,0 );
        if( FAILED(hr) )
        {
            logMgr.WriteError( 0,"Lock the pixels error, SetTransparency() failed!" );
            return hr;
        }

        // Overwrite the pixel data.
        for( UINT y=0;y<desc.Height;y++ )
        {
            for( UINT x=0;x<desc.Width;x++ )
            {
                UCHAR uR,uG,uB,uA;

                uR = (UCHAR)((((DWORD*)d3dRect.pBits)[y*desc.Width+x] & 0x00ff0000) >> 16);
                uG = (UCHAR)((((DWORD*)d3dRect.pBits)[y*desc.Width+x] & 0x0000ff00) >> 8);
                uB = (UCHAR)(((DWORD*)d3dRect.pBits)[y*desc.Width+x] & 0x000000ff);
                uA = (UCHAR)((((DWORD*)d3dRect.pBits)[y*desc.Width+x] & 0xff000000) >> 24);

                if( alpha < uA )
                    ((DWORD*)d3dRect.pBits)[y*desc.Width+x] = MAKE3DCOLOR( uR,uG,uB,alpha );
            }
        }

        pTexture->UnlockRect( m );
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// The texture is a PFT_A8R8G8B8 format heightmap, Convert itself to a 
// normal map.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::ConvertToNormalmap(void)
{
    HRESULT hr;
    LPDIRECT3DTEXTURE9 pTexture;
    D3DSURFACE_DESC desc;
    D3DLOCKED_RECT rect;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    // Check the valid type.
    if( m_Type != TT_2D ) return FLY_FAIL;

    // Check the valid format.
    if( m_Format != PFT_A8R8G8B8 && m_Format != PFT_A8B8G8R8 )
    {
        logMgr.WriteError( 0,"Need the valid format! ConvertToNormalmap() failed!" );
        return FLY_UNKNOWNFORMAT;
    }

    pTexture = (LPDIRECT3DTEXTURE9)m_pBaseTexture;

    // Loop through all the mipmaps.
    for( DWORD m=0;m<pTexture->GetLevelCount();m++ )
    {
        pTexture->GetLevelDesc( m,&desc );

        // Lock the pixel data.
        hr = pTexture->LockRect( m,&rect,NULL,0 );
        if( FAILED(hr) )
        {
            logMgr.WriteError( 0,"Lock the pixels error, ConvertNormalmap() failed!" );
            return hr;
        }

        for( UINT y=0;y<desc.Height;y++ )
        {
            for( UINT x=0;x<desc.Width;x++ )
            {
                DWORD color00 = ((DWORD*)rect.pBits)[0];
                DWORD color10 = ((DWORD*)rect.pBits)[1];
                DWORD color01 = ((DWORD*)rect.pBits)[rect.Pitch>>2];

                float fHeight00 = (float)((color00 & 0x00ff0000)>>16)/255.0f;
                float fHeight10 = (float)((color10 & 0x00ff0000)>>16)/255.0f;
                float fHeight01 = (float)((color01 & 0x00ff0000)>>16)/255.0f;

                FlyVector vPoint00( x+0.0f,y+0.0f,fHeight00 );
                FlyVector vPoint10( x+1.0f,y+0.0f,fHeight10 );
                FlyVector vPoint01( x+0.0f,y+1.0f,fHeight01 );
                FlyVector v10 = vPoint10 - vPoint00;
                FlyVector v01 = vPoint01 - vPoint00;

                FlyVector vNormal;
                vNormal.Cross( v10,v01 );
                vNormal.Normalize();

                // Store normal as RGBA in normal map
                ((DWORD*)rect.pBits)[y*desc.Width+x] = VectorToRGBA( &vNormal,fHeight00 );
            }
        }

        pTexture->UnlockRect( 0 );
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Access the 2D texture's data, get the data pointer.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:            Which level of the texture to access.
//      TEXTUREACCESS:  Privilage of the access.
//      void**:         To store the data pointer of the texture.
//      int*:           To store the line pitch of the texture.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::Access2DMap( int nLevel,TEXTUREACESS tma,void** ppBits,int* pitch )
{
    HRESULT hr;
    LPDIRECT3DTEXTURE9 pTexture;
    DWORD dwFlags = 0;
    D3DLOCKED_RECT rect;

    // Check the texture type.
    if( m_Type != TT_2D ) return FLY_FAIL;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    if( tma == TMA_READONLY ) dwFlags |= D3DLOCK_READONLY;

    pTexture = (LPDIRECT3DTEXTURE9)m_pBaseTexture;
    hr = pTexture->LockRect( nLevel,&rect,NULL,dwFlags );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Lock the texture's data error, Access2DMap() failed!" );
        return hr;
    }

    *ppBits = rect.pBits;
    *pitch = rect.Pitch;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Access the 3D texture's data, get the data pointer.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:            Which level of the texture to access.
//      TEXTUREACCESS:  Privilage of the access.
//      void**:         To store the data pointer of the texture.
//      int*:           To store the Row pitch of the texture.
//      int*:           To store the slice pitch of the texture.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::Access3DMap( int nLevel,TEXTUREACESS tma,void** ppBits,
                                    int* rowPitch,int* slicePitch )
{
    HRESULT hr;
    LPDIRECT3DVOLUMETEXTURE9 pTexture;
    DWORD dwFlags = 0;
    D3DLOCKED_BOX box;

    // Check the texture type.
    if( m_Type != TT_3D ) return FLY_FAIL;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    if( tma == TMA_READONLY ) dwFlags |= D3DLOCK_READONLY;

    pTexture = (LPDIRECT3DVOLUMETEXTURE9)m_pBaseTexture;
    hr = pTexture->LockBox( nLevel,&box,NULL,dwFlags );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Lock the 3D texture's data error, Access3DMap() failed!" );
        return hr;
    }

    *rowPitch = box.RowPitch;
    *slicePitch = box.SlicePitch;
    *ppBits = box.pBits;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Access the cube texture's data, get the data pointer.
// ----------------------------------------------------------------------
// Param -> IN:
//      CUBEFACE:       Which face of the map to access.
//      int:            Which level of the texture to access.
//      TEXTUREACCESS:  Privilage of the access.
//      void**:         To store the data pointer of the texture.
//      int*:           To store the row pitch of the texture.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::AccessCubeMap( CUBEFACE Face,int nLevel,TEXTUREACESS tma,void** ppBits,
                                      int* pitch )
{
    HRESULT hr;
    LPDIRECT3DCUBETEXTURE9 pTexture;
    DWORD dwFlags = 0;
    D3DLOCKED_RECT rect;

    // Check the texture type.
    if( m_Type != TT_CUBEMAP ) return FLY_FAIL;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    if( tma == TMA_READONLY ) dwFlags |= D3DLOCK_READONLY;

    pTexture = (LPDIRECT3DCUBETEXTURE9)m_pBaseTexture;
    hr = pTexture->LockRect( (D3DCUBEMAP_FACES)Face,nLevel,&rect,NULL,dwFlags );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Lock the cubemap's data error, AccessCubeMap() failed!" );
        return hr;
    }

    *ppBits = rect.pBits;
    *pitch = rect.Pitch;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Unaccess the 2D texture's data.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:            Which level of the texture to unaccess.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::Unaccess2DMap( int nLevel )
{
    LPDIRECT3DTEXTURE9 pTexture;

    if( m_Type != TT_2D ) return FLY_FAIL;

    pTexture = (LPDIRECT3DTEXTURE9)m_pBaseTexture;
    pTexture->UnlockRect( nLevel );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Unaccess the 3D texture's data.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:            Which level of the texture to unaccess.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::Unaccess3DMap( int nLevel )
{
    LPDIRECT3DVOLUMETEXTURE9 pTexture;

    if( m_Type != TT_3D ) return FLY_FAIL;

    pTexture = (LPDIRECT3DVOLUMETEXTURE9)m_pBaseTexture;
    pTexture->UnlockBox( nLevel );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Unaccess the cube texture's data.
// ----------------------------------------------------------------------
// Param -> IN:
//      CUBEFACE:       Which face of the map to unaccess.
//      int:            Which level of the texture to unaccess.
//------------------------------------------------------------------------
HRESULT FlyD3DTexture::UnaccessCubeMap( CUBEFACE Face,int nLevel )
{
    LPDIRECT3DCUBETEXTURE9 pTexture;

    if( m_Type != TT_CUBEMAP ) return FLY_FAIL;

    pTexture = (LPDIRECT3DCUBETEXTURE9)m_pBaseTexture;
    pTexture->UnlockRect( (D3DCUBEMAP_FACES)Face,nLevel );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Get the base Direct3D texture pointer.
//------------------------------------------------------------------------
LPDIRECT3DBASETEXTURE9 FlyD3DTexture::GetD3DTexturePtr(void)
{
    return m_pBaseTexture;
}