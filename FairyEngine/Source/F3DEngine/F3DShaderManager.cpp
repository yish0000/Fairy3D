/*
 * ------------------------------------------------------------------------
 *  Name:   F3DShaderManager.cpp
 *  Desc:   ���ļ�Ϊ����ʵ����һ����ɫ����������
 *  Author: Yish
 *  Date:   2011/5/21
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DShaderManager.h"


/** Get the singleton.
*/
F3DShaderManager& F3DShaderManager::GetInstance()
{
	static F3DShaderManager obj;
	return obj;
}

/** ���캯��
*/
F3DShaderManager::F3DShaderManager(void)
{
}

/** ��������
*/
F3DShaderManager::~F3DShaderManager(void)
{
}