/*
 * ------------------------------------------------------------------------
 *  Name:   F3DConfig.cpp
 *  Desc:   ���ļ�������һ���洢����ȫ�����õ�һ���ࡣ
 *  Author: Yish
 *  Date:   2012/12/22
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DConfig.h"

/** ��ȡȫ��ʵ��
*/
F3DConfig& F3DConfig::GetInstance()
{
	static F3DConfig obj;
	return obj;
}

bool F3DConfig::LoadConfigFile(const char* filename)
{
	FIniFile iniFile;
	if (!iniFile.LoadFile(filename))
	{
		FLOG_WARNINGF("F3DConfig::LoadConfigFile, failed to load the config file! (%s)", filename);
		return false;
	}

	return true;
}

void F3DConfig::SaveConfigFile(const char* filename)
{
}