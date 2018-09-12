/*
 * ------------------------------------------------------------------------
 *  Name:   FBase.h
 *  Desc:   This file define the Base Types for the engine.
 *  Author: Yish
 *  Date:   2011/6/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_BASE_H__
#define __FAIRY_BASE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "Fairy.h"
#include "FBaseType.h"
#include "FMemConfig.h"
#include "FContainer.h"

#include "FSmartPtr.h"

///////////////////////////////////////////////////////////////////////////

// String
typedef std::string AString;
typedef std::stringstream AStringStream;
typedef std::wstring WString;
typedef std::wstringstream WStringStream;

// String Vector
typedef std::vector<AString> AStringList;
typedef std::vector<WString> WStringList;

// Key-Value
typedef std::map<AString, AString> AKeyValueList;
typedef std::map<WString, WString> WKeyValueList;

// For windows. We should use TString to make strings compatible between ANSI and UNICODE.
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	#ifdef FAIRY_UNICODE
		typedef WString TString;
		typedef WStringStream TStringStream;
		typedef WKeyValueList TKeyValueList;
	#else
		typedef AString TString;
		typedef AStringStream TStringStream;
		typedef AKeyValueList TKeyValueList;
	#endif
#endif

/** Pre-defined classes
*/
class FBitset;
class FCounter;
class FRandom;
class FPerlinNoiseBase;
class FPerlinNoise1D;
class FPerlinNoise2D;
class FPerlinNoise3D;
class FLogTarget;
class FLogManager;
class FPerformance;
class FThread;
class FThreadMutex;
class FThreadAtomic;
class FThreadCond;
class FThreadSemaphore;
class FThreadTask;
class FThreadPool;
class FVFile;
class FVFileStream;
class FVFilePack;
class FVFilePackFactory;
class FVFileSystem;
class FIniFile;
class FTokenFile;
class FIniFile;
class FXMLReader;
class FXMLWriter;
class FTimer;

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_BASE_H__