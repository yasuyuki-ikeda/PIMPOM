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
//CSocketCtrlDlg
//ソケット通信設定ウィンドウ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "SocketCtrlDlg.h"

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
IMPLEMENT_DYNAMIC(CSocketCtrlDlg, CDialog)

CSocketCtrlDlg::CSocketCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketCtrlDlg::IDD, pParent)
{

}

CSocketCtrlDlg::~CSocketCtrlDlg()
{
}

void CSocketCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SOCKET_SENDSTRING, m_sendString);
}

/********************************************************************
機  能  名  称 : 送信文字列をバックアップ
関    数    名 : backup_com_str
引          数 : CComboBox &combo　(in)コマンドの入ったコンボボックスを参照
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CSocketCtrlDlg::backup_com_str(CComboBox &combo)
{
	CString str,b_str;

	combo.GetWindowText(str);

	for(int i=0 ; i<combo.GetCount() ; i++){//バックアップされた履歴がないか調べる
		combo.GetLBText(i,b_str);

		if(b_str == str){
			return;//すでにバックアップされていれば処理終了
		}
	}

	combo.AddString(str);//バックアップされてなければリストに追加
}

/********************************************************************
機  能  名  称 : 通信設定情報をダイアログに反映する
関    数    名 : SetOpenInfoToDlg
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CSocketCtrlDlg::SetOpenInfoToDlg(CString host, int port)
{
	API.SetEditValue(m_hWnd, IDC_EDIT_SOKET_HOSTNAME, host);
	API.SetEditValue(m_hWnd, IDC_EDIT_SOCKET_PORTNUM, port);
}


/********************************************************************
機  能  名  称 : 送信した文字列をダイアログに反映する
関    数    名 : SetSendStringToDlg
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CSocketCtrlDlg::SetSendStringToDlg(CString message)
{
	m_sendString.SetWindowText(message);

	backup_com_str(m_sendString);

	CString	ret; ret.Format("%c%c", (char)CR , (char)LF);//改行コード
	CString log_str;
	API.GetEditValue(m_hWnd, IDC_EDIT_SOCKET_RECVSTRING, &log_str);
	API.SetEditValue(m_hWnd, IDC_EDIT_SOCKET_RECVSTRING, "<-" + message + ret + log_str);
}

//////////////////////////////////////////////////////////////////////////////////////////
//  メッセージ ハンドラ
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSocketCtrlDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_SOCKET_OPEN, &CSocketCtrlDlg::OnBnClickedCheckSocketOpen)
	ON_BN_CLICKED(IDC_BUTTON_SOCKET_SEND, &CSocketCtrlDlg::OnBnClickedButtonSend)
	ON_EN_CHANGE(IDC_EDIT_SOCKET_DELIMITER, &CSocketCtrlDlg::OnEnChangeEditSocketDelimiter)
	ON_BN_CLICKED(IDC_BUTTON_SOCKET_RES_CLEAR, &CSocketCtrlDlg::OnBnClickedButtonSocketResClear)
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
BOOL CSocketCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	API.SetCheck(m_hWnd, IDC_RADIO_SOCKET_CLIENT, true);
	API.SetEditValue(m_hWnd,IDC_EDIT_SOKET_HOSTNAME,"localhost");
	API.SetEditValue(m_hWnd,IDC_EDIT_SOCKET_PORTNUM, 9876);

	API.SetEditValue(m_hWnd,IDC_EDIT_SOCKET_DELIMITER, API.socket_delimiter);

	return true;
}

/********************************************************************
機  能  名  称 : 受信時のコールバック関数
関    数    名 : ReceiveCallback
引          数 : HWND	wnd		(in)
				 BYTE	*data	(out)
				 int	*ndata	(out)
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CSocketCtrlDlg::ReceiveCallback(HWND wnd, BYTE *data, int *ndata)
{
	char rcvdata[RECIVE_BUFFER_SIZE];
	memcpy(rcvdata, data, *ndata);
	rcvdata[*ndata]='\0';

	//受信結果の出力
	CString	ret; ret.Format("%c%c", (char)CR , (char)LF);//改行コード

	CString log_str;
	API.GetEditValue(wnd, IDC_EDIT_SOCKET_RECVSTRING, &log_str);
	API.SetEditValue(wnd, IDC_EDIT_SOCKET_RECVSTRING, "->" + CString(rcvdata) + ret + log_str);

	for(int i=0 ; i<CUSTOM_FUNC_MAX ;i++)
	{
		API.UserDialogReceive(i, data, ndata);
	}
}


/********************************************************************
機  能  名  称 : 接続完了のコールバック関数
関    数    名 : AcceptCallback
引          数 : HWND	wnd		(in)
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CSocketCtrlDlg::AcceptCallback(HWND wnd, int *errCode)
{
	if(*errCode){
		API.SetCheck(wnd, IDC_CHECK_SOCKET_OPEN, false);
		::EnableWindow(::GetDlgItem(wnd, IDC_EDIT_SOKET_HOSTNAME), true);
		::EnableWindow(::GetDlgItem(wnd, IDC_EDIT_SOCKET_PORTNUM), true);
		::EnableWindow(::GetDlgItem(wnd, IDC_RADIO_SOCKET_CLIENT), true);
		::EnableWindow(::GetDlgItem(wnd, IDC_RADIO_SOCKET_SERVER), true);
		API.ShowMessage(false, 0,true, "接続失敗");
	}else{
		//ダイアログの有効・無効
		API.SetCheck(wnd, IDC_CHECK_SOCKET_OPEN, true);
		::EnableWindow(::GetDlgItem(wnd, IDC_EDIT_SOKET_HOSTNAME), false);
		::EnableWindow(::GetDlgItem(wnd, IDC_EDIT_SOCKET_PORTNUM), false);
		::EnableWindow(::GetDlgItem(wnd, IDC_RADIO_SOCKET_CLIENT), false);
		::EnableWindow(::GetDlgItem(wnd, IDC_RADIO_SOCKET_SERVER), false);
		API.ShowMessage(false, 0,true, "接続完了");
	}
}

/********************************************************************
機  能  名  称 : 通信遮断時のコールバック関数
関    数    名 : CloseCallback
引          数 : HWND	wnd		(in)
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CSocketCtrlDlg::CloseCallback(HWND wnd)
{
	API.CloseSocket();
	API.SetCheck(wnd, IDC_CHECK_SOCKET_OPEN, false);
	::EnableWindow(::GetDlgItem(wnd, IDC_EDIT_SOKET_HOSTNAME), true);
	::EnableWindow(::GetDlgItem(wnd, IDC_EDIT_SOCKET_PORTNUM), true);
	::EnableWindow(::GetDlgItem(wnd, IDC_RADIO_SOCKET_CLIENT), true);
	::EnableWindow(::GetDlgItem(wnd, IDC_RADIO_SOCKET_SERVER), true);
	API.ShowMessage(false, 0,true, "切断");
}

void CSocketCtrlDlg::SetSocketStatusToDlgItem()
{
	CString hostname;
	UINT portno;
	int cs;

	cs = API.GetSocketStatus(&hostname,&portno);
	if(cs!=-1)
	{
		API.SetCheck(m_hWnd, IDC_CHECK_SOCKET_OPEN, true);
		API.SetEditValue(m_hWnd,IDC_EDIT_SOKET_HOSTNAME,hostname);//ホスト名
		API.SetEditValue(m_hWnd,IDC_EDIT_SOCKET_PORTNUM,(int)portno);//ポート番号

		if(cs==1){
			API.SetCheck(m_hWnd, IDC_RADIO_SOCKET_CLIENT, true);
		}else{
			API.SetCheck(m_hWnd, IDC_RADIO_SOCKET_SERVER, true);
		}

		//ダイアログの有効・無効
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_SOKET_HOSTNAME), false);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_SOCKET_PORTNUM), false);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_SOCKET_CLIENT), false);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_SOCKET_SERVER), false);

		API.ShowMessage(false, 0,true, "接続完了");
	}
	else
	{
		API.SetCheck(m_hWnd, IDC_CHECK_SOCKET_OPEN, false);

		//ダイアログの有効・無効
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_SOKET_HOSTNAME), true);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_SOCKET_PORTNUM), true);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_SOCKET_CLIENT), true);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_SOCKET_SERVER), true);

		API.ShowMessage(false, 0,true, "切断");
	}
}


/********************************************************************
機  能  名  称 : 「ポートを開く」ボタン押下
関    数    名 : OnBnClickedCheckSocketOpen
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CSocketCtrlDlg::OnBnClickedCheckSocketOpen()
{
	CString hostname;
	int portno;
	

	API.GetEditValue(m_hWnd,IDC_EDIT_SOKET_HOSTNAME,&hostname);//ホスト名取得
	API.GetEditValue(m_hWnd,IDC_EDIT_SOCKET_PORTNUM,&portno);//ポート番号取得

	if(API.GetCheck(m_hWnd,IDC_CHECK_SOCKET_OPEN))//ポートを開く
	{
		if(API.GetCheck(m_hWnd, IDC_RADIO_SOCKET_SERVER))//サーバ
		{
			API.ShowMessage(true, 0,true, "接続待機中...");
			API.OpenSocket(SOCKET_SERVER, portno, hostname);
		}
		else//クライアント
		{
			API.ShowMessage(true, 0,true, "接続待機中...");
			API.OpenSocket(SOCKET_CLIENT, portno, hostname );
		}
	}else{//ポートを閉じる
		API.CloseSocket();
	}

}

/********************************************************************
機  能  名  称 : 「文字列を送る」ボタン押下
関    数    名 : OnBnClickedButtonSend
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CSocketCtrlDlg::OnBnClickedButtonSend()
{
	CString  str;

	m_sendString.GetWindowText(str);
	API.SendString(str);
}

/********************************************************************
機  能  名  称 : デリミタ変更
関    数    名 : OnEnChangeEditSocketDelimiter
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CSocketCtrlDlg::OnEnChangeEditSocketDelimiter()
{
	// TODO:  これが RICHEDIT コントロールの場合、
	// まず、CDialog::OnInitDialog() 関数をオーバーライドして、OR 状態の ENM_CHANGE
	// フラグをマスクに入れて、CRichEditCtrl().SetEventMask() を呼び出さない限り、
	// コントロールは、この通知を送信しません。

	API.GetEditValue(m_hWnd, IDC_EDIT_SOCKET_DELIMITER, &API.socket_delimiter);
}


/********************************************************************
機  能  名  称 : 受信履歴クリア
関    数    名 : OnBnClickedButtonSocketResClear
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CSocketCtrlDlg::OnBnClickedButtonSocketResClear()
{
	API.SetEditValue(m_hWnd, IDC_EDIT_SOCKET_RECVSTRING, "");
}
