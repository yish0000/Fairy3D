/*
 * ------------------------------------------------------------------------
 *  Name:   FInputTouchGuesture.cpp
 *  Desc:   Analysis the gesture information for touch device.
 *  Author: Yish
 *  Date:   2015/2/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FInputTouchGuesture.h"

// Get the raw touch info from list.
static SRawTouchInfo* GetRawTouchInfo(uptrint fingerID, std::list<SRawTouchInfo>& touches)
{
    std::list<SRawTouchInfo>::iterator it = touches.begin();
    for(; it != touches.end(); ++it)
    {
        if( it->fingerID == fingerID )
            return &(*it);
    }
    
    return NULL;
}

///////////////////////////////////////////////////////////////////////////
//
//  class FInputTouchGuestureRecognizer
//
///////////////////////////////////////////////////////////////////////////

FInputTouchGuestureRecognizer::FInputTouchGuestureRecognizer(FInputTouchGuestureManager* pManager)
{
    m_pManager = pManager;
    m_RecognizerState = RECOGNIZER_STATE_POSSIBLE;
    m_GuestureType = GUESTURE_TYPE_INVALID;
    m_nTimeRecord = 0;
}

FInputTouchGuestureRecognizer::~FInputTouchGuestureRecognizer()
{
}

void FInputTouchGuestureRecognizer::Reset()
{
    m_RawTouches.clear();
    m_nTimeRecord = 0;
}

void FInputTouchGuestureRecognizer::TimerUpdate()
{
    if (m_RecognizerState == RECOGNIZER_STATE_ENDED ||
        m_RecognizerState == RECOGNIZER_STATE_CANCELLED ||
        m_RecognizerState == RECOGNIZER_STATE_FAILED)
    {
        ChangeState(RECOGNIZER_STATE_POSSIBLE);
    }
}

void FInputTouchGuestureRecognizer::OnEnterState(FInputTouchGuestureRecognizer::ERecognizerState state)
{
    switch(state)
    {
    case RECOGNIZER_STATE_ENDED:
    case RECOGNIZER_STATE_CANCELLED:
    case RECOGNIZER_STATE_POSSIBLE:
    case RECOGNIZER_STATE_FAILED:
        Reset();
        break;
    case RECOGNIZER_STATE_BEGAN:
    case RECOGNIZER_STATE_CHANGED:
        break;
    default:
        break;
    }
}

bool FInputTouchGuestureRecognizer::ChangeState(FInputTouchGuestureRecognizer::ERecognizerState state)
{
    m_RecognizerState = state;
    OnEnterState(state);
    return true;
}

void FInputTouchGuestureRecognizer::AddRejectRecognizer(FInputTouchGuestureRecognizer *pRecognizer)
{
    FASSERT(pRecognizer && "pRecognizer == null!");
    m_Rejects.push_back(pRecognizer);
}

bool FInputTouchGuestureRecognizer::CheckRejectRecognizer()
{
    RejectRecgnizerList::iterator it = m_Rejects.begin();
    for(; it != m_Rejects.end(); ++it)
    {
        if ((*it)->m_RecognizerState == RECOGNIZER_STATE_CHANGED ||
            (*it)->m_RecognizerState == RECOGNIZER_STATE_ENDED)
        {
            return false;
        }
    }
    
    return true;
}

///////////////////////////////////////////////////////////////////////////
//
//  class FInputTouchGuestureManager
//
///////////////////////////////////////////////////////////////////////////

FInputTouchGuestureManager::FInputTouchGuestureManager(FInputTouch* pTouch)
{
}

FInputTouchGuestureManager::~FInputTouchGuestureManager()
{
}

void FInputTouchGuestureManager::UpdateTimer()
{
}

void FInputTouchGuestureManager::DealTouch(const SRawTouchInfo* pTouches, int iCount)
{
}