//------------------------------------------------------------------------
// Name: etBrush.cpp
// Desc: This file is to implement the class etBrush.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etBrush.h"

#include "etTerrain.h"
#include "etCoreManager.h"
#include "etSceneManager.h"
#include "etTile.h"

#include "etFloatRect.h"


const float fDelta = FLY_PI / 12.0f;

//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etBrush::etBrush(void)
{
    m_pTerrain = NULL;

    m_BrushType = TT_RAISE;
    m_BrushShape = TS_CIRCLE;

    m_fX = 0.0f;
    m_fZ = 0.0f;
    m_fInner = 1.0f;
    m_fOuter = 1.0f;
    m_fStrength = 1.0f;
    m_nCurLayer = 0;

    m_bBrushChanged = true;

    // Create the renderable object.
    m_pRenderable = new etBrushRenderable( this );
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etBrush::~etBrush(void)
{
    SAFE_DELETE( m_pRenderable );
}


//------------------------------------------------------------------------
// Attach a terrain to the brush.
// ----------------------------------------------------------------------
// Param -> IN:
//      etTerrain*:     Pointer to the terrain.
//------------------------------------------------------------------------
void etBrush::AttachTerrain( etTerrain* pTerrain )
{
    m_pTerrain = pTerrain;
}


//------------------------------------------------------------------------
// Function to modify the terrain, Use the rectangle shape brush.
//------------------------------------------------------------------------
void etBrush::ModifyTerrainRS(void)
{
    float fDelta;
    int   cx,cy;
    int   nIndex;
    float fStrength;
    float fAverH;

    UINT nSize = m_pTerrain->GetTerrainSize();

    // Scale the Brush position and size.
    float fX = m_fX / m_pTerrain->GetLengthPerUnit();
    float fY = m_fZ / m_pTerrain->GetLengthPerUnit();
    float fInner = m_fInner / m_pTerrain->GetLengthPerUnit();
    float fOuter = m_fOuter / m_pTerrain->GetLengthPerUnit();

    // Get the pointer to height map.
    float* pBuffer = m_pTerrain->GetHeightData();

    // Loop through each point for the height map.
    int nMinX = (int)(fX - fInner);
    int nMaxX = (int)(fX + fInner);
    int nMinY = (int)(fY - fInner);
    int nMaxY = (int)(fY + fInner);

    // Get the height at cursor position.
    fAverH = (float)m_pTerrain->GetHeightValue( (int)fX,(int)fY );
    float fValue = m_fStrength * etCoreManager::Instance().GetElapsed();

    float fMaxHeight = etCoreManager::Instance().GetConfigData()->GetMaxHeight();

    // Modify the terrain with Inner brush.
    for( int j=nMinY;j<=nMaxY;j++ )
    {
        for( int i=nMinX;i<=nMaxX;i++ )
        {
            if( i < 0 || i > nSize ||
                j < 0 || j > nSize )
            {
                continue;
            }

            nIndex = j*(nSize+1)+i;

            if( m_BrushType == TT_RAISE )
            {
                if( pBuffer[nIndex] + fValue > fMaxHeight )
                    pBuffer[nIndex] = fMaxHeight;
                else
                    pBuffer[nIndex] += fValue;
            }
            else if( m_BrushType == TT_LOWER )
            {
                if( pBuffer[nIndex] - fValue < 0.0f )
                    pBuffer[nIndex] = 0.0f;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_SMOOTH )
            {
                SmoothTerrain( i,j );
            }
            else if( m_BrushType == TT_DIG )
            {
                pBuffer[nIndex] = -m_fStrength;
            }
            else if( m_BrushType == TT_FLATTEN )
            {
                pBuffer[nIndex] = fAverH;
            }
        }
    }

    // Modify the terrain from outer to inner brush.
    // Linar increasing for the strength.
    fDelta = fOuter - fInner;

    nMinX = (int)(fX - fOuter);
    nMaxX = (int)(fX + fOuter);
    nMinY = (int)(fY - fOuter);
    nMaxY = (int)(fY + fOuter);

    for( int i=0;i<(int)fDelta;i++ )
    {
        // Get the strength
        fStrength = i / fDelta * m_fStrength;
        fValue = fStrength * etCoreManager::Instance().GetElapsed();

        for( cx=nMinX+i;cx<=nMaxX-i;cx++ )
        {
            if( cx < 0 || cx > nSize ||
                nMinY+i < 0 || nMinY+i > nSize )
            {
                continue;
            }

            // Up row.
            nIndex = (nMinY+i)*(nSize+1)+cx;
            if( m_BrushType == TT_RAISE )
            {
                if( pBuffer[nIndex] + fValue > fMaxHeight )
                    pBuffer[nIndex] = fMaxHeight;
                else
                    pBuffer[nIndex] += fValue;
            }
            else if( m_BrushType == TT_LOWER )
            {
                if( pBuffer[nIndex] - fValue < 0.0f )
                    pBuffer[nIndex] = 0.0f;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_DIG )
            {
                if( pBuffer[nIndex] <= -m_fStrength )
                    pBuffer[nIndex] = -m_fStrength;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_FLATTEN )
            {
                if( pBuffer[nIndex] > fAverH )
                    pBuffer[nIndex] -= ((pBuffer[nIndex]-fAverH)*(i+1)/fDelta);
                else if( pBuffer[nIndex] < fAverH )
                    pBuffer[nIndex] += ((fAverH-pBuffer[nIndex])*(i+1)/fDelta);
            }

            if( cx < 0 || cx > nSize ||
                nMaxY-i < 0 || nMaxY-i > nSize )
            {
                continue;
            }

            // Down row.
            nIndex = (nMaxY-i)*(nSize+1)+cx;
            if( m_BrushType == TT_RAISE )
            {
                if( pBuffer[nIndex] + fValue > fMaxHeight )
                    pBuffer[nIndex] = fMaxHeight;
                else
                    pBuffer[nIndex] += fValue;
            }
            else if( m_BrushType == TT_LOWER )
            {
                if( pBuffer[nIndex] - fValue < 0.0f )
                    pBuffer[nIndex] = 0.0f;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_DIG )
            {
                if( pBuffer[nIndex] <= -m_fStrength )
                    pBuffer[nIndex] = -m_fStrength;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_FLATTEN )
            {
                if( pBuffer[nIndex] > fAverH )
                    pBuffer[nIndex] -= (short)((pBuffer[nIndex]-fAverH)*(i+1)/fDelta);
                else if( pBuffer[nIndex] < fAverH )
                    pBuffer[nIndex] += (short)((fAverH-pBuffer[nIndex])*(i+1)/fDelta);
            }
        }

        for( cy=nMinY+i+1;cy<nMaxY-i;cy++ )
        {
            if( nMinX+i < 0 || nMinX+i > nSize ||
                cy < 0 || cy > nSize )
            {
                continue;
            }

            // Left column.
            nIndex = cy*(nSize+1)+nMinX+i;
            if( m_BrushType == TT_RAISE )
            {
                if( pBuffer[nIndex] + fValue > fMaxHeight )
                    pBuffer[nIndex] = fMaxHeight;
                else
                    pBuffer[nIndex] += fValue;
            }
            else if( m_BrushType == TT_LOWER )
            {
                if( pBuffer[nIndex] - fValue < 0.0f )
                    pBuffer[nIndex] = 0.0f;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_DIG )
            {
                if( pBuffer[nIndex] <= -m_fStrength )
                    pBuffer[nIndex] = -m_fStrength;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_FLATTEN )
            {
                if( pBuffer[nIndex] > fAverH )
                    pBuffer[nIndex] -= ((pBuffer[nIndex]-fAverH)*(i+1)/fDelta);
                else if( pBuffer[nIndex] < fAverH )
                    pBuffer[nIndex] += ((fAverH-pBuffer[nIndex])*(i+1)/fDelta);
            }

            if( nMaxX-i < 0 || nMaxX-i > nSize ||
                cy < 0 || cy > nSize )
            {
                continue;
            }

            // Down row.
            nIndex = cy*(nSize+1)+nMaxX-i;
            if( m_BrushType == TT_RAISE )
            {
                if( pBuffer[nIndex] + fValue > fMaxHeight )
                    pBuffer[nIndex] = fMaxHeight;
                else
                    pBuffer[nIndex] += fValue;
            }
            else if( m_BrushType == TT_LOWER )
            {
                if( pBuffer[nIndex] - fValue < 0.0f )
                    pBuffer[nIndex] = 0.0f;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_DIG )
            {
                if( pBuffer[nIndex] <= -m_fStrength )
                    pBuffer[nIndex] = -m_fStrength;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_FLATTEN )
            {
                if( pBuffer[nIndex] > fAverH )
                    pBuffer[nIndex] -= (pBuffer[nIndex]-fAverH)*(i+1)/fDelta;
                else if( pBuffer[nIndex] < fAverH )
                    pBuffer[nIndex] += (fAverH-pBuffer[nIndex])*(i+1)/fDelta;
            }
        }
    }
}


//------------------------------------------------------------------------
// Function to modify the terrain, Use the circle shape brush.
//------------------------------------------------------------------------
void etBrush::ModifyTerrainCS(void)
{
    int   nIndex;
    int   n = 0;     // For inner loop.
    float fAverH;
    float fDelta;
    int cx,cy,i,j;
    float fTmp,fTmp1,fTmp2,fStrength;

    UINT nSize = m_pTerrain->GetTerrainSize();

    // Scale the position and size of the brush.
    float fX = m_fX / m_pTerrain->GetLengthPerUnit();
    float fY = m_fZ / m_pTerrain->GetLengthPerUnit();
    float fInner = m_fInner / m_pTerrain->GetLengthPerUnit();
    float fOuter = m_fOuter / m_pTerrain->GetLengthPerUnit();

    // Get the pointer to height map.
    float* pBuffer = m_pTerrain->GetHeightData();

    int nMinX,nMaxX;
    int nMinY = (int)(fY - fInner);
    int nMaxY = (int)(fY + fInner);

    // Get the height at cursor position.
    fAverH = m_pTerrain->GetHeightValue( (int)fX,(int)fY );
    float fValue = m_fStrength * etCoreManager::Instance().GetElapsed();

    float fMaxHeight = etCoreManager::Instance().GetConfigData()->GetMaxHeight();

    for( j=nMinY;j<=nMaxY;j++ )
    {
        fTmp = sqrt( fInner*fInner-(int)(fY-j)*(int)(fY-j) );
        nMinX = (int)(fX - fTmp);
        nMaxX = (int)(fX + fTmp);

        for( i=nMinX;i<=nMaxX;i++ )
        {
            if( i < 0 || i > nSize ||
                j < 0 || j > nSize )
            {
                continue;
            }
            nIndex = j*(nSize+1)+i;

            if( m_BrushType == TT_RAISE )
            {
                if( pBuffer[nIndex] + fValue > fMaxHeight )
                    pBuffer[nIndex] = fMaxHeight;
                else
                    pBuffer[nIndex] += fValue;
            }
            else if( m_BrushType == TT_LOWER )
            {
                if( pBuffer[nIndex] - fValue < 0.0f )
                    pBuffer[nIndex] = 0.0f;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_SMOOTH )
            {
                SmoothTerrain( i,j );
            }
            else if( m_BrushType == TT_DIG )
            {
                pBuffer[nIndex] = -m_fStrength;
            }
            else if( m_BrushType == TT_FLATTEN )
            {
                pBuffer[nIndex] = fAverH;
            }
        }
    }

    // if have the same size, return the function.
    if( fInner == fOuter ) return;

    // Modify the terrain with inner to outer brush.
    // Linar increasing for the strength.
    fDelta = fOuter - fInner;

    nMinY = (int)(fY - fOuter);
    nMaxY = (int)(fY + fOuter);

    for( cy=nMinY;cy<(int)(fY-fInner);cy++ )
    {
        fTmp = sqrt( fOuter*fOuter-(int)(fY-cy)*(int)(fY-cy) );
        nMinX = (int)(fX - fTmp);
        nMaxX = (int)(fX + fTmp);

        for( cx=nMinX;cx<=nMaxX;cx++ )
        {
            if( cx < 0 || cx > nSize ||
                cy < 0 || cy > nSize )
            {
                continue;
            }

            fTmp = sqrt( (int)(fY-cy)*(int)(fY-cy)+(int)(fX-cx)*(int)(fX-cx) );
            fStrength = (fOuter-fTmp)/fDelta * m_fStrength;
            nIndex = cy*(nSize+1)+cx;
            fValue = fStrength * etCoreManager::Instance().GetElapsed();

            if( m_BrushType == TT_RAISE )
            {
                if( pBuffer[nIndex] + fValue > fMaxHeight )
                    pBuffer[nIndex] = fMaxHeight;
                else
                    pBuffer[nIndex] += fValue;
            }
            else if( m_BrushType == TT_LOWER )
            {
                if( pBuffer[nIndex] - fValue < 0.0f )
                    pBuffer[nIndex] = 0.0f;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_DIG )
            {
                if( pBuffer[nIndex] <= -m_fStrength )
                    pBuffer[nIndex] = -m_fStrength;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_FLATTEN )
            {
                if( pBuffer[nIndex] > fAverH )
                    pBuffer[nIndex] -= ((pBuffer[nIndex]-fAverH)*(fOuter-fTmp)/fDelta);
                else if( pBuffer[nIndex] < fAverH )
                    pBuffer[nIndex] += ((fAverH-pBuffer[nIndex])*(fOuter-fTmp)/fDelta);
            }
        }
    }

    for( cy=(int)(fY-fInner);cy<=(int)(fY+fInner);cy++ )
    {
        fTmp1 = sqrt( fOuter*fOuter-(int)(fY-cy)*(int)(fY-cy) );
        fTmp2 = sqrt( fInner*fInner-(int)(fY-cy)*(int)(fY-cy) );
        nMinX = (int)(fX - fTmp1);
        nMaxX = (int)(fX - fTmp2);

        for( cx=nMinX;cx<nMaxX;cx++ )
        {
            if( cx < 0 || cx > nSize ||
                cy < 0 || cy > nSize )
            {
                continue;
            }

            fTmp = sqrt( (int)(fY-cy)*(int)(fY-cy)+(int)(fX-cx)*(int)(fX-cx) );
            fStrength = (fOuter-fTmp)/fDelta * m_fStrength;
            nIndex = cy*(nSize+1)+cx;
            fValue = fStrength * etCoreManager::Instance().GetElapsed();

            if( m_BrushType == TT_RAISE )
            {
                if( pBuffer[nIndex] + fValue > fMaxHeight )
                    pBuffer[nIndex] = fMaxHeight;
                else
                    pBuffer[nIndex] += fValue;
            }
            else if( m_BrushType == TT_LOWER )
            {
                if( pBuffer[nIndex] - fValue < 0.0f )
                    pBuffer[nIndex] = 0.0f;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_DIG )
            {
                if( pBuffer[nIndex] <= -m_fStrength )
                    pBuffer[nIndex] = -m_fStrength;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_FLATTEN )
            {
                if( pBuffer[nIndex] > fAverH )
                    pBuffer[nIndex] -= ((pBuffer[nIndex]-fAverH)*(fOuter-fTmp)/fDelta);
                else if( pBuffer[nIndex] < fAverH )
                    pBuffer[nIndex] += ((fAverH-pBuffer[nIndex])*(fOuter-fTmp)/fDelta);
            }
        }

        nMinX = (int)(fX + fTmp2);
        nMaxX = (int)(fX + fTmp1);

        for( cx=nMinX+1;cx<=nMaxX;cx++ )
        {
            if( cx < 0 || cx > nSize ||
                cy < 0 || cy > nSize )
            {
                continue;
            }

            fTmp = sqrt( (int)(fY-cy)*(int)(fY-cy)+(int)(fX-cx)*(int)(fX-cx) );
            fStrength = (fOuter-fTmp)/fDelta * m_fStrength;
            nIndex = cy*(nSize+1)+cx;
            fValue = fStrength * etCoreManager::Instance().GetElapsed();

            if( m_BrushType == TT_RAISE )
            {
                if( pBuffer[nIndex] + fValue > fMaxHeight )
                    pBuffer[nIndex] = fMaxHeight;
                else
                    pBuffer[nIndex] += fValue;
            }
            else if( m_BrushType == TT_LOWER )
            {
                if( pBuffer[nIndex] - fValue < 0.0f )
                    pBuffer[nIndex] = 0.0f;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_DIG )
            {
                if( pBuffer[nIndex] <= -m_fStrength )
                    pBuffer[nIndex] = -m_fStrength;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_FLATTEN )
            {
                if( pBuffer[nIndex] > fAverH )
                    pBuffer[nIndex] -= ((pBuffer[nIndex]-fAverH)*(fOuter-fTmp)/fDelta);
                else if( pBuffer[nIndex] < fAverH )
                    pBuffer[nIndex] += ((fAverH-pBuffer[nIndex])*(fOuter-fTmp)/fDelta);
            }
        }
    }

    for( cy=(int)(fY+fInner)+1;cy<=nMaxY;cy++ )
    {
        fTmp = sqrt( fOuter*fOuter-(int)(fY-cy)*(int)(fY-cy) );
        nMinX = (int)(fX - fTmp);
        nMaxX = (int)(fX + fTmp);

        for( cx=nMinX;cx<=nMaxX;cx++ )
        {
            if( cx < 0 || cx > nSize ||
                cy < 0 || cy > nSize )
            {
                continue;
            }

            fTmp = sqrt( (int)(fY-cy)*(int)(fY-cy)+(int)(fX-cx)*(int)(fX-cx) );
            fStrength = (fOuter-fTmp)/fDelta * m_fStrength;
            nIndex = cy*(nSize+1)+cx;
            fValue = fStrength * etCoreManager::Instance().GetElapsed();

            if( m_BrushType == TT_RAISE )
            {
                if( pBuffer[nIndex] + fValue > fMaxHeight )
                    pBuffer[nIndex] = fMaxHeight;
                else
                    pBuffer[nIndex] += fValue;
            }
            else if( m_BrushType == TT_LOWER )
            {
                if( pBuffer[nIndex] - fValue < 0.0f )
                    pBuffer[nIndex] = 0.0f;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_DIG )
            {
                if( pBuffer[nIndex] <= -m_fStrength )
                    pBuffer[nIndex] = -m_fStrength;
                else
                    pBuffer[nIndex] -= fValue;
            }
            else if( m_BrushType == TT_FLATTEN )
            {
                if( pBuffer[nIndex] > fAverH )
                    pBuffer[nIndex] -= ((pBuffer[nIndex]-fAverH)*(fOuter-fTmp)/fDelta);
                else if( pBuffer[nIndex] < fAverH )
                    pBuffer[nIndex] += ((fAverH-pBuffer[nIndex])*(fOuter-fTmp)/fDelta);
            }
        }
    }
}


//------------------------------------------------------------------------
// Paint the terrain with a detail layer.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the detail layer.
//------------------------------------------------------------------------
void etBrush::PaintTerrainRS( UINT nDetailLayer )
{
    BYTE val;

    // Get the pointer to the alpha data.
    etDetailLayer* pLayer = m_pTerrain->GetDetailLayer();

    // Scale the position and size of the brush.
    float fX = m_fX / m_pTerrain->GetLengthPerUnit();
    float fY = m_fZ / m_pTerrain->GetLengthPerUnit();
    float fInner = m_fInner / m_pTerrain->GetLengthPerUnit();
    float fOuter = m_fOuter / m_pTerrain->GetLengthPerUnit();

    UINT nSize = pLayer->GetAlphamapSize();
    UINT nScale = m_pTerrain->GetTerrainSize() / nSize;

    // Loop through each point for the height map.
    int nMinX = (int)(fX - fInner) / nScale;
    int nMaxX = (int)(fX + fInner) / nScale;
    int nMinY = (int)(fY - fInner) / nScale;
    int nMaxY = (int)(fY + fInner) / nScale;

    // Modify the terrain with Inner brush.
    for( int j=nMinY;j<=nMaxY;j++ )
    {
        for( int i=nMinX;i<=nMaxX;i++ )
        {
            if( i < 0 || i > nSize ||
                j < 0 || j > nSize )
            {
                continue;
            }

            // Get the alpha value.
            val = pLayer->GetAlphaValue( i,j,nDetailLayer );

            if( m_BrushType == TT_PAINT )
            {
                if( val + (BYTE)m_fStrength > 255 )
                    val = 255;
                else
                    val += (BYTE)m_fStrength;

                pLayer->SetAlphaValue( i,j,nDetailLayer,val );
            }
            else if( m_BrushType == TT_ERASE )
            {
                if( val - (BYTE)m_fStrength < 0 )
                    val = 0;
                else
                    val -= (BYTE)m_fStrength;

                pLayer->SetAlphaValue( i,j,nDetailLayer,val );
            }
            else if( m_BrushType == TT_BLEND )
            {
                BlendTerrain( i,j,nDetailLayer );
            }
        }
    }

    pLayer->SetModified( true );
}


//------------------------------------------------------------------------
// Paint the terrain with the circle shape brush.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Which detail to paint with.
//------------------------------------------------------------------------
void etBrush::PaintTerrainCS( UINT nDetailLayer )
{
    BYTE val;
    int  n = 0;     // For inner loop.

    // Get the pointer to the alpha data.
    etDetailLayer* pLayer = m_pTerrain->GetDetailLayer();

    // Scale the position and size of the brush.
    float fX = m_fX / m_pTerrain->GetLengthPerUnit();
    float fY = m_fZ / m_pTerrain->GetLengthPerUnit();
    float fInner = m_fInner / m_pTerrain->GetLengthPerUnit();
    float fOuter = m_fOuter / m_pTerrain->GetLengthPerUnit();

    int nSize = pLayer->GetAlphamapSize();
    int nScale = m_pTerrain->GetTerrainSize() / nSize;

    int nMinY = (int)(fY - fInner) / nScale;
    int nMaxY = (int)(fY + fInner) / nScale;

    for( int j=nMinY;j<=nMaxY;j++ )
    {
        n = (int)sqrt(fInner*fInner-(fY-j*nScale)*(fY-j*nScale));

        for( int i=(fX-n)/nScale;i<=(fX+n)/nScale;i++ )
        {
            if( i < 0 || i > nSize ||
                j < 0 || j > nSize )
            {
                continue;
            }

            // Get the alpha value.
            val = pLayer->GetAlphaValue( i,j,nDetailLayer );

            if( m_BrushType == TT_PAINT )
            {
                if( val + (BYTE)m_fStrength > 255 )
                    val = 255;
                else
                    val += (BYTE)m_fStrength;

                pLayer->SetAlphaValue( i,j,nDetailLayer,val );
            }
            else if( m_BrushType == TT_ERASE )
            {
                if( val - (BYTE)m_fStrength < 0 )
                    val = 0;
                else
                    val -= (BYTE)m_fStrength;

                pLayer->SetAlphaValue( i,j,nDetailLayer,val );
            }
            else if( m_BrushType == TT_BLEND )
            {
                BlendTerrain( i,j,nDetailLayer );
            }
        }
    }

    pLayer->SetModified( true );
}


//------------------------------------------------------------------------
// Smooth the level for one point.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        X position of the terrain.
//      int:        Y position of the terrain.
//------------------------------------------------------------------------
void etBrush::SmoothTerrain( int x,int y )
{
    int lx,ly,ux,uy;
    float height = 0.0f;

    float* pBuffer = m_pTerrain->GetHeightData();
    UINT nSize = m_pTerrain->GetTerrainSize();

    lx = x - 1;
    if( lx < 0 ) lx = nSize;
    ly = y - 1;
    if( ly < 0 ) ly = nSize;
    ux = x + 1;
    if( ux > nSize ) ux = 0;
    uy = y + 1;
    if( uy > nSize ) uy = 0;

    height  = 4 * pBuffer[y*(nSize+1)+lx];
    height += 4 * pBuffer[y*(nSize+1)+ux];
    height += 4 * pBuffer[ly*(nSize+1)+x];
    height += 4 * pBuffer[uy*(nSize+1)+x];
    height += 2 * pBuffer[ly*(nSize+1)+lx];
    height += 2 * pBuffer[ly*(nSize+1)+ux];
    height += 2 * pBuffer[uy*(nSize+1)+lx];
    height += 2 * pBuffer[uy*(nSize+1)+ux];
    height += 6 * pBuffer[y*(nSize+1)+x];
    height /= 30.0f;

    m_pTerrain->SetHeightValue( x,y,height );
}


//------------------------------------------------------------------------
// Blend the detail layer with the other layers.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        X position of the point.
//      int:        Y position of the point.
//      int:        ID for the detail layer.
//------------------------------------------------------------------------
void etBrush::BlendTerrain( int x,int y,UINT nDetailID )
{
    int lx,ly,ux,uy;
    float val = 0.0f;

    UINT nSize = m_pTerrain->GetDetailLayer()->GetAlphamapSize();
    UCHAR* pBuffer = m_pTerrain->GetDetailLayer()->GetTempBuffer();

    lx = x - 1;
    if( lx < 0 ) lx = nSize - 1;
    ly = y - 1;
    if( ly < 0 ) ly = nSize - 1;
    ux = x + 1;
    if( ux >= nSize ) ux = 0;
    uy = y + 1;
    if( uy >= nSize ) uy = 0;

    val  = 4 * pBuffer[(y*nSize+lx)*4+nDetailID];
    val += 4 * pBuffer[(y*nSize+ux)*4+nDetailID];
    val += 4 * pBuffer[(ly*nSize+x)*4+nDetailID];
    val += 4 * pBuffer[(uy*nSize+x)*4+nDetailID];
    val += 2 * pBuffer[(ly*nSize+lx)*4+nDetailID];
    val += 2 * pBuffer[(ly*nSize+ux)*4+nDetailID];
    val += 2 * pBuffer[(uy*nSize+lx)*4+nDetailID];
    val += 2 * pBuffer[(uy*nSize+ux)*4+nDetailID];
    val += 6 * pBuffer[(y*nSize+x)*4+nDetailID];
    val /= 30.0f;

    pBuffer[(y*nSize+x)*4+nDetailID] = (UCHAR)val;
}


//------------------------------------------------------------------------
// Modify the terrain's height value.
//------------------------------------------------------------------------
void etBrush::ModifyTerrain(void)
{
    if( m_BrushType == TT_PAINT ||
        m_BrushType == TT_ERASE ||
        m_BrushType == TT_BLEND )
        PaintTerrain();
    else
    {
        if( m_BrushShape == TS_CIRCLE )
            ModifyTerrainCS();
        else
            ModifyTerrainRS();

        etFloatRect rect;
        rect.SetMinPoint( m_fX-m_fOuter,m_fZ-m_fOuter );
        rect.SetMaxPoint( m_fX+m_fOuter,m_fZ+m_fOuter );

        // Need to update the tiles.
        for( int i=0;i<m_pTerrain->GetNumTiles();i++ )
        {
            etTile* pTile = m_pTerrain->GetTerrainTile( i );
            if( rect.IsIntersected(pTile->GetBoundingRect()) )
                pTile->NeedUpdateHeight();
        }
    }
}


//------------------------------------------------------------------------
// Paint the terrain with a detail layer.
//------------------------------------------------------------------------
void etBrush::PaintTerrain(void)
{
    if( m_BrushShape == TS_CIRCLE )
        PaintTerrainCS( m_nCurLayer );
    else
        PaintTerrainRS( m_nCurLayer );
}


//------------------------------------------------------------------------
// Update the brush for the editor.
//------------------------------------------------------------------------
void etBrush::UpdateObject(void)
{
    etCoreManager& core = etCoreManager::Instance();

    if( core.GetWorkMode() != WKM_TERRAIN ||
        core.GetCameraMode() != CM_EDITCAMERA )
        return;

    // Build the renderable object.
    if( m_bBrushChanged )
    {
        m_pRenderable->BuildBrushData( m_BrushShape,m_fInner,m_fOuter,m_fX,m_fZ );
        m_bBrushChanged = false;
    }

    // Update the boundingBox.
    m_WorldBBox = m_pRenderable->GetBoundingBox();
    m_WorldBBox.TransformBox( GetWorldTransform() );
}


//------------------------------------------------------------------------
// Render the brush for the editor.
//------------------------------------------------------------------------
void etBrush::RenderObject(void)
{
    etCoreManager& core = etCoreManager::Instance();

    if( core.GetWorkMode() != WKM_TERRAIN ||
        core.GetCameraMode() != CM_EDITCAMERA )
        return;

    // Add the object to renderQueue.
    m_pRenderable->AddToRenderQueue();
}


//------------------------------------------------------------------------
// Get the local boundingBox of the brush.
//------------------------------------------------------------------------
const FlyAabb& etBrush::GetLocalBoundingBox(void) const
{
    return m_pRenderable->GetBoundingBox();
}


//------------------------------------------------------------------------
// Change the shape of the brush.
//------------------------------------------------------------------------
void etBrush::SetBrushShape( TOOLSHAPE Shape )
{
    if( m_BrushShape != Shape )
    {
        m_BrushShape = Shape;
        m_bBrushChanged = true;
    }
}


//------------------------------------------------------------------------
// Change the inner radius of the brush.
//------------------------------------------------------------------------
void etBrush::SetInnerRadius( float fRadius )
{
    if( m_fInner != fRadius )
    {
        m_fInner = fRadius;
        m_bBrushChanged = true;
    }
}


//------------------------------------------------------------------------
// Change the outer radius of the brush.
//------------------------------------------------------------------------
void etBrush::SetOuterRadius( float fRadius )
{
    if( m_fOuter != fRadius )
    {
        m_fOuter = fRadius;
        m_bBrushChanged = true;
    }
}


//------------------------------------------------------------------------
// Set the position for the brush.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      X- coordinates of the position.
//      float:      Z- coordinates of the position.
//------------------------------------------------------------------------
void etBrush::SetPosition( float fX,float fZ )
{
    if( m_fX != fX )
    {
        m_fX = fX;
        m_bBrushChanged = true;
    }

    if( m_fZ != fZ )
    {
        m_fZ = fZ;
        m_bBrushChanged = true;
    }
}


//////////////////////////////////////////////////////////////////////////
// IMPLEMENTING THE CLASS ETBRUSHRENDERABLE.

//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etBrushRenderable::etBrushRenderable( etBrush* pBrush )
{
    m_pParent = pBrush;
    m_nPriority = PRIORITY_HIGH;

    m_nNumVerts = 50;
    m_nNumIndis = 0;
    m_nNumPrims = 48;

    // Get the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    // Create the vertex buffer.
    m_pInnerVB = pResMgr->MakeVertexBuffer( 25*sizeof(VERTEXL),BU_WRITEONLY,MM_MANAGED );
    m_pOuterVB = pResMgr->MakeVertexBuffer( 25*sizeof(VERTEXL),BU_WRITEONLY,MM_MANAGED );

    VERTEXL* pContent;
    pContent = (VERTEXL*)m_pInnerVB->Lock( LOCK_NORMAL );
    for( int i=0;i<25;i++ )
    {
        pContent->x = 0.0f;
        pContent->y = 0.0f;
        pContent->z = 0.0f;
        pContent->color = 0xffff0000;
        pContent->tu = 0.0f;
        pContent->tv = 0.0f;
        pContent++;
    }
    m_pInnerVB->Unlock();

    pContent = (VERTEXL*)m_pOuterVB->Lock( LOCK_NORMAL );
    for( i=0;i<25;i++ )
    {
        pContent->x = 0.0f;
        pContent->y = 0.0f;
        pContent->z = 0.0f;
        pContent->color = 0xff00ff00;
        pContent->tu = 0.0f;
        pContent->tv = 0.0f;
        pContent++;
    }
    m_pOuterVB->Unlock();

    // Create the material for brush.
    m_pMaterial = pResMgr->GetMaterialByName( "et-Brush" );
    if( !m_pMaterial )
    {
        m_pMaterial = pResMgr->CreateMaterial( "et-Brush" );
        m_pMaterial->SetDepthMode( 0,DEPTH_NONE );
    }
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etBrushRenderable::~etBrushRenderable(void)
{
    SAFE_DELETE( m_pInnerVB );
    SAFE_DELETE( m_pOuterVB );
}


//------------------------------------------------------------------------
// Build the geometry data for the brush.
// ----------------------------------------------------------------------
// Param -> IN:
//      TOOLSHAPE:      Shape of the brush.
//      float:          Inner radius of the brush.
//      float:          Outer radius of the brush.
//      float:          X- coordinates of the position.
//      float:          Z- coordinates of the position.
//------------------------------------------------------------------------
HRESULT etBrushRenderable::BuildBrushData( TOOLSHAPE Shape,float fInner,float fOuter,
                                           float fX,float fZ )
{
    VERTEXL* pContent;

    // Get the pointer to the terrain.
    etTerrain* pTerrain = m_pParent->GetTerrain();

    m_BBox.vcMin.x = fX - fOuter;
    m_BBox.vcMin.y = -1.0f;
    m_BBox.vcMin.z = fZ - fOuter;
    m_BBox.vcMax.x = fX + fOuter;
    m_BBox.vcMax.z = 1.0f;
    m_BBox.vcMax.z = fZ + fOuter;

    if( Shape == TS_CIRCLE )
    {
        // Inner circle data.
        pContent = static_cast<VERTEXL*>( m_pInnerVB->Lock(LOCK_NORMAL) );
        for( int i=0;i<25;i++ )
        {
            pContent->x = fX + fInner*sinf(fDelta*i);
            pContent->z = fZ + fInner*cosf(fDelta*i);
            pContent->y = pTerrain->GetScaledHeightValue(pContent->x,pContent->z)+1.0f;

            if( pContent->y < m_BBox.vcMin.y ) m_BBox.vcMin.y = pContent->y;
            if( pContent->y > m_BBox.vcMax.y ) m_BBox.vcMax.y = pContent->y;
            pContent++;
        }
        m_pInnerVB->Unlock();

        // Outer circle data.
        pContent = static_cast<VERTEXL*>( m_pOuterVB->Lock(LOCK_NORMAL) );
        for( i=0;i<25;i++ )
        {
            pContent->x = fX + fOuter*sinf(fDelta*i);
            pContent->z = fZ + fOuter*cosf(fDelta*i);
            pContent->y = pTerrain->GetScaledHeightValue(pContent->x,pContent->z)+1.0f;

            if( pContent->y < m_BBox.vcMin.y ) m_BBox.vcMin.y = pContent->y;
            if( pContent->y > m_BBox.vcMax.y ) m_BBox.vcMax.y = pContent->y;
            pContent++;
        }
        m_pOuterVB->Unlock();
    }
    else
    {
        float fLen;
        VERTEXL vTemp;

        // Inner rectangle data.
        fLen = fInner / 3.0f;
        pContent = static_cast<VERTEXL*>( m_pInnerVB->Lock(LOCK_NORMAL) );
        pContent->x = fX - fInner;
        pContent->z = fZ + fInner;
        pContent->y = pTerrain->GetScaledHeightValue(pContent->x,pContent->z)+1.0f;

        if( pContent->y < m_BBox.vcMin.y ) m_BBox.vcMin.y = pContent->y;
        if( pContent->y > m_BBox.vcMax.y ) m_BBox.vcMax.y = pContent->y;
        pContent++;

        vTemp.x = fX - fInner;
        vTemp.z = fZ + fInner;
        for( int i=1;i<=6;i++ )
        {
            pContent->x = vTemp.x + fLen * i;
            pContent->z = vTemp.z;
            pContent->y = pTerrain->GetScaledHeightValue(pContent->x,pContent->z)+1.0f;

            if( pContent->y < m_BBox.vcMin.y ) m_BBox.vcMin.y = pContent->y;
            if( pContent->y > m_BBox.vcMax.y ) m_BBox.vcMax.y = pContent->y;
            pContent++;
        }

        vTemp.x = fX + fInner;
        vTemp.z = fZ + fInner;
        for( i=7;i<=12;i++ )
        {
            pContent->x = vTemp.x;
            pContent->z = vTemp.z - fLen * (i-6);
            pContent->y = pTerrain->GetScaledHeightValue(pContent->x,pContent->z)+1.0f;

            if( pContent->y < m_BBox.vcMin.y ) m_BBox.vcMin.y = pContent->y;
            if( pContent->y > m_BBox.vcMax.y ) m_BBox.vcMax.y = pContent->y;
            pContent++;
        }

        vTemp.x = fX + fInner;
        vTemp.z = fZ - fInner;
        for( i=13;i<=18;i++ )
        {
            pContent->x = vTemp.x - fLen * (i-12);
            pContent->z = vTemp.z;
            pContent->y = pTerrain->GetScaledHeightValue(pContent->x,pContent->z)+1.0f;

            if( pContent->y < m_BBox.vcMin.y ) m_BBox.vcMin.y = pContent->y;
            if( pContent->y > m_BBox.vcMax.y ) m_BBox.vcMax.y = pContent->y;
            pContent++;
        }

        vTemp.x = fX - fInner;
        vTemp.z = fZ - fInner;
        for( i=19;i<=24;i++ )
        {
            pContent->x = vTemp.x;
            pContent->z = vTemp.z + fLen * (i-18);
            pContent->y = pTerrain->GetScaledHeightValue(pContent->x,pContent->z)+1.0f;

            if( pContent->y < m_BBox.vcMin.y ) m_BBox.vcMin.y = pContent->y;
            if( pContent->y > m_BBox.vcMax.y ) m_BBox.vcMax.y = pContent->y;
            pContent++;
        }
        m_pInnerVB->Unlock();

        // Outer rectangle data.
        fLen = fOuter / 3.0f;
        pContent = static_cast<VERTEXL*>( m_pOuterVB->Lock(LOCK_NORMAL) );
        pContent->x = fX - fOuter;
        pContent->z = fZ + fOuter;
        pContent->y = pTerrain->GetScaledHeightValue(pContent->x,pContent->z)+1.0f;

        if( pContent->y < m_BBox.vcMin.y ) m_BBox.vcMin.y = pContent->y;
        if( pContent->y > m_BBox.vcMax.y ) m_BBox.vcMax.y = pContent->y;
        pContent++;

        vTemp.x = fX - fOuter;
        vTemp.z = fZ + fOuter;
        for( i=1;i<=6;i++ )
        {
            pContent->x = vTemp.x + fLen * i;
            pContent->z = vTemp.z;
            pContent->y = pTerrain->GetScaledHeightValue(pContent->x,pContent->z)+1.0f;

            if( pContent->y < m_BBox.vcMin.y ) m_BBox.vcMin.y = pContent->y;
            if( pContent->y > m_BBox.vcMax.y ) m_BBox.vcMax.y = pContent->y;
            pContent++;
        }

        vTemp.x = fX + fOuter;
        vTemp.z = fZ + fOuter;
        for( i=7;i<=12;i++ )
        {
            pContent->x = vTemp.x;
            pContent->z = vTemp.z - fLen * (i-6);
            pContent->y = pTerrain->GetScaledHeightValue(pContent->x,pContent->z)+1.0f;

            if( pContent->y < m_BBox.vcMin.y ) m_BBox.vcMin.y = pContent->y;
            if( pContent->y > m_BBox.vcMax.y ) m_BBox.vcMax.y = pContent->y;
            pContent++;
        }

        vTemp.x = fX + fOuter;
        vTemp.z = fZ - fOuter;
        for( i=13;i<=18;i++ )
        {
            pContent->x = vTemp.x - fLen * (i-12);
            pContent->z = vTemp.z;
            pContent->y = pTerrain->GetScaledHeightValue(pContent->x,pContent->z)+1.0f;

            if( pContent->y < m_BBox.vcMin.y ) m_BBox.vcMin.y = pContent->y;
            if( pContent->y > m_BBox.vcMax.y ) m_BBox.vcMax.y = pContent->y;
            pContent++;
        }

        vTemp.x = fX - fOuter;
        vTemp.z = fZ - fOuter;
        for( i=19;i<=24;i++ )
        {
            pContent->x = vTemp.x;
            pContent->z = vTemp.z + fLen * (i-18);
            pContent->y = pTerrain->GetScaledHeightValue(pContent->x,pContent->z)+1.0f;

            if( pContent->y < m_BBox.vcMin.y ) m_BBox.vcMin.y = pContent->y;
            if( pContent->y > m_BBox.vcMax.y ) m_BBox.vcMax.y = pContent->y;
            pContent++;
        }
        m_pOuterVB->Unlock();
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Render the brush for the editor.
//------------------------------------------------------------------------
void etBrushRenderable::Render(void)
{
    // Get the pointer to the renderer.
    LPFLYRENDERER pRenderer = etCoreManager::Instance().GetRenderer();

    pRenderer->SetRenderMaterial( m_pMaterial );
    pRenderer->SetRenderType( RT_LINESTRIP );
    pRenderer->RenderBuffer( VF_DEFAULTNOLIGHT,25,0,m_pInnerVB,NULL );
    pRenderer->RenderBuffer( VF_DEFAULTNOLIGHT,25,0,m_pOuterVB,NULL );
}


//------------------------------------------------------------------------
// Get the count of the vertices.
//------------------------------------------------------------------------
UINT etBrushRenderable::GetNumVerts(void) const
{
    return m_nNumVerts;
}

//------------------------------------------------------------------------
// Get the count of the indices.
//------------------------------------------------------------------------
UINT etBrushRenderable::GetNumIndis(void) const
{
    return m_nNumIndis;
}

//------------------------------------------------------------------------
// Get the count of the primitives.
//------------------------------------------------------------------------
UINT etBrushRenderable::GetNumPrims(void) const
{
    return m_nNumPrims;
}

//------------------------------------------------------------------------
// Get the local boundingBox of the object.
//------------------------------------------------------------------------
const FlyAabb& etBrushRenderable::GetBoundingBox(void) const
{
    return m_BBox;
}

//------------------------------------------------------------------------
// Get the square distance to the camera.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   Position of the camera.
//------------------------------------------------------------------------
float etBrushRenderable::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    FlyVector vTemp = m_pParent->GetWorldBoundingBox().GetCenter();

    return (vTemp - vCamPos).GetSqrLength();
}

//------------------------------------------------------------------------
// Get the world matrices of the object.
//------------------------------------------------------------------------
void etBrushRenderable::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    *pMatrices = m_pParent->GetWorldTransform();
}

//------------------------------------------------------------------------
// Get the count of the world matrices.
//------------------------------------------------------------------------
UINT etBrushRenderable::GetNumWorldMatrices(void) const
{
    return 1;
}

//------------------------------------------------------------------------
// Get the count of the light object.
//------------------------------------------------------------------------
UINT etBrushRenderable::GetLightCount(void) const
{
    return 0;
}

//------------------------------------------------------------------------
// Get the specified light object.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:        Index of the light object.
//------------------------------------------------------------------------
FlyLightObject* etBrushRenderable::GetLightObject( UINT nIndex )
{
    return NULL;
}