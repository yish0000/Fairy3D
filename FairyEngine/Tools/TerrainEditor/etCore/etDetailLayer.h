//------------------------------------------------------------------------
// Name: etDetailLayer.h
// Desc: This file define a class to store the detail layer data.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_DETAILLAYER_H__
#define __ET_DETAILLAYER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etCommon.h"

#include "FlyTexture.h"
//////////////////////////////////////////////////////////////////////////


/*
* To store the detail layer data.
* Red,Green,Blue,Alpha. So each tile can have four detail textures.
*/
class _ET_EXPORT etDetailLayer
{
protected:
    FlyTexture* m_pTexture; // Pointer to the texture.

    UINT m_nMapSize;        // Size of the alphamap.
    UCHAR* m_pTempData;     // To store the temp data of the alphamap.
    bool m_bModified;       // Is the detail layer modified ?

public:
    // Constructor and destructor.
    etDetailLayer(void);
    ~etDetailLayer(void);

    // Initialize the detail layer.
    HRESULT Initialize( UINT nMapSize );

    // Destroy the detail layer, clean the memory.
    void DestroyLayer(void);

    // Load the data from an image file.
    HRESULT LoadLayerFromImage( const char* cFilename );

    // Save the data to an image file.
    HRESULT SaveLayerToImage( const char* cFilename );

    UINT GetAlphamapSize(void) const { return m_nMapSize; }
    UCHAR* GetTempBuffer(void) { return m_pTempData; }

    UCHAR GetAlphaValue( int x,int y,int nLayer );
    void SetAlphaValue( int x,int y,int nLayer,UCHAR ucValue );

    void UpdateTextureData(void);

    void SetModified( bool bModified ) { m_bModified = bModified; }
    FlyTexture* GetTexture(void) { return m_pTexture; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_DETAILLAYER_H__