//------------------------------------------------------------------------
// Name: AppUIManager.cpp
// Desc: This file is to implement the class CAppUIManager.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "AppUIManager.h"
#include "CallbackFunc.h"
#include "Application.h"


extern CApplication* g_pApp;

//------------------------------------------------------------------------
// Initialize the application's UIManager.
// ----------------------------------------------------------------------
// Param -> IN:
//      HINSTANCE:  Handle to the application.
//      HWND:       Handle to the main window.
//------------------------------------------------------------------------
bool CAppUIManager::Init( HINSTANCE hInst,HWND hWnd )
{
    m_hInst = hInst;
    m_hWnd  = hWnd;

    m_uiMgr.InitGUIManager( hInst,hWnd );

    m_uiStatus.CreateStatusBar( hWnd,3 );
    m_uiStatus.SetStatusText( 0,"就绪" );

    return CreateControls();
}


//------------------------------------------------------------------------
// Create controls for the particle editor.
//------------------------------------------------------------------------
bool CAppUIManager::CreateControls(void)
{
    POINT pt;

    pt.x = 15; pt.y = 15;
    m_uiMgr.AddGUINode( IDC_PARTEMPLATE,GUI_GROUPBOX,"粒子模板",pt,270,240 );
    pt.x = 25; pt.y = 35;
    m_uiMgr.AddGUINode( IDC_PARLIST,GUI_LISTBOX,TEXT(""),pt,250,195 );
    pt.x = 30; pt.y = 220;
    m_uiMgr.AddGUINode( IDC_ADDPARTEM,GUI_BUTTON,"添加",pt,70,22 );
    pt.x = 115; pt.y = 220;
    m_uiMgr.AddGUINode( IDC_DELPARTEM,GUI_BUTTON,"删除",pt,70,22 );
    pt.x = 200; pt.y = 220;
    m_uiMgr.AddGUINode( IDC_EDITPARTEM,GUI_BUTTON,"编辑",pt,70,22 );

    pt.x = 15; pt.y = 265;
    m_uiMgr.AddGUINode( IDC_EDITPARTEM,GUI_GROUPBOX,"发射器模板",pt,270,245 );
    pt.x = 25; pt.y = 285;
    m_uiMgr.AddGUINode( IDC_EMITTERLIST,GUI_LISTBOX,TEXT(""),pt,250,195 );
    pt.x = 30; pt.y = 470;
    m_uiMgr.AddGUINode( IDC_ADDEMITTER,GUI_BUTTON,"添加",pt,70,22 );
    pt.x = 115; pt.y = 470;
    m_uiMgr.AddGUINode( IDC_DELEMITTER,GUI_BUTTON,"删除",pt,70,22 );
    pt.x = 200; pt.y = 470;
    m_uiMgr.AddGUINode( IDC_EDITEMITTER,GUI_BUTTON,"编辑",pt,70,22 );

    pt.x = 300; pt.y = 370;
    m_uiMgr.AddGUINode( IDC_CONTROLLER,GUI_GROUPBOX,"控制器列表",pt,290,154);
    pt.x = 310; pt.y = 388;
    m_uiMgr.AddGUINode( IDC_CONTROLLERLIST,GUI_LISTBOX,"",pt,200,135 );
    pt.x = 515; pt.y = 390;
    m_uiMgr.AddGUINode( IDC_ADDCONTROLLER,GUI_BUTTON,"添加",pt,60,22 );
    pt.x = 515; pt.y = 423;
    m_uiMgr.AddGUINode( IDC_DELCONTROLLER,GUI_BUTTON,"删除",pt,60,22 );
    pt.x = 515; pt.y = 456;
    m_uiMgr.AddGUINode( IDC_EDITCONTROLLER,GUI_BUTTON,"编辑",pt,60,22 );
    pt.x = 515; pt.y = 489;
    m_uiMgr.AddGUINode( IDC_HIDECONTROLLER,GUI_BUTTON,"隐藏",pt,60,22 );

    pt.x = 597; pt.y = 370;
    m_uiMgr.AddGUINode( IDC_SCENE,GUI_GROUPBOX,"场景控制",pt,182,154 );
    pt.x = 607; pt.y = 390;
    m_uiMgr.AddGUINode( IDC_NEWSCENE,GUI_BUTTON,"新建场景",pt,80,25 );
    pt.x = 687; pt.y = 390;
    m_uiMgr.AddGUINode( IDC_LOADSCENE,GUI_BUTTON,"载入场景",pt,80,25 );
    pt.x = 607; pt.y = 415;
    m_uiMgr.AddGUINode( IDC_SAVESCENE,GUI_BUTTON,"保存场景",pt,80,25 );
    pt.x = 687; pt.y = 415;
    m_uiMgr.AddGUINode( IDC_SAVEAS,GUI_BUTTON,"另存为...",pt,80,25 );
    pt.x = 607; pt.y = 460;
    m_uiMgr.AddGUINode( IDC_PREVIEW,GUI_BUTTON,"预览场景",pt,80,25 );
    pt.x = 687; pt.y = 460;
    m_uiMgr.AddGUINode( IDC_STOPPREVIEW,GUI_BUTTON,"停止预览",pt,80,25 );
    pt.x = 607; pt.y = 485;
    m_uiMgr.AddGUINode( IDC_RESETCAM,GUI_BUTTON,"重置相机",pt,160,25 );

    return true;
}


