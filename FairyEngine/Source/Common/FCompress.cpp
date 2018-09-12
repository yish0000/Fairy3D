/*
 * ------------------------------------------------------------------------
 *  Name:   FCompress.cpp
 *  Desc:   This file implements the interface of compressor.
 *  Author: Yish
 *  Date:   2014/2/1
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FCompress.h"

#include "zlib/zlib.h"
#include "minilzo/minilzo.h"

///////////////////////////////////////////////////////////////////////////
//  
//  DEFLATE algorithm to compress the data.
//  
///////////////////////////////////////////////////////////////////////////

class FCompressZLib : public FCompressor
{
public:
	FCompressZLib()
	{
		m_Type = COMPRESS_ZLIB;
	}

	// Estimate the allocation size to store the compressed data
	uint32 CompressBound( uint32 srcLen )
	{
		return (uint32)compressBound( srcLen );
	}

	// Compress the data
	bool Compress( const fbyte* src, uint32 srcLen, fbyte* dest, uint32& destLen )
	{
		if( !src || !srcLen || !dest )
		{
			FASSERT(0);
			return false;
		}

		uLongf dstLen = (uLongf)destLen;
		int ret = compress( dest,&dstLen,src,srcLen );
		destLen = (uint32)dstLen;
		return (ret == Z_OK);
	}

	// Decompress the data
	bool Decompress( const fbyte* src, uint32 srcLen, fbyte* dest, uint32& destLen )
	{
		if( !src || !srcLen || !dest )
		{
			FASSERT(0);
			return false;
		}

		uLongf dstLen = (uLongf)destLen;
		int ret = uncompress( dest,&dstLen,src,srcLen );
		destLen = static_cast<uint32>(dstLen);
		return (ret == Z_OK);
	}
};

///////////////////////////////////////////////////////////////////////////
//  
//  MiniLZO algorithm to compress the data.
//  
///////////////////////////////////////////////////////////////////////////

class FCompressMiniLZO : public FCompressor
{
public:
	FCompressMiniLZO()
	{
		m_Type = COMPRESS_MINILZO;
	}

	// Estimate the allocation size to store the compressed data
	uint32 CompressBound( uint32 srcLen )
	{
		return (srcLen + srcLen / 16 + 64 + 3);
	}

	// Compress the data
	bool Compress( const fbyte* src, uint32 srcLen, fbyte* dest, uint32& destLen )
	{
		if( !src || !srcLen || !dest )
		{
			FASSERT(0);
			return false;
		}

		// Buffer for the compression.
		static lzo_align_t __LZO_MMODEL lzo_mem[((LZO1X_1_MEM_COMPRESS)+(sizeof(lzo_align_t)-1))/sizeof(lzo_align_t)];
		lzo_uint dstLen;
		int ret = lzo1x_1_compress( src,srcLen,dest,&dstLen,&lzo_mem[0] );
		destLen = (uint32)dstLen;
		return (ret == LZO_E_OK);
	}

	// Decompress the data
	bool Decompress( const fbyte* src, uint32 srcLen, fbyte* dest, uint32& destLen )
	{
		if( !src || !srcLen || !dest )
		{
			FASSERT(0);
			return false;
		}

		lzo_uint dstLen;
		int ret = lzo1x_decompress( src,srcLen,dest,&dstLen,NULL );
		destLen = static_cast<uint32>(dstLen);
		return (ret == LZO_E_OK);
	}
};

/** Create a specified compressor.
*/
FCompressor* FCompressor::Create(ECompressType Type)
{
	switch(Type)
	{
	case COMPRESS_ZLIB:		return new FCompressZLib();
	case COMPRESS_MINILZO:	return new FCompressMiniLZO();
	default: FASSERT(0);	return NULL;
	}
}