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
//CPythonBatchDlg
//Python�o�b�`���s�E�B���h�E
/*******************************************************************/
#pragma once


///*************************************
//         �N���X   ��   �`         
///*************************************

class CPythonBatchDlg : public CDialog
{
	DECLARE_DYNAMIC(CPythonBatchDlg)

	enum { IDD = IDD_PYTHON_BATCH_DLG };// �_�C�A���O �f�[�^


public:
	CPythonBatchDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CPythonBatchDlg();
	bool OnBatchExecute();


protected:
	CString			message;//���b�Z�[�W�{�b�N�X�ɕ\�����镶����
	bool			verCheck = false;//python���`�F�b�N�`�F�b�N���邩

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual  BOOL OnInitDialog();
	bool execCmd(CString cmd,CString currentdir);
	void arrange_dlg_item();


//���b�Z�[�W�n���h��
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedPythonBatchDlgBrowseFile();
	afx_msg void OnBnClickedButtonIdcPythonBatchDlgTrial();
	afx_msg void OnBnClickedButtonIdcPythonBatchDlgEdit(); 
	afx_msg void OnBnClickedButtonIdcPythonBatchDlgVerchk();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
