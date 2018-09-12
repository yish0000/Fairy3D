/*
 * ------------------------------------------------------------------------
 *  Name:   FVFileSystem.h
 *  Desc:   本文件定义了引擎的文件包管理系统。
 *  Author: Yish
 *  Date:   2010/10/2
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_VFILESYSTEM_H__
#define __FAIRY_VFILESYSTEM_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

#include "FVFilePack.h"
#include "FVFileStream.h"

///////////////////////////////////////////////////////////////////////////

/** 文件系统
@remarks
    本类用于管理引擎的文件系统，加载资源时优先从包内寻找文件，找不到
    时再尝试从工作目录下的相对路径中寻找。
*/
class FVFileSystem : public FGeneralAlloc
{
    typedef std::map<AString, FVFilePackFactory*> VFilePackFactoryMap;
    typedef std::map<AString, FVFilePack*> VFilePackMap;

protected:
    VFilePackMap m_VFilePacks;                  // 文件包列表
    VFilePackFactoryMap m_VFilePackFactorys;    // 文件包工厂(用于创建各种文件包，如: FPK,ZIP,TAR,7Z ...)
    AString m_sBaseDir;                         // 工作路径

public:
    ~FVFileSystem(void);

    /** 注册一个新的文件包类型
    */
    void RegisterFilePack( FVFilePackFactory* pFactory );

    /** 反注册指定文件包类型
    */
    void UnregisterFilePack( const AString& packType );

    // 设置文件系统的工作路径
    void SetBaseDir( const AString& strDir );

    /** 加载一个文件包
    @param [0] 文件包的路径
    @param [1] 文件包的别名
    */
    FVFilePack* LoadPack( const AString& filename, const AString& alias );

    /** 卸载一个文件包
    */
    void UnloadPack( FVFilePack* pFilePack );
    void UnloadPack( const AString& alias );

    /** 指定文件包的名字，将其卸载
    */
    void UnloadPackByName( const AString& filename );

    /** 根据文件包的别名获取其指针
    */
    FVFilePack* GetVFilePack( const AString& alias );

	// 根据所给的路径，查询是否存在一个包
	FVFilePack* SearchVFilePack( const AString& filename, AString& innerfile );

    // 获取文件系统的工作路径
    const AString& GetBaseDir(void) const { return m_sBaseDir; }

    // 获取该类的实例
    static FVFileSystem& GetInstance(void);

private:
    // 私有构造函数
    FVFileSystem(void);
	F_DISABLE_COPY_ASSIGNMENT(FVFileSystem);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_VFILESYSTEM_H__