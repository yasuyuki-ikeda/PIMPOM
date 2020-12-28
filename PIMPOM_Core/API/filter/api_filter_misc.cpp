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
//�t�B���^
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "template\filter_misc.h"
#include "template\filter_convolution.h"

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


//�t�B���^�`��ɍ��킹���ȉ~�`��0/1�}�X�N���쐬����
void createMask(	CSize	filter_size,	//(in)�t�B���^�T�C�Y
					char	pMask[]			//(out)�}�X�N��0/1�z��
				)
{
	int cnt=0;
	for(int y=-filter_size.cy/2 ; y<=filter_size.cy/2 ; y++)
	{
		for(int x=-filter_size.cx/2 ; x<=filter_size.cx/2 ; x++)
		{
			//�ȉ~�`���l�������t�B���^�̐���
			int sx,sy;
			double dir2, nx, ny;
			sx = filter_size.cx/2;
			sy = filter_size.cy/2;
			nx= (double)x/(double)sx;
			ny= (double)y/(double)sy;
			dir2 = nx*nx + ny*ny;

			pMask[cnt++] = (dir2<=1);
		}
	}
}



/********************************************************************
�@  �\  ��  �� : ���E���]
��    ��    �� : FilterMirror
��          �� : long		image_num		(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long		dst_data_number	(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int		dir				(in)�������]�̕��� ( 0:��  1:�c 0/1�ȊO:180��)
				 bool		toMultiImage	(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
��    ��    �l : ��������� true
�@          �\ : �t�B���^�������ʂ́A���摜�������ɏ㏑�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool  CPimpomAPI::FilterMirror(long image_num, long dst_data_number, int dir, bool toMultiImage)
{
	CDataUnit	*p_du=NULL;
	long		imsize;
	CRect		calc_area;


	//�����`�F�b�N

	if(image_num == CURRENT_IMAGE){
		image_num = GetCurrentImageNumber();
	}else if(image_num >= GetDataUnitNumber() || image_num<0){
		return false;
	}

	if(dst_data_number == CURRENT_IMAGE){
		dst_data_number = GetCurrentImageNumber();
	}else if(dst_data_number >= GetDataUnitNumber() || dst_data_number<0){
		return false;
	}



	if(image_num!=dst_data_number){//�f�[�^�R�s�[
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


	for (int p = pS; p <= pE; p++)
	{
		for (int c = cS; c <= cE; c++)
		{
			if(p_du->pByteData){//8bit�摜
				filter_mirror_overwrite(p_du->pByteData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize,calc_area, dir);

			}else if(p_du->pShortData){//16bit�摜
				filter_mirror_overwrite(p_du->pShortData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize, calc_area, dir);

			}else if(p_du->pWordData){//16bit�摜
				filter_mirror_overwrite(p_du->pWordData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize, calc_area, dir);

			}else if(p_du->pLongData){//32bit�摜
				filter_mirror_overwrite(p_du->pLongData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize, calc_area, dir);

			}else if(p_du->pDwordData){//32bit�摜
				filter_mirror_overwrite(p_du->pDwordData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize, calc_area, dir);

			}else if(p_du->pFloatData){//�����摜
				filter_mirror_overwrite(p_du->pFloatData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize, calc_area, dir);
			}
		}
	}

	return true;
}



/********************************************************************
�@  �\  ��  �� : ���x���]
��    ��    �� : FilterNega
��          �� : long		image_num	(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long		dst_data_number	(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 bool		toMultiImage	(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
��    ��    �l : ��������� true
�@          �\ : �t�B���^�������ʂ́A���摜�������ɏ㏑�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool  CPimpomAPI::FilterNega(long image_num, 
							long dst_data_number, 
							bool toMultiImage
							)
{
	CDataUnit	*p_du=NULL;
	long		imsize;
	CRect		calc_area;


	//�����`�F�b�N

	if(image_num == CURRENT_IMAGE){
		image_num = GetCurrentImageNumber();
	}else if(image_num >= GetDataUnitNumber() || image_num<0){
		return false;
	}

	if(dst_data_number == CURRENT_IMAGE){
		dst_data_number = GetCurrentImageNumber();
	}else if(dst_data_number >= GetDataUnitNumber() || dst_data_number<0){
		return false;
	}


	if(image_num!=dst_data_number){//�f�[�^�R�s�[
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


	for (int p = pS; p <= pE; p++)
	{
		for (int c = cS; c <= cE; c++)
		{
			if(p_du->pByteData){//8bit�摜
				filter_nega_overwrite(p_du->pByteData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize,calc_area);

			}else if(p_du->pShortData){//16bit�摜
				filter_nega_overwrite(p_du->pShortData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize, calc_area);

			}else if(p_du->pWordData){//16bit�摜
				filter_nega_overwrite(p_du->pWordData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize, calc_area);

			}else if(p_du->pLongData){//32bit�摜
				filter_nega_overwrite(p_du->pLongData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize, calc_area);

			}else if(p_du->pDwordData){//32bit�摜
				filter_nega_overwrite(p_du->pDwordData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize, calc_area);

			}else if(p_du->pFloatData){//�����摜
				filter_nega_overwrite(p_du->pFloatData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize, calc_area);
			}
		}
	}

	return true;
}




/********************************************************************
�@  �\  ��  �� : ��݂��݃t�B���^
��    ��    �� : FilterConvolution
��          �� : long		image_num		(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long		dst_data_number	(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		kernel_size		(in)�J�[�l���T�C�Y
				 float		*pkernel		(in)�J�[�l���l�̔z��
				 float		offset			(in)�I�t�Z�b�g
				 float		contrast		(in)�R���g���X�g
				 int		pad_type		(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
				 bool		toMultiImage	(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
��    ��    �l : ��������� true
�@          �\ : �t�B���^�������ʂ́A���摜�������ɏ㏑�������
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool  CPimpomAPI::FilterConvolution(
							long		image_num, 
							long		dst_data_number, 
							CSize		kernel_size,
							float		*pKernel ,
							float		offset,	
							float		contrast,
							int			pad_type,
							bool		toMultiImage
							)
{
	bool ret = true;
	CDataUnit	*p_du = NULL;
	long		imsize;
	CRect		calc_area;


	//�����`�F�b�N

	if (image_num == CURRENT_IMAGE){
		image_num = GetCurrentImageNumber();
	}
	else if (image_num >= GetDataUnitNumber() || image_num<0){
		return false;
	}

	if (dst_data_number == CURRENT_IMAGE){
		dst_data_number = GetCurrentImageNumber();
	}
	else if (dst_data_number >= GetDataUnitNumber() || dst_data_number<0){
		return false;
	}


	if (image_num != dst_data_number){//�f�[�^�R�s�[
		if (!CopyDataUnit(image_num, dst_data_number)){
			return false;
		}
	}

	p_du = GetDataUnit(dst_data_number);
	if (!p_du)	return	false;
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


	for (int p = pS; p <= pE; p++)
	{
		for (int c = cS; c <= cE; c++)
		{
			if (p_du->pByteData){//8bit�摜
				ret = filter_convolution_overwrite(p_du->pByteData + imsize * (p * p_du->ChannelNumber + c),
					p_du->DataSize, kernel_size, pKernel, calc_area, offset, contrast, p_du->GetMaxLimitValue(), p_du->GetMinLimitValue(), pad_type);

			}
			else if (p_du->pShortData){//16bit�摜
				ret = filter_convolution_overwrite(p_du->pShortData + imsize * (p * p_du->ChannelNumber + c),
					p_du->DataSize, kernel_size, pKernel, calc_area, offset, contrast, p_du->GetMaxLimitValue(), p_du->GetMinLimitValue(), pad_type);

			}
			else if (p_du->pWordData){//16bit�摜
				ret = filter_convolution_overwrite(p_du->pWordData + imsize * (p * p_du->ChannelNumber + c),
					p_du->DataSize, kernel_size, pKernel, calc_area, offset, contrast, p_du->GetMaxLimitValue(), p_du->GetMinLimitValue(), pad_type);

			}
			else if (p_du->pLongData){//32bit�摜
				ret = filter_convolution_overwrite(p_du->pLongData + imsize * (p * p_du->ChannelNumber + c),
					p_du->DataSize, kernel_size, pKernel, calc_area, offset, contrast, p_du->GetMaxLimitValue(), p_du->GetMinLimitValue(), pad_type);

			}
			else if (p_du->pDwordData){//32bit�摜
				ret = filter_convolution_overwrite(p_du->pDwordData + imsize * (p * p_du->ChannelNumber + c),
					p_du->DataSize, kernel_size, pKernel, calc_area, offset, contrast, p_du->GetMaxLimitValue(), p_du->GetMinLimitValue(), pad_type);

			}
			else if (p_du->pFloatData){//�����摜
				ret = filter_convolution_overwrite(p_du->pFloatData + imsize * (p * p_du->ChannelNumber + c),
					p_du->DataSize, kernel_size, pKernel, calc_area, offset, contrast, p_du->GetMaxLimitValue(), p_du->GetMinLimitValue(), pad_type);
			}

			if (!ret){
				return false;
			}
		}
	}

	return true;
}
