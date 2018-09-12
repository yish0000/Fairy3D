//------------------------------------------------------------------------
// Name: AppKFManager.cpp
// Desc: This file is to implement the class CAppKFManager.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "AppKFManager.h"
#include "CallbackFunc.h"


//------------------------------------------------------------------------
// Create the Keyframe data manager.
// ----------------------------------------------------------------------
// Param -> IN:
//      HINSTANCE:  Handle of the application.
//      HWND:       Handle to the main window.
//------------------------------------------------------------------------
void CAppKFManager::Create( HINSTANCE hInst,HWND hWnd )
{
    m_hInst = hInst;
    m_hWnd  = hWnd;
}


//------------------------------------------------------------------------
// Reset the keyframe arrays.
//------------------------------------------------------------------------
void CAppKFManager::Reset(void)
{
    m_Particle.Init();
    m_Emitter.Init();
}


//------------------------------------------------------------------------
// Show the dialog to modify the size keyframe.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the parent dialog.
//------------------------------------------------------------------------
bool CAppKFManager::ShowKFSize( HWND hWnd )
{
    INT nResult;
    KFDIALOG dlg = { this,GRAPH_SIZE };
    nResult = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_KEYFRAME,hWnd,Callback_SetKeyframe,
                              (LPARAM)&dlg );

    return (nResult==1);
}


//------------------------------------------------------------------------
// Show the dialog to modify the angle keyframe.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the parent dialog.
//------------------------------------------------------------------------
bool CAppKFManager::ShowKFAngle( HWND hWnd )
{
    INT nResult;
    KFDIALOG dlg = { this,GRAPH_ANGLE };
    nResult = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_KEYFRAME,hWnd,Callback_SetKeyframe,
                              (LPARAM)&dlg );

    return (nResult==1);
}


//------------------------------------------------------------------------
// Show the dialog to modify the alpha keyframe.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the parent dialog.
//------------------------------------------------------------------------
bool CAppKFManager::ShowKFAlpha( HWND hWnd )
{
    INT nResult;
    KFDIALOG dlg = { this,GRAPH_ALPHA };
    nResult = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_KEYFRAME,hWnd,Callback_SetKeyframe,
                              (LPARAM)&dlg );

    return (nResult==1);
}


//------------------------------------------------------------------------
// Show the dialog to modify the color keyframe.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the parent dialog.
//------------------------------------------------------------------------
bool CAppKFManager::ShowKFColor( HWND hWnd )
{
    INT nResult;
    KFDIALOG dlg = { this,GRAPH_COLOR };
    nResult = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_KEYFRAME,hWnd,Callback_SetKeyframe,
                              (LPARAM)&dlg );

    return (nResult==1);
}


//------------------------------------------------------------------------
// Show the dialog to modify the life keyframe.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the parent dialog.
//------------------------------------------------------------------------
bool CAppKFManager::ShowKFLife( HWND hWnd )
{
    INT nResult;
    KFDIALOG dlg = { this,GRAPH_LIFE };
    nResult = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_KEYFRAME,hWnd,Callback_SetKeyframe,
                              (LPARAM)&dlg );

    return (nResult==1);
}


//------------------------------------------------------------------------
// Show the dialog to modify the velocity keyframe.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the parent dialog.
//------------------------------------------------------------------------
bool CAppKFManager::ShowKFVelocity( HWND hWnd )
{
    INT nResult;
    KFDIALOG dlg = { this,GRAPH_VELOCITY };
    nResult = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_KEYFRAME,hWnd,Callback_SetKeyframe,
                              (LPARAM)&dlg );

    return (nResult==1);
}


//------------------------------------------------------------------------
// Show the dialog to modify the velocity direction keyframe.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the parent dialog.
//------------------------------------------------------------------------
bool CAppKFManager::ShowKFVelocityDir( HWND hWnd )
{
    INT nResult;
    KFDIALOG dlg = { this,GRAPH_VELOCITYDIR };
    nResult = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_KEYFRAME,hWnd,Callback_SetKeyframe,
                              (LPARAM)&dlg );

    return (nResult==1);
}


