/*******************************************************************
                           License Agreement
            Ploaform for IMage Procession Obserber and Modifier

Copyright (c) 2020, Yasuyuki Ikeda All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.
* The name of the copyright holders may not be used to endorse or promote products
  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************/
//CPimpomAPI
//�t�@�C��IO(ifz)
/*******************************************************************/

#include "stdafx.h"
#include "PIMPOM_API.h"
#include "atlimage.h"

// *************************************
//         �}  �N  ��   ��   �`         
// *************************************

//ifz��̫�ϯ�
#define FORMAT_RAW8_RGGB_SC	0				// Raw 8bit(RG/GB) ����1��f�]������  
#define FORMAT_RAW8_GRGB_SC	1				// Raw 8bit(GR/BG) ����1��f�]������  
#define FORMAT_RAW8_BGGR_SC	2				// Raw 8bit(BG/GR) ����1��f�]������  
#define FORMAT_RAW8_GBRG_SC	3				// Raw 8bit(GB/RG) ����1��f�]������  
#define FORMAT_MONO8_FRAME	10				// Mono 8bit �t���[���捞  
#define FORMAT_MONO8_FIELD	11				// Mono 8bit �t�B�[���h�捞  
#define FORMAT_MONO8		100				// Mono 8bit  
#define FORMAT_RAW8_RGGB	101				// Raw 8bit(RG/GB)  
#define FORMAT_RAW8_GRBG	102				// Raw 8bit(GR/BG)  
#define FORMAT_RAW8_BGGR	103				// Raw 8bit(BG/GR)  
#define FORMAT_RAW8_GBRG	104				// Raw 8bit(GB/RG)


// �x�C���[�z���`
#define	ARRAY_RGGB	0
#define	ARRAY_GRBG	1
#define	ARRAY_BGGR	2
#define	ARRAY_GBRG	3

// *************************************
//         �O    ��    ��    ��         
// *************************************

// *************************************
//         �\  ��  ��   ��   �`         
// *************************************

struct IFZ_FILEHEADER {
	long	id;
	long	cam_connect;
};

struct IFZ_HEADER {
	long	format;
	long	sizeX;
	long	sizeY;
	long	grabX;
	long	grabY;
	long	grabW;
	long	grabH;
	long	size;
};

// *************************************
//         ��    �I    ��    ��         
// *************************************

// �x�C���[�z��ϊ���`�\
const int BayerType[4][2][2] = {
	{{ARRAY_RGGB, ARRAY_GRBG}, {ARRAY_GBRG, ARRAY_BGGR}},		// RGGB
	{{ARRAY_GRBG, ARRAY_RGGB}, {ARRAY_BGGR, ARRAY_GBRG}},		// GRBG
	{{ARRAY_BGGR, ARRAY_GBRG}, {ARRAY_GRBG, ARRAY_RGGB}},		// BGGR
	{{ARRAY_GBRG, ARRAY_BGGR}, {ARRAY_RGGB, ARRAY_GRBG}},		// GBRG
};

// *************************************
//         �O    ��    ��    ��         
// *************************************

// *************************************
//         ��    ��    �Q    ��         
// *************************************

// *********************************************************************  
/*! < pre>
 * �@  �\  ��  �� : �x�C���[�J���[�摜�ϊ�
 * ��    ��    �� : int transrgb_C()
 * ��          �� : const Uchar *src;           ���͉摜�i�[�̈�
 *                  const int   sizeX;          X�����摜�T�C�Y
 *                  const int   sizeY;          Y�����摜�T�C�Y
 *                  const int   x1, y1;         �̈捶����W
 *                  const int   x2, y2;         �̈�E�����W
 *                  const int   arrayType       �x�C���[�z��^�C�v
 *                                               0:R
 *                                               1:GR
 *                                               2:B
 *                                               3:GB
 *                  Uchar       *dest;          �o�͉摜�i�[�̈�
 *
 * ��    ��    �l : �G���[�R�[�h
 *                    NORMAL(0):����I��
 * �@          �\ : 
 *
 * ���t         �쐬��          ���e
 * ------------ --------------- --------------------------------------- 

 * < /pre> */
