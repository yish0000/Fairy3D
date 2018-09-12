/*
 * ------------------------------------------------------------------------
 *  Name:   FLinuxThread.cpp
 *  Desc:   Thread on linux platform.
 *  Author: Yish
 *  Date:   2014/3/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FLinuxThread.h"

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FLinuxThreadAtomic
//  
///////////////////////////////////////////////////////////////////////////

int FLinuxThreadAtomic::Fetch_Add(int iNum)
{
	return __sync_fetch_and_add(&m_iValue, iNum);
}

int FLinuxThreadAtomic::Fetch_Set(int iNum)
{
	return __sync_lock_test_and_set(&m_iValue, iNum);
}

int FLinuxThreadAtomic::Fetch_CompareSet(int iNum, int iComparand)
{
	return __sync_val_compare_and_swap(&m_iValue, iComparand, iNum);
}

int FLinuxThreadAtomic::Fetch_Increment()
{
	return __sync_add_and_fetch(&m_iValue, 1);
}

int FLinuxThreadAtomic::Fetch_Decrement()
{
	return __sync_sub_and_fetch(&m_iValue, 1);
}

void FLinuxThreadAtomic::Reset()
{
	__sync_lock_release(&m_iValue);
}

FThreadAtomic* FThreadAtomic::Create(int iValue)
{
	return new FLinuxThreadAtomic(iValue);
}

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FLinuxThreadMutex
//  
///////////////////////////////////////////////////////////////////////////

FLinuxThreadMutex::FLinuxThreadMutex()
{
	pthread_mutexattr_t ma;
	pthread_mutexattr_init(&ma);
	pthread_mutexattr_settype(&ma, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m_mutex, &ma);
	pthread_mutexattr_destroy(&ma);
}

FLinuxThreadMutex::~FLinuxThreadMutex()
{
    pthread_mutex_destroy(&m_mutex);
}

void FLinuxThreadMutex::Lock()
{
    pthread_mutex_lock(&m_mutex);
}

void FLinuxThreadMutex::Unlock()
{
    pthread_mutex_unlock(&m_mutex);
}

FThreadMutex* FThreadMutex::Create()
{
	return new FLinuxThreadMutex();
}