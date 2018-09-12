/*
 * ------------------------------------------------------------------------
 *  Name:   F3DWindow.h
 *  Desc:   This file define a window interface for 3d engine.
 *  Author: Yish
 *  Date:   2015/2/26
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_WINDOW_H__
#define __F3D_WINDOW_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

class F3DWindow : public FGeneralAlloc
{
public:
    virtual ~F3DWindow() {}
	virtual void* GetWinHandle() const = 0;
	virtual uint32 GetWidth() const = 0;
	virtual uint32 GetHeight() const = 0;
	virtual bool IsFullscreen() const = 0;
    virtual void SetWindowSize(uint32 nWidth, uint32 nHeight) = 0;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_WINDOW_H__