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
//CSocketCtrl
//ソケット通信のラッパー
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "SocketCtrl.h"
#include "PIMPOM_API.h"
#include <stdio.h>

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



CSocketCtrl::CSocketCtrl()
{
	pSubSock = NULL;
	memset(RecvBuffer,0,RECIVE_BUFFER_SIZE);
	RecvBufferCounter=0;
	

	for(int i=0 ; i<CALLBACK_FUNC_NUM ; i++)
	{
		recieve_callback_func[i] = NULL;
		close_callback_func[i] = NULL;
		h_collback_wind[i] = NULL;
	}

}

CSocketCtrl::~CSocketCtrl()
{
	if(pSubSock){
		pSubSock->Close();
		delete	pSubSock;
		pSubSock = NULL;
	}
}


void CSocketCtrl::SetDelimiter(char delimiter[])
{
	if(API.socket_delimiter==1){
		sprintf_s(delimiter, 10, "%c",CR);
	}else if(API.socket_delimiter==2){
		sprintf_s(delimiter, 10, "%c",LF);
	}else{
		sprintf_s(delimiter, 10, "%c%c",CR,LF);
	}
}

/********************************************************************
機  能  名  称 : ソケットを開く
関    数    名 : CSocketCtrl
引          数 : int			type		(in)ソケットタイプ 0:サーバ 1:クライアント
				 UINT			port		(in)ポート番号
				 CString		host		(in)ホスト名
				 HWND			wnd			(in)コールバック関数に渡すウィンドウのハンドル
				 void (*recive_callback)(HWND, BYTE*, int*)		(in)データ受信時のコールバック関数（関数ポインタ）
				 void (*acc_callback)(HWND,int*)				(in)接続完了時のコールバック関数（関数ポインタ）
				 void (*close_callback)(HWND)					(in)通信遮断時のコールバック関数（関数ポインタ）
戻    り    値 : 成功すれば true
機          能 : ソケットを開くとともに、通信イベントに対するコールバック関数を登録する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CSocketCtrl::OpenSock(int type, UINT port, CString host, HWND wnd, void (*rcv_callback)(HWND, BYTE*, int*), void (*acc_callback)(HWND,int*), void (*cls_callback)(HWND))
{
	C_S_type = type;
	PortNumber = port;
	HostName = host;

		if(C_S_type == SOCKET_SERVER){//Server
			//コールバック設定
			recieve_callback_func[0] = rcv_callback;
			accept_callback_func[0] = acc_callback;
			close_callback_func[0] = cls_callback;
			h_collback_wind[0] = wnd;

			if( !CAsyncSocket::Create(PortNumber) ){//ソケット作成
				AfxMessageBox( "Create error" );
				return	false;
			}

			if( !CAsyncSocket::Listen() ){//接続待ち
				AfxMessageBox( "Listen error ");
				Close();
				return	false;
			}

		}else if(C_S_type == SOCKET_CLIENT){//Client

			//コールバック設定
			recieve_callback_func[0] = rcv_callback;
			accept_callback_func[0] = acc_callback;
			close_callback_func[0] = cls_callback;
			h_collback_wind[0] = wnd;

			if( !CAsyncSocket::Create() ){//ソケット作成
				AfxMessageBox( "Create error");
				return	false;
			}

			int err;
			if( !(err = CAsyncSocket::Connect(HostName, PortNumber) ) ){//サーバーに接続
				if(CAsyncSocket::GetLastError()!=WSAEWOULDBLOCK){
					AfxMessageBox( "Connect error");
					return	false;
				}
			}

		}else{
			//コールバック設定
			recieve_callback_func[0] = rcv_callback;
			accept_callback_func[0] = acc_callback;
			close_callback_func[0] = cls_callback;
			h_collback_wind[0] = wnd;
		}

	return	true;
}

void CSocketCtrl::SetAdditionalCallBack(HWND wnd, void (*rcv_callback)(HWND, BYTE*, int*), void (*acc_callback)(HWND, int*), void (*cls_callback)(HWND))
{
	recieve_callback_func[1] = rcv_callback;
	accept_callback_func[1] = acc_callback;
	close_callback_func[1] = cls_callback;
	h_collback_wind[1] = wnd;
}

/********************************************************************
機  能  名  称 : ソケットを閉じる
関    数    名 : CloseSocket
引          数 : 
戻    り    値 : 
機          能 : ソケットを閉じるコールバックを実施
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CSocketCtrl::OnClose(int nErrorCode)
{
	if(nErrorCode){
		Close();
	}

	for(int i=0 ; i<CALLBACK_FUNC_NUM ; i++)
	{
		if(close_callback_func[i]!=NULL){
			(close_callback_func[i])(h_collback_wind[i]);
			RecvBufferCounter=0;
		}
	}

	CAsyncSocket::OnClose(nErrorCode);
}

/********************************************************************
機  能  名  称 : 文字列を送信する
関    数    名 : SendString
引          数 : CString		message			(in)送信する文字列
戻    り    値 : 送信されたデータ数[byte]
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CSocketCtrl::SendString(CString message)
{
	if(C_S_type == SOCKET_SERVER){
		if(pSubSock){
			return  pSubSock->SendString( message );
		}
	}else{
		int len;
		char *buf;
		char Delimiter[10];//ﾃﾞﾘﾐﾀ

		SetDelimiter(Delimiter);
		
		message = message + CString(Delimiter);//メッセージの最後にデリミタを付け足す
		len = message.GetLength();
		buf = message.GetBuffer();

		//メッセージサイズ分送信を繰り返す
		int sendsum=0;
		do{
			sendsum += Send( &buf[sendsum] , len-sendsum);
		}while(sendsum<len);

		return sendsum;
	}
	return 0;
}


void CSocketCtrl::OnAccept(int nErrorCode)
{
	if(nErrorCode){
		Close();
	}

	if(C_S_type == SOCKET_SERVER){//Server

			if(pSubSock){
				pSubSock->Close();
				delete	pSubSock;
				pSubSock = NULL;
			}
				
			pSubSock = new CSocketCtrl();//サブソケット生成
			pSubSock->OpenSock(SOCKET_SERVER_SUB , PortNumber , HostName );	
			if(!pSubSock)	return ;

			//サブソケットにコールバックをコピー
			for(int i=0 ; i<CALLBACK_FUNC_NUM ; i++){
				pSubSock->recieve_callback_func[i] = recieve_callback_func[i];
				pSubSock->accept_callback_func[i] = accept_callback_func[i];
				pSubSock->close_callback_func[i] = close_callback_func[i];
				pSubSock->h_collback_wind[i] = h_collback_wind[i];
			}

			if ( !CAsyncSocket::Accept( *pSubSock )){//接続受け入れ
				AfxMessageBox( "Accept error");
				return ;
			}
	}


	//コールバック実行
	for(int i=0 ; i<CALLBACK_FUNC_NUM ; i++)
	{
		if(accept_callback_func[i]!=NULL){
			(accept_callback_func[i])(h_collback_wind[i], &nErrorCode);
			RecvBufferCounter=0;
		}
	}


	CAsyncSocket::OnAccept(nErrorCode);
}


void CSocketCtrl::OnConnect(int nErrorCode)
{
	if(nErrorCode){
		Close();
	}

	//コールバック実行
	for(int i=0 ; i<CALLBACK_FUNC_NUM ; i++)
	{
		if(accept_callback_func[i]!=NULL){
			(accept_callback_func[i])(h_collback_wind[i], &nErrorCode);
			RecvBufferCounter=0;
		}
	}

	CAsyncSocket::OnConnect(nErrorCode);
}

/********************************************************************
機  能  名  称 : 文字列を受け取る
関    数    名 : OnReceive
引          数 : int nErrorCode
戻    り    値 : 
機          能 : クライアントソケット専用のコールバック関数
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CSocketCtrl::OnReceive(int nErrorCode)
{
	char	data[1000];
	char Delimiter[10];//ﾃﾞﾘﾐﾀ

	SetDelimiter(Delimiter);


	if ( !nErrorCode){
		int rcvlen;
		if( (rcvlen = Receive( data , 1000)) > 0 )//メッセージ受信
		{
			for(int i=0 ; i<rcvlen ; i++)//一文字ずつチェック
			{
				//デリミタであるかどうか
				bool is_delimiter=true;
				for(int n=0 ; n<(int)strlen(Delimiter) && n+i<rcvlen  ; n++){
					if(data[i+n]!=Delimiter[n]){
						is_delimiter=false;
						break;
					}
				}

				if(!is_delimiter)//デリミタでなければバッファに入れる
				{
					RecvBuffer[RecvBufferCounter] = data[i];
					RecvBufferCounter++;
					if(RecvBufferCounter>=RECIVE_BUFFER_SIZE){//バッファサイズを越えた
						RecvBufferCounter=0;
						AfxMessageBox( "Buffer over run" );
						return;
					}
				}else{//デリミタが見つかった場合は、コールバック関数を呼び出す
					for(int i=0 ; i<CALLBACK_FUNC_NUM ; i++)
					{
						if(recieve_callback_func[i]!=NULL){
							(recieve_callback_func[i])(h_collback_wind[i], (BYTE*)RecvBuffer, &RecvBufferCounter);
							RecvBufferCounter=0;
						}
					}
				}
			}
		}
	}
	CAsyncSocket::OnReceive(nErrorCode);
}






