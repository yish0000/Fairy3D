/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPVRCodec.h
 *  Desc:   This file define an image codec to load pvr file.
 *  Author: Yish
 *  Date:   2015/3/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_PVRCODEC_H__
#define __F3D_PVRCODEC_H__

//// INCLUDE HEADER FILES /////////////////////////////////////////////////
#include "F3DImageCodec.h"

///////////////////////////////////////////////////////////////////////////

/** PVR image codec.
*/
class FAIRY_API F3DPVRCodec : public F3DImageCodec
{
public:
	F3DPVRCodec();
	~F3DPVRCodec();

	// Load the texture from specified file.
	F3DImage* Load( const char* filename );
	// Save the specified mipmap to file. 
	void SaveMipmap( const char* filename,F3DImage* image,size_t level );

private:
	// Load the image data from file stream.
	F3DImage* LoadImageFromVFile( FVFile* pFile );
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_PVRCODEC_H__