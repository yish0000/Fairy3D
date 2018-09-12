//------------------------------------------------------------------------
// Name: uiToolBar.cpp
// Desc: This file is to implement the class uiToolBar.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "uiMacros.h"
#include "uiToolBar.h"


const int NUM_TOOLBAR_BUTTONS = 14;
const int NUM_TOOLBAR_SEPS = 4;

//------------------------------------------------------------------------
// Create the ToolBar for the parent window
// ----------------------------------------------------------------------
// Param -> In:
//		HINSTANCE:	Handle of the application
//		HWND:		Handle of the parent window
//------------------------------------------------------------------------
BOOL uiToolBar::CreateToolBar( HINSTANCE hInst,HWND hWnd )
{
    TBADDBITMAP	tb_bmp;

    m_hInst	= hInst;
    m_hWnd	= hWnd;

    // Create the bar
    m_hToolBar = CreateWindowEx( 0,TOOLBARCLASSNAME,NULL,WS_CHILD|WS_BORDER|CCS_ADJUSTABLE|
        TBSTYLE_FLAT|TBSTYLE_TOOLTIPS,0,0,32,32,m_hWnd,0,m_hInst,NULL );

    if( !m_hToolBar ) return FALSE;

    SendMessage( m_hToolBar,TB_BUTTONSTRUCTSIZE,(WPARAM)sizeof(TBBUTTON),0 );

    // Add the Bitmap to the bar
    tb_bmp.hInst = m_hInst;
    tb_bmp.nID = IDR_TOOLBAR;

    if( SendMessage(m_hToolBar,TB_ADDBITMAP,(WPARAM)(NUM_TOOLBAR_BUTTONS+NUM_TOOLBAR_SEPS),
        (WPARAM)&tb_bmp ) == -1 )
        return FALSE;

    // Add the buttons
    if( !CreateButtons() ) return FALSE;

    // Show the ToolBar
    ShowWindow( m_hToolBar,SW_SHOW );
    UpdateWindow( m_hToolBar );

    return TRUE;
}


//------------------------------------------------------------------------
// Create buttons for the ToolBar
//------------------------------------------------------------------------
BOOL uiToolBar::CreateButtons(void)
{
    TBBUTTON tbb[NUM_TOOLBAR_BUTTONS+NUM_TOOLBAR_SEPS];
    memset( &tbb,0,sizeof(tbb) );

    for( int i=0,s=0;i<NUM_TOOLBAR_BUTTONS+NUM_TOOLBAR_SEPS;i++ )
    {
        if( i==0 || i==4 || i==8 || i==16 )
        {
            tbb[i].fsStyle = TBSTYLE_SEP;
            s++;
        }
        else
        {
            tbb[i].iBitmap = i - s;
            tbb[i].fsStyle = TBSTYLE_BUTTON;
            tbb[i].fsState = TBSTATE_ENABLED;
        }
    }

    // Set the message ID for buttons
    tbb[1].idCommand = IDT_NEWSCENE;
    tbb[2].idCommand = IDT_OPENSCENE;
    tbb[3].idCommand = IDT_SAVESCENE;

    tbb[5].idCommand = IDT_SELOBJECT;
    tbb[6].idCommand = IDT_MAPINFO;
    tbb[7].idCommand = IDT_LIGHTMODE;

    tbb[9].idCommand = IDT_SELECT;
    tbb[9].fsStyle = TBSTYLE_CHECKGROUP;
    tbb[9].fsState = TBSTATE_CHECKED|TBSTATE_ENABLED;
    tbb[10].idCommand = IDT_MOVEXZ;
    tbb[10].fsStyle = TBSTYLE_CHECKGROUP;
    tbb[11].idCommand = IDT_MOVEY;
    tbb[11].fsStyle = TBSTYLE_CHECKGROUP;
    tbb[12].idCommand = IDT_ROTATEX;
    tbb[12].fsStyle = TBSTYLE_CHECKGROUP;
    tbb[13].idCommand = IDT_ROTATEY;
    tbb[13].fsStyle = TBSTYLE_CHECKGROUP;
    tbb[14].idCommand = IDT_ROTATEZ;
    tbb[14].fsStyle = TBSTYLE_CHECKGROUP;
    tbb[15].idCommand = IDT_SCALE;
    tbb[15].fsStyle = TBSTYLE_CHECKGROUP;

    tbb[17].idCommand = IDT_ABOUT;

    // Add the button to the ToolBar
    if( !SendMessage(m_hToolBar,TB_ADDBUTTONS,
        (WPARAM)(NUM_TOOLBAR_BUTTONS+NUM_TOOLBAR_SEPS),
        (LPARAM)&tbb) )
    {
        return FALSE;
    }

    return TRUE;
}


//------------------------------------------------------------------------
// Enable a button of the toolbar.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the button.
//      bool:       Enable or disable.
//------------------------------------------------------------------------
void uiToolBar::EnableButton( UINT nID,bool bEnable )
{
    SendMessage( m_hToolBar,TB_ENABLEBUTTON,(WPARAM)nID,(LPARAM)MAKELONG(bEnable,0) );
}


//------------------------------------------------------------------------
// Set the check state of the button.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the button.
//------------------------------------------------------------------------
void uiToolBar::CheckButton( UINT nID,bool bChecked )
{
    SendMessage( m_hToolBar,TB_CHECKBUTTON,(WPARAM)nID,MAKELONG(bChecked,0) );
}