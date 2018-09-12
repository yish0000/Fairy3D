/*
 * ------------------------------------------------------------------------
 *  Name:   FFixArray.h
 *  Desc:   本文件定义了一个模板数组容器。
 *  Author: Yish
 *  Date:   2012/12/22
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_FIXARRAY_H__
#define __FAIRY_FIXARRAY_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBitset.h"
#include "FTemplate.h"

///////////////////////////////////////////////////////////////////////////

/** 活动数组
@remarks
	本容器分配了固定大小的空间存放数组，每个元素对应有一个标记用来标识
	该位置是否已被分配。
@note
	不支持元素的构造和析构。
*/
template <class T>
class FFixArray
{
protected:
	T* m_pElements;				// 数组的指针
	FBitset m_AliveFlags;		// 数组的活动元素标记
	size_t m_nMaxElements;		// 数组的最大容量
	size_t m_nUsedElements;		// 数组已经用了多少个元素
	int m_nLastNew;				// 上一个新元素的位置

public:
	// 构造函数
	FFixArray(void) : m_nMaxElements(0),m_nUsedElements(0),m_pElements(NULL),m_nLastNew(0)
	{
	}

	// 构造函数、构造指定大小的数组
	FFixArray( size_t nNumElements )
	{
		Resize( nNumElements );
	}

	// 析构函数
	~FFixArray(void)
	{
		F_SAFE_FREE( m_pElements );
	}

	// 创建指定数量元素的数组
	bool Resize( size_t nNumElements )
	{
		F_SAFE_FREE( m_pElements );

		// 创建新的数组
		m_pElements = F_MALLOC( sizeof(T)*nNumElements );
		if( !m_pElements || !m_AliveFlags.Resize(nNumElements) )
			return false;

		m_nMaxElements = nNumElements;
		m_nUsedElements = 0;
		m_nLastNew = 0;

		m_AliveFlags.ClearAll();
		memset( m_pElements,0,sizeof(T)*nNumElements );
		return true;
	}

	/** 添加一个新的元素，并返回其指针
	@Param 是否清除新添加的元素
	*/
	T* AddNew(bool bClear = false)
	{
		if( m_nUsedElements == m_nMaxElements )
			return NULL;

		// 我们从上一个新元素开始，希望能在其后
		// 找到空闲位置
		size_t i = m_nLastNew;

		for( size_t q=0;q<m_nMaxElements;q++ )
		{
			if( !m_AliveFlags.Test(i) )
				break;
			else
			{
				i++;
				if( i >= m_nMaxElements ) i = 0;
			}
		}

		// 清除该元素
		if( bClear )
			memset( &m_pElements[i],0,sizeof(T) );

		m_AliveFlags.SetBit(i);
		m_nUsedElements++;
		m_nLastNew = i;

		return &m_pElements[i];
	}

	/** 从数组中删除指定的活动元素
	@Param 要删除元素的索引
	*/
	void Delete( size_t nIndex )
	{
		if( nIndex < m_nMaxElements && m_AliveFlags.Test(nIndex) )
		{
			m_AliveFlags.ClearBit(nIndex);
			m_nUsedElements--;
		}
	}

	/** 从数组中删除指定的活动元素
	@Param 要删除元素的指针
	*/
	void Delete( T* pItem )
	{
		if( m_nUsedElements == 0 )
			return;

		int iElem = reinterpret_cast<int>(pItem);
		iElem -= reinterpret_cast<int>(m_pElements);
		int nIndex = iElem / sizeof(T);

		if( m_AliveFlags.Test(nIndex) )
		{
			m_AliveFlags.ClearBit(nIndex);
			m_nUsedElements--;
		}
	}

	/** 删除所有元素
	*/
	void DeleteAll(void)
	{
		m_nUsedElements = 0;
		m_nLastNew = 0;
		m_AliveFlags.ClearAll();
	}

	/** 获取已经使用的元素个数
	*/
	size_t GetNumUsedElements(void) const { return m_nUsedElements; }

	/** 获取最大元素个数
	*/
	size_t GetNumMaxElements(void) const { return m_nMaxElements; }

	/** 获取空闲的元素个数
	*/
	size_t GetNumFreeElements(void) const { return m_nMaxElements-m_nUsedElements; }

	/** 获取指定位置的元素指针
	@Param 指定元素的索引
	*/
	T* GetAt( size_t nIndex )
	{
		FASSERT( nIndex < m_nMaxElements );
		return &m_pElements[nIndex];
	}

	/** 重载数组的索引运算符
	@Param 指定元素的索引
	*/
	T& operator [] ( size_t nIndex )
	{
		FASSERT( nIndex < m_nMaxElements );
		return m_pElements[nIndex];
	}

	const T& operator [] ( size_t nIndex ) const
	{
		FASSERT( nIndex < m_nMaxElements );
		return m_pElements[nIndex];
	}

	/** 检测指定元素是否是活动的
	@Param 指定元素的索引
	*/
	bool IsAlive( size_t nIndex ) const
	{
		FASSERT( nIndex < m_nMaxElements );
		return m_AliveFlags.Test(nIndex);
	}
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_FIXARRAY_H__