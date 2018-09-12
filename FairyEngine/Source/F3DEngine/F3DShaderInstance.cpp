/*
 * -----------------------------------------------------------------------
 *  Name:   F3DShaderInstance.cpp
 *  Desc:   ���ļ�Ϊ����ʵ����һ����ɫ������ʵ���ࡣ
 *  Author: Yish
 *  Date:   2011/5/22
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DShaderInstance.h"


/** ���캯��
*/
F3DShaderInstance::F3DShaderInstance(void) : m_pShader(NULL)
{
}

/** ��������
*/
F3DShaderInstance::~F3DShaderInstance(void)
{
    ParameterList::iterator it;
    for( it=m_Parameters.begin();it!=m_Parameters.end();it++ )
    {
        F_SAFE_DELETE( *it );
    }
}


/** ��Ӧ�ó���
@Param ��ʵ�����󶨵�GPU����
*/
void F3DShaderInstance::SetShaderObject( F3DShaderObject* pShader )
{
    // ��ȡ�����������б�
    SShaderNamedConstants* constList = pShader->GetNamedContants();

    // �Ƿ�Ϊ�߼�Shader����
    if( constList->constMap.size() > 0 )
    {
        if( constList->nIntBufferSize > m_IntContants.size() )
            m_IntContants.insert( m_IntContants.end(),constList->nIntBufferSize-m_IntContants.size(),0 );

        if( constList->nFloatBufferSize > m_FloatConstants.size() )
            m_FloatConstants.insert( m_FloatConstants.end(),constList->nFloatBufferSize-m_FloatConstants.size(),0.0f );

        // ���������б�
        ShaderConstantMap::iterator it;
        for( it=constList->constMap.begin();it!=constList->constMap.end();it++ )
        {
            F3DShaderParameter* param = new F3DShaderParameter( it->first );
            param->SetVariable( it->second.dataType,it->second.nArraySize );
            m_Parameters.push_back( param );
        }
    }
    else
    {
        SShaderLogicalBuffer* logicInt = pShader->GetLogicalBufferI();
        SShaderLogicalBuffer* logicFloat = pShader->GetLogicalBufferF();

        if( logicInt->nBufferSize > m_IntContants.size() )
            m_IntContants.insert( m_IntContants.end(),logicInt->nBufferSize-m_IntContants.size(),0 );

        if( logicFloat->nBufferSize > m_FloatConstants.size() )
            m_FloatConstants.insert( m_FloatConstants.end(),logicFloat->nBufferSize-m_FloatConstants.size(),0.0f );
    }
}

/** �����Զ�����
@Param ָ�����Զ���������
*/
void F3DShaderInstance::UpdateAutoParameters( F3DAutoParameter* autoParameters )
{
}

/** ��ȡָ���ĳ�������
@Param ����������
*/
F3DShaderParameter* F3DShaderInstance::GetParameterByName( const AString& name )
{
    ParameterList::iterator it;
    for( it=m_Parameters.begin();it!=m_Parameters.end();it++ )
    {
        if( (*it)->GetName() == name )
            return (*it);
    }

    return NULL;
}

/** ��ȡָ���ĳ�������
@Param ָ������������
*/
F3DShaderParameter* F3DShaderInstance::GetParameterByIndex( size_t index )
{
    FASSERT( index < m_Parameters.size() );
    return m_Parameters[index];
}

/** ������ɫ������
@Param �������߼�����
@Param ������ֵ
*/
void F3DShaderInstance::SetConstant( size_t index,int val )
{
    SetConstant( index,&val,1 );
}

/** ������ɫ������
@Param �������߼�����
@Param ����������
@Param Ҫ��������Ĵ�С
*/
void F3DShaderInstance::SetConstant( size_t index,const int* val,size_t count )
{
    size_t rawCount = count * 4;

    // ����һ���߼�����
    size_t physicalIndex = AllocPhysicalBufferI( index,rawCount );

    // д������
    WriteConstants( physicalIndex,val,rawCount );
}

/** ������ɫ������
@Param �������߼�����
@Param ������ֵ
*/
void F3DShaderInstance::SetConstant( size_t index,float val )
{
    SetConstant( index,&val,1 );
}

/** ������ɫ������
@Param �������߼�����
@Param ����������
@Param ����Ĵ�С
*/
void F3DShaderInstance::SetConstant( size_t index,const float* val,size_t count )
{
    size_t rawCount = count * 4;

    // ����һ���߼�����
    size_t physicalIndex = AllocPhysicalBufferF( index,rawCount );

    // д������
    WriteConstants( physicalIndex,val,rawCount );
}

/** ������ɫ������(����)
@Param �������߼�����
@Param ����������
*/
void F3DShaderInstance::SetConstant( size_t index,const F3DVector3& vec )
{
    SetConstant( index,&vec.x,1 );
}

/** ������ɫ������(����)
@Param �������߼�����
@Param ����������(��������)
@Param ����Ĵ�С
*/
void F3DShaderInstance::SetConstant( size_t index,const F3DVector3* vec,size_t count )
{
    SetConstant( index,&vec[0].x,count );
}

/** ������ɫ������(����)
@Param �������߼�����
@Param ����������
*/
void F3DShaderInstance::SetConstant( size_t index,const F3DMatrix4& m )
{
    SetConstant( index,&m,4 );
}

