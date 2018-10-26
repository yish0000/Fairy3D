/*
 * ------------------------------------------------------------------------
 *  Name:   FStrUtil.cpp
 *  Desc:   本文件实现了一些有关字符串的实用函数。
 *  Author: Yish
 *  Date:   2010/11/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FStrUtil.h"
#include "FLogManager.h"
#include "FTempMemBuffer.h"
#include <locale.h>

#if FAIRY_PLATFORM != FAIRY_PLATFORM_WINDOWS
    #include <iconv.h>
#endif

///////////////////////////////////////////////////////////////////////////
//  
//  Define some macros
//  
///////////////////////////////////////////////////////////////////////////

#define MAX_STRING_BUF_SIZE 4096

///////////////////////////////////////////////////////////////////////////
//  
//  Local variables
//  
///////////////////////////////////////////////////////////////////////////

static uint32 l_uniqueNameID = 0;

///////////////////////////////////////////////////////////////////////////
//  
//  String encoding conversion
//  
///////////////////////////////////////////////////////////////////////////

#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS

// 获取当前代码页
uint32 F_GetCurCodePage()
{
	return GetACP();
}

// 将ANSI字符串转换为UNICODE
WString F_ANSIToUnicodeCP( const char* str, uint32 iCodePage )
{
    WString ret;
    if( !str || !strlen(str) ) return ret;
    
    int strLen = (int)strlen(str);
    int iLen = MultiByteToWideChar( iCodePage, 0, str, strLen, NULL, 0 );
    FTempWStringBuffer buffer((iLen+1) * sizeof(wchar_t));
    MultiByteToWideChar( iCodePage, 0, str, strLen, buffer.GetBuffer(), iLen );
    buffer[iLen] = L'\0';
    ret = buffer.GetBuffer();
    
    return ret;
}

// 将UNICODE字符串转换为默认代码页的字符串
AString F_UnicodeToANSICP( const wchar_t* str, uint32 iCodePage )
{
    AString ret;
    if( !str || !wcslen(str) ) return ret;
    
    int strLen = (int)wcslen(str);
    int iLen = WideCharToMultiByte( iCodePage, 0, str, strLen, NULL, NULL, NULL, NULL );
    FTempStringBuffer buffer(iLen+1);
    WideCharToMultiByte( iCodePage, 0, str, strLen, buffer.GetBuffer(), iLen, NULL, NULL );
    buffer[iLen] = '\0';
    ret = buffer.GetBuffer();
    
    return ret;
}

// 将ANSI字符串转换为UNICODE(UTF-16)
AString F_ANSIToUTF8CP(const char* str, uint32 iCodePage)
{
    AString ret;
    if( !str || !strlen(str) ) return ret;
    if( iCodePage == CP_UTF8 ) return str;
    
    int strLen = (int)strlen(str);
    int iLen = MultiByteToWideChar( iCodePage, 0, str, strLen, NULL, 0 );
    FTempWStringBuffer buffer((iLen+1) * sizeof(wchar_t));
    MultiByteToWideChar(iCodePage, 0, str, strLen, buffer.GetBuffer(), iLen);
    buffer[iLen] = L'\0';
    
    int mbLen = WideCharToMultiByte(CP_UTF8, 0, buffer.GetBuffer(), iLen, NULL, 0, NULL, NULL );
    ret.resize(mbLen);
    int cbLen = WideCharToMultiByte(CP_UTF8, 0, buffer.GetBuffer(), iLen, &(*ret.begin()), mbLen, NULL, NULL );
    if( cbLen != mbLen ) ret.clear();
    
    return ret;
}

// 将UNICODE字符串转换为ANSI
AString F_UTF8ToANSICP(const char* str, uint32 iCodePage)
{
    AString ret;
    if( !str || !strlen(str) ) return ret;
    if( iCodePage == CP_UTF8 ) return str;
    
    int strLen = (int)strlen(str);
    int iLen = MultiByteToWideChar(CP_UTF8, 0, str, strLen, NULL, 0);
    FTempWStringBuffer buffer((iLen+1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, str, strLen, buffer.GetBuffer(), iLen);
    buffer[iLen] = L'\0';
    
    int mbLen = WideCharToMultiByte(iCodePage, 0, buffer.GetBuffer(), iLen, NULL, 0, NULL, NULL );
    ret.resize(mbLen);
    int cbLen = WideCharToMultiByte(iCodePage, 0, buffer.GetBuffer(), iLen, &(*ret.begin()), mbLen, NULL, NULL );
    if( cbLen != mbLen ) ret.clear();
    
    return ret;
}

WString F_ANSIToUnicode(const char* str)
{
	return F_ANSIToUnicodeCP(str, F_GetCurCodePage());
}

AString F_UnicodeToANSI(const wchar_t* str)
{
	return F_UnicodeToANSICP(str, F_GetCurCodePage());
}

WString F_GB2312ToUnicode(const char* str)
{
	return F_ANSIToUnicodeCP(str, 936);
}

AString F_UnicodeToGB2312(const wchar_t* str)
{
	return F_UnicodeToANSICP(str, 936);
}

WString F_UTF8ToUnicode(const char* str)
{
	return F_ANSIToUnicodeCP(str, CP_UTF8);
}

AString F_UnicodeToUTF8(const wchar_t* str)
{
	return F_UnicodeToANSICP(str, CP_UTF8);
}

#endif

AString F_ANSIToUTF8(const char* str)
{
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	return F_ANSIToUTF8CP(str, F_GetCurCodePage());
#else
    return str;
#endif
}

AString F_UTF8ToANSI(const char* str)
{
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	return F_UTF8ToANSICP(str, F_GetCurCodePage());
#else
    return str;
#endif
}

AString F_GB2312ToUTF8(const char* str)
{
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	return F_ANSIToUTF8CP(str, 936);
#else
    AString ret;
    size_t src_len = strlen(str) + 1;
    FTempStringBuffer buffer(src_len * 2 + 1);
    char* destBuf = buffer.GetBuffer();

    iconv_t cv = iconv_open("utf-8", "gb2312");
    if( !cv ) return ret;
    
    char** pin = (char**)&str;
    char** pout = (char**)&destBuf;
    
    size_t src_len1 = src_len;
    size_t dest_len = buffer.GetSize();
    
    if( iconv(cv, pin, &src_len1, pout, &dest_len) == -1 )
        return "";
    
    iconv_close(cv);
    
    ret = buffer.GetBuffer();
    return ret;
#endif
}

AString F_UTF8ToGB2312(const char* str)
{
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	return F_UTF8ToANSICP(str, 936);
#else
    AString ret;
    size_t src_len = strlen(str) + 1;
    FTempStringBuffer buffer(src_len * 2 + 1);
    char* destBuf = buffer.GetBuffer();
    
    iconv_t cv = iconv_open("gb2312", "utf-8");
    if( !cv ) return ret;
    
    char** pin = (char**)&str;
    char** pout = (char**)&destBuf;
    
    size_t src_len1 = src_len;
    size_t dest_len = buffer.GetSize();
    
    if( iconv(cv, pin, &src_len1, pout, &dest_len) == -1 )
        return "";
    
    iconv_close(cv);
    
    ret = buffer.GetBuffer();
    return ret;
#endif
}

///////////////////////////////////////////////////////////////////////////
//  
//  处理AString的一些实用函数
//  
///////////////////////////////////////////////////////////////////////////

const AString AStringUtil::BLANK = "";

/** 获取指定文件的扩展名
@Param 指定文件的全路径
@Param 存储文件的扩展名
*/
void AStringUtil::FileExt( const AString& path,AString& ext,bool bHaveDot )
{
    size_t pos = path.find_last_of( '.' );
    if( pos != AString::npos )
        ext = path.substr( bHaveDot ? pos : pos+1,path.length()-pos );
    else
        ext = "";
}

