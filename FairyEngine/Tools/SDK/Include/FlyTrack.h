//------------------------------------------------------------------------
// Name: FlyTrack.h
// Desc: This file define a class to store the track data.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_TRACK_H__
#define __FLY_TRACK_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


/*
* This class is used to describe the track of an movable object.
*/
class _FLY_EXPORT FlyTrack
{
protected:
    std::vector<FlyVector> m_Verts;
    float m_fWholeTime;

public:
    // Constructor and destructor.
    FlyTrack(void);
    ~FlyTrack(void);

    // Load the track data from file.
    HRESULT LoadFromFile( const char* cFilename );

    void AddVertex( const FlyVector& vert );
    void Clear(void);
    void SetWholeTime( float fWholeTime );
    FlyVector GetCurPos( float fTime );

    UINT GetNumVerts(void) const;
    const FlyVector& GetVertex( UINT nIndex ) const;
    float GetWholeTime(void) const;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_TRACK_H__