//------------------------------------------------------------------------
// Name: etInterface.h
// Desc: This file define the class to control the UI of the application.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_INTERFACE_H__
#define __ET_INTERFACE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "uiToolBar.h"
#include "uiToolDialog.h"
#include "uiManager.h"
#include "uiStatusBar.h"

//////////////////////////////////////////////////////////////////////////


class etInterface
{
public:
    enum WORKPANEL { WP_TERRAIN,WP_TEXTURE,WP_ENTITY };

    struct sTileInfo
    {
        UINT nTexture;
        float fScale;
    };

    struct sVector { float x,y,z,w; };

protected:
    HINSTANCE m_hInst;          // Handle to the application instance.
    HWND m_hWnd;                // Handle to the main window.
    HWND m_hRenderWnd;          // Handle to the renderWindow.

    uiManager m_uiMgr;          // Manage the controls for application.
    uiToolDialog m_ToolDlg;     // Show the dialogs. (FileOpen, ColorChoose ...)
    uiToolBar m_ToolBar;        // Control the toolBar.
    uiStatusBar m_StatusBar;    // Control the statusBar.

    WORKPANEL m_ActivePanel;    // Current active panel.

    void CreateTerrainPanel(void);
    void CreateTexturePanel(void);
    void CreateEntityPanel(void);

public:
    // Constructor and destructor.
    etInterface(void);
    ~etInterface(void);

    // Initialize the interface of the application.
    HRESULT Initialize( HINSTANCE hInst,HWND hWnd,HWND hRenderWnd );

    uiManager* GetUIManager(void) { return &m_uiMgr; }
    uiToolDialog* GetToolDialog(void) { return &m_ToolDlg; }
    uiToolBar* GetToolBar(void) { return &m_ToolBar; }
    uiStatusBar* GetStatusBar(void) { return &m_StatusBar; }

    // Set the active work panel.
    void SetActiveWorkPanel( WORKPANEL activePanel );

    void MsgProcMenu( WPARAM wParam,LPARAM lParam );
    void MsgProcToolbar( WPARAM wParam,LPARAM lParam );
    void MsgProcGUI( WPARAM wParam,LPARAM lParam );

    void MsgProcTrackBar( WPARAM wParam,LPARAM lParam );
    void MsgProcTreeView( WPARAM wParam,LPARAM lParam );

    // CALLBACK function to handle the window message.
    void MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

    // Show error dialog for user.
    void ShowErrorDialog( HRESULT hrErrorCode );

    // Show the vector setting dialog.
    bool ShowVectorSetting( sVector* pVector );

protected:
    void ShowTileInfo(void);

    // Show the selected entity info.
    void ShowEntityInfo(void);

    // Edit the tile's texture.
    void EditTileTexture( int nLayer );

    // Apply the texture to the whole terrain.
    void ApplyTerrainTexture( UINT nTextureID );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#infdef __ET_INTERFACE_H__