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
//
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "global_define.h"

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

//�T�u�s�N�Z���l���v�Z�ibi-linear interporation�j
template <class DATA>
double	sub_pix_value(DATA *p_src_data, CSize src_size, double x, double y)
{
	double	wx, wy, w0, w1, w2, w3;
	int		addr;
	double  sum = 0;

	//�摜����1pix�ȏ�͂ݏo���ꍇ��0��Ԃ�
	if (x<-1 || x>src_size.cx || y<-1 || y>src_size.cy)	return 0;

	//�摜�̋�
	if (x < 0) {
		wx = 1.0;
	}
	else if (x > src_size.cx - 1) {
		wx = 0.0;
	}
	else {
		wx = x - (int)x;
	}

	if (y < 0) {
		wy = 1.0;
	}
	else if (y > src_size.cy - 1) {
		wy = 0.0;
	}
	else {
		wy = y - (int)y;
	}



	addr = floor(x) + floor(y)*src_size.cx;

	w0 = (1.0 - wx)*(1.0 - wy);
	w1 = (wx)*(1.0 - wy);
	w2 = (1.0 - wx)*(wy);
	w3 = (wx)*(wy);

	if (w0 > 0) {
		sum += (double)*(p_src_data + addr)*w0;
	}
	if (w1 > 0) {
		sum += (double)*(p_src_data + addr + 1)*w1;
	}
	if (w2 > 0) {
		sum += (double)*(p_src_data + addr + src_size.cx) *w2;
	}
	if (w3 > 0) {
		sum += (double)*(p_src_data + addr + src_size.cx + 1) *w3;
	}


	return sum;
}



