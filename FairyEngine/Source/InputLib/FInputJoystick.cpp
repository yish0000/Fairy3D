/*
 * ------------------------------------------------------------------------
 *  Name:   FInputJoystick.cpp
 *  Desc:   This file define a base class for joystick device.
 *  Author: Yish
 *  Date:   2015/2/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FInputJoystick.h"

/** Constructor.
*/
FInputJoystick::FInputJoystick() : FInputDevice(IDT_JOYSTICK)
{
}

/** Destructor.
*/
FInputJoystick::~FInputJoystick()
{
}

/** Initialize the joystick.
*/
bool FInputJoystick::Init()
{
	return true;
}

/** Update the joystick
*/
void FInputJoystick::Update()
{
}