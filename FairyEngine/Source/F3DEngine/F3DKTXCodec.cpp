/*
 * ------------------------------------------------------------------------
 *  Name:   F3DKTXCodec.cpp
 *  Desc:   This file implements the ktx image codec.
 *  Author: Yish
 *  Date:   2015/3/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DKTXCodec.h"

/** Constructor
*/
F3DKTXCodec::F3DKTXCodec()
{
	m_sCodecName = "KTX";
}

/** Destructor
*/
F3DKTXCodec::~F3DKTXCodec()
{
}

/** Load the texture from specified file.
*/
F3DImage* F3DKTXCodec::Load( const char* filename )
{
	FVFile file;
	if( !file.Open(filename, FVFile::VFILE_OPENEXIST) )
	{
		FLOG_WARNINGF( "F3DKTXCodec::Load, Open image file (%s) failed!",filename );
		return NULL;
	}

	return LoadImageFromVFile(&file);
}

/** Load the image data from file stream.
*/
F3DImage* F3DKTXCodec::LoadImageFromVFile( FVFile* pFile )
{
	return NULL;
}

/** Save the specified mipmap to file. 
*/
void F3DKTXCodec::SaveMipmap( const char* filename, F3DImage* image, size_t level )
{
}