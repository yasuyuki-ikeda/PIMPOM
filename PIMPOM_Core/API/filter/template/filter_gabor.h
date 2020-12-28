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
//ガボールフィルタ
/*******************************************************************/
#pragma once

//**************************************
//         インクルードファイル         
//**************************************
#include "filter_convolution.h"

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
// テンプレートで全ての型の画像に対応できるようにする
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : ガボールフィルタ
関    数    名 : filter_gabor
引          数 : 
戻    り    値 : 成功すれば true
機          能 : 画像データにガボールフィルタをかける

           カーネル：
                 xt = cos(πθ/180)x + sin(πθ/180)y
                 yt = -sin(πθ/180)x + cos(πθ/180)y 
                 σ=(fs-1)/3
                 λ=fs/freq
　　　　　　　　 Gabor(x,y) = exp( - (xt^2 + γ^2 yt^2) / 2σ^2 ) cos( 2πxt/λ + πΨ/180)

                 上式で算出後、平均0になるように正規化

           [濃度] =  conv{I(x,y) , Gabor(x,y)} * contrast + offset
                 (I(x,y)は元画像の濃度、conv{a,b}はaとbの畳み込み)

日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA, class DATA2>
bool	filter_gabor(DATA			*p_src_data,	//(in)入力画像配列の先頭ポインタ
					 DATA2			*p_dst_data,	//(out)出力画像配列の先頭ポインタ
					 SIZE			image_size,		//(in)画像サイズ
					 int			filter_size,	//(in)フィルタサイズ  奇数値のみ入力可能
					 float			gamma,			//(in)γ：カーネル扁平率
					 float			freq,			//(in)カーネル内での周期
					 float			theta,			//(in)θ：カーネル回転角度
					 float			psi,			//(in)Ψ：カーネル初期位相
					 RECT			area,			//(in)処理領域
					 float			offset,			//(in)値のオフセット
					 float			contrast,		//(in)コントラスト
					 float			data_max,		//(in)データ上限値
					 float			data_min,		//(in)データ下限値
				 	int				pad_type,		//(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
					 float			*pOutKernel		//(out)カーネルパラメタ(NULL可)
					 )
{
	float  kernel[COMB_FILTER_SIZE_MAX*COMB_FILTER_SIZE_MAX]={0};



	if (filter_size % 2 == 0)	return false;//フィルタサイズは奇数のみ

	if(freq<=0)	return false;

	memset(p_dst_data, 0, image_size.cx * image_size.cy * sizeof(DATA2) );//出力データ初期化


	//カーネルの作成
	float sigma, sigma2, cos_t, sin_t, ip, lambda;
	sigma = (float)(filter_size-1)/6.0;//(フィルタサイズが3σ×2+1になるようにする)
	sigma2 = 2 * sigma * sigma;
	cos_t = cos(theta/180.0*3.141592);
	sin_t = sin(theta/180.0*3.141592);
	ip = psi/180.0*3.141592;
	lambda = filter_size/freq;


	float ave=0;
    for (int j = 0; j < filter_size; j++) 
	{
		for (int i = 0; i < filter_size; i++) 
		{
			double x = (i - filter_size/2);
			double y = (j - filter_size/2);
			double xt = cos_t*x + sin_t*y;
			double yt = -sin_t*x + cos_t*y;
			kernel[j*filter_size+i] = exp(- (xt*xt+yt*yt*gamma*gamma) / sigma2) * cos(2*3.1415*xt/lambda + ip) ;//  exp( - (xt^2 + γ^2 yt^2) / 2σ^2 ) cos( 2πxt/λ + psi )

			ave += kernel[j*filter_size + i];
		}
	}
	ave /= (float)(filter_size*filter_size);

	//カーネル平均を0にする
	for (int j = 0; j < filter_size; j++){ 
	    for (int i = 0; i < filter_size; i++) {
			kernel[j*filter_size + i] -= ave;
		}
	}


	if (pOutKernel) {
		memcpy(pOutKernel, kernel, filter_size*filter_size * sizeof(float));
	}



	//畳みこみ
	SIZE fs;
	fs.cx = filter_size;
	fs.cy = filter_size;
	filter_convolution(p_src_data, p_dst_data, image_size, fs, kernel, area, offset, contrast, data_max, data_min, pad_type);

	return true;
}


/********************************************************************
機  能  名  称 : ガボール)フィルタ
関    数    名 : filter_gabor_overwrite
引          数 : 
戻    り    値 : 成功すれば true
機          能 : 画像データにガボールフィルタをかける
                 結果を原画像のメモリ上に上書きする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
bool	filter_gabor_overwrite(
					 DATA			*p_src_data,	//(in/out)対象画像配列の先頭ポインタ
					 SIZE			image_size,		//(in)画像サイズ
					 int			filter_size,	//(in)フィルタサイズ  奇数値のみ入力可能
					 float			gamma,			//(in)γ：カーネル扁平率
					 float			freq,			//(in)カーネル内での周期
					 float			theta,			//(in)θ：カーネル回転角度
					 float			psi,			//(in)Ψ：カーネル初期位相
					 RECT			work_area,		//(in)処理領域
					 float			offset,			//(in)値のオフセット
					 float			contrast,		//(in)コントラスト
					 float			data_max,		//(in)データ上限値
					 float			data_min,		//(in)データ下限値
					 int			pad_type,		//(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
					 float			*pOutKernel		//(out)カーネルパラメタ(NULL可)
						)
{
	bool	ret=true;
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//バッファメモリ確保
	if(!p_buffer)	return false;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//バッファ初期化

	//実行
	ret = filter_gabor(p_src_data,	p_buffer, image_size, filter_size, gamma, freq, theta, psi, work_area, offset, contrast, data_max, data_min, pad_type, pOutKernel);

	//バッファの値を画像メモリに戻す
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;
}
