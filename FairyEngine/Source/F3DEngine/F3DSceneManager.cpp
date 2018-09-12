/*
 * -----------------------------------------------------------------------
 *  Name:   F3DSceneManager.cpp
 *  Desc:   This file define the class of SceneManager.
 *  Author: Yish
 *  Date:   2011/2/3
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DSceneManager.h"
#include "F3DLightObject.h"

/** Constructor.
 */
F3DSceneManager::F3DSceneManager()
    : m_pSceneRoot(NULL), m_pActiveCamera(NULL)
{
	m_cntGC.SetPeriod(10000);
	m_cntGC.Reset();
}

/** Destructor.
 */
F3DSceneManager::~F3DSceneManager()
{
	F_SAFE_DELETE(m_pSceneRoot);

	SceneObjectFactoryMap::iterator itf = m_ObjFactory.begin();
	for (; itf != m_ObjFactory.end(); ++itf)
	{
		F_SAFE_DELETE(itf->second);
	}
}

/** Initialize the scene manager.
 */
bool F3DSceneManager::Init()
{
	RegisterObjectFactory("object", new F3DSceneObjectFactory());
	RegisterObjectFactory("light", new F3DLightObjectFactory());

	// Create the root node.
	m_pSceneRoot = new F3DSceneObject("SceneRoot", "object");

    return true;
}

/** Register the scene object factory.
*/
bool F3DSceneManager::RegisterObjectFactory(const AString& type, F3DSceneObjectFactory* pFactory)
{
	if (m_ObjFactory.find(type) != m_ObjFactory.end())
	{
		F_SAFE_DELETE(pFactory);
		FLOG_ERROR("F3DSceneManager::RegisterObjectFactory, the specified factory has already exist!");
		return false;
	}

	if (type != pFactory->GetType())
	{
		F_SAFE_DELETE(pFactory);
		FLOG_ERRORF("F3DSceneManager::RegisterObjectFactory, the type(%s) is different with the type(%s) of factory!", type.c_str(), pFactory->GetType().c_str());
		return false;
	}

	m_ObjFactory[type] = pFactory;
	return true;
}

/** Update the scene manager.
 */
void F3DSceneManager::Update()
{
	// Update the camera.
	m_pActiveCamera->Update();

    // Update the scene from the root node.
    m_pSceneRoot->UpdateNode(false);

	// Release the dead scene objects.
	if (m_cntGC.IncCounter(FTimer::GetInstance().GetDeltaTime()))
	{
		RemoveAllDeadObjects();
		m_cntGC.Reset();
	}
}

/** Release the dead scene objects.
*/
void F3DSceneManager::RemoveAllDeadObjects()
{
	DeadSceneNodePool::iterator it = m_DeadObjs.begin();
	while (it != m_DeadObjs.end())
	{
		F3DSceneObject* pObject = dynamic_cast<F3DSceneObject*>(*it);
		FASSERT(pObject && "pNode is not a F3DSceneObject!");

		if (pObject->GetRefCount() <= 1)
		{
			SceneObjectFactoryMap::iterator itf = m_ObjFactory.find(pObject->GetClassID());
			if (itf == m_ObjFactory.end())
			{
				FASSERT(0);
				FLOG_ERRORF("F3DSceneManager::RemoveAllDeadObjects, Cannot find the factory of object(%s)", pObject->GetName().c_str());
				++it;
			}
			else
			{
				m_SceneObjs.erase(pObject->GetName());
				itf->second->DestroyInstance(pObject);
				it = m_DeadObjs.erase(it);
			}
		}
		else
		{
			++it;
		}
	}
}

/** Create a specified scene object.
 */