/** 将给定字符串转换为小写
@Param 指定的字符串
*/
void AStringUtil::ToLowerCase( AString& str )
{
    std::transform( str.begin(),str.end(),str.begin(),tolower );
}

/** 将给定字符串转换为大写
@Param 指定的字符串
*/
void AStringUtil::ToUpperCase( AString& str )
{
    std::transform( str.begin(),str.end(),str.begin(),toupper );
}

/** 将字符串分割为多个
@Param 待分割的字符串
@Param 字符串列表
*/
void AStringUtil::Split( const AString& str,const char* split,AStringList& strList )
{
	strList.clear();

	FTempStringBuffer strTemp((uint32)str.length() + 1);
	strcpy(strTemp.GetBuffer(), str.c_str());

	char *pCur = strTemp.GetBuffer(), *p = NULL;
	while( true )
	{
		p = strstr( pCur,split );
		if( p ) *p = '\0';

		if( strlen(pCur) > 0 )
			strList.push_back( pCur );
		if( !p ) break;

		pCur = p + strlen( split );
	}
}

/** 检测字符串是否以另一个字符串开头
@Param 指定的字符串
@Param 另一个字符串
@Param 是否忽略大小写
*/
bool AStringUtil::StartWith( const AString& str,const AString& pattern,
                             bool noCase/* =true */ )
{
    size_t nThisLen = str.length();
    size_t nPatternLen = pattern.length();

    if( nThisLen < nPatternLen || nPatternLen == 0 )
        return false;

    AString startOfThis = str.substr( 0,nPatternLen );
    if( noCase ) ToLowerCase( startOfThis );

    return (startOfThis == pattern);
}

