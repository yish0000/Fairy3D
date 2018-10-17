/*
 * ------------------------------------------------------------------------
 *  Name:   FTempMemManager.cpp
 *  Desc:   This file define a memory manager for temp category.
 *  Author: Yish
 *  Date:   2013/12/5
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FTempMemManager.h"
#include "FTempMemPool.h"
#include "FMemManager.h"
#include "FMemConfig.h"
#include "FTimer.h"

// Allocate memory with general method
inline void* _Allocate(size_t sz) { return FMemManager::GetInstance()->Allocate(sz, MCATE_GENERAL); }
inline void _Free(void* ptr) { FMemManager::GetInstance()->Free(ptr, MCATE_GENERAL); }

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FTempMemManager
//  
///////////////////////////////////////////////////////////////////////////

FTempMemManager::FTempMemManager(uint32 nPoolSize, uint32 nPoolNum)
	: m_nPoolSize(nPoolSize)
	, m_nPoolNum(nPoolNum)
	, m_bIsInit(false)
	, m_ppPools(NULL)
	, m_nGCCount(0)
	, m_fGCTime(0.0f)
	, m_nCurActivePool(0)
	, m_nMaxActivePool(0)
	, m_nOversizeCnt(0)
	, m_nGeneralAllocCnt(0)
	, m_nAllocCount(0)
	, m_nFreeCount(0)
{
	FASSERT(nPoolNum > 0 && nPoolSize > 0);
}

FTempMemManager::~FTempMemManager()
{
	Release();
}

// Initialize the manager
bool FTempMemManager::Init()
{
	if( m_bIsInit )
		return true;

	m_ppPools = (FTempMemPool**)_Allocate(sizeof(FTempMemPool*) * m_nPoolNum);
	if( !m_ppPools ) return false;

	for(uint32 n=0; n<m_nPoolNum; n++)
	{
		m_ppPools[n] = new FTempMemPool(m_nPoolSize);
	}

	m_bIsInit = true;
	return true;
}

// Release the manager
void FTempMemManager::Release()
{
	if( !m_bIsInit )
		return;

	FASSERT(m_nAllocCount == m_nFreeCount);

	if (m_ppPools)
	{
		for (uint32 n = 0; n < m_nPoolNum; n++)
		{
			FTempMemPool* pPool = m_ppPools[n];
			if (pPool)
				delete pPool;
		}

		_Free(m_ppPools);
		m_ppPools = NULL;
	}

	m_bIsInit = false;
}

// Allocate the temp memory
void* FTempMemManager::Allocate(size_t sz)
{
	void* pData = NULL;

	m_Mutex.Lock();

	m_nAllocCount++;

	// Try to allocate from the pools
	if( sz < m_nPoolSize )
	{
		sz = (sz + 3) & ~3;

		if( !m_bIsInit )
		{
			if (!Init())
			{
				m_Mutex.Unlock();
				return NULL;
			}
		}

		for(uint32 n=0; n<m_nPoolNum; n++)
		{
			FTempMemPool* pPool = m_ppPools[n];
			pData = pPool->Allocate((uint32)sz);
			if( pData )
				break;
		}
	}

	// Allocate from the pool failed! then allocate with general method
	if( !pData )
	{
		m_nGeneralAllocCnt++;

		if( sz > m_nPoolSize - sizeof(FTempMemPool::SMemBlock) )
			m_nOversizeCnt++;

		m_Mutex.Unlock();

		pData = _Allocate(sz);
	}
	else
	{
		m_Mutex.Unlock();
	}

	return pData;
}

// Free the temp memory
void FTempMemManager::Free(void* ptr)
{
	if( !ptr )
		return;

	m_Mutex.Lock();

	m_nFreeCount++;

	// Try to free from the pools
	for(uint32 n=0; n<m_nPoolNum; n++)
	{
		FTempMemPool* pPool = m_ppPools[n];
		if( pPool->Free(ptr) )
		{
			ptr = NULL;
			break;
		}
	}

	if( ++m_nGCCount >= 200 )
	{
		GarbageCollect();
		m_nGCCount = 0;
	}

	m_Mutex.Unlock();

	if( ptr )
		_Free(ptr);
}

// Reallocate the temp memory
void* FTempMemManager::Realloc(void* ptr, size_t sz)
{
	if( ptr && !sz )
	{
		Free(ptr);
		return NULL;
	}

	if( !ptr )
		return Allocate(sz);

	uint32 nUsableSize = FTempMemPool::GetBlockSize(ptr);
	if( nUsableSize >= (uint32)sz )
		return ptr;

	void* pNewMem = Allocate(sz);
	if( !pNewMem ) return NULL;

	if( pNewMem != ptr )
	{
		memcpy(pNewMem, ptr, nUsableSize);
		Free(ptr);
	}

	return pNewMem;
}

// Garbage collect
void FTempMemManager::GarbageCollect()
{
	if (m_nPoolNum <= 1)
		return;

	float fTime = FTimer::GetInstance().GetCurTime();
	if (fTime < m_fGCTime + 10.0f)
		return;

	uint32 nOldActiveCount = 0;
	m_nCurActivePool = 0;

	FTempMemPool* pPool = m_ppPools[0];
	if( pPool->IsInit() )
	{
		nOldActiveCount = 1;
		m_nCurActivePool = 1;
	}

	for(uint32 n=1; n<m_nPoolNum; n++)
	{
		FTempMemPool* pPool = m_ppPools[n];

		if( pPool->IsInit() )
			nOldActiveCount++;

		pPool->GarbageCollect();

		if( pPool->IsInit() )
			m_nCurActivePool++;
	}

	if( nOldActiveCount > m_nMaxActivePool )
		m_nMaxActivePool = nOldActiveCount;

	m_fGCTime = fTime;
}