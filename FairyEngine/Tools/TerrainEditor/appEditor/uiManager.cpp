//------------------------------------------------------------------------
// Name: uiManager.cpp
// Desc: This file is to implementing the class uiManager!
// ----------------------------------------------------------------------
// CopyRight (C) FlySoft Corp. All right Observed.
//------------------------------------------------------------------------


#include "uiManager.h"
#include "etApplication.h"

#include <commctrl.h>
#include "Res/resource.h"

#include "uiMacros.h"


/*************************************************************************
*                                                                        *
*    IMPLEMENTING THE CLASS GUIMANAGER                                   *
*                                                                        *
*************************************************************************/


extern etApplication* g_pApp;

//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
uiManager::uiManager(void)
{
	m_hInst = NULL;
	m_hWnd = NULL;
	m_hFont = NULL;
    m_bTVDragging = FALSE;
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
uiManager::~uiManager(void)
{
	Release();
}


//------------------------------------------------------------------------
// Initialize the GUI manager
// ----------------------------------------------------------------------
// Param -> In:
//		HINSTANCE:	Handle of the application
//		HWND:		Handle of the parent window
//------------------------------------------------------------------------
BOOL uiManager::InitGUIManager( HINSTANCE hInst,HWND hWnd )
{
	m_hInst	= hInst;
	m_hWnd	= hWnd;

	// Create the font for controls
	LOGFONT	lf;
	memset( &lf,0,sizeof(LOGFONT) );
	lf.lfHeight			= 12;
	lf.lfWidth			= 6;
	lf.lfEscapement		= 0;
	lf.lfOrientation	= 0;
	lf.lfWeight			= FW_NORMAL;
	lf.lfItalic			= FALSE;
	lf.lfUnderline		= FALSE;
	lf.lfStrikeOut		= FALSE;
	lf.lfCharSet		= DEFAULT_CHARSET;
	lf.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	lf.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	lf.lfQuality		= DEFAULT_QUALITY;
	lf.lfPitchAndFamily	= DEFAULT_PITCH;
	strcpy( lf.lfFaceName,"ËÎÌו" );

	m_hFont	= CreateFontIndirect( &lf );

	if( !m_hFont ) return FALSE;

	return TRUE;
}


//------------------------------------------------------------------------
// Add a GUI node to the GUI manager!
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the GUI controls
//		DWORD:		Type of the controls
//		char*:		Text on the control
//		POINT:		Position of the control
//		int:		Width of the control
//		int:		Height of the control
//      bool:       Does is the first of group button
//------------------------------------------------------------------------
HRESULT uiManager::AddGUINode( UINT nID,DWORD Type,char* ch,POINT pos,
								 int width,int height,bool bGroup )
{
	// Create a new node
	sGUINode newUI;
	newUI.Type = Type;
	newUI.text = ch;
	newUI.pos = pos;
	newUI.width = width;
	newUI.height = height;

	if( FAILED(CreateGUINode(&newUI,bGroup)) )
	{
		char err[40];
		sprintf( err,"Create the GUI node failed! ID: %d",nID );
		MessageBox( NULL,err,"Error",MB_OK|MB_ICONERROR );
		return FLY_FAIL;
	}

	// Add the new node to list
	m_GUIList.push_back( newUI );

	return FLY_OK;
}


//------------------------------------------------------------------------
// Create a real control on the parent window for this GUI node!
// ----------------------------------------------------------------------
// Param -> In:
//		sGUINode*:		Pointer of the node
//      bool:           Does is the group button (For checkBox...)
//------------------------------------------------------------------------
HRESULT uiManager::CreateGUINode( sGUINode* pNode,bool bGroup )
{
    HIMAGELIST imgList;
    HBITMAP hBmp;

	// Check valid ?
	if( !pNode ) return FLY_FAIL;

	int	x = pNode->pos.x;
	int	y = pNode->pos.y;
	int	width = pNode->width;
	int height = pNode->height;

    HWND hWnd;           // Temp handle
    DWORD dwStyle = 0;   // Style of the control

	// Switch the type of the control
	switch( pNode->Type )
	{
	case GUI_STATIC:
		dwStyle = WS_CHILD|WS_VISIBLE|SS_LEFT;
		hWnd = CreateWindow( "STATIC",pNode->text.c_str(),dwStyle,x,y,width,height,
            m_hWnd,NULL,m_hInst,NULL );
		if( hWnd ) pNode->hWnd = hWnd;
		break;
    case GUI_LABEL:
        dwStyle = WS_CHILD|WS_VISIBLE|SS_LEFT;
        hWnd = CreateWindowEx( WS_EX_STATICEDGE,"STATIC",pNode->text.c_str(),dwStyle,x,y,
            width,height,m_hWnd,NULL,m_hInst,NULL );
        if( hWnd ) pNode->hWnd = hWnd;
        break;
	case GUI_BUTTON:
		dwStyle = WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON;
		hWnd = CreateWindow( "BUTTON",pNode->text.c_str(),dwStyle,x,y,width,height,
            m_hWnd,NULL,m_hInst,NULL );
		if( hWnd ) pNode->hWnd = hWnd;
		break;
	case GUI_CHECKBOX:
		dwStyle = WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX;
        if( bGroup ) dwStyle |= WS_GROUP;
		hWnd = CreateWindow( "BUTTON",pNode->text.c_str(),dwStyle,x,y,width,height,
            m_hWnd,NULL,m_hInst,NULL );
		if( hWnd ) pNode->hWnd = hWnd;
		break;
	case GUI_RADIOBUTTON:
		dwStyle = WS_CHILD|WS_VISIBLE|BS_PUSHLIKE|BS_AUTORADIOBUTTON;
        if( bGroup ) dwStyle |= WS_GROUP;
		hWnd = CreateWindowEx( 0,"BUTTON",pNode->text.c_str(),dwStyle,x,y,width,height,
            m_hWnd,NULL,m_hInst,NULL );
		if( hWnd ) pNode->hWnd = hWnd;
		break;
	case GUI_SLIDER:
        dwStyle = WS_CHILD|WS_VISIBLE|TBS_ENABLESELRANGE|TBS_NOTICKS;
        hWnd = CreateWindowEx( 0,TRACKBAR_CLASS,TEXT(""),dwStyle,x,y,width,height,
            m_hWnd,NULL,m_hInst,NULL );
        if( hWnd ) pNode->hWnd = hWnd;
        SendMessage( hWnd,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,10) );
        SendMessage( hWnd,TBM_SETPAGESIZE,0,(LPARAM)1 );
        SendMessage( hWnd,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)0 );
		break;
	case GUI_SCROLLBAR:
		dwStyle = WS_CHILD|WS_VISIBLE;
		hWnd = CreateWindow( "SCROLLBAR",TEXT(""),dwStyle,x,y,width,height,
            m_hWnd,NULL,m_hInst,NULL );
		if( hWnd ) pNode->hWnd = hWnd;
		break;
	case GUI_LISTBOX:
		dwStyle = WS_CHILD|WS_VISIBLE|LBS_NOINTEGRALHEIGHT|WS_VSCROLL;
		hWnd = CreateWindowEx( WS_EX_CLIENTEDGE,"LISTBOX",pNode->text.c_str(),dwStyle,x,y,width,height,
            m_hWnd,NULL,m_hInst,NULL );
		if( hWnd ) pNode->hWnd = hWnd;
		break;
	case GUI_COMBOBOX:
        dwStyle = WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL|CBS_HASSTRINGS;
		hWnd = CreateWindow( "COMBOBOX",pNode->text.c_str(),dwStyle,x,y,width,height,
            m_hWnd,NULL,m_hInst,NULL );
		if( hWnd ) pNode->hWnd = hWnd;
		break;
    case GUI_EDITBOX:
        dwStyle = WS_CHILD|WS_VISIBLE|ES_LEFT;
        hWnd = CreateWindowEx( WS_EX_CLIENTEDGE,"EDIT",pNode->text.c_str(),dwStyle,x,y,width,height,
            m_hWnd,NULL,m_hInst,NULL );
        if( hWnd ) pNode->hWnd = hWnd;
        break;
	case GUI_GROUPBOX:
		dwStyle = WS_CHILD|WS_VISIBLE|BS_GROUPBOX|BS_TEXT;
		hWnd = CreateWindow( "BUTTON",pNode->text.c_str(),dwStyle,x,y,width,height,
            m_hWnd,NULL,m_hInst,NULL );
		if( hWnd ) pNode->hWnd = hWnd;
		break;
    case GUI_TREEVIEW:
        // Create the image list.
        imgList = ImageList_Create( 16,16,FALSE,3,0 );
        hBmp = LoadBitmap( g_pApp->GetInstance(),MAKEINTRESOURCE(IDB_ACTOR_TYPE) );
        ImageList_Add( imgList,hBmp,NULL );
        DeleteObject( hBmp );
        hBmp = LoadBitmap( g_pApp->GetInstance(),MAKEINTRESOURCE(IDB_ACTOR_ELEMENT) );
        ImageList_Add( imgList,hBmp,NULL );
        DeleteObject( hBmp );
        hBmp = LoadBitmap( g_pApp->GetInstance(),MAKEINTRESOURCE(IDB_ACTOR_ADDTYPE) );
        ImageList_Add( imgList,hBmp,NULL );
        DeleteObject( hBmp );

        dwStyle = WS_CHILD|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT/*|TVS_TRACKSELECT*/;
        hWnd = CreateWindowEx( WS_EX_CLIENTEDGE,"SysTreeView32",TEXT(""),dwStyle,x,y,width,height,
            m_hWnd,NULL,m_hInst,NULL );
        if( hWnd )
        {
            pNode->hWnd = hWnd;
            SendMessage( hWnd,TVM_SETIMAGELIST,TVSIL_NORMAL,(LPARAM)imgList );
        }
        break;
    case GUI_LISTVIEW:
        dwStyle = WS_CHILD|LVS_REPORT|LVS_EDITLABELS;
        hWnd = CreateWindowEx( WS_EX_CLIENTEDGE,"SysListView32",TEXT(""),dwStyle,x,y,
            width,height,m_hWnd,NULL,m_hInst,NULL );
        if( hWnd ) pNode->hWnd = hWnd;
        SendMessage( hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );
        break;
	default: return FLY_FAIL;
	}

	// Set the font for control
	SendMessage( hWnd,WM_SETFONT,(WPARAM)m_hFont,(LPARAM)TRUE );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Add an item for the treeview control.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the treeview control.
