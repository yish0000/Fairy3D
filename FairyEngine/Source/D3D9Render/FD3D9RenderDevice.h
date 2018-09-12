/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9RenderDevice.h
 *  Desc:   ���ļ�Ϊ����ʵ����һ������D3D9����Ⱦ�豸��
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


/** D3D9��Ⱦ�豸
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
    HINSTANCE               m_hModule;              // ģ���ʵ�����

    LPDIRECT3D9             m_pD3D;                 // Direct3D����ָ��
    LPDIRECT3DDEVICE9       m_pd3dDevice;           // Direct3D�豸ָ��
    D3DPRESENT_PARAMETERS   m_d3dpp;                // Direct3D�豸����

    bool                    m_bDeviceLost;          // Direct3D�豸�Ƿ�ʧ
    bool                    m_bRendering;           // ��ǰ�豸���ڻ���
    D3D9DevResourceList     m_DevResources;         // �豸��Դ���豸��ʧʱ������

    SDeviceSettings         m_Settings;             // ��Ⱦ�豸����

	DWORD m_RenderStateCache[MAXNUM_D3DRS+1];
	IDirect3DVertexShader9* m_pVSCache;
	IDirect3DPixelShader9*  m_pPSCache;

public:
    // ���캯������������
    FD3D9RenderDevice( HINSTANCE hModule );
    ~FD3D9RenderDevice(void);

    // ��ʼ����Ⱦ�豸
    void Initialize( const SWindowInfo& mainWnd,bool bShowDialog = true,
		const AKeyValueList* configData = NULL);

    // �ر���Ⱦ�豸
    void Shutdown(void);

    // ˢ����Ⱦ�豸����
    void RefreshDeviceSettings( const AKeyValueList& settings );

    // ��Ⱦ����
    void BeginRendering(void);
    void ClearBuffer( EFrameBufferType clearType,const F3DColor& color,float fZValue,ulong nStencilValue );
    void EndRendering(void);

    // ����һ����Ⱦ����
    F3DRenderWindow* CreateRenderWindow( const AString& name, const SWindowInfo& wndInfo );

    // ����һ��������ȾĿ��
    F3DRenderTarget* CreateTextureTarget( const AString& name );

    // ���õ�ǰ�Ļ�ӿ�
    void SetViewport( F3DViewport* pViewport );

    // �����豸�ı任����
    void SetWorldMatrix( const F3DMatrix4& worldMat );
    void SetWorldMatrix( const F3DMatrix4* pMatrices, size_t count );
    void SetViewMatrix( const F3DMatrix4& viewMat );
    void SetProjectionMatrix( const F3DMatrix4& projMat );

    // �����豸��һЩ��Ⱦģʽ
    void SetCullMode( ECullMode mode );
    void SetShadeMode( EShadeMode mode );
    void SetPolygonMode( EPolygonMode mode );

    // �����豸�����
    void SetTexture( size_t nStage,const F3DTexture* pTexture );
    void SetVertexTexture( uint32 nStage,const F3DTexture* pTexture );
    void SetTextureFilterMode( size_t nStage,EFilterOption magFO,EFilterOption minFO,EFilterOption mipFO );
    void SetTextureAddressingMode( size_t nStage,ETextureAccess addrU,ETextureAccess addrV,ETextureAccess addrW );
    void SetTextureBlendingMode( size_t nStage,const STextureBlendMode& mode );
    void SetTextureCoordIndex( size_t nStage,uint32 nCoordIndex );
    void SetTextureMatrix( size_t nStage,const F3DMatrix* pMatrix );

    // �����豸�İ�͸��ģʽ
    void SetAlphaMode( bool bAlphaBlend,bool bAlphaTest );
    void SetAlphaTestFunc( ECompareFunc cmpFunc,ulong alphaRef );
    void SetAlphaBlendMode( EAlphaBlendMode src,EAlphaBlendMode dest );

    // �����豸�Ĺ��ղ���
    void SetLightingMode( bool bLighting );
    void SetSpecularMode( bool bSpecular );
    void SetLight( size_t nStage,const SLightDesc* pLight );
    void SetMaterial( const SMaterialDesc& pMaterial );
    void SetAmbientLight( float fR,float fG,float fB );

    // �����豸����Ȼ���ģʽ
    void SetDepthBufferMode( EDepthMode mode );
    void SetDepthFunc( ECompareFunc cmpFunc,float fBias,float fSlopeScale );

    // �����豸��ģ�建��ģʽ
    void SetStencilMode( bool bFront,bool bBack );
    void SetStencilFunc( ECompareFunc frontFunc,ECompareFunc backFunc );
    void SetStencilOperationF( EStencilOperation failOp,EStencilOperation zFailOp,EStencilOperation passOp );
    void SetStencilOperationB( EStencilOperation failOp,EStencilOperation zFailOp,EStencilOperation passOp );
    void SetStencilRef( int nStencilRef );
    void SetStencilMask( ulong ulMask,ulong ulWriteMask );

    // ������Ⱦ�豸����ģʽ
    void SetFog( EFogMode mode,const F3DColor& clr,float fDensity,float fNear,float fFar );

    // ������Ⱦ�豸�Ĳü�ƽ��
    void SetClipPlanes( const F3DClipPlaneList& planeList );

    // ��ǰ�豸�Ƿ�ʧ
    bool IsDeviceLost(void) const { return m_bDeviceLost; }

    // ��ǰ�豸�Ƿ����ڻ��Ƴ���
    bool IsRendering(void) const { return m_bRendering; }

    // ���һ���豸��Դ
    void AddUnmanagedResource( FD3D9Resource* pResource );

    // ɾ��һ���豸��Դ
    void RemoveUnmanagedResource( FD3D9Resource* pResource );

    // ��ȡDirect3D������豸ָ��
    LPDIRECT3D9 _GetD3DPtr(void) const { return m_pD3D; }
    LPDIRECT3DDEVICE9 _GetD3DDevicePtr(void) const { return m_pd3dDevice; }

    // ��ȡD3D��Ⱦ�豸����
    const D3DPRESENT_PARAMETERS& _GetD3DParameters(void) const { return m_d3dpp; }

    // ��ȡ��Ⱦ�豸������
    const SDeviceSettings& _GetDeviceSettings(void) const { return m_Settings; }

    ///////////////////////////////////////////////////////////////////////

private:

    // �ͷ��豸ָ��
    void _FreeDevice(void);

	// �ָ���ʧ�˵���Ⱦ�豸
	void _RestoreLostDevice(void);

	// ��Ӧ�豸��ʧ
	void _OnDeviceLost();

	// ��Ӧ�豸�ָ�
	void _OnDeviceReset();

    // �����豸����Ⱦ״̬
    void _SetSamplerState( DWORD nSampler,D3DSAMPLERSTATETYPE Type,DWORD dwValue );
    void _SetTextureStageState( DWORD nStage,D3DTEXTURESTAGESTATETYPE Type,DWORD dwValue );
    void _SetRenderState( D3DRENDERSTATETYPE State,DWORD dwValue );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FD3D9_RENDERDEVICE_H__