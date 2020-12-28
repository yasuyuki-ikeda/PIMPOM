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
//膨張フィルタ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "filter.h"

// *************************************
//         マ  ク  ロ   定   義         
// *************************************
#define		BIGGER(A,B)			((A)>(B)) ? (A):(B)
#define		SMALLER(A,B)		((A)<(B)) ? (A):(B)

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
//テンプレートで型なし(全ての型の画像に対応する)関数をつくる

// filter_sizeに偶数を指定できるようにした(2x2の実現のため)
// 偶数のときは円形マスク処理をしない
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class DATA>
void	filter_dilation(DATA *p_src_data, DATA *p_dst_data, CSize image_size, CSize filter_size, CRect work_area, bool circleMask)
{
	int	  i,j,k,l;
	float	maximum, val;
	CRect	area, comb;
	char	maskBuffer[1000]={0};


	//計測領域の設定
	area.left = BIGGER(work_area.left , 0);
	area.right = SMALLER(work_area.right , image_size.cx -1);
	area.top = BIGGER(work_area.top , 0);
	area.bottom = SMALLER(work_area.bottom , image_size.cy -1);


	//畳みこみ領域の設定
	if(filter_size.cx%2==1){//奇数サイズ
		comb.left   = -filter_size.cx/2;
		comb.right  =  filter_size.cx/2;
	}else{//偶数サイズ
		comb.left   = -filter_size.cx/2;
		comb.right  =  filter_size.cx/2-1;
		circleMask = false;
	}

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


	for(j = area.top ; j<=area.bottom ; j++)
	{
		for(i = area.left ; i<=area.right ; i++)
		{

			maximum = (double)*(p_src_data + i + j*image_size.cx);//最大値初期化
			char *ptr = maskBuffer;

			for(k=comb.top ; k<=comb.bottom ; k++)
			{
				for(l=comb.left ; l<=comb.right ; l++)
				{
					if(circleMask){
						if(*ptr++ == 0)		continue;//マスク外判定
					}

					if( (i+l)<0 || (i+l)>=image_size.cx || (j+k)<0 || (j+k)>=image_size.cy )	continue;//計測領域外判定


					val = (double)*(p_src_data + (i+l) + (j+k)*image_size.cx);

					if(val > maximum)//最大値更新
					{
						maximum = val;
					}
				}
			}
			*(p_dst_data + i + j*image_size.cx) = (DATA)maximum;
		}
	}
}


template <class DATA>
void	filter_dilation_wrap(DATA *p_src_data, CSize image_size, CSize filter_size, CRect work_area, bool circleMask)
{
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//バッファメモリ確保
	if(!p_buffer)	return;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//バッファ初期化



	//実行
	filter_dilation(p_src_data,	p_buffer, image_size, filter_size, work_area, circleMask);

	//バッファの値を画像メモリに戻す
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;
}


/********************************************************************
機  能  名  称 :  膨張フィルタ
関    数    名 : FilterDilation
引          数 : long		image_num	(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long		dst_data_number	(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
				 CSize		filter_size	(in)フィルタサイズ
				 bool		circleMask  (in)円形マスクをつける
戻    り    値 : 成功すれば true
機          能 : フィルタ処理結果は、同画像メモリに上書きされる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
2016.04.05   Y.Ikeda         2x2サイズを実行できるようにした（実行後は画像が半画素だけ右下による）
********************************************************************/
bool  CPimpomAPI::FilterDilation(long image_num, long dst_data_number, CSize filter_size, bool circleMask)
{
	CDataUnit	*p_du=NULL;
	long		imsize;
	CRect		calc_area;

		if(image_num == CURRENT_IMAGE)	image_num = GetCurrentImageNumber();
		if(dst_data_number == CURRENT_IMAGE)	dst_data_number = GetCurrentImageNumber();


	if(image_num!=dst_data_number){//データコピー
		if(!CopyDataUnit(image_num, dst_data_number)){
			return false;
		}
	}

	p_du = GetDataUnit(dst_data_number);
		if(!p_du)	return	false;
	imsize = p_du->DataSize.cx * p_du->DataSize.cy;//画像のサイズ（画素数）
	calc_area = p_du->GetRectArea();//処理対象の矩形範囲


		if(p_du->DispRGB == false || ( p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) )//グレー画像に対する処理
		{
			if(p_du->pByteData){//8bit画像
				filter_dilation_wrap(p_du->pByteData + imsize * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel),//平滑化
							p_du->DataSize, filter_size, calc_area, circleMask);

			}else if(p_du->pShortData){//16bit画像
				filter_dilation_wrap(p_du->pShortData + imsize * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel),
							p_du->DataSize, filter_size, calc_area, circleMask);

			}else if(p_du->pWordData){//16bit画像
				filter_dilation_wrap(p_du->pWordData + imsize * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel),
							p_du->DataSize, filter_size, calc_area, circleMask);

			}else if(p_du->pLongData){//32bit画像
				filter_dilation_wrap(p_du->pLongData + imsize * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel),
							p_du->DataSize, filter_size, calc_area, circleMask);

			}else if(p_du->pDwordData){//32bit画像
				filter_dilation_wrap(p_du->pDwordData + imsize * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel),
							p_du->DataSize, filter_size, calc_area, circleMask);

			}else if(p_du->pFloatData){//実数画像
				filter_dilation_wrap(p_du->pFloatData + imsize * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel),//平滑化
							p_du->DataSize, filter_size, calc_area, circleMask);
			}
		}
		else//RGB画像に対する処理
		{
			for(int c=0 ; c<3; c++){
				filter_dilation_wrap(p_du->pByteData + imsize * (p_du->DispPage * p_du->ChannelNumber + c),//平滑化
							p_du->DataSize, filter_size, calc_area, circleMask);
			}
		}
		return true;
}


