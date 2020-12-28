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
// ラプラシアンオブガウシアンフィルタ
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
//テンプレートで型なし(全ての型の画像に対応する)関数をつくる
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
template <class DATA>
void	filter_thin_log(DATA *p_src_data, DATA *p_dst_data, CSize image_size, 
				   CSize filter_size, CRect work_area, int min_thresh, double sigma)
{

	int	i, j;
	int	v;
	int	x1, y1, x2, y2;
	int	offs, xs, xs2, xs3;
	unsigned char	*p0, *p1;
	short	*sp;
	short	*p_short_buffer=NULL;

	// 初期設定
	p_short_buffer = new short[image_size.cx*image_size.cy];
	if(!p_short_buffer)	return;

	// 計測領域の設定
	x1 = BIGGER(work_area.left , 7/2);
	x2 = SMALLER(work_area.right , image_size.cx - 7/2 -1);
	y1 = BIGGER(work_area.top , 7/2);
	y2 = SMALLER(work_area.bottom , image_size.cy - 7/2 -1);


	// 初期設定
	xs  = image_size.cx;
	xs2 = xs * 2;
	xs3 = xs * 3;
	p0 = (unsigned char *)(p_src_data + y1 * xs + x1);
	sp = (short *)(p_short_buffer + y1 * xs + x1);
	offs = image_size.cx - (x2 - x1 + 1);

	// ７×７ＬＯＧフィルタ処理
	for(j = y1; j <= y2; j++, p0 += offs, sp += offs){
		for(i = x1; i <= x2; i++, p0++, sp++){
			v = -((int)*(p0-xs3-1)+(int)*(p0-xs3  )+(int)*(p0-xs3+1)+(int)*(p0-xs -3)+
			      (int)*(p0-xs +3)+(int)*(p0    -3)+(int)*(p0    +3)+(int)*(p0+xs -3)+
			      (int)*(p0+xs +3)+(int)*(p0+xs3-1)+(int)*(p0+xs3  )+(int)*(p0+xs3+1))
			    -((int)*(p0-xs2-2)+(int)*(p0-xs2+2)+(int)*(p0+xs2-2)+(int)*(p0+xs2+2)) * 2
			    -((int)*(p0-xs2-1)+(int)*(p0-xs2  )+(int)*(p0-xs2+1)+(int)*(p0-xs -2)+
			      (int)*(p0-xs +2)+(int)*(p0    -2)+(int)*(p0    +2)+(int)*(p0+xs -2)+
			      (int)*(p0+xs +2)+(int)*(p0+xs2-1)+(int)*(p0+xs2  )+(int)*(p0+xs2+1)) * 4
			    +((int)*(p0-xs   )+(int)*(p0    -1)+(int)*(p0    +1)+(int)*(p0+xs   )) * 9
			    +((int)*(p0      )) * 32;
			*sp = v / 16;
		}
	}

	// ゼロクロス点抽出
	sp = (short *)(p_short_buffer + (y1+1) * xs + (x1+1));
	p1 = (unsigned char *)(p_dst_data     + (y1+1) * xs + (x1+1));
	offs = image_size.cx - (x2 - x1 - 1);
	for(j = y1+1; j <= y2-1; j++, sp += offs, p1 += offs){
		for(i = x1+1; i <= x2-1; i++, sp++, p1++){
			v = (int)*(sp      - 1) * (int)*(sp      + 1) +
			    (int)*(sp - xs    ) * (int)*(sp + xs    ) +
			    (int)*(sp - xs - 1) * (int)*(sp + xs + 1) +
			    (int)*(sp - xs + 1) * (int)*(sp + xs - 1);
			if (v / 32 < -min_thresh) {
				*p1 = 255;
			} else {
				*p1 = 0;
			}
		}
	}

	// 孤立点除去
	p1 = (unsigned char *)(p_dst_data + (y1+1) * xs + (x1+1));
	offs = image_size.cx - (x2 - x1 - 1);
	for(j = y1+1; j <= y2-1; j++, p1 += offs){
		for(i = x1+1; i <= x2-1; i++, p1++){
			if (*(p1 - xs - 1) == 0 && *(p1 - xs) == 0 && *(p1 - xs + 1) == 0 &&
			    *(p1      - 1) == 0 &&                    *(p1      + 1) == 0 &&
			    *(p1 + xs - 1) == 0 && *(p1 + xs) == 0 && *(p1 + xs + 1) == 0) {
				*p1 = 0;
			}
		}
	}

	// 終了処理
	delete[]	p_short_buffer;
}
#endif


