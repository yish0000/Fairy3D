/*
 * ------------------------------------------------------------------------
 *	Name:   F3DImageCodec.h
 *  Desc:   This file define the base class of image codec.
 *  Author: Yish
 *  Date:   2010/8/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish. 2010- All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_IMAGECODEC_H__
#define __F3D_IMAGECODEC_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DImage.h"

///////////////////////////////////////////////////////////////////////////


/** Base class of image codec.
*/
class FAIRY_API F3DImageCodec : public FGeneralAlloc
{
protected:
    AString m_sCodecName;

public:
    F3DImageCodec(void) {};
    virtual ~F3DImageCodec(void) {};

	// Load the specified image.
    virtual F3DImage* Load( const char* filename ) = 0;

	// Save the specified image to file.
	virtual void Save( const char* filename, F3DImage* image ) { SaveMipmap(filename, image, 0); }

	// Save the specified mipmap of image to file.
    virtual void SaveMipmap( const char* filename, F3DImage* image, size_t level ) = 0;

	// Get the name of image codec.
    const AString& GetCodecName(void) const { return m_sCodecName; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_IMAGECODEC_H__