/*
 * ------------------------------------------------------------------------
 *  Name:   FMD5.h
 *  Desc:   MD5 hash algorithm.
 *  Author: Yish
 *  Date:   2012/2/1
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_MD5_H__
#define __FAIRY_MD5_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

class FMD5
{
public:		// Types

	struct Context
	{
		uint32 state[4];
		uint32 count[2];
		fbyte buffer[64];
	};

public:

	void Init( Context* context );
	void Update( Context* context, fbyte* input, int len );
	void Final( fbyte* digest, Context* context );
	void Transform( uint32* state, fbyte* block );
	void Encode( fbyte* output, uint32* input, int len );
	void Decode( uint32* output, fbyte* input, int len );

	uint32 GetHash( const void* input, int len );
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_MD5_H__