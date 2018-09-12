//------------------------------------------------------------------------
// Name: FlyUtil.h
// Desc: This file define some util class to use for the engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_UTIL_H__
#define __FLY_UTIL_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


template <class T>
class FlyRecylingArray
{
protected:
    UINT    m_nMaxElements;     // The max capability.
    UINT    m_nUsedElements;    // Number of the used elements.
    T*      m_pElements;        // Elements used.
    bool*   m_pAlive;           // Active element flag array.
    int     m_nLastNew;         // Last position add the new element.

public:
    // Constructor of the class
    FlyRecylingArray(void)
    {
        m_pElements = NULL;
        m_pAlive = NULL;
        m_nMaxElements = 0;
        m_nUsedElements = 0;
    }

    // Destructor of the class
    ~FlyRecylingArray(void)
    {
        SAFE_DELETE_ARRAY( m_pElements );
        SAFE_DELETE_ARRAY( m_pAlive );
    }

    // Create the array.
    bool Create( int nNumElements )
    {
        SAFE_DELETE_ARRAY( m_pElements );
        SAFE_DELETE_ARRAY( m_pAlive );

        m_pElements = new T[nNumElements];
        if( !m_pElements ) return false;
        m_pAlive = new bool[nNumElements];
        if( !m_pAlive ) return false;

        m_nMaxElements = nNumElements;
        for( int i=0;i<m_nMaxElements;i++ )
        {
            m_pAlive[i] = false;
        }

        memset( m_pElements,0,sizeof(T)*nNumElements );
        m_nUsedElements = 0;
        m_nLastNew = 0;
        return true;
    }

    // Add a new element and return the pointer.
    T* AddNew(void)
    {
        if( m_nUsedElements == m_nMaxElements )
            return NULL;

        // We start to seek the unused element
        // at the last new position. Hope we can find after it.
        int i = m_nLastNew;

        for( int q=0;q<m_nMaxElements;q++ )
        {
            if( !m_pAlive[i] )
                break;
            else
            {
                i++;
                if( i >= m_nMaxElements ) i = 0;
            }
        }

        // Clear the new item.
        memset( &m_pElements[i],0,sizeof(T) );

        // Increase the counter.
        m_pAlive[i] = true;
        m_nUsedElements++;
        m_nLastNew = i;

        return &m_pElements[i];
    }

    // Delete one item from the array.
    void Delete( int nIndex )
    {
        if( m_pAlive[nIndex] )
        {
            m_pAlive[nIndex] = false;
            m_nUsedElements--;
        }
    }

    // Delete one item from the array.
    void Delete( T* pItem )
    {
        if( m_nUsedElements == 0 )
            return;

        int iElem = reinterpret_cast<int>(pItem);
        iElem -= reinterpret_cast<int>(m_pElements);
        int nIndex = iElem / sizeof(T);

        if( m_pAlive[nIndex] )
        {
            m_pAlive[nIndex] = false;
            m_nUsedElements--;
        }
    }

    // Delete all the elements.
    void DeleteAll(void)
    {
        for( int i=0;i<m_nMaxElements;i++ )
        {
            m_pAlive[i] = false;
        }

        m_nUsedElements = 0;
        m_nLastNew = 0;
    }

    // Functions to get the attributes for the template.
    UINT GetNumUsedElements(void) { return m_nUsedElements; }
    UINT GetNumMaxElements(void) { return m_nMaxElements; }
    UINT GetNumFreeElements(void) { return m_nMaxElements-m_nUsedElements; }
    T* GetAt( int nIndex ) { return &m_pElements[nIndex]; }
    bool IsAlive( int nIndex ) { return m_pAlive[nIndex]; }
};

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlyUtil
{
public:
    static int RandNumber( int nMin,int nMax );
    static float RandNumber( float fMin,float fMax );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_UTIL_H__