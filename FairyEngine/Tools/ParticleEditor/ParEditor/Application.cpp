//------------------------------------------------------------------------
// Name: Application.cpp
// Desc: This file is to implement the class CApplication.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------

#define _WIN32_WINNT   0x0400

#include "Application.h"
#include "CallbackFunc.h"


CApplication* g_pApp;

//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
CApplication::CApplication(void)
{
    m_hInst = NULL;
    m_hWnd  = NULL;
    strcpy( m_cCurFile,"" );
    m_bChanged = false;

    g_pApp = this;
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
CApplication::~CApplication(void)
{
    Release();
}


//------------------------------------------------------------------------
// Release the application, clean all the resources.
//------------------------------------------------------------------------
void CApplication::Release(void)
{
}


//------------------------------------------------------------------------
// Create an instance for the application.
// ----------------------------------------------------------------------
// Param -> IN:
//      HINSTANCE:      Handle for the application.
//------------------------------------------------------------------------
BOOL CApplication::Create( HINSTANCE hInst )
{
    m_hInst = hInst;

    WNDCLASSEX wnd;
    memset( &wnd,0,sizeof(WNDCLASSEX) );
    wnd.cbSize          = sizeof(WNDCLASSEX);
    wnd.cbClsExtra      = 0;
    wnd.cbWndExtra      = 0;
    wnd.hbrBackground   = (HBRUSH)(COLOR_WINDOW);
    wnd.hCursor         = LoadCursor( NULL,IDC_ARROW );
    wnd.hIcon           = LoadIcon( m_hInst,(LPCTSTR)IDI_MAINWINDOW );
    wnd.hIconSm         = NULL;
    wnd.hInstance       = hInst;
    wnd.lpfnWndProc     = (WNDPROC)WndProc;
    wnd.lpszClassName   = g_szWndClass;
    wnd.lpszMenuName    = (LPCSTR)IDR_MAINMENU;
    wnd.style           = CS_HREDRAW|CS_VREDRAW;

    RegisterClassEx( &wnd );

    DWORD dw = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX;

    // Create the window.
    m_hWnd = CreateWindow( g_szWndClass,g_szWndTitle,dw,0,0,792,595,NULL,NULL,hInst,NULL );

    if( !m_hWnd )
    {
        MessageBox( NULL,"创建窗口失败，程序即将退出！","Error",MB_OK|MB_ICONERROR );
        return FALSE;
    }

    ShowWindow( m_hWnd,SW_SHOW );
    UpdateWindow( m_hWnd );

    return !FAILED(InitApplication());
}


//------------------------------------------------------------------------
// Run the application, Enter the main loop.
//------------------------------------------------------------------------
INT CApplication::Run(void)
{
    MSG msg;
    memset( &msg,0,sizeof(MSG) );

    while( msg.message != WM_QUIT )
    {
        if( PeekMessage(&msg,NULL,0,0,PM_REMOVE) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            if( !ProgramTick() ) break;
        }
    }

    // Shutdown the application.
    Release();

    UnregisterClass( g_szWndClass,m_hInst );
    return msg.wParam;
}


//------------------------------------------------------------------------
// Inialize the application for the editor.
//------------------------------------------------------------------------
HRESULT CApplication::InitApplication(void)
{
    // Create the RenderWindow.
    m_hRender = CreateWindowEx( WS_EX_CLIENTEDGE,"STATIC",NULL,WS_CHILD|WS_BORDER,300,5,480,360, 
                                m_hWnd,NULL,m_hInst,NULL );

    ShowWindow( m_hRender,SW_SHOW );
    UpdateWindow( m_hRender );

    // Create the client area.
    HWND hClient = CreateWindowEx( WS_EX_CLIENTEDGE,"STATIC",NULL,WS_CHILD|WS_BORDER,5,5,290,520, 
                                   m_hWnd,NULL,m_hInst,NULL );

    ShowWindow( hClient,SW_SHOW );
    UpdateWindow( hClient );

    m_AppUI.Init( m_hInst,m_hWnd );
    m_AppKF.Create( m_hInst,m_hWnd );

    // Create the editor's Renderer.
    m_Renderer.Create( m_hInst,m_hWnd,m_hRender );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Do something at every frame, update and render.
//------------------------------------------------------------------------
bool CApplication::ProgramTick(void)
{
    char cTemp[40];

    m_Renderer.Update();
    m_Renderer.Render();

    sprintf( cTemp,"当前粒子数目: %d",m_Renderer.GetNumParticles() );
    m_AppUI.GetStatusBar()->SetStatusText( 1,cTemp );
    sprintf( cTemp,"Current FPS: %.3f",m_Renderer.GetFPS() );
    m_AppUI.GetStatusBar()->SetStatusText( 2,cTemp );

    return true;
}


//------------------------------------------------------------------------
// Some events to run before the application exit.
//------------------------------------------------------------------------
void CApplication::OnExit(void)
{
    int nRes;

    FlyParticleNode* pNode = m_Renderer.GetParticleSystem()->GetSceneNode(0);

    if( m_bChanged )
    {
        nRes = MessageBox( m_hWnd,"场景已改变，要保存么？","Warning",
                           MB_YESNOCANCEL|MB_ICONWARNING );

        if( nRes == IDYES )
        {
            if( strcmp(m_cCurFile,"") == 0 )
            {
                if( m_AppUI.GetToolDialog()->DlgSaveFile(m_hWnd,"粒子文件(*.PAR)\0*.PAR") )
                {
                    strcpy( m_cCurFile,m_AppUI.GetToolDialog()->GetFilename() );
                    strcat( m_cCurFile,".PAR" );

                    pNode->WriteParticleToPAR( m_cCurFile );
                    PostQuitMessage(0);
                }
            }
            else
            {
                pNode->WriteParticleToPAR( m_cCurFile );
                PostQuitMessage(0);
            }
        }
        else if( nRes == IDNO )
        {
            PostQuitMessage(0);
        }
    }
    else PostQuitMessage(0);
}


//------------------------------------------------------------------------
// Handle the event that move the cursor.
// ----------------------------------------------------------------------
// Param -> IN:
//      WPARAM:     Key indicators of this event.
//      LPARAM:     Position of the cursor.
//------------------------------------------------------------------------
void CApplication::OnMouseMove( WPARAM wParam,LPARAM lParam )
{
    static POINT s_pt = { LOWORD(lParam),HIWORD(lParam) };

    POINT pt;
    GetCursorPos( &pt );
    ScreenToClient( m_hRender,&pt );

    short lDiffX = pt.x - s_pt.x;
    short lDiffY = pt.y - s_pt.y;

    CAppCamera* pCam = m_Renderer.GetCameraPtr();

    if( wParam & MK_RBUTTON )
    {
        if( lDiffY > 0 ) pCam->MoveAlongY( -0.1f );
        if( lDiffY < 0 ) pCam->MoveAlongY(  0.1f );
    }
    else if( wParam & MK_LBUTTON )
    {
        if( lDiffX > 0 ) pCam->Yaw(  0.05f );
        if( lDiffX < 0 ) pCam->Yaw( -0.05f );
    }

    s_pt = pt;
}


//------------------------------------------------------------------------
// Handle the event that scroll the wheel of the mouse.
// ----------------------------------------------------------------------
// Param -> IN:
//      WPARAM:     Key indicators of this event.
//      LPARAM:     horizontal and vertical position
//------------------------------------------------------------------------
void CApplication::OnMouseWheel( WPARAM wParam,LPARAM lParam )
{
    short lDelta = (short)HIWORD( wParam );

    if( lDelta > 0 ) m_Renderer.GetCameraPtr()->Pitch(  0.05f );
    if( lDelta < 0 ) m_Renderer.GetCameraPtr()->Pitch( -0.05f );
}


//------------------------------------------------------------------------
// CALLBACK Function to handle the events for main window.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle of the window.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
void CApplication::MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    POINT pt;
    RECT rcRender;
    HWND hList[3];

    switch( uMsg )
    {
    case WM_COMMAND:
        MsgProcMenu( wParam );
        m_AppUI.MsgProcGUI( wParam,lParam );
        break;
    case WM_MOUSEMOVE:
        GetCursorPos( &pt );
        GetClientRect( m_hRender,&rcRender );
        ScreenToClient( m_hRender,&pt );
        if( PtInRect(&rcRender,pt) )
        {
            hList[0] = m_AppUI.GetUIManager()->GetSafeHandle( IDC_PARLIST );
            hList[1] = m_AppUI.GetUIManager()->GetSafeHandle( IDC_EMITTERLIST );
            hList[2] = m_AppUI.GetUIManager()->GetSafeHandle( IDC_CONTROLLERLIST );

            if( GetFocus() == hList[0] ||
                GetFocus() == hList[1] ||
                GetFocus() == hList[2] )
            {
                SetFocus( m_hWnd );
            }

            OnMouseMove( wParam,lParam );
        }
        break;
    case WM_MOUSEWHEEL:
        GetCursorPos( &pt );
        GetClientRect( m_hRender,&rcRender );
        ScreenToClient( m_hRender,&pt );
        if( PtInRect(&rcRender,pt) ) OnMouseWheel( wParam,lParam );
        break;
    }
}


//------------------------------------------------------------------------
// This function handle the events when click the menu.
// ----------------------------------------------------------------------
// Param -> IN:
//      WPARAM:     Specified the ID of menu items.
//------------------------------------------------------------------------
void CApplication::MsgProcMenu( WPARAM wParam )
{
    int n;
    char cTemp[256];

    // Get the scene node.
    FlyParticleNode* pNode = m_Renderer.GetParticleSystem()->GetSceneNode(0);

    switch( wParam )
    {
    case IDM_FILE_NEW:
        pNode->Release();
        m_AppUI.GetUIManager()->ClearItems( IDC_PARLIST );
        m_AppUI.GetUIManager()->ClearItems( IDC_EMITTERLIST );
        m_AppUI.GetUIManager()->ClearItems( IDC_CONTROLLERLIST );
        strcpy( m_cCurFile,"" );
        m_Renderer.Preview( false );
        m_bChanged = false;
        break;
    case IDM_FILE_OPEN:
        if( m_AppUI.GetToolDialog()->DlgOpenFile(m_hWnd,"粒子文件(*.PAR)\0*.PAR") )
        {
            strcpy( m_cCurFile,m_AppUI.GetToolDialog()->GetFilename() );
            if( !pNode->LoadParticleFromPAR(m_cCurFile) )
            {
                MessageBox( m_hWnd,"不是有效的粒子文件！","Error",MB_OK|MB_ICONERROR );
                return;
            }

            m_AppUI.GetUIManager()->ClearItems( IDC_PARLIST );
            m_AppUI.GetUIManager()->ClearItems( IDC_EMITTERLIST );
            m_AppUI.GetUIManager()->ClearItems( IDC_CONTROLLERLIST );
            for( n=0;n<pNode->GetNumParTemplates();n++ )
            {
                m_AppUI.GetUIManager()->AddItem( IDC_PARLIST,(char*)pNode->GetParticle(n)->GetName(),NULL );
            }

            for( n=0;n<pNode->GetNumEmiTemplates();n++ )
            {
                m_AppUI.GetUIManager()->AddItem( IDC_EMITTERLIST,(char*)pNode->GetEmitter(n)->GetName(),NULL );
            }

            for( n=0;n<pNode->GetNumControllers();n++ )
            {
                m_AppUI.GetUIManager()->AddItem( IDC_CONTROLLERLIST,(char*)pNode->GetController(n)->GetName(),NULL );
            }

            m_Renderer.Preview( false );
            m_bChanged = false;
        }
        break;
    case IDM_FILE_SAVE:
        if( strcmp(m_cCurFile,"") == 0 )
        {
            if( m_AppUI.GetToolDialog()->DlgSaveFile(m_hWnd,"粒子文件(*.PAR)\0*.PAR") )
            {
                strcpy( m_cCurFile,m_AppUI.GetToolDialog()->GetFilename() );
                strcat( m_cCurFile,".PAR" );

                pNode->WriteParticleToPAR( m_cCurFile );
                m_bChanged = false;
            }
        }
        else
        {
            pNode->WriteParticleToPAR( m_cCurFile );
            m_bChanged = false;
        }
        break;
    case IDM_FILE_SAVEAS:
        if( m_AppUI.GetToolDialog()->DlgSaveFile(m_hWnd,"粒子文件(*.PAR)\0*.PAR") )
        {
            strcpy( m_cCurFile,m_AppUI.GetToolDialog()->GetFilename() );
            strcat( m_cCurFile,".PAR" );

            pNode->WriteParticleToPAR( m_cCurFile );
            m_bChanged = false;
        }
        break;
    case IDM_FILE_IMPXML:
        break;
    case IDM_FILE_EXPXML:
        if( m_AppUI.GetToolDialog()->DlgSaveFile(m_hWnd,"XML Document(*.xml)\0*.xml") )
        {
            strcpy( cTemp,m_AppUI.GetToolDialog()->GetFilename() );
            strcat( cTemp,".XML" );

            pNode->WriteParticleToXML( cTemp );
        }
        break;
    case IDM_FILE_EXIT:
        OnExit();
        break;
    case IDM_VIEW_PREVIEW:
        m_Renderer.Preview( true );
        break;
    case IDM_VIEW_STOPPREVIEW:
        m_Renderer.Preview( false );
        break;
    case IDM_VIEW_RESETCAM:
        m_Renderer.GetCameraPtr()->Reset();
        break;
    case IDM_HELP_HELP:
        WinExec( "notepad 帮助文件.txt",SW_SHOW );
        break;
    case IDM_HELP_ABOUT:
        m_Renderer.Preview( false );
        DialogBox( m_hInst,(LPCTSTR)IDD_DLG_ABOUT,m_hWnd,Callback_About );
        break;
    }
}


//------------------------------------------------------------------------
// CALLBACK Function of the application.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle of the main window.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    // First handle of CALLBACK Function
    // for application.
    g_pApp->MsgProc( hWnd,uMsg,wParam,lParam );

    switch( uMsg )
    {
    case WM_CLOSE:
        g_pApp->OnExit();
        break;
    default: return DefWindowProc( hWnd,uMsg,wParam,lParam );
    }

    return 0;
}