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
//画像合成
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
機  能  名  称 : ポワソン画像合成
関    数    名 : BlendPoisson
引          数 : long		sorce_image_num			(in)元画像メモリ番号(-1にした場合は現在選択中の画像)
long		blend_image_num			(in)ブレンド画像メモリ番号
long		dst_image_num			(in)結果格納先画像メモリ番号(-1にした場合は現在選択中の画像)
int			offsetx					(in)合成開始位置x
int			offsety					(in)合成開始位置y
int			mode					(in)合成方法 0:NORMAL_CLONE,  1:MIXED_CLONE  2:MONOCHROME_TRANSFER

戻    り    値 : 成功すれば true
機          能 : フィルタ処理結果は、同画像メモリに上書きされる

日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool  CPimpomAPI::BlendPoisson(long sorce_image_num, long blend_image_num, long dst_image_num, int offsetx, int offsety, int mode)
{
	if (sorce_image_num == blend_image_num)	return false;

	CDataUnit *pSrcDu = GetDataUnit(sorce_image_num);
	CDataUnit *pBlendDu = GetDataUnit(blend_image_num);

	if (!pSrcDu || !pBlendDu)	return false;
	if (mode<0 || mode >= 3)	return false;

	if (pSrcDu->DataType != BYTE_FORMAT && pSrcDu->DataType != RGB_FORMAT) {
		MessageBox("this function is supported only for BYTE/RGB format");
		return false;
	}

	if (pSrcDu->DataType != pBlendDu->DataType) {
		MessageBox("format of blend image must be same as sorce image");
		return false;
	}


	//元画像およびブレンド画像をopencvのフォーマットに変換
	cv::Mat srcMat, subSrcMat;
	cv::Mat blendMat, subBlendMat;
	cv::Mat subMaskMat;
	cv::Mat destMat;

	if (!ConvertToCvImage2(sorce_image_num, (void*)&srcMat, (void*)&subSrcMat))	return false;

	if (!ConvertToCvImage2(blend_image_num, (void*)&blendMat, (void*)&subBlendMat))	return false;


	//ブレンド画像のマスクをopencvのフォーマットに変換
	CRect maskRect = pBlendDu->GetRectArea();
	subMaskMat = cv::Mat::zeros(cv::Size(subBlendMat.cols, subBlendMat.rows), CV_8U);

	int cnt = 0;
	for (int j = 0; j < subBlendMat.rows; j++)
	{
		for (int i = 0; i < subBlendMat.cols; i++)
		{
			if (pBlendDu->pMaskData[maskRect.left + i + (maskRect.top + j)*pBlendDu->DataSize.cx] != MASKED_PIXEL)
			{
				subMaskMat.data[cnt] = 255;
			}
			cnt++;
		}
	}


	if (subSrcMat.channels() == 1)
		cv::cvtColor(subSrcMat, subSrcMat, cv::COLOR_GRAY2BGR);


	if (subBlendMat.channels() == 1)
		cv::cvtColor(subBlendMat, subBlendMat, cv::COLOR_GRAY2BGR);


	try
	{
		//subBlendMatのコピー領域　4隅
		int left, top, right, bottom;
		left = (offsetx < 0) ? -offsetx : 0;
		top = (offsety < 0) ? -offsety : 0;
		right = (offsetx + subBlendMat.cols > subSrcMat.cols) ? subSrcMat.cols - offsetx : subBlendMat.cols;
		bottom = (offsety + subBlendMat.rows > subSrcMat.rows) ? subSrcMat.rows - offsety : subBlendMat.rows;

		//subBlendMatのコピー領域
		cv::Rect roi;
		roi.x = left;
		roi.y = top;
		roi.width = right - left;
		roi.height = bottom - top;

		//クリップ
		cv::Mat subBlendMatClip = subBlendMat(roi);
		cv::Mat subMaskMatClip = subMaskMat(roi);
		int offsetxclip = (offsetx < 0) ? 0 : offsetx;
		int offsetyclip = (offsety < 0) ? 0 : offsety;


		//poisson合成
		Point p;
		p.x = offsetxclip + subBlendMatClip.cols / 2 - 1;
		p.y = offsetyclip + subBlendMatClip.rows / 2 - 1;
		cv::seamlessClone(subBlendMatClip, subSrcMat, subMaskMatClip, p, destMat, mode + 1);

	}
	catch (cv::Exception e)
	{
		return false;
	}


	//結果をpimpomのフォーマットに変換
	if (!ConvertFromCvImage2(dst_image_num, (void*)&destMat, sorce_image_num))	return false;

	return true;
}