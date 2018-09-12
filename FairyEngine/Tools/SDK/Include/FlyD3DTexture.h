//------------------------------------------------------------------------
// Name: FlyD3DTexture.h
// Desc: This file is to implement the interface FlyTexture with the
//       Direct3D platform.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_D3DTEXTURE_H__
#define __FLY_D3DTEXTURE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTexture.h"

//////////////////////////////////////////////////////////////////////////


class FlyD3DTexture : public FlyTexture
{
protected:
    LPDIRECT3DDEVICE9       m_pd3dDevice;       // Pointer to the Direct3D device.
    LPDIRECT3DBASETEXTURE9  m_pBaseTexture;     // Pointer to the Direct3D texture.

    // Build the mipmaps.
    HRESULT BuildSubMipmaps( int nLevel );

    // Use this function when Create the normal map.
    DWORD VectorToRGBA( FlyVector* pv,float fHeight );

public:
    // Constructor and destructor.
    FlyD3DTexture( LPDIRECT3DDEVICE9 pd3dDevice );
    ~FlyD3DTexture(void);

    // Override the interface's functions.
    HRESULT ResetContent(void);

    HRESULT Create2DTexture( UINT nWidth,UINT nHeight,TEXTUREUSAGE Usage,
                             PIXELFORMAT Format,UINT nMipLevels );
    HRESULT Create3DTexture( UINT nWidth,UINT nHeight,UINT nDepth,TEXTUREUSAGE Usage,
                             PIXELFORMAT Format,UINT nMipLevels );
    HRESULT CreateCubeTexture( UINT nEdgeLength,TEXTUREUSAGE Usage,
                               PIXELFORMAT Format,UINT nMipLevels );

    HRESULT AutoBuildLodData(void);

    HRESULT Load2DTexture( const char* cFilename,UINT nMipLevels );
    HRESULT Save2DTexture( const char* cFilename,FLYIMAGETYPE Type ) const;

    // Only for the DDS format.
    HRESULT Load3DTexture( const char* cFilename,UINT nMipLevels );
    HRESULT LoadCubeTexture( const char* cFilename,UINT nMipLevels );
    HRESULT Save3DTexture( const char* cFilename ) const;
    HRESULT SaveCubeTexture( CUBEFACE Face,const char* cFilename,FLYIMAGETYPE Type ) const;

    const char* GetName(void) const;
    void SetName( const char* cName );

    // Get the attributes for the texture.
    UINT GetWidth( int nLevel ) const;
    UINT GetHeight( int nLevel ) const;
    UINT GetDepth( int nLevel ) const;
    const char* GetFilename(void) const;
    TEXTURETYPE GetTextureType(void) const;
    TEXTUREUSAGE GetUsage(void) const;
    PIXELFORMAT GetFormat(void) const;
    UINT GetMipmapCount(void) const;
    bool HasAlphaChannel(void) const;

    // Modify the 2D texture.
    HRESULT SetAlphaKey( UCHAR r,UCHAR g,UCHAR b,UCHAR alpha );
    HRESULT SetTransparency( UCHAR alpha );
    HRESULT ConvertToNormalmap(void);

    // Access the texture's datas.
    HRESULT Access2DMap( int nLevel,TEXTUREACESS tma,void** ppBits,int* pitch );
    HRESULT Access3DMap( int nLevel,TEXTUREACESS tma,void** ppBits,int* rowPitch,int* slicePitch );
    HRESULT AccessCubeMap( CUBEFACE Face,int nLevel,TEXTUREACESS tma,void** ppBits,int* pitch );
    HRESULT Unaccess2DMap( int nLevel );
    HRESULT Unaccess3DMap( int nLevel );
    HRESULT UnaccessCubeMap( CUBEFACE Face,int nLevel );

    // Get the Direct3D texture pointer.
    LPDIRECT3DBASETEXTURE9 GetD3DTexturePtr(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_D3DTEXTURE_H__