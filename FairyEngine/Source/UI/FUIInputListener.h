/*
 * ------------------------------------------------------------------------
 *  Name:   FUIInputListener.h
 *  Desc:   This file define the input event listener for UI system.
 *  Author: Yish
 *  Date:   2016/2/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_INPUTLISTENER_H__
#define __FUI_INPUTLISTENER_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FInputEventTypes.h"

///////////////////////////////////////////////////////////////////////////

/** Input event listener for UI system.
*/
class FUIInputListener : public FInputEventListener
{
public:
	FUIInputListener();
	virtual ~FUIInputListener();

	bool Init();

	void OnEventMouseLButtonDown(FInputEvent* pEvent);
	void OnEventMouseLButtonUp(FInputEvent* pEvent);
	void OnEventMouseMove(FInputEvent* pEvent);
	void OnEventMouseWheel(FInputEvent* pEvent);

	void OnEventTouchTap(FInputEvent* pEvent);
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FUI_INPUTLISTENER_H__