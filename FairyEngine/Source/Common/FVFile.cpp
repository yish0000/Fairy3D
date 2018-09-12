/*
 * ------------------------------------------------------------------------
 *  Name:   FVFile.cpp
 *  Desc:   This file define the object to handle the files for engine.
 *  Author: Yish
 *  Date:   2011/12/9
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FVFile.h"
#include "FVFileSystem.h"
#include "FStrUtil.h"
#include "FSysFileStream.h"
#include "FLogManager.h"
#include "FTempMemBuffer.h"
#include "FSysAPI.h"

///////////////////////////////////////////////////////////////////////////
//  
//  Define some macros
//  
///////////////////////////////////////////////////////////////////////////

#define MAX_STRING_BUF_SIZE 4096

///////////////////////////////////////////////////////////////////////////

/** Constructor.
*/
FVFile::FVFile()
{
	m_bWriteFile = false;
	m_nTimeStamp = 0;
	m_pFile = NULL;
}

/** Destructor.
*/
FVFile::~FVFile()
{
	Close();
}

/** Open a specified file.
@remarks
	Search it from file package first, if not found then look for it on the disk.
*/
bool FVFile::Open( const char* filename, int iFlags )
{
	// Close the last file.
	Close();

	if( iFlags & VFILE_OPENEXIST )
	{
		if( AStringUtil::IsAbsPath(filename) )
		{
			FILE* pFile = fopen( filename,"rb" );
			if( !pFile ) return false;
			m_pIFile = new FSysFileStream( filename,pFile );
			m_nTimeStamp = FSysAPI::GetFileTimeStamp(filename);
		}
		else
		{
			AString innerFile;
			FVFilePack* pPack = FVFileSystem::GetInstance().SearchVFilePack( filename,innerFile );
			if( pPack )
			{
				m_pIFile = pPack->Open( innerFile );
				if( !m_pIFile ) return false;
			}
			else
			{
				AString strFile = FVFileSystem::GetInstance().GetBaseDir() + "\\" + filename;
				FILE* pFile = fopen( strFile.c_str(),"rb" );
				if( !pFile ) return false;
				m_pIFile = new FSysFileStream( strFile,pFile );
				m_nTimeStamp = FSysAPI::GetFileTimeStamp(strFile.c_str());
			}
		}

		m_bWriteFile = false;
	}
	else
	{
		AString strMode;
		if( iFlags & VFILE_CREATENEW )
			strMode = "w";
		else if( iFlags & VFILE_APPEND )
			strMode = "a";
		if( (iFlags & VFILE_BINARY) )
			strMode += "b";
		AString strFile = AStringUtil::IsAbsPath(filename) ? filename :
			FVFileSystem::GetInstance().GetBaseDir() + "\\" + filename;
		m_pOFile = fopen( strFile.c_str(),strMode.c_str() );
		if( !m_pOFile ) return false;

		m_bWriteFile = true;
	}

	return true;
}

/** Close the current file.
*/
void FVFile::Close()
{
	if( !m_pFile )
		return;

	m_sFilename = "";
	m_nTimeStamp = 0;

	if( m_bWriteFile )
	{
		fclose( m_pOFile );
		m_pOFile = NULL;
	}
	else
	{
		m_pIFile->Close();
		delete m_pIFile;
		m_pIFile = NULL;
	}
}

/** Get the size of file.
*/
size_t FVFile::GetFileSize() const
{
	if( !m_pFile )
		return 0;

	if( m_bWriteFile )
	{
		long offset = ftell( m_pOFile );
		fseek( m_pOFile,0,SEEK_END );
		size_t nSize = (size_t)ftell( m_pOFile );
		fseek( m_pOFile,offset,SEEK_SET );
		return nSize;
	}
	else
	{
		return m_pIFile->GetStreamSize();
	}
}

/** Seek the specified position.
*/
void FVFile::Seek( long nPos, int flag )
{
	if( !m_pFile )
		return;

	if( m_bWriteFile )
	{
		fseek(m_pOFile, nPos, flag);
	}
	else
	{
		m_pIFile->Seek(nPos, flag);
	}
}

/** Get current position in the current file.
*/
size_t FVFile::Tell() const
{
	if( !m_pFile )
		return 0;

	if( m_bWriteFile )
	{
		return ftell( m_pOFile );
	}
	else
    {
        return m_pIFile->Tell();
    }
}

/** Is the pointer reach the end of file?
*/
bool FVFile::Eof() const
{
	if( !m_pFile )
		return false;

	if( m_bWriteFile )
	{
		return feof(m_pOFile) ? true : false;
	}
	else
    {
        return m_pIFile->Eof();
    }
}

/** Skip some bytes.
*/
void FVFile::Skip( long nCount )
{
	if( !m_pFile )
		return;

	if( m_bWriteFile )
	{
		fseek( m_pOFile, nCount, SEEK_SET );
	}
	else
	{
		m_pIFile->Skip(nCount);
	}
}

