//------------------------------------------------------------------------
// Name: etInterface.cpp
// Desc: This file is to implement the class etInterface.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "uiMacros.h"
#include "etInterface.h"
#include "etCallbackFunc.h"

#include "etCoreManager.h"
#include "etSceneManager.h"
#include "etBrush.h"
#include "etEntityManager.h"
#include "etSelectionBuffer.h"
#include "etLiquidObject.h"

#include "etApplication.h"
#include "etStringUtil.h"


extern etApplication* g_pApp;

//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etInterface::etInterface(void)
{
    m_hInst = NULL;
    m_hWnd = NULL;
    m_hRenderWnd = NULL;

    m_ActivePanel = (WORKPANEL)-1;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etInterface::~etInterface(void)
{
}


//------------------------------------------------------------------------
// Initialize the interface of the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      HINSTANCE:  Handle to the instance of the application.
//      HWND:       Handle to the main window.
//      HWND:       Handle to the render window.
//------------------------------------------------------------------------
HRESULT etInterface::Initialize( HINSTANCE hInst,HWND hWnd,HWND hRenderWnd )
{
    POINT pt;
    DWORD dwStyle;

    m_hInst = hInst;
    m_hWnd = hWnd;
    m_hRenderWnd = hRenderWnd;

    m_StatusBar.CreateStatusBar( hWnd,3 );
    m_StatusBar.SetStatusText( 0,"就绪" );
    m_ToolBar.CreateToolBar( m_hInst,hWnd );

    // Create the controls for the application.
    m_uiMgr.InitGUIManager( hInst,hWnd );
    pt.x = 10; pt.y = 35;
    m_uiMgr.AddGUINode( IDC_WORKSPACE,GUI_GROUPBOX,"",pt,265,440,true );
    dwStyle = WS_CHILD|WS_VISIBLE|BS_PUSHLIKE|BS_AUTORADIOBUTTON;
    pt.x = 22; pt.y = 460;
    m_uiMgr.AddGUINode( IDC_SWITCH_TERRAIN,GUI_RADIOBUTTON,"地形编辑",pt,80,30,true );
    pt.x = 102; pt.y = 460;
    m_uiMgr.AddGUINode( IDC_SWITCH_TEXTURE,GUI_RADIOBUTTON,"纹理编辑",pt,80,30 );
    pt.x = 182; pt.y = 460;
    m_uiMgr.AddGUINode( IDC_SWITCH_ENTITY,GUI_RADIOBUTTON,"实体编辑",pt,80,30 );

    // Create the terrain panel.
    CreateTerrainPanel();

    // Create the texture panel.
    CreateTexturePanel();

    // Create the entity panel.
    CreateEntityPanel();

    m_uiMgr.SetChecked( IDC_SWITCH_TERRAIN,TRUE );
    SetActiveWorkPanel( WP_TERRAIN );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Create terrain panel for the editor.
//------------------------------------------------------------------------
void etInterface::CreateTerrainPanel(void)
{
    POINT pt;
    pt.x = 22; pt.y = 55;
    m_uiMgr.AddGUINode( IDC_HP_TOOL,GUI_GROUPBOX,"工具",pt,240,220,false );
    pt.x = 48; pt.y = 80;
    m_uiMgr.AddGUINode( IDC_HP_TOOLTYPE,GUI_COMBOBOX,TEXT(""),pt,180,150,false );
    m_uiMgr.AddItem( IDC_HP_TOOLTYPE,"抬高",NULL );
    m_uiMgr.AddItem( IDC_HP_TOOLTYPE,"降低",NULL );
    m_uiMgr.AddItem( IDC_HP_TOOLTYPE,"铺平",NULL );
    m_uiMgr.AddItem( IDC_HP_TOOLTYPE,"光滑",NULL );
    m_uiMgr.AddItem( IDC_HP_TOOLTYPE,"挖掘",NULL );
    m_uiMgr.AddItem( IDC_HP_TOOLTYPE,"绘制",NULL );
    m_uiMgr.AddItem( IDC_HP_TOOLTYPE,"擦除",NULL );
    m_uiMgr.AddItem( IDC_HP_TOOLTYPE,"混合",NULL );

    pt.x = 38; pt.y = 130;
    m_uiMgr.AddGUINode( IDC_HP_OUTRADIUS,GUI_SLIDER,TEXT(""),pt,200,22,false );
    m_uiMgr.SetRange( IDC_HP_OUTRADIUS,1,255 );
    m_uiMgr.SetPageSize( IDC_HP_OUTRADIUS,20 );
    pt.x = 38; pt.y = 170;
    m_uiMgr.AddGUINode( IDC_HP_INNERRADIUS,GUI_SLIDER,TEXT(""),pt,200,22,false );
    m_uiMgr.SetRange( IDC_HP_INNERRADIUS,1,255 );
    m_uiMgr.SetPageSize( IDC_HP_INNERRADIUS,20 );
    pt.x = 38; pt.y = 210;
    m_uiMgr.AddGUINode( IDC_HP_STRENGTH,GUI_SLIDER,TEXT(""),pt,200,22,false );
    m_uiMgr.SetRange( IDC_HP_STRENGTH,1,80 );
    m_uiMgr.SetPageSize( IDC_HP_STRENGTH,5 );
    pt.x = 45; pt.y = 115;
    m_uiMgr.AddGUINode( IDC_HP_LBLORADIUS,GUI_STATIC,"刷子外径: 1",pt,100,15,false );
    pt.x = 45; pt.y = 155;
    m_uiMgr.AddGUINode( IDC_HP_LBLIRADIUS,GUI_STATIC,"刷子内径: 1",pt,100,15,false );
    pt.x = 45; pt.y = 195;
    m_uiMgr.AddGUINode( IDC_HP_LBLSTRENGTH,GUI_STATIC,"刷子强度: 1",pt,100,15,false );

    pt.x = 45; pt.y = 240;
    m_uiMgr.AddGUINode( IDC_HP_LBLSHAPE,GUI_STATIC,"刷子形状: ",pt,100,15,false );
    pt.x = 105; pt.y = 238;
    m_uiMgr.AddGUINode( IDC_HP_CIRCLE,GUI_RADIOBUTTON,"圆形",pt,60,20,true );
    pt.x = 165; pt.y = 238;
    m_uiMgr.AddGUINode( IDC_HP_RECTANGLE,GUI_RADIOBUTTON,"方形",pt,60,20,false );

    pt.x = 22; pt.y = 290;
    m_uiMgr.AddGUINode( IDC_HP_DETAILLAYER,GUI_GROUPBOX,"细节层编辑",pt,240,55 );
    pt.x = 40; pt.y = 310;
    m_uiMgr.AddGUINode( IDC_HP_LAYER1,GUI_RADIOBUTTON,"一",pt,50,20,true );
    pt.x = 90; pt.y = 310;
    m_uiMgr.AddGUINode( IDC_HP_LAYER2,GUI_RADIOBUTTON,"二",pt,50,20 );
    pt.x = 140; pt.y = 310;
    m_uiMgr.AddGUINode( IDC_HP_LAYER3,GUI_RADIOBUTTON,"三",pt,50,20 );
    pt.x = 190; pt.y = 310;
    m_uiMgr.AddGUINode( IDC_HP_LAYER4,GUI_RADIOBUTTON,"四",pt,50,20 );

    m_uiMgr.SetEnabled( IDC_HP_LAYER1,FALSE );
    m_uiMgr.SetEnabled( IDC_HP_LAYER2,FALSE );
    m_uiMgr.SetEnabled( IDC_HP_LAYER3,FALSE );
    m_uiMgr.SetEnabled( IDC_HP_LAYER4,FALSE );

    m_uiMgr.SetChecked( IDC_HP_CIRCLE,TRUE );
    m_uiMgr.SetChecked( IDC_HP_LAYER1,TRUE );
}


//------------------------------------------------------------------------
// Create the texture panel for the editor.
//------------------------------------------------------------------------
void etInterface::CreateTexturePanel(void)
{
    POINT pt;
    pt.x = 22; pt.y = 55;
    m_uiMgr.AddGUINode( IDC_TP_TEXGROUP,GUI_GROUPBOX,"纹理列表",pt,240,200,false );
    pt.x = 32; pt.y = 75;
    m_uiMgr.AddGUINode( IDC_TP_TEXTURELIST,GUI_LISTBOX,"",pt,220,120,false );
    pt.x = 37; pt.y = 203;
    m_uiMgr.AddGUINode( IDC_TP_ADDTEX,GUI_BUTTON,"添加",pt,70,20,false );
    pt.x = 107; pt.y = 203;
    m_uiMgr.AddGUINode( IDC_TP_DELTEX,GUI_BUTTON,"删除",pt,70,20,false );
    pt.x = 177; pt.y = 203;
    m_uiMgr.AddGUINode( IDC_TP_PREVIEW,GUI_BUTTON,"预览",pt,70,20,false );
    pt.x = 37; pt.y = 223;
    m_uiMgr.AddGUINode( IDC_TP_APPLYALL,GUI_BUTTON,"应用到整个地形",pt,210,20,false );

    pt.x = 22; pt.y = 265;
    m_uiMgr.AddGUINode( IDC_TP_TILEGROUP,GUI_GROUPBOX,"Tile信息",pt,240,180,false );
    pt.x = 32; pt.y = 290;
    m_uiMgr.AddGUINode( IDC_TP_TXTMAINTEX,GUI_STATIC,"主纹理: ",pt,50,15 );
    pt.x = 89; pt.y = 286;
    m_uiMgr.AddGUINode( IDC_TP_BTNMAINTEX,GUI_BUTTON,"空",pt,160,20 );
    pt.x = 32; pt.y = 320;
    m_uiMgr.AddGUINode( IDC_TP_TXTDETAIL1,GUI_STATIC,"细节一: ",pt,50,15 );
    pt.x = 89; pt.y = 316;
    m_uiMgr.AddGUINode( IDC_TP_BTNDETAIL1,GUI_BUTTON,"空",pt,160,20 );
    pt.x = 32; pt.y = 350;
    m_uiMgr.AddGUINode( IDC_TP_TXTDETAIL2,GUI_STATIC,"细节二: ",pt,50,15 );
    pt.x = 89; pt.y = 346;
    m_uiMgr.AddGUINode( IDC_TP_BTNDETAIL2,GUI_BUTTON,"空",pt,160,20 );
    pt.x = 32; pt.y = 380;
    m_uiMgr.AddGUINode( IDC_TP_TXTDETAIL3,GUI_STATIC,"细节三: ",pt,50,15 );
    pt.x = 89; pt.y = 376;
    m_uiMgr.AddGUINode( IDC_TP_BTNDETAIL3,GUI_BUTTON,"空",pt,160,20 );
    pt.x = 32; pt.y = 410;
    m_uiMgr.AddGUINode( IDC_TP_TXTDETAIL4,GUI_STATIC,"细节四: ",pt,50,15 );
    pt.x = 89; pt.y = 406;
    m_uiMgr.AddGUINode( IDC_TP_BTNDETAIL4,GUI_BUTTON,"空",pt,160,20 );

    m_uiMgr.SetEnabled( IDC_TP_BTNMAINTEX,FALSE );
    m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL1,FALSE );
    m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL2,FALSE );
    m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL3,FALSE );
    m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL4,FALSE );
}


