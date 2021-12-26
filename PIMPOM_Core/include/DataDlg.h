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
//CDataDlg
//�摜�������ꗗ�\���E�B���h�E
/*******************************************************************/


//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#pragma once
#include "DataUnit.h"	
#include "ImageField.h"

///*************************************
//         �N���X   ��   �`         
///*************************************
class CDataDlg : public CDialog
{
	enum { IDD = IDD_DATA_DLG };// �_�C�A���O �f�[�^

public:
	CDataDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	long ChangeCurrentDataNumber(long number);//���ݑI�𒆂̉摜�f�[�^�̔ԍ���ύX����
	bool DataExistMessage(CString mes, int image_num);//�f�[�^�̑��ݒʒm
	void DrawImage(long number);//�w�肵���摜��`�悷��
	void DrawAllImages();//�S�Ẳ摜��`�悷��

	int  WindowHeight;//���̃E�B���h�E�̍����i�摜���������ɂ���ĕς��j
	

protected:
	bool			is_draging;//�}�E�X�h���b�O����\���t���O
	HBRUSH			bg_brush;//�w�i�F�̃u���V
	CImageField		**pp_image_disp_small;//���C���[�W�̃|�C���^�z��
	CPoint			mouse_point;//�}�E�X���W

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	void draw_image_info(long number = CURRENT_IMAGE);//�\���摜�̏���`�悷��i�ԍ��ƃf�[�^�̌`���j�i�f�[�^�ԍ��w��Ȃ��̏ꍇ�͌��ݑI�𒆂̃f�[�^�j
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) ;//�w�i�F�����肷��
	void draw_current_rect(long number, COLORREF color);//�\�����̉摜��������`��`�悷��
	int get_image_number_from_coordinate(CPoint coor);//�w�肵�����W�i�N���C�A���g�̈�j�ɕ\������Ă���摜�̔ԍ���Ԃ�
	void draw_background_rect(CRect rct);//�w�i�F�ŋ�`�`��
	CString clip_string(CDC *pdc, CString str, int max_width);//�������\�������ŃN���b�v����



//���b�Z�[�W�n���h��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDatamenuRename();
	afx_msg void OnDatamenuConvertType();
	afx_msg void OnDatamenuResize();
	afx_msg void OnDatamenuDelete();
	afx_msg void OnDatamenuSavebitmap();
	afx_msg void OnDatamenuUndo();
	afx_msg void OnDatamenuDisp3d();
};

