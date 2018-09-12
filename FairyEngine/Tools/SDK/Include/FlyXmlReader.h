//------------------------------------------------------------------------
// Name: FlyXmlReader.h
// Desc: This fiel define a class to load the xml document.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_XMLREADER_H__
#define __FLY_XMLREADER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"

#include "stl_headers.h"
//////////////////////////////////////////////////////////////////////////


class _FLY_EXPORT FlyXmlReader
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

    public: class XmlNodeAccess;

protected:
    sXmlNode* m_pRoot;

    // Process the xml nodes.
    void ProcessXmlNode( const void* pElement,sXmlNode* pNode );

public:
    // Constructor and destructor.
    FlyXmlReader(void);
    ~FlyXmlReader(void);

    // Load the xml data.
    HRESULT LoadDocument( const char* cXmlDoc );
    HRESULT ParseFromBuffer( const void* pContent );

    // Access the xml's content.
    XmlNodeAccess GetRootNode(void);

    // Define a sub-class to acesss the xml's content.
    class _FLY_EXPORT XmlNodeAccess
    {
        friend class FlyXmlReader;

    protected:
        sXmlNode* m_pNode;      // Pointer to the Xml node.

    public:
        XmlNodeAccess(void);
        ~XmlNodeAccess(void);

        const char* GetName(void) const;
        const char* GetText(void) const;

        // Get the parameters.
        size_t GetParamCount(void) const;
        const char* GetParamName( UINT nIndex ) const;
        const char* GetParamValue( UINT nIndex ) const;
        const char* GetParamValueByName( const char* cName ) const;

        // Get the parent node.
        XmlNodeAccess GetParent(void) const;
        size_t GetChildCount(void) const;
        XmlNodeAccess GetChild( UINT nIndex ) const;
        XmlNodeAccess GetChildByName( const char* cName ) const;

        bool IsRoot(void) const;

    protected:
        explicit XmlNodeAccess( sXmlNode* pNode );
    };
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_XMLREADER_H__