/*
 * ------------------------------------------------------------------------
 *  Name:   F3DColor.h
 *  Desc:   This file define the color types for engine.
 *  Author: Yish
 *  Date:   2010/11/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_COLOR_H__
#define __F3D_COLOR_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

// Macros to make color value.
#define F3DCOLOR_RGBA(r,g,b,a) (uint32)(((a)<<24) | ((r)<<16) | ((g)<<8) | (b))
#define F3DCOLOR_RGB(r,g,b) F3DCOLOR_RGBA(r,g,b,255)
#define F3DCOLOR_BGRA(b,g,r,a) F3DCOLOR_RGBA(r,g,b,a)
#define F3DCOLOR_GETRED(color) ((color & 0x00ff0000) >> 16)
#define F3DCOLOR_GETGREEN(color) ((color & 0x0000ff00) >> 8)
#define F3DCOLOR_GETBLUE(color) ((color & 0x000000ff))
#define F3DCOLOR_GETALPHA(color) ((color & 0xff000000) >> 24)

/** Color 8Bit
*/
class FAIRY_API F3DColorI
{
public:

    union
    {
        struct
        {
            unsigned char b;
            unsigned char g;
            unsigned char r;
            unsigned char a;
        };

        unsigned long color;
    };

public:
    F3DColorI(void) : color(0) {}
    F3DColorI( uchar R, uchar G, uchar B, uchar A = 255 ) : r(R), g(G), b(B), a(A) {}
    F3DColorI( uint32 nColor ) : color(nColor) {}

	// Operator ==
	bool operator == (const F3DColorI& clr) const
	{
		return color == clr.color;
	}

	// Operator !=
	bool operator != (const F3DColorI& clr) const
	{
		return color != clr.color;
	}

	// Operator +
    F3DColorI operator + ( const F3DColorI& clr ) const
    {
        F3DColorI ret;
        ret.r = r + clr.r;
        ret.g = g + clr.g;
        ret.b = b + clr.b;
        ret.a = a + clr.a;
        return ret;
    }

	// Operator +=
    F3DColorI& operator += ( const F3DColorI& clr )
    {
        r += clr.r;
        g += clr.g;
        b += clr.b;
        a += clr.a;
		return *this;
    }

	// Operator -
    F3DColorI operator - ( const F3DColorI& clr ) const
    {
        F3DColorI ret;
        ret.r = r - clr.r;
        ret.g = g - clr.g;
        ret.b = b - clr.b;
        ret.a = a - clr.a;
        return ret;
    }

	// Operator -=
    F3DColorI& operator -= ( const F3DColorI& clr )
    {
        r -= clr.r;
        g -= clr.g;
        b -= clr.b;
        a -= clr.a;
		return *this;
    }
};

/** Color value for 32 bit.
*/
class FAIRY_API F3DColor
{
public:

	// Define some constant variables.
	static const F3DColor Red;
	static const F3DColor Green;
	static const F3DColor Blue;
	static const F3DColor White;
	static const F3DColor Black;

public:
    float fR, fG, fB, fA;

public:

	// Constructors
    F3DColor() : fR(0.0f), fG(0.0f), fB(0.0f), fA(1.0f) {}
    F3DColor(float r, float g, float b, float a = 1.0f) : fR(r), fG(g), fB(b), fA(a) {}
	F3DColor(const F3DColor& clr) : fR(clr.fR), fG(clr.fG), fB(clr.fB), fA(clr.fA) {}
    F3DColor(const F3DColorI& clr)
    {
		const float fInv = 1.0f / 255.0f;
        fA = (float)clr.a * fInv;
        fR = (float)clr.r * fInv;
        fG = (float)clr.g * fInv;
        fB = (float)clr.b * fInv;
    }

    F3DColor( uint32 nColor )
    {
        const float fInv = 1.0f / 255.0f;
        fA = (float)((nColor & 0xff000000) >> 24) * fInv;
        fR = (float)((nColor & 0x00ff0000) >> 16) * fInv;
        fG = (float)((nColor & 0x0000ff00) >> 8) * fInv;
        fB = (float)(nColor & 0x000000ff) * fInv;
    }

