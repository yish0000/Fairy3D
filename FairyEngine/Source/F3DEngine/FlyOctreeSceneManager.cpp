//------------------------------------------------------------------------
// Name: FlyOctreeSceneManager.cpp
// Desc: This file is to implement the class FlyOctreeSceneManager.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyOctreeSceneManager.h"

#include "FlyOctree.h"
#include "FlyOctreeNode.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyOctreeSceneManager::FlyOctreeSceneManager(void)
{
    m_pOctreeRoot = NULL;

    FlyAabb box;
    box.vcMin.Set( -10000.0f,-10000.0f,-10000.0f );
    box.vcMax.Set(  10000.0f, 10000.0f, 10000.0f );
    InitScene( box,8 );
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyOctreeSceneManager::~FlyOctreeSceneManager(void)
{
    // Destroy all the scene nodes.
    SceneNodeMap::iterator node;
    for( node=m_SceneNodes.begin();node!=m_SceneNodes.end();node++ )
        SAFE_DELETE( node->second );

    SAFE_DELETE( m_pOctreeRoot );
}


//------------------------------------------------------------------------
// Initialize the octree scene manager.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyAabb&:     BoundingBox for the octree.
//      int:                Max depth value.
//------------------------------------------------------------------------
void FlyOctreeSceneManager::InitScene( const FlyAabb& bbox,int depth )
{
    // Clean the old data.
    SAFE_DELETE( m_pOctreeRoot );

    // Create the root node for octree.
    m_pOctreeRoot = new FlyOctree( NULL );
    m_pOctreeRoot->m_BBox = bbox;
    m_pOctreeRoot->m_vHalfSize = bbox.GetHalfSize();

    m_OctreeBox = bbox;
    m_nMaxDepth = depth;

    m_bShowBBox = false;
}


//------------------------------------------------------------------------
// Clear the scene data.
//------------------------------------------------------------------------
void FlyOctreeSceneManager::ClearScene(void)
{
    FlySceneManager::ClearScene();

    InitScene( m_OctreeBox,m_nMaxDepth );
}


//------------------------------------------------------------------------
// Create a new scene node for the sceneManager.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the scene node.
//------------------------------------------------------------------------
FlySceneNode* FlyOctreeSceneManager::CreateSceneNode( const char* cName )
{
    char nodeName[20];

    if( cName != NULL )
    {
        // Check whether the node has existed ?
        SceneNodeMap::iterator itr;
        itr = m_SceneNodes.find( cName );
        if( itr != m_SceneNodes.end() ) return itr->second;
    }

    // Create a new scene node.
    FlyOctreeNode* pNode = new FlyOctreeNode( this );
    if( !pNode ) return NULL;

    if( cName == NULL )
    {
        sprintf( nodeName,"Node%d",FlySceneNode::s_nCurrentNodeCount );
        pNode->SetName( nodeName );
        m_SceneNodes.insert( SceneNodeMap::value_type(nodeName,pNode) );
    }
    else
    {
        pNode->SetName( cName );
        m_SceneNodes.insert( SceneNodeMap::value_type(cName,pNode) );
    }

    FlySceneNode::s_nCurrentNodeCount++;
    return pNode;
}


//------------------------------------------------------------------------
// Destroy the specified octree node.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the octree node.
//      bool:           Destroy all the children ?
//------------------------------------------------------------------------
void FlyOctreeSceneManager::DestroySceneNode( const char* cName,bool bDestroyChildren )
{
    FlyOctreeNode* pNode = static_cast<FlyOctreeNode*>(GetSceneNode(cName));

    if( pNode )
        RemoveOctreeNode( pNode );

    FlySceneManager::DestroySceneNode( cName,bDestroyChildren );
}


//------------------------------------------------------------------------
// Destroy a specified scene node.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlySceneNode*:  Pointer to the scene node.
//      bool:           Destroy all the children ?
//------------------------------------------------------------------------
void FlyOctreeSceneManager::DestroySceneNode( FlySceneNode* pNode,bool bDestroyChildren )
{
    FlyOctreeNode* pOctNode = static_cast<FlyOctreeNode*>(pNode);

    if( pOctNode )
        RemoveOctreeNode( pOctNode );

    FlySceneManager::DestroySceneNode( pNode,bDestroyChildren );
}


//------------------------------------------------------------------------
// Update the scene node for the octree.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyOctreeNode*:     Pointer to the octree node.
//------------------------------------------------------------------------
void FlyOctreeSceneManager::UpdateOctreeNode( FlyOctreeNode* pNode )
{
    if( pNode->GetOctree() == NULL )
    {
        if( !pNode->IsBBoxIn(m_pOctreeRoot->m_BBox) )
            m_pOctreeRoot->AddSceneNode( pNode );
        else
            AddOctreeNode( pNode,m_pOctreeRoot );
    }
    else if( !pNode->IsBBoxIn(pNode->GetOctree()->m_BBox) )
    {
        RemoveOctreeNode( pNode );

        if( !pNode->IsBBoxIn(m_pOctreeRoot->m_BBox) )
            m_pOctreeRoot->AddSceneNode( pNode );
        else
            AddOctreeNode( pNode,m_pOctreeRoot );
    }
}


//------------------------------------------------------------------------
// Remove the specified scene node from the octree.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyOctreeNode*:     Pointer to the scene node.
//------------------------------------------------------------------------
void FlyOctreeSceneManager::RemoveOctreeNode( FlyOctreeNode* pNode )
{
    FlyOctree* pOctree = pNode->GetOctree();

    if( pOctree )
        pOctree->RemoveSceneNode( pNode );

    pNode->SetOctree( NULL );
}


//------------------------------------------------------------------------
// Add a scene node to the octree.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyOctreeNode*:     Pointer to the scene node.
//      FlyOctree*:         Pointer to the octreee to attach the node.
//      int:                Current depth value.
//------------------------------------------------------------------------
void FlyOctreeSceneManager::AddOctreeNode( FlyOctreeNode* pNode,FlyOctree* pOctree,
                                           int nDepth/* =0 */ )
{
    const FlyAabb& box = pNode->GetWorldBoundingBox();

    // If the octree is twice as big as the scene node,
    // We should add it to the child node.
    if( (nDepth < m_nMaxDepth) && pOctree->IsTwiceSize(box) )
    {
        int x,y,z;
        pOctree->GetChildIndices( box,&x,&y,&z );

        if( !pOctree->m_pChildren[x][y][z] )
        {
            FlyVector vMin,vMax;

            // Create the new child node.
            pOctree->m_pChildren[x][y][z] = new FlyOctree( pOctree );

            FlyVector* pOctMin = &pOctree->m_BBox.vcMin;
            FlyVector* pOctMax = &pOctree->m_BBox.vcMax;

            if( x == 0 )
            {
                vMin.x = pOctMin->x;
                vMax.x = (pOctMin->x + pOctMax->x) * 0.5f;
            }
            else
            {
                vMin.x = (pOctMin->x + pOctMax->x) * 0.5f;
                vMax.x = pOctMax->x;
            }

            if( y == 0 )
            {
                vMin.y = pOctMin->y;
                vMax.y = (pOctMin->y + pOctMax->y) * 0.5f;
            }
            else
            {
                vMin.y = (pOctMin->y + pOctMax->y) * 0.5f;
                vMax.y = pOctMax->y;
            }

            if( z == 0 )
            {
                vMin.z = pOctMin->z;
                vMax.z = (pOctMin->z + pOctMax->z) * 0.5f;
            }
            else
            {
                vMin.z = (pOctMin->z + pOctMax->z) * 0.5f;
                vMax.z = pOctMax->z;
            }

            pOctree->m_pChildren[x][y][z]->m_BBox.vcMin = vMin;
            pOctree->m_pChildren[x][y][z]->m_BBox.vcMax = vMax;
            pOctree->m_pChildren[x][y][z]->m_vHalfSize = (vMax - vMin) * 0.5f;
        }

        AddOctreeNode( pNode,pOctree->m_pChildren[x][y][z],++nDepth );
    }
    else
    {
        pOctree->AddSceneNode( pNode );
    }
}


//------------------------------------------------------------------------
// Render the visible scene objects for the sceneManager.
//------------------------------------------------------------------------
void FlyOctreeSceneManager::RenderVisibleObjects(void)
{


    // Find visible objects from the root node.
    ProcessNode( m_pOctreeRoot,false );
}


//------------------------------------------------------------------------
// Find the visible scene nodes at the current octree node.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyOctree*:         Pointer to the octree node.
//      bool:               We have found that current node is visible.
//------------------------------------------------------------------------
void FlyOctreeSceneManager::ProcessNode( FlyOctree* octree,bool bFoundVisible )
{
    if( octree->m_nNumSceneNodes == 0 )
        return;

    Visibility vsb = VSB_NONE;
    if( bFoundVisible )
        vsb = VSB_FULL;
    else if( octree == m_pOctreeRoot )
        vsb = VSB_PARTIAL;
    else
    {
        FlyAabb box;
        octree->GetCullBBox( &box );
        vsb = GetVisibility( box );
    }

    if( vsb != VSB_NONE )
    {
        bool bVisible = true;

        OctreeNodeList::iterator itr,end;
        end = octree->m_SceneNodes.end();
        for( itr=octree->m_SceneNodes.begin();itr!=end;itr++ )
        {
            FlyOctreeNode* nd = *itr;

            if( vsb == VSB_PARTIAL )
                bVisible = m_Frustum.IsBBoxVisible( nd->GetWorldBoundingBox() );

            if( bVisible && nd->IsVisible() )
                nd->AddObjectsToRenderQueue();
        }
    }

    FlyOctree* child;
    bool bFoundChildVisible = (vsb == VSB_FULL);
    if( (child=octree->m_pChildren[0][0][0]) != NULL )
        ProcessNode( child,bFoundChildVisible );
    if( (child=octree->m_pChildren[1][0][0]) != NULL )
        ProcessNode( child,bFoundChildVisible );
    if( (child=octree->m_pChildren[0][1][0]) != NULL )
        ProcessNode( child,bFoundChildVisible );
    if( (child=octree->m_pChildren[1][1][0]) != NULL )
        ProcessNode( child,bFoundChildVisible );
    if( (child=octree->m_pChildren[0][0][1]) != NULL )
        ProcessNode( child,bFoundChildVisible );
    if( (child=octree->m_pChildren[1][0][1]) != NULL )
        ProcessNode( child,bFoundChildVisible );
    if( (child=octree->m_pChildren[0][1][1]) != NULL )
        ProcessNode( child,bFoundChildVisible );
    if( (child=octree->m_pChildren[1][1][1]) != NULL )
        ProcessNode( child,bFoundChildVisible );
}


//------------------------------------------------------------------------
// Get the visibility of the specified boundingBox.
//------------------------------------------------------------------------
FlyOctreeSceneManager::Visibility FlyOctreeSceneManager::GetVisibility( const FlyAabb& bbox )
{
    bool bAllInside = true;
    for( int p=0;p<6;p++ )
    {
        const FlyPlane& plane = m_Frustum.GetFrustumPlane( (FlyFrustum::FrustumPlane)p );

        int nSide = plane.Classify( bbox );
        if( nSide == FLY_BACK ) return VSB_NONE;
        if( nSide == FLY_CLIPPED ) bAllInside = false;
    }

    if( bAllInside )
        return VSB_FULL;
    else
        return VSB_PARTIAL;
}


//------------------------------------------------------------------------
// Get the sceneNodes which is intersected by the boundingBox.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyAabb&:     BoundingBox to intersect.
//      std::list<FlySceneNode*>&:  To store the scene nodes.
//------------------------------------------------------------------------
void FlyOctreeSceneManager::FindInsectedNodes( const FlyAabb& bbox,std::list<FlySceneNode*>& nodeList )
{
    FindNodes( bbox,nodeList,false,m_pOctreeRoot );
}


//------------------------------------------------------------------------
// Get the sceneNodes which is intersected by the ray.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyRay&:      Ray to intersect.
//      std::list<FlySceneNode*>&:  To store the sceneNodes.
//------------------------------------------------------------------------
void FlyOctreeSceneManager::FindInsectedNodes( const FlyRay& ray,std::list<FlySceneNode*>& nodeList )
{
    FindNodes( ray,nodeList,m_pOctreeRoot );
}


//------------------------------------------------------------------------
// Get the sceneNodes which is intersected by the sphere.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlySphere&:   Sphere to intersect.
//      std::list<FlySceneNode*>&:  To store the sceneNodes.
//------------------------------------------------------------------------
void FlyOctreeSceneManager::FindInsectedNodes( const FlySphere& sphere,std::list<FlySceneNode*>& nodeList )
{
    FindNodes( sphere,nodeList,false,m_pOctreeRoot );
}


//------------------------------------------------------------------------
// Get the sceneNodes which is intersected by the boundingBox.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyAabb&:     BoundingBox to intersect with the nodes.
//      std::list<FlySceneNode*>&:  To store the scene nodes.
//      bool:               Is the current Oct node fully in the box ?
//      FlyOctree*:         Pointer to the current node.
//------------------------------------------------------------------------
void FlyOctreeSceneManager::FindNodes( const FlyAabb& bbox,std::list<FlySceneNode*>& nodeList,
                                       bool bFull,FlyOctree* pOctree )
{
    if( !bFull )
    {
        FlyAabb octBox;
        pOctree->GetCullBBox( &octBox );

        int intersect = bbox.ContainBox( octBox );
        if( intersect == FLY_OUTSIDE )
            return;

        bFull = (intersect == FLY_INSIDE);
    }

    OctreeNodeList::iterator itr,end;
    end = pOctree->m_SceneNodes.end();
    for( itr=pOctree->m_SceneNodes.begin();itr!=end;itr++ )
    {
        FlyOctreeNode* nd = *itr;

        if( bFull )
            nodeList.push_back( nd );
        else
        {
            int intersect = bbox.ContainBox( nd->GetWorldBoundingBox() );

            if( intersect != FLY_OUTSIDE )
                nodeList.push_back( nd );
        }
    }

    FlyOctree* child;
    if( (child=pOctree->m_pChildren[0][0][0]) != NULL )
        FindNodes( bbox,nodeList,bFull,child );
    if( (child=pOctree->m_pChildren[1][0][0]) != NULL )
        FindNodes( bbox,nodeList,bFull,child );
    if( (child=pOctree->m_pChildren[0][1][0]) != NULL )
        FindNodes( bbox,nodeList,bFull,child );
    if( (child=pOctree->m_pChildren[1][1][0]) != NULL )
        FindNodes( bbox,nodeList,bFull,child );
    if( (child=pOctree->m_pChildren[0][0][1]) != NULL )
        FindNodes( bbox,nodeList,bFull,child );
    if( (child=pOctree->m_pChildren[1][0][1]) != NULL )
        FindNodes( bbox,nodeList,bFull,child );
    if( (child=pOctree->m_pChildren[0][1][1]) != NULL )
        FindNodes( bbox,nodeList,bFull,child );
    if( (child=pOctree->m_pChildren[1][1][1]) != NULL )
        FindNodes( bbox,nodeList,bFull,child );
}


//------------------------------------------------------------------------
// Get the sceneNodes which is intersected by the sphere.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlySphere&:   Bounding sphere to intersect with the nodes.
//      std::list<FlySceneNode*>&:  To store the scene nodes.
//      bool:               Is the current oct node fully enveloped ?
//      FlyOctree*:         Pointer to the current oct node.
//------------------------------------------------------------------------
void FlyOctreeSceneManager::FindNodes( const FlySphere& sphere,std::list<FlySceneNode*>& nodeList,
                                       bool bFull,FlyOctree* pOctree )
{
    if( !bFull )
    {
        FlyAabb octBox;
        pOctree->GetCullBBox( &octBox );

        int intersect = sphere.ContainBox( octBox );
        if( intersect == FLY_OUTSIDE )
            return;

        bFull = (intersect == FLY_INSIDE);
    }

    OctreeNodeList::iterator itr,end;
    end = pOctree->m_SceneNodes.end();
    for( itr=pOctree->m_SceneNodes.begin();itr!=end;itr++ )
    {
        FlyOctreeNode* nd = *itr;

        if( bFull )
            nodeList.push_back( nd );
        else
        {
            int intersect = sphere.ContainBox( nd->GetWorldBoundingBox() );

            if( intersect != FLY_OUTSIDE )
                nodeList.push_back( nd );
        }
    }

    FlyOctree* child;
    if( (child=pOctree->m_pChildren[0][0][0]) != NULL )
        FindNodes( sphere,nodeList,bFull,child );
    if( (child=pOctree->m_pChildren[1][0][0]) != NULL )
        FindNodes( sphere,nodeList,bFull,child );
    if( (child=pOctree->m_pChildren[0][1][0]) != NULL )
        FindNodes( sphere,nodeList,bFull,child );
    if( (child=pOctree->m_pChildren[1][1][0]) != NULL )
        FindNodes( sphere,nodeList,bFull,child );
    if( (child=pOctree->m_pChildren[0][0][1]) != NULL )
        FindNodes( sphere,nodeList,bFull,child );
    if( (child=pOctree->m_pChildren[1][0][1]) != NULL )
        FindNodes( sphere,nodeList,bFull,child );
    if( (child=pOctree->m_pChildren[0][1][1]) != NULL )
        FindNodes( sphere,nodeList,bFull,child );
    if( (child=pOctree->m_pChildren[1][1][1]) != NULL )
        FindNodes( sphere,nodeList,bFull,child );
}


//------------------------------------------------------------------------
// Get the sceneNodes which is intersected by the ray.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyRay&:      Ray to intersect with the nodes.
//      std::list<FlySceneNode*>&:  To store the scene nodes.
//      bool:               Is the current oct node fully enveloped ?
//      FlyOctree*:         Pointer to the current oct node.
//------------------------------------------------------------------------
void FlyOctreeSceneManager::FindNodes( const FlyRay& ray,std::list<FlySceneNode*>& nodeList,
                                       FlyOctree* pOctree )
{
    FlyAabb octBox;
    pOctree->GetCullBBox( &octBox );

    if( !ray.Intersects(octBox,NULL) )
        return;

    OctreeNodeList::iterator itr,end;
    end = pOctree->m_SceneNodes.end();
    for( itr=pOctree->m_SceneNodes.begin();itr!=end;itr++ )
    {
        FlyOctreeNode* nd = *itr;

        if( ray.Intersects(nd->GetWorldBoundingBox(),NULL) )
            nodeList.push_back( nd );
    }

    FlyOctree* child;
    if( (child=pOctree->m_pChildren[0][0][0]) != NULL )
        FindNodes( ray,nodeList,child );
    if( (child=pOctree->m_pChildren[1][0][0]) != NULL )
        FindNodes( ray,nodeList,child );
    if( (child=pOctree->m_pChildren[0][1][0]) != NULL )
        FindNodes( ray,nodeList,child );
    if( (child=pOctree->m_pChildren[1][1][0]) != NULL )
        FindNodes( ray,nodeList,child );
    if( (child=pOctree->m_pChildren[0][0][1]) != NULL )
        FindNodes( ray,nodeList,child );
    if( (child=pOctree->m_pChildren[1][0][1]) != NULL )
        FindNodes( ray,nodeList,child );
    if( (child=pOctree->m_pChildren[0][1][1]) != NULL )
        FindNodes( ray,nodeList,child );
    if( (child=pOctree->m_pChildren[1][1][1]) != NULL )
        FindNodes( ray,nodeList,child );
}


//------------------------------------------------------------------------
// Resize the octree for the sceneManager.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyAabb&:     New boundingBox for the octree.
//------------------------------------------------------------------------
void FlyOctreeSceneManager::ResizeOctree( const FlyAabb& bbox )
{
    std::list<FlySceneNode*> nodes;
    std::list<FlySceneNode*>::iterator itr;

    FindNodes( m_pOctreeRoot->m_BBox,nodes,false,m_pOctreeRoot );

    // Delete the octree.
    SAFE_DELETE( m_pOctreeRoot );

    m_pOctreeRoot = new FlyOctree( NULL );
    m_pOctreeRoot->m_BBox = bbox;
    m_pOctreeRoot->m_vHalfSize = bbox.GetHalfSize();

    for( itr=nodes.begin();itr!=nodes.end();itr++ )
    {
        FlyOctreeNode* nd = static_cast<FlyOctreeNode*>(*itr);
        nd->SetOctree( NULL );
        UpdateOctreeNode( nd );
    }
}


//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// Create a new octreeSceneManager instance.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the sceneManager instance.
//------------------------------------------------------------------------
FlySceneManager* FlyOctreeSceneFactory::CreateInstance( const char* cName )
{
    FlyOctreeSceneManager* pManager = new FlyOctreeSceneManager();

    pManager->SetCreator( this );
    pManager->SetName( cName );
    return pManager;
}


//------------------------------------------------------------------------
// Destroy a specified sceneManager instance.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlySceneManager*:   Pointer to the sceneManager.
//------------------------------------------------------------------------
void FlyOctreeSceneFactory::DestroyInstance( FlySceneManager* pManager )
{
    SAFE_DELETE( pManager );
}