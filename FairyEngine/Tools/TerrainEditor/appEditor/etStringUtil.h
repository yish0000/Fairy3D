//------------------------------------------------------------------------
// Name: etStringUtil.h
// Desc: This file define a class to offer some functions to handle string.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_STRINGUTIL_H__
#define __ET_STRINGUTIL_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include <string.h>
#include <memory.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////


class etStringUtil
{
public:
    static char s_cBuffer[512];

    static const char* GetFileExt( const char* cFilename );
    static const char* GetDirFromPath( const char* cFilename );
    static const char* GetNameFromPath( const char* cFilename );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_STRINGUTIL_H__