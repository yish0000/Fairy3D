//------------------------------------------------------------------------
// Name: CallbackFunc.h
// Desc: This file define a list of CALLBACK functions.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __CALLBACK_FUNC_H__
#define __CALLBACK_FUNC_H__


#include <windows.h>

// Functions list.
INT_PTR CALLBACK Callback_AddPar( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_AddEmitter( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_AddController( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_ModPar( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_ModEmitter( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_ModController( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

INT_PTR CALLBACK Callback_SetKeyframe( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_AddKeyframe( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_ModKeyframe( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

INT_PTR CALLBACK Callback_SelTex( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_SelShape( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_SetVector( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
INT_PTR CALLBACK Callback_About( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __CALLBACK_FUNC_H__