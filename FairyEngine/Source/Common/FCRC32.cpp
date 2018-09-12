/*
 * ------------------------------------------------------------------------
 *  Name:   FCRC32.cpp
 *  Desc:   Cyclic Redundancy Check, the code is from internet.
 *  Author: Yish
 *  Date:   2012/2/3
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FCRC32.h"

bool FCRC32::CRC_TABLE_INIT = false;
uint32 FCRC32::CRC_TABLE[256];

/** Constructor.
*/
FCRC32::FCRC32()
{
	if( !CRC_TABLE_INIT )
	{
		for( int iCRC=0;iCRC<256;iCRC++ )
		{
			for( int c=iCRC<<24,j=8;j!=0;j-- )
			{
				CRC_TABLE[iCRC] = c = c & 0x80000000 ? (c << 1) ^ CRC32_POLY : (c << 1);
			}
		}

		CRC_TABLE_INIT = true;
	}
}

/** Calculate the CRC code of specified memory block.
*/
uint32 FCRC32::CrcMem( const void* data, int iLen, uint32 crc )
{
	fbyte* pData = (fbyte*)data;
	crc = ~crc;
	for( int i=0;i<iLen;i++ )
		crc = (crc << 8) ^ CRC_TABLE[(crc >> 24) ^ pData[i]];
	return ~crc;
}

/** Calculate the CRC code of specified string.
*/
uint32 FCRC32::CrcStr( const char* str, bool bCaseSensitive/* =true */ )
{
	size_t len = strlen( str );
	uint32 crc = 0xffffffff;
	for( size_t i=0;i<len;i++ )
	{
		char c = bCaseSensitive ? str[i] : toupper(str[i]);
		crc = (crc << 8) ^ CRC_TABLE[(crc >> 24) ^ c];
	}
	return ~crc;
}

/** Calculate the CRC code of specified string. (UNICODE)
*/
uint32 FCRC32::CrcStr( const wchar_t* str, bool bCaseSensitive/* =true */ )
{
	size_t Length = wcslen( str );
	uint32 crc = 0xffffffff;
	for( size_t i=0;i<Length;i++ )
	{
		wchar_t c = bCaseSensitive ? str[i] : towupper(str[i]);
		int cl = (c & 0xff);
		crc = (crc << 8) ^ CRC_TABLE[(crc >> 24) ^ cl];
		int ch = (c >> 8);
		crc = (crc << 8) ^ CRC_TABLE[(crc >> 24) ^ ch];
	}
	return ~crc;
}

/** Calculate the hash code of specified string.
*/
uint32 FCRC32::StrHash( const char* str, bool bCaseSensitive/* =true */ )
{
	uint32 hash = 0;
	while( *str )
	{
		char ch = bCaseSensitive ? *str++ : toupper(*str++);
		fbyte B = ch;
		hash = ((hash >> 8) & 0x00ffffff) ^ CRC_TABLE[(hash ^ B) & 0x000000ff];
	}
	return hash;
}

/** Calculate the hash code of specified string. (UNICODE)
*/
uint32 FCRC32::StrHash( const wchar_t* str, bool bCaseSensitive/* =true */ )
{
	uint32 hash=0;
	while( *str )
	{
		wchar_t Ch = bCaseSensitive ? *str++ : towupper(*str++);
		fbyte B = Ch;
		hash = ((hash >> 8) & 0x00ffffff) ^ CRC_TABLE[(hash ^ B) & 0x000000ff];
		B = Ch >> 8;
		hash = ((hash >> 8) & 0x00ffffff) ^ CRC_TABLE[(hash ^ B) & 0x000000ff];
	}
	return hash;
}