/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPluginManager.h
 *  Desc:   This file define a plugin manager for engine.
 *  Author: Yish
 *  Date:   2010/12/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_PLUGINMANAGER_H__
#define __F3D_PLUGINMANAGER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"
#include "FDModule.h"

///////////////////////////////////////////////////////////////////////////


/** Plugin manager.
@remarks
    加载和管理引擎的插件，一般插件以动态库的形式提供，也可使用静态库。
*/
class FAIRY_API F3DPluginManager
{
    // Plugin library list.
    typedef std::vector<FDModule*> PluginList;

    friend class F3DEngine;

protected:
    PluginList m_Plugins;   // 引擎的插件列表

public:
    ~F3DPluginManager(void);

    // 在指定的路径下加载所有插件
    bool InstallPlugins( const AString& path );

    // 卸载所有插件
    void UninstallPlugins();

    // 加载指定的插件
    bool InstallPlugin( const AString& pluginName );

    // 卸载指定的插件
    void UninstallPlugin( const AString& pluginName );

    // 获取指定插件的模块句柄
//    F_DLIB_HANDLE GetPluginHandle( const AString& moduleName );

    // Get the instance of the plugin manager.
    static F3DPluginManager& GetInstance();

private:
    F3DPluginManager();
	F_DISABLE_COPY_ASSIGNMENT(F3DPluginManager);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_PLUGINMANAGER_H__