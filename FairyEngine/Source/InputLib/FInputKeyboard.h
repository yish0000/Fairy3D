/*
 * ------------------------------------------------------------------------
 *  Name:   FInputKeyboard.h
 *  Desc:   This file define the base class of keyboard device.
 *  Author: Yish
 *  Date:   2015/2/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_INPUT_KEYBOARD_H__
#define __FAIRY_INPUT_KEYBOARD_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FInputDevice.h"

///////////////////////////////////////////////////////////////////////////

/** Keyboard device.
*/
class FInputKeyboard : public FInputDevice
{
public:

	enum
	{
		NUM_VIRTUALKEY = 256,
	};

	enum
	{
		KT_COMBO,	// Combo key is that some keys are pressed at the same time.
		KT_HOT,		// Hot key is that Hold the control key while pressing the other non-control keys, Such as: Ctrl+F, Ctrl+Alt+A...
	};

	struct LogicKeyInfo
	{
		int type;	// Combo key or hot key ?

		union
		{
			struct
			{
				uchar key[4];
			} combo_key;

			struct 
			{
				uchar vk;
				EControlKeyFlag ctrlKey;
			} hot_key;
		};
	};

	struct VK_GROUP
	{
		int type;
		uint32 nVK;
	};

	struct BufferedKey
	{
		uchar vk;
		uint32 nTime;
		BufferedKey(uchar _vk, uint32 _t) : vk(_vk), nTime(_t) {}
	};

	typedef std::map<int, VK_GROUP> LK2VKMap;
	typedef std::map<uint32, int> CK2LKMap;
	typedef std::list<BufferedKey> BufferedKeyQueue;

public:
	FInputKeyboard();
	virtual ~FInputKeyboard();

	// Initialize the keyboard device.
	virtual bool Init();
	// Update the keyboard device.
	virtual void Update();

	// Register a combo key as a logic key.
	// @note Any one to four keys can be composed as a ComboKey when they are pressed at the same time.
	bool RegisterComboKey(int iLKey, uchar key1, uchar key2 = 0, uchar key3 = 0, uchar key4 = 0);
	// Register a hot key as a logic key. Such as, Ctrl+F, Ctrl+Alt+A, Ctrl+Shift+B...
	bool RegisterHotKey(int iLKey, uchar vk, EControlKeyFlag ctrlKey = CONTROL_KEY_NONE);
	// Delete the logic key.
	void DeleteLogicKey(int iLKey);
	// Delete all the logic keys.
	void DeleteAllLogicKeys();
	// Get the logic key by combo key.
	int GetLogicKeyByComboKey(uchar key1, uchar key2, uchar key3, uchar key4) const;
	// Get the logic key by hot key.
	int GetLogicKeyByHotKey(uchar vk, EControlKeyFlag ctrlKey) const;
	// Get the virtual key of the logic key.
	bool GetLogicKeyInfo(int iLKey, LogicKeyInfo& info) const;

	// Is the specified virtual key pressed ?
	bool IsKeyPressed(uchar keycode) const;
	// Is the logic key pressed ?
	bool IsLogicKeyPressed(int iLKey) const;
	
protected:
	LK2VKMap m_LK2VKMap;
	int m_HK2LKMap[NUM_VIRTUALKEY][CONTROL_KEY_NUM];
	CK2LKMap m_CK2LKMap;
	bool m_bKeyState[NUM_VIRTUALKEY];
	BufferedKeyQueue m_BufferedKeysDown;
	BufferedKeyQueue m_BufferedKeysUp;
	FThreadMutex* m_pMutexKeyState;
	FThreadMutex* m_pMutexMap;

	// Change the key state.
	void SetKeyState(int vk, bool bPress);
	// Dispatch a key event.
	void DispatchKeyEvent(int vk, bool bPress);
	// Get the current control key states.
	uint32 GetCurCtrlKeys() const;
	// Add a key event to the buffered queue.
	void AddDeferredKey(uchar vk, uint32 nTime, bool bDown);
	// Handle the deferred key events.
	void HandleDeferredKey();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_INPUT_KEYBOARD_H__