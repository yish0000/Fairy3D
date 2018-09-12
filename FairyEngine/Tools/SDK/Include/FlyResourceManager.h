//------------------------------------------------------------------------
// Name: FlyResourceManager.h
// Desc: This file define a class to manage all kinds of resources for
//       the engine, such as, texture, material, effect ...
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_RESOURCEMANAGER_H__
#define __FLY_RESOURCEMANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"
#include "FlyRenderDevice.h"

#include "FlyRenderMaterial.h"
#include "FlyTexture.h"
#include "FlyFont.h"
#include "FlyRenderEffect.h"
#include "FlyGraphicBuffer.h"
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
class FlyResourceManager
{
public:
    // Constructor and destructor.
    FlyResourceManager(void) { /* Nothing to do! */ };
    virtual ~FlyResourceManager(void) { /* Nothing to do! */ };

    // Functions for the textures.
    virtual FlyTexture* Load2DTexture( const char* cFilename,int nMipLevels ) = 0;
    virtual FlyTexture* Load3DTexture( const char* cFilename,int nMipLevels ) = 0;
    virtual FlyTexture* LoadCubeTexture( const char* cFilename,int nMipLevels ) = 0;
    virtual FlyTexture* AddNullTexture( const char* cName ) = 0;
    virtual void DeleteTexture( FlyTexture* pTexture ) = 0;
    virtual void DeleteTextureByIndex( UINT nIndex ) = 0;
    virtual void DeleteTextureByName( const char* cFilename ) = 0;
    virtual void DeleteAllTextures(void) = 0;
    virtual FlyTexture* GetTextureByIndex( UINT nIndex ) = 0;
    virtual FlyTexture* GetTextureByName( const char* cFilename ) = 0;

    // Functions for the render effect.
    virtual FlyRenderEffect* LoadEffect( const char* cFilename ) = 0;
    virtual FlyRenderEffect* LoadEffectSource( const char* cFilename ) = 0;
    virtual FlyRenderEffect* AddNullEffect( const char* cName ) = 0;
    virtual void DeleteEffect( FlyRenderEffect* pEffect ) = 0;
    virtual void DeleteEffectByIndex( UINT nIndex ) = 0;
    virtual void DeleteEffectByName( const char* cFilename ) = 0;
    virtual void DeleteAllEffects(void) = 0;
    virtual FlyRenderEffect* GetEffectByIndex( UINT nIndex ) = 0;
    virtual FlyRenderEffect* GetEffectByName( const char* cFilename ) = 0;

    // Functions for the render material.
    virtual FlyRenderMaterial* CreateMaterial( const char* cMatName ) = 0;
    virtual void DeleteMaterial( FlyRenderMaterial* pMaterial ) = 0;
    virtual void DeleteMaterialByIndex( UINT nIndex ) = 0;
    virtual void DeleteMaterialByName( const char* cMatName ) = 0;
    virtual void DeleteAllMaterials(void) = 0;
    virtual FlyRenderMaterial* GetMaterialByIndex( UINT nIndex ) = 0;
    virtual FlyRenderMaterial* GetMaterialByName( const char* cMatName ) = 0;

    // Functions to make a shader object.
    virtual FlyShaderObject* MakeShaderObject(void) = 0;

    // Functions to make a vertex buffer.
    virtual FlyGraphicBuffer* MakeVertexBuffer( UINT nSize,BUFFERUSAGE Usage,
                                                MEMORYMODE MemMode ) = 0;

    // Functions to make an index buffer.
    virtual FlyGraphicBuffer* MakeIndexBuffer( UINT nSize,BUFFERUSAGE Usage,MEMORYMODE MemMode,
                                               INDEXFORMAT Format ) = 0;

    // Functions for the fonts.
    virtual FlyFont* MakeSystemFont(void) = 0;
};


//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_RESOURCEMANAGER_H__