/*
 * ------------------------------------------------------------------------
 *  Name:   FPerlinNoise.h
 *  Desc:   This file define a class for generating 1D, 2D, 3D perlin noise.
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
#include "FTemplate.h"

///////////////////////////////////////////////////////////////////////////

/** Class for generating 1D, 2D, 3D perlin noise.
	From: https://mrl.nyu.edu/~perlin/doc/oscar.html#noise
*/
class FPerlinNoise
{
	enum
	{
		PERMUTATION_SIZE = 256,
		MAX_OCTAVE = 16,
	};

public:
	FPerlinNoise();
	FPerlinNoise(bool bUseRandSeed, uint32 nRandSeed);
	virtual ~FPerlinNoise() {}

	// Generate a 1D noise value.
	float Noise1D(float x);
	// Generate a 2D noise value.
	float Noise2D(float x, float y);
	// Generate a 3D noise value.
	float Noise3D(float x, float y, float z);

	// Set the octave parameter for more natural noise.
	void SetOctaveParam(int nOctave, int nWaveLen, float fAmplitude, float fPersistence);

protected:
	bool m_bUseRandSeed;
	int m_nRandomSeed;
	int m_permutation[PERMUTATION_SIZE * 2 + 2];
	float g3[PERMUTATION_SIZE + PERMUTATION_SIZE + 2][3];
	float g2[PERMUTATION_SIZE + PERMUTATION_SIZE + 2][2];
	float g1[PERMUTATION_SIZE + PERMUTATION_SIZE + 2];
	int m_nOctave;
	float m_fAmplitude;
	int m_nWaveLength;
	float m_fPersistence;

	// Do some initialization work.
	// If bNewPermutation is true, we will generate the noise the specified seed, otherwise nRandSeed is invalid.
	bool Init(bool bUseRandSeed, uint32 nRandSeed);
	// Generate a seudo-random number.
	int RandInteger();

	// Generate a 1D noise value once.
	float Noise1DOnce(float x);
	// Generate a 2D noise value once.
	float Noise2DOnce(float x, float y);
	// Generate a 3D noise value once.
	float Noise3DOnce(float x, float y, float z);

	inline float Curve(float t) { return (t * t * (3.0f - 2.0f * t)); }
	inline float Fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }
	inline float Lerp(float t, float a, float b) { return (a + t * (b - a)); }
	inline float Clamp(float v)
	{
		v += 0.5f;
		return v;// F_Clamp2(v, 0.0f, 1.0f);
	}

	inline void Normalize2(float v[2])
	{
		float fLen = sqrt(v[0] * v[0] + v[1] * v[1]);
		if (fLen > 0.0f)
		{
			float s = 1.0f / fLen;
			v[0] *= s;
			v[1] *= s;
		}
	}

	inline void Normalize3(float v[3])
	{
		float fLen = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		if (fLen > 0.0f)
		{
			float s = 1.0f / fLen;
			v[0] *= s;
			v[1] *= s;
			v[2] *= s;
		}
	}
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_PERLINNOISE_H__