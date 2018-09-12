/*
 * -----------------------------------------------------------------------
 *  Name:   F3DShaderInstance.cpp
 *  Desc:   本文件为引擎实现了一个着色器程序实例类。
 *  Author: Yish
 *  Date:   2011/5/22
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DShaderInstance.h"


/** 构造函数
*/
F3DShaderInstance::F3DShaderInstance(void) : m_pShader(NULL)
{
}

/** 析构函数
*/
F3DShaderInstance::~F3DShaderInstance(void)
{
    ParameterList::iterator it;
    for( it=m_Parameters.begin();it!=m_Parameters.end();it++ )
    {
        F_SAFE_DELETE( *it );
    }
}


/** 绑定应用程序
@Param 该实例所绑定的GPU程序
*/
void F3DShaderInstance::SetShaderObject( F3DShaderObject* pShader )
{
    // 获取已命名常量列表
    SShaderNamedConstants* constList = pShader->GetNamedContants();

    // 是否为高级Shader程序
    if( constList->constMap.size() > 0 )
    {
        if( constList->nIntBufferSize > m_IntContants.size() )
            m_IntContants.insert( m_IntContants.end(),constList->nIntBufferSize-m_IntContants.size(),0 );

        if( constList->nFloatBufferSize > m_FloatConstants.size() )
            m_FloatConstants.insert( m_FloatConstants.end(),constList->nFloatBufferSize-m_FloatConstants.size(),0.0f );

        // 创建参数列表
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

/** 更新自动常量
@Param 指定的自动常量数据
*/
void F3DShaderInstance::UpdateAutoParameters( F3DAutoParameter* autoParameters )
{
}

/** 获取指定的常量参数
@Param 常量的名字
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

/** 获取指定的常量参数
@Param 指定常量的索引
*/
F3DShaderParameter* F3DShaderInstance::GetParameterByIndex( size_t index )
{
    FASSERT( index < m_Parameters.size() );
    return m_Parameters[index];
}

/** 设置着色器常量
@Param 常量的逻辑索引
@Param 常量的值
*/
void F3DShaderInstance::SetConstant( size_t index,int val )
{
    SetConstant( index,&val,1 );
}

/** 设置着色器常量
@Param 常量的逻辑索引
@Param 常量的数据
@Param 要设置数组的大小
*/
void F3DShaderInstance::SetConstant( size_t index,const int* val,size_t count )
{
    size_t rawCount = count * 4;

    // 分配一个逻辑常量
    size_t physicalIndex = AllocPhysicalBufferI( index,rawCount );

    // 写入数据
    WriteConstants( physicalIndex,val,rawCount );
}

/** 设置着色器常量
@Param 常量的逻辑索引
@Param 常量的值
*/
void F3DShaderInstance::SetConstant( size_t index,float val )
{
    SetConstant( index,&val,1 );
}

/** 设置着色器常量
@Param 常量的逻辑索引
@Param 常量的数据
@Param 数组的大小
*/
void F3DShaderInstance::SetConstant( size_t index,const float* val,size_t count )
{
    size_t rawCount = count * 4;

    // 分配一个逻辑常量
    size_t physicalIndex = AllocPhysicalBufferF( index,rawCount );

    // 写入数据
    WriteConstants( physicalIndex,val,rawCount );
}

/** 设置着色器常量(向量)
@Param 常量的逻辑索引
@Param 常量的数据
*/
void F3DShaderInstance::SetConstant( size_t index,const F3DVector3& vec )
{
    SetConstant( index,&vec.x,1 );
}

/** 设置着色器常量(向量)
@Param 常量的逻辑索引
@Param 常量的数据(向量数组)
@Param 数组的大小
*/
void F3DShaderInstance::SetConstant( size_t index,const F3DVector3* vec,size_t count )
{
    SetConstant( index,&vec[0].x,count );
}

/** 设置着色器常量(矩阵)
@Param 常量的逻辑索引
@Param 常量的数据
*/
void F3DShaderInstance::SetConstant( size_t index,const F3DMatrix4& m )
{
    SetConstant( index,&m,4 );
}

/** 设置着色器常量(矩阵数组)
@Param 常量的逻辑索引
@Param 常量的数据
@Param 矩阵数组的大小
*/
void F3DShaderInstance::SetConstant( size_t index,const F3DMatrix4* m,size_t count )
{
    SetConstant( index,&m[0]._11,4*count );
}

/** 设置着色器常量(颜色)
@Param 常量的逻辑索引
@Param 常量的值
*/
void F3DShaderInstance::SetConstant( size_t index,const F3DColor& clr )
{
    SetConstant( index,&clr.fR,1 );
}

/** 为指定的逻辑常量分配物理空间
@Param 常量的逻辑索引
@Param 常量所占空间的大小
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

            // 添加逻辑索引到物理索引的映射
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

            // 将所有在此位置之后的物理索引向后推移
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

/** 为指定的逻辑常量分配物理空间
@Param 常量的逻辑索引
@Param 常量所占空间的大小
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

            // 添加逻辑索引到物理索引的映射
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

            // 将所有在此位置之后的物理索引向后推移
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

/** 在指定的物理索引处写入整形数据
@Param 物理索引
@Param 要写入的整形数据
@Param 整形数据的个数
*/
void F3DShaderInstance::WriteConstants( size_t physicalIndex,const int* data,
                                        size_t dataCount )
{
    FASSERT( physicalIndex + dataCount <= m_IntContants.size() );
    memcpy( &m_IntContants[physicalIndex],data,sizeof(int)*dataCount );
}

/** 在指定的物理索引处写入浮点数据
@Param 物理索引
@Param 要写入的浮点数据
@Param 浮点数据的个数
*/
void F3DShaderInstance::WriteConstants( size_t physicalIndex,const float* data,
                                        size_t dataCount )
{
    FASSERT( physicalIndex + dataCount <= m_FloatConstants.size() );
    memcpy( &m_FloatConstants[physicalIndex],data,sizeof(float)*dataCount );
}