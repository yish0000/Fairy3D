//------------------------------------------------------------------------
// Name: CallbackFunc.cpp
// Desc: This file is to implement the CALLBACK functions.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "CallbackFunc.h"
#include "Application.h"


extern CApplication* g_pApp;

//------------------------------------------------------------------------
// CALLBACK Function for the dialog to show about infos.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_About( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    switch( uMsg )
    {
    case WM_MOVING:
    case WM_SIZING:
        g_pApp->ProgramTick();
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        if( wParam == IDOK )
        {
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}

//------------------------------------------------------------------------
// CALLBACK Function for the dialog to add particle template.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_AddPar( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    char cName[40];
    char cTexName[80];
    char cTemp[40];
    int nNumAnims,nNumFrames;
    float fGravity;
    float fMinSpeed,fResistValue;
    bool bUseResist;
    FlyParticle* pPar;

    // Get the handle of controls.
    HWND hName = GetDlgItem( hWnd,IDC_PAR_NAME );
    HWND hTexName = GetDlgItem( hWnd,IDC_PAR_TEXNAME );
    HWND hNumAnim = GetDlgItem( hWnd,IDC_PAR_ANIMNUM );
    HWND hNumFrame = GetDlgItem( hWnd,IDC_PAR_NUMFRAME );
    HWND hGravity = GetDlgItem( hWnd,IDC_PAR_GRAVITY );
    HWND hUseResist = GetDlgItem( hWnd,IDC_PAR_AIRRESIST );
    HWND hMinSpeed = GetDlgItem( hWnd,IDC_PAR_MINSPEED );
    HWND hResistVal = GetDlgItem( hWnd,IDC_PAR_RESISTVAL );

    CAppUIManager* pUI = g_pApp->GetUIManager();
    CAppKFManager* pKF = g_pApp->GetKFManager();
    FlyParticleSystem* pSystem = g_pApp->GetAppRenderer()->GetParticleSystem();

    switch( uMsg )
    {
    case WM_MOVING:
    case WM_SIZING:
        g_pApp->ProgramTick();
        break;
    case WM_INITDIALOG:
        SendMessage( hName,EM_LIMITTEXT,(WPARAM)40,0 );
        SendMessage( GetDlgItem(hWnd,IDC_PAR_SPIN1),UDM_SETRANGE,0,MAKELONG(100,0) );
        SendMessage( GetDlgItem(hWnd,IDC_PAR_SPIN2),UDM_SETRANGE,0,MAKELONG(100,0) );
        SendMessage( GetDlgItem(hWnd,IDC_PAR_SPIN3),UDM_SETRANGE,0,MAKELONG(100,-100) );
        SendMessage( GetDlgItem(hWnd,IDC_PAR_SPIN4),UDM_SETRANGE,0,MAKELONG(100,0) );
        SendMessage( GetDlgItem(hWnd,IDC_PAR_SPIN5),UDM_SETRANGE,0,MAKELONG(100,-100) );
        SetWindowText( hGravity,"9.81" );

        EnableWindow( hMinSpeed,FALSE );
        EnableWindow( GetDlgItem(hWnd,IDC_PAR_SPIN4),FALSE );
        EnableWindow( hResistVal,FALSE );
        EnableWindow( GetDlgItem(hWnd,IDC_PAR_SPIN5),FALSE );
        g_pApp->GetKFManager()->Reset();
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        switch( wParam )
        {
        case IDC_PAR_SELTEX:
            if( pUI->ShowSelTextureDlg(hWnd,cTexName) )
                SetWindowText( hTexName,cTexName );
            break;
        case IDC_PAR_AIRRESIST:
            if( SendMessage(hUseResist,BM_GETCHECK,0,0) == BST_CHECKED )
            {
                EnableWindow( hMinSpeed,TRUE );
                EnableWindow( GetDlgItem(hWnd,IDC_PAR_SPIN4),TRUE );
                EnableWindow( hResistVal,TRUE );
                EnableWindow( GetDlgItem(hWnd,IDC_PAR_SPIN5),TRUE );
            }
            else
            {
                EnableWindow( hMinSpeed,FALSE );
                EnableWindow( GetDlgItem(hWnd,IDC_PAR_SPIN4),FALSE );
                EnableWindow( hResistVal,FALSE );
                EnableWindow( GetDlgItem(hWnd,IDC_PAR_SPIN5),FALSE );
            }
            break;
        case IDC_PAR_SIZEDATA:
            g_pApp->GetKFManager()->ShowKFSize( hWnd );
            break;
        case IDC_PAR_ANGLEDATA:
            g_pApp->GetKFManager()->ShowKFAngle( hWnd );
            break;
        case IDC_PAR_ALPHADATA:
            g_pApp->GetKFManager()->ShowKFAlpha( hWnd );
            break;
        case IDC_PAR_COLORDATA:
            g_pApp->GetKFManager()->ShowKFColor( hWnd );
            break;
        }

        if( wParam == IDOK )
        {
            GetWindowText( hName,cName,40 );
            GetWindowText( hTexName,cTexName,80 );
            GetWindowText( hNumAnim,cTemp,40 );
            sscanf( cTemp,"%d",&nNumAnims );
            GetWindowText( hNumFrame,cTemp,40 );
            sscanf( cTemp,"%d",&nNumFrames );
            GetWindowText( hGravity,cTemp,40 );
            sscanf( cTemp,"%f",&fGravity );

            if( strcmp(cName,"") == 0 )
            {
                MessageBox( hWnd,"请输入粒子模板的名字！","Warning",MB_OK|MB_ICONWARNING );
                return 0;
            }
            if( strcmp(cTexName,"") == 0 )
            {
                MessageBox( hWnd,"请为粒子选择一个纹理文件！","Warning",MB_OK|MB_ICONWARNING );
                return 0;
            }
            if( nNumAnims == 0 || nNumFrames == 0 )
            {
                MessageBox( hWnd,"请指定动画数目和每个动画的帧数！","Warning",MB_OK|MB_ICONWARNING );
                return 0;
            }

            pSystem->GetSceneNode(0)->AddParticle( cName,cTexName,nNumAnims,nNumFrames );
            pPar = pSystem->GetSceneNode(0)->GetParticle( cName );
            pPar->SetGravity( fGravity );
            pKF->m_Particle.CopyKeyframes( pPar );

            bUseResist = (SendMessage(hUseResist,BM_GETCHECK,0,0)==BST_CHECKED);
            if( bUseResist )
            {
                GetWindowText( hMinSpeed,cTemp,40 );
                sscanf( cTemp,"%f",&fMinSpeed );
                GetWindowText( hResistVal,cTemp,40 );
                sscanf( cTemp,"%f",&fResistValue );
                pPar->UseResist( true );
                pPar->SetMinSpeedResist( fMinSpeed );
                pPar->SetResistValue( fResistValue );
            }

            pUI->GetUIManager()->AddItem( IDC_PARLIST,cName,NULL );
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
    }

    return 0;
}


//------------------------------------------------------------------------
// CALLBACK Function for the dialog to modify the particle template.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle of the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_ModPar( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    char cTexName[80];
    char cTemp[40];
    int nNumAnims,nNumFrames;
    float fGravity;
    float fMinSpeed,fResistValue;
    bool bUseResist;

    FlyParticle* pPar = (FlyParticle*)GetWindowLong( hWnd,GWL_USERDATA );

    CAppKFManager* pKF = g_pApp->GetKFManager();
    CAppUIManager* pUI = g_pApp->GetUIManager();
    FlyParticleSystem* pSystem = g_pApp->GetAppRenderer()->GetParticleSystem();

    switch( uMsg )
    {
    case WM_MOVING:
    case WM_SIZING:
        g_pApp->ProgramTick();
        break;
    case WM_INITDIALOG:
        SetWindowText( hWnd,"修改粒子模板" );
        pPar = (FlyParticle*)lParam;
        SetWindowLong( hWnd,GWL_USERDATA,lParam );

        EnableWindow( GetDlgItem(hWnd,IDC_PAR_NAME),FALSE );
        EnableWindow( GetDlgItem(hWnd,IDC_PAR_MINSPEED),FALSE );
        EnableWindow( GetDlgItem(hWnd,IDC_PAR_SPIN4),FALSE );
        EnableWindow( GetDlgItem(hWnd,IDC_PAR_RESISTVAL),FALSE );
        EnableWindow( GetDlgItem(hWnd,IDC_PAR_SPIN5),FALSE );
        SendDlgItemMessage( hWnd,IDC_PAR_SPIN1,UDM_SETRANGE,0,MAKELONG(100,0) );
        SendDlgItemMessage( hWnd,IDC_PAR_SPIN2,UDM_SETRANGE,0,MAKELONG(100,0) );
        SendDlgItemMessage( hWnd,IDC_PAR_SPIN3,UDM_SETRANGE,0,MAKELONG(100,-100) );
        SendDlgItemMessage( hWnd,IDC_PAR_SPIN4,UDM_SETRANGE,0,MAKELONG(100,0) );
        SendDlgItemMessage( hWnd,IDC_PAR_SPIN5,UDM_SETRANGE,0,MAKELONG(100,-100) );

        SetDlgItemText( hWnd,IDC_PAR_NAME,pPar->GetName() );
        SetDlgItemText( hWnd,IDC_PAR_TEXNAME,pPar->GetTexName() );
        sprintf( cTemp,"%d",pPar->GetNumAnims() );
        SendDlgItemMessage( hWnd,IDC_PAR_SPIN1,UDM_SETPOS,0,MAKELONG(pPar->GetNumAnims(),0) );
        SetDlgItemText( hWnd,IDC_PAR_ANIMNUM,cTemp );
        sprintf( cTemp,"%d",pPar->GetNumFrames() );
        SendDlgItemMessage( hWnd,IDC_PAR_SPIN2,UDM_SETPOS,0,MAKELONG(pPar->GetNumFrames(),0) );
        SetDlgItemText( hWnd,IDC_PAR_NUMFRAME,cTemp );
        sprintf( cTemp,"%.2f",pPar->GetGravity() );
        SendDlgItemMessage( hWnd,IDC_PAR_SPIN3,UDM_SETPOS,0,MAKELONG((short)pPar->GetGravity(),0) );
        SetDlgItemText( hWnd,IDC_PAR_GRAVITY,cTemp );
        bUseResist = pPar->IsUseResist();
        if( bUseResist )
        {
            SendDlgItemMessage( hWnd,IDC_PAR_AIRRESIST,BM_SETCHECK,BST_CHECKED,0 );
            EnableWindow( GetDlgItem(hWnd,IDC_PAR_MINSPEED),TRUE );
            EnableWindow( GetDlgItem(hWnd,IDC_PAR_SPIN4),TRUE );
            EnableWindow( GetDlgItem(hWnd,IDC_PAR_RESISTVAL),TRUE );
            EnableWindow( GetDlgItem(hWnd,IDC_PAR_SPIN5),TRUE );
            sprintf( cTemp,"%.2f",pPar->GetMinSpeedResist() );
            SendDlgItemMessage( hWnd,IDC_PAR_SPIN4,UDM_SETPOS,0,MAKELONG((short)pPar->GetMinSpeedResist(),0) );
            SetDlgItemText( hWnd,IDC_PAR_MINSPEED,cTemp );
            sprintf( cTemp,"%.2f",pPar->GetResistValue() );
            SendDlgItemMessage( hWnd,IDC_PAR_SPIN5,UDM_SETPOS,0,MAKELONG((short)pPar->GetResistValue(),0) );
            SetDlgItemText( hWnd,IDC_PAR_RESISTVAL,cTemp );
        }

        pPar->CopyKeyframes( &pKF->m_Particle );
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        switch( wParam )
        {
        case IDC_PAR_SELTEX:
            if( pUI->ShowSelTextureDlg(hWnd,cTexName) )
                SetDlgItemText( hWnd,IDC_PAR_TEXNAME,cTexName );
            break;
        case IDC_PAR_AIRRESIST:
            if( SendDlgItemMessage(hWnd,IDC_PAR_AIRRESIST,BM_GETCHECK,0,0) == BST_CHECKED )
            {
                EnableWindow( GetDlgItem(hWnd,IDC_PAR_MINSPEED),TRUE );
                EnableWindow( GetDlgItem(hWnd,IDC_PAR_SPIN4),TRUE );
                EnableWindow( GetDlgItem(hWnd,IDC_PAR_RESISTVAL),TRUE );
                EnableWindow( GetDlgItem(hWnd,IDC_PAR_SPIN5),TRUE );
            }
            else
            {
                EnableWindow( GetDlgItem(hWnd,IDC_PAR_MINSPEED),FALSE );
                EnableWindow( GetDlgItem(hWnd,IDC_PAR_SPIN4),FALSE );
                EnableWindow( GetDlgItem(hWnd,IDC_PAR_RESISTVAL),FALSE );
                EnableWindow( GetDlgItem(hWnd,IDC_PAR_SPIN5),FALSE );
            }
            break;
        case IDC_PAR_SIZEDATA:
            g_pApp->GetKFManager()->ShowKFSize( hWnd );
            break;
        case IDC_PAR_ANGLEDATA:
            g_pApp->GetKFManager()->ShowKFAngle( hWnd );
            break;
        case IDC_PAR_ALPHADATA:
            g_pApp->GetKFManager()->ShowKFAlpha( hWnd );
            break;
        case IDC_PAR_COLORDATA:
            g_pApp->GetKFManager()->ShowKFColor( hWnd );
            break;
        }

        if( wParam == IDOK )
        {
            GetDlgItemText( hWnd,IDC_PAR_TEXNAME,cTexName,80 );
            if( strcmp(pPar->GetTexName(),cTexName) != 0 )
            {
                pSystem->GetSceneNode(0)->ChangeTextureForPar( pPar->GetName(),cTexName );
            }
            GetDlgItemText( hWnd,IDC_PAR_ANIMNUM,cTemp,40 );
            sscanf( cTemp,"%d",&nNumAnims );
            GetDlgItemText( hWnd,IDC_PAR_NUMFRAME,cTemp,40 );
            sscanf( cTemp,"%d",&nNumFrames );
            if( nNumAnims == 0 || nNumFrames == 0 )
            {
                MessageBox( hWnd,"请指定动画数目和每个动画的帧数！","Warning",MB_OK|MB_ICONWARNING );
                return 0;
            }
            pPar->SetNumAnims( nNumAnims );
            pPar->SetNumFrames( nNumFrames );
            GetDlgItemText( hWnd,IDC_PAR_GRAVITY,cTemp,40 );
            sscanf( cTemp,"%f",&fGravity );
            pPar->SetGravity( fGravity );

            bUseResist = (SendDlgItemMessage(hWnd,IDC_PAR_AIRRESIST,BM_GETCHECK,0,0)==BST_CHECKED);
            pPar->UseResist( bUseResist );
            if( bUseResist )
            {
                GetDlgItemText( hWnd,IDC_PAR_MINSPEED,cTemp,40 );
                sscanf( cTemp,"%f",&fMinSpeed );
                pPar->SetMinSpeedResist( fMinSpeed );
                GetDlgItemText( hWnd,IDC_PAR_RESISTVAL,cTemp,40 );
                sscanf( cTemp,"%f",&fResistValue );
                pPar->SetResistValue( fResistValue );
            }

            pKF->m_Particle.CopyKeyframes( pPar );
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// CALLBACK Function for the dialog to add particle emitter.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_AddEmitter( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    char cName[40];
    char cShapeName[80];
    char cTemp[40];
    FlyVector vPos;
    EMITTERSHAPE shape;
    FlyVector vMinCube,vMaxCube;
    float fHot,fAttenuation;
    float fLife;
    float fRandVelX,fRandVelY,fRandVelZ;
    float fEmitScale;
    bool bTrack = false;
    float fTrackTime;
    char cTrackName[80];
    FlyParticleEmitter* pEmitter;

    // Get handles of controls.
    HWND hName = GetDlgItem( hWnd,IDC_EMITTER_NAME );
    HWND hPos = GetDlgItem( hWnd,IDC_EMITTER_POSITION );
    HWND hMinCube = GetDlgItem( hWnd,IDC_EMITTER_MINPOS );
    HWND hMaxCube = GetDlgItem( hWnd,IDC_EMITTER_MAXPOS );
    HWND hHot = GetDlgItem( hWnd,IDC_EMITTER_HOT );
    HWND hAttenuation = GetDlgItem( hWnd,IDC_EMITTER_ATTENUATION );
    HWND hShapeName = GetDlgItem( hWnd,IDC_EMITTER_SHAPEFILE );
    HWND hLife = GetDlgItem( hWnd,IDC_EMITTER_LIFE );
    HWND hRandVelX = GetDlgItem( hWnd,IDC_EMITTER_RANDVELX );
    HWND hRandVelY = GetDlgItem( hWnd,IDC_EMITTER_RANDVELY );
    HWND hRandVelZ = GetDlgItem( hWnd,IDC_EMITTER_RANDVELZ );
    HWND hEmitScale = GetDlgItem( hWnd,IDC_EMITTER_EMITSCALE );

    CAppUIManager* pUI = g_pApp->GetUIManager();
    CAppKFManager* pKF = g_pApp->GetKFManager();
    FlyParticleSystem* pSystem = g_pApp->GetAppRenderer()->GetParticleSystem();

    switch( uMsg )
    {
    case WM_MOVING:
    case WM_SIZING:
        g_pApp->ProgramTick();
        break;
    case WM_INITDIALOG:
        SendMessage( GetDlgItem(hWnd,IDC_EMITTER_CUBE),BM_SETCHECK,BST_CHECKED,0 );
        EnableWindow( hMinCube,TRUE );
        EnableWindow( hMaxCube,TRUE );
        EnableWindow( hHot,FALSE );
        EnableWindow( hAttenuation,FALSE );
        EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SELFILE1),FALSE );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN1,UDM_SETRANGE,0,MAKELONG(1000,0) );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN5,UDM_SETRANGE,0,MAKELONG(500,0) );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN6,UDM_SETRANGE,0,MAKELONG(500,0) );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN7,UDM_SETRANGE,0,MAKELONG(500,0) );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN8,UDM_SETRANGE,0,MAKELONG(20,0) );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN9,UDM_SETRANGE,0,MAKELONG(1000,0) );
        SetDlgItemText( hWnd,IDC_EMITTER_EMITSCALE,"1.0" );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN9,UDM_SETPOS,0,MAKELONG(1,0) );
        SetDlgItemText( hWnd,IDC_EMITTER_HOT,"0.0" );
        SetDlgItemText( hWnd,IDC_EMITTER_ATTENUATION,"0.0" );
        EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_TRACKCYCLE),FALSE );
        EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SPIN9),FALSE );
        EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SELFILE2),FALSE );
        g_pApp->GetKFManager()->Reset();
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        switch( wParam )
        {
        case IDC_EMITTER_POSITION:
            GetWindowText( hPos,cTemp,40 );
            sscanf( cTemp,"(%f,%f,%f)",&vPos.x,&vPos.y,&vPos.z );
            if( pUI->ShowVectorSetting(hWnd,&vPos) )
            {
                sprintf( cTemp,"(%.3f,%.3f,%.3f)",vPos.x,vPos.y,vPos.z );
                SetWindowText( hPos,cTemp );
            }
            break;
        case IDC_EMITTER_CUBE:
            EnableWindow( hMinCube,TRUE );
            EnableWindow( hMaxCube,TRUE );
            EnableWindow( hHot,FALSE );
            EnableWindow( hAttenuation,FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SELFILE1),FALSE );
            break;
        case IDC_EMITTER_SPHERE:
            EnableWindow( hMinCube,FALSE );
            EnableWindow( hMaxCube,FALSE );
            EnableWindow( hHot,TRUE );
            EnableWindow( hAttenuation,TRUE );
            EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SELFILE1),FALSE );
            break;
        case IDC_EMITTER_USER:
            EnableWindow( hMinCube,FALSE );
            EnableWindow( hMaxCube,FALSE );
            EnableWindow( hHot,FALSE );
            EnableWindow( hAttenuation,FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SELFILE1),TRUE );
            break;
        case IDC_EMITTER_MINPOS:
            GetWindowText( hMinCube,cTemp,40 );
            sscanf( cTemp,"(%f,%f,%f)",&vMinCube.x,&vMinCube.y,&vMinCube.z );
            if( pUI->ShowVectorSetting(hWnd,&vMinCube) )
            {
                sprintf( cTemp,"(%.2f,%.2f,%.2f)",vMinCube.x,vMinCube.y,vMinCube.z );
                SetWindowText( hMinCube,cTemp );
            }
            break;
        case IDC_EMITTER_MAXPOS:
            GetWindowText( hMaxCube,cTemp,40 );
            sscanf( cTemp,"(%f,%f,%f)",&vMaxCube.x,&vMaxCube.y,&vMaxCube.z );
            if( pUI->ShowVectorSetting(hWnd,&vMaxCube) )
            {
                sprintf( cTemp,"(%.2f,%.2f,%.2f)",vMaxCube.x,vMaxCube.y,vMaxCube.z );
                SetWindowText( hMaxCube,cTemp );
            }
            break;
        case IDC_EMITTER_SELFILE1:
            if( g_pApp->GetUIManager()->ShowSelShapeDlg(hWnd,cShapeName) )
                SetWindowText( hShapeName,cShapeName );
            break;
        case IDC_EMITTER_LIFEDATA:
            g_pApp->GetKFManager()->ShowKFLife( hWnd );
            break;
        case IDC_EMITTER_VELDATA:
            g_pApp->GetKFManager()->ShowKFVelocity( hWnd );
            break;
        case IDC_EMITTER_VELDIRDATA:
            g_pApp->GetKFManager()->ShowKFVelocityDir( hWnd );
            break;
        case IDC_EMITTER_EMITRATEDATA:
            g_pApp->GetKFManager()->ShowKFEmitRate( hWnd );
            break;
        case IDC_EMITTER_ENABLETRACK:
            if( SendDlgItemMessage(hWnd,IDC_EMITTER_ENABLETRACK,BM_GETCHECK,0,0) ==
                BST_CHECKED )
            {
                EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_TRACKCYCLE),TRUE );
                EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SPIN9),TRUE );
                EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SELFILE2),TRUE );
            }
            else
            {
                EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_TRACKCYCLE),FALSE );
                EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SPIN9),FALSE );
                EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SELFILE2),FALSE );
            }
            break;
        case IDC_EMITTER_SELFILE2:
            if( g_pApp->GetUIManager()->ShowSelShapeDlg(hWnd,cShapeName) )
                SetDlgItemText( hWnd,IDC_EMITTER_TRACKFILE,cShapeName );
            break;
        }

        if( wParam == IDOK )
        {
            GetWindowText( hName,cName,40 );
            if( strcmp(cName,"") == 0 )
            {
                MessageBox( NULL,"请为发射器指定名字！","Warning",MB_OK|MB_ICONWARNING );
                return 0;
            }

            GetWindowText( hPos,cTemp,40 );
            sscanf( cTemp,"(%f,%f,%f)",&vPos.x,&vPos.y,&vPos.z );
            GetWindowText( hLife,cTemp,40 );
            sscanf( cTemp,"%f",&fLife );
            GetWindowText( hRandVelX,cTemp,40 );
            sscanf( cTemp,"%f",&fRandVelX );
            GetWindowText( hRandVelY,cTemp,40 );
            sscanf( cTemp,"%f",&fRandVelY );
            GetWindowText( hRandVelZ,cTemp,40 );
            sscanf( cTemp,"%f",&fRandVelZ );
            GetWindowText( hEmitScale,cTemp,40 );
            sscanf( cTemp,"%f",&fEmitScale );

            if( SendDlgItemMessage(hWnd,IDC_EMITTER_CUBE,BM_GETCHECK,0,0) == BST_CHECKED )
            {
                shape = EMS_CUBE;
                GetWindowText( hMinCube,cTemp,40 );
                sscanf( cTemp,"(%f,%f,%f)",&vMinCube.x,&vMinCube.y,&vMinCube.z );
                GetWindowText( hMaxCube,cTemp,40 );
                sscanf( cTemp,"(%f,%f,%f)",&vMaxCube.x,&vMaxCube.y,&vMaxCube.z );
            }
            else if( SendDlgItemMessage(hWnd,IDC_EMITTER_SPHERE,BM_GETCHECK,0,0) == BST_CHECKED )
            {
                shape = EMS_SPHERE;
                GetWindowText( hHot,cTemp,40 );
                sscanf( cTemp,"%f",&fHot );
                GetWindowText( hAttenuation,cTemp,40 );
                sscanf( cTemp,"%f",&fAttenuation );
            }
            else if( SendDlgItemMessage(hWnd,IDC_EMITTER_USER,BM_GETCHECK,0,0) == BST_CHECKED )
            {
                shape = EMS_USERDEFINE;
                GetWindowText( hShapeName,cShapeName,80 );
                if( strcmp(cShapeName,"") == 0 )
                {
                    MessageBox( NULL,"请为发射器指定形状文件！","Warning",MB_OK|MB_ICONWARNING );
                    return 0;
                }
            }

            if( SendDlgItemMessage(hWnd,IDC_EMITTER_ENABLETRACK,BM_GETCHECK,0,0) ==
                BST_CHECKED )
            {
                GetDlgItemText( hWnd,IDC_EMITTER_TRACKFILE,cTrackName,80 );
                if( strcmp(cTrackName,"") == 0 )
                {
                    MessageBox( NULL,"请选择一个轨迹文件！","Warning",MB_OK|MB_ICONWARNING );
                    return 0;
                }

                bTrack = true;
                GetDlgItemText( hWnd,IDC_EMITTER_TRACKCYCLE,cTemp,40 );
                sscanf( cTemp,"%f",&fTrackTime );
            }
            else
            {
                bTrack = false;
                fTrackTime = 0.0f;
            }

            pSystem->GetSceneNode(0)->AddEmitter( cName );
            pEmitter = pSystem->GetSceneNode(0)->GetEmitter( cName );
            pEmitter->SetPosition( vPos );

            pEmitter->SetShape( shape );
            switch( shape )
            {
            case EMS_CUBE:
                pEmitter->SetCubeMinPos( vMinCube );
                pEmitter->SetCubeMaxPos( vMaxCube );
                break;
            case EMS_SPHERE:
                pEmitter->SetSphereHot( fHot );
                pEmitter->SetSphereAttenuation( fAttenuation );
                break;
            case EMS_USERDEFINE:
                pEmitter->LoadShapeFromFile( cShapeName );
                break;
            }

            pEmitter->SetLife( fLife );
            pEmitter->SetRandVelocityX( fRandVelX );
            pEmitter->SetRandVelocityY( fRandVelY );
            pEmitter->SetRandVelocityZ( fRandVelZ );
            pEmitter->SetEmitRateScale( fEmitScale );

            pKF->m_Emitter.CopyKeyframes( pEmitter );

            if( bTrack )
            {
                pEmitter->UseTrack( cTrackName );
                pEmitter->SetTrackCycle( fTrackTime );
            }
            else
            {
                pEmitter->UseTrack( NULL );
            }

            pUI->GetUIManager()->AddItem( IDC_EMITTERLIST,cName,NULL );
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// CALLBACK Function for the dialog to modify the particle emitter.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_ModEmitter( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    char cShapeName[80];
    char cTemp[40];
    FlyVector vPos;
    EMITTERSHAPE shape;
    FlyVector vMinCube,vMaxCube;
    float fHot,fAttenuation;
    float fLife;
    float fRandVelX,fRandVelY,fRandVelZ;
    float fEmitScale;
    bool bTrack;
    float fTrackTime;
    char cTrackName[80];

    // Get handles of controls.
    HWND hPos = GetDlgItem( hWnd,IDC_EMITTER_POSITION );
    HWND hMinCube = GetDlgItem( hWnd,IDC_EMITTER_MINPOS );
    HWND hMaxCube = GetDlgItem( hWnd,IDC_EMITTER_MAXPOS );
    HWND hHot = GetDlgItem( hWnd,IDC_EMITTER_HOT );
    HWND hAttenuation = GetDlgItem( hWnd,IDC_EMITTER_ATTENUATION );
    HWND hShapeName = GetDlgItem( hWnd,IDC_EMITTER_SHAPEFILE );
    HWND hLife = GetDlgItem( hWnd,IDC_EMITTER_LIFE );
    HWND hRandVelX = GetDlgItem( hWnd,IDC_EMITTER_RANDVELX );
    HWND hRandVelY = GetDlgItem( hWnd,IDC_EMITTER_RANDVELY );
    HWND hRandVelZ = GetDlgItem( hWnd,IDC_EMITTER_RANDVELZ );
    HWND hEmitRate = GetDlgItem( hWnd,IDC_EMITTER_EMITSCALE );

    CAppUIManager* pUI = g_pApp->GetUIManager();
    CAppKFManager* pKF = g_pApp->GetKFManager();
    FlyParticleSystem* pSystem = g_pApp->GetAppRenderer()->GetParticleSystem();

    FlyParticleEmitter* pEmitter = (FlyParticleEmitter*)GetWindowLong( hWnd,GWL_USERDATA );

    switch( uMsg )
    {
    case WM_MOVING:
    case WM_SIZING:
        g_pApp->ProgramTick();
        break;
    case WM_INITDIALOG:
        SetWindowText( hWnd,"修改粒子发射器" );
        pEmitter = (FlyParticleEmitter*)lParam;
        SetWindowLong( hWnd,GWL_USERDATA,lParam );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN1,UDM_SETRANGE,0,MAKELONG(1000,0) );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN5,UDM_SETRANGE,0,MAKELONG(500,0) );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN6,UDM_SETRANGE,0,MAKELONG(500,0) );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN7,UDM_SETRANGE,0,MAKELONG(500,0) );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN8,UDM_SETRANGE,0,MAKELONG(20,0) );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN9,UDM_SETRANGE,0,MAKELONG(1000,0) );
        SetDlgItemText( hWnd,IDC_EMITTER_HOT,"0.0" );
        SetDlgItemText( hWnd,IDC_EMITTER_ATTENUATION,"0.0" );

        EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_NAME),FALSE );
        SetDlgItemText( hWnd,IDC_EMITTER_NAME,pEmitter->GetName() );
        vPos = pEmitter->GetPosition();
        sprintf( cTemp,"(%.3f,%.3f,%.3f)",vPos.x,vPos.y,vPos.z );
        SetDlgItemText( hWnd,IDC_EMITTER_POSITION,cTemp );
        fLife = pEmitter->GetLife();
        sprintf( cTemp,"%.2f",fLife );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN1,UDM_SETPOS,0,MAKELONG((short)fLife,0) );
        SetDlgItemText( hWnd,IDC_EMITTER_LIFE,cTemp );
        fRandVelX = pEmitter->GetRandVelocityX();
        sprintf( cTemp,"%.2f",fRandVelX );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN5,UDM_SETPOS,0,MAKELONG((short)fRandVelX,0) );
        SetDlgItemText( hWnd,IDC_EMITTER_RANDVELX,cTemp );
        fRandVelY = pEmitter->GetRandVelocityY();
        sprintf( cTemp,"%.2f",fRandVelY );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN6,UDM_SETPOS,0,MAKELONG((short)fRandVelY,0) );
        SetDlgItemText( hWnd,IDC_EMITTER_RANDVELY,cTemp );
        fRandVelZ = pEmitter->GetRandVelocityZ();
        sprintf( cTemp,"%.2f",fRandVelZ );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN7,UDM_SETPOS,0,MAKELONG((short)fRandVelZ,0) );
        SetDlgItemText( hWnd,IDC_EMITTER_RANDVELZ,cTemp );
        fEmitScale = pEmitter->GetEmitRateScale();
        sprintf( cTemp,"%.2f",fEmitScale );
        SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN8,UDM_SETPOS,0,MAKELONG((short)fEmitScale,0) );
        SetDlgItemText( hWnd,IDC_EMITTER_EMITSCALE,cTemp );

        shape = pEmitter->GetShape();
        switch( shape )
        {
        case EMS_CUBE:
            SendDlgItemMessage( hWnd,IDC_EMITTER_CUBE,BM_SETCHECK,BST_CHECKED,0 );
            EnableWindow( hMinCube,TRUE );
            EnableWindow( hMaxCube,TRUE );
            EnableWindow( hHot,FALSE );
            EnableWindow( hAttenuation,FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SELFILE1),FALSE );
            vMinCube = pEmitter->GetCubeMinPos();
            sprintf( cTemp,"(%.2f,%.2f,%.2f)",vMinCube.x,vMinCube.y,vMinCube.z );
            SetDlgItemText( hWnd,IDC_EMITTER_MINPOS,cTemp );
            vMaxCube = pEmitter->GetCubeMaxPos();
            sprintf( cTemp,"(%.2f,%.2f,%.2f)",vMaxCube.x,vMaxCube.y,vMaxCube.z );
            SetDlgItemText( hWnd,IDC_EMITTER_MAXPOS,cTemp );
            break;
        case EMS_SPHERE:
            SendDlgItemMessage( hWnd,IDC_EMITTER_SPHERE,BM_SETCHECK,BST_CHECKED,0 );
            EnableWindow( hMinCube,FALSE );
            EnableWindow( hMaxCube,FALSE );
            EnableWindow( hHot,TRUE );
            EnableWindow( hAttenuation,TRUE );
            EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SELFILE1),FALSE );
            fHot = pEmitter->GetSphereHot();
            sprintf( cTemp,"%.3f",fHot );
            SetDlgItemText( hWnd,IDC_EMITTER_HOT,cTemp );
            fAttenuation = pEmitter->GetSphereAttenuation();
            sprintf( cTemp,"%.3f",fAttenuation );
            SetDlgItemText( hWnd,IDC_EMITTER_ATTENUATION,cTemp );
            break;
        case EMS_USERDEFINE:
            SendDlgItemMessage( hWnd,IDC_EMITTER_USER,BM_SETCHECK,BST_CHECKED,0 );
            EnableWindow( hMinCube,FALSE );
            EnableWindow( hMaxCube,FALSE );
            EnableWindow( hHot,FALSE );
            EnableWindow( hAttenuation,FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SELFILE1),TRUE );
            SetDlgItemText( hWnd,IDC_EMITTER_SHAPEFILE,"" );
            break;
        }

        pEmitter->CopyKeyframes( &pKF->m_Emitter );

        bTrack = pEmitter->IsUseTrack();
        if( bTrack )
        {
            SendDlgItemMessage( hWnd,IDC_EMITTER_ENABLETRACK,BM_SETCHECK,BST_CHECKED,0 );
            EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_TRACKCYCLE),TRUE );
            EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SPIN9),TRUE );
            EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SELFILE2),TRUE );
            fTrackTime = pEmitter->GetTrackCycle();
            sprintf( cTemp,"%f",fTrackTime );
            SetDlgItemText( hWnd,IDC_EMITTER_TRACKCYCLE,cTemp );
            SendDlgItemMessage( hWnd,IDC_EMITTER_SPIN9,UDM_SETPOS,0,MAKELONG((short)fTrackTime,0) );
        }
        else
        {
            EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_TRACKCYCLE),FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SPIN9),FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SELFILE2),FALSE );
        }
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        switch( wParam )
        {
        case IDC_EMITTER_POSITION:
            GetWindowText( hPos,cTemp,40 );
            sscanf( cTemp,"(%f,%f,%f)",&vPos.x,&vPos.y,&vPos.z );
            if( pUI->ShowVectorSetting(hWnd,&vPos) )
            {
                sprintf( cTemp,"(%.3f,%.3f,%.3f)",vPos.x,vPos.y,vPos.z );
                SetWindowText( hPos,cTemp );
            }
            break;
        case IDC_EMITTER_CUBE:
            EnableWindow( hMinCube,TRUE );
            EnableWindow( hMaxCube,TRUE );
            EnableWindow( hHot,FALSE );
            EnableWindow( hAttenuation,FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SELFILE1),FALSE );
            break;
        case IDC_EMITTER_SPHERE:
            EnableWindow( hMinCube,FALSE );
            EnableWindow( hMaxCube,FALSE );
            EnableWindow( hHot,TRUE );
            EnableWindow( hAttenuation,TRUE );
            EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SELFILE1),FALSE );
            break;
        case IDC_EMITTER_USER:
            EnableWindow( hMinCube,FALSE );
            EnableWindow( hMaxCube,FALSE );
            EnableWindow( hHot,FALSE );
            EnableWindow( hAttenuation,FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SELFILE1),TRUE );
            break;
        case IDC_EMITTER_MINPOS:
            GetWindowText( hMinCube,cTemp,40 );
            sscanf( cTemp,"(%f,%f,%f)",&vMinCube.x,&vMinCube.y,&vMinCube.z );
            if( pUI->ShowVectorSetting(hWnd,&vMinCube) )
            {
                sprintf( cTemp,"(%.2f,%.2f,%.2f)",vMinCube.x,vMinCube.y,vMinCube.z );
                SetWindowText( hMinCube,cTemp );
            }
            break;
        case IDC_EMITTER_MAXPOS:
            GetWindowText( hMaxCube,cTemp,40 );
            sscanf( cTemp,"(%f,%f,%f)",&vMaxCube.x,&vMaxCube.y,&vMaxCube.z );
            if( pUI->ShowVectorSetting(hWnd,&vMaxCube) )
            {
                sprintf( cTemp,"(%.2f,%.2f,%.2f)",vMaxCube.x,vMaxCube.y,vMaxCube.z );
                SetWindowText( hMaxCube,cTemp );
            }
            break;
        case IDC_EMITTER_SELFILE1:
            if( g_pApp->GetUIManager()->ShowSelShapeDlg(hWnd,cShapeName) )
                SetWindowText( hShapeName,cShapeName );
            break;
        case IDC_EMITTER_LIFEDATA:
            g_pApp->GetKFManager()->ShowKFLife( hWnd );
            break;
        case IDC_EMITTER_VELDATA:
            g_pApp->GetKFManager()->ShowKFVelocity( hWnd );
            break;
        case IDC_EMITTER_VELDIRDATA:
            g_pApp->GetKFManager()->ShowKFVelocityDir( hWnd );
            break;
        case IDC_EMITTER_EMITRATEDATA:
            g_pApp->GetKFManager()->ShowKFEmitRate( hWnd );
            break;
        case IDC_EMITTER_ENABLETRACK:
            if( SendDlgItemMessage(hWnd,IDC_EMITTER_ENABLETRACK,BM_GETCHECK,0,0) ==
                BST_CHECKED )
            {
                EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_TRACKCYCLE),TRUE );
                EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SPIN9),TRUE );
                EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SELFILE2),TRUE );
            }
            else
            {
                EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_TRACKCYCLE),FALSE );
                EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SPIN9),FALSE );
                EnableWindow( GetDlgItem(hWnd,IDC_EMITTER_SELFILE2),FALSE );
            }
            break;
        case IDC_EMITTER_SELFILE2:
            if( g_pApp->GetUIManager()->ShowSelShapeDlg(hWnd,cShapeName) )
                SetDlgItemText( hWnd,IDC_EMITTER_TRACKFILE,cShapeName );
            break;
        }

        if( wParam == IDOK )
        {
            GetWindowText( hPos,cTemp,40 );
            sscanf( cTemp,"(%f,%f,%f)",&vPos.x,&vPos.y,&vPos.z );
            GetWindowText( hLife,cTemp,40 );
            sscanf( cTemp,"%f",&fLife );
            GetWindowText( hRandVelX,cTemp,40 );
            sscanf( cTemp,"%f",&fRandVelX );
            GetWindowText( hRandVelY,cTemp,40 );
            sscanf( cTemp,"%f",&fRandVelY );
            GetWindowText( hRandVelZ,cTemp,40 );
            sscanf( cTemp,"%f",&fRandVelZ );
            GetWindowText( hEmitRate,cTemp,40 );
            sscanf( cTemp,"%f",&fEmitScale );

            if( SendDlgItemMessage(hWnd,IDC_EMITTER_CUBE,BM_GETCHECK,0,0) == BST_CHECKED )
            {
                shape = EMS_CUBE;
                GetWindowText( hMinCube,cTemp,40 );
                sscanf( cTemp,"(%f,%f,%f)",&vMinCube.x,&vMinCube.y,&vMinCube.z );
                GetWindowText( hMaxCube,cTemp,40 );
                sscanf( cTemp,"(%f,%f,%f)",&vMaxCube.x,&vMaxCube.y,&vMaxCube.z );
            }
            else if( SendDlgItemMessage(hWnd,IDC_EMITTER_SPHERE,BM_GETCHECK,0,0) == BST_CHECKED )
            {
                shape = EMS_SPHERE;
                GetWindowText( hHot,cTemp,40 );
                sscanf( cTemp,"%f",&fHot );
                GetWindowText( hAttenuation,cTemp,40 );
                sscanf( cTemp,"%f",&fAttenuation );
            }
            else if( SendDlgItemMessage(hWnd,IDC_EMITTER_USER,BM_GETCHECK,0,0) == BST_CHECKED )
            {
                shape = EMS_USERDEFINE;
                GetWindowText( hShapeName,cShapeName,80 );
                if( strcmp(cShapeName,"") == 0 && pEmitter->IsUserShapeEmpty() )
                {
                    MessageBox( NULL,"请为发射器指定形状文件！","Warning",MB_OK|MB_ICONWARNING );
                    return 0;
                }
            }

            if( SendDlgItemMessage(hWnd,IDC_EMITTER_ENABLETRACK,BM_GETCHECK,0,0) ==
                BST_CHECKED )
            {
                GetDlgItemText( hWnd,IDC_EMITTER_TRACKFILE,cTrackName,80 );
                if( strcmp(cTrackName,"") == 0 && pEmitter->IsTrackEmpty() )
                {
                    MessageBox( NULL,"请选择一个轨迹文件！","Warning",MB_OK|MB_ICONWARNING );
                    return 0;
                }

                bTrack = true;
                GetDlgItemText( hWnd,IDC_EMITTER_TRACKCYCLE,cTemp,40 );
                sscanf( cTemp,"%f",&fTrackTime );
            }
            else
            {
                bTrack = false;
                fTrackTime = 0.0f;
            }

            pEmitter->SetPosition( vPos );
            pEmitter->SetShape( shape );
            switch( shape )
            {
            case EMS_CUBE:
                pEmitter->SetCubeMinPos( vMinCube );
                pEmitter->SetCubeMaxPos( vMaxCube );
                break;
            case EMS_SPHERE:
                pEmitter->SetSphereHot( fHot );
                pEmitter->SetSphereAttenuation( fAttenuation );
                break;
            case EMS_USERDEFINE:
                if( strcmp(cShapeName,"") != 0 )
                    pEmitter->LoadShapeFromFile( cShapeName );
                break;
            }
            pEmitter->SetLife( fLife );
            pEmitter->SetRandVelocityX( fRandVelX );
            pEmitter->SetRandVelocityY( fRandVelY );
            pEmitter->SetRandVelocityZ( fRandVelZ );
            pEmitter->SetEmitRateScale( fEmitScale );
            pKF->m_Emitter.CopyKeyframes( pEmitter );

            if( bTrack )
            {
                if( strcmp(cTrackName,"") != 0 )
                    pEmitter->UseTrack( cTrackName );

                pEmitter->SetTrackCycle( fTrackTime );
            }
            else
            {
                pEmitter->UseTrack( NULL );
            }

            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// CALLBACK Function for the dialog to add particle controller.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_AddController( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    char cName[40];
    char cTemp[40];
    bool bLoop,bAlpha;
    int nMaxNum,n;
    int nPar,nEmitter;
    FlyParticleController* pCtrl;

    // Get handles of controls.
    HWND hName = GetDlgItem( hWnd,IDC_CTRL_NAME );
    HWND hParList = GetDlgItem( hWnd,IDC_CTRL_PARLIST );
    HWND hEmitterList = GetDlgItem( hWnd,IDC_CTRL_EMITTERLIST );
    HWND hMaxNum = GetDlgItem( hWnd,IDC_CTRL_MAXPARS );
    HWND hLoop = GetDlgItem( hWnd,IDC_CTRL_LOOP );

    CAppUIManager* pUI = g_pApp->GetUIManager();
    FlyParticleSystem* pSystem = g_pApp->GetAppRenderer()->GetParticleSystem();
    FlyParticleNode* pSceneNode = pSystem->GetSceneNode(0);

    switch( uMsg )
    {
    case WM_MOVING:
    case WM_SIZING:
        g_pApp->ProgramTick();
        break;
    case WM_INITDIALOG:
        SendDlgItemMessage( hWnd,IDC_CTRL_SPIN,UDM_SETRANGE,0,MAKELONG(2000,0) );
        for( n=0;n<pSceneNode->GetNumParTemplates();n++ )
        {
            SendMessage( hParList,CB_ADDSTRING,0,(LPARAM)pSceneNode->GetParticle(n)->GetName() );
        }
        for( n=0;n<pSceneNode->GetNumEmiTemplates();n++ )
        {
            SendMessage( hEmitterList,CB_ADDSTRING,0,(LPARAM)pSceneNode->GetEmitter(n)->GetName() );
        }
        SendMessage( hParList,CB_SETCURSEL,(WPARAM)0,0 );
        SendMessage( hEmitterList,CB_SETCURSEL,(WPARAM)0,0 );
        SetWindowText( hMaxNum,"200" );
        SendMessage( GetDlgItem(hWnd,IDC_CTRL_SPIN),UDM_SETPOS,0,MAKELONG(200,0) );
        SendDlgItemMessage( hWnd,IDC_CTRL_RSADD,BM_SETCHECK,BST_CHECKED,0 );
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        if( wParam == IDOK )
        {
            GetWindowText( hName,cName,40 );
            if( strcmp(cName,"") == 0 )
            {
                MessageBox( NULL,"请为控制器指定名字！","Warning",MB_OK|MB_ICONWARNING );
                return 0;
            }

            nPar = SendMessage( hParList,CB_GETCURSEL,0,0 );
            nEmitter = SendMessage( hEmitterList,CB_GETCURSEL,0,0 );
            GetWindowText( hMaxNum,cTemp,40 );
            sscanf( cTemp,"%d",&nMaxNum );
            if( nMaxNum <= 0 )
            {
                MessageBox( hWnd,"粒子数目必须大于零！","Warning",MB_OK|MB_ICONWARNING );
                return 0;
            }
            if( nMaxNum > 2000 ) nMaxNum = 2000;
            bLoop = (SendMessage(hLoop,BM_GETCHECK,0,0)==BST_CHECKED);
            bAlpha = (SendDlgItemMessage(hWnd,IDC_CTRL_RSBLEND,BM_GETCHECK,0,0)==BST_CHECKED);

            pSceneNode->AddController( cName,nPar,nEmitter,nMaxNum,bAlpha );
            pCtrl = pSceneNode->GetController( cName );
            pCtrl->SetEmitterLoop( bLoop );
            pUI->GetUIManager()->AddItem( IDC_CONTROLLERLIST,cName,NULL );
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// CALLBACK Function for the dialog to modify the controller.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_ModController( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    char cTemp[40];
    bool bLoop,bAlpha;
    int nMaxNum,n;
    int nPar,nEmitter;

    FlyParticleSystem* pSystem = g_pApp->GetAppRenderer()->GetParticleSystem();
    FlyParticleController* pCtrl = (FlyParticleController*)GetWindowLong( hWnd,GWL_USERDATA );

    switch( uMsg )
    {
    case WM_MOVING:
    case WM_SIZING:
        g_pApp->ProgramTick();
        break;
    case WM_INITDIALOG:
        SetWindowText( hWnd,"修改粒子控制器" );
        pCtrl = (FlyParticleController*)lParam;
        SetWindowLong( hWnd,GWL_USERDATA,lParam );

        EnableWindow( GetDlgItem(hWnd,IDC_CTRL_NAME),FALSE );
        SendDlgItemMessage( hWnd,IDC_CTRL_SPIN,UDM_SETRANGE,0,MAKELONG(2000,0) );
        SetDlgItemText( hWnd,IDC_CTRL_NAME,pCtrl->GetName() );
        for( n=0;n<pSystem->GetSceneNode(0)->GetNumParTemplates();n++ )
        {
            if( pCtrl->GetParticlePtr() == pSystem->GetSceneNode(0)->GetParticle(n) )
                nPar = n;

            strcpy( cTemp,pSystem->GetSceneNode(0)->GetParticle(n)->GetName() );
            SendDlgItemMessage( hWnd,IDC_CTRL_PARLIST,CB_ADDSTRING,0,(LPARAM)cTemp );
        }
        SendDlgItemMessage( hWnd,IDC_CTRL_PARLIST,CB_SETCURSEL,(WPARAM)nPar,0 );

        for( n=0;n<pSystem->GetSceneNode(0)->GetNumEmiTemplates();n++ )
        {
            if( pCtrl->GetParticleEmitterPtr() == pSystem->GetSceneNode(0)->GetEmitter(n) )
                nEmitter = n;

            strcpy( cTemp,pSystem->GetSceneNode(0)->GetEmitter(n)->GetName() );
            SendDlgItemMessage( hWnd,IDC_CTRL_EMITTERLIST,CB_ADDSTRING,0,(LPARAM)cTemp );
        }
        SendDlgItemMessage( hWnd,IDC_CTRL_EMITTERLIST,CB_SETCURSEL,(WPARAM)nEmitter,0 );

        sprintf( cTemp,"%d",pCtrl->GetMaxNumParticles() );
        SendDlgItemMessage( hWnd,IDC_CTRL_SPIN,UDM_SETPOS,0,MAKELONG(pCtrl->GetMaxNumParticles(),0) );
        SetDlgItemText( hWnd,IDC_CTRL_MAXPARS,cTemp );
        if( pCtrl->IsEmitterLoop() )
            SendDlgItemMessage( hWnd,IDC_CTRL_LOOP,BM_SETCHECK,(WPARAM)BST_CHECKED,0 );

        if( pCtrl->GetRenderMode() == 0 )
            SendDlgItemMessage( hWnd,IDC_CTRL_RSADD,BM_SETCHECK,BST_CHECKED,0 );
        else
            SendDlgItemMessage( hWnd,IDC_CTRL_RSBLEND,BM_SETCHECK,BST_CHECKED,0 );
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        if( wParam == IDOK )
        {
            nPar = SendDlgItemMessage( hWnd,IDC_CTRL_PARLIST,CB_GETCURSEL,0,0 );
            nEmitter = SendDlgItemMessage( hWnd,IDC_CTRL_EMITTERLIST,CB_GETCURSEL,0,0 );
            GetDlgItemText( hWnd,IDC_CTRL_MAXPARS,cTemp,40 );
            sscanf( cTemp,"%d",&nMaxNum );
            if( nMaxNum <= 0 )
            {
                MessageBox( hWnd,"粒子数目必须大于零！","Warning",MB_OK|MB_ICONWARNING );
                return 0;
            }
            if( nMaxNum > 2000 ) nMaxNum = 2000;
            bLoop = (SendDlgItemMessage(hWnd,IDC_CTRL_LOOP,BM_GETCHECK,0,0)==BST_CHECKED);
            bAlpha = (SendDlgItemMessage(hWnd,IDC_CTRL_RSBLEND,BM_GETCHECK,0,0)==BST_CHECKED);
            pCtrl->SetParticlePtr( pSystem->GetSceneNode(0)->GetParticle(nPar) );
            pCtrl->SetEmitterPtr( pSystem->GetSceneNode(0)->GetEmitter(nEmitter) );
            pCtrl->SetMaxNumParticles( nMaxNum );
            pCtrl->SetEmitterLoop( bLoop );
            if( bAlpha ) pCtrl->SetRenderMode( 1 );
            else pCtrl->SetRenderMode( 0 );
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// CALLBACK Function for the dialog to set vector.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_SetVector( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    char cTemp[80];

    // Get the handle of controls
    HWND hPosX = GetDlgItem( hWnd,IDC_SETVEC_X );
    HWND hPosY = GetDlgItem( hWnd,IDC_SETVEC_Y );
    HWND hPosZ = GetDlgItem( hWnd,IDC_SETVEC_Z );
    HWND hSpinX = GetDlgItem( hWnd,IDC_SETVEC_SPIN1 );
    HWND hSpinY = GetDlgItem( hWnd,IDC_SETVEC_SPIN2 );
    HWND hSpinZ = GetDlgItem( hWnd,IDC_SETVEC_SPIN3 );

    FlyVector* pVec = (FlyVector*)GetWindowLong( hWnd,GWL_USERDATA );

    switch( uMsg )
    {
    case WM_INITDIALOG:
        pVec = (FlyVector*)lParam;
        SetWindowLong( hWnd,GWL_USERDATA,lParam );

        // Initialize the controls.
        SendMessage( hSpinX,UDM_SETRANGE,0,MAKELONG(10000,-10000) );
        SendMessage( hSpinY,UDM_SETRANGE,0,MAKELONG(10000,-10000) );
        SendMessage( hSpinZ,UDM_SETRANGE,0,MAKELONG(10000,-10000) );
        SendMessage( hSpinX,UDM_SETPOS,0,(LPARAM)(int)pVec->x );
        SendMessage( hSpinY,UDM_SETPOS,0,(LPARAM)(int)pVec->y );
        SendMessage( hSpinZ,UDM_SETPOS,0,(LPARAM)(int)pVec->z );

        sprintf( cTemp,"%f",pVec->x );
        SetWindowText( hPosX,cTemp );
        sprintf( cTemp,"%f",pVec->y );
        SetWindowText( hPosY,cTemp );
        sprintf( cTemp,"%f",pVec->z );
        SetWindowText( hPosZ,cTemp );
        break;
    case WM_COMMAND:
        if( wParam == IDOK )
        {
            GetWindowText( hPosX,cTemp,80 );
            sscanf( cTemp,"%f",&pVec->x );
            GetWindowText( hPosY,cTemp,80 );
            sscanf( cTemp,"%f",&pVec->y );
            GetWindowText( hPosZ,cTemp,80 );
            sscanf( cTemp,"%f",&pVec->z );
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// CALLBACK Function for the dialog to select textures.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_SelTex( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    HANDLE hFind;
    WIN32_FIND_DATA data;
    int n;
    char cTemp[40];

    // Handle to the controls.
    HWND hFilelist = GetDlgItem( hWnd,IDC_SELTEX_TEXLIST );

    char* s_cFilename = (char*)GetWindowLong( hWnd,GWL_USERDATA );

    switch( uMsg )
    {
    case WM_SIZING:
    case WM_MOVING:
        g_pApp->ProgramTick();
        break;
    case WM_INITDIALOG:
        s_cFilename = (char*)lParam;
        SetWindowLong( hWnd,GWL_USERDATA,lParam );

        hFind = FindFirstFile( "Texture\\*.bmp",&data );
        if( hFind != INVALID_HANDLE_VALUE )
        {
            SendMessage( hFilelist,LB_ADDSTRING,0,(LPARAM)data.cFileName );

            while( FindNextFile(hFind,&data) )
                SendMessage( hFilelist,LB_ADDSTRING,0,(LPARAM)data.cFileName );
        }
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        if( wParam == IDOK )
        {
            if( SendMessage(hFilelist,LB_GETCURSEL,0,0) == LB_ERR )
            {
                MessageBox( hWnd,"请选择一个文件！","Warning",MB_OK|MB_ICONWARNING );
                return 0;
            }

            n = SendMessage( hFilelist,LB_GETCURSEL,0,0 );
            SendMessage( hFilelist,LB_GETTEXT,(WPARAM)n,(LPARAM)cTemp );
            sprintf( s_cFilename,"Texture\\%s",cTemp );
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// CALLBACK Function for the dialog to select shape files.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_SelShape( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    HANDLE hFind;
    WIN32_FIND_DATA data;
    int n;
    char cTemp[40];
    
    // Handle to the controls.
    HWND hFilelist = GetDlgItem( hWnd,IDC_SELSHP_FILELIST );
    
    char* s_cFilename = (char*)GetWindowLong( hWnd,GWL_USERDATA );
    
    switch( uMsg )
    {
    case WM_SIZING:
    case WM_MOVING:
        g_pApp->ProgramTick();
        break;
    case WM_INITDIALOG:
        s_cFilename = (char*)lParam;
        SetWindowLong( hWnd,GWL_USERDATA,lParam );

        hFind = FindFirstFile( "Tracks\\*.shp",&data );
        if( hFind != INVALID_HANDLE_VALUE )
        {
            SendMessage( hFilelist,LB_ADDSTRING,0,(LPARAM)data.cFileName );
            
            while( FindNextFile(hFind,&data) )
                SendMessage( hFilelist,LB_ADDSTRING,0,(LPARAM)data.cFileName );
        }
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        if( wParam == IDOK )
        {
            if( SendMessage(hFilelist,LB_GETCURSEL,0,0) == LB_ERR )
            {
                MessageBox( hWnd,"请选择一个文件！","Warning",MB_OK|MB_ICONWARNING );
                return 0;
            }
            
            n = SendMessage( hFilelist,LB_GETCURSEL,0,0 );
            SendMessage( hFilelist,LB_GETTEXT,(WPARAM)n,(LPARAM)cTemp );
            sprintf( s_cFilename,"Tracks\\%s",cTemp );
            EndDialog( hWnd,1 );
        }
        
        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }
    
    return 0;
}


//------------------------------------------------------------------------
// CALLBACK Function for the dialog to set the Keyframe data.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_SetKeyframe( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    int n;
    char cTemp[40];
    MODKFDIALOG modkf;

    CAppKFManager* pKF;
    GRAPH_TYPE gpType;
    KFSizeArray* pSize;
    KFAngleArray* pAngle;
    KFAlphaArray* pAlpha;
    KFColorArray* pColor;
    KFLifeArray* pLife;
    KFVelocityArray* pVelocity;
    KFVelocityDirArray* pVelocityDir;
    KFEmitRateArray* pEmitRate;

    // Get the pointer of data.
    if( (KFDIALOG*)GetWindowLong(hWnd,GWL_USERDATA) != NULL )
    {
        pKF = ((KFDIALOG*)GetWindowLong(hWnd,GWL_USERDATA))->pKF;
        gpType = ((KFDIALOG*)GetWindowLong(hWnd,GWL_USERDATA))->Type;

        pSize = pKF->m_Particle.GetKFSizeArray();
        pAngle = pKF->m_Particle.GetKFAngleArray();
        pAlpha = pKF->m_Particle.GetKFAlphaArray();
        pColor = pKF->m_Particle.GetKFColorArray();
        pLife = pKF->m_Emitter.GetKFLifeArray();
        pVelocity = pKF->m_Emitter.GetKFVelocityArray();
        pVelocityDir = pKF->m_Emitter.GetKFVelocityDirArray();
        pEmitRate = pKF->m_Emitter.GetKFEmitRateArray();
    }

    // Get controls' handle.
    HWND hList = GetDlgItem( hWnd,IDC_KEYF_KFLIST );
    HWND hComment = GetDlgItem( hWnd,IDC_KEYF_COMMENT );

    switch( uMsg )
    {
    case WM_MOVING:
    case WM_SIZING:
        g_pApp->ProgramTick();
        break;
    case WM_INITDIALOG:
        pKF = ((KFDIALOG*)lParam)->pKF;
        gpType = ((KFDIALOG*)lParam)->Type;
        pSize = pKF->m_Particle.GetKFSizeArray();
        pAngle = pKF->m_Particle.GetKFAngleArray();
        pAlpha = pKF->m_Particle.GetKFAlphaArray();
        pColor = pKF->m_Particle.GetKFColorArray();
        pLife = pKF->m_Emitter.GetKFLifeArray();
        pVelocity = pKF->m_Emitter.GetKFVelocityArray();
        pVelocityDir = pKF->m_Emitter.GetKFVelocityDirArray();
        pEmitRate = pKF->m_Emitter.GetKFEmitRateArray();
        SetWindowLong( hWnd,GWL_USERDATA,lParam );

        switch( gpType )
        {
        case GRAPH_SIZE:
            SetWindowText( hWnd,"设置关键帧(&Size)" );
            for( n=0;n<pSize->size();n++ )
            {
                sprintf( cTemp,"%f",((*pSize)[n]).fTime );
                SendMessage( hList,LB_ADDSTRING,0,(LPARAM)cTemp );
            }
            SetWindowText( hComment,"红:Size (0.0≤Y≤10.0)" );
            break;
        case GRAPH_ANGLE:
            SetWindowText( hWnd,"设置关键帧(&Angle)" );
            for( n=0;n<pAngle->size();n++ )
            {
                sprintf( cTemp,"%f",((*pAngle)[n]).fTime );
                SendMessage( hList,LB_ADDSTRING,0,(LPARAM)cTemp );
            }
            SetWindowText( hComment,"红:Angle (-10.0≤Y≤10.0)" );
            break;
        case GRAPH_ALPHA:
            SetWindowText( hWnd,"设置关键帧(&Alpha)" );
            for( n=0;n<pAlpha->size();n++ )
            {
                sprintf( cTemp,"%f",((*pAlpha)[n]).fTime );
                SendMessage( hList,LB_ADDSTRING,0,(LPARAM)cTemp );
            }
            SetWindowText( hComment,"红:Alpha (0≤Y≤255)" );
            break;
        case GRAPH_COLOR:
            SetWindowText( hWnd,"设置关键帧(&Color)" );
            for( n=0;n<pColor->size();n++ )
            {
                sprintf( cTemp,"%f",((*pColor)[n]).fTime );
                SendMessage( hList,LB_ADDSTRING,0,(LPARAM)cTemp );
            }
            SetWindowText( hComment,"红:R 绿:G 蓝:B (0≤Y≤255)" );
            break;
        case GRAPH_LIFE:
            SetWindowText( hWnd,"设置关键帧(&Life)" );
            for( n=0;n<pLife->size();n++ )
            {
                sprintf( cTemp,"%f",((*pLife)[n]).fTime );
                SendMessage( hList,LB_ADDSTRING,0,(LPARAM)cTemp );
            }
            SetWindowText( hComment,"红:Life (0.0≤Y≤20.0)" );
            break;
        case GRAPH_VELOCITY:
            SetWindowText( hWnd,"设置关键帧(&Velocity)" );
            for( n=0;n<pVelocity->size();n++ )
            {
                sprintf( cTemp,"%f",((*pVelocity)[n]).fTime );
                SendMessage( hList,LB_ADDSTRING,0,(LPARAM)cTemp );
            }
            SetWindowText( hComment,"红:Velocity (0.0≤Y≤10.0)" );
            break;
        case GRAPH_VELOCITYDIR:
            SetWindowText( hWnd,"设置关键帧(&Velocity Direction)" );
            for( n=0;n<pVelocityDir->size();n++ )
            {
                sprintf( cTemp,"%f",((*pVelocityDir)[n]).fTime );
                SendMessage( hList,LB_ADDSTRING,0,(LPARAM)cTemp );
            }
            SetWindowText( hComment,"红:X 绿:Y 蓝:Z (-10.0≤Y≤10.0)" );
            break;
        case GRAPH_EMITRATE:
            SetWindowText( hWnd,"设置关键帧(&Emit Rate)" );
            for( n=0;n<pEmitRate->size();n++ )
            {
                sprintf( cTemp,"%f",((*pEmitRate)[n]).fTime );
                SendMessage( hList,LB_ADDSTRING,0,(LPARAM)cTemp );
            }
            SetWindowText( hComment,"红:Emit Rate (0≤Y≤200)" );
            break;
        }
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        switch( wParam )
        {
        case IDC_KEYF_ADDKF:
            DialogBoxParam( g_pApp->GetHandle(),(LPCTSTR)IDD_DLG_SETKF,
                            hWnd,Callback_AddKeyframe,
                            GetWindowLong(hWnd,GWL_USERDATA) );
            SendMessage( hWnd,WM_PAINT,0,0 );
            break;
        case IDC_KEYF_DELKF:
            n = SendMessage( hList,LB_GETCURSEL,0,0 );
            if( n != -1 && n!=0 && n!= SendMessage(hList,LB_GETCOUNT,0,0)-1 )
            {
                if( MessageBox(hWnd,"确定要删除么？","Info",MB_YESNO|MB_ICONQUESTION)
                    == IDYES )
                {
                    switch( gpType )
                    {
                    case GRAPH_SIZE:
                        pSize->erase( pSize->begin() + n );
                        break;
                    case GRAPH_ANGLE:
                        pAngle->erase( pAngle->begin() + n );
                        break;
                    case GRAPH_ALPHA:
                        pAlpha->erase( pAlpha->begin() + n );
                        break;
                    case GRAPH_COLOR:
                        pColor->erase( pColor->begin() + n );
                        break;
                    case GRAPH_LIFE:
                        pLife->erase( pLife->begin() + n );
                        break;
                    case GRAPH_VELOCITY:
                        pVelocity->erase( pVelocity->begin() + n );
                        break;
                    case GRAPH_VELOCITYDIR:
                        pVelocityDir->erase( pVelocityDir->begin() + n );
                        break;
                    case GRAPH_EMITRATE:
                        pEmitRate->erase( pEmitRate->begin() + n );
                        break;
                    }

                    SendMessage( hList,LB_DELETESTRING,(WPARAM)n,0 );
                }
            }
            SendMessage( hWnd,WM_PAINT,0,0 );
            break;
        case IDC_KEYF_MODIFYKF:
            n = SendMessage( hList,LB_GETCURSEL,0,0 );
            if( n != -1 )
            {
                switch( gpType )
                {
                case GRAPH_SIZE:
                    modkf.pKFData = (LPVOID)(&((*pSize)[n]));
                    break;
                case GRAPH_ANGLE:
                    modkf.pKFData = (LPVOID)(&((*pAngle)[n]));
                    break;
                case GRAPH_ALPHA:
                    modkf.pKFData = (LPVOID)(&((*pAlpha)[n]));
                    break;
                case GRAPH_COLOR:
                    modkf.pKFData = (LPVOID)(&((*pColor)[n]));
                    break;
                case GRAPH_LIFE:
                    modkf.pKFData = (LPVOID)(&((*pLife)[n]));
                    break;
                case GRAPH_VELOCITY:
                    modkf.pKFData = (LPVOID)(&((*pVelocity)[n]));
                    break;
                case GRAPH_VELOCITYDIR:
                    modkf.pKFData = (LPVOID)(&((*pVelocityDir)[n]));
                    break;
                case GRAPH_EMITRATE:
                    modkf.pKFData = (LPVOID)(&((*pEmitRate)[n]));
                    break;
                }

                modkf.Type = gpType;
                modkf.nIndex = n;
                DialogBoxParam( g_pApp->GetHandle(),(LPCTSTR)IDD_DLG_SETKF,hWnd,Callback_ModKeyframe,
                                (LPARAM)&modkf );
            }
            SendMessage( hWnd,WM_PAINT,0,0 );
            break;
        }
        break;
    case WM_PAINT:
        g_pApp->GetKFManager()->PaintGraph( hWnd,gpType );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// Callback function to add a keyframe.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle of the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_AddKeyframe( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    int n;
    char cTemp[40];
    char cTemp2[40];
    float fTime,fX,fY,fZ;
    FLYCOLOR clr;
    bool bExist = false;

    CAppKFManager* pKF;
    GRAPH_TYPE gpType;

    // Get the handle of controls.
    HWND hTime = GetDlgItem( hWnd,IDC_FK_TIME );
    HWND hSetTime = GetDlgItem( hWnd,IDC_FK_SETTIME );
    HWND hX = GetDlgItem( hWnd,IDC_FK_X );
    HWND hY = GetDlgItem( hWnd,IDC_FK_Y );
    HWND hZ = GetDlgItem( hWnd,IDC_FK_Z );
    HWND hSpin1 = GetDlgItem( hWnd,IDC_FK_SPIN1 );
    HWND hSpin2 = GetDlgItem( hWnd,IDC_FK_SPIN2 );
    HWND hSpin3 = GetDlgItem( hWnd,IDC_FK_SPIN3 );
    HWND hKFList = GetDlgItem( GetParent(hWnd),IDC_KEYF_KFLIST );

    if( (KFDIALOG*)GetWindowLong(hWnd,GWL_USERDATA) )
    {
        pKF = ((KFDIALOG*)GetWindowLong(hWnd,GWL_USERDATA))->pKF;
        gpType = ((KFDIALOG*)GetWindowLong(hWnd,GWL_USERDATA))->Type;
    }

    switch( uMsg )
    {
    case WM_MOVING:
    case WM_SIZING:
        g_pApp->ProgramTick();
        break;
    case WM_INITDIALOG:
        pKF = ((KFDIALOG*)lParam)->pKF;
        gpType = ((KFDIALOG*)lParam)->Type;
        SetWindowLong( hWnd,GWL_USERDATA,lParam );
        SendMessage( hSetTime,TBM_SETRANGE,(WPARAM)TRUE,MAKELONG(0,100) );

        switch( gpType )
        {
        case GRAPH_SIZE:
            SetWindowText( hWnd,"添加关键帧(&Size)" );
            SendMessage( hSpin1,UDM_SETRANGE,0,MAKELONG(100,0) );
            EnableWindow( hY,FALSE );
            EnableWindow( hZ,FALSE );
            EnableWindow( hSpin2,FALSE );
            EnableWindow( hSpin3,FALSE );
            break;
        case GRAPH_ANGLE:
            SetWindowText( hWnd,"添加关键帧(&Angle)" );
            SendMessage( hSpin1,UDM_SETRANGE,0,MAKELONG(100,-100) );
            EnableWindow( hY,FALSE );
            EnableWindow( hZ,FALSE );
            EnableWindow( hSpin2,FALSE );
            EnableWindow( hSpin3,FALSE );
            break;
        case GRAPH_ALPHA:
            SetWindowText( hWnd,"添加关键帧(&Alpha)" );
            SendMessage( hSpin1,UDM_SETRANGE,0,MAKELONG(255,0) );
            EnableWindow( hY,FALSE );
            EnableWindow( hZ,FALSE );
            EnableWindow( hSpin2,FALSE );
            EnableWindow( hSpin3,FALSE );
            break;
        case GRAPH_COLOR:
            SetWindowText( hWnd,"添加关键帧(&Color)" );
            SendMessage( hSpin1,UDM_SETRANGE,0,MAKELONG(255,0) );
            SendMessage( hSpin2,UDM_SETRANGE,0,MAKELONG(255,0) );
            SendMessage( hSpin3,UDM_SETRANGE,0,MAKELONG(255,0) );
            break;
        case GRAPH_LIFE:
            SetWindowText( hWnd,"添加关键帧(&Life)" );
            SendMessage( hSpin1,UDM_SETRANGE,0,MAKELONG(200,0) );
            SendMessage( hSpin2,UDM_SETRANGE,0,MAKELONG(200,0) );
            EnableWindow( hZ,FALSE );
            EnableWindow( hSpin3,FALSE );
            break;
        case GRAPH_VELOCITY:
            SetWindowText( hWnd,"添加关键帧(&Velocity)" );
            SendMessage( hSpin1,UDM_SETRANGE,0,MAKELONG(100,0) );
            SendMessage( hSpin2,UDM_SETRANGE,0,MAKELONG(100,0) );
            EnableWindow( hZ,FALSE );
            EnableWindow( hSpin3,FALSE );
            break;
        case GRAPH_VELOCITYDIR:
            SetWindowText( hWnd,"添加关键帧(&Velocity Direction)" );
            SendMessage( hSpin1,UDM_SETRANGE,0,MAKELONG(100,-100) );
            SendMessage( hSpin2,UDM_SETRANGE,0,MAKELONG(100,-100) );
            SendMessage( hSpin3,UDM_SETRANGE,0,MAKELONG(100,-100) );
            break;
        case GRAPH_EMITRATE:
            SetWindowText( hWnd,"添加关键帧(&Emit Rate)" );
            SendMessage( hSpin1,UDM_SETRANGE,0,MAKELONG(100,0) );
            SendMessage( hSpin2,UDM_SETRANGE,0,MAKELONG(100,0) );
            EnableWindow( hZ,FALSE );
            EnableWindow( hSpin3,FALSE );
            break;
        }
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_HSCROLL:
        // Handle the trackbar.
        if( lParam == (LPARAM)hSetTime )
        {
            n = SendMessage( hSetTime,TBM_GETPOS,0,0 );
            sprintf( cTemp,"时间: %.2f",0.01f*n );
            SetWindowText( hTime,cTemp );
        }

        // Handle the Up-down controls.
        if( LOWORD(wParam) != SB_ENDSCROLL )
        {
            switch( gpType )
            {
            case GRAPH_SIZE:
            case GRAPH_ANGLE:
            case GRAPH_LIFE:
            case GRAPH_VELOCITY:
            case GRAPH_VELOCITYDIR:
                if( lParam == (LPARAM)hSpin1 )
                {
                    n = (short)SendMessage( hSpin1,UDM_GETPOS,0,0 );
                    sprintf( cTemp,"%.2f",(float)(n*0.1f) );
                    SetWindowText( hX,cTemp );
                }
                else if( lParam == (LPARAM)hSpin2 )
                {
                    n = (short)SendMessage( hSpin2,UDM_GETPOS,0,0 );
                    sprintf( cTemp,"%.2f",(float)(n*0.1f) );
                    SetWindowText( hY,cTemp );
                }
                else if( lParam == (LPARAM)hSpin3 )
                {
                    n = (short)SendMessage( hSpin3,UDM_GETPOS,0,0 );
                    sprintf( cTemp,"%.2f",(float)(n*0.1f) );
                    SetWindowText( hZ,cTemp );
                }
                break;
            }
        }
        break;
    case WM_COMMAND:
        if( wParam == IDOK )
        {
            GetWindowText( hTime,cTemp,40 );
            sscanf( cTemp,"时间: %f",&fTime );
            GetWindowText( hX,cTemp,40 );
            sscanf( cTemp,"%f",&fX );
            GetWindowText( hY,cTemp,40 );
            sscanf( cTemp,"%f",&fY );
            GetWindowText( hZ,cTemp,40 );
            sscanf( cTemp,"%f",&fZ );

            switch( gpType )
            {
            case GRAPH_SIZE:
                if( fX < 0.0f ) fX = 0.0f;
                if( fX > 10.0f ) fX = 10.0f;
                pKF->m_Particle.AddKFSize( fTime,fX );
                break;
            case GRAPH_ANGLE:
                if( fX < -10.0f ) fX = -10.0f;
                if( fX > 10.0f ) fX = 10.0f;
                pKF->m_Particle.AddKFAngle( fTime,fX );
                break;
            case GRAPH_ALPHA:
                fX /= 255.0f;
                if( fX < 0.0f ) fX = 0.0f;
                if( fX > 1.0f ) fX = 1.0f;
                pKF->m_Particle.AddKFAlpha( fTime,fX );
                break;
            case GRAPH_COLOR:
                if( fX < 0.0f ) fX = 0.0f;
                if( fY < 0.0f ) fY = 0.0f;
                if( fZ < 0.0f ) fZ = 0.0f;
                if( fX > 255.0f ) fX = 255.0f;
                if( fY > 255.0f ) fY = 255.0f;
                if( fZ > 255.0f ) fZ = 255.0f;
                clr.fR = fX / 255.0f;
                clr.fG = fY / 255.0f;
                clr.fB = fZ / 255.0f;
                clr.fA = 0.0f;
                pKF->m_Particle.AddKFColor( fTime,clr );
                break;
            case GRAPH_LIFE:
                if( fX < 0.0f ) fX = 0.0f;
                if( fY < 0.0f ) fY = 0.0f;
                if( fX > 20.0f ) fX = 20.0f;
                if( fY > 20.0f ) fY = 20.0f;
                pKF->m_Emitter.AddKFLife( fTime,fX,fY );
                break;
            case GRAPH_VELOCITY:
                if( fX < 0.0f ) fX = 0.0f;
                if( fY < 0.0f ) fY = 0.0f;
                if( fX > 10.0f ) fX = 10.0f;
                if( fY > 10.0f ) fY = 10.0f;
                pKF->m_Emitter.AddKFVelocity( fTime,fX,fY );
                break;
            case GRAPH_VELOCITYDIR:
                if( fX < -10.0f ) fX = -10.0f;
                if( fY < -10.0f ) fY = -10.0f;
                if( fZ < -10.0f ) fZ = -10.0f;
                if( fX > 10.0f ) fX = 10.0f;
                if( fY > 10.0f ) fY = 10.0f;
                if( fZ > 10.0f ) fZ = 10.0f;
                pKF->m_Emitter.AddKFVelocityDir( fTime,FlyVector(fX,fY,fZ) );
                break;
            case GRAPH_EMITRATE:
                if( fX < 0.0f ) fX = 0.0f;
                if( fY < 0.0f ) fY = 0.0f;
                if( fX > 100.0f ) fX = 100.0f;
                if( fY > 100.0f ) fY = 100.0f;
                pKF->m_Emitter.AddKFEmitRate( fTime,fX,fY );
                break;
            }

            // Check whether the listBox already contain
            // the item. If not found then add it.
            sprintf( cTemp,"%f",fTime );
            for( n=0;n<SendMessage(hKFList,LB_GETCOUNT,0,0);n++ )
            {
                SendMessage( hKFList,LB_GETTEXT,(WPARAM)n,(LPARAM)cTemp2 );
                if( strcmp(cTemp,cTemp2) == 0 )
                {
                    bExist = true;
                    break;
                }
            }

            if( !bExist ) SendMessage( hKFList,LB_ADDSTRING,0,(LPARAM)cTemp );
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// CALLBACK Funtion to the dialog modify the keyframe.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_ModKeyframe( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    int n;
    char cTemp[40];
    float fTime,fX,fY,fZ;

    MODKFDIALOG* modkf;
    sKeyframe_Size* pSize;
    sKeyframe_Angle* pAngle;
    sKeyframe_Alpha* pAlpha;
    sKeyframe_Color* pColor;
    sKeyframe_Life* pLife;
    sKeyframe_Velocity* pVelocity;
    sKeyframe_VelocityDir* pVelocityDir;
    sKeyframe_EmitRate* pEmitRate;

    // Get the handle of controls.
    HWND hTime = GetDlgItem( hWnd,IDC_FK_TIME );
    HWND hSetTime = GetDlgItem( hWnd,IDC_FK_SETTIME );
    HWND hX = GetDlgItem( hWnd,IDC_FK_X );
    HWND hY = GetDlgItem( hWnd,IDC_FK_Y );
    HWND hZ = GetDlgItem( hWnd,IDC_FK_Z );
    HWND hSpin1 = GetDlgItem( hWnd,IDC_FK_SPIN1 );
    HWND hSpin2 = GetDlgItem( hWnd,IDC_FK_SPIN2 );
    HWND hSpin3 = GetDlgItem( hWnd,IDC_FK_SPIN3 );
    HWND hKFList = GetDlgItem( GetParent(hWnd),IDC_KEYF_KFLIST );

    CAppKFManager* pKF = g_pApp->GetKFManager();
    modkf = (MODKFDIALOG*)GetWindowLong( hWnd,GWL_USERDATA );

    if( modkf )
    {
        switch( modkf->Type )
        {
        case GRAPH_SIZE:
            pSize = (sKeyframe_Size*)modkf->pKFData;
            break;
        case GRAPH_ANGLE:
            pAngle = (sKeyframe_Angle*)modkf->pKFData;
            break;
        case GRAPH_ALPHA:
            pAlpha = (sKeyframe_Alpha*)modkf->pKFData;
            break;
        case GRAPH_COLOR:
            pColor = (sKeyframe_Color*)modkf->pKFData;
            break;
        case GRAPH_LIFE:
            pLife = (sKeyframe_Life*)modkf->pKFData;
            break;
        case GRAPH_VELOCITY:
            pVelocity = (sKeyframe_Velocity*)modkf->pKFData;
            break;
        case GRAPH_VELOCITYDIR:
            pVelocityDir = (sKeyframe_VelocityDir*)modkf->pKFData;
            break;
        case GRAPH_EMITRATE:
            pEmitRate = (sKeyframe_EmitRate*)modkf->pKFData;
            break;
        }
    }

    switch( uMsg )
    {
    case WM_MOVING:
    case WM_SIZING:
        g_pApp->ProgramTick();
        break;
    case WM_INITDIALOG:
        modkf = (MODKFDIALOG*)lParam;
        SetWindowLong( hWnd,GWL_USERDATA,lParam );
        SendMessage( hSetTime,TBM_SETRANGE,(WPARAM)TRUE,MAKELONG(0,100) );

        switch( modkf->Type )
        {
        case GRAPH_SIZE:
            SetWindowText( hWnd,"修改关键帧(&Size)" );
            SendMessage( hSpin1,UDM_SETRANGE,0,MAKELONG(100,0) );
            EnableWindow( hY,FALSE );
            EnableWindow( hZ,FALSE );
            EnableWindow( hSpin2,FALSE );
            EnableWindow( hSpin3,FALSE );
            pSize = (sKeyframe_Size*)modkf->pKFData;
            SendMessage( hSetTime,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)(100*pSize->fTime) );
            sprintf( cTemp,"时间: %.2f",pSize->fTime );
            SetWindowText( hTime,cTemp );
            SendMessage( hSpin1,UDM_SETPOS,0,MAKELONG(10*pSize->fSize,0) );
            sprintf( cTemp,"%.2f",pSize->fSize );
            SetWindowText( hX,cTemp );
            if( pSize->fTime == 0.0f || pSize->fTime == 1.0f ) EnableWindow( hSetTime,FALSE );
            break;
        case GRAPH_ANGLE:
            SetWindowText( hWnd,"修改关键帧(&Angle)" );
            SendMessage( hSpin1,UDM_SETRANGE,0,MAKELONG(100,-100) );
            EnableWindow( hY,FALSE );
            EnableWindow( hZ,FALSE );
            EnableWindow( hSpin2,FALSE );
            EnableWindow( hSpin3,FALSE );
            pAngle = (sKeyframe_Angle*)modkf->pKFData;
            SendMessage( hSetTime,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)(100*pAngle->fTime) );
            sprintf( cTemp,"时间: %.2f",pAngle->fTime );
            SetWindowText( hTime,cTemp );
            SendMessage( hSpin1,UDM_SETPOS,0,MAKELONG(10*pAngle->fAngle,0) );
            sprintf( cTemp,"%.2f",pAngle->fAngle );
            SetWindowText( hX,cTemp );
            if( pAngle->fTime == 0.0f || pAngle->fTime == 1.0f ) EnableWindow( hSetTime,FALSE );
            break;
        case GRAPH_ALPHA:
            SetWindowText( hWnd,"修改关键帧(&Alpha)" );
            SendMessage( hSpin1,UDM_SETRANGE,0,MAKELONG(255,0) );
            EnableWindow( hY,FALSE );
            EnableWindow( hZ,FALSE );
            EnableWindow( hSpin2,FALSE );
            EnableWindow( hSpin3,FALSE );
            pAlpha = (sKeyframe_Alpha*)modkf->pKFData;
            SendMessage( hSetTime,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)(100*pAlpha->fTime) );
            sprintf( cTemp,"时间: %.2f",pAlpha->fTime );
            SetWindowText( hTime,cTemp );
            SendMessage( hSpin1,UDM_SETPOS,0,MAKELONG((short)(pAlpha->fAlpha*255),0) );
            sprintf( cTemp,"%d",(int)(pAlpha->fAlpha*255) );
            SetWindowText( hX,cTemp );
            if( pAlpha->fTime == 0.0f || pAlpha->fTime == 1.0f ) EnableWindow( hSetTime,FALSE );
            break;
        case GRAPH_COLOR:
            SetWindowText( hWnd,"修改关键帧(&Color)" );
            SendMessage( hSpin1,UDM_SETRANGE,0,MAKELONG(255,0) );
            SendMessage( hSpin2,UDM_SETRANGE,0,MAKELONG(255,0) );
            SendMessage( hSpin3,UDM_SETRANGE,0,MAKELONG(255,0) );
            pColor = (sKeyframe_Color*)modkf->pKFData;
            SendMessage( hSetTime,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)(100*pColor->fTime) );
            sprintf( cTemp,"时间: %.2f",pColor->fTime );
            SetWindowText( hTime,cTemp );
            SendMessage( hSpin1,UDM_SETPOS,0,MAKELONG((short)(pColor->cColor.fR*255),0) );
            sprintf( cTemp,"%d",(int)(pColor->cColor.fR*255) );
            SetWindowText( hX,cTemp );
            SendMessage( hSpin2,UDM_SETPOS,0,MAKELONG((short)(pColor->cColor.fG*255),0) );
            sprintf( cTemp,"%d",(int)(pColor->cColor.fG*255) );
            SetWindowText( hY,cTemp );
            SendMessage( hSpin3,UDM_SETPOS,0,MAKELONG((short)(pColor->cColor.fB*255),0) );
            sprintf( cTemp,"%d",(int)(pColor->cColor.fB*255) );
            SetWindowText( hZ,cTemp );
            if( pColor->fTime == 0.0f || pColor->fTime == 1.0f ) EnableWindow( hSetTime,FALSE );
            break;
        case GRAPH_LIFE:
            SetWindowText( hWnd,"修改关键帧(&Life)" );
            SendMessage( hSpin1,UDM_SETRANGE,0,MAKELONG(200,0) );
            SendMessage( hSpin2,UDM_SETRANGE,0,MAKELONG(200,0) );
            EnableWindow( hZ,FALSE );
            EnableWindow( hSpin3,FALSE );
            pLife = (sKeyframe_Life*)modkf->pKFData;
            SendMessage( hSetTime,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)(100*pLife->fTime) );
            sprintf( cTemp,"时间: %.2f",pLife->fTime );
            SetWindowText( hTime,cTemp );
            SendMessage( hSpin1,UDM_SETPOS,0,MAKELONG((short)(pLife->fLifeMin*10),0) );
            sprintf( cTemp,"%.2f",pLife->fLifeMin );
            SetWindowText( hX,cTemp );
            SendMessage( hSpin2,UDM_SETPOS,0,MAKELONG((short)(pLife->fLifeMax*10),0) );
            sprintf( cTemp,"%.2f",pLife->fLifeMax );
            SetWindowText( hY,cTemp );
            if( pLife->fTime == 0.0f || pLife->fTime == 1.0f ) EnableWindow( hSetTime,FALSE );
            break;
        case GRAPH_VELOCITY:
            SetWindowText( hWnd,"修改关键帧(&Velocity)" );
            SendMessage( hSpin1,UDM_SETRANGE,0,MAKELONG(100,0) );
            SendMessage( hSpin2,UDM_SETRANGE,0,MAKELONG(100,0) );
            EnableWindow( hZ,FALSE );
            EnableWindow( hSpin3,FALSE );
            pVelocity = (sKeyframe_Velocity*)modkf->pKFData;
            SendMessage( hSetTime,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)(100*pVelocity->fTime) );
            sprintf( cTemp,"时间: %.2f",pVelocity->fTime );
            SetWindowText( hTime,cTemp );
            SendMessage( hSpin1,UDM_SETPOS,0,MAKELONG((short)(pVelocity->fVelocityMin*10),0) );
            sprintf( cTemp,"%.2f",pVelocity->fVelocityMin );
            SetWindowText( hX,cTemp );
            SendMessage( hSpin2,UDM_SETPOS,0,MAKELONG((short)(pVelocity->fVelocityMax*10),0) );
            sprintf( cTemp,"%.2f",pVelocity->fVelocityMax );
            SetWindowText( hY,cTemp );
            if( pVelocity->fTime == 0.0f || pVelocity->fTime == 1.0f ) EnableWindow( hSetTime,FALSE );
            break;
        case GRAPH_VELOCITYDIR:
            SetWindowText( hWnd,"修改关键帧(&Velocity Direction)" );
            SendMessage( hSpin1,UDM_SETRANGE,0,MAKELONG(100,-100) );
            SendMessage( hSpin2,UDM_SETRANGE,0,MAKELONG(100,-100) );
            SendMessage( hSpin3,UDM_SETRANGE,0,MAKELONG(100,-100) );
            pVelocityDir = (sKeyframe_VelocityDir*)modkf->pKFData;
            SendMessage( hSetTime,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)(100*pVelocityDir->fTime) );
            sprintf( cTemp,"时间: %.2f",pVelocityDir->fTime );
            SetWindowText( hTime,cTemp );
            SendMessage( hSpin1,UDM_SETPOS,0,MAKELONG((short)(pVelocityDir->vVelocityDir.x*10),0) );
            sprintf( cTemp,"%.2f",pVelocityDir->vVelocityDir.x );
            SetWindowText( hX,cTemp );
            SendMessage( hSpin2,UDM_SETPOS,0,MAKELONG((short)(pVelocityDir->vVelocityDir.y*10),0) );
            sprintf( cTemp,"%.2f",pVelocityDir->vVelocityDir.y );
            SetWindowText( hY,cTemp );
            SendMessage( hSpin3,UDM_SETPOS,0,MAKELONG((short)(pVelocityDir->vVelocityDir.z*10),0) );
            sprintf( cTemp,"%.2f",pVelocityDir->vVelocityDir.z );
            SetWindowText( hZ,cTemp );
            if( pVelocityDir->fTime == 0.0f || pVelocityDir->fTime == 1.0f ) EnableWindow( hSetTime,FALSE );
            break;
        case GRAPH_EMITRATE:
            SetWindowText( hWnd,"修改关键帧(&Emit Rate)" );
            SendMessage( hSpin1,UDM_SETRANGE,0,MAKELONG(100,0) );
            SendMessage( hSpin2,UDM_SETRANGE,0,MAKELONG(100,0) );
            EnableWindow( hZ,FALSE );
            EnableWindow( hSpin3,FALSE );
            pEmitRate = (sKeyframe_EmitRate*)modkf->pKFData;
            SendMessage( hSetTime,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)(100*pEmitRate->fTime) );
            sprintf( cTemp,"时间: %.2f",pEmitRate->fTime );
            SetWindowText( hTime,cTemp );
            SendMessage( hSpin1,UDM_SETPOS,0,MAKELONG((short)pEmitRate->fEmitRateMin,0) );
            sprintf( cTemp,"%d",(int)pEmitRate->fEmitRateMin );
            SetWindowText( hX,cTemp );
            SendMessage( hSpin2,UDM_SETPOS,0,MAKELONG((short)pEmitRate->fEmitRateMax,0) );
            sprintf( cTemp,"%d",(int)pEmitRate->fEmitRateMax );
            SetWindowText( hY,cTemp );
            if( pEmitRate->fTime == 0.0f || pEmitRate->fTime == 1.0f ) EnableWindow( hSetTime,FALSE );
            break;
        }
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_HSCROLL:
        // Handle the trackbar.
        if( lParam == (LPARAM)hSetTime )
        {
            n = SendMessage( hSetTime,TBM_GETPOS,0,0 );
            sprintf( cTemp,"时间: %.2f",0.01f*n );
            SetWindowText( hTime,cTemp );
        }

        // Handle the Up-down controls.
        if( LOWORD(wParam) != SB_ENDSCROLL )
        {
            switch( modkf->Type )
            {
            case GRAPH_SIZE:
            case GRAPH_ANGLE:
            case GRAPH_LIFE:
            case GRAPH_VELOCITY:
            case GRAPH_VELOCITYDIR:
                if( lParam == (LPARAM)hSpin1 )
                {
                    n = (short)SendMessage( hSpin1,UDM_GETPOS,0,0 );
                    sprintf( cTemp,"%.2f",(float)(n*0.1f) );
                    SetWindowText( hX,cTemp );
                }
                else if( lParam == (LPARAM)hSpin2 )
                {
                    n = (short)SendMessage( hSpin2,UDM_GETPOS,0,0 );
                    sprintf( cTemp,"%.2f",(float)(n*0.1f) );
                    SetWindowText( hY,cTemp );
                }
                else if( lParam == (LPARAM)hSpin3 )
                {
                    n = (short)SendMessage( hSpin3,UDM_GETPOS,0,0 );
                    sprintf( cTemp,"%.2f",(float)(n*0.1f) );
                    SetWindowText( hZ,cTemp );
                }
                break;
            }
        }
        break;
    case WM_COMMAND:
        if( wParam == IDOK )
        {
            GetWindowText( hTime,cTemp,40 );
            sscanf( cTemp,"时间: %f",&fTime );
            GetWindowText( hX,cTemp,40 );
            sscanf( cTemp,"%f",&fX );
            GetWindowText( hY,cTemp,40 );
            sscanf( cTemp,"%f",&fY );
            GetWindowText( hZ,cTemp,40 );
            sscanf( cTemp,"%f",&fZ );

            switch( modkf->Type )
            {
            case GRAPH_SIZE:
                for( n=0;n<pKF->m_Particle.GetKFSizeArray()->size();n++ )
                {
                    if( pSize == &((*pKF->m_Particle.GetKFSizeArray())[n]) )
                        continue;

                    if( fTime == ((*pKF->m_Particle.GetKFSizeArray())[n]).fTime )
                    {
                        MessageBox( NULL,"关键帧时间存在重叠！","Warning",MB_OK|MB_ICONWARNING );
                        return 0;
                    }
                }
                if( fX < 0.0f ) fX = 0.0f;
                if( fX > 10.0f ) fX = 10.0f;
                pSize->fTime = fTime;
                pSize->fSize = fX;
                break;
            case GRAPH_ANGLE:
                for( n=0;n<pKF->m_Particle.GetKFAngleArray()->size();n++ )
                {
                    if( pAngle == &((*pKF->m_Particle.GetKFAngleArray())[n]) )
                        continue;

                    if( fTime == ((*pKF->m_Particle.GetKFAngleArray())[n]).fTime )
                    {
                        MessageBox( NULL,"关键帧时间存在重叠！","Warning",MB_OK|MB_ICONWARNING );
                        return 0;
                    }
                }
                if( fX < -10.0f ) fX = -10.0f;
                if( fX > 10.0f ) fX = 10.0f;
                pAngle->fTime = fTime;
                pAngle->fAngle = fX;
                break;
            case GRAPH_ALPHA:
                for( n=0;n<pKF->m_Particle.GetKFAlphaArray()->size();n++ )
                {
                    if( pAlpha == &((*pKF->m_Particle.GetKFAlphaArray())[n]) )
                        continue;

                    if( fTime == ((*pKF->m_Particle.GetKFAlphaArray())[n]).fTime )
                    {
                        MessageBox( NULL,"关键帧时间存在重叠！","Warning",MB_OK|MB_ICONWARNING );
                        return 0;
                    }
                }
                fX /= 255.0f;
                if( fX < 0.0f ) fX = 0.0f;
                if( fX > 1.0f ) fX = 1.0f;
                pAlpha->fTime = fTime;
                pAlpha->fAlpha = fX;
                break;
            case GRAPH_COLOR:
                for( n=0;n<pKF->m_Particle.GetKFColorArray()->size();n++ )
                {
                    if( pColor == &((*pKF->m_Particle.GetKFColorArray())[n]) )
                        continue;

                    if( fTime == ((*pKF->m_Particle.GetKFColorArray())[n]).fTime )
                    {
                        MessageBox( NULL,"关键帧时间存在重叠！","Warning",MB_OK|MB_ICONWARNING );
                        return 0;
                    }
                }
                if( fX < 0.0f ) fX = 0.0f;
                if( fY < 0.0f ) fY = 0.0f;
                if( fZ < 0.0f ) fZ = 0.0f;
                if( fX > 255.0f ) fX = 255.0f;
                if( fY > 255.0f ) fY = 255.0f;
                if( fZ > 255.0f ) fZ = 255.0f;
                pColor->fTime = fTime;
                pColor->cColor.fR = fX / 255.0f;
                pColor->cColor.fG = fY / 255.0f;
                pColor->cColor.fB = fZ / 255.0f;
                break;
            case GRAPH_LIFE:
                for( n=0;n<pKF->m_Emitter.GetKFLifeArray()->size();n++ )
                {
                    if( pLife == &((*pKF->m_Emitter.GetKFLifeArray())[n]) )
                        continue;

                    if( fTime == ((*pKF->m_Emitter.GetKFLifeArray())[n]).fTime )
                    {
                        MessageBox( NULL,"关键帧时间存在重叠！","Warning",MB_OK|MB_ICONWARNING );
                        return 0;
                    }
                }
                if( fX < 0.0f ) fX = 0.0f;
                if( fY < 0.0f ) fY = 0.0f;
                if( fX > 20.0f ) fX = 20.0f;
                if( fY > 20.0f ) fY = 20.0f;
                pLife->fTime = fTime;
                pLife->fLifeMin = fX;
                pLife->fLifeMax = fY;
                break;
            case GRAPH_VELOCITY:
                for( n=0;n<pKF->m_Emitter.GetKFVelocityArray()->size();n++ )
                {
                    if( pVelocity == &((*pKF->m_Emitter.GetKFVelocityArray())[n]) )
                        continue;
                    
                    if( fTime == ((*pKF->m_Emitter.GetKFVelocityArray())[n]).fTime )
                    {
                        MessageBox( NULL,"关键帧时间存在重叠！","Warning",MB_OK|MB_ICONWARNING );
                        return 0;
                    }
                }
                if( fX < 0.0f ) fX = 0.0f;
                if( fY < 0.0f ) fY = 0.0f;
                if( fX > 10.0f ) fX = 10.0f;
                if( fY > 10.0f ) fY = 10.0f;
                pVelocity->fTime = fTime;
                pVelocity->fVelocityMin = fX;
                pVelocity->fVelocityMax = fY;
                break;
            case GRAPH_VELOCITYDIR:
                for( n=0;n<pKF->m_Emitter.GetKFVelocityDirArray()->size();n++ )
                {
                    if( pVelocityDir == &((*pKF->m_Emitter.GetKFVelocityDirArray())[n]) )
                        continue;

                    if( fTime == ((*pKF->m_Emitter.GetKFVelocityDirArray())[n]).fTime )
                    {
                        MessageBox( NULL,"关键帧时间存在重叠！","Warning",MB_OK|MB_ICONWARNING );
                        return 0;
                    }
                }
                if( fX < -10.0f ) fX = -10.0f;
                if( fY < -10.0f ) fY = -10.0f;
                if( fZ < -10.0f ) fZ = -10.0f;
                if( fX > 10.0f ) fX = 10.0f;
                if( fY > 10.0f ) fY = 10.0f;
                if( fZ > 10.0f ) fZ = 10.0f;
                pVelocityDir->fTime = fTime;
                pVelocityDir->vVelocityDir.x = fX;
                pVelocityDir->vVelocityDir.y = fY;
                pVelocityDir->vVelocityDir.z = fZ;
                break;
            case GRAPH_EMITRATE:
                for( n=0;n<pKF->m_Emitter.GetKFEmitRateArray()->size();n++ )
                {
                    if( pEmitRate == &((*pKF->m_Emitter.GetKFEmitRateArray())[n]) )
                        continue;

                    if( fTime == ((*pKF->m_Emitter.GetKFEmitRateArray())[n]).fTime )
                    {
                        MessageBox( NULL,"关键帧时间存在重叠！","Warning",MB_OK|MB_ICONWARNING );
                        return 0;
                    }
                }
                if( fX < 0.0f ) fX = 0.0f;
                if( fY < 0.0f ) fY = 0.0f;
                if( fX > 100.0f ) fX = 100.0f;
                if( fY > 100.0f ) fY = 100.0f;
                pEmitRate->fTime = fTime;
                pEmitRate->fEmitRateMin = fX;
                pEmitRate->fEmitRateMax = fY;
                break;
            }

            sprintf( cTemp,"%f",fTime );
            SendMessage( hKFList,LB_INSERTSTRING,modkf->nIndex,(LPARAM)cTemp );
            SendMessage( hKFList,LB_DELETESTRING,modkf->nIndex+1,(LPARAM)0 );
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}