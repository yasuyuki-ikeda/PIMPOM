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
//ガウシアンフィルタ
/*******************************************************************/

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
機  能  名  称 : ガウシアンフィルタ
関    数    名 : filter_gauss
引          数 : 
戻    り    値 : 成功すれば true
機          能 : 画像データにガウシアンフィルタをかける
                 3σがフィルタサイズになるようにする

                 カーネル：
				 　  σ = (filter_size-1)/6
				     G(x,y) = exp( - x^2 / 2σ^2 )

				 [濃度]= conv{ I(x,y), G(x,y) }

日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
#if 0
//高速版　x方向フィルタ→y方向フィルタ
template <class DATA, class DATA2>
bool	filter_gauss(DATA			*p_src_data,	//(in)入力画像配列の先頭ポインタ
					 DATA2			*p_dst_data,	//(out)出力画像配列の先頭ポインタ
					 SIZE			image_size,		//(in)画像サイズ
					 int			filter_size,	//(in)フィルタサイズ  奇数値のみ入力可能
					 int			direction,		//(in)フィルタ方向 0:横のみ 1:縦のみ  それ以外:両方
					 RECT			work_area,		//(in)処理領域
					 float			data_max_limit, //(in)データ上限値
					 float			data_min_limit,	//(in)データ下限値
					int				pad_type,		//(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
					float			*pOutKernel		//(out)カーネルパラメタ(NULL可)
					 )
{
	float  kernel[1000]={0};
	int half_kernel_size;
	float  sum;
	float *p_buffer=NULL, *p_buffer2=NULL;


	if(filter_size%2==0)	return false;//フィルタサイズは奇数のみ


	p_buffer = new float[image_size.cx * image_size.cy * 2];//バッファメモリ確保(2面分)
	if(!p_buffer)	return false;

	memset(p_buffer, 0, image_size.cx * image_size.cy * 2*sizeof(float) );//バッファ初期化

	p_buffer2 = p_buffer + image_size.cx * image_size.cy;



	//カーネルの作成(フィルタサイズが3σ×2+1になるようにする)
	half_kernel_size= (filter_size - 1) / 2;//フィルタサイズの半分

	float sig, sig2;
	sig = (float)(filter_size-1)/6.0;
	sig2 = 2 * sig * sig;


	float kernel_sum=0;
    for (int i = 0; i < filter_size; i++) 
	{
        float x = (i - half_kernel_size);
        kernel[i] = exp(- x*x / sig2) ;//exp( - x^2 / 2σ^2 )

		//kernel_sum += kernel[i];
    }


	/*for (int i = 0; i < filter_size; i++) {
		kernel[i] /= kernel_sum;
	}*/




	//垂直方向

	if(direction!=1)
	{
		for(int y = work_area.top ; y<=work_area.bottom ; y++){
			for(int x = work_area.left ; x<=work_area.right ; x++){
				sum = 0;
				kernel_sum=0;
				for(int ky=-half_kernel_size ; ky<=half_kernel_size ; ky++)
				{
					if( (y+ky)<0 || (y+ky)>=image_size.cy )	continue;//はみ出しチェック

					sum += (float)*(p_src_data + x + (y+ky)*image_size.cx) * kernel[ky + half_kernel_size] ;
					kernel_sum += kernel[ky + half_kernel_size];
				}
				*(p_buffer + x + y*image_size.cx) = sum / kernel_sum;
			}
		}
	}
	else
	{	//水平方法のみにフィルタをかけるときはそのままコピー
		for(int y = work_area.top ; y<=work_area.bottom ; y++){
			for(int x = work_area.left ; x<=work_area.right ; x++){
				*(p_buffer + x + y*image_size.cx) = (float)*(p_src_data + x + y*image_size.cx);
			}
		}
	}

	
	//水平方向

	if(direction!=2)
	{
		for(int y = work_area.top ; y<=work_area.bottom ; y++){
			for(int x = work_area.left ; x<=work_area.right ; x++){
				sum = 0;
				kernel_sum=0;
				for(int kx=-half_kernel_size ; kx<=half_kernel_size ; kx++)
				{
					if( (x+kx)<0 || (x+kx)>=image_size.cx )	continue;//はみ出しチェック

					sum += (float)*(p_buffer + (x+kx) + y*image_size.cx) * kernel[kx + half_kernel_size] ;
					kernel_sum += kernel[kx + half_kernel_size];
				}
				*(p_buffer2 + x + y*image_size.cx) = sum/kernel_sum;
			}
		}
	}
	else
	{
		//垂直方向のみにフィルタをかけるときはそのままコピー
		for(int y = work_area.top ; y<=work_area.bottom ; y++){
			for(int x = work_area.left ; x<=work_area.right ; x++){
				*(p_buffer2 + x + y*image_size.cx) = (float)*(p_buffer + x + y*image_size.cx);
			}
		}
	}


	//データ上下限チェック
	for(int y = work_area.top ; y<=work_area.bottom ; y++){
		for(int x = work_area.left ; x<=work_area.right ; x++){
			float val = *(p_buffer2 + x + y*image_size.cx);

			if(val < data_min_limit )		val = data_min_limit;
			else if(val > data_max_limit )	val = data_max_limit;

			*(p_dst_data + x + y*image_size.cx) = (DATA2)(val);
		}
	}


	delete[]	p_buffer;

	return true;
}
#else
//通常版　2次元フィルタ
template <class DATA, class DATA2>
bool	filter_gauss(
	DATA			*p_src_data,	//(in)入力画像配列の先頭ポインタ
	DATA2			*p_dst_data,	//(out)出力画像配列の先頭ポインタ
	SIZE			image_size,		//(in)画像サイズ
	int				filter_size,	//(in)フィルタサイズ  奇数値のみ入力可能
	int				direction,		//(in)フィルタ方向 0:横のみ 1:縦のみ  それ以外:両方
	RECT			work_area,		//(in)処理領域
	float			data_max_limit, //(in)データ上限値
	float			data_min_limit,	//(in)データ下限値
	int				pad_type,		//(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
	float			*pOutKernel		//(out)カーネルパラメタ(NULL可)
)
{
	float  kernel[COMB_FILTER_SIZE_MAX*COMB_FILTER_SIZE_MAX] = { 0 };
	int filter_size_x, filter_size_y;



	if (filter_size % 2 == 0)	return false;//フィルタサイズは奇数のみ

	//フィルタ方向たいおう
	filter_size_x = filter_size;
	filter_size_y = filter_size;
	if (direction == 1) {//横のみ
		filter_size_y = 1;
	}
	else if (direction == 2) {//縦のみ
		filter_size_x = 1;
	}





	//カーネルの作成(フィルタサイズが3σ×2+1になるようにする)
	float sig, sig2;
	sig = (float)(filter_size - 1) / 6.0;
	sig2 = 2 * sig * sig;


	float sum = 0;
	for (int j = 0; j < filter_size_y; j++)
	{
		for (int i = 0; i < filter_size_x; i++)
		{
			int	addr = i + j*filter_size_x;
			double x = (i - filter_size_x / 2);
			double y = (j - filter_size_y / 2);

			kernel[addr] = exp(-(x*x + y*y) / sig2);//exp( - x^2 / 2σ^2 )

			sum += kernel[addr];
		}
	}
	
	//カーネル合計数で割る
	for (int j = 0; j < filter_size_y; j++) {
		for (int i = 0; i < filter_size_x; i++) {
			kernel[i + j*filter_size_x] /= sum;
		}
	}



	if (pOutKernel) {
		memcpy(pOutKernel, kernel, filter_size_y*filter_size_x * sizeof(float));
	}

	//畳みこみ
	SIZE fs;
	fs.cx = filter_size_x;
	fs.cy = filter_size_y;
	filter_convolution(p_src_data, p_dst_data, image_size, fs, kernel, work_area, 0, 1, data_max_limit, data_min_limit, pad_type);

	return true;
}

