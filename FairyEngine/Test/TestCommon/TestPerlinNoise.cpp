/*
 * ------------------------------------------------------------------------
 *  Name:   TestPerlinNoise.cpp
 *  Desc:   Test code for perlin noise.
 *  Author: Yish
 *  Date:   2018/9/20
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Shi. All rights Observed.
 * ------------------------------------------------------------------------
 */

#include "FTestCase.h"
#include <FPerlinNoise.h>

struct BMPFileHeader
{
	unsigned short  bfType;
	unsigned int  bfSize;
	unsigned short  bfReserved1;
	unsigned short  bfReserved2;
	unsigned int  bfOffBits;
};

struct BMPInfoHeader
{
	unsigned int  biSize;
	int     biWidth;
	int     biHeight;
	unsigned short  biPlanes;
	unsigned short  biBitCount;
	unsigned int  biCompression;
	unsigned int  biSizeImage;
	int     biXPelsPerMeter;
	int     biYPelsPerMeter;
	unsigned int  biClrUsed;
	unsigned int  biClrImportant;
};

static void SaveSurfaceToFile(int width, int height, unsigned int* pBuf, const char* filename)
{
	LONG bmofs;
	BITMAPFILEHEADER fileHead;
	BITMAPINFOHEADER infoHead;

	FILE* pFile = fopen(filename, "wb");
	if (!pFile) return;

	// 先写入文件头占位，等写完像素再来覆盖
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, pFile);

	// 位图信息
	infoHead.biSize = sizeof(BITMAPINFOHEADER);
	infoHead.biWidth = width;
	infoHead.biHeight = height;
	infoHead.biPlanes = 1;
	infoHead.biBitCount = 32;
	infoHead.biCompression = BI_RGB;
	infoHead.biSizeImage = width * height;
	infoHead.biXPelsPerMeter = 0;
	infoHead.biYPelsPerMeter = 0;
	infoHead.biClrUsed = 0;
	infoHead.biClrImportant = 0;
	fwrite(&infoHead, sizeof(BITMAPINFOHEADER), 1, pFile);

	// 保存像素区的偏移
	bmofs = ftell(pFile);

	// 写入像素数据
	DWORD* buf = (DWORD*)pBuf;
	for (int i = (int)height - 1; i >= 0; --i)
	{
		DWORD* pLine = &buf[width*i];
		fwrite(pLine, sizeof(DWORD), width, pFile);
	}

	fileHead.bfType = 0x4d42;
	fileHead.bfSize = ftell(pFile);
	fileHead.bfOffBits = bmofs;
	fseek(pFile, 0L, SEEK_SET);
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, pFile);
	fclose(pFile);
}

F_TESTCASE(PerlinNoise)
{
	FPerlinNoise noise(true, 8866983);
	noise.SetOctaveParam(4, 512, 1.0f, 2.0f);
	FILE* pFile = fopen("1.txt", "w");
	for (int i = 0; i <= 5000; i++)
	{
		fprintf(pFile, "%f\n", noise.Noise1D(i));
	}
	fclose(pFile);

	int width = 512;
	int height = 512;
	pFile = fopen("2.txt", "w");
	unsigned int* pBuf = new unsigned int[width * height];
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			unsigned int& pixel = pBuf[i*width + j];
			float x = (float)i;// / 32;
			float y = (float)j;// / 32;
			float val = noise.Noise2D(x, y);
			int valn = (int)(val * 255);
			fprintf(pFile, "%f\n", val);
			pixel = 0xff000000 | (valn << 16) | (valn << 8) | valn;
		}
	}
	fclose(pFile);
	SaveSurfaceToFile(width, height, pBuf, "1.bmp");
	delete[] pBuf;
}