//------------------------------------------------------------------------
// Create the entity panel for the editor.
//------------------------------------------------------------------------
void etInterface::CreateEntityPanel(void)
{
    POINT pt;
    pt.x = 22; pt.y = 55;
    m_uiMgr.AddGUINode( IDC_AP_ENTITYGROUP,GUI_GROUPBOX,"可用物体列表",pt,240,185 );
    pt.x = 32; pt.y = 75;
    m_uiMgr.AddGUINode( IDC_AP_ENTITYLIST,GUI_TREEVIEW,"",pt,220,120 );
    pt.x = 32; pt.y = 200;
    m_uiMgr.AddGUINode( IDC_AP_CURRENTTYPE,GUI_STATIC,"当前类型: 空",pt,220,15 );
    pt.x = 32; pt.y = 220;
    m_uiMgr.AddGUINode( IDC_AP_CURRENTENTITY,GUI_STATIC,"当前实体: 空",pt,220,15 );
    pt.x = 22; pt.y = 252;
    m_uiMgr.AddGUINode( IDC_AP_ENTITYATTRIB,GUI_GROUPBOX,"实体属性",pt,240,200 );
    pt.x = 32; pt.y = 272;
    m_uiMgr.AddGUINode( IDC_AP_ENTITYNAME,GUI_STATIC,"实体名字:",pt,80,20 );
    pt.x = 90; pt.y = 269;
    m_uiMgr.AddGUINode( IDC_AP_ENTITYNAMEBTN,GUI_BUTTON,"空",pt,160,20 );
    pt.x = 32; pt.y = 302;
    m_uiMgr.AddGUINode( IDC_AP_ENTITYTYPE,GUI_STATIC,"实体类型:",pt,80,20 );
    pt.x = 90; pt.y = 299;
    m_uiMgr.AddGUINode( IDC_AP_ENTITYTYPELBL,GUI_LABEL,"空",pt,160,20 );
    pt.x = 32; pt.y = 332;
    m_uiMgr.AddGUINode( IDC_AP_ENTITYTEM,GUI_STATIC,"实体模板:",pt,80,20 );
    pt.x = 90; pt.y = 329;
    m_uiMgr.AddGUINode( IDC_AP_ENTITYTEMLBL,GUI_LABEL,"空",pt,160,20 );
    pt.x = 32; pt.y = 362;
    m_uiMgr.AddGUINode( IDC_AP_ENTITYPOS,GUI_STATIC,"实体位置:",pt,80,20 );
    pt.x = 90; pt.y = 359;
    m_uiMgr.AddGUINode( IDC_AP_ENTITYPOSBTN,GUI_BUTTON,"(0.000,0.000,0.000)",pt,160,20 );
    pt.x = 32; pt.y = 392;
    m_uiMgr.AddGUINode( IDC_AP_ENTITYROT,GUI_STATIC,"实体旋转:",pt,80,20 );
    pt.x = 90; pt.y = 389;
    m_uiMgr.AddGUINode( IDC_AP_ENTITYROTBTN,GUI_BUTTON,"(0.000,0.000,0.000)",pt,160,20 );
    pt.x = 32; pt.y = 422;
    m_uiMgr.AddGUINode( IDC_AP_ENTITYSCALE,GUI_STATIC,"实体缩放:",pt,80,20 );
    pt.x = 90; pt.y = 419;
    m_uiMgr.AddGUINode( IDC_AP_ENTITYSCALEBTN,GUI_BUTTON,"(0.000,0.000,0.000)",pt,160,20 );

    HTREEITEM hItem;
    hItem = m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,"模型",0,TVI_ROOT,TVI_FIRST );
    hItem = m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,"动画",0,TVI_ROOT,hItem );
    hItem = m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,"粒子",0,TVI_ROOT,hItem );
    hItem = m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,"植被",0,TVI_ROOT,hItem );
    hItem = m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,"精灵",0,TVI_ROOT,hItem );
    hItem = m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,"液体",0,TVI_ROOT,hItem );

    hItem = m_uiMgr.GetRootItem( IDC_AP_ENTITYLIST );
    m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,"Add Type",2,hItem,NULL );
    hItem = m_uiMgr.GetNextSibling( IDC_AP_ENTITYLIST,hItem );
    m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,"Add Type",2,hItem,NULL );
    hItem = m_uiMgr.GetNextSibling( IDC_AP_ENTITYLIST,hItem );
    m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,"Add Type",2,hItem,NULL );
    hItem = m_uiMgr.GetNextSibling( IDC_AP_ENTITYLIST,hItem );
    m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,"Add Type",2,hItem,NULL );
    hItem = m_uiMgr.GetNextSibling( IDC_AP_ENTITYLIST,hItem );
    m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,"Add Type",2,hItem,NULL );
    hItem = m_uiMgr.GetNextSibling( IDC_AP_ENTITYLIST,hItem );
    m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,"Add Water",2,hItem,NULL );

    m_uiMgr.SetEnabled( IDC_AP_ENTITYNAMEBTN,FALSE );
    m_uiMgr.SetEnabled( IDC_AP_ENTITYPOSBTN,FALSE );
    m_uiMgr.SetEnabled( IDC_AP_ENTITYROTBTN,FALSE );
    m_uiMgr.SetEnabled( IDC_AP_ENTITYSCALEBTN,FALSE );
}


