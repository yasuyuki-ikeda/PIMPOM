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
//CBinalizeDlg
//２値化設定ウィンドウ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#pragma once
#include "afxcmn.h"

///*************************************
//         クラス   定   義         
///*************************************
class CBinalizeDlg : public CDialog
{
	DECLARE_DYNAMIC(CBinalizeDlg)

	enum { IDD = IDD_BINALIZE_DLG };// ダイアログ データ

public:
	CBinalizeDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CBinalizeDlg();


protected:
	CComboBox	m_combo_binalize_type;
	CComboBox	m_combo_binalize_dyn_size;
	CSliderCtrl m_slider_binalize_param0;
	CSliderCtrl m_slider_binalize_param1;
	CSliderCtrl m_slider_binalize_param3;
	int m_binalize_dyn_size[100];

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	bool execute();
	void show_dlg_item();


//メッセージハンドラ
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonBinalizeApply();
	afx_msg void OnBnClickedRadioBinalizeToMask();
	afx_msg void OnBnClickedRadioBinalizeToNewImage();
	afx_msg void OnBnClickedCheckBinalizeLabel();
	afx_msg void OnCbnEditchangeComboBinalizeType();
	afx_msg void OnBnClickedRadioBinalize0();
	afx_msg void OnBnClickedRadioBinalize1();
	afx_msg void OnEnChangeEditBinalizeParam3();
	afx_msg void OnEnChangeEditBinalizeParam0();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditBinalizeParam1();
	afx_msg void OnBnClickedButtonBinalizeSetOrgImg();
};
