//------------------------------------------------------------------------
// Name: FlyMemMacros.h
// Desc: This file is to define some macros for the memoryManager.
// ----------------------------------------------------------------------
// CopyRight (C) FlySoft Corp. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_MEM_MACROS_H__
#define __FLY_MEM_MACROS_H__


#include "FlyMemManager.h"


//////////////////////////////////////////////////////////////////////////

// Overriden global new/delete operator functions
inline void* operator new ( size_t Size )
{
    return FlyMemManager::instance().op_new_sc( Size );
}

inline void* operator new[] ( size_t Size )
{
    return FlyMemManager::instance().op_new_vc( Size );
}

inline void operator delete ( void* Address )
{
    FlyMemManager::instance().op_del_sc( Address );
}

inline void operator delete[] ( void* Address )
{
    FlyMemManager::instance().op_del_vc( Address );
}

#define new (FlyMemManager::instance().setOwner(__FILE__,__LINE__),false) ? NULL : new
#define delete (FlyMemManager::instance().setOwner(__FILE__,__LINE__),false) ? FlyMemManager::instance().setOwner("",0) : delete
#define malloc(sz) FlyMemManager::instance().allocMem(__FILE__,__LINE__,m_alloc_malloc,sz)
#define calloc(n,sz) FlyMemManager::instance().allocMem(__FILE__,__LINE__,m_alloc_calloc,n*sz)
#define realloc(p,sz) FlyMemManager::instance().rllocMem(__FILE__,__LINE__,m_alloc_realloc,sz,p)
#define free(p) FlyMemManager::instance().dllocMem(__FILE__,__LINE__,m_alloc_free,p)

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_MEM_MACROS_H__