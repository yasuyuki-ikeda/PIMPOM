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
//�K�{�[���t�B���^
/*******************************************************************/
#pragma once

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "filter_convolution.h"

// *************************************
//         �}  �N  ��   ��   �`         
// *************************************
#define		BIGGER(A,B)			( ((A)>(B)) ? (A):(B) )
#define		SMALLER(A,B)		( ((A)<(B)) ? (A):(B) )

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
// �e���v���[�g�őS�Ă̌^�̉摜�ɑΉ��ł���悤�ɂ���
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : �K�{�[���t�B���^
��    ��    �� : filter_gabor
��          �� : 
��    ��    �l : ��������� true
�@          �\ : �摜�f�[�^�ɃK�{�[���t�B���^��������

           �J�[�l���F
                 xt = cos(�΃�/180)x + sin(�΃�/180)y
                 yt = -sin(�΃�/180)x + cos(�΃�/180)y 
                 ��=(fs-1)/3
                 ��=fs/freq
�@�@�@�@�@�@�@�@ Gabor(x,y) = exp( - (xt^2 + ��^2 yt^2) / 2��^2 ) cos( 2��xt/�� + �΃�/180)

                 �㎮�ŎZ�o��A����0�ɂȂ�悤�ɐ��K��

           [�Z�x] =  conv{I(x,y) , Gabor(x,y)} * contrast + offset
                 (I(x,y)�͌��摜�̔Z�x�Aconv{a,b}��a��b�̏�ݍ���)

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA, class DATA2>
bool	filter_gabor(DATA			*p_src_data,	//(in)���͉摜�z��̐擪�|�C���^
					 DATA2			*p_dst_data,	//(out)�o�͉摜�z��̐擪�|�C���^
					 SIZE			image_size,		//(in)�摜�T�C�Y
					 int			filter_size,	//(in)�t�B���^�T�C�Y  ��l�̂ݓ��͉\
					 float			gamma,			//(in)���F�J�[�l���G����
					 float			freq,			//(in)�J�[�l�����ł̎���
					 float			theta,			//(in)�ƁF�J�[�l����]�p�x
					 float			psi,			//(in)���F�J�[�l�������ʑ�
					 RECT			area,			//(in)�����̈�
					 float			offset,			//(in)�l�̃I�t�Z�b�g
					 float			contrast,		//(in)�R���g���X�g
					 float			data_max,		//(in)�f�[�^����l
					 float			data_min,		//(in)�f�[�^�����l
				 	int				pad_type,		//(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
					 float			*pOutKernel		//(out)�J�[�l���p�����^(NULL��)
					 )
{
	float  kernel[COMB_FILTER_SIZE_MAX*COMB_FILTER_SIZE_MAX]={0};



	if (filter_size % 2 == 0)	return false;//�t�B���^�T�C�Y�͊�̂�

	if(freq<=0)	return false;

	memset(p_dst_data, 0, image_size.cx * image_size.cy * sizeof(DATA2) );//�o�̓f�[�^������


	//�J�[�l���̍쐬
	float sigma, sigma2, cos_t, sin_t, ip, lambda;
	sigma = (float)(filter_size-1)/6.0;//(�t�B���^�T�C�Y��3�Ё~2+1�ɂȂ�悤�ɂ���)
	sigma2 = 2 * sigma * sigma;
	cos_t = cos(theta/180.0*3.141592);
	sin_t = sin(theta/180.0*3.141592);
	ip = psi/180.0*3.141592;
	lambda = filter_size/freq;


	float ave=0;
    for (int j = 0; j < filter_size; j++) 
	{
		for (int i = 0; i < filter_size; i++) 
		{
			double x = (i - filter_size/2);
			double y = (j - filter_size/2);
			double xt = cos_t*x + sin_t*y;
			double yt = -sin_t*x + cos_t*y;
			kernel[j*filter_size+i] = exp(- (xt*xt+yt*yt*gamma*gamma) / sigma2) * cos(2*3.1415*xt/lambda + ip) ;//  exp( - (xt^2 + ��^2 yt^2) / 2��^2 ) cos( 2��xt/�� + psi )

			ave += kernel[j*filter_size + i];
		}
	}
	ave /= (float)(filter_size*filter_size);

	//�J�[�l�����ς�0�ɂ���
	for (int j = 0; j < filter_size; j++){ 
	    for (int i = 0; i < filter_size; i++) {
			kernel[j*filter_size + i] -= ave;
		}
	}


	if (pOutKernel) {
		memcpy(pOutKernel, kernel, filter_size*filter_size * sizeof(float));
	}



	//��݂���
	SIZE fs;
	fs.cx = filter_size;
	fs.cy = filter_size;
	filter_convolution(p_src_data, p_dst_data, image_size, fs, kernel, area, offset, contrast, data_max, data_min, pad_type);

	return true;
}


/********************************************************************
�@  �\  ��  �� : �K�{�[��)�t�B���^
��    ��    �� : filter_gabor_overwrite
��          �� : 
��    ��    �l : ��������� true
�@          �\ : �摜�f�[�^�ɃK�{�[���t�B���^��������
                 ���ʂ����摜�̃�������ɏ㏑������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
bool	filter_gabor_overwrite(
					 DATA			*p_src_data,	//(in/out)�Ώۉ摜�z��̐擪�|�C���^
					 SIZE			image_size,		//(in)�摜�T�C�Y
					 int			filter_size,	//(in)�t�B���^�T�C�Y  ��l�̂ݓ��͉\
					 float			gamma,			//(in)���F�J�[�l���G����
					 float			freq,			//(in)�J�[�l�����ł̎���
					 float			theta,			//(in)�ƁF�J�[�l����]�p�x
					 float			psi,			//(in)���F�J�[�l�������ʑ�
					 RECT			work_area,		//(in)�����̈�
					 float			offset,			//(in)�l�̃I�t�Z�b�g
					 float			contrast,		//(in)�R���g���X�g
					 float			data_max,		//(in)�f�[�^����l
					 float			data_min,		//(in)�f�[�^�����l
					 int			pad_type,		//(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
					 float			*pOutKernel		//(out)�J�[�l���p�����^(NULL��)
						)
{
	bool	ret=true;
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//�o�b�t�@�������m��
	if(!p_buffer)	return false;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//�o�b�t�@������

	//���s
	ret = filter_gabor(p_src_data,	p_buffer, image_size, filter_size, gamma, freq, theta, psi, work_area, offset, contrast, data_max, data_min, pad_type, pOutKernel);

	//�o�b�t�@�̒l���摜�������ɖ߂�
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;
}
