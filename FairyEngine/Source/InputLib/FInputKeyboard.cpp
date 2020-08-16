/*
 * ------------------------------------------------------------------------
 *  Name:   FInputKeyboard.cpp
 *  Desc:   This file define the base class of keyboard device.
 *  Author: Yish
 *  Date:   2015/2/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FInputKeyboard.h"
#include "FInputEventTypes.h"
#include "FInputEngine.h"
#include "FSysAPI.h"
#include "FLogManager.h"

// Max count of the combo keys.
#define MAX_COMBO_KEY_COUNT	4
// The minimum time interval to make a combo key event.
#define COMBOKEY_EVENT_INTERVAL 50

// Define some mask for control keys.
#define KEY_CTRL_PRESSED	0x0001
#define KEY_ALT_PRESSED		0x0002
#define KEY_SHIFT_PRESSED	0x0004
#define KEY_CTRLKEY_PRESSED	0x0007

// All kinds of control keys.
static uint32 s_dwComboKey[CONTROL_KEY_NUM] =
{
	KEY_CTRL_PRESSED | KEY_ALT_PRESSED | KEY_SHIFT_PRESSED,
	KEY_CTRL_PRESSED | KEY_ALT_PRESSED,
	KEY_SHIFT_PRESSED | KEY_ALT_PRESSED,
	KEY_SHIFT_PRESSED | KEY_CTRL_PRESSED,
	KEY_SHIFT_PRESSED,
	KEY_CTRL_PRESSED,
	KEY_ALT_PRESSED,
	0,
};

/** Get the index of combo key.
*/
static int GetControlKeyIndex(uint32 nCtrlKey)
{
	int i;
	nCtrlKey &= KEY_CTRLKEY_PRESSED;

	for (i = 0; i < CONTROL_KEY_NUM; i++)
	{
		if (s_dwComboKey[i] == nCtrlKey)
			return i;
	}

	FASSERT(i < CONTROL_KEY_NUM);
	return i;
}

/** Constructor.
*/
FInputKeyboard::FInputKeyboard() : FInputDevice(IDT_KEYBOARD)
{
	m_pMutexMap = FThreadMutex::Create();
	m_pMutexKeyState = FThreadMutex::Create();
	memset(m_bKeyState, 0, sizeof(m_bKeyState));
	memset(m_HK2LKMap, 0, sizeof(m_HK2LKMap));
}

/** Destructor.
*/
FInputKeyboard::~FInputKeyboard()
{
	F_SAFE_DELETE(m_pMutexMap);
	F_SAFE_DELETE(m_pMutexKeyState);
}

/** Initialize the keyboard device.
*/
bool FInputKeyboard::Init()
{
	return true;
}

/** Update the keyboard device.
*/
void FInputKeyboard::Update()
{
	// Handle the combo key event.
	HandleDeferredKey();
}

/** Register a combo key as a logic key.
*/
bool FInputKeyboard::RegisterComboKey(int iLKey, uchar key1, uchar key2 /* = 0 */, uchar key3 /* = 0 */, uchar key4 /* = 0 */)
{
	FScopedLock lock(m_pMutexMap);

	if (m_LK2VKMap.find(iLKey) != m_LK2VKMap.end())
	{
		FLOG_ERRORF("FInputKeyboard::RegisterComboKey, Failed to register the combokey, because the logic key(%s) has existed!", iLKey);
		return false;
	}

	uint32 iComboKey = (key1 << 24) | (key2 << 16) | (key3 << 8) | key4;
	m_CK2LKMap[iComboKey] = iLKey;

	VK_GROUP group;
	group.type = KT_COMBO;
	group.nVK = iComboKey;
	m_LK2VKMap[iLKey] = group;
	return true;
}

/** Register a hot key as a logic key.
*/
bool FInputKeyboard::RegisterHotKey(int iLKey, uchar vk, EControlKeyFlag ctrlKey /* = CONTROL_KEY_NONE */)
{
	FScopedLock lock(m_pMutexMap);

	if (m_LK2VKMap.find(iLKey) != m_LK2VKMap.end())
	{
		FLOG_ERRORF("FInputKeyboard::RegisterHotKey, Failed to register the hotkey, because the logic key(%s) has existed!", iLKey);
		return false;
	}

	FASSERT(ctrlKey >= CONTROL_KEY_SCA && ctrlKey < CONTROL_KEY_NUM);
	m_HK2LKMap[vk][ctrlKey] = iLKey;

	VK_GROUP group;
	group.type = KT_HOT;
	group.nVK = (vk << 16) | s_dwComboKey[ctrlKey];
	m_LK2VKMap[iLKey] = group;
	return true;
}

