/*
 * -----------------------------------------------------------------------
 *  Name:   FlyShaderManager.cpp
 *  Desc:   ���ļ�Ϊ����ʵ����һ����ɫ����������
 *  Author: Yish
 *  Date:   2011/5/21
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FlyPrecompile.h"

#include "FlyShaderManager.h"


// ȫ��Ψһ��ʵ��
FlyShaderManager* FlyShaderManager::s_pInstance = NULL;

/** ���캯��
*/
FlyShaderManager::FlyShaderManager(void)
{
    s_pInstance = this;
}

/** ��������
*/
FlyShaderManager::~FlyShaderManager(void)
{
}