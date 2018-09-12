/*
 * ------------------------------------------------------------------------
 *  Name:   FAudioEngine.h
 *  Desc:   Total interface of Audio engine.
 *  Author: Yish
 *  Date:   2016/5/24
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_AUDIOENGINE_H__
#define __FAIRY_AUDIOENGINE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

class FAudioEngine : public FGeneralAlloc
{
public:
	~FAudioEngine();

	// Initialize the audio engine.
	virtual bool Init();
	// Shutdown the audio engine.
	virtual void Shutdown();
	// Update the audio engine.
	virtual void Update();

	// Get the singleton instance.
	static FAudioEngine& GetInstance();

protected:
	FAudioEngine();
	F_DISABLE_COPY_ASSIGNMENT(FAudioEngine);
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_AUDIOENGINE_H__