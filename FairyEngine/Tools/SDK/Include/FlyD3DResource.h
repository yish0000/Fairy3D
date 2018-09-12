//------------------------------------------------------------------------
// Name: FlyD3DResource.h
// Desc: This file define an interface for all the D3DResource classes.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_D3DRESOURCE_H__
#define __FLY_D3DRESOURCE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


/*
* Every Direct3D resource classes must inherit from this interface.
* So when the device is lost, We can move the Direct3D resource to
* the temp buffer.
*/
class FlyD3DResource
{
protected:
    BYTE* m_pTempBuffer;    // Pointer to the temp buffer to store resource data.

public:
    // Constructor and destructor.
    FlyD3DResource(void);
    virtual ~FlyD3DResource(void);

    // Add me to the D3D resource list.
    virtual void AddToResourceList(void);

    // Remove me from the D3D resource list.
    virtual void RemoveFromResourceList(void);

    // Call me when lost the device.
    virtual HRESULT OnDeviceLost(void);

    // Call me when reset the device.
    virtual HRESULT OnDeviceReset(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_D3DRESOURCE_H__