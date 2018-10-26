/*
 * ------------------------------------------------------------------------
 *  Name:   FFpkFilePack.cpp
 *  Desc:   This file implements the class to load the fairy package.
 *  Author: Yish
 *  Date:   2011/10/3
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FFpkFilePack.h"
#include "FFpkFileStream.h"
#include "FLogManager.h"
#include "FStrUtil.h"
#include "FCompress.h"
#include "FSysAPI.h"

///////////////////////////////////////////////////////////////////////////
//  
//  Local types and global variables
//  
///////////////////////////////////////////////////////////////////////////

// Seed for calculating the hash code.
const uint32 HASH_SEED0 = 31;
const uint32 HASH_SEED1 = 1313;

///////////////////////////////////////////////////////////////////////////

/** Constructor.
*/
FFpkFilePack::FFpkFilePack( const AString& name,const AString& packType )
    : FVFilePack(name,packType),m_pFile(NULL)
{
    m_FileHeader.nFourcc = F_MAKEFOURCC('F','P','A','K');
    m_FileHeader.nVersion = FPK_CURRENT_VERSION;
    m_FileHeader.nEntryOffset = sizeof(SFPKHeader);
    m_FileHeader.nNumEntries = 0;
    m_FileHeader.nFilenameOffset = sizeof(SFPKHeader);
    m_FileHeader.nFilenameSize = 0;
}

/** Destructor.
*/
FFpkFilePack::~FFpkFilePack(void)
{
    Unload();
}

/** Load the specified package.
*/
bool FFpkFilePack::Load(void)
{
    // Check whether the file is readonly ?
    m_bReadonly = (FSysAPI::AccessFile(m_sName.c_str(), 2) == -1);

    // Open the package file.
    if( !m_bReadonly )
        m_pFile = fopen( m_sName.c_str(), "r+b" );
    else
        m_pFile = fopen( m_sName.c_str(), "rb" );
    if( !m_pFile )
    {
        FLOG_WARNINGF("FFpkFilePack::Load(), Load package(%s) failed, not exist!", m_sName.c_str());
        return false;
    }

    // Read the header of the package.
    fread(&m_FileHeader, sizeof(SFPKHeader), 1, m_pFile);
    if( m_FileHeader.nFourcc != F_MAKEFOURCC('F', 'P', 'A', 'K') )
    {
        fclose( m_pFile );
		FLOG_WARNING( "FFpkFilePack::Load(), Load package(" + m_sName + ") failed, Invalid format!" );
        return false;
    }

    // The package's version is too new, we can't load it.
    if( m_FileHeader.nVersion > FPK_CURRENT_VERSION )
    {
        fclose( m_pFile );
		FLOG_WARNING( "FFpkFilePack::Load(), The version of the package(" + m_sName + ") is too new!!" );
        return false;
    }

    // Read the file entries.
    if( m_FileHeader.nNumEntries )
    {
        m_Entries.resize( m_FileHeader.nNumEntries );
        fseek_64( m_pFile,m_FileHeader.nEntryOffset,SEEK_SET );
        size_t nRead = fread( &m_Entries[0],sizeof(SFPKFileEntry),m_FileHeader.nNumEntries,m_pFile );
		if (nRead != m_FileHeader.nNumEntries)
		{
			fclose(m_pFile);
			FLOG_WARNING("FFpkFilePack::Load(), Reading file entries error! Package=" + m_sName);
			return false;
		}
    }

    // Read the file name list.
	m_FilenameList.resize(m_FileHeader.nNumEntries);
	for (uint32 i = 0; i < m_FileHeader.nNumEntries; ++i)
	{
		uchar len;
		AString& str = m_FilenameList[i];
		fread(&len, sizeof(uchar), 1, m_pFile);
		
		str.resize(len, '\0');
		size_t nRead = fread(&str[0], 1, len, m_pFile);
		if (nRead != len)
		{
			fclose(m_pFile);
			FLOG_WARNING("FFpkFilePack::Load(), Reading file names error! Package=" + m_sName);
			return false;
		}

		DecodeFilename(str);
	}

    // Build the hash table.
    BuildHashTable();

    return true;
}

