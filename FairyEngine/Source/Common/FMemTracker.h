/*
 * ------------------------------------------------------------------------
 *  Name:   FMemTracker.h
 *  Desc:   本文件为引擎定义了一个内存泄露的检测工具类。
 *  Author: Yish
 *  Date:   2010/12/23
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_MEMTRACKER_H__
#define __FAIRY_MEMTRACKER_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBaseType.h"

///////////////////////////////////////////////////////////////////////////

/** 内存泄露检测
*/
class FMemTracker
{
public:

	enum { MAX_CALLER_LEVEL = 8 };

    // 内存分配单元
    struct AllocUnit
    {
        void* ptr;							// 内存单元地址
        size_t nSize;						// 内存单元大小
		uptrint nCaller[MAX_CALLER_LEVEL];	// 调用堆栈
		AllocUnit* next;					// 下一个分配记录
    };

    // 这里实现了一个简单的哈希表，用于快速查找内存分配记录
    class AllocUnitTable
    {
        enum
        {
            HASHTABLE_SIZE = 1 << 16
        };

    public:
        AllocUnitTable();

        // 添加一条记录到哈希表
        void Add( AllocUnit* pUnit );

        // 获取指定内存记录的索引
        AllocUnit* GetItem( void* ptr );

        // 移除指定的内存分配记录
        bool Remove( void* ptr );

        // 设置新位置
        void SetBucket( void* ptr, AllocUnit* pUnit );

        // 清除哈希表
        void Clear();

    private:
        AllocUnit* _buckets[HASHTABLE_SIZE];
    };

protected:
    AllocUnit* m_pMemUnits;		// 内存分配记录列表
	AllocUnitTable m_HashTab;	// 内存分配记录哈希表
    size_t m_nNumMemUnits;      // 内存分配记录的个数
    size_t m_nNumMaxMemUnits;   // 内存分配记录的空间大小

public:
    FMemTracker(void);
    ~FMemTracker(void);

    // 添加一个内存分配记录
    void AddTrack( void* pAddress, size_t nSize );

    // 移除一条内存分配记录
    void RemoveTrack( void* pAddress );

	// 更新已有的内存分配记录
	void ChangeTrack(void* pAddr, void* pNewAddr, size_t nSize);

    // 清除所有内存分配记录
    void ResetTracks(void);

    // 获取指定的内存分配记录
    AllocUnit* GetAllocUnit( size_t index )
    {
        FASSERT( index < m_nNumMemUnits );
        return &m_pMemUnits[index];
    }

    // 根据指针获取内存分配记录
    AllocUnit* GetAllocUnitByPtr( void* ptr )
    {
        FASSERT(ptr);
        return m_HashTab.GetItem(ptr);
    }

    // 获取内存分配记录的数量
    size_t GetNumUnits(void) const { return m_nNumMemUnits; }

    // 写内存泄露报告文件
    void DumpMemReport(void);

	// 将当前内存使用状态保存到文件
	void DumpCurMemoryUsage(FILE* pFile);

protected:

	// 扩充分配单元数组
	void ReallocTracks();

	// 输出指定分配单元的信息
	void DumpAllocUnit(AllocUnit* pUnit);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_MEM_MANAGER_H__