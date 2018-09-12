//------------------------------------------------------------------------
// Name: main.cpp
// Desc: This application is to test the shader effect.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "TestShader.h"


INT WINAPI WinMain( HINSTANCE hInst,HINSTANCE,LPSTR,INT )
{
    TestShaderApp myApp;

    if( !myApp.Create(hInst) )
        return 0;

    return myApp.Run();
}