/** Unload the file package.
*/
void FFpkFilePack::Unload(void)
{
    Flush();

    if( m_pFile )
    {
        fclose( m_pFile );
        m_pFile = NULL;

        m_Entries.clear();
        m_FilenameList.clear();
    }
}

// Add a file to the package.
// Param[1] Name of the file.
// Param[2] Size of the file.
// Param[3] Content of the file.
// Param[4] Flag of the new file. (Compress type, Compress quality ...）
// Return The size of compressed file. (Return -1 when failed to add file)
int FFpkFilePack::AddEntry( const AString& entry, uint32 nSize, void* data, uint32 flags )
{
    if( m_bReadonly )
    {
        FLOG_ERRORF( "FFpkFilePack::AddEntry(), Package(%s) is readonly, failed to add files!", m_sName.c_str() );
        return -1;
    }
    
    if( entry.length() > 255 )
    {
        FLOG_ERROR("FFpkFilePack::AddEntry(), The file name is too long!");
        return -1;
    }
    
    SFPKFileEntry newFile;
    newFile.nFlag = flags & 0xffff;
    newFile.nHash0 = GetStringHash( entry,HASH_SEED0 );
    newFile.nHash1 = 0;
    newFile.nOriginalSize = nSize;
    newFile.nOffset = m_FileHeader.nEntryOffset;

	// Create the compressor.
	std::auto_ptr<FCompressor> pCompress(FCompressor::Create((FCompressor::ECompressType)(flags & 0xff)));
	if( !pCompress.get() )
	{
		FLOG_ERRORF("FFpkFilePack::AddEntry, Create compressor failed, failed to add file(%s)!", entry.c_str());
		return -1;
	}

    int fileIndex = GetFileIndex( entry );
    if( fileIndex >= 0 )
    {
        SFPKFileEntry& oldFile = m_Entries[fileIndex];

        // Whether the old file name is same with the new one.
        if( m_FilenameList[fileIndex] == entry )
        {
            if( !(flags & FPK_FILE_REPLACE) )
			{
				FLOG_ERROR( "FFpkFilePack::AddEntry(), Add file (" + entry + ") failed, Already have a same file!" );
				return -1;
			}
            else
            {
                // Delete the old file.
                oldFile.nFlag |= FPK_FILE_DELETED;
            }
        }
        else
        {
            // Hash code duplicated!
            if( m_Entries[fileIndex].nHash1 != 0 )
            {
                FASSERT(0);
                FLOG_ERROR( "FFpkFilePack::AddEntry(), Add file(" + entry + ") failed, hash duplicated!!" );
                return -1;
            }

            m_Entries[fileIndex].nHash1 = GetStringHash( m_FilenameList[fileIndex],HASH_SEED1 );
            newFile.nHash1 = GetStringHash( entry,HASH_SEED1 );
        }
    }

    // Write the data to the tail.
    uint32 compressedSize = 0;
    uint32 quality = flags & 0xff00;
	if( nSize > 0 )
	{
		if( quality == FPK_QUALITY_NONE )
		{
			// Does not compress the file.
			compressedSize = nSize;
			fseek_64( m_pFile, newFile.nOffset, SEEK_SET );
			fwrite( data, nSize, 1, m_pFile );
		}
		else if( quality == FPK_QUALITY_ENTIRE )
		{
			// Compress the entire file.
			compressedSize = pCompress->CompressBound( nSize );
			FBYTE* dest = (FBYTE*)F_MALLOC( compressedSize );
			FASSERT_RET2( dest, false );
			pCompress->Compress( (FBYTE*)data, nSize, dest, compressedSize );
			fseek_64( m_pFile, newFile.nOffset, SEEK_SET );
			fwrite( dest, compressedSize, 1, m_pFile );
			F_FREE( dest );
		}
		else
		{
			// Compress the each blocks.
			FBYTE* tempMem = NULL;
			uint32 numBlocks, blockSize, lastSize, curCompSize;

			switch( quality )
			{
			case FPK_QUALITY_LOW:
				blockSize = BLOCK_LOW;
				break;
			case FPK_QUALITY_NORMAL:
				blockSize = BLOCK_NORMAL;
				break;
			case FPK_QUALITY_HIGH:
				blockSize = BLOCK_HIGH;
				break;
			default: FASSERT(0); break;
			}

			numBlocks = nSize / blockSize;
			lastSize = nSize % blockSize;
			if( lastSize ) ++numBlocks;
			uint32 destSize = pCompress->CompressBound( blockSize );
			tempMem = new FBYTE[destSize];
			FASSERT( tempMem );
			fseek_64( m_pFile, m_FileHeader.nEntryOffset, SEEK_SET );
			FBYTE* pCurOffset = (FBYTE*)data;
			while( numBlocks > 0 )
			{
				uint32 curBlkSize = ((numBlocks != 1) || !lastSize) ? blockSize : lastSize;
				curCompSize = destSize;
				pCompress->Compress( (FBYTE*)pCurOffset, curBlkSize, tempMem, curCompSize );
				fwrite( &curCompSize, sizeof(uint32), 1, m_pFile );
				fwrite( tempMem, curCompSize, 1, m_pFile );
				compressedSize += sizeof(uint32) + curCompSize;
				pCurOffset += curBlkSize;
				--numBlocks;
			}

			FASSERT( pCurOffset == reinterpret_cast<FBYTE*>(data) + nSize );
			F_SAFE_DELETE_ARRAY( tempMem );
		}
	}

	// Move the offset of entry data.
    newFile.nCompressedSize = compressedSize;
    m_FileHeader.nEntryOffset += newFile.nCompressedSize;
    fflush( m_pFile );

    m_Entries.push_back( newFile );
    m_FilenameList.push_back( entry );
    m_bDirty = true;
    return compressedSize;
}

