/*
 * ------------------------------------------------------------------------
 *  Name:   F3DSkinModel.h
 *  Desc:   ���ļ�������һ���������Ƥ����ģ���ࡣ
 *  Author: Yish
 *  Date:   2013/1/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_SKINMODEL_H__
#define __F3D_SKINMODEL_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


class F3DSkinMesh;
class F3DSkeleton;

/** ��Ƥ����ģ��
*/
class FAIRY_API F3DSkinModel : public FGeneralAlloc
{
	typedef std::vector<F3DSkinMesh*> SkinMeshList;

public:
	F3DSkinModel();
	virtual ~F3DSkinModel() {}

	bool AddMesh( F3DSkinMesh* pMesh );
	bool AddMeshFile( const char* filename );
	void RemoveMesh( size_t iIndex );
	void RemoveAllMeshes();

protected:
    F3DSkeleton* m_pSkeleton;       // �Ǽ�ϵͳ
	SkinMeshList m_SkinMeshes;		// ��Ƥ�����б�
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_SKINMODEL_H__