//------------------------------------------------------------------------
// Set the active work panel.
//------------------------------------------------------------------------
void etInterface::SetActiveWorkPanel( etInterface::WORKPANEL activePanel )
{
    if( activePanel == m_ActivePanel )
        return;

    // Hide all the controls.
    m_uiMgr.HideAllControls();

    switch( activePanel )
    {
    case WP_TERRAIN:
        m_uiMgr.SetVisible( IDC_HP_TOOL,TRUE );
        m_uiMgr.SetVisible( IDC_HP_TOOLTYPE,TRUE );
        m_uiMgr.SetVisible( IDC_HP_OUTRADIUS,TRUE );
        m_uiMgr.SetVisible( IDC_HP_INNERRADIUS,TRUE );
        m_uiMgr.SetVisible( IDC_HP_STRENGTH,TRUE );
        m_uiMgr.SetVisible( IDC_HP_LBLORADIUS,TRUE );
        m_uiMgr.SetVisible( IDC_HP_LBLIRADIUS,TRUE );
        m_uiMgr.SetVisible( IDC_HP_LBLSTRENGTH,TRUE );
        m_uiMgr.SetVisible( IDC_HP_LBLSHAPE,TRUE );
        m_uiMgr.SetVisible( IDC_HP_CIRCLE,TRUE );
        m_uiMgr.SetVisible( IDC_HP_RECTANGLE,TRUE );
        m_uiMgr.SetVisible( IDC_HP_DETAILLAYER,TRUE );
        m_uiMgr.SetVisible( IDC_HP_LAYER1,TRUE );
        m_uiMgr.SetVisible( IDC_HP_LAYER2,TRUE );
        m_uiMgr.SetVisible( IDC_HP_LAYER3,TRUE );
        m_uiMgr.SetVisible( IDC_HP_LAYER4,TRUE );
        etCoreManager::Instance().SetWorkMode( WKM_TERRAIN );
        break;
    case WP_TEXTURE:
        m_uiMgr.SetVisible( IDC_TP_TEXGROUP,TRUE );
        m_uiMgr.SetVisible( IDC_TP_TEXTURELIST,TRUE );
        m_uiMgr.SetVisible( IDC_TP_ADDTEX,TRUE );
        m_uiMgr.SetVisible( IDC_TP_DELTEX,TRUE );
        m_uiMgr.SetVisible( IDC_TP_PREVIEW,TRUE );
        m_uiMgr.SetVisible( IDC_TP_APPLYALL,TRUE );
        m_uiMgr.SetVisible( IDC_TP_TILEGROUP,TRUE );
        m_uiMgr.SetVisible( IDC_TP_TXTMAINTEX,TRUE );
        m_uiMgr.SetVisible( IDC_TP_BTNMAINTEX,TRUE );
        m_uiMgr.SetVisible( IDC_TP_TXTDETAIL1,TRUE );
        m_uiMgr.SetVisible( IDC_TP_BTNDETAIL1,TRUE );
        m_uiMgr.SetVisible( IDC_TP_TXTDETAIL2,TRUE );
        m_uiMgr.SetVisible( IDC_TP_BTNDETAIL2,TRUE );
        m_uiMgr.SetVisible( IDC_TP_TXTDETAIL3,TRUE );
        m_uiMgr.SetVisible( IDC_TP_BTNDETAIL3,TRUE );
        m_uiMgr.SetVisible( IDC_TP_TXTDETAIL4,TRUE );
        m_uiMgr.SetVisible( IDC_TP_BTNDETAIL4,TRUE );
        etCoreManager::Instance().SetWorkMode( WKM_TEXTURE );
        break;
    case WP_ENTITY:
        m_uiMgr.SetVisible( IDC_AP_ENTITYGROUP,TRUE );
        m_uiMgr.SetVisible( IDC_AP_ENTITYLIST,TRUE );
        m_uiMgr.SetVisible( IDC_AP_CURRENTTYPE,TRUE );
        m_uiMgr.SetVisible( IDC_AP_CURRENTENTITY,TRUE );
        m_uiMgr.SetVisible( IDC_AP_ENTITYATTRIB,TRUE );
        m_uiMgr.SetVisible( IDC_AP_ENTITYNAME,TRUE );
        m_uiMgr.SetVisible( IDC_AP_ENTITYNAMEBTN,TRUE );
        m_uiMgr.SetVisible( IDC_AP_ENTITYTYPE,TRUE );
        m_uiMgr.SetVisible( IDC_AP_ENTITYTYPELBL,TRUE );
        m_uiMgr.SetVisible( IDC_AP_ENTITYTEM,TRUE );
        m_uiMgr.SetVisible( IDC_AP_ENTITYTEMLBL,TRUE );
        m_uiMgr.SetVisible( IDC_AP_ENTITYPOS,TRUE );
        m_uiMgr.SetVisible( IDC_AP_ENTITYPOSBTN,TRUE );
        m_uiMgr.SetVisible( IDC_AP_ENTITYROT,TRUE );
        m_uiMgr.SetVisible( IDC_AP_ENTITYROTBTN,TRUE );
        m_uiMgr.SetVisible( IDC_AP_ENTITYSCALE,TRUE );
        m_uiMgr.SetVisible( IDC_AP_ENTITYSCALEBTN,TRUE );
        etCoreManager::Instance().SetWorkMode( WKM_ENTITY );
        break;
    }

    m_uiMgr.SetVisible( IDC_WORKSPACE,TRUE );
    m_uiMgr.SetVisible( IDC_SWITCH_TERRAIN,TRUE );
    m_uiMgr.SetVisible( IDC_SWITCH_TEXTURE,TRUE );
    m_uiMgr.SetVisible( IDC_SWITCH_ENTITY,TRUE );

    m_ActivePanel = activePanel;
}


