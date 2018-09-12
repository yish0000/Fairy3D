/*
 * ------------------------------------------------------------------------
 *  Name:   FNMarshal.cpp
 *  Desc:   本文件实现了网络库的数据编组。
 *  Author: Yish
 *  Date:   2012/5/15
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FNMarshal.h"


namespace FNET {

    /** 添加一个压缩的无符号整数
    */
    OctetsStream& OctetsStream::compact_uint32( uint32 x )
    {
        if( x < 0x80 ) return push_byte( (uchar)x );
        else if( x < 0x4000 ) return push_byte( BYTEORDER_16(x|0x8000) );
        else if( x < 0x20000000 ) return push_byte( BYTEORDER_32(x|0xC0000000) );
        push_byte((uchar)0xE0);
        return push_byte(BYTEORDER_32(x));
    }

    /** 添加一个压缩的整数
    */
    OctetsStream& OctetsStream::compact_sint32( int x )
    {
        if( x >= 0 )
        {
            if( x < 0x40 ) return push_byte( (uchar)x );
            else if( x < 0x2000 ) return push_byte( BYTEORDER_16(x|0x8000) );
            else if( x < 0x10000000 ) return push_byte( BYTEORDER_32(x|0xC0000000) );
            push_byte((uchar)0xE0);
            return push_byte(BYTEORDER_32(x));
        }

        if( -x > 0 )
        {
            x = -x;
            if( x < 0x40 ) return push_byte( (uchar)x|0x40 );
            else if( x < 0x2000 ) return push_byte( BYTEORDER_16(x|0xA000) );
            else if( x < 0x10000000 ) return push_byte( BYTEORDER_32(x|0xD0000000) );
            push_byte((uchar)0xF0);
            return push_byte(BYTEORDER_32(x));
        }

        push_byte((uchar)0xF0);
        return push_byte(BYTEORDER_32(x));
    }

    /** 从八位组流中取一个无符号整数
    */
    OctetsStream& OctetsStream::uncompact_uint32( uint32& x )
    {
        if( pos == size() )
            throw Exception();

        switch( (*(uchar*)begin()+pos) & 0xE0 )
        {
        case 0xE0:
            pop_byte_8();
            x = pop_byte_32();
            return *this;
        case 0xC0:
            x = pop_byte_32() & ~0xC0000000;
            return *this;
        case 0xA0:
        case 0x80:
            x = pop_byte_16() & ~0x8000;
            return *this;
        }

        x = pop_byte_8();
        return *this;
    }

    /** 从八位组流中取一个带符号整数
    */
    OctetsStream& OctetsStream::uncompact_sint32( int& x )
    {
        if( pos == size() )
            throw Exception();

        switch( (*(uchar*)begin()+pos) & 0xf0 )
        {
        case 0xF0:
            pop_byte_8();
            x = -(int)pop_byte_32();
            return *this;
        case 0xE0:
            pop_byte_8();
            x = pop_byte_32();
            return *this;
        case 0xD0:
            x = -(int)(pop_byte_32() & ~0xD0000000);
            return *this;
        case 0xC0:
            x = pop_byte_32() & ~0xC0000000;
            return *this;
        case 0xB0:
        case 0xA0:
            x = -(int)(pop_byte_16() & ~0xA000);
            return *this;
        case 0x90:
        case 0x80:
            x = pop_byte_16() & ~0x8000;
            return *this;
        case 0x70:
        case 0x60:
        case 0x50:
        case 0x40:
            x = -(int)(pop_byte_8() & ~0x40);
            return *this;
        }

        x = pop_byte_8();
        return *this;
    }
}