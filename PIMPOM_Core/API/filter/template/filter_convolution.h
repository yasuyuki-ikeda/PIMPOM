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
//�R���{�����[�V����
/*******************************************************************/
#pragma once

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "global_define.h"

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
// �e���v���[�g�őS�Ă̌^�̉摜�ɑΉ��ł���悤�ɂ���
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/********************************************************************
�@  �\  ��  �� : �R���{�����[�V����
��    ��    �� : filter_convolution
��          �� :
��    ��    �l : ��������� true
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA, class DATA2>
bool	filter_convolution(
	DATA			*p_src_data,	//(in)���͉摜�z��̐擪�|�C���^
	DATA2			*p_dst_data,	//(out)�o�͉摜�z��̐擪�|�C���^
	SIZE			image_size,		//(in)�摜�T�C�Y
	SIZE			kernel_size	,	//(in)�t�B���^�T�C�Y(��Őݒ肷�邱��)
	float			*pKernel,		//(in)�J�[�l��(kernel_size.cx * kernel_size.cy�̔z��)�̐擪�|�C���^
	RECT			area,			//(in)�����̈�
	float			offset,			//(in)�l�̃I�t�Z�b�g
	float			contrast,		//(in)�R���g���X�g
	float			data_max,		//(in)�f�[�^����l
	float			data_min,		//(in)�f�[�^�����l
	int				pad_type		//(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
)
{
	
	float	sum, val;
	DATA	*pbuffer, padval;
	int		buffer_w, buffer_h;

	//�͂ݏo�������l���������͉摜�o�b�t�@��p��

	buffer_w = image_size.cx + kernel_size.cx;
	buffer_h = image_size.cy + kernel_size.cy;
	pbuffer = new DATA[buffer_w * buffer_h];
	if (pbuffer == NULL)	return false;

	memset(pbuffer, 0, buffer_w * buffer_h * sizeof(DATA));


	for (int y = area.top - kernel_size.cy / 2; y < area.top; y++) 
	{//��
		for (int x = area.left - kernel_size.cx / 2; x < area.left; x++)
		{//��
			if (pad_type == 1) {//�O���̉�f������
				padval = *(p_src_data + area.left + area.top*image_size.cx);
			}
			else if (pad_type == 2) {//�O���Ő܂�Ԃ�
				padval = *(p_src_data + (area.left - x) + (area.top-y)*image_size.cx);
			}
			else {
				padval = 0;
			}
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = padval;
		}

		for (int x = area.left ; x <= area.right; x++)
		{//����
			if (pad_type == 1) {//�O���̉�f������
				padval = *(p_src_data + x + area.top*image_size.cx);
			}
			else if (pad_type == 2) {//�O���Ő܂�Ԃ�
				padval = *(p_src_data + x + (area.top - y)*image_size.cx);
			}
			else {
				padval = 0;
			}
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = padval;
		}

		for (int x = area.right+1; x <= area.right + kernel_size.cx / 2 ; x++)
		{//�E
			if (pad_type == 1) {//�O���̉�f������
				padval = *(p_src_data + area.right + area.top*image_size.cx);
			}
			else if (pad_type == 2) {//�O���Ő܂�Ԃ�
				padval = *(p_src_data + (2 * area.right + 1 - x) + (area.top - y)*image_size.cx);
			}
			else {
				padval = 0;
			}
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = padval;
		}
	}


	for (int y = area.top; y <= area.bottom; y++)
	{//����
		for (int x = area.left - kernel_size.cx / 2; x < area.left; x++)
		{//��
			if (pad_type == 1) {//�O���̉�f������
				padval = *(p_src_data + area.left + y*image_size.cx);
			}
			else if (pad_type == 2) {//�O���Ő܂�Ԃ�
				padval = *(p_src_data + (area.left-x) + y*image_size.cx);
			}
			else {
				padval = 0;
			}
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = padval;
		}

		for (int x = area.left; x <= area.right; x++)
		{//����
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = *(p_src_data + x + y*image_size.cx);
		}

		for (int x = area.right + 1; x <= area.right + kernel_size.cx / 2; x++)
		{//�E
			if (pad_type == 1) {//�O���̉�f������
				padval = *(p_src_data + area.right + y*image_size.cx);
			}
			else if (pad_type == 2) {//�O���Ő܂�Ԃ�
				padval = *(p_src_data + (2*area.right + 1 - x) + y*image_size.cx);
			}
			else {
				padval = 0;
			}
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = padval;
		}
	}

	for (int y = area.bottom +1; y <= area.bottom + kernel_size.cy / 2; y++)
	{//��
		for (int x = area.left - kernel_size.cx / 2; x < area.left; x++)
		{//��
			if (pad_type == 1) {//�O���̉�f������
				padval = *(p_src_data + area.left + area.bottom*image_size.cx);
			}
			else if (pad_type == 2) {//�O���Ő܂�Ԃ�
				padval = *(p_src_data + (area.left - x) + (2*area.bottom + 1 - y)*image_size.cx);
			}
			else {
				padval = 0;
			}
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = padval;
		}

		for (int x = area.left; x <= area.right; x++)
		{//����
			if (pad_type == 1) {//�O���̉�f������
				padval = *(p_src_data + x + area.bottom*image_size.cx);
			}
			else if (pad_type == 2) {//�O���Ő܂�Ԃ�
				padval = *(p_src_data + x + (2 * area.bottom + 1 - y)*image_size.cx);
			}
			else {
				padval = 0;
			}
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = padval;
		}

		for (int x = area.right + 1; x <= area.right + kernel_size.cx / 2; x++)
		{//�E
			if (pad_type == 1) {//�O���̉�f������
				padval = *(p_src_data + area.right + area.bottom*image_size.cx);
			}
			else if (pad_type == 2) {//�O���Ő܂�Ԃ�
				padval = *(p_src_data + (2 * area.right + 1 - x) + (2 * area.bottom + 1 - y)*image_size.cx);
			}
			else {
				padval = 0;
			}
			*(pbuffer + x + kernel_size.cx / 2 + (y + kernel_size.cy / 2)*buffer_w) = padval;
		}
	}



	memset(p_dst_data, 0, image_size.cx * image_size.cy * sizeof(DATA2));//�o�̓f�[�^������



	//��݂���

	for (int y = area.top ; y <= area.bottom ; y++)
	{
		for (int x = area.left ; x <= area.right ; x++)
		{
			sum = 0;
			for (int ky = -kernel_size.cy / 2; ky <= kernel_size.cy / 2; ky++)
			{
				for (int kx = -kernel_size.cx / 2; kx <= kernel_size.cx / 2; kx++)
				{
					sum += (float)*(pbuffer + (x + kx + kernel_size.cx / 2) + (y + ky + kernel_size.cy / 2)*buffer_w) *pKernel[kernel_size.cx*(ky + kernel_size.cy / 2) + kx + kernel_size.cx / 2];
				}
			}


			//�e�f�[�^�t�H�[�}�b�g�̍ő�E�ŏ��𒴂��Ȃ��悤�ɑΉ�
			val = (float)sum*contrast + offset;

			if (val < data_min)		val = data_min;
			else if (val > data_max)	val = data_max;

			*(p_dst_data + x + y*image_size.cx) = (DATA2)(val);
		}
	}

	delete[]	pbuffer;

	return true;
}



/********************************************************************
�@  �\  ��  �� : �R���{�����[�V����
��    ��    �� : filter_convolution_overwrite
��          �� :
��    ��    �l : ��������� true
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
bool	filter_convolution_overwrite(
	DATA			*p_src_data,	//(in/out)�Ώۉ摜�z��̐擪�|�C���^
	SIZE			image_size,		//(in)�摜�T�C�Y
	SIZE			kernel_size,	//(in)�t�B���^�T�C�Y(��Őݒ肷�邱��)
	float			*pKernel,		//(in)�J�[�l��(kernel_size.cx * kernel_size.cy�̔z��)�̐擪�|�C���^
	RECT			area,			//(in)�����̈�
	float			offset,			//(in)�l�̃I�t�Z�b�g
	float			contrast,		//(in)�R���g���X�g
	float			data_max,		//(in)�f�[�^����l
	float			data_min,		//(in)�f�[�^�����l
	int				pad_type		//(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
	)
{
	bool	ret = true;
	DATA	*p_buffer = NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//�o�b�t�@�������m��
	if (!p_buffer)	return  false;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//�o�b�t�@������

	//���s
	ret = filter_convolution(p_src_data, p_buffer, image_size, kernel_size, pKernel, area, offset, contrast, data_max, data_min, pad_type);

	//�o�b�t�@�̒l���摜�������ɖ߂�
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;

}