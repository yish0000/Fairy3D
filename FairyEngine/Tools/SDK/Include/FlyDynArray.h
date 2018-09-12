//------------------------------------------------------------------------
// Name: FlyDynArray.h
// Desc: This file define a dynamic array template for the engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_DYNARRAY_H__
#define __FLY_DYNARRAY_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"

//////////////////////////////////////////////////////////////////////////


template <typename Type>
class FlyDynArray
{
public: class iterator;

public:
    // Constructors of the class.
    explicit FlyDynArray(void)
    {
        m_pData = NULL;
        m_nNumElements = 0;
        m_nMemSize = 0;
        m_First = iterator(NULL);
        m_End = iterator(NULL);
        m_Last = iterator(NULL);
    }

    explicit FlyDynArray( size_t nSize,const Type& v=Type() )
    {
        m_pData = new Type[nSize];
        for( size_t i=0;i<nSize;i++ )
            m_pData[i] = v;
        m_nNumElements = nSize;
        m_nMemSize = nSize;
        m_First = iterator(m_pData);
        m_Last = iterator(m_pData+m_nNumElements);
        m_End = iterator(m_pData+nSize);
    }

    FlyDynArray( const FlyDynArray& arr )
    {
        size_t n = arr.size();
        m_pData = new Type[n];
        for( size_t i=0;i<n;i++ ) m_pData[i] = arr[i];
        m_nNumElements = n;
        m_nMemSize = n;
        m_First = iterator(m_pData);
        m_End = iterator(m_pData+n);
        m_Last = iterator(m_pData+n);
    }

    FlyDynArray( iterator first,iterator last )
    {
        m_pData = NULL;
        m_nNumElements = 0;
        m_nMemSize = 0;
        m_First = iterator(NULL);
        m_End = iterator(NULL);
        m_Last = iterator(NULL);

        insert( m_First,first,last );
    }

    // Destructor of the class.
    ~FlyDynArray(void) { SAFE_DELETE(m_pData); }

    FlyDynArray<Type>& operator = ( const FlyDynArray<Type>& _X )
    {
        if( this == &_X )
            return *this;

        SAFE_DELETE( m_pData );
        m_nNumElements = 0;
        m_nMemSize = 0;
        m_First = iterator(NULL);
        m_End = iterator(NULL);
        m_Last = iterator(NULL);

        insert( m_First,_X.begin(),_X.end() );
        return *this;
    }

    // Pre-allocate the memory for array.
    void reserve( size_t nSize )
    {
        if( m_nMemSize < nSize )
        {
            Type* pTemp = new Type[nSize];
            memcpy( pTemp,m_pData,sizeof(Type)*m_nNumElements );
            SAFE_DELETE( m_pData );
            m_pData = pTemp;
            m_nMemSize = nSize;
            m_First = iterator(m_pData);
            m_End = iterator(m_pData+nSize);
            m_Last = iterator(m_pData+m_nNumElements);
        }
    }

    size_t capacity(void) const { return m_nMemSize; }

    iterator begin(void) { return m_First; }
    const iterator& begin(void) const { return m_First; }
    iterator end(void) { return m_Last; }
    const iterator& end(void) const { return m_Last; }

    // Resize the array.
    void resize( size_t nSize,Type t=Type() )
    {
        if( m_nNumElements < nSize )
            insert( m_Last,nSize-m_nNumElements,t );
        else
            erase( m_First+nSize,m_Last );
    }

    size_t size(void) const { return m_nNumElements; }
    bool empty(void) const { return (m_nNumElements==0); }

    Type at( int nPos ) { return m_pData[nPos]; }
    const Type& at( int nPos ) const { return m_pData[nPos]; }

    Type operator [] ( int nPos ) { return m_pData[nPos]; }
    const Type& operator [] ( int nPos ) const { return m_pData[nPos]; }

    Type& front(void) { return *m_First; }
    const Type& front(void) const { return *m_First; }
    Type& back(void) { return *(m_Last - 1); }
    const Type& back(void) const { return *(m_Last - 1); }

    // Push a new element to array.
    void push_back( const Type& x ) { insert(m_Last,x); }
    void pop_back(void) { erase(m_Last-1); }

    // Insert a new element to array.
    iterator insert( iterator itr,const Type& x=Type() )
    {
        size_t offset = itr - m_First;
        insert( itr,1,x );
        return m_First + offset;
    }

