/*
 * ------------------------------------------------------------------------
 *  Name:   FSysLinux.inl
 *  Desc:   This file implements some linux-related API functions.
 *  Author: Yish
 *  Date:   2013/11/1
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FSys.h"

// Get the current tick count.
uint32 FSysAPI::GetMilliSecond()
{
	timeval tv;
	gettimeofday(&tv, NULL);
	uint64 nTime = ((uint64)tv.tv_sec * 1000000 + tv.tv_usec) / 1000;
	return (uint32)nTime;
}

// Get the current tick count.
uint32 FSysAPI::GetMicroSecond()
{
	timeval tv;
	gettimeofday(&tv, NULL);
	return (uint64)tv.tv_sec * 1000000 + tv.tv_usec;
}