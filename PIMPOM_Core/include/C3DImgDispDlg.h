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
//C3DImgDispDlg
//3D�t�H�[�}�b�g�f�[�^�̕\���ƕҏW
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#pragma once
#include "global_define.h"
#include "global_typedef.h"
#include "afxcmn.h"

///*************************************
//         �}  �N  ��   ��   �`         
///*************************************
#define  TD_IMAGE_DISP_MAX		(384)	//��x�ɕ\���ł���ő�摜��
#define  TD_IMAGE_DISP_OFFSET_X	(20)
#define  TD_IMAGE_DISP_OFFSET_Y	(50)

// *************************************
//         �\  ��  ��   ��   �`         
// *************************************

//3D�摜�f�[�^�\���p�\����
typedef struct _TD_IMAGE_DISP_STRUCT
{
	int		pageNumber;//�\������摜�f�[�^�̃y�[�W�ԍ�
	int		dispOffsetX;//�\���ʒux
	int		dispOffsetY;//�\���ʒuy
	int		dispWidth;//�\����
	int		dispHeight;//�\������
	BYTE	*pDispBuffers;//�\���p�������擪�|�C���^
	char	mask;//�}�X�N

}TD_IMAGE_DISP_STRUCT;


///*************************************
//         �N���X   ��   �`         
///*************************************
class C3DImgDispDlg : public CDialog
{
	DECLARE_DYNAMIC(C3DImgDispDlg)

	enum { IDD = IDD_3D_IMAGE_DISP_DLG };// �_�C�A���O �f�[�^

public:
	C3DImgDispDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~C3DImgDispDlg();
	bool DispImage(int image_number=-1);


protected:
	int		imageNumber;//�\������f�[�^�ԍ�
	TD_IMAGE_DISP_STRUCT DispStruct[TD_IMAGE_DISP_MAX];
	BYTE	*pMemories;//�\���p������
	CSliderCtrl m_slider;//�\���摜�I��p�X���C�_�[

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	int		arrange_images();
	void	clear_buffer();
	bool	copy_disp_buffers(int image_number, int page_offset = 0, int disp_cnt = TD_IMAGE_DISP_MAX);
	bool	draw_images(int image_number);
	int		set_slider(int image_number);


//���b�Z�[�W�n���h��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButton3dImgDispScale();
	afx_msg void OnBnClickedButton3dImgDispScale2();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonDispMinmax();
	afx_msg void OnBnClickedCheckMask();
};
