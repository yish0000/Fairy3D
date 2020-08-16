/*
 * ------------------------------------------------------------------------
 *  Name:   FCompress.h
 *  Desc:   This file defines the interface to compress the data.
 *  Author: Yish
 *  Date:   2014/2/1
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_COMPRESS_H__
#define __FAIRY_COMPRESS_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

/** Base class of the compressor.
*/
class FCompressor
{
public:

	enum ECompressType
	{
		COMPRESS_ZLIB = 1,  // Deflate algorithm
		COMPRESS_MINILZO,   // MiniLZO
	};

public:
    virtual ~FCompressor() {}
    
	// Estimate the allocation size to store the compressed data
	virtual uint32 CompressBound( uint32 srcLen ) = 0;
	
	// Compress the data
	virtual bool Compress( const FBYTE* src, uint32 srcLen, FBYTE* dest, uint32& destLen ) = 0;
	// Decompress the data
	virtual bool Decompress( const FBYTE* src, uint32 srcLen, FBYTE* dest, uint32& destLen ) = 0;

	// Get the type of compressor
	ECompressType GetType() const { return m_Type; }

	// Create a specified compressor
	static FCompressor* Create(ECompressType Type);

protected:
	ECompressType m_Type;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_COMPRESS_H__