/*
 * ------------------------------------------------------------------------
 *  Name:   FVFileSystem.cpp
 *  Desc:   本文件定义了本引擎的虚拟文件系统。
 *  Author: Yish
 *  Date:   2010/10/2
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FStrUtil.h"
#include "FLogManager.h"
#include "FVFileSystem.h"
#include "FSysFilePack.h"
#include "FZipFilePack.h"
#include "FFpkFilePack.h"

/** 获取全局唯一的实例
*/
FVFileSystem& FVFileSystem::GetInstance(void)
{
    static FVFileSystem obj;
    return obj;
}

/** 构造函数
*/
FVFileSystem::FVFileSystem(void) : m_sBaseDir(".")
{
    // Register the package types.
    RegisterFilePack( new FSysFilePackFactory() );
    RegisterFilePack( new FZipFilePackFactory() );
    RegisterFilePack( new FFpkFilePackFactory() );
}

/** 析构函数
*/
FVFileSystem::~FVFileSystem(void)
{
    // 卸载所有文件包
    VFilePackMap::iterator itr;
    for( itr=m_VFilePacks.begin();itr!=m_VFilePacks.end();itr++ )
    {
        FVFilePack* pPack = itr->second;
        pPack->Unload();

        // 查找该文件包的类型，若没有则报错
        VFilePackFactoryMap::iterator fac = m_VFilePackFactorys.find( pPack->GetPackType() );
        if( fac == m_VFilePackFactorys.end() )
        {
            FLOG_ERRORF("FVFileSystem::~FlyVFileSystem, Can't find the package type(%s) when unload packages!", fac->first.c_str());
            continue;
        }

        // 删除该对象
        FLOG_INFO( "Package(" + pPack->GetName() + "," + pPack->GetPackType() + ") is unloaded!" );
        fac->second->DestroyInstance( pPack );
    }

    // 卸载所有文件包工厂
    VFilePackFactoryMap::iterator fac;
    for( fac=m_VFilePackFactorys.begin();fac!=m_VFilePackFactorys.end();fac++ )
    {
        F_SAFE_DELETE( fac->second );
        FLOG_INFO( "Unregistered the package type(" + fac->first + ")!" );
    }
}

/** 加载一个指定的文件包。
@Param 文件包的路径
@Param 给文件包起一个别名(以后便于使用)
@Return 文件包的指针
*/
FVFilePack* FVFileSystem::LoadPack( const AString& filename,const AString& alias )
{
    // 检查是否存在该文件包
    VFilePackMap::iterator itr = m_VFilePacks.find( alias );

    if( itr != m_VFilePacks.end() )
    {
        // 该文件包已经存在
        return itr->second;
    }
    else
    {
        // 检测该文件包是否被加载
        for( itr=m_VFilePacks.begin();itr!=m_VFilePacks.end();itr++ )
        {
            if( itr->second->GetName() == filename )
                return itr->second;
        }

        // 查找指定的文件包工厂，如果没找到则报错
        AString fileExt;
        AStringUtil::FileExt( filename,fileExt );
        VFilePackFactoryMap::iterator fac = m_VFilePackFactorys.find( fileExt );
        if( fac == m_VFilePackFactorys.end() )
        {
            FLOG_WARNING("FVFileSystem::LoadPack(), Can't find the package type(" + fileExt + ")!");
            return NULL;
        }

        FVFilePack* pPack = fac->second->CreateInstance( filename );
        if( !pPack->Load() )
        {
			FLOG_WARNING( "FVFileSystem::LoadPack(), Load the package(" + filename + ") failed!" );
            fac->second->DestroyInstance( pPack );
            return NULL;
        }

        m_VFilePacks.insert( VFilePackMap::value_type(alias,pPack) );
        FLOG_INFO( "Package(" + filename + ", " + alias + ") is loaded!" );
        return pPack;
    }
}

/** 卸载一个指定的文件包
@Param 指定文件包的指针
*/
void FVFileSystem::UnloadPack( FVFilePack* pFilePack )
{
    UnloadPackByName( pFilePack->GetName() );
}

