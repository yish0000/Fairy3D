//------------------------------------------------------------------------
// Name: main.cpp
// Desc: This file contain the entry function for the Dynamic-link lib.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifdef _DEBUG
#pragma comment(lib,"NewRenderCore_d.lib")
#else
#pragma comment(lib,"NewRenderCore.lib")
#endif

#pragma comment(lib,"winmm.lib")

#include <windows.h>


HINSTANCE g_hInst;

// Entry function for the module.
BOOL APIENTRY DllMain( HANDLE hModule,DWORD dwReason,LPVOID lpvReseaved )
{
    g_hInst = (HINSTANCE)hModule;

    return TRUE;
}