/** 简单的字符串匹配，可以使用星号通配符
@note 常用于查找文件时的文件名匹配
@Param 指定的字符串
@Param 要匹配的字符串
@Param 是否忽略大小写
*/
bool AStringUtil::Match( const AString& str,const AString& pattern,
                         bool noCase/* =true */ )
{
    AString tempStr = str;
    AString tempPattern = pattern;

    if( noCase )
    {
        AStringUtil::ToLowerCase( tempStr );
        AStringUtil::ToLowerCase( tempPattern );
    }

    AString::const_iterator strI = tempStr.begin();
    AString::const_iterator patI = tempPattern.begin();

    // 最后一个通配符
    AString::const_iterator lastWildCardI = tempPattern.end();

    while( strI != tempStr.end() && patI != tempPattern.end() )
    {
        if( *patI == '*' )
        {
            lastWildCardI = patI;
            patI++;

            if( patI == tempPattern.end() )
            {
                strI = tempStr.end();
            }
            else
            {
                while( strI != tempStr.end() && *patI != *strI )
                    strI++;
            }
        }
        else
        {
            if( *patI != *strI )
            {
                if( lastWildCardI != tempPattern.end() )
                {
                    patI = lastWildCardI;
                    lastWildCardI = tempPattern.end();
                }
                else
                {
                    return false;
                }
            }
            else
            {
                patI++;
                strI++;
            }
        }
    }

    if( patI == tempPattern.end() && strI == tempStr.end() )
        return true;
    else
        return false;
}

/** 将文件的全路径分割为目录和文件名
@Param 文件的全路径
@Param 用于存储文件名
@Param 用于存储目录名字
*/
void AStringUtil::SplitPath( const AString& path,AString& baseName,AString& dir )
{
    AString temp = path;
    std::replace( temp.begin(),temp.end(),'\\','/' );
    size_t i = temp.find_last_of( '/' );

    if( i == AString::npos )
    {
        dir.clear();
        baseName = temp;
    }
    else
    {
        dir = temp.substr( 0,i+1 );
        baseName = temp.substr( i+1,temp.size()-i-1 );
    }
}

/** 检测是否绝对路径
@Param 文件路径
*/
bool AStringUtil::IsAbsPath( const AString& path )
{
	if (path.empty())
		return false;

#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	return (isalpha(path[0]) && path[1] == ':');
#else
	return path[0] == '/' || path[0] == '\\';
#endif
}

/** 将字符串中的空格消除
@Param 要消除空格的字符串
@Param 消除前端的空格
@Param 消除后端的空格
*/
void AStringUtil::Trim( AString& str,bool bTrimFront/* =true */,
                        bool bTrimBack/* =true */ )
{
    static const AString delim = " \t\r";
    if( bTrimFront )
        str.erase( 0,str.find_first_not_of(delim) );
    if( bTrimBack )
        str.erase( str.find_last_not_of(delim) + 1 );
}

