//------------------------------------------------------------------------
// Name: main.cpp
// Desc: This application is to test the light and shadow system.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "LightShadowApp.h"


INT WINAPI WinMain( HINSTANCE hInst,HINSTANCE,LPSTR,INT )
{
    LightShadowApp myApp;

    if( !myApp.Create(hInst) )
        return 0;

    return myApp.Run();
}