    void insert( iterator itr,size_t n,const Type& x )
    {
        if( m_nMemSize-m_nNumElements < n )
        {
            size_t _N = m_nNumElements+(n<m_nNumElements?m_nNumElements:n);
            Type* pTemp = new Type[_N];
            memcpy( pTemp,m_pData,(itr-m_First)*sizeof(Type) );
            size_t offset = itr - m_First;
            for( size_t i=0;i<n;i++ ) pTemp[offset+i] = x;
            memcpy( &pTemp[offset+n],itr.ptr,(m_Last-itr)*sizeof(Type) );
            SAFE_DELETE( m_pData );
            m_pData = pTemp;
            m_nNumElements += n;
            m_nMemSize = _N;
            m_First = iterator(m_pData);
            m_End = iterator(m_pData+m_nMemSize);
            m_Last = iterator(m_pData+m_nNumElements);
        }
        else
        {
            memcpy( (itr+n).ptr,itr.ptr,(m_Last-itr)*sizeof(Type) );
            for( size_t i=0;i<n;i++ ) itr.ptr[i] = x;
            m_nNumElements += n;
            m_Last = iterator(m_pData+m_nNumElements);
        }
    }

    void insert( iterator itr,iterator first,iterator last )
    {
        size_t n = last - first;
        if( m_nMemSize-m_nNumElements < n )
        {
            size_t _N = m_nNumElements+(n<m_nNumElements?m_nNumElements:n);
            Type* pTemp = new Type[_N];
            memcpy( pTemp,m_pData,(itr-m_First)*sizeof(Type) );
            size_t offset = itr - m_First;
            size_t i;
            iterator tmp;
            for( i=0,tmp=first;tmp!=last;tmp++,i++ )
                pTemp[offset+i] = *tmp;
            memcpy( &pTemp[offset+n],itr.ptr,(m_Last-itr)*sizeof(Type) );
            SAFE_DELETE( m_pData );
            m_pData = pTemp;
            m_nNumElements += n;
            m_nMemSize = _N;
            m_First = iterator(m_pData);
            m_End = iterator(m_pData+m_nMemSize);
            m_Last = iterator(m_pData+m_nNumElements);
        }
        else
        {
            memcpy( (itr+n).ptr,itr.ptr,(m_Last-itr)*sizeof(Type) );
            iterator tmp;
            size_t i;
            for( i=0,tmp=first;tmp!=last;tmp++,i++ ) itr.ptr[i] = *tmp;
            m_nNumElements += n;
            m_Last = iterator(m_pData+m_nNumElements);
        }
    }

    // Erase the item.
    void erase( iterator itr )
    {
        size_t i;
        iterator tmp;
        for( tmp=itr+1,i=0;tmp!=m_Last;tmp++,i++ )
            *(itr+i) = *tmp;
        m_Last--;
        m_nNumElements--;
    }

    void erase( iterator first,iterator last )
    {
        size_t i;
        size_t n = last - first;
        iterator tmp;
        for( i=0,tmp=last;tmp!=m_Last;i++,tmp++ )
            *(first+i) = *tmp;
        m_Last -= n;
        m_nNumElements -= n;
    }

    // Clear the item.
    void clear(void) { erase(m_First,m_Last); }

    class iterator
    {
        friend class FlyDynArray<Type>;

    protected:
        Type* ptr;

    public:
        iterator(void) { ptr = NULL; }
        iterator( Type* p ) { ptr = p; }
        ~iterator(void) { /* Nothing to do! */ };

        Type& operator * (void) { return *ptr; }
        iterator& operator ++ (void) { ptr++; return *this; }
        iterator operator ++ (int) { iterator tmp=*this; ptr++; return tmp; }
        iterator& operator -- (void) { ptr--; return *this; }
        iterator operator -- (int) { iterator tmp=*this; ptr--; return tmp; }
        iterator& operator += ( size_t n ) { ptr+=n; return *this; }
        iterator& operator -= ( size_t n ) { ptr-=n; return *this; }
        iterator operator + ( size_t n ) { iterator tmp=*this; tmp+=n; return tmp; }
        iterator operator - ( size_t n ) { iterator tmp=*this; tmp-=n; return tmp; }
        size_t operator - ( const iterator& x ) { return size_t(ptr-x.ptr); }
        bool operator == ( const iterator& x ) { return ptr==x.ptr; }
        bool operator != ( const iterator& x ) { return ptr!=x.ptr; }
        bool operator < ( const iterator& x ) { return ptr<x.ptr; }
        bool operator > ( const iterator& x ) { return ptr>x.ptr; }
        bool operator <= ( const iterator& x ) { return !(ptr>x.ptr); }
        bool operator >= ( const iterator& x ) { return !(ptr<x.ptr); }
    };

protected:
    Type* m_pData;          // Pointer to the data.
    size_t m_nNumElements;  // Count of the elements.
    size_t m_nMemSize;      // Current memory size.

    iterator m_First;       // Pointer to the first item.
    iterator m_Last;        // Pointer to the last item + 1.
    iterator m_End;         // Pointer to the end of memory +1.
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_DYNARRAY_H__