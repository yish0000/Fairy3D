/*
 * ------------------------------------------------------------------------
 *  Name:   FInputJoystick.h
 *  Desc:   This file define a base class for joystick device.
 *  Author: Yish
 *  Date:   2015/2/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_INPUT_JOYSTICK_H__
#define __FAIRY_INPUT_JOYSTICK_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FInputDevice.h"

///////////////////////////////////////////////////////////////////////////

/** Joystick device.
*/
class FInputJoystick : public FInputDevice
{
public:
	FInputJoystick();
	virtual ~FInputJoystick();

	// Initialize the joystick.
	virtual bool Init();
	// Update the joystick.
	virtual void Update();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_INPUT_JOYSTICK_H__