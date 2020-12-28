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
//�摜����
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "opencv_headers.h"
#include "PIMPOM_API.h"

// *************************************
//         �}  �N  ��   ��   �`         
// *************************************

// *************************************
//         �O    ��    ��    ��         
// *************************************

// *************************************
//         �\  ��  ��   ��   �`         
// *************************************

// *************************************
//         ��    �I    ��    ��         
// *************************************

// *************************************
//         �O    ��    ��    ��         
// *************************************

// *************************************
//         ��    ��    �Q    ��         
// *************************************

/********************************************************************
�@  �\  ��  �� : �|���\���摜����
��    ��    �� : BlendPoisson
��          �� : long		sorce_image_num			(in)���摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
long		blend_image_num			(in)�u�����h�摜�������ԍ�
long		dst_image_num			(in)���ʊi�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
int			offsetx					(in)�����J�n�ʒux
int			offsety					(in)�����J�n�ʒuy
int			mode					(in)�������@ 0:NORMAL_CLONE,  1:MIXED_CLONE  2:MONOCHROME_TRANSFER

��    ��    �l : ��������� true
�@          �\ : �t�B���^�������ʂ́A���摜�������ɏ㏑�������

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
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


	//���摜����уu�����h�摜��opencv�̃t�H�[�}�b�g�ɕϊ�
	cv::Mat srcMat, subSrcMat;
	cv::Mat blendMat, subBlendMat;
	cv::Mat subMaskMat;
	cv::Mat destMat;

	if (!ConvertToCvImage2(sorce_image_num, (void*)&srcMat, (void*)&subSrcMat))	return false;

	if (!ConvertToCvImage2(blend_image_num, (void*)&blendMat, (void*)&subBlendMat))	return false;


	//�u�����h�摜�̃}�X�N��opencv�̃t�H�[�}�b�g�ɕϊ�
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
		//subBlendMat�̃R�s�[�̈�@4��
		int left, top, right, bottom;
		left = (offsetx < 0) ? -offsetx : 0;
		top = (offsety < 0) ? -offsety : 0;
		right = (offsetx + subBlendMat.cols > subSrcMat.cols) ? subSrcMat.cols - offsetx : subBlendMat.cols;
		bottom = (offsety + subBlendMat.rows > subSrcMat.rows) ? subSrcMat.rows - offsety : subBlendMat.rows;

		//subBlendMat�̃R�s�[�̈�
		cv::Rect roi;
		roi.x = left;
		roi.y = top;
		roi.width = right - left;
		roi.height = bottom - top;

		//�N���b�v
		cv::Mat subBlendMatClip = subBlendMat(roi);
		cv::Mat subMaskMatClip = subMaskMat(roi);
		int offsetxclip = (offsetx < 0) ? 0 : offsetx;
		int offsetyclip = (offsety < 0) ? 0 : offsety;


		//poisson����
		Point p;
		p.x = offsetxclip + subBlendMatClip.cols / 2 - 1;
		p.y = offsetyclip + subBlendMatClip.rows / 2 - 1;
		cv::seamlessClone(subBlendMatClip, subSrcMat, subMaskMatClip, p, destMat, mode + 1);

	}
	catch (cv::Exception e)
	{
		return false;
	}


	//���ʂ�pimpom�̃t�H�[�}�b�g�ɕϊ�
	if (!ConvertFromCvImage2(dst_image_num, (void*)&destMat, sorce_image_num))	return false;

	return true;
}