/*
 * ------------------------------------------------------------------------
 *  Name:   FTokenFile.h
 *  Desc:   本文件定义了一个简单的脚本解析文件，可将文件流解析为单词序列。
 *  Author: Yish
 *  Date:   2011/11/27
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_TOKENFILE_H__
#define __FAIRY_TOKENFILE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

/** Class FTokenFile
@remarks
    以空格,逗号,\t,分号为分隔符
    (),"",'' 之内为一个单词, 支持//和/*的注释
*/
class FTokenFile
{
public:		// Types

	enum { MAX_LINELEN = 2048 };

    struct TOKENINFO
    {
        char* pStart;
        char* pCur;
        char* pEnd;
    };

public:
    FTokenFile(void);
    virtual ~FTokenFile(void);

	// Load the script file.
	bool Load( const char* filename );

    // Close the token file.
    void Close();

	// Get the next token, and move the file pointer.
	// @Param Do we cross the line ?
	bool GetNextToken( bool bCrossLine );
	// Get the next token, but we don't move the file pointer.
	bool PeekNextToken( bool bCrossLine );

    // Get the current token string.
    const char* GetTokenAsString() const { return m_szToken; }
	// Get the current token as an integer.
	int GetTokenAsInt() const { return atoi(m_szToken); }
	// Get the current token as a float.
	float GetTokenAsFloat() const { return (float)atof(m_szToken); }
	// Get the current token as an unsigned integer.
	uint32 GetTokenAsUint() const
	{
		uint32 nVal = 0;
		sscanf(m_szToken, "%u", &nVal);
		return nVal;
	}

	// Skip the current line.
	void SkipLine();

	// Find the next matched token.
	bool MatchToken( const char* szToken, bool bCaseSensitive );

	// Reach the end of file.
	bool IsEnd() const;

	// Get the current line.
    int GetCurLine() const { return m_iCurLine; }

protected:
    char m_szToken[MAX_LINELEN];        // To store the current token. 
    TOKENINFO* m_pInfo;                 // Token stream.
    int m_iCurLine;                     // Current line in file.

protected:  // Operations

    bool IsSeperators( char ch );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_TOKENFILE_H__