/** 字符串格式化
*/
AString AStringUtil::Format( const char* format, ... )
{
	FTempStringBuffer strBuf(MAX_STRING_BUF_SIZE);
	va_list argList;
	va_start( argList, format );
	int ret = vsnprintf(strBuf.GetBuffer(), MAX_STRING_BUF_SIZE, format, argList);
	if( ret < 0 || ret >= MAX_STRING_BUF_SIZE )
	{
		FASSERT(0);
		FLOG_ERROR("AStringUtil::Format, Format the string failed, write exceeds buffer!");
		return BLANK;
	}
	va_end( argList );
	return strBuf.GetBuffer();
}

/** 字符串格式化
*/
void AStringUtil::Format(AString& str, const char* format, ...)
{
	FTempStringBuffer strBuf(MAX_STRING_BUF_SIZE);
	va_list argList;
	va_start(argList, format);
	int ret = vsnprintf(strBuf.GetBuffer(), MAX_STRING_BUF_SIZE, format, argList);
	if( ret < 0 || ret >= MAX_STRING_BUF_SIZE )
	{
		FASSERT(0);
		FLOG_ERROR("AStringUtil::Format, Format the string failed, write exceeds buffer!");
		return;
	}
	va_end(argList);
	str = strBuf.GetBuffer();
}

/** 将字符串里出现的\r等转换为转义字符
*/
bool AStringUtil::Translate( AString& str )
{
    return true;
}

/** 将字符串里的转义字符转换为字符串形式
*/
bool AStringUtil::InvTranslate( AString& str )
{
    return true;
}

/** Get the unique name.
*/
AString AStringUtil::UniqueName()
{
	char szBuf[100];
	sprintf(szBuf, "object_%d", l_uniqueNameID++);
	return szBuf;
}

///////////////////////////////////////////////////////////////////////////
//  
//  处理WString的一些实用函数
//  
///////////////////////////////////////////////////////////////////////////

const WString WStringUtil::BLANK = L"";

/** 获取指定文件的扩展名
@Param 指定文件的全路径
@Param 存储文件的扩展名
*/
void WStringUtil::FileExt( const WString& path,WString& ext,bool bHaveDot )
{
	size_t pos = path.find_last_of( L'.' );
	if( pos != WString::npos )
		ext = path.substr( bHaveDot ? pos : pos+1,path.length()-pos );
	else
		ext = L"";
}

/** 将给定字符串转换为小写
@Param 指定的字符串
*/
void WStringUtil::ToLowerCase( WString& str )
{
	std::transform( str.begin(),str.end(),str.begin(),towlower );
}

/** 将给定字符串转换为大写
@Param 指定的字符串
*/
void WStringUtil::ToUpperCase( WString& str )
{
	std::transform( str.begin(),str.end(),str.begin(),towupper );
}

/** 将字符串分割为多个
@Param 待分割的字符串
@Param 字符串列表
*/
void WStringUtil::Split( const WString& str,const wchar_t* split,WStringList& strList )
{
	strList.clear();

	FTempWStringBuffer strTemp((uint32)str.length() + 1);
	wcscpy(strTemp.GetBuffer(), str.c_str());

	wchar_t *pCur = strTemp.GetBuffer(), *p = NULL;
	while( true )
	{
		p = wcsstr( pCur,split );
		if( p ) *p = L'\0';

		if( wcslen(pCur) > 0 )
			strList.push_back( pCur );
		if( !p ) break;

		pCur = p + wcslen( split );
	}
}

/** 检测字符串是否以另一个字符串开头
@Param 指定的字符串
@Param 另一个字符串
@Param 是否忽略大小写
*/
bool WStringUtil::StartWith( const WString& str,const WString& pattern,
							 bool noCase/* =true */ )
{
	size_t nThisLen = str.length();
	size_t nPatternLen = pattern.length();

	if( nThisLen < nPatternLen || nPatternLen == 0 )
		return false;

	WString startOfThis = str.substr( 0,nPatternLen );
	if( noCase ) ToLowerCase( startOfThis );

	return (startOfThis == pattern);
}

