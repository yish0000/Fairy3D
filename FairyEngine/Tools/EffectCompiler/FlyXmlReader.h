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


class FlyXmlReader
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

        sXmlNode(void) { /* Nothing to do! */ };
        ~sXmlNode(void)
        {
            for( int i=0;i<childs.size();i++ )
            {
                delete childs[i];
            }
        }
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
    class XmlNodeAccess
    {
        friend class FlyXmlReader;

    protected:
        sXmlNode* m_pNode;      // Pointer to the Xml node.

    public:
        XmlNodeAccess(void);
        ~XmlNodeAccess(void);

        const char* GetName(void);
        const char* GetText(void);

        // Get the parameters.
        size_t GetParamCount(void);
        const char* GetParamName( int nIndex );
        const char* GetParamValue( int nIndex );
        const char* GetParamValueByName( const char* cName );

        // Get the parent node.
        XmlNodeAccess GetParent(void);
        size_t GetChildCount(void);
        XmlNodeAccess GetChild( int nIndex );
        XmlNodeAccess GetChildByName( const char* cName );

        bool IsRoot(void);

    protected:
        explicit XmlNodeAccess( sXmlNode* pNode );
    };
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_XMLREADER_H__