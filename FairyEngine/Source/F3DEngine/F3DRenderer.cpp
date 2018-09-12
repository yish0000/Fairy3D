/*
 * -----------------------------------------------------------------------
 *  Name:   F3DRenderer.cpp
 *  Desc:   ���ļ�����ʵ���������Ⱦ�����ࡣ
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
// /** ����ͨ��Ⱦ���н�������ĺ�����
// */
// struct FSortDefault
// {
//     bool operator () ( F3DRenderable* obj1,F3DRenderable* obj2 ) const
//     {
//         return true;
//     }
// };
// 
// /** �԰�͸����Ⱦ���н�������ĺ�����
// */
// struct FSortTranparent
// {
//     bool operator () ( F3DRenderable* obj1,F3DRenderable* obj2 ) const
//     {
//         return true;
//     }
// };
// 
// /** ��2D��ĻԪ�ؽ�������ĺ�����
// */
// struct FSortLayout
// {
//     bool operator () ( F3DRenderable* obj1,F3DRenderable* obj2 ) const
//     {
//         return true;
//     }
// };
// 
// /** ���캯��
// */
// F3DRenderer::F3DRenderer(void) : m_nNumRenderedVerts(0),m_nNumRenderedIndis(0),
//     m_nNumRenderedPrims(0),m_nNumRenderedObjects(0),m_RenderType(RT_TRIANGLELIST)
// {
//     // ����һ���ղ�����Ϊ��Ⱦ����ȱʡ����
//     //m_pDefaultMaterial = F3DMaterialManager::Instance().CreateMaterial( "default" );
// }
// 
// /** ��������
// */
// F3DRenderer::~F3DRenderer(void)
// {
// }
// 
// /** ���һ����Ⱦ���󵽶���
// @Param ��Ⱦ�����ָ��
// @Param Ҫ��ӵ��Ǹ���Ⱦ����
// */
// void F3DRenderer::AddRenderable( F3DRenderable* pObject,uint32 nQueueID/* =RENDERQUEUE_DEFAULT */ )
// {
//     FASSERT( nQueueID < MAX_NUM_RENDERQUEUE );
//     m_RenderQueues[nQueueID].push_back( pObject );
// }
// 
// /** ��Ⱦָ��������
// @Param ָ�����ݵĶ�������
// @Param �������Ŀ
// @Param ��������Ŀ
// @Param ���㻺������ָ��
// @Param ������������ָ��
// */
// void F3DRenderer::RenderBuffer( FVertexType nVType,uint32 nNumVerts,uint32 nNumIndis,
//                                 F3DGraphicBuffer* pVB,F3DGraphicBuffer* pIB )
// {
//     RenderBuffer( nVType,0,0,nNumVerts,nNumIndis,pVB,pIB );
// }
// 
// /** ��Ⱦָ��������
// @Param ָ�����ݵĶ�������
// @Param �������ʼλ��
// @Param �������Ŀ
// @Param ���㻺������ָ��
// */
// void F3DRenderer::RenderBuffer( FVertexType nVType,uint32 nVOffset,uint32 nNumVerts,
//                                 F3DGraphicBuffer* pVB )
// {
//     RenderBuffer( nVType,nVOffset,0,nNumVerts,0,pVB,NULL );
// }
// 
// /** ��Ⱦָ��������
// @Param ָ�����ݵĶ�������
// @Param �������Ŀ
// @Param ���㻺������ָ��
// */
// void F3DRenderer::RenderBuffer( FVertexType nVType,uint32 nNumVerts,F3DGraphicBuffer* pVB )
// {
//     RenderBuffer( nVType,0,0,nNumVerts,0,pVB,NULL );
// }
// 
// /** ��ָ������Ⱦ���н�������
// @Param (ģ�����)����Ⱦ���е�ID
// @Param Ҫ�����������Ⱦ����
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
// /** �ͷ�ָ������Ⱦ����
// @Param ָ����Ⱦ���е�ID
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
//     // ����Ⱦ���н�������
//     pSortFunc[nQueueID]( m_RenderQueues[nQueueID] );
// 
//     RenderQueue::iterator it;
//     RenderQueue::iterator end = m_RenderQueues[nQueueID].end();
//     for( it=m_RenderQueues[nQueueID].begin();it!=end;it++ )
//     {
//         // ��Ⱦ������
//         (*it)->Render();
// 
//         m_nNumRenderedVerts += (*it)->GetNumVerts();
//         m_nNumRenderedIndis += (*it)->GetNumIndis();
//         m_nNumRenderedPrims += (*it)->GetNumPrims();
//     }
// 
//     // �����Ⱦ���������������
//     m_nNumRenderedObjects += (uint32)m_RenderQueues[nQueueID].size();
//     m_RenderQueues[nQueueID].clear();
// }
// 
// /** �ͷ�������Ⱦ����
// */
// void F3DRenderer::FlushRenderQueue(void)
// {
//     for( uint32 rq=0;rq<MAX_NUM_RENDERQUEUE;rq++ )
//     {
//         FlushRenderQueue( rq );
//     }
// }