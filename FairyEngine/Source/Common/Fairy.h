/*
 * ------------------------------------------------------------------------
 *  Name:   Fairy.h
 *  Desc:   Some macros of engine relevant to the platforms.
 *  Author: Yish
 *  Date:   2010/9/24
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_H__
#define __FAIRY_H__

///////////////////////////////////////////////////////////////////////////
//// Need C++ compiler!

#if !defined(__cplusplus)
    #error C++ compiler required!
#endif

///////////////////////////////////////////////////////////////////////////
//// DEBUG

#if defined(_DEBUG) || defined(DEBUG)
    #define FAIRY_DEBUG
#endif

///////////////////////////////////////////////////////////////////////////
//// Version of the engine

#define FAIRY_VERSION_MAJOR 1
#define FAIRY_VERSION_MINOR 0
#define FAIRY_VERSION_PATCH 0
#define FAIRY_VERSION ((FAIRY_VERSION_MAJOR << 16) | (FAIRY_VERSION_MINOR << 8) | FAIRY_VERSION_PATCH)
#define FAIRY_VERSION_STRING "1.0.0"

///////////////////////////////////////////////////////////////////////////
//// Compiler types

#define FAIRY_COMPILER_MSVC 1
#define FAIRY_COMPILER_GCC 2
#define FAIRY_COMPILER_CLANG 3

#if defined(_MSC_VER)
    #define FAIRY_COMPILER FAIRY_COMPILER_MSVC
	#if _MSC_VER >= 1800
		#define FAIRY_COMPILER_VERSION 120		// Microsoft Visual Studio 2013
	#elif _MSC_VER >= 1700
		#define FAIRY_COMPILER_VERSION 110		// Microsoft Visual Studio 2012
	#elif _MSC_VER >= 1600
		#define FAIRY_COMPILER_VERSION 100		// Microsoft Visual Studio 2010
    #elif _MSC_VER >= 1500
        #define FAIRY_COMPILER_VERSION 90		// Microsoft Visual Studio 2008
    #elif _MSC_VER >= 1400
        #define FAIRY_COMPILER_VERSION 80		// Microsoft Visual Studio 2005
    #elif _MSC_VER >= 1310
        #define FAIRY_COMPILER_VERSION 71		// Microsoft Visual Studio 2003
    #elif _MSC_VER >= 1200
        #define FAIRY_COMPILER_VERSION 60		// Microsoft Visual C++ 6.0
    #else
        #error Only Microsoft Visual C++ 6.0 and later are supported.
    #endif
#elif defined(__clang__)
    #define FAIRY_COMPILER FAIRY_COMPILER_CLANG
    #if defined(__APPLE__)
        #if __clang_major__ >= 6
            #if __clang_minor__ >= 0
                #define FAIRY_COMPILER_VERSION 60
            #endif
        #elif __clang_major__ >= 5
            #if __clang_minor__ >= 1
                #define FAIRY_COMPILER_VERSION 51
            #elif __clang_minor__ >= 0
                #define FAIRY_COMPILER_VERSION 50
            #endif
        #elif __clang_major__ >= 4
            #if __clang_minor__ >= 2
                #define FAIRY_COMPILER_VERSION 42
            #elif __clang_minor__ >= 1
                #define FAIRY_COMPILER_VERSION 41
            #elif __clang_minor__ >= 0
                #define FAIRY_COMPILER_VERSION 40
            #endif
        #else
            #error Only Xcode 4.0 and later are supported.
        #endif
    #else
        #error Unknown compiler.
    #endif
#elif defined(__GNUC__)
    #define FAIRY_COMPILER FAIRY_COMPILER_GCC
    #if __GNUC__ >= 4
        #if __GNUC_MINOR__ >= 3
            #define FAIRY_COMPILER_VERSION 43     // GCC 4.3
        #elif __GNUC_MINOR__ >= 2
            #define FAIRY_COMPILER_VERSION 42     // GCC 4.2
        #elif __GNUC_MINOR__ >= 1
            #define FAIRY_COMPILER_VERSION 41     // GCC 4.1
        #elif __GNUC_MINOR__ >= 0
            #define FAIRY_COMPILER_VERSION 40     // GCC 4.0
        #endif
    #elif __GNUC__ >= 3
        #define FAIRY_COMPILER_VERSION 30         // GCC 3.0
    #else
        #error Only GCC 3.0 and later are supported.
    #endif
#else
    #error Unknown compiler!
#endif

/*
* only certain compilers support __attribute__((deprecated))
*/
#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
	#define FAIRY_DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400 //vs 2005 or higher
	#define FAIRY_DEPRECATED_ATTRIBUTE __declspec(deprecated) 
