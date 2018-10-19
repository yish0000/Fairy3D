/*
 * ------------------------------------------------------------------------
 *  Name:   FApplication.cpp
 *  Desc:   Base class of the application.
 *  Author: Yish
 *  Date:   2015/5/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FApplication.h"

#include "FCommon.h"
#include "F3DEngine.h"
#include "FAudioEngine.h"
#include "FInputEngine.h"
#include "FUISystem.h"

extern FGame* g_pFairyGame;

/** Get the singleton instance.
*/
FApplication* FApplication::GetInstance()
{
	static FApplication obj;
	return &obj;
}

/** Constructor.
*/
FApplication::FApplication() : m_bInited(false)
{
}

/** Destructor.
*/
FApplication::~FApplication()
{
	OnAppShutdown();
}

/** Initialize the application.
*/
bool FApplication::OnAppInit()
{
	if (m_bInited)
		return true;

	if (!g_pFairyGame)
	{
		FLOG_ERROR("FApplication::OnAppInit, Application failed to initialize, because 'g_pFairyGame' must not be null!");
		return false;
	}

	// Initialize the random seed.
	srand((uint32)time(0));

	// Create a log file for the application.
	FLogManager::GetInstance().CreateLogFile("fairy.log", "FairyEngine Framework Log.");
    
    // Record the main thread id.
    FThread::RecordMainThreadID();

	// Set the work directory for file system.
	FVFileSystem::GetInstance().SetBaseDir(FSysAPI::GetCurrentDir());
	// Load the package which contains all the resources for 3d engine.
	FVFileSystem::GetInstance().LoadPack("package/f3d.fpk", "f3d");

	// Initialize the 3d engine.
	if (!F3DEngine::GetInstance().Initialize())
	{
		FLOG_ERROR("FApplication::OnAppInit, Failed to initialize the 3d engine!");
		return false;
	}

	// Initialize the audio engine.
	if (!FAudioEngine::GetInstance().Init())
	{
		FLOG_ERROR("FApplication::OnAppInit, Failed to initialize the audio engine!");
		return false;
	}

	// Initialize the input engine.
	if (!FInputEngine::GetInstance().Init())
	{
		FLOG_ERROR("FApplication::OnAppInit, Failed to initialize the input engine!");
		return false;
	}

	// Initialize the UI system.
	if (!FUISystem::GetInstance().Init())
	{
		FLOG_ERROR("FApplication::OnAppInit, Failed to initialize the UI system!");
		return false;
	}

	// Initialize the game object.
	if (!g_pFairyGame->Init())
	{
		FLOG_ERROR("FApplication::OnAppInit, Application failed to initialize, because we failed to intialize the game!");
		return false;
	}

	m_bInited = true;
	return true;
}

/** Shutdown the application.
*/
void FApplication::OnAppShutdown()
{
	if (!m_bInited)
		return;

	// First we shutdown the game.
	FASSERT(g_pFairyGame);
	g_pFairyGame->Shutdown();

	// Shutdown the UI system.
	FUISystem::GetInstance().Shutdown();

	// Shutdown the input engine.
	FInputEngine::GetInstance().Shutdown();

	// Shutdown the audio engine.
	FAudioEngine::GetInstance().Shutdown();

	// Shutdown the 3d engine.
	F3DEngine::GetInstance().Shutdown();

	m_bInited = false;
}

void FApplication::OnFrame()
{
	if (!m_bInited)
		return;

	// Update the timer at the start of each frame.
	if( !FTimer::GetInstance().Update() )
    {
        // Discard this frame.
        return;
    }

	// Update the game object.
	FASSERT(g_pFairyGame);
	g_pFairyGame->Frame();

	// Update the 3d engine.
	F3DEngine::GetInstance().Frame();

	// Update the audio engine.
	FAudioEngine::GetInstance().Update();

	// Update the input engine.
	FInputEngine::GetInstance().Update();
}

void FApplication::OnResizeWindow(uint32 nWidth, uint32 nHeight)
{
	FASSERT(g_pFairyGame);
	g_pFairyGame->OnResizeWindow();
}

void FApplication::OnAppEnterBackground()
{
	FASSERT(g_pFairyGame);
	g_pFairyGame->OnPause();
}

void FApplication::OnAppEnterForeground()
{
	FASSERT(g_pFairyGame);
	g_pFairyGame->OnResume();
}