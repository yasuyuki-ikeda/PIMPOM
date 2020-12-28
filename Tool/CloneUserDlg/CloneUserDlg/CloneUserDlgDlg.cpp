/*******************************************************************
Copyright (c) 2020, yasuyuki ikeda All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.
* Neither the name of the <organization> nor the names of its contributors 
  may be used to endorse or promote products derived from this software 
  without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************/
//

#include "stdafx.h"
#include "CloneUserDlg.h"
#include "CloneUserDlgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCloneUserDlgDlg ダイアログ

#define		USERDLG_DIR_PATH	"..\\..\\..\\CustomFunc\\"
#define		USERDLG_PROJ_NAME	"UserDlgTemplate"
#define		CR		(0x0d)
#define		LF		(0x0a)


CCloneUserDlgDlg::CCloneUserDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCloneUserDlgDlg::IDD, pParent)
	, m_DlgID(0)
	, m_ProjName(_T(""))
	, m_Result(_T(""))
	, m_Caption(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCloneUserDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_DlgID);
	DDX_Text(pDX, IDC_EDIT_PRJNAME, m_ProjName);
	DDX_Text(pDX, IDC_EDIT_RESULT, m_Result);
	DDX_Text(pDX, IDC_EDIT_CAPTION, m_Caption);
}

CString CCloneUserDlgDlg::GetDefaultPath()
{
	static CString strAppPath;
	if(strAppPath.IsEmpty())
	{
		// アプリケーションパスを取得
		if(GetModuleFileName(NULL, strAppPath.GetBuffer(_MAX_PATH), _MAX_PATH))
		{
			strAppPath.ReleaseBuffer();
			char szDrive[_MAX_PATH]; // D:
			char szDir[_MAX_PATH]; // \\folders\\ 
			char szFName[_MAX_PATH]; // filename
			char szExt[_MAX_PATH]; // .ext

			// フルパスを分解
			_tsplitpath(strAppPath, szDrive, szDir, szFName, szExt);

			// パスを作成
			strAppPath.Format(_T("%s%s"), szDrive, szDir); 
		} 
		else		
		{// エラーの場合は空のCStringを返す
			strAppPath.ReleaseBuffer();
			strAppPath.Empty();
		}
	}
	return strAppPath;
}


void	CCloneUserDlgDlg::ShowMessage(CString str)
{
	CString	ret;
	ret.Format("%c%c", (char)CR , (char)LF);//改行コード

	m_Result = m_Result + str + ret;
}


bool	GetCheck(HWND h_wnd, UINT id)
{
	HWND	h_button;
	CButton	*p_button;

	h_button = (HWND)::GetDlgItem(h_wnd, id);//ボタンのポインタを取得する
	if (h_button == NULL)		return false;

	p_button = (CButton*)CWnd::FromHandle(h_button);
	return	(p_button->GetCheck() == 1) ? true : false;
}

bool	SetCheck(HWND h_wnd, UINT id, bool value)
{
	HWND	h_button;
	CButton	*p_button;

	h_button = (HWND)::GetDlgItem(h_wnd, id);//ボタンのポインタを取得する
	if (h_button == NULL)		return false;

	p_button = (CButton*)CWnd::FromHandle(h_button);
	p_button->SetCheck(value);
	return	true;
}



BEGIN_MESSAGE_MAP(CCloneUserDlgDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CLONE, &CCloneUserDlgDlg::OnBnClickedButtonClone)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_INI, &CCloneUserDlgDlg::OnBnClickedButtonEditIni)
END_MESSAGE_MAP()


// CCloneUserDlgDlg メッセージ ハンドラ

