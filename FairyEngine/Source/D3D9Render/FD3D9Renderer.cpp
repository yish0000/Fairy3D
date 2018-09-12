//------------------------------------------------------------------------
// Name: FlyD3DRenderer.cpp
// Desc: This file is to implementing the class FlyD3DRenderer.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------

/*
#include "FlyD3DRenderer.h"
#include "FlyD3DGraphicBuffer.h"

#include "FlyKernel.h"
#include "FlyLogManager.h"


//------------------------------------------------------------------------
// Constructor of the class
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyD3DRenderDevice*:    Pointer to the Direct3D render device.
//------------------------------------------------------------------------
FlyD3DRenderer::FlyD3DRenderer( FlyD3DRenderDevice* pRDevice )
{
    m_pRDevice = pRDevice;
    m_pd3dDevice = pRDevice->GetD3DDevicePtr();

    m_pMaterial = NULL;
    m_RenderType = RT_TRIANGLELIST;
    m_pDefaultMaterial = NULL;
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
FlyD3DRenderer::~FlyD3DRenderer(void)
{
    Release();

    FlyLogManager::GetSingleton().WriteLine( 0,"Renderer for engine has been released!" );
}


//------------------------------------------------------------------------
// Initialize the renderer for the engine. User can't run this function.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderer::InitRenderer(void)
{
    HRESULT hr;
    D3DVertexFormat format;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    // 1. Init the default vertex format.
    format.dwFVF = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1;
    format.nStrideSize = sizeof(VERTEX);
    hr = BuildVertexDeclaration( FlyVertexType::MakeVertexType(VF_DEFAULT),&format.pD3DDecl );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Create the Vertex declaration failed! (Default)" );
        return hr;
    }
    m_VFormats.push_back( format );

    // 2. Init the vertex format without light.
    format.dwFVF = D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1;
    format.nStrideSize = sizeof(VERTEXL);
    hr = BuildVertexDeclaration( FlyVertexType::MakeVertexType(VF_DEFAULTNOLIGHT),&format.pD3DDecl );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Create the vertex declaration failed! (DefaultNoLight)" );
        return hr;
    }
    m_VFormats.push_back( format );

    // 3. Init the vertex format with two textures.
    format.dwFVF = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2;
    format.nStrideSize = sizeof(VERTEX2T);
    hr = BuildVertexDeclaration( FlyVertexType::MakeVertexType(VF_TWOTEXTURE),&format.pD3DDecl );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Create the vertex declaration failed! (TwoTexture)" );
        return hr;
    }
    m_VFormats.push_back( format );

    // 4. Init the vertex format used for animations.
    format.dwFVF = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1;
    format.nStrideSize = sizeof(AVERTEX);
    hr = BuildVertexDeclaration( FlyVertexType::MakeVertexType(VF_ANIMATION),&format.pD3DDecl );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Create the vertex declaration failed! (Animation)" );
        return hr;
    }
    m_VFormats.push_back( format );

    // 5. Init the vertex format (Only Position).
    format.dwFVF = D3DFVF_XYZ;
    format.nStrideSize = sizeof(float)*3;
    hr = BuildVertexDeclaration( FlyVertexType::MakeVertexType(VF_POSITION),&format.pD3DDecl );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Create the vertex declaration failed! (Only position)" );
        return hr;
    }
    m_VFormats.push_back( format );

    // 6. Init the vertex format (Only used for stencil shadow)
    format.dwFVF = D3DFVF_XYZRHW|D3DFVF_DIFFUSE;
    format.nStrideSize = 20;
    hr = BuildVertexDeclaration( FlyVertexType::MakeVertexType(VF_STENCILSHADOW),&format.pD3DDecl );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Create the vertex declaration failed! (Stencil shadow)" );
        return hr;
    }
    m_VFormats.push_back( format );

    // Create the default material for renderer.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    m_pDefaultMaterial = pResMgr->CreateMaterial( "default" );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Release the renderer for the renderDevice.
//------------------------------------------------------------------------
void FlyD3DRenderer::Release(void)
{
    for( size_t n=0;n<m_VFormats.size();n++ )
        SAFE_RELEASE( m_VFormats[n].pD3DDecl );

    // Clean the static buffer.
    for( size_t n=0;n<m_StaticBuffers.size();n++ )
    {
        SAFE_DELETE( m_StaticBuffers[n].pVB );
        SAFE_DELETE( m_StaticBuffers[n].pIB );
    }
}


//------------------------------------------------------------------------
// Register a user-defined vertex format.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVertexType&:   The user-defined vertex type.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderer::RegisterVertexType( const FlyVertexType& Type )
{
    HRESULT hr;
    D3DVertexFormat format;

    // Get the log manager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    hr = BuildVertexDeclaration( Type,&format.pD3DDecl );
    if( FAILED(hr) ) return hr;

    format.dwFVF = BuildVertexFVF( Type );
    format.nStrideSize = (UINT)Type.GetStrideSize();
    m_VFormats.push_back( format );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Build the vertex declaration for using when use shader.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVertexType&: The user-defined vertex type.
//      IDirect3DVertexDeclaration**: To store the declaration pointer.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderer::BuildVertexDeclaration( const FlyVertexType& vertexType,IDirect3DVertexDeclaration9** ppDecl )
{
    WORD wOffset = 0;
    D3DVERTEXELEMENT9 desc[33];
    HRESULT hr;

    if( vertexType.GetNumElements() == 0 || vertexType.GetNumElements() > 32 )
        return FLY_INVALIDPARAM;

    UINT i = 0;
    for( ;i<vertexType.GetNumElements();i++ )
    {
        desc[i].Stream = 0;
        desc[i].Offset = wOffset;
        desc[i].Method = D3DDECLMETHOD_DEFAULT;
        desc[i].UsageIndex = vertexType.GetUsageIndex( i );

        switch( vertexType.GetElementType(i) )
        {
        case ET_FLOAT1:
            desc[i].Type = D3DDECLTYPE_FLOAT1;
            wOffset += 4;
            break;
        case ET_FLOAT2:
            desc[i].Type = D3DDECLTYPE_FLOAT2;
            wOffset += 8;
            break;
        case ET_FLOAT3:
            desc[i].Type = D3DDECLTYPE_FLOAT3;
            wOffset += 12;
            break;
        case ET_FLOAT4:
            desc[i].Type = D3DDECLTYPE_FLOAT4;
            wOffset += 16;
            break;
        case ET_COLOR:
            desc[i].Type = D3DDECLTYPE_D3DCOLOR;
            wOffset += 4;
            break;
        case ET_UBYTE4:
            desc[i].Type = D3DDECLTYPE_UBYTE4;
            wOffset += 4;
            break;
        }

        switch( vertexType.GetElementUsage(i) )
        {
        case EU_POSITION:
            desc[i].Usage = D3DDECLUSAGE_POSITION;
            break;
        case EU_NORMAL:
            desc[i].Usage = D3DDECLUSAGE_NORMAL;
            break;
        case EU_DIFFUSE:
        case EU_SPECULAR:
            desc[i].Usage = D3DDECLUSAGE_COLOR;
            break;
        case EU_BLENDWEIGHT:
            desc[i].Usage = D3DDECLUSAGE_BLENDWEIGHT;
            break;
        case EU_BLENDINDEX:
            desc[i].Usage = D3DDECLUSAGE_BLENDINDICES;
            break;
        case EU_POSITIONT:
            desc[i].Usage = D3DDECLUSAGE_POSITIONT;
            break;
        case EU_TEXCOORD:
            desc[i].Usage = D3DDECLUSAGE_TEXCOORD;
            break;
        case EU_TAGENT:
            desc[i].Usage = D3DDECLUSAGE_TANGENT;
            break;
        case EU_BINORMAL:
            desc[i].Usage = D3DDECLUSAGE_BINORMAL;
            break;
        }
    }

    // Write the declaration flag.
    desc[i].Stream = 0xff;
    desc[i].Offset = 0;
    desc[i].Type = D3DDECLTYPE_UNUSED;
    desc[i].Method = D3DDECLMETHOD_DEFAULT;
    desc[i].Usage = 0;
    desc[i].UsageIndex = 0;

    // Build the vertex declaration.
    hr = m_pd3dDevice->CreateVertexDeclaration( desc,ppDecl );
    if( FAILED(hr) ) return hr;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Build the vertex FVF for the renderer, If the vertex type can only
// use the programed pipeline, return the -1.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyVertexType:  The user defined vertex type.
//------------------------------------------------------------------------
DWORD FlyD3DRenderer::BuildVertexFVF( const FlyVertexType& vertexType )
{
    int nNumTex = 0;
    DWORD dwFVF = 0;
    bool bBlendIndices = false;
    bool bBlendWeight = false;
    ELEMENTTYPE weightType;

    if( vertexType.GetNumElements() == 0 || vertexType.GetNumElements() > 32 )
        return 0;

    UINT i = 0;
    for( ;i<vertexType.GetNumElements();i++ )
    {
        if( vertexType.GetElementUsage(i) == EU_BLENDWEIGHT )
        {
            weightType = vertexType.GetElementType( i );
            bBlendWeight = true;
        }

        if( vertexType.GetElementUsage(i) == EU_BLENDINDEX )
        {
            if( vertexType.GetElementType(i) != ET_UBYTE4 )
                return -1;
            bBlendIndices = true;
        }
    }

    for( i=0;i<vertexType.GetNumElements();i++ )
    {
        switch( vertexType.GetElementUsage(i) )
        {
        case EU_POSITION:
            if( vertexType.GetElementType(i) != ET_FLOAT3 )
                return -1;
            dwFVF |= D3DFVF_XYZ;
            break;
        case EU_NORMAL:
            if( vertexType.GetElementType(i) != ET_FLOAT3 )
                return -1;
            dwFVF |= D3DFVF_NORMAL;
            break;
        case EU_DIFFUSE:
            if( vertexType.GetElementType(i) != ET_COLOR )
                return -1;
            dwFVF |= D3DFVF_DIFFUSE;
            break;
        case EU_SPECULAR:
            if( vertexType.GetElementType(i) != ET_COLOR )
                return -1;
            dwFVF |= D3DFVF_SPECULAR;
            break;
        case EU_POSITIONT:
            if( vertexType.GetElementType(i) != ET_FLOAT4 )
                return -1;
            dwFVF |= D3DFVF_XYZRHW;
            break;
        case EU_TAGENT:
        case EU_BINORMAL:
            return -1;
            break;
        case EU_TEXCOORD:
            switch( vertexType.GetElementType(i) )
            {
            case ET_FLOAT2:
                dwFVF |= D3DFVF_TEXCOORDSIZE2( vertexType.GetUsageIndex(i) );
                break;
            case ET_FLOAT3:
                dwFVF |= D3DFVF_TEXCOORDSIZE3( vertexType.GetUsageIndex(i) );
                break;
            default: return -1;
            }
            nNumTex++;
            break;
        }
    }

    if( bBlendWeight )
    {
        switch( weightType )
        {
        case ET_FLOAT1:
            dwFVF &= ~D3DFVF_XYZ;

            if( !bBlendIndices )
                dwFVF |= D3DFVF_XYZB1;
            else
            {
                dwFVF |= D3DFVF_XYZB2;
                dwFVF |= D3DFVF_LASTBETA_UBYTE4;
            }
            break;
        case ET_FLOAT2:
            dwFVF &= ~D3DFVF_XYZ;

            if( !bBlendIndices )
                dwFVF |= D3DFVF_XYZB2;
            else
            {
                dwFVF |= D3DFVF_XYZB3;
                dwFVF |= D3DFVF_LASTBETA_UBYTE4;
            }
            break;
        case ET_FLOAT3:
            dwFVF &= ~D3DFVF_XYZ;

            if( !bBlendIndices )
                dwFVF |= D3DFVF_XYZB3;
            else
            {
                dwFVF |= D3DFVF_XYZB4;
                dwFVF |= D3DFVF_LASTBETA_UBYTE4;
            }
            break;
        default: return -1;
        }
    }

    switch( nNumTex )
    {
    case 1: dwFVF |= D3DFVF_TEX1; break;
    case 2: dwFVF |= D3DFVF_TEX2; break;
    case 3: dwFVF |= D3DFVF_TEX3; break;
    case 4: dwFVF |= D3DFVF_TEX4; break;
    case 5: dwFVF |= D3DFVF_TEX5; break;
    case 6: dwFVF |= D3DFVF_TEX6; break;
    case 7: dwFVF |= D3DFVF_TEX7; break;
    case 8: dwFVF |= D3DFVF_TEX8; break;
    default: break;
    }

    return dwFVF;
}


//------------------------------------------------------------------------
// Add a renderable object to the renderer.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyRenderable*:     Pointer to the renderable object.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderer::AddRenderable( FlyRenderable* pObject )
{
    // Get the pointer to the material.
    FlyRenderMaterial* pMaterial = pObject->GetRenderMaterial();

    if( pMaterial && pMaterial->IsTransparent() )
        m_AlphaQueue.push_back( pObject );
    else
        m_DefaultQueue.push_back( pObject );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Add a screen surface object to the queue. (Such as font, UI elements
// and so on.)
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyRenderable*:     Pointer to the renderable object.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderer::AddLayoutObject( FlyRenderable* pObject )
{
    m_LayoutQueue.push_back( pObject );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Add a static geometry data buffer to render.
// ----------------------------------------------------------------------
// Param -> IN:
//      VERTEXTYPE:     Which type the vertex is ?
//      INDEXFORMAT:    Index format of the static buffer.
//      UINT:           Number of the vertices.
//      UINT:           Number of the indices.
//      const void*:    Pointer to the Vertices data.
//      const void*:    Pointer to the indicies data.
//      FlyRenderMaterial*: Pointer to the render material.
//      RENDERTYPE:     Render type of the buffer.
//      UINT*:          To store the static buffer ID.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderer::AddStaticBuffer( VERTEXTYPE nVType,INDEXFORMAT indexFmt,UINT nNumVerts,
                                         UINT nNumIndis,const void* pVerts,const void* pIndis,
                                         FlyRenderMaterial* pMaterial,RENDERTYPE RType,
                                         UINT* pnID )
{
    UINT nSize;
    D3DStaticBuffer buffer;

    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    if( nVType >= m_VFormats.size() )
    {
        logMgr.WriteError( 0,"Unknown vertex type, AddStaticBuffer() failed!" );
        return FLY_UNKNOWNFORMAT;
    }

    buffer.dwFVF = m_VFormats[nVType].dwFVF;
    buffer.nStrideSize = m_VFormats[nVType].nStrideSize;
    buffer.pD3DDecl = m_VFormats[nVType].pD3DDecl;
    buffer.pMaterial = pMaterial;
    buffer.nNumVerts = nNumVerts;
    buffer.nNumIndis = nNumIndis;
    buffer.RenderType = RType;

    // Create the vertex buffer.
    nSize = buffer.nStrideSize * buffer.nNumVerts;
    buffer.pVB = pResMgr->MakeVertexBuffer( nSize,BU_WRITEONLY,MM_MANAGED );
    if( !buffer.pVB )
    {
        logMgr.WriteError( 0,"Create the static vertex buffer failed!" );
        return FLY_CREATEBUFFER;
    }

    // Write the vertex data.
    buffer.pVB->WriteData( 0,nSize,pVerts,false );

    // Create the index buffer.
    if( nNumIndis == 0 )
    {
        buffer.pIB = NULL;
    }
    else
    {
        if( indexFmt == IF_INDEX16 )
            nSize = 2 * buffer.nNumIndis;
        else
            nSize = 4 * buffer.nNumIndis;

        buffer.pIB = pResMgr->MakeIndexBuffer( nSize,BU_WRITEONLY,MM_MANAGED,indexFmt );
        if( !buffer.pIB )
        {
            logMgr.WriteError( 0,"Create the static index buffer failed!" );
            return FLY_CREATEBUFFER;
        }

        // Write the index data.
        buffer.pIB->WriteData( 0,nSize,pIndis,false );
    }

    // Store the static buffer.
    *pnID = (UINT)m_StaticBuffers.size();
    m_StaticBuffers.push_back( buffer );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Clear the static buffer for the renderer.
//------------------------------------------------------------------------
void FlyD3DRenderer::ClearStaticBuffer(void)
{
    for( size_t i=0;i<m_StaticBuffers.size();i++ )
    {
        SAFE_DELETE( m_StaticBuffers[i].pVB );
        SAFE_DELETE( m_StaticBuffers[i].pIB );
    }

    m_StaticBuffers.clear();
}


//------------------------------------------------------------------------
// Render the static buffer data.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Index of the static buffer.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderer::RenderStaticBuffer( UINT nBufferIndex )
{
    D3DStaticBuffer* pBuffer;
    FlyRenderMaterial* pMaterial;

    if( nBufferIndex >= m_StaticBuffers.size() )
        return FLY_INVALIDPARAM;

    // Get the static buffer.
    pBuffer = &m_StaticBuffers[nBufferIndex];

    if( pBuffer->pMaterial )
        pMaterial = pBuffer->pMaterial;
    else
        pMaterial = m_pDefaultMaterial;

    // Set the Vertex format.
    if( pMaterial->IsFixedPipeline() )
    {
        m_pd3dDevice->SetFVF( m_StaticBuffers[nBufferIndex].dwFVF );
    }
    else
    {
        m_pd3dDevice->SetFVF( 0 );
        m_pd3dDevice->SetVertexDeclaration( m_StaticBuffers[nBufferIndex].pD3DDecl );
    }

    // Setup the vertex buffer.
    m_pd3dDevice->SetStreamSource( 0,((FlyD3DVertexBuffer*)pBuffer->pVB)->GetD3DVBPtr(),0,pBuffer->nStrideSize );
    if( pBuffer->nNumIndis > 0 ) m_pd3dDevice->SetIndices( ((FlyD3DIndexBuffer*)pBuffer->pIB)->GetD3DIBPtr() );

    // Setup the Render Material.
    if( pMaterial->IsFixedPipeline() )
    {
        for( UINT i=0;i<pMaterial->GetNumPasses();i++ )
        {
            pMaterial->ApplyPass( i );

            if( pBuffer->nNumIndis > 0 )
            {
                switch( pBuffer->RenderType )
                {
                case RT_POINTLIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_POINTLIST,0,0,pBuffer->nNumVerts,
                        0,pBuffer->nNumIndis );
                    break;
                case RT_LINELIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_LINELIST,0,0,pBuffer->nNumVerts,
                        0,pBuffer->nNumIndis/2 );
                    break;
                case RT_LINESTRIP:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_LINESTRIP,0,0,pBuffer->nNumVerts,
                        0,pBuffer->nNumIndis-1 );
                    break;
                case RT_TRIANGLELIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,pBuffer->nNumVerts,
                        0,pBuffer->nNumIndis/3 );
                    break;
                case RT_TRIANGLESTRIP:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP,0,0,pBuffer->nNumVerts,
                        0,pBuffer->nNumIndis-2 );
                    break;
                case RT_TRIANGLEFAN:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLEFAN,0,0,pBuffer->nNumVerts,
                        0,pBuffer->nNumIndis-2 );
                    break;
                }
            }
            else
            {
                switch( pBuffer->RenderType )
                {
                case RT_POINTLIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_POINTLIST,0,pBuffer->nNumVerts );
                    break;
                case RT_LINELIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST,0,pBuffer->nNumVerts/2 );
                    break;
                case RT_LINESTRIP:
                    m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP,0,pBuffer->nNumVerts-1 );
                    break;
                case RT_TRIANGLELIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0,pBuffer->nNumVerts/3 );
                    break;
                case RT_TRIANGLESTRIP:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,0,pBuffer->nNumVerts-2 );
                    break;
                case RT_TRIANGLEFAN:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN,0,pBuffer->nNumVerts-2 );
                    break;
                }
            }
        }
    }
    else
    {
        FlyRenderTechnique* pTechnique = pMaterial->GetActiveTechnique();

        // Update parameters for the GPU program.
        pMaterial->GetActiveRenderEffectInstance()->UpdateParameters( &m_AutoParameters );

        for( UINT p=0;p<pTechnique->GetNumPasses();p++ )
        {
            pTechnique->ApplyPass( p );

            if( pBuffer->nNumIndis > 0 )
            {
                switch( pBuffer->RenderType )
                {
                case RT_POINTLIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_POINTLIST,0,0,pBuffer->nNumVerts,
                        0,pBuffer->nNumIndis );
                    break;
                case RT_LINELIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_LINELIST,0,0,pBuffer->nNumVerts,
                        0,pBuffer->nNumIndis/2 );
                    break;
                case RT_LINESTRIP:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_LINESTRIP,0,0,pBuffer->nNumVerts,
                        0,pBuffer->nNumIndis-1 );
                    break;
                case RT_TRIANGLELIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,pBuffer->nNumVerts,
                        0,pBuffer->nNumIndis/3 );
                    break;
                case RT_TRIANGLESTRIP:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP,0,0,pBuffer->nNumVerts,
                        0,pBuffer->nNumIndis-2 );
                    break;
                case RT_TRIANGLEFAN:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLEFAN,0,0,pBuffer->nNumVerts,
                        0,pBuffer->nNumIndis-2 );
                    break;
                }
            }
            else
            {
                switch( pBuffer->RenderType )
                {
                case RT_POINTLIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_POINTLIST,0,pBuffer->nNumVerts );
                    break;
                case RT_LINELIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST,0,pBuffer->nNumVerts/2 );
                    break;
                case RT_LINESTRIP:
                    m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP,0,pBuffer->nNumVerts-1 );
                    break;
                case RT_TRIANGLELIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0,pBuffer->nNumVerts/3 );
                    break;
                case RT_TRIANGLESTRIP:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,0,pBuffer->nNumVerts-2 );
                    break;
                case RT_TRIANGLEFAN:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN,0,pBuffer->nNumVerts-2 );
                    break;
                }
            }
        }
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Render the geometry buffer directly.
// ----------------------------------------------------------------------
// Param -> IN:
//      VERTEXTYPE:         Vertex type to render.
//      UINT:               Number of the vertices.
//      UINT:               Number of the indices.
//      FlyGraphicBuffer*:  Pointer to the vertex buffer.
//      FlyGraphicBuffer*:  Pointer to the index buffer.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderer::RenderBuffer( VERTEXTYPE nVType,UINT nNumVerts,UINT nNumIndis,
                                      FlyGraphicBuffer* pVB,FlyGraphicBuffer* pIB )
{
    FlyRenderMaterial* pMaterial;

    if( nVType >= m_VFormats.size() || nNumVerts == 0 )
        return FLY_INVALIDPARAM;

    if( m_pMaterial )
        pMaterial = m_pMaterial;
    else
        pMaterial = m_pDefaultMaterial;

    // Set the Vertex format.
    if( pMaterial->IsFixedPipeline() )
    {
        m_pd3dDevice->SetFVF( m_VFormats[nVType].dwFVF );
    }
    else
    {
        m_pd3dDevice->SetFVF( 0 );
        m_pd3dDevice->SetVertexDeclaration( m_VFormats[nVType].pD3DDecl );
    }

    // Setup the vertex buffer.
    m_pd3dDevice->SetStreamSource( 0,((FlyD3DVertexBuffer*)pVB)->GetD3DVBPtr(),0,m_VFormats[nVType].nStrideSize );
    if( nNumIndis > 0 ) m_pd3dDevice->SetIndices( ((FlyD3DIndexBuffer*)pIB)->GetD3DIBPtr() );

    // Setup the Render Material.
    if( pMaterial->IsFixedPipeline() )
    {
        for( UINT i=0;i<pMaterial->GetNumPasses();i++ )
        {
            pMaterial->ApplyPass( i );

            if( nNumIndis > 0 )
            {
                switch( m_RenderType )
                {
                case RT_POINTLIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_POINTLIST,0,0,nNumVerts,0,nNumIndis );
                    break;
                case RT_LINELIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_LINELIST,0,0,nNumVerts,0,nNumIndis/2 );
                    break;
                case RT_LINESTRIP:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_LINESTRIP,0,0,nNumVerts,0,nNumIndis-1 );
                    break;
                case RT_TRIANGLELIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,nNumVerts,0,nNumIndis/3 );
                    break;
                case RT_TRIANGLESTRIP:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP,0,0,nNumVerts,0,nNumIndis-2 );
                    break;
                case RT_TRIANGLEFAN:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLEFAN,0,0,nNumVerts,0,nNumIndis-2 );
                    break;
                }
            }
            else
            {
                switch( m_RenderType )
                {
                case RT_POINTLIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_POINTLIST,0,nNumVerts );
                    break;
                case RT_LINELIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST,0,nNumVerts/2 );
                    break;
                case RT_LINESTRIP:
                    m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP,0,nNumVerts-1 );
                    break;
                case RT_TRIANGLELIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0,nNumVerts/3 );
                    break;
                case RT_TRIANGLESTRIP:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,0,nNumVerts-2 );
                    break;
                case RT_TRIANGLEFAN:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN,0,nNumVerts-2 );
                    break;
                }
            }
        }
    }
    else
    {
        FlyRenderTechnique* pTechnique = pMaterial->GetActiveTechnique();

        // Update parameters for the GPU program.
        pMaterial->GetActiveRenderEffectInstance()->UpdateParameters( &m_AutoParameters );

        for( UINT p=0;p<pTechnique->GetNumPasses();p++ )
        {
            pTechnique->ApplyPass( p );

            if( nNumIndis > 0 )
            {
                switch( m_RenderType )
                {
                case RT_POINTLIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_POINTLIST,0,0,nNumVerts,0,nNumIndis );
                    break;
                case RT_LINELIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_LINELIST,0,0,nNumVerts,0,nNumIndis/2 );
                    break;
                case RT_LINESTRIP:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_LINESTRIP,0,0,nNumVerts,0,nNumIndis-1 );
                    break;
                case RT_TRIANGLELIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,nNumVerts,0,nNumIndis/3 );
                    break;
                case RT_TRIANGLESTRIP:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP,0,0,nNumVerts,0,nNumIndis-2 );
                    break;
                case RT_TRIANGLEFAN:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLEFAN,0,0,nNumVerts,0,nNumIndis-2 );
                    break;
                }
            }
            else
            {
                switch( m_RenderType )
                {
                case RT_POINTLIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_POINTLIST,0,nNumVerts );
                    break;
                case RT_LINELIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST,0,nNumVerts/2 );
                    break;
                case RT_LINESTRIP:
                    m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP,0,nNumVerts-1 );
                    break;
                case RT_TRIANGLELIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0,nNumVerts/3 );
                    break;
                case RT_TRIANGLESTRIP:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,0,nNumVerts-2 );
                    break;
                case RT_TRIANGLEFAN:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN,0,nNumVerts-2 );
                    break;
                }
            }
        }
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Render the geometry buffer directly.
// ----------------------------------------------------------------------
// Param -> IN:
//      VERTEXTYPE:         Vertex type of the buffer.
//      UINT:               Offset of the buffer.
//      UINT:               Count of the vertices.
//      FlyGraphicBuffer*:  Pointer to the vertex buffer.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderer::RenderBuffer( VERTEXTYPE nVType,UINT nOffset,UINT nNumVerts,
                                      FlyGraphicBuffer* pVB )
{
    FlyRenderMaterial* pMaterial;

    if( nVType >= m_VFormats.size() || nNumVerts == 0 )
        return FLY_INVALIDPARAM;

    if( m_pMaterial )
        pMaterial = m_pMaterial;
    else
        pMaterial = m_pDefaultMaterial;

    // Set the Vertex format.
    if( pMaterial->IsFixedPipeline() )
    {
        m_pd3dDevice->SetFVF( m_VFormats[nVType].dwFVF );
    }
    else
    {
        m_pd3dDevice->SetFVF( 0 );
        m_pd3dDevice->SetVertexDeclaration( m_VFormats[nVType].pD3DDecl );
    }

    // Setup the vertex buffer.
    m_pd3dDevice->SetStreamSource( 0,((FlyD3DVertexBuffer*)pVB)->GetD3DVBPtr(),0,
        m_VFormats[nVType].nStrideSize );
    m_pd3dDevice->SetIndices( NULL );

    // Setup the Render Material.
    if( pMaterial->IsFixedPipeline() )
    {
        for( UINT i=0;i<pMaterial->GetNumPasses();i++ )
        {
            pMaterial->ApplyPass( i );

            switch( m_RenderType )
            {
            case RT_POINTLIST:
                m_pd3dDevice->DrawPrimitive( D3DPT_POINTLIST,nOffset,nNumVerts );
                break;
            case RT_LINELIST:
                m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST,nOffset,nNumVerts/2 );
                break;
            case RT_LINESTRIP:
                m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP,nOffset,nNumVerts-1 );
                break;
            case RT_TRIANGLELIST:
                m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,nOffset,nNumVerts/3 );
                break;
            case RT_TRIANGLESTRIP:
                m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,nOffset,nNumVerts-2 );
                break;
            case RT_TRIANGLEFAN:
                m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN,nOffset,nNumVerts-2 );
                break;
            }
        }
    }
    else
    {
        FlyRenderTechnique* pTechnique = pMaterial->GetActiveTechnique();

        // Update parameters for the GPU program.
        pMaterial->GetActiveRenderEffectInstance()->UpdateParameters( &m_AutoParameters );

        for( UINT p=0;p<pTechnique->GetNumPasses();p++ )
        {
            pTechnique->ApplyPass( p );

            switch( m_RenderType )
            {
            case RT_POINTLIST:
                m_pd3dDevice->DrawPrimitive( D3DPT_POINTLIST,nOffset,nNumVerts );
                break;
            case RT_LINELIST:
                m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST,nOffset,nNumVerts/2 );
                break;
            case RT_LINESTRIP:
                m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP,nOffset,nNumVerts-1 );
                break;
            case RT_TRIANGLELIST:
                m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,nOffset,nNumVerts/3 );
                break;
            case RT_TRIANGLESTRIP:
                m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,nOffset,nNumVerts-2 );
                break;
            case RT_TRIANGLEFAN:
                m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN,nOffset,nNumVerts-2 );
                break;
            }
        }
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Render the geometry buffer directly.
// ----------------------------------------------------------------------
// Param -> IN:
//      VERTEXTYPE:         Vertex type of the buffer.
//      UINT:               Offset of the vertex.
//      UINT:               Offset of the index.
//      UINT:               Count of the vertices.
//      UINT:               Count of the indices.
//      FlyGraphicBuffer*:  Pointer to the vertex buffer.
//      FlyGraphicBuffer*:  Pointer to the index buffer.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderer::RenderBuffer( VERTEXTYPE nVType,UINT nVOffset,UINT nIOffset,
                                      UINT nNumVerts,UINT nNumIndis,FlyGraphicBuffer* pVB,
                                      FlyGraphicBuffer* pIB )
{
    FlyRenderMaterial* pMaterial;

    if( nVType >= m_VFormats.size() || nNumVerts == 0 )
        return FLY_INVALIDPARAM;

    if( m_pMaterial )
        pMaterial = m_pMaterial;
    else
        pMaterial = m_pDefaultMaterial;

    // Set the Vertex format.
    if( pMaterial->IsFixedPipeline() )
    {
        m_pd3dDevice->SetFVF( m_VFormats[nVType].dwFVF );
    }
    else
    {
        m_pd3dDevice->SetFVF( 0 );
        m_pd3dDevice->SetVertexDeclaration( m_VFormats[nVType].pD3DDecl );
    }

    // Setup the vertex buffer.
    m_pd3dDevice->SetStreamSource( 0,((FlyD3DVertexBuffer*)pVB)->GetD3DVBPtr(),0,m_VFormats[nVType].nStrideSize );
    if( nNumIndis > 0 ) m_pd3dDevice->SetIndices( ((FlyD3DIndexBuffer*)pIB)->GetD3DIBPtr() );

    // Setup the Render Material.
    if( pMaterial->IsFixedPipeline() )
    {
        for( UINT i=0;i<pMaterial->GetNumPasses();i++ )
        {
            pMaterial->ApplyPass( i );

            if( nNumIndis > 0 )
            {
                switch( m_RenderType )
                {
                case RT_POINTLIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_POINTLIST,nVOffset,0,
                        nNumVerts,nIOffset,nNumIndis );
                    break;
                case RT_LINELIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_LINELIST,nVOffset,0,
                        nNumVerts,nIOffset,nNumIndis/2 );
                    break;
                case RT_LINESTRIP:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_LINESTRIP,nVOffset,0,
                        nNumVerts,nIOffset,nNumIndis-1 );
                    break;
                case RT_TRIANGLELIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,nVOffset,0,
                        nNumVerts,nIOffset,nNumIndis/3 );
                    break;
                case RT_TRIANGLESTRIP:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP,nVOffset,0,
                        nNumVerts,nIOffset,nNumIndis-2 );
                    break;
                case RT_TRIANGLEFAN:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLEFAN,nVOffset,0,
                        nNumVerts,nIOffset,nNumIndis-2 );
                    break;
                }
            }
            else
            {
                switch( m_RenderType )
                {
                case RT_POINTLIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_POINTLIST,nVOffset,nNumVerts );
                    break;
                case RT_LINELIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST,nVOffset,nNumVerts/2 );
                    break;
                case RT_LINESTRIP:
                    m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP,nVOffset,nNumVerts-1 );
                    break;
                case RT_TRIANGLELIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,nVOffset,nNumVerts/3 );
                    break;
                case RT_TRIANGLESTRIP:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,nVOffset,nNumVerts-2 );
                    break;
                case RT_TRIANGLEFAN:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN,nVOffset,nNumVerts-2 );
                    break;
                }
            }
        }
    }
    else
    {
        FlyRenderTechnique* pTechnique = pMaterial->GetActiveTechnique();

        // Update parameters for the GPU program.
        pMaterial->GetActiveRenderEffectInstance()->UpdateParameters( &m_AutoParameters );

        for( UINT p=0;p<pTechnique->GetNumPasses();p++ )
        {
            pTechnique->ApplyPass( p );

            if( nNumIndis > 0 )
            {
                switch( m_RenderType )
                {
                case RT_POINTLIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_POINTLIST,nVOffset,0,
                        nNumVerts,nIOffset,nNumIndis );
                    break;
                case RT_LINELIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_LINELIST,nVOffset,0,
                        nNumVerts,nIOffset,nNumIndis/2 );
                    break;
                case RT_LINESTRIP:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_LINESTRIP,nVOffset,0,
                        nNumVerts,nIOffset,nNumIndis-1 );
                    break;
                case RT_TRIANGLELIST:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,nVOffset,0,
                        nNumVerts,nIOffset,nNumIndis/3 );
                    break;
                case RT_TRIANGLESTRIP:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP,nIOffset,0,
                        nNumVerts,nIOffset,nNumIndis-2 );
                    break;
                case RT_TRIANGLEFAN:
                    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLEFAN,nVOffset,0,
                        nNumVerts,nIOffset,nNumIndis-2 );
                    break;
                }
            }
            else
            {
                switch( m_RenderType )
                {
                case RT_POINTLIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_POINTLIST,nVOffset,nNumVerts );
                    break;
                case RT_LINELIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST,nVOffset,nNumVerts/2 );
                    break;
                case RT_LINESTRIP:
                    m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP,nVOffset,nNumVerts-1 );
                    break;
                case RT_TRIANGLELIST:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,nVOffset,nNumVerts/3 );
                    break;
                case RT_TRIANGLESTRIP:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,nVOffset,nNumVerts-2 );
                    break;
                case RT_TRIANGLEFAN:
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN,nVOffset,nNumVerts-2 );
                    break;
                }
            }
        }
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Flush the render queue, Render all the data.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderer::FlushRenderQueue(void)
{
    FlyMatrix mWorldMatrix;
    RenderQueue::iterator itr;

    // 1. Render the default queue.
    std::sort( m_DefaultQueue.begin(),m_DefaultQueue.end(),DefaultObjCompare() );

    for( itr=m_DefaultQueue.begin();itr!=m_DefaultQueue.end();itr++ )
    {
        FlyRenderable* pRenderable = *itr;
        bool bIsFixedPipeline = pRenderable->IsFixedPipeline();

        // Update the world matrix.
        pRenderable->GetWorldMatrices( &mWorldMatrix );
        m_pRDevice->SetWorldMatrix( &mWorldMatrix );

        if( !bIsFixedPipeline )
            m_AutoParameters.SetWorldMatrix( mWorldMatrix );

        // Set the light stage.
        if( pRenderable->IsLighting() )
        {
            for( int i=0;i<MAX_LIGHTS;i++ )
            {
                FlyLightObject* pObj = pRenderable->GetLightObject( i );

                if( pObj )
                    m_pRDevice->SetLight( i,pObj->GetLightData() );
                else
                    m_pRDevice->SetLight( i,NULL );

                if( !bIsFixedPipeline )
                    m_AutoParameters.SetLightObject( i,pObj );
            }
        }

        // Render the object.
        pRenderable->Render();

        // Add the count of the rendered objects.
        m_nNumRenderedVerts += pRenderable->GetNumVerts();
        m_nNumRenderedIndis += pRenderable->GetNumIndis();
        m_nNumRenderedPrims += pRenderable->GetNumPrims();
    }

    // 2. Render the transparent renderQueue.
    std::sort( m_AlphaQueue.begin(),m_AlphaQueue.end(),AlphaObjCompare() );

    for( itr=m_AlphaQueue.begin();itr!=m_AlphaQueue.end();itr++ )
    {
        FlyRenderable* pRenderable = *itr;
        bool bFixedPipeline = pRenderable->IsFixedPipeline();

        // Update the world matrix.
        pRenderable->GetWorldMatrices( &mWorldMatrix );
        m_pRDevice->SetWorldMatrix( &mWorldMatrix );

        if( !bFixedPipeline )
            m_AutoParameters.SetWorldMatrix( mWorldMatrix );

        // Set the light stage.
        if( pRenderable->IsLighting() )
        {
            for( int i=0;i<MAX_LIGHTS;i++ )
            {
                FlyLightObject* pObj = pRenderable->GetLightObject( i );

                if( pObj )
                    m_pRDevice->SetLight( i,pObj->GetLightData() );
                else
                    m_pRDevice->SetLight( i,NULL );

                if( !bFixedPipeline )
                    m_AutoParameters.SetLightObject( i,pObj );
            }
        }

        // Render the object.
        pRenderable->Render();

        // Add the count of the rendered objects.
        m_nNumRenderedVerts += pRenderable->GetNumVerts();
        m_nNumRenderedIndis += pRenderable->GetNumIndis();
        m_nNumRenderedPrims += pRenderable->GetNumPrims();
    }

    m_nNumRenderedObjects = (UINT)(m_DefaultQueue.size()+m_AlphaQueue.size());

    // Clear the render objects.
    m_DefaultQueue.clear();
    m_AlphaQueue.clear();

    return FLY_OK;
}


//------------------------------------------------------------------------
// Flush all the screen layout objects.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderer::FlushLayoutQueue(void)
{
    RenderQueue::iterator itr;

    for( itr=m_LayoutQueue.begin();itr!=m_LayoutQueue.end();itr++ )
        (*itr)->Render();

    // Clear the layout queue.
    m_LayoutQueue.clear();

    return FLY_OK;
}


//------------------------------------------------------------------------
// Get the current render type. (Render Dynamicly)
//------------------------------------------------------------------------
RENDERTYPE FlyD3DRenderer::GetRenderType(void) const
{
    return m_RenderType;
}


//------------------------------------------------------------------------
// Get the current render material. (Render Dynamicly)
//------------------------------------------------------------------------
FlyRenderMaterial* FlyD3DRenderer::GetRenderMaterial(void) const
{
    return m_pMaterial;
}


//------------------------------------------------------------------------
// Set the current render type. (Render dynamicly)
// ----------------------------------------------------------------------
// Param -> IN:
//      RENDERTYPE:     New render type.
//------------------------------------------------------------------------
void FlyD3DRenderer::SetRenderType( RENDERTYPE Type )
{
    m_RenderType = Type;
}


//------------------------------------------------------------------------
// Set the current render material. (Render dynamicly)
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyRenderMaterial*:     Pointer to the render material.
//------------------------------------------------------------------------
void FlyD3DRenderer::SetRenderMaterial( FlyRenderMaterial* pMaterial )
{
    m_pMaterial = pMaterial;
}


//------------------------------------------------------------------------
// Get the vertex size of specified vertex type.
//------------------------------------------------------------------------
UINT FlyD3DRenderer::GetVertexSize( VERTEXTYPE vertexType ) const
{
    if( vertexType >= m_VFormats.size() )
        return 0;

    return m_VFormats[vertexType].nStrideSize;
}


//------------------------------------------------------------------------
// Set the count of rendered objects to zero.
//------------------------------------------------------------------------
void FlyD3DRenderer::ResetRenderedCount(void)
{
    m_nNumRenderedVerts = 0;
    m_nNumRenderedIndis = 0;
    m_nNumRenderedPrims = 0;
    m_nNumRenderedObjects = 0;
}


//------------------------------------------------------------------------
// Get the count of the rendered verts in this frame.
//------------------------------------------------------------------------
UINT FlyD3DRenderer::GetNumRenderedVerts(void) const
{
    return m_nNumRenderedVerts;
}


//------------------------------------------------------------------------
// Get the count of the rendered indis in this frame.
//------------------------------------------------------------------------
UINT FlyD3DRenderer::GetNumRenderedIndis(void) const
{
    return m_nNumRenderedIndis;
}


//------------------------------------------------------------------------
// Get the count of the rendered prims in this frame.
//------------------------------------------------------------------------
UINT FlyD3DRenderer::GetNumRenderedPrims(void) const
{
    return m_nNumRenderedPrims;
}


//------------------------------------------------------------------------
// Get the count of the rendered objects.
//------------------------------------------------------------------------
UINT FlyD3DRenderer::GetNumRenderedObjects(void) const
{
    return m_nNumRenderedObjects;
}


//////////////////////////////////////////////////////////////////////////

// Compare the default renderable Queue.
bool FlyD3DRenderer::DefaultObjCompare::operator() ( FlyRenderable* rhs1,
                                                     FlyRenderable* rhs2 ) const
{
    return rhs1->GetRenderPriority() < rhs2->GetRenderPriority();
}

// Compare the alpha renderable queue.
bool FlyD3DRenderer::AlphaObjCompare::operator() ( FlyRenderable* rhs1,
                                                   FlyRenderable* rhs2 ) const
{
    FlyVector vCamPos = FlyKernel::Instance().GetRenderDevice()->GetCameraPos();

    float fDist1 = rhs1->GetSqrCameraDist( vCamPos );
    float fDist2 = rhs2->GetSqrCameraDist( vCamPos );
    return (fDist1 > fDist2);
}
*/