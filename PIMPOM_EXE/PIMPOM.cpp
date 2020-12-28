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

//PIMPOMのアプリ実行
/*******************************************************************/

#include "stdafx.h"
#include "PIMPOM.h"

#include "resource_core.h"
#include "PIMPOM_API.h"
#include "PIMPOMDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CPIMPOM_EXEApp

BEGIN_MESSAGE_MAP(CPIMPOM_EXEApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPIMPOM_EXEApp コンストラクション

CPIMPOM_EXEApp::CPIMPOM_EXEApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CPIMPOM_EXEApp オブジェクトです。

CPIMPOM_EXEApp theApp;


// CPIMPOM_EXEApp 初期化

BOOL CPIMPOM_EXEApp::InitInstance()
{
	CWinApp::InitInstance();

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	//SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));



	// INIファイルのパス名を変更する
	if( m_pszProfileName ) {
		delete ((void*)m_pszProfileName);
		m_pszProfileName = new char[MAX_PATH];
		if( !m_pszProfileName ) {
			AfxMessageBox("メモリ不足エラーです。");
			return FALSE;
		}

		CString comm = CString( theApp.m_lpCmdLine );
		if(!comm.IsEmpty()){
			strcpy((LPTSTR)m_pszProfileName, comm);//コマンドライン引数からINIファイルを指定する
		}else{
			strcpy((LPTSTR)m_pszProfileName, CPimpomAPI::GetDefaultPath());
			strcat((LPTSTR)m_pszProfileName, "\\pimpom.ini");
		}
	}


	//メインウィンドウの起動
	CPIMPOMDlg	dlg;
	m_pMainWnd = &dlg;
	dlg.cmdLine = m_lpCmdLine;
	dlg.DoModal();


	// ダイアログは閉じられました。アプリケーションのメッセージ ポンプを開始しないで
	//  アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}


int CPIMPOM_EXEApp::ExitInstance()
{
	if( m_pszProfileName ) {
		delete ((void*)m_pszProfileName);
		m_pszProfileName = NULL; 
	}

	return CWinApp::ExitInstance();
}

