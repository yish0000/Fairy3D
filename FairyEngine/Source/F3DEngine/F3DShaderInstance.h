/*
 * ------------------------------------------------------------------------
 *  Name:   F3DShaderInstance.h
 *  Desc:   ���ļ�������һ��Shader����ʵ����
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


/** ��ɫ������ʵ��
*/
class FAIRY_API F3DShaderInstance : public FGeneralAlloc
{
    typedef std::vector<int> IntConstantList;
    typedef std::vector<float> FloatConstantList;

    // ���������б�
    typedef std::vector<F3DShaderParameter*> ParameterList;

protected:
    F3DShaderObject* m_pShader;         // ��ɫ���������
    IntConstantList m_IntContants;      // ���γ�������(��������)
    FloatConstantList m_FloatConstants; // ���㳣������(��������)
    ParameterList m_Parameters;         // ���������б�(�߼�Shader����)

public:
    F3DShaderInstance(void);
    virtual ~F3DShaderInstance(void);

    // ��Ӧ�ó���
    void SetShaderObject( F3DShaderObject* pShader );

    // �����Զ�����
    void UpdateAutoParameters( F3DAutoParameter* autoParameters );

    // ��ȡָ���ĳ���
    F3DShaderParameter* GetParameterByName( const AString& name );
    F3DShaderParameter* GetParameterByIndex( size_t index );

    // ������ɫ������(ͨ���߼�����)
    void SetConstant( size_t index,int val );
    void SetConstant( size_t index,const int* val,size_t count );
    void SetConstant( size_t index,float val );
    void SetConstant( size_t index,const float* val,size_t count );
    void SetConstant( size_t index,const F3DVector3& vec );
    void SetConstant( size_t index,const F3DVector3* vec,size_t count );
    void SetConstant( size_t index,const F3DMatrix4& m );
    void SetConstant( size_t index,const F3DMatrix4* m,size_t count );
    void SetConstant( size_t index,const F3DColor& clr );

    // ��ȡ��ʵ������ɫ���������
    F3DShaderObject* GetShaderObject(void) { return m_pShader; }

private:
    // Ϊָ�����߼�������������ռ�(����)
    size_t AllocPhysicalBufferF( size_t logicIndex,size_t bufSize );

    // Ϊָ�����߼�������������ռ�(����)
    size_t AllocPhysicalBufferI( size_t logicIndex,size_t bufSize );

    // ��ָ��������������д����������
    void WriteConstants( size_t physicalIndex,const int* data,size_t dataCount );

    // ��ָ��������������д�븡������
    void WriteConstants( size_t physicalIndex,const float* data,size_t dataCount );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_SHADERINSTANCE_H__