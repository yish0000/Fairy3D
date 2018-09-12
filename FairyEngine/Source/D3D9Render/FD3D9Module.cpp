/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9Module.cpp
 *  Desc:   This file export two functions which create and destroy the
			render device.
 *  Author: Yish
 *  Date:   2011/6/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FD3D9Common.h"
#include "FD3D9RenderDevice.h"

extern "C"
{
	F3DRenderDevice* g_pDevice = NULL;

    // Create a render device for engine.
    FAIRY_API bool CreateDevice(F3DRenderDevice*& pDevice)
    {
		if( g_pDevice )
		{
			FLOG_ERRORF("CreateDevice, The render device is not null!");
			pDevice = g_pDevice;
			return true;
		}

		// Create the log file for this module.
		FLogManager::GetInstance().CreateLogFile("Logs\\D3D9Render.log", "");

        // Create the render device.
        g_pDevice = new FD3D9RenderDevice();
		pDevice = g_pDevice;
		return true;
    }

    // Destroy the render device.
    FAIRY_API void DestroyDevice()
    {
        F_SAFE_DELETE(g_pDevice);
    }
}