/** Delete the specified logic key.
*/
void FInputKeyboard::DeleteLogicKey(int iLKey)
{
	FScopedLock lock(m_pMutexMap);

	LK2VKMap::iterator it = m_LK2VKMap.find(iLKey);
	if (it == m_LK2VKMap.end())
	{
		FLOG_ERRORF("FInputKeyboard::DeleteLogicKey, Cannot find the specified logic key(%d)!", iLKey);
		return;
	}

	if (it->second.type == KT_HOT)
	{
		uchar vk = (uchar)(it->second.nVK >> 16);
		uint32 ctrlKey = (it->second.nVK & KEY_CTRLKEY_PRESSED);
		int ctrlIdx = GetControlKeyIndex(ctrlKey);
		FASSERT(ctrlIdx >= CONTROL_KEY_SCA && ctrlIdx < CONTROL_KEY_NUM);
		m_HK2LKMap[vk][ctrlIdx] = 0;
	}
	else if (it->second.type == KT_COMBO)
	{
		CK2LKMap::iterator cit = m_CK2LKMap.find(it->second.nVK);
		FASSERT(cit != m_CK2LKMap.end());
		m_CK2LKMap.erase(cit);
	}

	m_LK2VKMap.erase(it);
}

/** Delete all the logic keys.
*/
void FInputKeyboard::DeleteAllLogicKeys()
{
	FScopedLock lock(m_pMutexMap);
	memset(m_HK2LKMap, 0, sizeof(m_HK2LKMap));
	m_CK2LKMap.clear();
	m_LK2VKMap.clear();
}

/** Get the specified logic key by the combo key.
*/
int FInputKeyboard::GetLogicKeyByComboKey(uchar key1, uchar key2, uchar key3, uchar key4) const
{
	FScopedLock lock(m_pMutexMap);
	uint32 nComboKey = (key1 << 24) | (key2 << 16) | (key3 << 8) | key4;
	CK2LKMap::const_iterator it = m_CK2LKMap.find(nComboKey);
	return it != m_CK2LKMap.end() ? it->second : 0;
}

/** Get the specified logic key by the hot key.
*/
int FInputKeyboard::GetLogicKeyByHotKey(uchar vk, EControlKeyFlag ctrlKey) const
{
	FScopedLock lock(m_pMutexMap);
	FASSERT(ctrlKey >= CONTROL_KEY_SCA && ctrlKey < CONTROL_KEY_NUM);
	return (ctrlKey >= CONTROL_KEY_SCA && ctrlKey < CONTROL_KEY_NUM) ? m_HK2LKMap[vk][ctrlKey] : 0;
}

/** Get the key configuration of the specified logic key.
*/
bool FInputKeyboard::GetLogicKeyInfo(int iLKey, LogicKeyInfo& info) const
{
	FScopedLock lock(m_pMutexMap);

	LK2VKMap::const_iterator it = m_LK2VKMap.find(iLKey);
	if (it == m_LK2VKMap.end())
	{
		FLOG_WARNINGF("FInputKeyboard::GetLogicKeyInfo, Cannot find the specified logic key(%d)!", iLKey);
		return false;
	}

	const VK_GROUP& group = it->second;
	info.type = group.type;
	if (group.type == KT_HOT)
	{
		uchar vk = (uchar)(group.nVK >> 16);
		uint32 nCtrlKeys = (group.nVK & KEY_CTRLKEY_PRESSED);
		int iCtrlIdx = GetControlKeyIndex(nCtrlKeys);
		FASSERT(iCtrlIdx >= CONTROL_KEY_SCA && iCtrlIdx < CONTROL_KEY_NUM);
		info.hot_key.vk = vk;
		info.hot_key.ctrlKey = (EControlKeyFlag)iCtrlIdx;
	}
	else if (group.type == KT_COMBO)
	{
		uchar key1 = (uchar)((group.nVK & 0xff000000) >> 24);
		uchar key2 = (uchar)((group.nVK & 0x00ff0000) >> 16);
		uchar key3 = (uchar)((group.nVK & 0x0000ff00) >> 8);
		uchar key4 = (uchar)(group.nVK & 0x000000ff);
		info.combo_key.key[0] = key1;
		info.combo_key.key[1] = key2;
		info.combo_key.key[2] = key3;
		info.combo_key.key[3] = key4;
	}

	return true;
}