// *********************************************************************  
static void transrgb_C(const BYTE *src, const int sizeX, const int sizeY, const int x1,
			   const int y1, const int x2, const int y2, const int arrayType, BYTE *dest)
{
	int			type = BayerType[arrayType][0][0];
	int			x, y;
	int			i, j;
	BYTE		*destR = &dest[0];
	BYTE		*destG = &dest[sizeX * sizeY];
	BYTE		*destB = &dest[sizeX * sizeY * 2];
	int			sizeX2 = sizeX + 2;

	// �ϊ����s��

	i = 0; j = 0;

	for (y = y1; y <= y2; y++) {
		j = 0;
		for (x = x1; x <= x2; x++) {
			switch (BayerType[arrayType][i][j]) {
			case ARRAY_RGGB:
				destR[sizeX * y + x] = (src[sizeX2 * y + x] + src[sizeX2 * y + x + 2] + src[sizeX2 * (y + 2) + x] + src[sizeX2 * (y + 2) + x + 2]) / 4;
				destG[sizeX * y + x] = (src[sizeX2 * y + x + 1] + src[sizeX2 * (y + 1) + x] + src[sizeX2 * (y + 1) + x + 2] + src[sizeX2 * (y + 2) + x + 1]) / 4;
				destB[sizeX * y + x] = (src[sizeX2 * (y + 1) + x + 1]);
				break;
			case ARRAY_GRBG:
				destR[sizeX * y + x] = (src[sizeX2 * y + x + 1] + src[sizeX2 * (y + 2) + x + 1]) / 2;
				destG[sizeX * y + x] = (src[sizeX2 * y + x] + src[sizeX2 * y + x + 2] + src[sizeX2 * (y + 2) + x] + src[sizeX2 * (y + 2) + x + 2] + src[sizeX2 * (y + 1) + x + 1] * 4) / 8;
				destB[sizeX * y + x] = (src[sizeX2 * (y + 1) + x] + src[sizeX2 * (y + 1) + x + 2]) / 2;
				break;
			case ARRAY_BGGR:
				destR[sizeX * y + x] = (src[sizeX2 * (y + 1) + x + 1]);
				destG[sizeX * y + x] = (src[sizeX2 * y + x + 1] + src[sizeX2 * (y + 1) + x] + src[sizeX2 * (y + 1) + x + 2] + src[sizeX2 * (y + 2) + x + 1]) / 4;
				destB[sizeX * y + x] = (src[sizeX2 * y + x] + src[sizeX2 * y + x + 2] + src[sizeX2 * (y + 2) + x] + src[sizeX2 * (y + 2) + x + 2]) / 4;
				break;
			case ARRAY_GBRG:
				destR[sizeX * y + x] = (src[sizeX2 * (y + 1) + x] + src[sizeX2 * (y + 1) + x + 2]) / 2;
				destG[sizeX * y + x] = (src[sizeX2 * y + x] + src[sizeX2 * y + x + 2] + src[sizeX2 * (y + 2) + x] + src[sizeX2 * (y + 2) + x + 2] + src[sizeX2 * (y + 1) + x + 1] * 4) / 8;
				destB[sizeX * y + x] = (src[sizeX2 * y + x + 1] + src[sizeX2 * (y + 2) + x + 1]) / 2;
				break;
			}
			destR[sizeX * y + x] = (destR[sizeX * y + x] < 1) ? 1 : destR[sizeX * y + x];
			destG[sizeX * y + x] = (destG[sizeX * y + x] < 1) ? 1 : destG[sizeX * y + x];
			destB[sizeX * y + x] = (destB[sizeX * y + x] < 1) ? 1 : destB[sizeX * y + x];
			j = 1 - j;
		}
		i = 1 - i;
	}
}