// Add a file to the package.
// Param[1] Name of the file.
// Param[2] Path of the source file.
// Param[3] Flag of the new file. (Compress type, Compress quality ...）
// Return The size of compressed file. (Return 0 when failed to add file)
int FFpkFilePack::AddEntry( const AString& entry, const AString& filename, uint32 flags )
{
	if( m_bReadonly )
	{
		FLOG_ERRORF( "FFpkFilePack::AddEntry(), Package(%s) is readonly, failed to add files!", m_sName.c_str() );
		return -1;
	}

	if( entry.length() > 255 )
	{
		FLOG_ERROR("FFpkFilePack::AddEntry(), The file name is too long!");
		return -1;
	}

	if( !FSysAPI::IsFileExist(filename.c_str()) )
	{
		FLOG_ERRORF("FFpkFilePack::AddEntry(), The specified file(%s) does not exist!", filename.c_str());
		return -1;
	}

	uint32 nSrcFileSize = FSysAPI::GetFileSize(filename.c_str());
	if( nSrcFileSize > 1024 * 1024 * 1024 )
	{
		FLOG_ERROR("FFpkFilePack::AddEntry(), The file is too large, Failed to append!");
		return -1;
	}

	// We cannot compress the entire file when it's too large!!
	if( nSrcFileSize > 1024 * 1024 * 64 )
	{
		if( (flags & 0xff00) == FPK_QUALITY_ENTIRE )
			flags = (flags & 0xffff00ff) | FPK_QUALITY_HIGH;
	}

	SFPKFileEntry newFile;
	newFile.nFlag = flags & 0xffff;
	newFile.nHash0 = GetStringHash( entry, HASH_SEED0 );
	newFile.nHash1 = 0;
	newFile.nOriginalSize = nSrcFileSize;
	newFile.nOffset = m_FileHeader.nEntryOffset;

	// Create the compressor.
	std::auto_ptr<FCompressor> pCompress(FCompressor::Create((FCompressor::ECompressType)(flags & 0xff)));
	if( !pCompress.get() )
	{
		FLOG_ERRORF("FFpkFilePack::AddEntry, Create compressor failed, failed to add file(%s)!", entry.c_str());
		return -1;
	}

	int fileIndex = GetFileIndex( entry );
	if( fileIndex >= 0 )
	{
		SFPKFileEntry& oldFile = m_Entries[fileIndex];

		// Whether the old file name is same with the old one.
		if( m_FilenameList[fileIndex] == entry )
		{
			if( !(flags & FPK_FILE_REPLACE) )
			{
				FLOG_ERROR( "FFpkFilePack::AddEntry(), Add file (" + entry + ") failed, Already have a same file!" );
				return -1;
			}
			else
			{
				// Delete the old file.
				oldFile.nFlag |= FPK_FILE_DELETED;
			}
		}
		else
		{
			// Hash code duplicated!!
			if( m_Entries[fileIndex].nHash1 != 0 )
			{
				FASSERT(0);
				FLOG_ERROR( "FFpkFilePack::AddEntry(), Add file(" + entry + ") failed, hash duplicated!!" );
				return -1;
			}

			m_Entries[fileIndex].nHash1 = GetStringHash( m_FilenameList[fileIndex],HASH_SEED1 );
			newFile.nHash1 = GetStringHash( entry,HASH_SEED1 );
		}
	}

	// Now we start to write the data to the tail of package.
	uint32 compressedSize = 0;
	uint32 quality = flags & 0xff00;
	if( nSrcFileSize > 0 )
	{
		if( quality == FPK_QUALITY_ENTIRE )
		{
			// Load the source file.
			FILE* pSrcFile = fopen(filename.c_str(), "rb");
			FBYTE* src_temp = (FBYTE*)F_MALLOC_TEMP(nSrcFileSize);
			fread(src_temp, sizeof(FBYTE), nSrcFileSize, pSrcFile);
			fclose(pSrcFile);

			// Compress the data.
			compressedSize = pCompress->CompressBound( nSrcFileSize );
			FBYTE* dest = (FBYTE*)F_MALLOC_TEMP( compressedSize );
			FASSERT_RET2( dest, false );
			pCompress->Compress( src_temp, nSrcFileSize, dest, compressedSize );
			fseek_64( m_pFile, newFile.nOffset, SEEK_SET );
			fwrite( dest, compressedSize, 1, m_pFile );

			F_FREE_TEMP( dest );
			F_FREE_TEMP( src_temp );
		}
		else
		{
			// Compress the each blocks.
			FBYTE* tempMem = NULL;
			FBYTE* srcTempMem = NULL;
			uint32 numBlocks, blockSize, lastSize, curCompSize;

			switch( quality )
			{
			case FPK_QUALITY_LOW:
				blockSize = BLOCK_LOW;
				break;
			case FPK_QUALITY_NORMAL:
			case FPK_QUALITY_NONE:			// If the file does not compress, we use the normal buffer to read data.
				blockSize = BLOCK_NORMAL;
				break;
			case FPK_QUALITY_HIGH:
				blockSize = BLOCK_HIGH;
				break;
			default: FASSERT(0); break;
			}

			FILE* pSrcFile = fopen(filename.c_str(), "rb");
			numBlocks = nSrcFileSize / blockSize;
			lastSize = nSrcFileSize % blockSize;
			if( lastSize ) ++numBlocks;
			srcTempMem = (FBYTE*)F_MALLOC_TEMP(blockSize);
			uint32 destSize = pCompress->CompressBound( blockSize );
			tempMem = (FBYTE*)F_MALLOC_TEMP(destSize);
			fseek_64( m_pFile, m_FileHeader.nEntryOffset, SEEK_SET );
			int curSrcOffset = 0;
			while( numBlocks > 0 )
			{
				uint32 curBlkSize = ((numBlocks != 1) || !lastSize) ? blockSize : lastSize;
				fseek_64(pSrcFile, curSrcOffset, SEEK_SET);
				fread(srcTempMem, sizeof(FBYTE), curBlkSize, pSrcFile);
				curCompSize = destSize;
				pCompress->Compress( srcTempMem, curBlkSize, tempMem, curCompSize );
				fwrite( &curCompSize, sizeof(uint32), 1, m_pFile );
				fwrite( tempMem, curCompSize, 1, m_pFile );
				compressedSize += sizeof(uint32) + curCompSize;
				curSrcOffset += curBlkSize;
				--numBlocks;
			}

			FASSERT( curSrcOffset == nSrcFileSize );
			F_FREE_TEMP( tempMem );
			F_FREE_TEMP( srcTempMem );
		}
	}

	// Move the offset of entry data.
	newFile.nCompressedSize = compressedSize;
	m_FileHeader.nEntryOffset += newFile.nCompressedSize;
	fflush( m_pFile );

	m_Entries.push_back( newFile );
	m_FilenameList.push_back( entry );
	m_bDirty = true;
	return compressedSize;
}