//------------------------------------------------------------------------
// Function to handle the event for controls.
// ----------------------------------------------------------------------
// Param -> IN:
//      WPARAM:     Specified the ID of controls.
//      LPARAM:     Specified the handle of the controls.
//------------------------------------------------------------------------
void CAppUIManager::MsgProcGUI( WPARAM wParam,LPARAM lParam )
{
    char cTmp[40];
    int n,i,nRes;

    // Get the controls' ID.
    UINT nID = m_uiMgr.GetIDFromHandle( (HWND)lParam );

    FlyParticleSystem* pSystem = g_pApp->GetAppRenderer()->GetParticleSystem();
    FlyParticleNode* pNode = pSystem->GetSceneNode(0);

    switch( nID )
    {
    case IDC_PARLIST:
        n = m_uiMgr.GetCurSel( IDC_PARLIST );
        if( HIWORD(wParam) == LBN_DBLCLK && n != -1 )
        {
            g_pApp->GetAppRenderer()->Preview( false );
            nRes = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_ADDPARTEM,m_hWnd,Callback_ModPar,
                                   (LPARAM)pNode->GetParticle(n) );

            if( nRes ) g_pApp->m_bChanged = true;
        }
        break;
    case IDC_ADDPARTEM:
        g_pApp->GetAppRenderer()->Preview( false );
        nRes = DialogBox( m_hInst,(LPCTSTR)IDD_DLG_ADDPARTEM,m_hWnd,Callback_AddPar );
        if( nRes ) g_pApp->m_bChanged = true;
        break;
    case IDC_DELPARTEM:
        n = m_uiMgr.GetCurSel( IDC_PARLIST );
        if( n != -1 )
        {
            if( MessageBox(m_hWnd,"删除粒子模板，使用该模板的控制器也会被删除，确定要删除么？",
                "Info",MB_YESNO|MB_ICONQUESTION) == IDYES )
            {
                for( i=0;i<pNode->GetNumControllers();i++ )
                {
                    if( pNode->GetController(i)->GetParticlePtr() == pNode->GetParticle(n) )
                    {
                        m_uiMgr.DeleteItem( IDC_CONTROLLERLIST,i );
                    }
                }

                pNode->DeleteParticle( n );
                m_uiMgr.DeleteItem( IDC_PARLIST,n );
                g_pApp->m_bChanged = true;
            }

            g_pApp->GetAppRenderer()->Preview( false );
        }
        break;
    case IDC_EDITPARTEM:
        n = m_uiMgr.GetCurSel( IDC_PARLIST );
        if( n != -1 )
        {
            g_pApp->GetAppRenderer()->Preview( false );
            nRes = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_ADDPARTEM,m_hWnd,Callback_ModPar,
                                   (LPARAM)pNode->GetParticle(n) );

            if( nRes ) g_pApp->m_bChanged = true;
        }
        break;
    case IDC_EMITTERLIST:
        n = m_uiMgr.GetCurSel( IDC_EMITTERLIST );
        if( HIWORD(wParam) == LBN_DBLCLK && n != -1 )
        {
            g_pApp->GetAppRenderer()->Preview( false );
            nRes = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_ADDEMITTER,m_hWnd,Callback_ModEmitter,
                                   (LPARAM)pNode->GetEmitter(n) );

            if( nRes ) g_pApp->m_bChanged = true;
        }
        break;
    case IDC_ADDEMITTER:
        g_pApp->GetAppRenderer()->Preview( false );
        nRes = DialogBox( m_hInst,(LPCTSTR)IDD_DLG_ADDEMITTER,m_hWnd,Callback_AddEmitter );
        if( nRes ) g_pApp->m_bChanged = true;
        break;
    case IDC_DELEMITTER:
        n = m_uiMgr.GetCurSel( IDC_EMITTERLIST );
        if( n != -1 )
        {
            if( MessageBox(m_hWnd,"删除发射器，使用该发射器的控制器也会被删除，确定要删除么？",
                "Info",MB_YESNO|MB_ICONQUESTION) == IDYES )
            {
                for( i=0;i<pNode->GetNumControllers();i++ )
                {
                    if( pNode->GetController(i)->GetParticleEmitterPtr() == pNode->GetEmitter(n) )
                    {
                        m_uiMgr.DeleteItem( IDC_CONTROLLERLIST,i );
                    }
                }

                pNode->DeleteEmitter( n );
                m_uiMgr.DeleteItem( IDC_EMITTERLIST,n );
                g_pApp->m_bChanged = true;
            }

            g_pApp->GetAppRenderer()->Preview( false );
        }
        break;
    case IDC_EDITEMITTER:
        n = m_uiMgr.GetCurSel( IDC_EMITTERLIST );
        if( n != -1 )
        {
            g_pApp->GetAppRenderer()->Preview( false );
            nRes = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_ADDEMITTER,m_hWnd,Callback_ModEmitter,
                                   (LPARAM)pNode->GetEmitter(n) );

            if( nRes ) g_pApp->m_bChanged = true;
        }
        break;
    case IDC_CONTROLLERLIST:
        n = m_uiMgr.GetCurSel( IDC_CONTROLLERLIST );
        if( HIWORD(wParam) == LBN_DBLCLK && n != -1 )
        {
            g_pApp->GetAppRenderer()->Preview( false );
            nRes = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_ADDCONTROLLER,m_hWnd,Callback_ModController,
                                   (LPARAM)pNode->GetController(n) );

            if( nRes ) g_pApp->m_bChanged = true;
        }
        break;
    case IDC_ADDCONTROLLER:
        if( pSystem->GetSceneNode(0)->GetNumParTemplates() == 0 )
        {
            MessageBox( NULL,"缺少粒子模板！","Warning",MB_OK|MB_ICONWARNING );
            return;
        }
        if( pSystem->GetSceneNode(0)->GetNumEmiTemplates() == 0 )
        {
            MessageBox( NULL,"缺少粒子发射器！","Warning",MB_OK|MB_ICONWARNING );
            return;
        }
        nRes = DialogBox( m_hInst,(LPCTSTR)IDD_DLG_ADDCONTROLLER,m_hWnd,Callback_AddController );
        g_pApp->GetAppRenderer()->Preview( false );
        if( nRes ) g_pApp->m_bChanged = true;
        break;
    case IDC_DELCONTROLLER:
        n = m_uiMgr.GetCurSel( IDC_CONTROLLERLIST );
        if( n != -1 )
        {
            if( MessageBox(m_hWnd,"确定要删除么？","Warning",MB_YESNO|MB_ICONQUESTION)
                == IDYES )
            {
                pNode->DeleteController( n );
                m_uiMgr.DeleteItem( IDC_CONTROLLERLIST,n );
                g_pApp->m_bChanged = true;
            }

            g_pApp->GetAppRenderer()->Preview( false );
        }
        break;
    case IDC_EDITCONTROLLER:
        n = m_uiMgr.GetCurSel( IDC_CONTROLLERLIST );
        if( n != -1 )
        {
            g_pApp->GetAppRenderer()->Preview( false );
            nRes = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_ADDCONTROLLER,m_hWnd,Callback_ModController,
                                   (LPARAM)pNode->GetController(n) );

            if( nRes ) g_pApp->m_bChanged = true;
        }
        break;
    case IDC_HIDECONTROLLER:
        n = m_uiMgr.GetCurSel( IDC_CONTROLLERLIST );
        if( n != -1 )
        {
            bool b = pNode->GetController(n)->IsShow();
            pNode->GetController(n)->Show( !b );

            if( !b )
            {
                strcpy( cTmp,pNode->GetController(n)->GetName() );
                m_uiMgr.SetItemString( IDC_CONTROLLERLIST,n,cTmp );
            }
            else
            {
                strcpy( cTmp,pNode->GetController(n)->GetName() );
                strcat( cTmp,"(Disable)" );
                m_uiMgr.SetItemString( IDC_CONTROLLERLIST,n,cTmp );
            }
        }
        break;
    case IDC_NEWSCENE:
        g_pApp->MsgProcMenu( IDM_FILE_NEW );
        break;
    case IDC_LOADSCENE:
        g_pApp->MsgProcMenu( IDM_FILE_OPEN );
        break;
    case IDC_SAVESCENE:
        g_pApp->MsgProcMenu( IDM_FILE_SAVE );
        break;
    case IDC_SAVEAS:
        g_pApp->MsgProcMenu( IDM_FILE_SAVEAS );
        break;
    case IDC_PREVIEW:
        g_pApp->MsgProcMenu( IDM_VIEW_PREVIEW );
        break;
    case IDC_STOPPREVIEW:
        g_pApp->MsgProcMenu( IDM_VIEW_STOPPREVIEW );
        break;
    case IDC_RESETCAM:
        g_pApp->MsgProcMenu( IDM_VIEW_RESETCAM );
        break;
    }
}