//      const char*: Text of the item.
//      int:        Image id of the item.
//      HTREEITEM:  Handle to the parent node.
//      HTREEITEM:  Handle to the after node.
//------------------------------------------------------------------------
HTREEITEM uiManager::AddItemToTree( UINT nID,const char* chItem,int nImage,HTREEITEM hParent,
                                    HTREEITEM hAfter )
{
    TVITEM tvi;
    TVINSERTSTRUCT tvins;

    tvi.mask = TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE;

    // Set the text of the item.
    tvi.pszText = (char*)chItem;
    tvi.cchTextMax = strlen(chItem);
    tvi.iImage = nImage;
    tvi.iSelectedImage = nImage;

    tvins.item = tvi;
    tvins.hInsertAfter = hAfter;
    tvins.hParent = hParent;

    // Add the item to the treeview.
    HWND hTreeView = m_GUIList[nID].hWnd;
    return (HTREEITEM)SendMessage( hTreeView,TVM_INSERTITEM,0,(LPARAM)&tvins );
}


//------------------------------------------------------------------------
// Get the root node for the treeview control.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the treeview control.
//------------------------------------------------------------------------
HTREEITEM uiManager::GetRootItem( UINT nID )
{
    HWND hTreeView = m_GUIList[nID].hWnd;

    return (HTREEITEM)SendMessage( hTreeView,TVM_GETNEXTITEM,(WPARAM)TVGN_ROOT,NULL );
}