F3DSceneObject* F3DSceneManager::CreateSceneObject(const AString& name, const AString& type, void* pParam)
{
    // Check whether the object is existed ?
    SceneObjectMap::iterator it = m_SceneObjs.find(name);
	if (it != m_SceneObjs.end())
		return it->second;

    // Find the factory, we will create a new one.
	SceneObjectFactoryMap::iterator itf = m_ObjFactory.find(type);
	if (itf == m_ObjFactory.end())
	{
		FLOG_ERRORF("F3DSceneManager::CreateSceneObject, Cannot find the factory(%s), failed to create the object!", type.c_str());
		return NULL;
	}

	F3DSceneObject* pObject = itf->second->CreateInstance(name);
	if (!pObject)
	{
		FLOG_ERRORF("F3DSceneManager::CreateSceneObject, Failed to create the scene object(%s)!", name.c_str());
		return NULL;
	}

	// Initialize the scene object.
	if (!pObject->Init(pParam))
	{
		F_SAFE_DELETE(pObject);
		FLOG_ERRORF("F3DSceneManager::CreateSceneObject, Failed to initialize the scene object(%s)!", name.c_str());
		return NULL;
	}

	m_SceneObjs[name] = pObject;
	m_DeadObjs.insert(pObject);	// Add to the dead pool, if we do not attach it to the scene tree, we will destroy it.
    return pObject;
}

/** Get the specified scene object.
*/
F3DSceneObject* F3DSceneManager::GetSceneObject(const AString& name) const
{
	SceneObjectMap::const_iterator it = m_SceneObjs.find(name);
	return it != m_SceneObjs.end() ? it->second : NULL;
}

/** Add the specified scene object to dead pool.
*/
void F3DSceneManager::AddObjectToDeadPool(F3DSceneNode* pNode)
{
	FASSERT(pNode && "pNode == null");

	m_DeadObjs.insert(pNode);

	// Handle the child node.
	for (size_t i = 0; i < pNode->GetNumChildren(); i++)
	{
		F3DSceneNode* pChild = pNode->GetChildNode(i);
		if (pChild)
			AddObjectToDeadPool(pChild);
	}
}

/** Remove the specified scene object from the dead pool.
*/
void F3DSceneManager::RemoveDeadObject(F3DSceneNode* pNode)
{
	FASSERT(pNode && "pNode == null");

	DeadSceneNodePool::iterator it = m_DeadObjs.find(pNode);
	if (it != m_DeadObjs.end())
		m_DeadObjs.erase(it);

	// Handle the child node.
	for (size_t i = 0; i < pNode->GetNumChildren(); i++)
	{
		F3DSceneNode* pChild = pNode->GetChildNode(i);
		if (pChild)
			RemoveDeadObject(pChild);
	}
}

/** Attach the scene object to the scene tree.
 */
void F3DSceneManager::AttachSceneObject(F3DSceneObject *pObject)
{
    FASSERT(pObject && "pObject == null");
    m_pSceneRoot->AddChildNode(pObject);
}

/** Attach the scene object to the specified path.
 */
void F3DSceneManager::AttachSceneObjectTo(F3DSceneObject *pObject, const AString &path)
{
    FASSERT(pObject && "pObject == null");
    
    F3DSceneObject* pNode = GetSceneObjectByPath(path);
    if( !pObject )
    {
        FLOG_WARNINGF("F3DSceneManager::AttachSceneObjectTo, Cannot find the specified scene node(%s)!", path.c_str());
        return;
    }
    
    pNode->AddChildNode(pObject);
}

/** Get the specified scene node.
 */
F3DSceneObject* F3DSceneManager::GetSceneObjectByPath(const AString &path) const
{
    return dynamic_cast<F3DSceneObject*>(m_pSceneRoot->GetChildNodeByPath(path));
}

/** Force the scene manager to collect garbage immediately.
*/
void F3DSceneManager::ForceGC()
{
	RemoveAllDeadObjects();
	m_cntGC.Reset();
}

