//------------------------------------------------------------------------
// Name: FlyAnimation.h
// Desc: This file define a class act as the skeleton animation object
//       for the FR-engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_ANIMATION_H__
#define __FLY_ANIMATION_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyAnimFormat.h"
#include "FlyRenderMaterial.h"

//////////////////////////////////////////////////////////////////////////


class _FLY_EXPORT FlyAnimation
{
    struct sAnimNode
    {
        sAnimHeader header;
        sJoint* pJoints;
    };

    typedef std::vector<sAnimNode> AnimNodeGroup;
    typedef std::vector<FlyRenderMaterial*> MaterialList;

protected:
    std::string     m_sName;            // Name of the animation.

    sModelHeader    m_sHeader;          // Header model file.
    AVERTEX*        m_pVertices;        // To store the vertices data.
    sVertexOrig*    m_pVertices_Orig;   // To store the original vertices data.
    sFace*          m_pFaces;           // To store the faces data.
    sMesh*          m_pMeshes;          // To store the meshes data.
    sMaterial*      m_pMaterials;       // To store the materials data.

    MaterialList    m_RMaterials;       // Render material list.
    FlyAabb         m_BBox;             // Current boundingBox.
    AnimNodeGroup   m_AnimGroup;        // Animation nodes group.

    int             m_nAnimID;          // ID of the current animNode.
    float           m_fAnimFrame;       // Current frame value.

    int             m_nDirtyState;      // Current state of the animation.

    // Prepare the joints data for the animation.
    HRESULT SetupAnimation( sAnimNode* pNode );

public:
    // Constructor and destructor.
    FlyAnimation(void);
    ~FlyAnimation(void);

    // Destroy the animation.
    void    Destroy(void);

    // Load the animation from file.
    HRESULT LoadAnimation( const char* cModelFile );
    HRESULT AddAnimNode( const char* cAnimFile );

    // Update the animation states.
    HRESULT GoAnimation( int nAnimID,float fFrame );

    UINT    GetNumVerts(void) const;
    UINT    GetNumIndis(void) const;
    UINT    GetNumFaces(void) const;
    UINT    GetNumMaterials(void) const;
    UINT    GetNumMeshes(void) const;

    UINT    GetNumAnimFrames( int nAnimID ) const;
    UINT    GetNumAnimJoints( int nAnimID ) const;

    UINT    GetNumAnimations(void) const;

    // Get the current boundingBox.
    const FlyAabb& GetBoundingBox(void) const;

    const char* GetName(void) const;
    void SetName( const char* cName );

    const AVERTEX* GetVertexData(void) const;
    const sMesh* GetMeshData( UINT nIndex ) const;
    const sFace* GetFaceData(void) const;

    // Get the render material.
    FlyRenderMaterial* GetRenderMaterial( UINT nIndex );

    // Get the dirty state.
    int GetDirtyState(void) const;

protected:
    // Build the render materials.
    HRESULT BuildMaterials(void);

    HRESULT AnimationPrepare(void);
    HRESULT AnimationVertices(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_ANIMATION_H__