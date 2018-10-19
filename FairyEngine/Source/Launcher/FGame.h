/*
 * ------------------------------------------------------------------------
 *  Name:   FGame.h
 *  Desc:   Logical game object.
 *  Author: Yish
 *  Date:   2015/5/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_GAME_H__
#define __FAIRY_GAME_H__

#include "FBase.h"

/** Logical game object.
*/
class FGame
{
public:
	FGame();
	virtual ~FGame();

    // Initialize the game.
    virtual bool Init() = 0;
	// Shutdown the game.
	virtual void Shutdown() = 0;
    // Update the frames.
    virtual void Frame() = 0;
    // Call me when the application enter foreground.
    virtual void OnResume() = 0;
    // Call me when the application enter background.
    virtual void OnPause() = 0;
	// Call me when the application resize the window.
	virtual void OnResizeWindow() = 0;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_GAME_H__