//------------------------------------------------------------------------
// CALLBACK function to notify the events for menu.
// ----------------------------------------------------------------------
// Param -> IN:
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
void etInterface::MsgProcMenu( WPARAM wParam,LPARAM lParam )
{
    // Get the handle to the menu.
    HMENU hMenu = GetMenu( m_hWnd );

    // Get the coreManager.
    etCoreManager& core = etCoreManager::Instance();

    switch( wParam )
    {
    case IDM_FILE_NEW:
        break;
    case IDM_FILE_RESET:
        break;
    case IDM_FILE_OPEN:
        break;
    case IDM_FILE_SAVE:
        break;
    case IDM_FILE_SAVEAS:
        break;
    case IDM_FILE_EXIT:
        g_pApp->Quit();
        break;

    case IDM_EDIT_SELECTALL:
        core.GetEntityManager()->SelectAll( core.GetSelectionBuffer() );
        core.Activate();
        break;
    case IDM_EDIT_INVSELECT:
        core.GetEntityManager()->SelectInvert( core.GetSelectionBuffer() );
        core.Activate();
        break;
    case IDM_EDIT_SELECTNO:
        core.GetSelectionBuffer()->Clear();
        core.Activate();
        break;
    case IDM_EDIT_HIDESEL:
        core.GetEntityManager()->HideSelected();
        core.Activate();
        break;
    case IDM_EDIT_HIDENOHIDE:
        core.GetEntityManager()->HideInvert();
        core.Activate();
        break;
    case IDM_EDIT_HIDEALL:
        core.GetEntityManager()->HideAll();
        core.Activate();
        break;
    case IDM_EDIT_SHOWALL:
        core.GetEntityManager()->UnHideAll();
        core.Activate();
        break;
    case IDM_EDIT_DELETE:
        core.GetSelectionBuffer()->DeleteSelected();
        core.Activate();
        break;

    case IDM_VIEW_TEXTURE:
        CheckMenuItem( hMenu,IDM_VIEW_TEXTURE,MF_CHECKED );
        CheckMenuItem( hMenu,IDM_VIEW_SOLID,MF_UNCHECKED );
        CheckMenuItem( hMenu,IDM_VIEW_WIREFRAME,MF_UNCHECKED );
        CheckMenuItem( hMenu,IDM_VIEW_WIRETEX,MF_UNCHECKED );
        core.GetSceneManager()->SetRenderMode( RM_TEXTURE );
        core.Activate();
        break;
    case IDM_VIEW_SOLID:
        CheckMenuItem( hMenu,IDM_VIEW_TEXTURE,MF_UNCHECKED );
        CheckMenuItem( hMenu,IDM_VIEW_SOLID,MF_CHECKED );
        CheckMenuItem( hMenu,IDM_VIEW_WIREFRAME,MF_UNCHECKED );
        CheckMenuItem( hMenu,IDM_VIEW_WIRETEX,MF_UNCHECKED );
        core.GetSceneManager()->SetRenderMode( RM_SOLID );
        core.Activate();
        break;
    case IDM_VIEW_WIREFRAME:
        CheckMenuItem( hMenu,IDM_VIEW_TEXTURE,MF_UNCHECKED );
        CheckMenuItem( hMenu,IDM_VIEW_SOLID,MF_UNCHECKED );
        CheckMenuItem( hMenu,IDM_VIEW_WIREFRAME,MF_CHECKED );
        CheckMenuItem( hMenu,IDM_VIEW_WIRETEX,MF_UNCHECKED );
        core.GetSceneManager()->SetRenderMode( RM_WIREFRAME );
        core.Activate();
        break;
    case IDM_VIEW_WIRETEX:
        CheckMenuItem( hMenu,IDM_VIEW_TEXTURE,MF_UNCHECKED );
        CheckMenuItem( hMenu,IDM_VIEW_SOLID,MF_UNCHECKED );
        CheckMenuItem( hMenu,IDM_VIEW_WIREFRAME,MF_UNCHECKED );
        CheckMenuItem( hMenu,IDM_VIEW_WIRETEX,MF_CHECKED );
        core.GetSceneManager()->SetRenderMode( RM_TEXTUREWIRE );
        core.Activate();
        break;
    case IDM_VIEW_RESETCAM:
        core.ResetCamera();
        core.Activate();
        break;
    case IDM_VIEW_CAM_EDIT:
        CheckMenuItem( hMenu,IDM_VIEW_CAM_EDIT,MF_CHECKED );
        CheckMenuItem( hMenu,IDM_VIEW_CAM_RAMBLE,MF_UNCHECKED );
        core.SetCameraMode( CM_EDITCAMERA );
        core.Activate();
        break;
    case IDM_VIEW_CAM_RAMBLE:
        CheckMenuItem( hMenu,IDM_VIEW_CAM_EDIT,MF_UNCHECKED );
        CheckMenuItem( hMenu,IDM_VIEW_CAM_RAMBLE,MF_CHECKED );
        core.SetCameraMode( CM_RAMBLECAMERA );
        core.Activate();
        break;

    case IDM_MAP_SKYBOX:
        DialogBox( m_hInst,(LPCTSTR)IDD_DLG_SKYBOX,m_hWnd,Callback_Skybox );
        break;
    case IDM_MAP_FOGSTATE:
        DialogBox( m_hInst,(LPCTSTR)IDD_DLG_FOGSTATE,m_hWnd,Callback_SetFog );
        break;

    case IDM_TOOL_OPTIONS:
        DialogBox( m_hInst,(LPCTSTR)IDD_DLG_EDITORCONFIG,m_hWnd,Callback_Config );
        break;

    case IDM_HELP_ABOUT:
        DialogBox( m_hInst,(LPCTSTR)IDD_DLG_ABOUT,m_hWnd,Callback_About );
        break;
    }
}


//------------------------------------------------------------------------
// CALLBACK function to notify the events for toolbar.
// ----------------------------------------------------------------------
// Param -> IN:
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
void etInterface::MsgProcToolbar( WPARAM wParam,LPARAM lParam )
{
    etCoreManager& core = etCoreManager::Instance();

    switch( wParam )
    {
    case IDT_NEWSCENE:
        break;
    case IDT_OPENSCENE:
        break;
    case IDT_SAVESCENE:
        break;

    case IDT_SELOBJECT:
        if( !core.IsHaveProject() ) return;
        DialogBox( m_hInst,(LPCTSTR)IDD_DLG_ENTITYLIST,m_hWnd,Callback_Entity );
        break;
    case IDT_MAPINFO:
        if( !core.IsHaveProject() ) return;
        DialogBox( m_hInst,(LPCTSTR)IDD_DLG_MAPINFO,m_hWnd,Callback_MapInfo );
        break;
    case IDT_LIGHTMODE:
        if( !core.IsHaveProject() ) return;
        DialogBox( m_hInst,(LPCTSTR)IDD_DLG_LIGHTMODE,m_hWnd,Callback_LightMode );
        break;

    case IDT_SELECT:
        core.GetSelectionBuffer()->SetToolType( ETT_SELECT );
        core.Activate();
        break;
    case IDT_MOVEXZ:
        core.GetSelectionBuffer()->SetToolType( ETT_MOVE_XZ );
        core.Activate();
        break;
    case IDT_MOVEY:
        core.GetSelectionBuffer()->SetToolType( ETT_MOVE_Y );
        core.Activate();
        break;
    case IDT_ROTATEX:
        core.GetSelectionBuffer()->SetToolType( ETT_ROTATION_X );
        core.Activate();
        break;
    case IDT_ROTATEY:
        core.GetSelectionBuffer()->SetToolType( ETT_ROTATION_Y );
        core.Activate();
        break;
    case IDT_ROTATEZ:
        core.GetSelectionBuffer()->SetToolType( ETT_ROTATION_Z );
        core.Activate();
        break;
    case IDT_SCALE:
        core.GetSelectionBuffer()->SetToolType( ETT_SCALE_XYZ );
        core.Activate();
        break;

    case IDT_ABOUT:
        DialogBox( m_hInst,(LPCTSTR)IDD_DLG_ABOUT,m_hWnd,Callback_About );
        break;
    }
}


