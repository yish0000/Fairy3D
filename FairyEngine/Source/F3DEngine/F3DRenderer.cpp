/*
 * -----------------------------------------------------------------------
 *  Name:   F3DRenderer.cpp
 *  Desc:   本文件用于实现引擎的渲染器基类。
 *  Author: Yish
 *  Date:   2010/12/18
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "F3DPCH.h"

// #include "F3DRenderer.h"
// //#include "F3DMaterialManager.h"
// 
// 
// /** 对普通渲染队列进行排序的函数器
// */
// struct FSortDefault
// {
//     bool operator () ( F3DRenderable* obj1,F3DRenderable* obj2 ) const
//     {
//         return true;
//     }
// };
// 
// /** 对半透明渲染队列进行排序的函数器
// */
// struct FSortTranparent
// {
//     bool operator () ( F3DRenderable* obj1,F3DRenderable* obj2 ) const
//     {
//         return true;
//     }
// };
// 
// /** 对2D屏幕元素进行排序的函数器
// */
// struct FSortLayout
// {
//     bool operator () ( F3DRenderable* obj1,F3DRenderable* obj2 ) const
//     {
//         return true;
//     }
// };
// 
// /** 构造函数
// */
// F3DRenderer::F3DRenderer(void) : m_nNumRenderedVerts(0),m_nNumRenderedIndis(0),
//     m_nNumRenderedPrims(0),m_nNumRenderedObjects(0),m_RenderType(RT_TRIANGLELIST)
// {
//     // 创建一个空材质作为渲染器的缺省材质
//     //m_pDefaultMaterial = F3DMaterialManager::Instance().CreateMaterial( "default" );
// }
// 
// /** 析构函数
// */
// F3DRenderer::~F3DRenderer(void)
// {
// }
// 
// /** 添加一个渲染对象到队列
// @Param 渲染对象的指针
// @Param 要添加到那个渲染队列
// */
// void F3DRenderer::AddRenderable( F3DRenderable* pObject,uint32 nQueueID/* =RENDERQUEUE_DEFAULT */ )
// {
//     FASSERT( nQueueID < MAX_NUM_RENDERQUEUE );
//     m_RenderQueues[nQueueID].push_back( pObject );
// }
// 
// /** 渲染指定的数据
// @Param 指定数据的顶点类型
// @Param 顶点的数目
// @Param 索引的数目
// @Param 顶点缓冲区的指针
// @Param 索引缓冲区的指针
// */
// void F3DRenderer::RenderBuffer( FVertexType nVType,uint32 nNumVerts,uint32 nNumIndis,
//                                 F3DGraphicBuffer* pVB,F3DGraphicBuffer* pIB )
// {
//     RenderBuffer( nVType,0,0,nNumVerts,nNumIndis,pVB,pIB );
// }
// 
// /** 渲染指定的数据
// @Param 指定数据的顶点类型
// @Param 顶点的起始位置
// @Param 顶点的数目
// @Param 顶点缓冲区的指针
// */
// void F3DRenderer::RenderBuffer( FVertexType nVType,uint32 nVOffset,uint32 nNumVerts,
//                                 F3DGraphicBuffer* pVB )
// {
//     RenderBuffer( nVType,nVOffset,0,nNumVerts,0,pVB,NULL );
// }
// 
// /** 渲染指定的数据
// @Param 指定数据的顶点类型
// @Param 顶点的数目
// @Param 顶点缓冲区的指针
// */
// void F3DRenderer::RenderBuffer( FVertexType nVType,uint32 nNumVerts,F3DGraphicBuffer* pVB )
// {
//     RenderBuffer( nVType,0,0,nNumVerts,0,pVB,NULL );
// }
// 
// /** 对指定的渲染队列进行排序
// @Param (模板参数)本渲染队列的ID
// @Param 要进行排序的渲染队列
// */
// template <int nQueueID>
// static void SortRenderQueue( std::vector<F3DRenderable*>& renderQueue )
// {
//     switch( nQueueID )
//     {
//     case RENDERQUEUE_DEFAULT:
//         std::sort( renderQueue.begin(),renderQueue.end(),FSortDefault() );
//         break;
//     case RENDERQUEUE_TRANSPARENT:
//         std::sort( renderQueue.begin(),renderQueue.end(),FSortTranparent() );
//         break;
//     case RENDERQUEUE_LAYOUT:
//         std::sort( renderQueue.begin(),renderQueue.end(),FSortLayout() );
//         break;
//     }
// }
// 
// /** 释放指定的渲染队列
// @Param 指定渲染队列的ID
// */
// void F3DRenderer::FlushRenderQueue( uint32 nQueueID )
// {
//     FASSERT( nQueueID < MAX_NUM_RENDERQUEUE );
// 
//     static void (*pSortFunc[MAX_NUM_RENDERQUEUE]) ( 
//         std::vector<F3DRenderable*>& renderQueue
//         ) =
//     {
//         SortRenderQueue<RENDERQUEUE_DEFAULT>,
//         SortRenderQueue<RENDERQUEUE_TRANSPARENT>,
//         SortRenderQueue<RENDERQUEUE_LAYOUT>,
//     };
// 
//     // 对渲染队列进行排序
//     pSortFunc[nQueueID]( m_RenderQueues[nQueueID] );
// 
//     RenderQueue::iterator it;
//     RenderQueue::iterator end = m_RenderQueues[nQueueID].end();
//     for( it=m_RenderQueues[nQueueID].begin();it!=end;it++ )
//     {
//         // 渲染该物体
//         (*it)->Render();
// 
//         m_nNumRenderedVerts += (*it)->GetNumVerts();
//         m_nNumRenderedIndis += (*it)->GetNumIndis();
//         m_nNumRenderedPrims += (*it)->GetNumPrims();
//     }
// 
//     // 清空渲染队列里的所有物体
//     m_nNumRenderedObjects += (uint32)m_RenderQueues[nQueueID].size();
//     m_RenderQueues[nQueueID].clear();
// }
// 
// /** 释放所有渲染队列
// */
// void F3DRenderer::FlushRenderQueue(void)
// {
//     for( uint32 rq=0;rq<MAX_NUM_RENDERQUEUE;rq++ )
//     {
//         FlushRenderQueue( rq );
//     }
// }