/** Is the specified key pressed ?
*/
bool FInputKeyboard::IsKeyPressed(uchar keycode) const
{
	FScopedLock lock(m_pMutexKeyState);
	return m_bKeyState[keycode];
}

/** Is the specified logic key pressed ?
*/
bool FInputKeyboard::IsLogicKeyPressed(int iLKey) const
{
	FScopedLock lock(m_pMutexMap);

	LK2VKMap::const_iterator it = m_LK2VKMap.find(iLKey);
	if (it != m_LK2VKMap.end())
	{
		const VK_GROUP& gp = it->second;
		if (gp.type == KT_HOT)
		{
			uchar vk = (uchar)(gp.nVK >> 16);
			int nCtrlKeys = (gp.nVK & KEY_CTRLKEY_PRESSED);
			int nCurCtrlKeys = GetCurCtrlKeys();
			if (IsKeyPressed(vk) && (nCtrlKeys & nCurCtrlKeys) == nCtrlKeys)
				return true;
		}
		else if (gp.type == KT_COMBO)
		{
			FASSERT_(gp.nVK, "gp.nVK == 0");
			uchar vk1 = (gp.nVK & 0xff000000) >> 24;
			uchar vk2 = (gp.nVK & 0x00ff0000) >> 16;
			uchar vk3 = (gp.nVK & 0x0000ff00) >> 8;
			uchar vk4 = (gp.nVK & 0x000000ff);
			if (vk1 && !IsKeyPressed(vk1))
				return false;
			if (vk2 && !IsKeyPressed(vk2))
				return false;
			if (vk3 && !IsKeyPressed(vk3))
				return false;
			if (vk4 && !IsKeyPressed(vk4))
				return false;
			return true;
		}
	}

	return false;
}

/** Get the current control key states.
*/
uint32 FInputKeyboard::GetCurCtrlKeys() const
{
	FScopedLock lock(m_pMutexKeyState);

	uint32 nCtrlKeys = 0;

	if (m_bKeyState[FI_KEY_CONTROL])
		nCtrlKeys |= KEY_CTRL_PRESSED;
	else
		nCtrlKeys &= ~KEY_CTRL_PRESSED;

	if (m_bKeyState[FI_KEY_ALT])
		nCtrlKeys |= KEY_ALT_PRESSED;
	else
		nCtrlKeys &= ~KEY_ALT_PRESSED;

	if (m_bKeyState[FI_KEY_SHIFT])
		nCtrlKeys |= KEY_SHIFT_PRESSED;
	else
		nCtrlKeys &= ~KEY_SHIFT_PRESSED;

	return nCtrlKeys;
}

/** Change the key state.
*/
void FInputKeyboard::SetKeyState(int vk, bool bPress)
{
	FASSERT(vk >= 0 && vk < NUM_VIRTUALKEY);
	FScopedLock lock(m_pMutexKeyState);
	m_bKeyState[vk] = bPress;
}

