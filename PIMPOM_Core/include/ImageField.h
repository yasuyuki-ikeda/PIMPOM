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
//CImageField
//�摜�̉�ʕ\���������N���X
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#pragma once
#include "DataUnit.h"

///*************************************
//         �}  �N  ��   ��   �`         
///*************************************
#define		NO_SIZE_DATA	( CSize(-1,-1) )
#define		NO_POINT_DATA	( CPoint(-1,-1) )
#define		NO_RECT_DATA	( CRect(-1,-1,-1,-1) )

///*************************************
//         �N���X   ��   �`         
///*************************************
class PIMPOM_EXT CImageField
{
public:
	CImageField();
	CImageField(HWND wnd, CSize field_size, CPoint field_coor);//�R���X�g���N�^
	virtual ~CImageField();//�f�X�g���N�^
	void DrawImage(CDataUnit *p_du, bool redraw_background);//�`�悷��
	void DrawPiece(CDataUnit *p_du, CPoint src_coor, CSize src_size, bool do_adjust_scale_as_field_size , bool do_update_scale, bool do_redraw_background);//�摜�f�[�^�̈ꕔ����`�悷��
	void DrawAdjustedImage(CDataUnit *p_du , bool do_update_scale);//ImageFiled�̑傫���ɍ��킹�Ċg��E�k�����ĕ`�悷��
	void ScaleImage(CDataUnit *p_du , float scale);//�摜�]�����̊g�嗦��ύX����
	void ScrollImage(CDataUnit *p_du , CPoint poi);//�摜�]�����̋�`�̈���X�N���[������
	bool GetSrcOffset(CDataUnit *p_du, CPoint *p_offset);//�摜�]�����̃I�t�Z�b�g�i��`�̈�̍���̍��W�j�𓾂�
	bool GetSrcSize(CDataUnit *p_du, CSize *p_src_size);//�摜�]�����̋�`�̈�̃T�C�Y���擾����
	bool GetSrcRect(CDataUnit *p_du, CRect *p_src_rect);//�摜�]�����̋�`�̈���擾����
	bool GetDestSize(CDataUnit *p_du, CSize *p_dest_size);//�摜�]����̋�`�̈�̃T�C�Y���擾����
	bool GetDestRect(CDataUnit *p_du, CRect *p_dest_rect);//�摜�]����̋�`�̈���擾����
	CRect GetFieldRect();//����ImageField������킷��`�𓾂�
	bool GetSrcCoor(CDataUnit *p_du ,  double dest_coor_x, double dest_coor_y , double *p_src_coor_x, double *p_src_coor_y);//�摜�]����i�E�C���h�E���j�̍��W����]�����i�摜�f�[�^���j�̍��W�ɕϊ�����
	bool GetDestCoor(CDataUnit *p_du , double src_coor_x ,double src_coor_y , double *p_dest_coor_x, double *p_dest_coor_y);//�摜�]�����i�摜�f�[�^���j�̍��W����]����i�E�C���h�E���j�̍��W�ɕϊ�����
	bool Resize(CSize size);
	bool Move(CPoint point);

	HWND			hImageWnd;//�摜��\������E�C���h�E�̃n���h��
	CSize			FieldSize;//�\���̈�̃T�C�Y
	CPoint			FieldCoordinate;//�\���̈�̍���̍��W

private:
	void fill_field(COLORREF color);//ImageFiled����P��F�œh��Ԃ�

};

