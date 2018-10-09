/*
 * ------------------------------------------------------------------------
 *  Name:   FPerlinNoise.cpp
 *  Desc:   This file define a class for generating 1D, 2D, 3D perlin noise.
 *  Author: Yish
 *  Date:   2012/2/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FPerlinNoise.h"

#define setup(v,b0,b1,r0,r1)\
	t = v + N;\
	b0 = ((int)t) & BM;\
	b1 = (b0+1) & BM;\
	r0 = t - (int)t;\
	r1 = r0 - 1.0f;

#define N 0x1000
#define BM 0xff
#define dot2(rx,ry) ( rx * q[0] + ry * q[1] )
#define dot3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

///////////////////////////////////////////////////////////////////////////
//  
//  Implement class FPerpinNoise
//  
///////////////////////////////////////////////////////////////////////////

FPerlinNoise::FPerlinNoise()
{
	Init(false, 0);
}

FPerlinNoise::FPerlinNoise(bool bUseRandSeed, uint32 nRandSeed)
{
	Init(bUseRandSeed, nRandSeed);
}

// Do some initialization work.
bool FPerlinNoise::Init(bool bUseRandSeed, uint32 nRandSeed)
{
	m_bUseRandSeed = bUseRandSeed;
	m_nRandomSeed = nRandSeed;
	m_nOctave = 1;
	m_fAmplitude = 1.0f;
	m_nWaveLength = 1;
	m_fPersistence = 1.0f;

	int i, j;
	int p[PERMUTATION_SIZE];
	for (i = 0; i < PERMUTATION_SIZE; i++)
	{
		p[i] = i;
	}
	while (--i)
	{
		int k = p[i];
		p[i] = p[j = RandInteger() % PERMUTATION_SIZE];
		p[j] = k;
	}
	for (i = 0; i < PERMUTATION_SIZE; i++)
		m_permutation[i] = p[i];
	for (i = 0; i < PERMUTATION_SIZE + 2; i++)
		m_permutation[PERMUTATION_SIZE + i] = m_permutation[i];

	for (i = 0; i < PERMUTATION_SIZE; i++)
	{
		g1[i] = (float)(RandInteger() % (PERMUTATION_SIZE + 1)) / PERMUTATION_SIZE;

		for (j = 0; j < 2; j++)
			g2[i][j] = (float)((RandInteger() % (PERMUTATION_SIZE + PERMUTATION_SIZE)) - PERMUTATION_SIZE) / PERMUTATION_SIZE;
		Normalize2(g2[i]);

		for (j = 0; j < 3; j++)
			g3[i][j] = (float)((RandInteger() % (PERMUTATION_SIZE + PERMUTATION_SIZE)) - PERMUTATION_SIZE) / PERMUTATION_SIZE;
		Normalize3(g3[i]);
	}

	for (i = 0; i < PERMUTATION_SIZE + 2; i++)
	{
		g1[PERMUTATION_SIZE + i] = g1[i];
		for (j = 0; j < 2; j++)
			g2[PERMUTATION_SIZE + i][j] = g2[i][j];
		for (j = 0; j < 3; j++)
			g3[PERMUTATION_SIZE + i][j] = g3[i][j];
	}

	return true;
}

// Generate a seudo - random number
int FPerlinNoise::RandInteger()
{
	if (m_bUseRandSeed)
	{
		m_nRandomSeed = ((int64)16807 * m_nRandomSeed) % (int64)2147483647;
		return m_nRandomSeed;
	}
	else
	{
		return rand();
	}
}

// Generate a 1D noise value.
float FPerlinNoise::Noise1DOnce(float x)
{
	int bx0, bx1;
	float rx0, rx1, sx, t, u, v;

	setup(x, bx0, bx1, rx0, rx1);

	sx = Curve(rx0);

	u = g1[m_permutation[bx0]];
	v = g1[m_permutation[bx1]];

	return Lerp(sx, u, v);
}

// Generate a 2D noise value.
float FPerlinNoise::Noise2DOnce(float x, float y)
{
	int bx0, bx1, by0, by1, b00, b10, b01, b11;
	float rx0, rx1, ry0, ry1, *q, sx, sy, a, b, t, u, v;
	register int i, j;

	setup(x, bx0, bx1, rx0, rx1);
	setup(y, by0, by1, ry0, ry1);

	i = m_permutation[bx0];
	j = m_permutation[bx1];

	b00 = m_permutation[i + by0];
	b10 = m_permutation[j + by0];
	b01 = m_permutation[i + by1];
	b11 = m_permutation[j + by1];

	sx = Curve(rx0);
	sy = Curve(ry0);

	q = g2[b00]; u = dot2(rx0, ry0);
	q = g2[b10]; v = dot2(rx1, ry0);
	a = Lerp(sx, u, v);

	q = g2[b01]; u = dot2(rx0, ry1);
	q = g2[b11]; v = dot2(rx1, ry1);
	b = Lerp(sx, u, v);

	return Lerp(sy, a, b);
}

// Generate a 3D noise value.
float FPerlinNoise::Noise3DOnce(float x, float y, float z)
{
	int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
	float rx0, rx1, ry0, ry1, rz0, rz1, *q, sx, sy, sz, a, b, c, d, t, u, v;
	register int i, j;

	setup(x, bx0, bx1, rx0, rx1);
	setup(y, by0, by1, ry0, ry1);
	setup(z, bz0, bz1, rz0, rz1);

	i = m_permutation[bx0];
	j = m_permutation[bx1];

	b00 = m_permutation[i + by0];
	b10 = m_permutation[j + by0];
	b01 = m_permutation[i + by1];
	b11 = m_permutation[j + by1];

	sx = Curve(rx0);
	sy = Curve(ry0);
	sz = Curve(rz0);

	q = g3[b00 + bz0]; u = dot3(rx0, ry0, rz0);
	q = g3[b10 + bz0]; v = dot3(rx1, ry0, rz0);
	a = Lerp(sx, u, v);

	q = g3[b01 + bz0]; u = dot3(rx0, ry1, rz0);
	q = g3[b11 + bz0]; v = dot3(rx1, ry1, rz0);
	b = Lerp(sx, u, v);

	c = Lerp(sy, a, b);

	q = g3[b00 + bz1]; u = dot3(rx0, ry0, rz1);
	q = g3[b10 + bz1]; v = dot3(rx1, ry0, rz1);
	a = Lerp(sx, u, v);

	q = g3[b01 + bz1]; u = dot3(rx0, ry1, rz1);
	q = g3[b11 + bz1]; v = dot3(rx1, ry1, rz1);
	b = Lerp(sx, u, v);

	d = Lerp(sy, a, b);

	return Lerp(sz, c, d);
}

// Set the octave parameter to generate more natural noise.
void FPerlinNoise::SetOctaveParam(int nOctave, int nWaveLen, float fAmplitude, float fPersistence)
{
	m_nOctave = nOctave;
	F_Clamp(m_nOctave, 1, (int)MAX_OCTAVE);
	m_fAmplitude = fAmplitude;
	F_ClampFloor(m_fAmplitude, 0.0f);
	m_nWaveLength = nWaveLen;
	F_ClampFloor(m_nWaveLength, 1);
	m_fPersistence = fPersistence;
	F_ClampFloor(m_fPersistence, 0.0f);

	int nCurWaveLen = m_nWaveLength;
	for (int i = 0; i < m_nOctave; i++)
	{
		nCurWaveLen /= 2;
		if (nCurWaveLen <= 0)
		{
			m_nOctave = i + 1;
			break;
		}
	}
}

// Generate a 1D noise value.
float FPerlinNoise::Noise1D(float x)
{
	float fTotal = 0.0f;
	int nWaveLen = m_nWaveLength;
	float fAmplitude = 1.0f;
	float fFrequency = 1.0f / m_nWaveLength;
	float fMaxValue = 0.0f;  // Used for normalizing result to 0.0 - 1.0
	for (int i = 0; i < m_nOctave; i++)
	{
		fTotal += Noise1DOnce(x * fFrequency) * fAmplitude;
		fMaxValue += fAmplitude;
		fAmplitude *= m_fPersistence;
		fFrequency *= 2;
	}

	return fTotal / fMaxValue * m_fAmplitude;
}

// Generate a 2D noise value.
float FPerlinNoise::Noise2D(float x, float y)
{
	float fTotal = 0.0f;
	int nWaveLen = m_nWaveLength;
	float fAmplitude = 1.0f;
	float fFrequency = 1.0f / m_nWaveLength;
	float fMaxValue = 0.0f;  // Used for normalizing result to 0.0 - 1.0
	for (int i = 0; i < m_nOctave; i++)
	{
		float fValue = Noise2DOnce(x * fFrequency, y * fFrequency);
		fTotal += Clamp(fValue) * fAmplitude;
		fMaxValue += fAmplitude;
		fAmplitude *= m_fPersistence;
		fFrequency *= 2;
	}

	return fTotal / fMaxValue * m_fAmplitude;
}

// Generate a 3D noise value.
float FPerlinNoise::Noise3D(float x, float y, float z)
{
	float fTotal = 0.0f;
	int nWaveLen = m_nWaveLength;
	float fAmplitude = 1.0f;
	float fFrequency = 1.0f / m_nWaveLength;
	float fMaxValue = 0.0f;  // Used for normalizing result to 0.0 - 1.0
	for (int i = 0; i < m_nOctave; i++)
	{
		float fValue = Noise3DOnce(x * fFrequency, y * fFrequency, z * fFrequency);
		fTotal += Clamp(fValue) * fAmplitude;
		fMaxValue += fAmplitude;
		fAmplitude *= m_fPersistence;
		fFrequency *= 2;
	}

	return fTotal / fMaxValue * m_fAmplitude;
}