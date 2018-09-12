/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRenderStateSet.h
 *  Desc:   This file define all the render states for render device.
 *  Author: Yish
 *  Date:   2015/3/9
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_RENDERSTATESET_H__
#define __F3D_RENDERSTATESET_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////

/** Render states of device.
*/
class FAIRY_API F3DRenderStateSet : public FGeneralAlloc
{
public:

	enum
	{
		MAX_MRT = 8,	// Max count of the multiple render targets.
	};

public:

	// Rasterization states.
    EFillMode FillMode;							// Determines the fill mode to use when rendering.
    EShadeMode ShadeMode;						// Shading modes.
	ECullMode CullMode;							// Indicates triangles facing the specified direction are not drawn.
	bool bFrontFaceCCW;							// Determines if a triangle is front- or back-facing.
	bool bScissorEnable;						// Enable scissor-rectangle culling. All pixels ouside an active scissor rectangle are culled. 
	bool bMultiSampleEnable;					// Enable multisample antialiasing.
	bool bDepthClipEnable;						// Enable clipping based on distance. 

	// Depth buffer states.
    EDepthMode DepthMode;						// 设备的深度缓冲模式
    ECompareFunc DepthFunc;						// 设备深度测试的比较方法
    float fSlopeScale;							// 此值用于决定多少偏移值可以应用到共面图元上来减少Z冲突
    float fDepthBias;							// 用于进行深度测试的相比较的值

	// Stencil buffer states.
	uint32 nStencilRef;							// 模板测试的参考值
	bool bFrontStencilEnable;					// 是否开启正面模板缓冲
	ECompareFunc FrontStencilFunc;				// 模板测试的比较方法
	uint32 nFrontStencilMask;					// 模板缓冲的掩码
	uint32 nFrontStencilWriteMask;				// 模板缓冲区写入时的掩码
	EStencilOperation FrontStencilFail;			// 模板测试失败后的操作
    EStencilOperation FrontStencilDepthFail;	// 深度测试失败后的操作
    EStencilOperation FrontStencilPass;			// 模板，深度测试都通过时的操作
	bool bBackStencilEnable;					// 是否开启背面模板缓冲
	ECompareFunc BackStencilFunc;				// 模板测试的比较方法
	uint32 nBackStencilMask;					// 模板缓冲的掩码
	uint32 nBackStencilWriteMask;				// 模板缓冲区写入时的掩码
	EStencilOperation BackStencilFail;			// 模板测试失败后的操作
    EStencilOperation BackStencilDepthFail;		// 深度测试失败后的操作
    EStencilOperation BackStencilPass;			// 模板，深度测试都通过时的操作

	// Blend states.
	bool bAlphaToCoverageEnable;
	bool bIndependentBlendEnable;
	bool bRTBlendEnable[MAX_MRT];
	EBlendOption SrcBlend[MAX_MRT];
	EBlendOption DestBlend[MAX_MRT];
	EBlendOperation BlendOP[MAX_MRT];
	EBlendOption SrcBlendAlpha[MAX_MRT];
	EBlendOption DestBlendAlpha[MAX_MRT];
	EBlendOperation BlendOPAlpha[MAX_MRT];
	uchar RTWriteMask[MAX_MRT];					// A per-pixel write mask that allows control over which components can be written.

public:
    F3DRenderStateSet() { Reset(); }

	// Reset the render states.
	void Reset(void);

	// Get the hash code of the render state set.
	uint32 GetHashCode() const;
};

/** Sampler states of device.
*/
class FAIRY_API F3DSamplerState : public FGeneralAlloc
{
public:
    F3DColor BorderColor;           // 纹理边框颜色
    ETextureAddress AddressU;       // 纹理U坐标的地址模式
    ETextureAddress AddressV;       // 纹理V坐标的地址模式
    ETextureAddress AddressW;       // 纹理W坐标的地址模式
    EFilterOption MagFilter;        // 纹理放大时的过滤模式
    EFilterOption MinFilter;        // 纹理缩小时的过滤模式
    EFilterOption MipFilter;        // 生成MIPMAP的过滤模式
    uint32 nMaxAnisotropy;          // 最大各向异性过滤值
	float fMinMipLevel;				// 要使用的最小的Mipmap层索引
    float fMaxMipLevel;             // 要使用的最大的MIPMAP层索引
    float fMipMapLodBias;           // MIPMAP的LOD偏移
	ECompareFunc CmpFunc;			// A function that compares sampled data against existing sampled data.

public:
	F3DSamplerState() { Reset(); }

	// Reset the sampler states.
	void Reset();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_RENDERSTATESET_H__