/********************************************************************
機  能  名  称 : ラプラシアンオブガウシアン(Log)フィルタ
関    数    名 : filter_lapcace_gauss_internal
引          数 : 
戻    り    値 : 成功すれば true
機          能 : 画像データにラプラシアンオブガウシアンフィルタをかける
          カーネルのゼロクロス間隔はフィルタサイズの半分とする
          
          カーネル：
             σ = (fs - 1) / 4√2
             LoG(x,y) = (x^2 + y^2 - 2σ^2) exp( - (x^2 + y^2) / 2σ^2 )
               
　　　　　　上式で算出後、最大1,平均0になるように正規化

　　　　　[濃度] =  conv{I(x,y) , LoG(x,y)} * contrast + offset
               (I(x,y)は元画像の濃度、conv{a,b}はaとbの畳み込み)
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA, class DATA2>
bool	filter_lapcace_gauss_internal(
									  DATA		*p_src_data,	//(in)入力画像配列の先頭ポインタ
									  DATA2		*p_dst_data,	//(out)出力画像配列の先頭ポインタ
									  SIZE		image_size,		//(in)画像サイズ
									  int		filter_size,	//(in)フィルタサイズ
									  int		direction,		//(in)フィルタ方向 1:横のみ 2:縦のみ  それ以外:両方
									  RECT		area,			//(in)処理領域
									  float	    offset,			//(in)値のオフセット
									  float	    contrast,		//(in)コントラスト
									  double	data_max,		//(in)データ上限値
									  double	data_min,		//(in)データ下限値
									 int		pad_type,		//(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
									  float		*pOutKernel		//(out)カーネルパラメタ(NULL可)
									  )
{
	float  kernel[COMB_FILTER_SIZE_MAX*COMB_FILTER_SIZE_MAX]={0};
	int filter_size_x, filter_size_y;



	if (filter_size % 2 == 0)	return false;//フィルタサイズは奇数のみ



	//フィルタ方向たいおう
	filter_size_x = filter_size;
	filter_size_y = filter_size;
	if(direction==1){//横のみ
		filter_size_y = 1;
	}else if(direction==2){//縦のみ
		filter_size_x = 1;
	}


	//LoGカーネルの作成(カーネルサイズの半分がゼロクロス間隔、x,y=0,0のとき最大値１)
	float sig, sig2;
	sig = (float)(filter_size-1)/(4.0*sqrt(2.0));
	sig2 = 2 * sig * sig;

	float ave=0;
    for (int j = 0; j < filter_size_y; j++) 
	{
		for (int i = 0; i < filter_size_x; i++) 
		{
			int	addr = i + j*filter_size_x;
			double x = (i - filter_size_x/2);
			double y = (j - filter_size_y/2);
			kernel[addr] = (x*x + y*y - sig2) * exp(- (x*x+y*y) / sig2) ;//  (x^2 + y^2 - 2σ^2) exp( - (x^2 + y^2) / 2σ^2 )
			kernel[addr] /= -sig2;//最大が１になるように正規化

			ave += kernel[addr];
		}
	}
	ave /= (float)(filter_size_y*filter_size_x);

	//カーネル平均を0にする
	for (int j = 0; j < filter_size_y; j++){ 
	    for (int i = 0; i < filter_size_x; i++) {
			kernel[i + j*filter_size_x] -= ave;
		}
	}

	if (pOutKernel) {
		memcpy(pOutKernel, kernel, filter_size_y*filter_size_x * sizeof(float));
	}

	//畳みこみ
	SIZE fs;
	fs.cx = filter_size_x;
	fs.cy = filter_size_y;
	filter_convolution(p_src_data, p_dst_data, image_size, fs, kernel, area, offset, contrast, data_max, data_min, pad_type);

	return true;
}


/********************************************************************
機  能  名  称 : ラプラシアンオブガウシアン(LoG)フィルタ
関    数    名 : filter_lapcace_gauss
引          数 : 
戻    り    値 : 成功すれば true
機          能 :  画像データにラプラシアンオブガウシアンフィルタをかける
          カーネルのゼロクロス間隔はフィルタサイズの半分とする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA, class DATA2>
bool	filter_lapcace_gauss(
							 DATA		*p_src_data,	//(in)入力画像配列の先頭ポインタ
							 DATA2		*p_dst_data,	//(out)出力画像配列の先頭ポインタ
							 SIZE		image_size,		//(in)画像サイズ
							 int		filter_size,	//(in)フィルタサイズ
							 int		direction,		//(in)フィルタ方向 1:横のみ 2:縦のみ  それ以外:両方
							 RECT		work_area,		//(in)処理領域
							 float	    offset,			//(in)値のオフセット
							 float	    contrast,		//(in)コントラスト
							 double		data_max,		//(in)データ上限値
							 double		data_min,		//(in)データ下限値
							int			pad_type,		//(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
							 float		*pOutKernel		//(out)カーネルパラメタ(NULL可)
							 )
{
	int ret=true;

	if(filter_size%2==0)	return false;//フィルタサイズは奇数のみ


	//ラプラシアンオブガウシアン実行
	ret = filter_lapcace_gauss_internal(p_src_data, p_dst_data, image_size, filter_size, direction, work_area, offset, contrast, data_max, data_min, pad_type, pOutKernel);


	return ret;
}



/********************************************************************
機  能  名  称 : LoG用いた細線エッジ抽出
関    数    名 : filter_lapcace_gauss
引          数 : 
戻    り    値 : 成功すれば true
機          能 :  LoGフィルタ後にゼロクロスする画素をエッジとして検出する
                 カーネルのゼロクロス間隔はフィルタサイズの半分とする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA, class DATA2>
bool	filter_edge_lapcace_gauss(
								 DATA		*p_src_data,	//(in)入力画像配列の先頭ポインタ
								 DATA2		*p_dst_data,	//(out)出力画像配列の先頭ポインタ
								 SIZE		image_size,		//(in)画像サイズ
								 int		filter_size,	//(in)フィルタサイズ
								 int		direction,		//(in)フィルタ方向 1:横のみ 2:縦のみ  それ以外:両方
								 RECT		work_area,		//(in)処理領域
								 float		min_thresh,		//(in)
								 int		pad_type		//(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
								  )
{
	bool	ret = true;

	if(filter_size%2==0)	return false;//フィルタサイズは奇数のみ


	float *p_buffer = new float[image_size.cx * image_size.cy];//バッファメモリ確保
	if(!p_buffer)	return false;

	memset(p_buffer, image_size.cx * image_size.cy * sizeof(float),0);//バッファ初期化



	//ラプラシアンオブガウシアン実行
	ret = filter_lapcace_gauss_internal(p_src_data, p_buffer, image_size, filter_size, direction, work_area, 0, 1, FLT_MAX,-FLT_MAX, pad_type, NULL);

	if(ret){
		//ゼロクロス検出
		for(int j = work_area.top ; j<=work_area.bottom ; j++){
			for(int i = work_area.left ; i<=work_area.right ; i++){

				*(p_dst_data + i + j*image_size.cx) = 0;//初期化

				float val = *(p_buffer + i + j*image_size.cx);

				if(i+1<=work_area.right){//横方向
					float valn = *(p_buffer + i+1 + j*image_size.cx);

					if(val*valn<0 && fabs(val - valn) > min_thresh){
						*(p_dst_data + i + j*image_size.cx) = 255;
					}
				}
					
				if(j+1<=work_area.bottom){//縦方向
					float valn = *(p_buffer + i + (j+1)*image_size.cx);

					if(val*valn<0 && fabs(val - valn) > min_thresh){
						*(p_dst_data + i + j*image_size.cx) = 255;
					}
				}
			}
		}
	}

	delete[]	p_buffer;

	return ret;
}


/********************************************************************
機  能  名  称 : ラプラシアンオブガウシアン(LoG)フィルタ
関    数    名 : filter_lapcace_gauss_overwrite
引          数 : 
戻    り    値 : 成功すれば true
機          能 : 画像データにラプラシアンオブガウシアンフィルタをかける
                 結果を原画像のメモリ上に上書きする
                  
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
bool	filter_lapcace_gauss_overwrite(
									DATA		*p_src_data,	//(in/out)対象画像配列の先頭ポインタ
									SIZE		image_size,		//(in)画像サイズ
									int			filter_size,	//(in)フィルタサイズ
									int			direction,		//(in)フィルタ方向 1:横のみ 2:縦のみ  それ以外:両方
									RECT		work_area,		//(in)処理領域
									float	    offset,			//(in)値のオフセット
									float	    contrast,		//(in)コントラスト
									float		data_max,		//(in)データ上限値
									float		data_min,		//(in)データ下限値
									int			pad_type,		//(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
									float		*pOutKernel		//(out)カーネルパラメタ(NULL可)
									)
{
	bool	ret=true;
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//バッファメモリ確保
	if(!p_buffer)	return false;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//バッファ初期化

	//実行
	ret = filter_lapcace_gauss(p_src_data,	p_buffer, image_size, filter_size, direction, work_area, offset, contrast, data_max, data_min, pad_type, pOutKernel);

	//バッファの値を画像メモリに戻す
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;
}


/********************************************************************
機  能  名  称 : LoG用いた細線エッジ抽出
関    数    名 : filter_edge_lapcace_gauss_overwrite
引          数 : 
戻    り    値 : 成功すれば true
機          能 :  LoGフィルタ後にゼロクロスする画素をエッジとして検出する
                  カーネルのゼロクロス間隔はフィルタサイズの半分とする
				  結果を原画像のメモリ上に上書きする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
bool	filter_edge_lapcace_gauss_overwrite(
										DATA		*p_src_data,	//(in/out)対象画像配列の先頭ポインタ
										SIZE		image_size,		//(in)画像サイズ
										int			filter_size,	//(in)フィルタサイズ
										int			direction,		//(in)フィルタ方向 1:横のみ 2:縦のみ  それ以外:両方
										RECT		work_area,		//(in)処理領域
										float		min_thresh,		//(in)
										int			pad_type		//(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
									  )
{
	bool	ret=true;
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//バッファメモリ確保
	if(!p_buffer)	return false;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//バッファ初期化

	//実行
	ret = filter_edge_lapcace_gauss(p_src_data,	p_buffer, image_size, filter_size, direction, work_area, min_thresh, pad_type);

	//バッファの値を画像メモリに戻す
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;
}

