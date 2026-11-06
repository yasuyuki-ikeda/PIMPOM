/*******************************************************************
                           License Agreement
            Ploaform for IMage Procession Obserber and Modifier

Copyright (c) 2026, Yasuyuki Ikeda All rights reserved.

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
//CMainFrame
//メインフレーム
/*******************************************************************/

//**************************************
//         インクルードファイル
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "MainFrm.h"

// *************************************
//         マ  ク  ロ   定   義
// *************************************
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		PANE_ID_BASE		(30000)	//ドッキングパネルのウィンドウID（AFX_IDW_* と重ならない範囲）
#define		TIMER_SYNC			(1)		//パネル・ツールバー・ステータスバーの同期タイマー
#define		SYNC_INTERVAL_MS	(150)
#define		STATUS_PANE_NUM		(6)		//ステータスバーの欄の数

// *************************************
//         外    部    宣    言
// *************************************
extern	CPimpomAPI	API;

// *************************************
//         静    的    変    数
// *************************************

//メイン画面のボタン（画面の外に置き、ツールバーから操作する）
static const struct {
	UINT	id;
	bool	auto_check;//チェックボックス型（押すたびに ON/OFF が切り替わる）
} main_buttons[] = {
	{ IDC_CHECK_PIMPON_DLG_DATA_SELECT0, true },
	{ IDC_CHECK_PIMPON_DLG_DATA_SELECT1, true },
	{ IDC_CHECK_PIMPON_DLG_DATA_SELECT2, true },
	{ IDC_CHECK_PIMPON_DLG_DATA_SELECT3, true },
	{ IDC_BUTTON_PIMPON_DLG_MARK, true },
	{ IDC_BUTTON_PIMPON_DLG_WORK_AREA, true },
	{ IDC_BUTTON_PIMPON_DLG_MASK, true },
	{ IDC_BUTTON_PIMPON_DLG_HISTGRAM, true },
	{ IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE, true },
	{ IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE2, true },
	{ IDC_BUTTON_PIMPON_DLG_PAINT, true },
	{ IDC_BUTTON_PIMPON_DLG_CALC, true },
	{ IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE, true },
	{ IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2, true },
	{ IDC_BUTTON_PIMPON_DLG_SCALE_DBL, false },
	{ IDC_BUTTON_PIMPON_DLG_SCALE_HLF, false },
	{ IDC_BUTTON_PIMPON_DLG_SCALE_ADJUST_FILED, false },
	{ IDC_BUTTON_PIMPON_DLG_NO_SCALE, false },
};

//表示・非表示や絵柄が画像の種類で変わるボタン
static const UINT dynamic_buttons[] = {
	IDC_CHECK_PIMPON_DLG_DATA_SELECT0, IDC_CHECK_PIMPON_DLG_DATA_SELECT1,
	IDC_CHECK_PIMPON_DLG_DATA_SELECT2, IDC_CHECK_PIMPON_DLG_DATA_SELECT3,
	IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2,
};

//ツールバーの並び（id が 0 は区切り）
static const struct {
	UINT	id;
	UINT	icon;
	LPCTSTR	tip;//NULL ならメイン画面のボタンのツールチップを使う
} toolbar_items[] = {
	{ ID_FILE_OPEN,								IDI_BTN_OPEN,		_T("画像ファイルを開く") },
	{ ID_FILE_SAVE_DATA,						IDI_BTN_SAVE,		_T("画像データを保存") },
	{ ID_EDIT_UNDO,								IDI_BTN_UNDO,		_T("画像データを元に戻す") },
	{ 0, 0, NULL },
	{ IDC_BUTTON_PIMPON_DLG_SCALE_HLF,			IDI_BTN_ZOOMOUT,	_T("表示倍率縮小") },
	{ IDC_BUTTON_PIMPON_DLG_SCALE_DBL,			IDI_BTN_ZOOMIN,		_T("表示倍率拡大") },
	{ IDC_BUTTON_PIMPON_DLG_SCALE_ADJUST_FILED,	IDI_BTN_FIT,		_T("ウィンドウに合わせる") },
	{ IDC_BUTTON_PIMPON_DLG_NO_SCALE,			IDI_BTN_ONE,		_T("等倍表示") },
	{ 0, 0, NULL },
	{ IDC_CHECK_PIMPON_DLG_DATA_SELECT0,		IDI_BTN_R,			NULL },
	{ IDC_CHECK_PIMPON_DLG_DATA_SELECT1,		IDI_BTN_G,			NULL },
	{ IDC_CHECK_PIMPON_DLG_DATA_SELECT2,		IDI_BTN_B,			NULL },
	{ IDC_CHECK_PIMPON_DLG_DATA_SELECT3,		IDI_BTN_RGB,		NULL },
	{ 0, 0, NULL },
	{ IDC_BUTTON_PIMPON_DLG_MARK,				IDI_BTN_MARK,		_T("マーク") },
	{ IDC_BUTTON_PIMPON_DLG_WORK_AREA,			IDI_BTN_WORKAREA,	_T("作業領域") },
	{ IDC_BUTTON_PIMPON_DLG_MASK,				IDI_BTN_MASK,		_T("マスク") },
	{ IDC_BUTTON_PIMPON_DLG_HISTGRAM,			IDI_BTN_HISTOGRAM,	_T("ヒストグラム") },
	{ IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE,		IDI_BTN_RULER,		_T("ルーラー") },
	{ IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE2,		IDI_BTN_PROFILE,	_T("ルーラーとラインプロファイル") },
	{ IDC_BUTTON_PIMPON_DLG_PAINT,				IDI_BTN_PEN,		_T("フリーハンドで画像を編集") },
	{ IDC_BUTTON_PIMPON_DLG_CALC,				IDI_BTN_FX,			_T("数式で画像を生成") },
	{ 0, 0, NULL },
	{ ID_IMAGE_FILTER,							IDI_BTN_FILTER,		_T("汎用フィルタ") },
	{ ID_IMAGE_BINALIZE,						IDI_BTN_BINARIZE,	_T("2値化") },
	{ ID_IMAGE_TRANSFORM,						IDI_BTN_TRANSFORM,	_T("幾何変換") },
	{ 0, 0, NULL },
	{ IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE,		IDI_BTN_CAMERA,		_T("ライブ表示") },
	{ IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2,		IDI_BTN_CAMERA_RUN,	_T("ライブ実行") },
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//外部プログラムからのメッセージの受け口
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//受けたメッセージをメイン画面（CPIMPOMDlg）にそのまま渡す
static INT_PTR CALLBACK receiver_proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_EXTERNAL_INPUT || msg == WM_EXTERNAL_OUTPUT || msg == WM_EXTERNAL_PROCESS)
	{
		LRESULT ret = 0;
		if (API.pPIMMOMDlg != NULL && ::IsWindow(API.pPIMMOMDlg->m_hWnd)) {
			ret = API.pPIMMOMDlg->SendMessage(msg, wParam, lParam);
		}
		::SetWindowLongPtr(hDlg, DWLP_MSGRESULT, ret);
		return TRUE;
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CDialogPane
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDialogPane, CDockablePane)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CDialogPane::CDialogPane()
{
	p_dialog = NULL;
	can_close = true;
}

