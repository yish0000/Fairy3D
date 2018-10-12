/*
 * ------------------------------------------------------------------------
 *  Name:   FInputDevice.cpp
 *  Desc:   This file implement the base class of InputDevice.
 *  Author: Yish
 *  Date:   2016/3/3
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FInputDevice.h"
#include "FInputEngine.h"

#if (FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS) || \
	(FAIRY_PLATFORM == FAIRY_PLATFORM_MACOS) || \
	(FAIRY_PLATFORM == FAIRY_PLATFORM_LINUX)
    template <> FInputDevice* CreateInputDevice<IDT_JOYSTICK>() { return NULL; }
    template <> FInputDevice* CreateInputDevice<IDT_SENSOR>() { return NULL; }
#elif (FAIRY_PLATFORM == FAIRY_PLATFORM_IOS) || (FAIRY_PLATFORM == FAIRY_PLATFORM_ANDROID)
    template <> FInputDevice* CreateInputDevice<IDT_KEYBOARD>() { return NULL; }
    template <> FInputDevice* CreateInputDevice<IDT_MOUSE>() { return NULL; }
    template <> FInputDevice* CreateInputDevice<IDT_JOYSTICK>() { return NULL; }
#else
	template <> FInputDevice* CreateInputDevice<IDT_KEYBOARD>() { return NULL; }
	template <> FInputDevice* CreateInputDevice<IDT_MOUSE>() { return NULL; }
	template <> FInputDevice* CreateInputDevice<IDT_TOUCH>() { return NULL; }
	template <> FInputDevice* CreateInputDevice<IDT_JOYSTICK>() { return NULL; }
	template <> FInputDevice* CreateInputDevice<IDT_SENSOR>() { return NULL; }
#endif

// Get the pointer of event dispatcher.
FEventDispatcher* FInputDevice::GetDispatcher()
{
	return FInputEngine::GetInstance().GetEventDispatcher();
}