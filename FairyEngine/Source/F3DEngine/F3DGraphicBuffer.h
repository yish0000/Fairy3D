/*
 * ------------------------------------------------------------------------
 *  Name:   FlyGraphicBuffer.h
 *  Desc:   本文件为引擎定义了一个图形缓冲区基类。
 *  Author: Yish
 *  Date:   2010/12/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_GRAPHICBUFFER_H__
#define __F3D_GRAPHICBUFFER_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// Some enumerations.

enum EBufferType
{
	BT_VERTEXBUFFER,			// Vertex buffer
	BT_INDEXBUFFER,				// Index buffer.
};

enum EBufferUsage
{
    BU_STATIC = 1,              // Static buffer.
    BU_DYNAMIC = 2,             // Dynamic buffer.
    BU_WRITEONLY = 4,           // Write only.

    BU_STATIC_WRITEONLY = BU_STATIC | BU_WRITEONLY,		// Static and write only
    BU_DYNAMIC_WRITEONLY = BU_DYNAMIC | BU_WRITEONLY,	// Dynamic and write only
};

enum ELockOption
{
    LOCK_NORMAL,        // 普通锁定模式
    LOCK_DISCARD,       // 凡锁定的区域将被全部重写
                        // 必须和动态缓冲区一起使用

    LOCK_NOOVERWRITE,   // 确保不会覆盖缓冲区内的任何数据
    LOCK_READONLY,      // 以只读模式锁定该缓冲区
};

enum EMemoryMode
{
    MM_DEFAULT,         // 缓冲区内的数据通常位于显存或AGP Memory
    MM_SYSMEMORY,       // 保存数据到系统内存
    MM_MANAGED,         // 缓冲区内的数据由系统自动管理
};

///////////////////////////////////////////////////////////////////////////

/** Base class of graphic buffers.
*/
class FAIRY_API F3DGraphicBuffer : public FGeneralAlloc
{
protected:
	EBufferType m_Type;			// Type of the buffer.
    uint32 m_nBufferSize;       // Size of the buffer.
    EBufferUsage m_Usage;       // Usage of the buffer.
    EMemoryMode m_MemMode;      // Memory mode of the buffer.

public:
    F3DGraphicBuffer(void);
    virtual ~F3DGraphicBuffer(void);

	// Resize the buffer size. (Will lost the old data)
    virtual bool Resize( uint32 nSize ) = 0;
    // Lock the buffer.
    virtual void* Lock( uint32 nOffset, uint32 nLength, ELockOption options ) = 0;
    // Unlock the buffer.
    virtual void Unlock(void) = 0;
    // Read some data.
    virtual void ReadData( uint32 nOffset, uint32 nLength, void* pDest );
    // Write some data.
    virtual void WriteData( uint32 nOffset, uint32 nLength, const void* pSource, bool bDiscard );

	// Lock the entire buffer.
	void* Lock( ELockOption options ) { return Lock(0, m_nBufferSize, options); }
	// Read the entire buffer.
	void ReadData(void* pDest) { ReadData(0, m_nBufferSize, pDest); }

    // Get the size.
    uint32 GetBufferSize(void) const { return m_nBufferSize; }
    // Get the usage of the buffer.
    EBufferUsage GetUsage(void) const { return m_Usage; }
    // Get the memory mode.
    EMemoryMode GetMemoryMode(void) const { return m_MemMode; }
	// Get the buffer type.
	EBufferType GetBufferType() const { return m_Type; }
};

/** Vertex buffer.
*/
class FAIRY_API F3DVertexBuffer : public F3DGraphicBuffer
{
public:
	F3DVertexBuffer(uint32 nStride) : m_nStride(nStride) {}
	virtual ~F3DVertexBuffer() {}

	// Get the count of vertices.
	virtual uint32 GetNumVerts() const = 0;
	
	// Get the stride of the vertex.
	uint32 GetStride() const { return m_nStride; }

protected:
	uint32 m_nStride;
};

/** Index buffer.
*/
class FAIRY_API F3DIndexBuffer : public F3DGraphicBuffer
{
public:
	F3DIndexBuffer(EIndexFormat format) : m_IdxFormat(format) {}
	virtual ~F3DIndexBuffer() {}

	// Get the count of indices.
	virtual uint32 GetNumIndices() const = 0;

	// Get the stride of the index.
	uint32 GetStride() const { return m_IdxFormat == IF_INDEX16 ? 2 : 4; }
	// Get the index format.
	EIndexFormat GetIndexFormat() const { return m_IdxFormat; }

protected:
	EIndexFormat m_IdxFormat;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_GRAPHICBUFFER_H__