// Delete the specified file.
void FFpkFilePack::Delete( const AString& path )
{
    int fileIndex = GetFileIndex( path );
    if( fileIndex >= 0 )
    {
        m_Entries[fileIndex].nFlag |= FPK_FILE_DELETED;
        m_bDirty = true;
    }
}

// Remove the fragments of this package.
// Param [0] Callback, Recording the current progress.
bool FFpkFilePack::DeFragment( CB_PACKAGEOP pCallBack )
{
    if( m_bReadonly || m_bDirty )
        return false;

    AString tempFile = m_sName + "_tmp";
    FILE* pFile = fopen( tempFile.c_str(), "wb" );
    if( !pFile ) return false;
    fwrite( &m_FileHeader, sizeof(SFPKHeader), 1, pFile );

    const uint32 MIN_CHUNK_SIZE = 0x200000; // 2MB buffer, reduce the count of writting disk.
    std::vector<char> tempBuf;
    uint32 nextPos = sizeof(SFPKHeader);
    uint32 curChkPos = nextPos;
    uint32 curChkSize = 0;
    uint32 fragmentSize = 0;

    for( size_t i=0; i<m_Entries.size(); ++i )
    {
        SFPKFileEntry& entry = m_Entries[i];
        if( pCallBack && !pCallBack(m_FilenameList[i]) )
        {
            fclose( pFile );
            remove( tempFile.c_str() );
            return false;
        }

        if( entry.nCompressedSize == 0 )
        {
            entry.nOffset = nextPos;
            continue;
        }

        if( entry.nFlag & FPK_FILE_DELETED )
            continue;

		// When we find the hole or the buffer is full, we flush the data.
        if( entry.nOffset != fragmentSize + nextPos ||
            curChkSize > MIN_CHUNK_SIZE )
        {
            if( curChkSize > 0 )
            {
                tempBuf.resize( curChkSize );
                fseek_64( m_pFile, curChkPos, SEEK_SET );
                fread( &tempBuf[0], 1, curChkSize, m_pFile );
                fwrite( &tempBuf[0], 1, curChkSize, pFile );
                fflush( pFile );
            }

            fragmentSize = entry.nOffset - nextPos;
            curChkPos = entry.nOffset;
            curChkSize = 0;
        }

        entry.nOffset = nextPos;
        nextPos += entry.nCompressedSize;
        curChkSize += entry.nCompressedSize;
    }

	// The last block.
    if( curChkSize > 0 )
    {
        tempBuf.resize( curChkSize );
        fseek_64( m_pFile, curChkPos, SEEK_SET );
        fread( &tempBuf[0], 1, curChkSize, m_pFile );
        fwrite( &tempBuf[0], 1, curChkSize, pFile );
    }

    fclose( m_pFile );
    fclose( pFile );

	// Calculate the new offset.
    m_FileHeader.nEntryOffset = nextPos;
    m_FileHeader.nFilenameOffset = (uint32)(nextPos + sizeof(SFPKFileEntry) * m_Entries.size());
    m_FileHeader.nNumEntries = (uint32)m_Entries.size();
    m_FileHeader.nFilenameSize = (uint32)m_FilenameList.size();

	// Write entry and file info to the new file.
    m_pFile = fopen( tempFile.c_str(), "r+b" );
    m_bDirty = true;
    Flush();
    fclose( m_pFile );

	// Remove the old file.
    remove( m_sName.c_str() );
    rename( tempFile.c_str(), m_sName.c_str() );
    m_pFile = fopen( m_sName.c_str(), "r+b" );
    FASSERT( m_pFile );
    return true;
}

