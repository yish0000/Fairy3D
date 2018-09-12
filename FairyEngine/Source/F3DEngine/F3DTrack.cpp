/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTrack.cpp
 *  Desc:   ���ļ�������һ��������Ҫ�Ĺ켣�ࡣ
 *  Author: Yish
 *  Date:   2010/11/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DTrack.h"

#pragma F_COMPILE_WARN("�켣�����߲�ֵ��δ��д��")


///////////////////////////////////////////////////////////////////////////
// Macros and Local types

#define F3D_TRACK_VERSION 0x0001


///////////////////////////////////////////////////////////////////////////

/** ���캯��
*/
F3DTrack::F3DTrack(void) : m_InterType(ITT_LINEAR),m_fWholeTime(0.0f)
{
}

/** ���캯��
@Param �ù켣�Ĳ�ֵ����
*/
F3DTrack::F3DTrack( EInterpolateType type ) : m_InterType(type),m_fWholeTime(0.0f)
{
}

/** ��������
*/
F3DTrack::~F3DTrack(void)
{
}

/** �ӹ켣�ļ��м�������
@Param �켣�ļ���·��
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

/** ���켣���ݱ��浽�ļ�
@Param �켣�ļ���·��
*/
void F3DTrack::SaveTrackFile( const char* filename )
{
	FVFile file;
	if( !file.Open(filename, FVFile::VFILE_CREATENEW|FVFile::VFILE_BINARY) )
	{
		FLOG_WARNINGF("F3DTrack::SaveTrackFile(), Save track file (%s) failed!", filename);
		return;
	}

	// д��汾
	file << (uint32)F3D_TRACK_VERSION;

	// д�붥������
	file << (uint32)m_Verts.size();
	for( size_t i=0;i<m_Verts.size();i++ )
	{
		file << m_Verts[i];
	}

	file.Close();
}

/** ���һ�����㵽�켣��
@Param Ҫ��ӵĶ���
*/
void F3DTrack::AddVertex( const F3DVector3& vert )
{
    m_Verts.push_back( vert );
}

/** �ӹ켣���Ƴ�һ������
@Param Ҫ�Ƴ����������
*/
void F3DTrack::RemoveVertex( size_t nIndex )
{
    if( nIndex < m_Verts.size() )
    {
        m_Verts.erase( m_Verts.begin() + nIndex );
    }
}

/** �Ƴ��켣�е����ж���
*/
void F3DTrack::Clear(void)
{
    m_Verts.clear();
}

/** �ӹ켣�л�ȡ�ƶ��Ķ���
@Param ָ�����������
*/
const F3DVector3& F3DTrack::GetVertex( size_t nIndex ) const
{
    return (nIndex < m_Verts.size()) ? m_Verts[nIndex] : F3DVector3::ZERO;
}

/** ��ȡָ��ʱ���λ�ã����ݲ�ֵ��ʽ����
@Param ��ȡ�Ǹ�ʱ����λ�ã�
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