//------------------------------------------------------------------------
// Name: FlyRenderState.cpp
// Desc: This file is to implement two functions to init render states.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyRenderState.h"


//------------------------------------------------------------------------
// Initialize the render states.
//------------------------------------------------------------------------
void FlyRenderStateBuffer::InitRenderStates(void)
{
    // Set the default values.
    this->CullMode = CM_CCW;
    this->PolygonMode = PM_SOLID;
    this->ShadeMode = SM_GOURAUD;

    this->DepthMode = DEPTH_READWRITE;
    this->DepthFunc = CMP_LESSEQUAL;
    this->fDepthBias = 0.0f;
    this->fSlopeScale = 0.0f;

    this->bFrontStencilEnable = false;
    this->bBackStencilEnable = false;
    this->StencilFunc = CMP_ALWAYS;
    this->nStencilRef = 0;
    this->dwStencilMask = 0xffffffff;
    this->dwStencilWriteMask = 0xffffffff;
    this->StencilFail = STOP_KEEP;
    this->StencilDepthFail = STOP_KEEP;
    this->StencilPass = STOP_KEEP;
    this->BackStencilFunc = CMP_ALWAYS;
    this->BackStencilFail = STOP_KEEP;
    this->BackStencilDepthFail = STOP_KEEP;
    this->BackStencilPass = STOP_KEEP;

    this->bAlphaBlend = false;
    this->bAlphaTest = false;
    this->AlphaTestFunc = CMP_ALWAYS;
    this->dwAlphaTestRef = 0;
    this->SourceBlend = SBM_ONE;
    this->DestBlend = SBM_ZERO;

    // Shutdown the light.
    this->bLighting = false;
    this->bSpecularLight = false;
    memset( &StdMaterial,0,sizeof(FLYMATERIAL) );

    // Initialize the texture blend mode.
    for( int i=0;i<MAX_TEXTURES;i++ )
    {
        CoordSetIndex[i] = i;
        ColorBlend[i].Type = BT_COLOR;
        ColorBlend[i].Arg1 = BSTA_TEXTURE;
        ColorBlend[i].Arg2 = BSTA_CURRENT;
        ColorBlend[i].Operation = TOP_DISABLE;
        ColorBlend[i].fFactor = 0.0f;
        AlphaBlend[i].Type = BT_ALPHA;
        AlphaBlend[i].Arg1 = BSTA_TEXTURE;
        AlphaBlend[i].Arg2 = BSTA_CURRENT;
        AlphaBlend[i].Operation = TOP_DISABLE;
        AlphaBlend[i].fFactor = 0.0f;
    }

    ColorBlend[0].Operation = TOP_MODULATE;
    AlphaBlend[0].Operation = TOP_SELECTARG1;
}


//------------------------------------------------------------------------
// Initialize the sampler states.
//------------------------------------------------------------------------
void FlySampler::InitSamplerStates(void)
{
    pTexture = NULL;

    dwBorderColor = 0x00000000;
    AddressU = TADDR_WRAP;
    AddressV = TADDR_WRAP;
    AddressW = TADDR_WRAP;
    MagFilter = FILTER_LINEAR;
    MinFilter = FILTER_LINEAR;
    MipFilter = FILTER_LINEAR;
    nMaxAnisotropy = 1;
    nMaxMipLevel = 0;
    fMipMapLodBias = 0.0f;
}