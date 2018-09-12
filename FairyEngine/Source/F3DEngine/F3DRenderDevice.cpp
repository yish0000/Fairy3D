/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRenderDevice.cpp
 *  Desc:   本文件为引擎实现了渲染设备的基类。
 *  Author: Yish
 *  Date:   2010/12/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DRenderDevice.h"


/** 构造函数
*/
F3DRenderDevice::F3DRenderDevice(void)
	: m_pRenderer(NULL), m_pMainWnd(NULL)
{
    // 初始化渲染状态集
//    m_RenderTargets.Init();

    // 初始化纹理层
//     for( uint32 i=0;i<MAX_TEXTURE_STAGES;i++ )
//     {
//         m_TextureStage[i].Init(i);
//     }
}

/** 析构函数
*/
F3DRenderDevice::~F3DRenderDevice(void)
{
}

/** 移除一个渲染目标
@Param 指定渲染目标的名字
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

/** 获取指定的渲染目标
@Param 指定渲染目标的名字
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

/** 更新所有的渲染目标
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

/** 截取当前的主屏幕保存到文件
@Param 要保存的文件的路径
@Param 是否保存前台缓冲区，保存后台缓冲区应在场景绘制结束，SwapBuffer之前调用。
*/
void F3DRenderDevice::PrintScreenToFile( const char* filename, bool bFront )
{
	if( !m_pMainWnd )
	{
		FLOG_ERROR("F3DRenderDevice::PrintScreenToFile, Can't find the main render window!");
		return;
	}

	// 保存主渲染窗口的内容
	m_pMainWnd->PrintScreenToFile( filename, bFront );
}