//------------------------------------------------------------------------
// Get the current selected item for the treeview control.
//------------------------------------------------------------------------
HTREEITEM uiManager::GetCurSelItem( UINT nID )
{
    HWND hTreeView = m_GUIList[nID].hWnd;

    return (HTREEITEM)SendMessage( hTreeView,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,NULL );
}


//------------------------------------------------------------------------
// Get the first child of the item.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the treeview control.
//      HTREEITEM:  Handle to the item.
//------------------------------------------------------------------------
HTREEITEM uiManager::GetChildItem( UINT nID,HTREEITEM hItem )
{
    HWND hTreeView = m_GUIList[nID].hWnd;

    return (HTREEITEM)SendMessage( hTreeView,TVM_GETNEXTITEM,(WPARAM)TVGN_CHILD,(LPARAM)hItem );
}


//------------------------------------------------------------------------
// Get the next sibling item for the item.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the treeView control.
//      HTREEITEM:  Handle to the item.
//------------------------------------------------------------------------
HTREEITEM uiManager::GetNextSibling( UINT nID,HTREEITEM hItem )
{
    HWND hTreeView = m_GUIList[nID].hWnd;

    return (HTREEITEM)SendMessage( hTreeView,TVM_GETNEXTITEM,(WPARAM)TVGN_NEXT,(LPARAM)hItem );
}


//------------------------------------------------------------------------
// Get the parent item for the item.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the treeView control.
//      HTREEITEM:  Handle to the item.
//------------------------------------------------------------------------
HTREEITEM uiManager::GetParentItem( UINT nID,HTREEITEM hItem )
{
    HWND hTreeView = m_GUIList[nID].hWnd;

    return (HTREEITEM)SendMessage( hTreeView,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)hItem );
}


//------------------------------------------------------------------------
// Delete an item from the treeview control.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the treeview control.
//      HTREEITEM:  Handle to the item.
//------------------------------------------------------------------------
void uiManager::DeleteTreeItem( UINT nID,HTREEITEM hItem )
{
    HWND hTreeView = m_GUIList[nID].hWnd;

    SendMessage( hTreeView,TVM_DELETEITEM,0,(LPARAM)hItem );
}


//------------------------------------------------------------------------
// Get the string of the selected item.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the treeview control.
//      HTREEITEM:  Handle to the item.
//      char*:      To store the string.
//      int:        Length of the buffer.
//------------------------------------------------------------------------
void uiManager::GetTreeItemString( UINT nID,HTREEITEM hItem,char* cText,int nLen )
{
    TVITEM item;
    HWND hTreeView = m_GUIList[nID].hWnd;

    item.hItem = hItem;
    item.mask = TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
    item.pszText = cText;
    item.cchTextMax = nLen;
    SendMessage( hTreeView,TVM_GETITEM,0,(LPARAM)&item );
}


