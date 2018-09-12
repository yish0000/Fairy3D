/*
 * ------------------------------------------------------------------------
 *  Name:   FXMLReader.cpp
 *  Desc:   This file implements the xml document parser for engine.
 *  Author: Yish
 *  Date:   2010/11/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FVFile.h"
#include "FXMLReader.h"
#include "FLogManager.h"
#include "FStrUtil.h"

#include "tinyxml/tinyxml.h"

/** Constructor
*/
FXMLReader::FXMLReader(void) : m_pDoc(NULL), m_pRoot(NULL)
{
}

/** Destructor
*/
FXMLReader::~FXMLReader(void)
{
	Close();
}

/** Load the xml document.
@Param Path to the XML document.
*/
bool FXMLReader::LoadDocument( const char* filename )
{
	// Load the xml file.
	FVFile file;
	if( !file.Open(filename,FVFile::VFILE_OPENEXIST) )
	{
		FLOG_WARNINGF( "FXMLReader::LoadDocument, Load XML file(%s) failed!",filename );
		return false;
	}

	size_t nSize = file.GetFileSize();
	char* pBuffer = (char*)F_MALLOC_TEMP( nSize );
	file.Read( pBuffer, nSize );
	file.Close();

	// Parse the data from buffer.
	if( !ParseFromBuffer(pBuffer) )
	{
		F_FREE_TEMP( pBuffer );
		return false;
	}

	F_FREE_TEMP( pBuffer );
	return true;
}

/** Parse the xml data from memory stream.
@Param Pointer to the string buffer.
*/
bool FXMLReader::ParseFromBuffer( const char* strContent )
{
	TiXmlNode* pNode;
	TiXmlNode* pRoot = NULL;

	// Close the current document.
	Close();

	// Parse the data from memory stream.
	m_pDoc = new TiXmlDocument();
	if( !m_pDoc->Parse(strContent) )
	{
		FLOG_ERRORF("FXMLReader::ParseFromBuffer(), Parse the xml data from buffer failed!\n Error Info: %s", m_pDoc->ErrorDesc());
		return false;
	}

	// Search the root node.
	pNode = m_pDoc->FirstChild();
	if( pNode->Type() == TiXmlNode::TINYXML_ELEMENT )
		pRoot = pNode;
	while( (pNode = pNode->NextSibling()) )
	{
		if( pNode->Type() == TiXmlNode::TINYXML_ELEMENT )
		{
			// We must only have one root node !
			if( pRoot )
			{
                FLOG_ERROR( "FXMLReader::ParseFromBuffer(), The document can only have one root node!" );
				F_SAFE_DELETE( m_pDoc );
				return false;
			}

			pRoot = pNode;
		}
	}

	// Found the root node.
	if( !pRoot )
	{
		FLOG_ERROR( "FXMLReader::ParseFromBuffer, Can't find the root node!" );
		return false;
	}

	m_pRoot = pRoot->ToElement();
	return true;
}

/** Close the xml document.
*/
void FXMLReader::Close()
{
	F_SAFE_DELETE( m_pDoc );
	m_pRoot = NULL;
}

///////////////////////////////////////////////////////////////////////////
//// Implement class FXMLReader::XMLNodeVisitor

/** Get the name of this node.
*/
const AString& FXMLReader::XMLNodeVisitor::GetName() const
{
	FASSERT(m_pNode);
	m_sText = m_pNode->Value();
	return m_sText;
}

/** Get the text of this node.
*/
const AString& FXMLReader::XMLNodeVisitor::GetText() const
{
	FASSERT( m_pNode );
	if( m_pNode->GetText() )
		m_sText = m_pNode->GetText();
	else
		m_sText.clear();
	return m_sText;
}

/** Get the count of the parameters.
*/
size_t FXMLReader::XMLNodeVisitor::GetParamCount() const
{
	FASSERT( m_pNode );
	size_t iCount = 0;
	TiXmlAttribute* pFirst = m_pNode->FirstAttribute();
	while( pFirst )
	{
		pFirst = pFirst->Next();
		iCount++;
	}
	return iCount;
}

/** Get the specified parameter's value.
*/
const AString& FXMLReader::XMLNodeVisitor::GetParamValue( size_t index ) const
{
	FASSERT( m_pNode );
	size_t i = 0;
	TiXmlAttribute* pFirst = m_pNode->FirstAttribute();
	while( pFirst )
	{
		if( index == i )
			break;
		pFirst = pFirst->Next();
		i++;
	}

	if( pFirst )
		m_sText = pFirst->Value();
	else
		m_sText.clear();
	return m_sText;
}

