/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9Common.h
 *  Desc:   ���ļ���D3D9��Ⱦϵͳ�Ĺ���ͷ�ļ���������һЩ��Ҫ����Ϣ��
 *  Author: Yish
 *  Date:   2010/8/15
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FD3D9_COMMON_H__
#define __FD3D9_COMMON_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

#include <d3d9.h>
#include <d3dx9.h>

///////////////////////////////////////////////////////////////////////////

#if FAIRY_PLATFORM != FAIRY_PLATFORM_WINDOWS
    #error D3D9 render module only can be compiled on Windows platform!
#endif

///////////////////////////////////////////////////////////////////////////

#define TODWORD( fl ) (DWORD)( *((DWORD*)(&fl)) )
#define TOFLOAT( dw ) (float)( *((float*)(&dw)) )

// �ͷ�COM�ӿ�
#define SAFE_RELEASE( p ) if( p ) { (p)->Release(); (p) = NULL; }

///////////////////////////////////////////////////////////////////////////
// ��ģ���������Ԥ����

class FD3D9Adapter;
class FD3D9ConfigDialog;
class FD3D9Enumerator;
class FD3D9RenderDevice;
class FD3D9Renderer;
class FD3D9RenderWindow;
class FD3D9Resource;
class FD3D9Texture;
class FD3D9ShaderObject;
class FD3D9VertexBuffer;
class FD3D9IndexBuffer;
class FD3D9PixelBuffer;

///////////////////////////////////////////////////////////////////////////
// Global variables.

extern FD3D9RenderDevice* g_pDevice;

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FD3D9_COMMON_H__