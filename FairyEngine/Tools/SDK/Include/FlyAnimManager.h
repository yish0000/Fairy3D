//------------------------------------------------------------------------
// Name: FlyAnimManager.h
// Desc: This file define a class to manage the skeleton animation 
//       resources for engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_ANIM_MANAGER_H__
#define __FLY_ANIM_MANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyAnimation.h"

//////////////////////////////////////////////////////////////////////////


/*
* This class is to manage the skeleton animation resources.
*/
class _FLY_EXPORT FlyAnimManager
{
    struct AnimationNode
    {
        FlyAnimation* pAnim;    // Pointer to the animation.
        int nReference;         // Reference count of the resource.
    };

    typedef std::vector<AnimationNode> AnimationList;

protected:
    AnimationList m_Animations;

public:
    // Constructor and destructor.
    FlyAnimManager(void);
    ~FlyAnimManager(void);

    // Reference count of the animation resource.
    void AddResourceRef( const char* cAnimName );
    void DecResourceRef( const char* cAnimName );

    // Load an animation from the file.
    FlyAnimation* LoadAnimation( const char* cFilename );

    // Create an empty animation manually.
    FlyAnimation* CreateManualAnimation( const char* cName );

    // Get the specified animation resource.
    FlyAnimation* GetAnimationByIndex( UINT nIndex );
    FlyAnimation* GetAnimationByName( const char* cName );

    // Get the count of the animations.
    UINT GetNumAnimations(void) const;

    // Get the animation's ref count.
    int GetRefCount( const char* cAnimName );

    // Destroy an animation resource.
    void DestroyAnimationByIndex( UINT nIndex );
    void DestroyAnimationByName( const char* cName );

    // Destroy all animation resources.
    void DestroyAllAnimations(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_ANIM_MANAGER_H__