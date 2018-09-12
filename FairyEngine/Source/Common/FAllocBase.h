/*
 * ------------------------------------------------------------------------
 *  Name:   FAllocBase.h
 *  Desc:   Base class of all classes which want to override the new and delete.
 *  Author: Yish
 *  Date:   2010/10/5
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_ALLOCBASE_H__
#define __FAIRY_ALLOCBASE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBaseType.h"

///////////////////////////////////////////////////////////////////////////

/** Inherit from me, then override the new and delete.
*/
template <class Alloc>
class FAllocBase
{
public:
    explicit FAllocBase(void) {}
    ~FAllocBase(void) {}

	void* operator new ( size_t sz ) { return Alloc::Allocate( sz ); }
    void* operator new[] ( size_t sz ) { return Alloc::Allocate( sz ); }
    void operator delete ( void* ptr ) { Alloc::Free( ptr ); }
    void operator delete[] ( void* ptr ) { Alloc::Free( ptr ); }
    
	// Placement new and delete.
	void* operator new ( size_t sz, void* ptr ) { return ptr; }
	void operator delete ( void* ptr, void* ) {}
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_ALLOCBASE_H__