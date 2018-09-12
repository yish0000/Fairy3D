/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRenderDevice.cpp
 *  Desc:   ���ļ�Ϊ����ʵ������Ⱦ�豸�Ļ��ࡣ
 *  Author: Yish
 *  Date:   2010/12/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DRenderDevice.h"


/** ���캯��
*/
F3DRenderDevice::F3DRenderDevice(void)
	: m_pRenderer(NULL), m_pMainWnd(NULL)
{
    // ��ʼ����Ⱦ״̬��
//    m_RenderTargets.Init();

    // ��ʼ�������
//     for( uint32 i=0;i<MAX_TEXTURE_STAGES;i++ )
//     {
//         m_TextureStage[i].Init(i);
//     }
}

/** ��������
*/
F3DRenderDevice::~F3DRenderDevice(void)
{
}

/** �Ƴ�һ����ȾĿ��
@Param ָ����ȾĿ�������
*/
void F3DRenderDevice::RemoveRenderTarget( const AString& name )
{
    TargetQueue::iterator it;
    for( it=m_RenderTargets.begin();it!=m_RenderTargets.end();it++ )
    {
        F3DRenderTarget* pTarget = *it;
        
        if( pTarget->GetName() == name )
        {
            F_SAFE_DELETE(pTarget);
            m_RenderTargets.erase( it );

            FLOG_INFO( "Render target (" + name + ") is removed!" );
            break;
        }
    }
}

/** ��ȡָ������ȾĿ��
@Param ָ����ȾĿ�������
*/
F3DRenderTarget* F3DRenderDevice::GetRenderTarget( const AString& name )
{
    TargetQueue::iterator it;
    for( it=m_RenderTargets.begin();it!=m_RenderTargets.end();it++ )
    {
        if( (*it)->GetName() == name )
            return *it;
    }

    return NULL;
}

void F3DRenderDevice::Render(F3DRenderData* pData)
{
}

/** �������е���ȾĿ��
*/
// void F3DRenderDevice::UpdateAllTargets(void)
// {
//     TargetQueue::iterator it = m_RenderTargets.begin(),
//         end = m_RenderTargets.end();
// 
//     while( it != end )
//     {
//         (*it)->Update();
//         ++it;
//     }
// }

/** Swap the back buffer to front for each renderTargets.
*/
void F3DRenderDevice::SwapBuffers(void)
{
    TargetQueue::iterator it = m_RenderTargets.begin(),
        end = m_RenderTargets.end();

    while( it != end )
    {
        (*it)->SwapBuffers();
        ++it;
    }
}

/** Set the render states
*/
void F3DRenderDevice::SetRenderStates(const F3DRenderStateSet& rs)
{
}

/** Set the scissor rect
*/
void F3DRenderDevice::SetSchissorRect(const F2DRECT& rect)
{
}

/** ��ȡ��ǰ������Ļ���浽�ļ�
@Param Ҫ������ļ���·��
@Param �Ƿ񱣴�ǰ̨�������������̨������Ӧ�ڳ������ƽ�����SwapBuffer֮ǰ���á�
*/
void F3DRenderDevice::PrintScreenToFile( const char* filename, bool bFront )
{
	if( !m_pMainWnd )
	{
		FLOG_ERROR("F3DRenderDevice::PrintScreenToFile, Can't find the main render window!");
		return;
	}

	// ��������Ⱦ���ڵ�����
	m_pMainWnd->PrintScreenToFile( filename, bFront );
}