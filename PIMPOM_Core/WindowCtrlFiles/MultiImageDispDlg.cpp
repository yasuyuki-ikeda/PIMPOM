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
//MultiImageDispDlg
//複数画像同時表示ウィンドウ
/*******************************************************************/

#include "stdafx.h"
#include "PIMPOM_API.h"
#include "MultiImageDispDlg.h"


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


// CMultiImageDispDlg ダイアログ


//////////////////////////////////////////////////////////////////////////////////////////
//  メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : 表示画像に関連するスクロールバーのポインタを取得する
関    数    名 : get_disp_scroll_bar
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::get_disp_scroll_bar(int dispImgNo, CScrollBar **pp_h_scroll, CScrollBar **pp_v_scroll)
{
	int hScrollBarID[]={IDC_SCROLL_MULTI_IMG_DLG_H0, IDC_SCROLL_MULTI_IMG_DLG_H1, IDC_SCROLL_MULTI_IMG_DLG_H2, IDC_SCROLL_MULTI_IMG_DLG_H3,IDC_SCROLL_MULTI_IMG_DLG_H4 ,IDC_SCROLL_MULTI_IMG_DLG_H5 };
	int vScrollBarID[]={IDC_SCROLL_MULTI_IMG_DLG_V0, IDC_SCROLL_MULTI_IMG_DLG_V1, IDC_SCROLL_MULTI_IMG_DLG_V2, IDC_SCROLL_MULTI_IMG_DLG_V3,IDC_SCROLL_MULTI_IMG_DLG_V4 ,IDC_SCROLL_MULTI_IMG_DLG_V5 };

	*pp_h_scroll = (CScrollBar*)GetDlgItem( hScrollBarID[dispImgNo] );
	*pp_v_scroll = (CScrollBar*)GetDlgItem( vScrollBarID[dispImgNo] );
}



