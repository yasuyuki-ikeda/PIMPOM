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
//各種フィルタ
/*******************************************************************/
#pragma once

//**************************************
//         インクルードファイル         
//**************************************
#include "PIMPOM_API.h"
#include "filter.h"

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




//フィルタ形状に合わせた楕円形の0/1マスクを作成する
void createMask(	CSize	filter_size,	//(in)フィルタサイズ
					char	pMask[]			//(out)マスクの0/1配列
				);




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//テンプレートで型なし(全ての型の画像に対応する)関数をつくる
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : ミラー反転
関    数    名 : filter_mirror
引          数 : 
戻    り    値 : 成功すれば true
機          能 :  
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
void	filter_mirror(
					  DATA *p_src_data, //(in)入力画像配列の先頭ポインタ
					  DATA *p_dst_data,	//(out)出力画像配列の先頭ポインタ
					  SIZE image_size,	//(in)画像サイズ
					  RECT work_area,	//(in)処理領域
					  int dir			//(in)0:左右反転 1:上下反転 2:180°反転
					  )
{
	for(int j = work_area.top ; j<=work_area.bottom ; j++){
		for(int i = work_area.left ; i<=work_area.right ; i++){
			if(dir==0){//左右反転
				*(p_dst_data + i + j*image_size.cx) = *(p_src_data + (image_size.cx-1-i) + j*image_size.cx);
			}else if(dir==1){//上下反転
				*(p_dst_data + i + j*image_size.cx) = *(p_src_data + i + (image_size.cy-1-j)*image_size.cx);
			}else{//180°反転
				*(p_dst_data + i + j*image_size.cx) = *(p_src_data + (image_size.cx-1-i) + (image_size.cy-1-j)*image_size.cx);
			}
		}
	}
}

/********************************************************************
機  能  名  称 : ミラー反転
関    数    名 : filter_mirror_overwrite
引          数 : 
戻    り    値 : 成功すれば true
機          能 :   結果を原画像のメモリ上に上書きする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
void	filter_mirror_overwrite(
								DATA *p_src_data,	//(in/out)対象画像配列の先頭ポインタ
								SIZE image_size,	//(in)画像サイズ
								RECT work_area,		//(in)処理領域
								int dir				//(in)0:左右反転 1:上下反転 2:180°反転
								)
{
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//バッファメモリ確保
	if(!p_buffer)	return;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//バッファ初期化

	//実行
	filter_mirror(p_src_data, p_buffer, image_size, work_area, dir);

	//バッファの値を画像メモリに戻す
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;
}


/********************************************************************
機  能  名  称 : 濃度反転
関    数    名 : filter_nega
引          数 : 
戻    り    値 : 成功すれば true
機          能 :  
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
void	filter_nega(
					DATA *p_src_data,	//(in)入力画像配列の先頭ポインタ
					DATA *p_dst_data,	//(our)出力画像配列の先頭ポインタ
					SIZE image_size,	//(in)画像サイズ
					RECT work_area		//(in)処理領域
					)
{
	for(int j = work_area.top ; j<=work_area.bottom ; j++){
		for(int i = work_area.left ; i<=work_area.right ; i++){
			*(p_dst_data + i + j*image_size.cx) = 255 - *(p_src_data + i + j*image_size.cx);
		}
	}
}

/********************************************************************
機  能  名  称 : 濃度反転
関    数    名 : filter_nega_overwrite
引          数 : 
戻    り    値 : 成功すれば true
機          能 :   結果を原画像のメモリ上に上書きする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
void	filter_nega_overwrite(DATA *p_src_data, //(in/out)対象画像配列の先頭ポインタ
							  SIZE image_size,	//(in)画像サイズ
							  RECT work_area	//(in)処理領域
							  )
{
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//バッファメモリ確保
	if(!p_buffer)	return;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//バッファ初期化

	//実行
	filter_nega(p_src_data, p_buffer, image_size, work_area);

	//バッファの値を画像メモリに戻す
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;
}
