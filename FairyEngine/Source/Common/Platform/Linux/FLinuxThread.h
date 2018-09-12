/*
 * ------------------------------------------------------------------------
 *  Name:   FLinuxThread.h
 *  Desc:   Thread on linux platform.
 *  Author: Yish
 *  Date:   2014/3/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_LINUXTHREAD_H__
#define __FAIRY_LINUXTHREAD_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FThread.h"
#include <pthread.h>

///////////////////////////////////////////////////////////////////////////

/** Atomic number.
*/
class FLinuxThreadAtomic : public FThreadAtomic
{
public:
	FLinuxThreadAtomic(int iValue)
		: m_iValue(iValue)
	{
	}

	void Reset();

	int GetValue() const { return m_iValue; }

	int Fetch_Add(int iNum);
	int Fetch_Set(int iNum);
	int Fetch_CompareSet(int iNum, int iComparand);

	int Fetch_Increment();
	int Fetch_Decrement();

protected:
	volatile int m_iValue;
};

/** Mutex object
*/
class FLinuxThreadMutex : public FThreadMutex
{
public:
	FLinuxThreadMutex();
	~FLinuxThreadMutex();

	void Lock();
	void Unlock();

protected:
	pthread_mutex_t	m_mutex;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_LINUXTHREAD_H__