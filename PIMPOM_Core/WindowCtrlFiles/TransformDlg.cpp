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
//CTransformDlg
//画像の幾何学変換設定ウィンドウ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "TransformDlg.h"

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
IMPLEMENT_DYNAMIC(CTransformDlg, CDialog)

CTransformDlg::CTransformDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransformDlg::IDD, pParent)
{

}

CTransformDlg::~CTransformDlg()
{
}

void CTransformDlg::DoDataExchange(CDataExchange* pDX)
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
void CTransformDlg::show_dlg_item()
{
	if(API.GetCheck(m_hWnd,IDC_RADIO_TRANSFORM_OVERWITE)){
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_TRANSFORM_FROM_IMAGE_NO), false);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_TRANSFORM_TO_IMAGE_NO), false);
	}else{
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_TRANSFORM_FROM_IMAGE_NO), true);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_TRANSFORM_TO_IMAGE_NO), true);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//  メッセージ ハンドラ
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTransformDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTransformDlg::OnBnClickedOk)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_TRANSFORM_OVERWITE, &CTransformDlg::OnBnClickedRadioTransformOverwite)
	ON_BN_CLICKED(IDC_RADIO_TRANSFORM_TO_IMAGE, &CTransformDlg::OnBnClickedRadioTransformToImage)
	ON_BN_CLICKED(IDC_BUTTON_TRASFORM_SET_ORG_IMG, &CTransformDlg::OnBnClickedButtonTrasformSetOrgImg)
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
BOOL CTransformDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	//「結果を別メモリへ入れる」を初期値にする
	API.SetCheck(m_hWnd, IDC_RADIO_TRANSFORM_TO_IMAGE, true);
	show_dlg_item();


	//結果を入れる画像メモリ番号を設定
	int dst_num, src_num; 
	src_num = API.GetCurrentImageNumber();
	dst_num = src_num + 1;

	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_FROM_IMAGE_NO, src_num);
	if(dst_num<API.GetDataUnitNumber())		API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_TO_IMAGE_NO, dst_num);


	//各種パラメタの初期値設定
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_TI, 0);
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_TJ, 0);	
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_FL, 16.0);	
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_CCD, 7.4);	
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_THETA, 0);	
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_PHAI, 0);	
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_PSI, 0);	
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_TX, 0);	
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_TY, 0);	
	


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
void CTransformDlg::OnBnClickedOk()
{
	CDataUnit	*p_du;
	double		ti, tj,tx,ty,theta,phai,psi,fl,ccd;
	int			outRgnMode = 0;

	if( (p_du = API.GetDataUnit(CURRENT_IMAGE) )==NULL)	return;

	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_TI, &ti);
	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_TJ, &tj);	
	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_FL, &fl);	
	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_CCD, &ccd);	
	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_THETA, &theta);	
	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_PHAI, &phai);	
	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_PSI, &psi);	
	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_TX, &tx);	
	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_TY, &ty);	
	if (API.GetCheck(m_hWnd, IDC_CHECK_TRANSFORM_OUTRGN)) {
		outRgnMode = 1;
	}
	

	if(API.GetCheck(m_hWnd, IDC_RADIO_TRANSFORM_OVERWITE))
	{
		API.BackupDataUnit(CURRENT_IMAGE);//現在のデータバックアップ

		if( !API.PerspectiveTransDataUnit( CURRENT_IMAGE, ti, tj,tx,ty,theta,phai,psi,fl,ccd,true, outRgnMode))	return ;

		API.DrawImage();//描画
	}
	else
	{
		int src_no=0,dst_no=1;
		API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_FROM_IMAGE_NO, &src_no);
		API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_TO_IMAGE_NO, &dst_no);

		API.BackupDataUnit(dst_no);//現在のデータバックアップ

		if( !API.PerspectiveTransDataUnit( src_no, dst_no, false, ti, tj,tx,ty,theta,phai,psi,fl,ccd,true,NULL,1, outRgnMode))	return ;
		
		API.DrawImage(dst_no);//描画
	}
}

void CTransformDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//解説用のビットマップを貼り付け領域
	CRect rgn;
	API.GetDialogItemRect(m_hWnd, IDC_STATIC_TRANSFORM_DESCRIPTION, &rgn);

	//解説用のビットマップを貼り付け
	CBitmap bmp;
	bmp.LoadBitmap(IDB_TRANSFORM);
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
機  能  名  称 : 「現在選択中の画像メモリに上書き」
関    数    名 : OnBnClickedRadioTransformOverwite
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CTransformDlg::OnBnClickedRadioTransformOverwite()
{
	show_dlg_item();
}


/********************************************************************
機  能  名  称 : 
関    数    名 : OnBnClickedRadioTransformToImage
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CTransformDlg::OnBnClickedRadioTransformToImage()
{
	show_dlg_item();
}


/********************************************************************
機  能  名  称 : 
関    数    名 : OnBnClickedButtonTrasformSetOrgImg
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CTransformDlg::OnBnClickedButtonTrasformSetOrgImg()
{
	int orgNo;

	orgNo = API.GetCurrentImageNumber();

	if(orgNo>=0 && orgNo+1<API.GetDataUnitNumber())
	{
		API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_FROM_IMAGE_NO, orgNo);
		API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_TO_IMAGE_NO, orgNo+1);
	}
}