/** 通过指定名字来卸载文件包
@Param 指定文件包的名字
*/
void FVFileSystem::UnloadPackByName( const AString& filename )
{
    VFilePackMap::iterator itr;
    for( itr=m_VFilePacks.begin();itr!=m_VFilePacks.end();itr++ )
    {
        if( itr->second->GetName() == filename )
        {
            itr->second->Unload();

            // 查找创建该报的工厂
            VFilePackFactoryMap::iterator fac = m_VFilePackFactorys.find( itr->second->GetPackType() );
            if( fac == m_VFilePackFactorys.end() )
            {
                FLOG_ERRORF("FVFileSystem::UnloadPackByName, Can't find the package type(%s) when unload package!", fac->first.c_str());
                break;
            }

            FLOG_INFO( "Package(" + filename + ", " + itr->first + ") is unloaded!" );
            fac->second->DestroyInstance( itr->second );
            m_VFilePacks.erase( itr );
            break;
        }
    }
}

/** 卸载一个指定的文件包
@Param 指定文件包的别名
*/
void FVFileSystem::UnloadPack( const AString& alias )
{
    // 查找指定的文件包
    VFilePackMap::iterator itr = m_VFilePacks.find( alias );
    if( itr != m_VFilePacks.end() )
    {
        itr->second->Unload();

        // 查找创建该包的工厂
        VFilePackFactoryMap::iterator fac = m_VFilePackFactorys.find( itr->second->GetPackType() );
        if( fac == m_VFilePackFactorys.end() )
        {
            FLOG_ERRORF("FVFileSystem::UnloadPack, Can't find the package type(%s) when unload package!", fac->first.c_str());
            return;
        }

        fac->second->DestroyInstance( itr->second );
        FLOG_INFO( "Package(" + itr->second->GetName() + "," + alias + ") is unloaded!" );
        m_VFilePacks.erase( itr );
    }
}

/** 注册一个新的文件包类型
@Param 该文件包类型的工厂
*/
void FVFileSystem::RegisterFilePack( FVFilePackFactory* pFactory )
{
    if( !pFactory )
	{
		FLOG_WARNING("FVFileSystem::RegisterFilePack(), Invalid parameters! (NULL)");
		return;
	}

    m_VFilePackFactorys.insert( VFilePackFactoryMap::value_type(pFactory->GetType(),pFactory) );
    FLOG_INFO( "Package type(" + pFactory->GetType() + ") is registered!" );
}

/** 反注册一个指定的文件包类型
@Param 该文件包的类型名字
*/
void FVFileSystem::UnregisterFilePack( const AString& packType )
{
    VFilePackFactoryMap::iterator fac = m_VFilePackFactorys.find( packType );
    if( fac == m_VFilePackFactorys.end() )
    {
        FLOG_WARNING("FVFileSystem::UnregisterFilePack(), Can't find the package type(" + packType + ")!");
        return;
    }

    // 所有该类型的文件包被卸载时，才能反注册该类型
    VFilePackMap::iterator itr;
    for( itr=m_VFilePacks.begin();itr!=m_VFilePacks.end();itr++ )
    {
        if( itr->second->GetPackType() == packType )
        {
			FASSERT(0);
            FLOG_ERRORF("FVFileSystem::UnregisterFilePack, Before unregistering package type(%s), "
				"you should unloading all the packages! ", itr->first.c_str());
            return;
        }
    }

    m_VFilePackFactorys.erase( fac );
    FLOG_INFO( "Unregistered the package type(" + packType + ")!" );
}

/** 获取指定的虚拟文件包
@Param 文件包的指针
*/
FVFilePack* FVFileSystem::GetVFilePack( const AString& alias )
{
    VFilePackMap::iterator it;
    it = m_VFilePacks.find( alias );
    return it != m_VFilePacks.end() ? it->second : NULL;
}

/** 根据所给的路径，查询所属的文件包
@Param 文件路径
*/
FVFilePack* FVFileSystem::SearchVFilePack( const AString& filename, AString& innerfile )
{
	AStringList strList;
	AString strFile = filename;
	std::replace( strFile.begin(),strFile.end(),'\\','/' );
	AStringUtil::Split( strFile,"/",strList );

	AString strPack;
	for( size_t i=0;i<strList.size()-1;++i )
	{
		if( strPack.empty() )
			strPack += strList[i];
		else
			strPack += "/" + strList[i];
		FVFilePack* pPack = GetVFilePack( strPack );
		if( pPack )
		{
			// 获取该文件在包内的绝对路径
			innerfile = "";
			for( size_t t=i+1;t<strList.size();++t )
			{
				innerfile += strList[t] + "/";
			}
			innerfile.erase( innerfile.length() - 1 );
			return pPack;
		}
	}

	return NULL;
}

/** 设置文件系统的工作目录
*/
void FVFileSystem::SetBaseDir( const AString& strDir )
{
	m_sBaseDir = strDir;
}