//------------------------------------------------------------------------
// Name: FlyUtil.h
// Desc: This file define some util class to use for the engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_UTIL_H__
#define __FLY_UTIL_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "stl_headers.h"

//////////////////////////////////////////////////////////////////////////


// Class to Use as the array.
template <typename T,int Size>
class FlyArrayT
{
protected:
    T m_array[Size];

public:
    // Constructor and destructor.
    FlyArrayT(void) {};
    ~FlyArrayT(void) {};

    T GetAt( int nIndex )
    {
        return m_array[nIndex];
    }

    T& operator [] ( int nIndex )
    {
        return m_array[nIndex];
    }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_UTIL_H__