BOOL CCloneUserDlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	FILE *fp;
	fp = fopen(GetDefaultPath() + "custom_func.ini","rt");
	if(fp){
		int cnt=0;
		for(;;){
			char buf[1024];
			if( fgets(buf,1024,fp) != NULL){
				if(strlen(buf)>2){
					cnt++;
				}
			}else{
				break;
			}
		}
		fclose(fp);

		m_DlgID = cnt;

		UpdateData(false);
	}

	SetCheck(m_hWnd, IDC_RADIO_VS14, true);


	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CCloneUserDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CCloneUserDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCloneUserDlgDlg::OnBnClickedButtonClone()
{
	CString srcpath, dstpath, srcfilename, dstfilename ;
	FILE *list_fp=NULL, *src_fp=NULL, *dst_fp=NULL, *ini_fp;

	UpdateData(true);

	//プロジェクト名がただしいかどうか
	if(m_ProjName.IsEmpty()){
		return;
	}

	for(int i=0 ; i<m_ProjName.GetLength() ; i++){
		if(!isalpha(m_ProjName.GetAt(i)) && !isdigit(m_ProjName.GetAt(i))){
			AfxMessageBox("プロジェクト名に半角英数字以外の文字が含まれています");
			return;
		}
	}


	//プロジェクトのファイルをひな形からコピー

	srcpath = GetDefaultPath() + CString(USERDLG_DIR_PATH) + CString(USERDLG_PROJ_NAME) + "\\";//コピー元ファイルパス
	dstpath = GetDefaultPath() + CString(USERDLG_DIR_PATH) + m_ProjName + "\\";//コピー先ファイルパス


	CreateDirectory(GetDefaultPath() + CString(USERDLG_DIR_PATH) + m_ProjName, NULL);//プロジェクトのディレクトリ作成


	list_fp = fopen(srcpath + "files.txt", "rt");//ファイルリストを開く
	if(list_fp)
	{
		char buf[1024];
		for(int i=0 ; i<100 ; i++)
		{
			if( fgets(buf, 1024, list_fp) != NULL)
			{
				//ファイルコピー
				dstfilename = srcfilename = CString(buf).Left(strlen(buf)-1);//最後の改行省く
				dstfilename.Replace( CString(USERDLG_PROJ_NAME), m_ProjName);//ファイル名を変更してコピー
		

				if (srcfilename == CString("UserDlgTemplate.vcxproj"))//プロジェクト
				{
					if (GetCheck(m_hWnd, IDC_RADIO_VS14)) {
						srcfilename = CString("UserDlgTemplate.vcxproj");
					}
					else if (GetCheck(m_hWnd, IDC_RADIO_VS12)) {

					}
					else {
	
					}
				}


				if( dstfilename.Find(".")==-1 )//ファイル名に拡張子がない場合はディレクトリ
				{
					CreateDirectory(dstpath + dstfilename, NULL);//ディレクトリ作成
				}
				else if( dstfilename.Find(".lib")!=-1 || dstfilename.Find(".dll")!=-1 )//バイナリ
				{
					::CopyFileA(srcpath + srcfilename, dstpath + dstfilename, true);
				}
				else
				{
					src_fp =  fopen(srcpath + srcfilename, "rt");//読み込み元ファイルを開く
					dst_fp =  fopen(dstpath + dstfilename, "wt");//読み込み元ファイルを開く
					if(src_fp && dst_fp)
					{
						for(;;)//コピー元を1行づつ読み込んでコピー先に書き込み
						{
							char buf2[1024];
							if( fgets(buf2, 1024, src_fp) != NULL)
							{
								CString docs(buf2);
								docs.Replace(CString(USERDLG_PROJ_NAME), m_ProjName);//プロジェクト名を変更してコピー
								docs.Replace(CString("custom_function_caption"), m_Caption);//キャプションを変更してコピー
								
								//リソース定義はIDと連動させる（PIMPOMCoreや他のユーザダイアログと被らないように）

								if(dstfilename.Find("Resource.h")!=-1){
									if(docs.Find("IDD_USER_DLG_EX") !=-1){
										docs.Format("#define IDD_USER_DLG_EX                 %d\n", m_DlgID*50+400 );
									}
									else if(docs.Find("_APS_NEXT_RESOURCE_VALUE") !=-1){
										docs.Format("#define _APS_NEXT_RESOURCE_VALUE        %d\n", m_DlgID*50+401 );
									}
									else if(docs.Find("_APS_NEXT_COMMAND_VALUE") !=-1){
										docs.Format("#define _APS_NEXT_COMMAND_VALUE         %d\n", m_DlgID*1000+40001 );
									}
									else if(docs.Find("_APS_NEXT_CONTROL_VALUE") !=-1){
										docs.Format("#define _APS_NEXT_CONTROL_VALUE        %d\n", m_DlgID*1000+2001 );
									}
									else if(docs.Find("_APS_NEXT_SYMED_VALUE") !=-1){
										docs.Format("#define _APS_NEXT_SYMED_VALUE        %d\n", m_DlgID*10+301 );
									}
								}

								
								fputs(docs, dst_fp);
							}
							else
							{
								break;//ファイルの最後または読み込み失敗
							}
						}
						fclose(src_fp);
						fclose(dst_fp);

						ShowMessage( dstpath + dstfilename );
						UpdateData(false);
					}
				}
			}
			else
			{
				break;//ファイルの最後または読み込み失敗
			}
		}

		fclose(list_fp);


		//iniファイルに反映(Debug/Release両方)
		CString inipath = GetDefaultPath() + "custom_func.ini";
		for(int i=0 ; i<2 ; i++){
			if(i==0)	inipath.Replace("Debug", "Release");
			else		inipath.Replace("Release", "Debug");
			
			ini_fp = fopen(inipath,"at");
			if(ini_fp){
				fprintf(ini_fp, m_ProjName + "=" + m_Caption + "\n", m_DlgID);
				fclose(ini_fp);
			}
		}

		AfxMessageBox("変更を反映するにはPIMPOMを再起動してください");


		//VisualStudio起動
		CString slnpath;
		
		if (GetCheck(m_hWnd, IDC_RADIO_VS14)) {
			slnpath = dstpath + m_ProjName + "2015.sln";
		}
		else if (GetCheck(m_hWnd, IDC_RADIO_VS12)) {
			slnpath = dstpath + m_ProjName + "2013.sln";
		}
		else {
			slnpath = dstpath + m_ProjName + "2008.sln";
		}
		ShellExecute(m_hWnd, "open", slnpath, NULL,NULL,SW_SHOWNORMAL);

		//ウィンドウを閉じる
		AfxGetMainWnd()->PostMessage(WM_CLOSE);	
	}

	
}

void CCloneUserDlgDlg::OnBnClickedButtonEditIni()
{
	CString inipath = GetDefaultPath() + "custom_func.ini";
	ShellExecute(m_hWnd, "open", inipath, NULL,NULL,SW_SHOWNORMAL);		
}
