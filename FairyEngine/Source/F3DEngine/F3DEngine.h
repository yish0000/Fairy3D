/*
 * ------------------------------------------------------------------------
 *  Name:   F3DEngine.h
 *  Desc:   本文件定义了引擎的图形模块的控制器。
 *  Author: Yish
 *  Date:   2011/1/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_ENGINE_H__
#define __F3D_ENGINE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////

/** 3D Engine Controller.
@remarks
    本类是3D引擎的主接口，用户可以通过该接口来控制整个3D引擎的各个模块。
*/
class FAIRY_API F3DEngine : public FGeneralAlloc
{
public:     // Types

public:
    // Destructor.
    ~F3DEngine(void);

    // Initialize the engine.
    bool Initialize();
    // Shutdown the engine.
    void Shutdown();
	// Call me at each game frame.
	void Frame();

    // Save the current screen shot.
    void SaveScreenshot();

	// Get the pointer to the scene manager.
	F3DSceneManager* GetSceneManager() { return m_pSceneMgr; }
	// Get the render device.
	F3DRenderDevice* GetRenderDevice() { return m_pDevice; }
	// Get the renderer.
	F3DRenderer* GetRenderer() { return m_pRenderer; }

	// Get the singleton instance.
    static F3DEngine& GetInstance();

protected:
	F3DRenderDevice* m_pDevice;			// Pointer to the render device.
	F3DRenderer* m_pRenderer;			// Pointer to the renderer.
	F3DSceneManager* m_pSceneMgr;		// Pointer to the scene manager.

private:
    F3DEngine(void);
	F_DISABLE_COPY_ASSIGNMENT(F3DEngine);
};

///////////////////////////////////////////////////////////////////////////
//  
//  Global Functions of 3D Engine
//  
///////////////////////////////////////////////////////////////////////////

extern "C" {

	// Initialize or finalize the engine.
	FAIRY_API bool F3D_Init( const char* logFile = "F3D.log" );
	FAIRY_API void F3D_Finalize();

	// Get the main interface of engine.
	FAIRY_API F3DEngine* F3D_GetEngine();

}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_ENGINE_H__