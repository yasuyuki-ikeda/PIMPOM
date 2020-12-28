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
//画像間演算　画素単位の計算
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"

// *************************************
//         マ  ク  ロ   定   義         
// *************************************
#define PIXWISE_SUM	(0)
#define PIXWISE_SUMABS	(1)
#define PIXWISE_MUL	(2)
#define PIXWISE_DIV	(3)

// *************************************
//         外    部    宣    言         
// *************************************

// *************************************
//         構  造  体   定   義         
// *************************************

// *************************************
//         静    的    変    数         
// *************************************

// *************************************
//         外    部    変    数         
// *************************************

// *************************************
//         関    数    参    照         
// *************************************


/********************************************************************
機  能  名  称 : 2画像の演算を行う
関    数    名 : Calc2DataUnit
引          数 : int			calc_type			(in)演算タイプ
                 int			src_imageA			(in)元画像Aのメモリ番号(-1にした場合は現在選択中の画像)
				 int			src_imageB			(in)元画像Bのメモリ番号(-1にした場合は現在選択中の画像)
				 int			dst_image			(in)結果画像のメモリ番号(-1にした場合は現在選択中の画像)
				 int			dst_type			(in)結果画像のフォーマット
				 double         weightA				(in)元画像Aにかける重み
				 double         weightA				(in)元画像Aにかける重み
				 double         bias				(in)バイアス
				 bool			satulate			(in)データ上下限で飽和させるかどうか(falseの場合はオーバーフローする)
				 
戻    り    値 : 成功すれば true
機          能 : 元画像Aと元画像Bに対して、以下のいづれかの演算をおこなう
					calc_type=0:  [結果画像] = [元画像A] * weightA + [元画像B] * weightB + bias
					calc_type=1:  [結果画像] = | [元画像A] * weightA + [元画像B] * weightB + bias |
					calc_type=2:  [結果画像] = [元画像A] * weightA * [元画像B] * weightB + bias
					calc_type=3:  [結果画像] = ([元画像A] * weightA) / ( [元画像B] * weightB ) + bias

                 画像A と 画像Bのサイズが同じでなければいけない
				 画像A と 画像BともRGBカラーの場合は、RGBそれぞれに対して演算をおこなう

日付         作成者          内容
------------ --------------- ---------------------------------------
              Y.Ikeda         新規作成
********************************************************************/

bool	CPimpomAPI::Calc2DataUnit(int calc_type, int src_imageA, int src_imageB, int dst_image, int dst_type, double weightA, double weightB, double bias, bool satulate )
{
	CDataUnit *pDU_A = GetDataUnit(src_imageA);
	CDataUnit *pDU_B = GetDataUnit(src_imageB);

	if (!pDU_A || !pDU_B)	return false;
	if (pDU_A->DataSize != pDU_B->DataSize)	return false;


	CDataUnit *pDstDU = FormatDataUnit(dst_image, dst_type, pDU_A->DataSize, 1, true);
	if (!pDstDU)	return false;

	bool rgb = false;
	if ((pDU_A->DataType == RGB_FORMAT || pDU_A->DataType == RGB_3D_FORMAT) && 
		(pDU_B->DataType == RGB_FORMAT || pDU_B->DataType == RGB_3D_FORMAT) && 
		(pDstDU->DataType == RGB_FORMAT || pDstDU->DataType == RGB_3D_FORMAT)
		)
	{//RGB 3チャネルを相手にする
		rgb = true;
	}


	for (int c = 0; c < pDstDU->ChannelNumber; c++)
	{
		for (int j = 0; j < pDstDU->DataSize.cy; j++)
		{
			for (int i = 0; i < pDstDU->DataSize.cx; i++)
			{
				double val0 = pDU_A->GetDataValue(CPoint(i, j), pDU_A->DispPage, rgb ? c : -1) * weightA;
				double val1 = pDU_B->GetDataValue(CPoint(i, j), pDU_B->DispPage, rgb ? c : -1) * weightB;
				double valD = bias;

				if (calc_type == PIXWISE_SUM)
				{
					valD += val0 + val1;
				}
				else if (calc_type == PIXWISE_SUMABS)
				{
					valD += val0 + val1;
					valD = fabs(valD);
				}
				else if (calc_type == PIXWISE_MUL)
				{
					valD += val0 * val1;
				}
				else if (val1 != 0){
					valD += val0 / val1;
				}

				pDstDU->SetDataValue(i, j, valD, 0, c, satulate);
			}
		}
	}

	return true;
}