//------------------------------------------------------------------------
// Get the index of current selected item.
//------------------------------------------------------------------------
UINT uiManager::GetChildIndex( UINT nID,HTREEITEM hItem )
{
    UINT nIndex = 0;
    HWND hTreeView = m_GUIList[nID].hWnd;

    HTREEITEM hParent = (HTREEITEM)SendMessage( hTreeView,TVM_GETNEXTITEM,TVGN_PARENT,(LPARAM)hItem );
    if( !hParent ) return -1;
    HTREEITEM hNext = (HTREEITEM)SendMessage( hTreeView,TVM_GETNEXTITEM,TVGN_CHILD,(LPARAM)hParent );
    if( hNext == hItem )
        return nIndex;
    else
        nIndex++;

    while( hNext )
    {
        hNext = (HTREEITEM)SendMessage( hTreeView,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)hNext );

        if( hNext == hItem )
            return nIndex;
        else
            nIndex++;
    }

    return -1;
}


//------------------------------------------------------------------------
// Hide all the controls for the surface.
//------------------------------------------------------------------------
void uiManager::HideAllControls(void)
{
    HWND hWnd;

    for( int i=0;i<m_GUIList.size();i++ )
    {
        hWnd = m_GUIList[i].hWnd;
        ShowWindow( hWnd,SW_HIDE );
    }
}


//------------------------------------------------------------------------
// Show all the controls for the surface.
//------------------------------------------------------------------------
void uiManager::ShowAllControls(void)
{
    HWND hWnd;

    for( int i=0;i<m_GUIList.size();i++ )
    {
        hWnd = m_GUIList[i].hWnd;
        ShowWindow( hWnd,SW_SHOW );
    }
}


//------------------------------------------------------------------------
// Release the GUI Manager
//------------------------------------------------------------------------
void uiManager::Release(void)
{
	// Release the font
	if( m_hFont )
	{
		DeleteObject( m_hFont );
		m_hFont = NULL;
	}
}


/*************************************************************************
*                                                                        *
*    MISC FUNCTIONS FOR THIS CLASS                                       *
*                                                                        *
*************************************************************************/


//------------------------------------------------------------------------
// Get the safe handle for the control (GUI node)
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the GUI node
//------------------------------------------------------------------------
HWND uiManager::GetSafeHandle( UINT nID )
{
    if( nID < 0 || nID >= m_GUIList.size() )
        return NULL;
    else
        return m_GUIList[nID].hWnd;
}


//------------------------------------------------------------------------
// Get the GUI node (child control) ID from its handle
// ----------------------------------------------------------------------
// Param -> In:
//		HWND:		Handle of the child control
//------------------------------------------------------------------------
UINT uiManager::GetIDFromHandle( HWND hWnd )
{
	for( int i=0;i<m_GUIList.size();i++ )
	{
		sGUINode* pNode = &m_GUIList[i];

		if( pNode->hWnd == hWnd )
			return i;
	}

	return -1;
}


//------------------------------------------------------------------------
// Get the GUI type for the control (GUI node)
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the GUI node
//------------------------------------------------------------------------
DWORD uiManager::GetNodeType( UINT nID )
{
    if( nID < 0 || nID >= m_GUIList.size() )
	    return GUI_NULL;
    else
        return m_GUIList[nID].Type;
}


//------------------------------------------------------------------------
// Get the window text for the control (GUI node)
// ----------------------------------------------------------------------
// Param -> In:
//      UINT:       ID of the GUI node.
//------------------------------------------------------------------------
const char* uiManager::GetText( UINT nID )
{
    if( nID < 0 || nID >= m_GUIList.size() )
        return NULL;
    else
        return m_GUIList[nID].text.c_str();
}


//------------------------------------------------------------------------
// Get the control's enable state 
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the GUI node
//------------------------------------------------------------------------
BOOL uiManager::GetEnabled( UINT nID )
{
    if( nID < 0 || nID >= m_GUIList.size() )
        return FALSE;

	return IsWindowEnabled( m_GUIList[nID].hWnd );
}


//------------------------------------------------------------------------
// Set the enabled state for the control
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the GUI node
//		BOOL:		Enabled state (True or False)
//------------------------------------------------------------------------
void uiManager::SetEnabled( UINT nID,BOOL bEnabled )
{
    if( nID < 0 || nID >= m_GUIList.size() )
        return;

	EnableWindow( m_GUIList[nID].hWnd,bEnabled );
}


//------------------------------------------------------------------------
// Set the window text for the control
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the GUI node
//		char*:		Text for the node
//------------------------------------------------------------------------
void uiManager::SetText( UINT nID,const char* chText )
{
    if( nID < 0 || nID >= m_GUIList.size() )
        return;

    m_GUIList[nID].text = chText;
	SetWindowText( m_GUIList[nID].hWnd,chText );
}


//------------------------------------------------------------------------
// Set the size for the control (GUI node)
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the GUI node
//		int:		Width of the control
//		int:		Height of the control
//------------------------------------------------------------------------
void uiManager::SetSize( UINT nID,int width,int height )
{
    if( nID < 0 || nID >= m_GUIList.size() )
        return;

    sGUINode* pNode = &m_GUIList[nID];
	pNode->width = width;
	pNode->height = height;
	MoveWindow( pNode->hWnd,pNode->pos.x,pNode->pos.y,width,height,TRUE );
}


