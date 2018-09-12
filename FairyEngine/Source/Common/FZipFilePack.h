/*
 * ------------------------------------------------------------------------
 *  Name:   FZipFilePack.h
 *  Desc:   本文件用于实现基于ZIP压缩包格式的文件包。
 *  Author: Yish
 *  Date:   10/3/2010
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_ZIPFILEPACK_H__
#define __FAIRY_ZIPFILEPACK_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FVFilePack.h"

///////////////////////////////////////////////////////////////////////////

typedef struct zzip_dir ZZIP_DIR;

/** ZIP文件包
*/
class FZipFilePack : public FVFilePack
{
protected:
    ZZIP_DIR* m_pZipDir;        // ZIP文件目录指针
    VFileInfoList m_FileInfos;  // 文件信息列表

    void GetZipErrorDesc( int zipError, AString& desc );

public:
    FZipFilePack( const AString& name, const AString& packType );
    ~FZipFilePack(void);

    bool Load(void);
    void Unload(void);

    int AddEntry( const AString& entry, uint32 nSize, void* data, uint32 flags ) { return -1; }
	int AddEntry( const AString& entry, const AString& filename, uint32 flags ) { return -1; }

    void Delete( const AString& path ) {};
    FVFileStream* Open( const AString& filename );
    bool DeFragment( CB_PACKAGEOP pCallBack ) { return false; }
    uint32 GetFragmentSize(void) const { return 0; }
    void Flush(void) {};
    void ListFiles( AStringList& fileList, bool bRecurse,bool bDir );
    void ListFiles( VFileInfoList& fileList, bool bRecurse,bool bDir );
    void FindFiles( AStringList& fileList, const AString& pattern, bool bRecurse/* =true */, bool bDir/* =true */ );
    void FindFiles( VFileInfoList& fileList, const AString& pattern, bool bRecurse/* =true */, bool bDir/* =true */ );
    bool Exists( const AString& filename );
    void GetFileInfo( const AString& filename, VFileInfo& info );
};

/** ZIP文件包工厂
*/
class FZipFilePackFactory : public FVFilePackFactory
{
public:
    FZipFilePackFactory(void) { m_sType = "zip"; }

    FVFilePack* CreateInstance( const AString& name );
    void DestroyInstance( FVFilePack* pPack );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_ZIPFILEPACK_H__