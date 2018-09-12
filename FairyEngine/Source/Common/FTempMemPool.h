/*
 * ------------------------------------------------------------------------
 *  Name:   FTempMemPool.h
 *  Desc:   This file define a temp memory pool.
 *  Author: Yish
 *  Date:   2013/12/5
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_TEMPMEMPOOL_H__
#define __FAIRY_TEMPMEMPOOL_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBaseType.h"

///////////////////////////////////////////////////////////////////////////

class FTempMemPool
{
public:

	struct SMemBlock
	{
		int iSize;
		SMemBlock* pPrev;
		SMemBlock* pNext;

		bool IsUsed() const
		{
			return iSize < 0;
		}
	};

public:
	FTempMemPool(uint32 nBlockSize);
	virtual ~FTempMemPool();

	// Initialize the pool
	bool Init();
	// Release the pool
	void Release();
	// Is the pool initialized ?
	bool IsInit() const { return m_bIsInit; }

	// Allocate the memory
	void* Allocate(uint32 nSize);
	// Free the memory
	bool Free(void* ptr);
	// Garbage collect
	void GarbageCollect();

	// Get the allocate counter
	uint32 GetAllocCount() const { return m_nAllocCount; }
	// Get the free counter
	uint32 GetFreeCount() const { return m_nFreeCount; }

	// Get the specified block's size
	static uint32 GetBlockSize(void* ptr);

protected:
	bool m_bIsInit;				// Is the block initialized ?
	char* m_pData;				// Pointer to the buffer
	uint32 m_nBlockSize;		// Size of the whole block
	SMemBlock* m_pFreeBlock;	// Pointer to the free blocks
	uint32 m_nAllocCount;		// Allocate count
	uint32 m_nFreeCount;		// Free count

	// Add a new node
	SMemBlock* GenNewNode(char* pBuf, char* pTail);
	// Find a free node
	SMemBlock* FindFreeNode(SMemBlock* pBlock);
	SMemBlock* FindFreeNode(SMemBlock* pBlock, uint32 nSize);
	// Allocate a new node and arrange the block
	SMemBlock* ArrangeFreeNode(SMemBlock* pBlock, uint32 nSize);
	// Merge the free node
	SMemBlock* MergeNode(SMemBlock* pBlock, bool& bMerged);
	// Merge the previous node
	SMemBlock* MergePrevNode(SMemBlock* pBlock, bool& bMerged);
	// Merge the next node
	SMemBlock* MergeNextNode(SMemBlock* pBlock, bool& bMerged);
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_TEMPMEMPOOL_H__