//------------------------------------------------------------------------
// CALLBACK function to notify the GUI events.
// ----------------------------------------------------------------------
// Param -> IN:
//      WPARAM:     Param of the window message.
//      WPARAM:     Param of the window message.
//------------------------------------------------------------------------
void etInterface::MsgProcGUI( WPARAM wParam,LPARAM lParam )
{
    int n;
    HRESULT hr;

    // Get the control id.
    UINT nControlID = m_uiMgr.GetIDFromHandle( (HWND)lParam );

    // Get the instance of coreManager.
    etCoreManager& core = etCoreManager::Instance();

    // Get the pointer to the brush.
    etBrush* pBrush = core.GetBrush();

    // Get the pointer to the selectionBuffer.
    etSelectionBuffer* pSelBuf = core.GetSelectionBuffer();

    // Get the pointer to the terrain.
    etTerrain* pTerrain = core.GetSceneManager()->GetTerrainLevel();

    switch( nControlID )
    {
    // 1. Switchers of the work panel.
    case IDC_SWITCH_TERRAIN:
        SetActiveWorkPanel( WP_TERRAIN );
        break;
    case IDC_SWITCH_TEXTURE:
        SetActiveWorkPanel( WP_TEXTURE );
        break;
    case IDC_SWITCH_ENTITY:
        SetActiveWorkPanel( WP_ENTITY );
        break;

    // 2. Terrain edit panel.
    case IDC_HP_TOOLTYPE:
        if( HIWORD(wParam) == CBN_SELCHANGE )
        {
            n = SendMessage( (HWND)lParam,CB_GETCURSEL,0,0 );

            switch( n )
            {
            case 0: pBrush->SetBrushType( TT_RAISE );   break;
            case 1: pBrush->SetBrushType( TT_LOWER );   break;
            case 2: pBrush->SetBrushType( TT_FLATTEN ); break;
            case 3: pBrush->SetBrushType( TT_SMOOTH );  break;
            case 4: pBrush->SetBrushType( TT_DIG );     break;
            case 5: pBrush->SetBrushType( TT_PAINT );   break;
            case 6: pBrush->SetBrushType( TT_ERASE );   break;
            case 7: pBrush->SetBrushType( TT_BLEND );   break;
            }

            if( n==0 || n==1 || n==2 || n==3 || n==4 )
            {
                m_uiMgr.SetEnabled( IDC_HP_LAYER1,FALSE );
                m_uiMgr.SetEnabled( IDC_HP_LAYER2,FALSE );
                m_uiMgr.SetEnabled( IDC_HP_LAYER3,FALSE );
                m_uiMgr.SetEnabled( IDC_HP_LAYER4,FALSE );
            }
            else
            {
                m_uiMgr.SetEnabled( IDC_HP_LAYER1,TRUE );
                m_uiMgr.SetEnabled( IDC_HP_LAYER2,TRUE );
                m_uiMgr.SetEnabled( IDC_HP_LAYER3,TRUE );
                m_uiMgr.SetEnabled( IDC_HP_LAYER4,TRUE );
            }
        }
        break;
    case IDC_HP_RECTANGLE:
        pBrush->SetBrushShape( TS_RECTANGLE );
        core.Activate();
        break;
    case IDC_HP_CIRCLE:
        pBrush->SetBrushShape( TS_CIRCLE );
        core.Activate();
        break;
    case IDC_HP_LAYER1:
        pBrush->SetCurrentLayer( 0 );
        break;
    case IDC_HP_LAYER2:
        pBrush->SetCurrentLayer( 1 );
        break;
    case IDC_HP_LAYER3:
        pBrush->SetCurrentLayer( 2 );
        break;
    case IDC_HP_LAYER4:
        pBrush->SetCurrentLayer( 3 );
        break;

    // 3. Texture edit panel.
    case IDC_TP_ADDTEX:
        if( m_ToolDlg.DlgOpenFile(m_hWnd,"位图文件(*.bmp)\0*.bmp") )
        {
            hr = pTerrain->AddTexture( m_ToolDlg.GetFilename() );
            if( FAILED(hr) )
            {
                ShowErrorDialog( hr );
                return;
            }

            m_uiMgr.AddItem( IDC_TP_TEXTURELIST,etStringUtil::GetNameFromPath(
                m_ToolDlg.GetFilename()),NULL );
        }
        break;
    case IDC_TP_DELTEX:
        n = m_uiMgr.GetCurSel( IDC_TP_TEXTURELIST );
        if( n == LB_ERR ) break;

        if( MessageBox(m_hWnd,"确定要删除么？","提示",MB_YESNO|MB_ICONQUESTION)
            == IDYES )
        {
            pTerrain->DeleteTexture( n );
            m_uiMgr.DeleteItem( IDC_TP_TEXTURELIST,n );
            ShowTileInfo();
        }
        break;
    case IDC_TP_PREVIEW:
        n = m_uiMgr.GetCurSel( IDC_TP_TEXTURELIST );
        if( n == LB_ERR ) break;

        // Open the dialog to preview the texture.
        DialogBoxParam( g_pApp->GetInstance(),(LPCTSTR)IDD_DLG_PREVIEWTEX,m_hWnd,
            Callback_PreviewTex,(LPARAM)pTerrain->GetTexture(n)->GetName() );
        break;
    case IDC_TP_APPLYALL:
        n = m_uiMgr.GetCurSel( IDC_TP_TEXTURELIST );
        if( n != LB_ERR ) ApplyTerrainTexture( n );
        break;
    case IDC_TP_BTNMAINTEX:
        EditTileTexture( -1 );
        break;
    case IDC_TP_BTNDETAIL1:
        EditTileTexture( 0 );
        break;
    case IDC_TP_BTNDETAIL2:
        EditTileTexture( 1 );
        break;
    case IDC_TP_BTNDETAIL3:
        EditTileTexture( 2 );
        break;
    case IDC_TP_BTNDETAIL4:
        EditTileTexture( 3 );
        break;

    // 4. Entity edit panel.
    case IDC_AP_ENTITYNAMEBTN:
        DialogBox( m_hInst,(LPCTSTR)IDD_DLG_ENTITYNAME,m_hWnd,Callback_SetName );
        break;
    case IDC_AP_ENTITYPOSBTN:
        {
            sVector vec;
            memcpy( &vec,&pSelBuf->GetSelectedEntity(0)->GetPosition(),sizeof(FlyVector) );
            if( ShowVectorSetting(&vec) )
            {
                pSelBuf->GetSelectedEntity(0)->SetPosition( *((FlyVector*)&vec) );
                ShowEntityInfo();
            }
        }
        break;
    case IDC_AP_ENTITYROTBTN:
        {
            sVector vec;
            memcpy( &vec,&pSelBuf->GetSelectedEntity(0)->GetOrientation(),sizeof(FlyVector) );
            if( ShowVectorSetting(&vec) )
            {
                pSelBuf->GetSelectedEntity(0)->SetOrientation( *((FlyVector*)&vec) );
                ShowEntityInfo();
            }
        }
        break;
    case IDC_AP_ENTITYSCALEBTN:
        {
            sVector vec;
            memcpy( &vec,&pSelBuf->GetSelectedEntity(0)->GetScale(),sizeof(FlyVector) );
            if( ShowVectorSetting(&vec) )
            {
                pSelBuf->GetSelectedEntity(0)->SetScale( *((FlyVector*)&vec) );
                ShowEntityInfo();
            }
        }
        break;
    }

    if( nControlID != IDC_HP_TOOLTYPE &&
        nControlID != IDC_TP_TEXTURELIST )
        SetFocus( m_hWnd );
}


//------------------------------------------------------------------------
// CALLBACK function to handle the trackbar's message.
// ----------------------------------------------------------------------
// Param -> IN:
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
void etInterface::MsgProcTrackBar( WPARAM wParam,LPARAM lParam )
{
    char cText[40];
    HWND hAnother; int nAnother;
    HWND hControl = (HWND)lParam;
    UINT nID = m_uiMgr.GetIDFromHandle( hControl );
    int nPos = SendMessage( hControl,TBM_GETPOS,0,0 );

    etBrush* pBrush = etCoreManager::Instance().GetBrush();

    switch( nID )
    {
    case IDC_HP_OUTRADIUS:
        sprintf( cText,"刷子外径: %d",nPos );
        m_uiMgr.SetText( IDC_HP_LBLORADIUS,cText );
        pBrush->SetOuterRadius( (float)nPos );

        hAnother = m_uiMgr.GetSafeHandle( IDC_HP_INNERRADIUS );
        nAnother = SendMessage( hAnother,TBM_GETPOS,0,0 );
        if( nPos < nAnother )
        {
            sprintf( cText,"刷子内径: %d",nPos );
            SendMessage( hAnother,TBM_SETPOS,TRUE,nPos );
            m_uiMgr.SetText( IDC_HP_LBLIRADIUS,cText );
            pBrush->SetInnerRadius( (float)nPos );
        }
        break;
    case IDC_HP_INNERRADIUS:
        sprintf( cText,"刷子内径: %d",nPos );
        m_uiMgr.SetText( IDC_HP_LBLIRADIUS,cText );
        pBrush->SetInnerRadius( (float)nPos );

        hAnother = m_uiMgr.GetSafeHandle( IDC_HP_OUTRADIUS );
        nAnother = SendMessage( hAnother,TBM_GETPOS,0,0 );
        if( nPos > nAnother )
        {
            sprintf( cText,"刷子外径: %d",nPos );
            SendMessage( hAnother,TBM_SETPOS,TRUE,nPos );
            m_uiMgr.SetText( IDC_HP_LBLORADIUS,cText );
            pBrush->SetOuterRadius( (float)nPos );
        }
        break;
    case IDC_HP_STRENGTH:
        sprintf( cText,"刷子强度: %d",nPos );
        m_uiMgr.SetText( IDC_HP_LBLSTRENGTH,cText );
        pBrush->SetStrength( (float)nPos );
        break;
    }

    etCoreManager::Instance().Activate();
    SetFocus( m_hWnd );
}


