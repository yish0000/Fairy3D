/*
 * ------------------------------------------------------------------------
 *  Name:   FWinTypes.inl
 *  Desc:   This file define the basic data types for windows platform.
 *  Author: Yish
 *  Date:   2013/11/1
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_WINTYPES_INL__
#define __FAIRY_WINTYPES_INL__

// Windows
#include <windows.h>

// CRT
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// STL stream
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

// STL container
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <bitset>

// STL general algorithm.
#include <algorithm>
#include <functional>
#include <limits>

///////////////////////////////////////////////////////////////////////////
//// Hide some warnings

#if FAIRY_COMPILER_VERSION >= 80
	#pragma warning(disable:4251)
	#pragma warning(disable:4244)
	#pragma warning(disable:4996)

	#ifndef _CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS
	#endif
	#ifndef _CRT_NONSTDC_NO_DEPRECATE
		#define _CRT_NONSTDC_NO_DEPRECATE
	#endif
#endif

///////////////////////////////////////////////////////////////////////////
//  
//  Base Types
//  
///////////////////////////////////////////////////////////////////////////

typedef unsigned char uchar;
typedef unsigned char fbyte;
typedef unsigned short ushort;
typedef unsigned int uint32;
typedef unsigned long ulong;

// 64bit integer
#if FAIRY_COMPILER == FAIRY_COMPILER_MSVC
	typedef __int64 int64;
	typedef unsigned __int64 uint64;
#else
	typedef long long int64;
	typedef unsigned long long uint64;
#endif

#ifdef FAIRY_PLATFORM_64
	typedef int64 ptrint;
	typedef uint64 uptrint;
#else
	typedef __w64 int ptrint;
	typedef __w64 uint32 uptrint;
#endif

///////////////////////////////////////////////////////////////////////////
//  
//  Some Macros
//  
///////////////////////////////////////////////////////////////////////////

#define F_FORCEINLINE	__forceinline
#define F_INLINE		inline
#define F_ALIGN(N)		__declspec(align(N))

// Shutdown min & max in windows.h
#ifndef NOMINMAX
	#define NOMINMAX
#endif

// Allocate memory from the stack
#ifdef FAIRY_PLATFORM_64
	#define F_ALLOC_STACK(sz) ((void*)((((uint64)_alloca((sz) + 15)) + 15) & ~15))
#else
	#define F_ALLOC_STACK(sz) ((void*)((((uint32)_alloca((sz) + 15)) + 15) & ~15))
#endif

///////////////////////////////////////////////////////////////////////////
//  
//  Some string functions
//  
///////////////////////////////////////////////////////////////////////////

#ifndef snprintf
	#define snprintf _snprintf
#endif

#ifndef strcmp_nocase
	#define strcmp_nocase stricmp
#endif

#ifndef wcscmp_nocase
	#define wcscmp_nocase wcsicmp
#endif

#ifndef f_localtime
	#define f_localtime(t, l) localtime_s(t, l)
#endif

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_WINTYPES_INL__