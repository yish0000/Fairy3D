/*
 * ------------------------------------------------------------------------
 *  Name:   FMacTypes.inl
 *  Desc:   This file define the basic data types for macos platform.
 *  Author: Yish
 *  Date:   2013/11/1
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_MACTYPES_INL__
#define __FAIRY_MACTYPES_INL__

// CRT
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

// STL stream
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

// STL containers
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

// STL algorithm
#include <algorithm>
#include <functional>
#include <limits>

///////////////////////////////////////////////////////////////////////////
//
//  Base Types
//
///////////////////////////////////////////////////////////////////////////

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint32;
typedef unsigned long ulong;

typedef unsigned char FBYTE;

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
	typedef int ptrint;
	typedef uint32 uptrint;
#endif

///////////////////////////////////////////////////////////////////////////
//
//  Some Macros
//
///////////////////////////////////////////////////////////////////////////

#define F_FORCEINLINE	__forceinline
#define F_INLINE		inline
#define F_ALIGN(N)		__declspec(align(N))

// Allocate memory from the stack
#ifdef FAIRY_PLATFORM_64
	#define F_ALLOC_STACK(sz) ((void*)((((uint64)alloca((sz) + 15)) + 15) & ~15))
#else
	#define F_ALLOC_STACK(sz) ((void*)((((uint32)alloca((sz) + 15)) + 15) & ~15))
#endif

///////////////////////////////////////////////////////////////////////////
//  
//  Some string functions
//  
///////////////////////////////////////////////////////////////////////////

#ifndef strcmp_nocase
	#define strcmp_nocase strcasecmp
#endif

#ifndef wcscmp_nocase
	#define wcscmp_nocase wcscasecmp
#endif

#ifndef f_localtime
	#define f_localtime(t, l) localtime_r(l, t)
#endif

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_MACTYPES_INL__