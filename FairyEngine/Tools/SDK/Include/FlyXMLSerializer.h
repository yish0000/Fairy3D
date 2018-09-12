//------------------------------------------------------------------------
// Name: FlyXMLSerializer.h
// Desc: This file define a class to write the xml document.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_XML_SERIALIZER_H__
#define __FLY_XML_SERIALIZER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"
#include "stl_headers.h"

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// A class to write the xml document.
class _FLY_EXPORT FlyXmlSerializer
{
    struct sXmlParam
    {
        std::string name;
        std::string value;
    };

    struct sXmlNode
    {
        std::string name;
        std::vector<sXmlParam> params;
        std::string text;
        bool bHaveText;

        std::vector<sXmlNode*> childs;
        sXmlNode* parent;

        sXmlNode(void);
        ~sXmlNode(void);
    };

protected:
    FILE*       m_pFile;        // Pointer to the file.
    int         m_nIndent;      // Number of spaces for indent.
    UINT        m_nTagCount;    // Number of tags.
    sXmlNode*   m_pRoot;        // Root node of the xml document.
    sXmlNode*   m_pCurNode;     // Current node.

    void WriteXmlNode( sXmlNode* pNode,int nDepth );
    void IndentLine( int nDepth );

public:
    // Constructor and destructor.
    FlyXmlSerializer( int nIndent = 4 );
    ~FlyXmlSerializer(void);

    // Create a new document.
    bool OpenDocument( const char* cXmlDoc );

    // Close the document.
    void CloseDocument(void);

    // Flush the xml nodes.
    void FlushDocument(void);

    // Start a new tag in the xml document.
    void OpenTag( const char* cTagName );

    // Close the current tag.
    void CloseTag(void);

    // Write an attribute for current tag.
    void WriteAttribute( const char* cAttribName,const char* cValue );

    // Write the text between the begin and end tag.
    void WriteText( const char* cText );

    // Write a comment for document.
    void WriteComment( const char* cComment );

    // Begin a new line for the document.
    void NewLine(void);
    
    // Get the number of tags.
    UINT GetTagCount(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_XML_SERIALIZER_H__