/** Read some data from the file.
*/
size_t FVFile::Read( void* buf, size_t nSize )
{
	if (m_bWriteFile || !m_pIFile)
	{
		FLOG_ERRORF("FVFile::Read, This file is in writing mode, we cannot read some data! (%s)", m_sFilename.c_str());
		return 0;
	}

	return m_pIFile->Read(buf, nSize);
}

/** Read a line from the file.
*/
size_t FVFile::ReadLine( char* pBuf, size_t maxLen, const AString& delimiter/* = */ )
{
	if (m_bWriteFile || !m_pIFile)
	{
		FLOG_ERRORF("FVFile::ReadLine, This file is in writing mode, we cannot read some data! (%s)", m_sFilename.c_str());
		return 0;
	}

	return m_pIFile->ReadLine(pBuf, maxLen, delimiter);
}

/** Skip a line.
*/
size_t FVFile::SkipLine( const AString& delimiter/* = */ )
{
	if (m_bWriteFile || !m_pIFile)
	{
		FLOG_ERRORF("FVFile::SkipLine, This file is in writing mode, we cannot read some data! (%s)", m_sFilename.c_str());
		return 0;
	}

	return m_pIFile->SkipLine(delimiter);
}

/** Read a line from file.
*/
size_t FVFile::GetLine( AString& retStr, bool bTrimLine/* =true */ )
{
	if (m_bWriteFile || !m_pIFile)
	{
		FLOG_ERRORF("FVFile::GetLine, This file is in writing mode, we cannot read some data! (%s)", m_sFilename.c_str());
		return 0;
	}

	return m_pIFile->GetLine(retStr, bTrimLine);
}

/** Write some data to the file.
*/
size_t FVFile::Write( const void* buf, size_t nSize )
{
	if (!m_bWriteFile || !m_pOFile)
	{
		FLOG_ERRORF("FVFile::WriteString, This file is in writing mode, we cannot write someting! (%s)", m_sFilename.c_str());
		return 0;
	}

	return fwrite(buf, nSize, 1, m_pOFile);
}

/** Write some string to the file.
*/
size_t FVFile::WriteString( const char* format, ... )
{
	if (!m_bWriteFile || !m_pOFile)
	{
		FLOG_ERRORF("FVFile::WriteString, This file is in writing mode, we cannot write someting! (%s)", m_sFilename.c_str());
		return 0;
	}

	FTempStringBuffer strBuf(MAX_STRING_BUF_SIZE);
    va_list argList;
    va_start( argList,format );
    int iLen = vsnprintf( strBuf.GetBuffer(), MAX_STRING_BUF_SIZE, format, argList );
    if( iLen < 0 || iLen >= MAX_STRING_BUF_SIZE )
    {
		FASSERT(0);
	    FLOG_ERROR("FVFile::WriteString, Format the string failed, write exceeds buffer!");
	    return 0;
    }
    va_end( argList );
    fwrite( strBuf.GetBuffer(), 1, iLen, m_pOFile );
    return iLen;
}

/** Flush the file. (Only write mode)
*/
void FVFile::Flush()
{
	if( m_bWriteFile && m_pOFile )
	{
		fflush(m_pOFile);
	}
	else
	{
		FLOG_ERRORF("FVFile::Flush, We cannot flush when file is in reading mode!");
	}
}

/** Whether the file has BOM of UTF-16LE.
*/
bool FVFile::HasUTF16BOM()
{	
	if( m_bWriteFile || !m_pIFile )
		return false;

	fbyte magic[2];
	size_t offset = Tell();
	Seek( 0, FVFileStream::START );
	Read( magic, sizeof(fbyte)*2 );
	Seek( (long)offset, FVFileStream::START );
	return magic[0] == 0xff && magic[1] == 0xfe;
}

/** Whether the file has BOM of UTF-8.
*/
bool FVFile::HasUTF8BOM()
{
	if( m_bWriteFile || !m_pIFile )
		return false;

	fbyte BOM[3];
	size_t offset = Tell();
	Seek(0, FVFileStream::START);
	Read(BOM, sizeof(fbyte)*3);
	Seek((long)offset, FVFileStream::START);
	return BOM[0] == 0xef && BOM[1] == 0xbb && BOM[2] == 0xbf;
}

///////////////////////////////////////////////////////////////////////////
//// Read and write some string to the file.

template <> FVFile& FVFile::operator >> ( AString& val )
{
	uint32 nLen = 0;
	Read(&nLen, sizeof(uint32));
	val.resize(nLen, '\0');
	Read(&val[0], nLen);
	return *this;
}

template <> FVFile& FVFile::operator << (const AString& val)
{
	uint32 nLen = (uint32)val.length();
	Write(&nLen, sizeof(uint32));
	Write(&val[0], nLen);
	return *this;
}

template <> FVFile& FVFile::operator >> (WString& val)
{
	uint32 nLen = 0;
	Read(&nLen, sizeof(uint32));
	val.resize(nLen, L'\0');
	Read(&val[0], nLen * sizeof(wchar_t));
	return *this;
}

template <> FVFile& FVFile::operator << (const WString& val)
{
	uint32 nLen = (uint32)val.length();
	Write(&nLen, sizeof(uint32));
	Write(&val[0], nLen * sizeof(wchar_t));
	return *this;
}