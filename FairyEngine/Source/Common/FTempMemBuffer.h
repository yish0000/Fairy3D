/*
 * ------------------------------------------------------------------------
 *  Name:   FTempMemBuffer.h
 *  Desc:   This file define a memory Buffer which uses temp category.
 *  Author: Yish
 *  Date:   2013/12/31
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_TEMPMEMBUFFER_H__
#define __FAIRY_TEMPMEMBUFFER_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

template <class T>
class FTempMemBuffer
{
public:
	FTempMemBuffer(size_t nSize) : m_nSize(0), m_pBuffer(NULL)
	{
		Resize(nSize);
	}

	~FTempMemBuffer()
	{
		Free();
	}

	// Free the buffer
	void Free()
	{
		if( m_pBuffer )
		{
			F_FREE_TEMP(m_pBuffer);
			m_pBuffer = NULL;
		}
	}

	// Resize the buffer
	void Resize(size_t nNewSize)
	{
		if( nNewSize == m_nSize )
			return;

		Free();

		m_nSize = nNewSize;
		m_pBuffer = nNewSize ? F_MALLOC_TEMP(nNewSize*sizeof(T)) : NULL;
	}

	// Operator []
	T& operator [] (size_t index)
	{
		FASSERT(index < m_nSize);
		return ((T*)m_pBuffer)[index];
	}

	// Operator [] (Read)
	const T& operator [] (size_t index) const
	{
		FASSERT(index < m_nSize);
		return ((const T*)m_pBuffer)[index];
	}

	// Get the buffer pointer
	T* GetBuffer() { return (T*)m_pBuffer; }
	const T* GetBuffer() const { return (const T*)m_pBuffer; }

	// Get the buffer size
	size_t GetSize() const { return m_nSize; }

protected:
	size_t m_nSize;
	void* m_pBuffer;
};

typedef FTempMemBuffer<FBYTE> FTempDataBuffer;
typedef FTempMemBuffer<char> FTempStringBuffer;
typedef FTempMemBuffer<wchar_t> FTempWStringBuffer;

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_TEMPMEMBUFFER_H__