//------------------------------------------------------------------------
// Name: uiMacros.h
// Desc: This file define the macros for the GUI elements and the toolBar.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __UI_MACROS_H__
#define __UI_MACROS_H__

// ID for the GUI elements
#define IDC_WORKSPACE           0       // 工作区(框架)
#define IDC_SWITCH_TERRAIN      1       // 地形编辑(单选框)
#define IDC_SWITCH_TEXTURE      2       // 贴图编辑(单选框)
#define IDC_SWITCH_ENTITY       3       // 实体编辑(单选框)

// 1. 地形编辑面板
#define IDC_HP_TOOL             4       // 工具(框架)
#define IDC_HP_TOOLTYPE         5       // 工具类型(下拉框)
#define IDC_HP_OUTRADIUS        6       // 外半径(滚动条)
#define IDC_HP_INNERRADIUS      7       // 内半径(滚动条)
#define IDC_HP_STRENGTH         8       // 强度(滚动条)
#define IDC_HP_LBLORADIUS       9       // 外半径(文本)
#define IDC_HP_LBLIRADIUS       10      // 内半径(文本)
#define IDC_HP_LBLSTRENGTH      11      // 强度(文本)
#define IDC_HP_LBLSHAPE         12      // 刷子形状(文本)
#define IDC_HP_CIRCLE           13      // 圆形(单选框)
#define IDC_HP_RECTANGLE        14      // 方形(单选框)
#define IDC_HP_DETAILLAYER      15      // 细节层编辑(框架)
#define IDC_HP_LAYER1           16      // 细节一(单选框)
#define IDC_HP_LAYER2           17      // 细节二(单选框)
#define IDC_HP_LAYER3           18      // 细节三(单选框)
#define IDC_HP_LAYER4           19      // 细节四(单选框)


// 2. 纹理编辑面板
#define IDC_TP_TEXGROUP         20      // 纹理列表(框架)
#define IDC_TP_TEXTURELIST      21      // 纹理列表(列表框)
#define IDC_TP_ADDTEX           22      // 添加纹理(按钮)
#define IDC_TP_DELTEX           23      // 删除纹理(按钮)
#define IDC_TP_PREVIEW          24      // 预览纹理(按钮)
#define IDC_TP_APPLYALL         25      // 应用到整个地形(按钮)

#define IDC_TP_TILEGROUP        26      // Tile信息(框架)
#define IDC_TP_TXTMAINTEX       27      // 主纹理(标签)
#define IDC_TP_BTNMAINTEX       28      // 主纹理(按钮)
#define IDC_TP_TXTDETAIL1       29      // 细节纹理一(标签)
#define IDC_TP_BTNDETAIL1       30      // 细节纹理一(按钮)
#define IDC_TP_TXTDETAIL2       31      // 细节纹理二(标签)
#define IDC_TP_BTNDETAIL2       32      // 细节纹理二(按钮)
#define IDC_TP_TXTDETAIL3       33      // 细节纹理三(标签)
#define IDC_TP_BTNDETAIL3       34      // 细节纹理三(按钮)
#define IDC_TP_TXTDETAIL4       35      // 细节纹理四(标签)
#define IDC_TP_BTNDETAIL4       36      // 细节纹理四(按钮)


// 3. 演员编辑面板
#define IDC_AP_ENTITYGROUP      37      // 可用实体模板(框架)
#define IDC_AP_ENTITYLIST       38      // 实体模板列表(树形结构)
#define IDC_AP_CURRENTTYPE      39      // 当前类型(文本)
#define IDC_AP_CURRENTENTITY    40      // 当前实体(文本)

#define IDC_AP_ENTITYATTRIB     41      // 实体对象属性(框架)
#define IDC_AP_ENTITYNAME       42      // 实体名字(标签)
#define IDC_AP_ENTITYNAMEBTN    43      // 实体名字(按钮)
#define IDC_AP_ENTITYTYPE       44      // 实体类型(文本)
#define IDC_AP_ENTITYTYPELBL    45      // 实体类型(标签)
#define IDC_AP_ENTITYTEM        46      // 实体模板(文本)
#define IDC_AP_ENTITYTEMLBL     47      // 实体模板(标签)
#define IDC_AP_ENTITYPOS        48      // 实体位置(文本)
#define IDC_AP_ENTITYPOSBTN     49      // 实体位置(按钮)
#define IDC_AP_ENTITYROT        50      // 实体旋转(文本)
#define IDC_AP_ENTITYROTBTN     51      // 实体旋转(按钮)
#define IDC_AP_ENTITYSCALE      52      // 实体缩放(文本)
#define IDC_AP_ENTITYSCALEBTN   53      // 实体缩放(按钮)


// 工具条
#define IDT_NEWSCENE        701     // 新建项目
#define IDT_OPENSCENE       702     // 打开项目
#define IDT_SAVESCENE       703     // 保存项目

#define IDT_SELOBJECT       704     // 选取物体
#define IDT_MAPINFO         705     // 地图属性
#define IDT_LIGHTMODE       706     // 地图光照

#define IDT_SELECT          707     // 选择物体
#define IDT_MOVEXZ          708     // 移动物体(X-Z平面)
#define IDT_MOVEY           709     // 移动物体(Y轴)
#define IDT_ROTATEX         710     // 旋转物体(X轴)
#define IDT_ROTATEY         711     // 旋转物体(Y轴)
#define IDT_ROTATEZ         712     // 旋转物体(Z轴)
#define IDT_SCALE           713     // 缩放物体(XYZ轴)

#define IDT_ABOUT           714     // 关于...

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __UI_MACROS_H__