/** 简单的字符串匹配，可以使用星号通配符
@note 常用于查找文件时的文件名匹配
@Param 指定的字符串
@Param 要匹配的字符串
@Param 是否忽略大小写
*/
bool WStringUtil::Match( const WString& str,const WString& pattern,
	 					 bool noCase/* =true */ )
{
	WString tempStr = str;
	WString tempPattern = pattern;

	if( noCase )
	{
		WStringUtil::ToLowerCase( tempStr );
		WStringUtil::ToLowerCase( tempPattern );
	}

	WString::const_iterator strI = tempStr.begin();
	WString::const_iterator patI = tempPattern.begin();

	// 最后一个通配符
	WString::const_iterator lastWildCardI = tempPattern.end();

	while( strI != tempStr.end() && patI != tempPattern.end() )
	{
		if( *patI == L'*' )
		{
			lastWildCardI = patI;
			patI++;

			if( patI == tempPattern.end() )
			{
				strI = tempStr.end();
			}
			else
			{
				while( strI != tempStr.end() && *patI != *strI )
					strI++;
			}
		}
		else
		{
			if( *patI != *strI )
			{
				if( lastWildCardI != tempPattern.end() )
				{
					patI = lastWildCardI;
					lastWildCardI = tempPattern.end();
				}
				else
				{
					return false;
				}
			}
			else
			{
				patI++;
				strI++;
			}
		}
	}

	if( patI == tempPattern.end() && strI == tempStr.end() )
		return true;
	else
		return false;
}

/** 将文件的全路径分割为目录和文件名
@Param 文件的全路径
@Param 用于存储文件名
@Param 用于存储目录名字
*/
void WStringUtil::SplitPath( const WString& path,WString& baseName,WString& dir )
{
	WString temp = path;
	std::replace( temp.begin(),temp.end(),L'\\',L'/' );
	size_t i = temp.find_last_of( L'/' );

	if( i == WString::npos )
	{
		dir.clear();
		baseName = temp;
	}
	else
	{
		dir = temp.substr( 0,i+1 );
		baseName = temp.substr( i+1,temp.size()-i-1 );
	}
}

/** 检测是否绝对路径
@Param 文件路径
*/
bool WStringUtil::IsAbsPath( const WString& path )
{
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	return (iswalpha(path[0]) && path[1] == L':');
#else
	return path[0] == L'/' || path[0] == L'\\';
#endif
}

/** 将字符串中的空格消除
@Param 要消除空格的字符串
@Param 消除前端的空格
@Param 消除后端的空格
*/
void WStringUtil::Trim( WString& str,bool bTrimFront/* =true */,
					   bool bTrimBack/* =true */ )
{
	static const WString delim = L" \t\r";
	if( bTrimFront )
		str.erase( 0,str.find_first_not_of(delim) );
	if( bTrimBack )
		str.erase( str.find_last_not_of(delim) + 1 );
}

/** 字符串格式化
*/
WString WStringUtil::Format( const wchar_t* format, ... )
{
	FTempWStringBuffer strBuf(MAX_STRING_BUF_SIZE);
	va_list argList;
	va_start( argList,format );
	int ret = vswprintf(strBuf.GetBuffer(), MAX_STRING_BUF_SIZE, format, argList);
	if( ret < 0 || ret >= MAX_STRING_BUF_SIZE )
	{
		FASSERT(0);
		FLOG_ERROR("WStringUtil::Format, Format the string failed, write exceeds buffer!");
		return BLANK;
	}
	va_end( argList );
	return strBuf.GetBuffer();
}

/** 字符串格式化
*/
void WStringUtil::Format(WString& str, const wchar_t* format, ...)
{
	FTempWStringBuffer strBuf(MAX_STRING_BUF_SIZE);
	va_list argList;
	va_start(argList, format);
	int ret = vswprintf(strBuf.GetBuffer(), MAX_STRING_BUF_SIZE, format, argList);
	if( ret < 0 || ret >= MAX_STRING_BUF_SIZE )
	{
		FASSERT(0);
		FLOG_ERROR("AStringUtil::Format, Format the string failed, write exceeds buffer!");
		return;
	}
	va_end(argList);
	str = strBuf.GetBuffer();
}

/** 将字符串里出现的\r等转换为转义字符
*/
bool WStringUtil::Translate( WString& str )
{
    return true;
}

/** 将字符串里的转义字符转换为字符串形式
*/
bool WStringUtil::InvTranslate( WString& str )
{
    return true;
}

/** Get the unique name.
*/
WString WStringUtil::UniqueName()
{
	wchar_t szBuf[100];
	swprintf(szBuf, L"object_%d", l_uniqueNameID++);
	return szBuf;
}
