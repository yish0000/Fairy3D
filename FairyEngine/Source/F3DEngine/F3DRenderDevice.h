/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRenderDevice.h
 *  Desc:   本文件为引擎定义了一个渲染设备的基类。
 *  Author: Yish
 *  Date:   2010/12/9
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_RENDERDEVICE_H__
#define __F3D_RENDERDEVICE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"
#include "F3DRenderDeviceCaps.h"
#include "F3DRenderWindow.h"
#include "F3DRenderStateSet.h"

///////////////////////////////////////////////////////////////////////////

/** Base class of render device.
@remarks
	This class is the base class of device in each platforms.
	It's used to manage the render targets and render states, Control the render sequence.
@note
    Implements me at each platforms.
*/
class FAIRY_API F3DRenderDevice : public FGeneralAlloc
{
public:

	enum ERenderDeviceType
	{
		RENDER_DEVICE_SW,		// Software renderer
		RENDER_DEVICE_D3D9,		// D3D9 renderer
		RENDER_DEVICE_D3D11,	// D3D11 renderer
		RENDER_DEVICE_GL,		// OpenGL renderer
		RENDER_DEVICE_GLES,		// GLES renderer
	};

	enum
	{
		RENDERER_DEFAULT,			// Default render mode
		RENDERER_DEFERREDSHADING,	// Deferred shading.
	};

    typedef std::set<F3DRenderTarget*, F3DRenderTarget::TargetCompare> TargetQueue;

protected:
	ERenderDeviceType m_DeviceType;		// Type of the render device.
    F3DRenderer* m_pRenderer;           // Pointer to the renderer.

    TargetQueue m_RenderTargets;        // Render target queue.
    F3DRenderWindow* m_pMainWnd;        // Main render window.
    F3DViewport* m_pActiveViewport;     // Active viewport.

	F3DRenderStateSet m_RenderStates;	// Render states

    SRenderDeviceCaps m_DevCaps;        // Capacities of the render device.

public:
    F3DRenderDevice(void);
    virtual ~F3DRenderDevice(void);

	///////////////////////////////////////////////////////////////////////
	// Initialize and shutdown.

    // Initialize the render device.
    virtual bool Init(F3DWindow* mainWnd, bool bShowDialog = true, const AKeyValueList* configData = NULL) = 0;
    // Shutdown the render device.
    virtual void Shutdown() = 0;
    // Refresh the device settings.
    virtual void RefreshDeviceSettings( const AKeyValueList& settings ) = 0;

    ///////////////////////////////////////////////////////////////////////
    // Control the rendering sequence.

    // Start to render.
    virtual void BeginRendering(void) = 0;
    // Clear the frame buffer.
    virtual void ClearBuffer( EFrameClearType clearType, const F3DColor& color = F3DColor::Black,
        float fZValue = 1.0f, ulong nStencilValue = 0 ) = 0;
    // Render the specified data.
    virtual void Render(F3DRenderData* pData);
    // End the rendering.
    virtual void EndRendering(void) = 0;
    // Swap the back buffer to front.
    virtual void SwapBuffers(void);

    ///////////////////////////////////////////////////////////////////////
    // Manage the render targets.

    // Create a new render window.
    virtual F3DRenderWindow* CreateRenderWindow( const AString& name, F3DWindow* wndInfo ) = 0;
    // Create a texture target.
    virtual F3DRenderTarget* CreateTextureTarget( const AString& name ) = 0;
	// Remove the specified render target.
    virtual void RemoveRenderTarget( const AString& name );
    // Get the count of render targets.
    size_t GetNumRenderTargets(void) const { return m_RenderTargets.size(); }
    // Get the specified render target.
    F3DRenderTarget* GetRenderTarget( const AString& name );
    // Get the main render window.
    F3DRenderWindow* GetMainRenderWindow(void) { return m_pMainWnd; }

	///////////////////////////////////////////////////////////////////////
	// Misc functions.

    // Get the active viewport.
    F3DViewport* GetViewport(void) { return m_pActiveViewport; }
    // Set the current viewport.
    virtual void SetViewport( F3DViewport* pViewport ) = 0;

	// Set the render states.
	virtual void SetRenderStates(const F3DRenderStateSet& rs);
	// Set scissor rect.
	virtual void SetSchissorRect(const F2DRECT& rect);
	// Set the clip planes.
    virtual void SetClipPlanes( const F3DClipPlaneList& planeList ) = 0;

    // Set the vertex shader.
    virtual void SetVertexShader( F3DShaderObject* pShader ) = 0;
	// Set the pixel shader.
    virtual void SetPixelShader( F3DShaderObject* pShader ) = 0;

    // Get the render states.
	const F3DRenderStateSet& GetRenderStates() const { return m_RenderStates; }

	// Print the current contents to file.
	virtual void PrintScreenToFile( const char* filename, bool bFront = true );

    // Get the capacities of render device.
	const SRenderDeviceCaps& GetDeviceCaps() const { return m_DevCaps; }

    // Get the current renderer.
    F3DRenderer* GetRenderer(void) { return m_pRenderer; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_RENDER_DEVICE_H__