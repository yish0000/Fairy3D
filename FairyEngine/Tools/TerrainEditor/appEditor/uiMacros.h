//------------------------------------------------------------------------
// Name: uiMacros.h
// Desc: This file define the macros for the GUI elements and the toolBar.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __UI_MACROS_H__
#define __UI_MACROS_H__

// ID for the GUI elements
#define IDC_WORKSPACE           0       // ������(���)
#define IDC_SWITCH_TERRAIN      1       // ���α༭(��ѡ��)
#define IDC_SWITCH_TEXTURE      2       // ��ͼ�༭(��ѡ��)
#define IDC_SWITCH_ENTITY       3       // ʵ��༭(��ѡ��)

// 1. ���α༭���
#define IDC_HP_TOOL             4       // ����(���)
#define IDC_HP_TOOLTYPE         5       // ��������(������)
#define IDC_HP_OUTRADIUS        6       // ��뾶(������)
#define IDC_HP_INNERRADIUS      7       // �ڰ뾶(������)
#define IDC_HP_STRENGTH         8       // ǿ��(������)
#define IDC_HP_LBLORADIUS       9       // ��뾶(�ı�)
#define IDC_HP_LBLIRADIUS       10      // �ڰ뾶(�ı�)
#define IDC_HP_LBLSTRENGTH      11      // ǿ��(�ı�)
#define IDC_HP_LBLSHAPE         12      // ˢ����״(�ı�)
#define IDC_HP_CIRCLE           13      // Բ��(��ѡ��)
#define IDC_HP_RECTANGLE        14      // ����(��ѡ��)
#define IDC_HP_DETAILLAYER      15      // ϸ�ڲ�༭(���)
#define IDC_HP_LAYER1           16      // ϸ��һ(��ѡ��)
#define IDC_HP_LAYER2           17      // ϸ�ڶ�(��ѡ��)
#define IDC_HP_LAYER3           18      // ϸ����(��ѡ��)
#define IDC_HP_LAYER4           19      // ϸ����(��ѡ��)


// 2. ����༭���
#define IDC_TP_TEXGROUP         20      // �����б�(���)
#define IDC_TP_TEXTURELIST      21      // �����б�(�б��)
#define IDC_TP_ADDTEX           22      // �������(��ť)
#define IDC_TP_DELTEX           23      // ɾ������(��ť)
#define IDC_TP_PREVIEW          24      // Ԥ������(��ť)
#define IDC_TP_APPLYALL         25      // Ӧ�õ���������(��ť)

#define IDC_TP_TILEGROUP        26      // Tile��Ϣ(���)
#define IDC_TP_TXTMAINTEX       27      // ������(��ǩ)
#define IDC_TP_BTNMAINTEX       28      // ������(��ť)
#define IDC_TP_TXTDETAIL1       29      // ϸ������һ(��ǩ)
#define IDC_TP_BTNDETAIL1       30      // ϸ������һ(��ť)
#define IDC_TP_TXTDETAIL2       31      // ϸ�������(��ǩ)
#define IDC_TP_BTNDETAIL2       32      // ϸ�������(��ť)
#define IDC_TP_TXTDETAIL3       33      // ϸ��������(��ǩ)
#define IDC_TP_BTNDETAIL3       34      // ϸ��������(��ť)
#define IDC_TP_TXTDETAIL4       35      // ϸ��������(��ǩ)
#define IDC_TP_BTNDETAIL4       36      // ϸ��������(��ť)


// 3. ��Ա�༭���
#define IDC_AP_ENTITYGROUP      37      // ����ʵ��ģ��(���)
#define IDC_AP_ENTITYLIST       38      // ʵ��ģ���б�(���νṹ)
#define IDC_AP_CURRENTTYPE      39      // ��ǰ����(�ı�)
#define IDC_AP_CURRENTENTITY    40      // ��ǰʵ��(�ı�)

#define IDC_AP_ENTITYATTRIB     41      // ʵ���������(���)
#define IDC_AP_ENTITYNAME       42      // ʵ������(��ǩ)
#define IDC_AP_ENTITYNAMEBTN    43      // ʵ������(��ť)
#define IDC_AP_ENTITYTYPE       44      // ʵ������(�ı�)
#define IDC_AP_ENTITYTYPELBL    45      // ʵ������(��ǩ)
#define IDC_AP_ENTITYTEM        46      // ʵ��ģ��(�ı�)
#define IDC_AP_ENTITYTEMLBL     47      // ʵ��ģ��(��ǩ)
#define IDC_AP_ENTITYPOS        48      // ʵ��λ��(�ı�)
#define IDC_AP_ENTITYPOSBTN     49      // ʵ��λ��(��ť)
#define IDC_AP_ENTITYROT        50      // ʵ����ת(�ı�)
#define IDC_AP_ENTITYROTBTN     51      // ʵ����ת(��ť)
#define IDC_AP_ENTITYSCALE      52      // ʵ������(�ı�)
#define IDC_AP_ENTITYSCALEBTN   53      // ʵ������(��ť)


// ������
#define IDT_NEWSCENE        701     // �½���Ŀ
#define IDT_OPENSCENE       702     // ����Ŀ
#define IDT_SAVESCENE       703     // ������Ŀ

#define IDT_SELOBJECT       704     // ѡȡ����
#define IDT_MAPINFO         705     // ��ͼ����
#define IDT_LIGHTMODE       706     // ��ͼ����

#define IDT_SELECT          707     // ѡ������
#define IDT_MOVEXZ          708     // �ƶ�����(X-Zƽ��)
#define IDT_MOVEY           709     // �ƶ�����(Y��)
#define IDT_ROTATEX         710     // ��ת����(X��)
#define IDT_ROTATEY         711     // ��ת����(Y��)
#define IDT_ROTATEZ         712     // ��ת����(Z��)
#define IDT_SCALE           713     // ��������(XYZ��)

#define IDT_ABOUT           714     // ����...

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __UI_MACROS_H__