/********************************************************************
機  能  名  称 : ダイアログをパネルに収める
関    数    名 : AttachDialog
引          数 : CDialog	*p_dlg	(in)モードレスダイアログ
戻    り    値 :
機          能 : 枠とタイトルバーを外して子ウィンドウに作り替え、パネル全体に広げる
				 ダイアログのクラスやロジックには手を加えない
********************************************************************/
void CDialogPane::AttachDialog(CDialog *p_dlg)
{
	if (p_dlg == NULL || !::IsWindow(p_dlg->m_hWnd) || p_dialog == p_dlg)	return;

	p_dialog = p_dlg;
	bool visible = (p_dlg->GetStyle() & WS_VISIBLE) != 0;

	p_dlg->ModifyStyle(WS_POPUP | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | DS_MODALFRAME, WS_CHILD);
	p_dlg->ModifyStyleEx(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE | WS_EX_APPWINDOW | WS_EX_TOOLWINDOW, WS_EX_CONTROLPARENT);
	p_dlg->SetParent(this);

	CRect rect;
	GetClientRect(&rect);
	p_dlg->SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(),
		SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | (visible ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));
}

/********************************************************************
機  能  名  称 : パネルの閉じるボタンが押された
関    数    名 : OnPressCloseButton
引          数 :
戻    り    値 :
機          能 : 従来のウィンドウの閉じるボタンと同じく、ダイアログに WM_CLOSE を送る
				 ダイアログが自分を非表示にすると、SyncPanes でパネルも閉じる
********************************************************************/
void CDialogPane::OnPressCloseButton()
{
	if (p_dialog != NULL && ::IsWindow(p_dialog->m_hWnd)) {
		p_dialog->PostMessage(WM_CLOSE);
	}
	else {
		CDockablePane::OnPressCloseButton();
	}
}

void CDialogPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (p_dialog != NULL && ::IsWindow(p_dialog->m_hWnd)) {
		p_dialog->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void CDialogPane::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	if (p_dialog != NULL && ::IsWindow(p_dialog->m_hWnd)) {
		p_dialog->SetFocus();
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CForwardDlg
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CForwardDlg, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()

CForwardDlg::CForwardDlg(UINT idd, CWnd *p_parent) : CDialog(idd, p_parent)
{
}

//部品からの通知（チェックボックスなど）はメイン画面のハンドラで処理する
BOOL CForwardDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)	return TRUE;//Enter・Esc では何もしない

	if (API.pPIMMOMDlg != NULL && ::IsWindow(API.pPIMMOMDlg->m_hWnd)) {
		API.pPIMMOMDlg->SendMessage(WM_COMMAND, wParam, lParam);
		return TRUE;
	}
	return CDialog::OnCommand(wParam, lParam);
}

BOOL CForwardDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (API.pPIMMOMDlg != NULL && ::IsWindow(API.pPIMMOMDlg->m_hWnd)) {
		*pResult = API.pPIMMOMDlg->SendMessage(WM_NOTIFY, wParam, lParam);
		return TRUE;
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}

BOOL CForwardDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))	return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