//------------------------------------------------------------------------
// Show the dialog to modify the emit rate keyframe.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the parent dialog.
//------------------------------------------------------------------------
bool CAppKFManager::ShowKFEmitRate( HWND hWnd )
{
    INT nResult;
    KFDIALOG dlg = { this,GRAPH_EMITRATE };
    nResult = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_KEYFRAME,hWnd,Callback_SetKeyframe,
                              (LPARAM)&dlg );

    return (nResult==1);
}


//------------------------------------------------------------------------
// Paint the keyframe data graph.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Destinate window to paint.
//      GRAPH_TYPE: Type of the graph to paint.
//------------------------------------------------------------------------
void CAppKFManager::PaintGraph( HWND hWnd,enum GRAPH_TYPE Type )
{
    int n;
    POINT ptCur,ptNew;
    float fTime1,fTime2;
    float fValue1,fValue2;
    FlyVector v1,v2;
    FLYCOLOR c1,c2;

    HDC hdc = GetDC( hWnd );

    // Create some Pens.
    HPEN hR = CreatePen( PS_SOLID,0,RGB(255,0,0) );
    HPEN hG = CreatePen( PS_SOLID,0,RGB(0,255,0) );
    HPEN hB = CreatePen( PS_SOLID,0,RGB(0,0,255) );

    // First, paint rectangle. Our graphs
    // are in it.
    Rectangle( hdc,20,220,260,340 );

    switch( Type )
    {
    case GRAPH_SIZE:
        for( n=0;n<m_Particle.GetKFSizeArray()->size()-1;n++ )
        {
            SelectObject( hdc,hR );

            fTime1 = ((*m_Particle.GetKFSizeArray())[n]).fTime;
            fValue1 = ((*m_Particle.GetKFSizeArray())[n]).fSize;
            ptCur.x = 20 + 240 * fTime1;
            ptCur.y = 340 - 120 * (fValue1/10.0f);
            MoveToEx( hdc,ptCur.x,ptCur.y,NULL );

            fTime2 = ((*m_Particle.GetKFSizeArray())[n+1]).fTime;
            fValue1 = ((*m_Particle.GetKFSizeArray())[n+1]).fSize;
            ptNew.x = 20 + 240 * fTime2;
            ptNew.y = 340 - 120 * (fValue1/10.0f);
            LineTo( hdc,ptNew.x,ptNew.y );
        }
        break;
    case GRAPH_ANGLE:
        for( n=0;n<m_Particle.GetKFAngleArray()->size()-1;n++ )
        {
            SelectObject( hdc,hR );

            fTime1 = ((*m_Particle.GetKFAngleArray())[n]).fTime;
            fValue1 = ((*m_Particle.GetKFAngleArray())[n]).fAngle;
            ptCur.x = 20 + 240 * fTime1;
            ptCur.y = 280 - 60 * (fValue1/10.0f);
            MoveToEx( hdc,ptCur.x,ptCur.y,NULL );

            fTime2 = ((*m_Particle.GetKFAngleArray())[n+1]).fTime;
            fValue1 = ((*m_Particle.GetKFAngleArray())[n+1]).fAngle;
            ptNew.x = 20 + 240 * fTime2;
            ptNew.y = 280 - 60 * (fValue1/10.0f);
            LineTo( hdc,ptNew.x,ptNew.y );
        }
        break;
    case GRAPH_ALPHA:
        for( n=0;n<m_Particle.GetKFAlphaArray()->size()-1;n++ )
        {
            SelectObject( hdc,hR );

            fTime1 = ((*m_Particle.GetKFAlphaArray())[n]).fTime;
            fValue1 = ((*m_Particle.GetKFAlphaArray())[n]).fAlpha;
            ptCur.x = 20 + 240 * fTime1;
            ptCur.y = 340 - 120 * fValue1;
            MoveToEx( hdc,ptCur.x,ptCur.y,NULL );

            fTime2 = ((*m_Particle.GetKFAlphaArray())[n+1]).fTime;
            fValue1 = ((*m_Particle.GetKFAlphaArray())[n+1]).fAlpha;
            ptNew.x = 20 + 240 * fTime2;
            ptNew.y = 340 - 120 * fValue1;
            LineTo( hdc,ptNew.x,ptNew.y );
        }
        break;
    case GRAPH_COLOR:
        for( n=0;n<m_Particle.GetKFColorArray()->size()-1;n++ )
        {
            fTime1 = ((*m_Particle.GetKFColorArray())[n]).fTime;
            c1 = ((*m_Particle.GetKFColorArray())[n]).cColor;
            fTime2 = ((*m_Particle.GetKFColorArray())[n+1]).fTime;
            c2 = ((*m_Particle.GetKFColorArray())[n+1]).cColor;

            SelectObject( hdc,hR );
            ptCur.x = 20 + 240 * fTime1;
            ptCur.y = 340 - 120 * c1.fR;
            ptNew.x = 20 + 240 * fTime2;
            ptNew.y = 340 - 120 * c2.fR;
            MoveToEx( hdc,ptCur.x,ptCur.y,NULL );
            LineTo( hdc,ptNew.x,ptNew.y );

            SelectObject( hdc,hG );
            ptCur.x = 20 + 240 * fTime1;
            ptCur.y = 340 - 120 * c1.fG;
            ptNew.x = 20 + 240 * fTime2;
            ptNew.y = 340 - 120 * c2.fG;
            MoveToEx( hdc,ptCur.x,ptCur.y,NULL );
            LineTo( hdc,ptNew.x,ptNew.y );

            SelectObject( hdc,hB );
            ptCur.x = 20 + 240 * fTime1;
            ptCur.y = 340 - 120 * c1.fB;
            ptNew.x = 20 + 240 * fTime2;
            ptNew.y = 340 - 120 * c2.fB;
            MoveToEx( hdc,ptCur.x,ptCur.y,NULL );
            LineTo( hdc,ptNew.x,ptNew.y );
        }
        break;
    case GRAPH_LIFE:
        for( n=0;n<m_Emitter.GetKFLifeArray()->size()-1;n++ )
        {
            fTime1 = ((*m_Emitter.GetKFLifeArray())[n]).fTime;
            fValue1 = ((*m_Emitter.GetKFLifeArray())[n]).fLifeMin;
            fTime2 = ((*m_Emitter.GetKFLifeArray())[n+1]).fTime;
            fValue2 = ((*m_Emitter.GetKFLifeArray())[n+1]).fLifeMin;

            SelectObject( hdc,hR );
            ptCur.x = 20 + 240 * fTime1;
            ptCur.y = 340 - 120 * (fValue1/20.0f);
            MoveToEx( hdc,ptCur.x,ptCur.y,NULL );
            ptNew.x = 20 + 240 * fTime2;
            ptNew.y = 340 - 120 * (fValue2/20.0f);
            LineTo( hdc,ptNew.x,ptNew.y );

            fValue1 = ((*m_Emitter.GetKFLifeArray())[n]).fLifeMax;
            fValue2 = ((*m_Emitter.GetKFLifeArray())[n+1]).fLifeMax;

            SelectObject( hdc,hG );
            ptCur.x = 20 + 240 * fTime1;
            ptCur.y = 340 - 120 * (fValue1/20.0f);
            MoveToEx( hdc,ptCur.x,ptCur.y,NULL );
            ptNew.x = 20 + 240 * fTime2;
            ptNew.y = 340 - 120 * (fValue2/20.0f);
            LineTo( hdc,ptNew.x,ptNew.y );
        }
        break;
    case GRAPH_VELOCITY:
        for( n=0;n<m_Emitter.GetKFVelocityArray()->size()-1;n++ )
        {
            fTime1 = ((*m_Emitter.GetKFVelocityArray())[n]).fTime;
            fValue1 = ((*m_Emitter.GetKFVelocityArray())[n]).fVelocityMin;
            fTime2 = ((*m_Emitter.GetKFVelocityArray())[n+1]).fTime;
            fValue2 = ((*m_Emitter.GetKFVelocityArray())[n+1]).fVelocityMin;

            SelectObject( hdc,hR );
            ptCur.x = 20 + 240 * fTime1;
            ptCur.y = 340 - 120 * (fValue1/10.0f);
            MoveToEx( hdc,ptCur.x,ptCur.y,NULL );
            ptNew.x = 20 + 240 * fTime2;
            ptNew.y = 340 - 120 * (fValue2/10.0f);
            LineTo( hdc,ptNew.x,ptNew.y );

            fValue1 = ((*m_Emitter.GetKFVelocityArray())[n]).fVelocityMax;
            fValue2 = ((*m_Emitter.GetKFVelocityArray())[n+1]).fVelocityMax;

            SelectObject( hdc,hG );
            ptCur.x = 20 + 240 * fTime1;
            ptCur.y = 340 - 120 * (fValue1/10.0f);
            MoveToEx( hdc,ptCur.x,ptCur.y,NULL );
            ptNew.x = 20 + 240 * fTime2;
            ptNew.y = 340 - 120 * (fValue2/10.0f);
            LineTo( hdc,ptNew.x,ptNew.y );
        }
        break;
    case GRAPH_VELOCITYDIR:
        for( n=0;n<m_Emitter.GetKFVelocityDirArray()->size()-1;n++ )
        {
            fTime1 = ((*m_Emitter.GetKFVelocityDirArray())[n]).fTime;
            v1 = ((*m_Emitter.GetKFVelocityDirArray())[n]).vVelocityDir;
            fTime2 = ((*m_Emitter.GetKFVelocityDirArray())[n+1]).fTime;
            v2 = ((*m_Emitter.GetKFVelocityDirArray())[n+1]).vVelocityDir;

            SelectObject( hdc,hR );
            ptCur.x = 20 + 240 * fTime1;
            ptCur.y = 280 - 60 * (v1.x/10.0f);
            MoveToEx( hdc,ptCur.x,ptCur.y,NULL );
            ptNew.x = 20 + 240 * fTime2;
            ptNew.y = 280 - 60 * (v2.x/10.0f);
            LineTo( hdc,ptNew.x,ptNew.y );

            SelectObject( hdc,hG );
            ptCur.x = 20 + 240 * fTime1;
            ptCur.y = 280 - 60 * (v1.y/10.0f);
            MoveToEx( hdc,ptCur.x,ptCur.y,NULL );
            ptNew.x = 20 + 240 * fTime2;
            ptNew.y = 280 - 60 * (v2.y/10.0f);
            LineTo( hdc,ptNew.x,ptNew.y );

            SelectObject( hdc,hB );
            ptCur.x = 20 + 240 * fTime1;
            ptCur.y = 280 - 60 * (v1.z/10.0f);
            MoveToEx( hdc,ptCur.x,ptCur.y,NULL );
            ptNew.x = 20 + 240 * fTime2;
            ptNew.y = 280 - 60 * (v2.z/10.0f);
            LineTo( hdc,ptNew.x,ptNew.y );
        }
        break;
    case GRAPH_EMITRATE:
        for( n=0;n<m_Emitter.GetKFEmitRateArray()->size()-1;n++ )
        {
            fTime1 = ((*m_Emitter.GetKFEmitRateArray())[n]).fTime;
            fValue1 = ((*m_Emitter.GetKFEmitRateArray())[n]).fEmitRateMin;
            fTime2 = ((*m_Emitter.GetKFEmitRateArray())[n+1]).fTime;
            fValue2 = ((*m_Emitter.GetKFEmitRateArray())[n+1]).fEmitRateMin;

            SelectObject( hdc,hR );
            ptCur.x = 20 + 240 * fTime1;
            ptCur.y = 340 - 120 * (fValue1/100.0f);
            MoveToEx( hdc,ptCur.x,ptCur.y,NULL );
            ptNew.x = 20 + 240 * fTime2;
            ptNew.y = 340 - 120 * (fValue2/100.0f);
            LineTo( hdc,ptNew.x,ptNew.y );

            fValue1 = ((*m_Emitter.GetKFEmitRateArray())[n]).fEmitRateMax;
            fValue2 = ((*m_Emitter.GetKFEmitRateArray())[n+1]).fEmitRateMax;

            SelectObject( hdc,hG );
            ptCur.x = 20 + 240 * fTime1;
            ptCur.y = 340 - 120 * (fValue1/100.0f);
            MoveToEx( hdc,ptCur.x,ptCur.y,NULL );
            ptNew.x = 20 + 240 * fTime2;
            ptNew.y = 340 - 120 * (fValue2/100.0f);
            LineTo( hdc,ptNew.x,ptNew.y );
        }
        break;
    }

    // Release the resources.
    DeleteObject( hR );
    DeleteObject( hG );
    DeleteObject( hB );
    ReleaseDC( hWnd,hdc );
}