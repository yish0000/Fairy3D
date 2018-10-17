/*
 * ------------------------------------------------------------------------
 *  Name:   FMemConfig.h
 *  Desc:   This file define the memory allocation category for engine.
 *  Author: Yish
 *  Date:   2010/12/23
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_MEMCONFIG_H__
#define __FAIRY_MEMCONFIG_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FAllocBase.h"

///////////////////////////////////////////////////////////////////////////

/** ## Memory management for engine ##
用户应使用引擎定义的宏来操作内存，避免使用malloc/calloc/realloc/free等C语言分配函数，
使用new, delete, F_MALLOC, F_MALLOC_C, F_FREE, F_FREE_C, F_REALLOC, F_REALLOC_C等宏。
使用new操作符定义对象时，如果该对象继承于一个分配器，则调用分配器内的new运算符，
否则调用全局new运算符。
F_MALLOC宏是指使用通用策略分配内存，F_MALLOC_C宏是指指定一个分配策略来分配内存。
使用一个策略分配的内存必须使用对应相同策略来释放！
*/

/** Memory allocation category
*/
enum EMemoryCategory
{
	MCATE_GENERAL,			// 通用分配策略（会启用小内存池机制，适用于一般情况）
	MCATE_TEMP,				// 临时内存，适用于临时内存（如函数内数组等短时间存在的数据）
	MCATE_RAW,				// 原始分配策略（直接调用系统分配函数）
};

class BaseAllocCategory
{
public:
	static void* Allocate(size_t sz, EMemoryCategory category);
	static void* Realloc(void* ptr, size_t sz, EMemoryCategory category);
	static void Free(void* ptr, EMemoryCategory category);
};

template <EMemoryCategory cat> class MemoryAllocCategory
{
public:
	static void* Allocate(size_t sz) { return BaseAllocCategory::Allocate(sz, cat); }
	static void* Realloc(void* ptr, size_t sz) { return BaseAllocCategory::Realloc(ptr, sz, cat); }
	static void Free(void* ptr) { BaseAllocCategory::Free(ptr, cat); }
};

typedef MemoryAllocCategory<MCATE_GENERAL> GeneralAllocCategory;
typedef MemoryAllocCategory<MCATE_TEMP> TempAllocCategory;
typedef MemoryAllocCategory<MCATE_RAW> RawAllocCategory;

typedef FAllocBase<GeneralAllocCategory> FGeneralAlloc;
typedef FAllocBase<TempAllocCategory> FTempAlloc;
typedef FAllocBase<RawAllocCategory> FRawAlloc;

///////////////////////////////////////////////////////////////////////////
// 内存相关的全局函数

struct SMemUsageInfo
{
#if FAIRY_FORCE_USE_STD_MEMORYALLOC == 1
	// For standard category
	uint32 nPeakSize;				// 内存使用峰值
	uint32 nSTDAllocCount;			// 分配单元个数
	uint32 nSTDMemoryUsed;			// 已使用的内存
#else
	// For smart category
	uint32 nPeakSize;				// 内存使用峰值
	uint32 nSmallRawSize;			// 小内存的使用大小
	uint32 nSmallSize;				// 小内存的实际占用大小
	uint32 nSmallPoolSize;			// 小内存池的大小
	uint32 nLargeRawSize;			// 大内存的使用大小
	uint32 nLargeSize;				// 大内存的实际占用大小
	uint32 nLargeBlockCnt;			// 大内存块的个数
#endif

	// For temp category
	uint32 nTempOversizeCnt;		// 临时内存池超限的次数
	uint32 nTempGeneralAllocCnt;	// 使用通用策略分配的次数
	uint32 nMaxTempPool;			// 临时内存池的最大个数
	uint32 nCurTempPool;			// 当前临时内存池的个数
};

// Get the memory usage information.
void F_GetMemInfo(SMemUsageInfo& info);
// Collect the memory garbage.
void F_MemGarbageCollect();
// Print the memory usage to a file. (Only at DEBUG mode)
void F_OuputMemoryUsage(const char* filename);

///////////////////////////////////////////////////////////////////////////
// 对于那些非继承于FAllocBase的对象，使用通用策略分配内存

void* operator new ( size_t sz );
void* operator new[] ( size_t sz );
void operator delete ( void* ptr );
void operator delete[] ( void* ptr );

// Placement new
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	#ifndef __PLACEMENT_NEW_INLINE
		#define __PLACEMENT_NEW_INLINE
		inline void* operator new ( size_t sz, void* ptr ) { return ptr; }
		inline void operator delete ( void* ptr, void* ) {}
	#endif
#endif

///////////////////////////////////////////////////////////////////////////
// Define some macros

#define F_MALLOC_C(sz, cat) MemoryAllocCategory<cat>::Allocate(sz)
#define F_FREE_C(ptr, cat) MemoryAllocCategory<cat>::Free(ptr)
#define F_REALLOC_C(ptr, sz, cat) MemoryAllocCategory<cat>::Realloc(ptr, sz)

#define F_MALLOC(sz) F_MALLOC_C(sz, MCATE_GENERAL)
#define F_FREE(ptr) F_FREE_C(ptr, MCATE_GENERAL)
#define F_REALLOC(ptr, sz) F_REALLOC_C(ptr, sz, MCATE_GENERAL)

#define F_MALLOC_TEMP(sz) F_MALLOC_C(sz, MCATE_TEMP)
#define F_FREE_TEMP(ptr) F_FREE_C(ptr, MCATE_TEMP)
#define F_REALLOC_TEMP(ptr, sz) F_REALLOC_C(ptr, sz, MCATE_TEMP)

#define F_SAFE_DELETE(p) if(p) { delete (p); (p) = NULL; }
#define F_SAFE_DELETE_ARRAY(p) if(p) { delete[] (p); (p) = NULL; }
#define F_SAFE_FREE(p) if(p) { F_FREE(p); (p) = NULL; }
#define F_SAFE_FREE_STD(p) if(p) { F_FREE_STD(p); (p) = NULL; }
#define F_SAFE_FREE_TEMP(p) if(p) { F_FREE_TEMP(p); (p) = NULL; }

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_MEMCONFIG_H__