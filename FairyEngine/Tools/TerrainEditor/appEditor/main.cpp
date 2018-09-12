//------------------------------------------------------------------------
// Name: main.cpp
// Desc: This file contain the entry function for the application.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#pragma comment(lib,"vfw32.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"etCore.lib")

#include "etApplication.h"


INT WINAPI WinMain( HINSTANCE hInst,HINSTANCE,LPSTR,INT )
{
    etApplication myApp;

    if( !myApp.Create(hInst) )
        return 0;

    return myApp.Run();
}