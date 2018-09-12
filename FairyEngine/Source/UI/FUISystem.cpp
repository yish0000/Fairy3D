/*
 * ------------------------------------------------------------------------
 *  Name:   FUISystem.cpp
 *  Desc:   本文件用于实现FUISystem类。
 *  Author: Yish
 *  Date:   2010/8/15
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FUISystem.h"
#include "FUIInputListener.h"
#include "FLogManager.h"

/** Constructor.
*/
FUISystem::FUISystem()
{
	m_pInputListener = new FUIInputListener();
}

/** Destructor.
*/
FUISystem::~FUISystem()
{
	Shutdown();
}

/** Get the singleton instance.
*/
FUISystem& FUISystem::GetInstance()
{
	static FUISystem obj;
	return obj;
}

/** Initialize the UI system.
*/
bool FUISystem::Init()
{
	if (!m_pInputListener->Init())
	{
		FLOG_ERROR("FUISystem::Init, Failed to initialize the input event listener!");
		return false;
	}

	return true;
}

/** Shutdown the UI system.
*/
void FUISystem::Shutdown()
{
	F_SAFE_DELETE(m_pInputListener);
}

/** Update the UI system.
 */
void FUISystem::Update()
{
}
