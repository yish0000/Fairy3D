//------------------------------------------------------------------------
// Name: Fly.h
// Desc: This file contain the macros for the engine. Pre-define all the
//       classes for the engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_H__
#define __FLY_H__


//////////////////////////////////////////////////////////////////////////
//// Shutdown some warnings

#pragma warning(disable:4251)
#pragma warning(disable:4244)
#pragma warning(disable:4786)


//////////////////////////////////////////////////////////////////////////
//// BASIC HEADERS FOR THIS FILE.

#include <stdio.h>
#include <tchar.h>
#include <assert.h>
#include <windows.h>
#include <vfw.h>
#include <time.h>
#include <commctrl.h>
#include <d3d9.h>
#include <d3dx9.h>


//////////////////////////////////////////////////////////////////////////
//// Macros of this engine

#ifdef _USRDLL
#define _FLY_EXPORT
#else
#define _FLY_EXPORT
#endif

// Error flags and so on
#define FLY_OK                  0x00000000L

// Just reports no errors
// User close the config dialog.
#define FLY_CANCELED            0x82000000L

// Fail messages
#define FLY_FAIL                0x82000001L

// specific error messages
#define FLY_CREATEAPI           0x82000002L
#define FLY_CREATEDEVICE        0x82000003L
#define FLY_CREATEBUFFER        0x82000004L
#define FLY_INVALIDPARAM        0x82000005L
#define FLY_INVALIDID           0x82000006L
#define FLY_BUFFERSIZE          0x82000007L
#define FLY_BUFFERLOCK          0x82000008L
#define FLY_NOTCOMPATIBLE       0x82000009L
#define FLY_OUTOFMEMORY         0x8200000aL
#define FLY_FILENOTFOUND        0x8200000bL
#define FLY_INVALIDFILE         0x8200000cL
#define FLY_NOSHADERSUPPORT     0x8200000dL
#define FLY_INVALIDVERSION      0x8200000eL
#define FLY_NOVIDEOCARD         0x8200000fL
#define FLY_UNKNOWNFORMAT       0x82000010L
#define FLY_COMPILEEFFECT       0x82000011L
#define FLY_COMPILEHLSL         0x82000012L
#define FLY_NULLSHADER          0x82000013L
#define FLY_NODYNAMICBUFFER     0x82000014L

// Code Helper Macros
#define SAFE_DELETE(p)          { if(p) { delete (p);       (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p)    { if(p) { delete[] (p);     (p) = NULL; } }
#define SAFE_RELEASE(p)         { if(p) { (p)->Release();   (p) = NULL; } }
#define SAFE_FREE(p)            { if(p) { free(p);          (p) = NULL; } }

#define FLY_VERSION( a,b )      (DWORD)((a << 16) | b)

#define MAX_3DHWND              8
#define MAX_TEXTURES            8
#define MAX_LIGHTS              8

#define MAX_ID                  65535

#define MAKE3DCOLOR( r,g,b,a )  (DWORD)( ((a)<<24) | ((r)<<16) | ((g)<<8) | (b) )
#define CONVERT2DWORD( f )      (DWORD)( *((DWORD*)(&f)) )


//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_H__