// *********************************************************************  
/*! < pre>
 * �@  �\  ��  �� : �x�C���[�J���[�摜�ϊ�2
 * ��    ��    �� : int transrgb2()
 * ��          �� : const Uchar *src;           ���͉摜�i�[�̈�
 *                  Uchar       *dest;          �o�͉摜�i�[�̈�
 *                  const int   inputSizeX;     X�������͉摜�T�C�Y
 *                  const int   inputSizeY;     Y�������͉摜�T�C�Y
 *                  const int   outputSizeX;    X�����o�͉摜�T�C�Y
 *                  const int   outputSizeY;    Y�����o�͉摜�T�C�Y
 *                  const int   x1, y1;         �̈捶����W
 *                  const int   x2, y2;         �̈�E�����W
 *                  const int   arrayType       �x�C���[�z��^�C�v
 *                                               0:R
 *                                               1:GR
 *                                               2:B
 *                                               3:GB
 *
 * ��    ��    �l : �G���[�R�[�h
 *                    NORMAL(0):����I��
 * �@          �\ : 
 *
 * ���t         �쐬��          ���e
 * ------------ --------------- --------------------------------------- 

 * < /pre> */
// *********************************************************************  

static int transrgb2(const BYTE *src, BYTE *dest, const int inputSizeX, const int inputSizeY,
				const int outputSizeX, const int outputSizeY,
				const int x1, const int y1, const int x2, const int y2, const int arrayType)
{
	int			type = BayerType[arrayType][0][0];
	int			x, y;
	int			i, j;
	BYTE		*destR = &dest[0];
	BYTE		*destG = &dest[outputSizeX * outputSizeY];
	BYTE		*destB = &dest[outputSizeX * outputSizeY * 2];

	// �ϊ����s��  

	i = 0; j = 0;

	int ix = 0;		// ����X���W  
	int iy = 0;		// ����Y���W  

	for (y = y1; y <= y2; y++) {
		j = 0;
		ix = 0;
		for (x = x1; x <= x2; x++) {
			switch (BayerType[arrayType][i][j]) {
			case ARRAY_RGGB:
				if (y == y1 && x == x1) {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
					destG[outputSizeX * y + x] = (src[inputSizeX * iy + ix + 1] + src[inputSizeX * (iy + 1) + ix]) / 2;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix + 1]);
				} else if (y == y1 && x == x2) {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
					destG[outputSizeX * y + x] = (src[inputSizeX * iy + (ix - 1)] + src[inputSizeX * (iy + 1) + ix]) / 2;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix - 1]);
				} else if (y == y2 && x == x1) {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * iy + ix + 1]) / 2;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix + 1]);
				} else if (y == y2 && x == x2) {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * iy + (ix - 1)]) / 2;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix - 1]);
				} else if (y == y1) {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
					destG[outputSizeX * y + x] = (src[inputSizeX * iy + (ix - 1)] + src[inputSizeX * iy + ix + 1] + src[inputSizeX * (iy + 1) + ix]) / 3;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix - 1] + src[inputSizeX * (iy + 1) + ix + 1]) / 2;
				} else if (y == y2) {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * iy + (ix - 1)] + src[inputSizeX * iy + ix + 1]) / 3;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix - 1] + src[inputSizeX * (iy - 1) + ix + 1]) / 2;
				} else if (x == x1) {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * iy + ix + 1] + src[inputSizeX * (iy + 1) + ix]) / 3;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix + 1] + src[inputSizeX * (iy + 1) + ix + 1]) / 2;
				} else if (x == x2) {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * iy + (ix - 1)] + src[inputSizeX * (iy + 1) + ix]) / 3;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix - 1] + src[inputSizeX * (iy + 1) + ix - 1]) / 2;
				} else {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * iy + (ix - 1)] + src[inputSizeX * iy + ix + 1] + src[inputSizeX * (iy + 1) + ix]) / 4;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix - 1] + src[inputSizeX * (iy - 1) + ix + 1] + src[inputSizeX * (iy + 1) + ix - 1] + src[inputSizeX * (iy + 1) + ix + 1]) / 4;
				}
				break;
			case ARRAY_GRBG:
				if (y == y1 && x == x1) {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix + 1]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix + 1] + src[inputSizeX * iy + ix] * 2) / 3;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix]);
				} else if (y == y1 && x == x2) {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix - 1]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix - 1] + src[inputSizeX * iy + ix] * 2) / 3;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix]);
				} else if (y == y2 && x == x1) {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix + 1]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix + 1] + src[inputSizeX * iy + ix] * 2) / 3;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix]);
				} else if (y == y2 && x == x2) {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix - 1]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix - 1] + src[inputSizeX * iy + ix] * 2) / 3;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix]);
				} else if (y == y1) {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix - 1] + src[inputSizeX * iy + ix + 1]) / 2;
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix - 1] + src[inputSizeX * (iy + 1) + ix + 1] + src[inputSizeX * iy + ix] * 2) / 4;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix]);
				} else if (y == y2) {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix - 1] + src[inputSizeX * iy + ix + 1]) / 2;
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix - 1] + src[inputSizeX * (iy - 1) + ix + 1] + src[inputSizeX * iy + ix] * 2) / 4;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix]);
				} else if (x == x1) {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix + 1]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix + 1] + src[inputSizeX * (iy + 1) + ix + 1] + src[inputSizeX * iy + ix] * 2) / 4;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * (iy + 1) + ix]) / 2;
				} else if (x == x2) {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix - 1]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix - 1] + src[inputSizeX * (iy + 1) + ix - 1] + src[inputSizeX * iy + ix] * 2) / 4;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * (iy + 1) + ix]) / 2;
				} else {
					destR[outputSizeX * y + x] = (src[inputSizeX * iy + ix - 1] + src[inputSizeX * iy + ix + 1]) / 2;
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix - 1] + src[inputSizeX * (iy - 1) + ix + 1] + src[inputSizeX * (iy + 1) + ix - 1] + src[inputSizeX * (iy + 1) + ix + 1] + src[inputSizeX * iy + ix] * 4) / 8;
					destB[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * (iy + 1) + ix]) / 2;
				}
				break;
			case ARRAY_BGGR:
				if (y == y1 && x == x1) {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix + 1]);
					destG[outputSizeX * y + x] = (src[inputSizeX * iy + ix + 1] + src[inputSizeX * (iy + 1) + ix]) / 2;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
				} else if (y == y1 && x == x2) {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix - 1]);
					destG[outputSizeX * y + x] = (src[inputSizeX * iy + (ix - 1)] + src[inputSizeX * (iy + 1) + ix]) / 2;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
				} else if (y == y2 && x == x1) {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix + 1]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * iy + ix + 1]) / 2;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
				} else if (y == y2 && x == x2) {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix - 1]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * iy + (ix - 1)]) / 2;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
				} else if (y == y1) {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix - 1] + src[inputSizeX * (iy + 1) + ix + 1]) / 2;
					destG[outputSizeX * y + x] = (src[inputSizeX * iy + (ix - 1)] + src[inputSizeX * iy + ix + 1] + src[inputSizeX * (iy + 1) + ix]) / 3;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
				} else if (y == y2) {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix - 1] + src[inputSizeX * (iy - 1) + ix + 1]) / 2;
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * iy + (ix - 1)] + src[inputSizeX * iy + ix + 1]) / 3;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
				} else if (x == x1) {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix + 1] + src[inputSizeX * (iy + 1) + ix + 1]) / 2;
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * iy + ix + 1] + src[inputSizeX * (iy + 1) + ix]) / 3;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
				} else if (x == x2) {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix - 1] + src[inputSizeX * (iy + 1) + ix - 1]) / 2;
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * iy + (ix - 1)] + src[inputSizeX * (iy + 1) + ix]) / 3;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
				} else {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix - 1] + src[inputSizeX * (iy - 1) + ix + 1] + src[inputSizeX * (iy + 1) + ix - 1] + src[inputSizeX * (iy + 1) + ix + 1]) / 4;
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * iy + (ix - 1)] + src[inputSizeX * iy + ix + 1] + src[inputSizeX * (iy + 1) + ix]) / 4;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix]);
				}
				break;
			case ARRAY_GBRG:
				if (y == y1 && x == x1) {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix + 1] + src[inputSizeX * iy + ix] * 2) / 3;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix + 1]);
				} else if (y == y1 && x == x2) {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix - 1] + src[inputSizeX * iy + ix] * 2) / 3;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix - 1]);
				} else if (y == y2 && x == x1) {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix + 1] + src[inputSizeX * iy + ix] * 2) / 3;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix + 1]);
				} else if (y == y2 && x == x2) {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix - 1] + src[inputSizeX * iy + ix] * 2) / 3;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix - 1]);
				} else if (y == y1) {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy + 1) + ix - 1] + src[inputSizeX * (iy + 1) + ix + 1] + src[inputSizeX * iy + ix] * 2) / 4;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix - 1] + src[inputSizeX * iy + ix + 1]) / 2;
				} else if (y == y2) {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix]);
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix - 1] + src[inputSizeX * (iy - 1) + ix + 1] + src[inputSizeX * iy + ix] * 2) / 4;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix - 1] + src[inputSizeX * iy + ix + 1]) / 2;
				} else if (x == x1) {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * (iy + 1) + ix]) / 2;
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix + 1] + src[inputSizeX * (iy + 1) + ix + 1] + src[inputSizeX * iy + ix] * 2) / 4;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix + 1]);
				} else if (x == x2) {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * (iy + 1) + ix]) / 2;
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix - 1] + src[inputSizeX * (iy + 1) + ix - 1] + src[inputSizeX * iy + ix] * 2) / 4;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix - 1]);
				} else {
					destR[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix] + src[inputSizeX * (iy + 1) + ix]) / 2;
					destG[outputSizeX * y + x] = (src[inputSizeX * (iy - 1) + ix - 1] + src[inputSizeX * (iy - 1) + ix + 1] + src[inputSizeX * (iy + 1) + ix - 1] + src[inputSizeX * (iy + 1) + ix + 1] + src[inputSizeX * iy + ix] * 4) / 8;
					destB[outputSizeX * y + x] = (src[inputSizeX * iy + ix - 1] + src[inputSizeX * iy + ix + 1]) / 2;
				}
				break;
			}
			destR[outputSizeX * y + x] = (destR[outputSizeX * y + x] < 1) ? 1 : destR[outputSizeX * y + x];
			destG[outputSizeX * y + x] = (destG[outputSizeX * y + x] < 1) ? 1 : destG[outputSizeX * y + x];
			destB[outputSizeX * y + x] = (destB[outputSizeX * y + x] < 1) ? 1 : destB[outputSizeX * y + x];
			j = 1 - j;
			ix++;
		}
		i = 1 - i;
		iy++;
	}

	return 0;
}

