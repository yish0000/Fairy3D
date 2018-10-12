/*
 * ------------------------------------------------------------------------
 *  Name:   FInputEngine.cpp
 *  Desc:   Implementing the FInputEngine class.
 *  Author: Yish
 *  Date:   2015/2/3
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FInputEngine.h"
#include "FInputDevice.h"
#include "FLogManager.h"

///////////////////////////////////////////////////////////////////////////
// Functions to create all the input devices.

#define CREATE_INPUT_DEVICE(type) m_pDevices[type] = CreateInputDevice<type>()

///////////////////////////////////////////////////////////////////////////

/** Constructor.
*/
FInputEngine::FInputEngine() : m_bInited(false)
{
    memset(m_pDevices, 0, sizeof(m_pDevices));
    m_pDispatcher = new FEventDispatcher();
}

/** Destructor.
*/
FInputEngine::~FInputEngine()
{
}

/** Get the instance of input engine.
*/
FInputEngine& FInputEngine::GetInstance()
{
    static FInputEngine obj;
    return obj;
}

/** Initialize the input engine.
 */
bool FInputEngine::Init()
{
	if (m_bInited)
	{
		FLOG_WARNING("FInputEngine::Init, Input engine has been initialized!");
		return true;
	}

	CREATE_INPUT_DEVICE(IDT_KEYBOARD);
	CREATE_INPUT_DEVICE(IDT_MOUSE);
 	CREATE_INPUT_DEVICE(IDT_JOYSTICK);
 	CREATE_INPUT_DEVICE(IDT_TOUCH);
 	CREATE_INPUT_DEVICE(IDT_SENSOR);

	for (int i = 0; i < IDT_COUNT; i++)
	{
		if (!m_pDevices[i])
			continue;

		if (!m_pDevices[i]->Init())
		{
			FLOG_WARNINGF("FInputEngine::Init, Failed to initialize the device(%d)!");
			return false;
		}
	}

	m_bInited = true;
    return true;
}

/** Shutdown the input engine.
*/
void FInputEngine::Shutdown()
{
	for (int i = 0; i < IDT_COUNT; i++)
	{
		F_SAFE_DELETE(m_pDevices[i]);
	}

	F_SAFE_DELETE(m_pDispatcher);
	m_bInited = false;
}

/** Update the input engine.
 */
void FInputEngine::Update()
{
    // Update all the devices.
	for (int i = 0; i < IDT_COUNT; i++)
	{
		if (m_pDevices[i])
			m_pDevices[i]->Update();
	}
    
    // Handle the input event in the queue.
    m_pDispatcher->Update();
}

/** Register a callback function for the specified event.
 */
void FInputEngine::AddEventListener(int eventType, FEventListener* listener, FEventCallback callback, int priority)
{
    m_pDispatcher->AddEventListener(eventType, listener, callback, priority);
}

/** Remove the specified event listener.
 */
void FInputEngine::RemoveEventListener(int eventType, FEventListener* listener, FEventCallback callback, int priority)
{
    m_pDispatcher->RemoveEventListener(eventType, listener, callback, priority);
}

/** Remove the specified event listener.
 */
void FInputEngine::RemoveAllForListener(FEventListener* listener)
{
    m_pDispatcher->RemoveAllForListener(listener);
}

/** Remove all the event listener.
 */
void FInputEngine::RemoveAllListeners()
{
    m_pDispatcher->RemoveAllListeners();
}

/** Get the specified input device.
*/
FInputDevice* FInputEngine::GetInputDevice(EInputDeviceType type) const
{
	if (type < 0 || type >= IDT_COUNT)
	{
		FASSERT(!"type < 0 || type >= IDT_COUNT");
		return NULL;
	}

	return m_pDevices[type];
}

/** Is the specified input device valid ?
*/
bool FInputEngine::IsDeviceValid(EInputDeviceType type) const
{
	FInputDevice* pDevice = GetInputDevice(type);
	return pDevice ? pDevice->IsValid() : false;
}