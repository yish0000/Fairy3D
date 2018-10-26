/*
 * ------------------------------------------------------------------------
 *  Name:   FVFileStream.h
 *  Desc:   This file define the base class of the file stream.
 *  Author: Yish
 *  Date:   2010/12/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_VFILESTREAM_H__
#define __FAIRY_VFILESTREAM_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

/** Base class of the file stream.
*/
class FVFileStream
{
public:		// Types
    
    enum { START, CURRENT, END };

protected:
    AString m_sName;        // Name of the current stream.
    size_t m_nStreamSize;   // Size of the current stream.

public:
    FVFileStream();
	FVFileStream(const AString& name);
    virtual ~FVFileStream(void);

	virtual void Seek(long nPos, int flag = START) = 0;
    virtual size_t Tell(void) const = 0;
    virtual bool Eof(void) const = 0;
	virtual void Skip(long nCount) = 0;
    virtual void Close(void) = 0;

    /** Read some data from the stream.
    @Param Pointer to the buffer.
    @Param Size to read.
    @Return The final size which is read.
    */
	virtual size_t Read(void* pBuf, size_t nSize) = 0;

	/** Write some data to the stream.
	@Param Pointer to the buffer.
	@Param Size to read.
	@Return The final size which is written.
	*/
	virtual size_t Write(void* pBuf, size_t nSize) = 0;

    /** Read some data.
    */
    template <class T>
	FVFileStream& operator >> (T& val)
    {
        Read( &val, sizeof(T) );
        return *this;
    }

	/** Write some data.
	*/
	template <class T>
	FVFileStream& operator << (T& val)
	{
		Write(&val, sizeof(T));
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////
	// Read strings. (UTF-8)

	virtual size_t ReadLine(char* pBuf, size_t maxLen, const AString& delimiter = "\n");
	virtual size_t SkipLine(const AString& delimiter = "\n");

	// Read a line. (UTF-8)
	virtual size_t GetLine(AString& retStr, bool bTrimLine = true);

	///////////////////////////////////////////////////////////////////////////

    // Get the name of stream.
    const AString& GetName(void) const { return m_sName; }
    
    // Get the size of stream.
    size_t GetStreamSize(void) const { return m_nStreamSize; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_VFILESTREAM_H__