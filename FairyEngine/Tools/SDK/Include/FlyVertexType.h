//------------------------------------------------------------------------
// Name: FlyVertexType.h
// Desc: This file define a class to store the vertex's element usage.
//       Use when create the new vertex type for Renderer.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_VERTEXTYPE_H__
#define __FLY_VERTEXTYPE_H__


//// HEADER OF THIS FILE /////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlyVertexType
{
    struct VertexElement
    {
        ELEMENTTYPE Type;
        ELEMENTUSAGE Usage;
        int nUsageIndex;
    };

    typedef std::vector<VertexElement> VertexElements;

protected:
    VertexElements m_Elements;
    size_t m_nSize;

public:
    // Constructor and destructor.
    FlyVertexType(void);
    ~FlyVertexType(void);

    bool AddElement( ELEMENTTYPE Type,ELEMENTUSAGE Usage );
    void DeleteElement( UINT nIndex );

    UINT GetNumElements(void) const;
    ELEMENTTYPE GetElementType( UINT nIndex ) const;
    ELEMENTUSAGE GetElementUsage( UINT nIndex ) const;
    int GetUsageIndex( UINT nIndex ) const;
    size_t GetStrideSize(void) const;

    // Make the system-defined vertex type.
    static FlyVertexType MakeVertexType( int type );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_VERTEXTYPE_H__