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
//CMarkDlg
//マークの設定ウィンドウ
/*******************************************************************/
#pragma once

///*************************************
//         クラス   定   義         
///*************************************
class CMarkDlg : public CDialog
{
	enum { IDD = IDD_MARK_DLG };// ダイアログ データ

// コンストラクション
public:
	CMarkDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	virtual ~CMarkDlg();
	int  GetMarkEditType();
	void SetMarkEditType(int type);
	void OnMouseDownOnImage(float point_x, float point_y);
	void OnMouseDrugOnImage(float point0_x, float point0_y, float point1_x, float point1_y);
	void OnMouseUpOnImage(float point0_x, float point0_y, float point1_x, float point1_y);
	void OnDrawOnImage();
	void ShowMarkVal(long num);
	
	
	

protected:
	CListCtrl m_list_contol;//リストコントロール
	int m_iSubItem;//押されたリストのヘッダの項目番号
	bool m_sort_dir_flg;//リストのソート方向（ヘッダ押下時）
	CBitmap	delbmp, editbmp0, editbmp1, clearbmp, editbmp, idbmp;
	int   highlight_mark_id=-1;//ハイライト表示するマーク番号
	CToolTipCtrl	m_toolTip;


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	void show_dlg_item();
	void init_list();
	int	get_selected_list_item();
	void select_list_item(int n);
	static int CALLBACK compare_func(LPARAM param1, LPARAM param2, LPARAM param3);
	void SelectMarkFromPoint(long num, double x, double y);

	
//メッセージハンドラ
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCheckMarkNumDisp();
	afx_msg void OnButtonMarkAlldelete();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	afx_msg void OnNMClickListMark(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickListMark(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonMarkDelete();
	afx_msg void OnBnClickedButtonMarkEdit();
	afx_msg void OnNMRClickListMark(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCheckMarkType0();
	afx_msg void OnCheckMarkType1();
};