//------------------------------------------------------------------------
// Set the size for the control (GUI node)
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the GUI node
//		POINT:		Position of the control
//------------------------------------------------------------------------
void uiManager::SetPos( UINT nID,POINT pos )
{
    if( nID < 0 || nID >= m_GUIList.size() )
        return;

    sGUINode* pNode = &m_GUIList[nID];
	pNode->pos = pos;
	MoveWindow( pNode->hWnd,pos.x,pos.y,pNode->width,pNode->height,TRUE );
}


//------------------------------------------------------------------------
// Get the pos for the control (GUI node)
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the GUI node
// Return -> :
//		POINT:		Position of the GUI node
//------------------------------------------------------------------------
POINT uiManager::GetPos( UINT nID )
{
	POINT pt = { 0,0 };

    if( nID < 0 || nID >= m_GUIList.size() )
        return pt;

	return m_GUIList[nID].pos;
}


//------------------------------------------------------------------------
// Set the control a focus or lost focus states
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the GUI node
//------------------------------------------------------------------------
void uiManager::SetFocus( UINT nID )
{
    // Get the handle
    HWND hWnd = GetSafeHandle( nID );

    SendMessage( hWnd,WM_SETFOCUS,(WPARAM)hWnd,0 );
}


//------------------------------------------------------------------------
// Set the visible for the control.
// ----------------------------------------------------------------------
// Param -> IN:
//      BOOL:       Is the control visible ?
//------------------------------------------------------------------------
void uiManager::SetVisible( UINT nID,BOOL bVisible )
{
    // Get the handle
    HWND hWnd = GetSafeHandle( nID );
    if( !hWnd ) return;

    if( bVisible ) ShowWindow( hWnd,SW_SHOW );
    else ShowWindow( hWnd,SW_HIDE );
}


/*************************************************************************
*                                                                        *
*    FUNCTIONS TO MANAGE THE LISTBOX AND COMBOBOX                        *
*                                                                        *
*************************************************************************/


//------------------------------------------------------------------------
// Add a item for a comboBox or a listBox
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the control
//		char*:		String of the item to add
//		LPVOID:		Pointer for data to add ( if ComboBox,data must be
//					DWORD! )
// Return -> Out:
//      int:        Index of the new item
//------------------------------------------------------------------------
int uiManager::AddItem( UINT nID,const char* chItem,LPVOID pData )
{
	// Get GUI's handle
	HWND hWnd = GetSafeHandle( nID );

	// Nothing found!
	if( !hWnd ) return -1;

	// Get the type
	DWORD dwType = GetNodeType( nID );
    WPARAM nI;

	if( dwType == GUI_LISTBOX )
	{
		nI = (WPARAM)SendMessage( hWnd,LB_ADDSTRING,(WPARAM)0,(LPARAM)chItem );
		SendMessage( hWnd,LB_SETITEMDATA,nI,(LPARAM)pData );
	}
	else if( dwType == GUI_COMBOBOX )
	{
        nI = (WPARAM)SendMessage( hWnd,CB_ADDSTRING,(WPARAM)0,(LPARAM)chItem );
        SendMessage( hWnd,CB_SETITEMDATA,nI,(LPARAM)pData );
        if( SendMessage(hWnd,CB_GETCOUNT,(WPARAM)0,(LPARAM)0) == 1 )
            SendMessage( hWnd,CB_SETCURSEL,(WPARAM)0,(LPARAM)0 );
    }

    return nI;
}


//------------------------------------------------------------------------
// Set the current selected item for listBox or comboBox
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the control
//		int:		Index of the item
//------------------------------------------------------------------------
void uiManager::SetCurSel( UINT nID,int nIndex )
{
	// Get the node's handle
	HWND hWnd = GetSafeHandle( nID );

	// Nothing found!
	if( !hWnd ) return;

	// Get the type of the gui
	DWORD dwType = GetNodeType( nID );

	if( dwType == GUI_LISTBOX )
		SendMessage( hWnd,LB_SETCURSEL,(WPARAM)nIndex,(LPARAM)0 );
	else if( dwType == GUI_COMBOBOX )
		SendMessage( hWnd,CB_SETCURSEL,(WPARAM)nIndex,(LPARAM)0 );
}


//------------------------------------------------------------------------
// Get the current selected item's index for listBox or comboBox
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the control
//------------------------------------------------------------------------
long uiManager::GetCurSel( UINT nID )
{
	// Get the node's handle
	HWND hWnd = GetSafeHandle( nID );
	if( !hWnd ) return 0;

	// Get the type of the node
	DWORD dwType = GetNodeType( nID );

	if( dwType == GUI_LISTBOX )
		return SendMessage( hWnd,LB_GETCURSEL,(WPARAM)0,(LPARAM)0 );
	else
		return SendMessage( hWnd,CB_GETCURSEL,(WPARAM)0,(LPARAM)0 );

	return -1;
}


