/*
 * ------------------------------------------------------------------------
 *  Name:   FMemThreadLock.h
 *  Desc:   Thread locker for memory manager.
 *  Author: Yish
 *  Date:   2018/10/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Shi. All rights Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_MEMTHREADLOCK_H__
#define __FAIRY_MEMTHREADLOCK_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSysAPI.h"

#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	#include "Platform/Windows/FWinThread.h"
#elif FAIRY_PLATFORM == FAIRY_PLATFORM_MACOS
	#include "Platform/Mac/FMacThread.h"
#elif FAIRY_PLATFORM == FAIRY_PLATFORM_LINUX
	#include "Platform/Linux/FLinuxThread.h"
#endif

///////////////////////////////////////////////////////////////////////////

/** Atomic number.
*/
class FMemThreadAtomic
{
public:
	FMemThreadAtomic() : m_atomic(0) {}

	void Reset() { m_atomic.Reset(); }

	int Fetch_Add(int iNum) { return m_atomic.Fetch_Add(iNum); }
	int Fetch_Set(int iNum) { return m_atomic.Fetch_Set(iNum); }
	int Fetch_CompareSet(int iNum, int iComparand) { return m_atomic.Fetch_CompareSet(iNum, iComparand); }

	int Fetch_Increment() { return m_atomic.Fetch_Increment(); }
	int Fetch_Decrement() { return m_atomic.Fetch_Decrement(); }

	int GetValue() const { return m_atomic.GetValue(); }

private:
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	FWinThreadAtomic m_atomic;
#elif FAIRY_PLATFORM == FAIRY_PLATFORM_MACOS
	FMacThreadAtomic m_atomic;
#else
	FLinuxThreadAtomic m_atomic;
#endif
};

/** Spin lock object.
*/
class FMemThreadSpin
{
public:
	void Lock()
	{
#ifdef FAIRY_DEBUG
		if (m_atomic.Fetch_Set(1) == 1)
		{
			int i;
			for (i = 0; i < 128 && m_atomic.Fetch_Set(1) == 1; i++)
				FSysAPI::Sleep(0);

			if (i >= 128)
			{
				for (i = 0; i < 64 && m_atomic.Fetch_Set(1) == 1; i++)
					FSysAPI::Sleep(50);

				// Wait too long time!!!
				if (i >= 64)
					FASSERT(0);
			}
		}
#else
		while (m_atomic.Fetch_Set(1) == 1)
			FSysAPI::Sleep(0);
#endif
	}

	void Unlock()
	{
		m_atomic.Reset();
	}

protected:
	FMemThreadAtomic m_atomic;
};


/** Mutex object.
*/
class FMemThreadMutex
{
public:
	void Lock()
	{
		m_mutex.Lock();
	}

	void Unlock()
	{
		m_mutex.Unlock();
	}

private:
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	FWinThreadMutex m_mutex;
#elif FAIRY_PLATFORM == FAIRY_PLATFORM_MACOS
	FMacThreadMutex m_mutex;
#else
	FLinuxThreadMutex m_mutex;
#endif
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_MEMTHREADLOCK_H__