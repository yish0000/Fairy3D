//------------------------------------------------------------------------
// Name: FlyXmlReader.cpp
// Desc: This file is to implement the class FlyXmlReader.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyXmlReader.h"

#include "tiny_xml/tinyxml.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyXmlReader::FlyXmlReader(void)
{
    m_pRoot = NULL;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyXmlReader::~FlyXmlReader(void)
{
    SAFE_DELETE( m_pRoot );
}


//------------------------------------------------------------------------
// Load the xml document from the file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    The path to the xml document.
//------------------------------------------------------------------------
HRESULT FlyXmlReader::LoadDocument( const char* cXmlDoc )
{
    TiXmlDocument doc;
    TiXmlNode* pNode;
    TiXmlNode* pRoot = NULL;

    // Load the xml file.
    if( !doc.LoadFile(cXmlDoc) ) return FLY_INVALIDFILE;

    // Search the root element.
    pNode = doc.FirstChild();

    if( pNode->Type() == TiXmlNode::TINYXML_ELEMENT )
        pRoot = pNode;

    while( pNode = pNode->NextSibling() )
    {
        if( pNode->Type() == TiXmlNode::TINYXML_ELEMENT )
        {
            // Error: More than one root element.
            if( pRoot ) return FLY_INVALIDFILE;

            pRoot = pNode;
        }
    }

    // If there is no root elements,
    // is an invalid xml doc.
    if( !pRoot ) return FLY_INVALIDFILE;

    // Destroy the old data.
    SAFE_DELETE( m_pRoot );

    // Process the nodes from root.
    m_pRoot = new sXmlNode;
    m_pRoot->parent = NULL;
    ProcessXmlNode( pRoot->ToElement(),m_pRoot );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Parse the xml data from the string buffer.
// ----------------------------------------------------------------------
// Param -> IN:
//      const void*:    Pointer to the buffer which contain xml datas.
//------------------------------------------------------------------------
HRESULT FlyXmlReader::ParseFromBuffer( const void* pContent )
{
    TiXmlDocument doc;
    TiXmlNode* pNode;
    TiXmlNode* pRoot = NULL;

    // Load the xml from buffer.
    if( !doc.Parse((const char*)pContent) )
        return FLY_INVALIDPARAM;

    // Search the root element.
    pNode = doc.FirstChild();

    if( pNode->Type() == TiXmlNode::TINYXML_ELEMENT )
        pRoot = pNode;

    while( pNode = pNode->NextSibling() )
    {
        if( pNode->Type() == TiXmlNode::TINYXML_ELEMENT )
        {
            // Error: More than one root element.
            if( pRoot ) return FLY_INVALIDFILE;

            pRoot = pNode;
        }
    }

    // If there is no root elements,
    // is an invalid xml doc.
    if( !pRoot ) return FLY_INVALIDFILE;

    // Destroy the old data.
    SAFE_DELETE( m_pRoot );

    // Process the nodes from root.
    m_pRoot = new sXmlNode;
    m_pRoot->parent = NULL;
    ProcessXmlNode( pRoot->ToElement(),m_pRoot );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Process the xml node from the root of the document.
// ----------------------------------------------------------------------
// Param -> IN:
//      const void*:    Pointer to the xml element. (Tiny xml)
//      sXmlNode*:      Pointer to the xml node.
//------------------------------------------------------------------------
void FlyXmlReader::ProcessXmlNode( const void* pElement,sXmlNode* pNode )
{
    sXmlParam param;
    sXmlNode* child;
    TiXmlAttribute* pNextAttrib;
    TiXmlElement* pNextChild;
    TiXmlElement* pCurTiElem;

    // Convert the current element pointer.
    pCurTiElem = (TiXmlElement*)pElement;

    // Write the current node.
    pNode->name = pCurTiElem->Value();

    if( pCurTiElem->GetText() )
    {
        pNode->bHaveText = true;
        pNode->text = pCurTiElem->GetText();
    }
    else
    {
        pNode->bHaveText = false;
        pNode->text = "";
    }

    // Write the parameters.
    pNextAttrib = pCurTiElem->FirstAttribute();
    if( pNextAttrib )
    {
        param.name = pNextAttrib->Name();
        param.value = pNextAttrib->Value();
        pNode->params.push_back( param );

        while( pNextAttrib = pNextAttrib->Next() )
        {
            param.name = pNextAttrib->Name();
            param.value = pNextAttrib->Value();
            pNode->params.push_back( param );
        }
    }

    // Process the children.
    pNextChild = pCurTiElem->FirstChildElement();
    if( pNextChild )
    {
        child = new sXmlNode;
        child->parent = pNode;
        ProcessXmlNode( pNextChild,child );
        pNode->childs.push_back( child );

        while( pNextChild = pNextChild->NextSiblingElement() )
        {
            child = new sXmlNode;
            child->parent = pNode;
            ProcessXmlNode( pNextChild,child );
            pNode->childs.push_back( child );
        }
    }
}


//------------------------------------------------------------------------
// Get the root node to access the content of the xml document.
//------------------------------------------------------------------------
FlyXmlReader::XmlNodeAccess FlyXmlReader::GetRootNode(void)
{
    return FlyXmlReader::XmlNodeAccess( m_pRoot );
}


/*************************************************************************
*                                                                        *
*     IMPLEMENT THE CLASS TO ACESS THE XML'S CONTENT                     *
*                                                                        *
*************************************************************************/


//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
FlyXmlReader::XmlNodeAccess::XmlNodeAccess(void)
{
    m_pNode = NULL;
}

FlyXmlReader::XmlNodeAccess::XmlNodeAccess( sXmlNode* pNode )
{
    m_pNode = pNode;
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
FlyXmlReader::XmlNodeAccess::~XmlNodeAccess(void)
{
}


//------------------------------------------------------------------------
// Get the name of the current node.
//------------------------------------------------------------------------
const char* FlyXmlReader::XmlNodeAccess::GetName(void)
{
    if( !m_pNode ) return NULL;

    return m_pNode->name.c_str();
}


//------------------------------------------------------------------------
// Get the text of the current node.
//------------------------------------------------------------------------
const char* FlyXmlReader::XmlNodeAccess::GetText(void)
{
    if( !m_pNode || !m_pNode->bHaveText )
        return NULL;
    else
        return m_pNode->text.c_str();
}


//------------------------------------------------------------------------
// Get the count of the parameter at this node.
//------------------------------------------------------------------------
size_t FlyXmlReader::XmlNodeAccess::GetParamCount(void)
{
    if( !m_pNode ) return 0;

    return m_pNode->params.size();
}


//------------------------------------------------------------------------
// Get the parameter's name at this node.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the parameter.
//------------------------------------------------------------------------
const char* FlyXmlReader::XmlNodeAccess::GetParamName( int nIndex )
{
    if( !m_pNode ) return NULL;

    if( nIndex < 0 || nIndex >= m_pNode->params.size() )
        return NULL;

    return m_pNode->params[nIndex].name.c_str();
}


//------------------------------------------------------------------------
// Get the parameter's value at this node.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the parameter.
//------------------------------------------------------------------------
const char* FlyXmlReader::XmlNodeAccess::GetParamValue( int nIndex )
{
    if( !m_pNode ) return NULL;

    if( nIndex < 0 || nIndex >= m_pNode->params.size() )
        return NULL;

    return m_pNode->params[nIndex].value.c_str();
}


//------------------------------------------------------------------------
// Get the parameter's value by its name.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the parameter.
//------------------------------------------------------------------------
const char* FlyXmlReader::XmlNodeAccess::GetParamValueByName( const char* cName )
{
    if( !m_pNode ) return NULL;

    for( int i=0;i<m_pNode->params.size();i++ )
    {
        if( m_pNode->params[i].name == cName )
            return m_pNode->params[i].value.c_str();
    }

    return NULL;
}


//------------------------------------------------------------------------
// Get the parent node of the current node.
//------------------------------------------------------------------------
FlyXmlReader::XmlNodeAccess FlyXmlReader::XmlNodeAccess::GetParent(void)
{
    if( !m_pNode ) return NULL;

    return FlyXmlReader::XmlNodeAccess( m_pNode->parent );
}


//------------------------------------------------------------------------
// Is the node a root node for the xml document ?
//------------------------------------------------------------------------
bool FlyXmlReader::XmlNodeAccess::IsRoot(void)
{
    if( m_pNode->parent )
        return false;
    else
        return true;
}


//------------------------------------------------------------------------
// Get the count of the children.
//------------------------------------------------------------------------
size_t FlyXmlReader::XmlNodeAccess::GetChildCount(void)
{
    if( !m_pNode ) return 0;

    return m_pNode->childs.size();
}


//------------------------------------------------------------------------
// Get the child node by its index.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the child node.
//------------------------------------------------------------------------
FlyXmlReader::XmlNodeAccess FlyXmlReader::XmlNodeAccess::GetChild( int nIndex )
{
    if( !m_pNode )
        return FlyXmlReader::XmlNodeAccess( NULL );

    if( nIndex < 0 || nIndex >= m_pNode->childs.size() )
        return FlyXmlReader::XmlNodeAccess( NULL );
    else
        return FlyXmlReader::XmlNodeAccess( m_pNode->childs[nIndex] );
}


//------------------------------------------------------------------------
// Get the child node by its name.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the child node.
//------------------------------------------------------------------------
FlyXmlReader::XmlNodeAccess FlyXmlReader::XmlNodeAccess::GetChildByName( const char* cName )
{
    std::string str;

    if( !m_pNode )
        return FlyXmlReader::XmlNodeAccess( NULL );

    for( int i=0;i<m_pNode->childs.size();i++ )
    {
        str = m_pNode->childs[i]->name;

        if( str == cName )
            return FlyXmlReader::XmlNodeAccess( m_pNode->childs[i] );
    }

    return FlyXmlReader::XmlNodeAccess( NULL );
}