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
//���f�B�A���t�B���^
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

// *************************************
//         �O    ��    ��    ��         
// *************************************

// *************************************
//         ��    ��    �Q    ��         
// *************************************
void createMask(CSize filter_size, char pMask[]);//�t�B���^�`��ɍ��킹���ȉ~�`��0/1�}�X�N���쐬����


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �e���v���[�g�őS�Ă̌^�̉摜�ɑΉ��ł���悤�ɂ���
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : ���f�B�A���t�B���^
��    ��    �� : filter_median
��          �� : 
��    ��    �l : ��������� true
�@          �\ :  
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
bool	filter_median(
					  DATA	*p_src_data, 	//(in)���͉摜�z��̐擪�|�C���^
					  DATA	*p_dst_data, 	//(out)�o�͉摜�z��̐擪�|�C���^
					  SIZE image_size, 		//(in)�摜�T�C�Y
					  SIZE filter_size, 	//(in)�t�B���^�T�C�Y (�c���ʐݒ�\ �����̂Ƃ��͉~�`�}�X�N���������Ȃ�)
					  RECT work_area, 		//(in)�����̈�
					  bool	circleMask		//(in)�~�`�}�X�N������
					  )
{
	int	  valCnt;
	int	  median_num;
	float	val[LANK_FILTER_SIZE_MAX*LANK_FILTER_SIZE_MAX], v;
	RECT	comb;
	char	maskBuffer[LANK_FILTER_SIZE_MAX*LANK_FILTER_SIZE_MAX]={0};



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



	//���f�B�A���t�B���^���s
	for(int j = work_area.top ; j<=work_area.bottom ; j++)
	{
		for(int i = work_area.left ; i<=work_area.right ; i++)
		{
			valCnt=0;
			char *ptr = maskBuffer;

			for(int k=comb.top ; k<=comb.bottom ; k++)
			{
				for(int l=comb.left ; l<=comb.right ; l++)
				{
					if(circleMask){
						if(*ptr++ == 0)		continue;//�}�X�N�O����
					}

					if( (i+l)<0 || (i+l)>=image_size.cx || (j+k)<0 || (j+k)>=image_size.cy )	continue;//�v���̈�O����

					val[valCnt++] = (float)*(p_src_data + (i+l) + (j+k)*image_size.cx);
				}
			}

			//�\�[�g���Ē����l���o
			median_num = valCnt/2;

			for(int m=0 ; m<=median_num ; m++){
				for(int n=1 ; n<valCnt-m ; n++){
					if(val[n-1]<val[n]){
						v = val[n-1];
						val[n-1] = val[n];
						val[n] = v;
					}
				}
			}

			*(p_dst_data + i + j*image_size.cx) = (DATA)val[median_num];
		}
	}

	return true;
}

/********************************************************************
�@  �\  ��  �� : ���f�B�A���t�B���^
��    ��    �� : filter_median_overwrite
��          �� : 
��    ��    �l : ��������� true
�@          �\ :  ���ʂ����摜�̃�������ɏ㏑������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
bool	filter_median_overwrite(
								DATA	*p_src_data,	//(in/out)�Ώۉ摜�z��̐擪�|�C���^
								SIZE	image_size,		//(in)�摜�T�C�Y
								SIZE	filter_size,	//(in)�t�B���^�T�C�Y
								RECT	work_area,		//(in)�����̈�
								bool	circleMask		//(in)�~�`�}�X�N������
								)
{
	bool	ret=true;
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//�o�b�t�@�������m��
	if(!p_buffer)	return false;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//�o�b�t�@������

	//���s
	ret = filter_median(p_src_data,	p_buffer, image_size, filter_size, work_area, circleMask);

	//�o�b�t�@�̒l���摜�������ɖ߂�
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;
}