//メッセージ欄はタブの下にパネル全体まで広げ、クリアボタンはタブの右端に置く（画素ルーペはテンプレートの配置のまま）
void CForwardDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CTabCtrl *p_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_PIMPOM_DLG_MESSAGE);
	CWnd *p_edit = GetDlgItem(IDC_EDIT_PIMPOM_DLG_MESSAGE);
	if (p_tab == NULL || p_edit == NULL)	return;

	int tab_h = ScaleByDpi(24);
	CRect item_rect;
	if (p_tab->GetItemCount() > 0 && p_tab->GetItemRect(0, &item_rect)) {
		tab_h = item_rect.bottom + ScaleByDpi(2);
	}
	CWnd *p_clear = GetDlgItem(IDC_BUTTON_PIMPOM_DLG_MESSAGE_CLEAR);
	int tab_w = cx;
	if (p_clear != NULL) {
		int btn_w = ScaleByDpi(48), btn_h = max(ScaleByDpi(18), tab_h - ScaleByDpi(4));
		tab_w = max(0, cx - btn_w - ScaleByDpi(4));
		p_clear->MoveWindow(max(0, cx - btn_w - ScaleByDpi(2)), max(0, (tab_h - btn_h) / 2), btn_w, btn_h);
	}
	p_tab->MoveWindow(0, 0, tab_w, tab_h);
	p_edit->MoveWindow(0, tab_h, cx, max(0, cy - tab_h));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CPimpomToolBar
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CPimpomToolBar::OnUserToolTip(CMFCToolBarButton* pButton, CString& strTTText) const
{
	if (p_frame == NULL || pButton == NULL)	return FALSE;

	strTTText = p_frame->GetToolTip(pButton->m_nID);
	return !strTTText.IsEmpty();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CMainFrame
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_INITMENU()
	ON_WM_MENUSELECT()
	ON_MESSAGE(WM_EXTERNAL_INPUT, &CMainFrame::OnExtInput)
	ON_MESSAGE(WM_EXTERNAL_OUTPUT, &CMainFrame::OnExtOutput)
	ON_MESSAGE(WM_EXTERNAL_PROCESS, &CMainFrame::OnExtProcess)
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
	p_main_dlg = NULL;
	p_info_dlg = NULL;
	p_output_dlg = NULL;
	h_receiver = NULL;
}

CMainFrame::~CMainFrame()
{
	for (size_t i = 0; i < panes.size(); i++) {
		delete panes[i].pane;
	}
	panes.clear();
	delete p_info_dlg;
	delete p_output_dlg;
}

bool CMainFrame::IsInvisibleMode() const
{
	return API.invisible;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	//CFrameWndEx::LoadFrame はレジストリから大アイコンの設定と配置を読む（レジストリキーが無いと ASSERT で落ちる）。
	//設定は pimpom.ini だけに置くので、基底の CFrameWnd を直接呼ぶ
	return CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWndEx::PreCreateWindow(cs))	return FALSE;

	cs.style &= ~FWS_ADDTOTITLE;//タイトルはメイン画面が画像の情報で設定する
	cs.style |= WS_CLIPCHILDREN;//メイン画面は GetDC で直接描くので、フレームの背景塗りで上書きされないようにする

	//作業領域の 9 割程度の大きさで開く（パネルの配置は OnCreate でこの大きさを基準に決まるので、生成時に決めておく）
	CRect work_area;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &work_area, 0);
	cs.cx = min(work_area.Width() * 92 / 100, ScaleByDpi(1400));
	cs.cy = min(work_area.Height() * 92 / 100, ScaleByDpi(940));
	cs.x = work_area.left + (work_area.Width() - cs.cx) / 2;
	cs.y = work_area.top + (work_area.Height() - cs.cy) / 2;
	return TRUE;
}

/********************************************************************
機  能  名  称 : フレームの生成
関    数    名 : OnCreate
引          数 :
戻    り    値 :
機          能 : ツールバー・ステータスバー・ドッキングパネル・外部プログラム用の受け口を作ってから、
				 中央にメイン画面（CPIMPOMDlg）を子ウィンドウとして作る
				 （メイン画面の初期化中に作られる付属ウィンドウを、ここで作ったパネルに収めるため）
********************************************************************/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)	return -1;

	API.pMainFrame = this;
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));//配色

	CDockingManager::SetDockingMode(DT_SMART);
	EnableDocking(CBRS_ALIGN_ANY);

	create_toolbar();
	create_statusbar();
	create_panes();
	create_receiver();

	p_main_dlg = new CPIMPOMDlg(this);
	p_main_dlg->cmdLine = cmdLine;
	if (!p_main_dlg->Create(IDD_PIMPOM_DLG, this))	return -1;
	p_main_dlg->SetDlgCtrlID(AFX_IDW_PANE_FIRST);//フレームの中央（パネルを除いた残り）に配置される
	p_main_dlg->ShowWindow(SW_SHOW);

	SyncPanes();
	SetTimer(TIMER_SYNC, SYNC_INTERVAL_MS, NULL);
	return 0;
}

void CMainFrame::OnDestroy()
{
	KillTimer(TIMER_SYNC);

	if (h_receiver != NULL) {
		::DestroyWindow(h_receiver);
		h_receiver = NULL;
	}

	CFrameWndEx::OnDestroy();
	API.pMainFrame = NULL;
}

