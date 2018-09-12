//------------------------------------------------------------------------
// Name: ToolDialog.h
// Desc: This file define a class to manage the tool dialogs.
//       Such as: ColorChooser, FileOpenDialog, FileSaveDialog. etc.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __UI_TOOL_DIALOG_H__
#define __UI_TOOL_DIALOG_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include <windows.h>

//////////////////////////////////////////////////////////////////////////

class uiToolDialog
{
protected:
    char m_cFilename[MAX_PATH]; // To store the file path.

public:
    // Constructor and destructor.
    uiToolDialog(void)  { /* Nothing to do! */ };
    ~uiToolDialog(void) { /* Nothing to do! */ };

    BOOL DlgOpenFile( HWND hWnd,char* cFilter );
    BOOL DlgSaveFile( HWND hWnd,char* cFilter );
    BOOL DlgChooseColor( HWND hWnd,COLORREF* pRefColor );

    const char* GetFilename(void) { return m_cFilename; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __UI_TOOL_DIALOG_H__