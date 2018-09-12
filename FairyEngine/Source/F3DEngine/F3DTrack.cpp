/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTrack.cpp
 *  Desc:   本文件定义了一个引擎需要的轨迹类。
 *  Author: Yish
 *  Date:   2010/11/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DTrack.h"

#pragma F_COMPILE_WARN("轨迹的曲线插值尚未编写！")


///////////////////////////////////////////////////////////////////////////
// Macros and Local types

#define F3D_TRACK_VERSION 0x0001


///////////////////////////////////////////////////////////////////////////

/** 构造函数
*/
F3DTrack::F3DTrack(void) : m_InterType(ITT_LINEAR),m_fWholeTime(0.0f)
{
}

/** 构造函数
@Param 该轨迹的插值类型
*/
F3DTrack::F3DTrack( EInterpolateType type ) : m_InterType(type),m_fWholeTime(0.0f)
{
}

/** 析构函数
*/
F3DTrack::~F3DTrack(void)
{
}

/** 从轨迹文件中加载数据
@Param 轨迹文件的路径
*/
bool F3DTrack::LoadTrackFile( const char* filename )
{
    FVFile file;
    if( !file.Open(filename, FVFile::VFILE_OPENEXIST) )
    {
        FLOG_WARNINGF("F3DTrack::LoadTrackFile(), Load track file (%s) failed!", filename);
        return false;
    }

	uint32 nVersion;
	file >> nVersion;
	if( nVersion > F3D_TRACK_VERSION )
	{
		FLOG_WARNINGF("F3DTrack::LoadTrackFile(), Invalid track file version (%s)!", filename);
		return false;
	}

	F3DVector3 vPt;
	uint32 nCount = 0;
	file >> nCount;
	for( uint32 i=0; i<nCount; i++ )
	{
		file >> vPt;
		m_Verts.push_back(vPt);
	}

	file.Close();
    return true;
}

/** 将轨迹数据保存到文件
@Param 轨迹文件的路径
*/
void F3DTrack::SaveTrackFile( const char* filename )
{
	FVFile file;
	if( !file.Open(filename, FVFile::VFILE_CREATENEW|FVFile::VFILE_BINARY) )
	{
		FLOG_WARNINGF("F3DTrack::SaveTrackFile(), Save track file (%s) failed!", filename);
		return;
	}

	// 写入版本
	file << (uint32)F3D_TRACK_VERSION;

	// 写入顶点数据
	file << (uint32)m_Verts.size();
	for( size_t i=0;i<m_Verts.size();i++ )
	{
		file << m_Verts[i];
	}

	file.Close();
}

/** 添加一个顶点到轨迹中
@Param 要添加的顶点
*/
void F3DTrack::AddVertex( const F3DVector3& vert )
{
    m_Verts.push_back( vert );
}

/** 从轨迹中移除一个顶点
@Param 要移除顶点的索引
*/
void F3DTrack::RemoveVertex( size_t nIndex )
{
    if( nIndex < m_Verts.size() )
    {
        m_Verts.erase( m_Verts.begin() + nIndex );
    }
}

/** 移除轨迹中的所有顶点
*/
void F3DTrack::Clear(void)
{
    m_Verts.clear();
}

/** 从轨迹中获取制定的顶点
@Param 指定顶点的索引
*/
const F3DVector3& F3DTrack::GetVertex( size_t nIndex ) const
{
    return (nIndex < m_Verts.size()) ? m_Verts[nIndex] : F3DVector3::ZERO;
}

/** 获取指定时间的位置，根据插值方式计算
@Param 获取那个时间点的位置？
*/
F3DVector3 F3DTrack::GetCurPos( float fTime )
{
    if( fTime < 0.0f || fTime > m_fWholeTime )
        return F3DVector3::ZERO;

    float fCur = (fTime / m_fWholeTime) * (m_Verts.size()-1);
    int nStart = (int)fCur;
    float fOffset = fCur - nStart;

    if( nStart == m_Verts.size() - 1 )
        return m_Verts[nStart];
    else
    {
        F3DVector3 vStart = m_Verts[nStart];
        F3DVector3 vDir = m_Verts[nStart+1] - vStart;

        return vStart + vDir * fOffset;
    }
}