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
//CIO3DDlg
//3�����摜�̓��o�̓_�C�A���O
/*******************************************************************/
#pragma once

///*************************************
//         �}  �N  ��   ��   �`         
///*************************************
#define		FILE_TYPE_RAW		(0)
#define		FILE_TYPE_BMP		(1)
#define		FILE_TYPE_FLOAT		(2)

///*************************************
//         �N���X   ��   �`         
///*************************************
class CIO3DDlg : public CDialog
{
	enum { IDD = IDD_IO_3D_DLG };// �_�C�A���O �f�[�^

public:
	CIO3DDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	bool DoFileExist(CString file_name);
	int DoModalLoadType(long data_number);
	int DoModalStoreType(long data_number);//�ۑ��^�C�v�̃_�C�A���O�Ƃ��ď���������


protected:
	bool	do_store;//�_�C�A���O�̃^�C�v ture �Ȃ�� �ۑ� / false �Ȃ�� �ǂݍ���
	long	target_image_number;//�ۑ�/�ǂݍ��݂̃^�[�Q�b�g�ƂȂ�摜�������̔ԍ�

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	void	change_filetype_radiobutton(int type);
	bool	check_bitmap_header(CString filename, BITMAPINFOHEADER &bmp_header);
	void	browse_for_load();
	void	setlist_for_load(CString path, CString file_name[], int file_number);
	void	browse_for_store();//�ۑ����[�h�ŎQ�ƃ{�^���������ꂽ���̏���
	void	disp_all_save_files();
	bool	load();
	bool    store();//�^�[�Q�b�g��3�����摜��A������raw�t�@�C���Ƃ��ĕۑ�����


//���b�Z�[�W�n���h��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnIo3dRefer();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedRadioIo3dRaw();
	afx_msg void OnBnClickedRadioIo3dBmp();
	afx_msg void OnBnClickedRadioIo3dFloat();
	afx_msg void OnEnChangeEditIo3dSerialName();
	afx_msg void OnBnClickedIo3dDlgFolderBrowse();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B
