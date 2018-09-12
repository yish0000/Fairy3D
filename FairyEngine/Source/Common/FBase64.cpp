/*
 * ------------------------------------------------------------------------
 *  Name:   FBase64.cpp
 *  Desc:   This file implements the base64 encryption algorithm.
 *  Author: Yish
 *  Date:   2012/8/27
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FBase64.h"

#define BASE64_INVALID_CHAR 64

// The default key.
static const char BASE64_DEFAULTKEY[] =
{
    'A','B','C','D','E','F','G','H','I','J','K','L','M',
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    'a','b','c','d','e','f','g','h','i','j','k','l','m',
    'n','o','p','q','r','s','t','u','v','w','x','y','z',
    '0','1','2','3','4','5','6','7','8','9','+','/','=',
};

/** Constructor.
*/
FBase64::FBase64()
{
    SetKey(NULL);
}

/** Change the key.
@param If the parameter is null, then use the default key.
*/
void FBase64::SetKey( const char* key )
{
    const char* kk;
    if( !key || strlen(key) < 65 )
        kk = BASE64_DEFAULTKEY;
    else
        kk = key;

    memcpy(m_Key, kk, sizeof(m_Key));
    memset(m_KeyMap, 0xff, sizeof(m_KeyMap));
    for( int i=0; i<65; i++ ) m_KeyMap[kk[i]] = i;
}

/** Encode the specified text.
*/
void FBase64::Encode( const char* src, AString& dest )
{
    size_t nSrcLen = strlen(src);
    size_t rowCount = nSrcLen / 3;
    size_t leftCount = nSrcLen % 3;
    size_t nDestLen = rowCount * 4 + (leftCount > 0 ? 4 : 0);
    dest.resize(nDestLen);
    
    size_t srcOff = 0;
    size_t destOff = 0;
    if( leftCount > 0 ) rowCount++;
    for(size_t i=0; i<rowCount; ++i)
    {
        char a = srcOff < nSrcLen ? src[srcOff] : 0;
        char b = (srcOff+1 < nSrcLen) ? src[srcOff+1] : 0;
        char c = (srcOff+2 < nSrcLen) ? src[srcOff+2] : 0;
        
        char k = (a & 0xfc) >> 2;
        char l = ((a & 0x3) << 4) | ((b & 0xf0) >> 4);
        char m = ((b & 0xf) << 2) | ((c & 0xc0) >> 6);
        char n = c & 0x3f;

        // The last row.
        if( i == rowCount - 1 )
        {
            if( leftCount == 1 )
                m = n = BASE64_INVALID_CHAR;
            else if( leftCount == 2 )
                n = BASE64_INVALID_CHAR;
        }

        dest[destOff] = m_Key[k];
        dest[destOff+1] = m_Key[l];
        dest[destOff+2] = m_Key[m];
        dest[destOff+3] = m_Key[n];

        srcOff += 3;
        destOff += 4;
    }
}

/** Decode the specified text.
*/
void FBase64::Decode( const char* src, AString& dest )
{
    size_t nSrcLen = strlen(src);
    size_t rowCount = nSrcLen / 4;
    size_t nDestLen = rowCount * 3;
    dest.resize(nDestLen);
    
    size_t srcOff = 0;
    size_t destOff = 0;
    for(size_t i=0; i<rowCount; ++i)
    {
        char a = src[srcOff];
        char b = src[srcOff+1];
        char c = src[srcOff+2];
        char d = src[srcOff+3];
        
        char e = m_KeyMap[a];
        char f = m_KeyMap[b];
        char g = m_KeyMap[c];
        char h = m_KeyMap[d];
        
        // The last row.
        if( i == rowCount - 1 )
        {
            if( a == '=' ) e = 0;
            if( b == '=' ) f = 0;
            if( c == '=' ) g = 0;
            if( d == '=' ) h = 0;
        }
        
        char k = (e << 2) | (f >> 4);
        char l = ((f & 0xf) << 4) | ((g & 0x3c) >> 2);
        char m = ((g & 0x3) << 6) | h;
        
        dest[destOff] = k;
        dest[destOff+1] = l;
        dest[destOff+2] = m;
        
        srcOff += 4;
        destOff += 3;
    }
}