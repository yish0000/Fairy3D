//------------------------------------------------------------------------
// Name: etConfig.h
// Desc: This file define a class to store the config data.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_CONFIG_H__
#define __ET_CONFIG_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etCommon.h"

#include "FlyXmlReader.h"
//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etConfig
{
public:
    struct sSkyboxNode
    {
        std::string sName;
        std::string sFront;
        std::string sBack;
        std::string sLeft;
        std::string sRight;
        std::string sTop;
        std::string sBottom;
    };

    typedef std::vector<sSkyboxNode> SkyboxList;

protected:
    float m_fClipNear;          // Near distance.
    float m_fClipFar;           // Far distance.
    FLYCOLOR m_cBackColor;      // BackColor of the editor.
    float m_fMaxHeight;         // Max height value we can raise.
    FLYCOLOR m_cWireColor;      // Color when we render objects as wireframe.
    float m_fCamMoveSpeed;      // Camera's moving speed.
    float m_fCamRotateSpeed;    // Camera's rotating speed.
    float m_fETMoveSpeed;       // Entity moving speed.
    float m_fETRotateSpeed;     // Entity rotating speed.
    float m_fETScaleSpeed;      // Entity scale speed.

    SkyboxList m_Skyboxes;      // Skybox list for the editor.

public:
    // Constructor and destructor.
    etConfig(void);
    ~etConfig(void);

    // Load the config data from xml document.
    HRESULT LoadConfig( const char* cConfigFile );

    // Save the config data to the xml document.
    HRESULT SaveConfig( const char* cConfigFile );

    // Add a new skybox node.
    void AddNewSkybox( const char* cName,const char* cFrontFace,const char* cBackFace,
        const char* cLeftFace,const char* cRightFace,const char* cTopFace,
        const char* cBottomFace );

    // Remove a skybox node.
    void RemoveSkybox( UINT nIndex );
    void RemoveSkyboxByName( const char* cName );

    bool IsSkyboxExisted( const char* cName ) const;
    UINT GetNumSkybox(void) const;
    const char* GetSkyboxName( UINT nIndex ) const;
    const char* GetSkyboxFront( UINT nIndex ) const;
    const char* GetSkyboxBack( UINT nIndex ) const;
    const char* GetSkyboxLeft( UINT nIndex ) const;
    const char* GetSkyboxRight( UINT nIndex ) const;
    const char* GetSkyboxTop( UINT nIndex ) const;
    const char* GetSkyboxBottom( UINT nIndex ) const;
    void SetSkyboxName( UINT nIndex,const char* cName );
    void SetSkyboxFront( UINT nIndex,const char* cFront );
    void SetSkyboxBack( UINT nIndex,const char* cBack );
    void SetSkyboxLeft( UINT nIndex,const char* cLeft );
    void SetSkyboxRight( UINT nIndex,const char* cRight );
    void SetSkyboxTop( UINT nIndex,const char* cTop );
    void SetSkyboxBottom( UINT nIndex,const char* cBottom );

    float GetNearDistance(void) const { return m_fClipNear; }
    float GetFarDistance(void) const { return m_fClipFar; }
    const FLYCOLOR& GetBackColor(void) const { return m_cBackColor; }
    float GetMaxHeight(void) const { return m_fMaxHeight; }
    const FLYCOLOR& GetWireColor(void) const { return m_cWireColor; }
    float GetCamMoveSpeed(void) const { return m_fCamMoveSpeed; }
    float GetCamRotateSpeed(void) const { return m_fCamRotateSpeed; }
    float GetETMoveSpeed(void) const { return m_fETMoveSpeed; }
    float GetETRotateSpeed(void) const { return m_fETRotateSpeed; }
    float GetETScaleSpeed(void) const { return m_fETScaleSpeed; }

    void SetNearDistance( float fDistance ) { m_fClipNear = fDistance; }
    void SetFarDistance( float fDistance ) { m_fClipFar = fDistance; }
    void SetBackColor( const FLYCOLOR& color ) { m_cBackColor = color; }
    void SetMaxHeight( float fMaxHeight ) { m_fMaxHeight = fMaxHeight; }
    void SetWireColor( const FLYCOLOR& color ) { m_cWireColor = color; }
    void SetCamMoveSpeed( float fMoveSpeed ) { m_fCamMoveSpeed = fMoveSpeed; }
    void SetCamRotateSpeed( float fRotateSpeed ) { m_fCamRotateSpeed = fRotateSpeed; }
    void SetETMoveSpeed( float fMoveSpeed ) { m_fETMoveSpeed = fMoveSpeed; }
    void SetETRotateSpeed( float fRotateSpeed ) { m_fETRotateSpeed = fRotateSpeed; }
    void SetETScaleSpeed( float fScaleSpeed ) { m_fETScaleSpeed = fScaleSpeed; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_CONFIG_H__