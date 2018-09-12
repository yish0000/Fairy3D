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
    ���غ͹�������Ĳ����һ�����Զ�̬�����ʽ�ṩ��Ҳ��ʹ�þ�̬�⡣
*/
class FAIRY_API F3DPluginManager
{
    // Plugin library list.
    typedef std::vector<FDModule*> PluginList;

    friend class F3DEngine;

protected:
    PluginList m_Plugins;   // ����Ĳ���б�

public:
    ~F3DPluginManager(void);

    // ��ָ����·���¼������в��
    bool InstallPlugins( const AString& path );

    // ж�����в��
    void UninstallPlugins();

    // ����ָ���Ĳ��
    bool InstallPlugin( const AString& pluginName );

    // ж��ָ���Ĳ��
    void UninstallPlugin( const AString& pluginName );

    // ��ȡָ�������ģ����
//    F_DLIB_HANDLE GetPluginHandle( const AString& moduleName );

    // Get the instance of the plugin manager.
    static F3DPluginManager& GetInstance();

private:
    F3DPluginManager();
	F_DISABLE_COPY_ASSIGNMENT(F3DPluginManager);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_PLUGINMANAGER_H__