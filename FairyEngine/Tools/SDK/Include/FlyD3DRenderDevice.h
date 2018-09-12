//------------------------------------------------------------------------
// Name: FlyD3DRenderDevice.h
// Desc: This file define a class to implement the interface RenderDevice
//       With the Direct3D.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_D3DRENDER_DEVICE_H__
#define __FLY_D3DRENDER_DEVICE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyRenderDevice.h"

#include "FlyD3DRenderWindow.h"
#include "FlyD3DConfigDialog.h"
//////////////////////////////////////////////////////////////////////////


class FlyD3DRenderDevice : public FlyRenderDevice
{
protected:
    LPDIRECT3D9             m_pD3D;                 // Pointer to the Direct3D object.
    LPDIRECT3DDEVICE9       m_pd3dDevice;           // Pointer to the Direct3D device.
    FlyD3DRenderWindow*     m_pWindows[MAX_3DHWND]; // Pointer to the sub windows.
    bool                    m_bDeviceLost;          // Does the D3D Device lost ?
    bool                    m_bRendering;           // Is the device rendering ?
    DEVICECAPS              m_DevCaps;              // To store the device caps.
    FlyRenderMaterial*      m_pActiveMaterial;      // Pointer to the active material.
    int                     m_nActiveFixedPass;     // Active pass of the fixed pipeline.
    FlyRenderPass*          m_pActivePass;          // Pointer to the active pass.
    FlyShaderObject*        m_pActiveShader;        // Pointer to the active shader.
    IDirect3DSurface9*      m_pMainTarget;          // Main render target.

    D3DPRESENT_PARAMETERS   m_d3dpp;                // Active Direct3D parameters.
    D3DCOLOR                m_ClearColor;           // Window clear color.

    FlyMatrix               m_mWorld;               // World matrix.
    FlyMatrix               m_mView3D;              // View matrix.
    FlyMatrix               m_mProjection;          // Projection matrix.
    FlyMatrix               m_mSubProj[MAX_3DHWND]; // Projection matrix for sub-Windows.
    FlyVector               m_vCameraPos;           // Position of the current camera.

    bool                    m_bStencilEnable;       // Do we enable the stencil buffer ?
    FLYCOLOR                m_cAmbient;             // Ambient color for the light effect.
    FLYLIGHT                m_Lights[MAX_LIGHTS];   // Store the light states.
    bool                    m_bLightEnable[MAX_LIGHTS]; // Store the light enable.
    FlySampler              m_Samplers[MAX_TEXTURES]; // Sampler states for the device.
    FlyRenderStateBuffer    m_RenderStates;         // Buffer to store render states.
    FOGMODE                 m_FogMode;              // Current fog mode.
    FLYCOLOR                m_cFogColor;            // Color of the fog.
    float                   m_fFogDensity;          // Density of the fog.
    float                   m_fFogNear;             // Near plane where the fog begin.
    float                   m_fFogFar;              // Far plane where the fog end.

    UINT                    m_nWidth;               // To store the fullscreen resolution.
    UINT                    m_nHeight;              // To store the fullscreen resolution.
    UINT                    m_nWndWidth;            // To store the wnd Mode resolution.
    UINT                    m_nWndHeight;           // To store the wnd Mode resolution.
    RECT                    m_rcWindow;             // To store the window rect.

    // Continue to start-up the Direct3D device.
    HRESULT Go( UINT nAdapterID,DEVICESETTINGS* pSettings,HWND* hWnds );
    HRESULT OneTimeInit(void);

    // When the Direct3D device is lost,
    // execute this function.
    void    OnDeviceLost(void);

    // Execute this function after the 
    // Direct3D device is reset.
    void    OnDeviceReset(void);

    // Calculate the project matrix.
    void    CalcPerspProjMatrix( float fFOV,float fAspect,float fN,float fF,FlyMatrix* pOut );

    // Write the Device caps.
    void    LogDeviceCaps( D3DCAPS9* pCaps );

    // Save the device caps.
    void    StoreDeviceCaps( D3DCAPS9* pCaps );

public:
    // Constructor and destructor.
    FlyD3DRenderDevice( HINSTANCE hModule );
    ~FlyD3DRenderDevice(void);