#endif


/********************************************************************
機  能  名  称 : ガウスフィルタ自己差分によるシェーディング除去
関    数    名 : filter_gauss_unshade
引          数 : 
戻    り    値 : 成功すれば true
機          能 : 元画像からガウシアンフィルタ後の画像を引く
　　　　　　ガウシアンフィルタは3σがフィルタサイズになるようにする

　　　　　[濃度] = ( G(x,y) - I(x,y) + offset )*contrast
              　(I(x,y)は原画像の濃度、G(x,y)はガウシアンフィルタ後の濃度を表す)

日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA, class DATA2>
bool	filter_gauss_unshade(
					 DATA			*p_src_data,	//(in)入力画像配列の先頭ポインタ
					 DATA2			*p_dst_data,	//(out)出力画像配列の先頭ポインタ
					 SIZE			image_size,		//(in)画像サイズ
					 int			filter_size,	//(in)フィルタサイズ  奇数値のみ入力可能
					 int			direction,		//(in)フィルタ方向 0:横のみ 1:縦のみ  それ以外:両方
					 float			offset,			//(in)
					 float			contrast,		//(in)
					 RECT			work_area,		//(in)処理領域
					 float			data_max_limit, //(in)データ上限値
					 float			data_min_limit,	//(in)データ下限値
					int				pad_type,		//(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
					float			*pOutKernel		//(out)カーネルパラメタ(NULL可)
					 )
{
	bool ret=true;
	float *p_buffer=NULL, *p_buffer2=NULL;

	p_buffer = new float[image_size.cx*image_size.cy*2];//バッファメモリ確保
	if(p_buffer==NULL)	return false;

	p_buffer2 = p_buffer + image_size.cx*image_size.cy;

	ret = filter_gauss(p_src_data, p_buffer, image_size, filter_size, direction, work_area, data_max_limit, data_min_limit, pad_type, pOutKernel);
	if(ret)
	{
		//差分をとる
		for(int j = work_area.top ; j<=work_area.bottom ; j++)
		{
			for(int i = work_area.left ; i<=work_area.right ; i++)
			{
				int addr = i + j*image_size.cx;
				
				float val= ( (float)*(p_src_data + addr) - *(p_buffer + addr) + offset ) * contrast;

				//データ上下限チェック
				if(val < data_min_limit )		val = data_min_limit;
				else if(val > data_max_limit )	val = data_max_limit;
				*(p_dst_data + addr) = (DATA2)(val);
			}
		}
	}

	delete[]	p_buffer;
	
	return ret;
}



/********************************************************************
機  能  名  称 : ガウスフィルタ
関    数    名 : filter_gauss_overwrite
引          数 : 
戻    り    値 : 成功すれば true
機          能 : 画像にガウシアンフィルタをかける
				ガウシアンフィルタは3σがフィルタサイズになるようにする
                 元のメモリ上に上書きする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
bool	filter_gauss_overwrite(
							   DATA		*p_src_data,		//(in/out)対象画像配列の先頭ポインタ
							   SIZE		image_size,			//(in)画像サイズ
							   int		filter_size,		//(in)フィルタサイズ
							   int		direction,			//(in)フィルタ方向 0:横のみ 1:縦のみ  それ以外:両方
							   RECT		work_area,			//(in)処理領域
							   float	data_max_limit,		//(in)データ上限値
							   float	data_min_limit,		//(in)データ下限値
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
	ret = filter_gauss(p_src_data,	p_buffer, image_size, filter_size, direction, work_area, data_max_limit, data_min_limit, pad_type, pOutKernel);


	//バッファの値を画像メモリに戻す
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;
}



/********************************************************************
機  能  名  称 : ガウスフィルタ自己差分によるシェーディング除去
関    数    名 : filter_gauss_unshade_overwrite
引          数 : 
戻    り    値 : 成功すれば true
機          能 : 元画像からガウシアンフィルタ後の画像を引く
                 filter_size = 3σ×2+1 とする
				  元のメモリ上に上書きする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
bool	filter_gauss_unshade_overwrite(
							   DATA		*p_src_data,		//(in/out)対象画像配列の先頭ポインタ
							   SIZE		image_size,			//(in)画像サイズ
							   int		filter_size,		//(in)フィルタサイズ
							   int		direction,			//(in)フィルタ方向 0:横のみ 1:縦のみ  それ以外:両方
							   float	offset,				//(in)
							   float	contrast,			//(in)
							   RECT		work_area,			//(in)処理領域
							   float	data_max_limit,		//(in)データ上限値
							   float	data_min_limit,		//(in)データ下限値
								int		pad_type,			//(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
								float	*pOutKernel			//(out)カーネルパラメタ(NULL可)
							   )
{
	bool	ret=true;
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//バッファメモリ確保
	if(!p_buffer)	return false;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//バッファ初期化

	//実行
	ret = filter_gauss_unshade(p_src_data,	p_buffer, image_size, filter_size, direction, offset, contrast, work_area, data_max_limit, data_min_limit, pad_type, pOutKernel);


	//バッファの値を画像メモリに戻す
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;
}