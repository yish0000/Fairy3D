/*
 * ------------------------------------------------------------------------
 *  Name:   FXMLReader.h
 *  Desc:   This file defines a parser for xml documents.
 *  Author: Yish
 *  Date:   2010/11/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_XMLREADER_H__
#define __FAIRY_XMLREADER_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

class TiXmlDocument;
class TiXmlElement;

/** XML Document Parser
@note XML document is encoding by UTF-8.
*/
class FXMLReader : public FGeneralAlloc
{
public:
	class XMLNodeVisitor;

protected:
	TiXmlDocument* m_pDoc;	// XML document.
	TiXmlElement* m_pRoot;	// Pointer to the root node.

public:
    FXMLReader();
    ~FXMLReader();

    // Load the xml document.
    bool LoadDocument( const char* filename );

    // Parse the data from string stream.
    bool ParseFromBuffer( const char* strContent );

	// Close the xml document.
	void Close();

	// Get the visitor to the root node. (UTF-8 version)
	XMLNodeVisitor GetRootNode() const { return XMLNodeVisitor(m_pRoot); }

	// Visitor to the xml node
	class XMLNodeVisitor
	{
		friend class FXMLReader;

	protected:
		mutable AString m_sText;
		TiXmlElement* m_pNode;

	public:
		XMLNodeVisitor(void) : m_pNode(NULL) {}
		~XMLNodeVisitor(void) {}

		// Get the name of the node.
		const AString& GetName(void) const;
		const AString& GetText(void) const;

		size_t GetParamCount(void) const;
		const AString& GetParamValue( size_t index ) const;
		const AString& GetParamValueByName( const AString& name ) const;
		void EnumParam( AKeyValueList& paramList ) const;

		// Get the visitor of parent node.
		XMLNodeVisitor GetParent(void) const;

		// Get the visitor of the child node.
		size_t GetChildCount(void) const;
		XMLNodeVisitor GetChild( size_t index ) const;
		XMLNodeVisitor GetChildByName( const AString& name ) const;
		XMLNodeVisitor FirstChild(void) const;
		XMLNodeVisitor LastChild(void) const;
		XMLNodeVisitor GetPrevNode(void) const;
		XMLNodeVisitor GetNextNode(void) const;

		// Is me the root node.
		bool IsRoot(void) const;
		// Check whether the node is valid ?
		bool IsValid(void) const { return (m_pNode != NULL); }

	protected:
		explicit XMLNodeVisitor( TiXmlElement* pNode ) : m_pNode(pNode) {}
	};
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_XMLREADER_H__