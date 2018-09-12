/*
 * ------------------------------------------------------------------------
 *  Name:   FPerlinNoise.h
 *  Desc:   本文件定义了引擎中柏林噪声函数相关的类。
 *  Author: Yish
 *  Date:   2012/1/31
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_PERLINNOISE_H__
#define __FAIRY_PERLINNOISE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

/** 柏林噪声函数基类
*/
class FPerlinNoiseBase
{
public:

    enum
    {
        MAX_OCTAVE = 16,
    };

    struct _NOISEVALUE
    {
        float v[3];
    };

public:
    FPerlinNoiseBase();
    virtual ~FPerlinNoiseBase() {}

protected:
    int m_iBaseWaveLength;
    float m_fBaseAmplitude;
    float m_fPersistence;

    int m_iActiveOctave;
    int m_iNumOctaves;
    int m_iStartPos[MAX_OCTAVE];
    int m_iWaveLength[MAX_OCTAVE];
    float m_fAmplitude[MAX_OCTAVE];
};

/** 一维柏林噪声函数
*/
class FPerlinNoise1D : public FPerlinNoiseBase
{
protected:
    int m_iBufferLen;
    _NOISEVALUE* m_pValues;

public:
    FPerlinNoise1D();

    bool Init( int iBufLen );

    // Get a value
    void GetValue( float x,float* pValues,int iNumValue );
};

/** 二维柏林噪声函数
*/
class FPerlinNoise2D : public FPerlinNoiseBase
{
protected:
    int m_iBufferWidth;
    int m_iBufferHeight;
    _NOISEVALUE* m_pValues;

public:
    FPerlinNoise2D();

    bool Init( int iWidth,int iHeight );

    // Get a value
    void GetValue( float x,float y,float* pValues,int iNumValue );
};

/** 三维柏林噪声函数
*/
class FPerlinNoise3D : public FPerlinNoiseBase
{
protected:
    int m_iBufferWidth;
    int m_iBufferHeight;
    int m_iBufferDepth;
    _NOISEVALUE* m_pValues;

public:
    FPerlinNoise3D();

    bool Init( int iWidth,int iHeight,int iDepth );

    // Get a value
    void GetValue( float x,float y,float z,float* pValues,int iNumValue );
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_PERLINNOISE_H__