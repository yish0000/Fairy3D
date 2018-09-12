//------------------------------------------------------------------------
// Name: main.cpp
// Desc: This file contain the entry function for the application.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "TerrainViewer.h"


INT WINAPI WinMain( HINSTANCE hInst,HINSTANCE,LPSTR,INT )
{
    TerrainViewerApp myApp;

    if( !myApp.Create(hInst) )
        return 0;

    return myApp.Run();
}