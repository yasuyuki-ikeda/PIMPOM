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
// ���v���V�A���I�u�K�E�V�A���t�B���^
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
//�e���v���[�g�Ō^�Ȃ�(�S�Ă̌^�̉摜�ɑΉ�����)�֐�������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
template <class DATA>
void	filter_thin_log(DATA *p_src_data, DATA *p_dst_data, CSize image_size, 
				   CSize filter_size, CRect work_area, int min_thresh, double sigma)
{

	int	i, j;
	int	v;
	int	x1, y1, x2, y2;
	int	offs, xs, xs2, xs3;
	unsigned char	*p0, *p1;
	short	*sp;
	short	*p_short_buffer=NULL;

	// �����ݒ�
	p_short_buffer = new short[image_size.cx*image_size.cy];
	if(!p_short_buffer)	return;

	// �v���̈�̐ݒ�
	x1 = BIGGER(work_area.left , 7/2);
	x2 = SMALLER(work_area.right , image_size.cx - 7/2 -1);
	y1 = BIGGER(work_area.top , 7/2);
	y2 = SMALLER(work_area.bottom , image_size.cy - 7/2 -1);


	// �����ݒ�
	xs  = image_size.cx;
	xs2 = xs * 2;
	xs3 = xs * 3;
	p0 = (unsigned char *)(p_src_data + y1 * xs + x1);
	sp = (short *)(p_short_buffer + y1 * xs + x1);
	offs = image_size.cx - (x2 - x1 + 1);

	// �V�~�V�k�n�f�t�B���^����
	for(j = y1; j <= y2; j++, p0 += offs, sp += offs){
		for(i = x1; i <= x2; i++, p0++, sp++){
			v = -((int)*(p0-xs3-1)+(int)*(p0-xs3  )+(int)*(p0-xs3+1)+(int)*(p0-xs -3)+
			      (int)*(p0-xs +3)+(int)*(p0    -3)+(int)*(p0    +3)+(int)*(p0+xs -3)+
			      (int)*(p0+xs +3)+(int)*(p0+xs3-1)+(int)*(p0+xs3  )+(int)*(p0+xs3+1))
			    -((int)*(p0-xs2-2)+(int)*(p0-xs2+2)+(int)*(p0+xs2-2)+(int)*(p0+xs2+2)) * 2
			    -((int)*(p0-xs2-1)+(int)*(p0-xs2  )+(int)*(p0-xs2+1)+(int)*(p0-xs -2)+
			      (int)*(p0-xs +2)+(int)*(p0    -2)+(int)*(p0    +2)+(int)*(p0+xs -2)+
			      (int)*(p0+xs +2)+(int)*(p0+xs2-1)+(int)*(p0+xs2  )+(int)*(p0+xs2+1)) * 4
			    +((int)*(p0-xs   )+(int)*(p0    -1)+(int)*(p0    +1)+(int)*(p0+xs   )) * 9
			    +((int)*(p0      )) * 32;
			*sp = v / 16;
		}
	}

	// �[���N���X�_���o
	sp = (short *)(p_short_buffer + (y1+1) * xs + (x1+1));
	p1 = (unsigned char *)(p_dst_data     + (y1+1) * xs + (x1+1));
	offs = image_size.cx - (x2 - x1 - 1);
	for(j = y1+1; j <= y2-1; j++, sp += offs, p1 += offs){
		for(i = x1+1; i <= x2-1; i++, sp++, p1++){
			v = (int)*(sp      - 1) * (int)*(sp      + 1) +
			    (int)*(sp - xs    ) * (int)*(sp + xs    ) +
			    (int)*(sp - xs - 1) * (int)*(sp + xs + 1) +
			    (int)*(sp - xs + 1) * (int)*(sp + xs - 1);
			if (v / 32 < -min_thresh) {
				*p1 = 255;
			} else {
				*p1 = 0;
			}
		}
	}

	// �Ǘ��_����
	p1 = (unsigned char *)(p_dst_data + (y1+1) * xs + (x1+1));
	offs = image_size.cx - (x2 - x1 - 1);
	for(j = y1+1; j <= y2-1; j++, p1 += offs){
		for(i = x1+1; i <= x2-1; i++, p1++){
			if (*(p1 - xs - 1) == 0 && *(p1 - xs) == 0 && *(p1 - xs + 1) == 0 &&
			    *(p1      - 1) == 0 &&                    *(p1      + 1) == 0 &&
			    *(p1 + xs - 1) == 0 && *(p1 + xs) == 0 && *(p1 + xs + 1) == 0) {
				*p1 = 0;
			}
		}
	}

	// �I������
	delete[]	p_short_buffer;
}
#endif


