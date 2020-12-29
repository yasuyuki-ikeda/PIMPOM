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
//CPythonBatchDlg
//pythonのバッチ実行
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "PythonBatchDlg.h"


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

unsigned __stdcall PythonBatchThread(void* pArg)
{
	CPythonBatchDlg* pDlg = (CPythonBatchDlg*)pArg;

	pDlg->OnBatchExecute();

	return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////
//  メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : コマンド実行
関    数    名 : execCmd
引          数 :
戻    り    値 :
機          能 : コマンドを子プロセスで実行し、結果を表示領域に表示する
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool CPythonBatchDlg::execCmd(CString cmd, CString currentdir)
{
	// パイプの作成
	HANDLE readPipe;
	HANDLE writePipe;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	if (CreatePipe(&readPipe, &writePipe, &sa, 0) == 0) 
	{
		return false;
	}


	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.hStdOutput = writePipe;
	si.hStdError = writePipe;
	si.wShowWindow = SW_HIDE;

	//コマンドの実行
	if (CreateProcess(NULL, cmd.GetBuffer(), NULL, NULL, TRUE, 0, NULL, currentdir.GetBuffer(), &si, &pi) == 0)
	{
		cmd.ReleaseBuffer();
		currentdir.ReleaseBuffer();
		CloseHandle(readPipe);
		return false;
	}
	cmd.ReleaseBuffer();
	currentdir.ReleaseBuffer();

	HANDLE childProcess = pi.hProcess;
	CloseHandle(pi.hThread);
	

	//標準出力結果を読み取って表示
	bool continue_flg = true;
	while (continue_flg)
	{
		DWORD totalLen, len;
		if (WaitForSingleObject(childProcess, 30) == WAIT_OBJECT_0)
		{
			continue_flg = false;
		}

		if (PeekNamedPipe(readPipe, NULL, 0, NULL, &totalLen, NULL) == 0)
		{
			break;
		}


		if (0 < totalLen) 
		{
			char readBuf[5000];
			if (ReadFile(readPipe, readBuf, sizeof(readBuf) - 1, &len, NULL) == 0)
			{
				CloseHandle(writePipe);
				CloseHandle(readPipe);
				CloseHandle(pi.hProcess);
				return false;
			}
			readBuf[len] = 0;

			message += CString(readBuf);

			API.SetEditValue(m_hWnd, IDC_EDIT_YTHON_BATCH_DLG_OUTPUT, message);

			//最後の文字選択(スクロールさせるため)
			CEdit	*p_wnd_edit = (CEdit*)GetDlgItem(IDC_EDIT_YTHON_BATCH_DLG_OUTPUT);
			int lastidx = message.GetLength() - 1;
			p_wnd_edit->SetSel(lastidx, lastidx);
		}
	} 



	if (CloseHandle(writePipe) == 0) {
		CloseHandle(readPipe);
		CloseHandle(pi.hProcess);
		return false;
	}

	if (CloseHandle(readPipe) == 0) {
		CloseHandle(pi.hProcess);
		return false;
	}
	CloseHandle(pi.hProcess);

	return true;
}

/********************************************************************
機  能  名  称 : pythonのバッチ実行
関    数    名 : PythonBatchExecute
引          数 :
戻    り    値 : 成功したらtrue
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool CPythonBatchDlg::OnBatchExecute()
{
	bool ret = true;

	if (::IsWindowVisible(m_hWnd) == false)	return false;

	//この関数の多重起動を防ぐフラグ
	static bool flg = false;

	if (!flg)
	{
		flg = true;

		if (verCheck)
		{//python環境チェック
			ret = execCmd(_T("python vercheck.py"),API.GetDefaultPath());

			verCheck = false;
		}
		else 
		{//pythonスクリプト実行
			CString filepath, currentdir;
			API.GetEditValue(m_hWnd, IDC_EDIT_PYTHON_BATCH_DLG_FILEPATH, &filepath);
			currentdir = filepath.Left(filepath.ReverseFind('\\'));

			ret = execCmd(_T("python ") + filepath, currentdir);
		}


		flg = false;
	}

	return ret;
}


void CPythonBatchDlg::arrange_dlg_item()
{
	RECT clientRct = { 0 };

	::GetClientRect(m_hWnd, &clientRct);//クライアント領域を表す矩形を取得

	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_PYTHON_BATCH_DLG_FILEPATH), 120, 0, clientRct.right - clientRct.left - 170, 20, true);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_PYTHON_BATCH_DLG_BROWSE_FILE), clientRct.right - clientRct.left - 60, 0, 50, 20, true);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_IDC_PYTHON_BATCH_DLG_EDIT), 0, 22, 100, 30, true);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_IDC_PYTHON_BATCH_DLG_TRIAL), 102, 22, 100, 30, true); 
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_IDC_PYTHON_BATCH_DLG_VERCHK), 202, 22, 150, 30, true);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_YTHON_BATCH_DLG_OUTPUT), 0, 54, clientRct.right - clientRct.left - 4, clientRct.bottom - clientRct.top - 60, true);
	
}

//////////////////////////////////////////////////////////////////////////////////////////////
//イベントハンドラ
//////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CPythonBatchDlg, CDialog)

CPythonBatchDlg::CPythonBatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PYTHON_BATCH_DLG, pParent)
{

}

CPythonBatchDlg::~CPythonBatchDlg()
{
	execbmp.DeleteObject();
	editbmp.DeleteObject();
}

void CPythonBatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPythonBatchDlg, CDialog)
	ON_BN_CLICKED(IDC_PYTHON_BATCH_DLG_BROWSE_FILE, &CPythonBatchDlg::OnBnClickedPythonBatchDlgBrowseFile)
	ON_BN_CLICKED(IDC_BUTTON_IDC_PYTHON_BATCH_DLG_TRIAL, &CPythonBatchDlg::OnBnClickedButtonIdcPythonBatchDlgTrial)
	ON_BN_CLICKED(IDC_BUTTON_IDC_PYTHON_BATCH_DLG_VERCHK, &CPythonBatchDlg::OnBnClickedButtonIdcPythonBatchDlgVerchk)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_IDC_PYTHON_BATCH_DLG_EDIT, &CPythonBatchDlg::OnBnClickedButtonIdcPythonBatchDlgEdit)
	ON_WM_DROPFILES()
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
BOOL CPythonBatchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(AfxGetApp()->LoadIcon(IDI_RUN), TRUE);			// アイコンを設定

	//ビットマップボタン
	execbmp.LoadBitmap(IDB_BUTTON_3D_0);
	editbmp.LoadBitmap(IDB_BUTTON_EDIT);
	((CButton*)GetDlgItem(IDC_BUTTON_IDC_PYTHON_BATCH_DLG_TRIAL))->SetBitmap((HBITMAP)execbmp);
	((CButton*)GetDlgItem(IDC_BUTTON_IDC_PYTHON_BATCH_DLG_EDIT))->SetBitmap((HBITMAP)editbmp);

	//ツールチップ
	m_toolTip.Create(this, TTS_ALWAYSTIP);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_IDC_PYTHON_BATCH_DLG_TRIAL), "実行");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_IDC_PYTHON_BATCH_DLG_EDIT), "編集");
										
	DragAcceptFiles();//ドラッグ＆ドロップ対応

	arrange_dlg_item();

	API.SetEditValue(m_hWnd, IDC_EDIT_PYTHON_BATCH_DLG_FILEPATH, API.python_batch_file);

	return true;
}

/********************************************************************
機  能  名  称 : ウィンドウのリサイズ
関    数    名 : OnSize
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPythonBatchDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	arrange_dlg_item();
}

/********************************************************************
機  能  名  称 :キーボードからの入力
関    数    名 : PreTranslateMessage
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
BOOL CPythonBatchDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSEMOVE)
	{
		//ツールチップ表示
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_IDC_PYTHON_BATCH_DLG_TRIAL)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_IDC_PYTHON_BATCH_DLG_EDIT)->m_hWnd
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
機  能  名  称 : pyファイルのブラウズ
関    数    名 : OnBnClickedPythonBatchDlgBrowseFile
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPythonBatchDlg::OnBnClickedPythonBatchDlgBrowseFile()
{
	int fileNumber;
	CString filePath, fileName, ext, default_file;

	API.GetEditValue(m_hWnd, IDC_EDIT_PYTHON_BATCH_DLG_FILEPATH, &default_file);

	if (API.OpenFileDialog(1, _T("*.py|*.py"), &filePath, &fileName, &fileNumber, default_file.GetBuffer()))
	{
		API.python_batch_file = filePath + fileName;

		API.SetEditValue(m_hWnd, IDC_EDIT_PYTHON_BATCH_DLG_FILEPATH, API.python_batch_file);

		API.WriteProfile("SETTING", "PYTHON_BATCH_FILE", API.python_batch_file);//PIMPOM.iniに書き込む
	}
	default_file.ReleaseBuffer();
}

/********************************************************************
機  能  名  称 : ドラッグアンドドロップ
関    数    名 : OnDropFiles
引          数 :
戻    り    値 :
機          能 : 
			日付         作成者          内容
			------------ --------------- ---------------------------------------
			Y.Ikeda         新規作成
********************************************************************/
void CPythonBatchDlg::OnDropFiles(HDROP hDropInfo)
{
	UINT size = DragQueryFile(hDropInfo, 0, NULL, 0) + 1;//ファイル名の長さを取得

	CString filename;
	DragQueryFile(hDropInfo, 0, filename.GetBuffer(size), size);//ファイル名の取得
	filename.ReleaseBuffer();

	if (filename.Right(2).MakeLower() == _T("py"))//pyの場合はファイル展開
	{
		API.python_batch_file = filename;

		API.SetEditValue(m_hWnd, IDC_EDIT_PYTHON_BATCH_DLG_FILEPATH, API.python_batch_file);

		API.WriteProfile("SETTING", "PYTHON_BATCH_FILE", API.python_batch_file);//PIMPOM.iniに書き込む
	}
}


/********************************************************************
機  能  名  称 : 実行ボタン押下
関    数    名 : OnBnClickedButtonIdcPythonBatchDlgTrial
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPythonBatchDlg::OnBnClickedButtonIdcPythonBatchDlgTrial()
{
	UINT thrID = 0;
	HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, &PythonBatchThread, this, 0, &thrID);
}

/********************************************************************
機  能  名  称 : 編集ボタン押下
関    数    名 : OnBnClickedButtonIdcPythonBatchDlgEdit
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPythonBatchDlg::OnBnClickedButtonIdcPythonBatchDlgEdit()
{
	CString filepath;
	API.GetEditValue(m_hWnd, IDC_EDIT_PYTHON_BATCH_DLG_FILEPATH, &filepath);

	ShellExecute(NULL, "open", filepath, NULL, NULL, SW_SHOWNORMAL);
}


/********************************************************************
機  能  名  称 : python環境チェック
関    数    名 : OnBnClickedButtonIdcPythonBatchDlgVerchk
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPythonBatchDlg::OnBnClickedButtonIdcPythonBatchDlgVerchk()
{
	verCheck = true;

	UINT thrID = 0;
	HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, &PythonBatchThread, this, 0, &thrID);
}