//------------------------------------------------------------------------
// Open the dialog to set a vector's value.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the parent dialog.
//      FlyVector*: To store the vector.
//------------------------------------------------------------------------
bool CAppUIManager::ShowVectorSetting( HWND hWnd,FlyVector* pVec )
{
    INT nResult;
    nResult = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_SETVECTOR,hWnd,Callback_SetVector,
                              (LPARAM)pVec );

    return (nResult == 1);
}


//------------------------------------------------------------------------
// Open the dialog to select a texture.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the parent dialog.
//      char*:      To store the file name.
//------------------------------------------------------------------------
bool CAppUIManager::ShowSelTextureDlg( HWND hWnd,char* cFilename )
{
    INT nResult;

    nResult = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_SELTEX,hWnd,Callback_SelTex,
                              (LPARAM)cFilename );

    return (nResult == 1);
}


//------------------------------------------------------------------------
// Open the dialog to select a shape file.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the parent dialog.
//      char*:      To store the shape file name.
//------------------------------------------------------------------------
bool CAppUIManager::ShowSelShapeDlg( HWND hWnd,char* cFilename )
{
    INT nResult;

    nResult = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_SELSHAPE,hWnd,Callback_SelShape,
                              (LPARAM)cFilename );

    return (nResult == 1);
}