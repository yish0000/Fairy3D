/*
 * ------------------------------------------------------------------------
 *  Name:   F3DVertexDecl.h
 *  Desc:   This file define a class which describe the vertex format.
 *  Author: Yish
 *  Date:   2010/12/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_VERTEXDECL_H__
#define __F3D_VERTEXDECL_H__

//// HEADER OF THIS FILE //////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////

/** Some internal vertex type.
*/
enum EVertexType
{
	VTT_NORMAL,				// Position, color, texture.
	VTT_STATIC_VLIGHT,		// Vertex type for static model.
	VTT_STATIC_LIGHTMAP,	// Vertex type for static model using light map.
	VTT_SKIN,				// For skin model.
	VTT_RIGID,				// For rigid animation.
};

///////////////////////////////////////////////////////////////////////////

/** Vertex declaration.
@remarks
	Defines the vertex data layout. Each vertex can contain one or more data types,
	and each data type is described by a vertex element.
*/
class FAIRY_API F3DVertexDecl : public FGeneralAlloc
{
public:

    struct SVertexElement
    {
		uint32 StreamIdx;		// Stream index.
		uint32 Offset;			// Offset of the element.
        EElementType Type;      // The data type.
        EElementUsage Usage;    // Defines what the data will be used for.
        uint32 nUsageIndex;     // Modifies the usage data to allow the user to specify multiple usage types. 
    };

    typedef std::vector<SVertexElement> VertexElementList;

protected:
    VertexElementList m_Elements;   // Vertex element list.
    uint32 m_nSize;                 // Size of each vertex.

public:
    F3DVertexDecl(void);
    virtual ~F3DVertexDecl(void);

	// Add a new vertex element.
    bool AddElement(uint32 streamIdx, EElementType type, EElementUsage usage);
	// Delete the specified element.
    void DeleteElement(uint32 index);
	// Clear all the elements
	void ClearAllElements();

	// Get the count of vertex elements.
    uint32 GetNumElements() const { return (uint32)m_Elements.size(); }
	// Get the type of specified element.
    EElementType GetElementType( uint32 index ) const;
	// Get the usage of specified element.
    EElementUsage GetElementUsage( uint32 index ) const;
	// Get the usage index of specified element.
    uint32 GetUsageIndex( uint32 index ) const;
	// Get the offset of the specified element.
	uint32 GetElementOffset(uint32 index) const;
	// Get the stream index of specified element.
	uint32 GetElementStream(uint32 index) const;

	// Get the count of stream.
	uint32 GetNumStreams() const;
	// Get the stride of specified stream.
	uint32 GetStreamStride(uint32 nStreamIdx) const;
	// Get the size of each entire vertex.
	uint32 GetStrideSize() const { return m_nSize; }

    // Make the internal vertex type.
    static F3DVertexDecl MakeVertexDecl( EVertexType type );

protected:

	// Get the size of specified element type.
	uint32 GetElementSize(EElementType type) const;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_VERTEXDECL_H__