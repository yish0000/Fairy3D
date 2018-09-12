/*
 * ------------------------------------------------------------------------
 *  Name:   FNMarshal.h
 *  Desc:   本文件实现了网络库的数据编组。
 *  Author: Yish
 *  Date:   2012/3/4
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FNET_MARSHAL_H__
#define __FNET_MARSHAL_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FNOctets.h"
#include "FNByteOrder.h"

///////////////////////////////////////////////////////////////////////////


namespace FNET {

    class OctetsStream;

    /** 异常对象
    */
    class Exception {};

    /** 数据编组
    */
    class _FNET_EXPORT Marshal
    {
    public:
        virtual ~Marshal() {};

        virtual OctetsStream& marshal( OctetsStream& os ) const = 0;
        virtual OctetsStream& unmarshal( OctetsStream& is ) = 0;
    };

    /** 八位组流
    */
    class _FNET_EXPORT OctetsStream : public Octets
    {
        enum { MAXSPARE = 16384 };
        mutable uint32 pos;         // 
        mutable uint32 tranpos;     // 

        template <typename T> OctetsStream& push_byte( T t )
        {
            insert( end(),&t,sizeof(t) );
            return *this;
        }

        template <typename T> void pop_byte( T& t ) const
        {
            if( pos + sizeof(t) > size() )
                throw Exception();
            t = *(T*)((char*)begin() + pos);
            pos += sizeof(t);
        }

        uchar pop_byte_8() const
        {
            uchar c;
            pop_byte(c);
            return c;
        }

        ushort pop_byte_16() const
        {
            ushort us;
            pop_byte(us);
            return BYTEORDER_16(us);
        }

        uint32 pop_byte_32() const
        {
            uint32 i;
            pop_byte(i);
            return BYTEORDER_32(i);
        }

        uint64 pop_byte_64() const
        {
            uint64 ii;
            pop_byte(ii);
            return BYTEORDER_64(ii);
        }

        friend class CompactUINT;
        friend class CompactSINT;

        OctetsStream& compact_uint32( uint32 x );
        OctetsStream& compact_sint32( int x );
        OctetsStream& uncompact_uint32( uint32& x );
        OctetsStream& uncompact_sint32( int& x );

    public:
        OctetsStream() : pos(0) {}
        OctetsStream( const Octets& o ) : Octets(0),pos(0) {}
        OctetsStream( const OctetsStream& os ) : Octets(os),pos(0) {}

        OctetsStream& operator = ( const OctetsStream& os )
        {
            if( &os != this )
            {
                pos = os.pos;
                replace( os.begin(),os.size() );
            }

            return *this;
        }

        OctetsStream& operator << ( char x )    { return push_byte(x); }
        OctetsStream& operator << ( uchar x )   { return push_byte(x); }
        OctetsStream& operator << ( short x )   { return push_byte(BYTEORDER_16(x)); }
        OctetsStream& operator << ( ushort x )  { return push_byte(BYTEORDER_16(x)); }
        OctetsStream& operator << ( int x )     { return push_byte(BYTEORDER_32(x)); }
        OctetsStream& operator << ( uint32 x )  { return push_byte(BYTEORDER_32(x)); }
        OctetsStream& operator << ( long x )    { return push_byte(BYTEORDER_32(x)); }
        OctetsStream& operator << ( ulong x )   { return push_byte(BYTEORDER_32(x)); }
        OctetsStream& operator << ( int64 x )   { return push_byte(BYTEORDER_64(x)); }
        OctetsStream& operator << ( uint64 x )  { return push_byte(BYTEORDER_64(x)); }
        OctetsStream& operator << ( float x )   { return push_byte(BYTEORDER_32(*(ulong*)&x)); }
        OctetsStream& operator << ( double x )  { return push_byte(BYTEORDER_64(*(uint64*)&x)); }
        OctetsStream& operator << ( const Marshal& x ) { return x.marshal(*this); }

        OctetsStream& operator << ( const Octets& x )
        {
            compact_uint32( (uint32)x.size() );
            insert( end(),x.begin(),x.end() );
            return *this;
        }

        OctetsStream& operator << ( const AString& x )
        {
            compact_uint32( (uint32)x.length() );
            insert( end(),x.c_str(),x.length() );
            return *this;
        }

        OctetsStream& operator << ( const WString& x )
        {
            compact_uint32( (uint32)x.length() );
            insert( end(),x.c_str(),x.length()*sizeof(wchar_t) );
            return *this;
        }

        OctetsStream& push_byte( const char* data,size_t len )
        {
            insert( end(),data,len );
            return *this;
        }

        template <typename T,typename F>
        OctetsStream& operator << ( const std::pair<T,F>& x )
        {
            return *this << x.first << x.second;
        }

        // 是否已经到数据流的底部
        bool eos() const { return pos == size(); }

        /** 八位组流的行为
        */
        enum Transaction
        {
            BEGIN,      // 记录当前位置
            COMMIT,     // 回到记录的位置
            ROLLBACK,
        };

        OctetsStream& operator >> ( Transaction trans )
        {
            switch( trans )
            {
            case BEGIN:
                tranpos = pos;
                break;
            case COMMIT:
                pos = tranpos;
                break;
            case ROLLBACK:
                if( pos >= MAXSPARE )
                {
                    erase( begin(),(char*)begin()+pos );
                    pos = 0;
                }
                break;
            }

            return *this;
        }

        OctetsStream& operator >> ( char& x )
        {
            x = pop_byte_8();
            return *this;
        }

        OctetsStream& operator >> ( uchar& x )
        {
            x = pop_byte_8();
            return *this;
        }

        OctetsStream& operator >> ( short& x )
        {
            x = pop_byte_16();
            return *this;
        }

        OctetsStream& operator >> ( ushort& x )
        {
            x = pop_byte_16();
            return *this;
        }

        OctetsStream& operator >> ( int& x )
        {
            x = pop_byte_32();
            return *this;
        }

        OctetsStream& operator >> ( uint32& x )
        {
            x = pop_byte_32();
            return *this;
        }

        OctetsStream& operator >> ( long& x )
        {
            x = pop_byte_32();
            return *this;
        }

        OctetsStream& operator >> ( ulong& x )
        {
            x = pop_byte_32();
            return *this;
        }

        OctetsStream& operator >> ( int64& x )
        {
            x = pop_byte_64();
            return *this;
        }

        OctetsStream& operator >> ( uint64& x )
        {
            x = pop_byte_64();
            return *this;
        }

        OctetsStream& operator >> ( float& x )
        {
            ulong l = pop_byte_32();
            x = *(float*)&l;
            return *this;
        }

        OctetsStream& operator >> ( double& x )
        {
            uint32 ii = pop_byte_64();
            x = *(double*)&ii;
            return *this;
        }

        OctetsStream& operator >> ( Marshal& x )
        {
            return x.unmarshal(*this);
        }

        OctetsStream& operator >> ( Octets& x )
        {
            uint32 len;
            uncompact_uint32( len );
            if( pos + len > size() ) throw Exception();
            x.replace( (char*)begin()+pos,len );
            pos += len;
            return *this;
        }

        OctetsStream& operator >> ( AString& x )
        {
            uint32 len;
            uncompact_uint32( len );
            if( pos + len > size() ) throw Exception();
            x.assign( (char*)begin()+pos,len );
            pos += len;
            return *this;
        }

        OctetsStream& operator >> ( WString& x )
        {
            uint32 len;
            uncompact_uint32( len );
            if( pos + len*sizeof(wchar_t) > size() ) throw Exception();
            x.assign( (wchar_t*)begin()+pos,len*sizeof(wchar_t) );
            pos += len*sizeof(wchar_t);
            return *this;
        }

        void pop_byte( char* data,size_t len )
        {
            if( pos + len > size() )
                throw Exception();
            memcpy( data,(char*)begin()+pos,len );
            pos += (uint32)len;
        }

        template <typename T>
        OctetsStream& operator >> ( const std::vector<T>& x )
        {
            return *this >> STLContainer(x);
        }

        template <typename T>
        OctetsStream& operator << ( const std::vector<T>& x )
        {
            return *this << STLContainer(x);
        }

        template <typename T,typename F>
        OctetsStream& operator >> ( const std::pair<T,F>& x )
        {
            return *this >> x.first >> x.second;
        }

        Octets& clear()
        {
            pos = 0;
            return Octets::clear();
        }

        Octets& replace( const void* data,size_t sz )
        {
            pos = 0;
            return Octets::replace( data,sz );
        }
    };

    /** 压缩后的无符号整数
    */
    class _FNET_EXPORT CompactUINT : public Marshal
    {
    protected:
        uint32* pi;

    public:
        explicit CompactUINT( uint32& i ) : pi(&i) {}
        explicit CompactUINT( const uint32& i ) : pi(const_cast<uint32*>(&i)) {}

        OctetsStream& marshal( OctetsStream& os ) const
        {
            return os.compact_uint32(*pi);
        }

        OctetsStream& unmarshal( OctetsStream& os )
        {
            return os.uncompact_uint32(*pi);
        }
    };

    /** 压缩后的带符号整数
    */
    class _FNET_EXPORT CompactSINT : public Marshal
    {
    protected:
        int* pi;

    public:
        explicit CompactSINT( int& i ) : pi(&i) {}
        explicit CompactSINT( const int& i ) : pi(const_cast<int*>(&i)) {}

        OctetsStream& marshal( OctetsStream& os ) const
        {
            return os.compact_sint32(*pi);
        }

        OctetsStream& unmarshal( OctetsStream& os )
        {
            return os.uncompact_sint32(*pi);
        }
    };

    /** STL容器装载
    */
    template <typename T>
    class _FNET_EXPORT STLContainer : public Marshal
    {
    protected:
        T* pc;

    public:
        explicit STLContainer( T& c ) : pc(&c) {}

        OctetsStream& marshal( OctetsStream& os ) const
        {
            os << CompactUINT(pc->size());
            for( typename T::const_iterator i=pc->begin();i!=pc->end();++i )
                os << *i;
            return os;
        }

        OctetsStream& unmarshal( OctetsStream& os )
        {
            pc->clear();

            size_t sz;
            for( os >> CompactUINT(sz);sz>0;--sz )
            {
                typename T::value_type tt;
                os >> tt;
                pc->insert( pc->end(),tt );
            }

            return os;
        }
    };
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FNET_MARSHAL_H__