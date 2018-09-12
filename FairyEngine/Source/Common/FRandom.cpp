/*
 * ------------------------------------------------------------------------
 *  Name:   FRandom.cpp
 *  Desc:   本文件实现了引擎生成随机数的接口。
 *  Author: Yish
 *  Date:   2011/11/28
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FRandom.h"

// 设置随机数种子
void F_Randomize( uint32 nSeed )
{
    srand(nSeed);
}

// 获取一个0 ~ 1的随机浮点数
float F_RandUniform()
{
    return rand() / float(RAND_MAX);
}

/** 获取一个指定范围的随机整数
@Param 指定范围的最小整数
@Param 指定范围的最大整数
*/
int F_Rand( int nMin,int nMax )
{
    if( nMin == nMax )
        return nMin;
    else
        return rand() % (abs(nMax-nMin)+1) + nMin;
}

/** 获取一个指定范围的浮点数
@Param 指定范围的最小浮点数
@Param 指定范围的最大浮点数
*/
float F_Rand( float fMin,float fMax )
{
    if( fMin == fMax ) return fMin;

    float fRandom = (float)rand() / (float)RAND_MAX;
    return fRandom * (float)fabs(fMax-fMin) + fMin;
}

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FRandom
//  
///////////////////////////////////////////////////////////////////////////

/** 构造函数
*/
FRandom::FRandom()
{
}

/** 析构函数
*/
FRandom::~FRandom()
{
}