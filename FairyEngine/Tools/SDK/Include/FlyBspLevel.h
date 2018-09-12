//------------------------------------------------------------------------
// Name: FlyBspLevel.h
// Desc: This file define a class which store the bsp level data.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_BSPLEVEL_H__
#define __FLY_BSPLEVEL_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyQuake3Level.h"

#include "FlyBspNode.h"
//////////////////////////////////////////////////////////////////////////


/*
* This class Holds all the data associated with a Binary Space Parition
* (BSP) based indoor level.
*/
class _FLY_EXPORT FlyBspLevel
{
    friend class FlyBspNode;
    friend class FlyBspSceneManager;

    typedef std::vector<FlyTexture*> TextureList;

protected:
    FlyBspNode* m_pRoot;            // Pointer to the root node.

    UINT        m_nTesselation;     // Tesselation value.
    UINT        m_nNumNodes;        // Number of bsp nodes.
    UINT        m_nNumLeaves;       // Number of bsp leaf nodes.

    UINT        m_nNumFaces;        // Number of whole faces.
    sBspFace*   m_pFaces;           // Pointer to the faces.
    UINT        m_nNumBrushes;      // Number of brushes.
    sBspBrush*  m_pBrushes;         // Pointer to the brushes.

    int*        m_pLeafFaces;       // Leaf faces index.
    int*        m_pLeafBrushes;     // Leaf brush index.

    TextureList m_Textures;         // Texture list.
    TextureList m_Lightmaps;        // Light map list.

    // Store the PVS data, to test whether 
    // a node can be seen at another node.
    sBspVisData m_VisData;

    HRESULT GenerateFaces( const FlyQuake3Level& level );
    HRESULT CreateTextures( const FlyQuake3Level& level );
    HRESULT CreateLightmaps( const FlyQuake3Level& level );
    HRESULT GenerateBspNodes( const FlyQuake3Level& level );
    HRESULT GenerateBrushes( const FlyQuake3Level& level );

    void MakePolygonFace( int nFaceID,const FlyQuake3Level& level );
    void MakeMeshFace( int nFaceID,const FlyQuake3Level& level );
    void MakePatchFace( int nFaceID,const FlyQuake3Level& level );
    void Tesselate( sBspVertex* pVerts,WORD* pIndis,int nIndexStart,sBspVertex* pCtrlPts );

public:
    // Constructor and destructor.
    FlyBspLevel(void);
    ~FlyBspLevel(void);

    // Load the map data.
    HRESULT LoadQuake3Level( const char* cFilename );

    // Get the root node of the level.
    FlyBspNode* GetRootNode(void);

    // Walks the entire BSP tree and returns the leaf
    // which contains the given point.
    FlyBspNode* FindLeaf( const FlyVector& vPos );

    // Check whether the leaf is visible
    bool IsLeafVisible( const FlyBspNode* pFrom,const FlyBspNode* pTo );

    // Test the intersection with the plane list.
    std::pair<bool,float> IntersectBrush( sBspBrush* pBrush,const FlyRay& ray,float fL );

    UINT GetNumNodes(void);
    UINT GetNumLeaves(void);
    UINT GetNumFaces(void);

    // Get the specified face data.
    sBspFace* GetBspFace( UINT nIndex );
    FlyTexture* GetTexture( UINT nIndex );
    FlyTexture* GetLightmap( UINT nIndex );

    UINT GetTesselation(void);
    void SetTesselation( UINT nTesselation );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_BSPLEVEL_H__