//------------------------------------------------------------------------
// Name: FlyLogManager.h
// Desc: This file define a class to manage the log file for engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_LOG_MANAGER_H__
#define __FLY_LOG_MANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"

#include <vector>
//////////////////////////////////////////////////////////////////////////


class _FLY_EXPORT FlyLogManager
{
    struct sLogNode
    {
        FILE* pLog;     // Pointer to the log file.
        bool bFlush;    // Flush file at each line.
    };

    typedef std::vector<sLogNode> LogNodeList;

protected:
    LogNodeList m_LogNodes;

public:
    // Constructor and destructor.
    FlyLogManager(void);
    ~FlyLogManager(void);

    int CreateLog( const char* cFilename,bool bFlush );
    void CloseLog( int nLog );
    void WriteLine( int nLog,const char* cFormat, ... );
    void WriteError( int nLog,const char* cFormat, ... );
    void Flush( int nLog );
    void WriteText( int nLog,const char* cText );

    // Get the current time.
    void GetTimeStamp( char* cText );

    // Get the singleton object.
    static FlyLogManager& GetSingleton(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_LOG_MANAGER_H__