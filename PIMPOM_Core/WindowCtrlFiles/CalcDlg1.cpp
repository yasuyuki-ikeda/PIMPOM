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
//CCalcDlg
//演算式設定ウィンドウ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "CalcDlg1.h"

// *************************************
//         マ  ク  ロ   定   義         
// *************************************
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

CCalcDlg::CCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcDlg)

	//}}AFX_DATA_INIT
}


void CCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcDlg)
	DDX_Control(pDX, IDC_COMBO_CALC_COMMAND, m_combo_command);
	DDX_Control(pDX, IDC_PROG_CALC, m_prog);
	//}}AFX_DATA_MAP
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
void CCalcDlg::show_dlg_item()
{
	bool enable_new_image=false, enable_mask_image=false;

	if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_NEW_IMAGE)){//新しく画像メモリ取得
		enable_new_image = true;
		enable_mask_image=false;
	}else if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_OVERWRITE_IMAGE)){//現在のメモリに設定
		enable_new_image = false;
		enable_mask_image=false;
	}else{//マスク生成
		enable_new_image = false;
		enable_mask_image=true;
	}

	//新しい画像メモリを取得するための設定値
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CALC_IMAGE_SIZE_W), enable_new_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CALC_IMAGE_SIZE_H), enable_new_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_BYTE), enable_new_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_SHORT), enable_new_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_WORD), enable_new_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_LONG), enable_new_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_DWORD), enable_new_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_FLOAT), enable_new_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_RGB), enable_new_image);

	//モデルに書き込むための設定値
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CALC_MASK_MIN), enable_mask_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CALC_MASK_MAX), enable_mask_image);
}


