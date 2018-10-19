/*
 * ------------------------------------------------------------------------
 *  Name:   F3DResourceManager.cpp
 *  Desc:   ���ļ�ʵ�����������Դ���������ࡣ
 *  Author: Yish
 *  Date:   2012/8/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"
#include "F3DResourceManager.h"

/** ���������
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

	// Ϊ��Դ����һ�����
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
			m_nFreeIndex = nNextFreeIndex;  // �ƶ�����һ�����о��
			return inst.m_handle;
		}
	}

	// ���������ʵ��ָ��
	TYPE* Dereference(const F3DHandle<TYPE>& handle)
	{
		FASSERT(handle.IsValid());
		int index = handle.GetIndex();

		if (index > m_Handles.size() || m_Handles[index].m_handle != handle)
		{
			// ��Ч�ľ��
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
			// ��Ч�ľ��
			FASSERT(0);
			return NULL;
		}

		return m_Handles[index].pInst;
	}

	// ����ָ�����
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
		m_Handles[index].m_handle = F3DHandle<TYPE>(m_nFreeIndex, 0);   // ָ����һ�����о��

		m_nFreeIndex = index;
		return true;
	}

protected:
	uint32 m_nNextUniqueID;
	uint32 m_nFreeIndex;
	InstanceArray m_Handles;
};