//------------------------------------------------------------------------
// Name: etCallbackFunc.h
// Desc: This file define all the CALLBACK functions for the dialogs.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_CALLBACKFUNC_H__
#define __ET_CALLBACKFUNC_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include <windows.h>
#include <vfw.h>
#include <string>
#include <vector>

#include "res/resource.h"
//////////////////////////////////////////////////////////////////////////


struct cbSkyboxNode
{
    std::string sName;
    std::string sFrontFace;
    std::string sBackFace;
    std::string sLeftFace;
    std::string sRightFace;
    std::string sTopFace;
    std::string sBottomFace;
};

struct cbVector
{
    float x,y,z,w;

    cbVector() { x = y = z = 0.0f; w = 1.0f; }
};

struct cbSpriteTemplate
{
    bool bFromXML;
    std::string sName;
    float fWidth;
    float fHeight;
    float fFPS;
    bool bBillboardY;
    std::vector<std::string> Textures;
};

struct cbLiquidData
{
    bool bEditLiquid;

    std::string sName;
    cbVector vPosition;
    float fWidth;
    float fHeight;
    bool bReflection;
    std::string sSurfaceMap;
    std::string sDistortMap;
    float fTexScale;
    float fDistortion;
    float fFlowSpeed;
    bool bReflectTerrain;
    bool bReflectEntity;
    bool bReflectSkybox;
};

// Callback functions for the editor.
INT_PTR CALLBACK Callback_About( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_PreviewTex( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_TileTex( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_ApplyAll( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_SetFog( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_LightMode( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_Config( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_EditSky( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_Skybox( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_SetName( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_SetVector( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_Entity( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_MapInfo( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_NewMap( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_AddAnim( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_AddSprite( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_AddVeg( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_EditWater( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_CALLBACKFUNC_H__