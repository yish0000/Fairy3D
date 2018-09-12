//------------------------------------------------------------------------
// Name: FlyConfigFile.h
// Desc: This file define a class to load and save the *.ini config file
//       for the engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_CONFIGFILE_H__
#define __FLY_CONFIGFILE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"
#include "stl_headers.h"
//////////////////////////////////////////////////////////////////////////


class _FLY_EXPORT FlyConfigFile
{
    struct _Item
    {
        char name[64];
        char* data;
    };

    struct _Section
    {
        char name[64];
        std::vector<_Item> items;
    };

    typedef std::vector<_Section> SectionList;

public: enum _OPENMODE { OM_READ,OM_SAVE };

protected:
    FILE* m_pFile;          // Pointer to the file.
    SectionList m_Sections;

    // Check whether the string is a section name.
    bool IsSectionName( const char* cText );

public:
    // Constructor and destructor.
    FlyConfigFile(void);
    ~FlyConfigFile(void);

    void Reset(void);
    bool Load( const char* cFilename );
    void Save( const char* cFilename );

    bool InsertItem( const char* cSection,const char* cItem,const char* cData );
    float GetFloat( const char* cSection,const char* cItem );
    int GetInt( const char* cSection,const char* cItem );
    bool GetBool( const char* cSection,const char* cItem );
    const char* GetString( const char* cSection,const char* cItem );

    int GetSectionCount(void);
    int GetItemCount( int nSection );
    float GetFloatEx( int nSection,int nItem );
    int GetIntEx( int nSection,int nItem );
    bool GetBoolEx( int nSection,int nItem );
    const char* GetStringEx( int nSection,int nItem );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_CONFIGFILE_H__