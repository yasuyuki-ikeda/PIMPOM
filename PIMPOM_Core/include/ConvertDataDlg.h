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
//CConvertDataDlg
//画像メモリの型変換のダイアログ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#pragma once
#include "afxwin.h"

///*************************************
//         クラス   定   義         
///*************************************
class CConvertDataDlg : public CDialog
{
	enum { IDD = IDD_CONVERT_DATA_DLG };// ダイアログ データ

public:
	CConvertDataDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ


protected:
	CComboBox m_combo_dst_type;
	CComboBox m_combo_src_type;
	CComboBox m_combo_src_prop;
	CComboBox m_combo_dst_prop;
	CComboBox m_combo_src_prop2;
	short	m_dst_type_table[20][2][20]; //[変換前タイプ][表示画像/データ][変換後タイプ]
	CString	m_data_type_name[20];

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	bool execute(int image_num);
	void make_cvt_type_table();
	void make_data_type_name();
	bool show_dlg_item(int image_num);
	bool show_dlg_item_src_props(int image_num);
	bool show_dlg_item_dst_type( int image_num);
	bool show_dlg_item_dst_props( int image_num);
	bool set_src_page_channel(int image_num);



//メッセージハンドラ
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboConvertDlgSrcType();
	afx_msg void OnCbnSelchangeComboConvertDlgSrcProp0();
	afx_msg void OnCbnSelendcancelComboConvertDlgSrcProp1();
	afx_msg void OnCbnSelchangeComboConvertDlgDstType();
	afx_msg void OnCbnSelchangeComboConvertDlgDstProp0();
	afx_msg void OnBnClickedButtonConvertDlgExecute();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTranslateDataButton1();
	afx_msg void OnBnClickedConvertDataButton1_1();
	afx_msg void OnTranslateDataButton2();
	afx_msg void OnTranslateDataButton3();
	afx_msg void OnTranslateDataButton4();
	afx_msg void OnTranslateDataButton5();
	afx_msg void OnBnClickedTranslateDataButton6();
	afx_msg void OnBnClickedTranslateDataButton7();
	afx_msg void OnBnClickedConvertDataButton8();
	afx_msg void OnBnClickedConvertDataButton9();
	afx_msg void OnBnClickedConvertDataButton10();
};

