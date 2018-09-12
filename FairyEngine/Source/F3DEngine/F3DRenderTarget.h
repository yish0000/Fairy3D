/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRenderTarget.h
 *  Desc:   This file define the base class of renderTarget for engine.
 *  Author: Yish
 *  Date:   2010/12/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_RENDERTARGET_H__
#define __F3D_RENDERTARGET_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DViewport.h"

///////////////////////////////////////////////////////////////////////////


/** Priority of the renderTarget
*/
#define TARGET_PRIORITY_UNKNOWN         0
#define TARGET_PRIORITY_RENDERWINDOW    1
#define TARGET_PRIORITY_TEXTURE         2

/** Render Target.
It's used to store the render result for engine.
*/
class FAIRY_API F3DRenderTarget : public FGeneralAlloc
{
public:

	// Sort the renderTarget by its priority.
	struct TargetCompare
	{
		bool operator () ( F3DRenderTarget* t1, F3DRenderTarget* t2 ) const
		{
			return t1->GetPriority() > t2->GetPriority();
		}
	};

    typedef std::vector<F3DViewport*> ViewportList;

protected:
    AString m_sName;                    // Name of the renderTarget.
    int m_nPriority;                    // Priority of the renderTarget.

    uint32 m_nWidth;                    // Width of the renderTarget.
    uint32 m_nHeight;                   // Height of the renderTarget.

    F3DClipPlaneList m_ClipPlanes;      // Clip plane list of this target.
    ViewportList m_Viewports;           // Viewport list of this renderTarget.

    F3DColor m_ClearColor;              // The default color after clear the renderTarget.

public:
    F3DRenderTarget( const AString& name );
    virtual ~F3DRenderTarget(void);

    // Add a new viewport for this target.
    F3DViewport* AddViewport(F3DCameraBase* camera, const SViewportDesc& vp);
	F3DViewport* AddViewport(F3DCameraBase* camera, float fX, float fY, float fWidth, float fHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
    // Remove the specified viewport.
    void RemoveViewport( size_t nIndex );
    // Get the count of viewports.
	size_t GetNumViewports(void) const { return m_Viewports.size(); }
    // Get the specified viewport.
    F3DViewport* GetViewport( size_t nIndex );

    // Add a clip plane.
    void AddClipPlane( const F3DPlane& plane );
    // Remove the specified clip plane.
    void RemoveClipPlane( size_t index );
    // Get the count of clip planes.
    size_t GetNumClipPlanes(void) const { return m_ClipPlanes.size(); }
    // Get the specified clip plane.
    const F3DPlane& GetClipPlane( size_t index ) const;
    // Get the clip plane list.
    const F3DClipPlaneList& GetClipPlaneList(void) const { return m_ClipPlanes; }

    // Update the renderTarget.
    virtual void Update(void) = 0;
	// Send the content to front buffer, after call this screen will present the result.
    virtual void SwapBuffers(void) = 0;

	// Set the clear color.
    void SetClearColor( float r, float g, float b )
    {
        m_ClearColor.fR = r;
        m_ClearColor.fG = g;
        m_ClearColor.fB = b;
    }

	// Set the clear color.
    void SetClearColor( const F3DColor& color ) { m_ClearColor = color; }
    // Get the clear color.
    const F3DColor& GetClearColor(void) const { return m_ClearColor; }

    // Get the width of renderTarget.
    uint32 GetWidth(void) const { return m_nWidth; }
    // Get the height of renderTarget.
    uint32 GetHeight(void) const { return m_nHeight; }

    // Get the priority of renderTarget.
    int GetPriority(void) const { return m_nPriority; }

    // Get the name of renderTarget.
    const AString& GetName(void) const { return m_sName; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_RENDERTARGET_H__