/*
 * ------------------------------------------------------------------------
 *  Name:   FMemFileStream.h
 *  Desc:   This file define a stream to handle a block of memory.
 *  Author: Yish
 *  Date:   2012/1/31
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_MEMFILESTREAM_H__
#define __FAIRY_MEMFILESTREAM_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FVFileStream.h"

///////////////////////////////////////////////////////////////////////////

/** Memory file stream.
@remarks
	Handle a the memory like a file.
*/
class FMemFileStream : public FVFileStream
{
public:
	FMemFileStream();
	FMemFileStream( fbyte* pBuf, uint32 nBufLen );
	FMemFileStream( uint32 nBufLen );
	FMemFileStream( FVFileStream* pStream );
	virtual ~FMemFileStream();

	void Seek( long nPos, int flag = START );
	size_t Tell(void) const;
	bool Eof(void) const;
	void Skip( long nCount );
	void Close(void);

	// Read some data from the memory stream.
	size_t Read( void* pBuf, size_t nSize );
	size_t Write(void* pBuf, size_t nSize);

protected:
	fbyte* m_pData;			// Pointer to the data
	fbyte* m_pEnd;			// End the buffer
	fbyte* m_pOffset;		// Current offset
	bool m_bFreeOnClose;	// Free the memory when close the stream ?
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_MEMFILESTREAM_H__