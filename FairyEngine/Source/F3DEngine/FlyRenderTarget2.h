//------------------------------------------------------------------------
// Name: FlyRenderTarget.h
// Desc: This file define a render target to store the render result.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_RENDERTARGET_H__
#define __FLY_RENDERTARGET_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTexture.h"
#include "FlyCameraBase.h"

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlyRenderTarget
{
    typedef std::vector<FlyPlane> ClipPlaneList;

protected:
    FlyTexture* m_pTarget;      // Pointer to the Texture target.
    UINT m_nWidth;              // Width of the target.
    UINT m_nHeight;             // Height of the target.
    FlyCameraBase* m_pCamera;   // Pointer to the camera.
    bool m_bActive;             // Is the target active ?
    bool m_bShowSkybox;         // Show the skybox for the renderTarget.
    ClipPlaneList m_ClipPlanes; // Clip plane list.

public:
    // Constructor and destructor.
    FlyRenderTarget(void);
    ~FlyRenderTarget(void);

    virtual void OnUpdateBegin(void) { /* Nothing to do! */ };
    virtual void OnUpdateEnd(void) { /* Nothing to do! */ };

    void AddClipPlane( const FlyPlane& plane );
    void RemoveClipPlane( UINT nIndex );
    void RemoveAllClipPlanes(void);

    // Get the clip plane list.
    const std::vector<FlyPlane>& GetClipPlaneList(void) const { return m_ClipPlanes; }

    HRESULT BuildRenderTarget( const char* cName,UINT nWidth,UINT nHeight );
    void SetActive( bool bActive ) { m_bActive = bActive; }
    void SetCamera( FlyCameraBase* pCamera ) { m_pCamera = pCamera; }
    void ShowSkybox( bool bShowSkybox ) { m_bShowSkybox = bShowSkybox; }

    bool IsActive(void) const { return m_bActive; }
    FlyTexture* GetTextureTarget(void) { return m_pTarget; }
    FlyCameraBase* GetCamera(void) const { return m_pCamera; }
    UINT GetWidth(void) const { return m_nWidth; }
    UINT GetHeight(void) const { return m_nHeight; }
    bool IsShowSkybox(void) const { return m_bShowSkybox; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_RENDERTARGET_H__