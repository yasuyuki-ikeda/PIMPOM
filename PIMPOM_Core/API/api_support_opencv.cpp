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
//OpenCVとPIMPOMの間を取りもつ
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//テンプレートで型なし(全ての型の画像に対応する)関数をつくる
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//CDataUnitからcv::Matにコピー
template <class DATA>
void	copy_image_to_mat(CDataUnit *p_du, DATA *p_src, void *pI)
{
	cv::Mat *pImage = (cv::Mat*)pI;

	//コピー実行
	int width = pImage->size().width;
	int height = pImage->size().height;
	int wstep = (int)pImage->step / sizeof(DATA);
	int channels = (int)pImage->channels();
	DATA *p_dst = (DATA*)pImage->ptr();
	for(int c=0 ; c<channels; c++)
	{
		for(int j=0 ; j<height; j++)
		{
			for(int i=0; i<width; i++)
			{
				int	mat_addr = i* channels + j * wstep + c;
				int img_addr = i + j * p_du->DataSize.cx + (channels -1-c) * p_du->DataSize.cy * p_du->DataSize.cx;

				*(p_dst + mat_addr) = *(p_src + img_addr);
			}
		}
	}
}


//IplImageからCDataUnitにコピー
template <class DATA>
void	copy_mat_to_image(void *pI, CDataUnit *p_du, DATA *p_dst, bool copyInArea)
{
	cv::Mat *pImage = (cv::Mat*)pI;

	CRect area;
	
	if(copyInArea){
		area = p_du->GetRectArea();
	}else{
		area = CRect(0,0,p_du->DataSize.cx-1, p_du->DataSize.cy-1);
	}

	//コピー実行
	int wstep = (int)pImage->step / sizeof(DATA);
	int channels = (int)pImage->channels();

	if (p_du->ChannelNumber == channels)//チャネル数が同じならばすべてコピー
	{
		for (int c = 0; c < channels; c++)
		{
			for (int j = area.top; j <= area.bottom; j++)
			{
				for (int i = area.left; i <= area.right; i++)
				{
					DATA *p_src = (DATA*)pImage->ptr();
					int	mat_addr = (i - area.left)* channels + (j - area.top) * wstep + c;
					int img_addr = i + j * p_du->DataSize.cx + ((channels - 1 - c) + p_du->DispPage*p_du->ChannelNumber) * p_du->DataSize.cy * p_du->DataSize.cx;


					*(p_dst + img_addr) = *(p_src + mat_addr);
				}
			}
		}
	}
	else
	{//チャネル数が異なる場合は、最初のチャネルのみ、現在表示中の画像上にコピー

		for (int j = area.top; j <= area.bottom; j++)
		{
			for (int i = area.left; i <= area.right; i++)
			{
				DATA *p_src = (DATA*)pImage->ptr();
				int	mat_addr = (i - area.left)* channels + (j - area.top) * wstep ;
				int img_addr = i + j * p_du->DataSize.cx + (p_du->DispChannel + p_du->DispPage*p_du->ChannelNumber) * p_du->DataSize.cy * p_du->DataSize.cx;

				*(p_dst + img_addr) = *(p_src + mat_addr);
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//OpenCVの画像作成
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : 画像メモリの内容をcv::Matに展開する
関    数    名 : ConvertToCvImage2
引          数 : long		image_num			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 void		*pSrcMat			(out)cv::Matの先頭ポインタ
				 bool		*pClippedMat		(out)計測領域でクリップしたcv::Matの先頭ポインタ
戻    り    値 : 成功すれば true
機          能 : 
				　
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::ConvertToCvImage2(long image_num, void *pSrcMat, void *pClippedMat)
{
	cv::Mat *pm = (cv::Mat*)ConvertToCvImage(image_num);
	if(!pm)	return false;

	if (pClippedMat) 
	{
		//計測領域（矩形でクリップ）
		cv::Mat *pmc = (cv::Mat*)pClippedMat;
		CRect calc_area = GetDataUnit(image_num)->GetRectArea();//処理対象の矩形範囲
		cv::Rect roi(calc_area.left, calc_area.top, calc_area.Width()+1, calc_area.Height()+1);//ROI
		*pmc = (*pm)(roi);//ROIでクリップ
	}
	*((cv::Mat*)pSrcMat) = *pm;

	delete pm;

	return true;
}

bool	CPimpomAPI::Img2Mat(long image_num, void *pSrcMat, void *pClippedMat)
{
	return ConvertToCvImage2(image_num, pSrcMat, pClippedMat);
}

/********************************************************************
機  能  名  称 : cv::Matのメモリを取得し、指定された画像メモリの内容をコピーする
関    数    名 : ConvertToCvImage
引          数 : long		image_num			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
戻    り    値 : 成功すればcv::Mat型のポインタ、失敗すればNULL
機          能 : 
				　
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void*	CPimpomAPI::ConvertToCvImage(long image_num)
{
	CDataUnit	*p_du;
	cv::Mat *pImage;
	int width, height;

	if( (p_du=GetDataUnit(image_num)) == NULL )	return NULL;

	width = p_du->DataSize.cx;
	height = p_du->DataSize.cy;

	//コピー実行
	switch(p_du->DataType)
	{
		case BYTE_FORMAT:
			//not break
		case THREE_D_FORMAT:
			pImage = new cv::Mat(height, width, CV_8UC1);
			if(pImage){//現在表示中の画像のみの1チャネル画像とする
				copy_image_to_mat(p_du, p_du->pByteData + p_du->DispPage * p_du->ChannelNumber * p_du->DataSize.cx * p_du->DataSize.cy, pImage);
			}
			break;
		case RGB_FORMAT:
			//not break
		case RGB_3D_FORMAT:
			pImage = new cv::Mat(height, width, CV_8UC3);
			if (pImage) {//現在表示中の画像のみの3チャネル画像とする
				copy_image_to_mat(p_du, p_du->pByteData + p_du->DispChannel * p_du->ChannelNumber * p_du->DataSize.cx * p_du->DataSize.cy, pImage);
			}
			break;

		case SHORT16_FORMAT:
			pImage = new cv::Mat(height, width, CV_16SC1);
			if (pImage) {
				copy_image_to_mat(p_du, p_du->pShortData + p_du->DispPage * p_du->ChannelNumber * p_du->DataSize.cx * p_du->DataSize.cy, pImage);
			}
			break;

		case WORD_FORMAT:
			pImage = new cv::Mat(height, width, CV_16UC1);
			if (pImage) {
				copy_image_to_mat(p_du, p_du->pWordData + p_du->DispPage * p_du->ChannelNumber * p_du->DataSize.cx * p_du->DataSize.cy, pImage);
			}
			break;

		case FLOAT_FORMAT:
			pImage = new cv::Mat(height, width, CV_32FC1);
			if (pImage) {
				copy_image_to_mat(p_du, p_du->pFloatData + p_du->DispPage * p_du->ChannelNumber * p_du->DataSize.cx * p_du->DataSize.cy, pImage);
			}
			break;

		case FLOAT_3D_FORMAT:
			pImage = new cv::Mat(height, width, CV_32FC1);
			if (pImage) {//現在表示中の画像のみの1チャネル画像とする
				copy_image_to_mat(p_du, p_du->pFloatData + p_du->DispPage * p_du->ChannelNumber * p_du->DataSize.cx * p_du->DataSize.cy, pImage);
			}
			break;

		case XYZ_FORMAT:
			pImage = new cv::Mat(height, width, CV_32FC3);
			if (pImage) {//3チャネル画像とする
				copy_image_to_mat(p_du, p_du->pFloatData + p_du->DispChannel * p_du->ChannelNumber * p_du->DataSize.cx * p_du->DataSize.cy, pImage);
			}
			break;

		case XY_FORMAT:
			pImage = new cv::Mat(height, width, CV_32FC2);
			if (pImage) {//2チャネル画像とする
				copy_image_to_mat(p_du, p_du->pFloatData + p_du->DispChannel * p_du->ChannelNumber * p_du->DataSize.cx * p_du->DataSize.cy, pImage);
			}
			break;

		case COMPLEX_FORMAT:
			pImage = new cv::Mat(height, width, CV_32FC4);
			if (pImage) {//4チャネル画像とする
				copy_image_to_mat(p_du, p_du->pFloatData + p_du->DispChannel * p_du->ChannelNumber * p_du->DataSize.cx * p_du->DataSize.cy, pImage);
			}
			break;
	}

	return (void*)pImage;
}


/********************************************************************
機  能  名  称 : cv::Matの内容を画像メモリに展開する
関    数    名 : ConvertFromCvImage2
引          数 : long		image_num			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 void		*pMat				(out)cv::Matの先頭ポインタ
				 long		src_image_num		(in)ベースとなる画像メモリへのポインタ
戻    り    値 : 成功すれば true
機          能 : 
				　
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::ConvertFromCvImage2(long image_num, void *pMat, long src_image_num)
{
	return ConvertFromCvImage(image_num, pMat, src_image_num);
}

bool	CPimpomAPI::Mat2Img(long image_num, void *pMat, long src_image_num)
{
	return ConvertFromCvImage(image_num, pMat, src_image_num);
}


/********************************************************************
機  能  名  称 : IplImageの内容を画像メモリに展開する
関    数    名 : ConvertFromCvImage
引          数 : long		image_num			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 void		*pMat				(out)cv::Matの先頭ポインタ
				 long		src_image_num		(in)ベースとなる画像メモリへのポインタ
戻    り    値 : 成功すれば true
機          能 : 
				　
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::ConvertFromCvImage(long image_num, void *pMat, long src_image_num)
{
	if(!pMat)	return false;

	cv::Mat *pImage = (cv::Mat *)pMat;

	CDataUnit *pSrcDu = NULL;


	CSize	size(pImage->size().width, pImage->size().height);//展開先画像メモリサイズ
	bool	copyInArea=false;//画像メモリの作業領域内にコピーするかどうか

	if(src_image_num>=0 && src_image_num<GetDataUnitNumber())
	{
		pSrcDu = GetDataUnit(src_image_num);
		if(pSrcDu)
		{
			CRect area = pSrcDu->GetRectArea();
			if(pImage->size().width == area.Width() && pImage->size().height == area.Height())
			{
				//IplImageがpSrcDuの作業領域と同じサイズの場合は作業領域内部のみコピーする
				size = pSrcDu->DataSize;
				copyInArea = true;
				CopyDataUnit(src_image_num, image_num);//ベース画像をコピーする
			}
		}
	}




	//コピー実行
	switch (pImage->type())
	{
	case CV_8UC3:
	
			if (pSrcDu == NULL || pSrcDu->DataType == RGB_FORMAT)//8bit 3ch -> RGBへコピー
			{
				BYTE *data = GetRGBMemory(image_num, size, false);
				if (!data)	return false;
				copy_mat_to_image(pImage, GetDataUnit(image_num), data, copyInArea);
				return true;
			}
			else if (pSrcDu->DataType == RGB_3D_FORMAT)//8bit 3ch -> RGB3Dの現在表示中のページへコピー
			{
				BYTE *data = GetRGB3DMemory(image_num, size, pSrcDu->PageNumber, false);
				if (!data)	return false;

				GetDataUnit(image_num)->DispPage = pSrcDu->DispPage;

				copy_mat_to_image(pImage, GetDataUnit(image_num), data, copyInArea);
				return true;
			}
			else{
				return false;
			}

			break;

	case CV_8UC1:

			if (pSrcDu == NULL || pSrcDu->DataType == BYTE_FORMAT)//8bit 3ch以外 ->  最初chのみBYTEへコピー
			{
				BYTE *data = GetByteMemory(image_num, size, false);
				if (!data)	return false;
				copy_mat_to_image(pImage, GetDataUnit(image_num), data, copyInArea);
				return true;
			}
			else if (pSrcDu->DataType == THREE_D_FORMAT)//8bit 3ch以外　-> 最初chのみ3Dの現在表示中のページへコピー
			{
				BYTE *data = Get3DMemory(image_num, size, pSrcDu->PageNumber, false);
				if (!data)	return false;

				GetDataUnit(image_num)->DispPage = pSrcDu->DispPage;

				copy_mat_to_image(pImage, GetDataUnit(image_num), data, copyInArea);
				return true;
			}
			else{
				return false;
			}


		break;

	case CV_16SC1:
		if (pSrcDu == NULL || pSrcDu->DataType == SHORT16_FORMAT)//符号付 16bit -> 最初chのみSHORTへコピー
		{
			short *data = GetShortMemory(image_num, size, false);
			if (!data)	return false;
			copy_mat_to_image(pImage, GetDataUnit(image_num), data, copyInArea);
			return true;
		}
		else{
			return false;
		}
		break;

	case CV_16UC1:
		if (pSrcDu == NULL || pSrcDu->DataType == WORD_FORMAT)//符号なし 16bit -> 最初chのみWORDへコピー
		{
			WORD *data = GetWordMemory(image_num, size, false);
			if (!data)	return false;
			copy_mat_to_image(pImage, GetDataUnit(image_num), data, copyInArea);
			return true;
		}
		else{
			return false;
		}
		break;


	case CV_32FC1:
		if (pSrcDu == NULL || pSrcDu->DataType == FLOAT_FORMAT)//浮動小数点
		{
			float *data = GetFloatMemory(image_num, size, false);
			if (!data)	return false;
			copy_mat_to_image(pImage, GetDataUnit(image_num), data, copyInArea);
			return true;
		}
		else if (pSrcDu->DataType == FLOAT_3D_FORMAT)
		{
			float *data = GetF3DMemory(image_num, size, pSrcDu->PageNumber, false);
			if (!data)	return false;

			GetDataUnit(image_num)->DispPage = pSrcDu->DispPage;

			copy_mat_to_image(pImage, GetDataUnit(image_num), data, copyInArea);
			return true;
		}
		else {
			return false;
		}
		break;

	case CV_32FC2:
		if (pSrcDu == NULL || pSrcDu->DataType == XY_FORMAT)
		{
			float *data = GetXYMemory(image_num, size, false);
			if (!data)	return false;

			copy_mat_to_image(pImage, GetDataUnit(image_num), data, copyInArea);
		}
		else {
			return false;
		}
		break;

	case CV_32FC3:
		if (pSrcDu == NULL || pSrcDu->DataType == XYZ_FORMAT)
		{
			float *data = GetXYZMemory(image_num, size, false);
			if (!data)	return false;

			copy_mat_to_image(pImage, GetDataUnit(image_num), data, copyInArea);
		}
		else {
			return false;
		}
		break;

	case CV_32FC4:
		if (pSrcDu == NULL || pSrcDu->DataType == COMPLEX_FORMAT)
		{
			float *data = GetComplexMemory(image_num, size, false);
			if (!data)	return false;

			copy_mat_to_image(pImage, GetDataUnit(image_num), data, copyInArea);
		}
		else{
			return false;
		}

	default:
		return false;
	}

	return false;
}