// Calculate the size of fragments.
uint32 FFpkFilePack::GetFragmentSize(void) const
{
    if( m_bDirty ) return 0;

    uint32 dataSection = m_FileHeader.nEntryOffset - sizeof(SFPKHeader);
    uint32 invalidSize = 0;
    FileEntryList::const_iterator it;
    for( it=m_Entries.begin();it!=m_Entries.end();++it )
    {
        invalidSize += it->nCompressedSize;
    }

    return dataSection - invalidSize;
}

// Flush the package.
void FFpkFilePack::Flush(void)
{
    if( m_bReadonly || !m_bDirty )
        return;

    if( m_Entries.size() == 0 )
        m_FileHeader.nEntryOffset = sizeof(SFPKHeader);
    else
    {
		// Remove the file which deleted.
        FileEntryList newEntries;
        AStringList newFilenames;

        FileEntryList::iterator it;
        AStringList::iterator strIt = m_FilenameList.begin();
        for( it=m_Entries.begin();it!=m_Entries.end();++it,++strIt )
        {
            if( it->nFlag & FPK_FILE_DELETED )
                continue;

            newEntries.push_back( *it );
            newFilenames.push_back( *strIt );
        }

        m_Entries = newEntries;
        m_FilenameList = newFilenames;
    }

	// Write the file entries.
    if( m_Entries.size() )
    {
        fseek_64( m_pFile,m_FileHeader.nEntryOffset,SEEK_SET );
        fwrite( &m_Entries[0],sizeof(SFPKFileEntry),m_Entries.size(),m_pFile );
    }

	// Write the file informations.
    m_FileHeader.nFilenameOffset = (uint32)(m_FileHeader.nEntryOffset + sizeof(SFPKFileEntry)*m_Entries.size());
    fseek_64( m_pFile,m_FileHeader.nFilenameOffset,SEEK_SET );
    AStringList::const_iterator strIt;
    for( strIt=m_FilenameList.begin();strIt!=m_FilenameList.end();++strIt )
    {
        AString str = *strIt;
        EncodeFilename( str );
        uchar len = (uchar)str.length();
        fwrite( &len,sizeof(uchar),1,m_pFile );
        fwrite( str.c_str(),len,1,m_pFile );
    }

	// Re-write the file header.
    m_FileHeader.nNumEntries = (uint32)m_Entries.size();
    m_FileHeader.nFilenameSize = (uint32)m_FilenameList.size();
    fseek_64( m_pFile,0,SEEK_SET );
    fwrite( &m_FileHeader,sizeof(SFPKHeader),1,m_pFile );
    fflush( m_pFile );

    // Rebuild the hash table.
    BuildHashTable();
    m_bDirty = false;
}

