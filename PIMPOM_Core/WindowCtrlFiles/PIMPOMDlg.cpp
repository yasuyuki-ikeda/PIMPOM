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
//CPIMPOMDlg
//メインウィンドウ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include <vfw.h>
#include "PIMPOMDlg.h"
#include "pimpom_plot.h"
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
extern CPimpomAPI	API;

// *************************************
//         関    数    参    照         
// *************************************

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われている CAboutDlg ダイアログ
/////////////////////////////////////////////////////////////////////////////
class CAboutDlg : public CDialog
{
	enum { IDD = IDD_ABOUTBOX };// ダイアログ データ

public:
	CAboutDlg();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{

}

BOOL CAboutDlg::OnInitDialog()
{
	::SetWindowText(::GetDlgItem(m_hWnd,IDC_LABEL_VERSION), API.GetVersion("ProductVersion") );
	::SetWindowText(::GetDlgItem(m_hWnd,IDC_VERSION_COPYRIGHT), API.GetVersion("LegalCopyright") );
	return true;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
		// メッセージ ハンドラがありません。
END_MESSAGE_MAP()




unsigned __stdcall BackGroundThread(void* pArg)
{
	CPIMPOMDlg* pDlg = (CPIMPOMDlg*)pArg;

	pDlg->BackGroundProc();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CPIMPOMDlg ダイアログ
/////////////////////////////////////////////////////////////////////////////


CPIMPOMDlg::CPIMPOMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPIMPOMDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	hThread = NULL;
}

void CPIMPOMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPIMPOMDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOVE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_WM_DROPFILES()
	ON_WM_INITMENU()
	ON_WM_MENUSELECT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_PIMPON_DLG_DATA_SELECT0, OnPimponDlgDataSelect0)
	ON_BN_CLICKED(IDC_CHECK_PIMPON_DLG_DATA_SELECT1, OnPimponDlgDataSelect1)
	ON_BN_CLICKED(IDC_CHECK_PIMPON_DLG_DATA_SELECT2, OnPimponDlgDataSelect2)
	ON_BN_CLICKED(IDC_CHECK_PIMPON_DLG_DATA_SELECT3, OnPimponDlgDataSelect3)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_PAINT, OnPimponDlgButtonPaint)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_CALC, OnPimponDlgButtonCalc)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_WORK_AREA, OnPimponDlgButtonWorkArea)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_MARK, OnPimponDlgButtonMark)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_MASK, OnPimponDlgButtonMask)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE, OnPimponDlgButtonRulerTool)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_SCALE_DBL, OnPimponDlgButtonScaleDbl)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_SCALE_HLF, OnPimponDlgButtonScaleHlf)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_SCALE_ADJUST_FILED, OnPimponDlgButtonScaleAdjustFiled)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_NO_SCALE, OnPimponDlgButtonNoScale)
	ON_BN_CLICKED(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE, &CPIMPOMDlg::OnBnClickedCheckVideoCapture)
	ON_BN_CLICKED(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2, &CPIMPOMDlg::OnBnClickedPimponDlgCheckVideoCapture2)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE2, &CPIMPOMDlg::OnBnClickedButtonPimponDlgCalcDistance2)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_HISTGRAM, &CPIMPOMDlg::OnBnClickedButtonPimponDlgHistgram)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PIMPOM_DLG_MESSAGE, &CPIMPOMDlg::OnTcnSelchangeTabPimpomDlgMessage)
	ON_MESSAGE(WM_EXTERNAL_INPUT, OnMessageExtInput)
	ON_MESSAGE(WM_EXTERNAL_OUTPUT, OnMessageExtOutput)
	ON_MESSAGE(WM_EXTERNAL_PROCESS, OnMessageExtProcess)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN_SERIAL, OnFileOpenSerial)
	ON_COMMAND(ID_FILE_SAVE_DATA, OnFileSaveData)
	ON_COMMAND(ID_FILE_SAVE_VIEW, OnFileSaveView)
	ON_COMMAND(ID_FILE_SAVE_IMAGE, OnFileSaveImage)
	ON_COMMAND(ID_FILE_SAVE_SERIAL, OnFileSaveSerial)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_QUIT, OnFileQuit)
	ON_COMMAND(ID_FILE_SAVE_ALLIMAGE, &CPIMPOMDlg::OnFileSaveAllimage)
	ON_COMMAND_RANGE(FILE_IO_LOG_MENU_ID, FILE_IO_LOG_MENU_ID + FILE_IO_LOG_MAX - 1, OnMenuLoadIOLog)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_CONVERT_TYPE, OnEditConvertType)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_EDIT_COPY_CLIPBORD, OnEditCopyClipbord)
	ON_COMMAND(ID_EDIT_RENAME, &CPIMPOMDlg::OnEditRename)
	ON_COMMAND(ID_EDIT_PASTE_CLIPBORD, &CPIMPOMDlg::OnEditPasteClipbord)
	ON_COMMAND(ID_EDIT_DELETE_ALL, &CPIMPOMDlg::OnEditDeleteAll)
	ON_COMMAND(ID_EDIT_MARK, &CPIMPOMDlg::OnEditMark)
	ON_COMMAND(ID_EDIT_WORKAREA, &CPIMPOMDlg::OnEditWorkarea)
	ON_COMMAND(ID_EDIT_PAINT, &CPIMPOMDlg::OnEditPaint)
	ON_COMMAND(ID_VIEW_COLOR_GRAYSCALE, OnViewColorGrayscale)
	ON_COMMAND(ID_VIEW_COLOR_NEGA, OnViewColorNega)
	ON_COMMAND(ID_VIEW_COLOR_RAINBOW, OnViewColorRainbow)
	ON_COMMAND(ID_VIEW_COLOR_RAINBOW2, OnViewColorRainbow2)
	ON_COMMAND(ID_VIEW_COLOR_COSINE, OnViewColorCosine)
	ON_COMMAND(ID_VIEW_COLOR_YTOB, OnViewColorYtob)
	ON_COMMAND(ID_VIEW_COLOR_GTOR, OnViewColorGtor)
	ON_COMMAND(ID_VIEW_MAKEFONT, OnViewMakefont)
	ON_COMMAND(ID_VIEW_MULTI_IMAGE_DISP, &CPIMPOMDlg::OnViewMultiImageDisp)
	ON_COMMAND(ID_VIEW_COLOR_LABEL, &CPIMPOMDlg::OnViewColorLabel)
	ON_COMMAND(ID_VIEW_MARK, &CPIMPOMDlg::OnViewMark)
	ON_COMMAND(ID_VIEW_WORKAREA, &CPIMPOMDlg::OnViewWorkarea)
	ON_COMMAND(ID_VIEW_SETUP_ENVIRONMENT, &CPIMPOMDlg::OnViewSetupEnvironment)
	ON_COMMAND(ID_VIEW_3D_IMG_DISP, &CPIMPOMDlg::OnView3dImgDisp)
	ON_COMMAND(ID_IMAGE_RESIZE, OnEditResize)
	ON_COMMAND(ID_IMAGE_EQUATION, OnCalcImage)
	ON_COMMAND(ID_IMAGE_FILTER, OnFilter)
	ON_COMMAND(ID_IMAGE_TRANSFORM, &CPIMPOMDlg::OnImageTransform)
	ON_COMMAND(ID_IMAGE_BINALIZE, &CPIMPOMDlg::OnBinalize)
	ON_COMMAND(ID_TOOL_SOCKET, &CPIMPOMDlg::OnToolSocket)
	ON_COMMAND(ID_TOOL_Ruler, &CPIMPOMDlg::OnToolRuler)
	ON_COMMAND(ID_TOOL_PYTHON_BATCH, &CPIMPOMDlg::OnToolPythonBatch)
	ON_COMMAND(ID_TOOL_BATCH, &CPIMPOMDlg::OnToolBatch)
	ON_COMMAND(ID_TOOL_COMMAND_PROMPT, &CPIMPOMDlg::OnToolCommandPrompt)
	ON_COMMAND(ID_TOOL_HISTGRAM, &CPIMPOMDlg::OnToolHistgram)
	ON_COMMAND(ID_TOOL_VIDEOCAPTURE, &CPIMPOMDlg::OnToolVideocapture)
	ON_COMMAND(ID_CUSTOM_ADD, &CPIMPOMDlg::OnCustomAdd)
	ON_COMMAND_RANGE(CUSTOM_MENU_ID, CUSTOM_MENU_ID + CUSTOM_FUNC_MAX - 1, OnMenuCustomFunc)
	ON_COMMAND(ID_HELP_SPECIFICATION_SHEET, &CPIMPOMDlg::OnHelpSpecificationSheet)
	ON_COMMAND(ID_HELP_APIREFERENCE, &CPIMPOMDlg::OnHelpApireference)
	ON_COMMAND(ID_HELP_APIREFERENCE2, &CPIMPOMDlg::OnHelpApireference2)
	ON_COMMAND(ID_HELP_APIREFERENCE3, &CPIMPOMDlg::OnHelpApireference3)
	ON_COMMAND(ID_HELP_VERSION, OnHelpVersion)
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//初期化と破棄
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
BOOL CPIMPOMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニュー項目をシステム メニューへ追加します。

	// IDM_ABOUTBOX はコマンド メニューの範囲でなければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}


	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定

	//INIファイルから設定を読み込む
	API.InitSettingFromProfile();

	/////////////////////////////////////////////////////////
	//表示画像の確保
	/////////////////////////////////////////////////////////
	p_image_disp_main = new CImageField( m_hWnd, API.main_image_size, CPoint(MAIN_WIN_LEFT_MARGIN, MAIN_WIN_TOP_MARGIN)  );//メイン画面を表すImageFiledのインスタンス化
	p_image_disp_piece = new CImageField( m_hWnd, CSize(60, 60), CPoint(DATA_VALUE_DISP_FIELD_X, API.main_window_size.cy-140)  );//指定した座標の周辺画素を表示するImageFieldのインスタンス化

	API.pPIMMOMDlg = this;
	API.DialogOpen(IDD_DATA_DLG,true);
	API.DialogOpen(IDD_HISTGRAM_DLG,false);
	API.DialogOpen(IDD_WORKAREA_DLG,false);
	API.DialogOpen(IDD_MARK_DLG,false);


	/////////////////////////////////////////////////////////
	//ウインドウの表示設定
	/////////////////////////////////////////////////////////

	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_PIMPOM_DLG_MESSAGE);
	p_wnd_tab->InsertItem(0, _T("メッセージ0"));
	p_wnd_tab->InsertItem(1, _T("メッセージ1"));

	load_bitmaps();//ボタンに貼り付けるためのビットマップリソースのロード
	arrange_buttons();//レイヤー選択ボタン，データ選択ボタンの配置
	set_bitmap_for_layer_button();//レイヤー選択ボタンにビットマップを関連づける
	arrange_data_value_disp_field();//指定した座標のデータ値を表示する領域の配置
	arrange_message_disp_field();//メッセージを表示するためのダイアログアイテムの配置する
	CWnd *p_h_scroll = GetDlgItem( IDC_SCROLL_PIMPOM_DLG_MAIN_IMAGE_H );
	CWnd *p_v_scroll = GetDlgItem( IDC_SCROLL_PIMPOM_DLG_MAIN_IMAGE_V );
	p_h_scroll->MoveWindow( MAIN_WIN_LEFT_MARGIN , MAIN_WIN_TOP_MARGIN + API.main_image_size.cy , API.main_image_size.cx , 10 );//メイン画面の水平方向スクロールバーの配置
	p_v_scroll->MoveWindow( MAIN_WIN_LEFT_MARGIN + API.main_image_size.cx , MAIN_WIN_TOP_MARGIN , 10, API.main_image_size.cy );//メイン画面の垂直方向スクロールバーの配置

	DragAcceptFiles();//ドラッグ＆ドロップを許可

	AVIFileInit();//AVI扱う

	//ツールチップ
	m_toolTip.Create(this, TTS_ALWAYSTIP);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_PAINT), "フリーハンドで画像を編集");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC), "数式で画像を生成");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_MARK), "マーク");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_WORK_AREA), "作業領域");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_MASK), "マスク");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_HISTGRAM), "ヒストグラム");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE), "ルーラー");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE2), "ルーラーとラインブライト");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_DBL), "表示倍率拡大");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_HLF), "表示倍率縮小");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_ADJUST_FILED), "ウィンドウサイズに合わせて表示倍率を調整");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_NO_SCALE), "等倍表示");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE), "ライブ表示");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2), "ライブ実行");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0), "");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1), "");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT2), "");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT3), "");
	

	//カスタムファンクションをメニューに追加
	CString capts[CUSTOM_FUNC_MAX];
	int custon_func_cnt = API.GetUserDlgCaptions(capts);
	CMenu *menu = this->GetMenu();
	CMenu *pSub =menu->GetSubMenu(5);
	for(int i=0 ; i<custon_func_cnt ; i++){
		CString strcnt;
		strcnt.Format("%d ", i);
		pSub->AppendMenu(MF_STRING,CUSTOM_MENU_ID+i, strcnt + capts[i]);
	}

	/////////////////////////////////////////////////////////
	//メンバ初期化
	////////////////////////////////////////////////////////
	is_draging = false;//ドラッグ中を表すフラグを倒す
	is_draging_mbutton = false;//ドラッグ中を表すフラグを倒す
	latest_clicked_coordinate_x = 0;//最近クリックされた座標をクリア
	latest_clicked_coordinate_y = 0;
	calc_begin_x = 0;	//距離計算
	calc_begin_y = 0;
	calc_end_x = 0;	//距離計算
	calc_end_y = 0;

	UINT thrID = 0;
	hThread = (HANDLE)::_beginthreadex(NULL, 0, &BackGroundThread, this, 0, &thrID);


	//アイコン上にドラッグされたファイルを読み込む
	if(!cmdLine.IsEmpty())
	{
		CString filenames[30];
		int filenamecnt=0;
		bool filenameflg=false;

		//コマンドラインからファイル名を分離する
		if(cmdLine.Find("\"")!=-1){//ファイル名が""で囲まれている場合
			for(int i=0 ; i<cmdLine.GetLength() ; i++){
				if(cmdLine[i]=='\"'){
					if(!filenameflg){
						filenames[filenamecnt].Empty();
						filenameflg=true;
					}else{
						filenamecnt++;
						filenameflg=false;
					}
				}else{
					filenames[filenamecnt] += CString(cmdLine[i]);
				}
			}
		}else{//ファイル名が""で囲まれていない場合
			filenames[0] = cmdLine;
			filenamecnt=1;
		}

		//ファイルを読みだす
		for(int n=0 ; n<filenamecnt ; n++){
			API.SelectAndLoadImageFile(n,filenames[n]);
			API.DrawImage(n,true,true);
		}
	}

	if (API.invisible)
	{//ウィンドウ非表示モード
		API.pPIMMOMDlg->ShowWindow(SW_HIDE);
		API.pDataDlg->ShowWindow(SW_HIDE);
		API.pHistgramDlg->ShowWindow(SW_HIDE);
	}


	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
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
void CPIMPOMDlg::OnDestroy() 
{
	KillTimer(1);

	API.CaptureDeInit();//ビデオキャプチャの開放

	AVIFileExit();

	CDialog::OnDestroy();
	
	delete		p_image_disp_piece;
	delete		p_image_disp_main;

}

void CPIMPOMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。


/********************************************************************
機  能  名  称 : 再描画
関    数    名 : OnPaint
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);


	}
	else
	{
		DrawImage(true);//メイン画面を描画

		CDialog::OnPaint();
	}

	if (API.invisible)//ウィンドウ非表示モード
	{
		ShowWindow(SW_HIDE);
		API.pDataDlg->ShowWindow(SW_HIDE);
		API.pHistgramDlg->ShowWindow(SW_HIDE);
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CPIMPOMDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/********************************************************************
機  能  名  称 :キーボードからの入力
関    数    名 : PreTranslateMessage
引          数 : 
戻    り    値 : 
機          能 : カーソルキー：カーソルの移動
				 Delキー：選択中の画像メモリクリア
				 Enter/Escキー：無効化
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
BOOL CPIMPOMDlg::PreTranslateMessage(MSG* pMsg) 
{
	POINT point;
	
	if(pMsg->message == WM_KEYDOWN ){	
		switch(pMsg->wParam){
			case VK_RETURN://Enter
				return  true;

			case VK_ESCAPE://Esc
				return  true;

			case VK_UP://↑
				GetCursorPos( &point ) ;
				SetCursorPos(point.x , point.y - 1);
				return  true;

			case VK_DOWN://↓
				GetCursorPos( &point ) ;
				SetCursorPos(point.x , point.y + 1);
				return  true;

			case VK_LEFT://←
				GetCursorPos( &point ) ;
				SetCursorPos(point.x - 1 , point.y );
				return  true;

			case VK_RIGHT://→
				GetCursorPos( &point ) ;
				SetCursorPos(point.x + 1 , point.y );
				return true;

			case VK_DELETE://Del
				OnEditDelete();
				break;

			case 'C'://Ctrl + C
				if((GetKeyState(VK_CONTROL)&0x8000)!=0){
					API.CopyToClipbord();
				}
				break;

			case 'V'://Ctrl + V
				if((GetKeyState(VK_CONTROL)&0x8000)!=0)
				{
					if (!API.pDataDlg->DataExistMessage("上書きしてもよろしいですか？", CURRENT_IMAGE))
					{
						API.BackupDataUnit(CURRENT_IMAGE);//現在のデータバックアップ
						API.PasteFromClipbord(API.GetCurrentImageNumber());//クリップボードからペースト
					}
				}
				break;


			default:
				break;
		}
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		//ツールチップ表示
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_PAINT)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_MARK)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_WORK_AREA)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_MASK)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_HISTGRAM)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE2)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_DBL)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_HLF)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_ADJUST_FILED)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_NO_SCALE)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT2)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT3)->m_hWnd
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
機  能  名  称 : ウィンドウサイズ変更
関    数    名 : OnSize
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnSize(UINT nType, int cx, int cy)
{
	static bool	init_flg = true;

	CDialog::OnSize(nType, cx, cy);

	if (nType != SIZE_MINIMIZED)

	if (!init_flg)//一番最初に呼び出されたときだけは処理しない
	{
		//メインウィンドウとメインイメージのサイズ変更
		API.main_window_size.cx = cx;
		API.main_window_size.cy = cy;
		API.main_image_size.cx = API.main_window_size.cx - MAIN_WIN_LEFT_MARGIN - MAIN_WIN_RIGHT_MARGIN;
		API.main_image_size.cy = API.main_window_size.cy - MAIN_WIN_TOP_MARGIN - MAIN_WIN_BOTTOM_MARGIN;

		arrange_buttons();//レイヤー選択ボタン，データ選択ボタンの配置
		arrange_data_value_disp_field();//指定した座標のデータ値を表示する領域の配置
		arrange_message_disp_field();//メッセージを表示するためのダイアログアイテムの配置する
		CWnd *p_h_scroll = GetDlgItem(IDC_SCROLL_PIMPOM_DLG_MAIN_IMAGE_H);
		CWnd *p_v_scroll = GetDlgItem(IDC_SCROLL_PIMPOM_DLG_MAIN_IMAGE_V);
		p_h_scroll->MoveWindow(MAIN_WIN_LEFT_MARGIN, MAIN_WIN_TOP_MARGIN + API.main_image_size.cy, API.main_image_size.cx, 10);//メイン画面の水平方向スクロールバーの配置
		p_v_scroll->MoveWindow(MAIN_WIN_LEFT_MARGIN + API.main_image_size.cx, MAIN_WIN_TOP_MARGIN, 10, API.main_image_size.cy);//メイン画面の垂直方向スクロールバーの配置

		//メインイメージ描画
		CDataUnit *p_du = API.GetDataUnit(CURRENT_IMAGE);
		p_image_disp_main->Resize(API.main_image_size);
		p_image_disp_main->DrawImage(p_du, true);//描画;
		p_image_disp_piece->Move(CPoint(DATA_VALUE_DISP_FIELD_X, API.main_window_size.cy - 140));

		//メイン画像外部再描画
		CRect rightToolRegn(API.main_image_size.cx, 0, API.main_window_size.cx, API.main_window_size.cy);
		this->InvalidateRect(rightToolRegn);

		CRect bottomToolRegn(0, API.main_image_size.cy, API.main_image_size.cx, API.main_window_size.cy);
		this->InvalidateRect(bottomToolRegn);
	}

	init_flg = false;
}


/********************************************************************
機  能  名  称 :メインウインドウが移動した
関    数    名 : OnMove
引          数 : 
戻    り    値 : 
機          能 : データウィンドウとヒストグラムウィンドウの位置も同時に移動する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnMove(int x, int y) 
{
	static bool	init_flg=true;

	CDialog::OnMove(x, y);
	
	if(API.pDataDlg != NULL)
	{
		RECT  win_rect, client_rect;
		int   mrgn_w, mrgn_h;//ウィンドウ領域とクライアント領域のサイズ差分

		GetWindowRect(&win_rect);//現在のウィンドウ領域を表す矩形
		GetClientRect(&client_rect);//現在のクライアント領域を表す矩形

		mrgn_w = (win_rect.right - win_rect.left) - (client_rect.right - client_rect.left);
		mrgn_h = (win_rect.bottom - win_rect.top) - (client_rect.bottom - client_rect.top);


		if(init_flg){//最初だけ画面左上に表示
			win_rect.top=0;
			win_rect.left=0;
			win_rect.right=API.main_window_size.cx;
			win_rect.bottom=MARK_WIN_H;
			init_flg=false;
		}

		//データウィンドウの移動
		API.pDataDlg->MoveWindow(win_rect.left, win_rect.top + API.main_window_size.cy + mrgn_h, API.data_window_width, API.pDataDlg->WindowHeight);


		//ﾋｽﾄｸﾞﾗﾑウィンドウの移動
		if(API.pHistgramDlg)
		{
			API.pHistgramDlg->MoveWindow(win_rect.left + API.main_window_size.cx + mrgn_w, win_rect.top, HISTGRAM_WIN_W, HISTGRAM_WIN_H);
		}
		
		//マークウィンドウの移動
		if(API.pMarkDlg){
			API.pMarkDlg->MoveWindow(win_rect.left + API.main_window_size.cx + mrgn_w, win_rect.top + 20, MARK_WIN_W, MARK_WIN_H);
		}

		//作業領域ウィンドウの移動
		if(API.pWorkAreaDlg)
		{
			RECT clientRct = { 0 }, winodowRct = { 0 };
			::GetWindowRect(API.pWorkAreaDlg->m_hWnd, &winodowRct);//ウィンドウ領域を表す矩形を取得 
			::GetClientRect(API.pWorkAreaDlg->m_hWnd, &clientRct);//クライアント領域を表す矩形を取得 
			int offset_h = winodowRct.bottom - winodowRct.top - clientRct.bottom;
			int offset_w = winodowRct.right - winodowRct.left - clientRct.right;

			API.pWorkAreaDlg->MoveWindow(win_rect.left + API.main_window_size.cx + mrgn_w, win_rect.top + HISTGRAM_WIN_H + 30, WORKAREA_WIN_W + offset_w, offset_h + WORKAREA_WIN_H);
		}

		//このウインドウの配置(クライアント領域との差分配慮して大きくする)
		MoveWindow(win_rect.left, win_rect.top, API.main_window_size.cx + mrgn_w, API.main_window_size.cy + mrgn_h);
	}		
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//マウスイベントのための関数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 :マウスの左ボタンが押された
関    数    名 : OnLButtonDown
引          数 : 
戻    り    値 : 
機          能 : ①ポイント位置のデータ値を表示
　　　　　　　　 ②マウスドラッグ開始
				 ③各種クリックイベント
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{

	GetCapture();//マウスをキャプチャーする
	is_draging = true;//ドラッグ中を表すフラグを立てる
	
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中の DataUnit を取得
	CPoint		src_coor;
	double		src_coor_subpix_x, src_coor_subpix_y;
	if( p_du == NULL )		return;//画像データが無ければ何もしない

	if( p_image_disp_main->GetFieldRect().PtInRect( point ) ) //メイン画像上でクリックした場合
	{
		if( p_image_disp_main->GetSrcCoor(p_du, point.x, point.y, &src_coor_subpix_x, &src_coor_subpix_y) )
		{
			src_coor.x = (int)(src_coor_subpix_x+0.5);
			src_coor.y = (int)(src_coor_subpix_y+0.5);

			if(!API.GetCheck(m_hWnd,IDC_CHECK_PIMPON_DLG_SUBPIX) )//サブピクセル表示しない
			{
				src_coor_subpix_x = src_coor.x;
				src_coor_subpix_y = src_coor.y;
			}


			//クリックしたウインドウ上の座標に対応する画像内の座標
			latest_clicked_coordinate_x = src_coor_subpix_x;
			latest_clicked_coordinate_y = src_coor_subpix_y;


			//作業領域のマウスドラッグ開始イベント
			if (API.pWorkAreaDlg) {
				API.pWorkAreaDlg->OnMouseDownOnImage(src_coor);
			}

			//ポイント位置のデータ値を表示
			if( API.GetCheck( m_hWnd, IDC_CHECK_PIMPON_DLG_FIX_COOR ) )
				DispDataValue( src_coor_subpix_x, src_coor_subpix_y);//現在マウスでクリックした座標(画像転送前)のデータを表示

			//マークのマウスクリックイベント
			if(API.pMarkDlg)
			{
				API.pMarkDlg->OnMouseDownOnImage(src_coor_subpix_x, src_coor_subpix_y);
			}



			//画像データのフリーハンド編集時のマウスクリックイベント
			if(API.pDataPickDlg){
				API.pDataPickDlg->OnMouseDownOnImage(src_coor);//データのピック/ペイント
			}

			//ユーザダイアログのマウスクリックイベント
			for(int i=0 ; i<CUSTOM_FUNC_MAX ; i++){	
				API.UserDialogOnMouseDown(i,src_coor);
			}

			DrawImage(false);//再描画する

		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

/********************************************************************
機  能  名  称 :マウスの左ボタンが離された
関    数    名 : OnLButtonUp
引          数 : 
戻    り    値 : 
機          能 : マウスドラッグ終了
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();//マウスキャプチャーを開放する

	if(is_draging)//マウスドラッグが終了した時の処理
	{
		//データに対する処理
		CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中の DataUnit を取得
		CPoint		src_coor;
		double		src_coor_subpix_x, src_coor_subpix_y;
		if( p_du )
		{
			if( p_image_disp_main->GetFieldRect().PtInRect( point ) ) //マウスポインタがメイン画像上にある場合
			{
				if( p_image_disp_main->GetSrcCoor(p_du, point.x, point.y, &src_coor_subpix_x, &src_coor_subpix_y) )
				{
					src_coor.x = (int)(src_coor_subpix_x+0.5);
					src_coor.y = (int)(src_coor_subpix_y+0.5);

					if(!API.GetCheck(m_hWnd,IDC_CHECK_PIMPON_DLG_SUBPIX) )//サブピクセル表示しない
					{
						src_coor_subpix_x = src_coor.x;
						src_coor_subpix_y = src_coor.y;
					}

					//作業領域のマウスドラッグ終了イベント
					if (API.pWorkAreaDlg) {
						API.pWorkAreaDlg->OnMouseUpOnImage(CPoint(latest_clicked_coordinate_x, latest_clicked_coordinate_y), src_coor);
					}


					//マークのマウスドラッグ終了イベント
					if(API.pMarkDlg){
						API.pMarkDlg->OnMouseUpOnImage(latest_clicked_coordinate_x, latest_clicked_coordinate_y, src_coor_subpix_x, src_coor_subpix_y);
					}


					//ユーザダイアログのマウスドラッグ終了イベント
					for(int i=0 ; i<CUSTOM_FUNC_MAX ; i++){	
						API.UserDialogOnFinishMouseDrug(i, CPoint(latest_clicked_coordinate_x, latest_clicked_coordinate_y), src_coor);
					}
				}
			}
		}
	}

	is_draging = false;//マウスドラッグを表すフラグを倒す
	
	CDialog::OnLButtonUp(nFlags, point);
}


/********************************************************************
機  能  名  称 :マウスの中央ボタンが押された
関    数    名 : OnMButtonDown
引          数 :
戻    り    値 :
機          能 : 画像表示位置の移動
  日付         作成者          内容
 ------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	GetCapture();//マウスをキャプチャーする
	is_draging_mbutton = true;//ドラッグ中を表すフラグを立てる


	CDataUnit	*p_du = API.GetDataUnit(CURRENT_IMAGE);//現在選択中の DataUnit を取得
	if (p_du == NULL)		return;//画像データが無ければ何もしない

	double		src_coor_subpix_x, src_coor_subpix_y;
	if (p_image_disp_main->GetFieldRect().PtInRect(point)) //メイン画像上でクリックした場合
	{
		if (p_image_disp_main->GetSrcCoor(p_du, point.x, point.y, &src_coor_subpix_x, &src_coor_subpix_y))
		{
			//クリックしたウインドウ上の座標に対応する画像内の座標
			latest_mbutton_clicked_coordinate_x = src_coor_subpix_x;
			latest_mbutton_clicked_coordinate_y = src_coor_subpix_y;
		}
	}
}

/********************************************************************
機  能  名  称 :マウスの中央ボタンが離された
関    数    名 : OnMButtonUp
引          数 :
戻    り    値 :
機          能 : マウスドラッグ終了
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();//マウスキャプチャーを開放する
	is_draging_mbutton = false;
}

/********************************************************************
機  能  名  称 :マウスのポインタの通過
関    数    名 : OnMouseMove
引          数 : 
戻    り    値 : 
機          能 : ①ポイント位置のデータ値を表示
				 ②マウスドラッグ中
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中の DataUnit を取得
	CPoint		src_coor;
	double		src_coor_subpix_x, src_coor_subpix_y;

	if( p_du )
	{
		if( p_image_disp_main->GetFieldRect().PtInRect( point ) ) //メイン画像上をマウスポインタが横切る場合

			if( p_image_disp_main->GetSrcCoor(p_du, point.x, point.y, &src_coor_subpix_x, &src_coor_subpix_y) )
			{
				src_coor.x = (int)(src_coor_subpix_x+0.5);
				src_coor.y = (int)(src_coor_subpix_y+0.5);

				if(!API.GetCheck(m_hWnd,IDC_CHECK_PIMPON_DLG_SUBPIX) )//サブピクセル表示しない
				{
					src_coor_subpix_x = src_coor.x;
					src_coor_subpix_y = src_coor.y;
				}

				//ポイント位置のデータ値を表示
				if( ! API.GetCheck( m_hWnd, IDC_CHECK_PIMPON_DLG_FIX_COOR ) ){
					DispDataValue( src_coor_subpix_x, src_coor_subpix_y);//現在マウスでポイント中の画像データを表示
				}

				if(is_draging)//マウスドラッグ中の処理
				{
					//作業領域のマウスドラッグイベント
					if (API.pWorkAreaDlg) {
						API.pWorkAreaDlg->OnMouseDrugOnImage(CPoint(latest_clicked_coordinate_x, latest_clicked_coordinate_y), src_coor);
					}


					//ドラッグ始点・現在点の距離計算
					if( API.GetCheck( m_hWnd , IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE ) ){
						calc_begin_x = latest_clicked_coordinate_x;
						calc_begin_y = latest_clicked_coordinate_y;
						calc_end_x = src_coor_subpix_x;
						calc_end_y = src_coor_subpix_y;
						DrawImage(false);//再描画
					}

					//フリーハンド編集のマウスドラッグイベント
					if(API.pDataPickDlg){
						API.pDataPickDlg->OnMouseDrugOnImage(src_coor);//データのペイント
					}


					//マークのマウスドラッグイベント
					if(API.pMarkDlg){
							API.pMarkDlg->OnMouseDrugOnImage(latest_clicked_coordinate_x, latest_clicked_coordinate_y,src_coor_subpix_x, src_coor_subpix_y);
					}

					//ユーザダイアログのマウスドラッグイベント
					for(int i=0 ; i<CUSTOM_FUNC_MAX ; i++){	
						API.UserDialogOnMouseDrug(i, CPoint(latest_clicked_coordinate_x, latest_clicked_coordinate_y), src_coor);
					}
				}


				if (is_draging_mbutton)//マウス中央ボタンドラッグ中の処理
				{
					p_image_disp_main->ScrollImage(p_du, CPoint(p_du->DataDispOffset.x - (src_coor_subpix_x - latest_mbutton_clicked_coordinate_x )
															, p_du->DataDispOffset.y - (src_coor_subpix_y - latest_mbutton_clicked_coordinate_y)));//画像転送元の矩形領域をスクロールさせる
					DrawImage(true);//再描画
				}


				//画像データ直接編集中はカーソルを変更する
				if(API.pDataPickDlg){
					API.pDataPickDlg->OnMouseMoveOnImage(src_coor);
				}
		}
	}
	
	CDialog::OnMouseMove(nFlags, point);
}


/********************************************************************
機  能  名  称 :マウスホイールの回転
関    数    名 : OnMouseWheel
引          数 :
戻    り    値 :
機          能 : 表示画像の拡大・縮小
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
BOOL CPIMPOMDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CDataUnit	*p_du = API.GetDataUnit(CURRENT_IMAGE);
	if (p_du != NULL)
	{
		//スケールを変更して描画
		float scale = p_du->DispScale;
		if (zDelta>0){
			scale *= 1.1;//10%拡大
		}
		else{
			scale *= 0.9;//10%縮小
		}

		API.ScaleImage(CURRENT_IMAGE, scale);
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//画面表示・描画
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : メイン画面描画
関    数    名 : DrawImage
引          数 : bool redraw_all (in)trueにすると，背景とスクロールバーが再描画される
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::DrawImage(bool redraw_all)
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中の画像のDataUnitを取得
	CSize		src_size;
	CScrollBar	*p_h_scroll, *p_v_scroll;//スクロールバーのポインタ
	int			scroll_condition_h = SW_HIDE;//水平方向スクロールバーの可視状態を表す変数（不可視状態で初期化）
	int			scroll_condition_v = SW_HIDE;//垂直方向スクロールバーの可視状態を表す変数（不可視状態で初期化）

		if(redraw_all){//背景，スクロールバー，データ表示ボタン等の再描画
			p_h_scroll = (CScrollBar*)GetDlgItem( IDC_SCROLL_PIMPOM_DLG_MAIN_IMAGE_H );
			p_v_scroll = (CScrollBar*)GetDlgItem( IDC_SCROLL_PIMPOM_DLG_MAIN_IMAGE_V );

				if( p_du != NULL ){//データがある場合の処理
					//スクロールバーを設定する
					p_image_disp_main->GetSrcSize(p_du, &src_size);//画像転送元の矩形領域のサイズを取得

						if( src_size.cx < p_du->DataSize.cx ){//水平方向スクロールバーが現れるとき＝画像転送元の幅が画像全体の幅より小さい場合
							set_scrollbar( *p_h_scroll, src_size.cx, p_du->DataDispOffset.x, 0, p_du->DataSize.cx - 1);
							scroll_condition_h = SW_SHOW;
						}

						if( src_size.cy < p_du->DataSize.cy ){//垂直方向スクロールバーが現れるとき＝画像転送元の高さが画像全体の高さより小さい場合
							set_scrollbar( *p_v_scroll, src_size.cy, p_du->DataDispOffset.y, 0, p_du->DataSize.cy - 1);
							scroll_condition_v = SW_SHOW;
						}

					API.SetCheck( m_hWnd , IDC_BUTTON_PIMPON_DLG_MASK , p_du->DoUseMask );//マスクの状態を表示する
					API.SetEditValue( m_hWnd , IDC_EDIT_PIMPON_DLG_SCALE , p_du->DispScale );//縮尺を表示する

						if( API.GetCheck( m_hWnd, IDC_CHECK_PIMPON_DLG_FIX_COOR ) )//クリック座標を表示する場合
							DispDataValue( latest_clicked_coordinate_x, latest_clicked_coordinate_y );
				}
					
			p_h_scroll->ShowWindow(scroll_condition_h);//スクロールバーの可視状態設定
			p_v_scroll->ShowWindow(scroll_condition_v);
			
			set_bitmap_for_data_button();//表示データ選択ボタン
		}

	message[0].Empty();//メッセージボックスクリア
	disp_information_titlebar();//タイトルバーに情報表示
	p_image_disp_main->DrawImage( p_du , redraw_all);//描画

	draw_2points_distance();//2点間の距離
	draw_work_area();//作業領域
	draw_marks();//マーク

	//ユーザダイアログの描画イベント
	for (int i = 0; i<CUSTOM_FUNC_MAX; i++){
		API.UserDialogOnDrawMainImage(i, API.GetCurrentImageNumber());
	}
}

/********************************************************************
機  能  名  称 : メイン画面のImageFieldへのポインタを返す
関    数    名 : GetMainImage
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
CImageField* CPIMPOMDlg::GetMainImage()
{
	return	p_image_disp_main;
}

/********************************************************************
機  能  名  称 : スクロールバーを設定する
関    数    名 : set_scrollbar
引          数 : CScrollBar		&sc		(out)スクロールバーのコントロール参照
				 int			page	(in)ページ幅
				 int			pos		(in)位置
				 int			min		(in)最小位置
				 int			max		(in)最大位置
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::set_scrollbar(CScrollBar &sc, int page, int pos, int min, int max)
{
	SCROLLINFO  scrollinfo;
	sc.GetScrollInfo( &scrollinfo , SIF_ALL );
	scrollinfo.nPage = page;
	scrollinfo.nPos  = pos ;
	scrollinfo.nMin  = min;
	scrollinfo.nMax  = max;
	sc.SetScrollInfo( &scrollinfo , true );	
}

/********************************************************************
機  能  名  称 : 
関    数    名 : control_main_image_scrollbar
引          数 : UINT			nSBCode		(in)
				 UINT			nPos		(in)
				 CScrollBar		*pScrollBar	(in)スクロールバーのコントロールへのポインタ
戻    り    値 : 操作後のスクロールバーの位置
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
long CPIMPOMDlg::control_main_image_scrollbar(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
	int dest_pos = pScrollBar->GetScrollPos();

		switch(nSBCode){
			case SB_LEFT:			//フォーカスを持っているとき[Home]を押したとき
				break;

			case SB_RIGHT:			//フォーカスを持っているとき[End]を押したとき
				break;

			case SB_LINELEFT:		//左矢印ボタンクリック
				dest_pos -= 10;		break;

			case SB_LINERIGHT:		//右矢印ボタンクリック
				dest_pos += 10;		break;

			case SB_PAGELEFT:		//左ページ領域クリック
				dest_pos -= 10;		break;

			case SB_PAGERIGHT:		//右ページ領域クリック
				dest_pos += 10;		break;

			case SB_THUMBPOSITION:	//つまみドラックで目的地に達した
				dest_pos = nPos;	break;

			case SB_THUMBTRACK:		//つまみドラッグ中である
				dest_pos = nPos;	break;

			case SB_ENDSCROLL:		//スクロールバー処理が終了した
				break;
		}
	pScrollBar->SetScrollPos(dest_pos);//スクロールバー再表示

	return	dest_pos;
}

/********************************************************************
機  能  名  称 : 水平方向スクロールバーが操作された
関    数    名 : OnHScroll
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中の画像のDataUnitを取得
	CPoint		src_offset;
	long		new_pos;

		if( pScrollBar == GetDlgItem( IDC_SCROLL_PIMPOM_DLG_MAIN_IMAGE_H ) ){//メイン画面のスクロールバーである

				if(p_du == NULL)	return;//データが無ければ何もしない

			p_image_disp_main->GetSrcOffset(p_du, &src_offset);//画像転送元の矩形領域のオフセット（左上の座標）を取得
			new_pos = control_main_image_scrollbar(nSBCode, nPos, pScrollBar);//スクロールバーから新しいオフセットを取得
			src_offset.x = new_pos;//新しいオフセットに設定
			p_image_disp_main->ScrollImage( p_du , src_offset);//画像転送元の矩形領域をスクロールさせる
			DrawImage(false);//再描画
		}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

/********************************************************************
機  能  名  称 : 垂直方向スクロールバーが操作された
関    数    名 : OnVScroll
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中の画像のDataUnitを取得
	CPoint		src_offset;
	long		new_pos;

		if( pScrollBar ==  GetDlgItem( IDC_SCROLL_PIMPOM_DLG_MAIN_IMAGE_V ) ){//メイン画面のスクロールバーである

				if(p_du == NULL)	return;//データが無ければ何もしない

			p_image_disp_main->GetSrcOffset(p_du, &src_offset);//画像転送元の矩形領域のオフセット（左上の座標）を取得
			new_pos = control_main_image_scrollbar(nSBCode, nPos, pScrollBar);//スクロールバーから新しいオフセットを取得
			src_offset.y = new_pos;//新しいオフセットに設定
			p_image_disp_main->ScrollImage( p_du , src_offset);//画像転送元の矩形領域をスクロールさせる
			DrawImage(false);//再描画

			
		}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

/********************************************************************
機  能  名  称 : タイトルバーに画像の名前やデータ種類を表示
関    数    名 : disp_information_titlebar
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::disp_information_titlebar()
{
	CString		info, type_name, size, page;
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );

		if( p_du != NULL){
			page.Empty();
			switch( p_du->DataType ){
				case BYTE_FORMAT:
					type_name.LoadString(IDS_DATATYPE_BYTE);			break;

				case FLOAT_FORMAT:
					type_name.LoadString(IDS_DATATYPE_FLOAT);		break;

				case THREE_D_FORMAT:
					type_name.LoadString(IDS_DATATYPE_3D);	
					page.Format("  %d/%d ", p_du->DispPage+1, p_du->PageNumber);		break;

				case FLOAT_3D_FORMAT:
					type_name.LoadString(IDS_DATATYPE_F3D);	
					page.Format("  %d/%d ", p_du->DispPage+1, p_du->PageNumber);		break;

				case COMPLEX_FORMAT:
					type_name.LoadString(IDS_DATATYPE_COMPLEX);			break;

				case XY_FORMAT:
					type_name.LoadString(IDS_DATATYPE_XY);		break;

				case XYZ_FORMAT:
					type_name.LoadString(IDS_DATATYPE_XYZ);			break;

				case RGB_FORMAT:
					type_name.LoadString(IDS_DATATYPE_RGB);			break;

				case SHORT16_FORMAT:
					type_name.LoadString(IDS_DATATYPE_SHORT);				break;

				case WORD_FORMAT:
					type_name.LoadString(IDS_DATATYPE_WORD);				break;

				case LONG32_FORMAT:
					type_name.LoadString(IDS_DATATYPE_LONG);				break;

				case DWORD_FORMAT:
					type_name.LoadString(IDS_DATATYPE_DWORD);				break;

				case RGB_3D_FORMAT:
					type_name.LoadString(IDS_DATATYPE_RGB3D);
					page.Format("  %d/%d ", p_du->DispPage+1, p_du->PageNumber);		break;

			}
			size.Format("(%d×%d)", p_du->DataSize.cx, p_du->DataSize.cy );//サイズを表示する
			info = p_du->DataName + _T("[") + type_name + page + _T("]") + size + _T(" - PIMPOM");

		}else{//データがなければこのソフトの名前を表示する
			info = _T("Platform for IMage Processing Observer and Modifier - PIMPOM");
		}

	CString idstr("  ");//ID付与
	if (API.pimpom_id > 0){
		idstr.Format(" %d", API.pimpom_id);
	}

	SetWindowText(info + idstr);//タイトルバーに表示する
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//画面右端のボタンに関する処理
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : ボタンに表示するビットマップを読み込む
関    数    名 : load_bitmaps
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::load_bitmaps()
{
	//ビットマップを読み込む
	bmp_button_3d[0].LoadBitmap(IDB_BUTTON_3D_0);
	bmp_button_3d[1].LoadBitmap(IDB_BUTTON_3D_1);
	bmp_button_3d[2].LoadBitmap(IDB_BUTTON_3D_2);
	bmp_button_3d[3].LoadBitmap(IDB_BUTTON_3D_3);

	bmp_button_complex[0].LoadBitmap(IDB_BUTTON_COMPLEX_0);
	bmp_button_complex[1].LoadBitmap(IDB_BUTTON_COMPLEX_1);
	bmp_button_complex[2].LoadBitmap(IDB_BUTTON_COMPLEX_2);
	bmp_button_complex[3].LoadBitmap(IDB_BUTTON_COMPLEX_3);

	bmp_button_xy[0].LoadBitmap(IDB_BUTTON_XY_0);
	bmp_button_xy[1].LoadBitmap(IDB_BUTTON_XY_1);

	bmp_button_xyz[0].LoadBitmap(IDB_BUTTON_XYZ_0);
	bmp_button_xyz[1].LoadBitmap(IDB_BUTTON_XYZ_1);
	bmp_button_xyz[2].LoadBitmap(IDB_BUTTON_XYZ_2);
	bmp_button_xyz[3].LoadBitmap(IDB_BUTTON_XYZ_3);

	bmp_button_rgb[0].LoadBitmap(IDB_BUTTON_RGB_R);
	bmp_button_rgb[1].LoadBitmap(IDB_BUTTON_RGB_G);
	bmp_button_rgb[2].LoadBitmap(IDB_BUTTON_RGB_B);
	bmp_button_rgb[3].LoadBitmap(IDB_BUTTON_RGB);

	bmp_button_paint.LoadBitmap(IDB_BUTTON_PAINT);
	bmp_button_calc.LoadBitmap(IDB_BUTTON_CALC);
	bmp_button_mark.LoadBitmap(IDB_BUTTON_MARK);
	bmp_button_work_area.LoadBitmap(IDB_BUTTON_WORK_AREA);
	bmp_button_mask.LoadBitmap(IDB_BUTTON_MASK);
	bmp_button_histgram.LoadBitmap(IDB_BUTTON_HISTGRAM);

	bmp_button_work_area_edit.LoadBitmap(IDB_BUTTON_WORK_AREA_EDIT);
	bmp_button_mark_edit.LoadBitmap(IDB_BUTTON_MARK_EDIT);
	bmp_button_calc_distance.LoadBitmap(IDB_BUTTON_CALC_DISTANCE);
	bmp_button_calc_distance2.LoadBitmap(IDB_BUTTON_CALC_DISTANCE2);

	bmp_button_scale_adjust_field.LoadBitmap(IDB_BUTTON_SCALE_ADJUST_FIELD);
	bmp_button_no_scale.LoadBitmap(IDB_BUTTON_NO_SCALE);

	bmp_button_video.LoadBitmap(IDB_BUTTON_VIDEO);
	bmp_button_video2.LoadBitmap(IDB_BUTTON_VIDEO2);

	bmp_button_inc.LoadBitmap(IDB_BUTTON_INCLIMENT);
	bmp_button_dec.LoadBitmap(IDB_BUTTON_DECLIMENT);
}

/********************************************************************
機  能  名  称 : ボタンを配置する
関    数    名 : arrange_buttons
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::arrange_buttons()
{
	//表示データの種類選択ボタン（マルチチャネル画像および動画のときに使う）
	CButton	*p_button_d0 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0);
	CButton	*p_button_d1 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1);
	CButton	*p_button_d2 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT2);
	CButton	*p_button_d3 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT3);

	p_button_d0->MoveWindow( API.main_window_size.cx-35 , DATA_SELECT_FIELD_Y      , 30 , 30);
	p_button_d1->MoveWindow( API.main_window_size.cx-35 , DATA_SELECT_FIELD_Y + 30 , 30 , 30);
	p_button_d2->MoveWindow( API.main_window_size.cx-35 , DATA_SELECT_FIELD_Y + 60 , 30 , 30);
	p_button_d3->MoveWindow( API.main_window_size.cx-35 , DATA_SELECT_FIELD_Y + 90 , 30 , 30);

	//各種機能の表示・編集ボタン
	CButton	*p_button_p0 = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_MARK);//マーク表示
	CButton	*p_button_p1 = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_WORK_AREA);//作業領域表示
	CButton	*p_button_p2 = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_MASK);//マスク表示
	
	CButton	*p_button_t0 = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_HISTGRAM);//ヒストグラム表示
	CButton *p_button_t1 = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE);//2点間の距離測定ツール
	CButton *p_button_t2 = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE2);//2点間の距離測定ツールをグラフと連動させる
	CButton	*p_button_t3 = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_PAINT);//フリーハンドで画像を編集
	CButton	*p_button_t4 = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC);//数式で画像を生成



	p_button_p0->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 0  , 30 , 30);
	p_button_p1->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 30  , 30 , 30);
	p_button_p2->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 60  , 30 , 30);

	p_button_t0->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 100 , 30 , 30);
	p_button_t1->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 130  , 30 , 30);
	p_button_t2->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 160  , 30 , 30);
	p_button_t3->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 190  , 30 , 30);
	p_button_t4->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 220  , 30 , 30);

	//表示画像のスケール変更機能
	CEdit	*p_edit_scale = (CEdit*)GetDlgItem(IDC_EDIT_PIMPON_DLG_SCALE);
	CButton	*p_button_scale_dbl = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_DBL);
	CButton	*p_button_scale_hlf = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_HLF);
	CButton	*p_button_scale_adjust_filed = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_ADJUST_FILED);
	CButton	*p_button_no_scale = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_NO_SCALE);

	p_edit_scale->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 340  , 30 , 20);
	p_button_scale_dbl->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 360  , 15 , 15);
	p_button_scale_hlf->MoveWindow( API.main_window_size.cx-35+15 , LAYER_SELECT_FIELD_Y + 360  , 15 , 15);
	p_button_scale_adjust_filed->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 375  , 30 , 20);
	p_button_no_scale->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 395  , 30 , 20);

	//ビデオキャプチャ
	CButton	*p_chk_video = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE);
	p_chk_video->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 430  , 30 , 30);
	

	CButton	*p_chk_video2 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2);
	p_chk_video2->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 460  , 30 , 30);

	if (API.IsCaptureOpened() )
	{
		p_chk_video->ShowWindow(SW_SHOW);
		p_chk_video2->ShowWindow(SW_SHOW);
	}
	else {
		p_chk_video->ShowWindow(SW_HIDE);
		p_chk_video2->ShowWindow(SW_HIDE);
	}
	
}

/********************************************************************
機  能  名  称 : 特定のボタンとビットマップを関連づける
関    数    名 : set_bitmap_button
戻    り    値 : HBITMAP	h_bmp	(in)ビットマップハンドル
				 UINT		id		(in)ボタンのダイアログID
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
HBITMAP CPIMPOMDlg::set_bitmap_button(HBITMAP h_bmp, UINT id)
{
	CButton	*p_button = (CButton*)GetDlgItem(id);
	return	p_button->SetBitmap( h_bmp );
}

/********************************************************************
機  能  名  称 : すべてのボタンにビットマップを関連づける
関    数    名 : set_bitmap_for_layer_button
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::set_bitmap_for_layer_button()
{
	set_bitmap_button( (HBITMAP)bmp_button_paint, IDC_BUTTON_PIMPON_DLG_PAINT);
	set_bitmap_button( (HBITMAP)bmp_button_calc, IDC_BUTTON_PIMPON_DLG_CALC);
	set_bitmap_button( (HBITMAP)bmp_button_mark, IDC_BUTTON_PIMPON_DLG_MARK );
	set_bitmap_button( (HBITMAP)bmp_button_work_area, IDC_BUTTON_PIMPON_DLG_WORK_AREA );
	set_bitmap_button( (HBITMAP)bmp_button_mask, IDC_BUTTON_PIMPON_DLG_MASK );
	set_bitmap_button( (HBITMAP)bmp_button_histgram, IDC_BUTTON_PIMPON_DLG_HISTGRAM );
	set_bitmap_button( (HBITMAP)bmp_button_calc_distance, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE);
	set_bitmap_button( (HBITMAP)bmp_button_calc_distance2, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE2);
	set_bitmap_button( (HBITMAP)bmp_button_scale_adjust_field, IDC_BUTTON_PIMPON_DLG_SCALE_ADJUST_FILED);
	set_bitmap_button( (HBITMAP)bmp_button_no_scale, IDC_BUTTON_PIMPON_DLG_NO_SCALE);
	set_bitmap_button( (HBITMAP)bmp_button_video, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE);
	set_bitmap_button( (HBITMAP)bmp_button_video2, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2); 
	set_bitmap_button((HBITMAP)bmp_button_inc, IDC_BUTTON_PIMPON_DLG_SCALE_DBL);
	set_bitmap_button((HBITMAP)bmp_button_dec, IDC_BUTTON_PIMPON_DLG_SCALE_HLF);
}

/********************************************************************
機  能  名  称 : 画像データに応じてボタンに関連づけるビットマップを変える
関    数    名 : set_bitmap_for_data_button
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::set_bitmap_for_data_button()
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中の画像のDataUnitを取得する
	CButton	*p_button_d0 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0);
	CButton	*p_button_d1 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1);
	CButton	*p_button_d2 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT2);
	CButton	*p_button_d3 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT3);
	


		if(p_du == NULL){//データが無ければ全てのデータ選択ボタンを非表示にする
			p_button_d0->ShowWindow( SW_HIDE );
			p_button_d1->ShowWindow( SW_HIDE );
			p_button_d2->ShowWindow( SW_HIDE );
			p_button_d3->ShowWindow( SW_HIDE );

		}else{
			switch( p_du->DataType ){
				case BYTE_FORMAT:
					//not break
				case SHORT16_FORMAT:
					//not break
				case WORD_FORMAT:
					//not break
				case LONG32_FORMAT:
					//not break
				case DWORD_FORMAT:
					//not break
				case FLOAT_FORMAT:
					p_button_d0->ShowWindow( SW_HIDE );
					p_button_d1->ShowWindow( SW_HIDE );
					p_button_d2->ShowWindow( SW_HIDE );
					p_button_d3->ShowWindow( SW_HIDE );
					break;

				case THREE_D_FORMAT:
					//not break
				case FLOAT_3D_FORMAT:
					//not break
				case RGB_3D_FORMAT:
					p_button_d0->ShowWindow( SW_SHOW );
					p_button_d1->ShowWindow( SW_SHOW );
					p_button_d2->ShowWindow( SW_SHOW );
					p_button_d3->ShowWindow( SW_SHOW );
					set_bitmap_button( (HBITMAP)bmp_button_3d[0] , IDC_CHECK_PIMPON_DLG_DATA_SELECT0);
					set_bitmap_button( (HBITMAP)bmp_button_3d[1] , IDC_CHECK_PIMPON_DLG_DATA_SELECT1);
					set_bitmap_button( (HBITMAP)bmp_button_3d[2] , IDC_CHECK_PIMPON_DLG_DATA_SELECT2);
					set_bitmap_button( (HBITMAP)bmp_button_3d[3] , IDC_CHECK_PIMPON_DLG_DATA_SELECT3);
					m_toolTip.UpdateTipText( "連続表示", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0));
					m_toolTip.UpdateTipText("表示ページを一つ進める", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1) );
					m_toolTip.UpdateTipText("表示ページを一戻す", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT2) );
					m_toolTip.UpdateTipText("連続表示を止める", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT3) );
					
						if( API.GetBGProcID()==BGPROC_MOVIE )		set_data_select_button(0);//ビデオ再生中
						else										set_data_select_button(3);//ビデオ停止中
					break;		

				case COMPLEX_FORMAT:
					p_button_d0->ShowWindow( SW_SHOW );
					p_button_d1->ShowWindow( SW_SHOW );
					p_button_d2->ShowWindow( SW_SHOW );
					p_button_d3->ShowWindow( SW_SHOW );
					set_bitmap_button( (HBITMAP)bmp_button_complex[0] , IDC_CHECK_PIMPON_DLG_DATA_SELECT0);
					set_bitmap_button( (HBITMAP)bmp_button_complex[1] , IDC_CHECK_PIMPON_DLG_DATA_SELECT1);
					set_bitmap_button( (HBITMAP)bmp_button_complex[2] , IDC_CHECK_PIMPON_DLG_DATA_SELECT2);
					set_bitmap_button( (HBITMAP)bmp_button_complex[3] , IDC_CHECK_PIMPON_DLG_DATA_SELECT3);
					set_data_select_button( p_du->DispChannel );
					m_toolTip.UpdateTipText("実部", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0) );
					m_toolTip.UpdateTipText("虚部", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1) );
					m_toolTip.UpdateTipText("絶対値", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT2) );
					m_toolTip.UpdateTipText("偏角", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT3));
					break;

				case XY_FORMAT:
					p_button_d0->ShowWindow( SW_SHOW );
					p_button_d1->ShowWindow( SW_SHOW );
					p_button_d2->ShowWindow( SW_HIDE );
					p_button_d3->ShowWindow( SW_HIDE );
					set_bitmap_button( (HBITMAP)bmp_button_xy[0] , IDC_CHECK_PIMPON_DLG_DATA_SELECT0);
					set_bitmap_button( (HBITMAP)bmp_button_xy[1] , IDC_CHECK_PIMPON_DLG_DATA_SELECT1);
					set_data_select_button( p_du->DispChannel );
					m_toolTip.UpdateTipText("X成分", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0));
					m_toolTip.UpdateTipText("Y成分", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1));
					break;
	
				case XYZ_FORMAT:
					p_button_d0->ShowWindow( SW_SHOW );
					p_button_d1->ShowWindow( SW_SHOW );
					p_button_d2->ShowWindow( SW_SHOW );
					p_button_d3->ShowWindow( SW_SHOW );
					set_bitmap_button( (HBITMAP)bmp_button_xyz[0] , IDC_CHECK_PIMPON_DLG_DATA_SELECT0);
					set_bitmap_button( (HBITMAP)bmp_button_xyz[1] , IDC_CHECK_PIMPON_DLG_DATA_SELECT1);
					set_bitmap_button( (HBITMAP)bmp_button_xyz[2] , IDC_CHECK_PIMPON_DLG_DATA_SELECT2);
					set_bitmap_button( (HBITMAP)bmp_button_xyz[3] , IDC_CHECK_PIMPON_DLG_DATA_SELECT3);
					set_data_select_button( p_du->DispChannel );
					m_toolTip.UpdateTipText("X成分", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0) );
					m_toolTip.UpdateTipText("Y成分", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1) );
					m_toolTip.UpdateTipText("Z成分", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT2) );
					m_toolTip.UpdateTipText("点群", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT3) );
					break;

				case RGB_FORMAT:
					p_button_d0->ShowWindow( SW_SHOW );
					p_button_d1->ShowWindow( SW_SHOW );
					p_button_d2->ShowWindow( SW_SHOW );
					p_button_d3->ShowWindow( SW_SHOW );
					set_bitmap_button( (HBITMAP)bmp_button_rgb[0] , IDC_CHECK_PIMPON_DLG_DATA_SELECT0);
					set_bitmap_button( (HBITMAP)bmp_button_rgb[1] , IDC_CHECK_PIMPON_DLG_DATA_SELECT1);
					set_bitmap_button( (HBITMAP)bmp_button_rgb[2] , IDC_CHECK_PIMPON_DLG_DATA_SELECT2);
					set_bitmap_button( (HBITMAP)bmp_button_rgb[3] , IDC_CHECK_PIMPON_DLG_DATA_SELECT3);
					m_toolTip.UpdateTipText("赤成分", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0) );
					m_toolTip.UpdateTipText("緑成分", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1) );
					m_toolTip.UpdateTipText("青成分", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT2) );
					m_toolTip.UpdateTipText("RGBカラー", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT3) );
						if( p_du->DispRGB == false )	set_data_select_button( p_du->DispChannel );//色成分表示
						else							set_data_select_button( 3 );			//RGBカラー表示
					break;
			}
		}
}

/********************************************************************
機  能  名  称 : 画像データの種類に応じてボタンの押下状態を変更する
関    数    名 : set_data_select_button
戻    り    値 : long type
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::set_data_select_button(long type)
{
	int i;
	bool	do_disp[4];

		for(i=0 ; i<4 ; i++)	do_disp[i] = false;
	do_disp[ type ] = true;	

	API.SetCheck(m_hWnd, IDC_CHECK_PIMPON_DLG_DATA_SELECT0, do_disp[0] );
	API.SetCheck(m_hWnd, IDC_CHECK_PIMPON_DLG_DATA_SELECT1, do_disp[1] );
	API.SetCheck(m_hWnd, IDC_CHECK_PIMPON_DLG_DATA_SELECT2, do_disp[2] );
	API.SetCheck(m_hWnd, IDC_CHECK_PIMPON_DLG_DATA_SELECT3, do_disp[3] );
}

/********************************************************************
機  能  名  称 : 表示データ選択ボタンが押された
関    数    名 : OnPimponDlgDataSelect
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnPimponDlgDataSelect0() 
{
	set_data_select_button(0);//表示データ選択ボタンのチェックの状態変更
	change_disp_data_by_button(0);//表示データの変更
}

void CPIMPOMDlg::OnPimponDlgDataSelect1() 
{
	set_data_select_button(1);//表示データ選択ボタンのチェックの状態変更
	change_disp_data_by_button(1);//表示データの変更
}

void CPIMPOMDlg::OnPimponDlgDataSelect2() 
{
	set_data_select_button(2);//表示データ選択ボタンのチェックの状態変更
	change_disp_data_by_button(2);//表示データの変更
}

void CPIMPOMDlg::OnPimponDlgDataSelect3() 
{
	set_data_select_button(3);//表示データ選択ボタンのチェックの状態変更
	change_disp_data_by_button(3);//表示データの変更
}

/********************************************************************
機  能  名  称 : 押されたボタンに応じて表示画像を変更して再描画
関    数    名 : change_disp_data_by_button
戻    り    値 : short button_number  (in)押されたボタンの番号
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::change_disp_data_by_button(short button_number)
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中の画像のDataUnitを取得する

		if(p_du == NULL)	return;//データが無ければ何もしない

		switch( p_du->DataType ){
			case BYTE_FORMAT:
				//not break;
			case SHORT16_FORMAT:
				//not break;
			case WORD_FORMAT:
				//not break;
			case LONG32_FORMAT:
				//not break;
			case DWORD_FORMAT:
				//not break;
			case FLOAT_FORMAT:
				break;

			case THREE_D_FORMAT:
				//not break;
			case FLOAT_3D_FORMAT:
				//not break;
			case RGB_3D_FORMAT:
					switch( button_number ){
						case 0://連続再生
							API.SetBGProcID(BGPROC_MOVIE);
							break;
						case 1://コマ送り
							p_du->MoveDispPage( 1 );
							API.DrawImage(CURRENT_IMAGE, true);
							break;
						case 2://逆コマ送り
							p_du->MoveDispPage( -1 );
							API.DrawImage(CURRENT_IMAGE, true);
							break;
						case 3://再生中止
							if(API.GetBGProcID()==BGPROC_MOVIE){
								API.StopBGProc();
							}
							break;
					}
				break;

			case COMPLEX_FORMAT:
				//not break
			case XY_FORMAT:
				p_du->SetDispChannel(button_number);
				API.DrawImage(CURRENT_IMAGE, true);
				break;

			case XYZ_FORMAT:
					if( button_number !=3 ){
						p_du->SetDispChannel(button_number);

					}else{
						CPointCloudDlg	pcd;
						pcd.CreatePointCloud(p_du);//3次元モデル表示
					}
				API.DrawImage(CURRENT_IMAGE, true);
				break;

			case RGB_FORMAT:
					if( button_number !=3 ){
						p_du->SetDispChannel(button_number);

					}else{
						p_du->SetDispChannel(-1);
					}
				API.DrawImage(CURRENT_IMAGE, true);
				break;
		}
}

/********************************************************************
機  能  名  称 : フリーハンドで画像を編集
関    数    名 : OnPimponDlgButtonPaint
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonPaint() 
{
	if (API.GetCheck(m_hWnd, IDC_BUTTON_PIMPON_DLG_PAINT)) {
		API.DialogOpen(IDD_DATAPICK_DLG, true);
	}else {
		API.DialogOpen(IDD_DATAPICK_DLG, false);
	}
}

/********************************************************************
機  能  名  称 : 数式で画像を作成
関    数    名 : OnPimponDlgButtonCalc
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonCalc() 
{
	if (API.GetCheck(m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC)) {
		API.DialogOpen(IDD_CALC_DLG, true);
	}
	else {
		API.DialogOpen(IDD_CALC_DLG, false);
	}
}

/********************************************************************
機  能  名  称 : 作業領域の表示ボタンが押された
関    数    名 : OnPimponDlgButtonWorkArea
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonWorkArea() 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中のDataUnitを取得

	if(p_du)//選択中の画像メモリがあれば，作業領域の表示状態を変更する
	{
			if( API.GetCheck( m_hWnd, IDC_BUTTON_PIMPON_DLG_WORK_AREA ) )//作業領域を使用できるようにするとき
			{
				p_du->DoUseWorkArea = true;//
				if(API.pWorkAreaDlg){
					API.pWorkAreaDlg->ShowWindow(SW_SHOW);//作業領域ウィンドうを表示
				}
			}
			else//作業領域を使用できないようにするとき
			{
				p_du->DoUseWorkArea = false;//
				if(API.pWorkAreaDlg){
					API.pWorkAreaDlg->ShowWindow(SW_HIDE);//作業領域ウィンドうを非表示
				}
				API.SetWorkAreaEditMode(0);
			}
		API.DrawImage( CURRENT_IMAGE , true );//作業領域内の表示データを更新して再描画する
	}
	else//画像メモリが無ければ何もしない
	{
		if(API.pWorkAreaDlg){
			API.pWorkAreaDlg->ShowWindow(SW_HIDE);//作業領域ウィンドうを非表示
		}
		return;
	}
}



/********************************************************************
機  能  名  称 : マークの表示ボタンが押された
関    数    名 : OnPimponDlgButtonMark
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonMark() 
{
	if( API.GetCheck( m_hWnd, IDC_BUTTON_PIMPON_DLG_MARK )){
		API.SetMarkEditMode(1);
	}else{
		API.SetMarkEditMode(0);
	}
}



/********************************************************************
機  能  名  称 : ヒストグラムの表示ボタンが押された
関    数    名 : OnBnClickedButtonPimponDlgHistgram
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnBnClickedButtonPimponDlgHistgram()
{
	if(API.GetCheck(m_hWnd,IDC_BUTTON_PIMPON_DLG_HISTGRAM)){
		API.DialogOpen(IDD_HISTGRAM_DLG,true);
	}else{
		API.DialogOpen(IDD_HISTGRAM_DLG,false);
	}
}

/********************************************************************
機  能  名  称 : 2点間の距離計算ボタンが押された
関    数    名 : OnPimponDlgButtonRulerTool
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonRulerTool() 
{
	API.SetRulerToolMode(API.GetCheck( m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE ));
}

/********************************************************************
機  能  名  称 : 2点間の距離計算とグラフを連動させるボタンが押された
関    数    名 : OnBnClickedButtonPimponDlgCalcDistance2
戻    り    値 : 
機          能 : 2点間の距離計算をONにし、グラフウィンドウを表示する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnBnClickedButtonPimponDlgCalcDistance2()
{
	if(API.GetCheck(m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE2) ){
		API.SetCheck(m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE,true);
		API.DialogOpen(IDD_CHART_DLG, true);
	}
}

/********************************************************************
機  能  名  称 : マスク表示ボタンが押された
関    数    名 : OnPimponDlgButtonMask
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonMask() 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中のDataUnitを取得

		if(p_du == NULL)		return;//データが無ければなにもしない

	p_du->DoUseMask = API.GetCheck( m_hWnd, IDC_BUTTON_PIMPON_DLG_MASK );//マスク
	API.DrawImage( CURRENT_IMAGE, true);//再描画
}


/********************************************************************
機  能  名  称 : 表示画像の倍率×2ボタンが押された
関    数    名 : OnPimponDlgButtonScaleDbl
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
			 Y.Ikeda		 現在表示されている中心座標を保ったまま拡大率変更
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonScaleDbl() 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du == NULL)	return;//現在選択中の画像メモリにデータが無ければなにもしない

	API.ScaleImage(CURRENT_IMAGE, p_du->DispScale*2);
}

/********************************************************************
機  能  名  称 : 表示画像の倍率/2ボタンが押された
関    数    名 : OnPimponDlgButtonScaleHlf
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
			 Y.Ikeda		 現在表示されている中心座標を保ったまま拡大率変更
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonScaleHlf() 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );
	if(p_du == NULL)	return;//現在選択中の画像メモリにデータが無ければなにもしない

	API.ScaleImage(CURRENT_IMAGE, p_du->DispScale/2.0);
}

/********************************************************************
機  能  名  称 : 表示画像をメイン画面の大きさにあわすボタンが押された
関    数    名 : OnPimponDlgButtonScaleAdjustFiled
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonScaleAdjustFiled() 
{
	CDataUnit	*p_du;

	p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du == NULL)	return;//現在選択中の画像メモリにデータが無ければなにもしない

	p_image_disp_main->DrawAdjustedImage(p_du, true);//縮尺変更
	DrawImage(true);//再描画
}

/********************************************************************
機  能  名  称 : 表示画像をフルスケール表示するボタンが押された
関    数    名 : OnPimponDlgButtonNoScale
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
			 Y.Ikeda		 現在表示されている中心座標を保ったまま拡大率変更
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonNoScale() 
{
	CDataUnit	*p_du;

	p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du == NULL)	return;//現在選択中の画像メモリにデータが無ければなにもしない

	//p_du->SetDispScale( 1 );//縮尺変更
	p_image_disp_main->ScaleImage(p_du,1);

	DrawImage(true);//再描画	
}

/********************************************************************
機  能  名  称 : ビデオキャプチャースルー表示ボタンが押された
関    数    名 : OnBnClickedCheckVideoCapture
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnBnClickedCheckVideoCapture()
{
	if(API.GetCheck(m_hWnd,IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE)){
		API.SetVideoCaptureThroughMode(1);
	}else{
		API.SetVideoCaptureThroughMode(0);
	}
}

/********************************************************************
機  能  名  称 : ビデオキャプチャー +バッチ実行　ボタンが押された
関    数    名 : OnBnClickedPimponDlgCheckVideoCapture2
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnBnClickedPimponDlgCheckVideoCapture2()
{
	if(API.GetCheck(m_hWnd,IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2)){
		API.SetVideoCaptureThroughMode(2);
	}else{
		API.SetVideoCaptureThroughMode(0);
	}
}

/********************************************************************
機  能  名  称 : ポイント座標値を表示するためのダイアログアイテムの配置
関    数    名 : arrange_data_value_disp_field
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::arrange_data_value_disp_field()
{
	CSize	picture(60,60) , value_disp(60,15) , coor_disp(200, 15); 

	//1点周辺の画像を表示するためのピクチャーボックスを配置する
	CWnd	*p_wnd_picture = GetDlgItem( IDC_PICT_PIMPON_DLG_DISP_PIECE );
	p_wnd_picture->MoveWindow( DATA_VALUE_DISP_FIELD_X, API.main_window_size.cy-140, picture.cx, picture.cy );

	//1点の画素のデータ値を表示するためのエディットボックスを配置する
	CWnd	*p_wnd_value_1_1 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE_1_1 );
	CWnd	*p_wnd_value0_1 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE0_1 );
	CWnd	*p_wnd_value1_1 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE1_1 );
	CWnd	*p_wnd_value_10 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE_10 );
	CWnd	*p_wnd_value00 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE00 );
	CWnd	*p_wnd_value10 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE10 );
	CWnd	*p_wnd_value_11 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE_11 );
	CWnd	*p_wnd_value01 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE01 );
	CWnd	*p_wnd_value11 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE11 );

	p_wnd_value_1_1->MoveWindow(DATA_VALUE_DISP_FIELD_X + picture.cx + 10						, API.main_window_size.cy-140 + value_disp.cy	, value_disp.cx , value_disp.cy);
	p_wnd_value0_1->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10 + (value_disp.cx+5)	, API.main_window_size.cy-140 + value_disp.cy	, value_disp.cx , value_disp.cy);
	p_wnd_value1_1->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10 + (value_disp.cx+5)*2	, API.main_window_size.cy-140 + value_disp.cy	, value_disp.cx , value_disp.cy);
	p_wnd_value_10->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10						, API.main_window_size.cy-140 + value_disp.cy*2	, value_disp.cx , value_disp.cy);
	p_wnd_value00->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10 + (value_disp.cx+5)	, API.main_window_size.cy-140 + value_disp.cy*2	, value_disp.cx , value_disp.cy);
	p_wnd_value10->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10 + (value_disp.cx+5)*2	, API.main_window_size.cy-140 + value_disp.cy*2	, value_disp.cx , value_disp.cy);
	p_wnd_value_11->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10						, API.main_window_size.cy-140 + value_disp.cy*3	, value_disp.cx , value_disp.cy);
	p_wnd_value01->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10 + (value_disp.cx+5)	, API.main_window_size.cy-140 + value_disp.cy*3	, value_disp.cx , value_disp.cy);
	p_wnd_value11->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10 + (value_disp.cx+5)*2	, API.main_window_size.cy-140 + value_disp.cy*3	, value_disp.cx , value_disp.cy);

	//1点の座標を表示するためのエディットボックスを配置する
	CWnd	*p_wnd_coor_ij = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_COOR_IJ );
	CWnd	*p_wnd_coor_xy = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_COOR_XY );
	p_wnd_coor_ij->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10 , API.main_window_size.cy-140	, coor_disp.cx , coor_disp.cy);

	//クリック座標固定ボタン
	CWnd	*p_wnd_filx_coor = (CWnd*)GetDlgItem( IDC_CHECK_PIMPON_DLG_FIX_COOR );
	p_wnd_filx_coor->MoveWindow( DATA_VALUE_DISP_FIELD_X, API.main_window_size.cy-140 + picture.cy + 5, 150, 15 );
	
	
	//サブピクセル座標表示ボタン
	CWnd	*p_wnd_subpix_coor = (CWnd*)GetDlgItem( IDC_CHECK_PIMPON_DLG_SUBPIX );
	p_wnd_subpix_coor->MoveWindow( DATA_VALUE_DISP_FIELD_X + 150, API.main_window_size.cy-140 + picture.cy + 5, 120, 15 );
}


/********************************************************************
機  能  名  称 : メッセージを表示するためのダイアログアイテムの配置
関    数    名 : arrange_message_disp_field
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::arrange_message_disp_field()
{
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem( IDC_TAB_PIMPOM_DLG_MESSAGE );
	CWnd	*p_wnd_message = (CWnd*)GetDlgItem( IDC_EDIT_PIMPOM_DLG_MESSAGE );

	p_wnd_tab->MoveWindow(MESSAGE_DISP_FIELD_X , API.main_window_size.cy-150, API.main_window_size.cx - 340 , MESSAGE_DISP_FIELD_H);
	p_wnd_message->MoveWindow(MESSAGE_DISP_FIELD_X+5 , API.main_window_size.cy-150+23, API.main_window_size.cx - 340 -10 , MESSAGE_DISP_FIELD_H-25);
}


/********************************************************************
機  能  名  称 : 指定座標含む周囲9画素の値を表示する
関    数    名 : DispDataValue 
戻    り    値 : CPoint src_coor (in)
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::DispDataValue(double point_x, double point_y )
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中の画像のDataUnitを取得する
	long		disp_piece_width = 5;//指定した座標の周囲の拡大表示する領域の幅
	int			dlgid[3][3] = { { IDC_EDIT_PIMPON_DLG_VALUE_1_1, IDC_EDIT_PIMPON_DLG_VALUE0_1, IDC_EDIT_PIMPON_DLG_VALUE1_1 },
								{ IDC_EDIT_PIMPON_DLG_VALUE_10, IDC_EDIT_PIMPON_DLG_VALUE00, IDC_EDIT_PIMPON_DLG_VALUE10 },
								{ IDC_EDIT_PIMPON_DLG_VALUE_11, IDC_EDIT_PIMPON_DLG_VALUE01, IDC_EDIT_PIMPON_DLG_VALUE11 }
	};
	CString		coor_xy, coor_ij;
	int			i,j;
	CPoint		src_coor((int)point_x, (int)point_y);

		if(p_du == NULL)
		{//データが無いときは全てに-を表示する
			for (j = 0; j < 3; j++){
				for (i = 0; i < 3; i++){
					API.SetEditValue(m_hWnd, dlgid[j][i], _T("-"));
				}
			}

		}else{
				//画像転送元の座標での表示可能な領域を表す矩形を決める
				CRect	src_disp_able_rect = CRect(0, 0, p_du->DataSize.cx - 1, p_du->DataSize.cy -1 ) 
					+ CRect( -disp_piece_width/2, -disp_piece_width/2, -disp_piece_width/2, -disp_piece_width/2);


				//9点のデータを表示する
				for (j = 0; j < 3; j++){
					for (i = 0; i < 3; i++){
						CPoint point = src_coor + CPoint(i - 1, j - 1);

						if (point.x < 0 || point.x >= p_du->DataSize.cx || point.y < 0 || point.y >= p_du->DataSize.cy){
							API.SetEditValue(m_hWnd, dlgid[j][i], _T("-"));
						}
						else{
							float val = (float)p_du->GetDataValue(point, -1, -1);
							API.SetEditValue(m_hWnd, dlgid[j][i], val);
						}
					}
				}

				//指定した座標（画像転送元）の周囲を拡大表示する
				p_image_disp_piece->DrawPiece(p_du, src_coor - CPoint(disp_piece_width/2, disp_piece_width/2)
					, CSize(disp_piece_width, disp_piece_width) , true , false , false);

				//指定した座標（画像転送元）の座標を表示する
				if(API.GetCheck(m_hWnd,IDC_CHECK_PIMPON_DLG_SUBPIX) )//サブピクセル表示
				{
					coor_ij.Format("(x,y)=(%0.3lf, %0.3lf)", point_x, point_y );//画像データの左上を原点とした場合の座標（i,j）
				}else{
					coor_ij.Format("(x,y)=(%d, %d)", (int)point_x, (int)point_y );//画像データの左上を原点とした場合の座標（i,j）
				}
				API.SetEditValue( m_hWnd, IDC_EDIT_PIMPON_DLG_COOR_IJ , coor_ij );
				API.SetEditValue( m_hWnd, IDC_EDIT_PIMPON_DLG_COOR_XY , coor_xy );
		}

}



/********************************************************************
機  能  名  称 : 作業領域を描く
関    数    名 : draw_work_area 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::draw_work_area()
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中の画像のDataUnitを取得する
	if(p_du == NULL){
		if(API.pWorkAreaDlg){
			API.pWorkAreaDlg->ShowWindow(SW_HIDE);//作業領域ウィンドうを非表示
		}
		return;//データが無ければ何もしない
	}


	if( p_du->DoUseWorkArea )//作業領域を使う場合
	{
		API.pWorkAreaDlg->ShowWindow(SW_SHOW);//作業領域ウィンドうを非表示
		API.pWorkAreaDlg->OnDrawOnImage();
	}
	else//作業領域を使わない場合
	{
		//作業領域ウィンドウを非表示
		if(API.pWorkAreaDlg){
			API.pWorkAreaDlg->ShowWindow(SW_HIDE);
		}
	}

	API.SetCheck( m_hWnd , IDC_BUTTON_PIMPON_DLG_WORK_AREA , p_du->DoUseWorkArea );//作業領域の使用/不使用を決める
}



/********************************************************************
機  能  名  称 : マーク表示
関    数    名 : draw_marks 
戻    り    値 : 
機          能 : マーク上に十字カーソルを描く
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::draw_marks()
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中の画像のDataUnitを取得する
	CString		str;
	long		mark_length = 20;//マークのラインの長さ


	if(p_du == NULL){//データが無ければ何もしない
		if(API.pMarkDlg){//マークウィンドウ非表示
			API.pMarkDlg->ShowWindow(SW_HIDE);
		}
		return;
	}

	if (API.GetMarkEditMode()>0)
	{//マーク使用時
		API.pMarkDlg->ShowWindow(SW_SHOW);
		API.pMarkDlg->OnDrawOnImage();
	}
	else 
	{//マーク不使用時
		API.pMarkDlg->ShowWindow(SW_HIDE);
	}
	
}

/********************************************************************
機  能  名  称 : 2点の距離を計算し、表示する
関    数    名 : draw_2points_distance 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::draw_2points_distance()
{
	float	value1, value2, value_dif, distance, di, dj;
	CDC			*p_dc;
	CRect		dest_rect;
	CString		s_coor_i, s_coor_j, s_value, s_distance, s_index;
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中の画像のDataUnitを取得する
	double		dest_point1x, dest_point1y, dest_point2x, dest_point2y;

		if(p_du == NULL)	return	;//選択中の画像メモリにデータが無ければここから後は何もしない
		if( !API.GetCheck( m_hWnd , IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE) )		return;//距離を計算するチェックボタンが押されていることを確認

	p_dc = GetDC();//デバイスコンテキストを取得する
	p_dc->SetROP2( R2_NOT );
	p_image_disp_main->GetDestRect(p_du, &dest_rect);//画像転送先の矩形領域

	p_image_disp_main->GetDestCoor( p_du , calc_begin_x, calc_begin_y , &dest_point1x , &dest_point1y );//画像転送先での座標を取得

	p_image_disp_main->GetDestCoor( p_du , calc_end_x, calc_end_y , &dest_point2x , &dest_point2y );//画像転送先での座標を取得

	API.DrawClippedSegment(p_dc, dest_rect, dest_point1x, dest_point1y, dest_point2x , dest_point2y);//線を描く

	ReleaseDC(p_dc);//デバイスコンテキストを開放する

	//メッセージボックスに表示する
	s_index = _T("     始点      終点       差");
	ShowMessage(false, 0, false, s_index);

	di = (float)(calc_end_x - calc_begin_x);
	s_coor_i = _T("x:  ") + API.ToString((float)calc_begin_x,10) + API.ToString((float)calc_end_x,10) + API.ToString(di,10);
	ShowMessage(false, 0, false, s_coor_i);

	dj = (float)(calc_end_y - calc_begin_y);
	s_coor_j = _T("y:  ") + API.ToString((float)calc_begin_y,10) + API.ToString((float)calc_end_y,10) + API.ToString(dj,10);
	ShowMessage(false, 0, false, s_coor_j);

	value1 = (float)p_du->GetDataValue( CPoint(calc_begin_x, calc_begin_y),-1,-1);
	value2 = (float)p_du->GetDataValue( CPoint(calc_end_x, calc_end_y) ,-1,-1);
	value_dif = value2 - value1;
	s_value = _T("値: ") + API.ToString(value1, 8) + _T("  ") + API.ToString(value2, 8) + _T("  ") + API.ToString(value_dif, 6);
	ShowMessage(false, 0, false, s_value);

	distance = (float)sqrt( (double)(di*di)  +   (double)(dj*dj) );
	s_distance = _T("距離: ") + API.ToString(distance,10);
	ShowMessage(false, 0, false, s_distance );



	if( API.GetCheck(m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE2) ){//グラフ描画を連動させる
		if(distance>1){
			int dataCnt=0;
			float dataVal[CHART_DATA_NUM]={0};
			float dx,dy;
			dx = (float)(calc_end_x - calc_begin_x) / distance;
			dy = (float)(calc_end_y - calc_begin_y) / distance;
			for(float pos=0 ; pos<=distance && dataCnt<CHART_DATA_NUM; pos++, dataCnt++)
			{
				float x = calc_begin_x + pos * dx;
				float y = calc_begin_y + pos * dy;

				dataVal[dataCnt] = (float)p_du->GetDataValue((double)x, (double)y ,-1,-1);
			}

			API.PlotDataToChart(CHART_NUM-1, dataCnt, dataVal, true);
		}

	}
}

/********************************************************************
機  能  名  称 : メッセージ表示領域にメッセージを表示する
関    数    名 : ShowMessage 
戻    り    値 : CString	str			(in)メッセージ
				 bool		do_clear	(in)true にしておくと前のメッセージを消す
				 int		num			(in)メッセージウィンドウ番号
				 bool		force		(in)強制的にタブを切り替えてメッセージ表示する
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::ShowMessage(bool do_clear, int num, bool force, CString str)
{
	CString	ret;
	ret.Format("%c%c", (char)CR , (char)LF);//改行コード

		if(do_clear){
			message[num] = _T("");
		}

	message[num] += str + ret;


	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem( IDC_TAB_PIMPOM_DLG_MESSAGE );//選択タブ変更

	bool dispflg = false;
	if(force)
	{//強制表示
		p_wnd_tab->SetCurSel(num);
		dispflg = true;
	}
	else
	{//該当メッセージ番号タブが選択されているときのみ表示
		if(num==p_wnd_tab->GetCurSel()){
			dispflg = true;
		}
	}


	if (dispflg)
	{
		API.SetEditValue(m_hWnd, IDC_EDIT_PIMPOM_DLG_MESSAGE, message[num]);

		//最後の文字選択(スクロールさせるため)
		CEdit	*p_wnd_edit = (CEdit*)GetDlgItem(IDC_EDIT_PIMPOM_DLG_MESSAGE);
		int lastidx = message[num].GetLength() - 1;
		p_wnd_edit->SetSel(lastidx, lastidx);
	}
}

/********************************************************************
機  能  名  称 : メッセージタブ変更
関    数    名 : OnTcnSelchangeTabPimpomDlgMessage 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnTcnSelchangeTabPimpomDlgMessage(NMHDR *pNMHDR, LRESULT *pResult)
{
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem( IDC_TAB_PIMPOM_DLG_MESSAGE );

	switch(p_wnd_tab->GetCurSel())
	{
	case 0:
		API.SetEditValue( m_hWnd , IDC_EDIT_PIMPOM_DLG_MESSAGE , message[0] );
		break;
	case 1:
		API.SetEditValue( m_hWnd , IDC_EDIT_PIMPOM_DLG_MESSAGE , message[1] );
		break;
	default:
		break;
	}
	
	*pResult = 0;
}

/********************************************************************
機  能  名  称 : バックグラウンド処理
関    数    名 : BackGroundProc 
戻    り    値 : 
機          能 : タイマー処理は動画連続表及びスルー表示に用いる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::BackGroundProc()
{
	for (;;)
	{
		switch(API.GetBGProcID())
		{
			case BGPROC_CAPTURING://カメラスルー中
				if(API.GetCheck(m_hWnd,IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2))//スルー表示＋バッチ実行
				{
					API.CaptureOne(false);	//キャプチャ

					BATCH_INPUT in_param = { 0 };
					BATCH_OUTPUT out_param = { 0 };

					API.BatchExecute(&in_param, &out_param);


				}else{//スルー表示のみ
					API.CaptureOne(true);
				}
				break;

			case BGPROC_MOVIE://動画表示中
				movie();
				break;

			case BGPROC_BATCH_EXC://バッチ処理中
				break;
		}

		Sleep(10);
	}
}

/********************************************************************
機  能  名  称 : 動画連続表示
関    数    名 : movie 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::movie()
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du == NULL){//データが無いとき
			if(API.GetBGProcID()==BGPROC_MOVIE){
				API.StopBGProc();//再生中止
			}
			return;
		}

		if( p_du->DataType != THREE_D_FORMAT	&&	p_du->DataType != FLOAT_3D_FORMAT &&  p_du->DataType != RGB_3D_FORMAT){//選択中のデータが動画ではないとき
			if(API.GetBGProcID()==BGPROC_MOVIE){
				API.StopBGProc();//再生中止
			}
			return;
		}

		if( !p_du->MoveDispPage(1) ){//最終ページまできたとき
			p_du->SetDispPage(0);//先頭ページにもどす
		}

	API.DrawImage( CURRENT_IMAGE, true );//再描画
}

/********************************************************************
機  能  名  称 : 動画連続表示中止
関    数    名 : StopMovie 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::StopMovie()
{
	if(API.GetBGProcID()==BGPROC_MOVIE){
		API.StopBGProc();//再生中止
	}
}

/********************************************************************
機  能  名  称 : ドラッグアンドドロップ
関    数    名 : OnDropFiles 
戻    り    値 : 
機          能 : メイン画面上に画像ファイルドロップされたら、選択中の画像メモリにファイルを読み込む
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnDropFiles(HDROP hDropInfo)
{
	CString		strFileName;
	UINT		size = DragQueryFile( hDropInfo, 0, NULL, 0) + 1;

	DragQueryFile( hDropInfo , 0 , strFileName.GetBuffer(size),size );
	strFileName.ReleaseBuffer();
		if( API.pDataDlg->DataExistMessage("上書きしてもよろしいですか？", CURRENT_IMAGE) )		return;

	API.BackupDataUnit(CURRENT_IMAGE);//現在のデータバックアップ

	//ファイルをロードする
		if( !API.SelectAndLoadImageFile( CURRENT_IMAGE, strFileName) ){
			return;
		}

	API.DrawImage(CURRENT_IMAGE,true,true,true);
	DragFinish(hDropInfo);
}

/********************************************************************
機  能  名  称 : バージョン情報
関    数    名 : OnHelpVersion 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnHelpVersion()
{
	CAboutDlg box;

	box.DoModal();
}


/********************************************************************
機  能  名  称 : 外部プログラムからPIMPOMに画像データを転送するメッセージ受信
関    数    名 : OnMessageExtInput 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
LRESULT CPIMPOMDlg::OnMessageExtInput(WPARAM wParam, LPARAM lParam)
{
	SHEAREMEM_PLOT_IMAGE	*pShmem;//共有メモリ領域
	SHEAREMEM_PLOT_IMAGE	shmem_header={0};
	HANDLE	hShare;//共有メモリのハンドル


	//共有メモリ領域のヘッダ部分を読む
	pShmem = (SHEAREMEM_PLOT_IMAGE*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_PLOT_IMAGE), &hShare, 0);
	if(!pShmem)	return 0;

	memcpy(&shmem_header, pShmem, sizeof(SHEAREMEM_PLOT_IMAGE));
	pimpom_plot_free_shere_mem(pShmem, hShare);//共有メモリ解放

	if(shmem_header.image_num<0 || shmem_header.image_num>=API.GetDataUnitNumber() 
		|| shmem_header.width<=0 || shmem_header.height<=0 || shmem_header.size<=0)	return 0;



	//共有メモリ領域の全体読み直す
	pShmem = (SHEAREMEM_PLOT_IMAGE*)pimpom_plot_get_shere_mem(shmem_header.size, &hShare, 0);
	if(!pShmem)	return 0;


	//データコピー実施
	bool doDrawImage = true;//再描画するかどうか
	switch(pShmem->format)
	{
		case PIMPOM_PLOT_FORMAT_BYTE_IMAGE://BYTE型メモリ
			{
				BYTE *pDstImg = API.GetByteMemory(pShmem->image_num, CSize(pShmem->width,pShmem->height), true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height);	
			}
			break;

		case PIMPOM_PLOT_FORMAT_SHORT_IMAGE://SHORT型メモリ
			{
				short *pDstImg = API.GetShortMemory(pShmem->image_num, CSize(pShmem->width,pShmem->height), true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height*sizeof(short));	
			}
			break;

		case PIMPOM_PLOT_FORMAT_WORD_IMAGE://WORD型メモリ
			{
				WORD *pDstImg = API.GetWordMemory(pShmem->image_num, CSize(pShmem->width,pShmem->height), true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height*sizeof(WORD));	
			}
			break;

		case PIMPOM_PLOT_FORMAT_LONG_IMAGE://LONG型メモリ
			{
				long *pDstImg = API.GetLongMemory(pShmem->image_num, CSize(pShmem->width,pShmem->height), true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height*sizeof(long));	
			}
			break;

		case PIMPOM_PLOT_FORMAT_DWORD_IMAGE://DWORD型メモリ
			{
				DWORD *pDstImg = API.GetDwordMemory(pShmem->image_num, CSize(pShmem->width,pShmem->height), true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height*sizeof(DWORD));	
			}
			break;

		case PIMPOM_PLOT_FORMAT_FLOAT_IMAGE://float型メモリ
			{
				float *pDstImg = API.GetFloatMemory(pShmem->image_num, CSize(pShmem->width,pShmem->height), true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height*sizeof(float));	
			}
			break;

		case PIMPOM_PLOT_FORMAT_3D_IMAGE://3D型メモリ
			{
				BYTE *pDstImg = API.Get3DMemory(pShmem->image_num, CSize(pShmem->width,pShmem->height),pShmem->page, true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height*pShmem->page);	
			}
			break;

		case PIMPOM_PLOT_FORMAT_F3D_IMAGE://F3D型メモリ
			{
				float *pDstImg = API.GetF3DMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), pShmem->page, true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height*pShmem->page*sizeof(float));
			}
			break;

		case PIMPOM_PLOT_FORMAT_RGB_IMAGE://RGB型メモリ
			{
				BYTE *pDstImg = API.GetRGBMemory(pShmem->image_num, CSize(pShmem->width,pShmem->height),true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height*pShmem->channel);	
			}
			break;

		case PIMPOM_PLOT_FORMAT_MASK_IMAGE://マスク画像
			{
				BYTE *pMskImg = API.GetMaskMemory(pShmem->image_num);
				if (pMskImg)
				{
					memcpy(pMskImg, pShmem->pData, pShmem->width*pShmem->height*pShmem->channel);
				}
			}
			break;

		case PIMPOM_PLOT_FORMAT_FLOAT_1D_CHART://1次元データ(float)
			{
				API.PlotDataToChart(pShmem->image_num, pShmem->width, (float*)pShmem->pData);
				doDrawImage = false;
			}
			break;

		case PIMPOM_PLOT_FORMAT_DOUBLE_1D_CHART://1次元データ(double)
			{
				API.PlotDataToChart(pShmem->image_num, pShmem->width, (double*)pShmem->pData);
				doDrawImage = false;
			}
			break;

		case PIMPOM_PLOT_FORMAT_INT_1D_CHART://1次元データ(int)
			{
				API.PlotDataToChart(pShmem->image_num, pShmem->width, (int*)pShmem->pData);
				doDrawImage = false;
			}
			break;

		case PIMPOM_PLOT_FORMAT_BYTE_1D_CHART://1次元データ(BYTE)
			{
				API.PlotDataToChart(pShmem->image_num, pShmem->width, (BYTE*)pShmem->pData);
				doDrawImage = false;
			}
			break;

		case PIMPOM_PLOT_FORMAT_FLOAT_2D_CHART://2次元データ(float)
			{
				API.PlotDataToChart2(pShmem->image_num, pShmem->width, (float*)pShmem->pData, (float*)pShmem->pData + pShmem->width);
				doDrawImage = false;
			}
			break;

		case PIMPOM_PLOT_FORMAT_DOUBLE_2D_CHART://2次元データ(double)
			{
				API.PlotDataToChart2(pShmem->image_num, pShmem->width, (double*)pShmem->pData, (double*)pShmem->pData + pShmem->width);
				doDrawImage = false;
			}
			break;

		case PIMPOM_PLOT_FORMAT_INT_2D_CHART://2次元データ(int)
			{
				API.PlotDataToChart2(pShmem->image_num, pShmem->width, (int*)pShmem->pData, (int*)pShmem->pData + pShmem->width);
				doDrawImage = false;
			}
			break;

		case PIMPOM_PLOT_FORMAT_BYTE_2D_CHART://2次元データ(BYTE)
			{
				API.PlotDataToChart2(pShmem->image_num, pShmem->width, (BYTE*)pShmem->pData, (BYTE*)pShmem->pData + pShmem->width);
				doDrawImage = false;
			}
			break;
	}

	if (doDrawImage) {
		API.DrawImage(pShmem->image_num, true, true);//描画
	}

	pimpom_plot_free_shere_mem(pShmem, hShare);//共有メモリ解放

	return 0;
}


/********************************************************************
機  能  名  称 : PIMPOMから外部プログラムに画像データを転送するメッセージ受信
関    数    名 : OnMessageExtInput
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
LRESULT CPIMPOMDlg::OnMessageExtOutput(WPARAM wParam, LPARAM lParam)
{
	SHEAREMEM_PLOT_IMAGE	*pShmem;//共有メモリ領域
	SHEAREMEM_PLOT_IMAGE	shmem_header = { 0 };
	HANDLE	hShare;//共有メモリのハンドル


				   //共有メモリ領域のヘッダ部分を読む
	pShmem = (SHEAREMEM_PLOT_IMAGE*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_PLOT_IMAGE), &hShare, 0);
	if (!pShmem)	return 0;

	memcpy(&shmem_header, pShmem, sizeof(SHEAREMEM_PLOT_IMAGE));
	pimpom_plot_free_shere_mem(pShmem, hShare);//共有メモリ解放




	if (shmem_header.image_num<0 || shmem_header.image_num >= API.GetDataUnitNumber() ||
		shmem_header.width <= 0 || shmem_header.height <= 0 || shmem_header.size <= 0) {
		return 0;
	}

	//共有メモリ領域の全体読み直す
	pShmem = (SHEAREMEM_PLOT_IMAGE*)pimpom_plot_get_shere_mem(shmem_header.size, &hShare, 1);
	if (!pShmem)	return 0;


	{
		//データコピー実施
		switch (pShmem->format)
		{
			case PIMPOM_PLOT_FORMAT_BYTE_IMAGE://BYTE型メモリ
			{
				BYTE *pDstImg = API.GetByteMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height);
			}
			break;

			case PIMPOM_PLOT_FORMAT_SHORT_IMAGE://SHORT型メモリ
			{
				short *pDstImg = API.GetShortMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height * sizeof(short));
			}
			break;

			case PIMPOM_PLOT_FORMAT_WORD_IMAGE://WORD型メモリ
			{
				WORD *pDstImg = API.GetWordMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height * sizeof(WORD));
			}
			break;

			case PIMPOM_PLOT_FORMAT_LONG_IMAGE://LONG型メモリ
			{
				long *pDstImg = API.GetLongMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height * sizeof(long));
			}
			break;

			case PIMPOM_PLOT_FORMAT_DWORD_IMAGE://DWORD型メモリ
			{
				DWORD *pDstImg = API.GetDwordMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height * sizeof(DWORD));
			}
			break;

			case PIMPOM_PLOT_FORMAT_FLOAT_IMAGE://float型メモリ
			{
				float *pDstImg = API.GetFloatMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height * sizeof(float));
			}
			break;

			case PIMPOM_PLOT_FORMAT_3D_IMAGE://3D型メモリ
			{
				BYTE *pDstImg = API.Get3DMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), pShmem->page, false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height*pShmem->page);
			}
			break;

			case PIMPOM_PLOT_FORMAT_F3D_IMAGE://F3D型メモリ
			{
				float *pDstImg = API.GetF3DMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), pShmem->page, false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height*pShmem->page * sizeof(float));
			}
			break;

			case PIMPOM_PLOT_FORMAT_RGB_IMAGE://RGB型メモリ
			{
				BYTE *pDstImg = API.GetRGBMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height*pShmem->channel);
			}
			break;

			case PIMPOM_PLOT_FORMAT_MASK_IMAGE://マスク画像メモリ
			{
				BYTE *pMskImg = API.GetMaskMemory(pShmem->image_num);
				memcpy(pShmem->pData, pMskImg, pShmem->width*pShmem->height*pShmem->channel);
			}
			break;
		}
	}


	pimpom_plot_free_shere_mem(pShmem, hShare);//共有メモリ解放


	return 0;
}



/********************************************************************
機  能  名  称 : 外部プログラムから画像転送を伴わないメッセージ受信
関    数    名 : OnMessageExtProcess 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
LRESULT CPIMPOMDlg::OnMessageExtProcess(WPARAM wParam, LPARAM lParam)
{
	SHEAREMEM_EXT_PROC	*pShmem;//共有メモリ領域
	HANDLE	hShare;//共有メモリのハンドル



	//共有メモリ領域のヘッダ部分を読む
	pShmem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC), &hShare, 1);
	if(!pShmem)	return 0;



	switch(pShmem->type)
	{
		case PIMPOM_PLOT_COMMAND_PREPAIR_DRAWING://画像上に描画する準備のために、背景となる画像をコピーしてRGBフォーマットに変換する
			if(pShmem->copy_src>=0 && pShmem->copy_src<API.GetDataUnitNumber()){
				API.CopyDataUnit(pShmem->copy_src, pShmem->image_num);
				CONVERT_DATA_OPTION	option;
				option.type_to = RGB_FORMAT;
				option.rgb_to = RGB_TO_ALL;
				API.ConvertDataUnit(pShmem->image_num,option);
				API.DrawImage(pShmem->image_num);
			}
			break;

		case PIMPOM_PLOT_COMMAND_DRAW_IMAGE://画像描画
			{
				int doAdjust;
				char buf[1024]={0};
				sscanf(pShmem->message,"%d,%s\n", &doAdjust, buf);
				if(buf[0]==0){
					API.DrawImage(pShmem->image_num, true, doAdjust);
				}else{
					API.DrawImage(pShmem->image_num, CString(buf), true, doAdjust);
				}
			}
			break;

		case PIMPOM_PLOT_COMMAND_DRAW_POINT_ON_IMAGE://点描画
			API.DrawPointOnImage(pShmem->image_num, pShmem->x0, pShmem->y0, pShmem->size, RGB(pShmem->red, pShmem->green, pShmem->blue), pShmem->copy_src);
			break;

		case PIMPOM_PLOT_COMMAND_DRAW_LINE_ON_IMAGE://直線描画
			API.DrawLineOnImage(pShmem->image_num, pShmem->x0, pShmem->y0, pShmem->x1, pShmem->y1, pShmem->size, RGB(pShmem->red, pShmem->green, pShmem->blue), pShmem->copy_src);
			break;

		case PIMPOM_PLOT_COMMAND_DRAW_RECT_ON_IMAGE://矩形描画
			API.DrawRectOnImage(pShmem->image_num, pShmem->x0, pShmem->y0, pShmem->x1, pShmem->y1, RGB(pShmem->red, pShmem->green, pShmem->blue), false, 0, pShmem->copy_src);
			break;

		case PIMPOM_PLOT_COMMAND_DRAW_CIRCLE_ON_IMAGE://円形描画
			API.DrawCircleOnImage(pShmem->image_num, pShmem->x0, pShmem->y0, pShmem->x1, RGB(pShmem->red, pShmem->green, pShmem->blue), pShmem->y1, pShmem->copy_src);
			break;

		case PIMPOM_PLOT_COMMAND_DRAW_CURSOR_ON_IMAGE://十字カーソル描画
			API.DrawCursorOnImage(pShmem->image_num, pShmem->x0, pShmem->y0, pShmem->size, RGB(pShmem->red, pShmem->green, pShmem->blue), pShmem->copy_src);
			break;

		case PIMPOM_PLOT_COMMAND_DRAW_TEXT_ON_IMAGE://テキスト描画
			API.DrawTextOnImage(pShmem->image_num, pShmem->x0, pShmem->y0, pShmem->message,  RGB(pShmem->red, pShmem->green, pShmem->blue), pShmem->copy_src);
			break;

		case PIMPOM_PLOT_COMMAND_SHOW_MESSAGE://メッセージ表示
			API.ShowMessage(false,1,true, CString(pShmem->message));
			break;

		case PIMPOM_PLOT_COMMAND_SHOW_NEW_MESSAGE://メッセージ表示(新規)
			API.ShowMessage(true,1,true, CString(pShmem->message));
			break;

		case PIMPOM_PLOT_COMMAND_ADD_MARK://マーカー追加
			{
				float x,y;
				double atrb[3]={0};
				sscanf(pShmem->message,"%f,%f,%lf,%lf,%lf\n", &x, &y, &atrb[0], &atrb[1], &atrb[2]);
				API.AddMark(pShmem->image_num, x, y, atrb); 
			}
			break;

		case PIMPOM_PLOT_COMMAND_ADD_DATA_TO_CAHRT://グラフ(折れ線)に値追加
			{
				float val;
				sscanf(pShmem->message,"%f\n",&val);
				API.AddDataToChart(pShmem->image_num, val);
			}
			break;

		case PIMPOM_PLOT_COMMAND_ADD_DATA_TO_CAHRT2://グラフ(2次元散布図)に値追加
			{
				float valx, valy;
				sscanf(pShmem->message,"%f,%f\n", &valx, &valy);
				API.AddDataToChart2(pShmem->image_num, valx, valy);
			}
			break;

		case PIMPOM_PLOT_COMMAND_CLEAR_CAHRT://グラフクリア
			API.ClearChart(pShmem->image_num);
			break;


		case PIMPOM_PLOT_COMMAND_BATCH_EXECUTE://バッチ実行
			{
				pimpom_plot_free_shere_mem(pShmem, hShare);//共有メモリいったん解放

				pShmem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC) + SHEAREMEM_EXPAND_MESSAGE_SIZE, &hShare, 1);//拡張領域を読み直す
				if (!pShmem)	return 0;


				BATCH_INPUT in_param={0};
				BATCH_OUTPUT out_param={0};

				in_param.image_number = pShmem->image_num;//画像

				//データ入力
				char *ptr = (char*)pShmem + sizeof(SHEAREMEM_EXT_PROC);

				sscanf(ptr, "%s", in_param.imae_file_path);
				ptr += 2048;

				for(int i=0 ; i<BATCH_IN_PARAM_NUM ; i++){
					sscanf(ptr, "%s", in_param.param[i]);
					ptr+=1024;
				}

				//バッチ実行
				API.BatchExecute(&in_param, &out_param);

				//データ出力
				ptr = (char*)pShmem + sizeof(SHEAREMEM_EXT_PROC) + SHEAREMEM_EXPAND_MESSAGE_SIZE/2;
				for(int i=0 ; i<BATCH_OUT_PARAM_NUM ; i++){
					sprintf(ptr,"%s", out_param.result[i]);
					ptr += 1024;
				}

				*ptr = (char)out_param.state;

			}
			break;

		case 31://バッチ実行シェア
			{
					pimpom_plot_free_shere_mem(pShmem, hShare);//共有メモリいったん解放

					pShmem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC)+SHEAREMEM_EXPAND_MESSAGE_SIZE, &hShare, 1);//拡張領域を読み直す
					if (!pShmem)	return 0;


					BATCH_INPUT in_param = { 0 };
					BATCH_OUTPUT out_param = { 0 };

					in_param.image_number = pShmem->image_num;//画像

					//データ入力
					char *ptr = (char*)pShmem + sizeof(SHEAREMEM_EXT_PROC);

					sscanf(ptr, "%s", in_param.imae_file_path);
					ptr += 2048;

					for (int i = 0; i < BATCH_IN_PARAM_NUM; i++){
						sscanf(ptr, "%s", in_param.param[i]);
						ptr += 1024;
					}


			}
			break;




		case PIMPOM_PLOT_COMMAND_EXECUTE://コマンド文字列
			{
				pimpom_plot_free_shere_mem(pShmem, hShare);//共有メモリいったん解放

				pShmem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC) + SHEAREMEM_EXPAND_MESSAGE_SIZE, &hShare, 1);//拡張領域を読み直す
				if (!pShmem)	return 0;


				//データ入力
				char *command = (char*)pShmem + sizeof(SHEAREMEM_EXT_PROC);


				CString			err_str, com_str;
				double			result_num;
				char			result_str[CALC_STRING_LEN_MAX];
				CCalculate		calc;

				com_str = CString(command);
				bool ret = calc.RegisterCommandString(&com_str, 1, &err_str);
				if (ret)
				{
					BATCH_INPUT in_param = { 0 };
					BATCH_OUTPUT out_param = { 0 };

					if (API.pBatchDlg)
					{
						API.pBatchDlg->GetBatchInput(&in_param);
					}


					ret = calc.ExecuteAPI(0,&err_str, NULL, &in_param, &out_param);
					if (ret)
					{
						if (calc.GetResult(0, &result_num, result_str, &err_str) == 0){
							sprintf(result_str, "%lf", result_num);
						}

						if (API.pBatchDlg)
						{
							API.pBatchDlg->SetBatchInOut(&in_param, &out_param);
						}
					}
				}


				//結果出力
				char *result_data = (char*)pShmem + sizeof(SHEAREMEM_EXT_PROC) + SHEAREMEM_EXPAND_MESSAGE_SIZE / 2;

				result_data[0] = ret;	//先頭Byte : 実行結果 (成功: 1   失敗:0)
				strcpy(&result_data[5000], result_str);	//先頭から5000～10000Byte : 実行結果
				strcpy(&result_data[10000], err_str.GetBuffer());//先頭から10000～15000Byte : エラーメッセージ
				err_str.ReleaseBuffer();
			}
			break;
	}

	pimpom_plot_free_shere_mem(pShmem, hShare);//共有メモリ解放

	return 0;
}

/********************************************************************
機  能  名  称 : クリック座標を設定する
関    数    名 : SetClickedCoordinate 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::SetClickedCoordinate(CPoint point)
{
	latest_clicked_coordinate_x = point.x;
	latest_clicked_coordinate_y = point.y;
}









