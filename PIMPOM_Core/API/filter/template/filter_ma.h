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
//�������t�B���^
/*******************************************************************/
#pragma once

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
//#include "PIMPOM_API.h"

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
//�e���v���[�g�Ō^�Ȃ�(�S�Ă̌^�̉摜�ɑΉ�����)�֐�������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : �ړ�����
��    ��    �� : filter_smooth
��          �� : 
��    ��    �l : ��������� true
�@          �\ :  �ړ����ςɂ�镽����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
bool	filter_smooth(
					  DATA		*p_src_data,	//(in)���͉摜�z��̐擪�|�C���^
					  DATA		*p_dst_data,	//(out)�o�͉摜�z��̐擪�|�C���^
					  SIZE		image_size,		//(in)�摜�T�C�Y
					  SIZE		filter_size,	//(in)�t�B���^�T�C�Y
					  RECT		work_area		//(in)�����̈�
					  )
{
	float	sum;
	int		cnt;


	for(int j = work_area.top ; j<=work_area.bottom ; j++){
		for(int i = work_area.left ; i<=work_area.right ; i++){

			sum = 0;
			cnt = 0;
			for(int k=-filter_size.cy/2 ; k<=filter_size.cy/2 ; k++){
				for(int l=-filter_size.cx/2 ; l<=filter_size.cx/2 ; l++){
					if( (i+l)<0 || (i+l)>=image_size.cx || (j+k)<0 || (j+k)>=image_size.cy )	continue;

					sum += (float)*(p_src_data + (i+l) + (j+k)*image_size.cx);
					cnt++;
				}
			}
			sum /= (float)cnt;
			*(p_dst_data + i + j*image_size.cx) = (DATA)sum;
		}
	}

	return true;
}

/********************************************************************
�@  �\  ��  �� : �ő�E�ŏ������ړ�����
��    ��    �� : filter_smooth_m
��          �� : 
��    ��    �l : ��������� true
�@          �\ :  �ő�E�ŏ������ړ����ςɂ�镽�����@�@
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
bool	filter_smooth_m(
					  DATA		*p_src_data,	//(in)���͉摜�z��̐擪�|�C���^
					  DATA		*p_dst_data,	//(out)�o�͉摜�z��̐擪�|�C���^
					  SIZE		image_size,		//(in)�摜�T�C�Y
					  SIZE		filter_size,	//(in)�t�B���^�T�C�Y
					  RECT		work_area		//(in)�����̈�
						)
{
	float	sum, val, max, min;
	int		cnt;


	for(int j = work_area.top ; j<=work_area.bottom ; j++)
	{
		for(int i = work_area.left ; i<=work_area.right ; i++)
		{
			sum = 0;
			max = min = (float)*(p_src_data + i + j*image_size.cx);
			cnt = 0;

			for(int k=-filter_size.cy/2 ; k<=filter_size.cy/2 ; k++)
			{
				for(int l=-filter_size.cx/2 ; l<=filter_size.cx/2 ; l++)
				{
					if( (i+l)<0 || (i+l)>=image_size.cx || (j+k)<0 || (j+k)>=image_size.cy )	continue;

					val = (float)*(p_src_data + (i+l) + (j+k)*image_size.cx);
						if( val > max ){
							max = val;
						}else if( val < min ){
							min = val;
						}
					sum += val;
					cnt++;
				}
			
			}
			sum -= (max + min);
			sum /= (float)(cnt-2);
			*(p_dst_data + i + j*image_size.cx) = (DATA)sum;
		}
	}

	return true;
}

/********************************************************************
�@  �\  ��  �� : �ړ�����
��    ��    �� : filter_smooth
��          �� : 
��    ��    �l : ��������� true
�@          �\ :  �ړ����ςɂ�镽����
�@�@�@�@�@�@�@�@�@ ���ʂ����摜�̃�������ɏ㏑������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
bool	filter_smooth_overwrite(
								DATA	*p_src_data,	//(in/out)�Ώۉ摜�z��̐擪�|�C���^
								SIZE	image_size,		//(in)�摜�T�C�Y
								SIZE	filter_size,	//(in)�t�B���^�T�C�Y
								RECT	work_area,		//(in)�����̈�
								bool	rem_min_max		//(in)�ő�ŏ����������邩�ǂ���
								)
{
	bool	ret=true;
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//�o�b�t�@�������m��
	if(!p_buffer)	return false;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//�o�b�t�@������

	//���s
	if( rem_min_max ){
		ret=filter_smooth_m(p_src_data,	p_buffer, image_size, filter_size, work_area);
	}else{
		ret=filter_smooth(p_src_data,	p_buffer, image_size, filter_size, work_area);
	}

	//�o�b�t�@�̒l���摜�������ɖ߂�
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;
}

