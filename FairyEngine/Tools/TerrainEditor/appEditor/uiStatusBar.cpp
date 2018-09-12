//------------------------------------------------------------------------
// Name: uiStatusBar.cpp
// Desc: This file is to implementing the class CStatusBar
// ----------------------------------------------------------------------
// CopyRight (C) FlySoft Corp. All right Observed.
//------------------------------------------------------------------------


#include "uiStatusBar.h"


/*************************************************************************
*                                                                        *
*    IMPLEMENTING THE CLASS CSTATUSBAR                                   *
*                                                                        *
*************************************************************************/


//------------------------------------------------------------------------
// Create the status bar
// ----------------------------------------------------------------------
// Param -> In:
//		HWND:		Handle of the parent window
//		int:		Number of the bars
//------------------------------------------------------------------------
BOOL uiStatusBar::CreateStatusBar( HWND hWnd,int nNumBars )
{
	m_hWnd	= hWnd;

	// Create the bar
	m_hBar = CreateWindow( STATUSCLASSNAME,NULL,WS_CHILD|WS_VISIBLE,0,0,0,0,
						   m_hWnd,NULL,NULL,NULL );

	// Split the status bar
	m_nNumBars = nNumBars;
	int*	Rightend = new int[3];
	RECT	WinRect;

	GetClientRect( m_hWnd,&WinRect );
    Rightend[0] = WinRect.right / 3;
    Rightend[1] = WinRect.right / 3 * 2;
	Rightend[2] = WinRect.right;

	SendMessage( m_hBar,SB_SETPARTS,(WPARAM)3,(LPARAM)Rightend );

	delete[] Rightend;
	return TRUE;
}


//------------------------------------------------------------------------
// Set the status bar's text
// ----------------------------------------------------------------------
// Param -> In:
//      int:            ID of the status bar
//      const char*:    Context of the text
//------------------------------------------------------------------------
void uiStatusBar::SetStatusText( int nID,const char* chText )
{
    SendMessage( m_hBar,SB_SETTEXT,(WPARAM)nID,(LPARAM)chText );
}


//------------------------------------------------------------------------
// Change the statusBar's position and size
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		Window message to get
//		WPARAM:		Parameter of the message
//		LPARAM:		Parameter of the message
//------------------------------------------------------------------------
void uiStatusBar::OnSize( UINT uMsg,WPARAM wParam,LPARAM lParam )
{
	// Reset the size
	SendMessage( m_hBar,uMsg,wParam,lParam );

	// Split the status bar
	int*	Rightend = new int[m_nNumBars];
	RECT	WinRect;

	GetClientRect( m_hWnd,&WinRect );
	for( int i=0;i<m_nNumBars;i++ )
	{
		Rightend[i] = (WinRect.right / m_nNumBars)*(i+1);
	}

	SendMessage( m_hBar,SB_SETPARTS,(WPARAM)m_nNumBars,(LPARAM)Rightend );
	delete[] Rightend;
}