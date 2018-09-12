/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9Resource.cpp
 *  Desc:   ���ļ�Ϊ��Ⱦ�豸ģ��ʵ����D3D��Դ�ӿڡ�
 *  Author: Yish
 *  Date:   2011/7/6
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FD3D9Resource.h"
#include "FD3D9RenderDevice.h"


extern FD3D9RenderDevice* g_pDevice;

/** ���캯��
*/
FD3D9Resource::FD3D9Resource(void)
{
    g_pDevice->AddUnmanagedResource( this );
}

/** ��������
*/
FD3D9Resource::~FD3D9Resource(void)
{
    g_pDevice->RemoveUnmanagedResource( this );
}