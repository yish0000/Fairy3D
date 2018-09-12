//------------------------------------------------------------------------
// Name: FlyRenderState.h
// Desc: This file define the render state buffer and sampler.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_RENDERSTATE_H__
#define __FLY_RENDERSTATE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


enum RSTYPE
{
    RS_CULLMODE,
    RS_POLYGONMODE,
    RS_SHADEMODE,
    RS_DEPTHMODE,
    RS_DEPTHFUNC,
    RS_SLOPESCALE,
    RS_DEPTHBIAS,
    RS_FRONTSTENCILENABLE,
    RS_BACKSTENCILENABLE,
    RS_STENCILFUNC,
    RS_STENCILREF,
    RS_STENCILMASK,
    RS_STENCILWRITEMASK,
    RS_STENCILFAIL,
    RS_STENCILDEPTHFAIL,
    RS_STENCILPASS,
    RS_BACKSTENCILFUNC,
    RS_BACKSTENCILFAIL,
    RS_BACKSTENCILDEPTHFAIL,
    RS_BACKSTENCILPASS,
    RS_ALPHABLENDENABLE,
    RS_ALPHATESTENABLE,
    RS_ALPHATESTFUNC,
    RS_ALPHATESTREF,
    RS_SOURCEBLEND,
    RS_DESTBLEND,
    RS_COORDSETINDEX,
    RS_TEXCOLORBLEND,
    RS_TEXALPHABLEND,
    RS_LIGHTING,
    RS_SPECULARLIGHT,
    RS_STDMATERIAL,
};

// A buffer to store the render states.
class FlyRenderStateBuffer
{
public:
    // 1. Rasterizer states.
    CULLMODE CullMode;                          // Cullmode for device.
    POLYGONMODE PolygonMode;                    // Polygon mode for device.
    SHADEMODE ShadeMode;                        // Shade mode for device.

    // 2. Depth buffer states.
    DEPTHMODE DepthMode;                        // The depth buffer mode.
    FLYCMPFUNC DepthFunc;                       // Depth buffer compare func.
    float fSlopeScale;                          // Slope scale depth bias value.
    float fDepthBias;                           // Depth bias value.

    // 3. Stencil buffer states.
    bool bFrontStencilEnable;
    bool bBackStencilEnable;
    FLYCMPFUNC StencilFunc;
    int nStencilRef;
    DWORD dwStencilMask;
    DWORD dwStencilWriteMask;
    STENCILOPERATION StencilFail;
    STENCILOPERATION StencilDepthFail;
    STENCILOPERATION StencilPass;
    FLYCMPFUNC BackStencilFunc;
    STENCILOPERATION BackStencilFail;
    STENCILOPERATION BackStencilDepthFail;
    STENCILOPERATION BackStencilPass;

    // 4. Alpha blend states.
    bool bAlphaBlend;                           // Open the alpha blend ?
    bool bAlphaTest;                            // Open the alpha test ?
    FLYCMPFUNC AlphaTestFunc;                   // Alpha test compare function.
    DWORD dwAlphaTestRef;                       // Alpha test compare value.
    SCENEBLENDMODE SourceBlend;                 // Source blend factor.
    SCENEBLENDMODE DestBlend;                   // Destination blend factor.

    // 5. Texture stage states.
    int CoordSetIndex[MAX_TEXTURES];
    TEXTUREBLENDMODE ColorBlend[MAX_TEXTURES];  // List of the color blend states.
    TEXTUREBLENDMODE AlphaBlend[MAX_TEXTURES];  // List of the alpha blend states.

    // 6. For Light calculation.
    bool bLighting;                             // Open the light effect ?
    bool bSpecularLight;                        // Open the specular light ?
    FLYMATERIAL StdMaterial;                    // Material data.

    // Initialize the state buffer.
    void InitRenderStates(void);
};

// A structure to store the sampler data.
class FlySampler
{
public:
    FlyTexture* pTexture;

    DWORD dwBorderColor;
    TEXTUREADDRESS AddressU;
    TEXTUREADDRESS AddressV;
    TEXTUREADDRESS AddressW;
    FILTEROPTION MagFilter;
    FILTEROPTION MinFilter;
    FILTEROPTION MipFilter;
    UINT nMaxAnisotropy;
    UINT nMaxMipLevel;
    float fMipMapLodBias;

    // Initialize the sampler states.
    void InitSamplerStates(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_RENDERSTATE_H__