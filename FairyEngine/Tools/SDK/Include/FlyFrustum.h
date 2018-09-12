//------------------------------------------------------------------------
// Name: FlyFrustum.h
// Desc: This file define a class which is used to cull the unvisible
//       geometrys.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_FRUSTUM_H__
#define __FLY_FRUSTUM_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


/*
* This class is used to cull unvisible geometrys.
* Used by the sceneManager.
*/
class _FLY_EXPORT FlyFrustum
{
public:
    enum FrustumPlane
    {
        FRUSTUM_PLANE_LEFT,
        FRUSTUM_PLANE_RIGHT,
        FRUSTUM_PLANE_TOP,
        FRUSTUM_PLANE_BOTTOM,
        FRUSTUM_PLANE_NEAR,
        FRUSTUM_PLANE_FAR,
    };

protected:
    FlyPlane m_Planes[6];
    float m_fFarDistance;
    float m_fNearDistance;

public:
    // Constructor and destructor.
    FlyFrustum(void);
    ~FlyFrustum(void);

    bool IsPointVisible( const FlyVector& vPoint ) const;
    bool IsSphereVisible( const FlySphere& sphere ) const;
    bool IsBBoxVisible( const FlyAabb& bbox ) const;

    float GetNearDistance(void) const;
    float GetFarDistance(void) const;
    void SetNearDistance( float fDistance );
    void SetFarDistance( float fDistance );

    const FlyPlane& GetFrustumPlane( FrustumPlane plane ) const;

    // Update the view frustum.
    void UpdateFrustum(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_FRUSTUM_H__