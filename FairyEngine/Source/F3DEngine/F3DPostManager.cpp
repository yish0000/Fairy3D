/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPostManager.cpp
 *  Desc:   ���ļ�ʵ����F3D����ĺ�����Ч��������
 *  Author: Yish
 *  Date:   2012/4/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DPostManager.h"


/** Get the singleton.
*/
F3DPostManager& F3DPostManager::GetInstance()
{
	static F3DPostManager obj;
	return obj;
}

/** ���캯��
*/
F3DPostManager::F3DPostManager()
{
}

/** ��������
*/
F3DPostManager::~F3DPostManager()
{
}