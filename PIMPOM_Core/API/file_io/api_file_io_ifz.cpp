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
//ファイルIO(ifz)
/*******************************************************************/

#include "stdafx.h"
#include "PIMPOM_API.h"
#include "atlimage.h"

// *************************************
//         マ  ク  ロ   定   義         
// *************************************

//ifzのﾌｫｰﾏｯﾄ
#define FORMAT_RAW8_RGGB_SC	0				// Raw 8bit(RG/GB) 周囲1画素余白あり  
#define FORMAT_RAW8_GRGB_SC	1				// Raw 8bit(GR/BG) 周囲1画素余白あり  
#define FORMAT_RAW8_BGGR_SC	2				// Raw 8bit(BG/GR) 周囲1画素余白あり  
#define FORMAT_RAW8_GBRG_SC	3				// Raw 8bit(GB/RG) 周囲1画素余白あり  
#define FORMAT_MONO8_FRAME	10				// Mono 8bit フレーム取込  
#define FORMAT_MONO8_FIELD	11				// Mono 8bit フィールド取込  
#define FORMAT_MONO8		100				// Mono 8bit  
#define FORMAT_RAW8_RGGB	101				// Raw 8bit(RG/GB)  
#define FORMAT_RAW8_GRBG	102				// Raw 8bit(GR/BG)  
#define FORMAT_RAW8_BGGR	103				// Raw 8bit(BG/GR)  
#define FORMAT_RAW8_GBRG	104				// Raw 8bit(GB/RG)


// ベイヤー配列定義
#define	ARRAY_RGGB	0
#define	ARRAY_GRBG	1
#define	ARRAY_BGGR	2
#define	ARRAY_GBRG	3

// *************************************
//         外    部    宣    言         
// *************************************

// *************************************
//         構  造  体   定   義         
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
//         静    的    変    数         
// *************************************

// ベイヤー配列変換定義表
const int BayerType[4][2][2] = {
	{{ARRAY_RGGB, ARRAY_GRBG}, {ARRAY_GBRG, ARRAY_BGGR}},		// RGGB
	{{ARRAY_GRBG, ARRAY_RGGB}, {ARRAY_BGGR, ARRAY_GBRG}},		// GRBG
	{{ARRAY_BGGR, ARRAY_GBRG}, {ARRAY_GRBG, ARRAY_RGGB}},		// BGGR
	{{ARRAY_GBRG, ARRAY_BGGR}, {ARRAY_RGGB, ARRAY_GRBG}},		// GBRG
};

// *************************************
//         外    部    変    数         
// *************************************

// *************************************
//         関    数    参    照         
// *************************************

// *********************************************************************  
/*! < pre>
 * 機  能  名  称 : ベイヤーカラー画像変換
 * 関    数    名 : int transrgb_C()
 * 引          数 : const Uchar *src;           入力画像格納領域
 *                  const int   sizeX;          X方向画像サイズ
 *                  const int   sizeY;          Y方向画像サイズ
 *                  const int   x1, y1;         領域左上座標
 *                  const int   x2, y2;         領域右下座標
 *                  const int   arrayType       ベイヤー配列タイプ
 *                                               0:R
 *                                               1:GR
 *                                               2:B
 *                                               3:GB
 *                  Uchar       *dest;          出力画像格納領域
 *
 * 戻    り    値 : エラーコード
 *                    NORMAL(0):正常終了
 * 機          能 : 
 *
 * 日付         作成者          内容
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

	// 変換を行う

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
 * 機  能  名  称 : ベイヤーカラー画像変換2
 * 関    数    名 : int transrgb2()
 * 引          数 : const Uchar *src;           入力画像格納領域
 *                  Uchar       *dest;          出力画像格納領域
 *                  const int   inputSizeX;     X方向入力画像サイズ
 *                  const int   inputSizeY;     Y方向入力画像サイズ
 *                  const int   outputSizeX;    X方向出力画像サイズ
 *                  const int   outputSizeY;    Y方向出力画像サイズ
 *                  const int   x1, y1;         領域左上座標
 *                  const int   x2, y2;         領域右下座標
 *                  const int   arrayType       ベイヤー配列タイプ
 *                                               0:R
 *                                               1:GR
 *                                               2:B
 *                                               3:GB
 *
 * 戻    り    値 : エラーコード
 *                    NORMAL(0):正常終了
 * 機          能 : 
 *
 * 日付         作成者          内容
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

	// 変換を行う  

	i = 0; j = 0;

	int ix = 0;		// 入力X座標  
	int iy = 0;		// 入力Y座標  

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
機  能  名  称 :  ifzファイルを読み込む
関    数    名 : LoadIFZ
引          数 : long		image_number(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		pathname	(in)ファイルのフルパス
戻    り    値 : 成功すれば true
機          能 : 8bitﾓﾉｸﾛ、24bitｶﾗｰに対応
				2カメラ以上には未対応
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::LoadIFZ(long image_number, CString pathname)
{
	CFile				file;
	IFZ_FILEHEADER		header1;
	int cam_num=0;

	if( ! file.Open(pathname , CFile::modeRead | CFile::typeBinary ) )	return false; //ファイルを開く	 

	//ヘッダ読み込み
	if( file.Read( &header1, sizeof(IFZ_FILEHEADER) ) != sizeof(IFZ_FILEHEADER) ){
		file.Close();
		return false;
	}

	//ヘッダ確認
	if(header1.id != -2 || header1.cam_connect==0){
		file.Close();
		return false;
	}

	//画像枚数確認
	cam_num=0;
	if(header1.cam_connect & 1)	cam_num++;
	if(header1.cam_connect & 2)	cam_num++;
	if(header1.cam_connect & 4)	cam_num++;
	if(header1.cam_connect & 8)	cam_num++;

	//画像読み出し
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
			header2.format == FORMAT_RAW8_GBRG )//ベイヤー
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
		else if(header2.format == FORMAT_MONO8_FRAME || header2.format == FORMAT_MONO8_FIELD || header2.format == FORMAT_MONO8)//モノクロ
		{
			BYTE *p_dst = GetByteMemory( image_number + n, CSize(header2.sizeX, header2.sizeY), true);

			for(int y=header2.grabY ; y<header2.grabH ; y++){
				file.Read( p_dst + header2.grabX, header2.grabW );
				p_dst += header2.sizeX;
			}
			DrawImage(image_number + n,true,true);
		}
	}

	//読み込んだファイルのパスを残す
	CDataUnit *p_du = GetDataUnit(image_number);
	p_du->DataName = ::PathFindFileNameA(pathname);
	p_du->SrcFilePath = pathname;

	file.Close();

	SetFileIOLog(pathname);//ファイル名のログを残す
	
	return	true;
}


bool CPimpomAPI::LoadBFZ(long image_number, CString pathname)
{
	
	CString files[100];
	int cam_num = GetFileList(pathname,_T("bmp"),100, files);


	//画像読み出し
	for (int n = 0; n<cam_num && image_number + n < data_number; n++)
	{
		CString filepathname = pathname + CString("\\") + files[n];
		LoadBitmap(image_number + n, filepathname);
		DrawImage(image_number + n,true,true);

		//読み込んだファイルのパスを残す
		CDataUnit *p_du = GetDataUnit(image_number + n);
		p_du->DataName = ::PathFindFileNameA(filepathname);
		p_du->SrcFilePath = filepathname;
	}

	SetFileIOLog(pathname);//ファイル名のログを残す

	return	true;
}
