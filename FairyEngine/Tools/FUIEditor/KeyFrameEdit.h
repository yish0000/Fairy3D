#pragma once

enum KEYFRAME_TYPE
{
    KF_SCALE,
    KF_ALPHA,
    KF_COLOR,
    KF_ANGLE,
    KF_SHAKE,
    KF_MOVE,
};

// Keyframe Data
template <class T>
struct KEYFRAME
{
    float fStart;
    float fEnd;
    T data1,data2;
    CPoint pt1;
    CPoint pt2;
};

typedef std::vector<KEYFRAME<float> > KEYFRAME_LIST_F;
typedef std::vector<KEYFRAME<DWORD> > KEYFRAME_LIST_D;

// CKeyFrameEdit 对话框

class CKeyFrameEdit : public CDialog
{
	DECLARE_DYNAMIC(CKeyFrameEdit)

public:
	CKeyFrameEdit(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKeyFrameEdit();

    // 初始化对话框
    bool Init( KEYFRAME_TYPE Type );

    // 添加一个关键帧
    bool AddKeyframe( float fStart,float fEnd );
    void DelKeyframe();

// 对话框数据
	enum { IDD = IDD_DLG_KEYFRAME };

    CString m_sTitle;
    KEYFRAME_TYPE m_Type;
    KEYFRAME_LIST_F m_FloatKF;
    float m_fMinVal,m_fMaxVal;
    UINT m_iCurSelected;     // 0x80000000 | index

protected:
    CPen m_PenR,m_PenG,m_PenB;
    CPen m_PenBlack;
    CRect m_rcContent;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnClose();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

    template <class T>
    void DrawKeyframe( CPaintDC* pDC,KEYFRAME<T>* pKF,int iIndex );
};
