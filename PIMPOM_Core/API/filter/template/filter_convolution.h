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
//コンボリューション
/*******************************************************************/
#pragma once

//**************************************
//         インクルードファイル         
//**************************************
#include "global_define.h"

// *************************************
//         マ  ク  ロ   定   義         
// *************************************

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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// テンプレートで全ての型の画像に対応できるようにする
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/********************************************************************
機  能  名  称 : コンボリューション
関    数    名 : filter_convolution
引          数 :
戻    り    値 : 成功すれば true
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
template <class DATA, class DATA2>
bool	filter_convolution(
	DATA			*p_src_data,	//(in)入力画像配列の先頭ポインタ
	DATA2			*p_dst_data,	//(out)出力画像配列の先頭ポインタ
	SIZE			image_size,		//(in)画像サイズ
	SIZE			kernel_size	,	//(in)フィルタサイズ(奇数で設定すること)
	float			*pKernel,		//(in)カーネル(kernel_size.cx * kernel_size.cyの配列)の先頭ポインタ
	RECT			area,			//(in)処理領域
	float			offset,			//(in)値のオフセット
	float			contrast,		//(in)コントラスト
	float			data_max,		//(in)データ上限値
	float			data_min,		//(in)データ下限値
	int				pad_type		//(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
)
{
	
	float	sum, val;
	DATA	*pbuffer, padval;
	int		buffer_w, buffer_h;

	//はみ出し分を考慮した入力画像バッファを用意

	buffer_w = image_size.cx + kernel_size.cx;
	buffer_h = image_size.cy + kernel_size.cy;
	pbuffer = new DATA[buffer_w * buffer_h];
	if (pbuffer == NULL)	return false;

	memset(pbuffer, 0, buffer_w * buffer_h * sizeof(DATA));


	for (int y = area.top - kernel_size.cy / 2; y < area.top; y++) 
	{//上
		for (int x = area.left - kernel_size.cx / 2; x < area.left; x++)
		{//左
			if (pad_type == 1) {//外周の画素を延長
				padval = *(p_src_data + area.left + area.top*image_size.cx);
			}
			else if (pad_type == 2) {//外周で折り返し
				padval = *(p_src_data + (area.left - x) + (area.top-y)*image_size.cx);
			}
			else {
				padval = 0;
			}
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = padval;
		}

		for (int x = area.left ; x <= area.right; x++)
		{//中央
			if (pad_type == 1) {//外周の画素を延長
				padval = *(p_src_data + x + area.top*image_size.cx);
			}
			else if (pad_type == 2) {//外周で折り返し
				padval = *(p_src_data + x + (area.top - y)*image_size.cx);
			}
			else {
				padval = 0;
			}
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = padval;
		}

		for (int x = area.right+1; x <= area.right + kernel_size.cx / 2 ; x++)
		{//右
			if (pad_type == 1) {//外周の画素を延長
				padval = *(p_src_data + area.right + area.top*image_size.cx);
			}
			else if (pad_type == 2) {//外周で折り返し
				padval = *(p_src_data + (2 * area.right + 1 - x) + (area.top - y)*image_size.cx);
			}
			else {
				padval = 0;
			}
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = padval;
		}
	}


	for (int y = area.top; y <= area.bottom; y++)
	{//中央
		for (int x = area.left - kernel_size.cx / 2; x < area.left; x++)
		{//左
			if (pad_type == 1) {//外周の画素を延長
				padval = *(p_src_data + area.left + y*image_size.cx);
			}
			else if (pad_type == 2) {//外周で折り返し
				padval = *(p_src_data + (area.left-x) + y*image_size.cx);
			}
			else {
				padval = 0;
			}
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = padval;
		}

		for (int x = area.left; x <= area.right; x++)
		{//中央
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = *(p_src_data + x + y*image_size.cx);
		}

		for (int x = area.right + 1; x <= area.right + kernel_size.cx / 2; x++)
		{//右
			if (pad_type == 1) {//外周の画素を延長
				padval = *(p_src_data + area.right + y*image_size.cx);
			}
			else if (pad_type == 2) {//外周で折り返し
				padval = *(p_src_data + (2*area.right + 1 - x) + y*image_size.cx);
			}
			else {
				padval = 0;
			}
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = padval;
		}
	}

	for (int y = area.bottom +1; y <= area.bottom + kernel_size.cy / 2; y++)
	{//下
		for (int x = area.left - kernel_size.cx / 2; x < area.left; x++)
		{//左
			if (pad_type == 1) {//外周の画素を延長
				padval = *(p_src_data + area.left + area.bottom*image_size.cx);
			}
			else if (pad_type == 2) {//外周で折り返し
				padval = *(p_src_data + (area.left - x) + (2*area.bottom + 1 - y)*image_size.cx);
			}
			else {
				padval = 0;
			}
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = padval;
		}

		for (int x = area.left; x <= area.right; x++)
		{//中央
			if (pad_type == 1) {//外周の画素を延長
				padval = *(p_src_data + x + area.bottom*image_size.cx);
			}
			else if (pad_type == 2) {//外周で折り返し
				padval = *(p_src_data + x + (2 * area.bottom + 1 - y)*image_size.cx);
			}
			else {
				padval = 0;
			}
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = padval;
		}

		for (int x = area.right + 1; x <= area.right + kernel_size.cx / 2; x++)
		{//右
			if (pad_type == 1) {//外周の画素を延長
				padval = *(p_src_data + area.right + area.bottom*image_size.cx);
			}
			else if (pad_type == 2) {//外周で折り返し
				padval = *(p_src_data + (2 * area.right + 1 - x) + (2 * area.bottom + 1 - y)*image_size.cx);
			}
			else {
				padval = 0;
			}
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = padval;
		}
	}



	memset(p_dst_data, 0, image_size.cx * image_size.cy * sizeof(DATA2));//出力データ初期化



	//畳みこみ

	for (int y = area.top ; y <= area.bottom ; y++)
	{
		for (int x = area.left ; x <= area.right ; x++)
		{
			sum = 0;
			for (int ky = -kernel_size.cy / 2; ky <= kernel_size.cy / 2; ky++)
			{
				for (int kx = -kernel_size.cx / 2; kx <= kernel_size.cx / 2; kx++)
				{
					sum += (float)*(pbuffer + (x + kx + kernel_size.cx / 2) + (y + ky + kernel_size.cy / 2)*buffer_w) *pKernel[kernel_size.cx*(ky + kernel_size.cy / 2) + kx + kernel_size.cx / 2];
				}
			}


			//各データフォーマットの最大・最小を超えないように対応
			val = (float)sum*contrast + offset;

			if (val < data_min)		val = data_min;
			else if (val > data_max)	val = data_max;

			*(p_dst_data + x + y*image_size.cx) = (DATA2)(val);
		}
	}

	delete[]	pbuffer;

	return true;
}



/********************************************************************
機  能  名  称 : コンボリューション
関    数    名 : filter_convolution_overwrite
引          数 :
戻    り    値 : 成功すれば true
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
bool	filter_convolution_overwrite(
	DATA			*p_src_data,	//(in/out)対象画像配列の先頭ポインタ
	SIZE			image_size,		//(in)画像サイズ
	SIZE			kernel_size,	//(in)フィルタサイズ(奇数で設定すること)
	float			*pKernel,		//(in)カーネル(kernel_size.cx * kernel_size.cyの配列)の先頭ポインタ
	RECT			area,			//(in)処理領域
	float			offset,			//(in)値のオフセット
	float			contrast,		//(in)コントラスト
	float			data_max,		//(in)データ上限値
	float			data_min,		//(in)データ下限値
	int				pad_type		//(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
	)
{
	bool	ret = true;
	DATA	*p_buffer = NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//バッファメモリ確保
	if (!p_buffer)	return  false;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//バッファ初期化

	//実行
	ret = filter_convolution(p_src_data, p_buffer, image_size, kernel_size, pKernel, area, offset, contrast, data_max, data_min, pad_type);

	//バッファの値を画像メモリに戻す
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;

}