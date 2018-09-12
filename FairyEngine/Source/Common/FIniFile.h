/*
 * ------------------------------------------------------------------------
 *  Name:   FIniFile.h
 *  Desc:   本文件用于加载INI配置文件，读取配置信息。
 *  Author: Yish
 *  Date:   2010/11/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_INIFILE_H__
#define __FAIRY_INIFILE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"
#include "FVFile.h"

///////////////////////////////////////////////////////////////////////////

/** 配置文件加载
@remarks
	用于加载INI配置文件，\t,:,=为分割符。例子：
	[display]
	driver=d3d9.dll
	resolution:800x600
	quality	high
@note
*/
class FIniFile : public FGeneralAlloc
{
    typedef std::map<AString, AString> SettingItem;
    typedef std::map<AString, SettingItem*> SettingSections;

protected:
    SettingSections m_Sections;

public:
    FIniFile(void);
    virtual ~FIniFile(void);

	// 加载配置文件
	bool LoadFile(const char* filename);

	// 保存配置文件
	void SaveFile(const char* filename);

	// 添加一个指定的字段
	void AddKey(const AString& section, const AString& key, const AString& val);

	// 清除所有配置信息
	void ClearSettings(void);

	// 获取指定的项
	const AString& GetSetting(const AString& section, const AString& key);

protected:
    // 从文件流中提取信息
    bool LoadFromVFile(FVFile* pFile);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_INIFILE_H__