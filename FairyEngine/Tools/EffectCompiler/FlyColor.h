//------------------------------------------------------------------------
// Name: FlyColor.h
// Desc: This file define struct to act as the color value.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_COLOR_H__
#define __FLY_COLOR_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"

//////////////////////////////////////////////////////////////////////////

struct FLYCOLOR
{
    float fR;           // Red color value
    float fG;           // Green color value
    float fB;           // Blue color value
    float fA;           // Alpha value

    //////////////////////////////////////////////////////////////////////
    // Some functions

    bool IsNull(void)
    {
        if( fR == 0.0f && fG == 0.0f && fB == 0.0f && fA == 0.0f )
            return true;
        else
            return false;
    }

    DWORD GetRGBAValue(void)
    {
        UCHAR r = fR * 255;
        UCHAR g = fG * 255;
        UCHAR b = fB * 255;
        UCHAR a = fA * 255;

        return MAKE3DCOLOR( r,g,b,a );
    }

    //////////////////////////////////////////////////////////////////////
    // Some operations

    FLYCOLOR operator + ( FLYCOLOR& color ) const
    {
        FLYCOLOR clr;
        clr.fR = fR + color.fR;
        clr.fG = fG + color.fG;
        clr.fB = fB + color.fB;
        clr.fA = fA + color.fA;
        return clr;
    }

    void operator += ( FLYCOLOR& color )
    {
        fR += color.fR;
        fG += color.fG;
        fB += color.fB;
        fA += color.fA;
    }

    FLYCOLOR operator - ( FLYCOLOR& color ) const
    {
        FLYCOLOR clr;
        clr.fR = fR - color.fR;
        clr.fG = fG - color.fG;
        clr.fB = fB - color.fB;
        clr.fA = fA - color.fA;
        return clr;
    }

    void operator -= ( FLYCOLOR& color )
    {
        fR -= color.fR;
        fG -= color.fG;
        fB -= color.fB;
        fA -= color.fA;
    }

    //////////////////////////////////////////////////////////////////////
    // Static function.
    static FLYCOLOR Red(void)
    {
        FLYCOLOR res;
        res.fR = 1.0f;
        res.fG = 0.0f;
        res.fB = 0.0f;
        res.fA = 1.0f;
        return res;
    }

    static FLYCOLOR Green(void)
    {
        FLYCOLOR res;
        res.fR = 0.0f;
        res.fG = 1.0f;
        res.fB = 0.0f;
        res.fA = 1.0f;
        return res;
    }

    static FLYCOLOR Blue(void)
    {
        FLYCOLOR res;
        res.fR = 0.0f;
        res.fG = 0.0f;
        res.fB = 1.0f;
        res.fA = 0.0f;
        return res;
    }

    static FLYCOLOR White(void)
    {
        FLYCOLOR res;
        res.fR = 1.0f;
        res.fG = 1.0f;
        res.fB = 1.0f;
        res.fA = 1.0f;
        return res;
    }

    static FLYCOLOR Black(void)
    {
        FLYCOLOR res;
        res.fR = 0.0f;
        res.fG = 0.0f;
        res.fB = 0.0f;
        res.fA = 1.0f;
        return res;
    }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_COLOR_H__