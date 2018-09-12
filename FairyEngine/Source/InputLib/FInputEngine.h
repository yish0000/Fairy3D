/*
 * ------------------------------------------------------------------------
 *  Name:   FInputEngine.h
 *  Desc:   Input engine for the Fairy3D.
 *  Author: Yish
 *  Date:   2015/2/3
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_INPUT_ENGINE_H__
#define __FAIRY_INPUT_ENGINE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FInputTypes.h"
#include "FInputEvent.h"

///////////////////////////////////////////////////////////////////////////

class FInputDevice;

/** Input engine.
*/
class FInputEngine : public FGeneralAlloc
{
public:
	virtual ~FInputEngine();

	// Initialize the input engine.
	virtual bool Init();
    // Shutdown the input engine.
    virtual void Shutdown();
    // Update the input engine.
    virtual void Update();

    // Register a callback function for the specified event.
    void AddEventListener(int eventType, FInputEventListener* listener, FInputEventCallback callback, int priority=0);
    // Remove the specified event listener.
    void RemoveEventListener(int eventType, FInputEventListener* listener, FInputEventCallback callback, int priority=0);
    // Remove the specified event listener.
    void RemoveAllForListener(FInputEventListener* listener);
    // Remove all the event listeners.
    void RemoveAllListeners();
    
	// Get the specified device.
	FInputDevice* GetInputDevice(EInputDeviceType type) const;
    // Is the device valid ?
    bool IsDeviceValid(EInputDeviceType type) const;
    // Get the pointer to the event dispatcher.
    FInputEventDispatcher* GetEventDispatcher() { return m_pDispatcher; }

	static FInputEngine& GetInstance();

protected:
	bool m_bInited;
    FInputEventDispatcher* m_pDispatcher;
    FInputDevice* m_pDevices[IDT_COUNT];
    
protected:
	FInputEngine();
	F_DISABLE_COPY_ASSIGNMENT(FInputEngine)
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_INPUT_ENGINE_H__