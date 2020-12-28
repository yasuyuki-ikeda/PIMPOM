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
//シグモイドフィルタ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "template\filter_sigmoid.h"

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


/********************************************************************
機  能  名  称 :  シグモイドを使ったエッジ検出フィルタ
関    数    名 : FilterEdgeSigmoid
引          数 : long		image_num		(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long		dst_data_number	(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
				 int		filter_size		(in)フィルタサイズ (奇数値のみ)
				 float		k				(in)k
				 float		theta,			(in)θ：カーネル回転角度
				 float		offset,			(in)値のオフセット
				 float		contrast		(in)コントラスト
				 int		pad_type		(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
				 float		*pKernel		(out)kernelの配列（NULL可）
				 bool		toMultiImage	(in)複数のページ／チャネルに同じ処理をするかどうか
戻    り    値 : 成功すれば true
機          能 : フィルタ処理結果は、同画像メモリに上書きされる
                 3σ×2+1がフィルタサイズ (フィルタサイズは5x5以上)
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool  CPimpomAPI::FilterEdgeSigmoid(
							  long			image_num, 
							  long			dst_data_number, 
							  int			filter_size,
							  float			k,				//(in)k
							  float			theta,			//(in)θ：カーネル回転角度
							  float			offset,			//(in)値のオフセット
							  float			contrast,		//(in)コントラスト
							  int			pad_type,		//(in)paddingのタイプ
							  bool			toMultiImage,	//(in)複数のページ／チャネルに同じ処理をするかどうか
							  float			*pOutKernel	//(out)カーネルパラメタ(NULL可能)
							  
								  )
{
	bool ret=true;
	CDataUnit	*p_du;
	long		imsize;
	CRect		calc_area;


	//引数チェック

	if(image_num == CURRENT_IMAGE){
		image_num = GetCurrentImageNumber();
	}else if(image_num >= GetDataUnitNumber() || image_num<0){
		return false;
	}

	if(dst_data_number == CURRENT_IMAGE){
		dst_data_number = GetCurrentImageNumber();
	}else if(dst_data_number >= GetDataUnitNumber() || dst_data_number<0){
		return false;
	}

	if (GetDataUnit(image_num)){
		if (GetDataUnit(image_num)->DataSize.cx <= filter_size || GetDataUnit(image_num)->DataSize.cy <= filter_size ||
			filter_size<3 || filter_size>COMB_FILTER_SIZE_MAX)
		{//フィルタのサイズチェック
			return false;
		}
	}
	else{
		return false;
	}




	//結果を別の画像メモリに入れる場合、まず画像データをコピーする
	if(image_num!=dst_data_number){
		if(!CopyDataUnit(image_num, dst_data_number)){
			return false;
		}
	}

	p_du = GetDataUnit(dst_data_number);
		if(!p_du)	return	false;

	imsize = p_du->DataSize.cx * p_du->DataSize.cy;//画像のサイズ（画素数）
	calc_area = p_du->GetRectArea();//処理対象の矩形範囲



	int cS, cE;//処理開始・終了チャネル
	int pS, pE;//処理開始・終了ページ
	if (toMultiImage){//複数画像に同じ処理を行う場合
		pS = cS = 0;
		pE = p_du->PageNumber - 1;
		cE = p_du->ChannelNumber - 1;
	}
	else{//現在表示中の画像にのみ処理を行う場合
		pS = pE = p_du->DispPage;
		cS = cE = p_du->DispChannel;
	}


	for (int p = pS; p <= pE; p++)
	{
		for (int c = cS; c <= cE; c++)
		{
			if (p_du->pByteData){//8bit画像
				ret = filter_sigmoid_overwrite(p_du->pByteData + imsize * (p * p_du->ChannelNumber + c),//平滑化
					p_du->DataSize, filter_size, k, theta, calc_area, offset, contrast, p_du->GetMaxLimitValue(), p_du->GetMinLimitValue(), pad_type, pOutKernel);

			}
			else if (p_du->pShortData){//8bit画像
				ret = filter_sigmoid_overwrite(p_du->pShortData + imsize * (p * p_du->ChannelNumber + c),//平滑化
					p_du->DataSize, filter_size, k, theta, calc_area, offset, contrast, p_du->GetMaxLimitValue(), p_du->GetMinLimitValue(), pad_type, pOutKernel);

			}
			else if (p_du->pWordData){//8bit画像
				ret = filter_sigmoid_overwrite(p_du->pWordData + imsize * (p * p_du->ChannelNumber + c),//平滑化
					p_du->DataSize, filter_size, k, theta, calc_area, offset, contrast, p_du->GetMaxLimitValue(), p_du->GetMinLimitValue(), pad_type, pOutKernel);

			}
			else if (p_du->pLongData){//8bit画像
				ret = filter_sigmoid_overwrite(p_du->pLongData + imsize * (p * p_du->ChannelNumber + c),//平滑化
					p_du->DataSize, filter_size, k, theta, calc_area, offset, contrast, p_du->GetMaxLimitValue(), p_du->GetMinLimitValue(), pad_type, pOutKernel);

			}
			else if (p_du->pDwordData){//8bit画像
				ret = filter_sigmoid_overwrite(p_du->pDwordData + imsize * (p * p_du->ChannelNumber + c),//平滑化
					p_du->DataSize, filter_size, k, theta, calc_area, offset, contrast, p_du->GetMaxLimitValue(), p_du->GetMinLimitValue(), pad_type, pOutKernel);

			}
			else if (p_du->pFloatData){//実数画像
				ret = filter_sigmoid_overwrite(p_du->pFloatData + imsize * (p * p_du->ChannelNumber + c),//平滑化
					p_du->DataSize, filter_size, k, theta, calc_area, offset, contrast, p_du->GetMaxLimitValue(), p_du->GetMinLimitValue(), pad_type, pOutKernel);
			}

			if (!ret){
				return false;
			}
		}
	}
	return true;
}



