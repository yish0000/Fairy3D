//------------------------------------------------------------------------
// Name: FlyStringUtil.cpp
// Desc: This file is to implement the class FlyStringUtil.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyStringUtil.h"


// Init the static member variables.
char FlyStringUtil::s_cBuffer[512] = {0};

//------------------------------------------------------------------------
// Get the file's extension name from the whole path.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    String of the whole path.
//------------------------------------------------------------------------
const char* FlyStringUtil::GetFileExt( const char* cFilename )
{
    if( !cFilename ) return NULL;

    strcpy( s_cBuffer,strchr(cFilename,'.') );
    strupr( s_cBuffer );
    return s_cBuffer;
}


//------------------------------------------------------------------------
// Get the file's directory from the whole path.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    String of the whole path.
//------------------------------------------------------------------------
const char* FlyStringUtil::GetDirFromPath( const char* cFilename )
{
    char* pSeperator;
    char* cNewStr;

    if( !cFilename ) return NULL;

    cNewStr = strdup( cFilename );
    strrev( cNewStr );
    pSeperator = strchr( cNewStr,'/' );
    if( !pSeperator ) pSeperator = strchr( cNewStr,'\\' );
    if( pSeperator ) strcpy( s_cBuffer,strrev(pSeperator) );
    else strcpy( s_cBuffer,"" );
    free( cNewStr );

    return s_cBuffer;
}


//------------------------------------------------------------------------
// Get the file's name from the whole path.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    String of the whole path.
//------------------------------------------------------------------------
const char* FlyStringUtil::GetNameFromPath( const char* cFilename )
{
    char* pSeperator;
    char* cNewStr;

    if( !cFilename ) return NULL;

    cNewStr = strdup( cFilename );
    strrev( cNewStr );
    pSeperator = strchr( cNewStr,'/' );
    if( !pSeperator ) pSeperator = strchr( cNewStr,'\\' );
    if( pSeperator )
    {
        *pSeperator = '\0';
        strcpy( s_cBuffer,strrev(cNewStr) );
    }
    else strcpy( s_cBuffer,cFilename );
    free( cNewStr );

    return s_cBuffer;
}