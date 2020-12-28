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
//�V�O���C�h�t�B���^
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
�@  �\  ��  �� : �V�O���C�h�t�B���^
��    ��    �� : filter_sigmoid
��          �� : 
��    ��    �l : ��������� true
�@          �\ : �摜�f�[�^�ɃV�O���C�h�֐����g�����G�b�W���o�t�B���^��������

           �J�[�l���F
                 xt = cos(�΃�/180)x + sin(�΃�/180)y
�@�@�@�@�@�@�@�@ Sig(x,y) = 1 / ( 1 + exp( -k * tx ) ) -0.5

		 [�Z�x] =  conv{I(x,y) , Gabor(x,y)} * contrast + offset
                 (I(x,y)�͌��摜�̔Z�x�Aconv{a,b}��a��b�̏�ݍ���)

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA, class DATA2>
bool	filter_sigmoid(DATA			*p_src_data,	//(in)���͉摜�z��̐擪�|�C���^
					 DATA2			*p_dst_data,	//(out)�o�͉摜�z��̐擪�|�C���^
					 SIZE			image_size,		//(in)�摜�T�C�Y
					 int			filter_size,	//(in)�t�B���^�T�C�Y  ��l�̂ݓ��͉�
					 float			k,				//(in)�p�����^k
					 float			theta,			//(in)�ƁF�J�[�l����]�p�x
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

	if(k==0)	return false;

	memset(p_dst_data, 0, image_size.cx * image_size.cy * sizeof(DATA2) );//�o�̓f�[�^������


	float cos_t, sin_t;
	cos_t = cos(theta/180.0*3.141592);
	sin_t = sin(theta/180.0*3.141592);

	//�J�[�l���̍쐬
    for (int j = 0; j < filter_size; j++) 
	{
		for (int i = 0; i < filter_size; i++) 
		{
			double x = (i - filter_size/2);
			double y = (j - filter_size/2);
			double xt = cos_t*x + sin_t*y;
			//double yt = -sin_t*x + cos_t*y;
			kernel[j*filter_size+i] = 1 / ( 1 + exp(- k*xt)) - 0.5 ;
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
�@  �\  ��  �� : �V�O���C�h�t�B���^
��    ��    �� : filter_sigmoid_overwrite
��          �� : 
��    ��    �l : ��������� true
�@          �\ : �摜�f�[�^�ɃV�O���C�h�t�B���^�t�B���^��������
                 ���ʂ����摜�̃�������ɏ㏑������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
bool	filter_sigmoid_overwrite(
					 DATA			*p_src_data,	//(in/out)�Ώۉ摜�z��̐擪�|�C���^
					 SIZE			image_size,		//(in)�摜�T�C�Y
					 int			filter_size,	//(in)�t�B���^�T�C�Y  ��l�̂ݓ��͉\
					 float			k,				//(in)
					 float			theta,			//(in)�ƁF�J�[�l����]�p�x
					 RECT			work_area,		//(in)�����̈�
					 float			offset,			//(in)�l�̃I�t�Z�b�g
					 float			contrast,		//(in)�R���g���X�g
					 float			data_max,		//(in)�f�[�^����l
					 float			data_min,		//(in)�f�[�^�����l
					int				pad_type,		//(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
					 float			*pOutKernel		//(out)�J�[�l���p�����^(NULL��)
						)
{
	bool	ret=true;
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//�o�b�t�@�������m��
	if(!p_buffer)	return false;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//�o�b�t�@������

	//���s
	ret = filter_sigmoid(p_src_data,	p_buffer, image_size, filter_size, k, theta, work_area, offset, contrast, data_max, data_min, pad_type, pOutKernel);

	//�o�b�t�@�̒l���摜�������ɖ߂�
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;
}
