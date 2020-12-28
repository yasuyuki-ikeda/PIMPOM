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
//�����t�H���W
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "template\filter_erosion_dilation.h"

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


/********************************************************************
�@  �\  ��  �� :  ���k�t�B���^
��    ��    �� : FilterErosion
��          �� : long		image_num	(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long		dst_data_number	(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		filter_size	(in)�t�B���^�T�C�Y (�c���ʁX�̃T�C�Y���w��\)
				 bool		circleMask  (in)�~�`�}�X�N������
				 bool		toMultiImage	(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
				 bool       doErosion   (in)false�ɂ���Ɩc���t�B���^�ɂȂ�
��    ��    �l : ��������� true
�@          �\ : �t�B���^�������ʂ́A���摜�������ɏ㏑�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
2016.04.05   Y.Ikeda         2x2�T�C�Y�����s�ł���悤�ɂ����i���s��͉摜������f�����E���ɂ��j
********************************************************************/
bool  CPimpomAPI::FilterErosion(long image_num, long dst_data_number, CSize filter_size, bool circleMask, bool toMultiImage, bool doErosion)
{
	bool		ret=true;
	CDataUnit	*p_du;
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

	if(filter_size.cx<1 || filter_size.cy<1 || filter_size.cx>LANK_FILTER_SIZE_MAX || filter_size.cy>LANK_FILTER_SIZE_MAX ){//�t�B���^�T�C�Y�`�F�b�N
		return false;
	}



	//���ʂ�ʂ̉摜�������ɓ����ꍇ�A�܂��摜�f�[�^���R�s�[����
	if(image_num!=dst_data_number){
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
				ret = filter_erosion_dilataion_overwrite(p_du->pByteData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize, filter_size,calc_area, circleMask, doErosion);

			}else if(p_du->pShortData){//16bit�摜
				ret = filter_erosion_dilataion_overwrite(p_du->pShortData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize, filter_size, calc_area, circleMask, doErosion);

			}else if(p_du->pWordData){//16bit�摜
				filter_erosion_dilataion_overwrite(p_du->pWordData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize, filter_size, calc_area, circleMask, doErosion);

			}else if(p_du->pLongData){//32bit�摜
				filter_erosion_dilataion_overwrite(p_du->pLongData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize, filter_size, calc_area, circleMask, doErosion);

			}else if(p_du->pDwordData){//32bit�摜
				filter_erosion_dilataion_overwrite(p_du->pDwordData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize, filter_size, calc_area, circleMask, doErosion);

			}else if(p_du->pFloatData){//�����摜
				filter_erosion_dilataion_overwrite(p_du->pFloatData + imsize * (p * p_du->ChannelNumber + c),
							p_du->DataSize, filter_size, calc_area, circleMask, doErosion);
			}

		}
	}

	return true;
}



/********************************************************************
�@  �\  ��  �� :  �c���t�B���^
��    ��    �� : FilterDilation
��          �� : long		image_num	(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long		dst_data_number	(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		filter_size	(in)�t�B���^�T�C�Y (�c���ʁX�̃T�C�Y���w��\)
				 bool		circleMask  (in)�~�`�}�X�N������
				 bool		toMultiImage	(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
��    ��    �l : ��������� true
�@          �\ : �t�B���^�������ʂ́A���摜�������ɏ㏑�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
2016.04.05   Y.Ikeda         2x2�T�C�Y�����s�ł���悤�ɂ����i���s��͉摜������f�����E���ɂ��j
********************************************************************/
bool  CPimpomAPI::FilterDilation(long image_num, long dst_data_number, CSize filter_size, bool circleMask, bool toMultiImage)
{
	return FilterErosion(image_num, dst_data_number, filter_size, circleMask, toMultiImage, false);
}


/********************************************************************
�@  �\  ��  �� :  �I�v�[�v�j���O�t�B���^
��    ��    �� : FilterOpening
��          �� : long		image_num	(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long		dst_data_number	(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		filter_size	(in)�t�B���^�T�C�Y
				  bool		circleMask  (in)�~�`�}�X�N������
				  bool		toMultiImage	(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool  CPimpomAPI::FilterOpening(long image_num, long dst_data_number, CSize filter_size, bool circleMask, bool toMultiImage)
{
	bool ret;
	
	ret = FilterErosion(image_num, dst_data_number, filter_size, circleMask, toMultiImage);
	if(!ret)	return false;

	ret = FilterDilation(dst_data_number, dst_data_number, filter_size, circleMask, toMultiImage);
	if(!ret)	return false;

	return true;
}


/********************************************************************
�@  �\  ��  �� :  �N���[�W���O�t�B���^
��    ��    �� : FilterClosing
��          �� : long		image_num	(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long		dst_data_number	(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		filter_size	(in)�t�B���^�T�C�Y
				 bool		circleMask  (in)�~�`�}�X�N������
				 bool		toMultiImage	(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool  CPimpomAPI::FilterClosing(long image_num, long dst_data_number, CSize filter_size, bool circleMask, bool toMultiImage)
{
	bool ret;

	ret = FilterDilation(image_num, dst_data_number, filter_size, circleMask, toMultiImage);
	if(!ret)	return false;

	ret = FilterErosion(dst_data_number, dst_data_number, filter_size, circleMask, toMultiImage);
	if(!ret)	return false;

	return true;
}
