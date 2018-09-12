/*
 * ------------------------------------------------------------------------
 *  Name:   FInputEvent.h
 *  Desc:   This file define the base class for input event.
 *  Author: Yish
 *  Date:   2015/2/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_INPUT_EVENT_H__
#define __FAIRY_INPUT_EVENT_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FInputTypes.h"
#include "FThread.h"

///////////////////////////////////////////////////////////////////////////

class FInputEventDispatcher;

/** Base class of Input event.
 */
class FInputEvent : public FGeneralAlloc
{
public:
    FInputEvent(int type) : m_iType(type), m_pSource(NULL) {}
    virtual ~FInputEvent() {}

	virtual void Free() = 0;
    
    int GetType() const { return m_iType; }
    FInputEventDispatcher* GetSource() { return m_pSource; }
    
    void SetSource(FInputEventDispatcher* pSource) { m_pSource = pSource; }

protected:
    int m_iType;
    FInputEventDispatcher* m_pSource;
};

/** Event pool.
 */
template <class T>
class FInputEventPool
{
    typedef std::set<T*> ActiveEventList;
    typedef std::vector<T*> FreeEventList;
    
public:
    FInputEventPool()
    {
        m_pMutex = FThreadMutex::Create();
    }
    
    ~FInputEventPool()
    {
        Clear();
        F_SAFE_DELETE(m_pMutex);
    }
    
    // Get the singleton instance.
    static FInputEventPool& GetInstance()
    {
        static FInputEventPool obj;
        return obj;
    }
    
    // Clear the pool.
    void Clear()
    {
        FScopedLock lock(m_pMutex);
        for (typename ActiveEventList::iterator it = m_ActiveEvents.begin(); it != m_ActiveEvents.end(); ++it)
        {
            if (*it)
                delete (*it);
        }
        
        for (typename FreeEventList::iterator it2 = m_FreeEvents.begin(); it2 != m_FreeEvents.end(); ++it2)
        {
            if (*it2)
                delete (*it2);
        }
        
        m_ActiveEvents.clear();
        m_FreeEvents.clear();
    }
    
    // Get an event from the pool.
    T* GetEvent()
    {
        FScopedLock lock(m_pMutex);
        
        if (m_FreeEvents.empty())
        {
            T* pEvent = new T();
            if (!pEvent)
                return NULL;
            
            m_ActiveEvents.insert(pEvent);
            return pEvent;
        }
        else
        {
            T* pEvent = m_FreeEvents.back();
            m_FreeEvents.pop_back();
            m_ActiveEvents.insert(pEvent);
            return pEvent;
        }
    }
    
    // Free the specified event.
    void FreeEvent(T* pEvent)
    {
        FASSERT_(pEvent, "pEvent == null");
        FScopedLock lock(m_pMutex);
        
        typename ActiveEventList::iterator it = m_ActiveEvents.find(pEvent);
        if (it == m_ActiveEvents.end())
        {
            FASSERT(0);
            return;
        }
        
        m_ActiveEvents.erase(it);
        
        typename FreeEventList::iterator it2 = std::find(m_FreeEvents.begin(), m_FreeEvents.end(), pEvent);
        if (it2 != m_FreeEvents.end())
        {
            FASSERT(0);
            return;
        }
        
        m_FreeEvents.push_back(pEvent);
    }
    
    // Get the active count.
    size_t GetActiveCount() const
    {
        FScopedLock lock(m_pMutex);
        return m_ActiveEvents.size();
    }
    
    // Get the free count.
    size_t GetFreeCount() const
    {
        FScopedLock lock(m_pMutex);
        return m_FreeEvents.size();
    }
    
protected:
    FThreadMutex* m_pMutex;
    ActiveEventList m_ActiveEvents;
    FreeEventList m_FreeEvents;
};

// Macro which to create an event.
#define FINPUT_CREATE_EVENT(cls) FInputEventPool<cls>::GetInstance().GetEvent()

/** Base class of all the input event listener.
 */
class FInputEventListener : public FGeneralAlloc
{
public:
    virtual void HandleEvent(FInputEvent* pEvent) {}
};

typedef void (FInputEventListener::*FInputEventCallback) (FInputEvent* pEvent);
#define FINPUTCALLBACK_MAKE(func) (FInputEventCallback)(&func)

/** This class can dispatch an input event.
 */
class FInputEventDispatcher : public FGeneralAlloc
{
    typedef std::multimap<FInputEventListener*, FInputEventCallback> ListenerMap;
    typedef std::map<int, ListenerMap, std::greater<int> > PriorityMap;
    typedef std::map<int, PriorityMap> EventMap;
    
    friend class FInputEventProcessQueue;
    
public:
    FInputEventDispatcher() {}
    virtual ~FInputEventDispatcher() {}
    
    void AddEventListener(int eventType, FInputEventListener* listener, FInputEventCallback callback, int priority=0);
    void RemoveEventListener(int eventType, FInputEventListener* listener, FInputEventCallback callback, int priority=0);
    void RemoveAllForListener(FInputEventListener* listener);
    void RemoveAllListeners();
    
    void DispatchEvent(FInputEvent* pEvent);
    
protected:
    EventMap m_eventMap;
    
    // Handle the specified event.
    void OnEvent(FInputEvent* pEvent);
};

/** Event queue.
 */
class FInputEventProcessQueue
{
    struct Event
    {
        FInputEventDispatcher* pDispatcher;
        FInputEvent* pEvent;
    };
    
public:
    FInputEventProcessQueue();
    virtual ~FInputEventProcessQueue();
    
    void AddEvent(FInputEventDispatcher* dispatcher, FInputEvent* pEvent);
    void Update();
    
    static FInputEventProcessQueue& GetInstance();
    
protected:
    std::queue<Event> m_dispatchQueue;
    FThreadMutex* m_mutexQueue;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_INPUT_EVENT_H__