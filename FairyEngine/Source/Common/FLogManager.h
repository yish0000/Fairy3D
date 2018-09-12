/*
 * ------------------------------------------------------------------------
 *  Name:   FLogManager.h
 *  Desc:   This file define the log manager for engine.
 *  Author: Yish
 *  Date:   2010/10/4
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_LOGMANAGER_H__
#define __FAIRY_LOGMANAGER_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FLogTarget.h"

///////////////////////////////////////////////////////////////////////////

/** Log manager.
*/
class FLogManager
{
    struct SLogTarget
    {
        ELogTarget type;        // Type of log target.
        FLogTarget* pTarget;    // Log target.

		bool operator == (ELogTarget rhs_type) const
		{
			return type == rhs_type;
		}
    };

    typedef std::vector<SLogTarget> LogTargetList;

protected:
    LogTargetList m_LogTargets;     // Log targets

public:
	// Destructor.
    ~FLogManager(void);

	// Create a log file.
	bool CreateLogFile(const char* logfile, const char* strHelloMsg, ELogLevel level = LOG_DETAIL, bool bAppend = false);
	// Close the log file.
	void CloseLogFile();

	// Enable the console log.
	bool EnableConsoleLog(ELogLevel level = LOG_DETAIL);
	// Disable the console log.
	void DisableConsoleLog();

    // Customize log target.
    bool RegisterLogTarget( FLogTarget* pTarget );
    void UnregisterLogTarget( FLogTarget* pTarget );

    // Write some log events.
    void LogEvent( ELogMessageLevel msgLevel,const AString& text );
    void LogEventF( ELogMessageLevel msgLevel,const char* fmtStr, ... );

    // Write some log messages.
	void LogMessage( const AString& text );

    // Get the singleton.
    static FLogManager& GetInstance(void);

private:
    // Constructor.
    FLogManager(void);
	F_DISABLE_COPY_ASSIGNMENT(FLogManager);

	// Destory the log target.
	void DestroyLogTarget( ELogTarget type );
};

// Define some macros.
#define FLOG_MESSAGE( str ) FLogManager::GetInstance().LogMessage( str )
#define FLOG_ERROR( str )   FLogManager::GetInstance().LogEvent( LOGM_ERROR, str )
#define FLOG_WARNING( str ) FLogManager::GetInstance().LogEvent( LOGM_WARNING, str )
#define FLOG_INFO( str )    FLogManager::GetInstance().LogEvent( LOGM_INFO, str )

#define FLOG_ERRORF( str, ... ) FLogManager::GetInstance().LogEventF( LOGM_ERROR, str, ##__VA_ARGS__ )
#define FLOG_WARNINGF( str, ... ) FLogManager::GetInstance().LogEventF( LOGM_WARNING, str, ##__VA_ARGS__ )
#define FLOG_INFOF( str, ... ) FLogManager::GetInstance().LogEventF( LOGM_INFO, str, ##__VA_ARGS__ )

#define FASSERT_LOG(c, str) if(!c) { FASSERT(0); FLOG_ERROR(str); }

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_LOGMANAGER_H__