/********************************************************************
�@  �\  ��  �� :  ifz�t�@�C����ǂݍ���
��    ��    �� : LoadIFZ
��          �� : long		image_number(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		pathname	(in)�t�@�C���̃t���p�X
��    ��    �l : ��������� true
�@          �\ : 8bit�ɸہA24bit�װ�ɑΉ�
				2�J�����ȏ�ɂ͖��Ή�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::LoadIFZ(long image_number, CString pathname)
{
	CFile				file;
	IFZ_FILEHEADER		header1;
	int cam_num=0;

	if( ! file.Open(pathname , CFile::modeRead | CFile::typeBinary ) )	return false; //�t�@�C�����J��	 

	//�w�b�_�ǂݍ���
	if( file.Read( &header1, sizeof(IFZ_FILEHEADER) ) != sizeof(IFZ_FILEHEADER) ){
		file.Close();
		return false;
	}

	//�w�b�_�m�F
	if(header1.id != -2 || header1.cam_connect==0){
		file.Close();
		return false;
	}

	//�摜�����m�F
	cam_num=0;
	if(header1.cam_connect & 1)	cam_num++;
	if(header1.cam_connect & 2)	cam_num++;
	if(header1.cam_connect & 4)	cam_num++;
	if(header1.cam_connect & 8)	cam_num++;

	//�摜�ǂݏo��
	for(int n=0 ; n<cam_num && image_number+n < data_number ; n++)
	{
		IFZ_HEADER		header2;
		if( file.Read( &header2, sizeof(IFZ_HEADER) ) != sizeof(IFZ_HEADER) ){
			file.Close();
			return false;
		}

		if( header2.format == FORMAT_RAW8_RGGB_SC || 
			header2.format == FORMAT_RAW8_GRGB_SC || 
			header2.format == FORMAT_RAW8_BGGR_SC || 
			header2.format == FORMAT_RAW8_GBRG_SC ||
			header2.format == FORMAT_RAW8_RGGB || 
			header2.format == FORMAT_RAW8_GRBG || 
			header2.format == FORMAT_RAW8_BGGR || 
			header2.format == FORMAT_RAW8_GBRG )//�x�C���[
		{
			int bayerW = header2.sizeX + 2;
			int	bayerH = header2.sizeY + 2;

			BYTE *p_buffer = new BYTE[ bayerW * bayerH ];
			if(!p_buffer)	return false;

			BYTE *p_dst = GetRGBMemory( image_number + n, CSize(header2.sizeX, header2.sizeY), true);
			if(!p_dst)	return false;

			file.Read( p_buffer, bayerW * bayerH );

			int array_type=0;
			switch(header2.format){
				case FORMAT_RAW8_RGGB_SC:
				case FORMAT_RAW8_GRGB_SC:
				case FORMAT_RAW8_BGGR_SC:
				case FORMAT_RAW8_GBRG_SC:
					array_type = header2.format - FORMAT_RAW8_RGGB_SC;
					transrgb_C(p_buffer, header2.sizeX, header2.sizeY, header2.grabX, header2.grabY, header2.grabW-1 , header2.grabH-1, array_type, p_dst);
					break;
				case FORMAT_RAW8_RGGB:
				case FORMAT_RAW8_GRBG:
				case FORMAT_RAW8_BGGR:
				case FORMAT_RAW8_GBRG:
					array_type = header2.format - FORMAT_RAW8_RGGB;
					transrgb2(p_buffer,p_dst, header2.grabW, header2.grabH,header2.sizeX, header2.sizeY , header2.grabX, header2.grabY, header2.grabX+header2.grabW-1 , header2.grabY+header2.grabH-1,  array_type);
					break;
			}


			DrawImage(image_number + n);
			delete[]	p_buffer;

		}
		else if(header2.format == FORMAT_MONO8_FRAME || header2.format == FORMAT_MONO8_FIELD || header2.format == FORMAT_MONO8)//���m�N��
		{
			BYTE *p_dst = GetByteMemory( image_number + n, CSize(header2.sizeX, header2.sizeY), true);

			for(int y=header2.grabY ; y<header2.grabH ; y++){
				file.Read( p_dst + header2.grabX, header2.grabW );
				p_dst += header2.sizeX;
			}
			DrawImage(image_number + n,true,true);
		}
	}

	//�ǂݍ��񂾃t�@�C���̃p�X���c��
	CDataUnit *p_du = GetDataUnit(image_number);
	p_du->DataName = ::PathFindFileNameA(pathname);
	p_du->SrcFilePath = pathname;

	file.Close();

	SetFileIOLog(pathname);//�t�@�C�����̃��O���c��
	
	return	true;
}


bool CPimpomAPI::LoadBFZ(long image_number, CString pathname)
{
	
	CString files[100];
	int cam_num = GetFileList(pathname,_T("bmp"),100, files);


	//�摜�ǂݏo��
	for (int n = 0; n<cam_num && image_number + n < data_number; n++)
	{
		CString filepathname = pathname + CString("\\") + files[n];
		LoadBitmap(image_number + n, filepathname);
		DrawImage(image_number + n,true,true);

		//�ǂݍ��񂾃t�@�C���̃p�X���c��
		CDataUnit *p_du = GetDataUnit(image_number + n);
		p_du->DataName = ::PathFindFileNameA(filepathname);
		p_du->SrcFilePath = filepathname;
	}

	SetFileIOLog(pathname);//�t�@�C�����̃��O���c��

	return	true;
}