/********************************************************************
機  能  名  称 : ツールバーを作る
関    数    名 : create_toolbar
引          数 :
戻    り    値 :
機          能 : メイン画面右端にあったボタン列と、よく使うメニュー項目を並べる
				 アイコンは表示倍率に合わせた大きさで読み込む（MFC 側での拡大はさせない）
********************************************************************/
void CMainFrame::create_toolbar()
{
	const CSize image_size(ScaleByDpi(20), ScaleByDpi(20));
	CMFCToolBar::m_bDontScaleImages = TRUE;
	CMFCToolBar::SetSizes(image_size + CSize(ScaleByDpi(9), ScaleByDpi(9)), image_size);

	toolbar.p_frame = this;
	if (!toolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
		CRect(1, 1, 1, 1), AFX_IDW_TOOLBAR))	return;
	toolbar.SetWindowText(_T("ツールバー"));

	for (int i = 0; i < sizeof(toolbar_items) / sizeof(toolbar_items[0]); i++)
	{
		if (toolbar_items[i].id == 0) {
			toolbar.InsertSeparator();
			continue;
		}
		toolbar.InsertButton(CMFCToolBarButton(toolbar_items[i].id, toolbar_image_from_resource(toolbar_items[i].icon)));
		if (toolbar_items[i].tip != NULL)	tool_tips[toolbar_items[i].id] = toolbar_items[i].tip;
	}

	toolbar.SetPaneStyle(toolbar.GetPaneStyle() & ~CBRS_GRIPPER);
	toolbar.EnableDocking(CBRS_ALIGN_TOP);
	DockPane(&toolbar);
}

int CMainFrame::toolbar_image_from_resource(UINT icon_id)
{
	std::map<UINT, int>::iterator it = res_images.find(icon_id);
	if (it != res_images.end())	return it->second;

	const int size = ScaleByDpi(20);
	HINSTANCE h_inst = AfxFindResourceHandle(MAKEINTRESOURCE(icon_id), RT_GROUP_ICON);
	HICON h_icon = (HICON)::LoadImage(h_inst, MAKEINTRESOURCE(icon_id), IMAGE_ICON, size, size, LR_DEFAULTCOLOR);
	int index = CMFCToolBar::GetImages()->AddIcon(h_icon, TRUE);
	if (h_icon != NULL)	::DestroyIcon(h_icon);

	res_images[icon_id] = index;
	return index;
}

//メイン画面のボタンに付いているアイコンをツールバーの画像として登録する
int CMainFrame::toolbar_image_from_icon(HICON h_icon)
{
	std::map<HICON, int>::iterator it = icon_images.find(h_icon);
	if (it != icon_images.end())	return it->second;

	int index = CMFCToolBar::GetImages()->AddIcon(h_icon, TRUE);
	icon_images[h_icon] = index;
	return index;
}

CString CMainFrame::GetToolTip(UINT id) const
{
	std::map<UINT, CString>::const_iterator it = tool_tips.find(id);
	if (it != tool_tips.end())	return it->second;

	if (p_main_dlg != NULL && ::IsWindow(p_main_dlg->m_hWnd))	return p_main_dlg->GetButtonTip(id);
	return CString();
}

/********************************************************************
機  能  名  称 : ステータスバーを作る
関    数    名 : create_statusbar
引          数 :
戻    り    値 :
機          能 : 座標と画素値・表示倍率・データの種類と大きさ・表示レンジ・作業領域とマスク・画像メモリ数
********************************************************************/
void CMainFrame::create_statusbar()
{
	static const UINT indicators[STATUS_PANE_NUM] = { ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR };
	static const int widths[STATUS_PANE_NUM] = { 0, 90, 300, 190, 190, 110 };

	if (!statusbar.Create(this))	return;
	statusbar.SetIndicators(indicators, STATUS_PANE_NUM);
	statusbar.SetPaneStyle(0, SBPS_STRETCH | SBPS_NOBORDERS);
	for (int i = 1; i < STATUS_PANE_NUM; i++) {
		statusbar.SetPaneWidth(i, ScaleByDpi(widths[i]));
	}
}

