// KeyFrameEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "FUIEditor.h"
#include "KeyFrameEdit.h"


const float MY_PI = 3.14159265f;

const int GRID_WIDTH = 20;
const int GRID_HEIGHT = 10;
const int VIEW_WIDTH = 400;
const int VIEW_HEIGHT = 200;

// CKeyFrameEdit 对话框

IMPLEMENT_DYNAMIC(CKeyFrameEdit, CDialog)

CKeyFrameEdit::CKeyFrameEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyFrameEdit::IDD, pParent)
{
    m_rcContent.SetRect( 20,20,420,220 );
    m_PenR.CreatePen( PS_SOLID,1,RGB(255,0,0) );
    m_PenG.CreatePen( PS_SOLID,1,RGB(255,0,0) );
    m_PenB.CreatePen( PS_SOLID,1,RGB(255,0,0) );
    m_PenBlack.CreatePen( PS_SOLID,1,RGB(0,0,0) );

    Init( KF_ALPHA );
    KEYFRAME<float> kf;
    kf.fStart = 0.5f;
    kf.fEnd = 0.7f;
    kf.data1 = 0.4f;
    kf.data2 = 1.0f;
    m_FloatKF.push_back( kf );
}

CKeyFrameEdit::~CKeyFrameEdit()
{
}

void CKeyFrameEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CKeyFrameEdit, CDialog)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_PAINT()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_CLOSE()
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CKeyFrameEdit 消息处理程序

void CKeyFrameEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialog::OnLButtonDown(nFlags, point);
}

void CKeyFrameEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialog::OnLButtonUp(nFlags, point);
}

void CKeyFrameEdit::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialog::OnMouseMove(nFlags, point);
}

void CKeyFrameEdit::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码
    // 不为绘图消息调用 CDialog::OnPaint()

    PAINTSTRUCT ps;
    BeginPaint( &ps );

    dc.SelectObject( &m_PenBlack );

    // 绘制坐标系
    for( int j=0;j<21;++j )
    {
        dc.MoveTo( m_rcContent.left,m_rcContent.top+j*GRID_HEIGHT );
        dc.LineTo( m_rcContent.right,m_rcContent.top+j*GRID_HEIGHT );

        for( int i=0;i<21;++i )
        {
            dc.MoveTo( m_rcContent.left+GRID_WIDTH*i,m_rcContent.top );
            dc.LineTo( m_rcContent.left+GRID_WIDTH*i,m_rcContent.bottom );
        }
    }

    // 绘制关键帧
    if( m_Type == KF_ALPHA )
    {
        for( size_t i=0;i<m_FloatKF.size();++i )
            DrawKeyframe( &dc,&m_FloatKF[i],(int)i );
    }

    EndPaint( &ps );
    CDialog::OnPaint();
}

template <class T>
void CKeyFrameEdit::DrawKeyframe( CPaintDC* pDC,KEYFRAME<T>* pKF,int iIndex )
{
    CPoint pt;
    pt.x = (long)((pKF->fStart / 1.0f) * VIEW_WIDTH);
    pt.y = (long)(((float)pKF->data1-m_fMinVal) / (m_fMaxVal-m_fMinVal) * VIEW_HEIGHT);
    pKF->pt1.x = pt.x + 20;
    pKF->pt1.y = VIEW_HEIGHT - pt.y + 20;
    pt.x = (long)((pKF->fEnd / 1.0f) * VIEW_WIDTH);
    pt.y = (long)(((float)pKF->data2-m_fMinVal) / (m_fMaxVal-m_fMinVal) * VIEW_HEIGHT);
    pKF->pt2.x = pt.x + 20;
    pKF->pt2.y = VIEW_HEIGHT - pt.y + 20;

    pDC->SelectObject( &m_PenR );
    pDC->MoveTo( pKF->pt1 );
    pDC->LineTo( pKF->pt2 );

    int iCurIndex = m_iCurSelected & ~0x80000000;
    if( iCurIndex == iIndex && !(m_iCurSelected & 0x80000000) )
        pDC->FillSolidRect( pKF->pt1.x-4,pKF->pt1.y-4,8,8,RGB(255,255,255) );
    else
        pDC->FillSolidRect( pKF->pt1.x-4,pKF->pt1.y-4,8,8,RGB(255,0,0) );
    if( iCurIndex == iIndex && (m_iCurSelected & 0x80000000) )
        pDC->FillSolidRect( pKF->pt2.x-4,pKF->pt2.y-4,8,8,RGB(255,255,255) );
    else
        pDC->FillSolidRect( pKF->pt2.x-4,pKF->pt2.y-4,8,8,RGB(255,0,0) );
}

void CKeyFrameEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialog::OnLButtonDblClk(nFlags, point);
}

void CKeyFrameEdit::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    // 保存更改的数据

    CDialog::OnClose();
}

void CKeyFrameEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if( nChar == VK_DELETE )
    {
        if( MessageBox(_T("要删除该关键帧吗？"),_T("提示"),MB_YESNO|MB_ICONQUESTION)
            == IDYES )
        {
            // 删除关键帧
        }
    }

    CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

bool CKeyFrameEdit::Init( KEYFRAME_TYPE Type )
{
    if( Type == KF_ANGLE )
    {
        m_fMinVal = -MY_PI*3;
        m_fMaxVal =  MY_PI*3;
    }
    else if( Type == KF_ALPHA )
    {
        m_fMinVal = 0.0f;
        m_fMaxVal = 1.0f;
    }
    else if( Type == KF_SCALE )
    {
        m_fMinVal = 0.0f;
        m_fMaxVal = 5.0f;
    }
    else if( Type == KF_COLOR )
    {
        m_fMinVal = 0.0f;
        m_fMaxVal = 255.0f;
    }

    m_Type = Type;
    return true;
}
