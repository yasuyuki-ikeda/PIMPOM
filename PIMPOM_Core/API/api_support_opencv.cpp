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
//OpenCV��PIMPOM�̊Ԃ�������
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�e���v���[�g�Ō^�Ȃ�(�S�Ă̌^�̉摜�ɑΉ�����)�֐�������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//CDataUnit����cv::Mat�ɃR�s�[
template <class DATA>
void	copy_image_to_mat(CDataUnit *p_du, DATA *p_src, void *pI)
{
	cv::Mat *pImage = (cv::Mat*)pI;

	//�R�s�[���s
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


//IplImage����CDataUnit�ɃR�s�[
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

	//�R�s�[���s
	int wstep = (int)pImage->step / sizeof(DATA);
	int channels = (int)pImage->channels();

	if (p_du->ChannelNumber == channels)//�`���l�����������Ȃ�΂��ׂăR�s�[
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
	{//�`���l�������قȂ�ꍇ�́A�ŏ��̃`���l���̂݁A���ݕ\�����̉摜��ɃR�s�[

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
//OpenCV�̉摜�쐬
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : �摜�������̓��e��cv::Mat�ɓW�J����
��    ��    �� : ConvertToCvImage2
��          �� : long		image_num			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 void		*pSrcMat			(out)cv::Mat�̐擪�|�C���^
				 bool		*pClippedMat		(out)�v���̈�ŃN���b�v����cv::Mat�̐擪�|�C���^
��    ��    �l : ��������� true
�@          �\ : 
				�@
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::ConvertToCvImage2(long image_num, void *pSrcMat, void *pClippedMat)
{
	cv::Mat *pm = (cv::Mat*)ConvertToCvImage(image_num);
	if(!pm)	return false;

	if (pClippedMat) 
	{
		//�v���̈�i��`�ŃN���b�v�j
		cv::Mat *pmc = (cv::Mat*)pClippedMat;
		CRect calc_area = GetDataUnit(image_num)->GetRectArea();//�����Ώۂ̋�`�͈�
		cv::Rect roi(calc_area.left, calc_area.top, calc_area.Width()+1, calc_area.Height()+1);//ROI
		*pmc = (*pm)(roi);//ROI�ŃN���b�v
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
�@  �\  ��  �� : cv::Mat�̃��������擾���A�w�肳�ꂽ�摜�������̓��e���R�s�[����
��    ��    �� : ConvertToCvImage
��          �� : long		image_num			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
��    ��    �l : ���������cv::Mat�^�̃|�C���^�A���s�����NULL
�@          �\ : 
				�@
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void*	CPimpomAPI::ConvertToCvImage(long image_num)
{
	CDataUnit	*p_du;
	cv::Mat *pImage;
	int width, height;

	if( (p_du=GetDataUnit(image_num)) == NULL )	return NULL;

	width = p_du->DataSize.cx;
	height = p_du->DataSize.cy;

	//�R�s�[���s
	switch(p_du->DataType)
	{
		case BYTE_FORMAT:
			//not break
		case THREE_D_FORMAT:
			pImage = new cv::Mat(height, width, CV_8UC1);
			if(pImage){//���ݕ\�����̉摜�݂̂�1�`���l���摜�Ƃ���
				copy_image_to_mat(p_du, p_du->pByteData + p_du->DispPage * p_du->ChannelNumber * p_du->DataSize.cx * p_du->DataSize.cy, pImage);
			}
			break;
		case RGB_FORMAT:
			//not break
		case RGB_3D_FORMAT:
			pImage = new cv::Mat(height, width, CV_8UC3);
			if (pImage) {//���ݕ\�����̉摜�݂̂�3�`���l���摜�Ƃ���
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
			if (pImage) {//���ݕ\�����̉摜�݂̂�1�`���l���摜�Ƃ���
				copy_image_to_mat(p_du, p_du->pFloatData + p_du->DispPage * p_du->ChannelNumber * p_du->DataSize.cx * p_du->DataSize.cy, pImage);
			}
			break;

		case XYZ_FORMAT:
			pImage = new cv::Mat(height, width, CV_32FC3);
			if (pImage) {//3�`���l���摜�Ƃ���
				copy_image_to_mat(p_du, p_du->pFloatData + p_du->DispChannel * p_du->ChannelNumber * p_du->DataSize.cx * p_du->DataSize.cy, pImage);
			}
			break;

		case XY_FORMAT:
			pImage = new cv::Mat(height, width, CV_32FC2);
			if (pImage) {//2�`���l���摜�Ƃ���
				copy_image_to_mat(p_du, p_du->pFloatData + p_du->DispChannel * p_du->ChannelNumber * p_du->DataSize.cx * p_du->DataSize.cy, pImage);
			}
			break;

		case COMPLEX_FORMAT:
			pImage = new cv::Mat(height, width, CV_32FC4);
			if (pImage) {//4�`���l���摜�Ƃ���
				copy_image_to_mat(p_du, p_du->pFloatData + p_du->DispChannel * p_du->ChannelNumber * p_du->DataSize.cx * p_du->DataSize.cy, pImage);
			}
			break;
	}

	return (void*)pImage;
}


/********************************************************************
�@  �\  ��  �� : cv::Mat�̓��e���摜�������ɓW�J����
��    ��    �� : ConvertFromCvImage2
��          �� : long		image_num			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 void		*pMat				(out)cv::Mat�̐擪�|�C���^
				 long		src_image_num		(in)�x�[�X�ƂȂ�摜�������ւ̃|�C���^
��    ��    �l : ��������� true
�@          �\ : 
				�@
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
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
�@  �\  ��  �� : IplImage�̓��e���摜�������ɓW�J����
��    ��    �� : ConvertFromCvImage
��          �� : long		image_num			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 void		*pMat				(out)cv::Mat�̐擪�|�C���^
				 long		src_image_num		(in)�x�[�X�ƂȂ�摜�������ւ̃|�C���^
��    ��    �l : ��������� true
�@          �\ : 
				�@
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::ConvertFromCvImage(long image_num, void *pMat, long src_image_num)
{
	if(!pMat)	return false;

	cv::Mat *pImage = (cv::Mat *)pMat;

	CDataUnit *pSrcDu = NULL;


	CSize	size(pImage->size().width, pImage->size().height);//�W�J��摜�������T�C�Y
	bool	copyInArea=false;//�摜�������̍�Ɨ̈���ɃR�s�[���邩�ǂ���

	if(src_image_num>=0 && src_image_num<GetDataUnitNumber())
	{
		pSrcDu = GetDataUnit(src_image_num);
		if(pSrcDu)
		{
			CRect area = pSrcDu->GetRectArea();
			if(pImage->size().width == area.Width() && pImage->size().height == area.Height())
			{
				//IplImage��pSrcDu�̍�Ɨ̈�Ɠ����T�C�Y�̏ꍇ�͍�Ɨ̈�����̂݃R�s�[����
				size = pSrcDu->DataSize;
				copyInArea = true;
				CopyDataUnit(src_image_num, image_num);//�x�[�X�摜���R�s�[����
			}
		}
	}




	//�R�s�[���s
	switch (pImage->type())
	{
	case CV_8UC3:
	
			if (pSrcDu == NULL || pSrcDu->DataType == RGB_FORMAT)//8bit 3ch -> RGB�փR�s�[
			{
				BYTE *data = GetRGBMemory(image_num, size, false);
				if (!data)	return false;
				copy_mat_to_image(pImage, GetDataUnit(image_num), data, copyInArea);
				return true;
			}
			else if (pSrcDu->DataType == RGB_3D_FORMAT)//8bit 3ch -> RGB3D�̌��ݕ\�����̃y�[�W�փR�s�[
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

			if (pSrcDu == NULL || pSrcDu->DataType == BYTE_FORMAT)//8bit 3ch�ȊO ->  �ŏ�ch�̂�BYTE�փR�s�[
			{
				BYTE *data = GetByteMemory(image_num, size, false);
				if (!data)	return false;
				copy_mat_to_image(pImage, GetDataUnit(image_num), data, copyInArea);
				return true;
			}
			else if (pSrcDu->DataType == THREE_D_FORMAT)//8bit 3ch�ȊO�@-> �ŏ�ch�̂�3D�̌��ݕ\�����̃y�[�W�փR�s�[
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
		if (pSrcDu == NULL || pSrcDu->DataType == SHORT16_FORMAT)//�����t 16bit -> �ŏ�ch�̂�SHORT�փR�s�[
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
		if (pSrcDu == NULL || pSrcDu->DataType == WORD_FORMAT)//�����Ȃ� 16bit -> �ŏ�ch�̂�WORD�փR�s�[
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
		if (pSrcDu == NULL || pSrcDu->DataType == FLOAT_FORMAT)//���������_
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
