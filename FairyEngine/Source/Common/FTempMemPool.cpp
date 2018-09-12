/*
 * ------------------------------------------------------------------------
 *  Name:   FTempMemPool.cpp
 *  Desc:   This file define a temp memory pool.
 *  Author: Yish
 *  Date:   2013/12/5
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FTempMemPool.h"
#include "FMemConfig.h"
#include "FMemManager.h"

// Allocate memory with general method
inline void* _Allocate(size_t sz) { return FMemManager::GetInstance()->Allocate(sz, MCATE_GENERAL); }
inline void _Free(void* ptr) { FMemManager::GetInstance()->Free(ptr, MCATE_GENERAL); }

FTempMemPool::FTempMemPool(uint32 nBlockSize)
	: m_nBlockSize(nBlockSize)
	, m_pData(NULL)
	, m_bIsInit(false)
	, m_pFreeBlock(NULL)
	, m_nAllocCount(0)
	, m_nFreeCount(0)
{
	FASSERT(m_nBlockSize > 0 && "The block size must be greater than 0!");
}

FTempMemPool::~FTempMemPool()
{
	Release();
}

// Initialize the block
bool FTempMemPool::Init()
{
	if( m_bIsInit )
		return true;

	m_pData = (char*)_Allocate(m_nBlockSize);
	if( !m_pData )
		return false;

	m_pFreeBlock = (SMemBlock*)m_pData;
	m_pFreeBlock->iSize = m_nBlockSize - sizeof(SMemBlock);
	m_pFreeBlock->pNext = NULL;
	m_pFreeBlock->pPrev = NULL;

	m_bIsInit = true;
	return true;
}

// Release the block
void FTempMemPool::Release()
{
	if( !m_bIsInit )
		return;

	if( m_pData )
	{
		_Free(m_pData);
		m_pData = NULL;
	}

	m_nBlockSize = 0;
	m_pFreeBlock = NULL;
	m_nAllocCount = 0;
	m_nFreeCount = 0;
	m_bIsInit = false;
}

// Allocate the memory
void* FTempMemPool::Allocate(uint32 nSize)
{
	void* pData = NULL;

	if( nSize == 0 || nSize > m_nBlockSize - sizeof(SMemBlock) )
		return NULL;

	if( !m_bIsInit && !Init() )
		return NULL;

	if( m_pFreeBlock )
	{
		SMemBlock* pFreeBlk = m_pFreeBlock;
		if( pFreeBlk->iSize < (int)nSize )
			pFreeBlk = FindFreeNode(pFreeBlk, nSize);

		if( pFreeBlk )
		{
			pData = (char*)pFreeBlk + sizeof(SMemBlock);
			pFreeBlk->iSize = -(int)nSize;
			m_pFreeBlock = ArrangeFreeNode(pFreeBlk, nSize);
		}
	}

	if( pData )
		m_nAllocCount++;

	return pData;
}

// Free the memory
bool FTempMemPool::Free(void* ptr)
{
	if( !m_bIsInit || !ptr )
		return false;

	if( ptr < m_pData || ptr >= m_pData + m_nBlockSize )
		return false;

	SMemBlock* pBlock = (SMemBlock*)((char*)ptr - sizeof(SMemBlock));
	if( !pBlock || !pBlock->IsUsed() )
	{
		FASSERT(!"Bad temp memory pointer!");
		return false;
	}

	pBlock->iSize = -pBlock->iSize;

	bool bMerged = false;
	pBlock = MergeNode(pBlock, bMerged);
	if( !m_pFreeBlock || bMerged )
		m_pFreeBlock = pBlock;

	m_nFreeCount++;
	return true;
}

// Garbage collect
void FTempMemPool::GarbageCollect()
{
	if( m_bIsInit && m_nAllocCount == m_nFreeCount )
		Release();
}

// Generate a new memory node
FTempMemPool::SMemBlock* FTempMemPool::GenNewNode(char* pBuf, char* pTail)
{
	SMemBlock* pBlock = (SMemBlock*)pBuf;
	pBlock->iSize = static_cast<uint32>(pTail - pBuf - sizeof(SMemBlock));
	pBlock->pPrev = NULL;
	pBlock->pNext = NULL;
	return pBlock;
}

// Find a free node
FTempMemPool::SMemBlock* FTempMemPool::FindFreeNode(SMemBlock* pBlock, uint32 nSize)
{
	FASSERT(pBlock);
	SMemBlock* pRet = NULL;

	SMemBlock* pPrev = pBlock->pPrev;
	while(pPrev)
	{
		if( !pPrev->IsUsed() && pPrev->iSize > (int)nSize )
		{
			pRet = pPrev;
			break;
		}

		pPrev = pPrev->pPrev;
	}

	if( !pRet )
	{
		SMemBlock* pNext = pBlock->pNext;
		while(pNext)
		{
			if( !pNext->IsUsed() && pNext->iSize > (int)nSize )
			{
				pRet = pNext;
				break;
			}

			pNext = pNext->pNext;
		}
	}

	return pRet;
}

// Find the free node
FTempMemPool::SMemBlock* FTempMemPool::FindFreeNode(SMemBlock* pBlock)
{
	return FindFreeNode(pBlock, 0);
}

// Allocate a new node and arrange the block
FTempMemPool::SMemBlock* FTempMemPool::ArrangeFreeNode(SMemBlock* pBlock, uint32 nSize)
{
	FASSERT(pBlock && nSize);

	SMemBlock* pRet = NULL;
	SMemBlock* pNext = pBlock->pNext;
	char* pBufTail = pNext ? (char*)pNext : m_pData + m_nBlockSize;

	char* pData = (char*)pBlock + sizeof(SMemBlock);
	if( pData + nSize + sizeof(SMemBlock) < pBufTail )
	{
		SMemBlock* pNewData = GenNewNode(pData + nSize, pBufTail);
		pBlock->pNext = pNewData;
		pNewData->pPrev = pBlock;
		if( pNext )
		{
			pNewData->pNext = pNext;
			pNext->pPrev = pNewData;
		}
	}
	else
	{
		pRet = FindFreeNode(pBlock);
	}

	return pRet;
}

// Merge the free node
FTempMemPool::SMemBlock* FTempMemPool::MergeNode(SMemBlock* pBlock, bool& bMerged)
{
	bMerged = false;
	bool bMergePrev = false;
	bool bMergeNext = false;
	SMemBlock* pPrevNode = MergePrevNode(pBlock, bMergePrev);
	SMemBlock* pRet = MergeNextNode(pPrevNode, bMergeNext);

	if( bMergePrev || bMergeNext )
		bMerged = true;
	return pRet;
}

// Merge the previous free node
FTempMemPool::SMemBlock* FTempMemPool::MergePrevNode(SMemBlock* pBlock, bool& bMerged)
{
	SMemBlock* pRet = pBlock;
	SMemBlock* pPrev = pBlock->pPrev;
	if( pPrev && !pPrev->IsUsed() )
	{
		bMerged = true;

		SMemBlock* pNext = pBlock->pNext;
		pPrev->iSize += pBlock->iSize + sizeof(SMemBlock);
		pPrev->pNext = pBlock->pNext;
		if( pNext )
			pNext->pPrev = pPrev;

		pRet = pPrev;
	}

	return pRet;
}

// Merge the next free node
FTempMemPool::SMemBlock* FTempMemPool::MergeNextNode(SMemBlock* pBlock, bool& bMerged)
{
	SMemBlock* pRet = pBlock;
	SMemBlock* pNext = pBlock->pNext;
	if( pNext && !pNext->IsUsed() )
	{
		bMerged = true;

		SMemBlock* pNextNext = pNext->pNext;
		pBlock->iSize += pNext->iSize + sizeof(SMemBlock);
		pBlock->pNext = pNextNext;
		if( pNextNext )
			pNextNext->pPrev = pBlock;
	}

	return pRet;
}

// Get the specified block's size
uint32 FTempMemPool::GetBlockSize(void* ptr)
{
	if( !ptr ) return 0;
	SMemBlock* pBlock = (SMemBlock*)((char*)ptr - sizeof(SMemBlock));
	if( pBlock )
		return pBlock->IsUsed() ? -pBlock->iSize : pBlock->iSize;
	else
		return 0;
}