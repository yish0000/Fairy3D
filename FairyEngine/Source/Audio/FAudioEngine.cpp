/*
 * ------------------------------------------------------------------------
 *  Name:   FAudioEngine.cpp
 *  Desc:   Total interface of the audio engine.
 *  Author: Yish
 *  Date:   2016/5/24
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FAudioEngine.h"

/** Get the singleton instance.
*/
FAudioEngine& FAudioEngine::GetInstance()
{
	static FAudioEngine obj;
	return obj;
}

/** Constructor.
*/
FAudioEngine::FAudioEngine()
{
}

/** Destructor.
*/
FAudioEngine::~FAudioEngine()
{
}

/** Initialize the audio engine.
*/
bool FAudioEngine::Init()
{
	return true;
}

/** Shutdown the audio engine.
*/
void FAudioEngine::Shutdown()
{
}

/** Update the audio engine.
*/
void FAudioEngine::Update()
{
}