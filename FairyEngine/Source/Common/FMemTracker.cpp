/*
 * ------------------------------------------------------------------------
 *  Name:   FMemTracker.cpp
 *  Desc:   本文件用于实现引擎的内存泄露检测工具类。
 *  Author: Yish
 *  Date:   2010/12/23
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FMemTracker.h"
#include "FStrUtil.h"
#include "FSysAPI.h"
#include "FMemConfig.h"
#include "FMemManager.h"

#include <time.h>

///////////////////////////////////////////////////////////////////////////
//  
//  Define and Macro
//  
///////////////////////////////////////////////////////////////////////////

#define MEM_REPORT FSysAPI::OutputDebugInfo
#define OUTPUT_SEP_LINE MEM_REPORT("--------------------------------------------------------------------------------------------------")

///////////////////////////////////////////////////////////////////////////
// 
//  Simple hash map, is used to get the memory track quickly.
// 
///////////////////////////////////////////////////////////////////////////

/** 构造函数
*/
FMemTracker::AllocUnitTable::AllocUnitTable()
{
    memset( _buckets, 0, sizeof(AllocUnit*)*HASHTABLE_SIZE );
}

/** 添加一条内存分配记录
*/
void FMemTracker::AllocUnitTable::Add( AllocUnit* pUnit )
{
    FASSERT(pUnit);
    size_t hashIdx = (reinterpret_cast<size_t>(pUnit->ptr) >> 4) & (HASHTABLE_SIZE - 1);
    pUnit->next = _buckets[hashIdx];
    _buckets[hashIdx] = pUnit;
}

/** 根据指针获取内存分配记录
*/
FMemTracker::AllocUnit* FMemTracker::AllocUnitTable::GetItem( void* ptr )
{
    FASSERT(ptr);
    size_t hashIdx = (reinterpret_cast<size_t>(ptr) >> 4) & (HASHTABLE_SIZE - 1);
    AllocUnit* p = _buckets[hashIdx];
    while( p )
    {
        if( p->ptr == ptr )
            return p;
        p = p->next;
    }

    return NULL;
}

/** 删除一条指定的记录
*/
bool FMemTracker::AllocUnitTable::Remove( void* ptr )
{
    FASSERT(ptr);
    size_t hashIdx = (reinterpret_cast<size_t>(ptr) >> 4) & (HASHTABLE_SIZE - 1);
    AllocUnit* first = _buckets[hashIdx];
    AllocUnit* cur = first;
    AllocUnit* prev = NULL;
    while( cur )
    {
        if( cur->ptr == ptr )
        {
            if( cur == first )
                _buckets[hashIdx] = cur->next;
            else
                prev->next = cur->next;

            return true;
        }

        prev = cur;
        cur = cur->next;
    }

    return false;
}

void FMemTracker::AllocUnitTable::SetBucket( void* ptr, AllocUnit* pUnit )
{
    FASSERT( ptr && pUnit );
    size_t hashIdx = (reinterpret_cast<size_t>(ptr) >> 4) & (HASHTABLE_SIZE - 1);
    AllocUnit* first = _buckets[hashIdx];
    AllocUnit* cur = first;
    AllocUnit* prev = NULL;
    while( cur )
    {
        if( cur->ptr == ptr )
        {
            if( cur == first )
                _buckets[hashIdx] = pUnit;
            else
                prev->next = pUnit;
            break;
        }

        prev = cur;
        cur = cur->next;
    }
}

/** 清除所有记录
*/
void FMemTracker::AllocUnitTable::Clear()
{
    memset( _buckets, 0, sizeof(AllocUnit*)*HASHTABLE_SIZE );
}

///////////////////////////////////////////////////////////////////////////

/** 构造函数
*/
FMemTracker::FMemTracker(void)
	: m_nNumMemUnits(0), m_nNumMaxMemUnits(0), m_pMemUnits(NULL)
{
}