// Open the specified file.
FVFileStream* FFpkFilePack::Open( const AString& filename )
{
    int fileIndex = GetFileIndex( filename );
    if( fileIndex < 0 ) return NULL;
    return new FFpkFileStream( filename,m_pFile,&m_Entries[fileIndex] );
}

// Build the hash table for all entries.
bool FFpkFilePack::BuildHashTable(void)
{
    m_Hashmap.clear();
    for( size_t i=0;i<m_Entries.size();++i )
    {
        SFPKFileEntry& entry = m_Entries[i];
        if( entry.nHash1 ) m_Hashmap[entry.nHash1] = (uint32)i;
        m_Hashmap[entry.nHash0] = (uint32)i;
    }

    return true;
}

// Get the index of specified file.
int FFpkFilePack::GetFileIndex( const AString& path )
{
    uint32 hash0,hash1;

    hash0 = GetStringHash( path,HASH_SEED0 );
    if( m_Hashmap.find(hash0) == m_Hashmap.end() )
        return -1;

    int fileIndex = m_Hashmap[hash0];
    const SFPKFileEntry& entry = m_Entries[fileIndex];
    if( entry.nHash1 )
    {
        hash1 = GetStringHash( path,HASH_SEED1 );
        fileIndex = m_Hashmap[hash1];
        return fileIndex;
    }
    else
    {
        return fileIndex;
    }
}