//------------------------------------------------------------------------
// Remove the item by its index from a listBox or comboBox
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the control
//		int:		Index of the item
//------------------------------------------------------------------------
void uiManager::DeleteItem( UINT nID,int nIndex )
{
	// Get the node's handle
	HWND hWnd = GetSafeHandle( nID );

	// Nothing found!
	if( !hWnd ) return;

	// Get the type of the gui
	DWORD dwType = GetNodeType( nID );

	if( dwType == GUI_LISTBOX )
		SendMessage( hWnd,LB_DELETESTRING,(WPARAM)nIndex,(LPARAM)0 );
	else if( dwType == GUI_COMBOBOX )
    {
		SendMessage( hWnd,CB_DELETESTRING,(WPARAM)nIndex,(LPARAM)0 );
		if( nIndex == 0 ) SendMessage( hWnd,CB_SETCURSEL,0,0 );
	}
}


//------------------------------------------------------------------------
// Remove all the items for the listBox or comboBox
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the control
//------------------------------------------------------------------------
void uiManager::ClearItems( UINT nID )
{
	// Get the node's handle
	HWND hWnd = GetSafeHandle( nID );

	// Nothing found!
	if( !hWnd ) return;

	// Get the type of the GUI
	DWORD dwType = GetNodeType( nID );

	if( dwType == GUI_LISTBOX )
		SendMessage( hWnd,LB_RESETCONTENT,(WPARAM)0,(LPARAM)0 );
	else
		SendMessage( hWnd,CB_RESETCONTENT,(WPARAM)0,(LPARAM)0 );
}


//------------------------------------------------------------------------
// Get the item's string from a listBox or comboBox
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the control
//		int:		Index of the item
//		char*:		To store the item string
//------------------------------------------------------------------------
void uiManager::GetItemString( UINT nID,int nIndex,char* chItem )
{
	// Get the node's handle
	HWND hWnd = GetSafeHandle( nID );
	if( !hWnd ) return;

	// Get the type of the GUI
	DWORD dwType = GetNodeType( nID );

	if( dwType == GUI_LISTBOX )
		SendMessage( hWnd,LB_GETTEXT,(WPARAM)nIndex,(LPARAM)chItem );
	else if( dwType == GUI_COMBOBOX )
		SendMessage( hWnd,CB_GETLBTEXT,(WPARAM)nIndex,(LPARAM)chItem );
}


//------------------------------------------------------------------------
// Get the item's data from a listBox or comboBox
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the control
//		int:		Index of the item
//		LPVOID:		Pointer to store the data
//------------------------------------------------------------------------
void uiManager::GetItemData( UINT nID,int nIndex,LPVOID pData )
{
	// Get the node's handle
	HWND hWnd = GetSafeHandle( nID );
	if( !hWnd ) return;

	// Get the type of the gui
	DWORD dwType = GetNodeType( nID );

	if( dwType == GUI_LISTBOX )
		pData = (VOID*)SendMessage( hWnd,LB_GETITEMDATA,(WPARAM)nIndex,0 );
	else if( dwType == GUI_COMBOBOX )
		pData = (VOID*)SendMessage( hWnd,CB_GETITEMDATA,(WPARAM)nIndex,0 );
}


//------------------------------------------------------------------------
// Check whether there is a string in the listBox or comboBox
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the control
//		char*:		String to check
//------------------------------------------------------------------------
int uiManager::ContainString( UINT nID,const char* chString )
{
	// Get the handle of control
	HWND hWnd = GetSafeHandle( nID );
	if( !hWnd ) return FALSE;

	// Get the type of the control
	DWORD dwType = GetNodeType( nID );
	if( dwType != GUI_LISTBOX && dwType != GUI_COMBOBOX )
		return FALSE;

	char chBuffer[80];

	if( dwType == GUI_LISTBOX )
	{
		long nNum = SendMessage( hWnd,LB_GETCOUNT,0,0 );
		for( long i=0;i<nNum;i++ )
		{
			SendMessage( hWnd,LB_GETTEXT,(WPARAM)i,(LPARAM)chBuffer );
			if( strcmp(chBuffer,chString) == 0 ) return i;
		}
	}
	else if( dwType == GUI_COMBOBOX )
	{
		long nNum = SendMessage( hWnd,CB_GETCOUNT,0,0 );
		for( long i=0;i<nNum;i++ )
		{
			SendMessage( hWnd,CB_GETLBTEXT,(WPARAM)i,(LPARAM)chBuffer );
			if( strcmp(chBuffer,chString) == 0 ) return i;
		}
	}

	return -1;
}


//------------------------------------------------------------------------
// Set the item's string for the listBox or a comboBox.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the control
//      int:        Index of the item.
//      char*:      String of the item.
//------------------------------------------------------------------------
void uiManager::SetItemString( UINT nID,int nIndex,const char* chItem )
{
    // Get the node's handle
    HWND hWnd = GetSafeHandle( nID );
    if( !hWnd ) return;

    // Get the type of the GUI
    DWORD dwType = GetNodeType( nID );

    if( dwType == GUI_LISTBOX )
    {
        SendMessage( hWnd,LB_INSERTSTRING,(WPARAM)nIndex,(LPARAM)chItem );
        SendMessage( hWnd,LB_DELETESTRING,(WPARAM)(nIndex+1),(LPARAM)0 );
        SendMessage( hWnd,LB_SETCURSEL,(WPARAM)nIndex,(LPARAM)0 );
    }
    else if( dwType == GUI_COMBOBOX )
    {
        SendMessage( hWnd,CB_INSERTSTRING,(WPARAM)nIndex,(LPARAM)chItem );
        SendMessage( hWnd,CB_DELETESTRING,(WPARAM)(nIndex+1),(LPARAM)0 );
        SendMessage( hWnd,CB_SETCURSEL,(WPARAM)nIndex,(LPARAM)0 );
    }
}


