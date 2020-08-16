/*
 * ------------------------------------------------------------------------
 *  Name:   FLogTarget.h
 *  Desc:   This file define some log target for log system.
 *  Author: Yish
 *  Date:   2010/10/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Studio. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_LOGTARGET_H__
#define __FAIRY_LOGTARGET_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

/** Log output level.
*/
enum ELogLevel
{
    LOG_DETAIL,         // Detail(Output all the detail message)
    LOG_NORMAL,         // Normal(Output all the warnings and errors)
    LOG_CRITICAL,       // Critical(Only output the error message)
};

/** Message level.
*/
enum ELogMessageLevel
{
    LOGM_INFO,          // Normal message.
    LOGM_WARNING,       // Warning message.
    LOGM_ERROR,         // Error message.
};

/** Output target.
*/
enum ELogTarget
{
	LOGT_DEBUGGER = 0,	// Print the messages to IDE.
    LOGT_CONSOLE,		// Print the messages to console view.
    LOGT_LOGFILE,		// Print the messages to log file.
	LOGT_NUM,

    LOGT_USER = 100,    // Log target defined by user.
};

///////////////////////////////////////////////////////////////////////////

/** Base class of log target.
*/
class FLogTarget
{
protected:
    ELogLevel m_LogLevel;       // Current log level.

public:
    FLogTarget(void) : m_LogLevel(LOG_DETAIL) {}
    virtual ~FLogTarget(void) {};

    // Write a log event.
    virtual void LogEvent( ELogMessageLevel msgLevel, const AString& text ) = 0;

    // Write some message.
    virtual void LogMessage( const AString& text ) = 0;

    // Get the current time stamp.
    AString GetTimeStamp(void) const;

	// Get the level of this target.
    ELogLevel GetLogLevel(void) const { return m_LogLevel; }

	// Set the level of this target.
    void SetLogLevel( ELogLevel level ) { m_LogLevel = level; }
};

///////////////////////////////////////////////////////////////////////////

/** Output log to file.
*/
class FLogToFile : public FLogTarget
{
protected:
    FILE* m_pFile;          // Pointer of log file.

public:
    FLogToFile( const AString& filename, const AString& strHelloMsg, bool bAppend );
    ~FLogToFile(void);

    // Write a log event.
    void LogEvent( ELogMessageLevel msgLevel, const AString& text );

    // Write some messages.
    void LogMessage( const AString& text );
};

///////////////////////////////////////////////////////////////////////////

/** Output log to console window.
*/
class FLogToConsole : public FLogTarget
{
public:
    FLogToConsole(void);
    ~FLogToConsole(void);

    // Write a log event.
    void LogEvent( ELogMessageLevel msgLevel, const AString& text );

    // Write some messages.
    void LogMessage( const AString& text );
};

///////////////////////////////////////////////////////////////////////////

/** Output log to IDE.
*/
class FLogToDebugger : public FLogTarget
{
public:
	FLogToDebugger();
	~FLogToDebugger();

	// Write a log event.
	void LogEvent(ELogMessageLevel msgLevel, const AString& text);

	// Write some messages.
	void LogMessage(const AString& text);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_LOGTARGET_H__