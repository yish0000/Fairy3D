/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRenderWindow.cpp
 *  Desc:   本文件用于实现引擎的渲染窗口基类。
 *  Author: Yish
 *  Date:   2010/12/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DRenderWindow.h"
#include "F3DRenderDevice.h"


/** 构造函数
@Param 渲染窗口的名字
@Param 是否为主渲染窗口
*/
F3DRenderWindow::F3DRenderWindow( const AString& name,bool bMainWnd )
    : F3DRenderTarget(name),m_bMainWnd(bMainWnd),m_bFullScreen(false)
{
    m_nPriority = TARGET_PRIORITY_RENDERWINDOW;
}

/** 析构函数
*/
F3DRenderWindow::~F3DRenderWindow(void)
{
}

/** 根据鼠标在窗口上的位置获取拾取射线
@Param 鼠标在渲染窗口上的相对坐标
@Param 输出拾取射线
*/
void F3DRenderWindow::GetPickRay( const FPointI& cursorPt,F3DRay& pickRay,int depthOfVP ) const
{
//     F3DVector vS;
//     F3DMatrix invView;

//     // 获取当前的投影矩阵
//     F3DMatrix mProj = m_pDevice->GetProjectionMatrix();
// 
//     vS.x = (((2.0f * cursorPt.x)/m_nWidth)-1) / mProj._11;
//     vS.y = -(((2.0f * cursorPt.y)/m_nHeight)-1) / mProj._22;
//     vS.z = 1.0f;
// 
//     // 获取当前相机矩阵的逆阵
//     invView.InverseOf( m_pDevice->GetViewMatrix() );
// 
//     pickRay.m_vDir.x = vS.x*invView._11 + vS.y*invView._21 + vS.z*invView._31;
//     pickRay.m_vDir.y = vS.x*invView._12 + vS.y*invView._22 + vS.z*invView._32;
//     pickRay.m_vDir.z = vS.x*invView._13 + vS.y*invView._23 + vS.z*invView._33;
//     pickRay.m_vOrig.x = invView._41;
//     pickRay.m_vOrig.y = invView._42;
//     pickRay.m_vOrig.z = invView._43;
}

/** 由世界坐标获取其投影在窗口上的二维坐标
@Param 指定三维世界内的点坐标
*/
F2DPOINT F3DRenderWindow::WorldToScreen( const F3DVector3& vPos3D, int depthOfVP ) const
{
    FPointI pt;
    float fClip_x,fClip_y;
    float fXp,fYp,fWp;
    F3DMatrix4 viewProj;

    // 计算相机矩阵和投影矩阵的乘绩
    //viewProj = m_pDevice->GetViewMatrix() * m_pDevice->GetProjectionMatrix();

    fClip_x = (float)(m_nWidth >> 1);
    fClip_y = (float)(m_nHeight >> 1);

    fXp = (viewProj._11*vPos3D.x) + (viewProj._21*vPos3D.y) +
          (viewProj._31*vPos3D.z) + viewProj._41;
    fYp = (viewProj._12*vPos3D.x) + (viewProj._22*vPos3D.y) +
          (viewProj._32*vPos3D.z) + viewProj._42;
    fWp = (viewProj._14*vPos3D.x) + (viewProj._24*vPos3D.y) +
          (viewProj._34*vPos3D.z) + viewProj._44;

    float fWpInv = 1.0f / fWp;

    pt.x = (long)( (1.0f+fXp*fWpInv)*fClip_x );
    pt.y = (long)( (1.0f+fYp*fWpInv)*fClip_y );

    return pt;
}