/*
 * ------------------------------------------------------------------------
 *  Name:   FEvent.h
 *  Desc:   This file define the base class for event system.
 *  Author: Yish
 *  Date:   2015/2/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_EVENT_H__
#define __FAIRY_EVENT_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FThread.h"

///////////////////////////////////////////////////////////////////////////

class FEventDispatcher;

#define F_IMPLEMENT_EVENT_CLASS(cls) \
	public: \
		static cls* Create() { return new cls(); }

#define F_IMPLEMENT_EVENT_CLASS_USEPOOL(cls) \
	public: \
		static cls* Create() { return FEventPool<cls>::GetInstance().GetEvent(); } \
		void Free() { FEventPool<cls>::GetInstance().FreeEvent(this); }

/** Base class of event.
 */
class FEvent
{
public:
    FEvent(int type) : m_iType(type), m_pSource(NULL) {}
    virtual ~FEvent() {}

	virtual void Free() { delete this; }
    
    int GetType() const { return m_iType; }
    FEventDispatcher* GetSource() { return m_pSource; }
    
    void SetSource(FEventDispatcher* pSource) { m_pSource = pSource; }

protected:
    int m_iType;
    FEventDispatcher* m_pSource;
};

/** Event pool for some frequent events.
 */
template <class T>
class FEventPool
{
    typedef std::set<T*> ActiveEventList;
    typedef std::vector<T*> FreeEventList;
    
public:
    FEventPool()
    {
        m_pMutex = FThreadMutex::Create();
    }
    
    ~FEventPool()
    {
        Clear();

        F_SAFE_DELETE(m_pMutex);
    }
    
    // Get the singleton instance.
    static FEventPool& GetInstance()
    {
        static FEventPool obj;
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

/** Base class of all the  event listener.
 */
class FEventListener
{
public:
    virtual void HandleEvent(FEvent* pEvent) {}
};

/** Event queue.
*/
class FEventProcessQueue
{
public:
	FEventProcessQueue(FEventDispatcher* pDispatcher);
	virtual ~FEventProcessQueue();

	void AddEvent(FEvent* pEvent);
	void Update();

protected:
	FEventDispatcher* m_pDispatcher;
	std::queue<FEvent*> m_dispatchQueue;
	FThreadMutex* m_mutexQueue;
};


typedef void (FEventListener::*FEventCallback) (FEvent* pEvent);
#define FCALLBACK_MAKE(func) (FEventCallback)(&func)

/** This class can dispatch an  event.
 */
class FEventDispatcher
{
    typedef std::multimap<FEventListener*, FEventCallback> ListenerMap;
    typedef std::map<int, ListenerMap, std::greater<int> > PriorityMap;
    typedef std::map<int, PriorityMap> EventMap;
    
    friend class FEventProcessQueue;
    
public:
    FEventDispatcher() : m_eventQueue(this) {}
    virtual ~FEventDispatcher() {}

	void Update();

    void AddEventListener(int eventType, FEventListener* listener, FEventCallback callback, int priority=0);
    void RemoveEventListener(int eventType, FEventListener* listener, FEventCallback callback, int priority=0);
    void RemoveAllForListener(FEventListener* listener);
    void RemoveAllListeners();
    
    void DispatchEvent(FEvent* pEvent);
    
protected:
    EventMap m_eventMap;
	FEventProcessQueue m_eventQueue;
    
    // Handle the specified event.
    void OnEvent(FEvent* pEvent);
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY__EVENT_H__