//------------------------------------------------------------------------
// CALLBACK function to handle the trackview's message.
// ----------------------------------------------------------------------
// Param -> IN:
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
void etInterface::MsgProcTreeView( WPARAM wParam,LPARAM lParam )
{
    HRESULT hr;
    char cTemp[MAX_PATH];
    HTREEITEM hTypes[6];
    HTREEITEM hSelectedItem;
    HTREEITEM hItem;

    if( !etCoreManager::Instance().IsHaveProject() )
        return;

    // Get the pointer to the entityManager.
    etEntityManager* pEntityMgr = etCoreManager::Instance().GetEntityManager();

    hTypes[0] = m_uiMgr.GetRootItem( IDC_AP_ENTITYLIST );
    hTypes[1] = m_uiMgr.GetNextSibling( IDC_AP_ENTITYLIST,hTypes[0] );
    hTypes[2] = m_uiMgr.GetNextSibling( IDC_AP_ENTITYLIST,hTypes[1] );
    hTypes[3] = m_uiMgr.GetNextSibling( IDC_AP_ENTITYLIST,hTypes[2] );
    hTypes[4] = m_uiMgr.GetNextSibling( IDC_AP_ENTITYLIST,hTypes[3] );
    hTypes[5] = m_uiMgr.GetNextSibling( IDC_AP_ENTITYLIST,hTypes[4] );

    switch( ((LPNMHDR)lParam)->code )
    {
    case NM_DBLCLK:
        hSelectedItem = m_uiMgr.GetCurSelItem( IDC_AP_ENTITYLIST );
        hItem = m_uiMgr.GetParentItem( IDC_AP_ENTITYLIST,hSelectedItem );

        if( hSelectedItem == m_uiMgr.GetChildItem(IDC_AP_ENTITYLIST,hItem) )
        {
            // 1. 模型
            if( hItem == hTypes[0] )
            {
                if( m_ToolDlg.DlgOpenFile(m_hWnd,"模型文件(*.smd)\0*.smd") )
                {
                    hr = pEntityMgr->AddMeshTemplate( m_ToolDlg.GetFilename() );

                    if( SUCCEEDED(hr) )
                    {
                        strcpy( cTemp,etStringUtil::GetNameFromPath(m_ToolDlg.GetFilename()) );
                        m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,cTemp,1,hItem,TVI_LAST );
                    }
                    else
                        ShowErrorDialog( hr );
                }
            }

            // 2. 动画
            else if( hItem == hTypes[1] )
            {
                INT_PTR nRes;
                nRes = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_ADDANIMATION,m_hWnd,
                    Callback_AddAnim,(LPARAM)cTemp );

                if( nRes == 1 )
                    m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,cTemp,1,hItem,TVI_LAST );
            }

            // 3. 粒子
            else if( hItem == hTypes[2] )
            {
                if( m_ToolDlg.DlgOpenFile(m_hWnd,"粒子文件(*.par)\0*.par") )
                {
                    hr = pEntityMgr->AddParticleTemplate( m_ToolDlg.GetFilename() );

                    if( SUCCEEDED(hr) )
                    {
                        strcpy( cTemp,etStringUtil::GetNameFromPath(m_ToolDlg.GetFilename()) );
                        m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,cTemp,1,hItem,TVI_LAST );
                    }
                    else
                        ShowErrorDialog( hr );
                }
            }

            // 4. 植被
            else if( hItem == hTypes[3] )
            {
                INT_PTR nRes;
                char cTemName[MAX_PATH];
                nRes = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_ADDVEGETATION,m_hWnd,
                    Callback_AddVeg,(LPARAM)cTemName );

                if( nRes == 1 )
                    m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,cTemName,1,hItem,TVI_LAST );
            }

            // 5. 精灵
            else if( hItem == hTypes[4] )
            {
                INT_PTR nRes;
                cbSpriteTemplate sprite;
                nRes = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_ADDSPRITE,m_hWnd,
                    Callback_AddSprite,(LPARAM)&sprite );

                if( nRes == 1 )
                    m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,sprite.sName.c_str(),1,hItem,TVI_LAST );
            }

            // 6. 液体
            else if( hItem == hTypes[5] )
            {
                INT_PTR nRes;
                cbLiquidData liquid;
                memset( &liquid,0,sizeof(cbLiquidData) );

                liquid.bEditLiquid = false;
                nRes = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_EDITWATER,m_hWnd,
                    Callback_EditWater,(LPARAM)&liquid );

                if( nRes == 1 )
                {
                    etLiquidObject* pLiquid;
                    pLiquid = pEntityMgr->AddLiquidEntity( liquid.sName.c_str(),liquid.sSurfaceMap.c_str(),
                        liquid.sDistortMap.c_str(),(float*)&liquid.vPosition,liquid.fWidth,liquid.fHeight );
                    pLiquid->SetTexScale( liquid.fTexScale );
                    pLiquid->SetDistortion( liquid.fDistortion );
                    pLiquid->SetLiquidSpeed( liquid.fFlowSpeed );
                    pLiquid->SetReflection( liquid.bReflection );
                    pLiquid->SetReflectTerrain( liquid.bReflectTerrain );
                    pLiquid->SetReflectMesh( liquid.bReflectEntity );
                    pLiquid->SetReflectSky( liquid.bReflectSkybox );
                    m_uiMgr.AddItemToTree( IDC_AP_ENTITYLIST,liquid.sName.c_str(),1,hItem,TVI_LAST );
                }
            }
        }
        else
        {
            UINT nIndex = m_uiMgr.GetChildIndex( IDC_AP_ENTITYLIST,hSelectedItem );

            if( nIndex != -1 && nIndex != 0 )
            {
                if( hItem == hTypes[0] )
                {
                    pEntityMgr->SetCurrentType( ENTITY_MESH );
                    m_uiMgr.SetText( IDC_AP_CURRENTTYPE,"当前类型: 模型" );
                }
                else if( hItem == hTypes[1] )
                {
                    pEntityMgr->SetCurrentType( ENTITY_ANIMATION );
                    m_uiMgr.SetText( IDC_AP_CURRENTTYPE,"当前类型: 动画" );
                }
                else if( hItem == hTypes[2] )
                {
                    pEntityMgr->SetCurrentType( ENTITY_PARTICLE );
                    m_uiMgr.SetText( IDC_AP_CURRENTTYPE,"当前类型: 粒子" );
                }
                else if( hItem == hTypes[3] )
                {
                    pEntityMgr->SetCurrentType( ENTITY_VEGETATION );
                    m_uiMgr.SetText( IDC_AP_CURRENTTYPE,"当前类型: 植被" );
                }
                else if( hItem == hTypes[4] )
                {
                    pEntityMgr->SetCurrentType( ENTITY_SPRITE );
                    m_uiMgr.SetText( IDC_AP_CURRENTTYPE,"当前类型: 精灵" );
                }
                else if( hItem == hTypes[5] )
                {
                    INT_PTR nRes;
                    cbLiquidData liquid;
                    memset( &liquid,0,sizeof(cbLiquidData) );

                    etLiquidObject* pLiquid = pEntityMgr->GetLiquidObject( nIndex-1 );
                    liquid.sName = pLiquid->GetLiquidName();
                    liquid.bReflection = pLiquid->IsReflection();
                    liquid.sSurfaceMap = pLiquid->GetLiquidSurface()->GetName();
                    liquid.sDistortMap = pLiquid->GetDistortMap()->GetName();
                    memcpy( &liquid.vPosition,&pLiquid->GetPosition(),sizeof(float)*4 );
                    liquid.fWidth = pLiquid->GetWidth();
                    liquid.fHeight = pLiquid->GetHeight();
                    liquid.fTexScale = pLiquid->GetTexScale();
                    liquid.fDistortion = pLiquid->GetDistortion();
                    liquid.fFlowSpeed = pLiquid->GetLiquidSpeed();
                    liquid.bReflectTerrain = pLiquid->IsReflectTerrain();
                    liquid.bReflectEntity = pLiquid->IsReflectMesh();
                    liquid.bReflectSkybox = pLiquid->IsReflectSky();

                    liquid.bEditLiquid = true;
                    nRes = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_EDITWATER,m_hWnd,
                        Callback_EditWater,(LPARAM)&liquid );
                    if( nRes == 1 )
                    {
                        pLiquid->SetSurfaceMap( liquid.sSurfaceMap.c_str() );
                        pLiquid->SetDistortMap( liquid.sDistortMap.c_str() );
                        pLiquid->UpdateGeometry( (float*)&liquid.vPosition,liquid.fWidth,liquid.fHeight );
                        pLiquid->SetTexScale( liquid.fTexScale );
                        pLiquid->SetDistortion( liquid.fDistortion );
                        pLiquid->SetLiquidSpeed( liquid.fFlowSpeed );
                        pLiquid->SetReflection( liquid.bReflection );
                        pLiquid->SetReflectTerrain( liquid.bReflectTerrain );
                        pLiquid->SetReflectMesh( liquid.bReflectEntity );
                        pLiquid->SetReflectSky( liquid.bReflectSkybox );
                    }
                    break;
                }

                char cLabelText[MAX_PATH];
                pEntityMgr->SetCurrentIndex( nIndex-1 );
                m_uiMgr.GetTreeItemString( IDC_AP_ENTITYLIST,hSelectedItem,cTemp,80 );
                sprintf( cLabelText,"当前实体: %s",cTemp );
                m_uiMgr.SetText( IDC_AP_CURRENTENTITY,cLabelText );
            }
        }
        break;
    case NM_KEYDOWN:
        break;
    }
}


