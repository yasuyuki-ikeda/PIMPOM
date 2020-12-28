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
//CChartDlg
//グラフ表示ウィンドウ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#pragma once
#include "afxwin.h"
#include "afxcmn.h"


///*************************************
//         マ  ク  ロ   定   義         
///************************************
#define		CHART_TYPE_1D	(0)
#define		CHART_TYPE_2D	(1)

#define		CHART_MODE_DATA	(0)
#define		CHART_MODE_HISTGRAM	(1)
#define		CHART_MODE_SPECTRUM	(2)

// *************************************
//         構  造  体   定   義         
// *************************************

//データ構造体
typedef struct CHART_DATA_STRUCT_{
	float	data[CHART_DATA_NUM];//グラフデータ
	int		dataCount;//グラフデータの数
	int		dataType;//データのタイプ 0:1次元分布の折れ線図 1:2次元分布の散布図
	int		graphMode;//グラフのタイプ 0:データ  1:ヒストグラム  2:スペクトラム （グラフのタイプが0のときのみ有効）	
}CHART_DATA_STRUCT;

///*************************************
//         クラス   定   義         
///*************************************

class CChartDlg : public CDialog
{
	DECLARE_DYNAMIC(CChartDlg)

	enum { IDD = IDD_CHART_DLG };// ダイアログ データ

public:
	CChartDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CChartDlg();
	
	//グラフデータにアクセスする
	void	ClearChart(int chart_id);//グラフをクリアする
	int		GetDataCount(int chart_id);//グラフのデータ数を取得する
	float	GetDataFromChart(int chart_id, int data_id);//グラフからデータを取得する
	int		GetDataType(int chart_id);//グラフのデータタイプを取得する
	int		GetGraphMode(int chart_id);//グラフ表示状態を取得する
	void	SetGraphMode(int chart_id, int mode);//グラフ表示状態を設定する
	bool	SetDataToChart(int chart_id, int data_id, float value);//グラフの値を設定する
	int		AddDataToChart(int chart_id, float value, bool do_redraw=true);//グラフにデータを追加（1次元分布）
	int		AddDataToChart(int chart_id, float valueX, float valueY, bool do_redraw=true);//グラフにデータを追加（2次元分布）
	bool	PlotDataToChart(int chart_id, int data_num, float *pValueArr, bool doClear=true);//データ列のプロット（1次元分布）
	bool	PlotDataToChart(int chart_id, int data_num, double *pValueArr, bool doClear=true);
	bool	PlotDataToChart(int chart_id, int data_num, int *pValueArr, bool doClear=true);
	bool	PlotDataToChart(int chart_id, int data_num, BYTE *pValueArr, bool doClear=true);
	bool	PlotDataToChart(int chart_id, int data_num, float *pValueArrX, float *pValueArrY, bool doClear=true);//データ列のプロット（2次元分布）
	bool	PlotDataToChart(int chart_id, int data_num, double *pValueArrX, double *pValueArrY, bool doClear=true);
	bool	PlotDataToChart(int chart_id, int data_num, int *pValueArrX, int *pValueArrY, bool doClear=true);
	bool	PlotDataToChart(int chart_id, int data_num, BYTE *pValueArrX, BYTE *pValueArrY, bool doClear=true);
	int		DrawChart(int chart_id=-1, bool drawList=true, int select_id = -1, CPoint *clicked_point=NULL);//グラフを描画する



protected:
	bool		overlayFlg[CHART_NUM];//重畳表示を許すかどうか
	COLORREF	chartColor[CHART_NUM];//グラフデータの描画色(重畳時)
	CHART_DATA_STRUCT *pChartDataMap[CHART_NUM];
	CListCtrl m_list_ctrl;
	CComboBox m_chart_type;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	void	init_list();//リストビューを初期化する
	void	show_data_val(long chart_id);//データ値をリストビューにて表示する
	void	select_list_item(int n);//指定されたリストビューのアイテムを選択する
	int		get_selected_list_item();//選択中のリストビューのアイテム番号を取得する
	int		calc_overlay_order(int overRayOrder[], int chart_id);//重畳表示の順番を決める


 //メッセージハンドラ
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonChartDlgOutputCsv();
	afx_msg void OnBnClickedButtonChartDlgClear();
	afx_msg void OnBnClickedChkChartAutoMaxY();
	afx_msg void OnBnClickedChkChartAutoMinY();
	afx_msg void OnBnClickedChkChartAutoMaxX();
	afx_msg void OnBnClickedChkChartAutoMinX();
	afx_msg void OnBnClickedButtonChartDlgOverlayDetail();
	afx_msg void OnBnClickedChkChartOverlay();
	afx_msg void OnEnUpdateEditChartDlgHistSplitNum();
	afx_msg void OnBnClickedCheckChartDlgDispData();
	afx_msg void OnNMClickListChart(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListChart(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonChartDlgMidValY();
	afx_msg void OnBnClickedButtonChartDlgMidValX();
	afx_msg void OnCbnSelchangeComboChartDlgType();
	afx_msg void OnTcnSelchangeTabChartDlg(NMHDR *pNMHDR, LRESULT *pResult);
};
