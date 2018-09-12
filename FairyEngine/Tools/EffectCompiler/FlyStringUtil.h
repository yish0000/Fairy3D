//------------------------------------------------------------------------
// Name: FlyStringUtil.h
// Desc: This file define a class to handle the string for engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_STRINGUTIL_H__
#define __FLY_STRINGUTIL_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"
#include "stl_headers.h"

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Class to override the stl string.


//////////////////////////////////////////////////////////////////////////
// Class contain some util functions to handle string.

class _FLY_EXPORT FlyStringUtil
{
public:
    // Constructor and destructor.
    FlyStringUtil(void) { /* Nothing to do! */ };
    ~FlyStringUtil(void) { /* Nothing to do! */ };

    // Static buffer for the functions of this class.
    static char s_cBuffer[512];

    static const char* GetFileExt( const char* cFilename );
    static const char* GetDirFromPath( const char* cFilename );
    static const char* GetNameFromPath( const char* cFilename );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_STRINGUTIL_H__