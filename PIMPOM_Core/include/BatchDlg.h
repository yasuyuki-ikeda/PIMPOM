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
//         インクルードファイル         
//**************************************
#include "afxwin.h"
#include "afxcmn.h"


///*************************************
//         クラス   定   義         
///*************************************
class CBatchDlg : public CDialog
{
	DECLARE_DYNAMIC(CBatchDlg)

	enum { IDD = IDD_BATCH_DLG };// ダイアログ データ

public:
	CBatchDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CBatchDlg();
	void BackGroundProc();
	void GetBatchInput(BATCH_INPUT *p_in_param, int *p_file_no = NULL);
	void SetBatchInOut(BATCH_INPUT *p_in_param, BATCH_OUTPUT *p_out_param);


protected:
	CListCtrl m_list_contol;//リストコントロール
	bool m_sort_dir_flg;//リストのソート方向（ヘッダ押下時）
	int m_iSubItem;//押されたリストのヘッダの項目番号
	CProgressCtrl m_progress;//プログレスバー
	HANDLE hThread;//メインプロセスのスレッド

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	bool	single_execute(bool auto_exec_flg);//1回実行
	void	show_dlg_item(int executing_mode);//ダイアログアイテムを表示する
	void	init_list();//リストコントロールを初期化
	void	add_list_item(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param=NULL);//リストアイテムを追加
	void	set_list_item(int n, BATCH_INPUT *in_param, BATCH_OUTPUT *out_param=NULL);//リストアイテムを設定
	int		get_selected_list_item();//選択中のリストアイテム番号を取得
	void	select_list_item(int n);//リストアイテムを選択
	bool	load_image_list(CString filename, char splitter=',');//画像一覧のファイルを開く
	void	arrange_dlg_item();//ダイアログアイテムの移動
	static int CALLBACK compare_func(LPARAM param1, LPARAM param2, LPARAM param3);



//メッセージハンドラ
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
