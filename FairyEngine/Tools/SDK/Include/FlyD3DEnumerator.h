//------------------------------------------------------------------------
// Name: FlyD3DEnumerator.h
// Desc: This file define a class to enumerate the video driver list for
//       the engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_D3DENUMERATOR_H__
#define __FLY_D3DENUMERATOR_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyD3DAdapter.h"

#include "stl_headers.h"
//////////////////////////////////////////////////////////////////////////

class FlyD3DEnumerator
{
    typedef std::vector<FlyD3DAdapter*> AdapterList;

protected:
    LPDIRECT3D9 m_pD3D;         // Pointer to the Direct3D object.
    AdapterList m_Adapters;     // Video Adapter card list.

public:
    // Constructor and destructor.
    FlyD3DEnumerator(void);
    ~FlyD3DEnumerator(void);

    HRESULT EnumAdapters(void);
    void DeleteAllAdapters(void);

    UINT GetAdapterCount(void);
    FlyD3DAdapter* GetAdapterPtr( int nIndex );

    LPDIRECT3D9 GetD3DPtr(void) { return m_pD3D; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_D3DENUMERATOR_H__