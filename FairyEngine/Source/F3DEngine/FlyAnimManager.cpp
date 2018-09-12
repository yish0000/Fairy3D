//------------------------------------------------------------------------
// Name: FlyAnimManager.cpp
// Desc: This file is to implement the class FlyAnimManager.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyAnimManager.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyAnimManager::FlyAnimManager(void)
{
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyAnimManager::~FlyAnimManager(void)
{
    AnimationList::iterator itr;
    for( itr=m_Animations.begin();itr!=m_Animations.end();itr++ )
        SAFE_DELETE( itr->pAnim );
}


//------------------------------------------------------------------------
// Destroy all the animation resources.
//------------------------------------------------------------------------
void FlyAnimManager::DestroyAllAnimations(void)
{
    AnimationList::iterator itr;
    for( itr=m_Animations.begin();itr!=m_Animations.end();itr++ )
        SAFE_DELETE( itr->pAnim );

    m_Animations.clear();
}


//------------------------------------------------------------------------
// Load an animation from the file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the animation file.
//------------------------------------------------------------------------
FlyAnimation* FlyAnimManager::LoadAnimation( const char* cFilename )
{
    AnimationNode node;
    FlyAnimation* pAnimation;

    // Check whether the animation is loaded ?
    if( (pAnimation = GetAnimationByName(cFilename)) )
        return pAnimation;

    // Create a new animation.
    pAnimation = new FlyAnimation();
    if( !pAnimation ) return NULL;

    // Load the animation file.
    if( FAILED(pAnimation->LoadAnimation(cFilename)) )
    {
        delete pAnimation;
        return NULL;
    }

    // Add the new animation resource node.
    node.pAnim = pAnimation;
    node.nReference = 0;
    m_Animations.push_back( node );

    return pAnimation;
}


//------------------------------------------------------------------------
// Create an empty animation manually.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*     Name of the empty animation.
//------------------------------------------------------------------------
FlyAnimation* FlyAnimManager::CreateManualAnimation( const char* cName )
{
    AnimationNode node;
    FlyAnimation* pAnimation;

    // Check whether the animation has existed ?
    if( (pAnimation = GetAnimationByName(cName)) )
        return pAnimation;

    // Create a new animation.
    pAnimation = new FlyAnimation();
    if( !pAnimation ) return NULL;
    pAnimation->SetName( cName );

    // Add the new animation resource node.
    node.pAnim = pAnimation;
    node.nReference = 0;
    m_Animations.push_back( node );

    return pAnimation;
}


//------------------------------------------------------------------------
// Get the pointer to the animation by its index.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:        Index of the animation resource.
//------------------------------------------------------------------------
FlyAnimation* FlyAnimManager::GetAnimationByIndex( UINT nIndex )
{
    if( nIndex >= m_Animations.size() )
        return NULL;

    return m_Animations[nIndex].pAnim;
}


//------------------------------------------------------------------------
// Get the pointer to the animation by its name.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the animation.
//------------------------------------------------------------------------
FlyAnimation* FlyAnimManager::GetAnimationByName( const char* cName )
{
    AnimationList::iterator itr;
    for( itr=m_Animations.begin();itr!=m_Animations.end();itr++ )
    {
        if( strcmp(itr->pAnim->GetName(),cName) == 0 )
            return itr->pAnim;
    }

    return NULL;
}


//------------------------------------------------------------------------
// Get the count of the animation resources.
//------------------------------------------------------------------------
UINT FlyAnimManager::GetNumAnimations(void) const
{
    return (UINT)m_Animations.size();
}


//------------------------------------------------------------------------
// Add the reference count for the animation resource.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:        Name of the animation.
//------------------------------------------------------------------------
void FlyAnimManager::AddResourceRef( const char* cAnimName )
{
    AnimationList::iterator itr;
    for( itr=m_Animations.begin();itr!=m_Animations.end();itr++ )
    {
        if( strcmp(itr->pAnim->GetName(),cAnimName) == 0 )
        {
            itr->nReference++;
            break;
        }
    }
}


//------------------------------------------------------------------------
// Dec the reference count for the animation resource.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the animation.
//------------------------------------------------------------------------
void FlyAnimManager::DecResourceRef( const char* cAnimName )
{
    AnimationList::iterator itr;
    for( itr=m_Animations.begin();itr!=m_Animations.end();itr++ )
    {
        if( strcmp(itr->pAnim->GetName(),cAnimName) == 0 )
        {
            itr->nReference--;

            if( itr->nReference <= 0 )
            {
                SAFE_DELETE( itr->pAnim );
                m_Animations.erase( itr );
            }

            break;
        }
    }
}


//------------------------------------------------------------------------
// Destroy the animation resource by its index.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:    Index of the animation resource.
//------------------------------------------------------------------------
void FlyAnimManager::DestroyAnimationByIndex( UINT nIndex )
{
    if( nIndex >= m_Animations.size() )
        return;

    SAFE_DELETE( m_Animations[nIndex].pAnim );
    m_Animations.erase( m_Animations.begin() + nIndex );
}


//------------------------------------------------------------------------
// Destroy the animation resource by its name.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the animation.
//------------------------------------------------------------------------
void FlyAnimManager::DestroyAnimationByName( const char* cName )
{
    AnimationList::iterator itr;
    for( itr=m_Animations.begin();itr!=m_Animations.end();itr++ )
    {
        if( strcmp(itr->pAnim->GetName(),cName) == 0 )
        {
            SAFE_DELETE( itr->pAnim );
            m_Animations.erase( itr );
            break;
        }
    }
}


//------------------------------------------------------------------------
// Get the reference count of the animation.
//------------------------------------------------------------------------
int FlyAnimManager::GetRefCount( const char* cAnimName )
{
    AnimationList::iterator itr;
    for( itr=m_Animations.begin();itr!=m_Animations.end();itr++ )
    {
        if( strcmp(itr->pAnim->GetName(),cAnimName) == 0 )
            return itr->nReference;
    }

    return -1;
}