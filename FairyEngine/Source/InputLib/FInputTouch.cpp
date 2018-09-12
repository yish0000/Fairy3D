/*
 * ------------------------------------------------------------------------
 *  Name:   FInputTouch.cpp
 *  Desc:   This file define a base class for Touch device.
 *  Author: Yish
 *  Date:   2015/2/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FInputTouch.h"
#include "FTimer.h"

/** Constructor.
*/
FInputTouch::FInputTouch() : FInputDevice(IDT_TOUCH)
{
    m_cntUpdateTimer.SetPeriod(20);
}

/** Destructor.
*/
FInputTouch::~FInputTouch()
{
}

/** Initialize the Touch pad.
*/
bool FInputTouch::Init()
{
	return true;
}

/** Update the Touch pad.
*/
void FInputTouch::Update()
{
    if( m_pGuesture && m_cntUpdateTimer.IncCounter(FTimer::GetInstance().GetDeltaTime()) )
    {
        m_cntUpdateTimer.Reset();
        m_pGuesture->UpdateTimer();
    }
}