/********************************************************************
�@  �\  ��  �� : ���v���V�A���I�u�K�E�V�A��(Log)�t�B���^
��    ��    �� : filter_lapcace_gauss_internal
��          �� : 
��    ��    �l : ��������� true
�@          �\ : �摜�f�[�^�Ƀ��v���V�A���I�u�K�E�V�A���t�B���^��������
          �J�[�l���̃[���N���X�Ԋu�̓t�B���^�T�C�Y�̔����Ƃ���
          
          �J�[�l���F
             �� = (fs - 1) / 4��2
             LoG(x,y) = (x^2 + y^2 - 2��^2) exp( - (x^2 + y^2) / 2��^2 )
               
�@�@�@�@�@�@�㎮�ŎZ�o��A�ő�1,����0�ɂȂ�悤�ɐ��K��

�@�@�@�@�@[�Z�x] =  conv{I(x,y) , LoG(x,y)} * contrast + offset
               (I(x,y)�͌��摜�̔Z�x�Aconv{a,b}��a��b�̏�ݍ���)
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA, class DATA2>
bool	filter_lapcace_gauss_internal(
									  DATA		*p_src_data,	//(in)���͉摜�z��̐擪�|�C���^
									  DATA2		*p_dst_data,	//(out)�o�͉摜�z��̐擪�|�C���^
									  SIZE		image_size,		//(in)�摜�T�C�Y
									  int		filter_size,	//(in)�t�B���^�T�C�Y
									  int		direction,		//(in)�t�B���^���� 1:���̂� 2:�c�̂�  ����ȊO:����
									  RECT		area,			//(in)�����̈�
									  float	    offset,			//(in)�l�̃I�t�Z�b�g
									  float	    contrast,		//(in)�R���g���X�g
									  double	data_max,		//(in)�f�[�^����l
									  double	data_min,		//(in)�f�[�^�����l
									 int		pad_type,		//(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
									  float		*pOutKernel		//(out)�J�[�l���p�����^(NULL��)
									  )
{
	float  kernel[COMB_FILTER_SIZE_MAX*COMB_FILTER_SIZE_MAX]={0};
	int filter_size_x, filter_size_y;



	if (filter_size % 2 == 0)	return false;//�t�B���^�T�C�Y�͊�̂�



	//�t�B���^������������
	filter_size_x = filter_size;
	filter_size_y = filter_size;
	if(direction==1){//���̂�
		filter_size_y = 1;
	}else if(direction==2){//�c�̂�
		filter_size_x = 1;
	}


	//LoG�J�[�l���̍쐬(�J�[�l���T�C�Y�̔������[���N���X�Ԋu�Ax,y=0,0�̂Ƃ��ő�l�P)
	float sig, sig2;
	sig = (float)(filter_size-1)/(4.0*sqrt(2.0));
	sig2 = 2 * sig * sig;

	float ave=0;
    for (int j = 0; j < filter_size_y; j++) 
	{
		for (int i = 0; i < filter_size_x; i++) 
		{
			int	addr = i + j*filter_size_x;
			double x = (i - filter_size_x/2);
			double y = (j - filter_size_y/2);
			kernel[addr] = (x*x + y*y - sig2) * exp(- (x*x+y*y) / sig2) ;//  (x^2 + y^2 - 2��^2) exp( - (x^2 + y^2) / 2��^2 )
			kernel[addr] /= -sig2;//�ő傪�P�ɂȂ�悤�ɐ��K��

			ave += kernel[addr];
		}
	}
	ave /= (float)(filter_size_y*filter_size_x);

	//�J�[�l�����ς�0�ɂ���
	for (int j = 0; j < filter_size_y; j++){ 
	    for (int i = 0; i < filter_size_x; i++) {
			kernel[i + j*filter_size_x] -= ave;
		}
	}

	if (pOutKernel) {
		memcpy(pOutKernel, kernel, filter_size_y*filter_size_x * sizeof(float));
	}

	//��݂���
	SIZE fs;
	fs.cx = filter_size_x;
	fs.cy = filter_size_y;
	filter_convolution(p_src_data, p_dst_data, image_size, fs, kernel, area, offset, contrast, data_max, data_min, pad_type);

	return true;
}


/********************************************************************
�@  �\  ��  �� : ���v���V�A���I�u�K�E�V�A��(LoG)�t�B���^
��    ��    �� : filter_lapcace_gauss
��          �� : 
��    ��    �l : ��������� true
�@          �\ :  �摜�f�[�^�Ƀ��v���V�A���I�u�K�E�V�A���t�B���^��������
          �J�[�l���̃[���N���X�Ԋu�̓t�B���^�T�C�Y�̔����Ƃ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA, class DATA2>
bool	filter_lapcace_gauss(
							 DATA		*p_src_data,	//(in)���͉摜�z��̐擪�|�C���^
							 DATA2		*p_dst_data,	//(out)�o�͉摜�z��̐擪�|�C���^
							 SIZE		image_size,		//(in)�摜�T�C�Y
							 int		filter_size,	//(in)�t�B���^�T�C�Y
							 int		direction,		//(in)�t�B���^���� 1:���̂� 2:�c�̂�  ����ȊO:����
							 RECT		work_area,		//(in)�����̈�
							 float	    offset,			//(in)�l�̃I�t�Z�b�g
							 float	    contrast,		//(in)�R���g���X�g
							 double		data_max,		//(in)�f�[�^����l
							 double		data_min,		//(in)�f�[�^�����l
							int			pad_type,		//(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
							 float		*pOutKernel		//(out)�J�[�l���p�����^(NULL��)
							 )
{
	int ret=true;

	if(filter_size%2==0)	return false;//�t�B���^�T�C�Y�͊�̂�


	//���v���V�A���I�u�K�E�V�A�����s
	ret = filter_lapcace_gauss_internal(p_src_data, p_dst_data, image_size, filter_size, direction, work_area, offset, contrast, data_max, data_min, pad_type, pOutKernel);


	return ret;
}



/********************************************************************
�@  �\  ��  �� : LoG�p�����א��G�b�W���o
��    ��    �� : filter_lapcace_gauss
��          �� : 
��    ��    �l : ��������� true
�@          �\ :  LoG�t�B���^��Ƀ[���N���X�����f���G�b�W�Ƃ��Č��o����
                 �J�[�l���̃[���N���X�Ԋu�̓t�B���^�T�C�Y�̔����Ƃ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA, class DATA2>
bool	filter_edge_lapcace_gauss(
								 DATA		*p_src_data,	//(in)���͉摜�z��̐擪�|�C���^
								 DATA2		*p_dst_data,	//(out)�o�͉摜�z��̐擪�|�C���^
								 SIZE		image_size,		//(in)�摜�T�C�Y
								 int		filter_size,	//(in)�t�B���^�T�C�Y
								 int		direction,		//(in)�t�B���^���� 1:���̂� 2:�c�̂�  ����ȊO:����
								 RECT		work_area,		//(in)�����̈�
								 float		min_thresh,		//(in)
								 int		pad_type		//(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
								  )
{
	bool	ret = true;

	if(filter_size%2==0)	return false;//�t�B���^�T�C�Y�͊�̂�


	float *p_buffer = new float[image_size.cx * image_size.cy];//�o�b�t�@�������m��
	if(!p_buffer)	return false;

	memset(p_buffer, image_size.cx * image_size.cy * sizeof(float),0);//�o�b�t�@������



	//���v���V�A���I�u�K�E�V�A�����s
	ret = filter_lapcace_gauss_internal(p_src_data, p_buffer, image_size, filter_size, direction, work_area, 0, 1, FLT_MAX,-FLT_MAX, pad_type, NULL);

	if(ret){
		//�[���N���X���o
		for(int j = work_area.top ; j<=work_area.bottom ; j++){
			for(int i = work_area.left ; i<=work_area.right ; i++){

				*(p_dst_data + i + j*image_size.cx) = 0;//������

				float val = *(p_buffer + i + j*image_size.cx);

				if(i+1<=work_area.right){//������
					float valn = *(p_buffer + i+1 + j*image_size.cx);

					if(val*valn<0 && fabs(val - valn) > min_thresh){
						*(p_dst_data + i + j*image_size.cx) = 255;
					}
				}
					
				if(j+1<=work_area.bottom){//�c����
					float valn = *(p_buffer + i + (j+1)*image_size.cx);

					if(val*valn<0 && fabs(val - valn) > min_thresh){
						*(p_dst_data + i + j*image_size.cx) = 255;
					}
				}
			}
		}
	}

	delete[]	p_buffer;

	return ret;
}


/********************************************************************
�@  �\  ��  �� : ���v���V�A���I�u�K�E�V�A��(LoG)�t�B���^
��    ��    �� : filter_lapcace_gauss_overwrite
��          �� : 
��    ��    �l : ��������� true
�@          �\ : �摜�f�[�^�Ƀ��v���V�A���I�u�K�E�V�A���t�B���^��������
                 ���ʂ����摜�̃�������ɏ㏑������
                  
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
bool	filter_lapcace_gauss_overwrite(
									DATA		*p_src_data,	//(in/out)�Ώۉ摜�z��̐擪�|�C���^
									SIZE		image_size,		//(in)�摜�T�C�Y
									int			filter_size,	//(in)�t�B���^�T�C�Y
									int			direction,		//(in)�t�B���^���� 1:���̂� 2:�c�̂�  ����ȊO:����
									RECT		work_area,		//(in)�����̈�
									float	    offset,			//(in)�l�̃I�t�Z�b�g
									float	    contrast,		//(in)�R���g���X�g
									float		data_max,		//(in)�f�[�^����l
									float		data_min,		//(in)�f�[�^�����l
									int			pad_type,		//(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
									float		*pOutKernel		//(out)�J�[�l���p�����^(NULL��)
									)
{
	bool	ret=true;
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//�o�b�t�@�������m��
	if(!p_buffer)	return false;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//�o�b�t�@������

	//���s
	ret = filter_lapcace_gauss(p_src_data,	p_buffer, image_size, filter_size, direction, work_area, offset, contrast, data_max, data_min, pad_type, pOutKernel);

	//�o�b�t�@�̒l���摜�������ɖ߂�
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;
}


/********************************************************************
�@  �\  ��  �� : LoG�p�����א��G�b�W���o
��    ��    �� : filter_edge_lapcace_gauss_overwrite
��          �� : 
��    ��    �l : ��������� true
�@          �\ :  LoG�t�B���^��Ƀ[���N���X�����f���G�b�W�Ƃ��Č��o����
                  �J�[�l���̃[���N���X�Ԋu�̓t�B���^�T�C�Y�̔����Ƃ���
				  ���ʂ����摜�̃�������ɏ㏑������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
bool	filter_edge_lapcace_gauss_overwrite(
										DATA		*p_src_data,	//(in/out)�Ώۉ摜�z��̐擪�|�C���^
										SIZE		image_size,		//(in)�摜�T�C�Y
										int			filter_size,	//(in)�t�B���^�T�C�Y
										int			direction,		//(in)�t�B���^���� 1:���̂� 2:�c�̂�  ����ȊO:����
										RECT		work_area,		//(in)�����̈�
										float		min_thresh,		//(in)
										int			pad_type		//(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
									  )
{
	bool	ret=true;
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//�o�b�t�@�������m��
	if(!p_buffer)	return false;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//�o�b�t�@������

	//���s
	ret = filter_edge_lapcace_gauss(p_src_data,	p_buffer, image_size, filter_size, direction, work_area, min_thresh, pad_type);

	//�o�b�t�@�̒l���摜�������ɖ߂�
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;
}