/** Get the specified parameter's value.
*/
const AString& FXMLReader::XMLNodeVisitor::GetParamValueByName( const AString& name ) const
{
	FASSERT( m_pNode );
	const char* val = m_pNode->Attribute( name.c_str() );
	if( val )
		m_sText = val;
	else
		m_sText.clear();
	return m_sText;
}

/** Enumerate all the parameters.
*/
void FXMLReader::XMLNodeVisitor::EnumParam( AKeyValueList& paramList ) const
{
	FASSERT( m_pNode );
	AString str1, str2;
	paramList.clear();
	TiXmlAttribute* pFirst = m_pNode->FirstAttribute();
	while( pFirst )
	{
		str1 = pFirst->Name();
		str2 = pFirst->Value();
		paramList[str1] = str2;
		pFirst = pFirst->Next();
	}
}

/** Get the parent node of this.
*/
FXMLReader::XMLNodeVisitor FXMLReader::XMLNodeVisitor::GetParent() const
{
	FASSERT( m_pNode );
	TiXmlElement* pElem = m_pNode->Parent()->ToElement();
	return XMLNodeVisitor( pElem );
}

/** Get the count of child node.
*/
size_t FXMLReader::XMLNodeVisitor::GetChildCount() const
{
	FASSERT( m_pNode );
	size_t iCount = 0;
	TiXmlElement* pElem = m_pNode->FirstChildElement();
	while( pElem )
	{
		pElem = pElem->NextSiblingElement();
		iCount++;
	}
	return iCount;
}

/** Get the specified child node.
*/
FXMLReader::XMLNodeVisitor FXMLReader::XMLNodeVisitor::GetChild( size_t index ) const
{
	FASSERT( m_pNode );
	size_t i = 0;
	TiXmlElement* pElem = m_pNode->FirstChildElement();
	while( pElem )
	{
		if( i == index )
			break;
		pElem = pElem->NextSiblingElement();
		i++;
	}
	return XMLNodeVisitor(pElem);
}

/** Get the specified child node by name.
*/
FXMLReader::XMLNodeVisitor FXMLReader::XMLNodeVisitor::GetChildByName( const AString& name ) const
{
	FASSERT( m_pNode );
	TiXmlElement* pElem = m_pNode->FirstChildElement( name.c_str() );
	return XMLNodeVisitor(pElem);
}

/** Get the first child node.
*/
FXMLReader::XMLNodeVisitor FXMLReader::XMLNodeVisitor::FirstChild() const
{
	FASSERT( m_pNode );
	TiXmlElement* pElem = m_pNode->FirstChildElement();
	return XMLNodeVisitor(pElem);
}

/** Get the last child node.
*/
FXMLReader::XMLNodeVisitor FXMLReader::XMLNodeVisitor::LastChild() const
{
	FASSERT( m_pNode );
	TiXmlNode* pLast = m_pNode->LastChild();
	while( pLast )
	{
		if( pLast->ToElement() )
			break;
		pLast = pLast->PreviousSibling();
	}
	return XMLNodeVisitor(pLast->ToElement());
}

/** Get the previous sibling node.
*/
FXMLReader::XMLNodeVisitor FXMLReader::XMLNodeVisitor::GetPrevNode() const
{
	FASSERT( m_pNode );
	TiXmlNode* pPrev = m_pNode->PreviousSibling();
	while( pPrev )
	{
		if( pPrev->ToElement() )
			break;
		pPrev = pPrev->PreviousSibling();
	}
	return XMLNodeVisitor(pPrev->ToElement());
}

/** Get the next sibling node.
*/
FXMLReader::XMLNodeVisitor FXMLReader::XMLNodeVisitor::GetNextNode() const
{
	FASSERT( m_pNode );
	TiXmlElement* pNext = m_pNode->NextSiblingElement();
	return XMLNodeVisitor(pNext);
}

/** Is This a root node ?
*/
bool FXMLReader::XMLNodeVisitor::IsRoot() const
{
	FASSERT( m_pNode );
	TiXmlNode* pNode = m_pNode->Parent();
	return pNode->ToElement() ? false : true;
}