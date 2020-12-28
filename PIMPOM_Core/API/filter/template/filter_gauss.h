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
//�K�E�V�A���t�B���^
/*******************************************************************/

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
�@  �\  ��  �� : �K�E�V�A���t�B���^
��    ��    �� : filter_gauss
��          �� : 
��    ��    �l : ��������� true
�@          �\ : �摜�f�[�^�ɃK�E�V�A���t�B���^��������
                 3�Ђ��t�B���^�T�C�Y�ɂȂ�悤�ɂ���

                 �J�[�l���F
				 �@  �� = (filter_size-1)/6
				     G(x,y) = exp( - x^2 / 2��^2 )

				 [�Z�x]= conv{ I(x,y), G(x,y) }

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
#if 0
//�����Ł@x�����t�B���^��y�����t�B���^
template <class DATA, class DATA2>
bool	filter_gauss(DATA			*p_src_data,	//(in)���͉摜�z��̐擪�|�C���^
					 DATA2			*p_dst_data,	//(out)�o�͉摜�z��̐擪�|�C���^
					 SIZE			image_size,		//(in)�摜�T�C�Y
					 int			filter_size,	//(in)�t�B���^�T�C�Y  ��l�̂ݓ��͉\
					 int			direction,		//(in)�t�B���^���� 0:���̂� 1:�c�̂�  ����ȊO:����
					 RECT			work_area,		//(in)�����̈�
					 float			data_max_limit, //(in)�f�[�^����l
					 float			data_min_limit,	//(in)�f�[�^�����l
					int				pad_type,		//(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
					float			*pOutKernel		//(out)�J�[�l���p�����^(NULL��)
					 )
{
	float  kernel[1000]={0};
	int half_kernel_size;
	float  sum;
	float *p_buffer=NULL, *p_buffer2=NULL;


	if(filter_size%2==0)	return false;//�t�B���^�T�C�Y�͊�̂�


	p_buffer = new float[image_size.cx * image_size.cy * 2];//�o�b�t�@�������m��(2�ʕ�)
	if(!p_buffer)	return false;

	memset(p_buffer, 0, image_size.cx * image_size.cy * 2*sizeof(float) );//�o�b�t�@������

	p_buffer2 = p_buffer + image_size.cx * image_size.cy;



	//�J�[�l���̍쐬(�t�B���^�T�C�Y��3�Ё~2+1�ɂȂ�悤�ɂ���)
	half_kernel_size= (filter_size - 1) / 2;//�t�B���^�T�C�Y�̔���

	float sig, sig2;
	sig = (float)(filter_size-1)/6.0;
	sig2 = 2 * sig * sig;


	float kernel_sum=0;
    for (int i = 0; i < filter_size; i++) 
	{
        float x = (i - half_kernel_size);
        kernel[i] = exp(- x*x / sig2) ;//exp( - x^2 / 2��^2 )

		//kernel_sum += kernel[i];
    }


	/*for (int i = 0; i < filter_size; i++) {
		kernel[i] /= kernel_sum;
	}*/




	//��������

	if(direction!=1)
	{
		for(int y = work_area.top ; y<=work_area.bottom ; y++){
			for(int x = work_area.left ; x<=work_area.right ; x++){
				sum = 0;
				kernel_sum=0;
				for(int ky=-half_kernel_size ; ky<=half_kernel_size ; ky++)
				{
					if( (y+ky)<0 || (y+ky)>=image_size.cy )	continue;//�͂ݏo���`�F�b�N

					sum += (float)*(p_src_data + x + (y+ky)*image_size.cx) * kernel[ky + half_kernel_size] ;
					kernel_sum += kernel[ky + half_kernel_size];
				}
				*(p_buffer + x + y*image_size.cx) = sum / kernel_sum;
			}
		}
	}
	else
	{	//�������@�݂̂Ƀt�B���^��������Ƃ��͂��̂܂܃R�s�[
		for(int y = work_area.top ; y<=work_area.bottom ; y++){
			for(int x = work_area.left ; x<=work_area.right ; x++){
				*(p_buffer + x + y*image_size.cx) = (float)*(p_src_data + x + y*image_size.cx);
			}
		}
	}

	
	//��������

	if(direction!=2)
	{
		for(int y = work_area.top ; y<=work_area.bottom ; y++){
			for(int x = work_area.left ; x<=work_area.right ; x++){
				sum = 0;
				kernel_sum=0;
				for(int kx=-half_kernel_size ; kx<=half_kernel_size ; kx++)
				{
					if( (x+kx)<0 || (x+kx)>=image_size.cx )	continue;//�͂ݏo���`�F�b�N

					sum += (float)*(p_buffer + (x+kx) + y*image_size.cx) * kernel[kx + half_kernel_size] ;
					kernel_sum += kernel[kx + half_kernel_size];
				}
				*(p_buffer2 + x + y*image_size.cx) = sum/kernel_sum;
			}
		}
	}
	else
	{
		//���������݂̂Ƀt�B���^��������Ƃ��͂��̂܂܃R�s�[
		for(int y = work_area.top ; y<=work_area.bottom ; y++){
			for(int x = work_area.left ; x<=work_area.right ; x++){
				*(p_buffer2 + x + y*image_size.cx) = (float)*(p_buffer + x + y*image_size.cx);
			}
		}
	}


	//�f�[�^�㉺���`�F�b�N
	for(int y = work_area.top ; y<=work_area.bottom ; y++){
		for(int x = work_area.left ; x<=work_area.right ; x++){
			float val = *(p_buffer2 + x + y*image_size.cx);

			if(val < data_min_limit )		val = data_min_limit;
			else if(val > data_max_limit )	val = data_max_limit;

			*(p_dst_data + x + y*image_size.cx) = (DATA2)(val);
		}
	}


	delete[]	p_buffer;

	return true;
}
#else
//�ʏ�Ł@2�����t�B���^
template <class DATA, class DATA2>
bool	filter_gauss(
	DATA			*p_src_data,	//(in)���͉摜�z��̐擪�|�C���^
	DATA2			*p_dst_data,	//(out)�o�͉摜�z��̐擪�|�C���^
	SIZE			image_size,		//(in)�摜�T�C�Y
	int				filter_size,	//(in)�t�B���^�T�C�Y  ��l�̂ݓ��͉\
	int				direction,		//(in)�t�B���^���� 0:���̂� 1:�c�̂�  ����ȊO:����
	RECT			work_area,		//(in)�����̈�
	float			data_max_limit, //(in)�f�[�^����l
	float			data_min_limit,	//(in)�f�[�^�����l
	int				pad_type,		//(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
	float			*pOutKernel		//(out)�J�[�l���p�����^(NULL��)
)
{
	float  kernel[COMB_FILTER_SIZE_MAX*COMB_FILTER_SIZE_MAX] = { 0 };
	int filter_size_x, filter_size_y;



	if (filter_size % 2 == 0)	return false;//�t�B���^�T�C�Y�͊�̂�

	//�t�B���^������������
	filter_size_x = filter_size;
	filter_size_y = filter_size;
	if (direction == 1) {//���̂�
		filter_size_y = 1;
	}
	else if (direction == 2) {//�c�̂�
		filter_size_x = 1;
	}





	//�J�[�l���̍쐬(�t�B���^�T�C�Y��3�Ё~2+1�ɂȂ�悤�ɂ���)
	float sig, sig2;
	sig = (float)(filter_size - 1) / 6.0;
	sig2 = 2 * sig * sig;


	float sum = 0;
	for (int j = 0; j < filter_size_y; j++)
	{
		for (int i = 0; i < filter_size_x; i++)
		{
			int	addr = i + j*filter_size_x;
			double x = (i - filter_size_x / 2);
			double y = (j - filter_size_y / 2);

			kernel[addr] = exp(-(x*x + y*y) / sig2);//exp( - x^2 / 2��^2 )

			sum += kernel[addr];
		}
	}
	
	//�J�[�l�����v���Ŋ���
	for (int j = 0; j < filter_size_y; j++) {
		for (int i = 0; i < filter_size_x; i++) {
			kernel[i + j*filter_size_x] /= sum;
		}
	}



	if (pOutKernel) {
		memcpy(pOutKernel, kernel, filter_size_y*filter_size_x * sizeof(float));
	}

	//��݂���
	SIZE fs;
	fs.cx = filter_size_x;
	fs.cy = filter_size_y;
	filter_convolution(p_src_data, p_dst_data, image_size, fs, kernel, work_area, 0, 1, data_max_limit, data_min_limit, pad_type);

	return true;
}

#endif


/********************************************************************
�@  �\  ��  �� : �K�E�X�t�B���^���ȍ����ɂ��V�F�[�f�B���O����
��    ��    �� : filter_gauss_unshade
��          �� : 
��    ��    �l : ��������� true
�@          �\ : ���摜����K�E�V�A���t�B���^��̉摜������
�@�@�@�@�@�@�K�E�V�A���t�B���^��3�Ђ��t�B���^�T�C�Y�ɂȂ�悤�ɂ���

�@�@�@�@�@[�Z�x] = ( G(x,y) - I(x,y) + offset )*contrast
              �@(I(x,y)�͌��摜�̔Z�x�AG(x,y)�̓K�E�V�A���t�B���^��̔Z�x��\��)

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA, class DATA2>
bool	filter_gauss_unshade(
					 DATA			*p_src_data,	//(in)���͉摜�z��̐擪�|�C���^
					 DATA2			*p_dst_data,	//(out)�o�͉摜�z��̐擪�|�C���^
					 SIZE			image_size,		//(in)�摜�T�C�Y
					 int			filter_size,	//(in)�t�B���^�T�C�Y  ��l�̂ݓ��͉\
					 int			direction,		//(in)�t�B���^���� 0:���̂� 1:�c�̂�  ����ȊO:����
					 float			offset,			//(in)
					 float			contrast,		//(in)
					 RECT			work_area,		//(in)�����̈�
					 float			data_max_limit, //(in)�f�[�^����l
					 float			data_min_limit,	//(in)�f�[�^�����l
					int				pad_type,		//(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
					float			*pOutKernel		//(out)�J�[�l���p�����^(NULL��)
					 )
{
	bool ret=true;
	float *p_buffer=NULL, *p_buffer2=NULL;

	p_buffer = new float[image_size.cx*image_size.cy*2];//�o�b�t�@�������m��
	if(p_buffer==NULL)	return false;

	p_buffer2 = p_buffer + image_size.cx*image_size.cy;

	ret = filter_gauss(p_src_data, p_buffer, image_size, filter_size, direction, work_area, data_max_limit, data_min_limit, pad_type, pOutKernel);
	if(ret)
	{
		//�������Ƃ�
		for(int j = work_area.top ; j<=work_area.bottom ; j++)
		{
			for(int i = work_area.left ; i<=work_area.right ; i++)
			{
				int addr = i + j*image_size.cx;
				
				float val= ( (float)*(p_src_data + addr) - *(p_buffer + addr) + offset ) * contrast;

				//�f�[�^�㉺���`�F�b�N
				if(val < data_min_limit )		val = data_min_limit;
				else if(val > data_max_limit )	val = data_max_limit;
				*(p_dst_data + addr) = (DATA2)(val);
			}
		}
	}

	delete[]	p_buffer;
	
	return ret;
}



/********************************************************************
�@  �\  ��  �� : �K�E�X�t�B���^
��    ��    �� : filter_gauss_overwrite
��          �� : 
��    ��    �l : ��������� true
�@          �\ : �摜�ɃK�E�V�A���t�B���^��������
				�K�E�V�A���t�B���^��3�Ђ��t�B���^�T�C�Y�ɂȂ�悤�ɂ���
                 ���̃�������ɏ㏑������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
bool	filter_gauss_overwrite(
							   DATA		*p_src_data,		//(in/out)�Ώۉ摜�z��̐擪�|�C���^
							   SIZE		image_size,			//(in)�摜�T�C�Y
							   int		filter_size,		//(in)�t�B���^�T�C�Y
							   int		direction,			//(in)�t�B���^���� 0:���̂� 1:�c�̂�  ����ȊO:����
							   RECT		work_area,			//(in)�����̈�
							   float	data_max_limit,		//(in)�f�[�^����l
							   float	data_min_limit,		//(in)�f�[�^�����l
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
	ret = filter_gauss(p_src_data,	p_buffer, image_size, filter_size, direction, work_area, data_max_limit, data_min_limit, pad_type, pOutKernel);


	//�o�b�t�@�̒l���摜�������ɖ߂�
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;
}



/********************************************************************
�@  �\  ��  �� : �K�E�X�t�B���^���ȍ����ɂ��V�F�[�f�B���O����
��    ��    �� : filter_gauss_unshade_overwrite
��          �� : 
��    ��    �l : ��������� true
�@          �\ : ���摜����K�E�V�A���t�B���^��̉摜������
                 filter_size = 3�Ё~2+1 �Ƃ���
				  ���̃�������ɏ㏑������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
bool	filter_gauss_unshade_overwrite(
							   DATA		*p_src_data,		//(in/out)�Ώۉ摜�z��̐擪�|�C���^
							   SIZE		image_size,			//(in)�摜�T�C�Y
							   int		filter_size,		//(in)�t�B���^�T�C�Y
							   int		direction,			//(in)�t�B���^���� 0:���̂� 1:�c�̂�  ����ȊO:����
							   float	offset,				//(in)
							   float	contrast,			//(in)
							   RECT		work_area,			//(in)�����̈�
							   float	data_max_limit,		//(in)�f�[�^����l
							   float	data_min_limit,		//(in)�f�[�^�����l
								int		pad_type,			//(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
								float	*pOutKernel			//(out)�J�[�l���p�����^(NULL��)
							   )
{
	bool	ret=true;
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//�o�b�t�@�������m��
	if(!p_buffer)	return false;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//�o�b�t�@������

	//���s
	ret = filter_gauss_unshade(p_src_data,	p_buffer, image_size, filter_size, direction, offset, contrast, work_area, data_max_limit, data_min_limit, pad_type, pOutKernel);


	//�o�b�t�@�̒l���摜�������ɖ߂�
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;
}