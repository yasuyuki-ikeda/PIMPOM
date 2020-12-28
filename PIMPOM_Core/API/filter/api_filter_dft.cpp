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
//DFT
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

// rearrange the quadrants of Fourier image  so that the origin is at the image center
//	  +----+----+    +----+----+
//	  |    |    |    |    |    |
//	  | q0 | q1 |    | q3 | q2 |
//	  |    |    |    |    |    |
//	  +----+----+ -> +----+----+
//	  |    |    |    |    |    |
//	  | q2 | q3 |    | q1 | q0 |
//	  |    |    |    |    |    |
//	  +----+----+    +----+----+

void	swap_spectrun(
	cv::Mat &cmpMat
	)
{
	int cx = cmpMat.cols / 2;
	int cy = cmpMat.rows / 2;

	Mat q0(cmpMat, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(cmpMat, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(cmpMat, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(cmpMat, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);
}



//DFT
template <class DATA>
void	filter_dft(
	DATA			*p_src_data,	//(in)���͉摜�z��̐擪�|�C���^
	float			*p_dst_data,	//(out)�o�͉摜�z��(conplex)�̐擪�|�C���^
	CSize			image_size,		//(in)�摜�T�C�Y
	CRect			area			//(in)�����̈�
	)
{
	cv::Mat srcMat, srcCmpMat, dstCmpMat, power, phase;
	cv::Size dftSize;
	
	dftSize.width = area.Width();
	dftSize.height = area.Height();

	// Mat�ɕϊ�
	srcMat = cv::Mat(dftSize, CV_32F, cv::Scalar::all(0));

	for (int y = 0; y < dftSize.height; y++){
		for (int x = 0; x < dftSize.width; x++){
			srcMat.at<float>(y, x) = (float)p_src_data[(area.left + x) + (area.top + y)*image_size.cx];
		}
	}

	//���f���ɕϊ�
	cv::Mat planes[] = { Mat_<float>(srcMat), Mat::zeros(srcMat.size(), CV_32F) };
	cv::merge(planes, 2, srcCmpMat);

	//dft���s
	cv::dft(srcCmpMat, dstCmpMat, 0);

	//�X�y�N�g��������ׂȂ���
	swap_spectrun(dstCmpMat);

	//�����Ƌ����ɕ���
	cv::split(dstCmpMat, planes);

	//�p���[�X�y�N�g�����v�Z
	cv::magnitude(planes[0], planes[1], power); 

	//�ʑ��v�Z
	cv::phase(planes[0], planes[1], phase);

	//���ʊi�[
	for (int y = 0; y < dftSize.height; y++){
		for (int x = 0; x < dftSize.width; x++){
			p_dst_data[(area.left + x) + (area.top + y)*image_size.cx] = planes[0].at<float>(y, x);
			p_dst_data[(area.left + x) + (area.top + y)*image_size.cx + image_size.cx*image_size.cy] = planes[1].at<float>(y, x);
			p_dst_data[(area.left + x) + (area.top + y)*image_size.cx + image_size.cx*image_size.cy*2] = power.at<float>(y, x);
			p_dst_data[(area.left + x) + (area.top + y)*image_size.cx + image_size.cx*image_size.cy * 3] = phase.at<float>(y, x);
		}
	}
}

//inverse DFT
void	filter_idft(
	float			*p_src_data,	//(in)���͉摜�z��(conplex)�̐擪�|�C���^
	float			*p_dst_data,	//(out)�o�͉摜�z��(conplex)�̐擪�|�C���^
	CSize			image_size,		//(in)�摜�T�C�Y
	CRect			area			//(in)�����̈�
	)
{
	cv::Mat srcCmpMat, dstCmpMat, power, phase;
	cv::Size dftSize;

	// DFT �ϊ��̃T�C�Y���v�Z
	dftSize.width = area.Width();
	dftSize.height = area.Height();

	// Mat�ɕϊ�
	srcCmpMat = cv::Mat(dftSize, CV_32FC2, cv::Scalar::all(0));

	for (int y = 0; y < dftSize.height; y++){
		for (int x = 0; x < dftSize.width; x++){
			srcCmpMat.at<cv::Vec2f>(y, x)[0] = p_src_data[(area.left + x) + (area.top + y)*image_size.cx];
			srcCmpMat.at<cv::Vec2f>(y, x)[1] = p_src_data[(area.left + x) + (area.top + y)*image_size.cx + image_size.cx*image_size.cy];
		}
	}

	//�X�y�N�g��������ׂȂ���
	swap_spectrun(srcCmpMat);

	//dft���s
	cv::dft(srcCmpMat, dstCmpMat, DFT_INVERSE);


	//�����Ƌ����ɕ���
	cv::Mat planes[] = { Mat_<float>(dstCmpMat), Mat::zeros(dstCmpMat.size(), CV_32F) };
	cv::split(dstCmpMat, planes);

	//�p���[�X�y�N�g�����v�Z
	cv::magnitude(planes[0], planes[1], power);

	//�ʑ��v�Z
	cv::phase(planes[0], planes[1], phase);

	for (int y = 0; y < dftSize.height; y++){
		for (int x = 0; x < dftSize.width; x++){
			p_dst_data[(area.left + x) + (area.top + y)*image_size.cx] = planes[0].at<float>(y, x);
			p_dst_data[(area.left + x) + (area.top + y)*image_size.cx + image_size.cx*image_size.cy] = planes[1].at<float>(y, x);
			p_dst_data[(area.left + x) + (area.top + y)*image_size.cx + image_size.cx*image_size.cy * 2] = power.at<float>(y, x);
			p_dst_data[(area.left + x) + (area.top + y)*image_size.cx + image_size.cx*image_size.cy * 3] = phase.at<float>(y, x);
		}
	}
}


/********************************************************************
�@  �\  ��  �� : ���U�t�[���G�ϊ�
��    ��    �� : FilterDFT
��          �� : long		image_num			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long		dst_data_number		(in)�i�[��摜�������ԍ�
				 bool		inverse				(in)true�ɂ���Ƌt�t�[���G�ϊ�
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool  CPimpomAPI::FilterDFT(long image_num, long dst_data_number, bool inverse)
{

	if(image_num == CURRENT_IMAGE){
		image_num = GetCurrentImageNumber();
	}else if(image_num >= GetDataUnitNumber() || image_num<0){
		return false;
	}

	if (dst_data_number >= GetDataUnitNumber() || dst_data_number < 0){
		return false;
	}

	if(dst_data_number == image_num)
	{
		MessageBox("src and dst must be different number");
		return false;
	}


	CDataUnit *psrcdu = GetDataUnit(image_num);
	if (!psrcdu)
	{
		return false;
	}

	CRect workarea = psrcdu->GetRectArea();
	CSize srcsize = psrcdu->DataSize;


	if (!inverse)
	{
		if (psrcdu->DataType == BYTE_FORMAT || psrcdu->DataType == FLOAT_FORMAT || psrcdu->DataType == SHORT16_FORMAT || psrcdu->DataType == WORD_FORMAT ||
			psrcdu->DataType == LONG32_FORMAT || psrcdu->DataType == DWORD_FORMAT)
		{
			if (psrcdu->DataType == BYTE_FORMAT)
			{
				filter_dft(psrcdu->pByteData, GetComplexMemory(dst_data_number, srcsize, true), srcsize, workarea);
			}
			else if (psrcdu->DataType == FLOAT_FORMAT)
			{
				filter_dft(psrcdu->pFloatData, GetComplexMemory(dst_data_number, srcsize, true), srcsize, workarea);
			}
			else if (psrcdu->DataType == SHORT16_FORMAT)
			{
				filter_dft(psrcdu->pShortData, GetComplexMemory(dst_data_number, srcsize, true), srcsize, workarea);
			}
			else if (psrcdu->DataType == WORD_FORMAT)
			{
				filter_dft(psrcdu->pWordData, GetComplexMemory(dst_data_number, srcsize, true), srcsize, workarea);
			}
			else if (psrcdu->DataType == LONG32_FORMAT)
			{
				filter_dft(psrcdu->pLongData, GetComplexMemory(dst_data_number, srcsize, true), srcsize, workarea);
			}
			else if (psrcdu->DataType == DWORD_FORMAT)
			{
				filter_dft(psrcdu->pDwordData, GetComplexMemory(dst_data_number, srcsize, true), srcsize, workarea);
			}

			CopyDataUnit(image_num, dst_data_number, false, false, true, false, false, false);//��Ɨ̈���R�s�[
			SetDispChannel(dst_data_number, 2);//�p���[�X�y�N�g����\������
			DrawImage(dst_data_number);//
			SetDispRangeAuto(dst_data_number, 1);//�p���[�X�y�N�g�������₷������
		}

		else
		{
			MessageBox("this function is supported for BYTE/SHORT/LONG/WORD/DWORD/FLOAT format");
			return false;
		}
	}
	else
	{
		if (psrcdu->DataType == COMPLEX_FORMAT)
		{
			filter_idft(psrcdu->pFloatData, GetComplexMemory(dst_data_number, srcsize, true), srcsize, workarea);
			CopyDataUnit(image_num, dst_data_number, false, false, true, false, false, false);
			DrawImage(dst_data_number);
			SetDispRangeAuto(dst_data_number, 1);
		}
		else{
			MessageBox("this function is supported  for only COMPLEX format on inverse mode");
			return false;
		}
	}
	
	return true;
}


/********************************************************************
�@  �\  ��  �� : ���U�t�[���G�ϊ�
��    ��    �� : calc_dft
��          �� : 
				float*		psrc				(in)
				float*		pPower				(out)
				float*		pPhase				(out)
				int			length				(in)�f�[�^��
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool  CPimpomAPI::calc_dft(float *pSrc, int length, float *pPower, float *pPhase )
{
	cv::Mat srcMat, srcCmpMat, dstCmpMat, power, phase;
	cv::Size dftSize;

	dftSize.width = length;
	dftSize.height = 1;

	// Mat�ɕϊ�
	srcMat = cv::Mat(dftSize, CV_32F, cv::Scalar::all(0));

	for (int i = 0; i < length; i++){
		srcMat.at<float>(0, i) = pSrc[i] * (0.54 - 0.46*cos(2 * PI* (float)i/ (float)length));//hamming window
	}

	//���f���ɕϊ�
	cv::Mat planes[] = { Mat_<float>(srcMat), Mat::zeros(srcMat.size(), CV_32F) };
	cv::merge(planes, 2, srcCmpMat);

	//dft���s
	cv::dft(srcCmpMat, dstCmpMat, 0);

	//�X�y�N�g��������ׂȂ���
	swap_spectrun(dstCmpMat);

	//�����Ƌ����ɕ���
	cv::split(dstCmpMat, planes);

	//�p���[�X�y�N�g�����v�Z
	cv::magnitude(planes[0], planes[1], power);
	for (int i = 0; i < length; i++){
		pPower [i] = power.at<float>(0, i);
	}

	if (pPhase)
	{
		//�ʑ��v�Z
		cv::phase(planes[0], planes[1], phase);
		for (int i = 0; i < length; i++){
			pPhase[i] = phase.at<float>(0, i);
		}
	}

	return true;
}