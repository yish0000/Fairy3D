//------------------------------------------------------------------------
// Name: FlyImage.h
// Desc: This file define an image class, This object just use the
//       D3DFMT_A8R8G8B8 format to store datas.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_IMAGE_H__
#define __FLY_IMAGE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


/*************************************************************************
*     Name: class FlyImage                                               *
*     Desc: This class to used to load the Image (*.tex)                 *
*************************************************************************/
class _FLY_EXPORT FlyImage
{
protected:
    UINT        m_nWidth;           // The width of the image (Pixel)
    UINT        m_nHeight;          // The height of the image (Pixel)
    size_t      m_nBufferSize;      // The size of the Buffer. (Byte)
    DWORD*      m_pBuffer;          // The pointer to the buffer.
    INT         m_nFlags;           // A flag if neccessary.

    // Interpolate the pixel. Used when Scale image
    // with linear method.
    DWORD   InterpolatePixel( float x,float y );
    DWORD   InterpolatePixelEx( float x,float y,float fScale );
    DWORD   InterpolateDword( DWORD dw1,DWORD dw2,float f1,float fCur );

public:
    // Standard constructor.
    FlyImage(void);

    // Copy Constructor.
    FlyImage( const FlyImage& image );

    // Standard destructor.
    ~FlyImage(void);

    // '=' Operator override.
    FlyImage& operator = ( const FlyImage& image );

    // Copy another image to this stuff.
    FlyImage& CopyOf( const FlyImage& image );

    // Reset the Content for the Image.
    void Reset(void);

    // Create an empty image with an initial color.
    FlyImage& CreateImage( UINT nWidth,UINT nHeight,DWORD dwInitColor );

    // Flip(Mirror) the Image around Y axis.
    FlyImage& FlipAroundY(void);

    // Flip(Mirror) the Image around X axis.
    FlyImage& FlipAroundX(void);

    // Scale the image.
    void ScaleImage( float fScaleX,float fScaleY,bool bLinear );

    // Load image from memory.
    HRESULT LoadImageFromMemory( UCHAR* pData,PIXELFORMAT format,UINT nWidth,
                                 UINT nHeight );

    // Load Image from the file.
    HRESULT LoadImageFromFile( const char* cFilename );

    // Save a image to a file.
    void SaveImageToFile( const char* cFilename,bool bAlpha );

    // Load Image from Bitmap.
    HRESULT LoadImageFromBitmap( const char* cFilename );

    // Save a image to a Bitmap.
    void SaveImageToBitmap( const char* cFilename,bool bAlpha );

    // Load image from TGA image.
    HRESULT LoadImageFromTGA( const char* cFilename );

    // Save a image to TGA file.
    void SaveImageToTGA( const char* cFilename,bool bAlpha );

    // Load a image from JPEG image.
    HRESULT LoadImageFromJPEG( const char* cFilename );

    // Save a image to JPG file.
    void SaveImageToJPEG( const char* cFilename,bool bAlpha );

    // Load a image from the PNG file.
    HRESULT LoadImageFromPNG( const char* cFilename );

    // Save a image to PNG file.
    void SaveImageToPNG( const char* cFilename,bool bAlpha );

    // Load a grey map from RAW
    HRESULT LoadImageFromRAW( const char* cFilename,int nWidth,int nHeight );

    // Save a image to a grey map.
    void SaveImageToRAW( const char* cFilename );

    // Load an alpha map from a grey map.
    HRESULT LoadAlphaMapFromRAW( const char* cFilename,int nWidth,int nHeight );

    // Get the Pixel value.
    DWORD GetPixelValue( int x,int y );

    // Change the pixel value.
    void  SetPixelValue( int x,int y,DWORD dwColor );

    // Set a RGB value With the alpha A.
    void  SetAlpha( UCHAR r,UCHAR g,UCHAR b,UCHAR alpha );

    // Set the Alpha value transparent.
    void  SetTransparent( UCHAR alpha );

    // Set the image to Grey color.
    void  SetImageToGrey(void);

    UCHAR* GetDataPtr(void) { return (UCHAR*)m_pBuffer; }
    const UCHAR* GetConstDataPtr(void) const { return (UCHAR*)m_pBuffer; }
    UINT   GetWidth(void) const  { return m_nWidth; }
    UINT   GetHeight(void) const { return m_nHeight; }
    size_t GetBufferSize(void) const { return m_nBufferSize; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_IMAGE_H__