// Calculate the hash code of specified file name.
uint32 FFpkFilePack::GetStringHash( const AString& str, uint32 seed ) const
{
    uint32 hash = 0;
    const char* temp = str.c_str();

    while( *temp )
    {
        // 这里统一处理成大小写敏感
		//  [5/10/2015 Yish]
#if 0//FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
        hash = hash * seed + tolower(*temp++);
#else
        hash = hash * seed + (*temp++);
#endif
    }

    return hash;
}

// Encode the specified file name.
void FFpkFilePack::EncodeFilename( AString& str )
{
    int len = (int)str.length();
    for( int i=0;i<len;++i )
    {
        int n = i % 4;
        if( n == 0 )
            str[i] = len - str[i];
        else if( n == 1 )
            str[i] = str[i] + len;
        else if( n == 2 )
            str[i] = str[i] - len;
        else if( n == 3 )
            str[i] = str[i] + (len >> 1);
    }
}

// Decode the specified file name.
void FFpkFilePack::DecodeFilename( AString& str )
{
    int len = (int)str.length();
    for( int i=0;i<len;++i )
    {
        int n = i % 4;
        if( n == 0 )
            str[i] = len - str[i];
        else if( n == 1 )
            str[i] = str[i] - len;
        else if( n == 2 )
            str[i] = str[i] + len;
        else if( n == 3 )
            str[i] = str[i] - (len >> 1);
    }
}

/** List all the files.
@Param To store the file name list.
@Param List the sub folder recursely ?
@Param List the folders ?
*/
void FFpkFilePack::ListFiles( AStringList& fileList, bool bRecurse, bool bDir )
{
	for (size_t i = 0; i < m_Entries.size(); i++)
    {
		SFPKFileEntry& entry = m_Entries[i];
		const AString& filename = m_FilenameList[i];

		AString sBaseName, sPath;
		AStringUtil::SplitPath(filename, sBaseName, sPath);

        if( ((entry.nCompressedSize != (size_t)-1) ||
            (entry.nCompressedSize == (size_t)-1 && bDir)) &&
            (bRecurse || sPath.empty()) )
        {
			fileList.push_back(filename);
        }
    }
}

/** List all the files.
@Param To store the file name list.
@Param List the sub folder recursely ?
@Param List the folders ?
*/
void FFpkFilePack::ListFiles( VFileInfoList& fileList, bool bRecurse, bool bDir )
{
	for (size_t i = 0; i < m_Entries.size(); i++)
    {
		SFPKFileEntry& entry = m_Entries[i];
		const AString& filename = m_FilenameList[i];

		AString sBaseName, sPath;
		AStringUtil::SplitPath(filename, sBaseName, sPath);

        if( ((entry.nCompressedSize != (size_t)-1) ||
            (entry.nCompressedSize == (size_t)-1 && bDir)) &&
            (bRecurse || sPath.empty()) )
        {
			VFileInfo info;
			info.pFilePack = this;
			info.nCompressedSize = entry.nCompressedSize;
			info.nUncompressedSize = entry.nOriginalSize;
			info.sFileName = filename;
			info.sPath = sPath;
			info.sBaseName = sBaseName;
            fileList.push_back(info);
        }
    }
}

