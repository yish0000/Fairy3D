/*
 * ------------------------------------------------------------------------
 *  Name:   FXMLWriter.h
 *  Desc:   This file define a xml document writer for engine.
 *  Author: Yish
 *  Date:   2010/11/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_XMLWRITER_H__
#define __FAIRY_XMLWRITER_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

class TiXmlDocument;
class TiXmlElement;

/** XML documents
@remarks
	XML document will be saved as UTF-8(with BOM).
*/
class FXMLWriter : public FGeneralAlloc
{
protected:
	TiXmlDocument* m_pDoc;		// XML document.
	TiXmlElement* m_pCurNode;	// Current node.

public:
    FXMLWriter(void);
    ~FXMLWriter(void);

    // Save the xml documents.
    bool SaveDocument( const char* filename );

    // Create a new tag.
    void OpenTag( const char* tagName );

    // Write a attribute for this tag.
    void WriteAttribute( const char* name, const char* val );

    // Write some text for this tag.
	// Param [0] Text
	// Param [1] CDATA Text, if this parameter is true, we will not handle "&lt;" ...
    void WriteText( const char* text, bool bCDATA = false );

	// Write the comment for current tag.
    void WriteComment( const char* comment, bool bInDoc=false );

	// Close the current tag.
	void CloseTag(void);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_XMLWRITER_H__