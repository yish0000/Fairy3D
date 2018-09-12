/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9VertexBuffer.cpp
 *  Desc:   ���ļ�ʵ����D3D9��Ⱦ���Ķ��㻺������
 *  Author: Yish
 *  Date:   2012/12/22
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FLogManager.h"
#include "FD3D9VertexBuffer.h"


/** ���캯��
@param D3D9�豸ָ��
@param ��������С
@param ��������USAGE
@param �������ڴ�ģʽ
*/
FD3D9VertexBuffer::FD3D9VertexBuffer( LPDIRECT3DDEVICE9 pd3dDevice,uint32 nSize,
	EBufferUsage Usage,EMemoryMode MemMode )
{
    m_pd3dDevice = pd3dDevice;
    m_pVB = NULL;
    m_nBufferSize = 0;

    m_Usage = Usage;
    m_MemMode = MemMode;
    Resize( nSize );
	m_pTempBuf = NULL;
}

/** ��������
*/
FD3D9VertexBuffer::~FD3D9VertexBuffer(void)
{
    SAFE_RELEASE( m_pVB );
	SAFE_DELETE_ARRAY( m_pTempBuf );
}

/** ���·��䶥�㻺�����Ĵ�С
*/
bool FD3D9VertexBuffer::Resize( uint32 nSize )
{
    HRESULT hr;
    DWORD dwUsage = 0;
    D3DPOOL pool;

    if( nSize != m_nBufferSize )
    {
        // ����ɵĻ�����
        SAFE_RELEASE( m_pVB );

        switch( m_MemMode )
        {
        case MM_DEFAULT:
            pool = D3DPOOL_DEFAULT;
            break;
        case MM_SYSMEMORY:
            pool = D3DPOOL_SYSTEMMEM;
            break;
        case MM_MANAGED:
            pool = D3DPOOL_MANAGED;
            break;
        }

        if( m_Usage & BU_DYNAMIC )
            dwUsage |= D3DUSAGE_DYNAMIC;
        else if( m_Usage & BU_WRITEONLY )
            dwUsage |= D3DUSAGE_WRITEONLY;

        hr = m_pd3dDevice->CreateVertexBuffer( nSize,dwUsage,0,pool,&m_pVB,NULL );
        if( FAILED(hr) )
		{
			FLOG_WARNING("FD3D9VertexBuffer::Resize(), Resize the vertex buffer failed!");
			return false;
		}

        m_nBufferSize = nSize;
    }

    return true;
}

/** �������㻺����
*/
void* FD3D9VertexBuffer::Lock( uint32 nOffset, uint32 nLength, ELockOption options )
{
    void* ptr;
    HRESULT hr;
    DWORD dwOptions = 0;

    switch( options )
    {
    case LOCK_DISCARD:
        dwOptions |= D3DLOCK_DISCARD;
        break;
    case LOCK_NOOVERWRITE:
        dwOptions |= D3DLOCK_NOOVERWRITE;
        break;
    case LOCK_READONLY:
        dwOptions |= D3DLOCK_READONLY;
        break;
    }

    hr = m_pVB->Lock( nOffset,nLength,&ptr,dwOptions );
    if( FAILED(hr) ) return NULL;

    return ptr;
}

/** ���㻺��������
*/
void FD3D9VertexBuffer::Unlock(void)
{
    m_pVB->Unlock();
}

/** ��ȡ���㻺����������
*/
void FD3D9VertexBuffer::ReadData( uint32 nOffset,uint32 nLength,void* pDest )
{
    void* ptr = Lock( nOffset,nLength,LOCK_READONLY );
    if( ptr )
	{
		memcpy( pDest,ptr,nLength );
		Unlock();
	}
}

/** �򶥵㻺������д������
*/
void FD3D9VertexBuffer::WriteData( uint32 nOffset,uint32 nLength,const void* pSource,bool bDiscard )
{
    void* ptr = Lock( nOffset,nLength,bDiscard ? LOCK_DISCARD : LOCK_NORMAL );
    if( ptr )
	{
		memcpy( ptr,pSource,nLength );
		Unlock();
	}
}

/** ��Ӧ�豸��ʧ
*/
void FD3D9VertexBuffer::OnDeviceLost(void)
{
    HRESULT hr;

    if( m_MemMode == MM_DEFAULT )
    {
		FASSERT(m_pTempBuf == NULL);
        m_pTempBuf = new BYTE[m_nBufferSize];
        if( !m_pTempBuf ) return;

        // �������ݵ���ʱ������
        BYTE* ptr;
        hr = m_pVB->Lock( 0,m_nBufferSize,(void**)&ptr,D3DLOCK_READONLY );
        if( FAILED(hr) ) return;
        memcpy( m_pTempBuf,ptr,m_nBufferSize );
        m_pVB->Unlock();

		// �ͷŶ��㻺����
        SAFE_RELEASE( m_pVB );
    }
}

/** ��Ӧ�豸�ָ�
*/
void FD3D9VertexBuffer::OnDeviceReset(void)
{
    HRESULT hr;
    DWORD dwUsage = 0;
    D3DPOOL pool;

    if( m_MemMode == MM_DEFAULT )
    {
		FASSERT(m_pTempBuf);

		// ����TEMP BUFFER������ݵ��µĻ�����
        switch( m_MemMode )
        {
        case MM_DEFAULT:
            pool = D3DPOOL_DEFAULT;
            break;
        case MM_SYSMEMORY:
            pool = D3DPOOL_SYSTEMMEM;
            break;
        case MM_MANAGED:
            pool = D3DPOOL_MANAGED;
            break;
        }

        if( m_Usage & BU_DYNAMIC )
            dwUsage |= D3DUSAGE_DYNAMIC;
        else if( m_Usage & BU_WRITEONLY )
            dwUsage |= D3DUSAGE_WRITEONLY;

        // �������㻺����
        hr = m_pd3dDevice->CreateVertexBuffer( m_nBufferSize,dwUsage,0,pool,&m_pVB,NULL );
        if( FAILED(hr) ) return;

        // д�����ݵ�������
        BYTE* ptr;
        hr = m_pVB->Lock( 0,m_nBufferSize,(VOID**)&ptr,0 );
        if( FAILED(hr) ) return;
        memcpy( ptr,m_pTempBuf,m_nBufferSize );
        m_pVB->Unlock();

        SAFE_DELETE_ARRAY( m_pTempBuf );
    }
}