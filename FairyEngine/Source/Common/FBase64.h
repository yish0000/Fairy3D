/*
 * ------------------------------------------------------------------------
 *  Name:   FBase64.h
 *  Desc:   This file implements the base64 encryption algorithm.
 *  Author: Yish
 *  Date:   2012/8/27
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_BASE64_H__
#define __FAIRY_BASE64_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

/** Base64
*/
class FBase64
{
public:
    FBase64();

    void Encode( const char* src, AString& dest );
    void Decode( const char* src, AString& dest );

    // Change the key.
    void SetKey( const char* key );

private:
    char m_Key[65];
    char m_KeyMap[256];
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_BASE64_H__