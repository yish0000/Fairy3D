/*
 * -----------------------------------------------------------------------
 *  Name:   FlyShaderObject.h
 *  Desc:   ���ļ�Ϊ���涨����һ��Shader����Ļ��ࡣ
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


/** �߼�����
@remarks
    ��¼���߼���������Ӧ�����������ʹ�С�����ݣ����ڵͼ�Shader���ԡ�
*/
struct SShaderLogical
{
    size_t nPhysicalIndex;      // �ó�������Ӧ����������
    size_t nCurrentSize;        // �ó����Ĵ�С

    SShaderLogical( size_t index,size_t bufSize )
        : nPhysicalIndex(index),nCurrentSize(bufSize)
    {
    }
};

// ���߼�����������������ӳ��
typedef std::map<size_t,SShaderLogical> ShaderLogicalIndexMap;

/** �߼�����������
@remarks
    ��¼�����е��߼����������ڵͼ�Shader���ԡ�
*/
struct SShaderLogicalBuffer
{
    size_t nBufferSize;             // ����������Ĵ�С
    ShaderLogicalIndexMap logicMap; // �߼�����������������ӳ��

    // ���캯��
    SShaderLogicalBuffer(void) : nBufferSize(0) {}
};

/** ��ɫ������(���ڸ߼�Shader����)
*/
struct SShaderConstant
{
    EShaderVariableType dataType;   // ��������������
    size_t nLogicIndex;             // �ò������߼�����
    size_t nPhysicalIndex;          // �ò�������������
    size_t nElementSize;            // �ò�����ռ�õ�Ԫ�ظ���
    size_t nArraySize;              // ���鳣���Ĵ�С
};

typedef std::map<FString,SShaderConstant> ShaderConstantMap;

/** �������ĳ�����(���ڸ߼�Shader����)
*/
struct SShaderNamedConstants
{
    size_t nIntBufferSize;          // ���γ������ݵĴ�С
    size_t nFloatBufferSize;        // ���㳣�����ݵĴ�С
    ShaderConstantMap constMap;     // ����ӳ���

    SShaderNamedConstants(void)
        : nIntBufferSize(0),nFloatBufferSize(0)
    {
    }
};

/** ��ɫ������
@remarks
    ��ɫ�������ж�����ɫ����������ɫ�����֡�
@note
    ����һ�������ࡣ
*/
class _F3D_EXPORT FlyShaderObject : public GfxResourceAlloc
{
protected:
    AString m_sName;            // ��ɫ�����������
    EShaderType m_Type;         // ��ɫ�����������
    AString m_sLanguage;        // ��ɫ���������������
    AString m_sProfile;         // ��ɫ���汾�ַ���

    SShaderNamedConstants m_NamedConstants; // ��¼���������ĳ���(�߼�Shader����)
    SShaderLogicalBuffer m_IntLogicBuf;     // ��¼���߼�����������������ӳ��(�ͼ�Shader����)
    SShaderLogicalBuffer m_FloatLogicBuf;   // ��¼���߼�����������������ӳ��(�ͼ�Shader����)

public:
    FlyShaderObject( const FString& name );
    virtual ~FlyShaderObject(void);

    // ����ָ����Shader����
    virtual void CompileShader( const void* data ) = 0;

    // ����ɫ��ʵ���󶨵�����(���ó�������)
    virtual void BindParameters( FlyShaderInstance* pInstance ) = 0;

    // ����ɫ������Ӧ�õ��豸
    virtual void Apply(void) = 0;

    // ��ȡ�߼�����������������ӳ��(����)
    SShaderLogicalBuffer* GetLogicalBufferI(void) { return &m_IntLogicBuf; }

    // ��ȡ�߼�����������������ӳ��(����)
    SShaderLogicalBuffer* GetLogicalBufferF(void) { return &m_FloatLogicBuf; }

    // ��ȡ�������ĳ�����
    SShaderNamedConstants* GetNamedContants(void) { return &m_NamedConstants; }

    // ��ȡ��ɫ�����������
    EShaderType GetShaderType(void) const { return m_Type; }

    // ��ȡ��ɫ���������������
    const AString& GetLanguage(void) const { return m_sLanguage; }

    // ��ȡ��ɫ�����������
    const AString& GetName(void) const { return m_sName; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_SHADEROBJECT_H__