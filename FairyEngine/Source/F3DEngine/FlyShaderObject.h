/*
 * -----------------------------------------------------------------------
 *  Name:   FlyShaderObject.h
 *  Desc:   本文件为引擎定义了一个Shader对象的基类。
 *  Author: Yish
 *  Date:   2011/1/10
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __FLY_SHADEROBJECT_H__
#define __FLY_SHADEROBJECT_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyShaderParameter.h"

//////////////////////////////////////////////////////////////////////////


/** 逻辑常量
@remarks
    记录了逻辑常量所对应的物理索引和大小等数据，用于低级Shader语言。
*/
struct SShaderLogical
{
    size_t nPhysicalIndex;      // 该常量所对应的物理索引
    size_t nCurrentSize;        // 该常量的大小

    SShaderLogical( size_t index,size_t bufSize )
        : nPhysicalIndex(index),nCurrentSize(bufSize)
    {
    }
};

// 从逻辑索引到物理索引的映射
typedef std::map<size_t,SShaderLogical> ShaderLogicalIndexMap;

/** 逻辑常量缓冲区
@remarks
    记录了所有的逻辑常量，用于低级Shader语言。
*/
struct SShaderLogicalBuffer
{
    size_t nBufferSize;             // 缓冲区所需的大小
    ShaderLogicalIndexMap logicMap; // 逻辑索引到物理索引的映射

    // 构造函数
    SShaderLogicalBuffer(void) : nBufferSize(0) {}
};

/** 着色器常量(用于高级Shader语言)
*/
struct SShaderConstant
{
    EShaderVariableType dataType;   // 常量参数的类型
    size_t nLogicIndex;             // 该参数的逻辑索引
    size_t nPhysicalIndex;          // 该参数的物理索引
    size_t nElementSize;            // 该参数所占用的元素个数
    size_t nArraySize;              // 数组常量的大小
};

typedef std::map<FString,SShaderConstant> ShaderConstantMap;

/** 已命名的常量集(用于高级Shader语言)
*/
struct SShaderNamedConstants
{
    size_t nIntBufferSize;          // 整形常量数据的大小
    size_t nFloatBufferSize;        // 浮点常量数据的大小
    ShaderConstantMap constMap;     // 常量映射表

    SShaderNamedConstants(void)
        : nIntBufferSize(0),nFloatBufferSize(0)
    {
    }
};

/** 着色器对象
@remarks
    着色器程序有顶点着色器和像素着色器两种。
@note
    这是一个抽象类。
*/
class _F3D_EXPORT FlyShaderObject : public GfxResourceAlloc
{
protected:
    AString m_sName;            // 着色器程序的名字
    EShaderType m_Type;         // 着色器程序的类型
    AString m_sLanguage;        // 着色器程序的语言类型
    AString m_sProfile;         // 着色器版本字符串

    SShaderNamedConstants m_NamedConstants; // 记录了已命名的常量(高级Shader语言)
    SShaderLogicalBuffer m_IntLogicBuf;     // 记录了逻辑常量到物理索引的映射(低级Shader语言)
    SShaderLogicalBuffer m_FloatLogicBuf;   // 记录了逻辑常量到物理索引的映射(低级Shader语言)

public:
    FlyShaderObject( const FString& name );
    virtual ~FlyShaderObject(void);

    // 编译指定的Shader程序
    virtual void CompileShader( const void* data ) = 0;

    // 将着色器实例绑定到程序(设置常量参数)
    virtual void BindParameters( FlyShaderInstance* pInstance ) = 0;

    // 将着色器程序应用到设备
    virtual void Apply(void) = 0;

    // 获取逻辑常量到物理索引的映射(整形)
    SShaderLogicalBuffer* GetLogicalBufferI(void) { return &m_IntLogicBuf; }

    // 获取逻辑常量到物理索引的映射(浮点)
    SShaderLogicalBuffer* GetLogicalBufferF(void) { return &m_FloatLogicBuf; }

    // 获取已命名的常量表
    SShaderNamedConstants* GetNamedContants(void) { return &m_NamedConstants; }

    // 获取着色器程序的类型
    EShaderType GetShaderType(void) const { return m_Type; }

    // 获取着色器程序的语言类型
    const AString& GetLanguage(void) const { return m_sLanguage; }

    // 获取着色器对象的名字
    const AString& GetName(void) const { return m_sName; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_SHADEROBJECT_H__