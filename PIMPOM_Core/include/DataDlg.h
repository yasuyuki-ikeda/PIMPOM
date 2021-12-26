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
//CDataDlg
//画像メモリ一覧表示ウィンドウ
/*******************************************************************/


//**************************************
//         インクルードファイル         
//**************************************
#pragma once
#include "DataUnit.h"	
#include "ImageField.h"

///*************************************
//         クラス   定   義         
///*************************************
class CDataDlg : public CDialog
{
	enum { IDD = IDD_DATA_DLG };// ダイアログ データ

public:
	CDataDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	long ChangeCurrentDataNumber(long number);//現在選択中の画像データの番号を変更する
	bool DataExistMessage(CString mes, int image_num);//データの存在通知
	void DrawImage(long number);//指定した画像を描画する
	void DrawAllImages();//全ての画像を描画する

	int  WindowHeight;//このウィンドウの高さ（画像メモリ数によって変わる）
	

protected:
	bool			is_draging;//マウスドラッグ中を表すフラグ
	HBRUSH			bg_brush;//背景色のブラシ
	CImageField		**pp_image_disp_small;//小イメージのポインタ配列
	CPoint			mouse_point;//マウス座標

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	void draw_image_info(long number = CURRENT_IMAGE);//表示画像の情報を描画する（番号とデータの形式）（データ番号指定なしの場合は現在選択中のデータ）
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) ;//背景色を決定する
	void draw_current_rect(long number, COLORREF color);//表示中の画像を示す矩形を描画する
	int get_image_number_from_coordinate(CPoint coor);//指定した座標（クライアント領域）に表示されている画像の番号を返す
	void draw_background_rect(CRect rct);//背景色で矩形描画
	CString clip_string(CDC *pdc, CString str, int max_width);//文字列を表示長さでクリップする



//メッセージハンドラ
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDatamenuRename();
	afx_msg void OnDatamenuConvertType();
	afx_msg void OnDatamenuResize();
	afx_msg void OnDatamenuDelete();
	afx_msg void OnDatamenuSavebitmap();
	afx_msg void OnDatamenuUndo();
	afx_msg void OnDatamenuDisp3d();
};

