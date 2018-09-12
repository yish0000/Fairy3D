/*
 * ------------------------------------------------------------------------
 *  Name:   FMemManager.h
 *  Desc:   This file define a memory category manager.
 *  Author: Yish
 *  Date:   2014/2/20
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_MEMMANAGER_H__
#define __FAIRY_MEMMANAGER_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBaseType.h"
#include "FThread.h"

///////////////////////////////////////////////////////////////////////////

class FSmallMem;
class FLargeMem;
class FTempMemManager;
class FMemTracker;

///////////////////////////////////////////////////////////////////////////
//  
//  class FMemManager
//  
///////////////////////////////////////////////////////////////////////////

class FMemManager
{
public:
	FMemManager();
	virtual ~FMemManager();

	// Get the instance of memory manager.
	static FMemManager* GetInstance();

	// Allocate the memory
	void* Allocate(size_t sz, int category);
	// Free the memory
	void Free(void* ptr, int category);
	// Reallocate the memory
	void* Realloc(void* ptr, size_t sz, int category);

	// Add the allocated memory size
	void AddAllocSmallSize(int iRawSize, int iBlkSize);
	void AddSmallMemPoolSize(int iPoolSize);
	void AddAllocLargeSize(int iRawSize, int iBlkSize);
	void AddAllocSTDSize(int iRawSize);

	// Get the small memory manager
	FSmallMem* GetSmallMem() { return m_pSmall; }
	// Get the large memory manager
	FLargeMem* GetLargeMem() { return m_pLarge; }
	// Get the temp memory manager
	FTempMemManager* GetTempMemMan() { return m_pTempMem; }
	// Get the standard memory tracker
	FMemTracker* GetSTDMemTracker() { return m_pSTDTracker; }

	// Get the peak memory usage
	uint32 GetPeakSize() const { return m_nPeakSize; }
	// Get the small memory usage
	uint32 GetSmallSize() const { return m_nSmallSize; }
	// Get the raw size of small memory
	uint32 GetSmallRawSize() const { return m_nSmallRawSize; }
	// Get the size of small memory pool.
	uint32 GetSmallMemPoolSize() const { return m_nSmallPoolSize; }
	// Get the large memory usage
	uint32 GetLargeSize() const { return m_nLargeSize; }
	// Get the raw size of large memory
	uint32 GetLargeRawSize() const { return m_nLargeRawSize; }
	// Get the standard memory usage
	uint32 GetSTDMemSize() const { return m_nSTDSize; }

	// Dump the memory leak information
	void DumpMemoryLeaks();

	// Dump the current memory usage
	void DumpCurMemoryUsage(FILE* pFile);

protected:
	FSmallMem* m_pSmall;			// Small memory manager
	FLargeMem* m_pLarge;			// Large memory manager
	FTempMemManager* m_pTempMem;	// Temp memory manager
	FMemTracker* m_pSTDTracker;		// Standard memory tracker

	FThreadSpin m_SpinLock;			// For thread-safe.
	uint32 m_nPeakSize;				// Peak memory usage
	uint32 m_nSmallSize;			// Small memory usage
	uint32 m_nSmallRawSize;			// Raw size of small memory
	uint32 m_nSmallPoolSize;		// Size of the small memory pool.
	uint32 m_nLargeSize;			// Large memory usage
	uint32 m_nLargeRawSize;			// Raw size of large memory
	uint32 m_nSTDSize;				// Standard memory usage

	// Get the usable size of the block
	uint32 GetUsableSize(void* pMem);
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_MEMMANAGER_H__