/********************************************************************
機  能  名  称 : 2画像の線形演算を行う
関    数    名 : Linear2DataUnit
引          数 : 
				int			src_imageA			(in)元画像Aのメモリ番号(-1にした場合は現在選択中の画像)
				int			src_imageB			(in)元画像Bのメモリ番号(-1にした場合は現在選択中の画像)
				int			dst_image			(in)結果画像のメモリ番号(-1にした場合は現在選択中の画像)
				int			dst_type			(in)結果画像のフォーマット
				double      weightA				(in)元画像Aにかける重み
				double      weightB				(in)元画像Bにかける重み
				double      bias				(in)バイアス
				bool	    satulate			(in)データ上下限で飽和させるかどうか(falseの場合はオーバーフローする)

戻    り    値 : 成功すれば true
機          能 : 元画像Aと元画像Bに対して、以下の演算をおこなう
					[結果画像] = [元画像A] * weightA + [元画像B] * weightB + bias
				画像A と 画像Bのサイズが同じでなければいけない
				画像A と 画像BともRGBカラーの場合かつ両方ともRGB表示の場合は、RGBそれぞれに対して演算をおこなう

日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::Linear2DataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, double weightA, double weightB, double bias, bool satulate)
{
	return Calc2DataUnit(PIXWISE_SUM, src_imageA, src_imageB, dst_image, dst_type, weightA, weightB, bias, satulate);
}


/********************************************************************
機  能  名  称 : 2画像の加算を行う
関    数    名 : SumDataUnit
引          数 : int			src_imageA			(in)元画像Aのメモリ番号(-1にした場合は現在選択中の画像)
                 int			src_imageB			(in)元画像Bのメモリ番号(-1にした場合は現在選択中の画像)
				 int			dst_image			(in)結果画像のメモリ番号(-1にした場合は現在選択中の画像)
				 int			dst_type			(in)結果画像のフォーマット
				 bool			satulate			(in)データ上下限で飽和させるかどうか(falseの場合はオーバーフローする)
戻    り    値 : 成功すれば true
機          能 : 元画像A + 元画像B
                 画像A と 画像Bのサイズが同じでなければいけない
                 画像A と 画像BともRGBカラーの場合かつ両方ともRGB表示の場合は、RGBそれぞれに対して演算をおこなう
日付         作成者          内容
------------ --------------- ---------------------------------------
              Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SumDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate)
{
	return Calc2DataUnit(PIXWISE_SUM, src_imageA, src_imageB, dst_image, dst_type, 1, 1, 0 , satulate );
}



/********************************************************************
機  能  名  称 : 2画像の減算を行う
関    数    名 : SubtractDataUnit
引          数 : int			src_imageA			(in)元画像Aのメモリ番号(-1にした場合は現在選択中の画像)
                 int			src_imageB			(in)元画像Bのメモリ番号(-1にした場合は現在選択中の画像)
                 int			dst_image			(in)結果画像のメモリ番号(-1にした場合は現在選択中の画像)
                 int			dst_type			(in)結果画像のフォーマット
                 bool			satulate			(in)データ上下限で飽和させるかどうか(falseの場合はオーバーフローする)                     
戻    り    値 : 成功すれば true
機          能 : 元画像A - 元画像B
                 画像A と 画像Bのサイズが同じでなければいけない
                 画像A と 画像BともRGBカラーの場合かつ両方ともRGB表示の場合は、RGBそれぞれに対して演算をおこなう
日付         作成者          内容
------------ --------------- ---------------------------------------
              Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SubtractDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate)
{
	return Calc2DataUnit(PIXWISE_SUM, src_imageA, src_imageB, dst_image, dst_type, 1, -1, 0, satulate);
}


/********************************************************************
機  能  名  称 : 2画像の減算を行う
関    数    名 : SubAbsDataUnit
引          数 : int			src_imageA			(in)元画像Aのメモリ番号(-1にした場合は現在選択中の画像)
				int			src_imageB			(in)元画像Bのメモリ番号(-1にした場合は現在選択中の画像)
				int			dst_image			(in)結果画像のメモリ番号(-1にした場合は現在選択中の画像)
				int			dst_type			(in)結果画像のフォーマット
bool			satulate			(in)データ上下限で飽和させるかどうか(falseの場合はオーバーフローする)
戻    り    値 : 成功すれば true
機          能 :
				| 元画像A - 元画像B |
				画像A と 画像Bのサイズが同じでなければいけない
				画像A と 画像BともRGBカラーの場合かつ両方ともRGB表示の場合は、RGBそれぞれに対して演算をおこなう
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SubAbsDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate)
{
	return Calc2DataUnit(PIXWISE_SUMABS, src_imageA, src_imageB, dst_image, dst_type, 1, -1, 0, satulate);
}


/********************************************************************
機  能  名  称 : 2画像の積算を行う
関    数    名 : MultiplyDataUnit
引          数 : int			src_imageA			(in)元画像Aのメモリ番号(-1にした場合は現在選択中の画像)
				 int			src_imageB			(in)元画像Bのメモリ番号(-1にした場合は現在選択中の画像)
				 int			dst_image			(in)結果画像のメモリ番号(-1にした場合は現在選択中の画像)
				 int			dst_type			(in)結果画像のフォーマット
				 bool			satulate			(in)データ上下限で飽和させるかどうか(falseの場合はオーバーフローする)
戻    り    値 : 成功すれば true
機          能 : 元画像A * 元画像B
				画像A と 画像Bのサイズが同じでなければいけない
				画像A と 画像BともRGBカラーの場合かつ両方ともRGB表示の場合は、RGBそれぞれに対して演算をおこなう
日付         作成者          内容
------------ --------------- ---------------------------------------
			Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::MultiplyDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate)
{
	return Calc2DataUnit(PIXWISE_MUL, src_imageA, src_imageB, dst_image, dst_type, 1, 1, 0, satulate);
}


/********************************************************************
機  能  名  称 : 2画像の商算を行う
関    数    名 : DivideDataUnit
引          数 : int			src_imageA			(in)元画像Aのメモリ番号(-1にした場合は現在選択中の画像)
				 int			src_imageB			(in)元画像Bのメモリ番号(-1にした場合は現在選択中の画像)
				 int			dst_image			(in)結果画像のメモリ番号(-1にした場合は現在選択中の画像)
				 int			dst_type			(in)結果画像のフォーマット
				 bool			satulate			(in)データ上下限で飽和させるかどうか(falseの場合はオーバーフローする)

戻    り    値 : 成功すれば true
機          能 : 元画像A / 元画像B
				画像A と 画像Bのサイズが同じでなければいけない
				画像A と 画像BともRGBカラーの場合かつ両方ともRGB表示の場合は、RGBそれぞれに対して演算をおこなう
				画像Bの値が0のときは、結果の値も0になる

日付         作成者          内容
------------ --------------- ---------------------------------------
			Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::DivideDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate)
{
	return Calc2DataUnit(PIXWISE_DIV, src_imageA, src_imageB, dst_image, dst_type, 1, 1, 0, satulate);
}



/********************************************************************
機  能  名  称 : 1画像の線形演算を行う
関    数    名 : LinearDataUnit
引          数 : int			src_image			(in)元画像のメモリ番号(-1にした場合は現在選択中の画像)
                 int			dst_image			(in)結果画像のメモリ番号(-1にした場合は現在選択中の画像)
				 int			dst_type			(in)結果画像のフォーマット
                 double         weight				(in)元画像に対する重み
				 double			bias				(in)バイアス
				 bool			satulate			(in)データ上下限で飽和させるかどうか(falseの場合はオーバーフローする)

戻    り    値 : 成功すれば true
機          能 :
                [結果画像] = [元画像] * weight + bias
				元画像がRGBカラーの場合は、RGBそれぞれに対して演算をおこなう

日付         作成者          内容
------------ --------------- ---------------------------------------
			Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LinearDataUnit(int src_image, int dst_image, int dst_type, double weight, double bias, bool satulate)
{
	CDataUnit *pDU = GetDataUnit(src_image);

	if (!pDU )	return false;

	CDataUnit *pDstDU = FormatDataUnit(dst_image, dst_type, pDU->DataSize, 1, true);
	if (!pDstDU)	return false;

	bool rgb = false;
	if ((pDU->DataType == RGB_FORMAT || pDU->DataType == RGB_3D_FORMAT) &&
		(pDstDU->DataType == RGB_FORMAT || pDstDU->DataType == RGB_3D_FORMAT)
		)
	{//RGB 3チャネルを相手にする
		rgb = true;
	}


	for (int c = 0; c < pDstDU->ChannelNumber; c++)
	{
		for (int j = 0; j < pDU->DataSize.cy; j++)
		{
			for (int i = 0; i < pDU->DataSize.cx; i++)
			{
				double valS = pDU->GetDataValue(CPoint(i, j), pDU->DispPage, rgb ? c : -1);
				double valD = valS * weight + bias;

				pDstDU->SetDataValue(i, j, valD, 0, c, satulate);
			}
		}
	}

	return true;
}



/********************************************************************
機  能  名  称 : 画像の絶対値をとる
関    数    名 : AbsDataUnit
引          数 : int			src_image			(in)元画像のメモリ番号(-1にした場合は現在選択中の画像)
				 int			dst_image			(in)結果画像のメモリ番号(-1にした場合は現在選択中の画像)
戻    り    値 : 成功すれば true
機          能 : 
			[結果画像] = [元画像]の絶対値
			元画像がRGBカラーの場合かつRGB表示の場合は、RGBそれぞれに対して演算をおこなう

日付         作成者          内容
------------ --------------- ---------------------------------------
			Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::AbsDataUnit(int src_image, int dst_image)
{
	CDataUnit *pDU = GetDataUnit(src_image);

	if (!pDU)	return false;

	CDataUnit *pDstDU = FormatDataUnit(dst_image, pDU->DataType, pDU->DataSize, 1, true);
	if (!pDstDU)	return false;

	int channels = 1;
	if ((pDU->DataType == RGB_FORMAT || pDU->DataType == RGB_3D_FORMAT) && pDU->DispRGB)
	{//RGB 3チャネルを相手にする
		channels = 3;
	}


	for (int c = 0; c < channels; c++)
	{
		for (int j = 0; j < pDU->DataSize.cy; j++)
		{
			for (int i = 0; i < pDU->DataSize.cx; i++)
			{
				double valS = pDU->GetDataValue(CPoint(i, j), pDU->DispPage, c);
				double valD = fabs(valS);

				pDstDU->SetDataValue(i, j, valD, 0, c, true);
			}
		}
	}

	return true;
}
