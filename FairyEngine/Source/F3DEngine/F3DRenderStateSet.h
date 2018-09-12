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
    EDepthMode DepthMode;						// �豸����Ȼ���ģʽ
    ECompareFunc DepthFunc;						// �豸��Ȳ��ԵıȽϷ���
    float fSlopeScale;							// ��ֵ���ھ�������ƫ��ֵ����Ӧ�õ�����ͼԪ��������Z��ͻ
    float fDepthBias;							// ���ڽ�����Ȳ��Ե���Ƚϵ�ֵ

	// Stencil buffer states.
	uint32 nStencilRef;							// ģ����ԵĲο�ֵ
	bool bFrontStencilEnable;					// �Ƿ�������ģ�建��
	ECompareFunc FrontStencilFunc;				// ģ����ԵıȽϷ���
	uint32 nFrontStencilMask;					// ģ�建�������
	uint32 nFrontStencilWriteMask;				// ģ�建����д��ʱ������
	EStencilOperation FrontStencilFail;			// ģ�����ʧ�ܺ�Ĳ���
    EStencilOperation FrontStencilDepthFail;	// ��Ȳ���ʧ�ܺ�Ĳ���
    EStencilOperation FrontStencilPass;			// ģ�壬��Ȳ��Զ�ͨ��ʱ�Ĳ���
	bool bBackStencilEnable;					// �Ƿ�������ģ�建��
	ECompareFunc BackStencilFunc;				// ģ����ԵıȽϷ���
	uint32 nBackStencilMask;					// ģ�建�������
	uint32 nBackStencilWriteMask;				// ģ�建����д��ʱ������
	EStencilOperation BackStencilFail;			// ģ�����ʧ�ܺ�Ĳ���
    EStencilOperation BackStencilDepthFail;		// ��Ȳ���ʧ�ܺ�Ĳ���
    EStencilOperation BackStencilPass;			// ģ�壬��Ȳ��Զ�ͨ��ʱ�Ĳ���

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
    F3DColor BorderColor;           // ����߿���ɫ
    ETextureAddress AddressU;       // ����U����ĵ�ַģʽ
    ETextureAddress AddressV;       // ����V����ĵ�ַģʽ
    ETextureAddress AddressW;       // ����W����ĵ�ַģʽ
    EFilterOption MagFilter;        // ����Ŵ�ʱ�Ĺ���ģʽ
    EFilterOption MinFilter;        // ������Сʱ�Ĺ���ģʽ
    EFilterOption MipFilter;        // ����MIPMAP�Ĺ���ģʽ
    uint32 nMaxAnisotropy;          // ���������Թ���ֵ
	float fMinMipLevel;				// Ҫʹ�õ���С��Mipmap������
    float fMaxMipLevel;             // Ҫʹ�õ�����MIPMAP������
    float fMipMapLodBias;           // MIPMAP��LODƫ��
	ECompareFunc CmpFunc;			// A function that compares sampled data against existing sampled data.

public:
	F3DSamplerState() { Reset(); }

	// Reset the sampler states.
	void Reset();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_RENDERSTATESET_H__