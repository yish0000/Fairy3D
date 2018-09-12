//------------------------------------------------------------------------
// Name: etApplication.cpp
// Desc: This file is to implement the class etApplication.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etApplication.h"


const char WND_CLASS_NAME[] = "Terrain Editor";
const char WND_TITLE_NAME[] = "Terrain Editor";

etApplication* g_pApp = NULL;

// Define two callback function.
void CALLBACK OnCoreRunningA(void)
{
    g_pApp->OnEngineRunningA();
}

void CALLBACK OnCoreRunningB(void)
{
    g_pApp->OnEngineRunningB();
}


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
// Constructor of the class.
//------------------------------------------------------------------------
etApplication::etApplication(void)
{
    m_hInst = NULL;
    m_hWnd = NULL;
    m_hRenderWnd = NULL;

    g_pApp = this;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etApplication::~etApplication(void)
{
    // Shutdown the core module.
    etCoreManager::Instance().Shutdown();
}


//------------------------------------------------------------------------
// Create the instance for the application.
// ----------------------------------------------------------------------
// Param -> IN:
//      HINSTANCE:  Handle to the application instance.
//------------------------------------------------------------------------
BOOL etApplication::Create( HINSTANCE hInst )
{
    WNDCLASSEX wnd;

    // Copy the application handle.
    m_hInst = hInst;

    // Write the Window class struct.
    wnd.cbSize          = sizeof(WNDCLASSEX);
    wnd.cbClsExtra      = 0;
    wnd.cbWndExtra      = 0;
    wnd.hbrBackground   = (HBRUSH)COLOR_BTNSHADOW;
    wnd.hCursor         = LoadCursor( NULL,IDC_ARROW );
    wnd.hIcon           = LoadIcon( NULL,(LPCTSTR)IDI_EDITOR );
    wnd.hIconSm         = NULL;
    wnd.hInstance       = hInst;
    wnd.lpfnWndProc     = (WNDPROC)WndProc;
    wnd.lpszClassName   = WND_CLASS_NAME;
    wnd.lpszMenuName    = (LPCTSTR)IDR_MAINWINDOW;
    wnd.style           = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;

    RegisterClassEx( &wnd );

    DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX;

    // Create the main window.
    m_hWnd = CreateWindow( WND_CLASS_NAME,WND_TITLE_NAME,dwStyle,
        0,0,900,600,NULL,NULL,hInst,NULL );
    if( !m_hWnd )
    {
        MessageBox( NULL,"Create window failed, Application will exit!",
            "Error",MB_OK|MB_ICONERROR );
        return 0;
    }

    // Initialize the 3D environment.
    if( FAILED(InitApplication()) ) return FALSE;

    // Show the window for application.
    ShowWindow( m_hWnd,SW_SHOW );
    UpdateWindow( m_hWnd );

    return TRUE;
}


//------------------------------------------------------------------------
// Enter the main loop for the application.
//------------------------------------------------------------------------
INT etApplication::Run(void)
{
    MSG msg;
    memset( &msg,0,sizeof(MSG) );
    BOOL bGotMsg;

    while( msg.message != WM_QUIT )
    {
        bool bActive = etCoreManager::Instance().IsActive();

        // Use PeekMessage() that we can use the idle time
        // to run the engine, and use GetMessage() to avoid
        // eating CPU time.
        if( bActive )
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
            // Run the engine.
            etCoreManager::Instance().EngineMainLoop();
        }
    }

    UnregisterClass( WND_CLASS_NAME,m_hInst );
    return msg.wParam;
}


//------------------------------------------------------------------------
// Quit the application.
//------------------------------------------------------------------------
void etApplication::Quit(void)
{
    PostQuitMessage(0);
}


//------------------------------------------------------------------------
// Initialize the application. (UI, etCore ...)
//------------------------------------------------------------------------
HRESULT etApplication::InitApplication(void)
{
    HRESULT hr;

    // Create the child window for render the scene.
    m_hRenderWnd = CreateWindowEx( WS_EX_CLIENTEDGE,"STATIC","",WS_CHILD|WS_BORDER,
        285,40,600,450,m_hWnd,NULL,m_hInst,NULL );

    // Show the render window.
    ShowWindow( m_hRenderWnd,SW_SHOW );
    UpdateWindow( m_hRenderWnd );

    // Initialize the user interface.
    m_UI.Initialize( m_hInst,m_hWnd,m_hRenderWnd );

    // Initialize the core module.
    etCoreManager& core = etCoreManager::Instance();
    hr = core.Initialize( m_hInst,m_hWnd,m_hRenderWnd );
    if( FAILED(hr) ) return hr;
    core.SetCallbackFuncA( OnCoreRunningA );
    core.SetCallbackFuncB( OnCoreRunningB );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Call this function before the engine rendering.
//------------------------------------------------------------------------
void etApplication::OnEngineRunningA(void)
{
}


//------------------------------------------------------------------------
// Call this function after the engine rendering.
//------------------------------------------------------------------------
void etApplication::OnEngineRunningB(void)
{
    char cTemp[80];
    etCoreManager& core = etCoreManager::Instance();

    // Show the count of triangles.
    sprintf( cTemp,"Triangles: %d",core.GetNumRenderedPrims() );
    m_UI.GetStatusBar()->SetStatusText( 1,cTemp );

    // Show the FPS for the editor.
    float fFPS = core.GetCurrentFPS();
    sprintf( cTemp,"Current FPS: %.3f",fFPS );
    m_UI.GetStatusBar()->SetStatusText( 2,cTemp );
}


//------------------------------------------------------------------------
// When there is a fetal error, popup a error dialog then exit the app.
//------------------------------------------------------------------------
void etApplication::OnErrorExit( const char* cErrorInfo )
{
    MessageBox( m_hWnd,cErrorInfo,"Fetal Error",MB_OK|MB_ICONERROR );
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
LRESULT etApplication::MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    // Handle the window message for coreModule.
    etCoreManager::Instance().MsgProc( hWnd,uMsg,wParam,lParam );

    // Handle the window message for User interface.
    m_UI.MsgProc( hWnd,uMsg,wParam,lParam );

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