//------------------------------------------------------------------------
// Get the item count for the listBox or comboBox
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the control
//------------------------------------------------------------------------
long uiManager::GetItemCount( UINT nID )
{
	// Get the handle of control
	HWND hWnd = GetSafeHandle( nID );
	if( !hWnd ) return 0;

	// Get the type of control
	DWORD dwType = GetNodeType( nID );

	if( dwType == GUI_LISTBOX )
		return (long)SendMessage( hWnd,LB_GETCOUNT,(WPARAM)0,(LPARAM)0 );
	else if( dwType == GUI_COMBOBOX )
		return (long)SendMessage( hWnd,CB_GETCOUNT,(WPARAM)0,(LPARAM)0 );

	return 0;
}


//------------------------------------------------------------------------
// Begins dragging an item in a tree view control.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:           Handle to the treeview. 
//      LPNMTREEVIEW:   Address of information about the item being dragged.
//------------------------------------------------------------------------
void uiManager::OnTVDragBegin( HWND hwndTV,LPNMTREEVIEW lpnmtv )
{
    HIMAGELIST himl;

    // Create an image to use for dragging.
    himl = TreeView_CreateDragImage( hwndTV,lpnmtv->itemNew.hItem );

    // Start the drag operation.
    ImageList_BeginDrag( himl,1,1,1 );

    ShowCursor( FALSE );
    SetCapture( GetParent(hwndTV) );
    m_bTVDragging = TRUE;
}


//------------------------------------------------------------------------
// Drags an item in a tree view control.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the tree view control.
//      LONG,LONG:  X- and y-coordinates of the mouse pointer.
//------------------------------------------------------------------------
void uiManager::OnTVMouseMove( HWND hwndTV,LONG xCur,LONG yCur )
{
    HTREEITEM htiTarget;
    TVHITTESTINFO tvht;

    if( m_bTVDragging )
    {
        // Drag the item to the current position of the mouse pointer.
        ImageList_DragMove( xCur,yCur );

        // Find out if the pointer is on the item. If it is, 
        // highlight the item as a drop target.
        tvht.pt.x = xCur;
        tvht.pt.y = yCur;

        if( (htiTarget=TreeView_HitTest(hwndTV,&tvht)) != NULL )
            TreeView_SelectDropTarget( hwndTV,htiTarget );
    }
}


//------------------------------------------------------------------------
// Stops dragging a tree view item, releases the mouse capture.
//------------------------------------------------------------------------
void uiManager::OnTVDragEnd(void)
{
    if( m_bTVDragging )
    {
        ImageList_EndDrag();
        ReleaseCapture();
        ShowCursor( TRUE );
        m_bTVDragging = FALSE;
    }
}


/*************************************************************************
*                                                                        *
*    IMPLEMENTING THE FUNCTIONS FOR CHECKBOX AND RADIOBUTTON             *
*                                                                        *
*************************************************************************/


//------------------------------------------------------------------------
// Get the checked state for the checkBox or radioButton
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the control
//------------------------------------------------------------------------
BOOL uiManager::GetChecked( UINT nID )
{
	// Get the handle of control
	HWND hWnd = GetSafeHandle( nID );
	if( !hWnd ) return FALSE;

	// Get the type of control
	DWORD dwType = GetNodeType( nID );

	if( dwType == GUI_CHECKBOX || dwType == GUI_RADIOBUTTON )
		return (BOOL)SendMessage( hWnd,BM_GETCHECK,(WPARAM)0,(LPARAM)0 );

	return FALSE;
}


//------------------------------------------------------------------------
// Set the checked state for the checkBox or radioButton
// ----------------------------------------------------------------------
// Param -> In:
//		UINT:		ID of the control
//		BOOL:		Enable or disable ?
//------------------------------------------------------------------------
void uiManager::SetChecked( UINT nID,BOOL bChecked )
{
    // Get the handle of control
    HWND hWnd = GetSafeHandle( nID );
    if( !hWnd ) return;

    DWORD dwType = GetNodeType( nID );

    if( dwType == GUI_CHECKBOX || dwType == GUI_RADIOBUTTON )
        SendMessage( hWnd,BM_SETCHECK,(WPARAM)bChecked,(LPARAM)0 );
}


//------------------------------------------------------------------------
// Set the range for the slider control.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the control.
//      int:        Min value of the range.
//      int:        Max value of the range.
//------------------------------------------------------------------------
void uiManager::SetRange( UINT nID,int nMin,int nMax )
{
    HWND hWnd = GetSafeHandle( nID );
    if( !hWnd ) return;

    SendMessage( hWnd,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(nMin,nMax) );
}


