//------------------------------------------------------------------------
// Name: FlyTimer.h
// Desc: A file to define a class control the timer of the engine.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_TIMER_H__
#define __FLY_TIMER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"

//////////////////////////////////////////////////////////////////////////


class _FLY_EXPORT FlyTimer
{
protected:
	LONGLONG	m_nCur_Time;		// Current Time
	LONGLONG	m_nPerf_cnt;		// Performance frequency
	bool		m_blnPerf_Flag;		// Flag for timer to use QueryPerformance
	LONGLONG	m_nLast_Time;		// Time of previous frame
	float		m_fTime_Elapsed;	// Elapsed time of each frame
	float		m_fTime_Scale;		// Scaling factor of time
	float		m_fFPS;				// Current Render FPS

	UCHAR		m_nHH;				// Hours
	UCHAR		m_nMM;				// Minutes
	UCHAR		m_nSS;				// Seconds
	float		m_fClock;			// Sum up milliseconds
	float		m_fFactor;			// Slow-mo or speed up
	float		m_fStamp;			// Timestamp

public:
	/* Constructor and destructor */
	FlyTimer( UCHAR nHH,UCHAR nMM );
	~FlyTimer(void);

	// Update the timer
	void	Update(void);

	void	SetClock( UCHAR nHH,UCHAR nMM );

	char*	GetClock( UCHAR* nHH,UCHAR* nMM,char* pChar );

	// Inline functions
	float	GetTimeStamp(void)			{ return m_fStamp; }
	float	GetElapsed(void)			{ return m_fTime_Elapsed*m_fFactor; }
	float	GetFPS(void)				{ return m_fFPS; }
	float	GetScale(void)				{ return m_fFactor; }
	void	SetScale( float fFactor )	{ m_fFactor = fFactor; }
};

//////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FLY_TIMER_H__