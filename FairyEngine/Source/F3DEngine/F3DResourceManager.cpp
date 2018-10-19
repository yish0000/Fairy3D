/*
 * ------------------------------------------------------------------------
 *  Name:   F3DResourceManager.cpp
 *  Desc:   本文件实现了引擎的资源管理器基类。
 *  Author: Yish
 *  Date:   2012/8/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"
#include "F3DResourceManager.h"

/** 句柄管理器
*/
template <class TYPE> class F3DHandleManager
{
public:

	struct INSTANCE
	{
		F3DHandle<TYPE> m_handle;
		TYPE* pInst;

		INSTANCE() : m_handle(0, 0), pInst(0) {}
	};

	typedef std::vector<INSTANCE> InstanceArray;

public:
	F3DHandleManager() : m_nNextUniqueID(1), m_nFreeIndex(0) {}

	// 为资源分配一个句柄
	F3DHandle<TYPE> Create(TYPE* pInst)
	{
		if (!pInst)
			return F3DHandle<TYPE>(0, 0);

		if (m_nFreeIndex >= m_Handles.size())
		{
			INSTANCE newHandle;
			newHandle.pInst = pInst;
			newHandle.m_handle = F3DHandle<TYPE>(m_Handles.size(), m_nNextUniqueID++);
			m_Handles.push_back(newHandle);
			m_nFreeIndex++;
			return newHandle.m_handle;
		}
		else
		{
			INSTANCE& inst = m_Handles[m_nFreeIndex];
			uint32 nNextFreeIndex = inst.m_handle.GetIndex();
			inst.pInst = pInst;
			inst.m_handle = F3DHandle<TYPE>(m_nFreeIndex, m_nNextUniqueID++);
			m_nFreeIndex = nNextFreeIndex;  // 移动到下一个空闲句柄
			return inst.m_handle;
		}
	}

	// 解析句柄到实例指针
	TYPE* Dereference(const F3DHandle<TYPE>& handle)
	{
		FASSERT(handle.IsValid());
		int index = handle.GetIndex();

		if (index > m_Handles.size() || m_Handles[index].m_handle != handle)
		{
			// 无效的句柄
			FASSERT(0);
			return NULL;
		}

		return m_Handles[index].pInst;
	}

	const TYPE* Dereference(const F3DHandle<TYPE>& handle) const
	{
		FASSERT(handle.IsValid());
		int index = handle.GetIndex();

		if (index > m_Handles.size() || m_Handles[index].m_handle != handle)
		{
			// 无效的句柄
			FASSERT(0);
			return NULL;
		}

		return m_Handles[index].pInst;
	}

	// 销毁指定句柄
	bool Destroy(const F3DHandle<TYPE>& handle)
	{
		FASSERT(handle.IsValid());
		int index = handle.GetIndex();

		if (index > m_Handles.size() || m_Handles[index].m_handle != handle)
		{
			FASSERT(0);
			return false;
		}

		m_Handles[index].pInst = NULL;
		m_Handles[index].m_handle = F3DHandle<TYPE>(m_nFreeIndex, 0);   // 指向上一个空闲句柄

		m_nFreeIndex = index;
		return true;
	}

protected:
	uint32 m_nNextUniqueID;
	uint32 m_nFreeIndex;
	InstanceArray m_Handles;
};