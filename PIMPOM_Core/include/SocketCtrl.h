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
#pragma once
#include <afxsock.h>
#include "global_define.h"
#include "global_typedef.h"

///*************************************
//         クラス   定   義         
///*************************************
class PIMPOM_EXT CSocketCtrl : public CAsyncSocket
{


// アトリビュート
public:
	CSocketCtrl*	pSubSock;
	CString			HostName;//ホスト名
	UINT			PortNumber;//ポート番号
	char			RecvBuffer[RECIVE_BUFFER_SIZE];//受信バッファ
	int				RecvBufferCounter;//受信バッファの使用サイズ
	int				C_S_type;
	
	HWND			h_collback_wind[CALLBACK_FUNC_NUM];//コールバックウィンドウ
	void			(*recieve_callback_func[CALLBACK_FUNC_NUM])(HWND, BYTE*, int*);//受信時のコールバック関数
	void			(*accept_callback_func[CALLBACK_FUNC_NUM])(HWND, int*);//受信時のコールバック関数
	void			(*close_callback_func[CALLBACK_FUNC_NUM])(HWND);//切断時のコールバック関数

// オペレーション
public:
	CSocketCtrl();
	virtual ~CSocketCtrl();
	bool OpenSock(int type, UINT port, CString host, HWND wnd=NULL, void (*rcv_callback)(HWND, BYTE*, int*)=NULL, void (*acc_callback)(HWND, int*)=NULL, void (*cls_callback)(HWND)=NULL);
	void SetAdditionalCallBack(HWND wnd, void (*rcv_callback)(HWND, BYTE*, int*), void (*acc_callback)(HWND, int*), void (*cls_callback)(HWND));
	int SendString(CString message);


// オーバーライド
public:
	void OnClose(int nErrorCode);
	void OnReceive(int nErrorCode);
	void OnAccept(int nErrorCode);
	void OnConnect(int nErrorCode);
	// ClassWizard は仮想関数のオーバーライドを生成します
	//{{AFX_VIRTUAL(CSocketCtrl)
	//}}AFX_VIRTUAL

protected:
	void SetDelimiter(char delimiter[]);


	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSocketCtrl)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。
