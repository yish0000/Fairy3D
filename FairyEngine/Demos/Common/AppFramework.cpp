//------------------------------------------------------------------------
// Name: AppFramework.cpp
// Desc: This file is to implement the class AppFramework.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "AppFramework.h"


#define FR_WND_CLASS _T("FR-Engine AppFramework")

AppFramework* g_pApp = NULL;

//------------------------------------------------------------------------
// CALLBACK Function for the application framework.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the window.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    return g_pApp->MsgProc( hWnd,uMsg,wParam,lParam );
}


//------------------------------------------------------------------------
// Callback function called by engine before rendering scene data.
//------------------------------------------------------------------------
void CALLBACK EngineCallbackFunA(void)
{
    g_pApp->OnEngineRunningA();
}


//------------------------------------------------------------------------
// Callback function called by engine after rendering scene data.
//------------------------------------------------------------------------
void CALLBACK EngineCallbackFunB(void)
{
    g_pApp->OnEngineRunningB();
}


//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
AppFramework::AppFramework(void)
{
    m_hInst     = NULL;
    m_hWnd      = NULL;
    m_pDevice   = NULL;
    m_pResMgr   = NULL;
    m_pRenderer = NULL;
    m_pTimer    = NULL;

    m_nWndWidth  = 800;
    m_nWndHeight = 600;
    m_sWndTitle  = "FR-Application";
    m_bActive    = false;

    g_pApp      = this;
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
AppFramework::~AppFramework(void)
{
    FlyKernel::Instance().Shutdown();
}


//------------------------------------------------------------------------
// Create the instance for the application.
// ----------------------------------------------------------------------
// Param -> IN:
//      HINSTANCE:  Handle to the application instance.
//------------------------------------------------------------------------
BOOL AppFramework::Create( HINSTANCE hInst )
{
    RECT rc;
    WNDCLASSEX wnd;

    // Copy the application handle.
    m_hInst = hInst;

    // Write the Window class struct.
    wnd.cbSize          = sizeof(WNDCLASSEX);
    wnd.cbClsExtra      = 0;
    wnd.cbWndExtra      = 0;
    wnd.hbrBackground   = (HBRUSH)GetStockObject( BLACK_BRUSH );
    wnd.hCursor         = LoadCursor( NULL,IDC_ARROW );
    wnd.hIcon           = LoadIcon( NULL,IDI_APPLICATION );
    wnd.hIconSm         = NULL;
    wnd.hInstance       = hInst;
    wnd.lpfnWndProc     = (WNDPROC)WndProc;
    wnd.lpszClassName   = FR_WND_CLASS;
    wnd.lpszMenuName    = NULL;
    wnd.style           = CS_HREDRAW|CS_VREDRAW;

    RegisterClassEx( &wnd );

    // Calculate the window rect.
    SetRect( &rc,0,0,m_nWndWidth,m_nWndHeight );
    AdjustWindowRect( &rc,WS_OVERLAPPEDWINDOW,FALSE );

    // Create the main window.
    m_hWnd = CreateWindow( FR_WND_CLASS,m_sWndTitle.c_str(),WS_OVERLAPPEDWINDOW,
        0,0,rc.right-rc.left,rc.bottom-rc.top,NULL,NULL,hInst,NULL );

    if( !m_hWnd )
    {
        MessageBox( NULL,"Create window failed, Application will exit!","Error",MB_OK|MB_ICONERROR );
        return 0;
    }

    // Initialize the 3D environment.
    if( FAILED(Init3DEnvironment()) ) return FALSE;

    // Initialize the scene content.
    if( FAILED(OneTimeSceneInit()) ) return FALSE;

    // Show the window for application.
    ShowWindow( m_hWnd,SW_SHOW );
    UpdateWindow( m_hWnd );

    m_bActive = true;

    return TRUE;
}


//------------------------------------------------------------------------
// Enter the main loop for the application.
//------------------------------------------------------------------------
INT AppFramework::Run(void)
{
    MSG msg;
    BOOL bGotMsg;
    memset( &msg,0,sizeof(MSG) );

    while( msg.message != WM_QUIT )
    {
        if( m_bActive )
            bGotMsg = ( PeekMessage(&msg,NULL,0,0,PM_REMOVE) != 0 );
        else
            bGotMsg = ( GetMessage(&msg,NULL,0,0) != 0 );

        if( bGotMsg )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            if( m_bActive )
            {
                // Run the engine.
                FlyKernel::Instance().EngineMainLoop();
            }
        }
    }

    UnregisterClass( FR_WND_CLASS,m_hInst );
    return msg.wParam;
}


//------------------------------------------------------------------------
// Quit the application.
//------------------------------------------------------------------------
void AppFramework::Quit(void)
{
    PostQuitMessage( 0 );
}


//------------------------------------------------------------------------
// Pause the application.
//------------------------------------------------------------------------
void AppFramework::Pause( bool bPaused )
{
    m_bActive = !bPaused;
}


//------------------------------------------------------------------------
// Initialize the 3d environment for the application.
//------------------------------------------------------------------------
HRESULT AppFramework::Init3DEnvironment(void)
{
    HRESULT hr;

    FlyKernel& engine = FlyKernel::Instance();

    // Initialize the engine.
    hr = engine.Initialize( "Direct3D",m_hWnd,NULL,0,true );
    if( FAILED(hr) )
    {
        OnErrorExit( "Initialize the engine failed!" );
        return hr;
    }

    m_pDevice = engine.GetRenderDevice();
    m_pResMgr = engine.GetResourceManager();
    m_pRenderer = engine.GetRenderer();

    // Set the default clear color.
    m_pDevice->SetClearColor( 0.4f,0.6f,0.4f );

    // Get the timer of the engine.
    m_pTimer = engine.GetTimer();

    // Set the camera for the engine.
    FlyVector vPos = FlyVector( 0.0f,0.0f,-1.0f );
    FlyVector vDir = FlyVector( 0.0f,0.0f,1.0f );
    FlyVector vRight = FlyVector( 1.0f,0.0f,0.0f );
    FlyVector vUp = FlyVector( 0.0f,1.0f,0.0f );
    m_pDevice->SetView3D( vRight,vUp,vDir,vPos );

    // Set the two callback function.
    engine.SetCallbackFuncA( EngineCallbackFunA );
    engine.SetCallbackFuncB( EngineCallbackFunB );

    return FLY_OK;
}


//------------------------------------------------------------------------
// When there is a fetal error, popup a error dialog then exit the app.
//------------------------------------------------------------------------
void AppFramework::OnErrorExit( const char* cErrorInfo )
{
    MessageBox( NULL,cErrorInfo,"Fetal Error",MB_OK|MB_ICONERROR );

    PostQuitMessage( 0 );
}


//------------------------------------------------------------------------
// Callback function for the main window.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the main window.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
LRESULT AppFramework::MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    switch( uMsg )
    {
    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;
    case WM_KEYDOWN:
        if( wParam == VK_ESCAPE ) Quit();
        break;
    default: return DefWindowProc( hWnd,uMsg,wParam,lParam );
    }

    return 0;
}