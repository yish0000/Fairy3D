/*
 * ------------------------------------------------------------------------
 *	Name:   FTimer.cpp
 *  Desc:   This file define a global timer for engine.
 *  Author: Yish
 *  Date:   8/8/2010
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FTimer.h"
#include "FTemplate.h"

#if FAIRY_PLATFORM != FAIRY_PLATFORM_WINDOWS
    #include <sys/time.h>
#endif

/** Get the singleton instance.
*/
FTimer& FTimer::GetInstance(void)
{
    static FTimer obj;
    return obj;
}

/** Constructor.
*/
FTimer::FTimer(void)
	: m_nCurTime(0), m_fElapsed(0.0f), m_fFPS(0.0f), m_nDeltaTime(0), m_fTimeScale(1.0f)
    , m_nMaxFPS(1000), m_fMinElapsed(1.0f / m_nMaxFPS)
	, m_nFrameCount(0)
{
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
    int64 freq;
    if( QueryPerformanceFrequency((LARGE_INTEGER*)&freq) )
    {
        m_fTimeFreq = 1.0f / freq;
        QueryPerformanceCounter( (LARGE_INTEGER*)&m_nLastTime );
        m_bCPUPerf = true;
    }
    else
    {
        m_fTimeFreq = 0.001f;
        m_nLastTime = (int64)GetTickCount64();
        m_bCPUPerf = false;
    }
#else
    timeval tv;
    gettimeofday(&tv, NULL);
    m_nLastTime = ((uint64)tv.tv_sec * 1000000 + tv.tv_usec) / 1000;
	m_fTimeFreq = 0.001f;
#endif

	m_nStartTime = m_nLastTime;
}

/** Destructor.
*/
FTimer::~FTimer(void)
{
}

/** Get the current real time.
*/
int64 FTimer::GetCurrentRealTime() const
{
	int64 curTime = 0;

#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	if (m_bCPUPerf)
		QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
	else
		curTime = (int64)GetTickCount64();
#else
	timeval tv;
	gettimeofday(&tv, NULL);
	curTime = ((uint64)tv.tv_sec * 1000000 + tv.tv_usec) / 1000;
#endif

	return curTime;
}

/** Call me at the start of each frame.
*/
bool FTimer::Update(void)
{
    static long s_nFrames = 0;
    static float s_fAccumulate = 0.0f;

	m_nCurTime = GetCurrentRealTime();
    m_fElapsed = (m_nCurTime - m_nLastTime) * m_fTimeFreq;
    if( m_fElapsed < m_fMinElapsed )
        return false;
    
    m_fElapsed *= m_fTimeScale;
	m_nDeltaTime = (uint32)(m_fElapsed * 1000);
    m_nLastTime = m_nCurTime;

	// Update the engine's current FPS every second.
    s_nFrames++;
    s_fAccumulate += m_fElapsed;

    if( s_fAccumulate > 1.0f )
    {
        m_fFPS = s_nFrames / s_fAccumulate;

        s_fAccumulate = 0.0f;
        s_nFrames = 0;
    }
    
	m_nFrameCount++;
    return true;
}

// Set the max FPS.
void FTimer::SetMaxFPS(uint32 nMaxFPS)
{
    if( nMaxFPS > 1000 )
        nMaxFPS = 1000;

    m_nMaxFPS = nMaxFPS;
    m_fMinElapsed = 1.0f / m_nMaxFPS;
}

// Get the time at the beginning of this frame. (Second)
float FTimer::GetCurTime() const
{
	int64 delta = m_nCurTime - m_nStartTime;
	return (float)(delta * m_fTimeFreq);
}

// Get the real time since the game started. (Second)
float FTimer::GetRealTime() const
{
	int64 delta = GetCurrentRealTime() - m_nStartTime;
	return (float)(delta * m_fTimeFreq);
}