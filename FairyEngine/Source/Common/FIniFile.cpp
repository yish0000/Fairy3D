/*
 * ------------------------------------------------------------------------
 *  Name:   FIniFile.cpp
 *  Desc:   本文件用于实现配置文件的读取。
 *  Author: Yish
 *  Date:   2010/11/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FIniFile.h"
#include "FSysFileStream.h"
#include "FVFileSystem.h"
#include "FLogManager.h"
#include "FStrUtil.h"

static const AString SEPERATORSA = "\t:=";

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FIniFile
//  
///////////////////////////////////////////////////////////////////////////

/** 构造函数
*/
FIniFile::FIniFile(void)
{
}

/** 析构函数
*/
FIniFile::~FIniFile(void)
{
    SettingSections::iterator it;
    for( it=m_Sections.begin();it!=m_Sections.end();it++ )
        F_SAFE_DELETE( it->second );
}

/** 清除所有配置信息
*/
void FIniFile::ClearSettings(void)
{
	SettingSections::iterator it;
	for( it=m_Sections.begin();it!=m_Sections.end();it++ )
		F_SAFE_DELETE( it->second );

    m_Sections.clear();
}

/** 加载配置文件
@Param 配置文件的路径
*/
bool FIniFile::LoadFile( const char* filename )
{
	FVFile file;
	if( !file.Open(filename,FVFile::VFILE_OPENEXIST) )
	{
		FLOG_WARNINGF( "FIniFile::LoadFile, Load ini file(%s) failed!",filename );
		return false;
	}

	// 如果有BOM，则跳过
	if( file.HasUTF8BOM() )
		file.Seek(3, FVFile::START);

	return LoadFromVFile( &file );
}

/** 读取配置信息
@Param 虚拟文件指针
*/
bool FIniFile::LoadFromVFile( FVFile* pFile )
{
	SettingItem* pItems = NULL;

	// 清除旧的配置数据
	ClearSettings();

	AString line,itemName,itemVal;
	while( !pFile->Eof() )
	{
		pFile->GetLine( line );

		// 如果是注释，则忽略这一行
		if( line.length() > 0 && line.at(0) != '#' && line.at(0) != '@' )
		{
			// 如果该行被中括号括起来，则是一个段名
			if( line.at(0) == '[' && line.at(line.length()-1) == ']' )
			{
				AString sectionName = line.substr( 1,line.length()-2 );

				// 检查该段是否存在
				SettingSections::iterator itr = m_Sections.find( sectionName );
				if( itr == m_Sections.end() )
				{
					pItems = new SettingItem;
					m_Sections[sectionName] = pItems;
				}
				else
				{
					pItems = itr->second;
				}
			}
			else
			{
				size_t pos = line.find_first_of( SEPERATORSA );
				if( pos != AString::npos )
				{
					itemName = line.substr( 0,pos );
					size_t pos2 = line.find_first_not_of( SEPERATORSA,pos );
					itemVal = (pos2 == AString::npos) ? "" : line.substr( pos2 );

					// 消除不必要的空格
					AStringUtil::Trim( itemName );
					AStringUtil::Trim( itemVal );

					// 添加该项到当前段
					if( pItems )
                        pItems->insert( SettingItem::value_type(itemName,itemVal) );
				}
			}
		}
	}

	return true;
}

/** 获取所需要的配置项
@Param 配置项所在段的名字
@Param 配置项的名字
*/
const AString& FIniFile::GetSetting( const AString& section,const AString& key )
{
    SettingSections::iterator seci;
    seci = m_Sections.find( section );

    if( seci != m_Sections.end() )
    {
        SettingItem::iterator item;
        item = seci->second->find( key );

        if( item != seci->second->end() )
            return item->second;
        else
            return AStringUtil::BLANK;
    }

    return AStringUtil::BLANK;
}

/** 添加指定的字段
@Param 段的名字
@Param 配置项的名字
@Param 配置项的值
*/
void FIniFile::AddKey( const AString& section,const AString& key,const AString& val )
{
	(*m_Sections[section])[key] = val;
}

/** 保存配置信息
@Param 要保存的文件名
*/
void FIniFile::SaveFile( const char* filename )
{
	FVFile file;
	if( !file.Open(filename,FVFile::VFILE_CREATENEW|FVFile::VFILE_BINARY) )
	{
		FLOG_WARNINGF( "FIniFile::SaveFile(), Save the ini file(%s) failed!", filename );
		return;
	}

	// Write the BOM.
	FBYTE UTF8BOM[3] = { 0xEF, 0xBB, 0xBF };
	file.Write( UTF8BOM, 3 );

	SettingSections::iterator seci;
	for( seci=m_Sections.begin();seci!=m_Sections.end();seci++ )
	{
		file.WriteString( "[%s]\n",seci->first.c_str() );

		SettingItem::iterator it;
		for( it=seci->second->begin();it!=seci->second->end();it++ )
			file.WriteString( "%s=%s\n", it->first.c_str(), it->second.c_str() );
	}

	file.Close();
}