    // Override the interface's function.
    HRESULT CreateDevice( HWND hWndMain,HWND* hWnds,int nNumWnds,bool bShowConfig );
    void    Release(void);
    HRESULT BeginRendering(void);
    HRESULT ClearBuffer( bool bC,bool bD,bool bS );
    HRESULT EndRendering(void);
    HRESULT SwapBuffers(void);
    HRESULT SetTextureRenderTarget( FlyTexture* pTexture );
    void    SetClearColor( float fR,float fG,float fB );

    HRESULT RestoreDevice(void);
    HRESULT ResetDevice(void);
    HRESULT ResizeMainWindow( int nWidth,int nHeight );
    HRESULT ResizeSubWindow( int nStage,int nWidth,int nHeight );
    HRESULT SetFullscreen( bool bFullscreen );
    HRESULT SaveScreenToFile( FLYIMAGETYPE Type,const char* cFilename );

    HRESULT SetWorldMatrix( const FlyMatrix* mat );
    HRESULT SetView3D( const FlyVector& vRight,const FlyVector& vUp,const FlyVector& vDir,
                       const FlyVector& vPos );
    HRESULT SetViewLookAt( const FlyVector& vPos,const FlyVector& vAt,
                           const FlyVector& vWorldUp=FlyVector(0.0f,1.0f,0.0f) );
    HRESULT SetViewPlanes( int nStage,float fNear,float fFar );
    HRESULT SetFOV( int nStage,float fFOV,float fAspect );
    HRESULT SetProjectionMatrix( const FlyMatrix& mProjection );
    HRESULT UseSceneStage( int nStage );
    HRESULT SetClipPlanes( const std::vector<FlyPlane>& planeList );

    HINSTANCE GetAppInstance(void) const;
    HWND GetMainWindow(void) const;
    HWND GetRenderWindow( int nIndex ) const;
    bool IsFullscreen(void) const;
    UINT GetBackWidth(void) const;
    UINT GetBackHeight(void) const;
    int GetActiveStage(void) const;
    bool IsDeviceLost(void) const;
    bool IsRendering(void) const;

    const FlyRenderMaterial* GetActiveMaterial(void) const;
    void SetActiveMaterial( FlyRenderMaterial* pMaterial );
    int GetActiveFixedPass(void) const;
    void SetActiveFixedPass( int nPass );
    const FlyRenderPass* GetActivePass(void) const;
    void SetActivePass( FlyRenderPass* pPass );

    // Get the device caps for the engine.
    const DEVICECAPS* GetDeviceCaps(void) const;

    const FlyMatrix& GetWorldMatrix(void) const;
    const FlyMatrix& GetViewMatrix(void) const;
    const FlyMatrix& GetProjectionMatrix(void) const;
    const FlyMatrix& GetStageProjMatrix( int nStage ) const;
    const FlyVector& GetCameraPos(void) const;

    float GetPixelDepth( int px,int py ) const;
    void Transfer2DTo3D( const POINT& pt,FlyVector* pOrig,FlyVector* pDir );
    POINT Transfer3DTo2D( const FlyVector& vPosition );

    const FlyShaderObject* GetShaderObject(void) const;
    void SetShaderObject( FlyShaderObject* pShader );

    // Function to change the render state.
    void SetTexture( int nStage,const FlyTexture* pTexture );
    void SetTextureFilterMode( int nStage,FILTEROPTION magFO,FILTEROPTION minFO,FILTEROPTION mipFO );
    void SetTextureAddressingMode( int nStage,TEXTUREADDRESS addrU,TEXTUREADDRESS addrV,TEXTUREADDRESS addrW );
    void SetTextureBlendingMode( int nStage,const TEXTUREBLENDMODE& mode );
    void SetTextureCoordIndex( int nStage,int nCoordIndex );
    void SetTextureMatrix( int nStage,const FlyMatrix* mat );

    void SetSceneAlphaMode( bool bAlphaBlend,bool bAlphaTest );
    void SetAlphaTestFunc( FLYCMPFUNC cmpFunc,DWORD alphaRef );
    void SetAlphaBlendMode( SCENEBLENDMODE src,SCENEBLENDMODE dest );

