//------------------------------------------------------------------------
// Name: AppFramework.h
// Desc: This file define the base class to control the application.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __APP_FRAMEWORK_H__
#define __APP_FRAMEWORK_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyKernel.h"

#include "FlyTimer.h"
//////////////////////////////////////////////////////////////////////////


class AppFramework
{
protected:
    // Member variables for render window.
    HINSTANCE m_hInst;              // Handle to the application.
    HWND m_hWnd;                    // Handle to the main window.
    UINT m_nWndWidth;               // Width of the window.
    UINT m_nWndHeight;              // Height of the window.
    std::string m_sWndTitle;        // Window title for application.
    bool m_bActive;                 // Is the application active ?

    // Member variables for render engine.
    FlyTimer* m_pTimer;             // Pointer to the engine timer.
    LPFLYRENDERDEVICE m_pDevice;    // Pointer to the render device.
    LPFLYRESOURCEMANAGER m_pResMgr; // Pointer to the resource manager.
    LPFLYRENDERER m_pRenderer;      // Pointer to the renderer.

public:
    // Constructor and destructor.
    AppFramework(void);
    virtual ~AppFramework(void);

    BOOL Create( HINSTANCE hInst ); // Create an instance for application.
    INT  Run(void);                 // Enter the main loop for application.
    void Quit(void);                // Quit the application.
    void Pause( bool bPaused );     // Pause or resume the application.

    // Function to handle the window events.
    virtual LRESULT MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

    // Two callback function for engine.
    virtual void OnEngineRunningA(void) { /* Nothing to do! */ };
    virtual void OnEngineRunningB(void) { /* Nothing to do! */ };

protected:
    HRESULT Init3DEnvironment(void);

    // Show the error info.
    void OnErrorExit( const char* cErrorInfo );

    // Create the scene content.
    virtual HRESULT OneTimeSceneInit(void)  { return FLY_OK; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __APP_FRAMEWORK_H__
