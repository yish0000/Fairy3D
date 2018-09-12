/*
 * ------------------------------------------------------------------------
 *  Name:   FFpkFilePack.h
 *  Desc:   This file define the fpk file package system for engine.
 *  Author: Yish
 *  Date:   2011/9/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft Studio 2011. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_FPKFILEPACK_H__
#define __FAIRY_FPKFILEPACK_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FVFilePack.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// Define and macros

// Version of the fpk file.
#define FPK_CURRENT_VERSION 0x0100

// Compression flag(Package).
#define FPK_COMP_ZLIB       0x00000001  // ZLib
#define FPK_COMP_MINILZO    0x00000002  // MiniLZO

// Compression quality(Package).
#define FPK_QUALITY_NONE    0x00000100  // Does not compress the data.
#define FPK_QUALITY_LOW     0x00000200  // Each 256KB as a group will be compressed.
#define FPK_QUALITY_NORMAL  0x00000300  // Each 1024KB as a group will be compressed.
#define FPK_QUALITY_HIGH    0x00000400  // Each 4096KB as a group will be compressed.
#define FPK_QUALITY_ENTIRE  0x00000500  // Compress the entire file.

// Some flags for file.
#define FPK_FILE_REPLACE    0x00000001  // Replace the old file when existed a file with the same name.
#define FPK_FILE_DELETED    0x00000002  // The file is deleted.

// Buffer size for each compression quality.
const uint32 BLOCK_LOW = 256 * 1024;
const uint32 BLOCK_NORMAL = 1024 * 1024;
const uint32 BLOCK_HIGH = 4096 * 1024;

///////////////////////////////////////////////////////////////////////////
// Local types 

/** FPK file header.
*/
struct SFPKHeader
{
    uint32 nFourcc;             // File BOM.
    uint32 nVersion;            // FPK package version.
    uint32 nEntryOffset;        // The offset of file entry data.
    uint32 nNumEntries;         // Count of file entries.
    uint32 nFilenameOffset;     // The offset of file name data.
    uint32 nFilenameSize;       // Size of file name data.
    uint32 nFlags;              // Some special flags.
};

/** Each file entry
*/
struct SFPKFileEntry
{
    uint32 nHash0;              // Hash code.
    uint32 nHash1;              // The second hash code. (Avoid hash collision)
    uint32 nOffset;             // The offset of this file's data.
    uint32 nCompressedSize;     // Size of the compressed file.
    uint32 nOriginalSize;       // Size of the original file.
    uint32 nFlag;               // Some special flags.
};

///////////////////////////////////////////////////////////////////////////

/** FPK package.
*/
class FFpkFilePack : public FVFilePack
{
    friend class FFpkFileStream;

    typedef std::vector<SFPKFileEntry> FileEntryList;
	typedef std::unordered_map<uint32, int> FileEntryMap;

protected:
    FILE* m_pFile;                      // File pointer to the fpk.
    SFPKHeader m_FileHeader;            // File header.
    FileEntryList m_Entries;            // File entry data.
    AStringList m_FilenameList;         // File name list for each entries.
	FileEntryMap m_Hashmap;				// Hash map (hashcode -> index).

public:
    FFpkFilePack( const AString& name, const AString& packType );
    ~FFpkFilePack(void);

    bool Load(void);
    void Unload(void);

    int AddEntry( const AString& entry, uint32 nSize, void* data, uint32 flags );
	int AddEntry( const AString& entry, const AString& filename, uint32 flags );
    void Delete( const AString& path );
    FVFileStream* Open( const AString& filename );
    bool DeFragment( CB_PACKAGEOP pCallBack );
    uint32 GetFragmentSize(void) const;
    void Flush(void);
    void ListFiles( AStringList& fileList, bool bRecurse, bool bDir );
    void ListFiles( VFileInfoList& fileList, bool bRecurse, bool bDir );
    void FindFiles( AStringList& fileList, const AString& pattern, bool bRecurse/* =true */, bool bDir/* =true */ );
    void FindFiles( VFileInfoList& fileList, const AString& pattern, bool bRecurse/* =true */, bool bDir/* =true */ );
    bool Exists( const AString& filename );
    void GetFileInfo( const AString& filename, VFileInfo& info );

private:
    // Build the hash table for this package.
    bool BuildHashTable(void);

    // Get the entry index of specified file path.
    int GetFileIndex( const AString& path );

    // Calculate the hash code for filename.
    uint32 GetStringHash( const AString& str, uint32 seed ) const;

    // Encode the filename.
    void EncodeFilename( AString& str );
    void DecodeFilename( AString& str );
};

/** FPK package factory.
*/
class FFpkFilePackFactory : public FVFilePackFactory
{
public:
    FFpkFilePackFactory(void) { m_sType = "fpk"; }
    ~FFpkFilePackFactory(void) {};

    FVFilePack* CreateInstance( const AString& name );
    void DestroyInstance( FVFilePack* pPack );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_FPKFILEPACK_H__