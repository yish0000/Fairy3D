/*
 * ------------------------------------------------------------------------
 *  Name:   FLogTarget.cpp
 *  Desc:   This file define some log target for log system.
 *  Author: Yish
 *  Date:   2010/10/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FLogTarget.h"
#include "FSysAPI.h"
#include "FStrUtil.h"
#include <time.h>

/** Get current time stamp.
*/
AString FLogTarget::GetTimeStamp(void) const
{
    char szTime[40];

	tm now;
    time_t ltime = time(0);
    f_localtime(&now, &ltime);
	snprintf(szTime, 40, "[%d/%d/%d %02d:%02d:%02d]: ", now.tm_year + 1900, now.tm_mon + 1,
		now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec );

	return szTime;
}

///////////////////////////////////////////////////////////////////////////
// Output log to file.

/** Constructor.
@Param Name of the log file.
*/
FLogToFile::FLogToFile( const AString& filename, const AString& strHelloMsg, bool bAppend )
{
	AString strDocDir = FSysAPI::GetDocumentDir();
	AString strLogDir = strDocDir + "/Logs/";
	FSysAPI::CreateFolder(strLogDir.c_str());
	AString strLogFile = strLogDir + filename;

	if (bAppend)
		m_pFile = fopen(strLogFile.c_str(), "a");
	else
		m_pFile = fopen(strLogFile.c_str(), "w");
    FASSERT( m_pFile && "Create log file failed!!" );

	char szTime[40];
	tm now;
	time_t ltime = time(0);
	f_localtime(&now, &ltime);
	snprintf(szTime, 40, "[%d/%d/%d %02d:%02d:%02d]", now.tm_year + 1900, now.tm_mon + 1,
		now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec);
	fprintf(m_pFile, "%s\n", strHelloMsg.c_str());
	fprintf(m_pFile, "Created or opened at %s\n\n", szTime);
}

/** Destructor.
*/
FLogToFile::~FLogToFile(void)
{
	LogEvent(LOGM_INFO, "Log file closed successfully!\n");

	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

/** Write a log event.
@Param Level of the message.
@Param Log content.
*/
void FLogToFile::LogEvent( ELogMessageLevel msgLevel, const AString& text )
{
    if( (int)msgLevel >= m_LogLevel )
    {
        fprintf( m_pFile, "%s%s\n", GetTimeStamp().c_str(), text.c_str() );
    }
}

/** Write some messages.
*/
void FLogToFile::LogMessage( const AString& text )
{
    fprintf( m_pFile, "%s\n", text.c_str() );
}


///////////////////////////////////////////////////////////////////////////
// Output log to console.

/** Constructor.
*/
FLogToConsole::FLogToConsole(void)
{
}

/** Destructor.
*/
FLogToConsole::~FLogToConsole(void)
{
}

/** Write a log event.
@Param Level of the message.
@Param Log content.
*/
void FLogToConsole::LogEvent( ELogMessageLevel msgLevel,const AString& text )
{
    if( (int)msgLevel >= m_LogLevel )
    {
        std::cout << GetTimeStamp();
        std::cout << text << std::endl;
    }
}

/** Write some messages.
*/
void FLogToConsole::LogMessage( const AString& text )
{
    std::cout << text << std::endl;
}

///////////////////////////////////////////////////////////////////////////
// Output log to IDE.

/** Constructor.
*/
FLogToDebugger::FLogToDebugger(void)
{
}

/** Destructor.
*/
FLogToDebugger::~FLogToDebugger(void)
{
}

/** Write a log event.
@Param Level of the message.
@Param Log content.
*/
void FLogToDebugger::LogEvent( ELogMessageLevel msgLevel,const AString& text )
{
	if( (int)msgLevel >= m_LogLevel )
	{
		FSysAPI::OutputDebugInfo(text.c_str());
	}
}

/** Write some messages.
*/
void FLogToDebugger::LogMessage( const AString& text )
{
	FSysAPI::OutputDebugInfo(text.c_str());
}