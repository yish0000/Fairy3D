/*
 * ------------------------------------------------------------------------
 *  Name:   FStrUtil.h
 *  Desc:   This file define some utility functions for string.
 *  Author: Yish
 *  Date:   2010/9/23
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_STRUTIL_H__
#define __FAIRY_STRUTIL_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//  
//  String encoding conversion
//  
///////////////////////////////////////////////////////////////////////////

//
// In our engine, we store and transfer the string as utf-8.
//

AString F_ANSIToUTF8(const char* str);
AString F_UTF8ToANSI(const char* str);
AString F_GB2312ToUTF8(const char* str);
AString F_UTF8ToGB2312(const char* str);

inline AString F_ANSIToUTF8(const AString& str) { return F_ANSIToUTF8(str.c_str()); }
inline AString F_UTF8ToANSI(const AString& str) { return F_UTF8ToANSI(str.c_str()); }
inline AString F_GB2312ToUTF8(const AString& str) { return F_GB2312ToUTF8(str.c_str()); }
inline AString F_UTF8ToGB2312(const AString& str) { return F_UTF8ToGB2312(str.c_str()); }

// 如果代码里面用VS编译，代码里的字符串请用本宏包装一下
#define F_CPPTEXTTOUTF8(str) F_ANSIToUTF8(str)

// For windows, We need to handle the wide chars.
// UNICODE, For windows platform, is UTF-16LE
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS

uint32 F_GetCurCodePage();

WString F_ANSIToUnicode(const char* str);
AString F_UnicodeToANSI(const wchar_t* str);
WString F_UTF8ToUnicode(const char* str);
AString F_UnicodeToUTF8(const wchar_t* str);
WString F_GB2312ToUnicode(const char* str);
AString F_UnicodeToGB2312(const wchar_t* str);

inline WString F_ANSIToUnicode(const AString& str) { return F_ANSIToUnicode(str.c_str()); }
inline AString F_UnicodeToANSI(const WString& str) { return F_UnicodeToANSI(str.c_str()); }
inline WString F_UTF8ToUnicode(const AString& str) { return F_UTF8ToUnicode(str.c_str()); }
inline AString F_UnicodeToUTF8(const WString& str) { return F_UnicodeToUTF8(str.c_str()); }
inline WString F_GB2312ToUnicode(const AString& str) { return F_GB2312ToUnicode(str.c_str()); }
inline AString F_UnicodeToGB2312(const WString& str) { return F_UnicodeToGB2312(str.c_str()); }

// For windows, TCHAR, _T, CString in MFC.
#ifdef FAIRY_UNICODE
	#define F_TEXTTOUTF8(str) F_UnicodeToUTF8(str)
	#define F_UTF8TOTEXT(str) F_UTF8ToUnicode(str)
#else
	#define F_TEXTTOUTF8(str) F_ANSIToUTF8(str)
	#define F_UTF8TOTEXT(str) F_UTF8ToANSI(str)
#endif

#endif

///////////////////////////////////////////////////////////////////////////

/** Some functions for string.
*/
struct AStringUtil
{
    // Get the extension name of file.
    static void FileExt( const AString& path, AString& ext, bool bHaveDot=false );

    // Split the whole file path to a directory and filename.
    static void SplitPath( const AString& path, AString& baseName, AString& dir );

	// Split the specified string.
	static void Split( const AString& str, const char* split, AStringList& strList );

	// Is an absolute path.
	// For unix/linux: "/bin/*", windows: C:\\Windows\\System32\\cmd.exe
	static bool IsAbsPath( const AString& path );

    // Removes the leading and trailing spaces.
    static void Trim( AString& str, bool bTrimFront=true, bool bTrimBack=true );

    // Make the string to lower case.
    static void ToLowerCase( AString& str );

    // Make the string to upper case.
    static void ToUpperCase( AString& str );

    // Is the string start with the specified pattern ?
    static bool StartWith( const AString& str, const AString& pattern, bool noCase=true );

	// Is the string matching with the specified pattern ?
    static bool Match( const AString& str, const AString& pattern, bool noCase=true );

	// Translate the escape characters in string.
	// For example: "Hello \\r world!\\n" -> "Hello \r world!\n"
    static bool Translate( AString& str );

	// Translate the escape characters in string.
	// For example: "Hello \r world!\n" -> "Hello \\r world!\\n"
    static bool InvTranslate( AString& str );

	// Get a unique name.
	static AString UniqueName();

    // Format a string.
    static AString Format( const char* format, ... );
	static void Format(AString& str, const char* format, ...);

	// Convert a value to a string.
	template <class T>
	static AString ToString(T val)
	{
		AStringStream ss;
		ss << val;
		return ss.str();
	}

    // Some constant variables for string.
    static const AString BLANK;
};

///////////////////////////////////////////////////////////////////////////

/** Some functions for wide-char string.
@notice
	Usually In a normal situation, for windows, wchar_t has 2 bytes, but for unix/linux platforms, wchar_t has 4 bytes.
	If we want to use the short wchar_t as 2 bytes, we can use -fshort-wchar on gcc.
*/
struct WStringUtil
{
	// Get the extension name of file.
	static void FileExt( const WString& path, WString& ext, bool bHaveDot=false );

	// Split the whole file path to a directory and filename.
	static void SplitPath( const WString& path, WString& baseName, WString& dir );

	// Split the specified string.
	static void Split( const WString& str, const wchar_t* split, WStringList& strList );

	// Is an absolute path.
	// For unix/linux: "/bin/*", windows: C:\\Windows\\System32\\cmd.exe
	static bool IsAbsPath( const WString& path );

    // Removes the leading and trailing spaces.
	static void Trim( WString& str, bool bTrimFront=true, bool bTrimBack=true );

    // Make the string to lower case.
	static void ToLowerCase( WString& str );

    // Make the string to upper case.
	static void ToUpperCase( WString& str );

    // Is the string start with the specified pattern ?
	static bool StartWith( const WString& str, const WString& pattern, bool noCase=true );

	// Is the string matching with the specified pattern ?
	static bool Match( const WString& str, const WString& pattern, bool noCase=true );

	// Translate the escape characters in string.
	// For example: "Hello \\r world!\\n" -> "Hello \r world!\n"
    static bool Translate( WString& str );

	// Translate the escape characters in string.
	// For example: "Hello \r world!\n" -> "Hello \\r world!\\n"
    static bool InvTranslate( WString& str );

	// Get a unique name.
	static WString UniqueName();

	// Format a string.
	static WString Format( const wchar_t* format, ... );
	static void Format(WString& str, const wchar_t* format, ...);

	// Convert a value to a string.
	template <class T>
	static WString ToString(T val)
	{
		WStringStream ss;
		ss << val;
		return ss.str();
	}

	// Some constant variables for string.
	static const WString BLANK;
};

// For windows. ANSI and UNICODE are compatible.
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	#ifdef FAIRY_UNICODE
		typedef AStringUtil TStringUtil;
	#else
		typedef WStringUtil TStringUtil;
	#endif
#endif

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_STRUTIL_H__