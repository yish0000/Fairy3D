/*
 * ------------------------------------------------------------------------
 *  Name:   FSmartPtr.h
 *  Desc:   This file define the smart pointer template for engine.
 *  Author: Yish
 *  Date:   2011/6/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_SMARTPTR_H__
#define __FAIRY_SMARTPTR_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBaseType.h"
#include "FMemConfig.h"

///////////////////////////////////////////////////////////////////////////

/** Smart pointer.
@note
You cannot construct a smart pointer which point to the local memory.
You cannot construct more than one smart pointers by the same original pointer.
*/
template <class T>
class FSmartPtr
{
public:

	// Default constructor.
    FSmartPtr(void) : m_pPtr(NULL), m_pUsed(NULL)
    {
    }

    // Constructor.
    FSmartPtr( T* ptr )
    {
        if( ptr )
        {
            m_pPtr = ptr;
            m_pUsed = (uint32*)F_MALLOC(sizeof(uint32));
            *m_pUsed = 1;
        }
        else
        {
            m_pPtr = NULL;
            m_pUsed = NULL;
        }
    }

	// Copy constructor.
    FSmartPtr( const FSmartPtr<T>& ptr ) : m_pPtr(ptr.m_pPtr), m_pUsed(ptr.m_pUsed)
    {
        if( m_pUsed ) ++(*m_pUsed);
    }

    // Destructor.
    ~FSmartPtr(void) { Release(); }

    // Operator =
    FSmartPtr& operator = ( const FSmartPtr<T>& ptr )
    {
        if( m_pPtr != ptr.m_pPtr )
        {
			// Release the last pointer.
            Release();

            m_pPtr = ptr.m_pPtr;
            m_pUsed = ptr.m_pUsed;
            ++(*m_pUsed);
        }

        return *this;
    }

    // Operator =
    FSmartPtr& operator = ( const T* ptr )
    {
        if( m_pPtr != ptr )
        {
			// Release the last pointer.
            Release();

            m_pPtr = ptr;
            m_pUsed = (uint32*)F_MALLOC(sizeof(uint32));
            *m_pUsed = 1;
        }

        return *this;
    }

    // Operator ==
    bool operator == ( const T* ptr ) const { return m_pPtr == ptr; }
    bool operator == ( const FSmartPtr<T>& ptr ) const { return m_pPtr == ptr.m_pPtr; }

    // Operator !=
    bool operator != ( const T* ptr ) const { return m_pPtr != ptr; }
    bool operator != ( const FSmartPtr<T>& ptr ) const { return m_pPtr != ptr.m_pPtr; }

    // Operator ->
    T* operator -> (void) { FASSERT(m_pPtr); return m_pPtr; }
    const T* operator -> (void) const { FASSERT(m_pPtr); return m_pPtr; }

    // Operator *
    T& operator * (void) { FASSERT(m_pPtr); return *m_pPtr; }
    const T& operator * (void) const { FASSERT(m_pPtr); return *m_pPtr; }

    // Operator !
    bool operator ! (void) const { return !m_pPtr; }

    // Get the bare pointer.
    operator T* (void) { return m_pPtr; }

    // Get the bare pointer.
    T* GetPtr() { return m_pPtr; }
    const T* GetPtr() const { return m_pPtr; }

	// Is the pointer valid ?
    bool IsValid(void) const { return m_pPtr != NULL; }

private:

    // Release the smart pointer.
    void Release(void)
    {
        if( m_pPtr && m_pUsed && --(*m_pUsed) == 0 )
        {
			// Delete the original pointer.
            F_SAFE_DELETE( m_pPtr );

			// Free the memory of reference count.
            F_FREE( m_pUsed );
        }
    }

    T* m_pPtr;          // Original pointer.
    uint32* m_pUsed;    // Reference count of the original pointer.
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_SMARTPTR_H__