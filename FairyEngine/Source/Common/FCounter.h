/*
 * ------------------------------------------------------------------------
 *  Name:   FCounter.h
 *  Desc:   本文件定义了一个计时器对象。
 *  Author: Yish
 *  Date:   2011/12/3
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_COUNTER_H__
#define __FAIRY_COUNTER_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBaseType.h"

///////////////////////////////////////////////////////////////////////////

/** 计时器对象
*/
class FCounter
{
public:
	FCounter() : m_nCounter(0),m_nPeriod(0),m_bPause(false) {}
	FCounter( uint32 nCounter, uint32 nPeriod ) : m_nCounter(nCounter), m_nPeriod(nPeriod), m_bPause(false) {}
	virtual ~FCounter() {}

	// 获取该计时器周期
    uint32 GetPeriod() const { return m_nPeriod; }
	// 设置计时器周期
    void SetPeriod( uint32 nPeriod ) { m_nPeriod = nPeriod; }

	// 获取计时器当前时间
    uint32 GetCounter() const { return m_nCounter; }
	// 设置计时器当前时间
    void SetCounter( uint32 nCounter ) { m_nCounter = nCounter; }

	// 增加计时器时间
    bool IncCounter( uint32 nCounter = 1 )
	{
		if( !m_bPause )
			m_nCounter += nCounter;
		return (m_nCounter >= m_nPeriod);
	}

    // 减少计时器时间
    void DecCounter( uint32 nCounter = 1 )
	{
		if( !m_bPause )
		{
			if( m_nCounter <= nCounter )
				m_nCounter = 0;
			else
				m_nCounter -= nCounter;
		}
	}

	// 重置计时器时间到开始或结束
	void Reset( bool bFull = false ) { m_nCounter = bFull ? m_nPeriod : 0; }
    bool IsFull() const { return (m_nCounter >= m_nPeriod); }

	// 暂停计时器
    void Pause( bool bPause ) { m_bPause = bPause; }
    bool IsPause() const { return m_bPause; }

protected:
    bool m_bPause;			// 计时器是否被暂停
    uint32 m_nCounter;		// 当前时间的计数
    uint32 m_nPeriod;		// 如果时间计数大于等于周期，则IsFull
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_COUNTER_H__