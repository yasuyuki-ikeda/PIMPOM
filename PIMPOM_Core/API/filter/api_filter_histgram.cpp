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
//�q�X�g�O�������p�t�B���^
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
�@  �\  ��  �� : CLAHE (Contrast Limited Adaptive Histogram Equalization)
��    ��    �� : FilterCLAHE
��          �� : long		image_num			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long		dst_data_number		(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 double		clipLimit			(in)�x������l
				 int		tileW				(in)������������
				 int		tileH				(in)�c����������
				 bool		toMultiImage		(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
��    ��    �l : ��������� true
�@          �\ : �t�B���^�������ʂ́A���摜�������ɏ㏑�������
				https://en.wikipedia.org/wiki/Adaptive_histogram_equalization

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool  CPimpomAPI::FilterCLAHE(long image_num, long dst_data_number, double clipLimit, int tileW, int tileH, bool toMultiImage )
{
	bool		ret=true;
	CDataUnit	*p_du;
	long		imsize;
	CRect		calc_area;


	//�����`�F�b�N

	if (clipLimit<1) {
		return false;
	}

	if(image_num == CURRENT_IMAGE){
		image_num = GetCurrentImageNumber();
	}else if(image_num >= GetDataUnitNumber() || image_num<0){
		return false;
	}

	if(dst_data_number == CURRENT_IMAGE){
		dst_data_number = GetCurrentImageNumber();
	}else if(dst_data_number >= GetDataUnitNumber() || dst_data_number <0){
		return false;
	}

	if (GetDataUnit(image_num)){
		if (GetDataUnit(image_num)->DataSize.cx <= tileW || GetDataUnit(image_num)->DataSize.cy <= tileH || tileW <= 0 || tileW <= 0)
		{//�t�B���^�̃T�C�Y�`�F�b�N
			return false;
		}
	}
	else{
		return false;
	}



	//���ʂ�ʂ̉摜�������ɓ����ꍇ�A�܂��摜�f�[�^���R�s�[����
	if(image_num!=dst_data_number){
		if(!CopyDataUnit(image_num, dst_data_number)){
			return false;
		}
	}

	p_du = GetDataUnit(dst_data_number);
		if(!p_du)	return	false;

	imsize = p_du->DataSize.cx * p_du->DataSize.cy;//�摜�̃T�C�Y�i��f���j
	calc_area = p_du->GetRectArea();//�����Ώۂ̋�`�͈�



	int cS, cE;//�����J�n�E�I���`���l��
	int pS, pE;//�����J�n�E�I���y�[�W
	if (toMultiImage){//�����摜�ɓ����������s���ꍇ
		pS = cS = 0;
		pE = p_du->PageNumber - 1;
		cE = p_du->ChannelNumber - 1;
	}
	else{//���ݕ\�����̉摜�ɂ̂ݏ������s���ꍇ
		pS = pE = p_du->DispPage;
		cS = cE = p_du->DispChannel;
	}


	int prevDispPage = p_du->DispPage;
	int prevDispChannel = p_du->DispChannel;

	Ptr<CLAHE> clahe = cv::createCLAHE(clipLimit, cv::Size(tileW, tileH));//�t�B���^�쐬

	for (int p = pS; p <= pE; p++)
	{
		for (int c = cS; c <= cE; c++)
		{
			if (p_du->pByteData)
			{//8bit�摜
				if (p_du->ChannelNumber == 1)
				{
					cv::Mat srcMat, subSrcMat, subDstMat;

					GetDataUnit(image_num)->DispPage = p;
					GetDataUnit(image_num)->DispPage = c;
					p_du->DispPage = p;
					p_du->DispChannel = c;

					if (!ConvertToCvImage2(image_num, (void*)&srcMat, (void*)&subSrcMat))//�摜��������cv::Mat
					{
						return false;
					}

					clahe->apply(subSrcMat, subDstMat);//�t�B���^���s

					if (!ConvertFromCvImage2(dst_data_number, (void*)&subDstMat, image_num))//cv::Mat�ˉ摜������
					{
						return false;
					}
				}
				else{
					MessageBox("this function is supported only for BYTE/3D format");
					return false;
				}
			}
			else if (p_du->pShortData)
			{//16bit�摜
				MessageBox("this function is supported only for BYTE/3D format");
				return false;
			}
			else if (p_du->pWordData)
			{//16bit�摜
				MessageBox("this function is supported only for BYTE/3D format");
				return false;
			}
			else if (p_du->pLongData)
			{//32bit�摜
				MessageBox("this function is supported only for BYTE/3D format");
				return false;
			}
			else if (p_du->pDwordData)
			{//32bit�摜
				MessageBox("this function is supported only for BYTE/3D format");
				return false;
			}
			else if (p_du->pFloatData)
			{//�����摜
				MessageBox("this function is supported only for BYTE/3D format");
				return false;
			}
			else{
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


