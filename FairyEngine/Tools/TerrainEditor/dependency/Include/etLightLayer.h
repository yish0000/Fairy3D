//------------------------------------------------------------------------
// Name: etLightLayer.h
// Desc: This file define a class to generate and store the light data.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_LIGHTLAYER_H__
#define __ET_LIGHTLAYER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etCommon.h"

#include "FlyTexture.h"
//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etLightLayer
{
protected:
    FlyVector m_vDirection;     // Direction of the light.
    FLYCOLOR m_cAmbient;        // Ambient color of the light.
    FLYCOLOR m_cLightColor;     // Light color.

    UINT m_nMapSize;            // Size of the lightmap.
    DWORD* m_pTempData;         // To store the temp data.
    FlyTexture* m_pTexture;     // Pointer to the texture.

public:
    // Constructor and destructor.
    etLightLayer(void);
    ~etLightLayer(void);

    // Initialize the lightMap for the terrain.
    HRESULT Initialize( UINT nMapSize );

    // Destroy the layer, clean the memory.
    void DestroyLayer(void);

    // Import and export the data from RAW file.
    HRESULT ImportLightmap( const char* cFilename );
    HRESULT ExportLightmap( const char* cFilename );

    const FlyVector& GetDirection(void) const { return m_vDirection; }
    const FLYCOLOR& GetAmbientColor(void) const { return m_cAmbient; }
    const FLYCOLOR& GetLightColor(void) const { return m_cLightColor; }

    void SetDirection( const FlyVector& vDirection ) { m_vDirection = vDirection; }
    void SetAmbientColor( const FLYCOLOR& color ) { m_cAmbient = color; }
    void SetLightColor( const FLYCOLOR& color ) { m_cLightColor = color; }

    DWORD* GetTempBuffer(void) { return m_pTempData; }
    UINT GetLightmapSize(void) const { return m_nMapSize; }

    // Get the pointer to the texture.
    FlyTexture* GetTexture(void) { return m_pTexture; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_LIGHTLAYER_H__