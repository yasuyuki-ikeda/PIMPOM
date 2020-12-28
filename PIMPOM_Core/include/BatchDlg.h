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
//CBatchDlg
//
/*******************************************************************/
#pragma once

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "afxwin.h"
#include "afxcmn.h"


///*************************************
//         �N���X   ��   �`         
///*************************************
class CBatchDlg : public CDialog
{
	DECLARE_DYNAMIC(CBatchDlg)

	enum { IDD = IDD_BATCH_DLG };// �_�C�A���O �f�[�^

public:
	CBatchDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CBatchDlg();
	void BackGroundProc();
	void GetBatchInput(BATCH_INPUT *p_in_param, int *p_file_no = NULL);
	void SetBatchInOut(BATCH_INPUT *p_in_param, BATCH_OUTPUT *p_out_param);


protected:
	CListCtrl m_list_contol;//���X�g�R���g���[��
	bool m_sort_dir_flg;//���X�g�̃\�[�g�����i�w�b�_�������j
	int m_iSubItem;//�����ꂽ���X�g�̃w�b�_�̍��ڔԍ�
	CProgressCtrl m_progress;//�v���O���X�o�[
	HANDLE hThread;//���C���v���Z�X�̃X���b�h

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	bool	single_execute(bool auto_exec_flg);//1����s
	void	show_dlg_item(int executing_mode);//�_�C�A���O�A�C�e����\������
	void	init_list();//���X�g�R���g���[����������
	void	add_list_item(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param=NULL);//���X�g�A�C�e����ǉ�
	void	set_list_item(int n, BATCH_INPUT *in_param, BATCH_OUTPUT *out_param=NULL);//���X�g�A�C�e����ݒ�
	int		get_selected_list_item();//�I�𒆂̃��X�g�A�C�e���ԍ����擾
	void	select_list_item(int n);//���X�g�A�C�e����I��
	bool	load_image_list(CString filename, char splitter=',');//�摜�ꗗ�̃t�@�C�����J��
	void	arrange_dlg_item();//�_�C�A���O�A�C�e���̈ړ�
	static int CALLBACK compare_func(LPARAM param1, LPARAM param2, LPARAM param3);



//���b�Z�[�W�n���h��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonBatchFileOpen();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButtonBatchFileDelete();
	afx_msg void OnBnClickedButtonBatchFileClear();
	afx_msg void OnBnClickedBatchExecSingle();
	afx_msg void OnBnClickedBatchExecSinglePrev();
	afx_msg void OnBnClickedBatchExecSingleNext();
	afx_msg void OnBnClickedButtonBatchFileOut();
	afx_msg void OnBnClickedButtonBatchFileIn();
	afx_msg void OnNMDblclkListBatch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickListBatch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListBatch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckBatchExec();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonBatchRoot();
	afx_msg void OnBnClickedButtonChangeExecuteRule();
};