#else
	#define FAIRY_DEPRECATED_ATTRIBUTE
#endif

///////////////////////////////////////////////////////////////////////////
//// Platform types

#define FAIRY_PLATFORM_WINDOWS	1
#define FAIRY_PLATFORM_LINUX	2
#define FAIRY_PLATFORM_MACOS	3
#define FAIRY_PLATFORM_IOS		4
#define FAIRY_PLATFORM_ANDROID	5
#define FAIRY_PLATFORM_WINRT	6
#define FAIRY_PLATFORM_WP8		7

#if defined(_MSC_VER) || defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    #define FAIRY_PLATFORM FAIRY_PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(__APPLE_CC__)
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
		#define FAIRY_PLATFORM FAIRY_PLATFORM_IOS
	#else
		#define FAIRY_PLATFORM FAIRY_PLATFORM_MACOS
	#endif
#elif defined(__ANDROID__)
	#define FAIRY_PLATFORM FAIRY_PLATFORM_ANDROID
#elif defined(linux) || defined(__linux) || defined(__linux__)
    #define FAIRY_PLATFORM FAIRY_PLATFORM_LINUX
#else
    #error Unknown platform!
#endif

///////////////////////////////////////////////////////////////////////////
//// CPU types

#if FAIRY_COMPILER == FAIRY_COMPILER_MSVC
	#if defined(_M_X64) || defined(_WIN64)
		#define FAIRY_CPUTYPE_X64
		#define FAIRY_PLATFORM_64
	#elif defined(_M_IX86) || defined(_WIN32)
		#define FAIRY_CPUTYPE_X86
		#define FAIRY_PLATFORM_32
	#elif defined(_M_ARM)
		#define FAIRY_CPUTYPE_ARM
		#define FAIRY_PLATFORM_32
	#else
		#define Unknown CPU Type!
	#endif
#elif (FAIRY_COMPILER == FAIRY_COMPILER_GCC) || (FAIRY_COMPILER == FAIRY_COMPILER_CLANG)
	#if defined(__x86_64__)
		#define FAIRY_CPUTYPE_X64
		#define FAIRY_PLATFORM_64
	#elif defined(__i386__)
		#define FAIRY_CPUTYPE_X86
		#define FAIRY_PLATFORM_32
	#elif defined(__arm__)
		#define FAIRY_CPUTYPE_ARM
		#define FAIRY_PLATFORM_32
	#elif defined(__arm64__)
		#define FAIRY_CPUTYPE_ARM64
		#define FAIRY_PLATFORM_64
	#else
		#error Unknown CPU Type!
	#endif
#endif

///////////////////////////////////////////////////////////////////////////
//// CPU bit order

#if defined(FAIRY_CPUTYPE_ARM)
	#ifdef __ARMEB__
		#define FAIRY_ENDIAN_BIG
	#else
		#define FAIRY_ENDIAN_LITTLE
	#endif
#elif defined(FAIRY_CPUTYPE_X86) || defined(FAIRY_CPUTYPE_X64) || (FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS)
	#define FAIRY_ENDIAN_LITTLE
#else
	#define FAIRY_ENDIAN_BIG
#endif

///////////////////////////////////////////////////////////////////////////
//// Memory Allocation Config

#define FAIRY_FORCE_USE_STD_CATEGORY 1
#define FAIRY_FORCE_USE_STD_NEWDELETE 1
#define FAIRY_STL_USE_STD_CATEGORY 1

///////////////////////////////////////////////////////////////////////////
//// UNICODE

#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	#if defined(_UNICODE) || defined(UNICODE)
		#define FAIRY_UNICODE
	#endif
#endif

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_H__