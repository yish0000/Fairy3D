//------------------------------------------------------------------------
// Name: FlyRenderDevice.h
// Desc: This file define an interface for the engine's RenderDevice.
//       We can use D3D or GL to implement it.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_RENDER_DEVICE_H__
#define __FLY_RENDER_DEVICE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

#include "FlyRenderMaterial.h"
#include "FlyTexture.h"
#include "FlyShaderObject.h"
//////////////////////////////////////////////////////////////////////////


class FlyRenderDevice
{
protected:
    HINSTANCE   m_hDLL;             // HINSTANCE for the module.

    HWND        m_hWndMain;         // Handle to the main window.
    int         m_nNumWnds;         // Number of sub RenderWindows.
    int         m_nActiveStage;     // Current active stage.

public:
    // Constructor and destructor.
    FlyRenderDevice(void) { /* Nothing to do! */ };
    virtual ~FlyRenderDevice(void) { /* Nothing to do! */ };

    virtual HRESULT CreateDevice( HWND hWndMain,HWND* hWnds,int nNumWnds,bool bShowConfig ) = 0;
    virtual void    Release(void) = 0;
    virtual HRESULT BeginRendering(void) = 0;
    virtual HRESULT ClearBuffer( bool bC,bool bD,bool bS ) = 0;
    virtual HRESULT EndRendering(void) = 0;
    virtual HRESULT SwapBuffers(void) = 0;
    virtual HRESULT SetTextureRenderTarget( FlyTexture* pTexture ) = 0;
    virtual void    SetClearColor( float fR,float fG,float fB ) = 0;

    virtual HRESULT ResetDevice(void) = 0;
    virtual HRESULT RestoreDevice(void) = 0;
    virtual HRESULT ResizeMainWindow( int nWidth,int nHeight ) = 0;
    virtual HRESULT ResizeSubWindow( int nStage,int nWidth,int nHeight ) = 0;
    virtual HRESULT SetFullscreen( bool bFullscreen ) = 0;
    virtual HRESULT SaveScreenToFile( FLYIMAGETYPE Type,const char* cFilename ) = 0;

    // Functions to handle the three matrix,
    // world, view, projection.
    virtual HRESULT SetWorldMatrix( const FlyMatrix* mat ) = 0;
    virtual HRESULT SetView3D( const FlyVector& vRight,const FlyVector& vUp,
                               const FlyVector& vDir,const FlyVector& vPos ) = 0;
    virtual HRESULT SetViewLookAt( const FlyVector& vPos,const FlyVector& vAt,
                                   const FlyVector& vWorldUp ) = 0;
    virtual HRESULT SetViewPlanes( int nStage,float fNear,float fFar ) = 0;
    virtual HRESULT SetFOV( int nStage,float fFOV,float fAspect ) = 0;
    virtual HRESULT SetProjectionMatrix( const FlyMatrix& mProjection ) = 0;
    virtual HRESULT UseSceneStage( int nStage ) = 0;
    virtual HRESULT SetClipPlanes( const std::vector<FlyPlane>& planeList ) = 0;

    virtual const FlyShaderObject* GetShaderObject(void) const = 0;
    virtual void SetShaderObject( FlyShaderObject* pShader ) = 0;

    virtual HINSTANCE GetAppInstance(void) const = 0;
    virtual HWND GetMainWindow(void) const = 0;
    virtual HWND GetRenderWindow( int nIndex ) const = 0;
    virtual bool IsFullscreen(void) const = 0;
    virtual UINT GetBackWidth(void) const = 0;
    virtual UINT GetBackHeight(void) const = 0;
    virtual int  GetActiveStage(void) const = 0;
    virtual bool IsDeviceLost(void) const = 0;
    virtual bool IsRendering(void) const = 0;

    virtual const FlyRenderMaterial* GetActiveMaterial(void) const = 0;
    virtual void SetActiveMaterial( FlyRenderMaterial* pMaterial ) = 0;
    virtual int GetActiveFixedPass(void) const = 0;
    virtual void SetActiveFixedPass( int nPass ) = 0;
    virtual const FlyRenderPass* GetActivePass(void) const = 0;
    virtual void SetActivePass( FlyRenderPass* pPass ) = 0;

    // Get the device caps for the engine.
    virtual const DEVICECAPS* GetDeviceCaps(void) const = 0;

    virtual const FlyMatrix& GetWorldMatrix(void) const = 0;
    virtual const FlyMatrix& GetViewMatrix(void) const = 0;
    virtual const FlyMatrix& GetProjectionMatrix(void) const = 0;
    virtual const FlyVector& GetCameraPos(void) const = 0;

    virtual float GetPixelDepth( int px,int py ) const = 0;
    virtual void Transfer2DTo3D( const POINT& pt,FlyVector* pOrig,FlyVector* pDir ) = 0;
    virtual POINT Transfer3DTo2D( const FlyVector& vPosition ) = 0;