/** ������ɫ������(��������)
@Param �������߼�����
@Param ����������
@Param ��������Ĵ�С
*/
void F3DShaderInstance::SetConstant( size_t index,const F3DMatrix4* m,size_t count )
{
    SetConstant( index,&m[0]._11,4*count );
}

/** ������ɫ������(��ɫ)
@Param �������߼�����
@Param ������ֵ
*/
void F3DShaderInstance::SetConstant( size_t index,const F3DColor& clr )
{
    SetConstant( index,&clr.fR,1 );
}

/** Ϊָ�����߼�������������ռ�
@Param �������߼�����
@Param ������ռ�ռ�Ĵ�С
*/
size_t F3DShaderInstance::AllocPhysicalBufferI( size_t logicIndex,size_t bufSize )
{
    size_t physicalIndex;
    SShaderLogicalBuffer* logicBuf = m_pShader->GetLogicalBufferI();

    ShaderLogicalIndexMap::iterator it;
    it = logicBuf->logicMap.find( logicIndex );
    if( it == logicBuf->logicMap.end() )
    {
        if( bufSize )
        {
            physicalIndex = m_IntContants.size();
            m_IntContants.insert( m_IntContants.end(),bufSize,0 );
            logicBuf->nBufferSize = m_IntContants.size();

            // ����߼�����������������ӳ��
//             size_t currPhys = physicalIndex;
//             size_t count = bufSize / 4;
//             for( size_t logici=0;logici<count;logici++ )
//             {
//                 logicBuf->logicMap.insert( ShaderLogicalIndexMap::value_type(
//                     logicIndex+logici,SShaderLogical(currPhys,bufSize)) );
//                 currPhys += 4;
//             }
            logicBuf->logicMap.insert( ShaderLogicalIndexMap::value_type(
                logicIndex,SShaderLogical(physicalIndex,bufSize)) );
        }
        else
        {
            return -1;
        }
    }
    else
    {
        physicalIndex = it->second.nPhysicalIndex;

        if( it->second.nCurrentSize < bufSize )
        {
            size_t insertCount = bufSize - it->second.nCurrentSize;
            IntConstantList::iterator insertPos = m_IntContants.begin();
            std::advance( insertPos,physicalIndex );
            m_IntContants.insert( insertPos,insertCount,0 );
            it->second.nCurrentSize = bufSize;

            // �������ڴ�λ��֮������������������
            ShaderLogicalIndexMap::iterator it2;
            for( it2=logicBuf->logicMap.begin();it2!=logicBuf->logicMap.end();it2++ )
            {
                if( it2->second.nPhysicalIndex > physicalIndex )
                    it2->second.nPhysicalIndex += insertCount;
            }
        }
    }

    return physicalIndex;
}

/** Ϊָ�����߼�������������ռ�
@Param �������߼�����
@Param ������ռ�ռ�Ĵ�С
*/
size_t F3DShaderInstance::AllocPhysicalBufferF( size_t logicIndex,size_t bufSize )
{
    size_t physicalIndex;
    SShaderLogicalBuffer* logicBuf = m_pShader->GetLogicalBufferF();

    ShaderLogicalIndexMap::iterator it;
    it = logicBuf->logicMap.find( logicIndex );
    if( it == logicBuf->logicMap.end() )
    {
        if( bufSize )
        {
            physicalIndex = m_FloatConstants.size();
            m_FloatConstants.insert( m_FloatConstants.end(),bufSize,0.0f );
            logicBuf->nBufferSize = m_FloatConstants.size();

            // ����߼�����������������ӳ��
//             size_t currPhys = physicalIndex;
//             size_t count = bufSize / 4;
//             for( size_t logici=0;logici<count;logici++ )
//             {
//             }
            logicBuf->logicMap.insert( ShaderLogicalIndexMap::value_type(
                logicIndex,SShaderLogical(physicalIndex,bufSize)) );
        }
        else
        {
            return -1;
        }
    }
    else
    {
        physicalIndex = it->second.nPhysicalIndex;

        if( it->second.nCurrentSize < bufSize )
        {
            size_t insertCount = bufSize - it->second.nCurrentSize;
            FloatConstantList::iterator insertPos = m_FloatConstants.begin();
            std::advance( insertPos,physicalIndex );
            m_FloatConstants.insert( insertPos,insertCount,0.0f );

            // �������ڴ�λ��֮������������������
            ShaderLogicalIndexMap::iterator it2;
            for( it2=logicBuf->logicMap.begin();it2!=logicBuf->logicMap.end();it2++ )
            {
                if( it2->second.nPhysicalIndex > physicalIndex )
                    it2->second.nPhysicalIndex += insertCount;
            }
        }
    }

    return physicalIndex;
}

/** ��ָ��������������д����������
@Param ��������
@Param Ҫд�����������
@Param �������ݵĸ���
*/
void F3DShaderInstance::WriteConstants( size_t physicalIndex,const int* data,
                                        size_t dataCount )
{
    FASSERT( physicalIndex + dataCount <= m_IntContants.size() );
    memcpy( &m_IntContants[physicalIndex],data,sizeof(int)*dataCount );
}

/** ��ָ��������������д�븡������
@Param ��������
@Param Ҫд��ĸ�������
@Param �������ݵĸ���
*/
void F3DShaderInstance::WriteConstants( size_t physicalIndex,const float* data,
                                        size_t dataCount )
{
    FASSERT( physicalIndex + dataCount <= m_FloatConstants.size() );
    memcpy( &m_FloatConstants[physicalIndex],data,sizeof(float)*dataCount );
}