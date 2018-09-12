/*
 * ------------------------------------------------------------------------
 *  Name:   FLogManager.cpp
 *  Desc:   Log Manager for the engine.
 *  Author: Yish
 *  Date:   2010/10/4
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FLogManager.h"

/** Get the singleton object.
*/
FLogManager& FLogManager::GetInstance(void)
{
    static FLogManager obj;
    return obj;
}

/** Constructor.
*/
FLogManager::FLogManager(void)
{
	SLogTarget target;
	target.type = LOGT_DEBUGGER;
	target.pTarget = new FLogToDebugger();
	target.pTarget->SetLogLevel(LOG_NORMAL);
	m_LogTargets.push_back(target);
}

/** Destructor.
*/
FLogManager::~FLogManager(void)
{
    LogTargetList::iterator itr;
    for( itr=m_LogTargets.begin();itr!=m_LogTargets.end();itr++ )
    {
        if( itr->type < LOGT_USER )
        {
            F_SAFE_DELETE( itr->pTarget );
        }
    }
}

/** Enable logging to the console.
@Param Log output level.
*/
bool FLogManager::EnableConsoleLog(ELogLevel level /* = LOG_DETAIL */)
{
	LogTargetList::const_iterator it;
	if( std::find(m_LogTargets.begin(), m_LogTargets.end(), LOGT_CONSOLE) != m_LogTargets.end() )
		return false;

	SLogTarget target;
	target.type = LOGT_CONSOLE;
	target.pTarget = new FLogToConsole();
	target.pTarget->SetLogLevel(level);
	m_LogTargets.push_back(target);
	return true;
}

/** Create a log file.
@Param Name of the log file.
@Param Some description of the log file.
@Param Log output level.
@Param Append the log file ?
*/
bool FLogManager::CreateLogFile(const char* logfile, const char* strHelloMsg, ELogLevel level /* = LOG_DETAIL */, bool bAppend /* = false */)
{
	LogTargetList::const_iterator it;
	if( std::find(m_LogTargets.begin(), m_LogTargets.end(), LOGT_LOGFILE) != m_LogTargets.end() )
		return false;

	SLogTarget target;
	target.type = LOGT_LOGFILE;
	target.pTarget = new FLogToFile(logfile, strHelloMsg, bAppend);
	target.pTarget->SetLogLevel(level);
	m_LogTargets.push_back(target);
	return true;
}

/** Close the console log target.
*/
void FLogManager::DisableConsoleLog()
{
	DestroyLogTarget(LOGT_CONSOLE);
}

/** Close the log file.
*/
void FLogManager::CloseLogFile()
{
	DestroyLogTarget(LOGT_LOGFILE);
}

/** Destroy the internal log target.
*/
void FLogManager::DestroyLogTarget( ELogTarget type )
{
	if( type >= LOGT_USER )
		return;

	LogTargetList::iterator itr;
	for( itr=m_LogTargets.begin();itr!=m_LogTargets.end();itr++ )
	{
		if( itr->type == type )
		{
			delete itr->pTarget;
			m_LogTargets.erase( itr );
			break;
		}
	}
}

/** Register a user-defined log target.
*/
bool FLogManager::RegisterLogTarget( FLogTarget* pTarget )
{
    LogTargetList::iterator itr;
    for( itr=m_LogTargets.begin();itr!=m_LogTargets.end();itr++ )
    {
        if( itr->pTarget == pTarget )
            return false;
    }

    SLogTarget newTarget;
    newTarget.type = (ELogTarget)(LOGT_USER + (m_LogTargets.size() - LOGT_NUM));
    newTarget.pTarget = pTarget;
    m_LogTargets.push_back( newTarget );
	return true;
}

/** Unregister the user-defined log target.
*/
void FLogManager::UnregisterLogTarget( FLogTarget* pTarget )
{
    LogTargetList::iterator itr;
    for( itr=m_LogTargets.begin();itr!=m_LogTargets.end();itr++ )
    {
        if( itr->pTarget == pTarget )
        {
			FASSERT(itr->type == LOGT_USER);
            m_LogTargets.erase( itr );
            break;
        }
    }
}

/** Write a log event.
*/
void FLogManager::LogEvent( ELogMessageLevel msgLevel, const AString& text )
{
    LogTargetList::iterator itr;
    for( itr=m_LogTargets.begin();itr!=m_LogTargets.end();itr++ )
    {
        itr->pTarget->LogEvent( msgLevel,text );
    }
}

/** Write a log event.
*/
void FLogManager::LogEventF( ELogMessageLevel msgLevel, const char* fmtStr, ... )
{
	static char LOG_TEMP_BUF[4096];

    va_list args;
    va_start( args,fmtStr );
    vsnprintf( LOG_TEMP_BUF, 4096, fmtStr, args );
    va_end( args );

    LogTargetList::iterator itr;
    for( itr=m_LogTargets.begin();itr!=m_LogTargets.end();itr++ )
    {
        itr->pTarget->LogEvent( msgLevel, LOG_TEMP_BUF );
    }
}

/** Write some messages.
*/
void FLogManager::LogMessage( const AString& text )
{
    LogTargetList::iterator itr;
    for( itr=m_LogTargets.begin();itr!=m_LogTargets.end();itr++ )
    {
        itr->pTarget->LogMessage( text );
    }
}