//------------------------------------------------------------------------
// Set the page size for the slider control.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the control.
//      int:        Page size of the slider control.
//------------------------------------------------------------------------
void uiManager::SetPageSize( UINT nID,int nSize )
{
    HWND hWnd = GetSafeHandle( nID );
    if( !hWnd ) return;

    SendMessage( hWnd,TBM_SETPAGESIZE,0,(LPARAM)nSize );
}


//------------------------------------------------------------------------
// Set the position of the slider control.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the control.
//      int:        Position of the track on the slider.
//------------------------------------------------------------------------
void uiManager::SetSliderPos( UINT nID,int nPos )
{
    HWND hWnd = GetSafeHandle( nID );
    if( !hWnd ) return;

    SendMessage( hWnd,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)nPos );
}


/*************************************************************************
*                                                                        *
*     SOME STATIC FUNCTIONS TO SET AND GET THE CONTROL'S TEXT            *
*                                                                        *
*************************************************************************/


//------------------------------------------------------------------------
// Get the control's text and convert it to integer
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle of the control
//------------------------------------------------------------------------
int uiManager::TextToInt( HWND hWnd )
{
    int nValue;
    char str[40];

    GetWindowText( hWnd,str,40 );
    sscanf( str,"%d",&nValue );

    return nValue;
}


//------------------------------------------------------------------------
// Get the controls's text and convert it to float
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:        Handle of the control
//------------------------------------------------------------------------
float uiManager::TextToFloat( HWND hWnd )
{
    float fValue;
    char str[40];

    GetWindowText( hWnd,str,40 );
    sscanf( str,"%f",&fValue );

    return fValue;
}


//------------------------------------------------------------------------
// Set a integer as the text of the control
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle of the control
//      int:        The value to set
//------------------------------------------------------------------------
void uiManager::IntToText( HWND hWnd,int nValue )
{
    char    str[40];

    sprintf( str,"%d",nValue );
    SetWindowText( hWnd,str );
}


//------------------------------------------------------------------------
// Set a float as the text of the control
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle of the control
//      float:      The value to set
//------------------------------------------------------------------------
void uiManager::FloatToText( HWND hWnd,float fValue )
{
    char str[40];

    sprintf( str,"%f",fValue );
    SetWindowText( hWnd,str );
}


//------------------------------------------------------------------------
// Get a file name from the file path.
// ----------------------------------------------------------------------
// Param -> IN:
//      char*:      String of the file path
//      char*:      To store the file name
//------------------------------------------------------------------------
void uiManager::PathToName( char* chPath,char* chName )
{
    char* pSeperator = NULL;
    char chTmp[256];

    strcpy( chTmp,strrev(strdup(chPath)) );
    pSeperator = strchr( chTmp,'/' );
    if( !pSeperator )
        pSeperator = strchr( chTmp,'\\' );
    *pSeperator = '\0';

    strcpy( chName,chTmp );
    strrev( chName );
}


//------------------------------------------------------------------------
// Get a file's directory from the whole file path
// ----------------------------------------------------------------------
// Param -> IN:
//      char*:      String of the file path
//      char*:      To store the file's directory
//------------------------------------------------------------------------
void uiManager::PathToDir( char* chPath,char* chDir )
{
    char* pSeperator = NULL;
    char chTmp[256];

    strcpy( chTmp,strrev(strdup(chPath)) );
    pSeperator = strchr( chTmp,'/' );
    if( !pSeperator ) pSeperator = strchr( chTmp,'\\' );

    int n = strlen(chTmp) - (pSeperator-chTmp);
    strrev( chTmp );
    chTmp[n] = '\0';

    strcpy( chDir,chTmp );
}


//------------------------------------------------------------------------
// Change the menu item's check state.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the menu item.
//------------------------------------------------------------------------
bool uiManager::ChangeCheckState( UINT nItemID )
{
    HMENU hMenu = GetMenu( m_hWnd );
    DWORD dwItem = GetMenuState( hMenu,nItemID,MF_BYCOMMAND );

    if( dwItem & MF_CHECKED )
    {
        CheckMenuItem( hMenu,nItemID,MF_BYCOMMAND|MF_UNCHECKED );
        return false;
    }
    else
    {
        CheckMenuItem( hMenu,nItemID,MF_BYCOMMAND|MF_CHECKED );
        return true;
    }
}


//------------------------------------------------------------------------
// Change the menu item's check state.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the menu item.
//      bool:       Check or unchecked ?
//------------------------------------------------------------------------
void uiManager::ChangeCheckState( UINT nItemID,bool bChecked )
{
    HMENU hMenu = GetMenu( m_hWnd );

    if( bChecked )
        CheckMenuItem( hMenu,nItemID,MF_BYCOMMAND|MF_CHECKED );
    else
        CheckMenuItem( hMenu,nItemID,MF_BYCOMMAND|MF_UNCHECKED );
}


//------------------------------------------------------------------------
// Get the check state of the menu item.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the menu item.
//------------------------------------------------------------------------
bool uiManager::GetCheckState( UINT nItemID )
{
    UINT ret;

    HMENU hMenu = GetMenu( m_hWnd );
    ret = GetMenuState( hMenu,nItemID,MF_BYCOMMAND );

    if ( ret & MF_CHECKED ) return true;

    return false;
}