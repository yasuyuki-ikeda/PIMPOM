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
//CPimpomAPI
//通信
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"

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

// *************************************
//         関    数    参    照         
// *************************************

/********************************************************************
機  能  名  称 : ソケットを開く
関    数    名 : OpenSocket
引          数 : int			type		(in)ソケットタイプ 0:サーバ 1:クライアント
				 UINT			port		(in)ポート番号
				 CString		host		(in)ホスト名
				 HWND			wnd			(in)コールバック関数に渡すウィンドウのハンドル
				 void (*recive_callback)(HWND, BYTE*, int*)		(in)データ受信時のコールバック関数（関数ポインタ）
				 void (*acc_callback)(HWND,int*)				(in)接続完了時のコールバック関数（関数ポインタ）
				 void (*close_callback)(HWND)					(in)通信遮断時のコールバック関数（関数ポインタ）
戻    り    値 : 成功すれば true
機          能 : ソケットを開くとともに、通信イベントに対するコールバック関数を登録する
                 この関数が呼ばれると、ソケット通信設定ウィンドウが開かれる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::OpenSocket(int type, UINT port, CString host, HWND wnd, void (*recive_callback)(HWND, BYTE*, int*), void (*acc_callback)(HWND,int*), void (*close_callback)(HWND))
{
	if(!pSockCtrl){
		pSockCtrl = new CSocketCtrl();
		if(!pSockCtrl)	return false;
	}

	if(!pSockCtrl->OpenSock(type, port, host, wnd, recive_callback, acc_callback, close_callback)){
		delete	pSockCtrl;
		pSockCtrl = NULL;
		return false;
	}


	//ソケット通信設定ウィンドウを開く
	if(!pSocketCtrlDlg)
	{
		if(!DialogOpen(IDD_SOCKET_CTRL_DLG,true)){
			delete	pSockCtrl;
			pSockCtrl = NULL;
			return false;
		}

		pSocketCtrlDlg->SetOpenInfoToDlg(host, port);
	}

	//ソケット通信設定ウィンドウのコールバックを設定
	//pSockCtrl->SetAdditionalCallBack(pSocketCtrlDlg->m_hWnd, recive_callback, acc_callback, close_callback);
	pSockCtrl->SetAdditionalCallBack(pSocketCtrlDlg->m_hWnd, CSocketCtrlDlg::ReceiveCallback, CSocketCtrlDlg::AcceptCallback, CSocketCtrlDlg::CloseCallback);


	return true; 
}

/********************************************************************
機  能  名  称 : 文字列を送信する
関    数    名 : SendString
引          数 : CString		str			(in)送信する文字列
戻    り    値 : 送信されたデータ数[byte]
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int	CPimpomAPI::SendString(CString str)
{
	if(!pSockCtrl)	return -1;

	int ret = pSockCtrl->SendString(str);
	if(ret){
		if(pSocketCtrlDlg )
		{
			pSocketCtrlDlg->SetSendStringToDlg(str);
		}
	}

	return ret;
}

/********************************************************************
機  能  名  称 : ソケットを閉じる
関    数    名 : CloseSocket
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::CloseSocket()
{
	if(pSockCtrl){
		pSockCtrl->Close();
		delete	pSockCtrl;
		pSockCtrl = NULL;
	}

	if(pSocketCtrlDlg){
		//ソケット通信設定ウィンドウにステータス表示
		pSocketCtrlDlg->SetSocketStatusToDlgItem();
	}
}

/********************************************************************
機  能  名  称 : ソケットの状態取得
関    数    名 : GetSocketStatus
引          数 : CString* pHostName (out)ホスト名
				 UINT* pPortNo		(out)ポート番号
戻    り    値 : 0:サーバ 1:クライアント -1:開かれず
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int	CPimpomAPI::GetSocketStatus(CString* pHostName, UINT* pPortNo)
{
	if(pSockCtrl){
		*pHostName = pSockCtrl->HostName;
		*pPortNo = pSockCtrl->PortNumber;
		return pSockCtrl->C_S_type;
	}else{
		return -1;
	}
}