/*
//------------------------------------------------------------------------
// Render the scene for the engine. This is the entry function
// to render all the geometrys.
//------------------------------------------------------------------------
void FlySceneManager::RenderScene(void)
{
    if( !m_pCamera ) return;

    // Get the pointer to the renderer.
    LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();

    // Update the camera.
    m_pCamera->Update();

    // Update the scene data.
    UpdateSceneData();

    // Set the ambient color for the scene.
    m_pDevice->SetAmbientLight( m_cAmbient.fR,m_cAmbient.fG,m_cAmbient.fB );

    // Set the fog state for the scene.
    m_pDevice->SetFog( m_FogMode,m_cFogColor,m_fFogDensity,m_fFogNear,m_fFogFar );

    // 1. Update all the texture targets.
    RenderToTarget();

    m_pDevice->ClearBuffer( true,true,true );

    // Setup the view matrix.
    m_pDevice->SetView3D( m_pCamera->GetRight(),
        m_pCamera->GetUp(),
        m_pCamera->GetDirection(),
        m_pCamera->GetPosition() );

    m_pAutoParameters->SetViewMatrix( m_pDevice->GetViewMatrix() );
    m_pAutoParameters->SetProjectionMatrix( m_pDevice->GetProjectionMatrix() );

    // Update the frustum.
    m_Frustum.UpdateFrustum();

    // Set the clip planes.
    m_pDevice->SetClipPlanes( m_ClipPlanes );

    // Render the world geometry.
    RenderWorldLevel();

    // Find the visible objects, and add them
    // to the renderQueue.
    RenderVisibleObjects();

    // Render the skybox.
    if( m_bShowSkybox )
    {
        m_pSkyHelper->SetCameraPos( m_pCamera->GetPosition() );
        m_pSkyHelper->AddToRenderQueue();
    }

    if( m_bShadowMode )
        m_pShadowHelper->AddToRenderQueue();

    // Flush the renderQueue.
    pRenderer->FlushRenderQueue();
}


//------------------------------------------------------------------------
// Render the visible scene objects.
//------------------------------------------------------------------------
void FlySceneManager::RenderVisibleObjects(void)
{
    m_pSceneRoot->RenderVisibleObjects();
}

//------------------------------------------------------------------------
// Render the content to the texture target.
//------------------------------------------------------------------------
void FlySceneManager::RenderToTarget(void)
{
    FlyCameraBase* pCamera;

    // Get the pointer to the renderer.
    LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();

    // Save the current projection Matrix.
    FlyMatrix mProjection = m_pDevice->GetProjectionMatrix();

    RenderTargetList::iterator itr;
    for( itr=m_Targets.begin();itr!=m_Targets.end();itr++ )
    {
        if( !(*itr)->IsActive() )
            continue;

        // Begin the rendering.
        m_pDevice->SetTextureRenderTarget( (*itr)->GetTextureTarget() );
        m_pDevice->ClearBuffer( true,true,true );

        (*itr)->OnUpdateBegin();

        // Update the camera for the target.
        pCamera = (*itr)->GetCamera();

        m_pDevice->SetView3D( pCamera->GetRight(),
            pCamera->GetUp(),
            pCamera->GetDirection(),
            pCamera->GetPosition() );
        m_pAutoParameters->SetViewMatrix( m_pDevice->GetViewMatrix() );

        float fAspect = (*itr)->GetWidth() / (float)(*itr)->GetHeight();
        m_pDevice->SetFOV( m_pDevice->GetActiveStage(),FLY_PI/4.0f,fAspect );
        m_pAutoParameters->SetProjectionMatrix( m_pDevice->GetProjectionMatrix() );

        // Update the frustum.
        m_Frustum.UpdateFrustum();

        // Set the clip planes.
        m_pDevice->SetClipPlanes( (*itr)->GetClipPlaneList() );

        // Render all the contents to texture.
        RenderVisibleObjects();

        if( m_pSkyHelper && (*itr)->IsShowSkybox() )
        {
            m_pSkyHelper->SetCameraPos( pCamera->GetPosition() );
            m_pSkyHelper->AddToRenderQueue();
        }

        // Flush the renderQueue.
        pRenderer->FlushRenderQueue();

        (*itr)->OnUpdateEnd();
    }

    // Return to default renderQueue.
    m_pDevice->SetTextureRenderTarget( NULL );
    m_pDevice->SetProjectionMatrix( mProjection );
}
*/