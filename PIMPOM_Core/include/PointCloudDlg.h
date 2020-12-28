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
//CPointCloudDlg
//�O�����_�Q�\���E�B���h�E
/*******************************************************************/
#pragma once

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "DataUnit.h"

///*************************************
//         �N���X   ��   �`         
///*************************************
class CPointCloudDlg : public CDialog
{
	enum { IDD = IDD_POINT_CLOUD_DLG };// �_�C�A���O �f�[�^

public:
	CPointCloudDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	void RotationTheta(float theta);////X������̉�]
	void RotationPhai(float phai);////Y������̉�]
	void Rendering(CSize sampling);//�_�Q���X�N���[��(�r�b�g�}�b�v)�ɕ`��
	int CreatePointCloud(CDataUnit *p_du);


protected:
	CSize	data_size;//�摜�f�[�^�̃T�C�Y
	long	bmp_data_width;//�摜�f�[�^�̕\���o�b�t�@�̃T�C�Y
	float	*p_transformed_x;//�ړ����x���W�i�z��j
	float	*p_transformed_y;//�ړ����y���W�i�z��j
	float	*p_transformed_z;//�ړ����z���W�i�z��j
	float	*p_original_z;//�ړ��O��z���W�i�z��j
	BYTE	*p_mask;//�}�X�N�̐擪�|�C���^
	BYTE	*p_brightness;//�f�[�^�_�̕\���Z�x�i�z��j
	float	*pZbuffer;//Z�o�b�t�@
	BYTE	*p_pixel_flg;//�X�N���[���̓�����f�Ɍ�����f�[�^�_�����邱�Ƃ�\���t���O�i�z��j
	BYTE	*p_screen_bmp_data;//�X�N���[����\���r�b�g�}�b�v�̃f�[�^�����̐擪�|�C���^
	BITMAPINFO		*p_screen_bmp_info;//�X�N���[����\���r�b�g�}�b�v�̃w�b�_����
	float			scale;//�k��
	CPoint			translate;//���S����̂���
	bool			is_druging;//�h���b�O�����ǂ�����\���t���O
	bool			is_draging_mbutton;//�����h���b�O�����ǂ�����\���t���O
	CPoint			mouse_down_point;//�}�E�X�{�^�������������W�i�X�N���[�����W�j
	CSize			sampling_size;//�f�[�^�_�̃T���v�����O�Ԋu

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	void set_ctrl_type(short type);//�R���g���[���^�C�v�����肷��(type: 0����]   1���g��/�k��   2�����s�ړ�)
	short get_ctrl_type();//���݂̃R���g���[���^�C�v��Ԃ�( 0����]   1���g��/�k��   2�����s�ړ�)
	void draw_scale();//�X�P�[���i�k�ځj��`��
	void draw_screen();//�X�N���[���i�r�b�g�}�b�v�j��`��
	void init_bmp();//�X�N���[����\���r�b�g�}�b�v������������


//���b�Z�[�W�n���h��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point); 
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPointCloudCtrlRotate();
	afx_msg void OnPointCloudCtrlScale();
	afx_msg void OnPointCloudCtrlTranslate();
};