template <class DATA>
bool	resize_image(
	DATA			*p_src,					//(in)�ϊ��O�̉摜�f�[�^
	DATA			*p_dst,					//(out)�ϊ���̉摜�f�[�^
	SIZE			src_size,				//(in)�ϊ��O�̉摜�T�C�Y
	int				page,					//(in)
	int				channel,				//(in)
	SIZE			dst_size,				//(in)�ϊ���̉摜�T�C�Y
	bool			do_stretch,				//(in)�ϊ���̃T�C�Y�ɍ��킹�ĉ摜���g��/�k�����邩(true:�g�傷�� false:�g�債�Ȃ�)
	POINT			offset,					//(in)�I�t�Z�b�g��
	int				reduction_type			//(in)�摜�k���̃A���S�i0:area-integration 1:max 2:min�j
	)
{
	if (do_stretch)//�ϊ���̃T�C�Y�ɍ��킹�ĉ摜�������L�΂�
	{
		//�{��
		double scale_x = (double)dst_size.cx / (double)src_size.cx;
		double scale_y = (double)dst_size.cy / (double)src_size.cy;

		//�摜�k��(�ʐϐϕ�)�p�f�[�^�i�[�o�b�t�@�m��
		int buffersize = BIGGER(src_size.cx, dst_size.cx) * BIGGER(src_size.cy, dst_size.cy) + src_size.cx + src_size.cy;
		float *p_buffer = new float[buffersize];
		if (p_buffer == NULL)
		{//�o�b�t�@�m�ۂł��Ȃ���΃G���[�I��
			return false;
		}

		float *p_x_integralval = p_buffer + BIGGER(src_size.cx, dst_size.cx) * BIGGER(src_size.cy, dst_size.cy);
		float *p_y_integralval = p_x_integralval + src_size.cx;


		//�f�[�^���T�C�Y
		for (int k = 0; k<page; k++)
		{
			for (int c = 0; c<channel; c++)
			{
				int src_offs = (k*channel + c)*src_size.cx*src_size.cy;
				int dst_offs = (k*channel + c)*dst_size.cx*dst_size.cy;

				//x����
				if (scale_x<1)
				{//�k��

					for (int j = 0; j<src_size.cy; j++)
					{
						for (int i = 0; i<dst_size.cx; i++)
						{
							float startx = (float)i / scale_x;//����pix�ɏW�񂳂��x���W�͈͂̎n�_
							float endx = (float)(i + 1) / scale_x;//����pix�ɏW�񂳂��x���W�͈͂̏I�_
							float weight;
							int cnt = 0;//����pix�ɏW�񂳂���f��
							float sum = 0, denom = 0;//����pix�ɏW�񂳂���f�l���v


							//�̈�n�_���F�����I�ɗ̈�ɓ����f
							weight = (ceil(startx) - startx);
							if (weight>0){
								p_x_integralval[cnt] = *(p_src + (int)startx + j*src_size.cx + src_offs);
								sum += p_x_integralval[cnt] * weight;
								denom += weight;
								cnt++;
							}

							//�̈撆���F���ׂĂ��̈�ɓ����f
							for (int pix = ceil(startx); pix< floor(endx); pix++)
							{
								p_x_integralval[cnt] = *(p_src + pix + j*src_size.cx + src_offs);
								sum += p_x_integralval[cnt];
								denom++;
								cnt++;
							}

							//�̈�I�_���F�����I�ɗ̈�ɓ����f
							weight = (endx - floor(endx));
							if (weight>0 && endx<src_size.cx){
								p_x_integralval[cnt] = *(p_src + (int)endx + j*src_size.cx + src_offs);
								sum += p_x_integralval[cnt] * weight;
								denom += weight;
								cnt++;
							}


							if (reduction_type == 1)
							{//max
								float maxval = p_x_integralval[0];
								for (int idx = 0; idx<cnt; idx++){
									if (maxval < p_x_integralval[idx]){
										maxval = p_x_integralval[idx];
									}
								}
								*(p_buffer + i + j*dst_size.cx) = maxval;
							}
							else if (reduction_type == 2)
							{//max
								float minval = p_x_integralval[0];
								for (int idx = 0; idx<cnt; idx++){
									if (minval > p_x_integralval[idx]){
										minval = p_x_integralval[idx];
									}
								}
								*(p_buffer + i + j*dst_size.cx) = minval;
							}
							else
							{//area integration
								*(p_buffer + i + j*dst_size.cx) = sum / denom;
							}
						}
					}
				}
				else
				{//�g�偨bi-linear

					for (int j = 0; j<src_size.cy; j++)
					{
						for (int i = 0; i<dst_size.cx; i++)
						{
							float x = ((float)i - 0.5) / scale_x;
							*(p_buffer + i + j*dst_size.cx) = (DATA)sub_pix_value(p_src + src_offs, src_size, x, j);
						}
					}
				}


				//y����
				if (scale_y<1)
				{//�k��

					for (int i = 0; i<dst_size.cx; i++)
					{
						for (int j = 0; j<dst_size.cy; j++)
						{
							float starty = (float)j / scale_y;//����pix�ɏW�񂳂��y���W�͈͂̎n�_
							float endy = (float)(j + 1) / scale_y;//����pix�ɏW�񂳂��y���W�͈͂̏I�_
							float weight;
							int cnt = 0;//����pix�ɏW�񂳂���f��
							float sum = 0, denom = 0;//����pix�ɏW�񂳂���f�l���v


							//�̈�n�_���F�����I�ɗ̈�ɓ����f
							weight = (ceil(starty) - starty);
							if (weight>0){
								p_y_integralval[cnt] = *(p_buffer + i + ((int)starty)*dst_size.cx);
								sum += p_y_integralval[cnt] * weight;
								denom += (ceil(starty) - starty);
								cnt++;
							}

							//�̈撆���F���ׂĂ��̈�ɓ����f
							for (int pix = ceil(starty); pix< floor(endy); pix++)
							{
								p_y_integralval[cnt] = *(p_buffer + i + pix*dst_size.cx);
								sum += p_y_integralval[cnt];
								denom++;
								cnt++;
							}

							//�̈�I�_���F�����I�ɗ̈�ɓ����f
							weight = (endy - floor(endy));
							if (weight>0 && endy<src_size.cy){
								p_y_integralval[cnt] = *(p_buffer + i + ((int)endy)*dst_size.cx);
								sum += p_y_integralval[cnt] * weight;
								denom += weight;
								cnt++;
							}

							if (reduction_type == 1)
							{//max
								float maxval = p_y_integralval[0];
								for (int idx = 0; idx<cnt; idx++){
									if (maxval < p_y_integralval[idx]){
										maxval = p_y_integralval[idx];
									}
								}
								*(p_dst + i + j*dst_size.cx + dst_offs) = maxval;
							}
							else if (reduction_type == 2)
							{//max
								float minval = p_y_integralval[0];
								for (int idx = 0; idx<cnt; idx++){
									if (minval > p_y_integralval[idx]){
										minval = p_y_integralval[idx];
									}
								}
								*(p_dst + i + j*dst_size.cx + dst_offs) = minval;
							}
							else
							{//area integration
								*(p_dst + i + j*dst_size.cx + dst_offs) = sum / denom;
							}
						}
					}
				}
				else
				{//�g��
					CSize buffseSize(dst_size.cx, src_size.cy);
					for (int j = 0; j<dst_size.cy; j++)
					{
						for (int i = 0; i<dst_size.cx; i++)
						{
							float y = (float)(j - 0.5) / scale_y;
							*(p_dst + i + j*dst_size.cx + dst_offs) = (DATA)sub_pix_value(p_buffer, buffseSize, i, y);
						}
					}
				}
			}
		}


		delete[]	p_buffer;
	}
	else//�ϊ��O�̏k�ڂ̂܂܃R�s�[�i�͂ݏo�������͐؂�����j
	{
		for (int j = 0; j<dst_size.cy; j++)
		{
			for (int i = 0; i<dst_size.cx; i++)
			{
				int im = i - offset.x;
				int jm = j - offset.y;

				if (im >= 0 && jm >= 0 && im<src_size.cx && jm<src_size.cy)
				{
					for (int k = 0; k<page; k++){
						for (int c = 0; c<channel; c++){
							*(p_dst + i + j*dst_size.cx + (k*channel + c)*dst_size.cx*dst_size.cy)
								= *(p_src + im + jm*src_size.cx + (k*channel + c)*src_size.cx*src_size.cy);//�f�[�^�R�s�[
						}
					}
				}
			}
		}
	}
	return true;
}


