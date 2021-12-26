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
//CDataPickDlg
//画像メモリフリーハンド編集ウィンドウ
/*******************************************************************/



//**************************************
//         インクルードファイル         
//**************************************
#pragma once
#include "afxcmn.h"
#include "global_define.h"
#include "global_typedef.h"


//**************************************
//         定数定義        
//**************************************
#define  DATAPICK_PICK		(0)
#define  DATAPICK_PAINT		(1)
#define  DATAPICK_FILL		(2)

///*************************************
//         クラス   定   義         
///*************************************
class CDataPickDlg : public CDialog
{
	DECLARE_DYNAMIC(CDataPickDlg)

	enum { IDD = IDD_DATAPICK_DLG };// ダイアログ データ

public:
	CDataPickDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDataPickDlg();
	bool OnMouseDownOnImage(CPoint point);
	bool OnMouseDrugOnImage(CPoint point);
	bool OnMouseMoveOnImage(CPoint point);
	bool PaintDot(CPoint point);
	bool PaintFloodFill(CPoint point);
	bool DataPick(CPoint point);
	int GetDataPickMode();
	void SetDataPickMode(int mode);

protected:
	CBitmap  paintbmp, pickbmp, fillbm, undobmp;
	CToolTipCtrl	m_toolTip;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	void show_dlg_item();


//メッセージハンドラ
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonFillWorkarea();
	afx_msg void OnClose();
	afx_msg void OnBnClickedRadioDatapick();
	afx_msg void OnBnClickedRadioDatapaint();
	afx_msg void OnBnClickedRadioDatafill();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedCheckDatapickMask();
	afx_msg void OnBnClickedUndo();
};