/** Dispatch a keyboard event.
*/
void FInputKeyboard::DispatchKeyEvent(int vk, bool bPress)
{
	FASSERT(vk >= 0 && vk < NUM_VIRTUALKEY);
	FEventDispatcher* pDispatcher = FInputEngine::GetInstance().GetEventDispatcher();

	// Change the key state.
	SetKeyState(vk, bPress);

	// Handle the combo key event.
	uint32 nCurTime = FSysAPI::GetMilliSecond();
	BufferedKeyQueue& eventQueue = bPress ? m_BufferedKeysDown : m_BufferedKeysUp;
	if (eventQueue.size() > 0 && nCurTime - eventQueue.front().nTime >= 50)
		HandleDeferredKey();
	AddDeferredKey(vk, nCurTime, bPress);

	// Dispatch the hot key event.
	uint32 nCurCtrlKeys = GetCurCtrlKeys();
	EControlKeyFlag ctrlFlag = (EControlKeyFlag)GetControlKeyIndex(nCurCtrlKeys);
	int lk = GetLogicKeyByHotKey((uchar)vk, ctrlFlag);
	if (lk)
	{
		if (bPress)
		{
			FInputLogicKeydownEvent* pEvent = FInputLogicKeydownEvent::Create();
			pEvent->logic_key = lk;
			pDispatcher->DispatchEvent(pEvent);
		}
		else
		{
			FInputLogicKeyupEvent* pEvent = FInputLogicKeyupEvent::Create();
			pEvent->logic_key = lk;
			pDispatcher->DispatchEvent(pEvent);
		}
	}

	// Dispatch the normal key event.
	if (bPress)
	{
		FInputKeydownEvent* pEvent = FInputKeydownEvent::Create();
		pEvent->vk = vk;
		pDispatcher->DispatchEvent(pEvent);
	}
	else
	{
		FInputKeyupEvent* pEvent = FInputKeyupEvent::Create();
		pEvent->vk = vk;
		pDispatcher->DispatchEvent(pEvent);
	}
}

/** Add the key event to deferred queue.
*/
void FInputKeyboard::AddDeferredKey(uchar vk, uint32 nTime, bool bDown)
{
	FScopedLock lock(m_pMutexKeyState);
	BufferedKeyQueue& eventQueue = bDown ? m_BufferedKeysDown : m_BufferedKeysUp;

	if (eventQueue.size() >= MAX_COMBO_KEY_COUNT)
		return;

	BufferedKeyQueue::iterator it = eventQueue.begin();
	while (it != eventQueue.end())
	{
		if (it->vk == vk)
			return;

		++it;
	}

	eventQueue.push_back(BufferedKey(vk, nTime));
}

// Handle the combo key event.
void FInputKeyboard::HandleDeferredKey()
{
	FScopedLock lock(m_pMutexKeyState);
	uint32 nCurTime = FSysAPI::GetMilliSecond();
	FEventDispatcher* pDispatcher = FInputEngine::GetInstance().GetEventDispatcher();

	FASSERT(m_BufferedKeysDown.size() <= MAX_COMBO_KEY_COUNT);
	FASSERT(m_BufferedKeysUp.size() <= MAX_COMBO_KEY_COUNT);

	if (m_BufferedKeysDown.size() > 0 && nCurTime - m_BufferedKeysDown.front().nTime > COMBOKEY_EVENT_INTERVAL)
	{
		int idx = 0;
		uchar vks[MAX_COMBO_KEY_COUNT] = { 0 };
		BufferedKeyQueue::iterator it = m_BufferedKeysDown.begin();
		while (it != m_BufferedKeysDown.end())
		{
			vks[idx] = it->vk;
			++it; ++idx;
		}

		int logic_key = GetLogicKeyByComboKey(vks[0], vks[1], vks[2], vks[3]);
		if (logic_key)
		{
			FInputLogicKeydownEvent* pEvent = FInputLogicKeydownEvent::Create();
			pEvent->logic_key = logic_key;
			pDispatcher->DispatchEvent(pEvent);
		}

		m_BufferedKeysDown.clear();
	}

	if (m_BufferedKeysUp.size() > 0 && nCurTime - m_BufferedKeysUp.front().nTime > COMBOKEY_EVENT_INTERVAL)
	{
		int idx = 0;
		uchar vks[MAX_COMBO_KEY_COUNT] = { 0 };
		BufferedKeyQueue::iterator it = m_BufferedKeysUp.begin();
		while (it != m_BufferedKeysUp.end())
		{
			vks[idx] = it->vk;
			++it; ++idx;
		}

		int logic_key = GetLogicKeyByComboKey(vks[0], vks[1], vks[2], vks[3]);
		if (logic_key)
		{
			FInputLogicKeyupEvent* pEvent = FInputLogicKeyupEvent::Create();
			pEvent->logic_key = logic_key;
			pDispatcher->DispatchEvent(pEvent);
		}

		m_BufferedKeysUp.clear();
	}
}