/********************************************************************
機  能  名  称 : ドッキングパネルを作る
関    数    名 : create_panes
引          数 :
戻    り    値 :
機          能 : 右：ヒストグラム／マーク／作業領域／画素ルーペ（縦に並べる）
				 下：データ／グラフ／スクリプト／Python／バッチ／出力（タブ）
				 既存のダイアログは後で生成されたときに収める（OnDialogCreated）
********************************************************************/
void CMainFrame::create_panes()
{
	const int right_w = ScaleByDpi(HISTGRAM_WIN_W);
	const CSize bottom_size(ScaleByDpi(800), ScaleByDpi(150));

	//メイン画面から移した部品のダイアログ（メイン画面より先に作る）
	p_info_dlg = new CForwardDlg(IDD_PIXEL_INFO_PANE, this);
	p_info_dlg->Create(IDD_PIXEL_INFO_PANE, this);
	p_output_dlg = new CForwardDlg(IDD_OUTPUT_PANE, this);
	p_output_dlg->Create(IDD_OUTPUT_PANE, this);
	CRect info_rect;
	p_info_dlg->GetWindowRect(&info_rect);

	CDialogPane *hist = create_pane(IDD_HISTGRAM_DLG, PANE_ID_BASE + 0, _T("ヒストグラム"), CSize(right_w, ScaleByDpi(HISTGRAM_WIN_H)), CBRS_RIGHT, true);
	CDialogPane *mark = create_pane(IDD_MARK_DLG, PANE_ID_BASE + 1, _T("マーク"), CSize(right_w, ScaleByDpi(MARK_WIN_H)), CBRS_RIGHT, true);
	CDialogPane *work = create_pane(IDD_WORKAREA_DLG, PANE_ID_BASE + 2, _T("作業領域"), CSize(right_w, ScaleByDpi(WORKAREA_WIN_H)), CBRS_RIGHT, true);
	CDialogPane *info = create_pane(IDD_PIXEL_INFO_PANE, PANE_ID_BASE + 3, _T("画素ルーペ"), CSize(right_w, info_rect.Height() + ScaleByDpi(24)), CBRS_RIGHT, false);
	CDialogPane *data = create_pane(IDD_DATA_DLG, PANE_ID_BASE + 4, _T("データ"), bottom_size, CBRS_BOTTOM, false);
	CDialogPane *output = create_pane(IDD_OUTPUT_PANE, PANE_ID_BASE + 5, _T("出力"), CSize(right_w, ScaleByDpi(160)), CBRS_RIGHT, false);
	if (!hist || !mark || !work || !info || !data || !output)	return;
	//グラフ・スクリプト・Python・バッチは大きく、ほかの画面と並べて使うことが多いので、パネルにせず従来どおり別ウィンドウにする

	DockPane(hist);
	mark->DockToWindow(hist, CBRS_ALIGN_BOTTOM);
	work->DockToWindow(mark, CBRS_ALIGN_BOTTOM);
	info->DockToWindow(work, CBRS_ALIGN_BOTTOM);
	output->DockToWindow(info, CBRS_ALIGN_BOTTOM);

	DockPane(data);

	info->AttachDialog(p_info_dlg);
	output->AttachDialog(p_output_dlg);
	p_info_dlg->ShowWindow(SW_SHOW);
	p_output_dlg->ShowWindow(SW_SHOW);

	//配置を確定させてから隠す（確定前に隠すと古い座標が残り、あとで表示したときに列の幅がその座標まで広がる）
	RecalcLayout();
	//中のダイアログがまだ無いパネルは隠しておく（SyncPanes が表示状態を合わせる）
	for (size_t i = 0; i < panes.size(); i++) {
		if (panes[i].pane->GetDialog() == NULL)	panes[i].pane->ShowPane(FALSE, FALSE, FALSE);
	}
}

CDialogPane* CMainFrame::create_pane(int idd, UINT pane_id, LPCTSTR caption, CSize size, DWORD align, bool closable)
{
	CDialogPane *pane = new CDialogPane();
	DWORD style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | align | CBRS_FLOAT_MULTI;

	if (!pane->Create(caption, this, CRect(0, 0, size.cx, size.cy), TRUE, pane_id, style)) {
		delete pane;
		return NULL;
	}
	pane->SetClosable(closable);
	pane->EnableDocking(CBRS_ALIGN_ANY);

	PANE_ENTRY entry = { idd, pane };
	panes.push_back(entry);
	return pane;
}

CDialogPane* CMainFrame::find_pane(int idd)
{
	for (size_t i = 0; i < panes.size(); i++) {
		if (panes[i].idd == idd)	return panes[i].pane;
	}
	return NULL;
}

/********************************************************************
機  能  名  称 : 外部プログラム用の受け口を作る
関    数    名 : create_receiver
引          数 :
戻    り    値 :
機          能 : pimpom_plot.h（Pimpom.pyd を含む）は、タイトルに " - PIMPOM  "（2 台目以降は " - PIMPOM n"）を含む
				 ダイアログクラス(#32770)のトップレベルウィンドウを探してメッセージを送る。
				 メイン画面はフレームの子ウィンドウになったので、同じ条件の非表示ウィンドウを置いて中継する
********************************************************************/
void CMainFrame::create_receiver()
{
	struct {
		DLGTEMPLATE	tmpl;
		WORD		menu, window_class, title;
	} dlg_template;
	ZeroMemory(&dlg_template, sizeof(dlg_template));
	dlg_template.tmpl.style = WS_POPUP;

	h_receiver = ::CreateDialogIndirect(AfxGetInstanceHandle(), &dlg_template.tmpl, NULL, receiver_proc);
	if (h_receiver == NULL)	return;

	CString title;
	if (API.pimpom_id > 0)	title.Format(_T("PIMPOM receiver - PIMPOM %d"), API.pimpom_id);
	else					title = _T("PIMPOM receiver - PIMPOM  ");
	::SetWindowText(h_receiver, title);
}

//メイン画面からパネルに移した部品を持つウィンドウ（無ければ NULL）
HWND CMainFrame::GetMovedItemOwner(UINT id) const
{
	if (p_info_dlg != NULL && ::IsWindow(p_info_dlg->m_hWnd) && ::GetDlgItem(p_info_dlg->m_hWnd, id) != NULL)	return p_info_dlg->m_hWnd;
	if (p_output_dlg != NULL && ::IsWindow(p_output_dlg->m_hWnd) && ::GetDlgItem(p_output_dlg->m_hWnd, id) != NULL)	return p_output_dlg->m_hWnd;
	return NULL;
}

/********************************************************************
機  能  名  称 : モードレスダイアログが生成された
関    数    名 : OnDialogCreated
引          数 : int		idd		(in)ダイアログのリソースID
				 CDialog	*p_dlg	(in)ダイアログ
戻    り    値 :
機          能 : CPimpomAPI::DialogOpen から呼ばれる。ドッキング対象ならパネルに収める
********************************************************************/
void CMainFrame::OnDialogCreated(int idd, CDialog *p_dlg)
{
	place_window(idd, p_dlg);

	CDialogPane *pane = find_pane(idd);
	if (pane != NULL && pane->GetDialog() != p_dlg) {
		pane->AttachDialog(p_dlg);
		if (idd == IDD_DATA_DLG)	fit_data_pane(pane);
	}
}

