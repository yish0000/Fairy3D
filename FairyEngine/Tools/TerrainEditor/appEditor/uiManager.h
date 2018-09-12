//------------------------------------------------------------------------
// Name: uiManager.h
// Desc: This file define a class To manage the GUI elements!
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"
#include <string>
#include <vector>

#pragma warning(disable:4800)
#pragma warning(disable:4251)
//////////////////////////////////////////////////////////////////////////


// GUI elements types
#define GUI_NULL            0x0000
#define	GUI_STATIC          0x0100
#define GUI_LABEL           0x0101
#define GUI_BUTTON		    0x0200
#define GUI_CHECKBOX	    0x0300
#define GUI_RADIOBUTTON	    0x0400
#define GUI_SLIDER		    0x0500
#define GUI_LISTBOX		    0x0600
#define GUI_COMBOBOX	    0x0700
#define GUI_SCROLLBAR	    0x0800
#define GUI_EDITBOX		    0x0900
#define GUI_GROUPBOX	    0x1000
#define GUI_TREEVIEW        0x1100
#define GUI_LISTVIEW        0x1200

//////////////////////////////////////////////////////////////////////////

class uiManager
{
public:
    struct sGUINode
    {
        DWORD   Type;           // Type of the control
        std::string text;       // Text of the control.
        POINT   pos;            // Position of the control
        int     width;          // Width of the control
        int     height;         // Height of the control
        HWND    hWnd;           // Safe handle of the control
    };

    typedef std::vector<sGUINode> GUINodeList;

protected:
    HINSTANCE       m_hInst;        // Handle of the application
    HWND            m_hWnd;         // HWND of the parent window
    HFONT           m_hFont;        // Handle of the font
    GUINodeList     m_GUIList;      // A list of GUI node
    BOOL            m_bTVDragging;

public:
    /* Constructor and destructor */
    uiManager(void);
    ~uiManager(void);

    // Init the GUI Manager
    BOOL InitGUIManager( HINSTANCE hInst,HWND hWnd );
    void Release(void);

    // Member functions...
    HRESULT	AddGUINode( UINT nID,DWORD Type,char* ch,POINT pos,int width,
                        int height,bool bGroup=false );
    HRESULT	CreateGUINode( sGUINode* pNode,bool bGroup );

    void HideAllControls(void);
    void ShowAllControls(void);

    // Functions to control the Node
    void SetText( UINT nID,const char* chText );
    void SetEnabled( UINT nID,BOOL bEnabled );
    void SetPos( UINT nID,POINT pos );
    void SetSize( UINT nID,int width,int height );
    void SetVisible( UINT nID,BOOL bVisible );

    DWORD GetNodeType( UINT nID );
    HWND GetSafeHandle( UINT nID );
    UINT GetIDFromHandle( HWND hWnd );
    const char* GetText( UINT nID );
    BOOL GetEnabled( UINT nID );
    POINT GetPos( UINT nID );
    int	GetNodeCount(void) { return m_GUIList.size(); }
    sGUINode* GetNodePtr( int nIndex ) { return &m_GUIList[nIndex]; }
    HFONT GetFont(void) { return m_hFont; }

    void SetFocus( UINT nID );
    bool ChangeCheckState( UINT nItemID );
    void ChangeCheckState( UINT nItemID,bool bChecked );
    bool GetCheckState( UINT nItemID );

    // Functions for the listBox and comboBox
    int AddItem( UINT nID,const char* chItem,LPVOID pData );
    void DeleteItem( UINT nID,int nIndex );
    void ClearItems( UINT nID );
    int ContainString( UINT nID,const char* chString );
    long GetCurSel( UINT nID );
    void SetCurSel( UINT nID,int nIndex );
    long GetItemCount( UINT nID );
    void GetItemString( UINT nID,int nIndex,char* chItem );
    void GetItemData( UINT nID,int nIndex,LPVOID pData );
    void SetItemString( UINT nID,int nIndex,const char* chItem );

    // Functions for the treeview.
    HTREEITEM AddItemToTree( UINT nID,const char* chItem,int nImage,HTREEITEM hParent,HTREEITEM hAfter );
    void DeleteTreeItem( UINT nID,HTREEITEM hItem );
    HTREEITEM GetRootItem( UINT nID );
    HTREEITEM GetCurSelItem( UINT nID );
    HTREEITEM GetChildItem( UINT nID,HTREEITEM hItem );
    HTREEITEM GetNextSibling( UINT nID,HTREEITEM hItem );
    HTREEITEM GetParentItem( UINT nID,HTREEITEM hItem );
    void GetTreeItemString( UINT nID,HTREEITEM hItem,char* cText,int nLen );
    UINT GetChildIndex( UINT nID,HTREEITEM hItem );

    void OnTVDragBegin( HWND hwndTV,LPNMTREEVIEW lpnmtv );
    void OnTVMouseMove( HWND hwndTV,LONG xCur,LONG yCur );
    void OnTVDragEnd(void);

    // Functions for the checkBox and radioButton
    void SetChecked( UINT nID,BOOL bChecked );
    BOOL GetChecked( UINT nID );

    // Functions for the slider.
    void SetRange( UINT nID,int nMin,int nMax );
    void SetPageSize( UINT nID,int nSize );
    void SetSliderPos( UINT nID,int nPos );

    // Some static functions
    static int TextToInt( HWND hWnd );
    static float TextToFloat( HWND hWnd );
    static void IntToText( HWND hWnd,int nValue );
    static void FloatToText( HWND hWnd,float fValue );
    static void PathToName( char* chPath,char* chName );
    static void PathToDir( char* chPath,char* chDir );
};

//////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __UI_MANAGER_H__