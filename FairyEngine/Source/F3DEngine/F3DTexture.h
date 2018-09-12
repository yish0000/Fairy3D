/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTexture.h
 *  Desc:   This file define the texture classes for engine.
 *  Author: Yish
 *  Date:   2010/12/9
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_TEXTURE_H__
#define __F3D_TEXTURE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////

class F3DImage;

/** Base class of texture
*/
class FAIRY_API F3DTextureBase : public FGeneralAlloc
{
protected:
    AString m_sName;        // Name of the texture.
    ETextureType m_Type;    // Type of the texture.
    ETextureUsage m_Usage;  // Usage of the texture.

    uint32 m_nWidth;        // Width.
    uint32 m_nHeight;       // Height.
    uint32 m_nDepth;        // Depth. (For 3D Texture)
    EPixelFormat m_Format;  // Pixel format.
    uint32 m_nMipCount;     // Count of mip maps.

public:
    F3DTextureBase(const AString& name);
    virtual ~F3DTextureBase(void);

    // Clear the content.
    virtual void ResetContent() = 0;
    // Generate the mip maps.
    virtual bool GenerateMipmaps(uint32 numMipmaps) = 0;

    // Get the width of texture.
    uint32 GetWidth() const { return m_nWidth; }
    // Get the height of texture.
    uint32 GetHeight() const { return m_nHeight; }
    // Get the depth of texture. (For 3d texture.)
    uint32 GetDepth() const { return m_nDepth; }

    // Get the texture's type.
    ETextureType GetTextureType(void) const { return m_Type; }
    // Get the usage of texture.
    ETextureUsage GetUsage(void) const { return m_Usage; }
    // Get the pixel format.
    EPixelFormat GetPixelFormat(void) const { return m_Format; }
    // Get the count of mipmaps.
    uint32 GetMipmapCount(void) const { return m_nMipCount; }
    // Has alpha channel ?
    bool HasAlphaChannel() const;
    // Get the name of texture.
    const AString& GetName(void) const { return m_sName; }
};

/** 2D Texture.
*/
class FAIRY_API F3DTexture2D : public F3DTextureBase
{
public:
	F3DTexture2D(const AString& name);
	virtual ~F3DTexture2D();

	// Create a texture.
	virtual bool CreateTexture(uint32 width, uint32 height, ETextureUsage usage,
		EPixelFormat format, uint32 numMipmaps) = 0;

	// Load the texture from file.
	virtual bool LoadTexture(const char* filename, uint32 numMipmaps = 0);
	// Load the texture from image.
	virtual bool LoadTextureFromImage(F3DImage* image, uint32 numMipmaps = 0);
	// Load the texture from memory.
	virtual bool LoadTextureFromMemory(fbyte* data, uint32 width, uint32 height, EPixelFormat format, uint32 numMipmaps = 0);

	// Save the texture to a specified file.
	virtual void SaveTexture( const char* filename );

	// Access the texture.
	virtual bool Lock(uint32 level, ETextureLock tl, void** ppBits, int* pitch) = 0;
	virtual void Unlock(uint32 level) = 0;
};

/** 3D Texture.
*/
class FAIRY_API F3DTexture3D : public F3DTextureBase
{
public:
	F3DTexture3D(const AString& name);
	virtual ~F3DTexture3D();

	// Create a 3d texture.
	virtual bool CreateTexture(uint32 width, uint32 height, uint32 depth, ETextureUsage usage,
		EPixelFormat format, uint32 numMipmaps) = 0;
	// Load the texture from memory.
	virtual bool LoadTextureFromMemory(fbyte* data, uint32 width, uint32 height, uint32 depth,
		EPixelFormat format, uint32 numMipmaps = 0);

	// Access the texture.
	virtual bool Lock(uint32 level, ETextureLock tl, void** ppBits, int* rowPitch, int* slicePitch) = 0;
	virtual void Unlock(uint32 level) = 0;
};

/** Cube texture.
*/
class FAIRY_API F3DTextureCube : public F3DTextureBase
{
public:
	F3DTextureCube(const AString& name);
	virtual ~F3DTextureCube();

	// Create a cube texture.
	virtual bool CreateTexture(uint32 edgeLength, ETextureUsage usage, EPixelFormat format, uint32 numMipmaps) = 0;

	// Load the cube texture from six images.
	virtual bool LoadTexture( const AStringList& imageList, uint32 numMipmaps = 0 );
	virtual bool LoadTexture( const char* imageList[6], uint32 numMipmaps = 0 );
	virtual bool LoadTextureFromImage( F3DImage* image[6], uint32 numMipmaps = 0 );

	// Save a specified face to file.
	virtual void SaveTexture(ECubeFace face, const char* filename);

	// Access the cube texture.
	virtual bool Lock(ECubeFace face, uint32 level, ETextureLock tl, void** ppBits, int* pitch) = 0;
	virtual void Unlock(ECubeFace face, uint32 level) = 0;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_TEXTURE_H__