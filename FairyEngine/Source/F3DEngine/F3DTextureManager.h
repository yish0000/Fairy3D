/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTextureManager.h
 *  Desc:   This file define a texture manager for engine.
 *  Author: Yish
 *  Date:   2010/12/15
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_TEXTUREMANAGER_H__
#define __F3D_TEXTUREMANAGER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTexture.h"

///////////////////////////////////////////////////////////////////////////


/** Texture manager.
Create and manage all the textures.
*/
class FAIRY_API F3DTextureManager : public FGeneralAlloc
{
public:

	friend class F3DTextureBase;

	struct TextureNode
	{
		int iRef;
		F3DTextureBase* pTex;
	};

	typedef std::map<AString, TextureNode> TextureMap;

protected:
    TextureMap m_Textures;	// Texture map.

public:
    ~F3DTextureManager(void);

	// Create a 2d texture.
	F3DTexture2D* Create2DTexture(const char* name, uint32 width, uint32 height, ETextureUsage usage,
		EPixelFormat format, uint32 numMipmaps);
	// Create a 3d texture.
	F3DTexture3D* Create3DTexture(const char* name, uint32 width, uint32 height, uint32 depth, ETextureUsage usage,
		EPixelFormat format, uint32 numMipmaps);
	// Create a cube texture.
	F3DTextureCube* CreateCubeTexture(const char* name, uint32 edgeLength, ETextureUsage usage,
		EPixelFormat format, uint32 numMipmaps);

	// Load the specified texture.
	F3DTextureBase* LoadTexture(const char* filename);

    // Get the specified texture.
    F3DTextureBase* GetTextureByName(const char* name);

    // Get the singleton.
    static F3DTextureManager& GetInstance(void);

protected:

	// Add the reference of the texture.
	void AddTextureRef(F3DTextureBase* pTex);
	// Release the reference of the texture.
	void ReleaseTexture(F3DTextureBase* pTex);
	// Get the reference count of the texture.
	int GetTextureRef(F3DTextureBase* pTex) const;

private:
	F3DTextureManager() {}
	F_DISABLE_COPY_ASSIGNMENT(F3DTextureManager);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_TEXTUREMANAGER_H__