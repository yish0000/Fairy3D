//------------------------------------------------------------------------
// Name: uiToolDialog.cpp
// Desc: This file is to implement the class uiToolDialog.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "uiToolDialog.h"


//------------------------------------------------------------------------
// Open a dialog to select a file and return the file path.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle of the main window.
//      char*:      String of the filter.
//------------------------------------------------------------------------
BOOL uiToolDialog::DlgOpenFile( HWND hWnd,char* cFilter )
{
    BOOL bResult;
    char cCurDir[256];

	// Get Current Directory
	GetCurrentDirectory( 256,cCurDir );

	// Clear the strings
	memset( m_cFilename,0,sizeof(char)*256 );

	// Write the OFN struct
    OPENFILENAME ofn;
	memset( &ofn,0,sizeof(OPENFILENAME) );
	ofn.lStructSize         = sizeof(OPENFILENAME);
	ofn.hwndOwner           = hWnd;
	ofn.hInstance           = NULL;
	ofn.lpstrCustomFilter   = NULL;
	ofn.nMaxCustFilter      = 0;
	ofn.nFilterIndex        = 0;
	ofn.lpstrFile           = m_cFilename;
	ofn.nMaxFile            = MAX_PATH;
	ofn.lpstrFileTitle      = NULL;
	ofn.nMaxFileTitle       = MAX_PATH;
	ofn.lpstrInitialDir     = cCurDir;
	ofn.lpstrTitle          = NULL;
	ofn.nFileOffset         = 0;
	ofn.nFileExtension      = 0;
	ofn.lpstrDefExt         = NULL;
	ofn.lCustData           = 0L;
	ofn.lpTemplateName      = NULL;
	ofn.lpfnHook            = NULL;
    ofn.lpstrFilter         = cFilter;
    ofn.Flags               = OFN_HIDEREADONLY|OFN_FILEMUSTEXIST;

    bResult = GetOpenFileName( &ofn );
    SetCurrentDirectory( cCurDir );

    return bResult;
}


//------------------------------------------------------------------------
// Open a dialog to save a file and save the file path.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle of the main window.
//      char*:      String of the filter.
//------------------------------------------------------------------------
BOOL uiToolDialog::DlgSaveFile( HWND hWnd,char* cFilter )
{
    BOOL bResult;
    char cCurDir[256];

	// Get Current Directory
	GetCurrentDirectory( 256,cCurDir );

	// Clear the strings
	memset( m_cFilename,0,sizeof(char)*256 );

	// Write the OFN struct
    OPENFILENAME ofn;
	memset( &ofn,0,sizeof(OPENFILENAME) );
	ofn.lStructSize         = sizeof(OPENFILENAME);
	ofn.hwndOwner           = hWnd;
	ofn.hInstance           = NULL;
	ofn.lpstrCustomFilter   = NULL;
	ofn.nMaxCustFilter      = 0;
	ofn.nFilterIndex        = 0;
	ofn.lpstrFile           = m_cFilename;
	ofn.nMaxFile            = MAX_PATH;
	ofn.lpstrFileTitle      = NULL;
	ofn.nMaxFileTitle       = MAX_PATH;
	ofn.lpstrInitialDir     = cCurDir;
	ofn.lpstrTitle          = NULL;
	ofn.nFileOffset         = 0;
	ofn.nFileExtension      = 0;
	ofn.lpstrDefExt         = NULL;
	ofn.lCustData           = 0L;
	ofn.lpTemplateName      = NULL;
	ofn.lpfnHook            = NULL;
    ofn.lpstrFilter         = cFilter;
    ofn.Flags               = OFN_OVERWRITEPROMPT;

    bResult = GetSaveFileName( &ofn );
    SetCurrentDirectory( cCurDir );

    return bResult;
}


//------------------------------------------------------------------------
// Open a dialog to select a color.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle of the main window.
//      COLORREF*:  To store the color.
//------------------------------------------------------------------------
BOOL uiToolDialog::DlgChooseColor( HWND hWnd,COLORREF* pRefColor )
{
    BOOL bChoose;
    CHOOSECOLOR cclr;
    COLORREF color;

    // Zero the structure.
    memset( &cclr,0,sizeof(CHOOSECOLOR) );
    cclr.lStructSize    = sizeof(CHOOSECOLOR);
    cclr.hwndOwner      = hWnd;
    cclr.hInstance      = NULL;
    cclr.lpCustColors   = &color;
    cclr.Flags          = CC_ANYCOLOR;
    cclr.lCustData      = 0L;
    cclr.lpfnHook       = NULL;
    cclr.lpTemplateName = NULL;

    bChoose = ChooseColor( &cclr );
    memcpy( pRefColor,&cclr.rgbResult,sizeof(COLORREF) );

    return bChoose;
}