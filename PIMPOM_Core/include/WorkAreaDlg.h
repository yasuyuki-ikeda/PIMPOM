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
//CWorkAreaDlg
//作業領域設定ウィンドウ
/*******************************************************************/
#pragma once

//**************************************
//         インクルードファイル         
//**************************************
#include "afxwin.h"

///*************************************
//         クラス   定   義         
///*************************************
class CWorkAreaDlg : public CDialog
{
	DECLARE_DYNAMIC(CWorkAreaDlg)

	enum { IDD = IDD_WORKAREA_DLG };// ダイアログ データ

public:
	CWorkAreaDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CWorkAreaDlg();
	bool  GetWorkAreaEditMode();
	void  SetWorkAreaEditMode(bool onoff);
	int  ShowWorkAreaProps();//作業領域の設定値を表示する
	void OnMouseDownOnImage(CPoint point);
	void OnMouseDrugOnImage(CPoint point0, CPoint point1);
	void OnMouseUpOnImage(CPoint point0, CPoint point1);
	void OnDrawOnImage();

protected:
	CListBox m_figure_list;
	CBitmap addbmp,delbmp,fullbmp,editbmp, maskcreatebmp, maskdeletebmp, clipbmp, zoombmp;
	CToolTipCtrl	m_toolTip;
	bool  edit_from_spin = false;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート	
	int GetWorkAreaNoFromDlg();
	void ApplyWorkAreaPropsFromDlg();
	CString figure_type_fo_list_string(int type);
	void update_figure_list();
	int SetWorkAreaBy2Points(CPoint src_p1, CPoint src_p2, bool postProc = false);//2点の座標から作業領域を設定する


//メッセージハンドラ
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonWorkareaApply();
	afx_msg void OnDeltaposSpinWorkareaParam1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinWorkareaParam2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinWorkareaParam3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinWorkareaParam4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinWorkareaParam5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinWorkareaParam6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditWorkareaParam1();
	afx_msg void OnEnChangeEditWorkareaParam2();
	afx_msg void OnEnChangeEditWorkareaParam3();
	afx_msg void OnEnChangeEditWorkareaParam4();
	afx_msg void OnEnChangeEditWorkareaParam5();
	afx_msg void OnEnChangeEditWorkareaParam6();
	afx_msg void OnBnClickedCheckWorkareaMask();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonWorkareaAdd();
	afx_msg void OnLbnSelchangeListWorkareaFigure();
	afx_msg void OnBnClickedButtonWorkareaDelete();
	afx_msg void OnBnClickedCheckWorkareaNot();
	afx_msg void OnBnClickedButtonWorkareaClip();
	afx_msg void OnBnClickedButtonWorkareaClearMask();
	afx_msg void OnBnClickedButtonWorkareaFull();
	afx_msg void OnBnClickedButtonWorkareaScale();
	afx_msg void OnBnClickedWorkareaEdit();
	afx_msg void OnClose();

};