	// Set the values
	void Set(float r, float g, float b, float a)
	{
		fR = r; fG = g; fB = b; fA = a;
	}

	// Get the color value rgba.
    uint32 GetARGB(void) const
    {
        uchar r = (uchar)(fR * 255);
        uchar g = (uchar)(fG * 255);
        uchar b = (uchar)(fB * 255);
        uchar a = (uchar)(fA * 255);

        return F3DCOLOR_RGBA( r, g, b, a );
    }

	// Get the Color value rgba.
    F3DColorI GetColorI(void) const
    {
        F3DColorI clr;
        clr.r = (uchar)(fR*255);
        clr.g = (uchar)(fG*255);
        clr.b = (uchar)(fB*255);
        clr.a = (uchar)(fA*255);
        return clr;
    }

	// Clamp the color value.
	void Clamp()
	{
		if( fR < 0.0f ) fR = 0.0f; if( fR > 1.0f ) fR = 1.0f;
		if( fG < 0.0f ) fG = 0.0f; if( fG > 1.0f ) fG = 1.0f;
		if( fB < 0.0f ) fB = 0.0f; if( fB > 1.0f ) fB = 1.0f;
		if( fA < 0.0f ) fA = 0.0f; if( fA > 1.0f ) fA = 1.0f;
	}

    ///////////////////////////////////////////////////////////////////////

	// Operator =
	F3DColor& operator = (const F3DColor& rhs)
	{
		fR = rhs.fR; fG = rhs.fG; fB = rhs.fB; fA = rhs.fA;
		return *this;
	}

	// Operator =
	F3DColor& operator = (const F3DColorI& rhs)
	{
		const float fInv = 1.0f / 255.0f;
		fA = (float)rhs.a * fInv;
		fR = (float)rhs.r * fInv;
		fG = (float)rhs.g * fInv;
		fB = (float)rhs.b * fInv;
		return *this;
	}

	// Operator +
    F3DColor operator + ( const F3DColor& color ) const
    {
        F3DColor clr;
        clr.fR = fR + color.fR;
        clr.fG = fG + color.fG;
        clr.fB = fB + color.fB;
        clr.fA = fA + color.fA;
        return clr;
    }

	// Operator +=
    F3DColor& operator += ( const F3DColor& color )
    {
        fR += color.fR;
        fG += color.fG;
        fB += color.fB;
        fA += color.fA;
		return *this;
    }

	// Operator -
    F3DColor operator - ( const F3DColor& color ) const
    {
        F3DColor clr;
        clr.fR = fR - color.fR;
        clr.fG = fG - color.fG;
        clr.fB = fB - color.fB;
        clr.fA = fA - color.fA;
        return clr;
    }

	// Operator -=
    F3DColor& operator -= ( const F3DColor& color )
    {
        fR -= color.fR;
        fG -= color.fG;
        fB -= color.fB;
        fA -= color.fA;
		return *this;
    }

	// Operator *
    F3DColor operator * ( float fValue ) const
    {
        F3DColor clr;
        clr.fR = fR * fValue;
        clr.fG = fG * fValue;
        clr.fB = fB * fValue;
        clr.fA = fA * fValue;
        return clr;
    }

	// Operator *
	friend F3DColor operator * (float fValue, const F3DColor& clr)
	{
		return F3DColor(
			clr.fR * fValue, clr.fG * fValue, clr.fB * fValue, clr.fA * fValue);
	}

	// Operator *=
    F3DColor& operator *= ( float fValue )
    {
        fR *= fValue;
        fG *= fValue;
        fB *= fValue;
        fA *= fValue;
		return *this;
    }

	// Operator /
    F3DColor operator / ( float fValue ) const
    {
        const float fInv = 1.0f / fValue;
        F3DColor clr;
        clr.fR = fR * fInv;
        clr.fG = fG * fInv;
        clr.fB = fB * fInv;
        clr.fA = fA * fInv;
        return clr;
    }

	// Operator /=
    F3DColor& operator /= ( float fValue )
    {
        const float fInv = 1.0f / fValue;
        fR *= fInv;
        fG *= fInv;
        fB *= fInv;
        fA *= fInv;
		return *this;
    }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_COLOR_H__