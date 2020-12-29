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

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "WorkAreaDlg.h"


#define MAX(a,b)    ((a)>(b) ? (a):(b))

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
extern CPimpomAPI	API;

// *************************************
//         関    数    参    照         
// *************************************

//////////////////////////////////////////////////////////////////////////////////////////
//  メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CWorkAreaDlg, CDialog)

CWorkAreaDlg::CWorkAreaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWorkAreaDlg::IDD, pParent)
{

}

CWorkAreaDlg::~CWorkAreaDlg()
{
	addbmp.DeleteObject(); 
	delbmp.DeleteObject(); 
	fullbmp.DeleteObject();
	editbmp.DeleteObject();
	maskcreatebmp.DeleteObject();
	maskdeletebmp.DeleteObject();
	clipbmp.DeleteObject();
	zoombmp.DeleteObject();
}



void CWorkAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTBOX_FIGURE_UNION, m_figure_list);
}


/********************************************************************
機  能  名  称 : 作業領域の編集状態を取得する
関    数    名 : GetWorkAreaNoFromDlg
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool CWorkAreaDlg::GetWorkAreaEditMode()
{
	return API.GetCheck(m_hWnd, IDC_CHECK_WORKAREA_EDIT);
}


/********************************************************************
機  能  名  称 : 作業領域の編集状態を設定する
関    数    名 : GetWorkAreaEditMode
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::SetWorkAreaEditMode(bool onoff)
{
	API.SetCheck(m_hWnd, IDC_CHECK_WORKAREA_EDIT, onoff);
}

/********************************************************************
機  能  名  称 : 編集中の作業領域番号を取得する
関    数    名 : GetWorkAreaNoFromDlg
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CWorkAreaDlg::GetWorkAreaNoFromDlg()
{
	return m_figure_list.GetCurSel();
}


static bool PtInEditMargin(CPoint pt, int x, int y)
{
	if (abs(x - pt.x) < WORKAREA_EDIT_MARGIN && abs(y - pt.y) < WORKAREA_EDIT_MARGIN){
		return true;
	}
	else{
		return false;
	}
}

/********************************************************************
機  能  名  称 : メイン画面上でマウスクリックされたときに呼ばれる関数
関    数    名 : OnMouseDownOnImage
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnMouseDownOnImage(CPoint point)
{
	if (!IsWindowVisible())	return;//非表示状態では無効

	if (API.GetCheck(m_hWnd, IDC_CHECK_WORKAREA_EDIT))
	{//編集時
		SetWorkAreaBy2Points(point, point);
	}
}

/********************************************************************
機  能  名  称 : メイン画面上でマウスドラッグされたときに呼ばれる関数
関    数    名 : OnMouseDrugOnImage
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnMouseDrugOnImage(CPoint point0, CPoint point1)
{
	if (!IsWindowVisible())	return;//非表示状態では無効

	if (API.GetCheck(m_hWnd, IDC_CHECK_WORKAREA_EDIT))
	{//編集時
		SetWorkAreaBy2Points(point0, point1);
		API.DrawImage(CURRENT_IMAGE,false);//再描画
	}
}

/********************************************************************
機  能  名  称 : メイン画面上でマウスクリックが外されたときに呼ばれる関数
関    数    名 : OnMouseUpOnImage
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnMouseUpOnImage(CPoint point0, CPoint point1)
{
	if (!IsWindowVisible())	return;//非表示状態では無効

	if (API.GetCheck(m_hWnd, IDC_CHECK_WORKAREA_EDIT))
	{//編集時
		SetWorkAreaBy2Points(point0, point1,true);
		API.SetCheck(m_hWnd, IDC_CHECK_WORKAREA_EDIT, false);//編集モードをやめる
		API.DrawImage(CURRENT_IMAGE, false);//再描画	
	}
}

/********************************************************************
機  能  名  称 : メイン画面上に描画
関    数    名 : OnDrawOnImage
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnDrawOnImage()
{
	if (!IsWindowVisible())	return;//非表示状態では無効

	CDataUnit	*p_du = API.GetDataUnit(CURRENT_IMAGE);//現在選択中の画像のDataUnitを取得する
	if (p_du == NULL) {
		return;//データが無ければ何もしない
	}

	int selected_fig_id = ShowWorkAreaProps();
	if (selected_fig_id<0)	return;


	API.ShowMessage(true, 1, false, "作業領域");

	for (int i = 0; i<WORKAREA_FIG_NUM; i++)
	{
		FIGURE_UNION	fig;
		bool ornot;
		int type = p_du->WorkArea.GetWorkArea(i, &fig, &ornot);
		if (type<0)	continue;

		//領域色
		COLORREF col;
		if (ornot) { col = RGB(0, 255, 0); }//OR図形は緑
		else { col = RGB(255, 0, 0); }//NOT図形は赤

									  //領域太さ
		int pensize = 1;
		if (selected_fig_id == i) { pensize = 2; }

		switch (type)
		{
		case WORK_AREA_TYPE_RECT://矩形
			API.DrawRect((double)fig.rect.left - 0.5, (double)fig.rect.top - 0.5, (double)fig.rect.right + 0.5, (double)fig.rect.bottom + 0.5, pensize, col);
			API.ShowMessage(false, 1, false, "%d:矩形(%0.2f x %0.2f)", i, fig.rect.right - fig.rect.left, fig.rect.bottom - fig.rect.top);

			break;

		case WORK_AREA_TYPE_LINE://幅つき直線
			API.DrawLine(fig.line.x1, fig.line.y1, fig.line.x2, fig.line.y2, pensize, col, fig.line.w);
			API.ShowMessage(false, 1, false, "%d:直線(len=%lf wid=%0.2f)", i, API.calc_dist(fig.line.x1, fig.line.y1, fig.line.x2, fig.line.y2), fig.line.w);
			break;

		case WORK_AREA_TYPE_ELLIPSE://楕円
			API.DrawEllipse(fig.ellipse.cx, fig.ellipse.cy, fig.ellipse.rx, fig.ellipse.ry, pensize, col);
			API.ShowMessage(false, 1, false, "%d:楕円(長径%0.2f 短径%0.2f)", i, fig.ellipse.rx, fig.ellipse.ry);
			break;

		case WORK_AREA_TYPE_CIRCLE://幅つき円周
			API.DrawCircle(fig.circle.cx, fig.circle.cy, fig.circle.r, pensize, col, fig.circle.w);
			API.ShowMessage(false, 1, false, "%d:円周(半径%0.2f 幅%0.2f)", i, fig.circle.r, fig.circle.w);
			break;

		case WORK_AREA_TYPE_ARC://幅つき円弧
			API.ShowMessage(false, 1, false, "%d:円弧", i);
			break;

		case WORK_AREA_TYPE_POLYGON://多角形
			API.ShowMessage(false, 1, false, "%d:多角形", i);
			break;

		default:
			API.ShowMessage(false, 1, false, "%d:無効", i);
			break;
		}



		//作業領域編集中
		if (API.GetCheck(m_hWnd, IDC_CHECK_WORKAREA_EDIT) && selected_fig_id == i)
		{
			//作業領域編集自につまんで操作する領域
			switch (type)
			{
			case WORK_AREA_TYPE_RECT://矩形
				API.DrawRect((double)fig.rect.left - WORKAREA_EDIT_MARGIN, (double)fig.rect.top - WORKAREA_EDIT_MARGIN, (double)fig.rect.left + WORKAREA_EDIT_MARGIN, (double)fig.rect.top + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.rect.right - WORKAREA_EDIT_MARGIN, (double)fig.rect.bottom - WORKAREA_EDIT_MARGIN, (double)fig.rect.right + WORKAREA_EDIT_MARGIN, (double)fig.rect.bottom + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.rect.left - WORKAREA_EDIT_MARGIN, (double)fig.rect.bottom - WORKAREA_EDIT_MARGIN, (double)fig.rect.left + WORKAREA_EDIT_MARGIN, (double)fig.rect.bottom + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.rect.right - WORKAREA_EDIT_MARGIN, (double)fig.rect.top - WORKAREA_EDIT_MARGIN, (double)fig.rect.right + WORKAREA_EDIT_MARGIN, (double)fig.rect.top + WORKAREA_EDIT_MARGIN, 1, col);
				break;

			case WORK_AREA_TYPE_LINE://幅つき直線
				API.DrawRect((double)fig.line.x1 - WORKAREA_EDIT_MARGIN, (double)fig.line.y1 - WORKAREA_EDIT_MARGIN, (double)fig.line.x1 + WORKAREA_EDIT_MARGIN, (double)fig.line.y1 + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.line.x2 - WORKAREA_EDIT_MARGIN, (double)fig.line.y2 - WORKAREA_EDIT_MARGIN, (double)fig.line.x2 + WORKAREA_EDIT_MARGIN, (double)fig.line.y2 + WORKAREA_EDIT_MARGIN, 1, col);
				break;

			case WORK_AREA_TYPE_ELLIPSE://楕円
				API.DrawRect((double)fig.ellipse.cx - fig.ellipse.rx - WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cy - WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cx - fig.ellipse.rx + WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cy + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.ellipse.cx + fig.ellipse.rx - WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cy - WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cx + fig.ellipse.rx + WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cy + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.ellipse.cx - WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cy - fig.ellipse.ry - WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cx + WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cy - fig.ellipse.ry + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.ellipse.cx - WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cy + fig.ellipse.ry - WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cx + WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cy + fig.ellipse.ry + WORKAREA_EDIT_MARGIN, 1, col);

				break;

			case WORK_AREA_TYPE_CIRCLE://幅つき円周
				API.DrawRect((double)fig.circle.cx - fig.circle.r - WORKAREA_EDIT_MARGIN, (double)fig.circle.cy - WORKAREA_EDIT_MARGIN, (double)fig.circle.cx - fig.circle.r + WORKAREA_EDIT_MARGIN, (double)fig.circle.cy + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.ellipse.cx + fig.circle.r - WORKAREA_EDIT_MARGIN, (double)fig.circle.cy - WORKAREA_EDIT_MARGIN, (double)fig.circle.cx + fig.circle.r + WORKAREA_EDIT_MARGIN, (double)fig.circle.cy + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.circle.cx - WORKAREA_EDIT_MARGIN, (double)fig.circle.cy - fig.circle.r - WORKAREA_EDIT_MARGIN, (double)fig.circle.cx + WORKAREA_EDIT_MARGIN, (double)fig.circle.cy - fig.circle.r + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.circle.cx - WORKAREA_EDIT_MARGIN, (double)fig.circle.cy + fig.circle.r - WORKAREA_EDIT_MARGIN, (double)fig.circle.cx + WORKAREA_EDIT_MARGIN, (double)fig.circle.cy + fig.circle.r + WORKAREA_EDIT_MARGIN, 1, col);
				break;

			case WORK_AREA_TYPE_ARC://幅つき円弧
				break;

			case WORK_AREA_TYPE_POLYGON://多角形
				break;

			default:
				break;
			}
		}
	}
}


/********************************************************************
機  能  名  称 : 2点の座標から作業領域を設定する
関    数    名 : SetWorkAreaBy2Points
引          数 : CPoint		src_p1		(in)
				 CPoint		src_p2		(in)
				 bool		postProc	(in)
戻    り    値 : 現在選択中の領域番号を返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CWorkAreaDlg::SetWorkAreaBy2Points(CPoint src_p1, CPoint src_p2, bool postProc)
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return -1;


	int selected_figure_num=GetWorkAreaNoFromDlg();
	if( selected_figure_num<0)	return -1;

	bool	ornot;
	ornot = !API.GetCheck(m_hWnd,IDC_CHECK_WORKAREA_NOT);


	FIGURE_UNION	fig;
	int type = pdu->WorkArea.GetWorkArea(selected_figure_num, &fig, &ornot);//リストで選択中の図形情報の取得
	if (type<0)	return -1;

	switch (type)
	{
	case WORK_AREA_TYPE_RECT://矩形
		
		if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 0)
		{//エディットモード・初期化
			//2点　src_p1  src_p2　を対角成分とする矩形を作る
			RECT rect;
			rect.left = src_p1.x;
			rect.top = src_p1.y;
			rect.right = src_p2.x;
			rect.bottom = src_p2.y;
			pdu->WorkArea.SetWorkArea(selected_figure_num, rect, ornot);
		}
		else
		{
			if (src_p1 == src_p2){//マウスダウン時
				if (PtInEditMargin(src_p1, fig.rect.right, fig.rect.bottom)){
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 3;//エディットモード変更・右下座標移動
				}
				else if (PtInEditMargin(src_p1, fig.rect.left, fig.rect.top)){
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 2;//エディットモード変更・左上座標移動
				}
				else if (PtInEditMargin(src_p1, fig.rect.right, fig.rect.top)){
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 4;//エディットモード変更・右上座標移動
				}
				else if (PtInEditMargin(src_p1, fig.rect.left, fig.rect.bottom)){
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 5;//エディットモード変更・左下座標移動
				}
			}
			else
			{//マウス移動時
				RECT rect;

				if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 3)//エディットモード・右下座標移動
				{
					rect.left = fig.rect.left;
					rect.top = fig.rect.top;
					rect.right =  src_p2.x;
					rect.bottom = src_p2.y;
				}
				else if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 2)//エディットモード・左上座標移動
				{
					rect.left = src_p2.x;
					rect.top = src_p2.y;
					rect.right = fig.rect.right;
					rect.bottom = fig.rect.bottom;
				}
				else if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 4)//エディットモード・右上座標移動
				{
					rect.left = fig.rect.left;
					rect.top = src_p2.y;
					rect.right = src_p2.x;
					rect.bottom = fig.rect.bottom;
				}
				else if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 5)//エディットモード・左下座標移動
				{
					rect.left = src_p2.x;
					rect.top = fig.rect.top;
					rect.right = fig.rect.right;
					rect.bottom = src_p2.y;
				}
				else
				{//エディットモード・全体移動
					int centerx = (fig.rect.left + fig.rect.right) / 2;
					int centery = (fig.rect.top + fig.rect.bottom) / 2;

					rect.left = fig.rect.left - centerx + src_p2.x;
					rect.top = fig.rect.top - centery + src_p2.y;
					rect.right = fig.rect.right - centerx + src_p2.x;
					rect.bottom = fig.rect.bottom - centery + src_p2.y;
				}

				pdu->WorkArea.SetWorkArea(selected_figure_num, rect, ornot);
			}


		}

		break;

	case WORK_AREA_TYPE_LINE://直線
		if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 0)
		{//エディットモード・初期化
			FIGURE_LINE line;
			line.x1 = src_p1.x;
			line.y1 = src_p1.y;
			line.x2 = src_p2.x;
			line.y2 = src_p2.y;
			line.w = 1;
			pdu->WorkArea.SetWorkArea(selected_figure_num, line, ornot);
		}
		else
		{
			if (src_p1 == src_p2){//マウスダウン時
				if (PtInEditMargin(src_p1, fig.line.x2, fig.line.y2)){
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 3;//エディットモード変更・終点座標移動
				}
				else if (PtInEditMargin(src_p1, fig.line.x1, fig.line.y1)){
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 2;//エディットモード変更・始点座標移動
				}
			}
			else
			{//マウス移動時

				FIGURE_LINE line;

				if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 3)//エディットモード・終点座標移動
				{
					line.x1 = fig.line.x1;
					line.y1 = fig.line.y1;
					line.x2 = src_p2.x;
					line.y2 = src_p2.y;
					line.w = fig.line.w;
				}
				else if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 2)//エディットモード・始点座標移動
				{
					line.x1 = src_p2.x;
					line.y1 = src_p2.y;
					line.x2 = fig.line.x2;
					line.y2 = fig.line.y2;
					line.w = fig.line.w;
				}
				else
				{//エディットモード・全体移動
					int centerx = (fig.line.x1 + fig.line.x2) / 2;
					int centery = (fig.line.y1 + fig.line.y2) / 2;

					line.x1 = fig.line.x1 - centerx + src_p2.x;
					line.y1 = fig.line.y1 - centery + src_p2.y;
					line.x2 = fig.line.x2 - centerx + src_p2.x;
					line.y2 = fig.line.y2 - centery + src_p2.y;
					line.w = fig.line.w;
				}

				pdu->WorkArea.SetWorkArea(selected_figure_num, line, ornot);
			}
		}
		break;


	case WORK_AREA_TYPE_ELLIPSE://楕円

		if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 0)
		{//エディットモード・初期化
			FIGURE_ELLIPSE ellipse;
			ellipse.cx = (src_p1.x + src_p2.x)/2;
			ellipse.cy = (src_p1.y + src_p2.y)/2;
			ellipse.rx = abs(src_p1.x - src_p2.x)/2;
			ellipse.ry = abs(src_p1.y - src_p2.y)/2;
			pdu->WorkArea.SetWorkArea(selected_figure_num, ellipse, ornot);
		}
		else
		{
			if (src_p1 == src_p2){//マウスダウン時
				if (PtInEditMargin(src_p1, fig.ellipse.cx + fig.ellipse.rx, fig.ellipse.cy)){
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 3;//エディットモード変更・横幅変化
				}
				else if (PtInEditMargin(src_p1, fig.ellipse.cx, fig.ellipse.cy + fig.ellipse.ry)){
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 2;//エディットモード変更・縦幅変化
				}
			}
			else
			{
				FIGURE_ELLIPSE ellipse;

				if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 3)//エディットモード・横幅変化
				{
					ellipse.cx = fig.ellipse.cx;
					ellipse.cy = fig.ellipse.cy;
					ellipse.rx = fabs((float)(src_p2.x - fig.ellipse.cx));
					ellipse.ry = fig.ellipse.ry;
				}
				else if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 2)//エディットモード・縦幅変化
				{
					ellipse.cx = fig.ellipse.cx;
					ellipse.cy = fig.ellipse.cy;
					ellipse.rx = fig.ellipse.rx;
					ellipse.ry = fabs((float)(src_p2.y - fig.ellipse.cy));
				}
				else
				{//エディットモード・全体移動
					ellipse.cx = src_p2.x;
					ellipse.cy = src_p2.y;
					ellipse.rx = fig.ellipse.rx;
					ellipse.ry = fig.ellipse.ry;

				}

				pdu->WorkArea.SetWorkArea(selected_figure_num, ellipse, ornot);
			}
			
		}

		break;

	case WORK_AREA_TYPE_CIRCLE://円

		if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 0)
		{//エディットモード・初期化
			FIGURE_CIRCLE circle;
			circle.cx = src_p1.x;
			circle.cy = src_p1.y;
			circle.r = (int)sqrt((double)(src_p1.y - src_p2.y)*(src_p1.y - src_p2.y) + (double)(src_p1.x - src_p2.x)*(src_p1.x - src_p2.x));
			circle.w = 1;
			pdu->WorkArea.SetWorkArea(selected_figure_num, circle, ornot);
		}
		else
		{
			if (src_p1 == src_p2){//マウスダウン時
				if (PtInEditMargin(src_p1, fig.circle.cx + fig.circle.r, fig.circle.cy) || 
					PtInEditMargin(src_p1, fig.circle.cx - fig.circle.r, fig.circle.cy) || 
					PtInEditMargin(src_p1, fig.circle.cx, fig.circle.cy + fig.circle.r) || 
					PtInEditMargin(src_p1, fig.circle.cx, fig.circle.cy - fig.circle.r) )
				{
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 2;//エディットモード変更・半径変化
				}
			}
			else
			{
				FIGURE_CIRCLE circle;

				if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 2)//エディットモード・半径変化
				{
					circle.cx = fig.circle.cx;
					circle.cy = fig.circle.cy;
					circle.r = MAX(fabs(src_p2.x - fig.circle.cx), fabs(src_p2.y - fig.circle.cy));
					circle.w = fig.circle.w;
				}
				else
				{//エディットモード・全体移動
					circle.cx = src_p2.x;
					circle.cy = src_p2.y;
					circle.r = fig.circle.r;
					circle.w = fig.circle.w;
				}

				pdu->WorkArea.SetWorkArea(selected_figure_num, circle, ornot);
			}

		}

		break;

	case WORK_AREA_TYPE_ARC://円弧
		break;

	case WORK_AREA_TYPE_POLYGON://多角形
		break;

	default ://設定なし
		break;
	}

	ShowWorkAreaProps();//領域座標を表示する

	if(postProc)
	{//図形設定後の後処理
		pdu->WorkArea.FigureEditMode[selected_figure_num] = 1;//エディットモード変更・全体移動

		if(API.GetCheck(m_hWnd, IDC_CHECK_WORKAREA_MASK)){//マスク生成
			API.BackupDataUnit(CURRENT_IMAGE);//現在の画像をバックアップ
			pdu->SetMaskByWorkArea();
		}
	}

	return selected_figure_num;
}

/********************************************************************
機  能  名  称 : 
関    数    名 : figure_type_fo_list_string
引          数 : int type
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
CString CWorkAreaDlg::figure_type_fo_list_string(int type)
{
	CString str;
	switch(type){
		case WORK_AREA_TYPE_RECT://矩形
			str = "矩形";
			break;
		case WORK_AREA_TYPE_LINE://直線
			str = "直線";
			break;
		case WORK_AREA_TYPE_ELLIPSE://楕円
			str = "楕円";
			break;
		case WORK_AREA_TYPE_CIRCLE://円
			str = "円周";
			break;
		case WORK_AREA_TYPE_ARC://円弧
			str = "円弧";
			break;
		case WORK_AREA_TYPE_POLYGON://多角形
			str = "多角形";
			break;
		case WORK_AREA_TYPE_NOTHING:
			str = "無効";
			break;
	}
	return str;
}

/********************************************************************
機  能  名  称 : 図形リストを更新する
関    数    名 : update_figure_list
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::update_figure_list()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;

	bool selected_flg=false;
	int valid_figure_num=-1;
	int sel = m_figure_list.GetCurSel();//現在選択中のリスト番号取得


	//有効な図形数をカウント
	valid_figure_num=-1;
	for(int i=0 ; i<WORKAREA_FIG_NUM ; i++){
		if(pdu->WorkArea.Type[i] != WORK_AREA_TYPE_NOTHING){
			valid_figure_num=i;
		}
	}

	m_figure_list.ResetContent();//図形リストを一旦リセット

	//図形リストを再設定
	for(int i=0 ; i<valid_figure_num+1 ; i++){
		CString fig_num;
		fig_num.Format("%d:", i);
		m_figure_list.AddString(fig_num + figure_type_fo_list_string(pdu->WorkArea.Type[i]));
	}

	if(sel>0 && sel<=valid_figure_num){//もともと選んでいたリスト番号を選択
		m_figure_list.SetCurSel(sel);
	}else{
		m_figure_list.SetCurSel(0);
	}
}

/********************************************************************
機  能  名  称 : 作業領域の設定値をダイアログに表示する
関    数    名 : ShowWorkAreaProps
引          数 : 
戻    り    値 : 現在選択中の領域番号を返す 失敗したら-1を返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CWorkAreaDlg::ShowWorkAreaProps()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return -1;

	update_figure_list();//作業領域の状態をリストに反映

	int selected_figure_num=GetWorkAreaNoFromDlg();//リストで選択中の図形番号の取得
	if(selected_figure_num<0)	return -1;

	FIGURE_UNION	fig;
	bool ornot;
	int type = pdu->WorkArea.GetWorkArea(selected_figure_num,&fig,&ornot);//リストで選択中の図形情報の取得
	if(type<0)	return -1;


	//リストで選択中の図形のデータを表示/////////////////////////////////
	API.SetCheck(m_hWnd, IDC_CHECK_WORKAREA_NOT, !ornot);

	CSpinButtonCtrl *spin1  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM1);
	CSpinButtonCtrl *spin2  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM2);
	CSpinButtonCtrl *spin3  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM3);
	CSpinButtonCtrl *spin4  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM4);
	CSpinButtonCtrl *spin5  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM5);
	CSpinButtonCtrl *spin6  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM6);

	int show_hyde[6];
	switch(type)
	{
	case WORK_AREA_TYPE_RECT://矩形

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM1,_T("右上X"));
		spin1->SetPos(fig.rect.left);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM1, fig.rect.left);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM2,_T("右上Y"));
		spin2->SetPos(fig.rect.top);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM2, fig.rect.top);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM3,_T("左下X"));
		spin3->SetPos(fig.rect.right);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM3, fig.rect.right);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM4,_T("左下Y"));
		spin4->SetPos(fig.rect.bottom);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM4, fig.rect.bottom);

		show_hyde[0]=SW_SHOW;
		show_hyde[1]=SW_SHOW;
		show_hyde[2]=SW_SHOW;
		show_hyde[3]=SW_SHOW;
		show_hyde[4]=SW_HIDE;
		show_hyde[5]=SW_HIDE;
		break;

	case WORK_AREA_TYPE_LINE://直線

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM1,_T("始点X"));
		spin1->SetPos(fig.line.x1);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM1, fig.line.x1);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM2,_T("始点Y"));
		spin2->SetPos(fig.line.y1);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM2, fig.line.y1);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM3,_T("終点X"));
		spin3->SetPos( fig.line.x2);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM3, fig.line.x2);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM4,_T("終点Y"));
		spin4->SetPos(fig.line.y2);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM4, fig.line.y2);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM5,_T("幅"));
		spin5->SetPos(fig.line.w);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM5, fig.line.w);

		show_hyde[0]=SW_SHOW;
		show_hyde[1]=SW_SHOW;
		show_hyde[2]=SW_SHOW;
		show_hyde[3]=SW_SHOW;
		show_hyde[4]=SW_SHOW;
		show_hyde[5]=SW_HIDE;
		break;

	case WORK_AREA_TYPE_ELLIPSE://楕円

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM1,_T("中心点X"));
		spin1->SetPos(fig.ellipse.cx);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM1, fig.ellipse.cx);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM2,_T("中心点Y"));
		spin2->SetPos(fig.ellipse.cy);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM2, fig.ellipse.cy);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM3,_T("X半径"));
		spin3->SetPos( fig.ellipse.rx);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM3, fig.ellipse.rx);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM4,_T("Y半径"));
		spin4->SetPos(fig.ellipse.ry);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM4, fig.ellipse.ry);

		show_hyde[0]=SW_SHOW;
		show_hyde[1]=SW_SHOW;
		show_hyde[2]=SW_SHOW;
		show_hyde[3]=SW_SHOW;
		show_hyde[4]=SW_HIDE;
		show_hyde[5]=SW_HIDE;
		break;

	case WORK_AREA_TYPE_CIRCLE://円

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM1,_T("中心点X"));
		spin1->SetPos(fig.circle.cx);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM1, fig.circle.cx);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM2,_T("中心点Y"));
		spin2->SetPos(fig.circle.cy);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM2, fig.circle.cy);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM3,_T("半径"));
		spin3->SetPos( fig.circle.r);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM3, fig.circle.r);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM4,_T("幅"));
		spin4->SetPos(fig.circle.w);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM4, fig.circle.w);

		show_hyde[0]=SW_SHOW;
		show_hyde[1]=SW_SHOW;
		show_hyde[2]=SW_SHOW;
		show_hyde[3]=SW_SHOW;
		show_hyde[4]=SW_HIDE;
		show_hyde[5]=SW_HIDE;
		break;


	case WORK_AREA_TYPE_ARC://円弧

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM1,_T("中心点X"));
		spin1->SetPos(fig.arc.cx);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM1, fig.arc.cx);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM2,_T("中心点Y"));
		spin2->SetPos(fig.arc.cy);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM2, fig.arc.cy);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM3,_T("半径"));
		spin3->SetPos( fig.arc.r);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM3, fig.arc.r);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM4,_T("始点角度"));
		spin4->SetPos(fig.arc.start);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM4, fig.arc.start);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM5,_T("終点角度"));
		spin5->SetPos(fig.arc.end);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM5, fig.arc.end);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM6,_T("幅"));
		spin5->SetPos(fig.arc.w);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM6, fig.arc.w);

		show_hyde[0]=SW_SHOW;
		show_hyde[1]=SW_SHOW;
		show_hyde[2]=SW_SHOW;
		show_hyde[3]=SW_SHOW;
		show_hyde[4]=SW_SHOW;
		show_hyde[5]=SW_SHOW;
		break;

	case WORK_AREA_TYPE_POLYGON://多角形
		show_hyde[0]=SW_HIDE;
		show_hyde[1]=SW_HIDE;
		show_hyde[2]=SW_HIDE;
		show_hyde[3]=SW_HIDE;
		show_hyde[4]=SW_HIDE;
		show_hyde[5]=SW_HIDE;
		break;

	default ://無効
		show_hyde[0]=SW_HIDE;
		show_hyde[1]=SW_HIDE;
		show_hyde[2]=SW_HIDE;
		show_hyde[3]=SW_HIDE;
		show_hyde[4]=SW_HIDE;
		show_hyde[5]=SW_HIDE;
		break;
	}

	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM1), show_hyde[0]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM2), show_hyde[1]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM3), show_hyde[2]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM4), show_hyde[3]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM5), show_hyde[4]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM6), show_hyde[5]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM1), show_hyde[0]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM2), show_hyde[1]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM3), show_hyde[2]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM4), show_hyde[3]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM5), show_hyde[4]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM6), show_hyde[5]);
	spin1->ShowWindow(show_hyde[0]);
	spin2->ShowWindow(show_hyde[1]);
	spin3->ShowWindow(show_hyde[2]);
	spin4->ShowWindow(show_hyde[3]);
	spin5->ShowWindow(show_hyde[4]);
	spin6->ShowWindow(show_hyde[5]);




	return selected_figure_num;
}

/********************************************************************
機  能  名  称 : 編集中の作業領域をメイン画面へ反映する
関    数    名 : ApplyWorkAreaPropsFromDlg
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::ApplyWorkAreaPropsFromDlg()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;

	int selected_figure_num=GetWorkAreaNoFromDlg();//ダイアログで選択中の図形番号
	if(selected_figure_num<0)	return;

	bool	ornot;
	int		val1, val2, val3, val4, val5, val6;
	API.GetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM1, &val1);
	API.GetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM2, &val2);
	API.GetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM3, &val3);
	API.GetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM4, &val4);
	API.GetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM5, &val5);
	API.GetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM6, &val6);
	ornot = !API.GetCheck(m_hWnd,IDC_CHECK_WORKAREA_NOT);

	switch(pdu->WorkArea.Type[selected_figure_num])
	{
	case WORK_AREA_TYPE_RECT://矩形
		{
			RECT rect = {val1, val2, val3, val4};
			pdu->WorkArea.SetWorkArea(selected_figure_num, rect, ornot);
		}
		break;

	case WORK_AREA_TYPE_LINE://幅つき直線
		{
			FIGURE_LINE line = {(float)val1, (float)val2, (float)val3, (float)val4, (float)val5};
			pdu->WorkArea.SetWorkArea(selected_figure_num, line, ornot);
		}
		break;

	case WORK_AREA_TYPE_ELLIPSE://楕円
		{
			FIGURE_ELLIPSE ellipse = { (float)val1, (float)val2, (float)val3, (float)val4};
			pdu->WorkArea.SetWorkArea(selected_figure_num, ellipse, ornot);
		}
		break;

	case WORK_AREA_TYPE_CIRCLE://幅つき円周
		{
			FIGURE_CIRCLE circle = { (float)val1, (float)val2, (float)val3, (float)val4};
			pdu->WorkArea.SetWorkArea(selected_figure_num, circle, ornot);
		}
		break;

	case WORK_AREA_TYPE_ARC://幅つき円弧
		{
			FIGURE_ARC arc = { (float)val1, (float)val2, (float)val3, (float)val4, (float)val5, (float)val6};
			pdu->WorkArea.SetWorkArea(selected_figure_num, arc, ornot);
		}
		break;

	case WORK_AREA_TYPE_POLYGON://多角形
		break;

	default :
		return;
	}

	API.DrawImage();

}


//////////////////////////////////////////////////////////////////////////////////////////
//  メッセージ ハンドラ
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWorkAreaDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_WORKAREA_APPLY, &CWorkAreaDlg::OnBnClickedButtonWorkareaApply)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WORKAREA_PARAM1, &CWorkAreaDlg::OnDeltaposSpinWorkareaParam1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WORKAREA_PARAM2, &CWorkAreaDlg::OnDeltaposSpinWorkareaParam2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WORKAREA_PARAM3, &CWorkAreaDlg::OnDeltaposSpinWorkareaParam3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WORKAREA_PARAM4, &CWorkAreaDlg::OnDeltaposSpinWorkareaParam4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WORKAREA_PARAM5, &CWorkAreaDlg::OnDeltaposSpinWorkareaParam5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WORKAREA_PARAM6, &CWorkAreaDlg::OnDeltaposSpinWorkareaParam6)
	ON_EN_CHANGE(IDC_EDIT_WORKAREA_PARAM1, &CWorkAreaDlg::OnEnChangeEditWorkareaParam1)
	ON_EN_CHANGE(IDC_EDIT_WORKAREA_PARAM2, &CWorkAreaDlg::OnEnChangeEditWorkareaParam2)
	ON_EN_CHANGE(IDC_EDIT_WORKAREA_PARAM3, &CWorkAreaDlg::OnEnChangeEditWorkareaParam3)
	ON_EN_CHANGE(IDC_EDIT_WORKAREA_PARAM4, &CWorkAreaDlg::OnEnChangeEditWorkareaParam4)
	ON_EN_CHANGE(IDC_EDIT_WORKAREA_PARAM5, &CWorkAreaDlg::OnEnChangeEditWorkareaParam5)
	ON_EN_CHANGE(IDC_EDIT_WORKAREA_PARAM6, &CWorkAreaDlg::OnEnChangeEditWorkareaParam6)
	ON_BN_CLICKED(IDC_CHECK_WORKAREA_MASK, &CWorkAreaDlg::OnBnClickedCheckWorkareaMask)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_WORKAREA_ADD, &CWorkAreaDlg::OnBnClickedButtonWorkareaAdd)
	ON_LBN_SELCHANGE(IDC_LISTBOX_FIGURE_UNION, &CWorkAreaDlg::OnLbnSelchangeListWorkareaFigure)
	ON_BN_CLICKED(IDC_BUTTON_WORKAREA_DELETE, &CWorkAreaDlg::OnBnClickedButtonWorkareaDelete)
	ON_BN_CLICKED(IDC_CHECK_WORKAREA_NOT, &CWorkAreaDlg::OnBnClickedCheckWorkareaNot)
	ON_BN_CLICKED(IDC_BUTTON_WORKAREA_CLIP, &CWorkAreaDlg::OnBnClickedButtonWorkareaClip)
	ON_BN_CLICKED(IDC_BUTTON_WORKAREA_CLEAR_MASK, &CWorkAreaDlg::OnBnClickedButtonWorkareaClearMask)
	ON_BN_CLICKED(IDC_BUTTON_WORKAREA_FULL, &CWorkAreaDlg::OnBnClickedButtonWorkareaFull)
	ON_BN_CLICKED(IDC_BUTTON_WORKAREA_SCALE, &CWorkAreaDlg::OnBnClickedButtonWorkareaScale)
	ON_BN_CLICKED(IDC_CHECK_WORKAREA_EDIT, &CWorkAreaDlg::OnBnClickedWorkareaEdit)
	ON_WM_CLOSE()
	
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
BOOL CWorkAreaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//ビットマップボタン
	addbmp.LoadBitmap(IDB_BUTTON_WORKAREA_ADD);
	delbmp.LoadBitmap(IDB_BUTTON_WORKAREA_DEL);
	fullbmp.LoadBitmap(IDB_BUTTON_WORKAREA_FULL);
	editbmp.LoadBitmap(IDB_BUTTON_WORK_AREA_EDIT);
	maskcreatebmp.LoadBitmap(IDB_BUTTON_WORKAREA_MASK);
	maskdeletebmp.LoadBitmap(IDB_BUTTON_CLEAR_MASK);
	clipbmp.LoadBitmap(IDB_BUTTON_WORKAREA_CLIP);
	zoombmp.LoadBitmap(IDB_BUTTON_WORKAREA_ZOOM);

	((CButton*)GetDlgItem(IDC_BUTTON_WORKAREA_ADD))->SetBitmap((HBITMAP)addbmp);
	((CButton*)GetDlgItem(IDC_BUTTON_WORKAREA_DELETE))->SetBitmap((HBITMAP)delbmp);
	((CButton*)GetDlgItem(IDC_BUTTON_WORKAREA_FULL))->SetBitmap((HBITMAP)fullbmp);
	((CButton*)GetDlgItem(IDC_CHECK_WORKAREA_EDIT))->SetBitmap((HBITMAP)editbmp);
	((CButton*)GetDlgItem(IDC_CHECK_WORKAREA_MASK))->SetBitmap((HBITMAP)maskcreatebmp);
	((CButton*)GetDlgItem(IDC_BUTTON_WORKAREA_CLEAR_MASK))->SetBitmap((HBITMAP)maskdeletebmp);
	((CButton*)GetDlgItem(IDC_BUTTON_WORKAREA_CLIP))->SetBitmap((HBITMAP)clipbmp);
	((CButton*)GetDlgItem(IDC_BUTTON_WORKAREA_SCALE))->SetBitmap((HBITMAP)zoombmp);

	//ツールチップ
	m_toolTip.Create(this, TTS_ALWAYSTIP);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_WORKAREA_ADD), "作業領域を追加");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_WORKAREA_DELETE), "作業領域を削除");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_WORKAREA_FULL), "作業領域を全画面化");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_WORKAREA_EDIT), "作業領域を編集");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_WORKAREA_MASK), "作業領域外部にマスクを設定");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_WORKAREA_CLEAR_MASK), "マスクをクリア");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_WORKAREA_CLIP), "作業領域で画像をクリップ");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_WORKAREA_SCALE), "計測領域を拡大表示");

	//スピンボタンの範囲設定
	CSpinButtonCtrl *spin1  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM1);
	CSpinButtonCtrl *spin2  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM2);
	CSpinButtonCtrl *spin3  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM3);
	CSpinButtonCtrl *spin4  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM4);
	CSpinButtonCtrl *spin5  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM5);
	CSpinButtonCtrl *spin6  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM6);
	spin1->SetRange(0,9999);
	spin2->SetRange(0,9999);
	spin3->SetRange(0,9999);
	spin4->SetRange(0,9999);
	spin5->SetRange(0,9999);
	spin6->SetRange(0,9999);
	
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHECK_WORKAREA_EDIT), 0, 0, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_WORKAREA_ADD), 30, 0, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_WORKAREA_DELETE), 60, 0, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_WORKAREA_FULL), 90, 0, 30, 30, false);

	::MoveWindow(::GetDlgItem(m_hWnd, IDC_LISTBOX_FIGURE_UNION), 0, 30, 100, 210, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHECK_WORKAREA_NOT), 105, 30, 100, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_WORKAREA_APPLY), 205, 30, 60, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM1), 105, 60, 80, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM1), 185, 60, 60, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_SPIN_WORKAREA_PARAM1), 245, 60, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM2), 105, 90, 80, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM2), 185, 90, 60, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_SPIN_WORKAREA_PARAM2), 245, 90, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM3), 105, 120, 80, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM3), 185, 120, 60, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_SPIN_WORKAREA_PARAM3), 245, 120, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM4), 105, 150, 80, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM4), 185, 150, 60, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_SPIN_WORKAREA_PARAM4), 245, 150, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM5), 105, 180, 80, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM5), 185, 180, 60, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_SPIN_WORKAREA_PARAM5), 245, 180, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM6), 105, 210, 80, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM6), 185, 210, 60, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_SPIN_WORKAREA_PARAM6), 245, 210, 30, 30, false);
	
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHECK_WORKAREA_MASK), 110, 240, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_WORKAREA_CLEAR_MASK), 140, 240, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_WORKAREA_CLIP), 170, 240, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_WORKAREA_SCALE), 200, 240, 30, 30, false);

	

	ShowWorkAreaProps();


	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/********************************************************************
機  能  名  称 :キーボードからの入力
関    数    名 : PreTranslateMessage
引          数 :
戻    り    値 :
機          能 : カーソルキー：カーソルの移動
Delキー：選択中の画像メモリクリア
Enter/Escキー：無効化
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
BOOL CWorkAreaDlg::PreTranslateMessage(MSG* pMsg)
{
	POINT point;

	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) {
		case VK_RETURN://Enter
			return  true;

		case VK_ESCAPE://Esc
			return  true;

		case VK_UP://↑
			GetCursorPos(&point);
			SetCursorPos(point.x, point.y - 1);
			return  true;

		case VK_DOWN://↓
			GetCursorPos(&point);
			SetCursorPos(point.x, point.y + 1);
			return  true;

		case VK_LEFT://←
			GetCursorPos(&point);
			SetCursorPos(point.x - 1, point.y);
			return  true;

		case VK_RIGHT://→
			GetCursorPos(&point);
			SetCursorPos(point.x + 1, point.y);
			return true;

		default:
			break;
		}
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		//ツールチップ表示
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_WORKAREA_ADD)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_WORKAREA_DELETE)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_WORKAREA_FULL)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_WORKAREA_EDIT)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_WORKAREA_MASK)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_WORKAREA_CLEAR_MASK)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_WORKAREA_CLIP)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_WORKAREA_SCALE)->m_hWnd
			)
		{
			m_toolTip.RelayEvent(pMsg);
		}
		else
		{
			m_toolTip.Pop();
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

/********************************************************************
機  能  名  称 : このウィンドウを表示状態にする
関    数    名 : OnShowWindow
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow) {
		ShowWorkAreaProps();
	}
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
void CWorkAreaDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}

/********************************************************************
機  能  名  称 : ダイアログで変更した内容をデータに反映する
関    数    名 : ApplyWorkAreaPropsFromDlg
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnBnClickedButtonWorkareaApply()
{
	ApplyWorkAreaPropsFromDlg();
}

/********************************************************************
機  能  名  称 : 「作業領域追加を追加」ボタン押下
関    数    名 : OnBnClickedButtonWorkareaAdd
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnBnClickedButtonWorkareaAdd()
{
	int addtype = 0;
	int addnumber;
	CConfirmDlg dlg;

	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;

	update_figure_list();//図形リストを更新
	addnumber = m_figure_list.GetCount();//追加する番号
	if(addnumber>=WORKAREA_FIG_NUM)		return;


	//図形種類選択ダイアログを出す
	bool type[6]={0};
	type[0]=true;
	dlg.SetTitle("作業領域図形");
	dlg.RegistRadioVar(figure_type_fo_list_string(WORK_AREA_TYPE_RECT),&type[0]);
	dlg.RegistRadioVar(figure_type_fo_list_string(WORK_AREA_TYPE_LINE),&type[1]);
	dlg.RegistRadioVar(figure_type_fo_list_string(WORK_AREA_TYPE_ELLIPSE),&type[2]);
	dlg.RegistRadioVar(figure_type_fo_list_string(WORK_AREA_TYPE_CIRCLE),&type[3]);
	if( dlg.DoModal() != IDOK )	return;

	for(int i=0 ; i<6 ; i++){
		if(type[i]){
			addtype=i;
			break;
		}
	}

	pdu->WorkArea.Type[addnumber] = addtype;//作業領域を有効化
	pdu->WorkArea.FigureEditMode[addnumber] = 0;//未初期化状態

	update_figure_list();//図形リストを再更新
	m_figure_list.SetCurSel(addnumber);//追加したリスト要素を選択

	API.DrawImage();//メイン画面再描画

	API.SetWorkAreaEditMode(true);//作業領域編集モードON
}

/********************************************************************
機  能  名  称 : 「作業領域を削除」ボタン押下
関    数    名 : OnBnClickedButtonWorkareaDelete
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnBnClickedButtonWorkareaDelete()
{
	int deletenumber = 0;


	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;

	update_figure_list();//図形リストを更新
	deletenumber = m_figure_list.GetCurSel();//削除する番号
	if(deletenumber<0)		return;

	pdu->WorkArea.Type[deletenumber] = WORK_AREA_TYPE_NOTHING;//作業領域を無効化

	//図形番号調整
	for(int i=deletenumber ; i<WORKAREA_FIG_NUM-1 ; i++){
		pdu->WorkArea.Type[i] = pdu->WorkArea.Type[i+1];
		pdu->WorkArea.OrNot[i] = pdu->WorkArea.OrNot[i+1];
		memcpy(&pdu->WorkArea.Figure[i], &pdu->WorkArea.Figure[i+1], sizeof(FIGURE_UNION)); 
	}

	API.DrawImage();//メイン画面再描画
}

/********************************************************************
機  能  名  称 : スピンボタン操作
関    数    名 : OnDeltaposSpinWorkareaParam 1-6
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnDeltaposSpinWorkareaParam1(NMHDR *pNMHDR, LRESULT *pResult)
{
	edit_from_spin = true;
}

void CWorkAreaDlg::OnDeltaposSpinWorkareaParam2(NMHDR *pNMHDR, LRESULT *pResult)
{
	edit_from_spin = true;
}

void CWorkAreaDlg::OnDeltaposSpinWorkareaParam3(NMHDR *pNMHDR, LRESULT *pResult)
{
	edit_from_spin = true;
}

void CWorkAreaDlg::OnDeltaposSpinWorkareaParam4(NMHDR *pNMHDR, LRESULT *pResult)
{
	edit_from_spin = true;
}

void CWorkAreaDlg::OnDeltaposSpinWorkareaParam5(NMHDR *pNMHDR, LRESULT *pResult)
{
	edit_from_spin = true;
}

void CWorkAreaDlg::OnDeltaposSpinWorkareaParam6(NMHDR *pNMHDR, LRESULT *pResult)
{
	edit_from_spin = true;
}

/********************************************************************
機  能  名  称 : コントロールの値変動
関    数    名 : OnEnChangeEditWorkareaParam 1-6
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnEnChangeEditWorkareaParam1()
{
	if (edit_from_spin) {
		edit_from_spin = false;
		ApplyWorkAreaPropsFromDlg();
	}
}

void CWorkAreaDlg::OnEnChangeEditWorkareaParam2()
{
	if (edit_from_spin) {
		edit_from_spin = false;
		ApplyWorkAreaPropsFromDlg();
	}
}

void CWorkAreaDlg::OnEnChangeEditWorkareaParam3()
{
	if (edit_from_spin) {
		edit_from_spin = false;
		ApplyWorkAreaPropsFromDlg();
	}
}

void CWorkAreaDlg::OnEnChangeEditWorkareaParam4()
{
	if (edit_from_spin) {
		edit_from_spin = false;
		ApplyWorkAreaPropsFromDlg();
	}
}

void CWorkAreaDlg::OnEnChangeEditWorkareaParam5()
{
	if (edit_from_spin) {
		edit_from_spin = false;
		ApplyWorkAreaPropsFromDlg();
	}
}

void CWorkAreaDlg::OnEnChangeEditWorkareaParam6()
{
	if (edit_from_spin) {
		edit_from_spin = false;
		ApplyWorkAreaPropsFromDlg();
	}
}

/********************************************************************
機  能  名  称 : 「作業領域からマスク生成」ボタン押下
関    数    名 : OnBnClickedCheckWorkareaMask
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnBnClickedCheckWorkareaMask()
{
	if(API.GetCheck(m_hWnd, IDC_CHECK_WORKAREA_MASK)){
		CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
		if(!pdu)	return;

		pdu->SetMaskByWorkArea();//ワークエリアからマスク生成
		API.DrawImage();//メイン画面再描画
	}	
}

/********************************************************************
機  能  名  称 : 作業領域の図形変更
関    数    名 : OnLbnSelchangeListWorkareaFigure
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnLbnSelchangeListWorkareaFigure()
{
	API.DrawImage();//メイン画面再描画
}

/********************************************************************
機  能  名  称 : 作業領域のOR/NOT設定変更
関    数    名 : OnBnClickedCheckWorkareaNot
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnBnClickedCheckWorkareaNot()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;

	bool	ornot;
	ornot = !API.GetCheck(m_hWnd,IDC_CHECK_WORKAREA_NOT);

	int sel = m_figure_list.GetCurSel();
	if(sel<0)		return;

	pdu->WorkArea.OrNot[sel] = ornot;

	API.DrawImage();//メイン画面再描画
}

/********************************************************************
機  能  名  称 : 「作業領域で画像をクリッピング」ボタン押下
関    数    名 : OnBnClickedButtonWorkareaClip
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnBnClickedButtonWorkareaClip()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;

	API.BackupDataUnit(CURRENT_IMAGE);//現在の画像をバックアップ

	API.ClipDataUnit(CURRENT_IMAGE, pdu->WorkArea.GetCircumRect(), true);//クリッピング

	API.DrawImage();//メイン画面再描画
}

/********************************************************************
機  能  名  称 : 「マスククリア」ボタン押下
関    数    名 : OnBnClickedButtonWorkareaClearMask
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnBnClickedButtonWorkareaClearMask()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;

	API.BackupDataUnit(CURRENT_IMAGE);//現在の画像をバックアップ

	pdu->ClearMask();//マスククリア
	API.DrawImage();//メイン画面再描画
}

/********************************************************************
機  能  名  称 : 「全領域化」ボタン押下
関    数    名 : OnBnClickedButtonWorkareaFull
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnBnClickedButtonWorkareaFull()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;

	API.BackupDataUnit(CURRENT_IMAGE);//現在の画像をバックアップ

	pdu->InitWorkArea();//
	API.DrawImage();//メイン画面再描画
}


/********************************************************************
機  能  名  称 : 「拡大表示」ボタン押下
関    数    名 : OnBnClickedButtonWorkareaScale
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnBnClickedButtonWorkareaScale()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;


	CRect rect = pdu->WorkArea.GetCircumRect(m_figure_list.GetCurSel());
	float scaleX = (float)API.main_image_size.cx / (float)(rect.right - rect.left + 1) ;
	float scaleY = (float)API.main_image_size.cy / (float)(rect.bottom - rect.top + 1);

	pdu->DispScale = (scaleX>scaleY) ? scaleY : scaleX;
	pdu->DataDispOffset = CPoint(rect.left, rect.top);


	API.DrawImage();//メイン画面再描画
}


/********************************************************************
機  能  名  称 : ウインドウ閉じる
関    数    名 : OnClose
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkAreaDlg::OnClose()
{

	CDialog::OnClose();
}


void CWorkAreaDlg::OnBnClickedWorkareaEdit()
{
	if (API.GetCheck(m_hWnd, IDC_CHECK_WORKAREA_EDIT)) 
	{
		API.SetWorkAreaEditMode(true);
	}
	else {
		API.SetWorkAreaEditMode(false);
	}
}