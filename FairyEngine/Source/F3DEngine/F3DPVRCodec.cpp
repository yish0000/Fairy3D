/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPVRCodec.cpp
 *  Desc:   This file implements the pvr image codec.
 *  Author: Yish
 *  Date:   2015/3/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DPVRCodec.h"

/** Constructor
*/
F3DPVRCodec::F3DPVRCodec()
{
	m_sCodecName = "PVR";
}

/** Destructor
*/
F3DPVRCodec::~F3DPVRCodec()
{
}

/** Load the texture from specified file.
*/
F3DImage* F3DPVRCodec::Load( const char* filename )
{
	FVFile file;
	if( !file.Open(filename, FVFile::VFILE_OPENEXIST) )
	{
		FLOG_WARNINGF( "F3DPVRCodec::Load, Open image file (%s) failed!",filename );
		return NULL;
	}

	return LoadImageFromVFile(&file);
}

/** Load the image data from file stream.
*/
F3DImage* F3DPVRCodec::LoadImageFromVFile( FVFile* pFile )
{
	return NULL;
}

/** Save the specified mipmap to file. 
*/
void F3DPVRCodec::SaveMipmap( const char* filename, F3DImage* image, size_t level )
{
}