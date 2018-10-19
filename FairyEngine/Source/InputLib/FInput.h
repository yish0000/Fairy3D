/*
 * ------------------------------------------------------------------------
 *  Name:   FInput.h
 *  Desc:   This file include all the headers of input engine.
 *  Author: Yish
 *  Date:   2016/3/2
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_INPUT_H__
#define __FAIRY_INPUT_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FInputTypes.h"

///////////////////////////////////////////////////////////////////////////

class FInput
{
public:
	static bool IsAnyKeyDown();
	static bool IsAnyKeyHeld();
	static bool IsKeyDown(int keycode);
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_INPUT_H__