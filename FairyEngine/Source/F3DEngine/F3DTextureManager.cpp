/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTextureManager.cpp
 *  Desc:   This file implements the texture manager for engine.
 *  Author: Yish
 *  Date:   2010/12/15
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DTextureManager.h"
#include "F3DImageLoader.h"


/** Get the singleton.
*/
F3DTextureManager& F3DTextureManager::GetInstance()
{
	static F3DTextureManager obj;
	return obj;
}

/** Destructor.
*/
F3DTextureManager::~F3DTextureManager(void)
{
	TextureMap::iterator it = m_Textures.begin();
	for(; it!=m_Textures.end(); ++it)
	{
		F_SAFE_DELETE(it->second.pTex);
	}
}

/** Load a texture from the file.
*/
F3DTextureBase* F3DTextureManager::LoadTexture(const char* filename)
{
	TextureMap::iterator it = m_Textures.find(filename);
	if( it != m_Textures.end() )
	{
		it->second.iRef++;
		return it->second.pTex;
	}

	F3DImage* pImage = F3DImageLoader::GetInstance().Load(filename);
	if( !pImage )
	{
		FLOG_WARNINGF("F3DTextureManager::LoadTexture, Load the texture from file(%s) failed!", filename);
		return NULL;
	}

	return NULL;
}

/** Get the specified texture.
*/
F3DTextureBase* F3DTextureManager::GetTextureByName(const char* filename)
{
	if( !filename ) return NULL;
	TextureMap::iterator it = m_Textures.find(filename);
	return it != m_Textures.end() ? it->second.pTex : NULL;
}

/** Add the reference of texture.
*/
void F3DTextureManager::AddTextureRef(F3DTextureBase* pTex)
{
	FASSERT(pTex);
	TextureMap::iterator it = m_Textures.find(pTex->GetName());
	if( it != m_Textures.end() )
		it->second.iRef++;
}

/** Release the specified texture.
*/
void F3DTextureManager::ReleaseTexture(F3DTextureBase* pTex)
{
	FASSERT(pTex);
	TextureMap::iterator it = m_Textures.find(pTex->GetName());
	if( it != m_Textures.end() )
	{
		if( (--it->second.iRef) == 0 )
		{
			delete it->second.pTex;
			m_Textures.erase(it);
		}
	}
}

/** Get the reference count of the specified texture.
*/
int F3DTextureManager::GetTextureRef(F3DTextureBase* pTex) const
{
	FASSERT(pTex);
	TextureMap::const_iterator it = m_Textures.find(pTex->GetName());
	return (it != m_Textures.end()) ? it->second.iRef : 0;
}