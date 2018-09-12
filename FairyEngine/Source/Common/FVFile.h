/*
 * ------------------------------------------------------------------------
 *  Name:   FVFile.h
 *  Desc:   This file define the object to handle the files for engine.
 *  Author: Yish
 *  Date:   2011/12/9
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_VFILE_H__
#define __FAIRY_VFILE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"
#include "FVFileStream.h"

///////////////////////////////////////////////////////////////////////////

/** File object.
@remarks
	This class is used to handle a file, For users they need not to care the source of file.
	We will use this class to access the file in the package or on the disk.
	We read the file package first, if not found then look for it on the disk.
@note
	Can't read and write at the same time, Can't write file to the package.
*/
class FVFile : public FGeneralAlloc
{
public:		// Types

	enum
	{
		VFILE_OPENEXIST	= 1,	// Open an existing file.
		VFILE_CREATENEW	= 2,	// Create a new file.
		VFILE_APPEND	= 4,	// Append some context to an existing file.
		VFILE_BINARY	= 8,	// Read or write the binary file.
	};

	enum { START, CURRENT, END };	// Seek Flags

public:
    FVFile(void);
    virtual ~FVFile(void);

	// Open a specified file.
    bool Open( const char* filename, int iFlags = VFILE_OPENEXIST );
	// Close the file
    void Close();

	// Move the pointer.
    void Seek( long nPos, int flag );
	// Get the current pointer to file.
    size_t Tell() const;
	// Do we reach the end ?
    bool Eof() const;
	// Skip some bytes of the file
    void Skip( long nCount );

	// Get the file size.
	size_t GetFileSize() const;
	// Get the name of file.
	const AString& GetFileName() const { return m_sFilename; }
	// Get the timestamp of file.
	uint32 GetTimeStamp() const { return m_nTimeStamp; }

	///////////////////////////////////////////////////////////////////////
	// Read functions

	size_t Read( void* buf, size_t nSize );
	size_t ReadLine( char* pBuf, size_t maxLen, const AString& delimiter="\n" );
	size_t SkipLine( const AString& delimiter="\n" );
	size_t GetLine( AString& retStr, bool bTrimLine=true );

	template <class T>
	FVFile& operator >> ( T& val )
	{
		Read( &val,sizeof(T) );
		return *this;
	}

	bool HasUTF16BOM();
	bool HasUTF8BOM();

	///////////////////////////////////////////////////////////////////////
	// Write functions

	size_t Write( const void* buf, size_t nSize );
	size_t WriteString( const char* format, ... );

	template <class T>
	FVFile& operator << ( const T& val )
	{
		Write( &val,sizeof(T) );
		return *this;
	}

	// Flush the data
	void Flush();

protected:
	bool m_bWriteFile;			// Is this file in writing mode ?
	AString m_sFilename;		// Name of the file.
	uint32 m_nTimeStamp;		// Time stamp of the file.

	union
	{
		void* m_pFile;				// File pointer.
		FVFileStream* m_pIFile;		// Input file stream.
		FILE* m_pOFile;				// Output file stream.
	};
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_VFILE_H__