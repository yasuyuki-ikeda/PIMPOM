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
//膨張・収縮フィルタ
/*******************************************************************/
#pragma once

//**************************************
//         インクルードファイル         
//**************************************
//#include "PIMPOM_API.h"
//#include "filter.h"

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

void createMask(CSize filter_size, char pMask[]);//フィルタ形状に合わせた楕円形の0/1マスクを作成する

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// テンプレートで全ての型の画像に対応できるようにする
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : 膨張・収縮
関    数    名 : filter_erosion_dilataion
引          数 : 
戻    り    値 : 成功すれば true
機          能 :  
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
bool	filter_erosion_dilataion(
								 DATA	*p_src_data,	//(in)入力画像配列の先頭ポインタ
								 DATA	*p_dst_data,	//(out)出力画像配列の先頭ポインタ
								 SIZE	image_size,		//(in)画像サイズ
								 SIZE	filter_size,	//(in)フィルタサイズ (縦横別設定可能 偶数のときは円形マスク処理をしない)
								 RECT	work_area,		//(in)処理領域
								 bool	circleMask,		//(in)円形マスクをつける
								 bool	doErosion		//(in)true:収縮  false:膨張
								 )
{

	float	tampVal, val;
	RECT	comb;
	char	maskBuffer[LANK_FILTER_SIZE_MAX*LANK_FILTER_SIZE_MAX]={0};



	//畳みこみ領域の設定

	//横
	if(filter_size.cx%2==1){//奇数サイズ
		comb.left   = -filter_size.cx/2;
		comb.right  =  filter_size.cx/2;
	}else{//偶数サイズ
		comb.left   = -filter_size.cx/2;
		comb.right  =  filter_size.cx/2-1;
		circleMask = false;
	}

	//縦
	if(filter_size.cy%2==1){//奇数サイズ
		comb.top    = -filter_size.cy/2;
		comb.bottom =  filter_size.cy/2;
	}else{//偶数サイズ
		comb.top    = -filter_size.cy/2;
		comb.bottom =  filter_size.cy/2-1;
		circleMask = false;
	}


	//マスク生成
	if(circleMask){
		createMask(filter_size, maskBuffer);
	}


	//収縮実行
	for(int j = work_area.top ; j<=work_area.bottom ; j++)
	{
		for(int i = work_area.left ; i<=work_area.right ; i++)
		{

			tampVal = (float)*(p_src_data + i + j*image_size.cx);//最小値初期化
			char *ptr = maskBuffer;

			for(int k=comb.top ; k<=comb.bottom ; k++)
			{
				for(int l=comb.left ; l<=comb.right ; l++)
				{
					if(circleMask){
						if(*ptr++ == 0)	continue;//マスク外判定
					}

					if( (i+l)<0 || (i+l)>=image_size.cx || (j+k)<0 || (j+k)>=image_size.cy )	continue;//計測領域外判定


					val = (float)*(p_src_data + (i+l) + (j+k)*image_size.cx);

					if(doErosion){//収縮
						if(val < tampVal)//最小値更新
						{
							tampVal = val;
						}
					}else{//膨張
						if(val > tampVal)//最大値更新
						{
							tampVal = val;
						}
					}
				}
			}
			*(p_dst_data + i + j*image_size.cx) = (DATA)tampVal;
		}
	}

	return true;
}

/********************************************************************
機  能  名  称 : 膨張・収縮
関    数    名 : filter_erosion_dilataion_overwrite
引          数 : 
戻    り    値 : 成功すれば true
機          能 :  結果を原画像のメモリ上に上書きする 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
bool	filter_erosion_dilataion_overwrite(
											DATA	*p_src_data,	//(in/out)対象画像配列の先頭ポインタ
											SIZE	image_size,		//(in)画像サイズ
											SIZE	filter_size,	//(in)フィルタサイズ (縦横別設定可能 偶数のときは円形マスク処理をしない)
											RECT	work_area,		//(in)処理領域
											bool	circleMask,		//(in)円形マスクをつける
											bool	doErosion		//(in)true:収縮  false:膨張
											)
{
	bool	ret=true;
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//バッファメモリ確保
	if(!p_buffer)	return  false;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//バッファ初期化

	//実行
	ret = filter_erosion_dilataion(p_src_data,	p_buffer, image_size, filter_size, work_area, circleMask, doErosion);

	//バッファの値を画像メモリに戻す
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;
}




