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
#pragma once
#include "global_define.h"
#include "global_typedef.h"
#include "afxwin.h"

///*************************************
//         クラス   定   義         
///*************************************
class CSocketCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CSocketCtrlDlg)

	enum { IDD = IDD_SOCKET_CTRL_DLG };// ダイアログ データ

public:
	CSocketCtrlDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSocketCtrlDlg();
	void SetSendStringToDlg(CString message);
	void SetOpenInfoToDlg(CString host, int port);
	void SetSocketStatusToDlgItem();
	static void  ReceiveCallback(HWND wnd, BYTE *data, int *ndata);
	static void  AcceptCallback(HWND wnd, int *errCode);
	static void  CloseCallback(HWND wnd);


protected:
	CComboBox m_sendString;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	void backup_com_str(CComboBox &combo);


//メッセージハンドラ
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckSocketOpen();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnEnChangeEditSocketDelimiter();
	afx_msg void OnBnClickedButtonSocketResClear();
};
