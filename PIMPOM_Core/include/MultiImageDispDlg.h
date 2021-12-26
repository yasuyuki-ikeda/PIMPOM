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
//CMultiImageDispDlg
//�����摜�����\���E�B���h�E
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#pragma once
#include "global_define.h"
#include "global_typedef.h"
#include "afxwin.h"

///*************************************
//         �N���X   ��   �`         
///*************************************

class CMultiImageDispDlg : public CDialog
{
	DECLARE_DYNAMIC(CMultiImageDispDlg)

	enum { IDD = IDD_MULTI_IMAGE_DISP_DLG };// �_�C�A���O �f�[�^

public:
	CMultiImageDispDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual BOOL OnInitDialog();
	virtual ~CMultiImageDispDlg();
	void  DispMultiImage();


protected:
	int  m_selected_disp_image_no;//�I������Ă���摜
	CImageField		*p_disp_image_field[6];//�\������摜��ImageField
	int		m_disp_image_number[6];//�\������摜�������ԍ�
	bool	m_is_draging;//�h���b�O���ł��邱�Ƃ������t���O
	CPoint	m_drag_start_offset;//�h���b�O�J�n���W
	CPoint  m_last_clicked_point;//�h���b�O�J�n���̕\�����̉摜�������̃I�t�Z�b�g
	CComboBox m_cmb_disp_num;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	void	arrange_dlg_item();//�_�C�A���O�A�C�e���̈ړ�
	void	set_scrollbar(CScrollBar *sc, int page, int pos, int min, int max);//�X�N���[���o�[��ݒ肷��
	int		calc_image_regn(CRect imageRect[]);//�摜�\���̈���Z�o����
	long	control_scrollbar(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	void	synchro_disp_image(int dispImgNo);
	void	update_scrollbar(int dispImgNo);
	void	update_image_overlap(int dispImgNo);
	CDataUnit	*get_disp_image_data(int dispImgNo);
	void	get_disp_scroll_bar(int dispImgNo, CScrollBar **pp_h_scroll, CScrollBar **pp_v_scroll);
	void	scroll_image(int dispImageNo, int newX, int newY);


//���b�Z�[�W�n���h��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCheckMultiImgDlgSelect();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedButtonMultiImgDlgMagnify0();
	afx_msg void OnBnClickedButtonMultiImgDlgMagnify1();
	afx_msg void OnCbnSelchangeComboMultiImageDispNum();
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
};
