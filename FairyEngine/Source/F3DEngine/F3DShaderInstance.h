/*
 * ------------------------------------------------------------------------
 *  Name:   F3DShaderInstance.h
 *  Desc:   本文件定义了一个Shader程序实例。
 *  Author: Yish
 *  Date:   2011/4/1
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_SHADERINSTANCE_H__
#define __F3D_SHADERINSTANCE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DShaderObject.h"

///////////////////////////////////////////////////////////////////////////


/** 着色器程序实例
*/
class FAIRY_API F3DShaderInstance : public FGeneralAlloc
{
    typedef std::vector<int> IntConstantList;
    typedef std::vector<float> FloatConstantList;

    // 常量参数列表
    typedef std::vector<F3DShaderParameter*> ParameterList;

protected:
    F3DShaderObject* m_pShader;         // 着色器程序对象
    IntConstantList m_IntContants;      // 整形常量数据(物理索引)
    FloatConstantList m_FloatConstants; // 浮点常量数据(物理索引)
    ParameterList m_Parameters;         // 常量参数列表(高级Shader语言)

public:
    F3DShaderInstance(void);
    virtual ~F3DShaderInstance(void);

    // 绑定应用程序
    void SetShaderObject( F3DShaderObject* pShader );

    // 更新自动常量
    void UpdateAutoParameters( F3DAutoParameter* autoParameters );

    // 获取指定的常量
    F3DShaderParameter* GetParameterByName( const AString& name );
    F3DShaderParameter* GetParameterByIndex( size_t index );

    // 设置着色器常量(通过逻辑索引)
    void SetConstant( size_t index,int val );
    void SetConstant( size_t index,const int* val,size_t count );
    void SetConstant( size_t index,float val );
    void SetConstant( size_t index,const float* val,size_t count );
    void SetConstant( size_t index,const F3DVector3& vec );
    void SetConstant( size_t index,const F3DVector3* vec,size_t count );
    void SetConstant( size_t index,const F3DMatrix4& m );
    void SetConstant( size_t index,const F3DMatrix4* m,size_t count );
    void SetConstant( size_t index,const F3DColor& clr );

    // 获取本实例的着色器程序对象
    F3DShaderObject* GetShaderObject(void) { return m_pShader; }

private:
    // 为指定的逻辑常量分配物理空间(浮点)
    size_t AllocPhysicalBufferF( size_t logicIndex,size_t bufSize );

    // 为指定的逻辑常量分配物理空间(整形)
    size_t AllocPhysicalBufferI( size_t logicIndex,size_t bufSize );

    // 在指定的物理索引处写入整形数据
    void WriteConstants( size_t physicalIndex,const int* data,size_t dataCount );

    // 在指定的物理索引处写入浮点数据
    void WriteConstants( size_t physicalIndex,const float* data,size_t dataCount );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_SHADERINSTANCE_H__