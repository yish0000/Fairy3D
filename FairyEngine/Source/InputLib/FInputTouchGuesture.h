/*
 * ------------------------------------------------------------------------
 *  Name:   FInputTouchGuesture.h
 *  Desc:   Analysis the gesture information for touch device.
 *  Author: Yish
 *  Date:   2015/2/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_INPUT_TOUCH_GUESTURE_H__
#define __FAIRY_INPUT_TOUCH_GUESTURE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FInputTypes.h"

///////////////////////////////////////////////////////////////////////////

/** Phase of the raw guesture info.
 */
enum ERawTouchPhase
{
    TOUCHPHASE_BEGAN,
    TOUCHPHASE_MOVED,
    TOUCHPHASE_STATIONARY,
    TOUCHPHASE_ENDED,
    TOUCHPHASE_CANCELLED,
};

/** Raw guesture info.
 */
struct SRawTouchInfo
{
    uptrint fingerID;
    int iPhase;
    float fPosX, fPosY;
    float fDeltaPosX, fDeltaPosY;
    float fDeltaTime;
    int iTapCount;
};

class FInputTouch;
class FInputTouchGuestureManager;

/** Base class of guesture recognizer
 */
class FInputTouchGuestureRecognizer
{
public:
    
    enum ERecognizerState
    {
        RECOGNIZER_STATE_POSSIBLE = 0,
        RECOGNIZER_STATE_BEGAN,
        RECOGNIZER_STATE_CHANGED,
        RECOGNIZER_STATE_ENDED,
        RECOGNIZER_STATE_CANCELLED,
        RECOGNIZER_STATE_FAILED,
    };
    
    typedef std::list<FInputTouchGuestureRecognizer*> RejectRecgnizerList;
    
public:
    FInputTouchGuestureRecognizer(FInputTouchGuestureManager* pManager);
    virtual ~FInputTouchGuestureRecognizer();
    
    virtual void Reset();
    virtual void TimerUpdate();
    
    virtual void OnTouchesBegan(std::list<SRawTouchInfo>& touches) = 0;
    virtual void OnTouchesMoved(std::list<SRawTouchInfo>& touches) = 0;
    virtual void OnTouchesEnded(std::list<SRawTouchInfo>& touches) = 0;
    virtual void OnTouchesCancelled(std::list<SRawTouchInfo>& touches) = 0;
    
    virtual void OnEnterState(ERecognizerState state);
    virtual bool ChangeState(ERecognizerState state);
    
    void AddRejectRecognizer(FInputTouchGuestureRecognizer* pRecognizer);
    bool CheckRejectRecognizer();
    
protected:
    EGuestureType m_GuestureType;
    ERecognizerState m_RecognizerState;
    FInputTouchGuestureManager* m_pManager;
    std::list<SRawTouchInfo> m_RawTouches;
    uint32 m_nTimeRecord;
    RejectRecgnizerList m_Rejects;
};

/** Guesture recognizer manager.
*/
class FInputTouchGuestureManager
{
public:
	FInputTouchGuestureManager(FInputTouch* pDevice);
	virtual ~FInputTouchGuestureManager();

    void UpdateTimer();
    void DealTouch(const SRawTouchInfo* pTouches, int iCount);
    
    
protected:
    FInputTouch* m_pTouchDevice;
    FInputTouchGuestureRecognizer* m_pRecongnizer[GUESTURE_TYPE_NUM];
    FThreadMutex* m_pMutex;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_INPUT_TOUCH_GUESTURE_H__