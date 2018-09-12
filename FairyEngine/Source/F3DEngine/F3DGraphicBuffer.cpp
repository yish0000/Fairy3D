/*
 * ------------------------------------------------------------------------
 *  Name:   F3DGraphicBuffer.cpp
 *  Desc:   This file implements some graphic buffers for engine.
 *  Author: Yish
 *  Date:   2010/12/19
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DGraphicBuffer.h"


///////////////////////////////////////////////////////////////////////////
//// Implements class F3DGraphicBuffer

/** Constructor.
*/
F3DGraphicBuffer::F3DGraphicBuffer(void)
	: m_Usage(BU_STATIC), m_MemMode(MM_DEFAULT), m_nBufferSize(0)
{
}

/** Destructor.
*/
F3DGraphicBuffer::~F3DGraphicBuffer(void)
{
}

/** Read some data from the buffer.
*/
void F3DGraphicBuffer::ReadData( uint32 nOffset, uint32 nLength, void* pDest )
{
    void* ptr = Lock( nOffset, nLength, LOCK_READONLY );

    if( ptr )
    {
        memcpy( pDest, ptr, nLength );
        Unlock();
    }
}

/** Write some data to the buffer.
*/
void F3DGraphicBuffer::WriteData( uint32 nOffset, uint32 nLength, const void* pSource,
                                  bool bDiscard )
{
    void* ptr = Lock( nOffset, nLength, bDiscard ? LOCK_DISCARD : LOCK_NORMAL );

    if( ptr )
    {
        memcpy( ptr,pSource,nLength );
        Unlock();
    }
}