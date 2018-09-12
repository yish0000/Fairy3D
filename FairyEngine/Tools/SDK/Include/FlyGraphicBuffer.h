//------------------------------------------------------------------------
// Name: FlyGraphicBuffer.h
// Desc: This file is to define a base class for the graphic data buffer.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_GRAPHIC_BUFFER_H__
#define __FLY_GRAPHIC_BUFFER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


typedef enum _BUFFERUSAGE
{
    BU_STATIC = 1,              // Craate the buffer with static memory.
    BU_DYNAMIC = 2,             // Create the buffer with dynamic memory.
    BU_WRITEONLY = 4,           // Application can only write to the buffer.

    BU_STATIC_WRITEONLY = 5,    // Static and writeonly.
    BU_DYNAMIC_WRITEONLY = 6,   // Dynamic and writeonly.
} BUFFERUSAGE;

typedef enum _LOCKOPTION
{
    LOCK_NORMAL,        // Normal buffer locking mode.
    LOCK_DISCARD,       // The application overwrites (with a write-only operation) every 
                        // location within the region being locked, Must used with the
                        // Dynamic buffer.

    LOCK_NOOVERWRITE,   // The application promises not to overwrite any data in buffers
    LOCK_READONLY,      // The application will not write the buffer.
} LOCKOPTION;

typedef enum _MEMORYMODE
{
    MM_DEFAULT,     // Usually place the data into video memory or AGP memory. 
    MM_SYSMEMORY,   // Store the data in the system memory.
    MM_MANAGED,     // The buffer's memory is managed by system.
} MEMORYMODE;

/*
* This abstract class define a graphic buffer object.
* It contained the vertex buffer and index buffer.
*/
class _FLY_EXPORT FlyGraphicBuffer
{
protected:
    BUFFERUSAGE m_Usage;        // Usage of the graphic buffer.
    MEMORYMODE m_MemMode;       // Memory mode of the buffer.
    UINT m_nBufferSize;         // Size of the graphic buffer.

public:
    // Constructor and destructor.
    FlyGraphicBuffer(void);
    virtual ~FlyGraphicBuffer(void);

    virtual HRESULT Resize( UINT nSize );
    virtual void* Lock( UINT nOffset,UINT nLength,LOCKOPTION options );
    virtual HRESULT Unlock(void);
    virtual HRESULT ReadData( UINT nOffset,UINT nLength,void* pDest );
    virtual HRESULT WriteData( UINT nOffset,UINT nLength,const void* pSource,bool bDiscard );

    void* Lock( LOCKOPTION options );
    UINT GetBufferSize(void);
    BUFFERUSAGE GetUsage(void);
    MEMORYMODE GetMemoryMode(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_GRAPHIC_BUFFER_H__