//------------------------------------------------------------------------
// CALLBACK function to handle the window message.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the window.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
void etInterface::MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    switch( uMsg )
    {
    case WM_COMMAND:
        MsgProcMenu( wParam,lParam );
        MsgProcToolbar( wParam,lParam );
        MsgProcGUI( wParam,lParam );
        break;
    case WM_HSCROLL:
        MsgProcTrackBar( wParam,lParam );
        break;
    case WM_MOUSEMOVE:
        HWND hList[3];
        hList[0] = m_uiMgr.GetSafeHandle( IDC_HP_TOOLTYPE );
        hList[1] = m_uiMgr.GetSafeHandle( IDC_TP_TEXTURELIST );
        hList[2] = m_uiMgr.GetSafeHandle( IDC_AP_ENTITYLIST );
        if( GetFocus() == hList[0] ||
            GetFocus() == hList[1] ||
            GetFocus() == hList[2] )
            SetFocus( m_hWnd );
        break;
    case WM_LBUTTONDOWN:
        if( m_ActivePanel == WP_TEXTURE )
            ShowTileInfo();
        else if( m_ActivePanel == WP_ENTITY )
        {
            m_uiMgr.SetText( IDC_AP_CURRENTTYPE,"当前类型: 空" );
            m_uiMgr.SetText( IDC_AP_CURRENTENTITY,"当前实体: 空" );
        }
        break;
    case WM_LBUTTONUP:
        if( m_ActivePanel == WP_ENTITY )
            ShowEntityInfo();
        break;
    case WM_NOTIFY:
        MsgProcTreeView( wParam,lParam );
        break;
    }
}


//------------------------------------------------------------------------
// Show the current terrain tile info.
//------------------------------------------------------------------------
void etInterface::ShowTileInfo(void)
{
    char cTemp[80];
    UINT nTextureID;

    // Get the pointer to the terrain.
    etTerrain* pTerrain = etCoreManager::Instance().GetSceneManager()
        ->GetTerrainLevel();

    if( !pTerrain )
    {
        m_uiMgr.SetText( IDC_TP_BTNMAINTEX,"空" );
        m_uiMgr.SetEnabled( IDC_TP_BTNMAINTEX,FALSE );
        m_uiMgr.SetText( IDC_TP_BTNDETAIL1,"空" );
        m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL1,FALSE );
        m_uiMgr.SetText( IDC_TP_BTNDETAIL2,"空" );
        m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL2,FALSE );
        m_uiMgr.SetText( IDC_TP_BTNDETAIL3,"空" );
        m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL3,FALSE );
        m_uiMgr.SetText( IDC_TP_BTNDETAIL4,"空" );
        m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL4,FALSE );
        return;
    }

    // Get the selected tile.
    etTile* pSelected = pTerrain->GetSelectedTile();
    if( !pSelected )
    {
        m_uiMgr.SetText( IDC_TP_BTNMAINTEX,"空" );
        m_uiMgr.SetEnabled( IDC_TP_BTNMAINTEX,FALSE );
        m_uiMgr.SetText( IDC_TP_BTNDETAIL1,"空" );
        m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL1,FALSE );
        m_uiMgr.SetText( IDC_TP_BTNDETAIL2,"空" );
        m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL2,FALSE );
        m_uiMgr.SetText( IDC_TP_BTNDETAIL3,"空" );
        m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL3,FALSE );
        m_uiMgr.SetText( IDC_TP_BTNDETAIL4,"空" );
        m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL4,FALSE );
        return;
    }

    m_uiMgr.SetEnabled( IDC_TP_BTNMAINTEX,TRUE );
    nTextureID = pTerrain->GetTextureID( pSelected->GetMainTexture() );
    if( nTextureID == -1 )
        m_uiMgr.SetText( IDC_TP_BTNMAINTEX,"空" );
    else
    {
        sprintf( cTemp,"Texture: %d",nTextureID );
        m_uiMgr.SetText( IDC_TP_BTNMAINTEX,cTemp );
    }

    // Enable all the button to set detail.
    m_uiMgr.SetText( IDC_TP_BTNDETAIL1,"空" );
    m_uiMgr.SetText( IDC_TP_BTNDETAIL2,"空" );
    m_uiMgr.SetText( IDC_TP_BTNDETAIL3,"空" );
    m_uiMgr.SetText( IDC_TP_BTNDETAIL4,"空" );
    m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL1,FALSE );
    m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL2,FALSE );
    m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL3,FALSE );
    m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL4,FALSE );

    m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL1,TRUE );
    nTextureID = pTerrain->GetTextureID( pSelected->GetDetailTexture(0) );
    if( nTextureID == -1 )
        return;
    else
    {
        sprintf( cTemp,"Texture: %d",nTextureID );
        m_uiMgr.SetText( IDC_TP_BTNDETAIL1,cTemp );
        m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL2,TRUE );
    }

    nTextureID = pTerrain->GetTextureID( pSelected->GetDetailTexture(1) );
    if( nTextureID == -1 )
        return;
    else
    {
        sprintf( cTemp,"Texture: %d",nTextureID );
        m_uiMgr.SetText( IDC_TP_BTNDETAIL2,cTemp );
        m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL3,TRUE );
    }

    nTextureID = pTerrain->GetTextureID( pSelected->GetDetailTexture(2) );
    if( nTextureID == -1 )
        return;
    else
    {
        sprintf( cTemp,"Texture: %d",nTextureID );
        m_uiMgr.SetText( IDC_TP_BTNDETAIL3,cTemp );
        m_uiMgr.SetEnabled( IDC_TP_BTNDETAIL4,TRUE );
    }

    nTextureID = pTerrain->GetTextureID( pSelected->GetDetailTexture(3) );
    if( nTextureID == -1 )
        return;
    else
    {
        sprintf( cTemp,"Texture: %d",nTextureID );
        m_uiMgr.SetText( IDC_TP_BTNDETAIL4,cTemp );
    }
}


