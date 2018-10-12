/*
 * ------------------------------------------------------------------------
 *  Name:   FMemManager.cpp
 *  Desc:   本文件实现了一个小型对象内存智能分配管理器。
 *  Author: Yish
 *  Date:   2014/2/20
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FMemConfig.h"
#include "FMemManager.h"
#include "FTempMemManager.h"
#include "FSysAPI.h"
#include "FMemTracker.h"
#include "FLogManager.h"

#if FAIRY_COMPILER == FAIRY_COMPILER_MSVC
	// 该标记确保FMemManagerWrapper全局变量最先构造和最后析构
	// 这非常重要，因为很多全局对象或静态局部对象都有可能分配和释放内存
	#pragma warning(disable:4073)
	#pragma init_seg(lib)
#endif

///////////////////////////////////////////////////////////////////////////
//  
//  Wrapper for memory manager
//  
///////////////////////////////////////////////////////////////////////////

class FMemManagerWrapper
{
public:
	~FMemManagerWrapper();

	// Get the memory manager
	FMemManager* GetMemManager();

protected:
	FMemManager* m_pMemMan;

	// Initialize the memory manager
	bool InitMemoryMan();
};

FMemManagerWrapper l_MemManWrapper;

FMemManager* FMemManager::GetInstance()
{
	return l_MemManWrapper.GetMemManager();
}

///////////////////////////////////////////////////////////////////////////
//  
//  Define and Macro
//  
///////////////////////////////////////////////////////////////////////////

#define MEM_THREADSAFE		1		// Enable the thread-safe for memory manager

#define MEM_ALIGN			16
#define MEM_SMALL_BLOCK		512
#define MEM_BLOCK_LEVEL		(MEM_SMALL_BLOCK / MEM_ALIGN)
#define MEM_BLOCK_POOL_SIZE	(128 * 1024)
#define MEM_MAX_BLOCK_COUNT	(MEM_BLOCK_POOL_SIZE / 16)
#define MEM_GC_COUNTER		1000
#define MAX_CALLER_LEVEL	8

#define MEM_ALLOC_FLAG_S	0x0100
#define MEM_FREE_FLAG_S		0x0101
#define MEM_ALLOC_FLAG_L	0x0200
#define MEM_FREE_FLAG_L		0x0201

#define MEM_ALLOC_FILL		0xcd
#define MEM_FREE_FILL		0xfe
#define MEM_SLOPOVER_FLAG	0x98989898

// Only windows platform enable the slopover check.
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	#ifdef FAIRY_DEBUG
		#define MEM_DUMP_INFO
		#define MEM_CHECK_SLOPOVER
		#define MEM_SLOPOVER_SIZE (sizeof(uint32) * 2)
	#else
		#define MEM_SLOPOVER_SIZE 0
	#endif
#else
	#define MEM_SLOPOVER_SIZE 0

	#ifdef FAIRY_DEBUG
//		#define MEM_DUMP_INFO
	#endif
#endif

///////////////////////////////////////////////////////////////////////////
//  
//  Types and Local variables
//  
///////////////////////////////////////////////////////////////////////////

struct SMemSmallBlock
{
#ifdef FAIRY_DEBUG
	uint32 nRawSize;		// Size of the data
#endif

#ifdef MEM_DUMP_INFO
	uptrint nCaller[MAX_CALLER_LEVEL];	// Caller address
#endif

	ushort nFlag;			// Flag of the small block
	ushort nPoolSlot;		// Slot index of the block

	SMemSmallBlock* pNext;	// Next block

#ifdef MEM_CHECK_SLOPOVER
	uint32 nSlopOver[2];	// Slop over check flags
#endif
};

struct SMemLargeBlock
{
#ifdef FAIRY_DEBUG
	SMemLargeBlock* pPrev;	// Pointer to the prev block
	SMemLargeBlock* pNext;	// Pointer to the next block
	uint32 nRawSize;		// Size of the data
#endif

#ifdef MEM_DUMP_INFO
	uptrint nCaller[MAX_CALLER_LEVEL];	// Caller address
#endif

	ushort nFlag;			// Flag of the large block
	ushort nReserved;		// Reserved
	uint32 nBlkSize;		// Size of the large block

#ifdef MEM_CHECK_SLOPOVER
	uint32 nSlopOver[2];	// Slop over check flags
#endif
};

///////////////////////////////////////////////////////////////////////////
//  
//  Raw memory functions
//  
///////////////////////////////////////////////////////////////////////////

static void* _RawMemAlloc(size_t sz) { return malloc(sz); }
static void _RawMemFree(void* ptr) { free(ptr); }

// Lock the thread
static void _MemThreadLock(FThreadSpin* pLock)
{
	if (!pLock)
		return;

	pLock->Lock();
}

// Unlock the thread
static void _MemThreadUnlock(FThreadSpin* pLock)
{
	if( !pLock )
		return;

	pLock->Unlock();
}

// Get the block info from a memory address
static SMemSmallBlock* _GetMemBlockInfo(void* ptr)
{
	return (SMemSmallBlock*)((char*)ptr - sizeof(SMemSmallBlock));
}

// Fill slop over flags
static void _FillSlopOverFlags(void* ptr)
{
#ifdef MEM_CHECK_SLOPOVER
	uint32* pBuf = (uint32*)ptr;
	pBuf[0] = MEM_SLOPOVER_FLAG;
	pBuf[1] = MEM_SLOPOVER_FLAG;
#endif
}

// Check the memory's slop over flags
static bool _CheckSlopOverFlags(void* ptr)
{
#ifdef MEM_CHECK_SLOPOVER
	const uint32* pBuf = (const uint32*)ptr;
	if( pBuf[0] == MEM_SLOPOVER_FLAG && pBuf[1] == MEM_SLOPOVER_FLAG )
		return true;
	else
		return false;
#else
	return false;
#endif
}

// Dump the block's call stack information
template <class Blk>
static void _DumpBlockInfo(const Blk* pBlk)
{
	char szMsg[1024];

	FSysAPI::OutputDebugInfo("Leak %d bytes at:", pBlk->nRawSize);

	for(int i=0; i<MAX_CALLER_LEVEL; i++)
	{
		if( pBlk->nCaller[i] < 0xffff )
			break;

		FSysAPI::ExportSymbolInfo((void*)pBlk->nCaller[i], szMsg);
		FSysAPI::OutputDebugInfo(szMsg);
	}
}

///////////////////////////////////////////////////////////////////////////
//  
//  Raw Mem Vector
//  
///////////////////////////////////////////////////////////////////////////

template <class T>
class FRawMemVector
{
public:
	FRawMemVector() : m_pData(NULL), m_nMaxSize(0), m_nSize(0) {}
	~FRawMemVector()
	{
		if( m_pData )
			_RawMemFree(m_pData);
	}

	// Add a new element to tail
	void push_back(const T& val)
	{
		if( m_nSize >= m_nMaxSize )
		{
			// Allocate 4K buffer from system for each time.
			// This is comfortable for most platform :)
			uint32 nSizeBuf = ((m_nMaxSize + 1) * sizeof(T) / 4096 + 1) * 4096;
			uint32 nNewSize = nSizeBuf / sizeof(T);

			T* pNewData = (T*)_RawMemAlloc(nSizeBuf);
			if( m_pData )
			{
				memcpy(pNewData, m_pData, m_nSize * sizeof(T));
				_RawMemFree(m_pData);
			}

			m_pData = pNewData;
			m_nMaxSize = nNewSize;
		}

		m_pData[m_nSize++] = val;
	}

	// Remove the element quickly
	void remove_fast(uint32 idx)
	{
		FASSERT(idx >= 0 && idx < m_nSize);
		if( m_nSize == 1 || idx == m_nSize - 1 )
			m_nSize--;
		else
		{
			m_pData[idx] = m_pData[m_nSize - 1];
			m_nSize--;
		}
	}

	// Get the specified element
	const T& operator [] (uint32 idx) const
	{
		FASSERT(idx < m_nSize);
		return m_pData[idx];
	}

	// Get the size of the vector
	uint32 size() const { return m_nSize; }

	// Get the max size of the vector
	uint32 max_size() const { return m_nMaxSize; }

protected:
	T* m_pData;			// Pointer to the data
	uint32 m_nMaxSize;	// Max size of the vector
	uint32 m_nSize;		// Size of the vector
};

///////////////////////////////////////////////////////////////////////////
//  
//  class FSmallMemPool
//  
///////////////////////////////////////////////////////////////////////////

class FSmallMemPool
{
	struct PoolSlot
	{
		SMemSmallBlock* pPool;
	};

public:
	FSmallMemPool();
	virtual ~FSmallMemPool();

	// Allocate a new slot of memory
	SMemSmallBlock* Allocate();

	// Set the size of the raw data
	void SetDataSize(uint32 nSize);
	// Get the size of the raw data
	uint32 GetDataSize() const { return m_nDataSize; }
	// Get the size of the block
	uint32 GetBlockSize() const { return m_nBlockSize; }
	// Get the count of block for each slot
	uint32 GetBlockCount() const { return m_nBlockCount; }
	// Get the pool num
	uint32 GetPoolSlotNum() const { return m_Slots.size(); }
	// Get the specified pool
	SMemSmallBlock* GetPoolSlot(int iSlot) { return m_Slots[iSlot].pPool; }

	// Increase the Garbage counter
	uint32 IncGCCounter() { return ++m_nGCCnt; }
	// Clear the Garbage counter
	void ClearGCCounter() { m_nGCCnt = 0; }
	// Get the garbage counter
	uint32 GetGCCounter() const { return m_nGCCnt; }

	// Release the specified slot
	void ReleasePool(uint32 nSlot);
	// Get the total memory size for this pool
	uint32 GetTotleMemSize() const { return m_Slots.size() * MEM_BLOCK_POOL_SIZE; }

protected:
	uint32 m_nDataSize;					// Raw data size of the block
	uint32 m_nBlockSize;				// Size of the block
	uint32 m_nBlockCount;				// Block count of each slot
	uint32 m_nGCCnt;					// Garbage counter
	FRawMemVector<PoolSlot> m_Slots;	// Memory slots.

	// Initialize the specified slot
	bool InitPool(PoolSlot& Slot);
	// Calculate the size of block
	uint32 CalcBlockSize() { return m_nDataSize + sizeof(SMemSmallBlock); }
};

FSmallMemPool::FSmallMemPool() : m_nDataSize(16)
{
	m_nBlockSize = CalcBlockSize();
	m_nBlockCount = MEM_BLOCK_POOL_SIZE / m_nBlockSize;
	m_nGCCnt = 0;
}

FSmallMemPool::~FSmallMemPool()
{
	for(uint32 i=0; i<m_Slots.size(); ++i)
		_RawMemFree(m_Slots[i].pPool);
}

// Set the data size
void FSmallMemPool::SetDataSize(uint32 nSize)
{
	m_nDataSize = nSize;
	m_nBlockSize = CalcBlockSize();
	m_nBlockCount = MEM_BLOCK_POOL_SIZE / m_nBlockSize;
}

// Initialize the slot
bool FSmallMemPool::InitPool(PoolSlot& Slot)
{
	uint32 i = 0;
	SMemSmallBlock* pBlk, *pNext = NULL;
	for(pBlk=Slot.pPool; i<m_nBlockCount-1; i++)
	{
		pNext = (SMemSmallBlock*)((char*)pBlk + m_nBlockSize);
		pBlk->pNext = pNext;
		pBlk = pNext;
	}

	// The last block
	pBlk->pNext = NULL;
	return true;
}

// Create a new slot
SMemSmallBlock* FSmallMemPool::Allocate()
{
	PoolSlot Slot;
	Slot.pPool = (SMemSmallBlock*)_RawMemAlloc(MEM_BLOCK_POOL_SIZE);
	if( !Slot.pPool )
		return NULL;

	InitPool(Slot);
	m_Slots.push_back(Slot);

	// 处理计数信息
	l_MemManWrapper.GetMemManager()->AddSmallMemPoolSize(MEM_BLOCK_POOL_SIZE);

	return Slot.pPool;
}

// Release the slot
void FSmallMemPool::ReleasePool(uint32 nSlot)
{
	FASSERT(nSlot < m_Slots.size());
	_RawMemFree(m_Slots[nSlot].pPool);
	m_Slots.remove_fast(nSlot);

	// 处理计数信息
	l_MemManWrapper.GetMemManager()->AddSmallMemPoolSize(-MEM_BLOCK_POOL_SIZE);
}

///////////////////////////////////////////////////////////////////////////
//  
//  class FSmallMem
//  
///////////////////////////////////////////////////////////////////////////

class FSmallMem
{
	struct PoolInfo
	{
		SMemSmallBlock* pFreeBlks;
		uint32 nDataSize;
		uint32 nBlockSize;
		uint32 nBlockCount;
		uint32 nFreeCount;
	};

	friend class FMemManager;

public:
	FSmallMem(FMemManager* pMemMgr);
	virtual ~FSmallMem();

	// Allocate memory
	void* Allocate(size_t sz);
	// Free memory
	void Free(void* ptr);
	// Garbage collect
	void GarbageCollect();

#ifdef MEM_DUMP_INFO
	// Dump the memory blocks' info to file
	void DumpMemoryBlocks(FILE* pFile);
#endif

	// Get the pool num of the specified level
	uint32 GetPoolSlotNum(int iLevel) const { return m_MemPool[iLevel].GetPoolSlotNum(); }
	// Get the blocks' count of the specified level
	uint32 GetPoolSlotBlockCnt(int iLevel) const { return m_MemPool[iLevel].GetBlockCount(); }
	// Get total memory size of the specified level
	uint32 GetPoolSlotMemSize(int iLevel) const { return m_MemPool[iLevel].GetTotleMemSize(); }
	// Get the data size of the specified level
	uint32 GetDataSize(int iLevel) const { return m_PoolInfo[iLevel].nDataSize; }
	// Get the block size of the specified level
	uint32 GetBlockSize(int iLevel) const { return m_PoolInfo[iLevel].nBlockSize; }
	// Get the allocated block counter of the specified level
	uint32 GetTotalBlockCnt(int iLevel) const { return m_PoolInfo[iLevel].nBlockCount; }
	// Get the freed block counter of the specified level
	uint32 GetFreeBlockCnt(int iLevel) const { return m_PoolInfo[iLevel].nFreeCount; }

protected:
	FMemManager* m_pMemMgr;	// Memory manager
	uint32 m_nGCCounter;	// Garbage collect counter
	FSmallMemPool m_MemPool[MEM_BLOCK_LEVEL];
	PoolInfo m_PoolInfo[MEM_BLOCK_LEVEL];
	FThreadSpin m_SpinLocks[MEM_BLOCK_LEVEL];

	// Get the aligned size
	uint32 GetAlignedSize(uint32 nSize);

#ifdef MEM_DUMP_INFO
	// Dump memory leak
	void Dump();
	// Dump the specified slot
	template <class T>
	void DumpPoolSlot(int iSlot, SMemSmallBlock* p, uint32 nBlkCount, uint32 nBlkSize, T dumpFunc);
#endif
};

///////////////////////////////////////////////////////////////////////////
//  
//  class FLargeMem
//  
///////////////////////////////////////////////////////////////////////////

class FLargeMem
{
	friend class FMemManager;

public:
	FLargeMem(FMemManager* pMemMgr);
	virtual ~FLargeMem();

	// Allocate memory
	void* Allocate(size_t sz);
	// Free memory
	void Free(void* ptr);

	// Get the current block count
	uint32 GetBlockCount() const { return m_nBlockCount; }

#ifdef MEM_DUMP_INFO
	void DumpMemoryBlocks(FILE* pFile);
#endif

protected:
	FMemManager* m_pMemMgr;			// Memory manager
	SMemLargeBlock* m_pBlockList;	// Large block list
	uint32 m_nBlockCount;			// Block count
	FThreadSpin m_SpinLock;			// For Thread-safe.

#ifdef MEM_DUMP_INFO

	// Dump the memory leak information
	void Dump();
	// Dump the specified block
	void DumpBlock(const SMemLargeBlock* pBlock);

#endif
};

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FSmallMem
//  
///////////////////////////////////////////////////////////////////////////

FSmallMem::FSmallMem(FMemManager* pMemMgr)
{
	m_pMemMgr = pMemMgr;
	m_nGCCounter = 0;

	for(int i=0; i<MEM_BLOCK_LEVEL; i++)
	{
		uint32 nSize = (i + 1) * MEM_ALIGN;
		m_MemPool[i].SetDataSize(nSize);

		m_PoolInfo[i].pFreeBlks = NULL;
		m_PoolInfo[i].nDataSize = m_MemPool[i].GetDataSize();
		m_PoolInfo[i].nBlockSize = m_MemPool[i].GetBlockSize();
		m_PoolInfo[i].nBlockCount = 0;
		m_PoolInfo[i].nFreeCount = 0;
	}
}

FSmallMem::~FSmallMem()
{
#ifdef MEM_DUMP_INFO
	Dump();
#endif
}

// Allocate memory
void* FSmallMem::Allocate(size_t sz)
{
	uint32 nDataSize = GetAlignedSize((uint32)sz + MEM_SLOPOVER_SIZE);

	// Large block
	if( nDataSize > MEM_SMALL_BLOCK )
		return m_pMemMgr->GetLargeMem()->Allocate(sz);

	int iPoolSlot = (nDataSize - 1) / MEM_ALIGN;

	// Record the alloc size
	m_pMemMgr->AddAllocSmallSize((int)sz, (int)GetBlockSize(iPoolSlot));

	if (MEM_THREADSAFE)
		_MemThreadLock(&m_SpinLocks[iPoolSlot]);

	PoolInfo& Slot = m_PoolInfo[iPoolSlot];

	if( !Slot.pFreeBlks )
	{
		Slot.pFreeBlks = m_MemPool[iPoolSlot].Allocate();
		int iNewBlkCount = m_MemPool[iPoolSlot].GetBlockCount();
		Slot.nBlockCount += iNewBlkCount;
		Slot.nFreeCount += iNewBlkCount;
	}

	SMemSmallBlock* pBlk = Slot.pFreeBlks;
	Slot.pFreeBlks = pBlk->pNext;
	Slot.nFreeCount--;

	if (MEM_THREADSAFE)
		_MemThreadUnlock(&m_SpinLocks[iPoolSlot]);

#ifdef FAIRY_DEBUG
	pBlk->nRawSize = (uint32)sz;

	char* pData = (char*)pBlk + sizeof(SMemSmallBlock);
	memset(pData, MEM_ALLOC_FILL, sz);

#ifdef MEM_CHECK_SLOPOVER
	_FillSlopOverFlags(pBlk->nSlopOver);
	_FillSlopOverFlags(pData + sz);
#endif

#endif

	pBlk->nPoolSlot = iPoolSlot;
	pBlk->pNext = NULL;
	pBlk->nFlag = MEM_ALLOC_FLAG_S;

	return ++pBlk;
}

// Free memory
void FSmallMem::Free(void* ptr)
{
	if( !ptr )
	{
		FASSERT(0);
		return;
	}

	SMemSmallBlock* pBlk = (SMemSmallBlock*)((char*)ptr - sizeof(SMemSmallBlock));

	if( pBlk->nFlag == MEM_ALLOC_FLAG_L )
	{
		m_pMemMgr->GetLargeMem()->Free(ptr);
		return;
	}
	else if( pBlk->nFlag != MEM_ALLOC_FLAG_S )
	{
		FASSERT(pBlk->nFlag == MEM_ALLOC_FLAG_S);
		return;
	}

	int iSlot = pBlk->nPoolSlot;
	if( iSlot < 0 || iSlot >= MEM_MAX_BLOCK_COUNT )
	{
		FASSERT(iSlot >= 0 && iSlot < MEM_MAX_BLOCK_COUNT);
		return;
	}

	PoolInfo& Slot = m_PoolInfo[iSlot];

#ifdef FAIRY_DEBUG
	char* pData = (char*)pBlk + sizeof(SMemSmallBlock);

#ifdef MEM_CHECK_SLOPOVER
	if( !_CheckSlopOverFlags(pBlk->nSlopOver) ||
		!_CheckSlopOverFlags(pData + pBlk->nRawSize) )
	{
		FASSERT(!"Memory slop over!");
		return;
	}
#endif

	memset(pData, MEM_FREE_FILL, pBlk->nRawSize);

	m_pMemMgr->AddAllocSmallSize(-(int)pBlk->nRawSize, -(int)Slot.nBlockSize);
#endif

	pBlk->nFlag = MEM_FREE_FLAG_S;

	if( MEM_THREADSAFE )
	{
		_MemThreadLock(&m_SpinLocks[iSlot]);
	}

	pBlk->pNext = Slot.pFreeBlks;
	Slot.pFreeBlks = pBlk;
	Slot.nFreeCount++;

	m_MemPool[pBlk->nPoolSlot].IncGCCounter();

	if( MEM_THREADSAFE )
	{
		_MemThreadUnlock(&m_SpinLocks[iSlot]);
	}
}

// Garbage collect
void FSmallMem::GarbageCollect()
{
	int iSlot = m_nGCCounter % MEM_MAX_BLOCK_COUNT;
	m_nGCCounter++;

	FSmallMemPool& Pool = m_MemPool[iSlot];
	PoolInfo& Slot = m_PoolInfo[iSlot];
	uint32 nBlockCount = Pool.GetBlockCount();

	if( Pool.GetGCCounter() < MEM_GC_COUNTER ||
		Slot.nFreeCount < nBlockCount * 15 )
		return;

	Pool.ClearGCCounter();

	struct FreeBlkList
	{
		SMemSmallBlock* pHead;
		SMemSmallBlock* pTail;
	};

	uint32 i;
	uint32 nTotalFree = Slot.nFreeCount;
	FreeBlkList FreeBlks = { NULL, NULL };
	FRawMemVector<uint32> SlotToFree;

	for(i=0; i<Pool.GetPoolSlotNum(); i++)
	{
		if( nTotalFree <= nBlockCount * 5 )
			break;

		uint32 nBlkCount = 0;
		FreeBlkList TempBlks = { NULL, NULL };
		SMemSmallBlock* pStart = Pool.GetPoolSlot(i);
		SMemSmallBlock* pEnd = (SMemSmallBlock*)((char*)pStart + Pool.GetBlockSize() * Pool.GetBlockCount());
		SMemSmallBlock* pCur = Slot.pFreeBlks;
		SMemSmallBlock* pPrev = NULL;

		while(pCur)
		{
			if( pCur >= pStart && pCur < pEnd )
			{
				FASSERT(nBlkCount < nBlockCount);
				nBlkCount++;

				SMemSmallBlock* pCurBlk = pCur;

				// 从FreeList中移除
				if( pPrev )
					pPrev->pNext = pCur->pNext;
				else
					Slot.pFreeBlks = pCur->pNext;

				pCur = pCur->pNext;

				// 插入到临时列表中
				if( !TempBlks.pHead )
				{
					pCurBlk->pNext = NULL;
					TempBlks.pHead = pCurBlk;
					TempBlks.pTail = pCurBlk;
				}
				else
				{
					pCurBlk->pNext = TempBlks.pHead;
					TempBlks.pHead = pCurBlk;
				}
			}
			else
			{
				pPrev = pCur;
				pCur = pCur->pNext;
			}
		}

		if( nBlkCount == Pool.GetBlockCount() )
		{
			// 整个Pool都无效了，删除该Pool
			SlotToFree.push_back(i);
			nTotalFree -= nBlockCount;
		}
		else if( TempBlks.pHead )
		{
			// 只有一部分Block无效，不能删除Pool
			if( !FreeBlks.pHead )
			{
				FreeBlks.pHead = TempBlks.pHead;
				FreeBlks.pTail = TempBlks.pTail;
			}
			else
			{
				TempBlks.pTail->pNext = FreeBlks.pHead;
				FreeBlks.pHead = TempBlks.pHead;
			}
		}
	}

	// 将没有删除的块重新加回去
	if( FreeBlks.pHead )
	{
		FreeBlks.pTail->pNext = Slot.pFreeBlks;
		Slot.pFreeBlks = FreeBlks.pHead;
	}

	// 删除无用的池
	for(i=0; i<SlotToFree.size(); i++)
	{
		Pool.ReleasePool(SlotToFree[i]);
		Slot.nBlockCount -= nBlockCount;
		Slot.nFreeCount -= nBlockCount;
	}
}

// Get the aligned size
uint32 FSmallMem::GetAlignedSize(uint32 nSize)
{
	if( nSize == 0 )
		return MEM_ALIGN;
	else
		return (nSize + MEM_ALIGN - 1) & ~(MEM_ALIGN - 1);
}

#ifdef MEM_DUMP_INFO

// Dump the blocks' info to file
void FSmallMem::DumpMemoryBlocks(FILE* pFile)
{
	struct DumpBlockToFile
	{
		FILE* pFile;

		DumpBlockToFile(FILE* pf) : pFile(pf) {}
		void operator () (const SMemSmallBlock* pBlk) const
		{
			fprintf(pFile, "Memory Usage: 0x%p(%d bytes), Call stack: ", (char*)pBlk + sizeof(SMemSmallBlock), pBlk->nRawSize);

			for(int i=0; i<MAX_CALLER_LEVEL; i++)
			{
				if( !pBlk->nCaller[i] )
					break;

				fprintf(pFile, "0x%p ", (void*)pBlk->nCaller[i]);
			}

			fprintf(pFile, "\n");
		}
	};

	fprintf(pFile, "-------------------------------------------------------------\n");
	fprintf(pFile, "Small memory usage:\n");

	DumpBlockToFile dumpFunc(pFile);
	for(int j=0; j<MEM_MAX_BLOCK_COUNT; j++)
	{
		FSmallMemPool& Pool = m_MemPool[j];

		int iSlotNum = Pool.GetPoolSlotNum();
		for(int i=0; i<iSlotNum; i++)
			DumpPoolSlot(i, Pool.GetPoolSlot(i), Pool.GetBlockCount(), Pool.GetBlockSize(), dumpFunc);
	}

	fprintf(pFile, "-------------------------------------------------------------\n");
}

// Dump the memory leaks
void FSmallMem::Dump()
{
	struct DumpBlockToOutput
	{
		void operator () (const SMemSmallBlock* pBlk) const
		{
			FSysAPI::OutputDebugInfo("--------------------------------------------------------------------------------------------------");

			char szMsg[1024];
			snprintf(szMsg, 1024, "Memory [%d bytes@0x%p] leak at(", pBlk->nRawSize, (char*)pBlk + sizeof(SMemSmallBlock));

			for(int i=0; i<MAX_CALLER_LEVEL; i++)
			{
				char szTemp[64];
				if( i == MAX_CALLER_LEVEL - 1 )
				{
					snprintf(szTemp, 64, "0x%p)", (void*)pBlk->nCaller[i]);
					strcat(szMsg, szTemp);
					break;
				}
				else
				{
					snprintf(szTemp, 64, "0x%p, ", (void*)pBlk->nCaller[i]);
					strcat(szMsg, szTemp);
				}
			}

			FSysAPI::OutputDebugInfo(szMsg);

			_DumpBlockInfo(pBlk);
		}
	};

	DumpBlockToOutput dumpFunc;
	for(uint32 i=0; i<MEM_MAX_BLOCK_COUNT; i++)
	{
		FSmallMemPool& Pool = m_MemPool[i];

		for(uint32 n=0; n<Pool.GetPoolSlotNum(); n++)
			DumpPoolSlot(i, Pool.GetPoolSlot(n), Pool.GetBlockCount(), Pool.GetBlockSize(), dumpFunc);
	}
}

// Dump the specified pool slot
template <class T>
void FSmallMem::DumpPoolSlot(int iSlot, SMemSmallBlock* p, uint32 nBlkCount, uint32 nBlkSize, T dumpFunc)
{
	SMemSmallBlock* aBlocks[MEM_MAX_BLOCK_COUNT];

	uint32 i;
	char* pBegin = (char*)p;
	for(i=0; i<nBlkCount; i++, pBegin += nBlkSize)
		aBlocks[i] = (SMemSmallBlock*)pBegin;

	pBegin = (char*)p;
	SMemSmallBlock* pCur = m_PoolInfo[iSlot].pFreeBlks;
	while(pCur)
	{
		uint32 n = ((char*)pCur - pBegin) / nBlkSize;
		if( n >= 0 && n < nBlkCount )
			aBlocks[i] = NULL;
		pCur = pCur->pNext;
	}

	for(i=0; i<nBlkCount; i++)
	{
		if( aBlocks[i] )
			dumpFunc(aBlocks[i]);
	}
}

#endif

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FLargeMem
//  
///////////////////////////////////////////////////////////////////////////

FLargeMem::FLargeMem(FMemManager* pMemMgr)
{
	m_pMemMgr = pMemMgr;
	m_pBlockList = NULL;
	m_nBlockCount = 0;
}

FLargeMem::~FLargeMem()
{
#ifdef MEM_DUMP_INFO
	Dump();
#endif
}

// Allocate memory
void* FLargeMem::Allocate(size_t sz)
{
	uint32 nMemSize = sizeof(SMemLargeBlock) + (uint32)sz + MEM_SLOPOVER_SIZE;
	SMemLargeBlock* pBlk = (SMemLargeBlock*)_RawMemAlloc(nMemSize);
	if( !pBlk ) return NULL;

	pBlk->nBlkSize = nMemSize;
	pBlk->nFlag = MEM_ALLOC_FLAG_L;
	pBlk->nReserved = 0;

#ifdef FAIRY_DEBUG
	pBlk->nRawSize = (uint32)sz;

	char* pData = (char*)pBlk + sizeof(SMemLargeBlock);
	memset(pData, MEM_ALLOC_FILL, sz);

#ifdef MEM_CHECK_SLOPOVER
	_FillSlopOverFlags(pBlk->nSlopOver);
	_FillSlopOverFlags(pData + sz);
#endif

	m_pMemMgr->AddAllocLargeSize((int)sz, (int)pBlk->nBlkSize);

	if( MEM_THREADSAFE )
	{
		_MemThreadLock(&m_SpinLock);
	}

	m_nBlockCount++;

	pBlk->pPrev = NULL;
	pBlk->pNext = m_pBlockList;
	if( m_pBlockList )
		m_pBlockList->pPrev = pBlk;
	m_pBlockList = pBlk;
	m_nBlockCount++;

	if( MEM_THREADSAFE )
	{
		_MemThreadUnlock(&m_SpinLock);
	}

#endif

	return ++pBlk;
}

// Free memory
void FLargeMem::Free(void* ptr)
{
	if( !ptr )
	{
		FASSERT(0);
		return;
	}

	SMemLargeBlock* pBlk = (SMemLargeBlock*)((char*)ptr - sizeof(SMemLargeBlock));

#ifdef FAIRY_DEBUG
	if( pBlk->nFlag != MEM_ALLOC_FLAG_L )
	{
		FASSERT(pBlk->nFlag == MEM_ALLOC_FLAG_L);
		return;
	}

	pBlk->nFlag = MEM_FREE_FLAG_L;

#ifdef MEM_CHECK_SLOPOVER
	if( _CheckSlopOverFlags(pBlk->nSlopOver) ||
		_CheckSlopOverFlags((char*)ptr + pBlk->nRawSize) )
	{
		FASSERT(!"Memory slop over!");
		return;
	}
#endif

	uint32 nMemSize = pBlk->nBlkSize - sizeof(SMemLargeBlock);
	memset((char*)ptr, MEM_FREE_FILL, nMemSize);

	m_pMemMgr->AddAllocLargeSize(-(int)pBlk->nRawSize, -(int)pBlk->nBlkSize);

	if( MEM_THREADSAFE )
	{
		_MemThreadLock(&m_SpinLock);
	}

	SMemLargeBlock* pPrev = pBlk->pPrev;
	SMemLargeBlock* pNext = pBlk->pNext;

	if( pPrev )
		pPrev->pNext = pNext;
	else
		m_pBlockList = pNext;
	if( pNext )
		pNext->pPrev = pPrev;

	m_nBlockCount--;

	if( MEM_THREADSAFE )
	{
		_MemThreadUnlock(&m_SpinLock);
	}

#endif

	_RawMemFree(pBlk);
}

#ifdef MEM_DUMP_INFO

// Dump the blocks' info to file
void FLargeMem::DumpMemoryBlocks(FILE* pFile)
{
	fprintf(pFile, "-------------------------------------------------------------\n");
	fprintf(pFile, "Large memory usage:\n");

	SMemLargeBlock* pBlk = m_pBlockList;
	while(pBlk)
	{
		fprintf(pFile, "Memory Usage: 0x%p(%d bytes), Call stack: ", (char*)pBlk + sizeof(SMemLargeBlock), pBlk->nRawSize);

		for(int i=0; i<MAX_CALLER_LEVEL; i++)
		{
			if( !pBlk->nCaller[i] )
				break;

			fprintf(pFile, "0x%p ", (void*)pBlk->nCaller[i]);
		}

		fprintf(pFile, "\n");
		pBlk = pBlk->pNext;
	}

	fprintf(pFile, "-------------------------------------------------------------\n");
}

// Dump the memory leak information
void FLargeMem::Dump()
{
	SMemLargeBlock* pCur = m_pBlockList;

	while( pCur )
	{
		DumpBlock(pCur);
		pCur = pCur->pNext;
	}
}

// Dump the specified block
void FLargeMem::DumpBlock(const SMemLargeBlock* pBlock)
{
	FSysAPI::OutputDebugInfo("--------------------------------------------------------------------------------------------------");

	char szMsg[1024];
	snprintf(szMsg, 1024, "Memory [%d bytes@0x%p] leak at(", pBlock->nRawSize, (char*)pBlock + sizeof(SMemLargeBlock));

	for(int i=0; i<MAX_CALLER_LEVEL; i++)
	{
		char szTemp[64];
		if( i == MAX_CALLER_LEVEL - 1 )
		{
			snprintf(szTemp, 64, "0x%p)", (void*)pBlock->nCaller[i]);
			strcat(szMsg, szTemp);
			break;
		}
		else
		{
			snprintf(szTemp, 64, "0x%p, ", (void*)pBlock->nCaller[i]);
			strcat(szMsg, szTemp);
		}
	}

	FSysAPI::OutputDebugInfo(szMsg);

	_DumpBlockInfo(pBlock);
}

#endif

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FMemManager
//  
///////////////////////////////////////////////////////////////////////////

FMemManager::FMemManager()
	: m_nPeakSize(0)
	, m_nSmallSize(0), m_nSmallRawSize(0)
	, m_nLargeSize(0), m_nLargeRawSize(0)
	, m_nSTDSize(0)
{
	m_pSmall = (FSmallMem*)_RawMemAlloc(sizeof(FSmallMem));
	new (m_pSmall) FSmallMem(this);
	m_pLarge = (FLargeMem*)_RawMemAlloc(sizeof(FLargeMem));
	new (m_pLarge) FLargeMem(this);
	m_pTempMem = (FTempMemManager*)_RawMemAlloc(sizeof(FTempMemManager));
	new (m_pTempMem) FTempMemManager(2 * 1024 * 1024, 4);

#ifdef MEM_DUMP_INFO
	m_pSTDTracker = (FMemTracker*)_RawMemAlloc(sizeof(FMemTracker));
	new (m_pSTDTracker) FMemTracker();
#else
	m_pSTDTracker = NULL;
#endif
}

FMemManager::~FMemManager()
{
	m_pSmall->~FSmallMem();
	_RawMemFree(m_pSmall);
	m_pLarge->~FLargeMem();
	_RawMemFree(m_pLarge);
	m_pTempMem->~FTempMemManager();
	_RawMemFree(m_pTempMem);

#ifdef MEM_DUMP_INFO
	m_pSTDTracker->~FMemTracker();
	_RawMemFree(m_pSTDTracker);
#endif

#ifdef FAIRY_DEBUG
	FSysAPI::OutputDebugInfo("Maximum memory used: %d(KB)", (m_nPeakSize + 1023) / 1024);
#endif
}

// Allocate the memory
void* FMemManager::Allocate(size_t sz, int category)
{
	void* pData = NULL;

	if( category == MCATE_GENERAL )
	{
#if FAIRY_FORCE_USE_STD_MEMORYALLOC == 1
		pData = (void*)malloc(sz);

	#ifdef MEM_DUMP_INFO
		m_pSTDTracker->AddTrack(pData, sz);
	#endif
#else
		pData = m_pSmall->Allocate(sz);
		if( !pData ) return NULL;

	#ifdef MEM_DUMP_INFO
		SMemSmallBlock* pBlk = _GetMemBlockInfo(pData);

		const int iMaxDeep = 32;
		uptrint frame_cur = 0;
		int c = 0;
		uptrint callers[iMaxDeep];
		memset(callers, 0, sizeof(callers));

		__asm
		{
			mov frame_cur, ebp
		}

		while(true)
		{
			uptrint thisCall = *(uptrint*)(frame_cur + sizeof(uptrint));
			callers[c] = thisCall;

			c++;

			if( c >= iMaxDeep || !thisCall )
				break;

			uptrint frame_up = *(uptrint*)frame_cur;
			if( frame_up <= frame_cur )
				break;

			frame_cur = frame_up;
		}

		memcpy(pBlk->nCaller, callers, sizeof(uptrint)*MAX_CALLER_LEVEL);
	#endif
#endif
	}
	else if( category == MCATE_TEMP )
	{
		pData = m_pTempMem->Allocate(sz);
	}
	else
	{
		FASSERT(!"Unknown memory category!");
	}

	return pData;
}

// Free the memory
void FMemManager::Free(void* ptr, int category)
{
	if( category == MCATE_GENERAL )
	{
#if FAIRY_FORCE_USE_STD_MEMORYALLOC == 1
		free(ptr);

	#ifdef MEM_DUMP_INFO
		m_pSTDTracker->RemoveTrack(ptr);
	#endif
#else
		m_pSmall->Free(ptr);
#endif
	}
	else if( category == MCATE_TEMP )
	{
		m_pTempMem->Free(ptr);
	}
	else
	{
		FASSERT(!"Unknown memory category!");
	}
}

// Reallocate the memory
void* FMemManager::Realloc(void* ptr, size_t sz, int category)
{
	if( ptr && !sz )
	{
		Free(ptr, category);
		return NULL;
	}

	if( !ptr )
		return Allocate(sz, category);

	if( category == MCATE_GENERAL )
	{
#if FAIRY_FORCE_USE_STD_MEMORYALLOC == 1
		void* newptr = realloc(ptr, sz);
		FASSERT(newptr && "Reallocate memory from heap failed!");

	#ifdef MEM_DUMP_INFO
		m_pSTDTracker->ChangeTrack(ptr, newptr, sz);
	#endif

		return newptr;
#else
		uint32 nMaxSize = GetUsableSize(ptr);
		if( sz < nMaxSize )
		{
	#ifdef FAIRY_DEBUG
			SMemSmallBlock* pBlock = _GetMemBlockInfo(ptr);
			int iDelta = (int)sz - pBlock->nRawSize;

			if( pBlock->nFlag == MEM_ALLOC_FLAG_S )
				AddAllocSmallSize(iDelta, 0);
			else if( pBlock->nFlag == MEM_ALLOC_FLAG_L )
				AddAllocLargeSize(iDelta, 0);
			else
			{
				FASSERT(0);
				return NULL;
			}

			pBlock->nRawSize = (uint32)sz;

		#ifdef MEM_CHECK_SLOPOVER
			uchar* pData = (uchar*)ptr;
			_FillSlopOverFlags(pData + sz);
		#endif

	#endif
			return ptr;
		}

		void* pNewMem = Allocate(sz, category);
		if( !pNewMem ) return NULL;

		if( pNewMem != ptr )
		{
			memcpy(pNewMem, ptr, nMaxSize);
			Free(ptr, category);
		}

		return pNewMem;
#endif
	}
	else if( category == MCATE_TEMP )
	{
		return m_pTempMem->Realloc(ptr, sz);
	}
	else
	{
		FASSERT(!"Unknown memory category!");
		return NULL;
	}
}

// Increase the size of the large memory
void FMemManager::AddAllocLargeSize(int iRawSize, int iBlkSize)
{
#ifdef FAIRY_DEBUG

	if (MEM_THREADSAFE)
		_MemThreadLock(&m_SpinLock);

	m_nLargeSize += iBlkSize;
	m_nLargeRawSize += iRawSize;

	uint32 nTotal = m_nSmallSize + m_nLargeSize + m_nSTDSize;
	if( nTotal > m_nPeakSize )
		m_nPeakSize = nTotal;

	if (MEM_THREADSAFE)
		_MemThreadUnlock(&m_SpinLock);

#endif
}

// Increase the size of the small memory
void FMemManager::AddAllocSmallSize(int iRawSize, int iBlkSize)
{
#ifdef FAIRY_DEBUG

	if (MEM_THREADSAFE)
		_MemThreadLock(&m_SpinLock);

	m_nSmallSize += iBlkSize;
	m_nSmallRawSize += iRawSize;

	uint32 nTotal = m_nSmallSize + m_nLargeSize + m_nSTDSize;
	if( nTotal > m_nPeakSize )
		m_nPeakSize = nTotal;

	if (MEM_THREADSAFE)
		_MemThreadUnlock(&m_SpinLock);

#endif
}

// Increase the size of small memory pool.
void FMemManager::AddSmallMemPoolSize(int iPoolSize)
{
#ifdef FAIRY_DEBUG

	if (MEM_THREADSAFE)
		_MemThreadLock(&m_SpinLock);

	m_nSmallPoolSize += iPoolSize;

	if (MEM_THREADSAFE)
		_MemThreadUnlock(&m_SpinLock);

#endif
}

// Increase the size of the standard allocation
void FMemManager::AddAllocSTDSize(int iRawSize)
{
#ifdef FAIRY_DEBUG

	if (MEM_THREADSAFE)
		_MemThreadLock(&m_SpinLock);

	m_nSTDSize += iRawSize;

	uint32 nTotal = m_nSmallSize + m_nLargeSize + m_nSTDSize;
	if( nTotal > m_nPeakSize )
		m_nPeakSize = nTotal;

	if (MEM_THREADSAFE)
		_MemThreadUnlock(&m_SpinLock);

#endif
}

// Get the usable size of the specified block
uint32 FMemManager::GetUsableSize(void* pMem)
{
	FASSERT(pMem);
	SMemSmallBlock* pBlk = _GetMemBlockInfo(pMem);
	if( pBlk->nFlag == MEM_ALLOC_FLAG_S )
	{
		int iDataSize = m_pSmall->GetDataSize(pBlk->nPoolSlot);
		return iDataSize - MEM_SLOPOVER_SIZE;
	}
	else if( pBlk->nFlag == MEM_ALLOC_FLAG_L )
	{
		SMemLargeBlock* pLargeBlk = (SMemLargeBlock*)((char*)pMem - sizeof(SMemLargeBlock));
		return pLargeBlk->nBlkSize - sizeof(SMemLargeBlock) - MEM_SLOPOVER_SIZE;
	}
	else
	{
		FASSERT(0);
		return 0;
	}
}

// Dump the memory leak information
void FMemManager::DumpMemoryLeaks()
{
#ifdef MEM_DUMP_INFO
	m_pSmall->Dump();
	m_pLarge->Dump();
	m_pSTDTracker->DumpMemReport();
#endif
}

// Dump the current memory usage to file
void FMemManager::DumpCurMemoryUsage(FILE* pFile)
{
#ifdef MEM_DUMP_INFO
	m_pSmall->DumpMemoryBlocks(pFile);
	m_pLarge->DumpMemoryBlocks(pFile);
	m_pSTDTracker->DumpCurMemoryUsage(pFile);
#endif
}

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FMemManagerWrapper
//  
///////////////////////////////////////////////////////////////////////////

FMemManagerWrapper::~FMemManagerWrapper()
{
	if( m_pMemMan )
	{
		m_pMemMan->DumpMemoryLeaks();

		// 在WINDOWS系统中，我们可以通过init_seg(lib)来确保本实例在所有全局变量中最后析构，
		// 但其他系统中我们不能保证，所以只在WINDOWS系统中析构内存管理器
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
		m_pMemMan->~FMemManager();
		_RawMemFree(m_pMemMan);
#endif
	}
}

FMemManager* FMemManagerWrapper::GetMemManager()
{
	if( !m_pMemMan )
		InitMemoryMan();

	return m_pMemMan;
}

bool FMemManagerWrapper::InitMemoryMan()
{
	if( !m_pMemMan )
	{
		m_pMemMan = (FMemManager*)_RawMemAlloc(sizeof(FMemManager));
		new (m_pMemMan) FMemManager();
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////
//  
//  Global Function
//  
///////////////////////////////////////////////////////////////////////////

// Get the memory usage information
void F_GetMemInfo(SMemUsageInfo& info)
{
	FMemManager* pMemMan = l_MemManWrapper.GetMemManager();

#if FAIRY_FORCE_USE_STD_MEMORYALLOC == 1
	info.nPeakSize = pMemMan->GetPeakSize();
	info.nSTDAllocCount = (uint32)pMemMan->GetSTDMemTracker()->GetNumUnits();
	info.nSTDMemoryUsed = pMemMan->GetSTDMemSize();
#else
	info.nPeakSize = pMemMan->GetPeakSize();
	info.nSmallRawSize = pMemMan->GetSmallRawSize();
	info.nSmallSize = pMemMan->GetSmallSize();
	info.nLargeRawSize = pMemMan->GetLargeRawSize();
	info.nLargeSize = pMemMan->GetLargeSize();
	info.nLargeBlockCnt = pMemMan->GetLargeMem()->GetBlockCount();

	info.nSmallPoolSize = 0;
	for(uint32 n=0; n<MEM_BLOCK_LEVEL; n++)
	{
		info.nSmallPoolSize += pMemMan->GetSmallMem()->GetPoolSlotMemSize(n);
	}
#endif

	info.nTempOversizeCnt = pMemMan->GetTempMemMan()->GetOversizeCnt();
	info.nTempGeneralAllocCnt = pMemMan->GetTempMemMan()->GetGeneralAllocCnt();
	info.nMaxTempPool = pMemMan->GetTempMemMan()->GetMaxActivePool();
	info.nCurTempPool = pMemMan->GetTempMemMan()->GetCurActivePool();
}

// Collect the garbage
void F_MemGarbageCollect()
{
#if FAIRY_FORCE_USE_STD_MEMORYALLOC == 0
	l_MemManWrapper.GetMemManager()->GetSmallMem()->GarbageCollect();
#endif
}

// Output the current memory usage states to file
void F_OuputMemoryUsage(const char* filename)
{
#if defined(MEM_DUMP_INFO)
	FILE* pFile = fopen(filename, "w");
	if( !pFile )
	{
		FLOG_ERRORF("F_OutputMemoryUsage, create the log file failed (%s)!", filename);
		return;
	}

	l_MemManWrapper.GetMemManager()->DumpCurMemoryUsage(pFile);
	fclose(pFile);
#endif
}

///////////////////////////////////////////////////////////////////////////
//  
//  内存分配
//  
///////////////////////////////////////////////////////////////////////////

void* BaseAllocCategory::Allocate(size_t sz, EMemoryCategory category)
{
	return l_MemManWrapper.GetMemManager()->Allocate(sz, category);
}

void* BaseAllocCategory::Realloc(void* ptr, size_t sz, EMemoryCategory category)
{
	return l_MemManWrapper.GetMemManager()->Realloc(ptr, sz, category);
}

void BaseAllocCategory::Free(void* ptr, EMemoryCategory category)
{
	l_MemManWrapper.GetMemManager()->Free(ptr, category);
}

///////////////////////////////////////////////////////////////////////////
//  
//  Global new operator
//  
///////////////////////////////////////////////////////////////////////////

void* operator new ( size_t sz ) { return GeneralAllocCategory::Allocate( sz ); }
void* operator new[] ( size_t sz ) { return GeneralAllocCategory::Allocate( sz ); }
void operator delete ( void* ptr ) { GeneralAllocCategory::Free( ptr ); }
void operator delete[] ( void* ptr ) { GeneralAllocCategory::Free( ptr ); }