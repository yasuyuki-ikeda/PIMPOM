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
//�􉽕ϊ�
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "opencv_headers.h"
#include "PIMPOM_API.h"
#include "filter\template\filter_ma.h"
#include "resize_data.h"

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

//�}�X�N�̃T�u�s�N�Z���l���v�Z
BYTE	sub_pix_mask_value(BYTE *p_mask_data, CSize src_size, double x, double y)
{
	//��ʒ[�Ŏ��܂�Ȃ��ꍇ�̓}�X�NON
	if (x<-1 || x>src_size.cx || y<-1 || y>src_size.cy)	return MASKED_PIXEL;

	double mask_interm = sub_pix_value(p_mask_data, src_size, x, y);
	if ((MASKED_PIXEL + UNMASKED_PIXEL) / 2 < mask_interm)	return MASKED_PIXEL;
	else	return UNMASKED_PIXEL;
}


//���T�C�Y���s
template <class DATA>
bool	resize_data_unit(CPimpomAPI		*pAPI,					//(in)CPimpomAPI�N���X�I�u�W�F�N�g�ւ̃|�C���^
	int				target_data_number,		//(in)�ϊ��Ώۂ̉摜�������ԍ�
	int				dst_data_number,		//(in)�ϊ���̉摜������摜�������ԍ�
	bool			do_clear_dst,			//(in)�ϊ��O�ɉ摜���������N���A���邩�ǂ���
	DATA			*p_src,				//(in)�ϊ��Ώۂ̉摜�f�[�^�itarget_data_number�Ŏ����ԍ��̃f�[�^��퓬�|�C���^�j
	CSize			dst_size,				//(in)�ϊ���̃T�C�Y
	bool			do_stretch,				//(in)�ϊ���̃T�C�Y�ɍ��킹�ĉ摜���g��/�k�����邩(true:�g�傷�� false:�g�債�Ȃ�)
	CPoint			offset,					//(in)�I�t�Z�b�g��
	int				reduction_type			//(in)�摜�k���̃A���S�i0:area-integration 1:max 2:min�j
	)
{
	CDataUnit	*p_du, *p_new_dst_du;
	int			page, channel;
	CSize		src_size;
	DATA		*p_dst, *p_src_buffer;
	BYTE		*p_src_mask_buffer;
	CWorkArea	src_workarea;
	bool		do_use_mask;
	bool		do_use_workarea;


	//���f�[�^�擾
	if ((p_du = pAPI->GetDataUnit(target_data_number)) == NULL)	return false;

	src_size = p_du->DataSize;
	page = p_du->PageNumber;
	channel = p_du->ChannelNumber;



	//���f�[�^�̈ꎞ�ޔ��o�b�t�@�m��
	p_src_buffer = new DATA[src_size.cx * src_size.cy * page * channel];//�f�[�^�i�[�o�b�t�@
	if (p_src_buffer == NULL)	return false;

	p_src_mask_buffer = new BYTE[src_size.cx * src_size.cy];//�}�X�N�i�[�o�b�t�@
	if (p_src_mask_buffer == NULL){
		delete[]	p_src_buffer;
		return false;
	}



	//���f�[�^�ޔ������T�C�Y��̃������擾

	memcpy(p_src_mask_buffer, p_du->pMaskData, src_size.cx * src_size.cy);//�}�X�N�ޔ�
	src_workarea.Copy(&p_du->WorkArea);//��Ɨ̈�ޔ�
	do_use_mask = p_du->DoUseMask;
	do_use_workarea = p_du->DoUseWorkArea;

	switch (p_du->DataType)
	{
	case BYTE_FORMAT:
		memcpy(p_src_buffer, p_du->pByteData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//�f�[�^�ޔ�
		p_dst = (DATA*)pAPI->GetByteMemory(dst_data_number, dst_size, do_clear_dst);//�V�������m��
		break;

	case SHORT16_FORMAT:
		memcpy(p_src_buffer, p_du->pShortData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//�f�[�^�ޔ�
		p_dst = (DATA*)pAPI->GetShortMemory(dst_data_number, dst_size, do_clear_dst);//�V�������m��
		break;

	case WORD_FORMAT:
		memcpy(p_src_buffer, p_du->pWordData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//�f�[�^�ޔ�
		p_dst = (DATA*)pAPI->GetWordMemory(dst_data_number, dst_size, do_clear_dst);//�V�������m��
		break;

	case LONG32_FORMAT:
		memcpy(p_src_buffer, p_du->pLongData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//�f�[�^�ޔ�
		p_dst = (DATA*)pAPI->GetLongMemory(dst_data_number, dst_size, do_clear_dst);//�V�������m��
		break;

	case DWORD_FORMAT:
		memcpy(p_src_buffer, p_du->pDwordData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//�f�[�^�ޔ�
		p_dst = (DATA*)pAPI->GetDwordMemory(dst_data_number, dst_size, do_clear_dst);//�V�������m��
		break;

	case RGB_FORMAT:
		memcpy(p_src_buffer, p_du->pByteData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//�f�[�^�ޔ�
		p_dst = (DATA*)pAPI->GetRGBMemory(dst_data_number, dst_size, do_clear_dst);//�V�������m��
		break;

	case FLOAT_FORMAT:
		memcpy(p_src_buffer, p_du->pFloatData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//�f�[�^�ޔ�
		p_dst = (DATA*)pAPI->GetFloatMemory(dst_data_number, dst_size, do_clear_dst);//�V�������m��
		break;

	case XYZ_FORMAT:
		memcpy(p_src_buffer, p_du->pFloatData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//�f�[�^�ޔ�
		p_dst = (DATA*)pAPI->GetXYZMemory(dst_data_number, dst_size, do_clear_dst);//�V�������m��
		break;

	case COMPLEX_FORMAT:
		memcpy(p_src_buffer, p_du->pFloatData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//�f�[�^�ޔ�
		p_dst = (DATA*)pAPI->GetComplexMemory(dst_data_number, dst_size, do_clear_dst);//�V�������m��
		break;

	case THREE_D_FORMAT:
		memcpy(p_src_buffer, p_du->pByteData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//�f�[�^�ޔ�
		p_dst = (DATA*)pAPI->Get3DMemory(dst_data_number, dst_size, page, do_clear_dst);//�V�������m��
		break;

	case FLOAT_3D_FORMAT:
		memcpy(p_src_buffer, p_du->pFloatData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//�f�[�^�ޔ�
		p_dst = (DATA*)pAPI->GetF3DMemory(dst_data_number, dst_size, page, do_clear_dst);//�V�������m��
		break;

	case RGB_3D_FORMAT:
		memcpy(p_src_buffer, p_du->pByteData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//�f�[�^�ޔ�
		p_dst = (DATA*)pAPI->GetRGB3DMemory(dst_data_number, dst_size, page, do_clear_dst);//�V�������m��
		break;
	}

	if (p_dst == NULL)
	{//�V�������擾�Ɏ��s������G���[�I��
		delete[]	p_src_buffer;
		delete[]	p_src_mask_buffer;

		return false;
	}

	p_new_dst_du = pAPI->GetDataUnit(dst_data_number);//���T�C�Y��̃f�[�^


	//�f�[�^���T�C�Y���s
	if (!resize_image(p_src_buffer, p_dst, src_size, page, channel, dst_size, do_stretch, offset, reduction_type)){
		delete[]	p_src_buffer;
		delete[]	p_src_mask_buffer;

		return false;
	}

	//�}�X�N�E��Ɨ̈�̃��T�C�Y���s
	if (do_stretch)
	{
		//�{��
		double scale_x = (double)dst_size.cx / (double)src_size.cx;
		double scale_y = (double)dst_size.cy / (double)src_size.cy;


		//�}�X�N���T�C�Y(bi-linear)
		for (int j = 0; j<dst_size.cy; j++)
		{
			for (int i = 0; i<dst_size.cx; i++)
			{
				float x = ((float)i - 0.5) / scale_x;
				float y = ((float)j - 0.5) / scale_y;

				if (x < 0 || y < 0){//�l�̌ܓ����ĕ����ɂȂ��Ă��܂��ꍇ�̑Ή�
					*(p_new_dst_du->pMaskData + i + j*dst_size.cx) = UNMASKED_PIXEL;
				}
				else{
					*(p_new_dst_du->pMaskData + i + j*dst_size.cx) = sub_pix_mask_value(p_src_mask_buffer, src_size, x, y);
				}
			}
		}

		//��Ɨ̈惊�T�C�Y
		for (int n = 0; n < WORKAREA_FIG_NUM; n++) {
			src_workarea.Scale(n, scale_x, scale_y, CPoint(0, 0));
		}
		p_new_dst_du->WorkArea.Copy(&src_workarea);


		//delete[]	p_buffer;
	}
	else//�ϊ��O�̏k�ڂ̂܂܃R�s�[�i�͂ݏo�������͐؂�����j
	{
		//��Ɨ̈�R�s�[
		for (int n = 0; n < WORKAREA_FIG_NUM; n++) {
			src_workarea.Scale(n, 1, 1, offset);
		}
		p_new_dst_du->WorkArea.Copy(&src_workarea);
	}

	p_new_dst_du->DoUseMask = do_use_mask;//�}�X�N�̎g�p��Ԃ𕜌�
	p_new_dst_du->DoUseWorkArea = do_use_workarea;//��Ɨ̈�̎g�p��Ԃ𕜌�


	delete[]	p_src_buffer;
	delete[]	p_src_mask_buffer;

	return true;
}



//�p�[�X�y�N�e�B�u�ϊ�
template <class DATA>
bool	perspective_data(CPimpomAPI		*pAPI,				//(in)CPimpomAPI�N���X�I�u�W�F�N�g�ւ̃|�C���^
						int				target_data_number, //(in)�ϊ��Ώۂ̉摜�������ԍ�
						int				dst_data_number,	//(in)�ϊ���̉摜������摜�������ԍ�
						bool			do_clear_dst,		//(in)�ϊ��O�ɉ摜���������N���A���邩�ǂ���
						DATA			*p_src ,			//(in)�ϊ��Ώۂ̉摜�f�[�^�itarget_data_number�Ŏ����ԍ��̃f�[�^��퓬�|�C���^�j
						double			p_param[8],			//(in)�ˉe�ϊ��̃p�����^
						int				outRgnMode			//(in)�摜����͂ݏo���������̏����̕��@(0:���̂܂� 1:�ŋߖT��f�̒l�ŕ��)
						)
{
	int i,j,k,c;
	int page, channel;
	CDataUnit	*p_du;
	CSize		src_size;
	DATA		*p_buffer, *p_dst;
	BYTE		*p_mask_buffer, *p_dst_mask;
	CWorkArea	src_workarea;
	bool		do_use_mask;
	bool		do_use_workarea;

	if( ( p_du = pAPI->GetDataUnit(target_data_number) ) == NULL )	return false;

	src_size = p_du->DataSize;
	page = p_du->PageNumber;
	channel = p_du->ChannelNumber;
	


	//���݂̃f�[�^����U�i�[����
	p_buffer = new DATA[ src_size.cx * src_size.cy * page * channel ];
		if(p_buffer==NULL)	return false;

	p_mask_buffer = new BYTE[ src_size.cx * src_size.cy ];
	if(p_mask_buffer==NULL){
		delete[]	p_buffer;
		return false;
	}

	memcpy(p_buffer, p_src, src_size.cx * src_size.cy * page * channel * sizeof(DATA) );
	memcpy(p_mask_buffer, p_du->pMaskData, src_size.cx * src_size.cy );
	src_workarea.Copy(&p_du->WorkArea);//��Ɨ̈�
	do_use_mask = p_du->DoUseMask;
	do_use_workarea = p_du->DoUseWorkArea;

	//�������擾
	switch(p_du->DataType)
	{
		case BYTE_FORMAT:
			p_dst = (DATA*)pAPI->GetByteMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case SHORT16_FORMAT:
			p_dst = (DATA*)pAPI->GetShortMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case WORD_FORMAT:
			p_dst = (DATA*)pAPI->GetWordMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case LONG32_FORMAT:
			p_dst = (DATA*)pAPI->GetLongMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case DWORD_FORMAT:
			p_dst = (DATA*)pAPI->GetDwordMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case RGB_FORMAT:
			p_dst = (DATA*)pAPI->GetRGBMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case FLOAT_FORMAT:
			p_dst = (DATA*)pAPI->GetFloatMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case XYZ_FORMAT:
			p_dst = (DATA*)pAPI->GetXYZMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case COMPLEX_FORMAT:
			p_dst = (DATA*)pAPI->GetComplexMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case THREE_D_FORMAT:
			p_dst = (DATA*)pAPI->Get3DMemory(dst_data_number, src_size, page, do_clear_dst);
			break;

		case FLOAT_3D_FORMAT:
			p_dst = (DATA*)pAPI->GetF3DMemory(dst_data_number, src_size, page, do_clear_dst);
			break;

		case RGB_3D_FORMAT:
			p_dst = (DATA*)pAPI->GetRGB3DMemory(dst_data_number, src_size, page, do_clear_dst);
			break;
	}

	if(p_dst == NULL)
	{
		delete[]	p_buffer;
		delete[]	p_mask_buffer;
		return false;
	}
	p_dst_mask = pAPI->GetMaskMemory(dst_data_number);

	//�ϊ����s
	double x,y,D,Nx,Ny;
	for(j=0 ; j<src_size.cy ; j++)
	{
		for(i=0 ; i<src_size.cx ; i++)
		{
			if(p_param[6]!=0 || p_param[7]!=0)//�p�[�X�y�N�e�B�u�ϊ�
			{
				D = p_param[6]*i + p_param[7]*j + 1;
				if(D!=0){
					Nx = p_param[0]*i + p_param[1]*j + p_param[2];
					Ny = p_param[3]*i + p_param[4]*j + p_param[5];
					x = Nx / D;
					y = Ny / D;
				}else{
					x = y = -9999;
				}
			}
			else{//�A�t�B���ϊ�
					x = p_param[0]*i + p_param[1]*j + p_param[2];
					y = p_param[3]*i + p_param[4]*j + p_param[5];
			}

			if(x>=0 && y>=0 && x<src_size.cx-1 && y<src_size.cy-1)
			{
				if(!do_use_workarea || src_workarea.PtInWorkArea(CPoint((int)x, (int)y)))//��Ɨ̈����
				{
					for(k=0 ; k<page ; k++){
						for(c=0 ; c<channel ; c++){
							*(p_dst + i + j*src_size.cx + (k*channel + c)*src_size.cx*src_size.cy) 
									= (DATA)sub_pix_value(p_buffer + (k*channel + c)*src_size.cx*src_size.cy, src_size, x, y);//�f�[�^�R�s�[
						}
					}

					*(p_dst_mask + i + j*src_size.cx) = sub_pix_mask_value(p_mask_buffer, src_size, x, y);//�}�X�N�R�s�[
				}
			}
			else if(outRgnMode==1)
			{
				//�ŋߖT��f�̎w��
				if(x<0){
					x = 0;
				}
				else if (x >= src_size.cx - 1) {
					x = src_size.cx - 1;
				}

				if (y<0) {
					y = 0;
				}
				else if (y >= src_size.cy - 1) {
					y = src_size.cy - 1;
				}

				for(k=0 ; k<page ; k++){
					for(c=0 ; c<channel ; c++){
						*(p_dst + i + j*src_size.cx + (k*channel + c)*src_size.cx*src_size.cy)
							= (DATA)sub_pix_value(p_buffer + (k*channel + c)*src_size.cx*src_size.cy, src_size, x, y);//�f�[�^�R�s�[
					}
				}
			}
		}
	}


	delete[]	p_buffer;
	delete[]	p_mask_buffer;

	return true;
}




//�N���b�s���O
template <class DATA>
bool	clip_data(CPimpomAPI *pAPI, int target_data_number, int dst_data_number, bool do_clear_dst,  DATA *p_src , CRect clip_rect)
{
	int i,j,k,c;
	int page, channel;
	CDataUnit	*p_du;
	CSize		src_size, dst_size;
	DATA		*p_buffer, *p_dst;
	BYTE		*p_mask_buffer, *p_dst_mask;


	if( ( p_du = pAPI->GetDataUnit(target_data_number) ) == NULL )	return false;

	src_size = p_du->DataSize;
	page = p_du->PageNumber;
	channel = p_du->ChannelNumber;

	clip_rect.NormalizeRect();
	dst_size = CSize(clip_rect.right - clip_rect.left + 1, clip_rect.bottom - clip_rect.top + 1);
	

	//���݂̃f�[�^����U�i�[����
	p_buffer = new DATA[ src_size.cx * src_size.cy * page * channel ];//�f�[�^�i�[�o�b�t�@
	if(p_buffer==NULL)	return false;

	p_mask_buffer = new BYTE[ src_size.cx * src_size.cy ];//�}�X�N�i�[�o�b�t�@
	if(p_mask_buffer==NULL){
		delete[]	p_buffer;
		return false;
	}

	memcpy(p_buffer, p_src, src_size.cx * src_size.cy * page * channel * sizeof(DATA) );//�f�[�^
	memcpy(p_mask_buffer, p_du->pMaskData, src_size.cx * src_size.cy );//�}�X�N

		
	//�ϊ���̃������擾
	switch(p_du->DataType)
	{
		case BYTE_FORMAT:
			p_dst = (DATA*)pAPI->GetByteMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case SHORT16_FORMAT:
			p_dst = (DATA*)pAPI->GetShortMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case WORD_FORMAT:
			p_dst = (DATA*)pAPI->GetWordMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case LONG32_FORMAT:
			p_dst = (DATA*)pAPI->GetLongMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case DWORD_FORMAT:
			p_dst = (DATA*)pAPI->GetDwordMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case RGB_FORMAT:
			p_dst = (DATA*)pAPI->GetRGBMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case FLOAT_FORMAT:
			p_dst = (DATA*)pAPI->GetFloatMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case XYZ_FORMAT:
			p_dst = (DATA*)pAPI->GetXYZMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case COMPLEX_FORMAT:
			p_dst = (DATA*)pAPI->GetComplexMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case THREE_D_FORMAT:
			p_dst = (DATA*)pAPI->Get3DMemory(dst_data_number, dst_size, page, do_clear_dst);
			break;

		case FLOAT_3D_FORMAT:
			p_dst = (DATA*)pAPI->GetF3DMemory(dst_data_number, dst_size, page, do_clear_dst);
			break;

		case RGB_3D_FORMAT:
			p_dst = (DATA*)pAPI->GetRGB3DMemory(dst_data_number, dst_size, page, do_clear_dst);
			break;
	}

	if(p_dst == NULL)
	{
		delete[]	p_buffer;
		delete[]	p_mask_buffer;
		return false;
	}
	p_dst_mask = pAPI->GetMaskMemory(dst_data_number);

	//�N���b�v���s
	int x,y;
	for(j=0 ; j<dst_size.cy ; j++)
	{
		for(i=0 ; i<dst_size.cx ; i++)
		{
			x = i + clip_rect.left;
			y = j + clip_rect.top;
			if(x>=0 && y>=0 && x<src_size.cx && j<src_size.cy)
			{
				for(k=0 ; k<page ; k++)
				{
					for(c=0 ; c<channel ; c++)
					{
						*(p_dst + i + j*dst_size.cx + (k*channel + c)*dst_size.cx*dst_size.cy) 
										= *(p_buffer + x + y*src_size.cx + (k*channel + c)*src_size.cx*src_size.cy) ;//�f�[�^�R�s�[
					}
				}
				*(p_dst_mask + i + j*dst_size.cx) = *(p_mask_buffer + x + y*src_size.cx) ;//�}�X�N�R�s�[
			}
		}
	}

	delete[]	p_buffer;
	delete[]	p_mask_buffer;

	return true;
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�̃T�C�Y�ϊ�
��    ��    �� : ResizeDataUnit
��          �� : long			number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CSize			new_size		(in)�ϊ���̃T�C�Y
				bool			do_stretch		(in)�ϊ���̃T�C�Y�ɍ��킹�ĉ摜���g��/�k�����邩
				int				offset_x		(in)x�����I�t�Z�b�g��
				int				offset_y		(in)y�����I�t�Z�b�g��
				bool			show_message	(in)�ϊ��O�ɖ₢���킹�_�C�A���O��\�����邩
				int				reduction_type,	(in)�摜�k���̃A���S�i0:area-integration 1:max 2:min�j
��    ��    �l : ��������� true
�@          �\ : �ϊ����ʂ́A���摜�������ɏ㏑�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::ResizeDataUnit(long number, CSize new_size, bool do_stretch, int offset_x, int offset_y, bool show_message, int reduction_type)
{
	return ResizeDataUnit(number,number,true,new_size,do_stretch,offset_x,offset_y,show_message,reduction_type);
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�̃T�C�Y�ϊ�
��    ��    �� : ResizeDataUnit
��          �� : long			number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long			dst_data_number	(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 bool			do_clear_dst	(in)�i�[��摜����������x�N���A���邩�ǂ���(true:�N���A���� false:���̂܂� )
				CSize			new_size		(in)�ϊ���̃T�C�Y
				bool			do_stretch		(in)�ϊ���̃T�C�Y�ɍ��킹�ĉ摜���g��/�k�����邩(true:�g�傷�� false:�g�債�Ȃ�)
				int				offset_x		(in)x�����I�t�Z�b�g��
				int				offset_y		(in)y�����I�t�Z�b�g��
				bool			show_message	(in)�ϊ��O�ɖ₢���킹�_�C�A���O��\�����邩
				int				reduction_type,	(in)�摜�k���̃A���S�i0:area-integration 1:max 2:min�j
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::ResizeDataUnit(long number, long dst_data_number, bool do_clear_dst, 
								   CSize new_size, bool do_stretch, int offset_x, int offset_y, bool show_message, int reduction_type)
{
	bool	ret;
	CDataUnit	*p_du, *p_du_after;
	CString		data_name;
	float		bright_max, bright_min;
	int			disp_page, disp_channel;


	//�����`�F�b�N
	if(new_size.cx<1 || new_size.cy<1)	return false;

	if( (p_du = GetDataUnit(number) )==NULL)	return false;

	if(dst_data_number==CURRENT_IMAGE){
		dst_data_number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��
	}
	else if(dst_data_number<0 || dst_data_number>= GetDataUnitNumber())
	{
		return false;
	}


	data_name = p_du->DataName;
	bright_max = p_du->BrightMax;
	bright_min = p_du->BrightMin;
	disp_page = p_du->DispPage;
	disp_channel = p_du->DispChannel;

	if(show_message){
		if( GetDataUnit(dst_data_number) ){
			CString str;
			str.Format("�摜������%d�͏㏑������܂�",dst_data_number);
			if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONINFORMATION ) != IDOK ){
				return false;
			}
		}
	}


	//�ϊ����s
	ret = false;
	switch(p_du->DataType)
	{
		case BYTE_FORMAT:
			//not break
		case THREE_D_FORMAT:
			//not break
		case RGB_FORMAT:
			//not break
		case RGB_3D_FORMAT:
			ret= resize_data_unit(this, number, dst_data_number, do_clear_dst, p_du->pByteData, new_size, do_stretch, CPoint(offset_x,offset_y),reduction_type);
			break;

		case SHORT16_FORMAT:
			ret = resize_data_unit(this, number, dst_data_number, do_clear_dst, p_du->pShortData, new_size, do_stretch, CPoint(offset_x, offset_y), reduction_type);
			break;

		case WORD_FORMAT:
			ret = resize_data_unit(this, number, dst_data_number, do_clear_dst, p_du->pWordData, new_size, do_stretch, CPoint(offset_x, offset_y), reduction_type);
			break;

		case LONG32_FORMAT:
			ret = resize_data_unit(this, number, dst_data_number, do_clear_dst, p_du->pLongData, new_size, do_stretch, CPoint(offset_x, offset_y), reduction_type);
			break;

		case DWORD_FORMAT:
			ret = resize_data_unit(this, number, dst_data_number, do_clear_dst, p_du->pDwordData, new_size, do_stretch, CPoint(offset_x, offset_y), reduction_type);
			break;

		case FLOAT_FORMAT:
			//not break
		case COMPLEX_FORMAT:
			//not break
		case XYZ_FORMAT:
			//not break
		case XY_FORMAT:
			//not break
		case FLOAT_3D_FORMAT:
			ret = resize_data_unit(this, number, dst_data_number, do_clear_dst, p_du->pFloatData, new_size, do_stretch, CPoint(offset_x, offset_y), reduction_type);
			break;
	}
	if(!ret)	return false;

	//�\���Ȃǂ����ɖ߂�
	p_du_after = GetDataUnit(dst_data_number);
	if(!p_du_after)		return false;
	
	p_du_after->DataName = data_name;
	p_du_after->BrightMax = bright_max;
	p_du_after->BrightMin = bright_min;
	p_du_after->DispPage = disp_page;
	p_du_after->DispChannel = disp_channel;

	return true;
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�̃p�[�X�y�N�e�B�u�ϊ�
��    ��    �� : PerspectiveTransDataUnit
��          �� : long			number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				double			p_param[8]		(in)�ϊ��p�����^�i�z���O���t�B�s��j
				bool			show_message	(in)�ϊ��O�ɖ₢���킹�_�C�A���O��\�����邩
				int				outRgnMode		(in)�摜����͂ݏo���������̏����̕��@(0:���̂܂� 1:�ŋߖT��f�̒l�ŕ��)
��    ��    �l : ��������� true
�@          �\ : �ϊ����ʂ́A���摜�������ɏ㏑�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::PerspectiveTransDataUnit(long number, double p_param[8], bool show_message, int outRgnMode)
{
	return PerspectiveTransDataUnit(number,number,true,p_param,show_message, outRgnMode);
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�̃p�[�X�y�N�e�B�u�ϊ�
��    ��    �� : PerspectiveTransDataUnit
��          �� : long			number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long			dst_data_number	(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 bool			do_clear_dst	(in)�i�[��摜����������x�N���A���邩�ǂ���
				double			p_param[8]		(in)�ϊ��p�����^�i�z���O���t�B�s��j
				bool			show_message	(in)�ϊ��O�ɖ₢���킹�_�C�A���O��\�����邩
				int				outRgnMode		(in)�摜����͂ݏo���������̏����̕��@(0:���̂܂� 1:�ŋߖT��f�̒l�ŕ��)
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::PerspectiveTransDataUnit(long number, long dst_data_number, bool do_clear_dst, double p_param[8], bool show_message, int outRgnMode)
{
	bool	ret;
	CDataUnit	*p_du, *p_du_after;
	CString		data_name;
	float		bright_max, bright_min;
	int			disp_page, disp_channel;


	if( (p_du = GetDataUnit(number) )==NULL)	return false;

	if(dst_data_number==CURRENT_IMAGE){
		dst_data_number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��
	}
	else if(dst_data_number<0 || dst_data_number>= GetDataUnitNumber())
	{
		return false;
	}


	data_name = p_du->DataName;
	bright_max = p_du->BrightMax;
	bright_min = p_du->BrightMin;
	disp_page = p_du->DispPage;
	disp_channel = p_du->DispChannel;


	if(show_message){
		if( GetDataUnit(dst_data_number) ){
			CString str;
			str.Format("�摜������%d�͏㏑������܂�",dst_data_number);
			if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONINFORMATION ) != IDOK ){
				return false;
			}
		}
	}


	//�ϊ����s
	ret = false;
	switch(p_du->DataType)
	{
		case BYTE_FORMAT:
			//not break
		case THREE_D_FORMAT:
			//not break
		case RGB_FORMAT:
			//not break
		case RGB_3D_FORMAT:
			ret= perspective_data(this, number, dst_data_number, do_clear_dst, p_du->pByteData, p_param, outRgnMode);
			break;

		case SHORT16_FORMAT:
			ret= perspective_data(this, number, dst_data_number, do_clear_dst, p_du->pShortData, p_param, outRgnMode);
			break;

		case WORD_FORMAT:
			ret= perspective_data(this, number, dst_data_number, do_clear_dst, p_du->pWordData, p_param, outRgnMode);
			break;

		case LONG32_FORMAT:
			ret= perspective_data(this, number, dst_data_number, do_clear_dst, p_du->pLongData, p_param, outRgnMode);
			break;

		case DWORD_FORMAT:
			ret= perspective_data(this, number, dst_data_number, do_clear_dst, p_du->pDwordData, p_param, outRgnMode);
			break;

		case FLOAT_FORMAT:
			//not break
		case COMPLEX_FORMAT:
			//not break
		case XYZ_FORMAT:
			//not break
		case XY_FORMAT:
			//not break
		case FLOAT_3D_FORMAT:
			ret= perspective_data(this, number, dst_data_number, do_clear_dst, p_du->pFloatData, p_param, outRgnMode);
			break;
	}
	if(!ret)	return false;

	//�\���Ȃǂ����ɖ߂�
	p_du_after = GetDataUnit(dst_data_number);
	if(!p_du_after)		return false;
	
	p_du_after->DataName = data_name;
	p_du_after->BrightMax = bright_max;
	p_du_after->BrightMin = bright_min;
	p_du_after->DispPage = disp_page;
	p_du_after->DispChannel = disp_channel;
	
	return true;
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�̃p�[�X�y�N�e�B�u�ϊ�
��    ��    �� : PerspectiveTransDataUnit
��          �� : long			number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				double			ti,tj 			(in)�摜�����s�ړ�
				double			tX,tY 			(in)�B���ʓ����s�ړ�
				double			theta 			(in)�B���ʓ���] [deg]
				double			phai  			(in)�J�����������X��[deg]
				double			psi   			(in)�J�����c�����X��[deg]
				double			fl_phisical		(in)�����Y�œ_����[mm]
				double			ccd_pix_size	(in)ccd1��f�T�C�Y[��m]
				bool			show_message	(in)�ϊ���Ƀ��b�Z�[�W��\�����邩
				double			*perspective_param	(out)�ϊ��p�����^�i�z���O���t�B�s��j
				double			magnify			(in)�g�嗦
				int				outRgnMode		(in)�摜����͂ݏo���������̏����̕��@(0:���̂܂� 1:�ŋߖT��f�̒l�ŕ��)
��    ��    �l : ��������� true
�@          �\ : �ϊ����ʂ́A���摜�������ɏ㏑�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::PerspectiveTransDataUnit(long number,
											 double ti, double tj, double tX, double tY, 
											 double theta, double phai, double psi, 
											 double fl_phisical, double ccd_pix_size, bool show_message, double *perspective_param, double magnify,int outRgnMode)
{
	return PerspectiveTransDataUnit(number,number,true,ti,tj,tX,tY,theta,phai,psi,fl_phisical,ccd_pix_size,show_message, perspective_param, magnify, outRgnMode);
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�̃p�[�X�y�N�e�B�u�ϊ�
��    ��    �� : PerspectiveTransDataUnit
��          �� : long			number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long			dst_data_number	(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 bool			do_clear_dst	(in)�i�[��摜����������x�N���A���邩�ǂ���
				double			ti,tj 			(in)�摜�����s�ړ�
				double			tX,tY 			(in)�B���ʓ����s�ړ�
				double			theta 			(in)�B���ʓ���] [deg]
				double			phai  			(in)�J�����������X��[deg]
				double			psi   			(in)�J�����c�����X��[deg]
				double			fl_phisical		(in)�����Y�œ_����[mm]
				double			ccd_pix_size	(in)ccd1��f�T�C�Y[��m]
				bool			show_message	(in)�ϊ���Ƀ��b�Z�[�W��\�����邩
				double			*perspective_param	(out)�ϊ��p�����^�i�z���O���t�B�s��j
				double			magnify			(in)�g�嗦
				int				outRgnMode		(in)�摜����͂ݏo���������̏����̕��@(0:���̂܂� 1:�ŋߖT��f�̒l�ŕ��)
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
2015.11.11   Y.Ikeda         �o�O�C��
2016.05.02   Y.Ikeda         �g�嗦�̃p�����^��ǉ�
********************************************************************/
bool	CPimpomAPI::PerspectiveTransDataUnit(long number, long dst_data_number, bool do_clear_dst,
	double ti, double tj, double tX, double tY,
	double theta, double phai, double psi,
	double fl_phisical, double ccd_pix_size, bool show_message, double *perspective_param, double magnify, int outRgnMode)
{
	CDataUnit	*p_du;
	if ((p_du = GetDataUnit(number)) == NULL)	return false;

	double fl;
	double p_param[8];
	double theta_rad, phai_rad, psi_rad;
	theta_rad = theta / 180 * PI;
	phai_rad = phai / 180 * PI;
	psi_rad = psi / 180 * PI;

	fl = fl_phisical / (ccd_pix_size / 1000 * sqrt(2.0));//�œ_����


														 //�����Y���S���W [C]
	double  C[3];
	C[0] = 0;
	C[1] = 0;
	C[2] = fl;


	//�ړ��O�i�������j�J�����p�����^�쐬***********************

	//�����p�����^ [A0]
	cv::Mat A0(3, 3, CV_64F);
	A0.at<double>(0, 0) = fl*magnify;		A0.at<double>(0, 1) = 0;				A0.at<double>(0, 2) = (double)p_du->DataSize.cx / 2.0 + ti;
	A0.at<double>(1, 0) = 0;				A0.at<double>(1, 1) = fl*magnify;		A0.at<double>(1, 2) = (double)p_du->DataSize.cy / 2.0 + tj;
	A0.at<double>(2, 0) = 0;				A0.at<double>(2, 1) = 0;				A0.at<double>(2, 2) = 1.;




	//�O���p�����^  [RT0] = [ E | C ]
	cv::Mat RT0(3, 4, CV_64F);
	RT0.at<double>(0, 0) = 1;	RT0.at<double>(0, 1) = 0;	RT0.at<double>(0, 2) = 0;	RT0.at<double>(0, 3) = C[0];
	RT0.at<double>(1, 0) = 0;	RT0.at<double>(1, 1) = 1;	RT0.at<double>(1, 2) = 0;	RT0.at<double>(1, 3) = C[1];
	RT0.at<double>(2, 0) = 0;	RT0.at<double>(2, 1) = 0;	RT0.at<double>(2, 2) = 1;	RT0.at<double>(2, 3) = C[2];

	//�v���W�F�N�V�����s�� [P0]=[A0][RT0]
	cv::Mat P0 = A0*RT0;


	//�ړ���J�����p�����^�쐬***********************

	//�����p�����^ [A]
	cv::Mat A(3, 3, CV_64F);
	A.at<double>(0, 0) = fl;	A.at<double>(0, 1) = 0;		A.at<double>(0, 2) = (double)p_du->DataSize.cx / 2.0 + tX;
	A.at<double>(1, 0) = 0;		A.at<double>(1, 1) = fl;	A.at<double>(1, 2) = (double)p_du->DataSize.cy / 2.0 + tY;
	A.at<double>(2, 0) = 0;		A.at<double>(2, 1) = 0;		A.at<double>(2, 2) = 1.0;

	//��]�s�� [R]=[R1][R2][R3]
	cv::Mat R1(3, 3, CV_64F);
	R1.at<double>(0, 0) = cos(theta_rad);	R1.at<double>(0, 1) = -sin(theta_rad);	R1.at<double>(0, 2) = 0;
	R1.at<double>(1, 0) = sin(theta_rad);	R1.at<double>(1, 1) = cos(theta_rad);	R1.at<double>(1, 2) = 0;
	R1.at<double>(2, 0) = 0;				R1.at<double>(2, 1) = 0;				R1.at<double>(2, 2) = 1.0;

	cv::Mat R2(3, 3, CV_64F);
	R2.at<double>(0, 0) = cos(phai_rad);	R2.at<double>(0, 1) = 0;				R2.at<double>(0, 2) = -sin(phai_rad);
	R2.at<double>(1, 0) = 0;				R2.at<double>(1, 1) = 1;				R2.at<double>(1, 2) = 0;
	R2.at<double>(2, 0) = sin(phai_rad);	R2.at<double>(2, 1) = 0;				R2.at<double>(2, 2) = cos(phai_rad);

	cv::Mat R3(3, 3, CV_64F);
	R3.at<double>(0, 0) = 1;				R3.at<double>(0, 1) = 0;				R3.at<double>(0, 2) = 0;
	R3.at<double>(1, 0) = 0;				R3.at<double>(1, 1) = cos(psi_rad); 	R3.at<double>(1, 2) = -sin(psi_rad);
	R3.at<double>(2, 0) = 0;				R3.at<double>(2, 1) = sin(psi_rad);		R3.at<double>(2, 2) = cos(psi_rad);

	cv::Mat TMP1 = R1*R2;
	cv::Mat R = TMP1*R3;


	//�O���p�����^ [RT] = [R | C]
	cv::Mat RT(3, 4, CV_64F);
	RT.at<double>(0, 0) = R.at<double>(0, 0);		RT.at<double>(0, 1) = R.at<double>(0, 1);		RT.at<double>(0, 2) = R.at<double>(0, 2);		RT.at<double>(0, 3) = C[0];
	RT.at<double>(1, 0) = R.at<double>(1, 0);		RT.at<double>(1, 1) = R.at<double>(1, 1);		RT.at<double>(1, 2) = R.at<double>(1, 2);		RT.at<double>(1, 3) = C[1];
	RT.at<double>(2, 0) = R.at<double>(2, 0);		RT.at<double>(2, 1) = R.at<double>(2, 1);		RT.at<double>(2, 2) = R.at<double>(2, 2);		RT.at<double>(2, 3) = C[2];

	//�v���W�F�N�V�����s�� [P] = [A][RT]
	cv::Mat P = A*RT;


	//�z���O���t�B�쐬***********************
	cv::Mat P_T = P.t();
	cv::Mat P_ = P_T*((P*P.t()).inv(cv::DECOMP_SVD));//�ړ���J�����v���W�F�N�V�����s��̈�ʉ��t�s�� [P_] = [P_T] [ [P][P_T] ]-1
	cv::Mat H = (P0*P_).inv(cv::DECOMP_SVD);//�z���O���t�B�s��


	double den = H.at<double>(2, 2);
	p_param[0] = H.at<double>(0, 0) / den;
	p_param[1] = H.at<double>(0, 1) / den;
	p_param[2] = H.at<double>(0, 2) / den;
	p_param[3] = H.at<double>(1, 0) / den;
	p_param[4] = H.at<double>(1, 1) / den;
	p_param[5] = H.at<double>(1, 2) / den;
	p_param[6] = H.at<double>(2, 0) / den;
	p_param[7] = H.at<double>(2, 1) / den;


	if (perspective_param) {
		memcpy(perspective_param, p_param, 8 * sizeof(double));
	}


	return PerspectiveTransDataUnit(number, dst_data_number, do_clear_dst, p_param, show_message, outRgnMode);
}


/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�̓����ϊ�(��]�A���s�ړ��A�g��)
��    ��    �� : IsoTarnsDataUnit
��          �� : long			number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				double			ti, tj			(in)���s�ړ�
				double			theta   		(in)��]�ړ�[deg]
				double			magnify 		(in)�g�嗦
				bool			show_message	(in)�ϊ��O�ɖ₢���킹�_�C�A���O��\�����邩
				double			magnify			(in)�g�嗦
				int				outRgnMode		(in)�摜����͂ݏo���������̏����̕��@(0:���̂܂� 1:�ŋߖT��f�̒l�ŕ��)
��    ��    �l : ��������� true
�@          �\ : �ϊ����ʂ́A���摜�������ɏ㏑�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::IsoTransDataUnit(long number, double ti, double tj, double theta, double magnify, bool show_message, double *perspective_param, int outRgnMode)
{
	if(magnify==0)	return false;

	return PerspectiveTransDataUnit(number, ti, tj, 0,0, theta, 0,0, 16, 7.4 ,show_message, perspective_param, magnify, outRgnMode);
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�̓����ϊ�(��]�A���s�ړ��A�g��)
��    ��    �� : IsoTarnsDataUnit
��          �� : long			number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long			dst_data_number	(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 bool			do_clear_dst	(in)�i�[��摜����������x�N���A���邩�ǂ���
				double			ti, tj			(in)���s�ړ�
				double			theta   		(in)��]�ړ�[deg]
				double			magnify 		(in)�g�嗦
				bool			show_message	(in)�ϊ��O�ɖ₢���킹�_�C�A���O��\�����邩
				int				outRgnMode		(in)�摜����͂ݏo���������̏����̕��@(0:���̂܂� 1:�ŋߖT��f�̒l�ŕ��)
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::IsoTransDataUnit(long number, long dst_data_number, bool do_clear_dst, 
									 double ti, double tj, double theta, double magnify, bool show_message, double *perspective_param,int outRgnMode)
{
	return PerspectiveTransDataUnit(number, dst_data_number, do_clear_dst, ti, tj, 0,0, theta, 0,0, 16, 7.4,show_message, perspective_param, magnify, outRgnMode);


}

/********************************************************************
�@  �\  ��  �� : //�p�[�X�y�N�e�B�u���W�ϊ�
��    ��    �� : PerspectiveTransCoor
��          �� : double			p_param[8]		(in)�ϊ��p�����^�i�z���O���t�B�s��j
				 double			srcX			(in)�ϊ��O���W
				 double			srcY			(in)�ϊ��O���W
				 double			pDstX			(out)�ϊ�����W
				 double			pDstY			(out)�ϊ�����W
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::PerspectiveTransCoor(double perspective_param[8], double srcX, double srcY, double *pDstX, double *pDstY)
{

	double p_param[8] = { 0 };

	cv::Mat H(3, 3, CV_64F);
	H.at<double>(0, 0) = perspective_param[0];	H.at<double>(0, 1) = perspective_param[1];	H.at<double>(0, 2) = perspective_param[2];
	H.at<double>(1, 0) = perspective_param[3];	H.at<double>(1, 1) = perspective_param[4];	H.at<double>(1, 2) = perspective_param[5];
	H.at<double>(2, 0) = perspective_param[6];	H.at<double>(2, 1) = perspective_param[7];	H.at<double>(2, 2) = 1.0;

	cv::Mat H_ = H.inv(cv::DECOMP_SVD);//�t�s��

	double den = H_.at<double>(2, 2);
	p_param[0] = H_.at<double>(0, 0) / den;
	p_param[1] = H_.at<double>(0, 1) / den;
	p_param[2] = H_.at<double>(0, 2) / den;
	p_param[3] = H_.at<double>(1, 0) / den;
	p_param[4] = H_.at<double>(1, 1) / den;
	p_param[5] = H_.at<double>(1, 2) / den;
	p_param[6] = H_.at<double>(2, 0) / den;
	p_param[7] = H_.at<double>(2, 1) / den;


	double denom = p_param[6] * srcX + p_param[7] * srcY + 1.0;
	*pDstX = (p_param[0] * srcX + p_param[1] * srcY + p_param[2]) / denom;
	*pDstY = (p_param[3] * srcX + p_param[4] * srcY + p_param[5]) / denom;


	return true;
}


/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�̃N���b�s���O
��    ��    �� : ClipDataUnit
��          �� : long			number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CRect			clip_rect		(in)�N���b�s���O�͈�
				bool			show_message	(in)�ϊ��O�ɖ₢���킹�_�C�A���O��\�����邩
��    ��    �l : ��������� true
�@          �\ : �ϊ����ʂ́A���摜�������ɏ㏑�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::ClipDataUnit(long number, CRect clip_rect, bool show_message)
{
	return ClipDataUnit(number, number, true, clip_rect, show_message);
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�̃N���b�s���O
��    ��    �� : ResizeDataUnit
��          �� : long			number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long			dst_data_number	(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 bool			do_clear_dst	(in)�i�[��摜����������x�N���A���邩�ǂ���
				CRect			clip_rect		(in)�N���b�s���O�͈�
				bool			show_message	(in)�ϊ��O�ɖ₢���킹�_�C�A���O��\�����邩
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::ClipDataUnit(long number, long dst_data_number, bool do_clear_dst, CRect clip_rect, bool show_message)
{
	bool	ret;
	CDataUnit	*p_du, *p_du_after;
	CString		data_name;
	float		bright_max, bright_min;
	int			disp_page, disp_channel;
	bool		use_workarea;
	CWorkArea	work_area;


	if( (p_du = GetDataUnit(number) )==NULL)	return false;

	if(dst_data_number==CURRENT_IMAGE){
		dst_data_number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��
	}
	else if(dst_data_number<0 || dst_data_number>= GetDataUnitNumber())
	{
		return false;
	}


	data_name = p_du->DataName;
	bright_max = p_du->BrightMax;
	bright_min = p_du->BrightMin;
	disp_page = p_du->DispPage;
	disp_channel = p_du->DispChannel;
	work_area.Copy(&p_du->WorkArea);
	use_workarea = p_du->DoUseWorkArea;

	if(show_message){
		if( GetDataUnit(dst_data_number) ){
			CString str;
			str.Format("�摜������%d�͏㏑������܂�",dst_data_number);
			if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONINFORMATION ) != IDOK ){
				return false;
			}
		}
	}


	//�ϊ����s
	ret=false;
	switch(p_du->DataType)
	{
		case BYTE_FORMAT:
			//not break
		case THREE_D_FORMAT:
			//not break
		case RGB_FORMAT:
			//not break
		case RGB_3D_FORMAT:
			ret= clip_data(this, number, dst_data_number, do_clear_dst, p_du->pByteData, clip_rect);
			break;

		case SHORT16_FORMAT:
			ret= clip_data(this, number, dst_data_number, do_clear_dst, p_du->pShortData, clip_rect);
			break;

		case WORD_FORMAT:
			ret= clip_data(this, number, dst_data_number, do_clear_dst, p_du->pWordData, clip_rect);
			break;

		case LONG32_FORMAT:
			ret= clip_data(this, number, dst_data_number, do_clear_dst, p_du->pLongData, clip_rect);
			break;

		case DWORD_FORMAT:
			ret= clip_data(this, number, dst_data_number, do_clear_dst, p_du->pDwordData, clip_rect);
			break;

		case FLOAT_FORMAT:
			//not break
		case COMPLEX_FORMAT:
			//not break
		case XYZ_FORMAT:
			//not break
		case XY_FORMAT:
			//not break
		case FLOAT_3D_FORMAT:
			ret= clip_data(this, number, dst_data_number, do_clear_dst, p_du->pFloatData, clip_rect);
			break;
	}
	if(!ret)	return false;

	//�\���Ȃǂ����ɖ߂�
	p_du_after = GetDataUnit(dst_data_number);
	if(!p_du_after)		return false;
	
	p_du_after->DataName = data_name;
	p_du_after->BrightMax = bright_max;
	p_du_after->BrightMin = bright_min;
	p_du_after->DispPage = disp_page;
	p_du_after->DispChannel = disp_channel;

	//��Ɨ̈���R�s�[
	work_area.Move(-clip_rect.left, -clip_rect.top);
	p_du_after->WorkArea.Copy(&work_area);
	p_du_after->DoUseWorkArea = use_workarea;
	
	return true;
}