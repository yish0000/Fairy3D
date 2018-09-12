//------------------------------------------------------------------------
// Name: FlyCameraBase.h
// Desc: This file is to define a base class for the camera, Users can
//       make their child class base at it.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_CAMERABASE_H__
#define __FLY_CAMERABASE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


class FlyCamFactory;

class _FLY_EXPORT FlyCameraBase
{
protected:
    std::string m_sName;            // Name of the camera.
    FlyCamFactory* m_pCreator;      // Creator of the camera.

    FlyVector   m_vPos;             // Position of the camera.
    FlyQuat     m_qOrientation;     // Rotation data of the camera.
    FlyVector   m_vRight;           // X axis.
    FlyVector   m_vUp;              // Y axis.
    FlyVector   m_vDir;             // Z axis.

    bool        m_bYawFixed;        // Fix the yaw-axis ?
    FlyVector   m_vYawAxis;         // Yaw the object around the axis.

    mutable bool m_bNeedUpdate;     // Need to update the billboard matrix.

public:
    // Constructor and destructor.
    FlyCameraBase(void);
    ~FlyCameraBase(void);

    virtual void Update(void) { /* Nothing to do! */ };

    // Set the position for the camera.
    void SetPosition( float fX,float fY,float fZ );
    void SetPosition( const FlyVector& vPos );

    // Move the camera along the world axis.
    void Move( const FlyVector& vec );
    void MoveX( float fX );
    void MoveY( float fY );
    void MoveZ( float fZ );

    // Move the camera along its own axis.
    void MoveRelative( const FlyVector& vec );
    void MoveRelativeX( float fX );
    void MoveRelativeY( float fY );
    void MoveRelativeZ( float fZ );

    // Get the position of the camera.
    const FlyVector& GetPosition(void) const { return m_vPos; }

    // Get the direction of the camera
    const FlyVector& GetDirection(void) const { return m_vDir; }

    // Get the up vector of the camera.
    const FlyVector& GetUp(void) const { return m_vUp; }

    // Get the right vector of the camera.
    const FlyVector& GetRight(void) const { return m_vRight; }

    // Get the rotation data.
    const FlyQuat& GetRotation(void) const { return m_qOrientation; }

    // Get the billboard matrix.
    const FlyMatrix& GetBillboardMatrix( bool bAxisY=false ) const;

    // Make the camera with its lookAt point.
    void LookAt( const FlyVector& vAt );
    void LookAt( float fX,float fY,float fZ );

    // Rotate the camera around its own X axis.
    void Pitch( float fAngle );

    // Rotate the camera around its own Y axis.
    void Yaw( float fAngle );

    // Rotate the camera around its own Z axis.
    void Roll( float fAngle );

    // Rotate the camera around the arbitrary axis.
    void Rotate( const FlyVector& vAxis,float fAngle );

    // Rotate the camera around the arbitrary axis with the quaternion.
    void Rotate( const FlyQuat& quat );

    // Set the rotation value.
    void SetRotation( const FlyVector& vAxis,float fAngle );
    void SetRotation( const FlyQuat& quat );

    // Use a fixed yaw axis for our camera,
    // This is good when you are implementing a
    // first person camera.
    void SetYawFixed( bool bFixed,const FlyVector& vYawAxis=FlyVector(0.0f,1.0f,0.0f) );

    // Get and set the name.
    const char* GetName(void) const;
    void SetName( const char* cName );

    // Get the typename of the camera object.
    const char* GetObjectType(void) const;

    FlyCamFactory* GetCreator(void);
    void SetCreator( FlyCamFactory* pCreator );
};

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlyCamFactory
{
protected:
    std::string m_sTypename;

public:
    // Constructor and destructor.
    FlyCamFactory(void);
    virtual ~FlyCamFactory(void);

    virtual FlyCameraBase* CreateInstance( const char* cName );
    virtual void DestroyInstance( FlyCameraBase* pCamera );

    const char* GetTypename(void);
    void SetTypename( const char* cTypename );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_CAMERABASE_H__
