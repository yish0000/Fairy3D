/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9RenderDevice.h
 *  Desc:   本文件为引擎实现了一个基于D3D9的渲染设备。
 *  Author: Yish
 *  Date:   2011/6/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FD3D9_RENDERDEVICE_H__
#define __FD3D9_RENDERDEVICE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FD3D9Common.h"
#include "FD3D9ConfigDialog.h"
#include "FD3D9Resource.h"

#include "F3DRenderDevice.h"

///////////////////////////////////////////////////////////////////////////


/** D3D9渲染设备
*/
class FD3D9RenderDevice : public F3DRenderDevice
{
public:     // Types

	enum
	{
		MAXNUM_D3DRS = D3DRS_BLENDOPALPHA + 1,
	};

    typedef std::list<FD3D9Resource*> D3D9DevResourceList;

protected:
    HINSTANCE               m_hModule;              // 模块的实例句柄

    LPDIRECT3D9             m_pD3D;                 // Direct3D对象指针
    LPDIRECT3DDEVICE9       m_pd3dDevice;           // Direct3D设备指针
    D3DPRESENT_PARAMETERS   m_d3dpp;                // Direct3D设备参数

    bool                    m_bDeviceLost;          // Direct3D设备是否丢失
    bool                    m_bRendering;           // 当前设备正在绘制
    D3D9DevResourceList     m_DevResources;         // 设备资源，设备丢失时需重置

    SDeviceSettings         m_Settings;             // 渲染设备配置

	DWORD m_RenderStateCache[MAXNUM_D3DRS+1];
	IDirect3DVertexShader9* m_pVSCache;
	IDirect3DPixelShader9*  m_pPSCache;

public:
    // 构造函数和析构函数
    FD3D9RenderDevice( HINSTANCE hModule );
    ~FD3D9RenderDevice(void);

    // 初始化渲染设备
    void Initialize( const SWindowInfo& mainWnd,bool bShowDialog = true,
		const AKeyValueList* configData = NULL);

    // 关闭渲染设备
    void Shutdown(void);

    // 刷新渲染设备配置
    void RefreshDeviceSettings( const AKeyValueList& settings );

    // 渲染管理
    void BeginRendering(void);
    void ClearBuffer( EFrameBufferType clearType,const F3DColor& color,float fZValue,ulong nStencilValue );
    void EndRendering(void);

    // 创建一个渲染窗口
    F3DRenderWindow* CreateRenderWindow( const AString& name, const SWindowInfo& wndInfo );

    // 创建一个纹理渲染目标
    F3DRenderTarget* CreateTextureTarget( const AString& name );

    // 设置当前的活动视口
    void SetViewport( F3DViewport* pViewport );

    // 设置设备的变换矩阵
    void SetWorldMatrix( const F3DMatrix4& worldMat );
    void SetWorldMatrix( const F3DMatrix4* pMatrices, size_t count );
    void SetViewMatrix( const F3DMatrix4& viewMat );
    void SetProjectionMatrix( const F3DMatrix4& projMat );

    // 设置设备的一些渲染模式
    void SetCullMode( ECullMode mode );
    void SetShadeMode( EShadeMode mode );
    void SetPolygonMode( EPolygonMode mode );

    // 操作设备纹理层
    void SetTexture( size_t nStage,const F3DTexture* pTexture );
    void SetVertexTexture( uint32 nStage,const F3DTexture* pTexture );
    void SetTextureFilterMode( size_t nStage,EFilterOption magFO,EFilterOption minFO,EFilterOption mipFO );
    void SetTextureAddressingMode( size_t nStage,ETextureAccess addrU,ETextureAccess addrV,ETextureAccess addrW );
    void SetTextureBlendingMode( size_t nStage,const STextureBlendMode& mode );
    void SetTextureCoordIndex( size_t nStage,uint32 nCoordIndex );
    void SetTextureMatrix( size_t nStage,const F3DMatrix* pMatrix );

    // 设置设备的半透明模式
    void SetAlphaMode( bool bAlphaBlend,bool bAlphaTest );
    void SetAlphaTestFunc( ECompareFunc cmpFunc,ulong alphaRef );
    void SetAlphaBlendMode( EAlphaBlendMode src,EAlphaBlendMode dest );

    // 设置设备的光照参数
    void SetLightingMode( bool bLighting );
    void SetSpecularMode( bool bSpecular );
    void SetLight( size_t nStage,const SLightDesc* pLight );
    void SetMaterial( const SMaterialDesc& pMaterial );
    void SetAmbientLight( float fR,float fG,float fB );

    // 设置设备的深度缓存模式
    void SetDepthBufferMode( EDepthMode mode );
    void SetDepthFunc( ECompareFunc cmpFunc,float fBias,float fSlopeScale );

    // 设置设备的模板缓存模式
    void SetStencilMode( bool bFront,bool bBack );
    void SetStencilFunc( ECompareFunc frontFunc,ECompareFunc backFunc );
    void SetStencilOperationF( EStencilOperation failOp,EStencilOperation zFailOp,EStencilOperation passOp );
    void SetStencilOperationB( EStencilOperation failOp,EStencilOperation zFailOp,EStencilOperation passOp );
    void SetStencilRef( int nStencilRef );
    void SetStencilMask( ulong ulMask,ulong ulWriteMask );

    // 设置渲染设备的雾化模式
    void SetFog( EFogMode mode,const F3DColor& clr,float fDensity,float fNear,float fFar );

    // 设置渲染设备的裁剪平面
    void SetClipPlanes( const F3DClipPlaneList& planeList );

    // 当前设备是否丢失
    bool IsDeviceLost(void) const { return m_bDeviceLost; }

    // 当前设备是否正在绘制场景
    bool IsRendering(void) const { return m_bRendering; }

    // 添加一个设备资源
    void AddUnmanagedResource( FD3D9Resource* pResource );

    // 删除一个设备资源
    void RemoveUnmanagedResource( FD3D9Resource* pResource );

    // 获取Direct3D对象和设备指针
    LPDIRECT3D9 _GetD3DPtr(void) const { return m_pD3D; }
    LPDIRECT3DDEVICE9 _GetD3DDevicePtr(void) const { return m_pd3dDevice; }

    // 获取D3D渲染设备参数
    const D3DPRESENT_PARAMETERS& _GetD3DParameters(void) const { return m_d3dpp; }

    // 获取渲染设备的配置
    const SDeviceSettings& _GetDeviceSettings(void) const { return m_Settings; }

    ///////////////////////////////////////////////////////////////////////

private:

    // 释放设备指针
    void _FreeDevice(void);

	// 恢复丢失了的渲染设备
	void _RestoreLostDevice(void);

	// 响应设备丢失
	void _OnDeviceLost();

	// 响应设备恢复
	void _OnDeviceReset();

    // 设置设备的渲染状态
    void _SetSamplerState( DWORD nSampler,D3DSAMPLERSTATETYPE Type,DWORD dwValue );
    void _SetTextureStageState( DWORD nStage,D3DTEXTURESTAGESTATETYPE Type,DWORD dwValue );
    void _SetRenderState( D3DRENDERSTATETYPE State,DWORD dwValue );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FD3D9_RENDERDEVICE_H__