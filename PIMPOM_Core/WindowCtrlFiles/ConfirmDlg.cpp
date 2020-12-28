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
//CConfirmDlg
//汎用の値設定ダイアログ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "ConfirmDlg.h"

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

CConfirmDlg::CConfirmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfirmDlg::IDD, pParent)
{
	init_setting();

	//{{AFX_DATA_INIT(CConfirmDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。

	//}}AFX_DATA_INIT
}


void CConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfirmDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}

/********************************************************************
機  能  名  称 :初期化
関    数    名 : init_setting
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CConfirmDlg::init_setting()
{
	int i;

	window_title = "";
	title_width = 150;
	edit_width = 100;
	item_count =0 ;
	for(i=0 ; i<CONFIRM_NUM_MAX ; i++){
		value[i].type = CONFIRM_NULL;
		value[i].title = "";
		value[i].p_var_int = NULL;
		value[i].p_var_short = NULL;
		value[i].p_var_float = NULL;
		value[i].p_var_double = NULL;
		value[i].p_var_string = NULL;
	}

	edit_id[0] = IDC_EDIT_CONFIRM0;
	edit_id[1] = IDC_EDIT_CONFIRM1;
	edit_id[2] = IDC_EDIT_CONFIRM2;
	edit_id[3] = IDC_EDIT_CONFIRM3;
	edit_id[4] = IDC_EDIT_CONFIRM4;
	edit_id[5] = IDC_EDIT_CONFIRM5;
	edit_id[6] = IDC_EDIT_CONFIRM6;
	edit_id[7] = IDC_EDIT_CONFIRM7;
	edit_id[8] = IDC_EDIT_CONFIRM8;
	edit_id[9] = IDC_EDIT_CONFIRM9;
	edit_id[10] = IDC_EDIT_CONFIRM10;
	edit_id[11] = IDC_EDIT_CONFIRM11;
	edit_id[12] = IDC_EDIT_CONFIRM12;
	edit_id[13] = IDC_EDIT_CONFIRM13;
	edit_id[14] = IDC_EDIT_CONFIRM14;
	edit_id[15] = IDC_EDIT_CONFIRM15;
	edit_id[16] = IDC_EDIT_CONFIRM16;
	edit_id[17] = IDC_EDIT_CONFIRM17;
	edit_id[18] = IDC_EDIT_CONFIRM18;
	edit_id[19] = IDC_EDIT_CONFIRM19;
	title_id[0] = IDC_STATIC_CONFIRM0;
	title_id[1] = IDC_STATIC_CONFIRM1;
	title_id[2] = IDC_STATIC_CONFIRM2;
	title_id[3] = IDC_STATIC_CONFIRM3;
	title_id[4] = IDC_STATIC_CONFIRM4;
	title_id[5] = IDC_STATIC_CONFIRM5;
	title_id[6] = IDC_STATIC_CONFIRM6;
	title_id[7] = IDC_STATIC_CONFIRM7;
	title_id[8] = IDC_STATIC_CONFIRM8;
	title_id[9] = IDC_STATIC_CONFIRM9;
	title_id[10] = IDC_STATIC_CONFIRM10;
	title_id[11] = IDC_STATIC_CONFIRM11;
	title_id[12] = IDC_STATIC_CONFIRM12;
	title_id[13] = IDC_STATIC_CONFIRM13;
	title_id[14] = IDC_STATIC_CONFIRM14;
	title_id[15] = IDC_STATIC_CONFIRM15;
	title_id[16] = IDC_STATIC_CONFIRM16;
	title_id[17] = IDC_STATIC_CONFIRM17;
	title_id[18] = IDC_STATIC_CONFIRM18;
	title_id[19] = IDC_STATIC_CONFIRM19;
	check_id[0] = IDC_CHECK_CONFIRM0;
	check_id[1] = IDC_CHECK_CONFIRM1;
	check_id[2] = IDC_CHECK_CONFIRM2;
	check_id[3] = IDC_CHECK_CONFIRM3;
	check_id[4] = IDC_CHECK_CONFIRM4;
	check_id[5] = IDC_CHECK_CONFIRM5;
	check_id[6] = IDC_CHECK_CONFIRM6;
	check_id[7] = IDC_CHECK_CONFIRM7;
	check_id[8] = IDC_CHECK_CONFIRM8;
	check_id[9] = IDC_CHECK_CONFIRM9;
	check_id[10] = IDC_CHECK_CONFIRM10;
	check_id[11] = IDC_CHECK_CONFIRM11;
	check_id[12] = IDC_CHECK_CONFIRM12;
	check_id[13] = IDC_CHECK_CONFIRM13;
	check_id[14] = IDC_CHECK_CONFIRM14;
	check_id[15] = IDC_CHECK_CONFIRM15;
	check_id[16] = IDC_CHECK_CONFIRM16;
	check_id[17] = IDC_CHECK_CONFIRM17;
	check_id[18] = IDC_CHECK_CONFIRM18;
	check_id[19] = IDC_CHECK_CONFIRM19;
	radio_id[0] = IDC_RADIO_CONFIRM0;
	radio_id[1] = IDC_RADIO_CONFIRM1;
	radio_id[2] = IDC_RADIO_CONFIRM2;
	radio_id[3] = IDC_RADIO_CONFIRM3;
	radio_id[4] = IDC_RADIO_CONFIRM4;
	radio_id[5] = IDC_RADIO_CONFIRM5;
	radio_id[6] = IDC_RADIO_CONFIRM6;
	radio_id[7] = IDC_RADIO_CONFIRM7;
	radio_id[8] = IDC_RADIO_CONFIRM8;
	radio_id[9] = IDC_RADIO_CONFIRM9;
	radio_id[10] = IDC_RADIO_CONFIRM10;
	radio_id[11] = IDC_RADIO_CONFIRM11;
	radio_id[12] = IDC_RADIO_CONFIRM12;
	radio_id[13] = IDC_RADIO_CONFIRM13;
	radio_id[14] = IDC_RADIO_CONFIRM14;
	radio_id[15] = IDC_RADIO_CONFIRM15;
	radio_id[16] = IDC_RADIO_CONFIRM16;
	radio_id[17] = IDC_RADIO_CONFIRM17;
	radio_id[18] = IDC_RADIO_CONFIRM18;
	radio_id[19] = IDC_RADIO_CONFIRM19;
}

/********************************************************************
機  能  名  称 : このダイアログで設定できるようにする変数を登録する
関    数    名 : RegistVar
引          数 : CString	title	(in)変数名
							*pval	(in)変数へのポインタ
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CConfirmDlg::RegistVar(CString title, int *pval)
{
		if(pval == NULL)					return false;
		if(item_count >= CONFIRM_NUM_MAX)	return false;
	value[item_count].type = CONFIRM_INT;
	value[item_count].title = title;
	value[item_count].p_var_int = pval;
	item_count++;

	return true;
}

bool CConfirmDlg::RegistVar(CString title, short *pval)
{
		if(pval == NULL)					return false;
		if(item_count >= CONFIRM_NUM_MAX)	return false;
	value[item_count].type = CONFIRM_SHORT;
	value[item_count].title = title;
	value[item_count].p_var_short = pval;
	item_count++;

	return true;
}

bool CConfirmDlg::RegistVar(CString title, float *pval)
{
		if(pval == NULL)					return false;
		if(item_count >= CONFIRM_NUM_MAX)	return false;
	value[item_count].type = CONFIRM_FLOAT;
	value[item_count].title = title;
	value[item_count].p_var_float = pval;
	item_count++;

	return true;
}

bool CConfirmDlg::RegistVar(CString title, double *pval)
{
		if(pval == NULL)					return false;
		if(item_count >= CONFIRM_NUM_MAX)	return false;
	value[item_count].type = CONFIRM_DOUBLE;
	value[item_count].title = title;
	value[item_count].p_var_double = pval;
	item_count++;

	return true;
}

bool CConfirmDlg::RegistVar(CString title, CString *pval)
{
		if(pval == NULL)					return false;
		if(item_count >= CONFIRM_NUM_MAX)	return false;
	value[item_count].type = CONFIRM_STRING;
	value[item_count].title = title;
	value[item_count].p_var_string = pval;
	item_count++;

	return true;
}

bool CConfirmDlg::RegistVar(CString title, bool *pval)
{
		if(pval == NULL)					return false;
		if(item_count >= CONFIRM_NUM_MAX)	return false;
	value[item_count].type = CONFIRM_BOOL;
	value[item_count].title = title;
	value[item_count].p_var_bool = pval;
	item_count++;

	return true;
}

bool CConfirmDlg::RegistRadioVar(CString title, bool *pval)
{
		if(pval == NULL)					return false;
		if(item_count >= CONFIRM_NUM_MAX)	return false;
	value[item_count].type = CONFIRM_RADIO_BOOL;
	value[item_count].title = title;
	value[item_count].p_var_bool = pval;
	item_count++;

	return true;
}

/********************************************************************
機  能  名  称 : このダイアログのタイトルを設定する
関    数    名 : SetTitle
引          数 : CString	title	(in)ダイアログのタイトル
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CConfirmDlg::SetTitle(CString title)
{
	window_title = title;
}

/********************************************************************
機  能  名  称 : このダイアログのサイズを設定する
関    数    名 : SetWidth
引          数 : int	titlesize	(in)変数名を表示する領域の幅
				 int	editsize	(in)変数の値を入力するエディットボックスの幅
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CConfirmDlg::SetWidth(int titlesize, int editsize)
{
	title_width = titlesize;
	edit_width = editsize;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  メッセージ ハンドラ
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CConfirmDlg, CDialog)
	//{{AFX_MSG_MAP(CConfirmDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CConfirmDlg::OnBnClickedOk)
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
BOOL CConfirmDlg::OnInitDialog() 
{
	int		i;
	HWND	edit_wnd, title_wnd, check_wnd, radio_wnd;
	CSize	edit_size, title_size;
	int		button_width, button_left;
	int		margin = 5;//ダイアログアイテムの間隔
	CRect	winRct, clientRct;

	CDialog::OnInitDialog();

	//ウインドウの配置
	title_size = CSize(title_width, 20);
	edit_size = CSize(edit_width, 20);

	//エディット・タイトル・チェックの配置
	for(i=0 ; i<CONFIRM_NUM_MAX ; i++){
		edit_wnd = ::GetDlgItem(m_hWnd, edit_id[i]);
		title_wnd = ::GetDlgItem(m_hWnd, title_id[i]);
		check_wnd = ::GetDlgItem(m_hWnd, check_id[i]);
		radio_wnd = ::GetDlgItem(m_hWnd, radio_id[i]);



		if(item_count > i)
		{
			if(value[i].type == CONFIRM_BOOL)
			{//チェックボックス
				::ShowWindow(title_wnd, SW_HIDE);
				::ShowWindow(edit_wnd, SW_HIDE);
				::ShowWindow(check_wnd, SW_SHOW);
				::ShowWindow(radio_wnd, SW_HIDE);
				
				::MoveWindow( check_wnd,
								0,				i*(title_size.cy + margin), 
								title_size.cx,	title_size.cy, true);
				::SetWindowText( check_wnd, value[i].title);

			}
			else if(value[i].type == CONFIRM_RADIO_BOOL)
			{//ラジオボタン
				::ShowWindow(title_wnd, SW_HIDE);
				::ShowWindow(edit_wnd, SW_HIDE);
				::ShowWindow(check_wnd, SW_HIDE);
				::ShowWindow(radio_wnd, SW_SHOW);
				
				::MoveWindow( radio_wnd,
								0,				i*(title_size.cy + margin), 
								title_size.cx,	title_size.cy, true);
				::SetWindowText( radio_wnd, value[i].title);

			}
			else if(value[i].type != CONFIRM_NULL)
			{//数値・文字
				::ShowWindow(title_wnd, SW_SHOW);
				::ShowWindow(edit_wnd, SW_SHOW);
				::ShowWindow(check_wnd, SW_HIDE);
				::ShowWindow(radio_wnd, SW_HIDE);

				::MoveWindow( title_wnd,
								0,				i*(title_size.cy + margin), 
								title_size.cx,	title_size.cy, true);
				::MoveWindow( edit_wnd,
								title_size.cx + margin,		i*(edit_size.cy + margin), 
								edit_size.cx,				edit_size.cy, true);

				::SetWindowText( title_wnd, value[i].title);
			}

			//初期値設定
			switch(value[i].type){
			case CONFIRM_INT:
				API.SetEditValue(m_hWnd, edit_id[i], *(value[i].p_var_int));
				break;
			case CONFIRM_SHORT:
				API.SetEditValue(m_hWnd, edit_id[i], *(value[i].p_var_short));
				break;
			case CONFIRM_FLOAT:
				API.SetEditValue(m_hWnd, edit_id[i], *(value[i].p_var_float));
				break;
			case CONFIRM_DOUBLE:
				API.SetEditValue(m_hWnd, edit_id[i], *(value[i].p_var_double));
				break;
			case CONFIRM_STRING:
				API.SetEditValue(m_hWnd, edit_id[i], *(value[i].p_var_string));
				break;
			case CONFIRM_BOOL:
				API.SetCheck(m_hWnd, check_id[i], *(value[i].p_var_bool));
				break;
			case CONFIRM_RADIO_BOOL:
				API.SetCheck(m_hWnd, radio_id[i], *(value[i].p_var_bool));
				break;
			default:
				break;
			}
		}else{
			::ShowWindow(title_wnd, SW_HIDE);
			::ShowWindow(edit_wnd, SW_HIDE);
			::ShowWindow(check_wnd, SW_HIDE);
		}
	}

	//ボタンがウィンドウの右端に来るように調整
	button_width = 100;
		if( title_width + edit_width < button_width*2 ){
			button_width = (title_width + edit_width)/2;
		}
	button_left = title_width + edit_width - button_width*2;

	//OKボタン
	::MoveWindow( ::GetDlgItem( m_hWnd, IDOK), 
				button_left,	item_count*(edit_size.cy + margin), 
				button_width,	23, true);
	//キャンセルボタン
	::MoveWindow( ::GetDlgItem( m_hWnd, IDCANCEL), 
				button_left + button_width + margin,			item_count*(edit_size.cy + margin), 
				button_width,	23, true);


	//ウインドウ自体のサイズ調整
	::GetWindowRect(m_hWnd, winRct);
	::GetClientRect(m_hWnd, clientRct);
	int non_client_w = (winRct.right - winRct.left) - (clientRct.right - clientRct.left);
	int non_client_h = (winRct.bottom - winRct.top) - (clientRct.bottom - clientRct.top);

	::MoveWindow(m_hWnd, 
				winRct.left,
				winRct.top,
				title_size.cx + edit_size.cx + margin + non_client_w,
				item_count*(edit_size.cy + margin) + 23 + non_client_h, true);
	::SetWindowText(m_hWnd, window_title);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/********************************************************************
機  能  名  称 : OKボタン押下
関    数    名 : OnOK
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CConfirmDlg::OnOK() 
{
	int i;

	for(i=0 ; i<item_count ; i++){
		switch(value[i].type){
		case CONFIRM_INT:
			API.GetEditValue(m_hWnd, edit_id[i], value[i].p_var_int);
			break;
		case CONFIRM_SHORT:
			API.GetEditValue(m_hWnd, edit_id[i], value[i].p_var_short);
			break;
		case CONFIRM_FLOAT:
			API.GetEditValue(m_hWnd, edit_id[i], value[i].p_var_float);
			break;
		case CONFIRM_DOUBLE:
			API.GetEditValue(m_hWnd, edit_id[i], value[i].p_var_double);
			break;
		case CONFIRM_STRING:
			API.GetEditValue(m_hWnd, edit_id[i], value[i].p_var_string);
			break;
		case CONFIRM_BOOL:
			*(value[i].p_var_bool) = API.GetCheck(m_hWnd, check_id[i]);
			break;
		case CONFIRM_RADIO_BOOL:
			*(value[i].p_var_bool) = API.GetCheck(m_hWnd, radio_id[i]);
			break;
		default:
			break;
		}
	}


	CDialog::OnOK();
}


void CConfirmDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OnOK();
}