/** Find the specified files.
@Param To store the file name list.
@Param Key word to find.
@Param List the sub folder recursely ?
@Param List the folders ?
*/
void FFpkFilePack::FindFiles( AStringList& fileList, const AString& pattern,
                              bool bRecurse, bool bDir )
{
    // 如果要查找的字符包含目录，则做一个全匹配
    bool full_match = (pattern.find('/') != AString::npos ||
        pattern.find('\\') != AString::npos);

    // 获取关键字的目录
    AString directory;
    if( full_match )
    {
        size_t pos1 = pattern.rfind( '/' );
        size_t pos2 = pattern.rfind( '\\' );

        if( pos1 == AString::npos || ((pos2 != AString::npos) && pos1 < pos2) )
            pos1 = pos2;

        if( pos1 != AString::npos )
            directory = pattern.substr( 0,pos1+1 );
    }

    // 遍历文件信息
	for (size_t i = 0; i < m_Entries.size(); i++)
    {
		SFPKFileEntry& entry = m_Entries[i];
		const AString& filename = m_FilenameList[i];

		AString sBaseName, sPath;
		AStringUtil::SplitPath(filename, sBaseName, sPath);

        if( ((entry.nCompressedSize != (size_t)-1) ||
            (entry.nCompressedSize == (size_t)-1 && bDir)) &&
            (bRecurse || sPath == directory) )
        {
            if( AStringUtil::Match(full_match ? filename : sBaseName, pattern) )
                fileList.push_back(filename);
        }
    }
}

/** Find the specified files.
@Param To store the file name list.
@Param Key word to find.
@Param List the sub folder recursely ?
@Param List the folders ?
*/
void FFpkFilePack::FindFiles( VFileInfoList& fileList, const AString& pattern,
                              bool bRecurse, bool bDir )
{
    // 如果要查找的字符包含目录，则做一个全匹配
    bool full_match = (pattern.find('/') != AString::npos ||
        pattern.find('\\') != AString::npos );

    // 获取关键字的目录
    AString directory;
    if( full_match )
    {
        size_t pos1 = pattern.rfind( '/' );
        size_t pos2 = pattern.rfind( '\\' );

        if( pos1 == AString::npos || ((pos2 != AString::npos) && pos1 < pos2) )
            pos1 = pos2;

        if( pos1 != AString::npos )
            directory = pattern.substr( 0,pos1+1 );
    }

    // 遍历文件信息
	for (size_t i = 0; i < m_Entries.size(); i++)
    {
		SFPKFileEntry& entry = m_Entries[i];
		const AString& filename = m_FilenameList[i];

		AString sBaseName, sPath;
		AStringUtil::SplitPath(filename, sBaseName, sPath);

        if( ((entry.nCompressedSize != (size_t)-1) ||
            (entry.nCompressedSize == (size_t)-1 && bDir)) &&
            (bRecurse || sPath == directory) )
        {
			if (AStringUtil::Match(full_match ? filename : sBaseName, pattern))
			{
				VFileInfo info;
				info.pFilePack = this;
				info.nCompressedSize = entry.nCompressedSize;
				info.nUncompressedSize = entry.nOriginalSize;
				info.sFileName = filename;
				info.sPath = sPath;
				info.sBaseName = sBaseName;
				fileList.push_back(info);
			}
        }
    }
}

/** Whether the specified file exists.
@Param File path in the package.
*/
bool FFpkFilePack::Exists( const AString& filename )
{
    int fileIndex = GetFileIndex( filename );
    return (fileIndex >= 0);
}

// Get the specified file information.
void FFpkFilePack::GetFileInfo( const AString& filename, VFileInfo& info )
{
    int fileIndex = GetFileIndex( filename );
	if (fileIndex >= 0)
	{
		// Build the file info.
		info.pFilePack = this;
		info.sFileName = filename;
		AStringUtil::SplitPath(filename, info.sBaseName, info.sPath);
		info.nCompressedSize = m_Entries[fileIndex].nCompressedSize;
		info.nUncompressedSize = m_Entries[fileIndex].nOriginalSize;
	}
}

///////////////////////////////////////////////////////////////////////////
//
//  FPK Package factory.
//
///////////////////////////////////////////////////////////////////////////

/** Create a package object.
*/
FVFilePack* FFpkFilePackFactory::CreateInstance( const AString& name )
{
    return new FFpkFilePack( name,m_sType );
}

/** Destroy the specified package.
*/
void FFpkFilePackFactory::DestroyInstance( FVFilePack* pPack )
{
    F_SAFE_DELETE( pPack );
}