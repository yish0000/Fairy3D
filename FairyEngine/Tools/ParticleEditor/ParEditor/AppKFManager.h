//------------------------------------------------------------------------
// Name: AppKFManager.h
// Desc: This file define a class to manage the Particle system's 
//       Keyframe datas.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __APPKF_MANAGER_H__
#define __APPKF_MANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"
#include "FlyParticleSystem.h"

#include "Res/resource.h"
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Define some enumeration and structures.

// Type enumeration for the graph.
enum GRAPH_TYPE
{
    GRAPH_SIZE,
    GRAPH_ANGLE,
    GRAPH_ALPHA,
    GRAPH_COLOR,
    GRAPH_LIFE,
    GRAPH_VELOCITY,
    GRAPH_VELOCITYDIR,
    GRAPH_EMITRATE,
};

class CAppKFManager;

struct KFDIALOG
{
    CAppKFManager* pKF;
    GRAPH_TYPE Type;
};

struct MODKFDIALOG
{
    GRAPH_TYPE Type;
    LPVOID pKFData;
    int nIndex;
};

//////////////////////////////////////////////////////////////////////////
// Define the class.

class CAppKFManager
{
protected:
    HINSTANCE   m_hInst;
    HWND        m_hWnd;

public:
    // Constructor and destructor.
    CAppKFManager(void)  { /* Nothing to do! */ };
    ~CAppKFManager(void) { /* Nothing to do! */ };

    FlyParticle m_Particle;
    FlyParticleEmitter m_Emitter;

    void Create( HINSTANCE hInst,HWND hWnd );
    void Reset(void);

    bool ShowKFSize( HWND hWnd );
    bool ShowKFAngle( HWND hWnd );
    bool ShowKFAlpha( HWND hWnd );
    bool ShowKFColor( HWND hWnd );

    bool ShowKFVelocity( HWND hWnd );
    bool ShowKFVelocityDir( HWND hWnd );
    bool ShowKFLife( HWND hWnd );
    bool ShowKFEmitRate( HWND hWnd );

    void PaintGraph( HWND hWnd,GRAPH_TYPE Type );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __APPKF_MANAGER_H__