/********************************************************************
機  能  名  称 : ダイアログアイテムで選択されているデータタイプを取得
関    数    名 : get_data_type_from_dlg
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CCalcDlg::get_data_type_from_dlg()
{
	if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_BYTE)){		return BYTE_FORMAT;		}
	else if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_SHORT)){	return SHORT16_FORMAT;	}
	else if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_WORD)){	return WORD_FORMAT;		}
	else if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_LONG)){	return LONG32_FORMAT;	}
	else if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_DWORD)){	return DWORD_FORMAT;	}
	else if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_FLOAT)){	return FLOAT_FORMAT;	}
	else if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_RGB)){	return RGB_FORMAT;		}

	return BYTE_FORMAT;
}


/********************************************************************
機  能  名  称 : コマンド文字列をバックアップ
関    数    名 : backup_com_str
引          数 : CComboBox &combo　(in)コマンド文字列の入ったコンボボックスを参照
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCalcDlg::backup_com_str(CComboBox &combo)
{
	CString str,b_str;

	combo.GetWindowText(str);

	for(int i=0 ; i<combo.GetCount() ; i++){//バックアップされた履歴がないか調べる
		combo.GetLBText(i,b_str);

		if(b_str == str){
			return;//すでにバックアップされていれば処理終了
		}
	}

	combo.InsertString(0,str);//バックアップされてなければリストに追加
}

/********************************************************************
機  能  名  称 : 全てのコマンド文字列をバックアップ
関    数    名 : backup_all_com_str
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCalcDlg::backup_all_com_str()
{
	backup_com_str(m_combo_command);
}

/********************************************************************
機  能  名  称 : コマンド文字列の履歴をiniファイルからロード
関    数    名 : load_com_str
引          数 : CComboBox &combo　 (in)コマンド文字列の入ったコンボボックスを参照
                 CString param_name (in)iniファイルのパス
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalcDlg::load_com_str(CComboBox &combo, CString param_name)
{
	CString buf, key, b_str;
	bool	do_insert;

	for(int i=0 ; i<10 ; i++){
		key.Format("_history%d",i);
		key = param_name + key;
		buf = API.GetProfile("CALCLATION", key);
		if(!buf.IsEmpty())
		{
			do_insert = true;

			for(int j=0 ; j<combo.GetCount() ; j++){//バックアップされた履歴がないか調べる
				b_str.Empty();
				combo.GetLBText(j,b_str);
				if(b_str == buf){
					do_insert = false;
					break;
				}
			}

			if(do_insert){
				combo.AddString(buf);
			}
		}
	}

	return true;
}

/********************************************************************
機  能  名  称 : コマンド文字列の履歴をiniファイルへセーブ
関    数    名 : save_com_str
引          数 : CComboBox &combo　 (in)コマンド文字列の入ったコンボボックスを参照
                 CString param_name (in)iniファイルのパス
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalcDlg::save_com_str(CComboBox &combo, CString param_name)
{
	CString buf, key;

	for(int i=0 ; i<combo.GetCount() && i<10 ; i++){
		combo.GetLBText(i,buf);
		key.Format("_history%d",i);
		key = param_name + key;
		API.WriteProfile("CALCLATION", key, buf);
	}

	return true;
}

/********************************************************************
機  能  名  称 : 演算式を実行し、現在選択中の画像メモリに入れる
関    数    名 : execute
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalcDlg::execute()
{
	CString			com_str, err_str;
	bool			ret;

	m_combo_command.GetWindowText(com_str);//コマンド文字列;

	//現在のデータバックアップ
	if( API.GetDataUnit(CURRENT_IMAGE) != NULL ){
		API.BackupDataUnit(CURRENT_IMAGE);
	}


	//演算実行
	if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_NEW_IMAGE))//新しく画像メモリ取得
	{
		int size_w=0, size_h=0;
		API.GetEditValue(m_hWnd, IDC_EDIT_CALC_IMAGE_SIZE_W, &size_w);
		API.GetEditValue(m_hWnd, IDC_EDIT_CALC_IMAGE_SIZE_H, &size_h);
		ret = API.CalcDataValue(CURRENT_IMAGE, get_data_type_from_dlg(), CSize(size_w, size_h), com_str, 
			API.GetCheck(m_hWnd,IDC_CHECK_CALC_SATULATE) , &err_str, &m_prog);
	}
	else if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_OVERWRITE_IMAGE))//現在のメモリに上書き
	{
		ret = API.CalcDataValue(CURRENT_IMAGE, com_str, API.GetCheck(m_hWnd,IDC_CHECK_CALC_SATULATE) ,&err_str, &m_prog);
	}
	else
	{//マスク生成
		double	mask_max=256, mask_min=0;
		API.GetEditValue(m_hWnd, IDC_EDIT_CALC_MASK_MIN, &mask_min);
		API.GetEditValue(m_hWnd, IDC_EDIT_CALC_MASK_MAX, &mask_max);

		ret = API.CalcMaskValue(CURRENT_IMAGE, com_str, mask_max, mask_min, &err_str, &m_prog);
	}

	
	if(ret){//演算がうまくいったら結果画像を表示
		API.DrawImage();
	}else{//失敗したらエラーメッセージ表示
		AfxMessageBox(err_str);
		return false;
	}

	backup_all_com_str();//コマンド文字列のバックアップ	
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  メッセージ ハンドラ
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCalcDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CALC_EXEC, OnCalcExec)
	ON_BN_CLICKED(IDC_BUTTON_CALC_SHOW_MACRO_HELP, &CCalcDlg::OnBnClickedShowMacroHelp)
	ON_BN_CLICKED(IDC_RADIO_CALC_NEW_IMAGE, &CCalcDlg::OnBnClickedRadioNew)
	ON_BN_CLICKED(IDC_RADIO_CALC_OVERWRITE_IMAGE, &CCalcDlg::OnBnClickedRadioOverwrite)
	ON_BN_CLICKED(IDC_RADIO_CALC_OVERWRITE_IMAGE2, &CCalcDlg::OnBnClickedRadioCalcOverwriteImage2)
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
BOOL CCalcDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	

	//ダイアログアイテムの初期化
	API.SetEditValue(m_hWnd, IDC_EDIT_CALC_IMAGE_SIZE_W, API.main_image_size.cx);
	API.SetEditValue(m_hWnd, IDC_EDIT_CALC_IMAGE_SIZE_H, API.main_image_size.cy);
	API.SetCheck(m_hWnd, IDC_RADIO_CALC_NEW_IMAGE, true);
	API.SetCheck(m_hWnd, IDC_RADIO_CALC_OVERWRITE_IMAGE, false);
	API.SetCheck(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_BYTE, true);
	API.SetEditValue(m_hWnd, IDC_EDIT_CALC_MASK_MIN, 0);
	API.SetEditValue(m_hWnd, IDC_EDIT_CALC_MASK_MAX, 256);
	API.SetCheck(m_hWnd, IDC_CHECK_CALC_SATULATE,true);

	//ビットマップボタン
	execbmp.LoadBitmap(IDB_BUTTON_3D_0);
	((CButton*)GetDlgItem(IDC_BUTTON_CALC_EXEC))->SetBitmap((HBITMAP)execbmp);

	//演算式の履歴を読み込む
	load_com_str(m_combo_command, CString("value"));

	show_dlg_item();//入力可否状態の設定

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/********************************************************************
機  能  名  称 : ダイアログの破棄
関    数    名 : OnDestroy
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCalcDlg::OnDestroy() 
{
	save_com_str(m_combo_command, CString("value"));

	CDialog::OnDestroy();
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
void CCalcDlg::OnClose()
{
	CDialog::OnClose();

	API.SetCalcToolMode(false);
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
void CCalcDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
}

/********************************************************************
機  能  名  称 : 演算式の実行ボタン押下
関    数    名 : OnCalcExec
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCalcDlg::OnCalcExec() 
{
	execute();
}

/********************************************************************
機  能  名  称 : マクロ一覧表示ボタン押下
関    数    名 : OnBnClickedShowMacroHelp
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCalcDlg::OnBnClickedShowMacroHelp()
{
	ShellExecute(NULL, "open", API.GetDefaultPath() + "..\\..\\..\\Doc\\macro_help.txt", NULL, NULL, SW_SHOWNORMAL);
}

/********************************************************************
機  能  名  称 : 「新しい画像メモリを取得する」チェックボックスの値を変更
関    数    名 : OnBnClickedRadioNew
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCalcDlg::OnBnClickedRadioNew()
{
	show_dlg_item();
}

/********************************************************************
機  能  名  称 : 「現在の画像メモリに書き込む」チェックボックスの値を変更
関    数    名 : OnBnClickedRadioOverwrite
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCalcDlg::OnBnClickedRadioOverwrite()
{
	show_dlg_item();
}

/********************************************************************
機  能  名  称 : 「演算結果を元に現在選択中の画像のマスクを生成する」チェックボックスの値を変更
関    数    名 : OnBnClickedRadioCalcOverwriteImage2
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCalcDlg::OnBnClickedRadioCalcOverwriteImage2()
{
	show_dlg_item();
}
