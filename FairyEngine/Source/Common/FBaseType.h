/*
 * ------------------------------------------------------------------------
 *  Name:   FBaseType.h
 *  Desc:   This file defines the basic data types for engine.
 *  Author: Yish
 *  Date:   2010/12/19
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_BASETYPE_H__
#define __FAIRY_BASETYPE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "Fairy.h"

#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	#include "Platform/Windows/FWinTypes.inl"
#elif (FAIRY_PLATFORM == FAIRY_PLATFORM_LINUX) || (FAIRY_PLATFORM == FAIRY_PLATFORM_ANDROID)
	#include "Platform/Linux/FLinuxTypes.inl"
#elif (FAIRY_PLATFORM == FAIRY_PLATFORM_MACOS) || (FAIRY_PLATFORM == FAIRY_PLATFORM_IOS)
	#include "Platform/Mac/FMacTypes.inl"
#endif

///////////////////////////////////////////////////////////////////////////
//// Define and Macro

#ifndef NULL
	#ifdef __cplusplus
		#define NULL 0
	#else
		#define NULL ((void*)0)
	#endif
#endif

// Max length of the file path.
#ifdef MAX_PATH
	#undef MAX_PATH
#endif
#define MAX_PATH 260

// Show some warning message at the compile time!
// Eg. #pragma F_COMPILE_WARN("Need to fix this bug!")
#if FAIRY_COMPILER == FAIRY_COMPILER_MSVC
	#define F_STRINGIZE(x) #x
	#define F_QUOTE(x) F_STRINGIZE(x)
	#define F_COMPILE_WARN(x) message( __FILE__ "(" F_QUOTE(__LINE__) ") : warning --> " x "\n" )
#else
	#define F_COMPILE_WARN(x) message("")
#endif

#define F_ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#if FAIRY_COMPILER == FAIRY_COMPILER_GCC
	#define F_STRUCT_OFFSET(s,m) ((unsigned long)((&(s*)0x1)->m) - 0x1)
#else
	#define F_STRUCT_OFFSET(s,m) (unsigned long)(&((s*)0)->m)
#endif

#if FAIRY_COMPILER == FAIRY_COMPILER_MSVC
	#define F_STATIC_ASSERT(expr, msg) typedef char COMPILE_ERROR_##msg[1][(expr)]
#else
	#define F_STATIC_ASSERT(expr, msg) typedef char COMPILE_ERROR_##msg[1][(expr)?1:-1]
#endif

#ifdef FAIRY_DEBUG
	#define FASSERT(c) assert( c )
	#define FASSERT_(c, str) assert( c && str )
	#define FASSERT_RET(c) if(!c) { assert(0); return; }
	#define FASSERT_RET2(c, ret) if(!c) { assert(0); return ret; }
#else
	#define FASSERT(c) ((void)0)
	#define FASSERT_(c, str) ((void)0)
	#define FASSERT_RET(c) ((void)0)
	#define FASSERT_RET2(c, ret) ((void)0)
#endif

#define F_UNUSED_PARAM(p) (void)p

#define F_BIT(b) (1 << (b))
#define F_MAKEFOURCC(ch0, ch1, ch2, ch3) \
    ((uint32)(uchar)(ch0) | ((uint32)(uchar)(ch1) << 8) | \
    ((uint32)(uchar)(ch2) << 16) | ((uint32)(uchar)(ch3) << 24 ))

// Disable the copy and equal operator for a class.
#define F_DISABLE_COPY_ASSIGNMENT(ClassName) \
	private: \
		ClassName(const ClassName&); \
		ClassName& operator = (const ClassName&);

// Export the DLL's symbols.
#if FAIRY_COMPILER == FAIRY_COMPILER_MSVC
	#if defined(_USRDLL)
		#define FAIRY_API __declspec(dllexport)
		#define FAIRY_PRIVATE_API
	#else
		#define FAIRY_API
		#define FAIRY_PRIVATE_API
	#endif
#else
	// gcc, clang
	#if defined(_BUILD_DYLIB_)
		#define FAIRY_API __attribute__(visibility("default"))
		#define FAIRY_PRIVATE_API __attribute__(visibility("hidden"))
	#else
		#define FAIRY_API
		#define FAIRY_PRIVATE_API
	#endif
#endif

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_BASETYPE_H__