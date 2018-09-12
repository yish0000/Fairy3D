//------------------------------------------------------------------------
// Name: FlyTexture.h
// Desc: This file define an interface to act as the texture object.
//       There are three types, 2D, 3D, Cube texture.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_TEXTURE_H__
#define __FLY_TEXTURE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


class FlyTexture
{
protected:
    std::string     m_sName;            // Name of the texture.
    TEXTURETYPE     m_Type;             // Type of the texture.
    TEXTUREUSAGE    m_Usage;            // Usage of the texture.
    PIXELFORMAT     m_Format;           // Pixel format for texture.
    UINT            m_nMipLevels;       // Count of mipmap level.

public:
    // Constructor and destructor.
    FlyTexture(void) { /* Nothing to do! */ };
    virtual ~FlyTexture(void) { /* Nothing to do! */ };

    // Reset the content for texture.
    virtual HRESULT ResetContent(void) = 0;

    virtual HRESULT Create2DTexture( UINT nWidth,UINT nHeight,TEXTUREUSAGE Usage,
                                     PIXELFORMAT Format,UINT nMipLevels ) = 0;
    virtual HRESULT Create3DTexture( UINT nWidth,UINT nHeight,UINT nDepth,TEXTUREUSAGE Usage,
                                     PIXELFORMAT Format,UINT nMipLevels ) = 0;
    virtual HRESULT CreateCubeTexture( UINT nEdgeLength,TEXTUREUSAGE Usage,
                                       PIXELFORMAT Format,UINT nMipLevels ) = 0;

    // Build the lod data for texture.
    virtual HRESULT AutoBuildLodData(void) = 0;

    virtual HRESULT Load2DTexture( const char* cFilename,UINT nMipLevels ) = 0;
    virtual HRESULT Save2DTexture( const char* cFilename,FLYIMAGETYPE Type ) const = 0;

    // Only for the *.dds format.
    virtual HRESULT Load3DTexture( const char* cFilename,UINT nMipLevels ) = 0;
    virtual HRESULT LoadCubeTexture( const char* cFilename,UINT nMipLevels ) = 0;
    virtual HRESULT Save3DTexture( const char* cFilename ) const = 0;
    virtual HRESULT SaveCubeTexture( CUBEFACE Face,const char* cFilename,FLYIMAGETYPE Type ) const = 0;

    virtual const char* GetName(void) const = 0;
    virtual void SetName( const char* cName ) = 0;

    // Get the attributes for the texture.
    virtual UINT GetWidth( int nLevel ) const = 0;
    virtual UINT GetHeight( int nLevel ) const = 0;
    virtual UINT GetDepth( int nLevel ) const = 0;
    virtual TEXTURETYPE GetTextureType(void) const = 0;
    virtual TEXTUREUSAGE GetUsage(void) const = 0;
    virtual PIXELFORMAT GetFormat(void) const = 0;
    virtual UINT GetMipmapCount(void) const = 0;
    virtual bool HasAlphaChannel(void) const = 0;

    // Modify the 2D texture.
    virtual HRESULT SetAlphaKey( UCHAR r,UCHAR g,UCHAR b,UCHAR a ) = 0;
    virtual HRESULT SetTransparency( UCHAR alpha ) = 0;
    virtual HRESULT ConvertToNormalmap(void) = 0;

    // Functions to access the data of texture.
    virtual HRESULT Access2DMap( int nLevel,TEXTUREACESS tma,void** ppBits,int* pitch ) = 0;
    virtual HRESULT Access3DMap( int nLevel,TEXTUREACESS tma,void** ppBits,int* rowPitch,int* slicePitch ) = 0;
    virtual HRESULT AccessCubeMap( CUBEFACE Face,int nLevel,TEXTUREACESS tma,void** ppBits,int* pitch ) = 0;
    virtual HRESULT Unaccess2DMap( int nLevel ) = 0;
    virtual HRESULT Unaccess3DMap( int nLevel ) = 0;
    virtual HRESULT UnaccessCubeMap( CUBEFACE Face,int nLevel ) = 0;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_TEXTURE_H__