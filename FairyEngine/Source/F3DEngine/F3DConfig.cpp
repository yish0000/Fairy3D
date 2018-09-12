/*
 * ------------------------------------------------------------------------
 *  Name:   F3DConfig.cpp
 *  Desc:   本文件定义了一个存储引擎全局配置的一个类。
 *  Author: Yish
 *  Date:   2012/12/22
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DConfig.h"

/** 获取全局实例
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