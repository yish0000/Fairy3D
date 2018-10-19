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
template <class TAG> class F3DHandle
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

/** Resource manager for engine.
*/
class FAIRY_API F3DResourceManager
{
public:
	virtual ~F3DResourceManager();

	static F3DResourceManager& GetInstance();

	// 

	// Unload all the unused resources.
	void UnloadDeadResources();

private:
    F3DResourceManager();
	F_DISABLE_COPY_ASSIGNMENT(F3DResourceManager);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_RESOURCEMANAGER_H__