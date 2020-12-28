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
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "ChartDlg.h"

// *************************************
//         マ  ク  ロ   定   義         
// *************************************

// *************************************
//         外    部    宣    言         
// *************************************

// *************************************
//         構  造  体   定   義         
// *************************************

// *************************************
//         静    的    変    数         
// *************************************

// *************************************
//         外    部    変    数         
// *************************************
extern	CPimpomAPI	API;

// *************************************
//         関    数    参    照         
// *************************************


/********************************************************************
機  能  名  称 : 円を描画する
関    数    名 : draw_point
引          数 : CDC *p_dc		(in)デバイスコンテキスト
				 int x,y		(in)円中心座標[pix]
				 int r			(in)半径[pix]
				 COLORREF col	(in)描画色
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
static void draw_point(CDC *p_dc, int x, int y, int r, COLORREF col)
{
	CPen pen, *poldpen;

	pen.CreatePen(PS_SOLID , 2 , col);
	poldpen = (CPen*)p_dc->SelectObject(&pen);
	p_dc->Ellipse(x-r,y-r,x+r,y+r);
	p_dc->SelectObject(poldpen);
}

/********************************************************************
機  能  名  称 : 線を描画する
関    数    名 : draw_point
引          数 : CDC *p_dc		(in)デバイスコンテキスト
				 int x0,y0		(in)始点座標[pix]
				 int x1,y1		(in)終点座標[pix]
				 COLORREF col	(in)描画色
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
static void draw_line(CDC *p_dc, int x0, int y0, int x1, int y1, COLORREF col)
{
	CPen pen, *poldpen;

	pen.CreatePen(PS_SOLID , 1 , col);
	poldpen = (CPen*)p_dc->SelectObject(&pen);
	p_dc->MoveTo(x0,y0);
	p_dc->LineTo(x1,y1);
	p_dc->SelectObject(poldpen);
}

/********************************************************************
機  能  名  称 : ヒストグラムのバーを描画する
関    数    名 : draw_hist_bar
引          数 : CDC *p_dc		(in)デバイスコンテキスト
				 int x0,y0		(in)始点座標[pix]
				 int x1,y1		(in)終点座標[pix]
				 int hist		(in)度数
				 COLORREF col	(in)描画色
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
static void draw_hist_bar(CDC *p_dc, int x0, int y0, int x1, int y1, int hist, COLORREF col)
{
	CPen	pen, *poldpen;
	CBrush	burush, *poldbrush;
	CFont	font, *poldfont;
	CString str;

	pen.CreatePen(PS_SOLID, 1, RGB(255,255,255));
	burush.CreateSolidBrush(col);
	poldpen = (CPen*)p_dc->SelectObject(&pen);
	poldbrush = (CBrush*)p_dc->SelectObject(&burush);
	p_dc->Rectangle(x0,y0,x1,y1);
	p_dc->SelectObject(poldbrush);
	p_dc->SelectObject(poldpen);

	if(hist>0){
		font.CreateFontIndirect(&API.default_font);
		poldfont = (CFont*)p_dc->SelectObject(&font);
		str.Format(_T("%d"), hist);
		p_dc->TextOut(x0, y1-9, str);
		p_dc->SelectObject(poldfont);
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//テンプレートで型なし(全ての型の画像に対応する)関数をつくる
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : データ配列（１次元）をグラフにプロットする
関    数    名 : template <class DATA> plot_data_to_chart
引          数 : CChartDlg		*pChartDlg		(in)
				 int			chart_id		(in)グラフ番号
				 int			data_num		(in)データ数
				 DATA			*pValueArr		(in)データ配列
				 bool			doClear			(in)既にグラフにプロットされているデータがある場合の挙動
													true：現在あるデータをクリアしてからプロット
													false:現在あるデータの後に追加する
				 COLORREF col	(in)描画色
戻    り    値 : 　引数不正の場合はfalseが帰る
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
bool	plot_data_to_chart(CChartDlg *pChartDlg, int chart_id, int data_num, DATA *pValueArr, bool doClear)
{
	if(doClear)	pChartDlg->ClearChart(chart_id);

	for(int i=0 ; i<data_num ; i++){
		if( pChartDlg->AddDataToChart(chart_id, (float)pValueArr[i], false)<0)	break;
	}

	pChartDlg->SetGraphMode(chart_id,false);
	pChartDlg->DrawChart(chart_id);	
	return true;
}

/********************************************************************
機  能  名  称 : データ配列（２次元）をグラフにプロットする
関    数    名 : template <class DATA> plot_data_to_chart2
引          数 : CChartDlg		*pChartDlg					(in)
				 int			chart_id					(in)グラフ番号
				 int			data_num					(in)データ数
				 DATA			*pValueArrX,*pValueArrY		(in)データ配列
				 bool			doClear						(in)既にグラフにプロットされているデータがある場合の挙動
															true：現在あるデータをクリアしてからプロット
															false:現在あるデータの後に追加する
				 COLORREF col	(in)描画色
戻    り    値 : 　引数不正の場合はfalseが帰る
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
bool	plot_data_to_chart2(CChartDlg *pChartDlg, int chart_id, int data_num, DATA *pValueArrX,  DATA *pValueArrY, bool doClear)
{
	if(doClear)	pChartDlg->ClearChart(chart_id);

	for(int i=0 ; i<data_num ; i++){
		if(pChartDlg->AddDataToChart(chart_id, (float)pValueArrX[i], (float)pValueArrY[i], false)<0)	break;
	}

	pChartDlg->SetGraphMode(chart_id,0);
	pChartDlg->DrawChart(chart_id);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CChartDlg, CDialog)

CChartDlg::CChartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChartDlg::IDD, pParent)
{
	for(int i=0 ; i<CHART_NUM ; i++){
		pChartDataMap[i] = NULL;
	}
}

CChartDlg::~CChartDlg()
{
	//グラフ用メモリ領域の解放
	for(int i=0 ; i<CHART_NUM ; i++){
		if(pChartDataMap[i]){
			delete pChartDataMap[i];
			pChartDataMap[i] = NULL;
		}
	}
}

void CChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHART, m_list_ctrl);
	DDX_Control(pDX, IDC_COMBO_CHART_DLG_TYPE, m_chart_type);
}


/********************************************************************
機  能  名  称 : 現在グラフにプロットされているデータをクリア
関    数    名 : ClearChart
引          数 : int chart_id (in)グラフ番号
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::ClearChart(int chart_id)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return;

	if(pChartDataMap[chart_id]){
		delete	pChartDataMap[chart_id];
		pChartDataMap[chart_id] = NULL;
	}
}


/********************************************************************
機  能  名  称 : 現在グラフにプロットされているデータ数を取得
関    数    名 : GetDataNum
引          数 : int chart_id (in)グラフ番号
戻    り    値 : データ数
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CChartDlg::GetDataCount(int chart_id)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return 0;

	if(!pChartDataMap[chart_id])	return 0;

	return pChartDataMap[chart_id]->dataCount;
}

/********************************************************************
機  能  名  称 : 現在グラフにプロットされているデータのタイプ取得
関    数    名 : GetDataNum
引          数 : int chart_id (in)グラフ番号
戻    り    値 : 0:1次元グラフ　1:2次元グラフ  データなしのときは0
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CChartDlg::GetDataType(int chart_id)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return 0;

	if(!pChartDataMap[chart_id])	return 0;

	return pChartDataMap[chart_id]->dataType;
}

/********************************************************************
機  能  名  称 : 現在グラフの表示モード取得
関    数    名 : GetGraphMode
引          数 : int chart_id (in)グラフ番号
戻    り    値 : 0:データ  1:ヒストグラム  2:スペクトラム
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CChartDlg::GetGraphMode(int chart_id)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return false;

	if(!pChartDataMap[chart_id])	return false;

	return pChartDataMap[chart_id]->graphMode;
}


void CChartDlg::SetGraphMode(int chart_id, int mode)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return;

	if(!pChartDataMap[chart_id])	return;

	pChartDataMap[chart_id]->graphMode = mode;
}



/********************************************************************
機  能  名  称 : 現在グラフにプロットされているデータを取得
関    数    名 : GetDataNum
引          数 : int chart_id	(in)グラフ番号
				 int data_id	(in)データ番号
戻    り    値 : データを返す。　引数不正の場合は0が帰る
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
float CChartDlg::GetDataFromChart(int chart_id, int data_id)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return 0;

	if(!pChartDataMap[chart_id])	return 0;

	if(data_id<0 || data_id >= pChartDataMap[chart_id]->dataCount)	return 0;

	return  pChartDataMap[chart_id]->data[data_id];
}



/********************************************************************
機  能  名  称 : 現在グラフにプロットされているデータを設定
関    数    名 : SetDataToChart
引          数 : int	chart_id	(in)グラフ番号
				 int	data_id		(in)データ番号
				 float	value		(in)設定値
戻    り    値 : 　引数不正の場合はfalseが帰る
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CChartDlg::SetDataToChart(int chart_id, int data_id, float value)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return false;

	if(!pChartDataMap[chart_id])	return false;

	if(data_id<0 || data_id>=CHART_DATA_NUM)	return false;

	pChartDataMap[chart_id]->data[data_id] = value;

	return true;
}

/********************************************************************
機  能  名  称 : グラフにプロットするデータを追加する(1次元)
関    数    名 : AddDataToChart
引          数 : int	chart_id	(in)グラフ番号
				 float	value		(in)設定値
				 bool	do_redraw	(in)グラフ再描画する場合はtrue
戻    り    値 : 　正常実行できたら追加したデータ番号を返す。失敗したら負数を返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int  CChartDlg::AddDataToChart(int chart_id, float value, bool do_redraw)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return -1;

	if( GetDataType(chart_id) != CHART_TYPE_1D  ){//すでに1D以外のメモリがあれば削除
		ClearChart(chart_id);
	}

	if(!pChartDataMap[chart_id])//メモリがなければ新規作成
	{
		pChartDataMap[chart_id] = new CHART_DATA_STRUCT;
		if(!pChartDataMap[chart_id])	return -1;

		memset(pChartDataMap[chart_id], 0, sizeof(CHART_DATA_STRUCT) );//初期化
		pChartDataMap[chart_id]->dataType = CHART_TYPE_1D;
	}
	else
	{
		if( pChartDataMap[chart_id]->dataCount + 1 >= CHART_DATA_NUM)	return -1;//これ以上データを入れられない
	}

	//データを入れる
	int idx = pChartDataMap[chart_id]->dataCount;
	if(!SetDataToChart(chart_id, idx, value)) return -1;

	//データ数をカウントアップ
	pChartDataMap[chart_id]->dataCount++;

	//グラフ描画
	if(do_redraw)  DrawChart(chart_id);

	return idx;
}

/********************************************************************
機  能  名  称 : グラフにプロットするデータを追加する(2次元)
関    数    名 : AddDataToChart
引          数 : int	chart_id		(in)グラフ番号
				 float	valueX,	valueY	(in)設定値
				 bool	do_redraw		(in)グラフ再描画する場合はtrue
戻    り    値 : 　正常実行できたら追加したデータ番号を返す。失敗したら負数を返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CChartDlg::AddDataToChart(int chart_id, float valueX, float valueY, bool do_redraw)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return -1;

	if( GetDataType(chart_id) != CHART_TYPE_2D  ){//すでに1D以外のメモリがあれば削除
		ClearChart(chart_id);
	}

	if(!pChartDataMap[chart_id])//メモリがなければ新規作成
	{
		pChartDataMap[chart_id] = new CHART_DATA_STRUCT;
		if(!pChartDataMap[chart_id])	return -1;

		memset(pChartDataMap[chart_id], 0, sizeof(CHART_DATA_STRUCT) );//初期化
		pChartDataMap[chart_id]->dataType = CHART_TYPE_2D;
	}
	else
	{
		if( pChartDataMap[chart_id]->dataCount + 2 >= CHART_DATA_NUM)	return -1;//これ以上データを入れられない
	}

	//データを入れる
	int idx = pChartDataMap[chart_id]->dataCount;
	if(!SetDataToChart(chart_id, idx, valueX)) return -1;
	if(!SetDataToChart(chart_id, idx+1, valueY)) return -1;

	//データ数をカウントアップ
	pChartDataMap[chart_id]->dataCount+=2;

	//グラフ描画
	if(do_redraw)	DrawChart(chart_id);

	return idx;
}

/********************************************************************
機  能  名  称 : データ配列（１次元）をグラフにプロットする
関    数    名 : PlotDataToChart
引          数 :  int			chart_id		(in)グラフ番号
				 int			data_num		(in)データ数
				 DATA			*pValueArrX		(in)データ配列
				 bool			doClear			(in)既にグラフにプロットされているデータがある場合の挙動
												true：現在あるデータをクリアしてからプロット
												false:現在あるデータの後に追加する
戻    り    値 : 　引数不正の場合はfalseが帰る
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CChartDlg::PlotDataToChart(int chart_id, int data_num, float *pValueArr, bool doClear)
{
	return plot_data_to_chart(this, chart_id, data_num, pValueArr, doClear);
}

bool CChartDlg::PlotDataToChart(int chart_id, int data_num, double *pValueArr, bool doClear)
{
	return plot_data_to_chart(this, chart_id, data_num, pValueArr, doClear);
}

bool CChartDlg::PlotDataToChart(int chart_id, int data_num, int *pValueArr, bool doClear)
{
	return plot_data_to_chart(this, chart_id, data_num, pValueArr, doClear);
}

bool CChartDlg::PlotDataToChart(int chart_id, int data_num, BYTE *pValueArr, bool doClear)
{
	return plot_data_to_chart(this, chart_id, data_num, pValueArr, doClear);
}


/********************************************************************
機  能  名  称 : データ配列（2次元）をグラフにプロットする
関    数    名 : PlotDataToChart
引          数 :  int			chart_id					(in)グラフ番号
				 int			data_num					(in)データ数
				 DATA			*pValueArrX,*pValueArrY		(in)データ配列
				 bool			doClear						(in)既にグラフにプロットされているデータがある場合の挙動
															true：現在あるデータをクリアしてからプロット
															false:現在あるデータの後に追加する
戻    り    値 : 　引数不正の場合はfalseが帰る
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CChartDlg::PlotDataToChart(int chart_id, int data_num, float *pValueArrX, float *pValueArrY, bool doClear)
{
	return plot_data_to_chart2(this, chart_id, data_num, pValueArrX, pValueArrY, doClear);
}

bool CChartDlg::PlotDataToChart(int chart_id, int data_num, double *pValueArrX, double *pValueArrY, bool doClear)
{
	return plot_data_to_chart2(this, chart_id, data_num, pValueArrX, pValueArrY, doClear);
}

bool CChartDlg::PlotDataToChart(int chart_id, int data_num, int *pValueArrX, int *pValueArrY, bool doClear)
{
	return plot_data_to_chart2(this, chart_id, data_num, pValueArrX, pValueArrY, doClear);
}

bool CChartDlg::PlotDataToChart(int chart_id, int data_num, BYTE *pValueArrX, BYTE *pValueArrY, bool doClear)
{
	return plot_data_to_chart2(this, chart_id, data_num, pValueArrX, pValueArrY, doClear);
}


/********************************************************************
機  能  名  称 : グラフを重畳表示する順番を決める
関    数    名 : calc_overlay_order
引          数 : int overRayOrder[]		(out)重畳表示の順番
				 int chart_id			(in)一番前に表示するグラフ番号
戻    り    値 : 重畳表示するグラフ数
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CChartDlg::calc_overlay_order(int overRayOrder[], int chart_id)
{
	int overRayCnt=0;

	overRayOrder[overRayCnt++] = chart_id;//chart_idを最優先で表示する

	if(API.GetCheck(m_hWnd, IDC_CHK_CHART_OVERLAY) && overlayFlg[chart_id] )//chart_id以外のグラフを重畳表示する場合
	{
		for(int n=0 ; n<CHART_NUM ; n++)
		{
			if( GetDataCount(n)>0 && //データがあるかどうか
				GetDataType(n) == GetDataType(chart_id) && //グラフタイプが同じかどうか
				n!=chart_id &&
				overlayFlg[n]	//重畳表示を許可しているグラフかどうか
				)
			{
				overRayOrder[overRayCnt++] = n;
			}
		}
	}

	return overRayCnt;
}


/********************************************************************
機  能  名  称 : データの表示
関    数    名 : show_data_val
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::show_data_val(long chart_id)
{
	LVITEM       lvi={0};
	lvi.mask = LVIF_TEXT;
	char        buf[1024];
	

	m_list_ctrl.DeleteAllItems();//一旦すべてのリスト要素をクリア


	if( GetDataType(chart_id)==CHART_TYPE_1D )//1次元
	{
		for(int i=0 ; i<GetDataCount(chart_id) ; i++){

			//No.
			lvi.iItem = i;
			lvi.iSubItem = 0;
			sprintf(buf, "%d", i);
			lvi.pszText = buf;
			m_list_ctrl.InsertItem(&lvi);//リスト挿入

			//x
			lvi.iItem = i;
			lvi.iSubItem = 1;
			sprintf(buf, "%0.3f", GetDataFromChart(chart_id,i));
			lvi.pszText = buf;
			m_list_ctrl.SetItem(&lvi);
		}
	}
	else if( GetDataType(chart_id)==CHART_TYPE_2D )//2次元
	{
		for(int i=0 ; i<GetDataCount(chart_id)/2 ; i++){

			//No.
			lvi.iItem = i;
			lvi.iSubItem = 0;
			sprintf(buf, "%d", i);
			lvi.pszText = buf;
			m_list_ctrl.InsertItem(&lvi);//リスト挿入

			//x
			lvi.iItem = i;
			lvi.iSubItem = 1;
			sprintf(buf, "%0.3f", GetDataFromChart(chart_id,i*2));
			lvi.pszText = buf;
			m_list_ctrl.SetItem(&lvi);

			//y
			lvi.iItem = i;
			lvi.iSubItem = 2;
			sprintf(buf, "%0.3f", GetDataFromChart(chart_id,i*2+1));
			lvi.pszText = buf;
			m_list_ctrl.SetItem(&lvi);
		}
	}

}

/********************************************************************
機  能  名  称 : リストの要素を選択する
関    数    名 : select_list_item
引          数 : int n (in)リストの要素番号
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::select_list_item(int n)
{
	m_list_ctrl.SetItemState(n, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_list_ctrl.SetSelectionMark(n);
	m_list_ctrl.EnsureVisible(n,true);
}

/********************************************************************
機  能  名  称 : リストの選択中の要素番号を返す
関    数    名 : get_selected_list_item
引          数 : 
戻    り    値 : リストの選択中の要素番号
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int	CChartDlg::get_selected_list_item()
{
	return m_list_ctrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
}




/********************************************************************
機  能  名  称 : グラフを描画する
関    数    名 : DrawChart
引          数 : int	chart_id		(in)グラフ番号 (負数の場合は現在選択中のもの)
				 bool	redrawList		(in)リスト再描画するか？　（OnPaintでは必ずfalseにすること）
				 int	select_id		(in)選択データ番号指定
				 CPoint *clicked_point	(in)選択データを座標指定（優先度は番号指定のほうが高い）
				 
戻    り    値 : clicked_pointで指定した座標に最も近い位置に描画されるデータの番号を返す
機          能 : ウィンドウサイズに合わせてグラフを描画する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CChartDlg::DrawChart(int chart_id, bool redrawList, int select_id, CPoint *clicked_point)
{
	CRect client_rect;//クライアント領域を表す矩形
	CRect tab_rect;
	CDC	*pdc;
	int list_w, list_h, list_top, list_left;//グラフ描画領域
	int chart_w, chart_h, chart_top, chart_left, chart_h_mrgn;//グラフ描画領域
	float valX_max, valY_max, valX_min, valY_min;
	int pointed_data_id=-1, pointed_data_x, pointed_data_y;
	double dist_min=9999;


	if (chart_id < 0) {
		CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_CHART_DLG);
		if(p_wnd_tab==NULL)	return -1;

		chart_id = p_wnd_tab->GetCurSel();
	}

	if (chart_id < 0 || chart_id >= CHART_NUM)
	{
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_DATA_NUM, 0);
		return -1;
	}



	//グラフの配置位置決定
	::GetClientRect(m_hWnd, &client_rect);
	API.GetDialogItemRect(m_hWnd, IDC_TAB_CHART_DLG, &tab_rect);

	if(API.GetCheck(m_hWnd,IDC_CHECK_CHART_DLG_DISP_DATA))//リスト表示する
	{
		list_left = tab_rect.left + 2;
		list_top = tab_rect.bottom + 2;
		list_w = 120;
		list_h = client_rect.Height() - list_top - 25;

		::ShowWindow(::GetDlgItem(m_hWnd, IDC_LIST_CHART),SW_SHOW);
		::MoveWindow(::GetDlgItem(m_hWnd, IDC_LIST_CHART), list_left,list_top,list_w,list_h,false);

		if(redrawList){
			show_data_val(chart_id);
		}
	}else{//リスト表示しない
		list_left = tab_rect.left + 2;
		list_top = tab_rect.bottom + 2;
		list_w = 0;
		list_h = 0;
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_LIST_CHART),SW_HIDE);
	}

	chart_left = list_left + list_w + 40;//50;//グラフの左上X座標
	chart_top = list_top;//グラフの左上Y座標
	chart_w = client_rect.right - client_rect.left - chart_left - 5;//グラフの幅
	chart_h = client_rect.bottom - client_rect.top - chart_top - 25;//グラフの高さ
	chart_h_mrgn = 10;//グラフ描画の上下マージン



	//ダイアログアイテムの配置

	::MoveWindow(::GetDlgItem(m_hWnd, IDC_TAB_CHART_DLG), tab_rect.left, tab_rect.top, client_rect.Width() - 4, tab_rect.Height(), false);//タブ
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_X), chart_left,						chart_top + chart_h, 40,20,false);//X表示最小値のエディットボックス
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_MID_VAL_X), chart_left+ chart_w/2,				chart_top + chart_h, 40,20,false);//X表示中間値のエディットボックス
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_WID_VAL_X), chart_left+ chart_w/2+60,				chart_top + chart_h, 40,20,false);//X表示中間値のエディットボックス
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_STATIC_CHART_DLG_MID_X), chart_left+ chart_w/2+40,				chart_top + chart_h, 20,20,false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_X), chart_left + chart_w-40,			chart_top + chart_h, 40,20,false);//X表示最大値のエディットボックス
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_Y), chart_left-40, chart_top+chart_h-25, 40,20,false);//Y表示最小値のエディットボックス
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_MID_VAL_Y), chart_left-40, chart_top+(chart_h-25)/2, 40,20,false);//Y表示中間値のエディットボックス
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_STATIC_CHART_DLG_MID_Y),   chart_left-40, chart_top+(chart_h-25)/2+20, 40,20,false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_WID_VAL_Y), chart_left-40, chart_top+(chart_h-25)/2+40, 40,20,false);//Y表示値幅のエディットボックス
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_Y), chart_left-40, chart_top, 40,20,false);//Y表示最大値のエディットボックス

	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_CHART_DLG_MID_VAL_X), chart_left+ chart_w/2-30,				chart_top + chart_h, 30,20,false);//X中央値設定ボタン
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHK_CHART_AUTO_MIN_X), chart_left+40,	chart_top + chart_h,	40,20,false);//X表示最小値自動設定のチェックボックス
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHK_CHART_AUTO_MAX_X), chart_left + chart_w-80,	chart_top + chart_h,	40,20,false);//X表示最大値自動設定のチェックボックス
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_CHART_DLG_MID_VAL_Y), chart_left-40, chart_top+(chart_h-25)/2-20, 30,20,false);//Y中央値設定ボタン
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHK_CHART_AUTO_MIN_Y), chart_left-40,	chart_top+chart_h-25-20, 40,20,false);//Y表示最小値自動設定のチェックボックス
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHK_CHART_AUTO_MAX_Y), chart_left-40,	chart_top+25,			40,20,false);//Y表示最大値自動設定のチェックボックス


	int type = GetGraphMode(chart_id);//グラフタイプ
	m_chart_type.SetCurSel(type);
	if (type == CHART_MODE_HISTGRAM) 
	{
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_HIST_SPLIT_NUM), SW_SHOW);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_SPINT_CHART_DLG_HIST_SPLIT_NUM), SW_SHOW);
	}
	else {
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_HIST_SPLIT_NUM), SW_HIDE);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_SPINT_CHART_DLG_HIST_SPLIT_NUM), SW_HIDE);
	}


	//背景描画
	pdc = GetDC();
	pdc->Rectangle(chart_left, chart_top, chart_left + chart_w, chart_top + chart_h);


	if(GetDataCount(chart_id)>=2)//2点以上のデータがある
	{
		if(GetDataType(chart_id) == CHART_TYPE_1D)//1次元
		{
			m_chart_type.EnableWindow(1);

			if (GetGraphMode(chart_id)!= CHART_MODE_HISTGRAM)
			{
				//データ描画

				int overRayOrder[CHART_NUM] = { 0 };//重畳表示するグラフ番号の優先順
				int overRayCnt = 0;//重畳表示するグラフ数
				float spectlum[CHART_DATA_NUM] = { 0 };

				overRayCnt = calc_overlay_order(overRayOrder, chart_id);//グラフ重畳表示順決め


				//データの最大・最小を計算する

				valX_min = 0;
				valX_max = (float)GetDataCount(chart_id) - 1;
				valY_min = 999999;
				valY_max = -999999;

				if (GetGraphMode(chart_id) == CHART_MODE_SPECTRUM)
				{//周波数領域

					for (int i = 0; i < GetDataCount(chart_id) && i < CHART_DATA_NUM; i++)
					{
						spectlum[i] = GetDataFromChart(chart_id, i);
					}

					API.calc_dft(spectlum, GetDataCount(chart_id), spectlum);

					//周波数3未満は信用できない
					for (int i = 3; i < GetDataCount(chart_id) / 2 && i < CHART_DATA_NUM; i++)
					{
						float val = spectlum[i];
						if (valY_max < val) valY_max = val;
						if (valY_min > val) valY_min = val;
					}
				}
				else{//データそのもの
					for (int n = overRayCnt - 1; n >= 0; n--)//グラフ重畳表示(優先順の高いものほど前面に描画)
					{
						for (int i = 0; i<GetDataCount(overRayOrder[n]) && i<CHART_DATA_NUM; i++)
						{
							float val = GetDataFromChart(overRayOrder[n], i);
							if (valY_max < val) valY_max = val;
							if (valY_min > val) valY_min = val;
						}
					}
				}


				if(valY_min==valY_max){
					valY_max+=1;
					valY_min-=1;
				}


				//グラフ表示の最大・最小に固定値 or データ最大・最小 のどちらを使うかを決める

				if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_X) ){//X方向最小に固定値を使う
					API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_X, &valX_min);
				}

				if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_X) ){//X方向最大に固定値を使う
					API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_X, &valX_max);
				}

				if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_Y) ){//Y方向最小に固定値を使う
					API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_Y, &valY_min);
				}

				if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_Y) ){//Y方向最大に固定値を使う
					API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_Y, &valY_max);
				}



				//補助線の描画

				draw_line(pdc, chart_left, chart_top+chart_h_mrgn, chart_left+chart_w, chart_top+chart_h_mrgn, RGB(128,128,128));//補助線
				draw_line(pdc, chart_left, chart_top+chart_h-chart_h_mrgn, chart_left+chart_w, chart_top+chart_h-chart_h_mrgn, RGB(128,128,128));//補助線
				draw_line(pdc, chart_left, chart_top+chart_h/2, chart_left+chart_w, chart_top+chart_h/2, RGB(128,128,128));//補助線


				//データ点の描画

				if(valX_max>valX_min && valY_max>valY_min )
				{

					for(int n=overRayCnt-1 ; n>=0 ; n--)//グラフ重畳表示(優先順の高いものほど前面に描画)
					{
						float x,y;//データ点描画座標
						float xo=-1,yo=-1;//一つ前のデータ点描画座標
						int dataCnt = GetDataCount(overRayOrder[n]);
						for (int i = 0; i< dataCnt && i<CHART_DATA_NUM; i++)
						{

							if (GetGraphMode(chart_id) == CHART_MODE_SPECTRUM)
							{//周波数領域
								if (i > dataCnt / 2)	break;
								x = (i - valX_min) / (valX_max - valX_min)*chart_w + chart_left;
								y = chart_top + chart_h - chart_h_mrgn
									- (spectlum[i] - valY_min) / (valY_max - valY_min)*(chart_h - chart_h_mrgn * 2);
							}
							else
							{	
								x = (i - valX_min) / (valX_max - valX_min)*chart_w + chart_left;
								y = chart_top + chart_h - chart_h_mrgn
									- (GetDataFromChart(overRayOrder[n], i) - valY_min) / (valY_max - valY_min)*(chart_h - chart_h_mrgn * 2);
							}

							if( chart_left <= x && 
								chart_left + chart_w >= x &&
								chart_top <= y &&
								chart_top + chart_h >= y)//グラフ領域からのはみだしチェック
							{
								if(select_id>=0)//選択データIDが指定されている
								{
									if(select_id==i){
										pointed_data_id = i;
										pointed_data_x=(int)x;
										pointed_data_y=(int)y;
									}
								}
								else if(clicked_point)//選択データ座標が指定されている
								{
									double d = (clicked_point->x - x)*(clicked_point->x - x) + (clicked_point->y - y)*(clicked_point->y - y);
									if(d<dist_min && d<50*50){
										dist_min = d;
										pointed_data_id=i;
										pointed_data_x=(int)x;
										pointed_data_y=(int)y;
									}
								}

								//データ点間を線で結ぶ
								if(xo>=0 && yo>=0){
									draw_line(pdc,(int)xo,(int)yo,(int)x,(int)y,RGB(0,0,0));
								}
								xo = x;
								yo = y;

								draw_point(pdc, (int)x, (int)y, 2, chartColor[n]);//データ点に円を描画
							}
							else
							{
								xo = -1;
								yo = -1;
							}
						}
					}
				}
			}
			else//ヒストグラム
			{
				int histgram[CHART_NUM][CHART_HISTGRAM_SPLIT_MAX]={0};
				int maxHist=0;
				float histStep;
				int hist_split_cnt;

				API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_HIST_SPLIT_NUM, &hist_split_cnt);
				if(hist_split_cnt>CHART_HISTGRAM_SPLIT_MAX){
					hist_split_cnt = CHART_HISTGRAM_SPLIT_MAX;
				}

				int overRayOrder[CHART_NUM]={0};//重畳表示するグラフ番号の優先順
				int overRayCnt=0;//重畳表示するグラフ数

				overRayCnt = calc_overlay_order(overRayOrder, chart_id);//グラフ重畳表示順決め

				//データの最大・最小を計算する

				valX_min = 999999;
				valX_max = -999999;
				for(int n=overRayCnt-1 ; n>=0 ; n--)//グラフ重畳表示(優先順の高いものほど前面に描画)
				{
					for(int i=0 ; i<GetDataCount( overRayOrder[n] ) && i<CHART_DATA_NUM ; i++)
					{
						float val = GetDataFromChart(overRayOrder[n], i);
						if(valX_max < val) valX_max = val;
						if(valX_min > val) valX_min = val;
					}
				}

				if(valX_min==valX_max){
					valX_max+=1;
					valX_min-=1;
				}


				//度数を数える
				histStep = (valX_max - valX_min)/hist_split_cnt;
				for(int n=overRayCnt-1 ; n>=0 ; n--)//グラフ重畳表示(優先順の高いものほど前面に描画)
				{
					for(int i=0 ; i<GetDataCount( overRayOrder[n] ) && i<CHART_DATA_NUM ; i++)
					{
						for(int j=0 ; j<hist_split_cnt ; j++)
						{
							if( GetDataFromChart(overRayOrder[n], i) - valX_min <= (float)(j+1)*histStep ){
								histgram[n][j]++;
								if(maxHist < histgram[n][j])	maxHist = histgram[n][j];
								break;
							}
						}
					}
				}
				valY_min = 0;
				valY_max = (float)maxHist;


				//グラフ表示の最大・最小に固定値 or データ最大・最小 のどちらを使うかを決める

				if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_Y) ){//Y方向最大に固定値を使う
					API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_Y, &valY_max);
				}



				if(valX_max>valX_min && valY_max>valY_min )
				{
					//描画
					float x0,y0,x1,y1;
					for(int n=overRayCnt-1 ; n>=0 ; n--)//グラフ重畳表示(優先順の高いものほど前面に描画)
					{
						for(int j=0 ; j<hist_split_cnt ; j++)
						{
							x0 = (float)j*(float)chart_w/(float)hist_split_cnt + chart_left;
							y0 = (float)chart_top + (float)chart_h - (float)chart_h_mrgn;
							x1 = x0 + (float)chart_w/(float)hist_split_cnt;
							y1 = y0 - (float)histgram[n][j] / (float)valY_max * (float)(chart_h - chart_h_mrgn*2);
							if(y1<chart_top)	y1=chart_top;


							draw_hist_bar(pdc, (int)x0, (int)y0, (int)x1, (int)y1, histgram[n][j], chartColor[n]);//ヒストグラム描画

							if(select_id>=0)//選択データIDが指定されている
							{
		
							}
							else if(clicked_point)//選択データ座標が指定されている
							{
								if(x0<clicked_point->x && x1>clicked_point->x){
									draw_hist_bar(pdc, (int)x0, (int)y0, (int)x1, (int)y1, histgram[n][j], RGB(255,0,0));//ヒストグラム色変え描画

									CString str;
									str.Format("freqency=%d (%f～%f)", histgram[n][j], (float)(j*histStep), (float)(j+1)*histStep);
									API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_POINTED_VAL, str);
								}
							}
						}
					}

					draw_line(pdc, chart_left, chart_top+chart_h_mrgn, chart_left+chart_w, chart_top+chart_h_mrgn, RGB(128,128,128));//補助線
					draw_line(pdc, chart_left, chart_top+chart_h-chart_h_mrgn, chart_left+chart_w, chart_top+chart_h-chart_h_mrgn, RGB(128,128,128));//補助線
					draw_line(pdc, chart_left, chart_top+chart_h/2, chart_left+chart_w, chart_top+chart_h/2, RGB(128,128,128));//補助線
				}
			}

			API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_DATA_NUM, GetDataCount(chart_id));//データ数表示

		}
		else if(GetDataType(chart_id) == CHART_TYPE_2D)//2次元散布図
		{
			m_chart_type.EnableWindow(0);


			int overRayOrder[CHART_NUM]={0};//重畳表示するグラフ番号の優先順
			int overRayCnt=0;//重畳表示するグラフ数

			overRayCnt = calc_overlay_order(overRayOrder, chart_id);//グラフ重畳表示順決め


			//データの最大・最小を計算する

			valX_min = 999999;
			valX_max = -999999;
			valY_min = 999999;
			valY_max = -999999;
			for(int n=overRayCnt-1 ; n>=0 ; n--)//グラフ重畳表示(優先順の高いものほど全面に描画)
			{
				for(int i=0 ; i<GetDataCount( overRayOrder[n] ) && i<CHART_DATA_NUM ; i+=2)
				{
					float valx = GetDataFromChart(overRayOrder[n], i);
					if(valX_max < valx) valX_max = valx;
					if(valX_min > valx) valX_min = valx;

					float valy = GetDataFromChart(overRayOrder[n], i+1);
					if(valY_max < valy) valY_max = valy;
					if(valY_min > valy) valY_min = valy;
				}
			}

			if(valX_min==valX_max){
				valX_max+=1;
				valX_min-=1;
			}

			if(valY_min==valY_max){
				valY_max+=1;
				valY_min-=1;
			}


			//グラフ表示の最大・最小に固定値 or データ最大・最小 のどちらを使うかを決める

			if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_X) ){//X方向最小に固定値を使う
				API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_X, &valX_min);
			}

			if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_X) ){//X方向最大に固定値を使う
				API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_X, &valX_max);
			}

			if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_Y) ){//Y方向最小に固定値を使う
				API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_Y, &valY_min);
			}

			if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_Y) ){//Y方向最大に固定値を使う
				API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_Y, &valY_max);
			}


			//補助線の描画

			draw_line(pdc, chart_left, chart_top+chart_h_mrgn,			chart_left+chart_w, chart_top+chart_h_mrgn,			RGB(128,128,128));//補助線
			draw_line(pdc, chart_left, chart_top+chart_h-chart_h_mrgn,	chart_left+chart_w, chart_top+chart_h-chart_h_mrgn, RGB(128,128,128));//補助線
			draw_line(pdc, chart_left, chart_top+chart_h/2,				chart_left+chart_w, chart_top+chart_h/2,			RGB(128,128,128));//補助線
			draw_line(pdc, chart_left+chart_h_mrgn,				chart_top, chart_left+chart_h_mrgn,				chart_top+chart_h,	RGB(128,128,128));//補助線
			draw_line(pdc, chart_left+chart_w-chart_h_mrgn,		chart_top, chart_left+chart_w-chart_h_mrgn,		chart_top+chart_h,	RGB(128,128,128));//補助線
			draw_line(pdc, chart_left+chart_w/2,				chart_top, chart_left+chart_w/2,				chart_top+chart_h,	RGB(128,128,128));//補助線


			//データ点の描画

			if(valX_max>valX_min && valY_max>valY_min )
			{
				for(int n=overRayCnt-1 ; n>=0 ; n--)//グラフ重畳表示(優先順の高いものほど全面に描画)
				{
					float x,y;
					for(int i=0 ; i<GetDataCount( overRayOrder[n] ) && i<CHART_DATA_NUM ; i+=2)
					{
						x = chart_left + chart_h_mrgn
							 + ( GetDataFromChart(overRayOrder[n], i  ) - valX_min) / (valX_max - valX_min)*(chart_w - chart_h_mrgn*2) ;
						y = chart_top + chart_h - chart_h_mrgn
							 - ( GetDataFromChart(overRayOrder[n], i+1) - valY_min) / (valY_max - valY_min)*(chart_h - chart_h_mrgn*2) ;

						if( chart_left <= x && 
							chart_left + chart_w >= x &&
							chart_top <= y &&
							chart_top + chart_h >= y)//グラフ領域からのはみだしチェック
						{
							if(select_id>=0)//選択データIDが指定されている
							{
								if(select_id==i){
									pointed_data_id=i;
									pointed_data_x=(int)x;
									pointed_data_y=(int)y;
								}
							}
							else if(clicked_point)//選択データ座標が指定されている
							{
								double d = (clicked_point->x - x)*(clicked_point->x - x) + (clicked_point->y - y)*(clicked_point->y - y);
								if(d<dist_min && d<50*50){
									dist_min = d;
									pointed_data_id=i;
									pointed_data_x=(int)x;
									pointed_data_y=(int)y;
								}
							}

							draw_point(pdc, (int)x, (int)y, 2, chartColor[n]);//データ点に円を描画
						}
					}
				}
			}

			API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_DATA_NUM, GetDataCount(chart_id)/2);//データ数表示
		}
	}
	else
	{
		valX_min = 0;
		valX_max = 0;
		valY_min = 0;
		valY_max = 0;
	}

	
	//画面上クリックされたデータを描画

	CString str;
	if(GetDataType(chart_id) == CHART_TYPE_1D)//1次元
	{
		if(GetGraphMode(chart_id)!= CHART_MODE_HISTGRAM)
		{
			//折れ線グラフ

			if(pointed_data_id>=0)
			{
				draw_point(pdc, pointed_data_x, pointed_data_y, 2, RGB(255,0,0));//データ点に円を描画

				float val=GetDataFromChart(chart_id, pointed_data_id);

				if (GetGraphMode(chart_id) == CHART_MODE_SPECTRUM)
				{//周波数
					int dataCnt = GetDataCount(chart_id);
					float period = (float)dataCnt / (float)pointed_data_id;
					str.Format("period=%f", period);
				}
				else {
					str.Format("ID=%d  value=%f", pointed_data_id, val);
				}
				API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_POINTED_VAL, str);

				if(API.GetCheck(m_hWnd,IDC_CHECK_CHART_DLG_DISP_DATA))
				{//リスト表示する
					select_list_item(pointed_data_id);
				}
			}else{
				API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_POINTED_VAL, CString(""));
			}
		}
	}
	else if(GetDataType(chart_id) == CHART_TYPE_2D)//2次元散布図
	{
		if(pointed_data_id>=0)
		{
			draw_point(pdc, pointed_data_x, pointed_data_y, 2, RGB(255,0,0));//データ点に円を描画

			float valx=GetDataFromChart(chart_id, pointed_data_id);
			float valy=GetDataFromChart(chart_id, pointed_data_id+1);
			str.Format("ID=%d  X=%f  Y=%f", pointed_data_id, valx, valy);
			API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_POINTED_VAL, str);


			if(API.GetCheck(m_hWnd,IDC_CHECK_CHART_DLG_DISP_DATA))
			{//リスト表示する
				select_list_item(pointed_data_id/2);
			}
		}else{
			API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_POINTED_VAL, CString(""));
		}
	}


	//最大・最小・中間値・幅の表示
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_X, valX_max);
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MID_VAL_X, (valX_max + valX_min)/2);
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_WID_VAL_X, (valX_max - valX_min)/2);
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_X, valX_min);
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_Y, valY_max);
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MID_VAL_Y, (valY_max + valY_min)/2);
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_WID_VAL_Y, (valY_max - valY_min)/2);
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_Y, valY_min);


	//現在選択中のグラフ番号と異なる番号が指定された場合は再度描画する
	int chart_id_now;
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_CHART_DLG);
	chart_id_now = p_wnd_tab->GetCurSel();

	if(chart_id_now!=chart_id){
		p_wnd_tab->SetCurSel(chart_id);
	}

	ReleaseDC(pdc);

	return pointed_data_id;
}


/********************************************************************
機  能  名  称 : リスト初期化
関    数    名 : init_list
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::init_list()
{
    LVCOLUMN    lvc;
    char       caption[32];

	m_list_ctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

    lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // 有効フラグ


	//No.
	sprintf(caption, "ID");
    lvc.iSubItem    = 0;            // サブアイテム番号
    lvc.pszText     = caption;   // 見出しテキスト
    lvc.cx          = 20;          // 横幅
    m_list_ctrl.InsertColumn(0, &lvc);

	//X
	sprintf(caption, "x");
    lvc.iSubItem    = 1;            // サブアイテム番号
    lvc.pszText     = caption;   // 見出しテキスト
    lvc.cx          = 50;          // 横幅
    m_list_ctrl.InsertColumn(1, &lvc);
   
	//Y
	sprintf(caption, "y");
    lvc.iSubItem    = 2;            // サブアイテム番号
    lvc.pszText     = caption;   // 見出しテキスト
    lvc.cx          = 50;          // 横幅
    m_list_ctrl.InsertColumn(2, &lvc);
}
//////////////////////////////////////////////////////////////////////////////////////////
//  メッセージ ハンドラ
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CChartDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_CHART_DLG_OUTPUT_CSV, &CChartDlg::OnBnClickedButtonChartDlgOutputCsv)
	ON_BN_CLICKED(IDC_BUTTON_CHART_DLG_CLEAR, &CChartDlg::OnBnClickedButtonChartDlgClear)
	ON_BN_CLICKED(IDC_CHK_CHART_AUTO_MAX_Y, &CChartDlg::OnBnClickedChkChartAutoMaxY)
	ON_BN_CLICKED(IDC_CHK_CHART_AUTO_MIN_Y, &CChartDlg::OnBnClickedChkChartAutoMinY)
	ON_BN_CLICKED(IDC_CHK_CHART_AUTO_MAX_X, &CChartDlg::OnBnClickedChkChartAutoMaxX)
	ON_BN_CLICKED(IDC_CHK_CHART_AUTO_MIN_X, &CChartDlg::OnBnClickedChkChartAutoMinX)
	ON_BN_CLICKED(IDC_BUTTON_CHART_DLG_OVERLAY_DETAIL, &CChartDlg::OnBnClickedButtonChartDlgOverlayDetail)
	ON_BN_CLICKED(IDC_CHK_CHART_OVERLAY, &CChartDlg::OnBnClickedChkChartOverlay)
	ON_EN_UPDATE(IDC_EDIT_CHART_DLG_HIST_SPLIT_NUM, &CChartDlg::OnEnUpdateEditChartDlgHistSplitNum)
	ON_BN_CLICKED(IDC_CHECK_CHART_DLG_DISP_DATA, &CChartDlg::OnBnClickedCheckChartDlgDispData)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CHART, &CChartDlg::OnNMClickListChart)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_CHART, &CChartDlg::OnNMRClickListChart)
	ON_BN_CLICKED(IDC_BUTTON_CHART_DLG_MID_VAL_Y, &CChartDlg::OnBnClickedButtonChartDlgMidValY)
	ON_BN_CLICKED(IDC_BUTTON_CHART_DLG_MID_VAL_X, &CChartDlg::OnBnClickedButtonChartDlgMidValX)
	ON_CBN_SELCHANGE(IDC_COMBO_CHART_DLG_TYPE, &CChartDlg::OnCbnSelchangeComboChartDlgType)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CHART_DLG, &CChartDlg::OnTcnSelchangeTabChartDlg)
END_MESSAGE_MAP()


/********************************************************************
機  能  名  称 : ダイアログの初期化
関    数    名 : OnInitDialog
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
BOOL CChartDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetIcon(AfxGetApp()->LoadIcon(IDI_GRAPH), TRUE);			// アイコンを設定

	init_list();

	//グラフ用メモリ領域の取得
	for(int i=0 ; i<CHART_NUM ; i++){
		ClearChart(i);
	}

	//ダイアログアイテムの初期化
	
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_CHART_DLG);
	p_wnd_tab->SetCurSel(0);
	for (int n = 0; n < CHART_NUM; n++) 
	{
		CString str;
		str.Format("%d", n);
		p_wnd_tab->InsertItem(n, str);
	}
	
	API.SetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_X, true);
	API.SetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_X, true);
	API.SetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_Y, true);
	API.SetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_Y, true);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MAX_VAL_X), false);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MIN_VAL_X), false);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MAX_VAL_Y), false);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MIN_VAL_Y), false);

	CSpinButtonCtrl *spin2  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPINT_CHART_DLG_HIST_SPLIT_NUM);
	spin2->SetRange(10,CHART_HISTGRAM_SPLIT_MAX-1);
	spin2->SetPos(30);
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_HIST_SPLIT_NUM, 30);


	overlayFlg[0] = true;
	overlayFlg[1] = true;
	overlayFlg[2] = true;
	overlayFlg[3] = true;
	overlayFlg[4] = true;
	overlayFlg[5] = true;
	overlayFlg[6] = true;
	overlayFlg[7] = true;
	overlayFlg[8] = true;
	overlayFlg[9] = false;

	//グラフデータの描画色(重畳時)
	chartColor[0] = RGB(0,0,128);
	chartColor[1] = RGB(128,0,0);
	chartColor[2] = RGB(0,128,0);
	chartColor[3] = RGB(128,0,128);
	chartColor[4] = RGB(0,128,128);
	chartColor[5] = RGB(128,128,0);
	chartColor[6] = RGB(0,0,255);
	chartColor[7] = RGB(255,0,0);
	chartColor[8] = RGB(0,255,0);
	chartColor[9] = RGB(0,255,0);

	
	//グラフタイプ
	m_chart_type.AddString("データ");
	m_chart_type.AddString("ヒストグラム");
	m_chart_type.AddString("スペクトラム");


	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/********************************************************************
機  能  名  称 : 再描画
関    数    名 : OnPaint
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	DrawChart(-1, false);
}

/********************************************************************
機  能  名  称 : 表示するグラフ番号が変更された
関    数    名 : OnTcnSelchangeTabChartDlg
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnTcnSelchangeTabChartDlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	this->Invalidate();
	DrawChart();

	*pResult = 0;
}

/********************************************************************
機  能  名  称 :ウィンドウサイズが変更された
関    数    名 : OnSize
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	this->Invalidate();
	DrawChart();//再描画実行
}

/********************************************************************
機  能  名  称 :マウスの左ボタンが押下された
関    数    名 : OnLButtonDown
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	DrawChart(-1, false, -1, &point);//押下位置のデータ番号を取得

	CDialog::OnLButtonDown(nFlags, point);
}

/********************************************************************
機  能  名  称 :「データをCSVに出力する」ボタンが押下された
関    数    名 : OnBnClickedButtonChartDlgOutputCsv
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnBnClickedButtonChartDlgOutputCsv()
{
	CFileDialog myDLG(false, _T("csv"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
							"画像ファイルリスト(*.csv)|*.CSV|テキスト(*.txt)|*.txt||");
	if( myDLG.DoModal() != IDOK )	return	;


	int chart_id=0;//一番前に表示するグラフ番号
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_CHART_DLG);
	chart_id = p_wnd_tab->GetCurSel();


	int overRayOrder[CHART_NUM]={0};//重畳表示するグラフ番号の優先順
	int overRayCnt=0;//重畳表示するグラフ数
	overRayCnt = calc_overlay_order(overRayOrder, chart_id);//グラフ重畳表示順決め


	FILE *fp = fopen(myDLG.GetPathName(),"wt");
	if(fp)
	{
		if(GetDataType(chart_id)==CHART_TYPE_1D)//折れ線グラフ
		{
			for(int i=0 ; i<GetDataCount(chart_id) ; i++)
			{
				fprintf(fp,"%d",i);
				for(int n=0 ; n<overRayCnt ; n++)
				{
					float val=GetDataFromChart(overRayOrder[n], i);
					fprintf(fp,",%f", val);
				}
				fprintf(fp,"\n");
			}
		}
		else if(GetDataType(chart_id)==CHART_TYPE_2D)//2次元散布図
		{
			for(int i=0 ; i<GetDataCount(chart_id) ; i+=2)
			{
				fprintf(fp,"%d",i);
				for(int n=0 ; n<overRayCnt ; n++)
				{
					float valx=GetDataFromChart(chart_id, i);
					float valy=GetDataFromChart(chart_id, i+1);
					fprintf(fp,",%f,%f",valx, valy);
				}
				fprintf(fp,"\n");
			}
		}

		fclose(fp);
	}
}



/********************************************************************
機  能  名  称 : クリアボタン押下
関    数    名 : OnBnClickedButtonChartDlgClear
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnBnClickedButtonChartDlgClear()
{
	int chart_id;
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_CHART_DLG);
	chart_id = p_wnd_tab->GetCurSel();
	ClearChart(chart_id);
	DrawChart(chart_id);
}


/********************************************************************
機  能  名  称 : Y最大値を自動設定するチェックボックスの値変更
関    数    名 : OnBnClickedChkChartAutoMaxY
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnBnClickedChkChartAutoMaxY()
{
	if( API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_Y) ){
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MAX_VAL_Y), false);
	}else{
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MAX_VAL_Y), true);
	}
}

/********************************************************************
機  能  名  称 : Y最小値を自動設定するチェックボックスの値変更
関    数    名 : OnBnClickedChkChartAutoMinY
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnBnClickedChkChartAutoMinY()
{
	if( API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_Y) ){
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MIN_VAL_Y), false);
		
	}else{
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MIN_VAL_Y), true);
	}
}




/********************************************************************
機  能  名  称 : X最大値を自動設定するチェックボックスの値変更
関    数    名 : OnBnClickedChkChartAutoMaxX
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnBnClickedChkChartAutoMaxX()
{
	if( API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_X) ){
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MAX_VAL_X), false);
	}else{
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MAX_VAL_X), true);
	}
}

/********************************************************************
機  能  名  称 : X最小値を自動設定するチェックボックスの値変更
関    数    名 : OnBnClickedChkChartAutoMinX
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnBnClickedChkChartAutoMinX()
{
	if( API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_X) ){
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MIN_VAL_X), false);
	}else{
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MIN_VAL_X), true);
	}
}


/********************************************************************
機  能  名  称 :キーボードからの入力
関    数    名 : PreTranslateMessage
引          数 : 
戻    り    値 : 
機          能 : リターン：再描画
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
BOOL CChartDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN ){
		CPoint	point;
		CString	str;

		switch(pMsg->wParam){
			case VK_RETURN://Enter
				this->Invalidate();
				DrawChart();//再描画実行
				return  true;

			case VK_ESCAPE://Esc
				return  true;

			case VK_UP://↑
				break;

			case VK_DOWN://↓
				break;

			case VK_LEFT://←
				break;

			case VK_RIGHT://→
				break;

			case VK_DELETE://Del
				break;
		}
	}		
	return CDialog::PreTranslateMessage(pMsg);
}

/********************************************************************
機  能  名  称 :重畳表示詳細設定ボタン押下
関    数    名 : OnBnClickedButtonChartDlgOverlayDetail
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnBnClickedButtonChartDlgOverlayDetail()
{
	CConfirmDlg cfm;

	cfm.SetTitle("重畳表示するグラフ");
	cfm.RegistVar("グラフ0", &overlayFlg[0]);
	cfm.RegistVar("グラフ1", &overlayFlg[1]);
	cfm.RegistVar("グラフ2", &overlayFlg[2]);
	cfm.RegistVar("グラフ3", &overlayFlg[3]);
	cfm.RegistVar("グラフ4", &overlayFlg[4]);
	cfm.RegistVar("グラフ5", &overlayFlg[5]);
	cfm.RegistVar("グラフ6", &overlayFlg[6]);
	cfm.RegistVar("グラフ7", &overlayFlg[7]);
	cfm.RegistVar("グラフ8", &overlayFlg[8]);
	cfm.RegistVar("グラフ9", &overlayFlg[9]);

	if( cfm.DoModal()==IDOK){
		this->Invalidate();
		DrawChart();//再描画実行
	}
}

/********************************************************************
機  能  名  称 :重畳表示チェックボックス状態変化
関    数    名 : OnBnClickedChkChartOverlay
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnBnClickedChkChartOverlay()
{
	this->Invalidate();
	DrawChart();//再描画実行
}


/********************************************************************
機  能  名  称 : ヒストグラム
関    数    名 : OnEnUpdateEditChartDlgHistSplitNum
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnEnUpdateEditChartDlgHistSplitNum()
{
	this->Invalidate();
	DrawChart();//再描画実行
}

/********************************************************************
機  能  名  称 : グラフモード
関    数    名 : OnCbnSelchangeComboChartDlgType
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnCbnSelchangeComboChartDlgType()
{
	int chart_id;
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_CHART_DLG);
	chart_id = p_wnd_tab->GetCurSel();
	SetGraphMode(chart_id, m_chart_type.GetCurSel());
	this->Invalidate();
	DrawChart(chart_id);
}


/********************************************************************
機  能  名  称 :値の表示
関    数    名 : OnBnClickedCheckChartDlgDispData
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnBnClickedCheckChartDlgDispData()
{
	this->Invalidate();
	DrawChart();//再描画実行
}




/********************************************************************
機  能  名  称 : リスト選択
関    数    名 : OnNMClickListChart
引          数 : 
戻    り    値 : 
機          能 : 選択されたカーソルの色を変えて表示
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnNMClickListChart(NMHDR *pNMHDR, LRESULT *pResult)
{

	int chart_id;
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_CHART_DLG);
	chart_id = p_wnd_tab->GetCurSel();

	if( GetDataType(chart_id)==CHART_TYPE_1D )
	{//1次元
		DrawChart(chart_id, false, get_selected_list_item());//グラフ再描画
	}
	else if( GetDataType(chart_id)==CHART_TYPE_2D )
	{//2次元
		DrawChart(chart_id, false, get_selected_list_item()*2);//グラフ再描画
	}

	*pResult = 0;
}



/********************************************************************
機  能  名  称 : マウス右ボタンを押下
関    数    名 : OnNMRClickListMark
引          数 : 
戻    り    値 : 
機          能 : 編集のためのダイアログを出す。編集後に再表示する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnNMRClickListChart(NMHDR *pNMHDR, LRESULT *pResult)
{
	int chart_id;//グラフ番号
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_CHART_DLG);
	chart_id = p_wnd_tab->GetCurSel();

	if( GetDataType(chart_id)==CHART_TYPE_1D )
	{//1次元
		CConfirmDlg cfm;
		float x = GetDataFromChart(chart_id, get_selected_list_item()) ;

		cfm.SetTitle("データ編集");
		cfm.SetWidth(50,50);
		cfm.RegistVar("x", &x);

		if(cfm.DoModal()==IDOK)
		{
			SetDataToChart(chart_id, get_selected_list_item(), x);
			DrawChart(chart_id);
		}
	}
	else if( GetDataType(chart_id)==CHART_TYPE_2D )
	{//2次元
		CConfirmDlg cfm;
		float x = GetDataFromChart(chart_id, get_selected_list_item()*2 );
		float y = GetDataFromChart(chart_id, get_selected_list_item()*2+1 );

		cfm.SetTitle("データ編集");
		cfm.SetWidth(50,50);
		cfm.RegistVar("x", &x);
		cfm.RegistVar("y", &y);

		if(cfm.DoModal()==IDOK)
		{
			SetDataToChart(chart_id, get_selected_list_item()*2, x);
			SetDataToChart(chart_id, get_selected_list_item()*2+1, y);
			DrawChart(chart_id);
		}
	}



	*pResult = 0;
}



/********************************************************************
機  能  名  称 : Y中央値設定ボタン押下
関    数    名 : OnBnClickedButtonChartDlgMidValY
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnBnClickedButtonChartDlgMidValY()
{
	CConfirmDlg cfm;
	float mid, wid;

	API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MID_VAL_Y, &mid);
	API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_WID_VAL_Y, &wid);

	cfm.SetTitle("Y軸設定");
	cfm.RegistVar("中央", &mid);
	cfm.RegistVar("±  ", &wid);

	if( cfm.DoModal()==IDOK){
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MID_VAL_Y, mid);
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_WID_VAL_Y, wid);
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_Y, mid + wid);
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_Y, mid - wid);
		API.SetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_Y, false);
		API.SetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_Y, false);
		this->Invalidate();
		DrawChart();//再描画実行
	}
}

/********************************************************************
機  能  名  称 : X中央値設定ボタン押下
関    数    名 : OnBnClickedButtonChartDlgMidValX
引          数 : 
戻    り    値 : 
機          能 : 編集のためのダイアログを出す。編集後に再表示する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CChartDlg::OnBnClickedButtonChartDlgMidValX()
{
	CConfirmDlg cfm;
	float mid, wid;

	API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MID_VAL_X, &mid);
	API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_WID_VAL_X, &wid);

	cfm.SetTitle("X軸設定");
	cfm.RegistVar("中央", &mid);
	cfm.RegistVar("±  ", &wid);

	if( cfm.DoModal()==IDOK){
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MID_VAL_X, mid);
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_WID_VAL_X, wid);
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_X, mid + wid);
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_X, mid - wid);
		API.SetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_X, false);
		API.SetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_X, false);
		this->Invalidate();
		DrawChart();//再描画実行
	}
}


