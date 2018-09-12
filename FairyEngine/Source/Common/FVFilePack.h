/*
 * ------------------------------------------------------------------------
 *  Name:   FVFilePack.h
 *  Desc:   This file define the base class for file package.
 *  Author: Yish
 *  Date:   2010/12/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_VFILEPACK_H__
#define __FAIRY_VFILEPACK_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

/** File information in the package.
*/
struct VFileInfo
{
    FVFilePack* pFilePack;      // Pointer to the file package.
    AString sFileName;          // Entire name of the file(Include folder)
    AString sPath;              // Folder of the file.
    AString sBaseName;          // Basic name of the file.
    size_t nCompressedSize;     // Compressed size of the file.
    size_t nUncompressedSize;   // Uncompressed size of the file.
};

typedef std::vector<VFileInfo> VFileInfoList;

// Callback function when handle the package.
typedef bool (*CB_PACKAGEOP) ( const AString& filename );

/** File package.
*/
class FVFilePack : public FGeneralAlloc
{
public:

	// Option when adding files.
    enum
    {
        ADD_ENTRY_FOLDER,   // Add a folder.
        ADD_ENTRY_REPLACE,  // Replace the file which have a same name.
    };

protected:
    AString m_sName;    // Name of the file package.
    AString m_sType;    // Type of the file package.
    bool m_bReadonly;   // The file package is readonly. (*.zip is readonly)
    bool m_bDirty;      // Is the package's data dirty.

public:
    // Constructor and destructor.
    FVFilePack( const AString& name,const AString& packType );
    virtual ~FVFilePack(void);

	// Load the file package.
    virtual bool Load(void) = 0;
	// Unload the file package.
    virtual void Unload(void) = 0;

	// Add a file's data to the package.
    virtual int AddEntry( const AString& entry, uint32 nSize, void* data, uint32 flags ) = 0;
	// Add a file to the package.
	virtual int AddEntry( const AString& entry, const AString& filename, uint32 flags ) = 0;

	// Delete the specified file.
    virtual void Delete( const AString& path ) = 0;

	// Open the specified file in the package.
    virtual FVFileStream* Open( const AString& filename ) = 0;

    // Arrange the fragment of file package(it's slow)
    virtual bool DeFragment( CB_PACKAGEOP pCallBack ) = 0;

	// Get the size of fragments.
    virtual uint32 GetFragmentSize(void) const = 0;

	// Flush the file package.
    virtual void Flush(void) = 0;

	// List the file informations.
    virtual void ListFiles( AStringList& fileList, bool bRecurse=true, bool bDir=true ) = 0;
    virtual void ListFiles( VFileInfoList& fileList, bool bRecurse=true, bool bDir=true ) = 0;
    virtual void FindFiles( AStringList& fileList, const AString& pattern, bool bRecurse=true, bool bDir=true ) = 0;
    virtual void FindFiles( VFileInfoList& fileList, const AString& pattern, bool bRecurse=true, bool bDir=true ) = 0;

	// Whether the file exists ?
    virtual bool Exists( const AString& filename ) = 0;

	// Get the specified file information.
    virtual void GetFileInfo( const AString& filename, VFileInfo& info ) = 0;

    // Is the package readonly ?
    bool IsReadonly(void) const { return m_bReadonly; }
    // Is the data dirty ?
    bool IsDirty(void) const { return m_bDirty; }
    // Get the package name.
    const AString& GetName(void) const { return m_sName; }
    // Get the package type.
    const AString& GetPackType(void) const { return m_sType; }
};

//////////////////////////////////////////////////////////////////////////

/** Factory of the file package.
*/
class FVFilePackFactory : public FGeneralAlloc
{
protected:
    AString m_sType;    // Type of the file package.

public:
    // Constructor and destructor.
    FVFilePackFactory(void) {};
    virtual ~FVFilePackFactory(void) {};

    virtual FVFilePack* CreateInstance( const AString& name ) = 0;
    virtual void DestroyInstance( FVFilePack* pPack ) = 0;

    const AString& GetType(void) const { return m_sType; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_VFILEPACK_H__