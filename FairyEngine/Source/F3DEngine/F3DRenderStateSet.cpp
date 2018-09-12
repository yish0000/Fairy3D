/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRenderStateSet.cpp
 *  Desc:   This file define all the render states for render device.
 *  Author: Yish
 *  Date:   2015/3/10
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DRenderStateSet.h"

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class F3DRenderStateSet
//  
///////////////////////////////////////////////////////////////////////////

// Reset the render states.
void F3DRenderStateSet::Reset()
{
	CullMode = CULL_CCW;
	FillMode = FM_SOLID;
	ShadeMode = SM_GOURAUD;

	DepthMode = DEPTH_READWRITE;
	DepthFunc = CMP_LESSEQUAL;
	fSlopeScale = 0.0f;
	fDepthBias = 0.0f;

	bFrontStencilEnable = false;
	bBackStencilEnable = false;
	nStencilRef = 0;
	nFrontStencilMask = 0xffffffff;
	nFrontStencilWriteMask = 0xffffffff;
	FrontStencilFunc = CMP_ALWAYS;
	FrontStencilFail = STOP_KEEP;
	FrontStencilDepthFail = STOP_KEEP;
	FrontStencilPass = STOP_KEEP;
	BackStencilFunc = CMP_ALWAYS;
	BackStencilFail = STOP_KEEP;
	BackStencilDepthFail = STOP_KEEP;
	BackStencilPass = STOP_KEEP;

	bAlphaToCoverageEnable = false;
	bIndependentBlendEnable = false;

	for(int i=0; i<MAX_MRT; i++)
	{
		bRTBlendEnable[i] = false;
		SrcBlend[i] = BLEND_ONE;
		DestBlend[i] = BLEND_ZERO;
		BlendOP[i] = BLEND_OP_ADD;
		SrcBlendAlpha[i] = BLEND_ONE;
		DestBlendAlpha[i] = BLEND_ZERO;
		BlendOPAlpha[i] = BLEND_OP_ADD;
		RTWriteMask[i] = CMASK_ALL;
	}
}

// Get the hash code of the render state set.
uint32 F3DRenderStateSet::GetHashCode() const
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////
//  
//  Implements class F3DSamplerStateSet
//  
///////////////////////////////////////////////////////////////////////////

// Reset the sampler states.
void F3DSamplerState::Reset()
{
	BorderColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
	AddressU = TADDR_CLAMP;
	AddressV = TADDR_CLAMP;
	AddressW = TADDR_CLAMP;
	MagFilter = FILTER_POINT;
	MinFilter = FILTER_POINT;
	MipFilter = FILTER_POINT;
	nMaxAnisotropy = 16;
	fMinMipLevel = 0.0f;
	fMaxMipLevel = F3D_FLOAT_MAX;
	fMipMapLodBias = 0.0f;
	CmpFunc = CMP_NEVER;
}