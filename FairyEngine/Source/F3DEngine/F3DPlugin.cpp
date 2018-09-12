/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPluginManager.cpp
 *  Desc:   ���ļ�ʵ��������Ĳ����������
 *  Author: Yish
 *  Date:   2010/12/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"
#include "F3DPlugin.h"

// ÿ�����ģ��������ĺ���
typedef void (*INSTALLPLUGIN) (void);
typedef void (*UNINSTALLPLUGIN) (void);

F3DPluginManager::F3DPluginManager()
{
    FLOG_INFO( "Plugin Manager has been created!" );
}

F3DPluginManager::~F3DPluginManager()
{
    UninstallPlugins();
	FLOG_INFO( "Plugin Manager has been destroyed!" );
}

F3DPluginManager& F3DPluginManager::GetInstance()
{
	static F3DPluginManager obj;
	return obj;
}

/** ��װ������ָ��·���Ĳ��
*/
bool F3DPluginManager::InstallPlugins( const AString& path )
{
    AStringList pluginList;
    FSysAPI::SearchFile(path.c_str(), "*.f3d", pluginList);

    for(AStringList::iterator it=pluginList.begin(); it!=pluginList.end(); ++it)
    {
        InstallPlugin(*it);
    }

    return true;
}

/** ��װָ���Ĳ��
@Param ָ�������ģ������
*/
bool F3DPluginManager::InstallPlugin( const AString& pluginName )
{
    // Load the specified dynamic library.
	FDModule* module = FDModule::Create(pluginName);
    if( !module->Load() )
    {
        delete module;
        FLOG_ERRORF("F3DPluginManager::InstallPlugin(), Load the module %s failed!", pluginName.c_str());
        return false;
    }

    INSTALLPLUGIN pInstall;
    UNINSTALLPLUGIN pUninstall;
    pInstall = (INSTALLPLUGIN)module->GetFunction( "InstallPlugin" );
    pUninstall = (UNINSTALLPLUGIN)module->GetFunction( "UninstallPlugin" );

    if( !pInstall || !pUninstall )
    {
        module->Unload();
        delete module;

		FLOG_ERRORF( "F3DPluginManager::InstallPlugin(), Invalid F3D plugin module(%s)!", pluginName.c_str() );
        return false;
    }

    // Install the plugins.
    pInstall();

    // Add to the plugin list.
    m_Plugins.push_back( module );
	FLOG_INFOF( "Install the plugin(%s) successfully!",pluginName.c_str() );
    return true;
}

/** ж��һ��ָ���Ĳ��
@Param ָ�����ģ�������
*/
void F3DPluginManager::UninstallPlugin( const AString& pluginName )
{
    PluginList::iterator itr;
    for( itr=m_Plugins.begin();itr!=m_Plugins.end();itr++ )
    {
        if( (*itr)->GetName() == pluginName )
            break;
    }

    if( itr == m_Plugins.end() )
    {
        FLOG_WARNING( "F3DPluginManager::UninstallPlugin(), Can't find the plugin(" + pluginName + ")!" );
		return;
    }

    UNINSTALLPLUGIN pUninstall;

    // ��ȡж�ز���ĺ���ָ��
    pUninstall = (UNINSTALLPLUGIN)(*itr)->GetFunction( "UninstallPlugin" );
    pUninstall();

    // �ͷ�ģ��
    (*itr)->Unload();
    delete (*itr);

    m_Plugins.erase( itr );
    FLOG_INFO( "Plugin (" + pluginName + ") has been uninstalled!" );
}

/** ж����������в��
*/
void F3DPluginManager::UninstallPlugins(void)
{
    PluginList::iterator itr;
    for( itr=m_Plugins.begin();itr!=m_Plugins.end();itr++ )
    {
        UNINSTALLPLUGIN pUninstall;

        // ��ȡж�ز���ĺ���ָ��
        pUninstall = (UNINSTALLPLUGIN)(*itr)->GetFunction( "UninstallPlugin" );
        pUninstall();

        // �ͷ�ģ��
        (*itr)->Unload();
        delete (*itr);

        FLOG_INFO( "Plugin(" + (*itr)->GetName() + ") has been uninstalled!" );
    }

    m_Plugins.clear();
}