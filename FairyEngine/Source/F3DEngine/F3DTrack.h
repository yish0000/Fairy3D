/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTrack.h
 *  Desc:   ���ļ�������һ����������Ĺ켣�ࡣ
 *  Author: Yish
 *  Date:   2010/11/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_TRACK_H__
#define __F3D_TRACK_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


enum EInterpolateType
{
    ITT_LINEAR,     // ���Բ�ֵ(�ٶȿ�)
    ITT_SPLINE,     // ���߲�ֵ(�ٶ���������ʹ����ı仯ƽ��)
};

/** ������������һ�����ƶ�����Ĺ켣��
@remarks
@note
*/
class FAIRY_API F3DTrack : public FGeneralAlloc
{
protected:
    std::vector<F3DVector3> m_Verts;     // �ؼ������б�
    float m_fWholeTime;                 // �켣�������ʱ��
    EInterpolateType m_InterType;       // ��ֵ����

public:
    F3DTrack();
    F3DTrack( EInterpolateType type );
    ~F3DTrack();

    // ����/����켣�ļ�
    bool LoadTrackFile( const char* filename );
    void SaveTrackFile( const char* filename );

    void AddVertex( const F3DVector3& vert );
    void RemoveVertex( size_t nIndex );
    void Clear(void);

    size_t GetNumVerts(void) const { return m_Verts.size(); }
    const F3DVector3& GetVertex( size_t nIndex ) const;

    // ��ȥһ��ָ��ʱ����λ��
    F3DVector3 GetCurPos( float fTime );

    // ���ù켣�������ʱ��
    void SetWholeTime( float fWholeTime ) { m_fWholeTime = fWholeTime; }
    float GetWholeTime(void) const { return m_fWholeTime; }

    // ���ù켣�Ĳ�ֵ����
    EInterpolateType GetInterpolateType(void) const { return m_InterType; }
    void SetInterpolateType( EInterpolateType type ) { m_InterType = type; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_TRACK_H__