/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRenderWindow.h
 *  Desc:   This file define a render window for engine.
 *  Author: Yish
 *  Date:   2010/12/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Studio. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_RENDERWINDOW_H__
#define __F3D_RENDERWINDOW_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DWindow.h"
#include "F3DRenderTarget.h"

///////////////////////////////////////////////////////////////////////////

/** Render Window.
Render the scenes to a specified window.
Please implement me on different platforms.
*/
class FAIRY_API F3DRenderWindow : public F3DRenderTarget
{
protected:
    bool m_bMainWnd;            // Is me a main window ?
    bool m_bFullScreen;         // Is fullscreen mode ?

public:
    F3DRenderWindow( const AString& name, bool bMainWnd );
    virtual ~F3DRenderWindow(void);

	// Create the render window.
    virtual void Create( const F3DWindow* wndInfo ) = 0;

    // Resize the render window.
    virtual void ResizeWindow(uint32 width, uint32 height) = 0;
    // Change the fullscreen mode.
    virtual void SetFullScreen(bool bFullScreen) = 0;

	// Get the pick ray by a specified window point.
    virtual void GetPickRay( const F2DPOINT& cursorPt, F3DRay& pickRay, int vpIdx = 0 ) const;
	// Transform the point in world space to screen.
    virtual F2DPOINT WorldToScreen( const F3DVector3& vPos3D, int vpIdx = 0 ) const;

	// Print the current screen to file.
	// @param bFront, PrintScreen with the front buffer(slow), 
	// backbuffer(fast, but must called me after render all the scenes and before swapBuffers)
	virtual void PrintScreenToFile( const char* filename, bool bFront = true ) = 0;

    // Is fullscreen mode
    bool IsFullScreen(void) const { return m_bFullScreen; }
    // Is main window ?
    bool IsMainWindow(void) const { return m_bMainWnd; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_RENDERWINDOW_H__