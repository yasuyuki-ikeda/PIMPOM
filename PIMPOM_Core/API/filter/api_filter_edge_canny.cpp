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
//Cannyﾌｨﾙﾀ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"

#include "opencv_headers.h"
#include "PIMPOM_API.h"



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
機  能  名  称 :  Cannyフィルタ
関    数    名 : FilterEdgeCanny
引          数 : long		image_num			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long		dst_data_number		(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
				 int		filter_size			(in)フィルタサイズ ( 3: 3x3,  5:5x5,  7:7x7 )
				 float		thresh				(in)
				 float		thresh_weak			(in)
				 bool		toMultiImage		(in)複数のページ／チャネルに同じ処理をするかどうか
戻    り    値 : 成功すれば true
機          能 : フィルタ処理結果は、同画像メモリに上書きされる

				　
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool  CPimpomAPI::FilterEdgeCanny(long image_num, long dst_data_number, int filter_size, float thresh, float thresh_weak, bool toMultiImage)
{
	bool		ret = true;
	CDataUnit	*p_du;
	long		imsize;
	CRect		calc_area;



	//引数チェック
	if (filter_size != 3 && filter_size != 5 && filter_size != 7)	return false;

	if (image_num == CURRENT_IMAGE){
		image_num = GetCurrentImageNumber();
	}
	else if (image_num >= GetDataUnitNumber() || image_num < 0){
		return false;
	}

	if (dst_data_number == CURRENT_IMAGE){
		dst_data_number = GetCurrentImageNumber();
	}
	else if (dst_data_number >= GetDataUnitNumber() || dst_data_number < 0){
		return false;
	}




	//結果を別の画像メモリに入れる場合、まず画像データをコピーする
	if (image_num != dst_data_number){
		if (!CopyDataUnit(image_num, dst_data_number)){
			return false;
		}
	}

	p_du = GetDataUnit(dst_data_number);
	if (!p_du)	return	false;

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


	int prevDispPage = p_du->DispPage;
	int prevDispChannel = p_du->DispChannel;
	for (int p = pS; p <= pE; p++)
	{
		for (int c = cS; c <= cE; c++)
		{
			if (p_du->pByteData)
			{//8bit画像
				if (p_du->ChannelNumber == 1)
				{
					cv::Mat srcMat, subSrcMat, subDstMat;


					GetDataUnit(image_num)->DispPage = p;
					GetDataUnit(image_num)->DispChannel = c;
					p_du->DispPage = p;
					p_du->DispChannel = c;

					if (!ConvertToCvImage2(image_num, (void*)&srcMat, (void*)&subSrcMat))//画像メモリ⇒cv::Mat
					{
						return false;
					}

					cv::Canny(subSrcMat, subDstMat, thresh_weak, thresh, filter_size);//cannyフィルタ実行

					if (!ConvertFromCvImage2(dst_data_number, (void*)&subDstMat, image_num))//cv::Mat⇒画像メモリ
					{
						return false;
					}
				}
				else{
					MessageBox("this function is supported only for BYTE/3D format");
					return false;
				}
			}
			else if (p_du->pShortData){//16bit画像
				MessageBox("this function is supported only for BYTE/3D format");
				return false;
			}
			else if (p_du->pWordData){//16bit画像
				MessageBox("this function is supported only for BYTE/3D format");
				return false;
			}
			else if (p_du->pLongData){//32bit画像
				MessageBox("this function is supported only for BYTE/3D format");
				return false;
			}
			else if (p_du->pDwordData){//32bit画像
				MessageBox("this function is supported only for BYTE/3D format");
				return false;
			}
			else if (p_du->pFloatData){//実数画像
				MessageBox("this function is supported only for BYTE/3D format");
				return false;
			}
		}
	}

	GetDataUnit(image_num)->DispPage = prevDispPage;
	GetDataUnit(image_num)->DispChannel = prevDispChannel;
	p_du->DispPage = prevDispPage;
	p_du->DispChannel = prevDispChannel;

	return true;
}


