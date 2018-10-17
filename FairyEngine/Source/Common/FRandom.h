/*
 * ------------------------------------------------------------------------
 *  Name:   FRandom.h
 *  Desc:   This file define the random functions for engine.
 *  Author: Yish
 *  Date:   2011/11/28
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_RANDOM_H__
#define __FAIRY_RANDOM_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBaseType.h"

///////////////////////////////////////////////////////////////////////////

int F_Rand(int nMin, int nMax);
float F_Rand(float fMin, float fMax);
float F_RandUniform();

/** 随机数生成器
*/
class FRandom
{
public:
	FRandom();
	virtual ~FRandom();

	// 指定随机数序列的种子
	bool InitSeed( int iSeed );

	double RandomUniform();
	double RandomGaussian( double mean, double stddev );
	int RandomInt( int iLower, int iUpper );
	float RandomFloat( float fLower, float fUpper );

protected:	// Attributes

    int m_iSeed;

protected:	// Operations

};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_RANDOM_H__