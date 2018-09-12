/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTrack.h
 *  Desc:   本文件定义了一个引擎所需的轨迹类。
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
    ITT_LINEAR,     // 线性插值(速度快)
    ITT_SPLINE,     // 曲线插值(速度慢，但会使方向的变化平滑)
};

/** 该类用来描述一个可移动物体的轨迹。
@remarks
@note
*/
class FAIRY_API F3DTrack : public FGeneralAlloc
{
protected:
    std::vector<F3DVector3> m_Verts;     // 关键顶点列表
    float m_fWholeTime;                 // 轨迹所需的总时间
    EInterpolateType m_InterType;       // 插值类型

public:
    F3DTrack();
    F3DTrack( EInterpolateType type );
    ~F3DTrack();

    // 加载/保存轨迹文件
    bool LoadTrackFile( const char* filename );
    void SaveTrackFile( const char* filename );

    void AddVertex( const F3DVector3& vert );
    void RemoveVertex( size_t nIndex );
    void Clear(void);

    size_t GetNumVerts(void) const { return m_Verts.size(); }
    const F3DVector3& GetVertex( size_t nIndex ) const;

    // 后去一个指定时间点的位置
    F3DVector3 GetCurPos( float fTime );

    // 设置轨迹所需的总时间
    void SetWholeTime( float fWholeTime ) { m_fWholeTime = fWholeTime; }
    float GetWholeTime(void) const { return m_fWholeTime; }

    // 设置轨迹的插值类型
    EInterpolateType GetInterpolateType(void) const { return m_InterType; }
    void SetInterpolateType( EInterpolateType type ) { m_InterType = type; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_TRACK_H__