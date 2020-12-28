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
//平滑化フィルタ
/*******************************************************************/
#pragma once

//**************************************
//         インクルードファイル         
//**************************************
//#include "PIMPOM_API.h"

// *************************************
//         マ  ク  ロ   定   義         
// *************************************
#define		BIGGER(A,B)			( ((A)>(B)) ? (A):(B) )
#define		SMALLER(A,B)		( ((A)<(B)) ? (A):(B) )

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
//テンプレートで型なし(全ての型の画像に対応する)関数をつくる
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : 移動平均
関    数    名 : filter_smooth
引          数 : 
戻    り    値 : 成功すれば true
機          能 :  移動平均による平滑化
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
bool	filter_smooth(
					  DATA		*p_src_data,	//(in)入力画像配列の先頭ポインタ
					  DATA		*p_dst_data,	//(out)出力画像配列の先頭ポインタ
					  SIZE		image_size,		//(in)画像サイズ
					  SIZE		filter_size,	//(in)フィルタサイズ
					  RECT		work_area		//(in)処理領域
					  )
{
	float	sum;
	int		cnt;


	for(int j = work_area.top ; j<=work_area.bottom ; j++){
		for(int i = work_area.left ; i<=work_area.right ; i++){

			sum = 0;
			cnt = 0;
			for(int k=-filter_size.cy/2 ; k<=filter_size.cy/2 ; k++){
				for(int l=-filter_size.cx/2 ; l<=filter_size.cx/2 ; l++){
					if( (i+l)<0 || (i+l)>=image_size.cx || (j+k)<0 || (j+k)>=image_size.cy )	continue;

					sum += (float)*(p_src_data + (i+l) + (j+k)*image_size.cx);
					cnt++;
				}
			}
			sum /= (float)cnt;
			*(p_dst_data + i + j*image_size.cx) = (DATA)sum;
		}
	}

	return true;
}

/********************************************************************
機  能  名  称 : 最大・最小抜き移動平均
関    数    名 : filter_smooth_m
引          数 : 
戻    り    値 : 成功すれば true
機          能 :  最大・最小抜き移動平均による平滑化　　
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
bool	filter_smooth_m(
					  DATA		*p_src_data,	//(in)入力画像配列の先頭ポインタ
					  DATA		*p_dst_data,	//(out)出力画像配列の先頭ポインタ
					  SIZE		image_size,		//(in)画像サイズ
					  SIZE		filter_size,	//(in)フィルタサイズ
					  RECT		work_area		//(in)処理領域
						)
{
	float	sum, val, max, min;
	int		cnt;


	for(int j = work_area.top ; j<=work_area.bottom ; j++)
	{
		for(int i = work_area.left ; i<=work_area.right ; i++)
		{
			sum = 0;
			max = min = (float)*(p_src_data + i + j*image_size.cx);
			cnt = 0;

			for(int k=-filter_size.cy/2 ; k<=filter_size.cy/2 ; k++)
			{
				for(int l=-filter_size.cx/2 ; l<=filter_size.cx/2 ; l++)
				{
					if( (i+l)<0 || (i+l)>=image_size.cx || (j+k)<0 || (j+k)>=image_size.cy )	continue;

					val = (float)*(p_src_data + (i+l) + (j+k)*image_size.cx);
						if( val > max ){
							max = val;
						}else if( val < min ){
							min = val;
						}
					sum += val;
					cnt++;
				}
			
			}
			sum -= (max + min);
			sum /= (float)(cnt-2);
			*(p_dst_data + i + j*image_size.cx) = (DATA)sum;
		}
	}

	return true;
}

/********************************************************************
機  能  名  称 : 移動平均
関    数    名 : filter_smooth
引          数 : 
戻    り    値 : 成功すれば true
機          能 :  移動平均による平滑化
　　　　　　　　　 結果を原画像のメモリ上に上書きする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
bool	filter_smooth_overwrite(
								DATA	*p_src_data,	//(in/out)対象画像配列の先頭ポインタ
								SIZE	image_size,		//(in)画像サイズ
								SIZE	filter_size,	//(in)フィルタサイズ
								RECT	work_area,		//(in)処理領域
								bool	rem_min_max		//(in)最大最小限除去するかどうか
								)
{
	bool	ret=true;
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//バッファメモリ確保
	if(!p_buffer)	return false;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//バッファ初期化

	//実行
	if( rem_min_max ){
		ret=filter_smooth_m(p_src_data,	p_buffer, image_size, filter_size, work_area);
	}else{
		ret=filter_smooth(p_src_data,	p_buffer, image_size, filter_size, work_area);
	}

	//バッファの値を画像メモリに戻す
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;
}