/********************************************************************
機  能  名  称 : 表示画像データを取得する
関    数    名 : get_disp_image_data
引          数 : 
戻    り    値 : 表示画像データの先頭ポインタ
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
CDataUnit *CMultiImageDispDlg::get_disp_image_data(int dispImgNo)
{
	return  API.GetDataUnit(m_disp_image_number[dispImgNo]);
}


/********************************************************************
機  能  名  称 : スクロールバーを設定する
関    数    名 : set_scrollbar
引          数 : CScrollBar		*sc		(out)スクロールバーのコントロール参照
				 int			page	(in)ページ幅
				 int			pos		(in)位置
				 int			min		(in)最小位置
				 int			max		(in)最大位置
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::set_scrollbar(CScrollBar *sc, int page, int pos, int min, int max)
{
	SCROLLINFO  scrollinfo;
	sc->GetScrollInfo( &scrollinfo , SIF_ALL );
	scrollinfo.nPage = page;
	scrollinfo.nPos  = pos ;
	scrollinfo.nMin  = min;
	scrollinfo.nMax  = max;
	sc->SetScrollInfo( &scrollinfo , true );	
}



/********************************************************************
機  能  名  称 : 画像表示領域を算出する
関    数    名 : calc_image_regn
引          数 : CRect imageRect[]   //画像表示領域
戻    り    値 : 画像表示領域数
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CMultiImageDispDlg::calc_image_regn(CRect imageRect[])
{
	int imageCnt=0;
	RECT clientRct={0};//クライアント領域を表す矩形
	CRect canvasRect;//画像描画する領域

	::GetClientRect(m_hWnd, &clientRct);//このウィンドウのクライアント領域を表す矩形を取得

	canvasRect.left =  2;
	canvasRect.right = clientRct.right - clientRct.left - 4;
	canvasRect.top =  40;
	canvasRect.bottom = clientRct.bottom - clientRct.top - 2;

	//きちんと分割できるように描画領域のサイズを偶数にする
	if( canvasRect.Width()%2 !=0 )	canvasRect.right +=1;
	if( canvasRect.Height()%2 !=0 )	canvasRect.bottom +=1;



	if(m_cmb_disp_num.GetCurSel()==0)//2画像表示時
	{
		//画像0 = 左
		imageRect[0].left   = canvasRect.left;
		imageRect[0].top    = canvasRect.top;
		imageRect[0].right  = canvasRect.left + canvasRect.Width()/2 -1;
		imageRect[0].bottom = canvasRect.bottom;

		//画像1 = 右
		imageRect[1].left   = canvasRect.left + canvasRect.Width()/2;
		imageRect[1].top    = canvasRect.top;
		imageRect[1].right  = canvasRect.right;
		imageRect[1].bottom = canvasRect.bottom;

		imageCnt = 2;
	}
	else if (m_cmb_disp_num.GetCurSel() == 1)
	{//4画像表示
		//画像0 = 左上
		imageRect[0].left   = canvasRect.left;
		imageRect[0].top    = canvasRect.top;
		imageRect[0].right  = canvasRect.left + canvasRect.Width()/2 -1;
		imageRect[0].bottom = canvasRect.top + canvasRect.Height()/2 -1;

		//画像1 = 右上
		imageRect[1].left   = canvasRect.left + canvasRect.Width()/2;
		imageRect[1].top    = canvasRect.top;
		imageRect[1].right  = canvasRect.right;
		imageRect[1].bottom = canvasRect.top + canvasRect.Height()/2 -1;

		//画像2 = 左下
		imageRect[2].left   = canvasRect.left;
		imageRect[2].top    = canvasRect.top + canvasRect.Height()/2;
		imageRect[2].right  = canvasRect.left + canvasRect.Width()/2 -1;
		imageRect[2].bottom = canvasRect.bottom;

		//画像3 = 右下
		imageRect[3].left   = canvasRect.left + canvasRect.Width()/2;
		imageRect[3].top    = canvasRect.top + canvasRect.Height()/2;
		imageRect[3].right  = canvasRect.right;
		imageRect[3].bottom = canvasRect.bottom;

		imageCnt = 4;
	}
	else
	{//6画面表示
	 //画像0 = 左上
		imageRect[0].left = canvasRect.left;
		imageRect[0].top = canvasRect.top;
		imageRect[0].right = canvasRect.left + canvasRect.Width() / 3 - 1;
		imageRect[0].bottom = canvasRect.top + canvasRect.Height() / 2 - 1;

		//画像1 = 中央上
		imageRect[1].left = canvasRect.left + canvasRect.Width() / 3;
		imageRect[1].top = canvasRect.top;
		imageRect[1].right = canvasRect.left + canvasRect.Width() *2 / 3 -1;
		imageRect[1].bottom = canvasRect.top + canvasRect.Height() / 2 - 1;

		//画像2 = 右上
		imageRect[2].left = canvasRect.left + canvasRect.Width() * 2 / 3;
		imageRect[2].top = canvasRect.top;
		imageRect[2].right = canvasRect.right;
		imageRect[2].bottom = canvasRect.top + canvasRect.Height() / 2 - 1;

		//画像3 = 左下
		imageRect[3].left = canvasRect.left;
		imageRect[3].top = canvasRect.top + canvasRect.Height() / 2;
		imageRect[3].right = canvasRect.left + canvasRect.Width() / 3 - 1;
		imageRect[3].bottom = canvasRect.bottom;

		//画像4 = 左中央
		imageRect[4].left = canvasRect.left + canvasRect.Width() / 3;
		imageRect[4].top = canvasRect.top + canvasRect.Height() / 2;
		imageRect[4].right = canvasRect.left + canvasRect.Width() *2/ 3 - 1;
		imageRect[4].bottom = canvasRect.bottom;

		//画像5 = 右下
		imageRect[5].left = canvasRect.left + canvasRect.Width() *2 / 3;
		imageRect[5].top = canvasRect.top + canvasRect.Height() / 2;
		imageRect[5].right = canvasRect.right;
		imageRect[5].bottom = canvasRect.bottom;

		imageCnt = 6;
	}



	return imageCnt;
}

/********************************************************************
機  能  名  称 : 画像およびダイアログアイテムの配置
関    数    名 : arrange_dlg_item
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::arrange_dlg_item()
{
	CRect imageRegn[6]={0};//画像の表示領域
	int	imageCnt=0;


	for(int i=0 ; i<6 ; i++){
		if(p_disp_image_field[i]==NULL){//表示領域のメモリ未確保の場合はなにもしない
			return;
		}
	}


	if( m_cmb_disp_num.GetCurSel()==0)//2画像表示時
	{
		//スクロールバーの表示
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V5)->ShowWindow(SW_HIDE);
	}
	else if (m_cmb_disp_num.GetCurSel() == 1)
	{//4画像表示

		//スクロールバーの表示
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V5)->ShowWindow(SW_HIDE);
	}
	else {
		//6画像表示

		//スクロールバーの表示
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V5)->ShowWindow(SW_SHOW);
	}




	//画像表示ごとの設定

	imageCnt = calc_image_regn(imageRegn);//領域計算

	for(int i=0 ; i<imageCnt ; i++)
	{
		CRect rct(imageRegn[i]);//ImageFieldで描画する矩形領域
		rct += CRect(-3,-3,-13,-13);
		
		if(rct.top>rct.bottom || rct.left>rct.right)	continue;


		//画像描画の位置設定
		p_disp_image_field[i]->FieldCoordinate.x = rct.left;
		p_disp_image_field[i]->FieldCoordinate.y = rct.top;
		p_disp_image_field[i]->FieldSize.cx = rct.Width();
		p_disp_image_field[i]->FieldSize.cy = rct.Height();

		p_disp_image_field[i]->DrawImage( get_disp_image_data(i), true);//画像描画


		//スクロールバーの位置設定
		CScrollBar *p_h_scroll, *p_v_scroll;
		get_disp_scroll_bar(i, &p_h_scroll, &p_v_scroll);
		p_h_scroll->MoveWindow( rct.left  , rct.bottom+1 , rct.Width() , 10 );//水平方向スクロールバーの配置
		p_v_scroll->MoveWindow( rct.right+1 , rct.top     , 10, rct.Height() );//垂直方向スクロールバーの配置


		//スクロールバーのスクロール量を画像にあわせて設定
		update_scrollbar(i);

		//画像上に枠と画像番号を表示
		update_image_overlap(i);

	}

}






/********************************************************************
機  能  名  称 : 
関    数    名 : update_scrollbar
引          数 : 
戻    り    値 : スクロールバーのスクロール量を設定する
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::update_scrollbar(int dispImgNo)
{
	CScrollBar *p_h_scroll, *p_v_scroll;

	get_disp_scroll_bar(dispImgNo, &p_h_scroll, &p_v_scroll);//スクロールバーのポインタを取得

	CDataUnit *pDispImageData = get_disp_image_data(dispImgNo);//画像データ取得

	if(pDispImageData!=NULL)//データがある場合はスクロールバーサイズをデータサイズに合わせる
	{
			CSize src_size;
			p_disp_image_field[dispImgNo]->GetSrcSize(pDispImageData, &src_size);//画像転送元の矩形領域のサイズを取得

			if( src_size.cx < pDispImageData->DataSize.cx ){//水平方向スクロールバーが現れるとき＝画像転送元の幅が画像全体の幅より小さい場合
				set_scrollbar( p_h_scroll, src_size.cx, pDispImageData->DataDispOffset.x, 0, pDispImageData->DataSize.cx - 1);
			}else{
				set_scrollbar( p_h_scroll, pDispImageData->DataSize.cx, 0, 0, 0);
			}

			if( src_size.cy < pDispImageData->DataSize.cy ){//垂直方向スクロールバーが現れるとき＝画像転送元の高さが画像全体の高さより小さい場合
				set_scrollbar( p_v_scroll, src_size.cy, pDispImageData->DataDispOffset.y, 0, pDispImageData->DataSize.cy - 1);
			}else{
				set_scrollbar( p_v_scroll, pDispImageData->DataSize.cy, 0, 0, 0);
			}
	}
	else//データがない場合はスクロールバーはフルサイズ
	{
		set_scrollbar( p_h_scroll, 0, 0, 0, 0);
		set_scrollbar( p_v_scroll, 0, 0, 0, 0);
	}
}

/********************************************************************
機  能  名  称 :
関    数    名 : update_image_overlap
引          数 :
戻    り    値 : 画像上に枠・番号を描画する
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::update_image_overlap(int dispImgNo)
{
	CRect imageRegn[6];

	calc_image_regn(imageRegn);//領域計算


	CDC *pdc = GetDC();//
	pdc->SelectStockObject(NULL_BRUSH);

	//枠の描画
	if (dispImgNo == m_selected_disp_image_no && API.GetCheck(m_hWnd, IDC_CHECK_MULTI_IMG_DLG_SELECT))
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 2, API.default_fg_color);
		CPen *poldpen = pdc->SelectObject(&pen);
		pdc->Rectangle(imageRegn[dispImgNo].left + 1, imageRegn[dispImgNo].top + 1, imageRegn[dispImgNo].right - 1, imageRegn[dispImgNo].bottom - 1);
		pdc->SelectObject(poldpen);
		pen.DeleteObject();
	}
	else
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 2, API.default_bg_color);
		CPen *poldpen = pdc->SelectObject(&pen);
		pdc->Rectangle(imageRegn[dispImgNo].left + 1, imageRegn[dispImgNo].top + 1, imageRegn[dispImgNo].right - 1, imageRegn[dispImgNo].bottom - 1);
		pdc->SelectObject(poldpen);
		pen.DeleteObject();
	}


	//画像メモリ番号の描画
	CFont font;  font.CreateFontIndirect(&API.default_font);
	CFont *poldfont = pdc->SelectObject(&font);
	CString str; str.Format(_T("%d"), m_disp_image_number[dispImgNo]);
	pdc->TextOut(imageRegn[dispImgNo].left+3, imageRegn[dispImgNo].top+3, str);
	pdc->SelectObject(poldfont);
	font.DeleteObject();
	

	ReleaseDC(pdc);
}

/********************************************************************
機  能  名  称 : 
関    数    名 : synchro_disp_image
引          数 : 
戻    り    値 : 画像の表示状態を所定の番号のものにあわせる
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::synchro_disp_image(int dispImageNo)
{
	CDataUnit *pDispImageData = get_disp_image_data(dispImageNo);//所定の番号の表示画像データ
	if(pDispImageData==NULL){
		return;
	}


	for(int i=0 ; i<6 ; i++)
	{
		if( m_cmb_disp_num.GetCurSel() == 0 && i >= 2 )	break;//2画像表示時
		else if( m_cmb_disp_num.GetCurSel() == 1 && i>=4)	break;//4画像表示時

		CDataUnit *p_du = get_disp_image_data(i);

		if(dispImageNo != i )
		{
			if( p_du != NULL)//画像データあり
			{
				if(pDispImageData->DataSize == p_du->DataSize )//画像データサイズが所定画像と同じ
				{
					p_du->DataDispOffset = pDispImageData->DataDispOffset;//表示位置をあわせる
					p_du->DispScale = pDispImageData->DispScale ;//表示倍率をあわせる

					p_disp_image_field[i]->DrawImage(p_du, true);//画像再描画

					update_scrollbar(i);//スクロールバー再描画

					update_image_overlap(i);//枠と画像番号を再描画
				}
			}
		}

	}

}


/********************************************************************
機  能  名  称 : 画像メモリ番号設定
関    数    名 : DispMultiImage
引          数 : 
戻    り    値 : 
機          能 : m_selected_disp_image_no に表示する画像メモリを
　　　　　　　　現在選択中の画像メモリ番号に設定する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::DispMultiImage()
{
	if( API.GetCheck(m_hWnd, IDC_CHECK_MULTI_IMG_DLG_SELECT) )
	{
		if(m_selected_disp_image_no>=0)
		{
			m_disp_image_number[m_selected_disp_image_no] = API.GetCurrentImageNumber();
		}
	}

	arrange_dlg_item();
}

/********************************************************************
機  能  名  称 :
関    数    名 : control_scrollbar
引          数 : UINT			nSBCode		(in)
UINT			nPos		(in)
CScrollBar		*pScrollBar	(in)スクロールバーのコントロールへのポインタ
戻    り    値 : 操作後のスクロールバーの位置
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
long CMultiImageDispDlg::control_scrollbar(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
	int dest_pos = pScrollBar->GetScrollPos();

	switch (nSBCode) {
	case SB_LEFT:			//フォーカスを持っているとき[Home]を押したとき
		break;

	case SB_RIGHT:			//フォーカスを持っているとき[End]を押したとき
		break;

	case SB_LINELEFT:		//左矢印ボタンクリック
		dest_pos -= 10;		break;

	case SB_LINERIGHT:		//右矢印ボタンクリック
		dest_pos += 10;		break;

	case SB_PAGELEFT:		//左ページ領域クリック
		dest_pos -= 10;		break;

	case SB_PAGERIGHT:		//右ページ領域クリック
		dest_pos += 10;		break;

	case SB_THUMBPOSITION:	//つまみドラックで目的地に達した
		dest_pos = nPos;	break;

	case SB_THUMBTRACK:		//つまみドラッグ中である
		dest_pos = nPos;	break;

	case SB_ENDSCROLL:		//スクロールバー処理が終了した
		break;
	}
	pScrollBar->SetScrollPos(dest_pos);//スクロールバー再表示

	return	dest_pos;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  イベントハンドラ
//////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMultiImageDispDlg, CDialog)

CMultiImageDispDlg::CMultiImageDispDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiImageDispDlg::IDD, pParent)
{
	for(int i=0 ; i<6 ; i++){
		p_disp_image_field[i] = NULL;
	}
	m_selected_disp_image_no = -1;
}

CMultiImageDispDlg::~CMultiImageDispDlg()
{
	for(int i=0 ; i<6 ; i++){
		if(p_disp_image_field[i] != NULL){
			delete	p_disp_image_field[i];
			p_disp_image_field[i] = NULL;
		}
	}
}

void CMultiImageDispDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MULTI_IMAGE_DISP_NUM, m_cmb_disp_num);
}


BEGIN_MESSAGE_MAP(CMultiImageDispDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHECK_MULTI_IMG_DLG_SELECT, &CMultiImageDispDlg::OnBnClickedCheckMultiImgDlgSelect)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_MULTI_IMG_DLG_MAGNIFY0, &CMultiImageDispDlg::OnBnClickedButtonMultiImgDlgMagnify0)
	ON_BN_CLICKED(IDC_BUTTON_MULTI_IMG_DLG_MAGNIFY1, &CMultiImageDispDlg::OnBnClickedButtonMultiImgDlgMagnify1)
	ON_CBN_SELCHANGE(IDC_COMBO_MULTI_IMAGE_DISP_NUM, &CMultiImageDispDlg::OnCbnSelchangeComboMultiImageDispNum)
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
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
BOOL CMultiImageDispDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for(int i=0 ; i<6 ; i++){
		p_disp_image_field[i] = new CImageField( m_hWnd, API.main_image_size, CPoint(MAIN_WIN_LEFT_MARGIN, MAIN_WIN_TOP_MARGIN)  );//ImageFiledのインスタンス化
		m_disp_image_number[i] = i;
	}
	

	m_cmb_disp_num.AddString("2画像"); 
	m_cmb_disp_num.AddString("4画像");
	m_cmb_disp_num.AddString("6画像");
	m_cmb_disp_num.SetCurSel(0);

	m_selected_disp_image_no=0;

	arrange_dlg_item();

	m_is_draging = false;
	

	return true;
}


/********************************************************************
機  能  名  称 : ウィンドウのリサイズ
関    数    名 : OnSize
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	arrange_dlg_item();
}

/********************************************************************
機  能  名  称 : ウィンドウの再描画
関    数    名 : OnPaint
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	arrange_dlg_item();
}


/********************************************************************
機  能  名  称 : 表示画像のスクロール
関    数    名 : scroll_image
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::scroll_image(int dispImageNo, int newOffsetX, int newOffsetY)
{
	CDataUnit *p_du = get_disp_image_data(dispImageNo);

	if (p_du == NULL)	return;//データが無ければ何もしない

	TRACE("%d,%d", newOffsetX, newOffsetY);

	p_disp_image_field[dispImageNo]->ScrollImage(p_du, CPoint(newOffsetX, newOffsetY));//画像転送元の矩形領域をスクロールさせる
	p_disp_image_field[dispImageNo]->DrawImage(p_du, false);
}


/********************************************************************
機  能  名  称 : 水平方向スクロールバーが操作された
関    数    名 : OnHScroll
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	for(int i=0 ; i<6 ; i++)//どのスクロールバーが動かされたのか探す
	{
		//スクロールバーのポインタを取得
		CScrollBar *p_h_scroll, *p_v_scroll;
		get_disp_scroll_bar(i, &p_h_scroll, &p_v_scroll);

		if( pScrollBar == p_h_scroll )
		{
			CDataUnit *p_du=NULL;//表示画像データ
			CPoint		src_offset;
			long		new_pos;

			p_du = get_disp_image_data(i);

				if(p_du == NULL)	return;//データが無ければ何もしない

			p_disp_image_field[i]->GetSrcOffset(p_du, &src_offset);//表示中の画像メモリの現在のオフセット（左上の座標）を取得
			new_pos = control_scrollbar(nSBCode, nPos, pScrollBar);//スクロールバーから新しいオフセット(x)を取得

			scroll_image(i, new_pos, src_offset.y);//画像転送元の矩形領域をスクロールさせる

			update_image_overlap(i);//枠と画像番号を再描画

			synchro_disp_image(i);//他の画像の表示位置をこの画像にあわせる

			break;
		}
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


/********************************************************************
機  能  名  称 : 垂直方向スクロールバーが操作された
関    数    名 : OnVScroll
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	for(int i=0 ; i<6 ; i++)//どのスクロールバーが動かされたのか探す
	{
		//スクロールバーのポインタを取得
		CScrollBar *p_h_scroll, *p_v_scroll;
		get_disp_scroll_bar(i, &p_h_scroll, &p_v_scroll);

		if( pScrollBar == p_v_scroll )
		{
			CDataUnit *p_du=NULL;//表示画像データ
			CPoint		src_offset;
			long		new_pos;

			p_du = get_disp_image_data(i);

				if(p_du == NULL)	return;//データが無ければ何もしない

			p_disp_image_field[i]->GetSrcOffset(p_du, &src_offset);//表示中の画像メモリの現在のオフセット（左上の座標）を取得
			new_pos = control_scrollbar(nSBCode, nPos, pScrollBar);//スクロールバーから新しいオフセット(y)を取得

			scroll_image(i, src_offset.x, new_pos);//画像転送元の矩形領域をスクロールさせる

			update_image_overlap(i);//枠と画像番号を再描画

			synchro_disp_image(i);//他の画像の表示位置をこの画像にあわせる

			break;
		}
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}




/********************************************************************
機  能  名  称 : 
関    数    名 : OnBnClickedCheckMultiImgDlgSelect
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::OnBnClickedCheckMultiImgDlgSelect()
{
	arrange_dlg_item();//再描画
}



/********************************************************************
機  能  名  称 : マウス左ボタン押下
関    数    名 : OnLButtonDown
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect imageRect[6];
	int imageCnt=0;

	
	imageCnt = calc_image_regn(imageRect);//領域計算

	for(int i=0 ; i<imageCnt ; i++)
	{
		if( imageRect[i].PtInRect(point) )
		{
			m_selected_disp_image_no = i;//選択番号変更

			arrange_dlg_item();//再描画


			GetCapture();//マウスをキャプチャーする
			m_is_draging = true;//ドラッグ中を表すフラグを立てる
			m_last_clicked_point = point;//ドラッグ開始座標を保存
			p_disp_image_field[i]->GetSrcOffset(get_disp_image_data(i), &m_drag_start_offset); //ドラッグ開始時の表示中の画像メモリのオフセットを取得


			HCURSOR csr = AfxGetApp()->LoadCursor(IDC_CURSOR_MOVE);//カーソルをドラッグ中の物に替える
			SetCursor(csr);

			break;
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}




/********************************************************************
機  能  名  称 : 
関    数    名 : OnLButtonUp
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();//マウスキャプチャーを開放する
	m_is_draging = false;


	CDialog::OnLButtonUp(nFlags, point);
}


/********************************************************************
機  能  名  称 : マウス中央ボタン押下
関    数    名 : OnMButtonDown
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	CRect imageRect[6];
	int imageCnt = 0;


	imageCnt = calc_image_regn(imageRect);//領域計算

	for (int i = 0; i<imageCnt; i++)
	{
		if (imageRect[i].PtInRect(point))
		{
			m_selected_disp_image_no = i;//選択番号変更

			arrange_dlg_item();//再描画


			GetCapture();//マウスをキャプチャーする
			m_is_draging = true;//ドラッグ中を表すフラグを立てる
			m_last_clicked_point = point;//ドラッグ開始座標を保存
			p_disp_image_field[i]->GetSrcOffset(get_disp_image_data(i), &m_drag_start_offset); //ドラッグ開始時の表示中の画像メモリのオフセットを取得


			HCURSOR csr = AfxGetApp()->LoadCursor(IDC_CURSOR_MOVE);//カーソルをドラッグ中の物に替える
			SetCursor(csr);

			break;
		}
	}

	CDialog::OnMButtonDown(nFlags, point);
}

/********************************************************************
機  能  名  称 :
関    数    名 : OnMButtonUp
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();//マウスキャプチャーを開放する
	m_is_draging = false;

	CDialog::OnMButtonUp(nFlags, point);
}



/********************************************************************
機  能  名  称 : 
関    数    名 : OnMouseMove
戻    り    値 : 
機          能 : マウスポインタがウィンドウ上で動いた
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect imageRect[6];
	int imageCnt=0;
	double srcX=-999, srcY=-999;

	//マウスポインタが画像領域に入っているかどうか
	imageCnt = calc_image_regn(imageRect);

	for(int i=0 ; i<imageCnt ; i++)
	{
		if( imageRect[i].PtInRect(point) )//どこのウィンドウ上にマウスカーソルがあるかチェック
		{
			CDataUnit *pdu = get_disp_image_data(i);
			if(get_disp_image_data(i))
			{
				if (m_is_draging) //ドラッグ中ならば、画像をスクロールする
				{
					HCURSOR csr = AfxGetApp()->LoadCursor(IDC_CURSOR_MOVE);//カーソルをドラッグ中の物に替える
					SetCursor(csr);

					
					double src0_x, src0_y, src1_x, src1_y;
					p_disp_image_field[i]->GetSrcCoor(pdu, m_last_clicked_point.x, m_last_clicked_point.y, &src0_x, &src0_y);//ドラッグ開始の元画像座標取得
					p_disp_image_field[i]->GetSrcCoor(pdu, point.x, point.y, &src1_x, &src1_y);//ドラッグ終了点の元画像座標取得
					scroll_image(i, m_drag_start_offset.x - (src1_x - src0_x), m_drag_start_offset.y - (src1_y - src0_y));//画像転送元の矩形領域をスクロールさせる

					update_scrollbar(i);//スクロールバー再描画

					update_image_overlap(i);//枠と画像番号を再描画

					synchro_disp_image(i);//他の画像の表示位置をこの画像にあわせる

				}

				p_disp_image_field[i]->GetSrcCoor(pdu, point.x , point.y, &srcX, &srcY);//元画像上の座標取得

				break;
			}
		}
	}

	//マウスポインタが画像領域に入っていれば、
	//画像上の座標および各画像の画素値を表示する
	if(srcX>=0 && srcY>=0)
	{
		CString message,str;

		message.Format("(x,y)=(%d,%d)    ", (int)srcX, (int)srcY );

		for(int i=0 ; i<imageCnt ; i++)
		{
			str.Format("I(%d)=", m_disp_image_number[i]);

			message += str;

			CDataUnit *pdu = get_disp_image_data(i);
			if(pdu){
				str.Format("%lf     ", pdu->GetDataValue( (int)srcX, (int)srcY) );
			}else{
				str.Format("*  ");
			}
			message += str;
		}

		API.SetEditValue(m_hWnd, IDC_MULTI_IMAGE_DISP_OUTPUT, message);
	}

	CDialog::OnMouseMove(nFlags, point);
}


/********************************************************************
機  能  名  称 : 
関    数    名 : OnMouseMove
戻    り    値 : 
機          能 : 表示画像の拡大・縮小
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
BOOL CMultiImageDispDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	RECT Rct={0};
	CRect imageRect[6];
	int imageCnt=0;


	::GetWindowRect(m_hWnd, &Rct);//このウィンドウのクライアント領域を表す矩形を取得
	CPoint point = pt - CPoint(Rct.left, Rct.top);//ウィンドウ上の座標に変換

	//領域計算
	imageCnt = calc_image_regn(imageRect);

	for(int i=0 ; i<imageCnt ; i++)
	{
		if( imageRect[i].PtInRect(point) )//どこのウィンドウ上にマウスカーソルがあるかチェック
		{
			CDataUnit *pdu = get_disp_image_data(i);
			if(pdu)
			{
				//スケールを変更して描画
				float scale = pdu->DispScale;
				if(zDelta<0){
					scale*=0.9;//10%縮小
				}else{
					scale*=1.1;//10%拡大
				}

				p_disp_image_field[i]->ScaleImage(pdu, scale);
				p_disp_image_field[i]->DrawImage(pdu, true);//画像再描画

				update_scrollbar(i);//スクロールバー再描画

				update_image_overlap(i);//枠と画像番号を再描画

				synchro_disp_image(i);//他の画像の表示位置をこの画像にあわせる
			}
		}
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

/********************************************************************
機  能  名  称 : 表示画像数変更
関    数    名 : OnCbnSelchangeComboMultiImageDispNum
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::OnCbnSelchangeComboMultiImageDispNum()
{
	arrange_dlg_item();//再描画
}


/********************************************************************
機  能  名  称 : 2倍拡大
関    数    名 : OnBnClickedButtonMultiImgDlgMagnify0
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::OnBnClickedButtonMultiImgDlgMagnify0()
{
	if( m_selected_disp_image_no >=0 )
	{
		CDataUnit *pdu = get_disp_image_data(m_selected_disp_image_no);
		if(pdu)
		{
			float scale = pdu->DispScale;

			p_disp_image_field[m_selected_disp_image_no]->ScaleImage(pdu, scale*2.0);
			p_disp_image_field[m_selected_disp_image_no]->DrawImage(pdu, false);//画像再描画

			update_scrollbar(m_selected_disp_image_no);//スクロールバー再描画

			update_image_overlap(m_selected_disp_image_no);//枠と画像番号を再描画

			synchro_disp_image(m_selected_disp_image_no);//他の画像の表示位置をこの画像にあわせる
		}
	}
}


/********************************************************************
機  能  名  称 : 1/2倍縮小
関    数    名 : OnBnClickedButtonMultiImgDlgMagnify1
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMultiImageDispDlg::OnBnClickedButtonMultiImgDlgMagnify1()
{
	if( m_selected_disp_image_no >=0 )
	{
		CDataUnit *pdu = get_disp_image_data(m_selected_disp_image_no);
		if(pdu)
		{
			float scale = pdu->DispScale;

			p_disp_image_field[m_selected_disp_image_no]->ScaleImage(pdu, scale/2.0);
			p_disp_image_field[m_selected_disp_image_no]->DrawImage(pdu, false);//画像再描画

			update_scrollbar(m_selected_disp_image_no);//スクロールバー再描画

			update_image_overlap(m_selected_disp_image_no);//枠と画像番号を再描画

			synchro_disp_image(m_selected_disp_image_no);//他の画像の表示位置をこの画像にあわせる
		}
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
BOOL CMultiImageDispDlg::PreTranslateMessage(MSG* pMsg) 
{
	POINT point;
	
	if(pMsg->message == WM_KEYDOWN ){	
		switch(pMsg->wParam){
			case VK_RETURN://Enter
				return  true;

			case VK_ESCAPE://Esc
				return  true;

			case VK_UP://↑
				GetCursorPos( &point ) ;
				SetCursorPos(point.x , point.y - 1);
				return  true;

			case VK_DOWN://↓
				GetCursorPos( &point ) ;
				SetCursorPos(point.x , point.y + 1);
				return  true;

			case VK_LEFT://←
				GetCursorPos( &point ) ;
				SetCursorPos(point.x - 1 , point.y );
				return  true;

			case VK_RIGHT://→
				GetCursorPos( &point ) ;
				SetCursorPos(point.x + 1 , point.y );
				return true;

			default:
				break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}






