//------------------------------------------------------------------------
// Name: FlyD3DConfigDialog.h
// Desc: This file define a class to select the Direct3D settings.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_D3DCONFIGDIALOG_H__
#define __FLY_D3DCONFIGDIALOG_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"
#include "FlyTypes.h"

#include "FlyD3DEnumerator.h"
//////////////////////////////////////////////////////////////////////////


// Config for the RenderEngine.
typedef struct _DEVICESETTINGS
{
    int nWidth,nHeight;
    bool bWindowed;
    D3DFORMAT backBufferFmt;
    D3DFORMAT depthStencilFmt;
    LONG nPresentInterval;
} DEVICESETTINGS;

//////////////////////////////////////////////////////////////////////////

class FlyD3DConfigDialog
{
protected:
    UINT m_nAdapterID;
    DEVICESETTINGS m_Settings;
    FlyD3DEnumerator m_Enum;

    // Callback function of the config dialog.
    static INT_PTR CALLBACK MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

    // Check whether the combobox contain the item.
    bool ContainItem( HWND hComboBox,const char* cItem );

public:
    // Constructor and destructor.
    FlyD3DConfigDialog(void)  { /* Nothing to do! */ };
    ~FlyD3DConfigDialog(void) { /* Nothing to do! */ };

    HRESULT LoadConfigFile( const char* cFilename );
    HRESULT SaveConfigFile( const char* cFilename );
    bool ShowDialog(void);

    DEVICESETTINGS GetDeviceSettings(void) { return m_Settings; }
    UINT GetAdapterID(void) { return m_nAdapterID; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_D3DCONFIGDIALOG_H__