/*
 * ------------------------------------------------------------------------
 *  Name:   FTempMemManager.h
 *  Desc:   This file define a memory manager for temp category.
 *  Author: Yish
 *  Date:   2013/12/5
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_TEMPMEMMANAGER_H__
#define __FAIRY_TEMPMEMMANAGER_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FTempMemPool.h"
#include "FMemThreadLock.h"

///////////////////////////////////////////////////////////////////////////

class FTempMemPool;

/** Temp memory manager.
*/
class FTempMemManager : public FRawAlloc
{
public:
	FTempMemManager(uint32 nPoolSize, uint32 nPoolNum);
	virtual ~FTempMemManager();

	// Allocate memory
	void* Allocate(size_t sz);
	// Free memory
	void Free(void* ptr);
	// Reallocate the memory
	void* Realloc(void* ptr, size_t sz);

	// Get current count of active pools
	uint32 GetCurActivePool() const { return m_nCurActivePool; }
	// Get max count of active pools
	uint32 GetMaxActivePool() const { return m_nMaxActivePool; }
	// Get the count of oversize allocation
	uint32 GetOversizeCnt() const { return m_nOversizeCnt; }
	// Get the count of general allocation
	uint32 GetGeneralAllocCnt() const { return m_nGeneralAllocCnt; }

protected:
	bool m_bIsInit;					// Is the manager initialized ?
	uint32 m_nPoolSize;				// Size of each pool
	uint32 m_nPoolNum;				// Max count of pools
	FTempMemPool** m_ppPools;		// Pool array
	uint32 m_nGCCount;				// Garbage collect count
	float m_fGCTime;				// Garbage collect time
	uint32 m_nCurActivePool;		// Current count of active pools
	uint32 m_nMaxActivePool;		// Max count of active pools
	uint32 m_nOversizeCnt;			// Count of oversize allocation
	uint32 m_nGeneralAllocCnt;		// Count of general allocation
	uint32 m_nAllocCount;			// Allocate count
	uint32 m_nFreeCount;			// Free count
	FMemThreadMutex m_Mutex;		// Locker for pools.

	// Initialize the manager
	bool Init();
	// Release the manager
	void Release();
	// Collect the garbage
	void GarbageCollect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_TEMPMEMMANAGER_H__