/********************************************************************
機  能  名  称 : データパネルの高さをサムネイルの行数に合わせる
関    数    名 : fit_data_pane
引          数 : CDialogPane	*pane	(in)データパネル
戻    り    値 :
機          能 : 画像メモリの数とサムネイルの大きさは pimpom.ini で変わり、パネルを作る時点では分からないので、
				 データのダイアログを収めたときに、いまの幅で全部の行が入る高さまで広げる。
				 行が多いときはフレームの高さの 4 割までにとどめ、残りはデータのダイアログのスクロールバーで見る
********************************************************************/
void CMainFrame::fit_data_pane(CDialogPane *pane)
{
	CDialog *p_dlg = pane->GetDialog();
	if (p_dlg == NULL)	return;

	CRect dlg_rect, frame_rect;
	p_dlg->GetClientRect(&dlg_rect);
	GetClientRect(&frame_rect);
	if (dlg_rect.Width() <= 0)	return;

	int need = CDataDlg::GetContentHeight(dlg_rect.Width(), 0);
	int limit = frame_rect.Height() * 4 / 10 - (pane->GetCaptionHeight() + ScaleByDpi(8));
	if (need > limit)	need = max(dlg_rect.Height(), limit);
	int delta = need - dlg_rect.Height();
	if (delta <= 0)	return;

	CRect pane_rect;
	pane->GetWindowRect(&pane_rect);
	pane_rect.top -= delta;//下のパネルの上端を上げる（配置はパネルの大きさから決め直される）
	ScreenToClient(&pane_rect);
	pane->SetWindowPos(NULL, pane_rect.left, pane_rect.top, pane_rect.Width(), pane_rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	RecalcLayout();
}

/********************************************************************
機  能  名  称 : 別ウィンドウの置き場所を決める
関    数    名 : place_window
引          数 : int		idd		(in)ダイアログのリソースID
				 CDialog	*p_dlg	(in)ダイアログ
戻    り    値 :
機          能 : グラフ・スクリプト・Python・バッチは初めて開くときに、メインウィンドウの右隣（入らなければ画面の右端）へ置く
				 既定の置き場所はメイン画像の左上で画像を覆い、ラインプロファイルのドラッグ中に開いたグラフがマウスを取ってしまう
********************************************************************/
void CMainFrame::place_window(int idd, CDialog *p_dlg)
{
	if (idd != IDD_CHART_DLG && idd != IDD_COMMAND_RPOMPT_DLG && idd != IDD_PYTHON_BATCH_DLG && idd != IDD_BATCH_DLG)	return;
	for (size_t i = 0; i < placed_windows.size(); i++) {
		if (placed_windows[i] == idd)	return;
	}
	placed_windows.push_back(idd);

	CRect frame_rect, dlg_rect;
	GetWindowRect(&frame_rect);
	p_dlg->GetWindowRect(&dlg_rect);
	MONITORINFO mi = { sizeof(mi) };
	::GetMonitorInfo(::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST), &mi);
	const CRect work(mi.rcWork);

	int x = frame_rect.right;
	if (x + dlg_rect.Width() > work.right)	x = max(work.left, work.right - dlg_rect.Width());
	int y = frame_rect.top + ScaleByDpi(60) + (int)(placed_windows.size() - 1) * ScaleByDpi(30);//重ならないよう少しずつ下げる
	if (y + dlg_rect.Height() > work.bottom)	y = max(work.top, work.bottom - dlg_rect.Height());
	p_dlg->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

/********************************************************************
機  能  名  称 : モードレスダイアログが表示された
関    数    名 : OnDialogShown
引          数 : int		idd		(in)ダイアログのリソースID
				 CDialog	*p_dlg	(in)ダイアログ
戻    り    値 :
機          能 : 従来はウィンドウを最前面に出していたので、パネルを表示してタブを前に出す
********************************************************************/
void CMainFrame::OnDialogShown(int idd, CDialog *p_dlg)
{
	CDialogPane *pane = find_pane(idd);
	if (pane != NULL && pane->GetDialog() == p_dlg) {
		show_pane(pane, true);
	}
}

/********************************************************************
機  能  名  称 : パネルを表示する
関    数    名 : show_pane
引          数 : CDialogPane	*pane		(in)パネル
				 bool			activate	(in)タブを前に出すかどうか
戻    り    値 :
機          能 : 隠れているパネルはフレームの大きさが変わっても動かず古い座標に残る。そのまま表示すると
				 同じ列の幅がその座標まで広がるので、表示する前に列のいまの位置と幅に合わせる
********************************************************************/
void CMainFrame::show_pane(CDialogPane *pane, bool activate)
{
	CPaneDivider *divider = ((pane->GetStyle() & WS_VISIBLE) == 0) ? pane->GetDefaultPaneDivider() : NULL;
	if (divider != NULL)
	{
		CRect column = divider->GetRootContainerRect();//列のうち表示中のパネルが占める範囲（スクリーン座標）
		if (!column.IsRectEmpty())
		{
			CRect rect;
			pane->GetWindowRect(&rect);
			CRect target(column.left, column.top, column.right, column.top + min(rect.Height(), column.Height()));
			pane->GetParent()->ScreenToClient(&target);
			pane->SetWindowPos(NULL, target.left, target.top, target.Width(), target.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}
	pane->ShowPane(TRUE, FALSE, activate ? TRUE : FALSE);

	CBaseTabbedPane *tabbed = activate ? pane->GetParentTabbedPane() : NULL;
	if (tabbed != NULL)
	{//タブにまとめたパネルは、すでに開いていると ShowPane ではタブが前に出ないので、明示的に前に出す
		CMFCBaseTabCtrl *tab = tabbed->GetUnderlyingWindow();
		int idx = tab->GetTabFromHwnd(pane->m_hWnd);
		if (idx >= 0 && tab->GetActiveTab() != idx)	tab->SetActiveTab(idx);
	}
}

/********************************************************************
機  能  名  称 : パネルの表示状態を中のダイアログに合わせる
関    数    名 : SyncPanes
引          数 :
戻    り    値 :
機          能 : 既存のコードはダイアログを ShowWindow で表示・非表示にしているので、
				 その状態（WS_VISIBLE）を見てパネルを開閉する
				 IsVisible はタブの裏や自動非表示のときも「開いている」と答える
********************************************************************/
void CMainFrame::SyncPanes()
{
	for (size_t i = 0; i < panes.size(); i++)
	{
		CDialogPane *pane = panes[i].pane;
		CDialog *p_dlg = pane->GetDialog();
		bool want = (p_dlg != NULL && ::IsWindow(p_dlg->m_hWnd) && (p_dlg->GetStyle() & WS_VISIBLE) != 0);
		bool shown = (pane->IsVisible() != FALSE);

		if (want && !shown) {
			show_pane(pane, true);
		}
		else if (!want && shown) {
			pane->ShowPane(FALSE, FALSE, FALSE);
		}
	}
}

/********************************************************************
機  能  名  称 : ツールバーをメイン画面のボタンの状態に合わせる
関    数    名 : sync_toolbar
引          数 :
戻    り    値 :
機          能 : 表示データ選択（チャネル・ページ）とライブのボタンは、画像の種類やキャプチャの有無で
				 表示・非表示や絵柄が変わるので、メイン画面のボタンに合わせる
				 （チェック状態と有効・無効は OnCmdMsg の CN_UPDATE_COMMAND_UI で合わせる）
********************************************************************/
void CMainFrame::sync_toolbar()
{
	if (p_main_dlg == NULL || !::IsWindow(p_main_dlg->m_hWnd) || !::IsWindow(toolbar.m_hWnd))	return;

	bool layout_changed = false;
	for (int i = 0; i < sizeof(dynamic_buttons) / sizeof(dynamic_buttons[0]); i++)
	{
		UINT id = dynamic_buttons[i];
		int index = toolbar.CommandToIndex(id);
		CMFCToolBarButton *p_tb_button = (index >= 0) ? toolbar.GetButton(index) : NULL;
		CWnd *p_button = p_main_dlg->GetDlgItem(id);
		if (p_tb_button == NULL || p_button == NULL)	continue;

		BOOL visible = (p_button->GetStyle() & WS_VISIBLE) != 0;
		if (p_tb_button->IsVisible() != visible) {
			p_tb_button->SetVisible(visible);
			layout_changed = true;
		}

		HICON h_icon = (HICON)p_button->SendMessage(BM_GETIMAGE, IMAGE_ICON, 0);
		if (h_icon != NULL) {
			int image = toolbar_image_from_icon(h_icon);
			if (p_tb_button->GetImage() != image) {
				p_tb_button->SetImage(image);
				toolbar.InvalidateButton(index);
			}
		}
	}

	if (layout_changed) {
		toolbar.AdjustLayout();
		RecalcLayout();
	}
}

/********************************************************************
機  能  名  称 : ステータスバーの表示を更新する
関    数    名 : sync_statusbar
引          数 :
戻    り    値 :
機          能 :
********************************************************************/
void CMainFrame::sync_statusbar()
{
	if (!::IsWindow(statusbar.m_hWnd))	return;

	CString texts[STATUS_PANE_NUM];
	CDataUnit *p_du = API.GetDataUnit(CURRENT_IMAGE);

	//座標と画素値（画素ルーペの表示を写す）
	if (p_info_dlg != NULL && ::IsWindow(p_info_dlg->m_hWnd) && p_du != NULL) {
		CString coor, value;
		p_info_dlg->GetDlgItemText(IDC_EDIT_PIMPON_DLG_COOR_IJ, coor);
		p_info_dlg->GetDlgItemText(IDC_EDIT_PIMPON_DLG_VALUE00, value);
		if (!coor.IsEmpty())	texts[0] = coor + _T("    値 = ") + value;
	}

	if (p_du != NULL)
	{
		texts[1].Format(_T("倍率 %.0f%%"), p_du->DispScale * 100.0);

		CString title;
		GetWindowText(title);
		int pos = title.Find(_T(" - PIMPOM"));
		if (pos > 0)	title = title.Left(pos);
		texts[2].Format(_T("[%d] "), API.GetCurrentImageNumber());
		texts[2] += title;

		texts[3].Format(_T("表示レンジ %g ～ %g"), (double)p_du->BrightMin, (double)p_du->BrightMax);
		texts[4].Format(_T("作業領域 %s ／ マスク %s"), p_du->DoUseWorkArea ? _T("ON") : _T("OFF"), p_du->DoUseMask ? _T("ON") : _T("OFF"));
	}

	int used = 0;
	for (int n = 0; n < API.GetDataUnitNumber(); n++) {
		if (API.GetDataUnit(n) != NULL)	used++;
	}
	texts[5].Format(_T("メモリ %d / %d"), used, API.GetDataUnitNumber());

	for (int i = 0; i < STATUS_PANE_NUM; i++) {
		if (statusbar.GetPaneText(i) != texts[i])	statusbar.SetPaneText(i, texts[i]);
	}
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_SYNC) {
		SyncPanes();
		sync_toolbar();
		sync_statusbar();
	}
	CFrameWndEx::OnTimer(nIDEvent);
}

bool CMainFrame::is_main_button(UINT id, bool *p_auto_check) const
{
	for (int i = 0; i < sizeof(main_buttons) / sizeof(main_buttons[0]); i++) {
		if (main_buttons[i].id == id) {
			*p_auto_check = main_buttons[i].auto_check;
			return true;
		}
	}
	return false;
}

/********************************************************************
機  能  名  称 : コマンドの振り分け
関    数    名 : OnCmdMsg
引          数 :
戻    り    値 :
機          能 : ツールバーのうちメイン画面のボタンに当たるものは、そのボタンをクリックしたのと同じにする
				 （チェックボックス型は ON/OFF を切り替えてから BN_CLICKED を送る）
				 メニューのコマンドは従来どおりメイン画面（CPIMPOMDlg）のハンドラで処理する
********************************************************************/
BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (p_main_dlg != NULL && ::IsWindow(p_main_dlg->m_hWnd))
	{
		bool auto_check = false;
		CButton *p_button = is_main_button(nID, &auto_check) ? (CButton*)p_main_dlg->GetDlgItem(nID) : NULL;
		if (p_button != NULL)
		{
			if (nCode == CN_COMMAND)
			{
				if (pHandlerInfo == NULL)
				{
					if (auto_check)	p_button->SetCheck(p_button->GetCheck() == BST_CHECKED ? BST_UNCHECKED : BST_CHECKED);
					p_main_dlg->SendMessage(WM_COMMAND, MAKEWPARAM(nID, BN_CLICKED), (LPARAM)p_button->m_hWnd);
				}
				return TRUE;
			}
			else if (nCode == CN_UPDATE_COMMAND_UI)
			{
				CCmdUI *p_cmd_ui = (CCmdUI*)pExtra;
				p_cmd_ui->Enable((p_button->GetStyle() & WS_VISIBLE) != 0 && p_button->IsWindowEnabled());
				if (auto_check)	p_cmd_ui->SetCheck(p_button->GetCheck() == BST_CHECKED ? 1 : 0);
				return TRUE;
			}
		}

		//CDialog::OnCmdMsg は処理できないコマンドを親（このフレーム）へ回し返して無限に往復するので、メイン画面自身の処理表だけを引く
		if (p_main_dlg->CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))	return TRUE;
	}
	return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

/********************************************************************
機  能  名  称 : キー入力の前処理
関    数    名 : PreTranslateMessage
引          数 :
戻    り    値 :
機          能 : フォーカスがフレーム自身にあるときのキー操作（Delete、矢印、Ctrl+C/V など）を
				 従来どおりメイン画面で処理する
********************************************************************/
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (p_main_dlg != NULL && ::IsWindow(p_main_dlg->m_hWnd) &&
		pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST && pMsg->hwnd == m_hWnd)
	{
		if (p_main_dlg->PreTranslateMessage(pMsg))	return TRUE;
	}
	return CFrameWndEx::PreTranslateMessage(pMsg);
}

