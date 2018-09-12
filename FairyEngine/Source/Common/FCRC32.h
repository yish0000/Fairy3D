/*
 * ------------------------------------------------------------------------
 *  Name:   FCRC32.h
 *  Desc:   Cyclic Redundancy Check.
 *  Author: Yish
 *  Date:   2011/12/4
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_CRC32_H__
#define __FAIRY_CRC32_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

class FCRC32
{
public:		// Types

	enum { CRC32_POLY = 0x04c11db7 };

public:
	FCRC32();

	uint32 CrcMem( const void* data, int iLen, uint32 crc = 0 );
	uint32 CrcStr( const char* str, bool bCaseSensitive = true );
	uint32 CrcStr( const wchar_t* str, bool bCaseSensitive = true );
	uint32 StrHash( const char* str, bool bCaseSensitive = true );
	uint32 StrHash( const wchar_t* str, bool bCaseSensitive = true );

protected:
	static uint32 CRC_TABLE[256];
	static bool CRC_TABLE_INIT;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_CRC32_H__