    // Function to set the RenderState for device.
    virtual void SetTexture( int nStage,const FlyTexture* pTexture ) = 0;
    virtual void SetTextureFilterMode( int nStage,FILTEROPTION magFO,FILTEROPTION minFO,FILTEROPTION mipFO ) = 0;
    virtual void SetTextureAddressingMode( int nStage,TEXTUREADDRESS addrU,TEXTUREADDRESS addrV,TEXTUREADDRESS addrW ) = 0;
    virtual void SetTextureBlendingMode( int nStage,const TEXTUREBLENDMODE& mode ) = 0;
    virtual void SetTextureCoordIndex( int nStage,int nCoordIndex ) = 0;
    virtual void SetTextureMatrix( int nStage,const FlyMatrix* mat ) = 0;

    virtual void SetSceneAlphaMode( bool bAlphaBlend,bool bAlphaTest ) = 0;
    virtual void SetAlphaTestFunc( FLYCMPFUNC cmpFunc,DWORD alphaRef ) = 0;
    virtual void SetAlphaBlendMode( SCENEBLENDMODE src,SCENEBLENDMODE dest ) = 0;

    virtual void SetLightingMode( bool bLighting ) = 0;
    virtual void SetSpecularMode( bool bSpecular ) = 0;
    virtual void SetLight( int nStage,const FLYLIGHT* pLight ) = 0;
    virtual void SetStdMaterial( const FLYMATERIAL* pMaterial ) = 0;
    virtual void SetDepthBufferMode( DEPTHMODE mode ) = 0;
    virtual void SetDepthFunc( FLYCMPFUNC cmpFunc,float fBias,float fSlopeScale ) = 0;
    virtual void SetStencilMode( bool bFront,bool bBack ) = 0;
    virtual void SetStencilFunc( FLYCMPFUNC frontFunc,FLYCMPFUNC backFunc ) = 0;
    virtual void SetStencilOperationF( STENCILOPERATION failOp,STENCILOPERATION zFailOp,STENCILOPERATION passOp ) = 0;
    virtual void SetStencilOperationB( STENCILOPERATION failOp,STENCILOPERATION zFailOp,STENCILOPERATION passOp ) = 0;
    virtual void SetStencilRef( int nStencilRef ) = 0;
    virtual void SetStencilMask( DWORD dwMask,DWORD dwWriteMask ) = 0;
    virtual void SetCullMode( CULLMODE mode ) = 0;
    virtual void SetPolygonMode( POLYGONMODE mode ) = 0;
    virtual void SetShadeMode( SHADEMODE mode ) = 0;
    virtual void SetAmbientLight( float fR,float fG,float fB ) = 0;
    virtual void SetFog( FOGMODE mode,const FLYCOLOR& clr,float fDensity,float fNear,float fFar ) = 0;

    // Function to get the render states.
    virtual const FlyTexture* GetTexture( int nStage ) const = 0;
    virtual void GetTextureFilterMode( int nStage,FILTEROPTION* pMagFO,FILTEROPTION* pMinFO,FILTEROPTION* pMipFO ) const = 0;
    virtual void GetTextureAddressingMode( int nStage,TEXTUREADDRESS* pAddrU,TEXTUREADDRESS* pAddrV,TEXTUREADDRESS* pAddrW ) const = 0;
    virtual void GetTextureBlendingMode( int nStage,BLENDTYPE Type,TEXTUREBLENDMODE* pMode ) const = 0;
    virtual int  GetTextureCoordIndex( int nStage ) const = 0;

    virtual void GetSceneAlphaMode( bool* pbAlphaBlend,bool* pbAlphaTest ) const = 0;
    virtual void GetAlphaTestFunc( FLYCMPFUNC* pCmpFunc,DWORD* pAlphaRef ) const = 0;
    virtual void GetAlphaBlendMode( SCENEBLENDMODE* pSrc,SCENEBLENDMODE* pDest ) const = 0;

    virtual bool GetLightingMode(void) const = 0;
    virtual bool GetSpecularMode(void) const = 0;
    virtual void GetLight( int nStage,FLYLIGHT* pLight ) const = 0;
    virtual void GetStdMaterial( FLYMATERIAL* pMaterial ) const = 0;
    virtual DEPTHMODE GetDepthBufferMode(void) const = 0;
    virtual void GetDepthFunc( FLYCMPFUNC* pCmpFunc,float* pfBias,float* pfSlopeScale ) const = 0;
    virtual void GetStencilMode( bool* pbStencilEnableF,bool* pbStencilEnableB ) const = 0;
    virtual void GetStencilFunc( FLYCMPFUNC* pFuncF,FLYCMPFUNC* pFuncB ) const = 0;
    virtual int  GetStencilRef(void) const = 0;
    virtual void GetStencilMask( DWORD* pMask,DWORD* pWriteMask ) const = 0;
    virtual void GetStencilOperationF( STENCILOPERATION* pFail,STENCILOPERATION* pZFail,STENCILOPERATION* pPass ) const = 0;
    virtual void GetStencilOperationB( STENCILOPERATION* pFail,STENCILOPERATION* pZFail,STENCILOPERATION* pPass ) const = 0;
    virtual CULLMODE GetCullMode(void) const = 0;
    virtual POLYGONMODE GetPolygonMode(void) const = 0;
    virtual SHADEMODE GetShadeMode(void) const = 0;
    virtual const FLYCOLOR& GetAmbientColor(void) const = 0;
    virtual void GetFog( FOGMODE* pMode,FLYCOLOR* pClr,float* pfDensity,float* pfNear,float* pfFar ) const = 0;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_RENDER_DEVICE_H__