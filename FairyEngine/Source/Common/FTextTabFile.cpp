/*
 * ------------------------------------------------------------------------
 *  Name:   FTextTabFile.cpp
 *  Desc:   This file define the table text file exported by Excel.
 *  Author: Yish
 *  Date:   2014/3/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Studio. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FVFile.h"
#include "FTextTabFile.h"

#include "FLogManager.h"

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FTextTabFile
//  
///////////////////////////////////////////////////////////////////////////

FTextTabFile::FTextTabFile()
{
}

FTextTabFile::~FTextTabFile()
{
}

bool FTextTabFile::Open(const char* filename)
{
	FVFile file;
	if( !file.Open(filename, FVFile::VFILE_OPENEXIST) )
	{
		FLOG_WARNINGF("FTextTabFile::Open, Failed to load the text file (%s)!", filename);
		return false;
	}

	return LoadFromVFile(&file);
}

void FTextTabFile::Close()
{
	m_TextTab.clear();
}

bool FTextTabFile::LoadFromVFile(FVFile* pFile)
{
	return true;
}