/*
 * ------------------------------------------------------------------------
 *  Name:   F3DResourceManager.h
 *  Desc:   This file define the resource manager for engine.
 *  Author: Yish
 *  Date:   2012/8/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_RESOURCEMANAGER_H__
#define __F3D_RESOURCEMANAGER_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DResource.h"

///////////////////////////////////////////////////////////////////////////

/** Handle of the resource.
*/
template <class T> class F3DHandle
{
private:
	uint32 m_handle;

public:
	F3DHandle(uint32 idx, uint32 uid) : m_handle(idx | (uid << 16)) {}
	~F3DHandle();

	// Index of the resource.
	uint32 GetIndex() const { return m_handle & 0xffff; }
	// Unique ID of the resource.
	uint32 GetUniqueID() const { return m_handle >> 16; }

	// Is this handle valid ?
	bool IsValid() const;

	// Convert the handle to uint32.
	operator uint32 () const { return m_handle; }

	// Compare operator.
	bool operator == (const F3DHandle& rhs) const { return m_handle == rhs.m_handle; }
	bool operator != (const F3DHandle& rhs) const { return m_handle != rhs.m_handle; }
};

typedef F3DHandle<F3DResource> F3DResourceHandle;

/** Resource manager for engine.
*/
class FAIRY_API F3DResourceManager
{
public:
	typedef std::function<void(F3DResourceHandle)> AsyncLoadFinish;

public:
	virtual ~F3DResourceManager();

	// Get the singleton instance.
	static F3DResourceManager& GetInstance();

	// Load the specified resource from file synchronously.
	F3DResourceHandle Load(const char* filename);
	// Load the specified resource from memory synchronously.
	F3DResourceHandle LoadFromMemory(const FBYTE* buffer);
	// Load the specified resource asynchronously.
	void LoadAsync(const char* filename, AsyncLoadFinish callback);
	// Load the specified resource from memory synchronously.
	void LoadFromMemoryAsync(const FBYTE* buffer, AsyncLoadFinish callback);
	// Unload the specified resource.
	void Unload(F3DResourceHandle handle);

	// Unload all the unused resources.
	void UnloadDeadResources();

protected:


private:
    F3DResourceManager();
	F_DISABLE_COPY_ASSIGNMENT(F3DResourceManager);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_RESOURCEMANAGER_H__