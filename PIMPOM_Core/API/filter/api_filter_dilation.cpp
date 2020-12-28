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
//�c���t�B���^
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "filter.h"

// *************************************
//         �}  �N  ��   ��   �`         
// *************************************
#define		BIGGER(A,B)			((A)>(B)) ? (A):(B)
#define		SMALLER(A,B)		((A)<(B)) ? (A):(B)

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
void createMask(CSize filter_size, char pMask[]);//�t�B���^�`��ɍ��킹���ȉ~�`��0/1�}�X�N���쐬����


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�e���v���[�g�Ō^�Ȃ�(�S�Ă̌^�̉摜�ɑΉ�����)�֐�������

// filter_size�ɋ������w��ł���悤�ɂ���(2x2�̎����̂���)
// �����̂Ƃ��͉~�`�}�X�N���������Ȃ�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class DATA>
void	filter_dilation(DATA *p_src_data, DATA *p_dst_data, CSize image_size, CSize filter_size, CRect work_area, bool circleMask)
{
	int	  i,j,k,l;
	float	maximum, val;
	CRect	area, comb;
	char	maskBuffer[1000]={0};


	//�v���̈�̐ݒ�
	area.left = BIGGER(work_area.left , 0);
	area.right = SMALLER(work_area.right , image_size.cx -1);
	area.top = BIGGER(work_area.top , 0);
	area.bottom = SMALLER(work_area.bottom , image_size.cy -1);


	//��݂��ݗ̈�̐ݒ�
	if(filter_size.cx%2==1){//��T�C�Y
		comb.left   = -filter_size.cx/2;
		comb.right  =  filter_size.cx/2;
	}else{//�����T�C�Y
		comb.left   = -filter_size.cx/2;
		comb.right  =  filter_size.cx/2-1;
		circleMask = false;
	}

	if(filter_size.cy%2==1){//��T�C�Y
		comb.top    = -filter_size.cy/2;
		comb.bottom =  filter_size.cy/2;
	}else{//�����T�C�Y
		comb.top    = -filter_size.cy/2;
		comb.bottom =  filter_size.cy/2-1;
		circleMask = false;
	}


	//�}�X�N����
	if(circleMask){
		createMask(filter_size, maskBuffer);
	}


	for(j = area.top ; j<=area.bottom ; j++)
	{
		for(i = area.left ; i<=area.right ; i++)
		{

			maximum = (double)*(p_src_data + i + j*image_size.cx);//�ő�l������
			char *ptr = maskBuffer;

			for(k=comb.top ; k<=comb.bottom ; k++)
			{
				for(l=comb.left ; l<=comb.right ; l++)
				{
					if(circleMask){
						if(*ptr++ == 0)		continue;//�}�X�N�O����
					}

					if( (i+l)<0 || (i+l)>=image_size.cx || (j+k)<0 || (j+k)>=image_size.cy )	continue;//�v���̈�O����


					val = (double)*(p_src_data + (i+l) + (j+k)*image_size.cx);

					if(val > maximum)//�ő�l�X�V
					{
						maximum = val;
					}
				}
			}
			*(p_dst_data + i + j*image_size.cx) = (DATA)maximum;
		}
	}
}


template <class DATA>
void	filter_dilation_wrap(DATA *p_src_data, CSize image_size, CSize filter_size, CRect work_area, bool circleMask)
{
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//�o�b�t�@�������m��
	if(!p_buffer)	return;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//�o�b�t�@������



	//���s
	filter_dilation(p_src_data,	p_buffer, image_size, filter_size, work_area, circleMask);

	//�o�b�t�@�̒l���摜�������ɖ߂�
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;
}


/********************************************************************
�@  �\  ��  �� :  �c���t�B���^
��    ��    �� : FilterDilation
��          �� : long		image_num	(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long		dst_data_number	(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		filter_size	(in)�t�B���^�T�C�Y
				 bool		circleMask  (in)�~�`�}�X�N������
��    ��    �l : ��������� true
�@          �\ : �t�B���^�������ʂ́A���摜�������ɏ㏑�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
2016.04.05   Y.Ikeda         2x2�T�C�Y�����s�ł���悤�ɂ����i���s��͉摜������f�����E���ɂ��j
********************************************************************/
bool  CPimpomAPI::FilterDilation(long image_num, long dst_data_number, CSize filter_size, bool circleMask)
{
	CDataUnit	*p_du=NULL;
	long		imsize;
	CRect		calc_area;

		if(image_num == CURRENT_IMAGE)	image_num = GetCurrentImageNumber();
		if(dst_data_number == CURRENT_IMAGE)	dst_data_number = GetCurrentImageNumber();


	if(image_num!=dst_data_number){//�f�[�^�R�s�[
		if(!CopyDataUnit(image_num, dst_data_number)){
			return false;
		}
	}

	p_du = GetDataUnit(dst_data_number);
		if(!p_du)	return	false;
	imsize = p_du->DataSize.cx * p_du->DataSize.cy;//�摜�̃T�C�Y�i��f���j
	calc_area = p_du->GetRectArea();//�����Ώۂ̋�`�͈�


		if(p_du->DispRGB == false || ( p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) )//�O���[�摜�ɑ΂��鏈��
		{
			if(p_du->pByteData){//8bit�摜
				filter_dilation_wrap(p_du->pByteData + imsize * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel),//������
							p_du->DataSize, filter_size, calc_area, circleMask);

			}else if(p_du->pShortData){//16bit�摜
				filter_dilation_wrap(p_du->pShortData + imsize * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel),
							p_du->DataSize, filter_size, calc_area, circleMask);

			}else if(p_du->pWordData){//16bit�摜
				filter_dilation_wrap(p_du->pWordData + imsize * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel),
							p_du->DataSize, filter_size, calc_area, circleMask);

			}else if(p_du->pLongData){//32bit�摜
				filter_dilation_wrap(p_du->pLongData + imsize * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel),
							p_du->DataSize, filter_size, calc_area, circleMask);

			}else if(p_du->pDwordData){//32bit�摜
				filter_dilation_wrap(p_du->pDwordData + imsize * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel),
							p_du->DataSize, filter_size, calc_area, circleMask);

			}else if(p_du->pFloatData){//�����摜
				filter_dilation_wrap(p_du->pFloatData + imsize * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel),//������
							p_du->DataSize, filter_size, calc_area, circleMask);
			}
		}
		else//RGB�摜�ɑ΂��鏈��
		{
			for(int c=0 ; c<3; c++){
				filter_dilation_wrap(p_du->pByteData + imsize * (p_du->DispPage * p_du->ChannelNumber + c),//������
							p_du->DataSize, filter_size, calc_area, circleMask);
			}
		}
		return true;
}


