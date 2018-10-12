/*
 * ------------------------------------------------------------------------
 *  Name:   FEvent.cpp
 *  Desc:   This file define the base class for event.
 *  Author: Yish
 *  Date:   2015/2/3
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FEvent.h"
#include "FLogManager.h"

///////////////////////////////////////////////////////////////////////////
//
//  class FEventDispatcher
//
///////////////////////////////////////////////////////////////////////////

void FEventDispatcher::DispatchEvent(FEvent* pEvent)
{
    m_eventQueue.AddEvent(pEvent);
}

void FEventDispatcher::AddEventListener(int iEventType, FEventListener* pListener, FEventCallback callback, int priority)
{
	if (!pListener || !callback)
	{
		FASSERT(0);
		FLOG_ERROR("FEventDispatcher::AddEventListener, pointer of listener and callback function mustn't be null!");
		return;
	}
    
    ListenerMap& listenerMap = m_eventMap[iEventType][priority];
    ListenerMap::iterator it = listenerMap.lower_bound(pListener);
    ListenerMap::iterator ie = listenerMap.end();
    while( it != ie )
    {
        if( it->first != pListener )
            break;
        if( it->second == callback )
            return;
        ++it;
    }
    
    listenerMap.insert(std::make_pair(pListener, callback));
}

void FEventDispatcher::RemoveEventListener(int eventType, FEventListener* listener, FEventCallback callback, int priority)
{
    EventMap::iterator eit = m_eventMap.find(eventType);
    if( eit == m_eventMap.end() )
        return;
    
    PriorityMap::iterator pit = eit->second.find(priority);
    if( pit == eit->second.end() )
        return;
    
    ListenerMap::iterator lie = pit->second.end();
    ListenerMap::iterator lit = pit->second.lower_bound(listener);
    for(; lit != lie; ++lit)
    {
        if( lit->first != listener )
            break;
        
        if( lit->second == callback )
        {
            pit->second.erase(lit);
            
            if( pit->second.empty() )
            {
                eit->second.erase(pit);
                
                if( eit->second.empty() )
                {
                    m_eventMap.erase(eit);
                }
            }
            
            break;
        }
    }
}

void FEventDispatcher::RemoveAllForListener(FEventListener* listener)
{
    EventMap::iterator eit = m_eventMap.begin(), eie = m_eventMap.end();
    while( eit != eie )
    {
        PriorityMap::iterator pit = eit->second.begin(), pie = eit->second.end();
        while( pit != pie )
        {
            if( pit->second.erase(listener) && pit->second.empty() )
            {
                eit->second.erase(pit++);
            }
            else
                ++pit;
        }
        
        if( eit->second.empty() )
        {
            m_eventMap.erase(eit++);
        }
        else
            ++eit;
    }
}

void FEventDispatcher::RemoveAllListeners()
{
    m_eventMap.clear();
}

void FEventDispatcher::OnEvent(FEvent* pEvent)
{
    FASSERT_(pEvent, "pEvent is null!");
    EventMap::iterator eit = m_eventMap.find(pEvent->GetType());
    if( eit == m_eventMap.end() )
    {
		pEvent->Free();
        return;
    }
    
    FASSERT(!eit->second.empty());
    PriorityMap backup = eit->second;
    pEvent->SetSource(this);
    
    PriorityMap::iterator pit = backup.begin(), pie = backup.end();
    for(; pit != pie; ++pit)
    {
        ListenerMap::iterator lit = pit->second.begin(), lie = pit->second.end();
        for(; lit != lie; ++lit)
        {
            (lit->first->*lit->second)(pEvent);
        }
    }

	pEvent->Free();
}

void FEventDispatcher::Update()
{
	m_eventQueue.Update();
}

///////////////////////////////////////////////////////////////////////////
//
//  class FEventProcessQueue
//
///////////////////////////////////////////////////////////////////////////

FEventProcessQueue::FEventProcessQueue(FEventDispatcher* pDispatcher)
{
	m_pDispatcher = pDispatcher;
    m_mutexQueue = FThreadMutex::Create();
}

FEventProcessQueue::~FEventProcessQueue()
{
    F_SAFE_DELETE(m_mutexQueue);
}

void FEventProcessQueue::AddEvent(FEvent* pEvent)
{
    FScopedLock keeper(m_mutexQueue);    
    m_dispatchQueue.push(pEvent);
}

void FEventProcessQueue::Update()
{
    FScopedLock keeper(m_mutexQueue);
    
    while( !m_dispatchQueue.empty() )
    {
        FEvent* pEvent = m_dispatchQueue.front();
        m_dispatchQueue.pop();
        m_pDispatcher->OnEvent(pEvent);
    }
}