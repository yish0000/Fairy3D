/*
 * ------------------------------------------------------------------------
 *  Name:   FUIInputListener.cpp
 *  Desc:   This file define the input event listener for UI system.
 *  Author: Yish
 *  Date:   2016/2/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FUIInputListener.h"
#include "FInputEngine.h"

FUIInputListener::FUIInputListener()
{
}

FUIInputListener::~FUIInputListener()
{
}

bool FUIInputListener::Init()
{
	FInputEngine::GetInstance().AddEventListener(FINPUT_TOUCH_TAP, this, FINPUTCALLBACK_MAKE(FUIInputListener::OnEventTouchTap));
	return true;
}

void FUIInputListener::OnEventTouchTap(FInputEvent* pEvent)
{
}