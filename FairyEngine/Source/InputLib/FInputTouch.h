/*
 * ------------------------------------------------------------------------
 *  Name:   FInputTouch.h
 *  Desc:   This file define a base class for the touch of mobile devices.
 *  Author: Yish
 *  Date:   2015/2/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_INPUT_TOUCH_H__
#define __FAIRY_INPUT_TOUCH_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FInputDevice.h"
#include "FInputTouchGuesture.h"
#include "FCounter.h"

///////////////////////////////////////////////////////////////////////////

/** Touch pad for mobiles.
*/
class FInputTouch : public FInputDevice
{
public:
	FInputTouch();
	virtual ~FInputTouch();

	// Initialize the touch pad device.
	virtual bool Init();
	// Update the touch pad device.
	virtual void Update();
    
protected:
    FCounter m_cntUpdateTimer;
    FInputTouchGuestureManager* m_pGuesture;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_INPUT_TOUCH_H__