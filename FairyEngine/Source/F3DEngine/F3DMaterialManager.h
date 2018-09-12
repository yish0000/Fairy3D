/*
 * ------------------------------------------------------------------------
 *	Name:   F3DMaterialManager.h
 *  Desc:   本文件为引擎定义了一个材质管理器。
 *  Author: Yish
 *  Date:   2010/8/29
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_MATERIALMANAGER_H__
#define __F3D_MATERIALMANAGER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DMaterial.h"

///////////////////////////////////////////////////////////////////////////


/** 材质管理器
@remarks
@note
    这是一个单件对象，由F3DEngine自动创建，请勿自行创建。
*/
class FAIRY_API F3DMaterialManager
{
    typedef std::map<AString, F3DMaterial*> MaterialMap;
    typedef std::vector<F3DMaterial*> MaterialList;

protected:
    MaterialMap m_Materials;

public:
    ~F3DMaterialManager();

    F3DMaterial* CreateMaterial( const AString& name );
    bool LoadMaterial( const AString& name );

    size_t GetNumMaterials() const { return m_Materials.size(); }
    F3DMaterial* GetMaterialByName( const AString& name );

    // Get the singleton.
    static F3DMaterialManager& GetInstance();

private:
	F3DMaterialManager();
	F_DISABLE_COPY_ASSIGNMENT(F3DMaterialManager);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_MATERIALMANAGER_H__