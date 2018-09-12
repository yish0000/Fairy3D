/*
 * ------------------------------------------------------------------------
 *  Name:   FXMLWriter.cpp
 *  Desc:   This file define a xml document writer for engine.
 *  Author: Yish
 *  Date:   2010/11/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FXMLWriter.h"
#include "FVFile.h"
#include "FStrUtil.h"
#include "FLogManager.h"

#include "tinyxml/tinyxml.h"

/** Constructor
*/
FXMLWriter::FXMLWriter(void)
{
	m_pDoc = new TiXmlDocument();
	m_pCurNode = NULL;

	TiXmlDeclaration* pDecl = new TiXmlDeclaration( "1.0", "utf-8", "" );
	m_pDoc->LinkEndChild( pDecl );
}

/** Destructor
*/
FXMLWriter::~FXMLWriter(void)
{
    F_SAFE_DELETE( m_pDoc );
}

/** Create a new tag.
@Param name of the tag.
*/
void FXMLWriter::OpenTag( const char* tagName )
{
	if( !tagName )
	{
		FLOG_ERROR("FXMLWriter::OpenTag, tag name must have a valid value!");
		return;
	}

	TiXmlElement* pNode = new TiXmlElement(tagName);
	if( !m_pCurNode )
		m_pDoc->LinkEndChild( pNode );
	else
		m_pCurNode->LinkEndChild( pNode );
	m_pCurNode = pNode;
}

/** Write the specified value for current tag.
@Param Name of the attribute.
@Param Value of the attribute.
*/
void FXMLWriter::WriteAttribute( const char* name, const char* val )
{
	if( !name || !val )
	{
		FLOG_ERROR("FXMLWriter::WriteAttribute, name and value must have a valid value!");
		return;
	}

	if( m_pCurNode )
	{
		m_pCurNode->SetAttribute(name, val);
	}
}

/** Write the text for current tag.
@Param Text
@Param Is CDATA Text ?
*/
void FXMLWriter::WriteText( const char* text, bool bCDATA/* =false */ )
{
	if( !text )
	{
		FLOG_ERROR("FXMLWriter::WriteText, text must have a valid value!");
		return;
	}

	if( m_pCurNode )
	{
		TiXmlText* pText = new TiXmlText( text );
		pText->SetCDATA( bCDATA );
		m_pCurNode->LinkEndChild( pText );
	}
}

/** Write a comment for this tag.
*/
void FXMLWriter::WriteComment( const char* comment, bool bInDoc )
{
	if( comment )
	{
		FLOG_ERROR("FXMLWriter::WriteComment, comment must have a valid value!");
		return;
	}

	TiXmlComment* pComment = new TiXmlComment( comment );
	if( m_pCurNode && !bInDoc )
		m_pCurNode->LinkEndChild( pComment );
	else
		m_pDoc->LinkEndChild( pComment );
}

/** Close the current tag.
*/
void FXMLWriter::CloseTag()
{
	if( m_pCurNode )
		m_pCurNode = m_pCurNode->Parent()->ToElement();
}

/** Save the xml document.(UTF-8)
*/
bool FXMLWriter::SaveDocument( const char* filename )
{
	FVFile file;
	if( !file.Open(filename, FVFile::VFILE_CREATENEW|FVFile::VFILE_BINARY) )
	{
		FLOG_WARNINGF( "FXMLWriter::SaveDocument, Create the xml document (%s) failed!", filename );
		return false;
	}

	// Write the BOM
	fbyte UTF8BOM[3] = { 0xEF, 0xBB, 0xBF };
	file.Write( UTF8BOM, 3 );

	TiXmlPrinter printer;
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
	printer.SetLineBreak( "\r\n" );
#endif

	m_pDoc->Accept( &printer );
	file.Write( printer.CStr(), printer.Size() );
	file.Close();
	return true;
}
