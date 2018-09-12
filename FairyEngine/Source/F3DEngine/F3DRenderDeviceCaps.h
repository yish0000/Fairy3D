/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRenderDeviceCaps.h
 *  Desc:   This file define the capacities of the render device.
 *  Author: Yish
 *  Date:   2015/3/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_RENDERDEVICE_CAPS_H__
#define __F3D_RENDERDEVICE_CAPS_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////

/** Store the capacities for render device.
*/
struct SRenderDeviceCaps
{
	uint32 nVSVersion;              // 最高顶点着色器版本
	uint32 nPSVersion;              // 最高像素着色器版本
	uint32 nMaxLights;              // 最大的活动光源个数
	uint32 nMaxTextures;            // 最大的纹理层数
	uint32 nMaxPrimitiveCount;      // 每个批次可以提交的最大图元数
	uint32 nMaxStreamStride;        // 最大的顶点元素大小
	uint32 nMaxTextureWidth;        // 最大的纹理宽度
	uint32 nMaxTextureHeight;       // 最大的纹理高度
	uint32 nMaxTextureDepth;
	uint32 nMaxTextureCubeSize;
	uint32 nMaxVertexTextures;      // 最大的顶点纹理个数

	bool bAlphaTexture : 1;         // 是否支持Alpha纹理
	bool bCubeMap : 1;              // 是否支持立方体纹理
	bool bVolumeMap : 1;            // 是否支持立体纹理
	bool bMipCubeMap : 1;           // 对于立方体纹理，是否支持Mipmap
	bool bMipMap : 1;               // 对于普通纹理，是否支持Mipmap
	bool bMipVolumeMap : 1;         // 对于立体纹理，是否支持Mipmap
	bool bTableFog : 1;             // 是否支持像素雾化
	bool bVertexFog : 1;            // 是否支持顶点雾化

	bool bHWStencil : 1;            // 是否支持硬件模板缓冲
	bool bTwoSideStencil : 1;       // 是否支持双面模板缓冲
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_RENDERDEVICE_CAPS_H__