//------------------------------------------------------------------------
// Name: FlyPluginManager.h
// Desc: This file define a class to manage the plugin for engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_PLUGIN_MANAGER_H__
#define __FLY_PLUGIN_MANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


typedef void (CALLBACK *INSTALLPLUGIN) (void);
typedef void (CALLBACK *UNINSTALLPLUGIN) (void);

/*
* A class to manage the plugins for the engine.
*/
class _FLY_EXPORT FlyPluginManager
{
    struct sPluginNode
    {
        std::string sModuleName;
        HINSTANCE hModule;
    };

    typedef std::vector<sPluginNode> PluginList;

protected:
    PluginList m_Plugins;   // Plugin list for the engine.

public:
    // Constructor and destructor.
    FlyPluginManager(void);
    ~FlyPluginManager(void);

    // Setup all the plugins in the current directory.
    void InstallPlugins(void);

    // Uninstall all the plugins.
    void UninstallPlugins(void);

    HRESULT InstallPlugin( const char* cPluginName );
    HRESULT UninstallPlugin( const char* cPluginName );

    // Get the handle of the specified plugin.
    HINSTANCE GetPluginHandle( const char* cModuleName );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_PLUGIN_MANAGER_H__