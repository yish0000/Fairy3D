/*
 * ------------------------------------------------------------------------
 *  Name:   FTemplate.h
 *  Desc:   本文件为引擎定义了一些有用的模板工具类。
 *  Author: Yish
 *  Date:   2010/12/19
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_TEMPLATE_H__
#define __FAIRY_TEMPLATE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBaseType.h"
#include "FMemConfig.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//  
//  Template Classes
//  
///////////////////////////////////////////////////////////////////////////

/** 二维点
*/
template <class T>
class FPointT
{
public:
	FPointT() {}
	FPointT( const T& _x, const T& _y ) { x = _x; y = _y; }
	FPointT( const FPointT& pt ) { x = pt.x; y = pt.y; }

	void Set( const T& _x, const T& _y ) { x = _x; y = _y; }
	void Offset( const T& _x, const T& _y ) { x += _x; y += _y; }

	friend bool operator != ( const FPointT& p1, const FPointT& p2 ) { return p1.x!=p2.x || p1.y!=p2.y; }
	friend bool operator == ( const FPointT& p1, const FPointT& p2 ) { return p1.x==p2.x && p1.y==p2.y; }

	friend FPointT operator + ( const FPointT& p1, const FPointT& p2 ) { return FPointT(p1.x+p2.x, p1.y+p2.y); }
	friend FPointT operator - ( const FPointT& p1, const FPointT& p2 ) { return FPointT(p1.x-p2.x, p1.y-p2.y); }

	FPointT operator + () const { return *this; }
	FPointT operator - () const { return FPointT(-x,-y); }

	FPointT& operator += ( const FPointT& pt ) { x += pt.x; y += pt.y; return *this; }
	FPointT& operator -= ( const FPointT& pt ) { x -= pt.x; y -= pt.y; return *this; }

public:
	T x, y;
};

typedef FPointT<int> FPointI;
typedef FPointT<float> FPointF;

///////////////////////////////////////////////////////////////////////////

/** 二维矩形
*/
template <class T>
class FRectT
{
public:
	FRectT() {}
	FRectT( const T& _l, const T& _t, const T& _r, const T& _b ) { l=_l; t=_t; r=_r; b=_b; }
	FRectT( const FRectT& rc ) { l=rc.l; t=rc.t; r=rc.r; b=rc.b; }

	friend bool operator != ( const FRectT& rc1, const FRectT& rc2 ) { return rc1.l != rc2.l || rc1.t != rc2.t || rc1.r != rc2.r || rc1.b != rc2.b; }
	friend bool operator == ( const FRectT& rc1, const FRectT& rc2 ) { return rc1.l == rc2.l && rc1.t == rc2.t && rc1.r == rc2.r && rc1.b == rc2.b; }

	friend FRectT operator + ( const FRectT& rc1, const FRectT& rc2 ) { return FRectT(rc1.l+rc2.l,rc1.t+rc2.t,rc1.r+rc2.r,rc1.b+rc2.b); }
	friend FRectT operator - ( const FRectT& rc1, const FRectT& rc2 ) { return FRectT(rc1.l-rc2.l,rc1.t-rc2.t,rc1.r-rc2.r,rc1.b-rc2.b); }
	friend FRectT operator + ( const FRectT& rc1, const FPointT<T>& pt ) { return FRectT(rc1.l+pt.x,rc1.t+pt.y,rc1.r+pt.x,rc1.b+pt.y); }
	friend FRectT operator - ( const FRectT& rc1, const FPointT<T>& pt ) { return FRectT(rc1.l-pt.x,rc1.t-pt.y,rc1.r-pt.x,rc1.b-pt.y); }

	friend FRectT operator & ( const FRectT& rc1, const FRectT& rc2 )
	{
		if( rc1.IsEmpty() || rc2.IsEmpty() )
			return FRectT(0,0,0,0);
		if( rc1.l >= rc2.r || rc1.t >= rc2.b ||
			rc1.r <= rc2.l || rc1.b <= rc2.t )
			return FRectT(0,0,0,0);
		return FRectT(
			rc1.l > rc2.l ? rc1.l : rc2.l,
			rc1.t > rc2.t ? rc1.t : rc2.t,
			rc1.r < rc2.r ? rc1.r : rc2.r,
			rc1.b < rc2.b ? rc1.b : rc2.b );
	}

	friend FRectT operator | ( const FRectT& rc1, const FRectT& rc2 )
	{
		if( rc1.IsEmpty() ) return rc2;
		if( rc2.IsEmpty() ) return rc1;
		return FRectT(
			rc1.l < rc2.l ? rc1.l : rc2.l,
			rc1.t < rc2.t ? rc1.t : rc2.t,
			rc1.r > rc2.r ? rc1.r : rc2.r,
			rc1.b > rc2.b ? rc1.b : rc2.b );
	}

	FRectT operator + () const { return *this; }
	FRectT operator - () const { return FRectT(-l,-t,-r,-b); }

	FRectT& operator += ( const FRectT& rc ) { l+=rc.l; t+=rc.t; r+=rc.r; b+=rc.b; return *this; }
	FRectT& operator -= ( const FRectT& rc ) { l-=rc.l; t-=rc.t; r-=rc.r; b-=rc.b; return *this; }
	FRectT& operator += ( const FPointT<T>& pt ) { l+=pt.x; t+=pt.y; r+=pt.x; b+=pt.y; return *this; }
	FRectT& operator -= ( const FPointT<T>& pt ) { l-=pt.x; t-=pt.y; r-=pt.x; b-=pt.y; return *this; }

