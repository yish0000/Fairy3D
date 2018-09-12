//------------------------------------------------------------------------
// Name: etCallbackFunc.cpp
// Desc: This file is to implement all the CALLBACK functions.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#pragma warning(disable:4786)

#include "uiMacros.h"
#include "etCallbackFunc.h"

#include "etApplication.h"

#include "etCoreManager.h"
#include "etSceneManager.h"
#include "etSelectionBuffer.h"
#include "etEntityManager.h"

#include "etFloatRect.h"
#include "etStringUtil.h"


extern etApplication* g_pApp;

//------------------------------------------------------------------------
// CALLBACK function of the about dialog.
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
    case WM_CLOSE:
    case WM_LBUTTONDBLCLK:
        EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// CALLBACK Function to preview a texture.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_PreviewTex( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    HDRAWDIB hdd;
    HDC hdc;
    DIBSECTION dib;
    char* cImageFile;

    static HBITMAP hBmp = NULL;

    switch( uMsg )
    {
    case WM_PAINT:
        if( hBmp )
        {
            hdc = GetDC( hWnd );
            hdd = DrawDibOpen();
            GetObject( hBmp,sizeof(DIBSECTION),&dib );
            DrawDibDraw( hdd,hdc,0,0,195,195,&dib.dsBmih,dib.dsBm.bmBits,
                0,0,dib.dsBm.bmWidth,dib.dsBm.bmHeight,0 );
            DrawDibClose( hdd );
            ReleaseDC( hWnd,hdc );
        }
        break;
    case WM_INITDIALOG:
        cImageFile = (char*)lParam;
        SetWindowLong( hWnd,GWL_USERDATA,lParam );

        // Load the image.
        hBmp = (HBITMAP)LoadImage( g_pApp->GetInstance(),cImageFile,IMAGE_BITMAP,0,0,
            LR_CREATEDIBSECTION|LR_LOADFROMFILE );
        break;
    case WM_CLOSE:
        if( hBmp )
        {
            DeleteObject( hBmp );
            hBmp = NULL;
        }

        EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// Callback function for the dialog to set tile's texture.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle of the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_TileTex( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    int n;
    char cTemp[MAX_PATH];
    HDC hdc;
    HDRAWDIB hdd;
    DIBSECTION dib;
    float fScale;
    int nTexIndex;

    // Get the pointer to the UIManager.
    uiManager* pUIMgr = g_pApp->GetInterface()->GetUIManager();

    // Get the pointer to tile.
    etTerrain* pTerrain = etCoreManager::Instance().GetSceneManager()->GetTerrainLevel();
    etTile* pTile = pTerrain->GetSelectedTile();

    etInterface::sTileInfo* pInfo = (etInterface::sTileInfo*)GetWindowLong( hWnd,GWL_USERDATA );

    // Get the handle to controls.
    HWND hList = GetDlgItem( hWnd,IDC_TILE_TEXTURELIST );
    HWND hTexture = GetDlgItem( hWnd,IDC_TILE_TEXINDEX );
    HWND hScale = GetDlgItem( hWnd,IDC_TILE_TEXSCALE );

    static HBITMAP hBmp = NULL;

    switch( uMsg )
    {
    case WM_PAINT:
        if( hBmp )
        {
            hdc = GetDC( hWnd );
            hdd = DrawDibOpen();
            GetObject( hBmp,sizeof(DIBSECTION),&dib );
            DrawDibDraw( hdd,hdc,210,30,148,148,&dib.dsBmih,dib.dsBm.bmBits,
                         0,0,dib.dsBm.bmWidth,dib.dsBm.bmHeight,0 );
            DrawDibClose( hdd );
            ReleaseDC( hWnd,hdc );
        }
        break;
    case WM_INITDIALOG:
        pInfo = (etInterface::sTileInfo*)lParam;
        SetWindowLong( hWnd,GWL_USERDATA,lParam );
        SendDlgItemMessage( hWnd,IDC_TILE_SPIN1,UDM_SETRANGE,0,MAKELONG(100,0) );

        strcpy( cTemp,"空" );
        SendMessage( hList,LB_ADDSTRING,0,(LPARAM)cTemp );
        for( n=0;n<pUIMgr->GetItemCount(IDC_TP_TEXTURELIST);n++ )
        {
            pUIMgr->GetItemString( IDC_TP_TEXTURELIST,n,cTemp );
            SendMessage( hList,LB_ADDSTRING,0,(LPARAM)cTemp );
        }

        SendMessage( hList,LB_SETCURSEL,(WPARAM)(pInfo->nTexture+1),0 );
        if( pInfo->nTexture == -1 ) sprintf( cTemp,"纹理ID: 空" );
        else sprintf( cTemp,"纹理ID: %d",pInfo->nTexture );
        SetWindowText( hTexture,cTemp );
        sprintf( cTemp,"%.2f",pInfo->fScale );
        SetWindowText( hScale,cTemp );
        SendDlgItemMessage( hWnd,IDC_TILE_SPIN1,UDM_SETPOS,0,MAKELONG((short)pInfo->fScale,0) );

        if( pInfo->nTexture != -1 )
        {
            strcpy( cTemp,pTerrain->GetTexture(pInfo->nTexture)->GetName() );
            hBmp = (HBITMAP)LoadImage( g_pApp->GetInstance(),cTemp,IMAGE_BITMAP,0,0,
                                       LR_CREATEDIBSECTION|LR_LOADFROMFILE );
        }
        else
        {
            strcpy( cTemp,"Texture\\notexture.bmp" );
            hBmp = (HBITMAP)LoadImage( g_pApp->GetInstance(),cTemp,IMAGE_BITMAP,0,0,
                                       LR_CREATEDIBSECTION|LR_LOADFROMFILE );
        }
        break;
    case WM_CLOSE:
        if( hBmp ) DeleteObject( hBmp );
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        if( LOWORD(wParam) == IDC_TILE_TEXTURELIST )
        {
            n = SendMessage( hList,LB_GETCURSEL,0,0 );
            if( n == 0 ) strcpy( cTemp,"纹理ID: 空" );
            else sprintf( cTemp,"纹理ID: %d",n-1 );
            SetWindowText( hTexture,cTemp );

            // Clean the old image.
            if( hBmp ) DeleteObject( hBmp );
            if( n == 0 )
            {
                strcpy( cTemp,"Texture\\notexture.bmp" );
                hBmp = (HBITMAP)LoadImage( g_pApp->GetInstance(),cTemp,IMAGE_BITMAP,0,0,
                                           LR_LOADFROMFILE|LR_CREATEDIBSECTION );
            }
            else
            {
                strcpy( cTemp,pTerrain->GetTexture(n-1)->GetName() );
                hBmp = (HBITMAP)LoadImage( g_pApp->GetInstance(),cTemp,IMAGE_BITMAP,0,0,
                                           LR_LOADFROMFILE|LR_CREATEDIBSECTION );
            }

            SendMessage( hWnd,WM_PAINT,0,0 );
        }

        if( wParam == IDOK )
        {
            nTexIndex = SendMessage( hList,LB_GETCURSEL,0,0 );
            GetWindowText( hScale,cTemp,80 );
            sscanf( cTemp,"%f",&fScale );
            if( fScale <= 0.0f || fScale > 100.0f )
            {
                MessageBox( hWnd,"纹理Scale的范围必须在0-100之间！","Warning",MB_OK|MB_ICONWARNING );
                return 0;
            }

            pInfo->nTexture = nTexIndex-1;
            pInfo->fScale = fScale;

            if( hBmp ) DeleteObject( hBmp );
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL )
        {
            if( hBmp ) DeleteObject( hBmp );
            EndDialog( hWnd,0 );
        }
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// Callback function to put the specified texture to the whole terrain.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the window message.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_ApplyAll( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    float fScale;
    FlyTexture* pTexture;

    // Get the handle of the control.
    HWND hMain = GetDlgItem( hWnd,IDC_APPLYALL_MAIN );
    HWND hDetail1 = GetDlgItem( hWnd,IDC_APPLYALL_DETAIL1 );
    HWND hDetail2 = GetDlgItem( hWnd,IDC_APPLYALL_DETAIL2 );
    HWND hDetail3 = GetDlgItem( hWnd,IDC_APPLYALL_DETAIL3 );
    HWND hDetail4 = GetDlgItem( hWnd,IDC_APPLYALL_DETAIL4 );
    HWND hScale = GetDlgItem( hWnd,IDC_APPLYALL_SCALE );
    HWND hSpin1 = GetDlgItem( hWnd,IDC_APPLYALL_SPIN1 );

    // Get the pointer to the terrain.
    etTerrain* pTerrain = etCoreManager::Instance().GetSceneManager()->GetTerrainLevel();
    UINT nTextureID = (UINT)GetWindowLong( hWnd,GWL_USERDATA );

    switch( uMsg )
    {
    case WM_INITDIALOG:
        nTextureID = (UINT)lParam;
        SetWindowLong( hWnd,GWL_USERDATA,lParam );
        SendMessage( hMain,BM_SETCHECK,(WPARAM)BST_CHECKED,0 );
        SetWindowText( hScale,"1.000000" );
        SendMessage( hSpin1,UDM_SETRANGE,0,MAKELONG(100,0) );
        SendMessage( hSpin1,UDM_SETPOS,0,MAKELONG(1,0) );
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        if( wParam == IDOK )
        {
            char cTemp[40];
            GetWindowText( hScale,cTemp,40 );
            sscanf( cTemp,"%f",&fScale );
            pTexture = pTerrain->GetTexture( nTextureID );

            for( int i=0;i<pTerrain->GetNumTiles();i++ )
            {
                etTile* pTile = pTerrain->GetTerrainTile( i );

                if( SendMessage(hMain,BM_GETCHECK,0,0) == BST_CHECKED )
                {
                    pTile->SetMainTexture( pTexture );
                    pTile->SetMainScale( fScale );
                }
                else if( SendMessage(hDetail1,BM_GETCHECK,0,0) == BST_CHECKED )
                {
                    pTile->SetDetailTexture( 0,pTexture );
                    pTile->SetDetailScale( 0,fScale );
                }
                else if( SendMessage(hDetail2,BM_GETCHECK,0,0) == BST_CHECKED )
                {
                    pTile->SetDetailTexture( 1,pTexture );
                    pTile->SetDetailScale( 1,fScale );
                }
                else if( SendMessage(hDetail3,BM_GETCHECK,0,0) == BST_CHECKED )
                {
                    pTile->SetDetailTexture( 2,pTexture );
                    pTile->SetDetailScale( 2,fScale );
                }
                else if( SendMessage(hDetail4,BM_GETCHECK,0,0) == BST_CHECKED )
                {
                    pTile->SetDetailTexture( 3,pTexture );
                    pTile->SetDetailScale( 3,fScale );
                }
            }

            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// Set the fog mode for the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the window.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_SetFog( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    char cTemp[40];
    FOGMODE mode;
    float fNear,fFar;
    float fDensity;
    HDC hdc;
    HBRUSH hBrush;
    COLORREF rgb;

    // Get the handle to the controls.
    HWND hMode = GetDlgItem( hWnd,IDC_FOG_MODE );
    HWND hSel = GetDlgItem( hWnd,IDC_FOG_SELCOLOR );
    HWND hNear = GetDlgItem( hWnd,IDC_FOG_NEAR );
    HWND hFar = GetDlgItem( hWnd,IDC_FOG_FAR );
    HWND hDensity = GetDlgItem( hWnd,IDC_FOG_DENSITY );

    static FLYCOLOR s_cFogColor;

    // Get the sceneManager of the editor.
    etCoreManager& core = etCoreManager::Instance();
    etInterface* pUI = g_pApp->GetInterface();

    switch( uMsg )
    {
    case WM_PAINT:
        hdc = GetDC( hWnd );
        rgb = RGB( s_cFogColor.fR*255,s_cFogColor.fG*255,s_cFogColor.fB*255 );
        hBrush = CreateSolidBrush( rgb );
        SelectObject( hdc,hBrush );
        Rectangle( hdc,57,60,169,76 );
        DeleteObject( hBrush );
        ReleaseDC( hWnd,hdc );
        break;
    case WM_INITDIALOG:
        SendMessage( hMode,CB_ADDSTRING,0,(LPARAM)("FOG_NONE") );
        SendMessage( hMode,CB_ADDSTRING,0,(LPARAM)("FOG_EXP") );
        SendMessage( hMode,CB_ADDSTRING,0,(LPARAM)("FOG_EXP2") );
        SendMessage( hMode,CB_ADDSTRING,0,(LPARAM)("FOG_LINEAR") );
        core.GetFog( &mode,&s_cFogColor,&fDensity,&fNear,&fFar );
        SendDlgItemMessage( hWnd,IDC_FOG_SPIN1,UDM_SETRANGE,0,MAKELONG(1,10000) );
        SendDlgItemMessage( hWnd,IDC_FOG_SPIN2,UDM_SETRANGE,0,MAKELONG(1,10000) );
        SendDlgItemMessage( hWnd,IDC_FOG_SPIN3,UDM_SETRANGE,0,MAKELONG(1,100) );
        SendMessage( hMode,CB_SETCURSEL,(WPARAM)mode,0 );
        sprintf( cTemp,"%f",fNear );
        SetWindowText( hNear,cTemp );
        SendDlgItemMessage( hWnd,IDC_FOG_SPIN1,UDM_SETPOS,0,MAKELONG((short)fNear,0) );
        sprintf( cTemp,"%f",fFar );
        SetWindowText( hFar,cTemp );
        SendDlgItemMessage( hWnd,IDC_FOG_SPIN2,UDM_SETPOS,0,MAKELONG((short)fFar,0) );
        sprintf( cTemp,"%f",fDensity );
        SetWindowText( hDensity,cTemp );
        SendDlgItemMessage( hWnd,IDC_FOG_SPIN3,UDM_SETPOS,0,MAKELONG((short)fDensity,0) );
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        if( wParam == IDC_FOG_SELCOLOR )
        {
            rgb = RGB( s_cFogColor.fR*255,s_cFogColor.fG*255,s_cFogColor.fB*255 );
            if( pUI->GetToolDialog()->DlgChooseColor(hWnd,&rgb) )
            {
                s_cFogColor.fR = (float)GetRValue( rgb ) / 255;
                s_cFogColor.fG = (float)GetGValue( rgb ) / 255;
                s_cFogColor.fB = (float)GetBValue( rgb ) / 255;
            }
            SendMessage( hWnd,WM_PAINT,0,0 );
        }

        if( wParam == IDOK )
        {
            int n = SendMessage( hMode,CB_GETCURSEL,0,0 );
            GetWindowText( hNear,cTemp,40 );
            sscanf( cTemp,"%f",&fNear );
            GetWindowText( hFar,cTemp,40 );
            sscanf( cTemp,"%f",&fFar );
            GetWindowText( hDensity,cTemp,40 );
            sscanf( cTemp,"%f",&fDensity );
            core.SetFog( (FOGMODE)n,s_cFogColor,fDensity,fNear,fFar );
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// Callback function to set the lighting mode.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the window.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_LightMode( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    HDC hdc;
    HBRUSH hBrush;
    COLORREF rgb;
    char cTemp[40];
    etInterface::sVector vDirection;

    // Get the handle of the control.
    HWND hAmbient = GetDlgItem( hWnd,IDC_LGT_AMBIENT );
    HWND hDiffuse = GetDlgItem( hWnd,IDC_LGT_DIFFUSE );
    HWND hDirection = GetDlgItem( hWnd,IDC_LGT_DIRECTION );
    HWND hEnable = GetDlgItem( hWnd,IDC_LGT_ENABLELIGHT );

    static FLYCOLOR s_cDiffuse;
    static FLYCOLOR s_cAmbient;

    // Get the sceneManager.
    etSceneManager* pSceneMgr = etCoreManager::Instance().GetSceneManager();

    // Get the user interface.
    etInterface* pUI = g_pApp->GetInterface();

    switch( uMsg )
    {
    case WM_PAINT:
        hdc = GetDC( hWnd );

        rgb = RGB( s_cDiffuse.fR*255,s_cDiffuse.fG*255,s_cDiffuse.fB*255 );
        hBrush = CreateSolidBrush( rgb );
        SelectObject( hdc,hBrush );
        Rectangle( hdc,81,63,193,82 );
        DeleteObject( hBrush );

        rgb = RGB( s_cAmbient.fR*255,s_cAmbient.fG*255,s_cAmbient.fB*255 );
        hBrush = CreateSolidBrush( rgb );
        SelectObject( hdc,hBrush );
        Rectangle( hdc,81,92,193,111 );
        DeleteObject( hBrush );

        ReleaseDC( hWnd,hdc );
        break;
    case WM_INITDIALOG:
        if( pSceneMgr->GetLightingMode() )
            SendMessage( hEnable,BM_SETCHECK,(WPARAM)BST_CHECKED,0 );
        else
            SendMessage( hEnable,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0 );

        memcpy( &vDirection,&pSceneMgr->GetLightDirection(),12 );
        sprintf( cTemp,"(%f,%f,%f)",vDirection.x,vDirection.y,vDirection.z );
        SetWindowText( hDirection,cTemp );

        s_cAmbient = pSceneMgr->GetLightAmbient();
        s_cDiffuse = pSceneMgr->GetLightDiffuse();
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        switch( wParam )
        {
        case IDC_LGT_DIRECTION:
            memcpy( &vDirection,&pSceneMgr->GetLightDirection(),12 );
            if( pUI->ShowVectorSetting(&vDirection) )
            {
                sprintf( cTemp,"(%f,%f,%f)",vDirection.x,vDirection.y,vDirection.z );
                SetWindowText( hDirection,cTemp );
            }
            break;
        case IDC_LGT_DIFFUSE:
            rgb = RGB( s_cDiffuse.fR*255,s_cDiffuse.fG*255,s_cDiffuse.fB*255 );
            if( pUI->GetToolDialog()->DlgChooseColor(hWnd,&rgb) )
            {
                s_cDiffuse.fR = (float)GetRValue( rgb ) / 255;
                s_cDiffuse.fG = (float)GetGValue( rgb ) / 255;
                s_cDiffuse.fB = (float)GetBValue( rgb ) / 255;
            }
            SendMessage( hWnd,WM_PAINT,0,0 );
            break;
        case IDC_LGT_AMBIENT:
            rgb = RGB( s_cAmbient.fR*255,s_cAmbient.fG*255,s_cAmbient.fB*255 );
            if( pUI->GetToolDialog()->DlgChooseColor(hWnd,&rgb) )
            {
                s_cAmbient.fR = (float)GetRValue( rgb ) / 255;
                s_cAmbient.fG = (float)GetGValue( rgb ) / 255;
                s_cAmbient.fB = (float)GetBValue( rgb ) / 255;
            }
            SendMessage( hWnd,WM_PAINT,0,0 );
            break;
        }

        if( wParam == IDOK )
        {
            GetWindowText( hDirection,cTemp,40 );
            sscanf( cTemp,"(%f,%f,%f)",&vDirection.x,&vDirection.y,&vDirection.z );
            vDirection.w = 1.0f;
            pSceneMgr->SetLightDirection( *((FlyVector*)&vDirection) );
            pSceneMgr->SetLightAmbient( s_cAmbient );
            pSceneMgr->SetLightDiffuse( s_cDiffuse );

            if( SendMessage(hEnable,BM_GETCHECK,0,0) == BST_CHECKED )
                pSceneMgr->SetLightingMode( true );
            else
                pSceneMgr->SetLightingMode( false );
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// Callback function to config the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the window.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_Config( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    int n;
    char cTemp[40];
    float fClipNear,fClipFar;
    FLYCOLOR cBackColor;
    float fMaxHeight;
    FLYCOLOR cWireColor;
    float fCamMoveSpeed;
    float fCamRotateSpeed;
    float fETMoveSpeed;
    float fETRotateSpeed;
    float fETScaleSpeed;

    // Get the handle to the controls.
    HWND hClipNear = GetDlgItem( hWnd,IDC_CFG_CLIPNEAR );
    HWND hClipFar = GetDlgItem( hWnd,IDC_CFG_CLIPFAR );
    HWND hBackColor = GetDlgItem( hWnd,IDC_CFG_BACKCOLOR );
    HWND hMaxHeight = GetDlgItem( hWnd,IDC_CFG_MAXHEIGHT );
    HWND hWireColor = GetDlgItem( hWnd,IDC_CFG_WIRECOLOR );
    HWND hCamMoveSpeed = GetDlgItem( hWnd,IDC_CFG_CAMMOVESPEED );
    HWND hCamRotateSpeed = GetDlgItem( hWnd,IDC_CFG_CAMROTATESPEED );
    HWND hETMoveSpeed = GetDlgItem( hWnd,IDC_CFG_ETMOVESPEED );
    HWND hETRotateSpeed = GetDlgItem( hWnd,IDC_CFG_ETROTATESPEED );
    HWND hETScaleSpeed = GetDlgItem( hWnd,IDC_CFG_ETSCALESPEED );
    HWND hSkyList = GetDlgItem( hWnd,IDC_CFG_SKYBOXLIST );

    etConfig* pConfig = etCoreManager::Instance().GetConfigData();

    // Get the User interface.
    etInterface* pUI = g_pApp->GetInterface();

    switch( uMsg )
    {
    case WM_INITDIALOG:
        fClipNear = pConfig->GetNearDistance();
        SendDlgItemMessage( hWnd,IDC_CFG_SPIN4,UDM_SETRANGE,0,MAKELONG(30000,0) );
        SendDlgItemMessage( hWnd,IDC_CFG_SPIN4,UDM_SETPOS,0,MAKELONG((short)fClipNear,0) );
        sprintf( cTemp,"%f",fClipNear );
        SetWindowText( hClipNear,cTemp );
        fClipFar = pConfig->GetFarDistance();
        SendDlgItemMessage( hWnd,IDC_CFG_SPIN5,UDM_SETRANGE,0,MAKELONG(30000,0) );
        SendDlgItemMessage( hWnd,IDC_CFG_SPIN5,UDM_SETPOS,0,MAKELONG((short)fClipFar,0) );
        sprintf( cTemp,"%f",fClipFar );
        SetWindowText( hClipFar,cTemp );
        cBackColor = pConfig->GetBackColor();
        sprintf( cTemp,"(%d,%d,%d)",(UCHAR)(255*cBackColor.fR),(UCHAR)(255*cBackColor.fG),
            (UCHAR)(255*cBackColor.fB) );
        SetWindowText( hBackColor,cTemp );
        fMaxHeight = pConfig->GetMaxHeight();
        SendDlgItemMessage( hWnd,IDC_CFG_SPIN1,UDM_SETRANGE,0,MAKELONG(1024,64) );
        SendDlgItemMessage( hWnd,IDC_CFG_SPIN1,UDM_SETPOS,0,MAKELONG((short)fMaxHeight,0) );
        sprintf( cTemp,"%f",fMaxHeight );
        SetWindowText( hMaxHeight,cTemp );
        cWireColor = pConfig->GetWireColor();
        sprintf( cTemp,"(%d,%d,%d)",(UCHAR)(255*cWireColor.fR),(UCHAR)(255*cWireColor.fG),
            (UCHAR)(255*cWireColor.fB) );
        SetWindowText( hWireColor,cTemp );
        fCamMoveSpeed = pConfig->GetCamMoveSpeed();
        SendDlgItemMessage( hWnd,IDC_CFG_SPIN2,UDM_SETRANGE,0,MAKELONG(100,0) );
        SendDlgItemMessage( hWnd,IDC_CFG_SPIN2,UDM_SETPOS,0,MAKELONG((short)fCamMoveSpeed,0) );
        sprintf( cTemp,"%f",fCamMoveSpeed );
        SetWindowText( hCamMoveSpeed,cTemp );
        fCamRotateSpeed = pConfig->GetCamRotateSpeed();
        SendDlgItemMessage( hWnd,IDC_CFG_SPIN3,UDM_SETRANGE,0,MAKELONG(100,0) );
        SendDlgItemMessage( hWnd,IDC_CFG_SPIN3,UDM_SETPOS,0,MAKELONG((short)fCamRotateSpeed,0) );
        sprintf( cTemp,"%f",fCamRotateSpeed );
        SetWindowText( hCamRotateSpeed,cTemp );
        fETMoveSpeed = pConfig->GetETMoveSpeed();
        SendDlgItemMessage( hWnd,IDC_CFG_SPIN6,UDM_SETRANGE,0,MAKELONG(100,0) );
        SendDlgItemMessage( hWnd,IDC_CFG_SPIN6,UDM_SETPOS,0,MAKELONG((short)fETMoveSpeed,0) );
        sprintf( cTemp,"%f",fETMoveSpeed );
        SetWindowText( hETMoveSpeed,cTemp );
        fETRotateSpeed = pConfig->GetETRotateSpeed();
        SendDlgItemMessage( hWnd,IDC_CFG_SPIN7,UDM_SETRANGE,0,MAKELONG(100,0) );
        SendDlgItemMessage( hWnd,IDC_CFG_SPIN7,UDM_SETPOS,0,MAKELONG((short)fETRotateSpeed,0) );
        sprintf( cTemp,"%f",fETRotateSpeed );
        SetWindowText( hETRotateSpeed,cTemp );
        fETScaleSpeed = pConfig->GetETScaleSpeed();
        SendDlgItemMessage( hWnd,IDC_CFG_SPIN8,UDM_SETRANGE,0,MAKELONG(100,0) );
        SendDlgItemMessage( hWnd,IDC_CFG_SPIN8,UDM_SETPOS,0,MAKELONG((short)fETScaleSpeed,0) );
        sprintf( cTemp,"%f",fETScaleSpeed );
        SetWindowText( hETScaleSpeed,cTemp );

        for( n=0;n<pConfig->GetNumSkybox();n++ )
            SendMessage( hSkyList,LB_ADDSTRING,0,(LPARAM)pConfig->GetSkyboxName(n) );
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        switch( LOWORD(wParam) )
        {
        case IDC_CFG_BACKCOLOR:
            {
                COLORREF dwColor;
                UCHAR ucR,ucG,ucB;

                GetWindowText( hBackColor,cTemp,40 );
                sscanf( cTemp,"(%d,%d,%d)",&ucR,&ucG,&ucB );
                dwColor = RGB( ucR,ucG,ucB );
                if( pUI->GetToolDialog()->DlgChooseColor(hWnd,&dwColor) )
                {
                    ucR = GetRValue( dwColor );
                    ucG = GetGValue( dwColor );
                    ucB = GetBValue( dwColor );
                    sprintf( cTemp,"(%d,%d,%d)",ucR,ucG,ucB );
                    SetWindowText( hBackColor,cTemp );
                }
            }
            break;
        case IDC_CFG_WIRECOLOR:
            {
                COLORREF dwColor;
                UCHAR ucR,ucG,ucB;

                GetWindowText( hWireColor,cTemp,40 );
                sscanf( cTemp,"(%d,%d,%d)",&ucR,&ucG,&ucB );
                dwColor = RGB( ucR,ucG,ucB );
                if( pUI->GetToolDialog()->DlgChooseColor(hWnd,&dwColor) )
                {
                    ucR = GetRValue( dwColor );
                    ucG = GetGValue( dwColor );
                    ucB = GetBValue( dwColor );
                    sprintf( cTemp,"(%d,%d,%d)",ucR,ucG,ucB );
                    SetWindowText( hWireColor,cTemp );
                }
            }
            break;
        case IDC_CFG_ADDSKY:
            {
                int nRes;
                cbSkyboxNode node;
                nRes = DialogBoxParam( g_pApp->GetInstance(),(LPCTSTR)IDD_DLG_EDITSKYBOX,
                    hWnd,Callback_EditSky,(LPARAM)&node );
                if( nRes == 1 )
                {
                    pConfig->AddNewSkybox( node.sName.c_str(),node.sFrontFace.c_str(),
                        node.sBackFace.c_str(),node.sLeftFace.c_str(),
                        node.sRightFace.c_str(),node.sTopFace.c_str(),
                        node.sBottomFace.c_str() );

                    SendMessage( hSkyList,LB_ADDSTRING,0,(LPARAM)node.sName.c_str() );
                }
            }
            break;
        case IDC_CFG_DELETESKY:
            n = SendMessage( hSkyList,LB_GETCURSEL,0,0 );
            if( n == LB_ERR ) return 0;

            if( MessageBox(hWnd,"确定要删除么？","提示",MB_YESNO|MB_ICONQUESTION)
                == IDYES )
            {
                pConfig->RemoveSkybox( n );
                SendMessage( hSkyList,LB_DELETESTRING,n,0 );
            }
            break;
        case IDC_CFG_SKYBOXLIST:
            n = SendMessage( hSkyList,LB_GETCURSEL,0,0 );
            if( n == LB_ERR ) return 0;

            if( HIWORD(wParam) == LBN_DBLCLK )
            {
                int nRes;
                cbSkyboxNode node;

                node.sName = pConfig->GetSkyboxName(n);
                node.sFrontFace = pConfig->GetSkyboxFront(n);
                node.sBackFace = pConfig->GetSkyboxBack(n);
                node.sLeftFace = pConfig->GetSkyboxLeft(n);
                node.sRightFace = pConfig->GetSkyboxRight(n);
                node.sTopFace = pConfig->GetSkyboxTop(n);
                node.sBottomFace = pConfig->GetSkyboxBottom(n);
                nRes = DialogBoxParam( g_pApp->GetInstance(),(LPCTSTR)IDD_DLG_EDITSKYBOX,
                    hWnd,Callback_EditSky,(LPARAM)&node );
                if( nRes == 1 )
                {
                    pConfig->SetSkyboxName( n,node.sName.c_str() );
                    pConfig->SetSkyboxFront( n,node.sFrontFace.c_str() );
                    pConfig->SetSkyboxBack( n,node.sBackFace.c_str() );
                    pConfig->SetSkyboxLeft( n,node.sLeftFace.c_str() );
                    pConfig->SetSkyboxRight( n,node.sRightFace.c_str() );
                    pConfig->SetSkyboxTop( n,node.sTopFace.c_str() );
                    pConfig->SetSkyboxBottom( n,node.sBottomFace.c_str() );

                    SendMessage( hSkyList,LB_DELETESTRING,n,0 );
                    SendMessage( hSkyList,LB_INSERTSTRING,n,(LPARAM)node.sName.c_str() );
                }
            }
            break;
        }

        if( wParam == IDOK )
        {
            UCHAR ucR,ucG,ucB;
            GetWindowText( hClipNear,cTemp,40 );
            sscanf( cTemp,"%f",&fClipNear );
            pConfig->SetNearDistance( fClipNear );
            GetWindowText( hClipFar,cTemp,40 );
            sscanf( cTemp,"%f",&fClipFar );
            pConfig->SetFarDistance( fClipFar );
            GetWindowText( hBackColor,cTemp,40 );
            sscanf( cTemp,"(%d,%d,%d)",&ucR,&ucG,&ucB );
            cBackColor.fR = (float)ucR / 255.0f;
            cBackColor.fG = (float)ucG / 255.0f;
            cBackColor.fB = (float)ucB / 255.0f;
            cBackColor.fA = 1.0f;
            pConfig->SetBackColor( cBackColor );
            GetWindowText( hMaxHeight,cTemp,40 );
            sscanf( cTemp,"%f",&fMaxHeight );
            pConfig->SetMaxHeight( fMaxHeight );
            GetWindowText( hWireColor,cTemp,40 );
            sscanf( cTemp,"(%d,%d,%d)",&ucR,&ucG,&ucB );
            cWireColor.fR = (float)ucR / 255.0f;
            cWireColor.fG = (float)ucG / 255.0f;
            cWireColor.fB = (float)ucB / 255.0f;
            cWireColor.fA = 1.0f;
            pConfig->SetWireColor( cWireColor );
            GetWindowText( hCamMoveSpeed,cTemp,40 );
            sscanf( cTemp,"%f",&fCamMoveSpeed );
            pConfig->SetCamMoveSpeed( fCamMoveSpeed );
            GetWindowText( hCamRotateSpeed,cTemp,40 );
            sscanf( cTemp,"%f",&fCamRotateSpeed );
            pConfig->SetCamRotateSpeed( fCamRotateSpeed );
            GetWindowText( hETMoveSpeed,cTemp,40 );
            sscanf( cTemp,"%f",&fETMoveSpeed );
            pConfig->SetETMoveSpeed( fETMoveSpeed );
            GetWindowText( hETRotateSpeed,cTemp,40 );
            sscanf( cTemp,"%f",&fETRotateSpeed );
            pConfig->SetETRotateSpeed( fETRotateSpeed );
            GetWindowText( hETScaleSpeed,cTemp,40 );
            sscanf( cTemp,"%f",&fETScaleSpeed );
            pConfig->SetETScaleSpeed( fETScaleSpeed );

            etCoreManager& core = etCoreManager::Instance();
            core.SetViewPlanes( fClipNear,fClipFar );
            core.SetBackColor( cBackColor );
            core.GetSceneManager()->SetWireColor( cWireColor );
            pConfig->SaveConfig( "editor_config.xml" );
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// Callback function to edit the skybox.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the window.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_EditSky( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    char cTemp[MAX_PATH];
    cbSkyboxNode* pSkyNode;

    // Get the handle to the controls.
    HWND hName = GetDlgItem( hWnd,IDC_EDITSKY_NAME );
    HWND hFront = GetDlgItem( hWnd,IDC_EDITSKY_FRONT );
    HWND hBack = GetDlgItem( hWnd,IDC_EDITSKY_BACK );
    HWND hLeft = GetDlgItem( hWnd,IDC_EDITSKY_LEFT );
    HWND hRight = GetDlgItem( hWnd,IDC_EDITSKY_RIGHT );
    HWND hTop = GetDlgItem( hWnd,IDC_EDITSKY_TOP );
    HWND hBottom = GetDlgItem( hWnd,IDC_EDITSKY_BOTTOM );

    pSkyNode = (cbSkyboxNode*)GetWindowLong( hWnd,GWL_USERDATA );

    // Get the config data.
    etConfig* pConfig = etCoreManager::Instance().GetConfigData();

    switch( uMsg )
    {
    case WM_INITDIALOG:
        pSkyNode = (cbSkyboxNode*)lParam;
        SetWindowLong( hWnd,GWL_USERDATA,lParam );

        SetWindowText( hName,pSkyNode->sName.c_str() );
        SetWindowText( hFront,pSkyNode->sFrontFace.c_str() );
        SetWindowText( hBack,pSkyNode->sBackFace.c_str() );
        SetWindowText( hLeft,pSkyNode->sLeftFace.c_str() );
        SetWindowText( hRight,pSkyNode->sRightFace.c_str() );
        SetWindowText( hTop,pSkyNode->sTopFace.c_str() );
        SetWindowText( hBottom,pSkyNode->sBottomFace.c_str() );
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        if( wParam == IDOK )
        {
            GetWindowText( hName,cTemp,MAX_PATH );
            if( pConfig->IsSkyboxExisted(cTemp) )
            {
                MessageBox( hWnd,"天空盒已经存在！","错误",MB_OK|MB_ICONWARNING );
                return 0;
            }

            pSkyNode->sName = cTemp;
            GetWindowText( hFront,cTemp,MAX_PATH );
            pSkyNode->sFrontFace = cTemp;
            GetWindowText( hBack,cTemp,MAX_PATH );
            pSkyNode->sBackFace = cTemp;
            GetWindowText( hLeft,cTemp,MAX_PATH );
            pSkyNode->sLeftFace = cTemp;
            GetWindowText( hRight,cTemp,MAX_PATH );
            pSkyNode->sRightFace = cTemp;
            GetWindowText( hTop,cTemp,MAX_PATH );
            pSkyNode->sTopFace = cTemp;
            GetWindowText( hBottom,cTemp,MAX_PATH );
            pSkyNode->sBottomFace = cTemp;
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// Callback function to set the skybox.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the window.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_Skybox( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    int n;
    HDC hdc;
    HDRAWDIB hdd;
    DIBSECTION dib;

    // Get the handle to the controls.
    HWND hList = GetDlgItem( hWnd,IDC_SKYBOX_SKYLIST );
    HWND hEnable = GetDlgItem( hWnd,IDC_SKYBOX_ENABLESKY );

    static HBITMAP hBmp[6];

    // Get the config data.
    etConfig* pConfig = etCoreManager::Instance().GetConfigData();

    switch( uMsg )
    {
    case WM_PAINT:
        hdc = GetDC( hWnd );
        hdd = DrawDibOpen();

        if( hBmp[0] )
        {
            GetObject( hBmp[0],sizeof(DIBSECTION),&dib );
            DrawDibDraw( hdd,hdc,20,60,120,120,&dib.dsBmih,dib.dsBm.bmBits,
                0,0,dib.dsBm.bmWidth,dib.dsBm.bmHeight,0 );
        }

        if( hBmp[1] )
        {
            GetObject( hBmp[1],sizeof(DIBSECTION),&dib );
            DrawDibDraw( hdd,hdc,141,60,120,120,&dib.dsBmih,dib.dsBm.bmBits,
                0,0,dib.dsBm.bmWidth,dib.dsBm.bmHeight,0 );
        }

        if( hBmp[2] )
        {
            GetObject( hBmp[2],sizeof(DIBSECTION),&dib );
            DrawDibDraw( hdd,hdc,262,60,120,120,&dib.dsBmih,dib.dsBm.bmBits,
                0,0,dib.dsBm.bmWidth,dib.dsBm.bmHeight,0 );
        }

        if( hBmp[3] )
        {
            GetObject( hBmp[3],sizeof(DIBSECTION),&dib );
            DrawDibDraw( hdd,hdc,20,181,120,120,&dib.dsBmih,dib.dsBm.bmBits,
                0,0,dib.dsBm.bmWidth,dib.dsBm.bmHeight,0 );
        }

        if( hBmp[4] )
        {
            GetObject( hBmp[4],sizeof(DIBSECTION),&dib );
            DrawDibDraw( hdd,hdc,141,181,120,120,&dib.dsBmih,dib.dsBm.bmBits,
                0,0,dib.dsBm.bmWidth,dib.dsBm.bmHeight,0 );
        }

        if( hBmp[5] )
        {
            GetObject( hBmp[5],sizeof(DIBSECTION),&dib );
            DrawDibDraw( hdd,hdc,262,181,120,120,&dib.dsBmih,dib.dsBm.bmBits,
                0,0,dib.dsBm.bmWidth,dib.dsBm.bmHeight,0 );
        }

        DrawDibClose( hdd );
        ReleaseDC( hWnd,hdc );
        break;
    case WM_INITDIALOG:
        for( n=0;n<pConfig->GetNumSkybox();n++ )
            SendMessage( hList,CB_ADDSTRING,0,(LPARAM)pConfig->GetSkyboxName(n) );

        if( etCoreManager::Instance().IsShowSkybox() )
            SendMessage( hEnable,BM_SETCHECK,(WPARAM)BST_CHECKED,0 );
        else
            SendMessage( hEnable,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0 );

        SendMessage( hList,CB_SETCURSEL,0,0 );
        hBmp[0] = (HBITMAP)LoadImage( g_pApp->GetInstance(),pConfig->GetSkyboxFront(0),
            IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
        hBmp[1] = (HBITMAP)LoadImage( g_pApp->GetInstance(),pConfig->GetSkyboxBack(0),
            IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
        hBmp[2] = (HBITMAP)LoadImage( g_pApp->GetInstance(),pConfig->GetSkyboxLeft(0),
            IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
        hBmp[3] = (HBITMAP)LoadImage( g_pApp->GetInstance(),pConfig->GetSkyboxRight(0),
            IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
        hBmp[4] = (HBITMAP)LoadImage( g_pApp->GetInstance(),pConfig->GetSkyboxTop(0),
            IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
        hBmp[5] = (HBITMAP)LoadImage( g_pApp->GetInstance(),pConfig->GetSkyboxBottom(0),
            IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
        break;
    case WM_CLOSE:
        for( n=0;n<6;n++ )
        {
            DeleteObject( hBmp[n] );
            hBmp[n] = NULL;
        }
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        if( HIWORD(wParam) == CBN_SELCHANGE )
        {
            for( n=0;n<6;n++ )
            {
                DeleteObject( hBmp[n] );
                hBmp[n] = NULL;
            }

            n = SendMessage( hList,CB_GETCURSEL,0,0 );
            hBmp[0] = (HBITMAP)LoadImage( g_pApp->GetInstance(),pConfig->GetSkyboxFront(n),
                IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
            hBmp[1] = (HBITMAP)LoadImage( g_pApp->GetInstance(),pConfig->GetSkyboxBack(n),
                IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
            hBmp[2] = (HBITMAP)LoadImage( g_pApp->GetInstance(),pConfig->GetSkyboxLeft(n),
                IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
            hBmp[3] = (HBITMAP)LoadImage( g_pApp->GetInstance(),pConfig->GetSkyboxRight(n),
                IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
            hBmp[4] = (HBITMAP)LoadImage( g_pApp->GetInstance(),pConfig->GetSkyboxTop(n),
                IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
            hBmp[5] = (HBITMAP)LoadImage( g_pApp->GetInstance(),pConfig->GetSkyboxBottom(n),
                IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );

            SendMessage( hWnd,WM_PAINT,0,0 );
        }

        if( wParam == IDOK )
        {
            etCoreManager& core = etCoreManager::Instance();

            n = SendMessage( hList,CB_GETCURSEL,0,0 );
            if( SendMessage(hEnable,BM_GETCHECK,0,0) == BST_CHECKED )
                core.SetSkyboxByIndex( true,n );
            else
                core.SetSkyboxByIndex( false,n );
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// Callback function for the dialog to set the entity name.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_SetName( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    char cTemp[40];
    HWND hName = GetDlgItem( hWnd,IDC_ETNAME_NAME );

    // Get the pointer to the selection buffer.
    etSelectionBuffer* pSelBuf = etCoreManager::Instance().GetSelectionBuffer();

    // Get the pointer to the entityManager.
    etEntityManager* pEntityMgr = etCoreManager::Instance().GetEntityManager();

    switch( uMsg )
    {
    case WM_INITDIALOG:
        strcpy( cTemp,pSelBuf->GetSelectedEntity(0)->GetName() );
        SetWindowText( hName,cTemp );
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        if( wParam == IDOK )
        {
            GetWindowText( hName,cTemp,40 );
            if( strcmp(cTemp,"") == 0 )
            {
                MessageBox( hWnd,"请给实体指定一个名字！","警告",MB_OK|MB_ICONWARNING );
                return 0;
            }

            // Get the current selected entity.
            etEntity* pEntity = pSelBuf->GetSelectedEntity( 0 );

            // Check whether the name is repeated ?
            for( UINT i=0;i<pEntityMgr->GetNumEntities();i++ )
            {
                if( pEntityMgr->GetEntityByIndex(i) != pEntity &&
                    strcmp(pEntityMgr->GetEntityByIndex(i)->GetName(),cTemp) == 0 )
                {
                    MessageBox( hWnd,"指定名字重复！","警告",MB_OK|MB_ICONWARNING );
                    return 0;
                }
            }

            pEntity->SetName( cTemp );
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// Callback function of the dialog to set the vector.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the window.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_SetVector( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    char cTemp[40];

    // Get the handle of the controls.
    HWND hX = GetDlgItem( hWnd,IDC_SETVEC_X );
    HWND hY = GetDlgItem( hWnd,IDC_SETVEC_Y );
    HWND hZ = GetDlgItem( hWnd,IDC_SETVEC_Z );

    cbVector* pVector = (cbVector*)GetWindowLong( hWnd,GWL_USERDATA );

    switch( uMsg )
    {
    case WM_INITDIALOG:
        pVector = (cbVector*)lParam;
        SetWindowLong( hWnd,GWL_USERDATA,lParam );

        SendDlgItemMessage( hWnd,IDC_SETVEC_SPINX,UDM_SETRANGE,0,MAKELONG(10000,-10000) );
        SendDlgItemMessage( hWnd,IDC_SETVEC_SPINX,UDM_SETPOS,0,MAKELONG((short)pVector->x,0) );
        SendDlgItemMessage( hWnd,IDC_SETVEC_SPINY,UDM_SETRANGE,0,MAKELONG(10000,-10000) );
        SendDlgItemMessage( hWnd,IDC_SETVEC_SPINY,UDM_SETPOS,0,MAKELONG((short)pVector->y,0) );
        SendDlgItemMessage( hWnd,IDC_SETVEC_SPINZ,UDM_SETRANGE,0,MAKELONG(10000,-10000) );
        SendDlgItemMessage( hWnd,IDC_SETVEC_SPINZ,UDM_SETPOS,0,MAKELONG((short)pVector->z,0) );

        sprintf( cTemp,"%f",pVector->x );
        SetWindowText( hX,cTemp );
        sprintf( cTemp,"%f",pVector->y );
        SetWindowText( hY,cTemp );
        sprintf( cTemp,"%f",pVector->z );
        SetWindowText( hZ,cTemp );
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        if( wParam == IDOK )
        {
            GetWindowText( hX,cTemp,40 );
            sscanf( cTemp,"%f",&pVector->x );
            GetWindowText( hY,cTemp,40 );
            sscanf( cTemp,"%f",&pVector->y );
            GetWindowText( hZ,cTemp,40 );
            sscanf( cTemp,"%f",&pVector->z );
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// CALLBACK function of the dialog to select entities.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_Entity( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    int i,n;

    // Get the handle of the controls.
    HWND hObjList = GetDlgItem( hWnd,IDC_ETLIST_OBJLIST );
    HWND hMesh = GetDlgItem( hWnd,IDC_ETLIST_MESH );
    HWND hAnimation = GetDlgItem( hWnd,IDC_ETLIST_ANIMATION );
    HWND hParticle = GetDlgItem( hWnd,IDC_ETLIST_PARTICLE );
    HWND hVegetation = GetDlgItem( hWnd,IDC_ETLIST_VEGETATION );
    HWND hSpirit = GetDlgItem( hWnd,IDC_ETLIST_SPIRIT );
    HWND hLiquid = GetDlgItem( hWnd,IDC_ETLIST_LIQUID );

#define YISH_LOVE_NANA 8866983

    // Get the pointer to the entityManager.
    etEntityManager* pEntityMgr = etCoreManager::Instance().GetEntityManager();

    // Get the pointer to the selectionBuffer.
    etSelectionBuffer* pSelBuf = etCoreManager::Instance().GetSelectionBuffer();

    switch( uMsg )
    {
    case WM_INITDIALOG:
        SendMessage( hMesh,BM_SETCHECK,BST_CHECKED,0 );
        SendMessage( hAnimation,BM_SETCHECK,BST_CHECKED,0 );
        SendMessage( hParticle,BM_SETCHECK,BST_CHECKED,0 );
        SendMessage( hVegetation,BM_SETCHECK,BST_CHECKED,0 );
        SendMessage( hSpirit,BM_SETCHECK,BST_CHECKED,0 );
        SendMessage( hLiquid,BM_SETCHECK,BST_CHECKED,0 );

        // Update the entity list.
        SendMessage( hWnd,WM_COMMAND,YISH_LOVE_NANA,0 );
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        switch( wParam )
        {
        case IDC_ETLIST_SELECTALL:
            n = SendMessage( hObjList,LB_GETCOUNT,0,0 );
            for( i=0;i<n;i++ )
                SendMessage( hObjList,LB_SETSEL,(WPARAM)TRUE,i );
            break;
        case IDC_ETLIST_SELECTNONE:
            n = SendMessage( hObjList,LB_GETCOUNT,0,0 );
            for( i=0;i<n;i++ )
                SendMessage( hObjList,LB_SETSEL,(WPARAM)FALSE,i );
            break;
        case IDC_ETLIST_INVERT:
            n = SendMessage( hObjList,LB_GETCOUNT,0,0 );
            for( i=0;i<n;i++ )
            {
                if( SendMessage( hObjList,LB_GETSEL,i,0 ) > 0 )
                    SendMessage( hObjList,LB_SETSEL,(WPARAM)FALSE,i );
                else
                    SendMessage( hObjList,LB_SETSEL,(WPARAM)TRUE,i );
            }
            break;
        case IDC_ETLIST_MESH:
        case IDC_ETLIST_ANIMATION:
        case IDC_ETLIST_PARTICLE:
        case IDC_ETLIST_VEGETATION:
        case IDC_ETLIST_SPIRIT:
        case IDC_ETLIST_LIQUID:
            SendMessage( hWnd,WM_COMMAND,YISH_LOVE_NANA,0 );
            break;
        case IDC_ETLIST_SELECTALLTYPE:
            SendMessage( hMesh,BM_SETCHECK,BST_CHECKED,0 );
            SendMessage( hAnimation,BM_SETCHECK,BST_CHECKED,0 );
            SendMessage( hParticle,BM_SETCHECK,BST_CHECKED,0 );
            SendMessage( hVegetation,BM_SETCHECK,BST_CHECKED,0 );
            SendMessage( hSpirit,BM_SETCHECK,BST_CHECKED,0 );
            SendMessage( hLiquid,BM_SETCHECK,BST_CHECKED,0 );

            // Update the entity list.
            SendMessage( hWnd,WM_COMMAND,YISH_LOVE_NANA,0 );
            break;
        case IDC_ETLIST_SELECTNONETYPE:
            SendMessage( hMesh,BM_SETCHECK,BST_UNCHECKED,0 );
            SendMessage( hAnimation,BM_SETCHECK,BST_UNCHECKED,0 );
            SendMessage( hParticle,BM_SETCHECK,BST_UNCHECKED,0 );
            SendMessage( hVegetation,BM_SETCHECK,BST_UNCHECKED,0 );
            SendMessage( hSpirit,BM_SETCHECK,BST_UNCHECKED,0 );
            SendMessage( hLiquid,BM_SETCHECK,BST_UNCHECKED,0 );

            // Update the entity list.
            SendMessage( hWnd,WM_COMMAND,YISH_LOVE_NANA,0 );
            break;
        case IDC_ETLIST_INVERTTYPE:
            if( SendMessage(hMesh,BM_GETCHECK,0,0) == BST_CHECKED )
                SendMessage( hMesh,BM_SETCHECK,BST_UNCHECKED,0 );
            else
                SendMessage( hMesh,BM_SETCHECK,BST_CHECKED,0 );
            if( SendMessage(hAnimation,BM_GETCHECK,0,0) == BST_CHECKED )
                SendMessage( hAnimation,BM_SETCHECK,BST_UNCHECKED,0 );
            else
                SendMessage( hAnimation,BM_SETCHECK,BST_CHECKED,0 );
            if( SendMessage(hParticle,BM_GETCHECK,0,0) == BST_CHECKED )
                SendMessage( hParticle,BM_SETCHECK,BST_UNCHECKED,0 );
            else
                SendMessage( hParticle,BM_SETCHECK,BST_CHECKED,0 );
            if( SendMessage(hVegetation,BM_GETCHECK,0,0) == BST_CHECKED )
                SendMessage( hVegetation,BM_SETCHECK,BST_UNCHECKED,0 );
            else
                SendMessage( hVegetation,BM_SETCHECK,BST_CHECKED,0 );
            if( SendMessage(hSpirit,BM_GETCHECK,0,0) == BST_CHECKED )
                SendMessage( hSpirit,BM_SETCHECK,BST_UNCHECKED,0 );
            else
                SendMessage( hSpirit,BM_SETCHECK,BST_CHECKED,0 );
            if( SendMessage(hLiquid,BM_GETCHECK,0,0) == BST_CHECKED )
                SendMessage( hLiquid,BM_SETCHECK,BST_UNCHECKED,0 );
            else
                SendMessage( hLiquid,BM_SETCHECK,BST_CHECKED,0 );

            // Update the entity list.
            SendMessage( hWnd,WM_COMMAND,YISH_LOVE_NANA,0 );
            break;
        case YISH_LOVE_NANA:
            SendMessage( hObjList,LB_RESETCONTENT,0,0 );
            for( i=0;i<pEntityMgr->GetNumEntities();i++ )
            {
                etEntity* pEntity = pEntityMgr->GetEntityByIndex( i );

                switch( pEntity->GetEntityType() )
                {
                case ENTITY_MESH:
                    if( SendMessage(hMesh,BM_GETCHECK,0,0) == BST_CHECKED )
                        SendMessage( hObjList,LB_ADDSTRING,0,(LPARAM)pEntity->GetName() );
                    break;
                case ENTITY_ANIMATION:
                    if( SendMessage(hAnimation,BM_GETCHECK,0,0) == BST_CHECKED )
                        SendMessage( hObjList,LB_ADDSTRING,0,(LPARAM)pEntity->GetName() );
                    break;
                case ENTITY_PARTICLE:
                    if( SendMessage(hParticle,BM_GETCHECK,0,0) == BST_CHECKED )
                        SendMessage( hObjList,LB_ADDSTRING,0,(LPARAM)pEntity->GetName() );
                    break;
                case ENTITY_VEGETATION:
                    if( SendMessage(hVegetation,BM_GETCHECK,0,0) == BST_CHECKED )
                        SendMessage( hObjList,LB_ADDSTRING,0,(LPARAM)pEntity->GetName() );
                    break;
                case ENTITY_SPRITE:
                    if( SendMessage(hSpirit,BM_GETCHECK,0,0) == BST_CHECKED )
                        SendMessage( hObjList,LB_ADDSTRING,0,(LPARAM)pEntity->GetName() );
                    break;
                case ENTITY_LIQUID:
                    if( SendMessage(hLiquid,BM_GETCHECK,0,0) == BST_CHECKED )
                        SendMessage( hObjList,LB_ADDSTRING,0,(LPARAM)pEntity->GetName() );
                    break;
                }
            }
            break;
        }

        if( wParam == IDOK )
        {
            if( etCoreManager::Instance().GetWorkMode() == WKM_ENTITY &&
                etCoreManager::Instance().GetCameraMode() == CM_EDITCAMERA )
            {
                pSelBuf->Clear();

                n = SendMessage( hObjList,LB_GETCOUNT,0,0 );
                for( i=0;i<n;i++ )
                {
                    if( SendMessage(hObjList,LB_GETSEL,i,0) > 0 )
                    {
                        etEntity* pEntity = pEntityMgr->GetEntityByIndex( i );
                        pSelBuf->AddEntity( pEntity );
                    }
                }
            }

            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

#undef YISH_LOVE_NANA

    return 0;
}


//------------------------------------------------------------------------
// CALLBACK function for the dialog to show map's information.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_MapInfo( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    int nValue;
    float fValue;
    char cTemp[40];

    // Get the handle to the controls.
    HWND hMapSize = GetDlgItem( hWnd,IDC_MAPA_SIZE );
    HWND hAlphaSize = GetDlgItem( hWnd,IDC_MAPA_ALPHASIZE );
    HWND hTileSize = GetDlgItem( hWnd,IDC_MAPA_TILESIZE );
    HWND hLength = GetDlgItem( hWnd,IDC_MAPA_UNITLENGTH );
    HWND hHScale = GetDlgItem( hWnd,IDC_MAPA_HEIGHTSCALE );
    HWND hETCount = GetDlgItem( hWnd,IDC_MAPA_ENTITYCOUNT );
    HWND hActualSize = GetDlgItem( hWnd,IDC_MAPA_MAPSIZE );

    // Get the pointer to the terrain.
    etTerrain* pTerrain = etCoreManager::Instance().GetSceneManager()->GetTerrainLevel();

    // Get the pointer to the selectionBuffer.
    etEntityManager* pEntityMgr = etCoreManager::Instance().GetEntityManager();

    switch( uMsg )
    {
    case WM_INITDIALOG:
        nValue = pTerrain->GetTerrainSize();
        sprintf( cTemp,"%d x %d",nValue,nValue );
        SetWindowText( hMapSize,cTemp );
        nValue = pTerrain->GetDetailLayer()->GetAlphamapSize();
        sprintf( cTemp,"%d x %d",nValue,nValue );
        SetWindowText( hAlphaSize,cTemp );
        nValue = pTerrain->GetTileSize();
        sprintf( cTemp,"%d x %d",nValue,nValue );
        SetWindowText( hTileSize,cTemp );
        fValue = pTerrain->GetLengthPerUnit();
        sprintf( cTemp,"%f",fValue );
        SetWindowText( hLength,cTemp );
        fValue = pTerrain->GetHeightScale();
        sprintf( cTemp,"%f",fValue );
        SetWindowText( hHScale,cTemp );
        nValue = pEntityMgr->GetNumEntities();
        sprintf( cTemp,"%d",nValue );
        SetWindowText( hETCount,cTemp );
        nValue = pTerrain->GetTerrainSize() * pTerrain->GetLengthPerUnit();
        sprintf( cTemp,"地形实际大小:   %d x %d",nValue,nValue );
        SetWindowText( hActualSize,cTemp );
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        if( wParam == IDOK ) EndDialog( hWnd,1 );
        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// CALLBACK function for the dialog to add the animation template.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_AddAnim( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    char* cTemplateName;

    // Get the handle to the controls.
    HWND hModel = GetDlgItem( hWnd,IDC_ADDANIM_MODEL );
    HWND hAnimation = GetDlgItem( hWnd,IDC_ADDANIM_ANIMATION );
    HWND hFPS = GetDlgItem( hWnd,IDC_ADDANIM_FPS );

    etInterface* pUI = g_pApp->GetInterface();

    // Get the pointer to the entityManager.
    etEntityManager* pEntityMgr = etCoreManager::Instance().GetEntityManager();

    cTemplateName = (char*)GetWindowLong( hWnd,GWL_USERDATA );

    switch( uMsg )
    {
    case WM_INITDIALOG:
        cTemplateName = (char*)lParam;
        SetWindowLong( hWnd,GWL_USERDATA,lParam );

        SetWindowText( hFPS,"10.000000" );
        SendDlgItemMessage( hWnd,IDC_ADDANIM_SPIN1,UDM_SETRANGE,0,MAKELONG(1000,0) );
        SendDlgItemMessage( hWnd,IDC_ADDANIM_SPIN1,UDM_SETPOS,0,MAKELONG(0,0) );
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        switch( wParam )
        {
        case IDC_ADDANIM_SEL1:
            if( pUI->GetToolDialog()->DlgOpenFile(hWnd,
                "模型几何数据(*.amd)\0*.amd") )
            {
                SetWindowText( hModel,pUI->GetToolDialog()->GetFilename() );
            }
            break;
        case IDC_ADDANIM_SEL2:
            if( pUI->GetToolDialog()->DlgOpenFile(hWnd,
                "模型动画数据(*.anm)\0*.anm") )
            {
                SetWindowText( hAnimation,pUI->GetToolDialog()->GetFilename() );
            }
            break;
        }

        if( wParam == IDOK )
        {
            HRESULT hr;
            char cModel[MAX_PATH];
            char cAnimation[MAX_PATH];
            char cTemp[40];
            float fFPS;

            GetWindowText( hModel,cModel,MAX_PATH );
            GetWindowText( hAnimation,cAnimation,MAX_PATH );
            if( strcmp(cModel,"") == 0 )
            {
                MessageBox( hWnd,"请输入模型文件路径！","提示",
                    MB_OK|MB_ICONWARNING );
                return 0;
            }
            if( strcmp(cAnimation,"") == 0 )
            {
                MessageBox( hWnd,"请输入动画文件路径！","提示",
                    MB_OK|MB_ICONWARNING );
                return 0;
            }

            GetWindowText( hFPS,cTemp,20 );
            sscanf( cTemp,"%f",&fFPS );
            hr = pEntityMgr->AddAnimTemplate( cModel,cAnimation,fFPS );
            if( FAILED(hr) )
            {
                pUI->ShowErrorDialog( hr );
                return 0;
            }

            strcpy( cTemplateName,etStringUtil::GetNameFromPath(cModel) );
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// Callback function of the dialog to add a sprite object.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_AddSprite( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    int n;
    HRESULT hr;

    // Get the handle to the controls.
    HWND hXMLPath = GetDlgItem( hWnd,IDC_ADDSPT_XMLPATH );
    HWND hName = GetDlgItem( hWnd,IDC_ADDSPT_NAME );
    HWND hWidth = GetDlgItem( hWnd,IDC_ADDSPT_WIDTH );
    HWND hHeight = GetDlgItem( hWnd,IDC_ADDSPT_HEIGHT );
    HWND hFPS = GetDlgItem( hWnd,IDC_ADDSPT_FPS );
    HWND hBillboardY = GetDlgItem( hWnd,IDC_ADDSPT_BILLBOARDY );
    HWND hTexList = GetDlgItem( hWnd,IDC_ADDSPT_TEXTURELIST );

    cbSpriteTemplate* sprite = (cbSpriteTemplate*)GetWindowLong( hWnd,GWL_USERDATA );

    // Get the pointer to the UI.
    etInterface* pUI = g_pApp->GetInterface();

    switch( uMsg )
    {
    case WM_INITDIALOG:
        sprite = (cbSpriteTemplate*)lParam;
        SetWindowLong( hWnd,GWL_USERDATA,lParam );
        SendDlgItemMessage( hWnd,IDC_ADDSPT_FROMXML,BM_SETCHECK,BST_CHECKED,0 );
        SendDlgItemMessage( hWnd,IDC_ADDSPT_USERDEFINE,BM_SETCHECK,BST_UNCHECKED,0 );
        SendDlgItemMessage( hWnd,IDC_ADDSPT_SPIN1,UDM_SETRANGE,0,MAKELONG(1000,0) );
        SendDlgItemMessage( hWnd,IDC_ADDSPT_SPIN2,UDM_SETRANGE,0,MAKELONG(1000,0) );
        SendDlgItemMessage( hWnd,IDC_ADDSPT_SPIN3,UDM_SETRANGE,0,MAKELONG(100,0) );
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        switch( wParam )
        {
        case IDC_ADDSPT_FROMXML:
            EnableWindow( hXMLPath,TRUE );
            EnableWindow( GetDlgItem(hWnd,IDC_ADDSPT_SELXML),TRUE );
            EnableWindow( hName,FALSE );
            EnableWindow( hWidth,FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_ADDSPT_SPIN1),FALSE );
            EnableWindow( hHeight,FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_ADDSPT_SPIN2),FALSE );
            EnableWindow( hFPS,FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_ADDSPT_SPIN3),FALSE );
            EnableWindow( hBillboardY,FALSE );
            EnableWindow( hTexList,FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_ADDSPT_ADDTEX),FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_ADDSPT_DELTEX),FALSE );
            break;
        case IDC_ADDSPT_USERDEFINE:
            EnableWindow( hXMLPath,FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_ADDSPT_SELXML),FALSE );
            EnableWindow( hName,TRUE );
            EnableWindow( hWidth,TRUE );
            EnableWindow( GetDlgItem(hWnd,IDC_ADDSPT_SPIN1),TRUE );
            EnableWindow( hHeight,TRUE );
            EnableWindow( GetDlgItem(hWnd,IDC_ADDSPT_SPIN2),TRUE );
            EnableWindow( hFPS,TRUE );
            EnableWindow( GetDlgItem(hWnd,IDC_ADDSPT_SPIN3),TRUE );
            EnableWindow( hBillboardY,TRUE );
            EnableWindow( hTexList,TRUE );
            EnableWindow( GetDlgItem(hWnd,IDC_ADDSPT_ADDTEX),TRUE );
            EnableWindow( GetDlgItem(hWnd,IDC_ADDSPT_DELTEX),TRUE );
            break;
        case IDC_ADDSPT_SELXML:
            if( pUI->GetToolDialog()->DlgOpenFile(hWnd,"XML文档(*.xml)\0*.xml") )
                SetWindowText( hXMLPath,pUI->GetToolDialog()->GetFilename() );
            break;
        case IDC_ADDSPT_ADDTEX:
            if( pUI->GetToolDialog()->DlgOpenFile(hWnd,"位图(*.bmp)\0*.bmp") )
            {
                SendMessage( hTexList,LB_ADDSTRING,0,(LPARAM)
                    pUI->GetToolDialog()->GetFilename() );
                sprite->Textures.push_back( pUI->GetToolDialog()->GetFilename() );
            }
            break;
        case IDC_ADDSPT_DELTEX:
            n = SendMessage( hTexList,LB_GETCURSEL,0,0 );
            if( n != LB_ERR  )
            {
                if( MessageBox(hWnd,"确定要删除么？","提示",MB_YESNO|MB_ICONQUESTION)
                    == IDYES )
                {
                    SendMessage( hTexList,LB_DELETESTRING,n,0 );
                    sprite->Textures.erase( sprite->Textures.begin()+n );
                }
            }
            break;
        }

        if( wParam == IDOK )
        {
            char cTemp[MAX_PATH];
            bool bBillboardY;

            // Get the pointer to the entityManager.
            etEntityManager* pEntityMgr = etCoreManager::Instance().GetEntityManager();

            if( SendDlgItemMessage(hWnd,IDC_ADDSPT_FROMXML,BM_GETCHECK,0,0)
                == BST_CHECKED )
            {
                GetWindowText( hXMLPath,cTemp,MAX_PATH );
                sprite->sName = etStringUtil::GetNameFromPath( cTemp );
                hr = pEntityMgr->AddSpriteTemplateFromXML( cTemp );
                if( FAILED(hr) )
                {
                    pUI->ShowErrorDialog( hr );
                    return 0;
                }
            }
            else
            {
                GetWindowText( hName,cTemp,80 );
                sprite->sName = cTemp;
                if( sprite->sName == "" )
                {
                    MessageBox( hWnd,"请输入精灵模板的名字！","警告",MB_OK|MB_ICONWARNING );
                    return 0;
                }

                GetWindowText( hWidth,cTemp,80 );
                sscanf( cTemp,"%f",&sprite->fWidth );
                GetWindowText( hHeight,cTemp,80 );
                sscanf( cTemp,"%f",&sprite->fHeight );
                GetWindowText( hFPS,cTemp,80 );
                sscanf( cTemp,"%f",&sprite->fFPS );
                bBillboardY = (SendDlgItemMessage( hWnd,IDC_ADDSPT_BILLBOARDY,
                    BM_GETCHECK,0,0) == BST_CHECKED);

                hr = pEntityMgr->AddSpriteTemplate( sprite->sName.c_str(),
                    sprite->fWidth,sprite->fHeight,sprite->bBillboardY,
                    sprite->fFPS,sprite->Textures );
                if( FAILED(hr) )
                {
                    pUI->ShowErrorDialog( hr );
                    return 0;
                }
            }

            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// Callback function of the dialog to add a vegetation template.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_AddVeg( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    int nType;
    char cTemp[MAX_PATH];
    float fWidth,fHeight;
    UINT nPolyCount;
    float fX1,fY1,fX2,fY2;
    HRESULT hr;

    // Get the handle of the controls.
    HWND hMesh = GetDlgItem( hWnd,IDC_ADDVEG_MESH );
    HWND hTexture = GetDlgItem( hWnd,IDC_ADDVEG_TEXTURE );
    HWND hWidth = GetDlgItem( hWnd,IDC_ADDVEG_WIDTH );
    HWND hHeight = GetDlgItem( hWnd,IDC_ADDVEG_HEIGHT );
    HWND hCount = GetDlgItem( hWnd,IDC_ADDVEG_POLYCOUNT );
    HWND hX1 = GetDlgItem( hWnd,IDC_ADDVEG_POS1X );
    HWND hY1 = GetDlgItem( hWnd,IDC_ADDVEG_POS1Y );
    HWND hX2 = GetDlgItem( hWnd,IDC_ADDVEG_POS2X );
    HWND hY2 = GetDlgItem( hWnd,IDC_ADDVEG_POS2Y );

    char* cTemplateName = (char*)GetWindowLong( hWnd,GWL_USERDATA );

    etInterface* pUI = g_pApp->GetInterface();

    // Get the pointer to the entityManager.
    etEntityManager* pEntityMgr = etCoreManager::Instance().GetEntityManager();

    switch( uMsg )
    {
    case WM_INITDIALOG:
        cTemplateName = (char*)lParam;
        SetWindowLong( hWnd,GWL_USERDATA,lParam );
        SendDlgItemMessage( hWnd,IDC_ADDVEG_FROMMESH,BM_SETCHECK,BST_CHECKED,0 );
        SendMessage( hCount,CB_ADDSTRING,0,(LPARAM)("1") );
        SendMessage( hCount,CB_ADDSTRING,0,(LPARAM)("2") );
        SendMessage( hCount,CB_ADDSTRING,0,(LPARAM)("3") );
        SendMessage( hCount,CB_ADDSTRING,0,(LPARAM)("4") );
        SendMessage( hCount,CB_SETCURSEL,0,0 );
        SetWindowText( hWidth,"5.0" );
        SetWindowText( hHeight,"5.0" );
        SetWindowText( hX1,"0.0" );
        SetWindowText( hY1,"0.0" );
        SetWindowText( hX2,"1.0" );
        SetWindowText( hY2,"1.0" );
        SendMessage( hWnd,WM_COMMAND,MAKELONG(IDC_ADDVEG_FROMMESH,0),0 );
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        switch( wParam )
        {
        case IDC_ADDVEG_FROMMESH:
            EnableWindow( hMesh,TRUE );
            EnableWindow( GetDlgItem(hWnd,IDC_ADDVEG_SEL1),TRUE );
            EnableWindow( hTexture,FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_ADDVEG_SEL2),FALSE );
            EnableWindow( hWidth,FALSE );
            EnableWindow( hHeight,FALSE );
            EnableWindow( hCount,FALSE );
            EnableWindow( hX1,FALSE );
            EnableWindow( hY1,FALSE );
            EnableWindow( hX2,FALSE );
            EnableWindow( hY2,FALSE );
            break;
        case IDC_ADDVEG_USERDEFINE:
            EnableWindow( hMesh,FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_ADDVEG_SEL1),FALSE );
            EnableWindow( hTexture,TRUE );
            EnableWindow( GetDlgItem(hWnd,IDC_ADDVEG_SEL2),TRUE );
            EnableWindow( hWidth,TRUE );
            EnableWindow( hHeight,TRUE );
            EnableWindow( hCount,TRUE );
            EnableWindow( hX1,TRUE );
            EnableWindow( hY1,TRUE );
            EnableWindow( hX2,TRUE );
            EnableWindow( hY2,TRUE );
            break;
        case IDC_ADDVEG_SEL1:
            if( pUI->GetToolDialog()->DlgOpenFile(hWnd,"静态模型(*.smd)\0*.smd") )
                SetWindowText( hMesh,pUI->GetToolDialog()->GetFilename() );
            break;
        case IDC_ADDVEG_SEL2:
            if( pUI->GetToolDialog()->DlgOpenFile(hWnd,"纹理文件(*.bmp)\0*.bmp") )
                SetWindowText( hTexture,pUI->GetToolDialog()->GetFilename() );
            break;
        }

        if( wParam == IDOK )
        {
            if( SendDlgItemMessage(hWnd,IDC_ADDVEG_FROMMESH,BM_GETCHECK,0,0)
                == BST_CHECKED )
                nType = 1;
            else
                nType = 0;

            if( nType == 1 )
            {
                GetWindowText( hMesh,cTemp,MAX_PATH );
                if( strcmp(cTemp,"") == 0 )
                {
                    MessageBox( hWnd,"请输入模型文件的名字！","警告",MB_OK|MB_ICONWARNING );
                    return 0;
                }

                hr = pEntityMgr->AddMeshVegetation( cTemp );
                if( FAILED(hr) )
                {
                    pUI->ShowErrorDialog( hr );
                    return 0;
                }

                strcpy( cTemplateName,etStringUtil::GetNameFromPath(cTemp) );
            }
            else
            {
                int nSel;
                etFloatRect texRect;
                char cValue[20];

                GetWindowText( hTexture,cTemp,MAX_PATH );
                if( strcmp(cTemp,"") == 0 )
                {
                    MessageBox( hWnd,"请输入纹理的名字！","警告",MB_OK|MB_ICONWARNING );
                    return 0;
                }

                GetWindowText( hWidth,cValue,20 );
                sscanf( cValue,"%f",&fWidth );
                GetWindowText( hHeight,cValue,20 );
                sscanf( cValue,"%f",&fHeight );
                nSel = SendMessage( hCount,CB_GETCURSEL,0,0 );
                SendMessage( hCount,CB_GETLBTEXT,nSel,(LPARAM)cValue );
                sscanf( cValue,"%d",&nPolyCount );
                GetWindowText( hX1,cValue,20 );
                sscanf( cValue,"%f",&fX1 );
                GetWindowText( hY1,cValue,20 );
                sscanf( cValue,"%f",&fY1 );
                GetWindowText( hX2,cValue,20 );
                sscanf( cValue,"%f",&fX2 );
                GetWindowText( hY2,cValue,20 );
                sscanf( cValue,"%f",&fY2 );

                texRect.SetMinPoint( fX1,fY1 );
                texRect.SetMaxPoint( fX2,fY2 );
                hr = pEntityMgr->AddPolygonVegetation( fWidth,fHeight,texRect,
                    cTemp,nPolyCount );
                if( FAILED(hr) )
                {
                    pUI->ShowErrorDialog( hr );
                    return 0;
                }

                strcpy( cTemplateName,etStringUtil::GetNameFromPath(cTemp) );
            }

            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}


//------------------------------------------------------------------------
// CALLBACK function of the dialog to edit the water.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the dialog.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
INT_PTR CALLBACK Callback_EditWater( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    char cTemp[MAX_PATH];

    // Get the handle to the controls.
    HWND hName = GetDlgItem( hWnd,IDC_WATER_NAME );
    HWND hReflect = GetDlgItem( hWnd,IDC_WATER_REFLECTION );
    HWND hSurfaceMap = GetDlgItem( hWnd,IDC_WATER_SURFACEMAP );
    HWND hDistortMap = GetDlgItem( hWnd,IDC_WATER_DISTORTMAP );
    HWND hTexScale = GetDlgItem( hWnd,IDC_WATER_TEXSCALE );
    HWND hDistortion = GetDlgItem( hWnd,IDC_WATER_DISTORTION );
    HWND hFlowSpeed = GetDlgItem( hWnd,IDC_WATER_FLOWSPEED );
    HWND hPosition = GetDlgItem( hWnd,IDC_WATER_POSITION );
    HWND hWidth = GetDlgItem( hWnd,IDC_WATER_WIDTH );
    HWND hHeight = GetDlgItem( hWnd,IDC_WATER_HEIGHT );

    cbLiquidData* liquid = (cbLiquidData*)GetWindowLong( hWnd,GWL_USERDATA );
    etInterface* pUI = g_pApp->GetInterface();

    switch( uMsg )
    {
    case WM_INITDIALOG:
        liquid = (cbLiquidData*)lParam;
        SetWindowLong( hWnd,GWL_USERDATA,lParam );
        SendDlgItemMessage( hWnd,IDC_WATER_SPIN1,UDM_SETRANGE,0,MAKELONG(100,0) );
        SendDlgItemMessage( hWnd,IDC_WATER_SPIN2,UDM_SETRANGE,0,MAKELONG(10,0) );
        SendDlgItemMessage( hWnd,IDC_WATER_SPIN3,UDM_SETRANGE,0,MAKELONG(100,0) );
        SendDlgItemMessage( hWnd,IDC_WATER_SPIN4,UDM_SETRANGE,0,MAKELONG(10000,0) );
        SendDlgItemMessage( hWnd,IDC_WATER_SPIN5,UDM_SETRANGE,0,MAKELONG(10000,0) );

        if( !liquid->bEditLiquid )
        {
            EnableWindow( GetDlgItem(hWnd,IDC_WATER_REFLTERRAIN),FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_WATER_REFLENTITY),FALSE );
            EnableWindow( GetDlgItem(hWnd,IDC_WATER_REFLSKY),FALSE );
            SendDlgItemMessage( hWnd,IDC_WATER_SPIN1,UDM_SETPOS,0,MAKELONG(0,0) );
            SendDlgItemMessage( hWnd,IDC_WATER_SPIN2,UDM_SETPOS,0,MAKELONG(0,0) );
            SendDlgItemMessage( hWnd,IDC_WATER_SPIN3,UDM_SETPOS,0,MAKELONG(0,0) );
            SendDlgItemMessage( hWnd,IDC_WATER_SPIN4,UDM_SETPOS,0,MAKELONG(0,0) );
            SendDlgItemMessage( hWnd,IDC_WATER_SPIN5,UDM_SETPOS,0,MAKELONG(0,0) );
            SetWindowText( hPosition,"(0.000,0.000,0.000)" );
            SetWindowText( hWidth,"100" );
            SetWindowText( hHeight,"100" );
            SetWindowText( hTexScale,"1.000000" );
            SetWindowText( hDistortion,"8.000000" );
            SetWindowText( hFlowSpeed,"1.000000" );
        }
        else
        {
            SetWindowText( hName,liquid->sName.c_str() );
            EnableWindow( hName,FALSE );
            if( liquid->bReflection )
                SendDlgItemMessage( hWnd,IDC_WATER_REFLECTION,BM_SETCHECK,BST_CHECKED,0 );
            SendMessage( hWnd,WM_COMMAND,IDC_WATER_REFLECTION,0 );

            SetWindowText( hSurfaceMap,liquid->sSurfaceMap.c_str() );
            SetWindowText( hDistortMap,liquid->sDistortMap.c_str() );
            sprintf( cTemp,"(%.3f,%.3f,%.3f)",liquid->vPosition.x,liquid->vPosition.y,liquid->vPosition.z );
            SetWindowText( hPosition,cTemp );
            sprintf( cTemp,"%f",liquid->fWidth );
            SetWindowText( hWidth,cTemp );
            sprintf( cTemp,"%f",liquid->fHeight );
            SetWindowText( hHeight,cTemp );
            sprintf( cTemp,"%f",liquid->fTexScale );
            SetWindowText( hTexScale,cTemp );
            SendDlgItemMessage( hWnd,IDC_WATER_SPIN1,UDM_SETPOS,0,MAKELONG((short)liquid->fTexScale,0) );
            sprintf( cTemp,"%f",liquid->fDistortion );
            SetWindowText( hDistortion,cTemp );
            SendDlgItemMessage( hWnd,IDC_WATER_SPIN2,UDM_SETPOS,0,MAKELONG((short)liquid->fDistortion,0) );
            sprintf( cTemp,"%f",liquid->fFlowSpeed );
            SetWindowText( hFlowSpeed,cTemp );
            SendDlgItemMessage( hWnd,IDC_WATER_SPIN3,UDM_SETPOS,0,MAKELONG((short)liquid->fFlowSpeed,0) );
            SendDlgItemMessage( hWnd,IDC_WATER_REFLTERRAIN,BM_SETCHECK,
                liquid->bReflectTerrain ? BST_CHECKED : BST_UNCHECKED,0 );
            SendDlgItemMessage( hWnd,IDC_WATER_REFLENTITY,BM_SETCHECK,
                liquid->bReflectEntity ? BST_CHECKED : BST_UNCHECKED,0 );
            SendDlgItemMessage( hWnd,IDC_WATER_REFLSKY,BM_SETCHECK,
                liquid->bReflectSkybox ? BST_CHECKED : BST_UNCHECKED,0 );
        }
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        switch( wParam )
        {
        case IDC_WATER_POSITION:
            if( pUI->ShowVectorSetting((etInterface::sVector*)&liquid->vPosition) )
            {
                sprintf( cTemp,"(%.3f,%.3f,%.3f)",liquid->vPosition.x,liquid->vPosition.y,
                    liquid->vPosition.z );
                SetWindowText( hPosition,cTemp );
            }
            break;
        case IDC_WATER_REFLECTION:
            if( SendDlgItemMessage(hWnd,IDC_WATER_REFLECTION,BM_GETCHECK,0,0)
                == BST_CHECKED )
            {
                EnableWindow( GetDlgItem(hWnd,IDC_WATER_REFLTERRAIN),TRUE );
                EnableWindow( GetDlgItem(hWnd,IDC_WATER_REFLENTITY),TRUE );
                EnableWindow( GetDlgItem(hWnd,IDC_WATER_REFLSKY),TRUE );
            }
            else
            {
                EnableWindow( GetDlgItem(hWnd,IDC_WATER_REFLTERRAIN),FALSE );
                EnableWindow( GetDlgItem(hWnd,IDC_WATER_REFLENTITY),FALSE );
                EnableWindow( GetDlgItem(hWnd,IDC_WATER_REFLSKY),FALSE );
            }
            break;
        case IDC_WATER_SEL1:
            if( pUI->GetToolDialog()->DlgOpenFile(hWnd,"位图(*.bmp)\0*.bmp") )
                SetWindowText( hSurfaceMap,pUI->GetToolDialog()->GetFilename() );
            break;
        case IDC_WATER_SEL2:
            if( pUI->GetToolDialog()->DlgOpenFile(hWnd,"位图(*.bmp)\0*.bmp") )
                SetWindowText( hDistortMap,pUI->GetToolDialog()->GetFilename() );
            break;
        }

        if( wParam == IDOK )
        {
            GetWindowText( hName,cTemp,MAX_PATH );
            liquid->sName = cTemp;
            if( liquid->sName == "" )
            {
                MessageBox( hWnd,"请输入水面对象的名字！","警告",MB_OK|MB_ICONERROR );
                return 0;
            }
            liquid->bReflection = (SendDlgItemMessage(hWnd,IDC_WATER_REFLECTION,
                BM_GETCHECK,0,0) == BST_CHECKED);
            GetWindowText( hSurfaceMap,cTemp,MAX_PATH );
            liquid->sSurfaceMap = cTemp;
            if( liquid->sSurfaceMap == "" )
            {
                MessageBox( hWnd,"请选择一个表面纹理！","警告",MB_OK|MB_ICONWARNING );
                return 0;
            }
            GetWindowText( hDistortMap,cTemp,MAX_PATH );
            liquid->sDistortMap = cTemp;
            if( liquid->sDistortMap == "" )
            {
                MessageBox( hWnd,"请选择一个扭曲纹理！","警告",MB_OK|MB_ICONWARNING );
                return 0;
            }
            GetWindowText( hPosition,cTemp,MAX_PATH );
            sscanf( cTemp,"(%f,%f,%f)",&liquid->vPosition.x,&liquid->vPosition.y,
                &liquid->vPosition.z );
            GetWindowText( hWidth,cTemp,MAX_PATH );
            sscanf( cTemp,"%f",&liquid->fWidth );
            GetWindowText( hHeight,cTemp,MAX_PATH );
            sscanf( cTemp,"%f",&liquid->fHeight );
            GetWindowText( hTexScale,cTemp,MAX_PATH );
            sscanf( cTemp,"%f",&liquid->fTexScale );
            GetWindowText( hDistortion,cTemp,MAX_PATH );
            sscanf( cTemp,"%f",&liquid->fDistortion );
            GetWindowText( hFlowSpeed,cTemp,MAX_PATH );
            sscanf( cTemp,"%f",&liquid->fFlowSpeed );
            liquid->bReflectTerrain = (SendDlgItemMessage(hWnd,IDC_WATER_REFLTERRAIN,
                BM_GETCHECK,0,0) == BST_CHECKED);
            liquid->bReflectEntity = (SendDlgItemMessage(hWnd,IDC_WATER_REFLENTITY,
                BM_GETCHECK,0,0) == BST_CHECKED);
            liquid->bReflectSkybox = (SendDlgItemMessage(hWnd,IDC_WATER_REFLSKY,
                BM_GETCHECK,0,0) == BST_CHECKED);
            EndDialog( hWnd,1 );
        }

        if( wParam == IDCANCEL ) EndDialog( hWnd,0 );
        break;
    }

    return 0;
}