/** 析构函数
*/
FMemTracker::~FMemTracker(void)
{
    if( m_pMemUnits )
    {
        free( m_pMemUnits );
        m_pMemUnits = NULL;
    }
}

/** 分配单元个数满，扩充内存
*/
void FMemTracker::ReallocTracks()
{
	if( m_nNumMemUnits < m_nNumMaxMemUnits )
		return;

	if( m_nNumMaxMemUnits == 0 )
		m_nNumMaxMemUnits = 1;
	else
		m_nNumMaxMemUnits <<= 1;

	size_t nSize = sizeof(AllocUnit) * m_nNumMaxMemUnits;
	m_pMemUnits = (AllocUnit*)realloc( m_pMemUnits, nSize );
	FASSERT(m_pMemUnits && "Out of memory!");

	// 重构哈希表
	m_HashTab.Clear();
	for( size_t i=0;i<m_nNumMemUnits;i++ )
		m_HashTab.Add( &m_pMemUnits[i] );

	memset( &m_pMemUnits[m_nNumMemUnits], 0, m_nNumMemUnits*sizeof(AllocUnit) );
}

/** 添加一个内存记录
@Param 分配得到的地址
@Param 分配内存单元的大小
*/
void FMemTracker::AddTrack( void* pAddress, size_t nSize )
{
    ReallocTracks();

	// 记录调用栈信息
	const int iMaxDeep = 32;

	uptrint callers[iMaxDeep];
	memset(callers, 0, sizeof(callers));

#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS

    int c = 0;
    uptrint frame_cur = 0;
    
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

#endif

	// 记录新的内存分配记录
    m_pMemUnits[m_nNumMemUnits].ptr = pAddress;
    m_pMemUnits[m_nNumMemUnits].nSize = (uint32)nSize;
	memcpy(m_pMemUnits[m_nNumMemUnits].nCaller, callers, sizeof(uptrint)*MAX_CALLER_LEVEL);
    m_pMemUnits[m_nNumMemUnits].next = NULL;
    m_HashTab.Add( &m_pMemUnits[m_nNumMemUnits] );
    m_nNumMemUnits++;

	FMemManager::GetInstance()->AddAllocSTDSize((int)nSize);
}

/** 移除一个记录
@Param 分配内存单元的地址
*/
void FMemTracker::RemoveTrack( void* pAddress )
{
    AllocUnit* pUnit = m_HashTab.GetItem( pAddress );
    if( pUnit )
    {
		FMemManager::GetInstance()->AddAllocSTDSize(-(int)pUnit->nSize);

        m_HashTab.Remove( pAddress );
        *pUnit = m_pMemUnits[m_nNumMemUnits-1];
        m_HashTab.SetBucket( m_pMemUnits[m_nNumMemUnits-1].ptr, pUnit );
        m_nNumMemUnits--;
    }
}

/** 更改已经存在的分配记录
@Param 旧的分配地址
@Param 新的分配地址
@Param 新的分配单元大小
*/
void FMemTracker::ChangeTrack(void* pAddr, void* pNewAddr, size_t nSize)
{
	// 记录调用栈信息
	const int iMaxDeep = 32;

	uptrint callers[iMaxDeep];
	memset(callers, 0, sizeof(callers));

#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS

    int c = 0;
    uptrint frame_cur = 0;
    
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

#endif

	int iDelta = 0;

	if( pAddr == pNewAddr )
	{
		AllocUnit* pUnit = m_HashTab.GetItem(pAddr);
		if( pUnit )
		{
			iDelta = (int)nSize - (int)pUnit->nSize;
			pUnit->nSize = nSize;
			memcpy(pUnit->nCaller, callers, sizeof(uptrint)*MAX_CALLER_LEVEL);
		}
	}
	else
	{
		AllocUnit* pUnit = m_HashTab.GetItem(pAddr);
		if( pUnit )
		{
			if( !pNewAddr )
			{
				RemoveTrack(pAddr);
				return;
			}
			else
			{
				iDelta = (int)nSize - (int)pUnit->nSize;
				pUnit->ptr = pNewAddr;
				pUnit->nSize = nSize;
				memcpy(pUnit->nCaller, callers, sizeof(uptrint)*MAX_CALLER_LEVEL);
				m_HashTab.Remove(pAddr);
				m_HashTab.Add(pUnit);
			}
		}
		else
		{
			// 添加一个新的记录
			ReallocTracks();

			// 记录新的内存分配记录
			m_pMemUnits[m_nNumMemUnits].ptr = pNewAddr;
			m_pMemUnits[m_nNumMemUnits].nSize = (uint32)nSize;
			memcpy(m_pMemUnits[m_nNumMemUnits].nCaller, callers, sizeof(uptrint)*MAX_CALLER_LEVEL);
			m_pMemUnits[m_nNumMemUnits].next = NULL;
			m_HashTab.Add( &m_pMemUnits[m_nNumMemUnits] );
			m_nNumMemUnits++;
			iDelta = (int)nSize;
		}
	}

	FMemManager::GetInstance()->AddAllocSTDSize(iDelta);
}

