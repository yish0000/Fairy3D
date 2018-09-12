//------------------------------------------------------------------------
// Name: FlyImageCodec.h
// Desc: This file define a class to load the images. (*.bmp,*.jpg,*.tga
//       *.dds,*.pcx,*.tex,*.png).
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_IMAGE_CODEC_H__
#define __FLY_IMAGE_CODEC_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


typedef struct _FLYIMAGEDATA
{
    int nWidth;
    int nHeight;
    PIXELFORMAT Format;
    int nPixelBytes;
    BYTE* pData;
} FLYIMAGEDATA;

//////////////////////////////////////////////////////////////////////////
// Class to load all kinds of image.
class _FLY_EXPORT FlyImageCodec
{
public:
    // Constructor and destructor.
    FlyImageCodec(void) { /* Nothing to do! */ };
    ~FlyImageCodec(void) { /* Nothing to do! */ };

    HRESULT LoadImage( const char* cFilename,FLYIMAGEDATA* pOut );
    HRESULT LoadImageFromTEX( const char* cFilename,FLYIMAGEDATA* pOut );
    HRESULT LoadImageFromBitmap( const char* cFilename,FLYIMAGEDATA* pOut );
    HRESULT LoadImageFromJPEG( const char* cFilename,FLYIMAGEDATA* pOut );
    HRESULT LoadImageFromTGA( const char* cFilename,FLYIMAGEDATA* pOut );
    HRESULT LoadImageFromPNG( const char* cFilename,FLYIMAGEDATA* pOut );

    void WriteImageToTEX( const char* cFilename,FLYIMAGEDATA* pOut );
    void WriteImageToBitmap( const char* cFilename,FLYIMAGEDATA* pOut );
    void WriteImageToJPEG( const char* cFilename,FLYIMAGEDATA* pOut );
    void WriteImageToTGA( const char* cFilename,FLYIMAGEDATA* pOut );
    void WriteImageToPNG( const char* cFilename,FLYIMAGEDATA* pOut );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_IMAGE_CODEC_H__