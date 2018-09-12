//------------------------------------------------------------------------
// Name: FlyError.h
// Desc: This file define some functions to handle the error info 
//       for engine.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_ERROR_H__
#define __FLY_ERROR_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"

#include <string>
//////////////////////////////////////////////////////////////////////////


/*
* Base class to store the error Info.
*/
class _FLY_EXPORT FlyErrorInfo
{
protected:
    std::string m_sSource;      // To store the source file name.
    std::string m_sFunc;        // To store the function name.
    long m_nLine;               // To store the line number.
    std::string m_sFullDesc;    // To store the full description of the exception.

public:
    // Constructor and destructor.
    FlyErrorInfo( const char* cDesc,const char* cFunc,const char* cSource,long nLine );
    ~FlyErrorInfo(void);

    // Call this function when catch an exception.
    void ShowErrorDialog(void);

    // Call this function when catch an exception.
    void PrintErrorInfo(void);
};

//////////////////////////////////////////////////////////////////////////

_FLY_EXPORT HRESULT flyTrace( const char* cSource,DWORD nLine,const char* strMsg,HRESULT hr,int type );
_FLY_EXPORT void flyDebugString( const char* cDebugMsg, ... );

#define FLYTRACE_MSG( str )     flyTrace( __FILE__,__LINE__,str,FLY_OK,0 );
#define FLYTRACE_WARNING( str ) flyTrace( __FILE__,__LINE__,str,FLY_OK,1 );
#define FLYTRACE_ERR( str,hr )  flyTrace( __FILE__,__LINE__,str,hr,2 );

#define MAKE_ERRORINFO( desc,func ) throw FlyErrorInfo( desc,func,__FILE__,__LINE__ );

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_ERROR_H__