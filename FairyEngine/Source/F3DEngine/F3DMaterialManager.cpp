/*
 * -----------------------------------------------------------------------
 *	Name:   F3DMaterialManager.cpp
 *  Desc:   ���ļ�����ʵ������Ĳ��ʹ�������
 *  Author: Yish
 *  Date:   2010/8/29
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * -----------------------------------------------------------------------
 */


#include "F3DPCH.h"
#include "F3DMaterialManager.h"


/** Get the singleton
*/
F3DMaterialManager& F3DMaterialManager::GetInstance()
{
	static F3DMaterialManager obj;
	return obj;
}

// ���캯��
F3DMaterialManager::F3DMaterialManager(void)
{
}

// ��������
F3DMaterialManager::~F3DMaterialManager(void)
{
}