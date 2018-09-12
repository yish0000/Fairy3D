//------------------------------------------------------------------------
// Name: etConfig.cpp
// Desc: This file is to implement the class etConfig.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etConfig.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etConfig::etConfig(void)
{
    m_fClipNear = 1.0f;
    m_fClipFar = 1000.0f;
    m_cBackColor.fR = 0.4f;
    m_cBackColor.fG = 0.6f;
    m_cBackColor.fB = 0.4f;
    m_fMaxHeight = 255.0f;
    m_cWireColor = FLYCOLOR::White();
    m_fCamMoveSpeed = 5.0f;
    m_fCamRotateSpeed = FLY_PI / 2.0f;
    m_fETMoveSpeed = 1.0f;
    m_fETRotateSpeed = 0.01f;
    m_fETScaleSpeed = 0.01f;
}

//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etConfig::~etConfig(void)
{
}


//------------------------------------------------------------------------
// Load the config data from the xml document.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the config file.
//------------------------------------------------------------------------
HRESULT etConfig::LoadConfig( const char* cConfigFile )
{
    HRESULT hr;
    FlyXmlReader xmlDoc;
    FlyXmlReader::XmlNodeAccess root;
    FlyXmlReader::XmlNodeAccess curr;
    char cTemp[40];

    // Load the xml document.
    hr = xmlDoc.LoadDocument( cConfigFile );
    if( FAILED(hr) ) return hr;

    // Start to parse the data.
    root = xmlDoc.GetRootNode();
    if( strcmp(root.GetName(),"editor") != 0 ||
        strcmp(root.GetParamValueByName("version"),"1.0") != 0)
        return ET_INVALIDCFGFILE;

    for( size_t i=0;i<root.GetChildCount();i++ )
    {
        curr = root.GetChild( i );

        if( strcmp(curr.GetName(),"clipnear") == 0 )
        {
            if( curr.GetParamValueByName("value") )
                sscanf( curr.GetParamValueByName("value"),"%f",&m_fClipNear );
            else
                m_fClipNear = 1.0f;
        }
        else if( strcmp(curr.GetName(),"clipfar") == 0 )
        {
            if( curr.GetParamValueByName("value") )
                sscanf( curr.GetParamValueByName("value"),"%f",&m_fClipFar );
            else
                m_fClipFar = 1000.0f;
        }
        else if( strcmp(curr.GetName(),"backcolor") == 0 )
        {
            UCHAR ucR,ucG,ucB;

            if( curr.GetParamValueByName("value") )
                sscanf( curr.GetParamValueByName("value"),"(%d,%d,%d)",&ucR,&ucG,&ucB );
            else
            {
                ucR = 100;
                ucG = 150;
                ucB = 100;
            }

            m_cBackColor.fR = (float)ucR / 255.0f;
            m_cBackColor.fG = (float)ucG / 255.0f;
            m_cBackColor.fB = (float)ucB / 255.0f;
            m_cBackColor.fA = 1.0f;
        }
        else if( strcmp(curr.GetName(),"maxheight") == 0 )
        {
            if( curr.GetParamValueByName("value") )
                sscanf( curr.GetParamValueByName("value"),"%f",&m_fMaxHeight );
            else
                m_fMaxHeight = 255.0f;
        }
        else if( strcmp(curr.GetName(),"wirecolor") == 0 )
        {
            UCHAR ucR,ucG,ucB;

            if( curr.GetParamValueByName("value") )
                sscanf( curr.GetParamValueByName("value"),"(%d,%d,%d)",&ucR,&ucG,&ucB );
            else
                ucR = ucG = ucB = 255;

            m_cWireColor.fR = (float)ucR / 255.0f;
            m_cWireColor.fG = (float)ucG / 255.0f;
            m_cWireColor.fB = (float)ucB / 255.0f;
            m_cWireColor.fA = 1.0f;
        }
        else if( strcmp(curr.GetName(),"cammovespeed") == 0 )
        {
            if( curr.GetParamValueByName("value") )
                sscanf( curr.GetParamValueByName("value"),"%f",&m_fCamMoveSpeed );
            else
                m_fCamMoveSpeed = 5.0f;
        }
        else if( strcmp(curr.GetName(),"camrotatespeed") == 0 )
        {
            if( curr.GetParamValueByName("value") )
                sscanf( curr.GetParamValueByName("value"),"%f",&m_fCamRotateSpeed );
            else
                m_fCamRotateSpeed = FLY_PI / 2.0f;
        }
        else if( strcmp(curr.GetName(),"etmovespeed") == 0 )
        {
            if( curr.GetParamValueByName("value") )
                sscanf( curr.GetParamValueByName("value"),"%f",&m_fETMoveSpeed );
            else
                m_fETMoveSpeed = 0.5f;
        }
        else if( strcmp(curr.GetName(),"etrotatespeed") == 0 )
        {
            if( curr.GetParamValueByName("value") )
                sscanf( curr.GetParamValueByName("value"),"%f",&m_fETRotateSpeed );
            else
                m_fETRotateSpeed = 0.01f;
        }
        else if( strcmp(curr.GetName(),"etscalespeed") == 0 )
        {
            if( curr.GetParamValueByName("value") == 0 )
                sscanf( curr.GetParamValueByName("value"),"%f",&m_fETScaleSpeed );
            else
                m_fETScaleSpeed = 0.01f;
        }
        else if( strcmp(curr.GetName(),"skybox") == 0 )
        {
            sSkyboxNode node;
            FlyXmlReader::XmlNodeAccess child;

            // Set the skybox's name.
            if( curr.GetParamValueByName("name") )
                node.sName = curr.GetParamValueByName( "name" );
            else
            {
                sprintf( cTemp,"Skybox%d",m_Skyboxes.size() );
                node.sName = cTemp;
            }

            for( size_t n=0;n<curr.GetChildCount();n++ )
            {
                child = curr.GetChild( n );

                if( strcmp(child.GetName(),"front") == 0 )
                {
                    if( child.GetParamValueByName("texture") )
                        node.sFront = child.GetParamValueByName( "texture" );
                    else
                        node.sFront = "";
                }
                else if( strcmp(child.GetName(),"back") == 0 )
                {
                    if( child.GetParamValueByName("texture") )
                        node.sBack = child.GetParamValueByName( "texture" );
                    else
                        node.sBack = "";
                }
                else if( strcmp(child.GetName(),"left") == 0 )
                {
                    if( child.GetParamValueByName("texture") )
                        node.sLeft = child.GetParamValueByName( "texture" );
                    else
                        node.sLeft = "";
                }
                else if( strcmp(child.GetName(),"right") == 0 )
                {
                    if( child.GetParamValueByName("texture") )
                        node.sRight = child.GetParamValueByName( "texture" );
                    else
                        node.sRight = "";
                }
                else if( strcmp(child.GetName(),"top") == 0 )
                {
                    if( child.GetParamValueByName("texture") )
                        node.sTop = child.GetParamValueByName( "texture" );
                    else
                        node.sTop = "";
                }
                else if( strcmp(child.GetName(),"bottom") == 0 )
                {
                    if( child.GetParamValueByName("texture") )
                        node.sBottom = child.GetParamValueByName( "texture" );
                    else
                        node.sBottom = "";
                }
            }

            m_Skyboxes.push_back( node );
        }
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Save the config data of the editor.
//------------------------------------------------------------------------
HRESULT etConfig::SaveConfig( const char* cConfigFile )
{
    FILE* pFile = fopen( cConfigFile,"w" );
    if( !pFile ) return FLY_INVALIDPARAM;
    fprintf( pFile,"<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n\n" );

    fprintf( pFile,"<editor version=\"1.0\">\n" );
    fprintf( pFile,"    <clipnear value=\"%f\" />\n",m_fClipNear );
    fprintf( pFile,"    <clipfar value=\"%f\" />\n",m_fClipFar );
    fprintf( pFile,"    <backcolor value=\"(%d,%d,%d)\" />\n",(UCHAR)(255*m_cBackColor.fR),
        (UCHAR)(255*m_cBackColor.fG),(UCHAR)(255*m_cBackColor.fB) );
    fprintf( pFile,"    <maxheight value=\"%f\" />\n",m_fMaxHeight );
    fprintf( pFile,"    <wirecolor value=\"(%d,%d,%d)\" />\n",(UCHAR)(255*m_cWireColor.fR),
        (UCHAR)(255*m_cWireColor.fG),(UCHAR)(255*m_cWireColor.fB) );
    fprintf( pFile,"    <cammovespeed value=\"%f\" />\n",m_fCamMoveSpeed );
    fprintf( pFile,"    <camrotatespeed value=\"%f\" />\n\n",m_fCamRotateSpeed );
    fprintf( pFile,"    <etmovespeed value=\"%f\" />\n",m_fETMoveSpeed );
    fprintf( pFile,"    <etrotatespeed value=\"%f\" />\n",m_fETRotateSpeed );
    fprintf( pFile,"    <etscalespeed value=\"%f\" />\n",m_fETScaleSpeed );

    for( size_t i=0;i<m_Skyboxes.size();i++ )
    {
        fprintf( pFile,"    <skybox name=\"%s\">\n",m_Skyboxes[i].sName.c_str() );
        fprintf( pFile,"        <front texture=\"%s\" />\n",m_Skyboxes[i].sFront.c_str() );
        fprintf( pFile,"        <back texture=\"%s\" />\n",m_Skyboxes[i].sBack.c_str() );
        fprintf( pFile,"        <left texture=\"%s\" />\n",m_Skyboxes[i].sLeft.c_str() );
        fprintf( pFile,"        <right texture=\"%s\" />\n",m_Skyboxes[i].sRight.c_str() );
        fprintf( pFile,"        <top texture=\"%s\" />\n",m_Skyboxes[i].sTop.c_str() );
        fprintf( pFile,"        <bottom texture=\"%s\" />\n",m_Skyboxes[i].sBottom.c_str() );
        fprintf( pFile,"    </skybox>\n" );
    }

    fprintf( pFile,"</editor>\n" );
    fclose( pFile );
    return FLY_OK;
}


//------------------------------------------------------------------------
// Add a new skybox for the editor.
//------------------------------------------------------------------------
void etConfig::AddNewSkybox( const char* cName,const char* cFrontFace,
                             const char* cBackFace,const char* cLeftFace,
                             const char* cRightFace,const char* cTopFace,
                             const char* cBottomFace )
{
    // Check whether the skybox existed.
    SkyboxList::iterator itr;
    for( itr=m_Skyboxes.begin();itr!=m_Skyboxes.end();itr++ )
    {
        if( itr->sName == cName )
            return;
    }

    sSkyboxNode node;
    node.sName = cName;
    node.sFront = cFrontFace;
    node.sBack = cBackFace;
    node.sLeft = cLeftFace;
    node.sRight = cRightFace;
    node.sTop = cTopFace;
    node.sBottom = cBottomFace;
    m_Skyboxes.push_back( node );
}


//------------------------------------------------------------------------
// Remove the specified skybox.
//------------------------------------------------------------------------
void etConfig::RemoveSkybox( UINT nIndex )
{
    if( nIndex >= m_Skyboxes.size() )
        return;

    m_Skyboxes.erase( m_Skyboxes.begin() + nIndex );
}


//------------------------------------------------------------------------
// Remove the specified skybox by its name.
//------------------------------------------------------------------------
void etConfig::RemoveSkyboxByName( const char* cName )
{
    SkyboxList::iterator itr;
    for( itr=m_Skyboxes.begin();itr!=m_Skyboxes.end();itr++ )
    {
        if( itr->sName == cName )
        {
            m_Skyboxes.erase( itr );
            break;
        }
    }
}


//------------------------------------------------------------------------
// Get the count of the skybox.
//------------------------------------------------------------------------
UINT etConfig::GetNumSkybox(void) const
{
    return m_Skyboxes.size();
}


//------------------------------------------------------------------------
// Is the specifed skybox existed ?
//------------------------------------------------------------------------
bool etConfig::IsSkyboxExisted( const char* cName ) const
{
    for( size_t n=0;n<m_Skyboxes.size();n++ )
    {
        if( m_Skyboxes[n].sName == cName )
            return true;
    }

    return false;
}


//------------------------------------------------------------------------
// Get the name of the skybox.
//------------------------------------------------------------------------
const char* etConfig::GetSkyboxName( UINT nIndex ) const
{
    if( nIndex >= m_Skyboxes.size() )
        return NULL;

    return m_Skyboxes[nIndex].sName.c_str();
}

//------------------------------------------------------------------------
// Get the front face of the skybox.
//------------------------------------------------------------------------
const char* etConfig::GetSkyboxFront( UINT nIndex ) const
{
    if( nIndex >= m_Skyboxes.size() )
        return NULL;

    return m_Skyboxes[nIndex].sFront.c_str();
}

//------------------------------------------------------------------------
// Get the back face of the skybox.
//------------------------------------------------------------------------
const char* etConfig::GetSkyboxBack( UINT nIndex ) const
{
    if( nIndex >= m_Skyboxes.size() )
        return NULL;

    return m_Skyboxes[nIndex].sBack.c_str();
}

//------------------------------------------------------------------------
// Get the left face of the skybox.
//------------------------------------------------------------------------
const char* etConfig::GetSkyboxLeft( UINT nIndex ) const
{
    if( nIndex >= m_Skyboxes.size() )
        return NULL;

    return m_Skyboxes[nIndex].sLeft.c_str();
}

//------------------------------------------------------------------------
// Get the right face of the skybox.
//------------------------------------------------------------------------
const char* etConfig::GetSkyboxRight( UINT nIndex ) const
{
    if( nIndex >= m_Skyboxes.size() )
        return NULL;

    return m_Skyboxes[nIndex].sRight.c_str();
}

//------------------------------------------------------------------------
// Get the top face of the skybox.
//------------------------------------------------------------------------
const char* etConfig::GetSkyboxTop( UINT nIndex ) const
{
    if( nIndex >= m_Skyboxes.size() )
        return NULL;

    return m_Skyboxes[nIndex].sTop.c_str();
}

//------------------------------------------------------------------------
// Get the bottom face of the skybox.
//------------------------------------------------------------------------
const char* etConfig::GetSkyboxBottom( UINT nIndex ) const
{
    if( nIndex >= m_Skyboxes.size() )
        return NULL;

    return m_Skyboxes[nIndex].sBottom.c_str();
}


//------------------------------------------------------------------------
// Set the name for the skybox.
//------------------------------------------------------------------------
void etConfig::SetSkyboxName( UINT nIndex,const char* cName )
{
    if( nIndex >= m_Skyboxes.size() )
        return;

    m_Skyboxes[nIndex].sName = cName;
}

//------------------------------------------------------------------------
// Set the front face for the skybox.
//------------------------------------------------------------------------
void etConfig::SetSkyboxFront( UINT nIndex,const char* cFront )
{
    if( nIndex >= m_Skyboxes.size() )
        return;

    m_Skyboxes[nIndex].sFront = cFront;
}

//------------------------------------------------------------------------
// Set the back face for the skybox.
//------------------------------------------------------------------------
void etConfig::SetSkyboxBack( UINT nIndex,const char* cBack )
{
    if( nIndex >= m_Skyboxes.size() )
        return;

    m_Skyboxes[nIndex].sBack = cBack;
}

//------------------------------------------------------------------------
// Set the left face for the skybox.
//------------------------------------------------------------------------
void etConfig::SetSkyboxLeft( UINT nIndex,const char* cLeft )
{
    if( nIndex >= m_Skyboxes.size() )
        return;

    m_Skyboxes[nIndex].sLeft = cLeft;
}

//------------------------------------------------------------------------
// Set the right face for the skybox.
//------------------------------------------------------------------------
void etConfig::SetSkyboxRight( UINT nIndex,const char* cRight )
{
    if( nIndex >= m_Skyboxes.size() )
        return;

    m_Skyboxes[nIndex].sRight = cRight;
}

//------------------------------------------------------------------------
// Set the top face for the skybox.
//------------------------------------------------------------------------
void etConfig::SetSkyboxTop( UINT nIndex,const char* cTop )
{
    if( nIndex >= m_Skyboxes.size() )
        return;

    m_Skyboxes[nIndex].sTop = cTop;
}

//------------------------------------------------------------------------
// Set the bottom face for the skybox.
//------------------------------------------------------------------------
void etConfig::SetSkyboxBottom( UINT nIndex,const char* cBottom )
{
    if( nIndex >= m_Skyboxes.size() )
        return;

    m_Skyboxes[nIndex].sBottom = cBottom;
}