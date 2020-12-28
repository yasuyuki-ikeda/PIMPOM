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
//CHistgramDlg
//ヒストグラムウィンドウ
/*******************************************************************/
#pragma once

//**************************************
//         インクルードファイル         
//**************************************
#include "ImageField.h"

///*************************************
//         クラス   定   義         
///*************************************
class CHistgramDlg : public CDialog
{
	enum { IDD = IDD_HISTGRAM_DLG };// ダイアログ データ

public:
	CHistgramDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	void DrawHistgram();//ヒストグラムを描画する
	bool set_min_max_from_mid_width(float mid, float width, float *min, float *max);//中央値のレンジ幅から表示レンジの最大と最小を求める





protected:
	unsigned char	*p_color_bar_data;//カラーバーを表すビットマップ（DIB）のデータ
	BITMAPINFO		*p_color_bar_data_info;//カラーバーを表すビットマップ（DIB）のBITMAPINFO
	CBitmap			bmp_button_inc, bmp_button_dec;//ボタンに関連づけるビットマップ
	bool			flg_str_from_key;//キーボードからの文字入力があったことを表すフラグ
	bool			is_draging_r;//ヒストグラム上で右ドラッグ中を表すフラグを立てる
	int				hist_value_drag_start;//右ドラッグ始点のヒストグラム値

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	HBITMAP set_bitmap_button(HBITMAP h_bmp, UINT id);//idで示されるボタンにビットマップを関連づける
	void draw_color_bar();//カラーバーを描画する
	void create_color_bar();//カラーバーを表すビットマップの作成
	void show_dlg_item();//ダイアログアイテム
	




//メッセージハンドラ
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnButtonHistgramMaxInc();
	afx_msg void OnButtonHistgramMaxDec();
	afx_msg void OnButtonHistgramMaxHlf();
	afx_msg void OnButtonHistgramMaxDbl();
	afx_msg void OnButtonRangeMinInc();
	afx_msg void OnButtonRangeMinDec();
	afx_msg void OnButtonRangeMinHlf();
	afx_msg void OnButtonRangeMinDbl();
	afx_msg void OnButtonRangeMidInc();
	afx_msg void OnButtonRangeMidDec();
	afx_msg void OnButtonRangeMidHlf();
	afx_msg void OnButtonRangeMidDbl();
	afx_msg void OnButtonRangeWidthInc();
	afx_msg void OnButtonRangeWidthDec();
	afx_msg void OnButtonRangeWidthHlf();
	afx_msg void OnButtonRangeWidthDbl();
	afx_msg void OnButtonRangeMaxInc();
	afx_msg void OnButtonRangeMaxDec();
	afx_msg void OnButtonRangeMaxHlf();
	afx_msg void OnButtonRangeMaxDbl();
	afx_msg void OnHistgramSet();
	afx_msg void OnChangeHistgramRangeMin();
	afx_msg void OnChangeHistgramRangeMax();
	afx_msg void OnChangeHistgramRangeMid();
	afx_msg void OnChangeHistgramRangeWidth();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHistgramSetMinMax();
	afx_msg void OnHistgramSetAve();
	afx_msg void OnHistgramSet0256();
	afx_msg void OnHistgramSetPiPi();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};