    void SetLightingMode( bool bLighting );
    void SetSpecularMode( bool bSpecular );
    void SetLight( int nStage,const FLYLIGHT* pLight );
    void SetStdMaterial( const FLYMATERIAL* pMaterial );
    void SetDepthBufferMode( DEPTHMODE mode );
    void SetDepthFunc( FLYCMPFUNC cmpFunc,float fBias,float fSlopeScale );
    void SetStencilMode( bool bFront,bool bBack );
    void SetStencilFunc( FLYCMPFUNC frontFunc,FLYCMPFUNC backFunc );
    void SetStencilOperationF( STENCILOPERATION failOp,STENCILOPERATION zFailOp,STENCILOPERATION passOp );
    void SetStencilOperationB( STENCILOPERATION failOp,STENCILOPERATION zFailOp,STENCILOPERATION passOp );
    void SetStencilRef( int nStencilRef );
    void SetStencilMask( DWORD dwMask,DWORD dwWriteMask );
    void SetCullMode( CULLMODE mode );
    void SetShadeMode( SHADEMODE mode );
    void SetPolygonMode( POLYGONMODE mode );
    void SetAmbientLight( float fR,float fG,float fB );
    void SetFog( FOGMODE mode,const FLYCOLOR& clr,float fDensity,float fNear,float fFar );

    // Function to get the render states.
    const FlyTexture* GetTexture( int nStage ) const;
    void GetTextureFilterMode( int nStage,FILTEROPTION* pMagFO,FILTEROPTION* pMinFO,FILTEROPTION* pMipFO ) const;
    void GetTextureAddressingMode( int nStage,TEXTUREADDRESS* pAddrU,TEXTUREADDRESS* pAddrV,TEXTUREADDRESS* pAddrW ) const;
    void GetTextureBlendingMode( int nStage,BLENDTYPE Type,TEXTUREBLENDMODE* pMode ) const;
    int  GetTextureCoordIndex( int nStage ) const;

    void GetSceneAlphaMode( bool* pbAlphaBlend,bool* pbAlphaTest ) const;
    void GetAlphaTestFunc( FLYCMPFUNC* pCmpFunc,DWORD* pAlphaRef ) const;
    void GetAlphaBlendMode( SCENEBLENDMODE* pSrc,SCENEBLENDMODE* pDest ) const;

    bool GetLightingMode(void) const;
    bool GetSpecularMode(void) const;
    void GetLight( int nStage,FLYLIGHT* pLight ) const;
    void GetStdMaterial( FLYMATERIAL* pMaterial ) const;
    DEPTHMODE GetDepthBufferMode(void) const;
    void GetDepthFunc( FLYCMPFUNC* pCmpFunc,float* pfBias,float* pfSlopeScale ) const;
    void GetStencilMode( bool* pbStencilEnableF,bool* pbStencilEnableB ) const;
    void GetStencilFunc( FLYCMPFUNC* pFuncF,FLYCMPFUNC* pFuncB ) const;
    int GetStencilRef(void) const;
    void GetStencilMask( DWORD* pMask,DWORD* pWriteMask ) const;
    void GetStencilOperationF( STENCILOPERATION* pFail,STENCILOPERATION* pZFail,STENCILOPERATION* pPass ) const;
    void GetStencilOperationB( STENCILOPERATION* pFail,STENCILOPERATION* pZFail,STENCILOPERATION* pPass ) const;
    CULLMODE GetCullMode(void) const;
    SHADEMODE GetShadeMode(void) const;
    POLYGONMODE GetPolygonMode(void) const;
    const FLYCOLOR& GetAmbientColor(void) const;
    void GetFog( FOGMODE* pMode,FLYCOLOR* pClr,float* pfDensity,float* pfNear,float* pfFar ) const;

    LPDIRECT3D9 GetD3DPtr(void) { return m_pD3D; }
    LPDIRECT3DDEVICE9 GetD3DDevicePtr(void) { return m_pd3dDevice; }

    //////////////////////////////////////////////////////////////////////

    // Low level functions.
    HRESULT _SetSamplerState( DWORD nSampler,D3DSAMPLERSTATETYPE Type,DWORD dwValue );
    HRESULT _SetTextureStageState( DWORD nStage,D3DTEXTURESTAGESTATETYPE Type,DWORD dwValue );
    HRESULT _SetRenderState( D3DRENDERSTATETYPE State,DWORD dwValue );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_D3DRENDER_DEVICE_H__