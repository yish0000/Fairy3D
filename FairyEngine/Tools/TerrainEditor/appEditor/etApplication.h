//------------------------------------------------------------------------
// Name: etApplication.h
// Desc:
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_APPLICATION_H__
#define __ET_APPLICATION_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etInterface.h"
#include "etCoreManager.h"

#include "res/resource.h"
//////////////////////////////////////////////////////////////////////////


class etApplication
{
protected:
    HINSTANCE m_hInst;      // HINSTANCE to the application.
    HWND m_hWnd;            // Handle to the window.
    HWND m_hRenderWnd;      // Handle to the render window.

    etInterface m_UI;       // User Interface of the application.

public:
    // Constructor and destructor.
    etApplication(void);
    ~etApplication(void);

    BOOL Create( HINSTANCE hInst ); // Create an instance for application.
    INT  Run(void);                 // Enter the main loop for application.
    void Quit(void);                // Quit the application.

    // Function to handle the window events.
    LRESULT MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

    // Two callback function for engine.
    void OnEngineRunningA(void);
    void OnEngineRunningB(void);

    HINSTANCE GetInstance(void) const { return m_hInst; }
    HWND GetMainWindow(void) const { return m_hWnd; }
    HWND GetRenderWindow(void) const { return m_hRenderWnd; }

    etInterface* GetInterface(void) { return &m_UI; }

protected:
    HRESULT InitApplication(void);

    // Show the error info.
    void OnErrorExit( const char* cErrorInfo );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_APPLICATION_H__