//メニュー関連の通知はメイン画面にも渡す（「最近使ったファイル」の更新など）
void CMainFrame::OnInitMenu(CMenu* pMenu)
{
	CFrameWndEx::OnInitMenu(pMenu);

	if (p_main_dlg != NULL && ::IsWindow(p_main_dlg->m_hWnd)) {
		p_main_dlg->SendMessage(WM_INITMENU, (WPARAM)pMenu->GetSafeHmenu(), 0);
	}
}

void CMainFrame::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	CFrameWndEx::OnMenuSelect(nItemID, nFlags, hSysMenu);

	if (p_main_dlg != NULL && ::IsWindow(p_main_dlg->m_hWnd)) {
		p_main_dlg->SendMessage(WM_MENUSELECT, MAKEWPARAM(nItemID, nFlags), (LPARAM)hSysMenu);
	}
}

//Pimpom.py はタイトルに "- PIMPOM" を含む表示中のトップレベルウィンドウ（このフレーム）に送るので中継する
LRESULT CMainFrame::OnExtInput(WPARAM wParam, LPARAM lParam)
{
	return (p_main_dlg != NULL) ? p_main_dlg->SendMessage(WM_EXTERNAL_INPUT, wParam, lParam) : 0;
}

LRESULT CMainFrame::OnExtOutput(WPARAM wParam, LPARAM lParam)
{
	return (p_main_dlg != NULL) ? p_main_dlg->SendMessage(WM_EXTERNAL_OUTPUT, wParam, lParam) : 0;
}

LRESULT CMainFrame::OnExtProcess(WPARAM wParam, LPARAM lParam)
{
	return (p_main_dlg != NULL) ? p_main_dlg->SendMessage(WM_EXTERNAL_PROCESS, wParam, lParam) : 0;
}

