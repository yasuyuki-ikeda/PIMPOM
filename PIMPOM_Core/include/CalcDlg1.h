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
//CCalcDlg
//演算式設定ウィンドウ
/*******************************************************************/
#pragma once

///*************************************
//         クラス   定   義         
///*************************************
class CCalcDlg : public CDialog
{
	enum { IDD = IDD_CALC_DLG };// ダイアログ データ

public:
	CCalcDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ



protected:
	CComboBox	m_combo_command;
	CProgressCtrl	m_prog;
	CBitmap	execbmp;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	bool	execute();//実行
	int		get_data_type_from_dlg();
	void	backup_all_com_str();//全てのコマンド文字列のバックアップをとる
	void	backup_com_str(CComboBox &combo);//指定されたコンボボックスからコマンド文字列のバックアップをとる
	void	show_dlg_item();//ダイアログアイテムを表示する
	bool	load_com_str(CComboBox &combo, CString param_name);
	bool	save_com_str(CComboBox &combo, CString param_name);


//メッセージハンドラ
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnCalcExec();
	afx_msg void OnBnClickedShowMacroHelp();
	afx_msg void OnBnClickedRadioNew();
	afx_msg void OnBnClickedRadioOverwrite();
	afx_msg void OnBnClickedRadioCalcOverwriteImage2();
};


