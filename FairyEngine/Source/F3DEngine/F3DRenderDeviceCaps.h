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
	uint32 nVSVersion;              // ��߶�����ɫ���汾
	uint32 nPSVersion;              // ���������ɫ���汾
	uint32 nMaxLights;              // ���Ļ��Դ����
	uint32 nMaxTextures;            // �����������
	uint32 nMaxPrimitiveCount;      // ÿ�����ο����ύ�����ͼԪ��
	uint32 nMaxStreamStride;        // ���Ķ���Ԫ�ش�С
	uint32 nMaxTextureWidth;        // ����������
	uint32 nMaxTextureHeight;       // ��������߶�
	uint32 nMaxTextureDepth;
	uint32 nMaxTextureCubeSize;
	uint32 nMaxVertexTextures;      // ���Ķ����������

	bool bAlphaTexture : 1;         // �Ƿ�֧��Alpha����
	bool bCubeMap : 1;              // �Ƿ�֧������������
	bool bVolumeMap : 1;            // �Ƿ�֧����������
	bool bMipCubeMap : 1;           // ���������������Ƿ�֧��Mipmap
	bool bMipMap : 1;               // ������ͨ�����Ƿ�֧��Mipmap
	bool bMipVolumeMap : 1;         // �������������Ƿ�֧��Mipmap
	bool bTableFog : 1;             // �Ƿ�֧��������
	bool bVertexFog : 1;            // �Ƿ�֧�ֶ�����

	bool bHWStencil : 1;            // �Ƿ�֧��Ӳ��ģ�建��
	bool bTwoSideStencil : 1;       // �Ƿ�֧��˫��ģ�建��
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_RENDERDEVICE_CAPS_H__