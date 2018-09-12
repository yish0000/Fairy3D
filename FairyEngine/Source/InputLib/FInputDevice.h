/*
 * ------------------------------------------------------------------------
 *  Name:   FInputDevice.h
 *  Desc:   Base class of all the input devices.
 *  Author: Yish
 *  Date:   2015/2/3
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_INPUT_DEVICE_H__
#define __FAIRY_INPUT_DEVICE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FInputTypes.h"

///////////////////////////////////////////////////////////////////////////

class FInputEventDispatcher;

/** Base class of input device.
*/
class FInputDevice : public FGeneralAlloc
{
public:
    FInputDevice(EInputDeviceType type) : m_Type(type), m_bValid(false) {}
    virtual ~FInputDevice() {}
    
	// Initalize the device.
	virtual bool Init() = 0;
	// Update the device.
    virtual void Update() = 0;

	// Get the type of device.
    EInputDeviceType GetDeviceType() const { return m_Type; }
	// Is this device valid ?
    bool IsValid() const { return m_bValid; }

	// Get the event dispatcher.
	FInputEventDispatcher* GetDispatcher();
    
protected:
    EInputDeviceType m_Type;
    bool m_bValid;
};

// Functions to create all the input devices.
template <EInputDeviceType type> FInputDevice* CreateInputDevice();

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_INPUT_DEVICE_H__