/*
 * ------------------------------------------------------------------------
 *  Name:   FTextTabFile.h
 *  Desc:   This file define the table text file exported by Excel.
 *  Author: Yish
 *  Date:   2014/3/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_TEXTTABFILE_H__
#define __FAIRY_TEXTTABFILE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

/** UTF-8 Version.
*/
class FTextTabFile : public FGeneralAlloc
{
	typedef std::vector<AStringList> TextTable;

public:
	FTextTabFile();
	virtual ~FTextTabFile();

	// Open the specified text file
	bool Open(const char* filename);
	// Close the file
	void Close();

	// Get the specified text
	const AString& GetCell(int iRow, int iCol) const;
	// Get column count of specified row
	uint32 GetNumCol(int iRow) const;
	// Get row count
	uint32 GetNumRow() const { return (uint32)m_TextTab.size(); }

protected:
	TextTable m_TextTab;	// Text data

	// Load the data from file stream.
	bool LoadFromVFile(FVFile* pFile);
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_TEXTTABFILE_H__