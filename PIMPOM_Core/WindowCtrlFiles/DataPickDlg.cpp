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
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "DataPickDlg.h"

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

//////////////////////////////////////////////////////////////////////////////////////////
//  メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CDataPickDlg, CDialog)

CDataPickDlg::CDataPickDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataPickDlg::IDD, pParent)
{

}

CDataPickDlg::~CDataPickDlg()
{
	paintbmp.DeleteObject(); 
	pickbmp.DeleteObject();
	fillbm.DeleteObject();
	undobmp.DeleteObject();
}

void CDataPickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/********************************************************************
機  能  名  称 : データピックモードなのか、ペイントモードなのか取得
関    数    名 : GetDataPickMode
引          数 : 
戻    り    値 :  DATAPICK_PICK:ピック  DATAPICK_PAINT:ペイント DATAPICK_FILL:フィル
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
int CDataPickDlg::GetDataPickMode()
{
	if (API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK))
	{
		return DATAPICK_PICK;
	}
	else if (API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_DOT))
	{
		return DATAPICK_PAINT;
	}
	else 
	{
		return DATAPICK_FILL;
	}
}

/********************************************************************
機  能  名  称 : データピックモードなのか、ペイントモードなのか設定
関    数    名 : SetDataPickMode
引          数 : DATAPICK_PICK:ピック  DATAPICK_PAINT:ペイント DATAPICK_FILL:フィル
戻    り    値 : 
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CDataPickDlg::SetDataPickMode(int mode)
{
	if (mode== DATAPICK_PICK)
	{
		API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK, true);
	}
	else if (mode == DATAPICK_PAINT)
	{
		API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_DOT, true);
		show_dlg_item();
	}
	else
	{
		API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_FILL, true);
		show_dlg_item();
	}
	
}

/********************************************************************
機  能  名  称 : 特定の座標の画素をピックする
関    数    名 : DataPick
引          数 : CPoint point	(in)ピックする座標
戻    り    値 : 正常に終了したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataPickDlg::DataPick(CPoint point)
{
	if( !API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK) )		return false;

	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中のDataUnitを取得

		if(p_du == NULL)		return false;//データが無ければなにもしない
		if(point.x<0 || point.y<0 || point.x >= p_du->DataSize.cx || point.y >= p_du->DataSize.cy)	return false;

		//データピック
		if( (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) || p_du->DispRGB==false )
		{//モノクロ
			double bri;
			bri = p_du->GetDataValue(point,-1,-1);
			API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA0,bri);
			API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA1,bri);
			API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA2,bri);
		}
		else
		{//RGB
			double r,g,b;
			r = p_du->GetDataValue(point, -1,0);
			g = p_du->GetDataValue(point, -1,1);
			b = p_du->GetDataValue(point, -1,2);
			API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA0,r);
			API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA1,g);
			API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA2,b);

		}

		//マスクピック
		if(p_du->GetMaskValue(point)==MASKED_PIXEL){
			API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK_MASK_ON, true);
			API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK_MASK_OFF, false);
		}else{
			API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK_MASK_ON, false);
			API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK_MASK_OFF, true);
		}

	return true;
}





/********************************************************************
機  能  名  称 : 特定の座標の画素にドットを描画
関    数    名 : PaintDot
引          数 : CPoint point	(in)描画する座標
戻    り    値 : 正常に終了したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataPickDlg::PaintDot(CPoint point)
{
	if( API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK) || !API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_DOT))		return false;

	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中のDataUnitを取得
	if(p_du == NULL)		return false;//データが無ければなにもしない

	int dot_size;
	double r,g,b;
	API.GetEditValue(m_hWnd, IDC_EDIT_DATAPICK_PAINT_DOT_SIZE, &dot_size);
	API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA0,&r);
	API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA1,&g);
	API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA2,&b);


	if(!API.GetCheck(m_hWnd,IDC_CHECK_DATAPICK_MASK))
	{//データに描画
		if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
		{//RGB
			API.DrawPointOnImage(CURRENT_IMAGE, point.x, point.y, dot_size, RGB((BYTE)r,(BYTE)g,(BYTE)b));
		}
		else
		{//モノクロ
			API.DrawPointOnImage(CURRENT_IMAGE, point.x, point.y, dot_size, r);
		}
	}
	else
	{//マスクに描画
		int mask_val=UNMASKED_PIXEL;
		if(API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_MASK_ON))	mask_val=MASKED_PIXEL;
		API.DrawPointOnMask(CURRENT_IMAGE, point.x, point.y, dot_size, mask_val);
	}

	return true;
}

/********************************************************************
機  能  名  称 : 特定の座標の画素を起点として、単一色で塗りつぶす
関    数    名 : PaintFloodFill
引          数 : CPoint point	(in)起点とする座標
戻    り    値 : 正常に終了したらtrue
機          能 : まず画像全体でラべリングを行い、起点位置と同ラベルになった画素をすべて塗りつぶす
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataPickDlg::PaintFloodFill(CPoint point)
{
	if( API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK) || API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_DOT))		return false;

	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中のDataUnitを取得
	if(p_du == NULL)		return false;//データが無ければなにもしない

	double r,g,b;
	API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA0,&r);
	API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA1,&g);
	API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA2,&b);

	if(!API.GetCheck(m_hWnd,IDC_CHECK_DATAPICK_MASK))
	{//データに描画
		if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
		{//RGB
			API.FloodFillOnImage(CURRENT_IMAGE, point.x, point.y, RGB((BYTE)r,(BYTE)g,(BYTE)b));
		}
		else
		{//モノクロ
			API.FloodFillOnImage(CURRENT_IMAGE, point.x, point.y, r);
		}
	}
	else
	{//マスクに描画
		int mask_val=UNMASKED_PIXEL;
		if(API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_MASK_ON))	mask_val=MASKED_PIXEL;
		API.FloodFillOnMask(CURRENT_IMAGE, point.x, point.y, mask_val);
	}

	return true;
}

/********************************************************************
機  能  名  称 : メイン画面上でマウスクリックされたときに呼ばれる関数
関    数    名 : OnMouseDownOnImage
引          数 : CPoint point	(in)画像座標
戻    り    値 : 
機          能 : ダイアログの状態に合わせ、ピック／ペイント／塗りつぶしを行う
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataPickDlg::OnMouseDownOnImage(CPoint point)
{
	if(!IsWindowVisible())	return true;//非表示状態では無効

	bool ret=false;
	if(API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK))
	{//ピック
		ret = DataPick(point);
	}
	else
	{//ペイント
		HCURSOR csr = AfxGetApp()->LoadCursor( IDC_CURSOR_PEN );//カーソルをペンに替える
		SetCursor( csr );

		if(API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_DOT))
		{//ドット
			API.BackupDataUnit(CURRENT_IMAGE);
			ret = PaintDot(point);
		}
		else
		{//塗りつぶし
			API.BackupDataUnit(CURRENT_IMAGE);
			ret = PaintFloodFill(point);
		}

		API.DrawImage();
	}

	return ret;
}

/********************************************************************
機  能  名  称 : メイン画面上でマウスドラッグされたときに呼ばれる関数
関    数    名 : OnMouseDrugOnImage
引          数 : CPoint point	(in)画像座標
戻    り    値 : 
機          能 : ペイントモードのときは、ドラッグ位置にペイントを行う
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataPickDlg::OnMouseDrugOnImage(CPoint point)
{
	if(!IsWindowVisible())	return true;//非表示状態では無効

	bool ret=false;
	if(API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK))
	{//ピック

	}
	else
	{//ペイント
		if(API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_DOT))
		{//ドット
			ret = PaintDot(point);
		}
		else
		{//塗りつぶし
			ret = PaintFloodFill(point);
		}
		API.DrawImage();
	}

	return ret;
}

/********************************************************************
機  能  名  称 : メイン画面上をマウスが通貨したときに呼ばれる関数
関    数    名 : OnMouseMoveOnImage
引          数 : CPoint point	(in)画像座標
戻    り    値 : 
機          能 : ペイントモードのときはアイコンを変更する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataPickDlg::OnMouseMoveOnImage(CPoint point)
{
	if(!IsWindowVisible())	return true;//非表示状態では無効

	if(API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK))
	{//ピック

	}
	else
	{//ペイント
		int cur_id;
		if (API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_DOT))
		{//ドット
			cur_id = IDC_CURSOR_PEN;
		}
		else 
		{//塗りつぶし
			cur_id = IDC_CURSOR_FLOOD;
		}
		HCURSOR csr = AfxGetApp()->LoadCursor(cur_id);//カーソルをペンに替える
		SetCursor( csr );
	}
	return true;
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
void CDataPickDlg::show_dlg_item()
{
	int pickflg, maskflg;

	//ピック・ペイントに連動して変わるもの

	if( API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK) )	pickflg = FALSE;
	else	pickflg = TRUE;

	::EnableWindow(::GetDlgItem(m_hWnd, IDC_GROUP_DATAPICK_PAINT_DETAIL), pickflg);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_DATAPICK_PAINT_FILL_WORKAREA), pickflg);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_LABEL_DATAPICK_PAINT_DOT), pickflg);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_DATAPICK_PAINT_DOT_SIZE), pickflg);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_DATAPICK_UNDO), pickflg);

	//マスクONOFFに連動して変わるもの

	if( API.GetCheck(m_hWnd, IDC_CHECK_DATAPICK_MASK) )	maskflg = TRUE;
	else	maskflg = FALSE;

	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_DATAPICK_DATA0), !maskflg);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_DATAPICK_DATA1), !maskflg);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_DATAPICK_DATA2), !maskflg);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_DATAPICK_MASK_ON), maskflg);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_DATAPICK_MASK_OFF), maskflg);
}

//////////////////////////////////////////////////////////////////////////////////////////
//  メッセージ ハンドラ
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDataPickDlg, CDialog)

	ON_BN_CLICKED(IDC_BUTTON_DATAPICK_PAINT_FILL_WORKAREA, &CDataPickDlg::OnBnClickedButtonFillWorkarea)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RADIO_DATAPICK, &CDataPickDlg::OnBnClickedRadioDatapick)
	ON_BN_CLICKED(IDC_RADIO_DATAPICK_PAINT_DOT, &CDataPickDlg::OnBnClickedRadioDatapaint)
	ON_BN_CLICKED(IDC_RADIO_DATAPICK_PAINT_FILL, &CDataPickDlg::OnBnClickedRadioDatafill)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHECK_DATAPICK_MASK, &CDataPickDlg::OnBnClickedCheckDatapickMask)
	ON_BN_CLICKED(IDC_BUTTON_DATAPICK_UNDO, &CDataPickDlg::OnBnClickedUndo)
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
BOOL CDataPickDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//ビットマップボタン
	pickbmp.LoadBitmap(IDB_BUTTON_PICK);
	paintbmp.LoadBitmap(IDB_BUTTON_PAINT);
	fillbm.LoadBitmap(IDB_BUTTON_FILL);
	undobmp.LoadBitmap(IDB_BUTTON_UNDO);
	((CButton*)GetDlgItem(IDC_RADIO_DATAPICK))->SetBitmap((HBITMAP)pickbmp);
	((CButton*)GetDlgItem(IDC_RADIO_DATAPICK_PAINT_DOT))->SetBitmap((HBITMAP)paintbmp);
	((CButton*)GetDlgItem(IDC_RADIO_DATAPICK_PAINT_FILL))->SetBitmap((HBITMAP)fillbm);
	((CButton*)GetDlgItem(IDC_BUTTON_DATAPICK_UNDO))->SetBitmap((HBITMAP)undobmp);

	//ツールチップ
	m_toolTip.Create(this, TTS_ALWAYSTIP);
	m_toolTip.AddTool(GetDlgItem(IDC_RADIO_DATAPICK), "ピック");
	m_toolTip.AddTool(GetDlgItem(IDC_RADIO_DATAPICK_PAINT_DOT), "ペイント");
	m_toolTip.AddTool(GetDlgItem(IDC_RADIO_DATAPICK_PAINT_FILL), "塗りつぶし");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_DATAPICK_UNDO), "元に戻す");

	//値の初期化
	API.SetCheck(m_hWnd,IDC_CHECK_DATAPICK_MASK,false);
	API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK, true);
	API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_DOT, false);
	API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_FILL, false);
	API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA0,0);
	API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA1,0);
	API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA2,0);
	API.SetEditValue(m_hWnd, IDC_EDIT_DATAPICK_PAINT_DOT_SIZE, 1);
	CSpinButtonCtrl *spin1  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_PAINT_DOT_SIZE);
	spin1->SetRange(1,100);	

	show_dlg_item();
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/********************************************************************
機  能  名  称 : このウィンドウが可視化された
関    数    名 : OnShowWindow
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataPickDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow) {
		show_dlg_item();
	}
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
BOOL CDataPickDlg::PreTranslateMessage(MSG* pMsg)
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
		if (pMsg->hwnd == GetDlgItem(IDC_RADIO_DATAPICK)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_RADIO_DATAPICK_PAINT_DOT)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_DATAPICK_UNDO)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_RADIO_DATAPICK_PAINT_FILL)->m_hWnd
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
機  能  名  称 : 作業領域内部を塗りつぶす
関    数    名 : OnBnClickedButtonFillWorkarea
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataPickDlg::OnBnClickedButtonFillWorkarea()
{
	if(!API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK))
	{
		CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中のDataUnitを取得
		if(p_du == NULL)		return ;//データが無ければなにもしない


		API.BackupDataUnit(CURRENT_IMAGE);//データのバックアップ

		if(!API.GetCheck(m_hWnd,IDC_CHECK_DATAPICK_MASK))
		{//データに描画
			if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
			{//RGB
				double r=0,g=0,b=0;
				API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA0,&r);
				API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA1,&g);
				API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA2,&b);

				API.FillWorkAreaOnImage(CURRENT_IMAGE, RGB(r,g,b));
			}
			else
			{//モノクロ
				double bri=0;
				API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA0,&bri);
				
				API.FillWorkAreaOnImage(CURRENT_IMAGE, bri);
				API.DrawImage();
			}
		}else{//マスクに描画
			int mask_val=UNMASKED_PIXEL;
			if(API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_MASK_ON))	mask_val=MASKED_PIXEL;

			API.FillWorkAreaOnMask(CURRENT_IMAGE, mask_val);
		}
	}
	API.DrawImage();
}




/********************************************************************
機  能  名  称 : ウィンドウを閉じる
関    数    名 : OnClose
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataPickDlg::OnClose()
{
	CDialog::OnClose();

	API.SetDataPickEditType(0);
}

/********************************************************************
機  能  名  称 : ピックモード選択
関    数    名 : OnBnClickedRadioDatapick
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataPickDlg::OnBnClickedRadioDatapick()
{
	show_dlg_item();
}

/********************************************************************
機  能  名  称 : ペイントモード選択
関    数    名 : OnBnClickedRadioDatapaint
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataPickDlg::OnBnClickedRadioDatapaint()
{
	show_dlg_item();
}

/********************************************************************
機  能  名  称 : フィルモード選択
関    数    名 : OnBnClickedRadioDatafill
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CDataPickDlg::OnBnClickedRadioDatafill()
{
	show_dlg_item();
}

/********************************************************************
機  能  名  称 : マスクON/OFF選択
関    数    名 : OnBnClickedCheckDatapickMask
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataPickDlg::OnBnClickedCheckDatapickMask()
{
	show_dlg_item();
}


/********************************************************************
機  能  名  称 : Undo
関    数    名 : OnBnClickedUndo
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CDataPickDlg::OnBnClickedUndo()
{
	API.RecoverDataUnit(CURRENT_IMAGE);
}