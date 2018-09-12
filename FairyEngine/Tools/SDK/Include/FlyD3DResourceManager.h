//------------------------------------------------------------------------
// Name: FlyD3DResourceManager.h
// Desc: This file is to implement the interface FlyResourceManager with
//       the Direct3D platform.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_D3DRESOURCEMANAGER_H__
#define __FLY_D3DRESOURCEMANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyResourceManager.h"

#include "FlyD3DRenderDevice.h"
//////////////////////////////////////////////////////////////////////////


class FlyD3DResource;

class FlyD3DResourceManager : public FlyResourceManager
{
    struct sTextureNode
    {
        FlyTexture* pTexture;
        int nReference;
    };
    typedef std::vector<sTextureNode> TexturePool;

    struct sRenderEffectNode
    {
        FlyRenderEffect* pEffect;
        int nReference;
    };
    typedef std::vector<sRenderEffectNode> RenderEffectPool;

    typedef std::vector<FlyFont*> FontPool;
    typedef std::vector<FlyRenderMaterial*> MaterialPool;

    typedef std::vector<FlyD3DResource*> D3DResourceList;

protected:
    FlyD3DRenderDevice* m_pDevice;      // Pointer to the D3D render device.

    TexturePool         m_Textures;     // To store the textures.
    RenderEffectPool    m_Effects;      // To store the render effects.
    MaterialPool        m_Materials;    // To store the materials.
    FontPool            m_Fonts;        // To store the fonts.

    D3DResourceList     m_D3DResources; // Direct3D resource list, Must be lost and reset when
                                        // The device is lost.

public:
    // Constructor and destructor.
    FlyD3DResourceManager( FlyD3DRenderDevice* pD3DDevice );
    ~FlyD3DResourceManager(void);

    // Execute the function when device lost and reset.
    // Only for the D3D RenderSystem.
    HRESULT OnDeviceLost(void);
    HRESULT OnDeviceReset(void);

    // Add a direct3D resource to the list.
    void AddD3DResource( FlyD3DResource* pResource );

    // Remove a direct3D resource from the list.
    void RemoveD3DResource( FlyD3DResource* pResource );

    // Functions for the texture resources.
    FlyTexture* Load2DTexture( const char* cFilename,int nMipLevels );
    FlyTexture* Load3DTexture( const char* cFilename,int nMipLevels );
    FlyTexture* LoadCubeTexture( const char* cFilename,int nMipLevels );
    FlyTexture* AddNullTexture( const char* cName );
    void DeleteTextureByIndex( UINT nIndex );
    void DeleteTextureByName( const char* cFilename );
    void DeleteTexture( FlyTexture* pTexture );
    void DeleteAllTextures(void);
    FlyTexture* GetTextureByIndex( UINT nIndex );
    FlyTexture* GetTextureByName( const char* cFilename );

    // Function to load the render effects.
    FlyRenderEffect* LoadEffect( const char* cFilename );
    FlyRenderEffect* LoadEffectSource( const char* cFilename );
    FlyRenderEffect* AddNullEffect( const char* cName );
    void DeleteEffectByIndex( UINT nIndex );
    void DeleteEffectByName( const char* cFilename );
    void DeleteEffect( FlyRenderEffect* pEffect );
    void DeleteAllEffects(void);
    FlyRenderEffect* GetEffectByIndex( UINT nIndex );
    FlyRenderEffect* GetEffectByName( const char* cFilename );

    // Functions for the render material.
    FlyRenderMaterial* CreateMaterial( const char* cMatName );
    void DeleteMaterial( FlyRenderMaterial* pMaterial );
    void DeleteMaterialByIndex( UINT nIndex );
    void DeleteMaterialByName( const char* cMatName );
    void DeleteAllMaterials(void);
    FlyRenderMaterial* GetMaterialByIndex( UINT nIndex );
    FlyRenderMaterial* GetMaterialByName( const char* cMatName );

    // Function to make a shader object.
    FlyShaderObject* MakeShaderObject(void);

    // Function to make a vertex buffer.
    FlyGraphicBuffer* MakeVertexBuffer( UINT nSize,BUFFERUSAGE Usage,MEMORYMODE MemMode );

    // Function to make an index buffer.
    FlyGraphicBuffer* MakeIndexBuffer( UINT nSize,BUFFERUSAGE Usage,MEMORYMODE MemMode,
                                       INDEXFORMAT Format );

    // Make a new font object.
    FlyFont* MakeSystemFont(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_D3DRESOURCEMANAGER_H__