//------------------------------------------------------------------------
// Show the selected entity info.
//------------------------------------------------------------------------
void etInterface::ShowEntityInfo(void)
{
    char cTemp[40];
    struct _Vector { float x,y,z,w; } vTemp;

    // Get the pointer to the selectionBuffer.
    etSelectionBuffer* pBuffer = etCoreManager::Instance().GetSelectionBuffer();

    if( pBuffer->GetNumSelected() == 0 )
    {
        m_uiMgr.SetText( IDC_AP_ENTITYNAMEBTN,"空" );
        m_uiMgr.SetText( IDC_AP_ENTITYTYPELBL,"空" );
        m_uiMgr.SetText( IDC_AP_ENTITYTEMLBL,"空" );
        m_uiMgr.SetText( IDC_AP_ENTITYPOSBTN,"(0.000,0.000,0.000)" );
        m_uiMgr.SetText( IDC_AP_ENTITYROTBTN,"(0.000,0.000,0.000)" );
        m_uiMgr.SetText( IDC_AP_ENTITYSCALEBTN,"(0.000,0.000,0.000)" );
        m_uiMgr.SetEnabled( IDC_AP_ENTITYNAMEBTN,FALSE );
        m_uiMgr.SetEnabled( IDC_AP_ENTITYPOSBTN,FALSE );
        m_uiMgr.SetEnabled( IDC_AP_ENTITYROTBTN,FALSE );
        m_uiMgr.SetEnabled( IDC_AP_ENTITYSCALEBTN,FALSE );
    }
    else if( pBuffer->GetNumSelected() > 1 )
    {
        m_uiMgr.SetText( IDC_AP_ENTITYNAMEBTN,"多个实体" );
        m_uiMgr.SetText( IDC_AP_ENTITYTYPELBL,"多个实体" );
        m_uiMgr.SetText( IDC_AP_ENTITYTEMLBL,"多个实体" );
        m_uiMgr.SetText( IDC_AP_ENTITYPOSBTN,"(0.000,0.000,0.000)" );
        m_uiMgr.SetText( IDC_AP_ENTITYROTBTN,"(0.000,0.000,0.000)" );
        m_uiMgr.SetText( IDC_AP_ENTITYSCALEBTN,"(0.000,0.000,0.000)" );
        m_uiMgr.SetEnabled( IDC_AP_ENTITYNAMEBTN,FALSE );
        m_uiMgr.SetEnabled( IDC_AP_ENTITYPOSBTN,FALSE );
        m_uiMgr.SetEnabled( IDC_AP_ENTITYROTBTN,FALSE );
        m_uiMgr.SetEnabled( IDC_AP_ENTITYSCALEBTN,FALSE );
    }
    else
    {
        etEntity* pEntity = pBuffer->GetSelectedEntity( 0 );
        m_uiMgr.SetText( IDC_AP_ENTITYNAMEBTN,pEntity->GetName() );

        switch( pEntity->GetEntityType() )
        {
        case ENTITY_MESH:
            m_uiMgr.SetText( IDC_AP_ENTITYTYPELBL,"模型" );
            break;
        case ENTITY_ANIMATION:
            m_uiMgr.SetText( IDC_AP_ENTITYTYPELBL,"动画" );
            break;
        case ENTITY_PARTICLE:
            m_uiMgr.SetText( IDC_AP_ENTITYTYPELBL,"粒子" );
            break;
        case ENTITY_VEGETATION:
            m_uiMgr.SetText( IDC_AP_ENTITYTYPELBL,"植被" );
            break;
        case ENTITY_SPRITE:
            m_uiMgr.SetText( IDC_AP_ENTITYTYPELBL,"精灵" );
            break;
        case ENTITY_LIQUID:
            m_uiMgr.SetText( IDC_AP_ENTITYTYPELBL,"液体" );
            break;
        }

        m_uiMgr.SetText( IDC_AP_ENTITYTEMLBL,
            etStringUtil::GetNameFromPath(pEntity->GetTemplateName()) );
        memcpy( &vTemp,&pEntity->GetPosition(),sizeof(_Vector) );
        sprintf( cTemp,"(%.3f,%.3f,%.3f)",vTemp.x,vTemp.y,vTemp.z );
        m_uiMgr.SetText( IDC_AP_ENTITYPOSBTN,cTemp );
        memcpy( &vTemp,&pEntity->GetOrientation(),sizeof(_Vector) );
        sprintf( cTemp,"(%.3f,%.3f,%.3f)",vTemp.x,vTemp.y,vTemp.z );
        m_uiMgr.SetText( IDC_AP_ENTITYROTBTN,cTemp );
        memcpy( &vTemp,&pEntity->GetScale(),sizeof(_Vector) );
        sprintf( cTemp,"(%.3f,%.3f,%.3f)",vTemp.x,vTemp.y,vTemp.z );
        m_uiMgr.SetText( IDC_AP_ENTITYSCALEBTN,cTemp );

        m_uiMgr.SetEnabled( IDC_AP_ENTITYNAMEBTN,TRUE );
        m_uiMgr.SetEnabled( IDC_AP_ENTITYPOSBTN,TRUE );
        m_uiMgr.SetEnabled( IDC_AP_ENTITYROTBTN,TRUE );
        m_uiMgr.SetEnabled( IDC_AP_ENTITYSCALEBTN,TRUE );
    }
}


//------------------------------------------------------------------------
// Edit the terrain tile's texture.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Which layer to edit. (When is -1, edit the main layer).
//------------------------------------------------------------------------
void etInterface::EditTileTexture( int nLayer )
{
    int nRes;
    bool bFound = false;
    UINT nTextureID;
    sTileInfo tile;

    // Get the terrain and seleced tile.
    etTerrain* pTerrain = etCoreManager::Instance().GetSceneManager()->GetTerrainLevel();
    etTile* pSelected = pTerrain->GetSelectedTile();
    if( !pSelected )
    {
        MessageBox( m_hWnd,"请选择一个Tile!","提示",MB_OK|MB_ICONWARNING );
        return;
    }

    if( nLayer == -1 )
        nTextureID = pTerrain->GetTextureID( pSelected->GetMainTexture() );
    else
        nTextureID = pTerrain->GetTextureID( pSelected->GetDetailTexture(nLayer) );

    tile.nTexture = nTextureID;
    tile.fScale = (nLayer == -1) ? pSelected->GetMainScale() : pSelected->GetDetailScale(nLayer);

    // Open the edit dialog.
    nRes = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_TILETEXTURE,m_hWnd,Callback_TileTex,
        (LPARAM)&tile );

    if( nRes == 1 )
    {
        if( nLayer == -1 )
        {
            if( tile.nTexture == -1 )
            {
                pSelected->SetMainTexture( pTerrain->GetDefaultTexture() );
                pSelected->SetMainScale( tile.fScale );
            }
            else
            {
                pSelected->SetMainTexture( pTerrain->GetTexture(tile.nTexture) );
                pSelected->SetMainScale( tile.fScale );
            }
        }
        else
        {
            if( tile.nTexture == -1 )
                pSelected->SetDetailTexture( nLayer,NULL );
            else
            {
                pSelected->SetDetailTexture( nLayer,pTerrain->GetTexture(tile.nTexture) );
                pSelected->SetDetailScale( nLayer,tile.fScale );
            }
        }
    }

    // Show the new terrain tile info.
    ShowTileInfo();
}


//------------------------------------------------------------------------
// Apply the texture to the whole terrain.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Texture id to apply to the terrain.
//------------------------------------------------------------------------
void etInterface::ApplyTerrainTexture( UINT nTextureID )
{
    INT_PTR nRes;

    if( !etCoreManager::Instance().IsHaveProject() )
        return;

    nRes = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_APPLYTEXTURE,m_hWnd,
        Callback_ApplyAll,(LPARAM)nTextureID );
    if( nRes == 1 ) ShowTileInfo();
}


//------------------------------------------------------------------------
// Show the vector setting dialog.
//------------------------------------------------------------------------
bool etInterface::ShowVectorSetting( etInterface::sVector* pVector )
{
    INT_PTR nRes;

    nRes = DialogBoxParam( m_hInst,(LPCTSTR)IDD_DLG_SETVECTOR,m_hWnd,
        Callback_SetVector,(LPARAM)pVector );
    return nRes == 1;
}


//------------------------------------------------------------------------
// Show the error info for the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      HRESULT:        Error code when fail.
//------------------------------------------------------------------------
void etInterface::ShowErrorDialog( HRESULT hrErrorCode )
{
    switch( hrErrorCode )
    {
    case ET_TEXTUREEXISTED:
        MessageBox( m_hWnd,"纹理已存在！","提示",MB_OK|MB_ICONWARNING );
        break;
    case ET_TEXTURELOADFAIL:
        MessageBox( m_hWnd,"加载纹理失败，纹理宽高必须相同且为2的指数值！","错误",
            MB_OK|MB_ICONERROR );
        break;
    }
}