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
//�e��t�B���^
/*******************************************************************/
#pragma once

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "PIMPOM_API.h"
#include "filter.h"

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
				);




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�e���v���[�g�Ō^�Ȃ�(�S�Ă̌^�̉摜�ɑΉ�����)�֐�������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : �~���[���]
��    ��    �� : filter_mirror
��          �� : 
��    ��    �l : ��������� true
�@          �\ :  
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
void	filter_mirror(
					  DATA *p_src_data, //(in)���͉摜�z��̐擪�|�C���^
					  DATA *p_dst_data,	//(out)�o�͉摜�z��̐擪�|�C���^
					  SIZE image_size,	//(in)�摜�T�C�Y
					  RECT work_area,	//(in)�����̈�
					  int dir			//(in)0:���E���] 1:�㉺���] 2:180�����]
					  )
{
	for(int j = work_area.top ; j<=work_area.bottom ; j++){
		for(int i = work_area.left ; i<=work_area.right ; i++){
			if(dir==0){//���E���]
				*(p_dst_data + i + j*image_size.cx) = *(p_src_data + (image_size.cx-1-i) + j*image_size.cx);
			}else if(dir==1){//�㉺���]
				*(p_dst_data + i + j*image_size.cx) = *(p_src_data + i + (image_size.cy-1-j)*image_size.cx);
			}else{//180�����]
				*(p_dst_data + i + j*image_size.cx) = *(p_src_data + (image_size.cx-1-i) + (image_size.cy-1-j)*image_size.cx);
			}
		}
	}
}

/********************************************************************
�@  �\  ��  �� : �~���[���]
��    ��    �� : filter_mirror_overwrite
��          �� : 
��    ��    �l : ��������� true
�@          �\ :   ���ʂ����摜�̃�������ɏ㏑������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
void	filter_mirror_overwrite(
								DATA *p_src_data,	//(in/out)�Ώۉ摜�z��̐擪�|�C���^
								SIZE image_size,	//(in)�摜�T�C�Y
								RECT work_area,		//(in)�����̈�
								int dir				//(in)0:���E���] 1:�㉺���] 2:180�����]
								)
{
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//�o�b�t�@�������m��
	if(!p_buffer)	return;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//�o�b�t�@������

	//���s
	filter_mirror(p_src_data, p_buffer, image_size, work_area, dir);

	//�o�b�t�@�̒l���摜�������ɖ߂�
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;
}


/********************************************************************
�@  �\  ��  �� : �Z�x���]
��    ��    �� : filter_nega
��          �� : 
��    ��    �l : ��������� true
�@          �\ :  
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
void	filter_nega(
					DATA *p_src_data,	//(in)���͉摜�z��̐擪�|�C���^
					DATA *p_dst_data,	//(our)�o�͉摜�z��̐擪�|�C���^
					SIZE image_size,	//(in)�摜�T�C�Y
					RECT work_area		//(in)�����̈�
					)
{
	for(int j = work_area.top ; j<=work_area.bottom ; j++){
		for(int i = work_area.left ; i<=work_area.right ; i++){
			*(p_dst_data + i + j*image_size.cx) = 255 - *(p_src_data + i + j*image_size.cx);
		}
	}
}

/********************************************************************
�@  �\  ��  �� : �Z�x���]
��    ��    �� : filter_nega_overwrite
��          �� : 
��    ��    �l : ��������� true
�@          �\ :   ���ʂ����摜�̃�������ɏ㏑������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
void	filter_nega_overwrite(DATA *p_src_data, //(in/out)�Ώۉ摜�z��̐擪�|�C���^
							  SIZE image_size,	//(in)�摜�T�C�Y
							  RECT work_area	//(in)�����̈�
							  )
{
	DATA	*p_buffer=NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//�o�b�t�@�������m��
	if(!p_buffer)	return;

	memcpy(p_buffer, p_src_data, image_size.cx * image_size.cy * sizeof(DATA));//�o�b�t�@������

	//���s
	filter_nega(p_src_data, p_buffer, image_size, work_area);

	//�o�b�t�@�̒l���摜�������ɖ߂�
	memcpy(p_src_data, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;
}
