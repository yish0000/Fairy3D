/*
 * ------------------------------------------------------------------------
 *  Name:   F3DMathCommon.cpp
 *  Desc:   本文件实现了数学库一些公共的函数。
 *  Author: Yish
 *  Date:   2013/1/17
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"
#include "F3DMathCommon.h"

// Inverse sqrt using Newton approximation.
float F3DMath::InvSqrtFast(float v)
{
	float vhalf = 0.5f * v;
	int i = *(int*)&v;
	i = 0x5f3759df - (i >> 1);
	v = *(float*)&i;
	v = v * (1.5f - vhalf * v * v);
	return v;
}