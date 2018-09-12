//------------------------------------------------------------------------
// Name: main.cpp
// Desc: This file contain the entry function of the application.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "ParticleViewer.h"


INT WINAPI WinMain( HINSTANCE hInst,HINSTANCE,LPSTR,INT )
{
    ParticleViewerApp myApp;

    if( !myApp.Create( hInst ) )
        return 0;

    return myApp.Run();
}