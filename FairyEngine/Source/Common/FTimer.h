/*
 * ------------------------------------------------------------------------
 *  Name:   FTimer.h
 *  Desc:   This file define a global timer for engine.
 *  Author: Yish
 *  Date:   2011/1/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_TIMER_H__
#define __FAIRY_TIMER_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

/** Timer object.
@remarks
	We can calculate the elapsed time for each frame, and get the FPS.
*/
class FTimer
{
protected:
    int64 m_nCurTime;		// Current time.
    int64 m_nLastTime;		// Time of last frame.
	int64 m_nStartTime;		// Time when we start the game.
	int64 m_nFrameCount;	// The total number of frames that have passed.
    float m_fTimeFreq;		// 1 / Timer's frequency
	float m_fElapsed;		// Time consumed by this frame(unit: sec)
	uint32 m_nDeltaTime;	// Time consumed by this frame(unit: millisecond)
    float m_fFPS;			// Current FPS.
	float m_fTimeScale;		// Scale the timer, the game will be slow or fast.
    uint32 m_nMaxFPS;       // Max FPS.
    float m_fMinElapsed;    // Min elapsed time of each frame.

#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
    bool m_bCPUPerf;		// Do you want to enable CPU high precision timer
#endif

public:
    ~FTimer(void);

	// Update the timer, call me in each game frame.
    bool Update(void);

	// Scale the timer.
	float GetTimeScale() const { return m_fTimeScale; }
	void SetTimeScale(float fScale) { m_fTimeScale = fScale; }

    // Get the elapsed time of this frame.
    float GetElapsed(void) const { return m_fElapsed; }
	uint32 GetDeltaTime() const { return m_nDeltaTime; }

    // Get the current FPS.
    float GetFPS(void) const { return m_fFPS; }
    
    // Get the max FPS.
    uint32 GetMaxFPS() const { return m_nMaxFPS; }
    // Set the max FPS.
    void SetMaxFPS(uint32 nMaxFPS);

	// Get the number of frames that have passed.
	int64 GetFrameCount() const { return m_nFrameCount; }
	// Get the time at the beginning of this frame. (Second)
	float GetCurTime() const;
	// Get the real time since the game started. (Second)
	float GetRealTime() const;

    // Get the singleton.
    static FTimer& GetInstance(void);

private:
    // Constructor.
    FTimer(void);

	// Get the current real time.
	int64 GetCurrentRealTime() const;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_TIMER_H__