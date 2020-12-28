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
//Sobel̨��
/*******************************************************************/
#pragma once

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
//#include "PIMPOM_API.h"
//#include "filter.h"

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

//�t�B���^�J�[�l��
 float sobelx3[] = {-1, 0, 1,
					-2, 0, 2,
					-1, 0, 1 };


 float sobelx5[] = {-1, -1,  0, 1,  1,
					-4, -6,  0, 6,  4,
					-5, -10, 0, 10, 5,
					-4, -6,  0, 6,  4,
					-1, -1,  0, 1,  1 };


 float sobelx7[] = { -1,  -2,  -2, 0,  2,  2, 1,
					 -7, -14, -14, 0, 14, 14, 7,
					-12, -32, -29, 0, 29, 32, 12,
					-16, -40, -38, 0, 38, 40, 16,
					-12, -32, -29, 0, 29, 32, 12,
					 -7, -14, -14, 0, 14, 14, 7,
 					 -1,  -2,  -2, 0,  2,  2, 1 };


 float sobelx9[] = { -1,  -4,  -6,  -5,  0,   5,   6,   4,   1,
					-11, -29, -47, -36,  0,  36,  47,  29,  11,
					-25, -75,-117, -93,  0,  93, 117,  75,  25,
					-45,-131,-201,-160,  0, 160, 201, 131,  45,
					-50,-155,-234,-189,  0, 189, 234, 155,  50,
					-45,-131,-201,-160,  0, 160, 201, 131,  45,
					-25, -75,-117, -93,  0,  93, 117,  75,  25,
					-11, -29, -47, -36,  0,  36,  47,  29,  11,
					 -1,  -4,  -6,  -5,  0,   5,   6,   4,   1 };


// *************************************
//         �O    ��    ��    ��         
// *************************************

// *************************************
//         ��    ��    �Q    ��         
// *************************************



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�e���v���[�g�őS�Ă̌^�̉摜�ɑΉ��ł���悤�ɂ���
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� :  �\�[�x���t�B���^
��    ��    �� : filter_edge_sobel
��          �� : 
��    ��    �l : ��������� true
�@          �\ : �G�b�W���o
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA, class DATA2>
bool	filter_edge_sobel(
						  DATA *p_src_data,			//(in)���͉摜�z��̐擪�|�C���^
						  DATA2 *p_dst_data,		//(out)�o�͉摜�z��̐擪�|�C���^
						  SIZE image_size,			//(in)�摜�T�C�Y
						  int filter_size,			//(in)�t�B���^�T�C�Y  3x3,5x5,7x7,9x9
						  int direction,			//(in)�t�B���^���� 0:���̂� 1:�c�̂�  ����ȊO:����
						  RECT work_area,			//(in)�����̈�
						  float min_thresh,			//(in)
						  float data_max_limit,		//(in)���̓f�[�^�̏���l
						  float data_min_limit		//(in)���̓f�[�^�̉����l
						  )
{
	float	val, sum_x, sum_y, edge, scale;
	float  kernel_x[10][10];
	CRect	area;
	DATA	*p;


	//�J�[�l���쐬
	for(int k=0 ; k<filter_size ; k++)
	{
		for(int l=0 ; l<filter_size ; l++)
		{
			int addr = l + k*filter_size;
			if(filter_size == 3){//3x3
				kernel_x[k][l] = sobelx3[addr];
				scale = 1.0;
			}else if(filter_size == 5){//5x5
				kernel_x[k][l] = sobelx5[addr];
				scale = 4.0;
			}else if(filter_size == 7){//7x7
				kernel_x[k][l] = sobelx7[addr];
				scale = 32.0;
			}else if(filter_size == 9){//9x9
				kernel_x[k][l] = sobelx9[addr];
				scale = 256.0;
			}else{
				return false;
			}	
		}
	}

	//�o�͏�����
	for(int j = work_area.top ; j<=work_area.bottom ; j++)
	{
		for(int i = work_area.left ; i<=work_area.right ; i++)
		{
			*(p_dst_data + i + j*image_size.cx)=0;
		}
	}


	//�t�B���^����
	for(int j = work_area.top+filter_size/2 ; j<=work_area.bottom-filter_size/2 ; j++)
	{
		for(int i = work_area.left+filter_size/2 ; i<=work_area.right-filter_size/2 ; i++)
		{

			sum_x = 0;
			sum_y = 0;
			p = p_src_data + i + j*image_size.cx;


			p -= filter_size/2 + image_size.cx*(filter_size/2);
			for(int k=-filter_size/2 ; k<=filter_size/2 ; k++)
			{
				for(int l=-filter_size/2 ; l<=filter_size/2 ; l++)
				{
					val = (float)*p;
					sum_x += val * kernel_x[k+filter_size/2][l+filter_size/2];
					sum_y += val * kernel_x[l+filter_size/2][k+filter_size/2];
					p++;
				}
				p += image_size.cx-filter_size;
			}

			if(direction==1){//���̂�
				edge = 2 * fabs(sum_x) / scale;
			}else if(direction==2){//�c�̂�
				edge = 2 * fabs(sum_y) /scale;
			}else{//����
				edge = (fabs(sum_x) + fabs(sum_y))/scale;
			}

			if(edge < min_thresh){
				edge = 0;
			}

			//�e�f�[�^�t�H�[�}�b�g�̍ő�E�ŏ��𒴂��Ȃ��悤�ɑΉ�
			if(edge < data_min_limit )		edge = data_min_limit;
			else if(edge > data_max_limit  )	edge = data_max_limit ;
		

			*(p_dst_data + i + j*image_size.cx) = (DATA2)edge;
		}
	}

	return true;
}


/********************************************************************
�@  �\  ��  �� :  �\�[�x���t�B���^
��    ��    �� : filter_edge_sobel_overwrite
��          �� : 
��    ��    �l : ��������� true
�@          �\ : �G�b�W���o
                 ���̃�������ɏ㏑������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
bool	filter_edge_sobel_overwrite(
									DATA	*p_src_data,		//(in/out)�Ώۉ摜�z��̐擪�|�C���^
									SIZE	image_size,			//(in)�摜�T�C�Y
									int		filter_size,		//(in)�t�B���^�T�C�Y
									int		direction,			//(in)�t�B���^���� 0:���̂� 1:�c�̂�  ����ȊO:����
									RECT	work_area,			//(in)�����̈�
									float	min_thresh,			//(in)
									float	data_max_limit,		//(in)�f�[�^����l
									float	data_min_limit		//(in)�f�[�^�����l
									)
{
	bool ret=true;
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//�o�b�t�@�������m��
	if(!p_buffer)	return false;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//�o�b�t�@������

	//���s
	ret = filter_edge_sobel(p_src_data,	p_buffer, image_size, filter_size, direction, work_area, min_thresh, data_max_limit, data_min_limit);

	//�o�b�t�@�̒l���摜�������ɖ߂�
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;


	return ret;
}