	void SetRect( const T& _l, const T& _t, const T& _r, const T& _b ) { l=_l; t=_t; r=_r; b=_b; }
	void Offset( const T& x, const T& y ) { l+=x; t+=y; r+=x; b+=y; }
	void Offset( const FPointT<T>& pt ) { Offset(pt.x,pt.y); }

	T Width() const { return r - l; }
	T Height() const { return b - t; }
	FPointT<T> CenterPoint() const { return FPointT<T>((l+r)/2,(t+b)/2); }

	bool PtInRect( const T& x, const T& y ) const { return (x >= l && x <= r && y >= t && y <= b); }
	bool PtInRect( const FPointT<T>& pt ) { return PtInRect(pt.x,pt.y); }

	void Clear() { l = t = r = b = 0; }
	bool IsEmpty() { return Width() == 0 || Height() == 0; }

	void Deflate( const T& x, const T& y ) { l+=x; t+=y; r-=x; b-=y; }
	void Deflate( const FRectT<T>& rc ) { l+=rc.l; t+=rc.t; r-=rc.r; b-=rc.b; }
	void Deflate( const T& l, const T& t, const T& r, const T& b ) { l+=l; t+=t; r-=r; b-=b; }

	void Inflate( const T& x, const T& y ) { l-=x; t-=y; r+=x; b+=y; }
	void Inflate( const FRectT<T>& rc ) { l-=rc.l; t-=rc.t; r+=rc.r; b+=rc.b; }
	void Inflate( const T& l, const T& t, const T& r, const T& b ) { l-=l; t-=t; r+=r; b+=b; }

	void SetUnion( const FRectT<T>& rc1, const FRectT<T>& rc2 ) { *this = rc1 | rc2; }
	void SetIntersect( const FRectT<T>& rc1, const FRectT<T>& rc2 ) { *this = rc1 & rc2; }

public:
	T l, t, r, b;
};

typedef FRectT<int> FRectI;
typedef FRectT<float> FRectF;

///////////////////////////////////////////////////////////////////////////
//  
//  Template functions
//  
///////////////////////////////////////////////////////////////////////////

template <class T>
inline T F_Abs( const T& t )
{
	return (t < 0) ? -t : t;
}

template <class T>
inline T F_Min( const T& t1, const T& t2 )
{
	return (t1 < t2) ? t1 : t2;
}

template <class T>
inline T F_Max( const T& t1, const T& t2 )
{
	return (t1 > t2) ? t1 : t2;
}

template <class T>
inline T F_Min( const T& t1, const T& t2, const T& t3 )
{
	return F_Min( F_Min(t1, t2), t3 );
}

template <class T>
inline T F_Min( const T& t1, const T& t2, const T& t3, const T& t4 )
{
	return F_Min( F_Min(t1, t2, t3), t4 );
}

template <class T>
inline T F_Max( const T& t1, const T& t2, const T& t3 )
{
	return F_Max( F_Max(t1, t2), t3 );
}

template <class T>
inline T F_Max( const T& t1, const T& t2, const T& t3, const T& t4 )
{
	return F_Max( F_Max(t1, t2, t3), t4 );
}

template <class T>
inline void F_ClampFloor(T& val, const T& min)
{
	if( val < min ) val = min;
}

template <class T>
inline void F_ClampRoof(T& val, const T& max)
{
	if( val > max ) val = max;
}

template <class T>
inline void F_Clamp( T& val, const T& l, const T& h )
{
	if( val < l ) val = l;
	if( val > h ) val = h;
}

template <class T>
inline T F_Clamp2( const T& val, const T& l, const T& h )
{
	if( val < l ) return l;
	if( val > h ) return h;
	return val;
}

template <class T>
inline void F_Swap( T& v1, T& v2 )
{
	const T tmp = v1;
	v1 = v2;
	v2 = tmp;
}

///////////////////////////////////////////////////////////////////////////
//  
//  Some Sort Functions
//  
///////////////////////////////////////////////////////////////////////////

template <class T>
class F_CompareLess
{
public:
	static bool Compare(T a, T b) { return a < b; }
};

template <class T>
class F_SimpleSwapFn
{
public:
	static void Swap(T& a, T& b)
	{
		T rhs = a;
		a = b;
		b = rhs;
	}
};

/** Sort utilities.
*/
template <class T, class CmpFn = F_CompareLess<T>, class SwapFn = F_SimpleSwapFn<T> >
class FSortUtil
{
public:

	// Insert sort.
	inline void F_SortInsert(T* pData, uint32 nCount)
	{
	}

	// Choice sort.
	inline void F_SortChoice(T* pData, uint32 nCount)
	{
	}

	// Shell sort, (non-stable)
	inline void F_SortShell(T* pData, uint32 nCount)
	{		
	}

	// Bubble sort. (stable, slowest)
	inline void F_SortBubble(T* pData, uint32 nCount)
	{
		for(uint32 i=0; i<nCount; i++)
		{
			for(uint32 j=i+1; j<nCount; j++)
			{
				if( !CmpFn::Compare(pData[i], pData[j]) )
					SwapFn::Swap(pData[i], pData[j]);
			}
		}
	}
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_TEMPLATE_H__