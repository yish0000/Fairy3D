/*
 * -----------------------------------------------------------------------
 *  Name:   F3DSceneManager.h
 *  Desc:   本文件为引擎定义了一个场景管理器的基类。
 *  Author: Yish
 *  Date:   2011/1/20
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */

#ifndef __F3D_SCENEMANAGER_H__
#define __F3D_SCENEMANAGER_H__

//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "F3DSceneObject.h"

//////////////////////////////////////////////////////////////////////////

/** 场景管理器
@remarks
    本对象代表3D世界中的整个场景，可以在渲染过程中将不可见的部分裁剪掉，
    只渲染可见的来提高速度。
@note
    用户可以通过派生此类来实现自己的场景类型。
*/
class FAIRY_API F3DSceneManager
{
public:
    typedef std::map<AString, F3DSceneObject*> SceneObjectMap;
    typedef std::set<F3DSceneNode*> DeadSceneNodePool;
	typedef std::map<AString, F3DSceneObjectFactory*> SceneObjectFactoryMap;

    friend class F3DSceneNode;
    
protected:
	SceneObjectFactoryMap m_ObjFactory;	// Factory of scene objects.
    SceneObjectMap m_SceneObjs;			// Scene objects.
    F3DSceneObject* m_pSceneRoot;		// Root node of the scene.
    F3DCameraBase* m_pActiveCamera;		// Current active camera.

	DeadSceneNodePool m_DeadObjs;		// Scene objects which is dead, will be destroyed.
	FCounter m_cntGC;					// Garbage collect counter.

public:
    F3DSceneManager();
    virtual ~F3DSceneManager();

	// Initialize the scene manager.
	virtual bool Init();
    // Update the scene manager.
    virtual void Update();

	// Register the scene object factory.
	bool RegisterObjectFactory(const AString& type, F3DSceneObjectFactory* pFactory);
    // Create a new scene object.
    F3DSceneObject* CreateSceneObject( const AString& name, const AString& type, void* pParam );
    // Get a specified scene object from the root node.
    F3DSceneObject* GetSceneObject(const AString& name) const;
    
	// Attach a scene object to the root node.
	void AttachSceneObject(F3DSceneObject* pObject);
	// Attach a scene object to the specified position.
	void AttachSceneObjectTo(F3DSceneObject* pObject, const AString& path);
	// Remove the specified scene object.
	void RemoveSceneObject(const AString& name);
	void RemoveSceneObjectByPath(const AString& path);
	// Remove all the scene objects.
    void RemoveAllSceneObjects(void);
	// Get a scene object by its path.
    F3DSceneObject* GetSceneObjectByPath(const AString& path) const;
	// Get the root node.
    F3DSceneObject* GetSceneRoot(void) const { return m_pSceneRoot; }

    // Get the active camera.
    F3DCameraBase* GetActiveCamera(void) const { return m_pActiveCamera; }
    // Set the active camera.
    void SetActiveCamera( F3DCameraBase* pCamera ) { m_pActiveCamera = pCamera; }

	// Force the scene manager to collect garbage immediately.
	void ForceGC();

protected:
    // Add a specified object to dead pool.
    void AddObjectToDeadPool(F3DSceneNode* pNode);
    // Remove the object from the dead pool.
    void RemoveDeadObject(F3DSceneNode* pNode);
	// Collect the dead scene objects.
    void RemoveAllDeadObjects();
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_SCENEMANAGER_H__