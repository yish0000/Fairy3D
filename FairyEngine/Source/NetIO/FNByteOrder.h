/*
 * ------------------------------------------------------------------------
 *  Name:   FNByteOrder.h
 *  Desc:   本文件定义了网络库的字节序转换。
 *  Author: Yish
 *  Date:   2012/3/6
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FNET_BYTEORDER_H__
#define __FNET_BYTEORDER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FNBase.h"

///////////////////////////////////////////////////////////////////////////


namespace FNET {

#if FAIRY_ENDIAN_BIG
	#define BYTEORDER_16(x) (x)
	#define BYTEORDER_32(x) (x)
	#define BYTEORDER_64(x) (x)
#else
	#if FAIRY_COMPILER == FAIRY_COMPILER_GCC
		inline unsigned short BYTEORDER_16( unsigned short x )
		{
			__asm__ ("xchg %b0, %h0" : "=q"(x) : "0"(x));
			return x;
		}

		inline unsigned long BYTEORDER_32( unsigned long x )
		{
			__asm__ ("bswap %0" : "=r"(x) : "0"(x));
			return x;
		}

		inline unsigned long long BYTEORDER_64( unsigned long long x )
		{
			union
			{
				unsigned long long __ll;
				unsigned long __l[2];
			} i, o;

			i.__ll = x;
			o.__l[0] = BYTEORDER_32( i.__l[1] );
			o.__l[1] = BYTEORDER_32( i.__l[0] );
			return o.__ll;
		}
	#else
		inline unsigned __int16 BYTEORDER_16( unsigned __int16 x )
		{
			__asm ror x, 8
			return x;
		}

		inline unsigned __int32 BYTEORDER_32( unsigned __int32 x )
		{
			__asm mov eax, x
			__asm bswap eax
			__asm mov x, eax
			return x;
		}

		inline unsigned __int64 BYTEORDER_64( unsigned __int64 x )
		{
			union
			{
				unsigned __int64 __ll;
				unsigned __int32 __l[2];
			} i, o;

			i.__ll = x;
			o.__l[0] = BYTEORDER_32( i.__l[1] );
			o.__l[1] = BYTEORDER_32( i.__l[0] );
			return o.__ll;
		}
	#endif
#endif
}

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FNET_BYTEORDER_H__