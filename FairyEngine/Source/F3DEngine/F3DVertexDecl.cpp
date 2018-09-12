/*
 * ------------------------------------------------------------------------
 *  Name:   F3DVertexDecl.cpp
 *  Desc:   This file define a class which describe the vertex format.
 *  Author: Yish
 *  Date:   2010/12/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DVertexDecl.h"

/** Constructor.
*/
F3DVertexDecl::F3DVertexDecl(void) : m_nSize(0)
{
}

/** Destructor.
*/
F3DVertexDecl::~F3DVertexDecl(void)
{
}

/** Add a new vertex element.
@Param The data type.
@Param What the data will be used for.
*/
bool F3DVertexDecl::AddElement( uint32 streamIdx, EElementType type, EElementUsage usage )
{
    uint32 nExisted = 0;

    for( size_t i=0; i<m_Elements.size(); i++ )
    {
        if( m_Elements[i].Usage == usage )
            nExisted++;
    }

    switch( usage )
    {
    case EU_POSITION:
    case EU_NORMAL:
        if( nExisted >= 2 )
        {
            FLOG_ERROR("F3DVertexDecl::AddElement, Each vertex only can has two position or normal!");
			return false;
        }
        break;
    case EU_TEXCOORD:
        if( nExisted >= MAX_TEXTURE_STAGES )
        {
			FLOG_ERRORF("F3DVertexDecl::AddElement, Each vertex only can has (%d) texcoord!", MAX_TEXTURE_STAGES);
			return false;
        }
        break;
    default:
        if( nExisted >= 1 )
        {
            FLOG_ERROR("F3DVertexDecl::AddElement, The specified vertex element must only can be one!");
			return false;
        }
    }

    SVertexElement element;
	element.StreamIdx = streamIdx;
	element.Offset = GetStreamStride(streamIdx);
    element.Type = type;
    element.Usage = usage;
    element.nUsageIndex = nExisted;
    m_Elements.push_back(element);

	m_nSize += GetElementSize(type);
	return true;
}

/** Delete the vertex element.
*/
void F3DVertexDecl::DeleteElement( uint32 index )
{
    if( index >= m_Elements.size() )
    {
        FLOG_ERROR( "F3DVertexDecl::DeleteElement, Cannot find the specified element!" );
		return;
    }

	uint32 nElemSize = GetElementSize(m_Elements[index].Type);
	m_nSize -= nElemSize;
    m_Elements.erase( m_Elements.begin() + index );

	// Rebuild the offset.
	std::map<uint32, uint32> streamStride;
	VertexElementList::iterator it = m_Elements.begin();
	for(; it!=m_Elements.end(); ++it)
	{
		it->Offset = streamStride[it->StreamIdx];
		streamStride[it->StreamIdx] += GetElementSize(it->Type);
	}
}

/** Delete all the vertex elements.
*/
void F3DVertexDecl::ClearAllElements()
{
	m_Elements.clear();
	m_nSize = 0;
}

/** Get the specified element type.
*/
EElementType F3DVertexDecl::GetElementType( uint32 index ) const
{
    if( index >= m_Elements.size() )
    {
		FLOG_ERROR( "F3DVertexDecl::GetElementType, Cannot find the specified element!" );
		return (EElementType)-1;
    }

    return m_Elements[index].Type;
}

/** Get the specified element usage.
*/
EElementUsage F3DVertexDecl::GetElementUsage( uint32 index ) const
{
    if( index >= m_Elements.size() )
    {
		FLOG_ERROR( "F3DVertexDecl::GetElementUsage, Cannot find the specified element!" );
		return (EElementUsage)-1;
    }

    return m_Elements[index].Usage;
}

/** Get the specified usage index.
*/
uint32 F3DVertexDecl::GetUsageIndex( uint32 index ) const
{
    if( index >= m_Elements.size() )
    {
        FLOG_ERROR( "F3DVertexDecl::GetUsageIndex, Cannot find the specified element!" );
		return 0;
    }

    return m_Elements[index].nUsageIndex;
}

/** Get the offset of the specified element.
*/
uint32 F3DVertexDecl::GetElementOffset(uint32 index) const
{
	if( index >= m_Elements.size() )
	{
		FLOG_ERROR("F3DVertexDecl::GetElementOffset, Cannot find the specified element!");
		return 0;
	}

	return m_Elements[index].Offset;
}

/** Get the stream index of specified element.
*/
uint32 F3DVertexDecl::GetElementStream(uint32 index) const
{
	if( index >= m_Elements.size() )
	{
		FLOG_ERROR("F3DVertexDecl::GetElementStream, Cannot find the specified element!");
		return 0;
	}

	return m_Elements[index].StreamIdx;
}

/** Get the stride of specified stream.
*/
uint32 F3DVertexDecl::GetStreamStride(uint32 nStreamIdx) const
{
	uint32 nSize = 0;

	VertexElementList::const_iterator it = m_Elements.begin();
	for(; it!=m_Elements.end(); ++it)
	{
		if( it->StreamIdx == nStreamIdx )
			nSize += GetElementSize(it->Type);
	}

	return nSize;
}

/** Get the size of specified element type.
*/
uint32 F3DVertexDecl::GetElementSize(EElementType type) const
{
	uint32 nSize = 0;

	switch( type )
	{
	case ET_FLOAT1:
	case ET_COLOR:
	case ET_UBYTE4:
		nSize = 4;
		break;
	case ET_FLOAT2:
		nSize = 8;
		break;
	case ET_FLOAT3:
		nSize = 12;
		break;
	case ET_FLOAT4:
		nSize = 16;
		break;
	default:
		FASSERT(!"Unknown element type!");
	}

	return nSize;
}

/** Make the internal vertex type.
*/
F3DVertexDecl F3DVertexDecl::MakeVertexDecl( EVertexType type )
{
    F3DVertexDecl ret;

    switch( type )
    {
	case VTT_NORMAL:
		ret.AddElement(0, ET_FLOAT3, EU_POSITION);
		ret.AddElement(0, ET_COLOR, EU_DIFFUSE);
		ret.AddElement(0, ET_FLOAT2, EU_TEXCOORD);
		break;
	case VTT_STATIC_VLIGHT:
		ret.AddElement(0, ET_FLOAT3, EU_POSITION);
		//ret.AddElement();
		break;
    }

    return ret;
}