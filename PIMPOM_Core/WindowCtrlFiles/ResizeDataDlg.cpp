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
//CResizeDataDlg
//画像サイズ変更設定ウィンドウ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "ResizeDataDlg.h"

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
IMPLEMENT_DYNAMIC(CResizeDataDlg, CDialog)

CResizeDataDlg::CResizeDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResizeDataDlg::IDD, pParent)
{

}

CResizeDataDlg::~CResizeDataDlg()
{
}

void CResizeDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


/********************************************************************
機  能  名  称 : ダイアログアイテムの表示状態変更
関    数    名 : show_dlg_item
引          数 :
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CResizeDataDlg::show_dlg_item()
{
	if(API.GetCheck(m_hWnd,IDC_RADIO_RESIZE_OVERWITE)){
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_RESIZE_FROM_IMAGE_NO), false);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_RESIZE_TO_IMAGE_NO), false);
	}else{
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_RESIZE_FROM_IMAGE_NO), true);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_RESIZE_TO_IMAGE_NO), true);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
//  メッセージ ハンドラ
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CResizeDataDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CResizeDataDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RESIZE_HALF, &CResizeDataDlg::OnBnClickedButtonResizeHalf)
	ON_BN_CLICKED(IDC_BUTTON_RESIZE_DOUBLE, &CResizeDataDlg::OnBnClickedButtonResizeDouble)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_RESIZE_STRETCH_DATA, &CResizeDataDlg::OnBnClickedCheckResizeStretchData)
	ON_BN_CLICKED(IDC_RADIO_RESIZE_OVERWITE, &CResizeDataDlg::OnBnClickedRadioResizeOverwite)
	ON_BN_CLICKED(IDC_RADIO_RESIZE_TO_IMAGE, &CResizeDataDlg::OnBnClickedRadioResizeToImage)
	ON_BN_CLICKED(IDC_BUTTON_RESIZE_SET_ORG_IMG, &CResizeDataDlg::OnBnClickedButtonResizeSetOrgImg)
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
BOOL CResizeDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CDataUnit *p_du=NULL;
	if( (p_du = API.GetDataUnit(CURRENT_IMAGE)) ==NULL)	return false;


	//「結果を別メモリへ入れる」をデフォルトにする
	API.SetCheck(m_hWnd, IDC_RADIO_RESIZE_TO_IMAGE, true);
	show_dlg_item();


	//結果を入れる画像メモリ番号を設定
	int dst_num, src_num; 
	src_num = API.GetCurrentImageNumber();
	dst_num = src_num + 1;

	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_FROM_IMAGE_NO, src_num);
	if(dst_num<API.GetDataUnitNumber())		API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_TO_IMAGE_NO, dst_num);


	//各種パラメタの初期値設定
	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_W, p_du->DataSize.cx);
	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_H, p_du->DataSize.cy);
	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_OFFSET_X, 0);
	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_OFFSET_Y, 0);
	API.SetCheck(m_hWnd,IDC_RADIO_RESIZE_INTP0,true);


	return true;
}

/********************************************************************
機  能  名  称 : 「実行」ボタン押下
関    数    名 : OnBnClickedOk
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CResizeDataDlg::OnBnClickedOk()
{
	CDataUnit	*p_du;
	int			width, height, offset_x, offset_y, interp_type;

	if( (p_du = API.GetDataUnit(CURRENT_IMAGE) )==NULL)	return;

	API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_W, &width);
	API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_H, &height);
	API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_OFFSET_X, &offset_x);
	API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_OFFSET_Y, &offset_y);

	if(API.GetCheck(m_hWnd,IDC_RADIO_RESIZE_INTP1)){
		interp_type=1;
	}else if(API.GetCheck(m_hWnd,IDC_RADIO_RESIZE_INTP2)){
		interp_type=2;
	}else{
		interp_type=0;
	}
	

	if(API.GetCheck(m_hWnd, IDC_RADIO_RESIZE_OVERWITE))
	{
		API.BackupDataUnit(CURRENT_IMAGE);//現在のデータバックアップ

		if( !API.ResizeDataUnit( CURRENT_IMAGE, CSize(width,height), API.GetCheck(m_hWnd, IDC_CHECK_RESIZE_STRETCH_DATA),offset_x,offset_y,true, interp_type))	return ;//リサイズ実行

		API.DrawImage();//描画
	}
	else
	{
		int src_no=0,dst_no=0;
		API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_FROM_IMAGE_NO, &src_no);
		API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_TO_IMAGE_NO, &dst_no);

		API.BackupDataUnit(dst_no);//現在のデータバックアップ

		if( !API.ResizeDataUnit( src_no, dst_no, false, CSize(width,height), API.GetCheck(m_hWnd, IDC_CHECK_RESIZE_STRETCH_DATA),offset_x,offset_y,true, interp_type))	return ;//リサイズ実行
		
		API.DrawImage(dst_no);//描画
	}
}

/********************************************************************
機  能  名  称 : 「1/2」ボタン押下
関    数    名 : OnBnClickedButtonResizeHalf
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CResizeDataDlg::OnBnClickedButtonResizeHalf()
{
	int			width, height;	

	API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_W, &width);
	API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_H, &height);

	if(width<10 || height<10)	return;
	width /=2;
	height /=2;

	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_W, width);
	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_H, height);
}

/********************************************************************
機  能  名  称 : 「x2」ボタン押下
関    数    名 : OnBnClickedButtonResizeDouble
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CResizeDataDlg::OnBnClickedButtonResizeDouble()
{
	int			width, height;	

	API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_W, &width);
	API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_H, &height);

	width *=2;
	height *=2;

	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_W, width);
	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_H, height);
}


/********************************************************************
機  能  名  称 : 描画
関    数    名 : OnPaint
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CResizeDataDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//解説用のビットマップを貼り付け領域
	CRect rgn;
	API.GetDialogItemRect(m_hWnd, IDC_STATIC_RESIZE_RESCRIPTION, &rgn);
	

	//解説用のビットマップを貼り付け
	CBitmap bmp;
	if(API.GetCheck(m_hWnd,IDC_CHECK_RESIZE_STRETCH_DATA)){
		bmp.LoadBitmap(IDB_RESIZE2);
	}else{
		bmp.LoadBitmap(IDB_RESIZE1);
	}
	CDC* pDC=GetDC();
	CDC bmpDC;
	bmpDC.CreateCompatibleDC(pDC);
	CBitmap* oldbmp=bmpDC.SelectObject(&bmp);
	pDC->BitBlt(rgn.left, rgn.top, rgn.Width(), rgn.Height(),&bmpDC,0,0,SRCCOPY);
	bmpDC.SelectObject(oldbmp);
	bmp.DeleteObject();
	ReleaseDC(pDC);
}

/********************************************************************
機  能  名  称 : 「サイズに合わせて画像データを伸縮する」
関    数    名 : OnBnClickedCheckResizeStretchData
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CResizeDataDlg::OnBnClickedCheckResizeStretchData()
{
	Invalidate();
}


/********************************************************************
機  能  名  称 : 「現在選択中の画像メモリに上書き」
関    数    名 : OnBnClickedRadioResizeOverwite
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CResizeDataDlg::OnBnClickedRadioResizeOverwite()
{
	show_dlg_item();
}

/********************************************************************
機  能  名  称 :
関    数    名 : OnBnClickedRadioResizeOverwite
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CResizeDataDlg::OnBnClickedRadioResizeToImage()
{
	show_dlg_item();
}


/********************************************************************
機  能  名  称 :
関    数    名 : OnBnClickedButtonResizeSetOrgImg
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CResizeDataDlg::OnBnClickedButtonResizeSetOrgImg()
{
	int orgNo;

	orgNo = API.GetCurrentImageNumber();

	if(orgNo>=0 && orgNo+1<API.GetDataUnitNumber())
	{
		API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_FROM_IMAGE_NO, orgNo);
		API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_TO_IMAGE_NO, orgNo+1);
	}
}