/** 清除所有内存分配记录
*/
void FMemTracker::ResetTracks(void)
{
    if( m_pMemUnits )
    {
        free( m_pMemUnits );
        m_pMemUnits = NULL;
    }

    m_nNumMemUnits = 0;
    m_nNumMaxMemUnits = 0;
    m_HashTab.Clear();
}

/** 输出指定分配单元的信息
*/
void FMemTracker::DumpAllocUnit(AllocUnit* pUnit)
{
	FASSERT(pUnit);

	int i;
	char szTemp[64];
	char szMessage[1024];

	OUTPUT_SEP_LINE;

	snprintf(szMessage, 1024, "Memory [%d bytes@0x%p] leak at (", (uint32)pUnit->nSize, pUnit->ptr);
	for(i=0; i<MAX_CALLER_LEVEL; i++)
	{
		if( i == MAX_CALLER_LEVEL - 1 || !pUnit->nCaller[i] )
		{
			snprintf(szTemp, 64, "0x%p)", (void*)pUnit->nCaller[i]);
			strcat(szMessage, szTemp);
			break;
		}
		else
		{
			snprintf(szTemp, 64, "0x%p <- ", (void*)pUnit->nCaller[i]);
			strcat(szMessage, szTemp);
		}
	}

	MEM_REPORT(szMessage);

	// 输出调用栈信息
	MEM_REPORT("Leak %d bytes at:", pUnit->nSize);

	for(i=0; i<MAX_CALLER_LEVEL; i++)
	{
		if( pUnit->nCaller[i] < 0xffff )
			break;

		FSysAPI::ExportSymbolInfo((void*)pUnit->nCaller[i], szMessage);
		MEM_REPORT(szMessage);
	}
}

/** 写出内存泄露检测报告(输出到IDE)
*/
void FMemTracker::DumpMemReport(void)
{
    for( size_t i=0; i<m_nNumMemUnits; i++ )
    {
		AllocUnit* pUnit = &m_pMemUnits[i];
		DumpAllocUnit(pUnit);
    }
}

/** 将当前内存使用状况保存到文件
*/
void FMemTracker::DumpCurMemoryUsage(FILE* pFile)
{
	fprintf(pFile, "-------------------------------------------------------------\n");
	fprintf(pFile, "Standard memory usage:\n");

	for( size_t i=0; i<m_nNumMemUnits; i++ )
	{
		AllocUnit* pUnit = &m_pMemUnits[i];
		fprintf(pFile, "Memory Usage: 0x%p(%d bytes), Call stack: ", pUnit->ptr, (uint32)pUnit->nSize);

		for(int c=0; c<MAX_CALLER_LEVEL; c++)
		{
			if( !pUnit->nCaller[c] )
				break;

			fprintf(pFile, "0x%p ", (void*)pUnit->nCaller[c]);
		}

		fprintf(pFile, "\n");
	}

	fprintf(pFile, "-------------------------------------------------------------\n");
}