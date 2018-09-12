/*
 * ------------------------------------------------------------------------
 *  Name:   FSysFilePack.h
 *  Desc:   本文件定义了一个文件包的特例，本对象代表系统中的一个目录。
 *  Author: Yish
 *  Date:   2010/10/2
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_SYSFILEPACK_H__
#define __FAIRY_SYSFILEPACK_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FVFilePack.h"

///////////////////////////////////////////////////////////////////////////

/** 针对系统文件夹所做的一个文件包特例。
*/
class FSysFilePack : public FVFilePack
{
public:
    FSysFilePack( const AString& name,const AString& packType );
    ~FSysFilePack(void);

    bool Load(void) { return true; }
    void Unload(void) {};

    int AddEntry( const AString& entry, uint32 nSize, void* data, uint32 flags );
	int AddEntry( const AString& entry, const AString& filename, uint32 flags );
	void Delete( const AString& path );
    FVFileStream* Open( const AString& filename );
    bool DeFragment( CB_PACKAGEOP pCallBack ) { return false; }
    uint32 GetFragmentSize(void) const { return 0; }
    void Flush(void) {};
    void ListFiles( AStringList& fileList,bool bRecurse=true,bool bDir=true );
    void ListFiles( VFileInfoList& fileList,bool bRecurse=true,bool bDir=true );
    void FindFiles( AStringList& fileList,const AString& pattern,bool bRecurse=true,bool bDir=true );
    void FindFiles( VFileInfoList& fileList,const AString& pattern,bool bRecurse=true,bool bDir=true );
    bool Exists( const AString& filename );
    void GetFileInfo( const AString& filename,VFileInfo& info );

protected:
	void EnumFiles(const AString& pattern, bool bRecurse, bool bDir,
		VFileInfoList* fileinfos, AStringList* fileList);
};

/** 创建该文件包类型的工厂
*/
class FSysFilePackFactory : public FVFilePackFactory
{
public:
    FSysFilePackFactory(void) { m_sType = "folder"; }
    ~FSysFilePackFactory(void) {};

    FVFilePack* CreateInstance( const AString& name );
    void DestroyInstance( FVFilePack* pPack );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_SYSFILEPACK_H__