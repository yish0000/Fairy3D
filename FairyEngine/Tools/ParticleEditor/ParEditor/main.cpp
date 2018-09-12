//------------------------------------------------------------------------
// Name: main.cpp
// Desc: This file contain the entry function of the application.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


// Link some static library.
#pragma comment(lib,"FlyMain.lib")
#pragma comment(lib,"uiHelper.lib")


#include "Application.h"


// Main function of the application.
INT WINAPI WinMain( HINSTANCE hInst,HINSTANCE,LPSTR,INT )
{
    CApplication myApp;

    if( !myApp.Create(hInst) )
        return 0;

    return myApp.Run();
}