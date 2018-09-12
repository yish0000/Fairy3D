/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9IndexBuffer.cpp
 *  Desc:   ���ļ�ʵ����һ��D3D9��Ⱦ����������������
 *  Author: Yish
 *  Date:   2012/12/22
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FLogManager.h"
#include "FD3D9IndexBuffer.h"


/** ���캯��
*/
FD3D9IndexBuffer::FD3D9IndexBuffer( LPDIRECT3DDEVICE9 pd3dDevice,uint32 nSize,
	EBufferUsage Usage,EMemoryMode MemMode,EIndexFormat Format )
{
	m_pd3dDevice = pd3dDevice;
	m_pIB = NULL;

	m_Usage = Usage;
	m_MemMode = MemMode;
	m_IFormat = Format;
	Resize( nSize );
}

/** ��������
*/
FD3D9IndexBuffer::~FD3D9IndexBuffer(void)
{
	SAFE_RELEASE( m_pIB );
}

/** ���·��������������Ĵ�С
*/
bool FD3D9IndexBuffer::Resize( uint32 nSize )
{
	HRESULT hr;
	DWORD dwUsage = 0;
	D3DPOOL pool;
	D3DFORMAT format;

	if( nSize != m_nBufferSize )
	{
		// ����ɵ�����
		SAFE_RELEASE( m_pIB );

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

		if( m_IFormat == IF_INDEX16 )
			format = D3DFMT_INDEX16;
		else if( m_IFormat == IF_INDEX32 )
			format = D3DFMT_INDEX32;

		hr = m_pd3dDevice->CreateIndexBuffer( nSize,dwUsage,format,pool,&m_pIB,NULL );
		if( FAILED(hr) )
		{
			FLOG_WARNING("FD3D9IndexBuffer::Resize(), Create the index buffer failed!");
			return false;
		}

		m_nBufferSize = nSize;
	}

	return true;
}

/** ��������������
*/
void* FD3D9IndexBuffer::Lock( uint32 nOffset,uint32 nLength,ELockOption options )
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

	hr = m_pIB->Lock( nOffset,nLength,&ptr,dwOptions );
	if( FAILED(hr) ) return NULL;

	return ptr;
}

/** ��������������
*/
void FD3D9IndexBuffer::Unlock(void)
{
	m_pIB->Unlock();
}

/** ��ȡ��������
*/
void FD3D9IndexBuffer::ReadData( uint32 nOffset,uint32 nLength,void* pDest )
{
	void* ptr = Lock( nOffset,nLength,LOCK_READONLY );
	if( !ptr ) return;

	memcpy( pDest,ptr,nLength );
	Unlock();
}

/** д����������
*/
void FD3D9IndexBuffer::WriteData( uint32 nOffset,uint32 nLength,const void* pSource,bool bDiscard )
{
	void* ptr = Lock( nOffset,nLength,bDiscard ? LOCK_DISCARD : LOCK_NORMAL );
	if( !ptr ) return;

	memcpy( ptr,pSource,nLength );
	Unlock();
}

/** ��Ӧ�豸��ʧ
*/
void FD3D9IndexBuffer::OnDeviceLost(void)
{
	HRESULT hr;

	if( m_MemMode == MM_DEFAULT )
	{
		m_pTempBuf = new BYTE[m_nBufferSize];
		if( !m_pTempBuf ) return;

		// �������ݵ���ʱ������
		BYTE* ptr;
		hr = m_pIB->Lock( 0,m_nBufferSize,(VOID**)&ptr,D3DLOCK_READONLY );
		memcpy( m_pTempBuf,ptr,m_nBufferSize );
		m_pIB->Unlock();

		// �ͷ�����������
		SAFE_RELEASE( m_pIB );
	}
}

/** ��Ӧ�����������ָ�
*/
void FD3D9IndexBuffer::OnDeviceReset(void)
{
	HRESULT hr;
	DWORD dwUsage = 0;
	D3DPOOL pool;
	D3DFORMAT indexFormat;

	if( m_MemMode == MM_DEFAULT )
	{
		// ������ʱ�������ڵ����ݵ��µ�����������
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

		indexFormat = (m_IFormat == IF_INDEX16) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;

		// �������㻺����
		hr = m_pd3dDevice->CreateIndexBuffer( m_nBufferSize,dwUsage,indexFormat,pool,&m_pIB,NULL );
		if( FAILED(hr) )
		{
			F_EXCEPTION("FD3D9IndexBuffer::OnDeviceReset", "Create the index buffer failed!");
			return;
		}

		// д����ʱ�������ڵ�����
		BYTE* ptr;
		hr = m_pIB->Lock( 0,m_nBufferSize,(VOID**)&ptr,0 );
		if( FAILED(hr) )
		{
			F_EXCEPTION("FD3D9IndexBuffer::OnDeviceReset", "Lock the index buffer failed!");
			return;
		}

		memcpy( ptr,m_pTempBuf,m_nBufferSize );
		m_pIB->Unlock();

		// ɾ����ʱ������
		F_SAFE_DELETE_ARRAY( m_pTempBuf );
	}
}