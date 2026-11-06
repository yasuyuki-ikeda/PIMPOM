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
//メインフレーム（メニュー・ツールバー・ステータスバー・ドッキングパネルを持ち、
//中央に CPIMPOMDlg を子ウィンドウとして置く）
/*******************************************************************/
#pragma once

//**************************************
//         インクルードファイル
//**************************************
#include <vector>
#include <map>

class CPIMPOMDlg;
class CMainFrame;

//**************************************
//         定数定義
//**************************************

///*************************************
//         クラス   定   義
///*************************************

//既存のモードレスダイアログを中に収めるドッキングパネル
//パネルの表示状態は、中のダイアログ自身の表示状態（ShowWindow）に追従させる
class PIMPOM_EXT CDialogPane : public CDockablePane
{
public:
	CDialogPane();
	void		AttachDialog(CDialog *p_dlg);//ダイアログを子ウィンドウに変えてパネルに収める
	CDialog*	GetDialog() const { return p_dialog; }
	void		SetClosable(bool closable) { can_close = closable; }
	virtual BOOL CanBeClosed() const { return can_close; }

protected:
	CDialog		*p_dialog;//中に収めたダイアログ
	bool		can_close;//閉じるボタンを出すかどうか

	virtual void OnPressCloseButton();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()
};


//メイン画面から移した部品（画素ルーペ・メッセージ）を持つダイアログ
//部品からの通知（WM_COMMAND・WM_NOTIFY）はメイン画面に転送し、既存のハンドラで処理する
class PIMPOM_EXT CForwardDlg : public CDialog
{
public:
	CForwardDlg(UINT idd, CWnd *p_parent);

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK() {}
	virtual void OnCancel() {}
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};


//ツールチップを自前の表から出すツールバー
class PIMPOM_EXT CPimpomToolBar : public CMFCToolBar
{
public:
	CMainFrame	*p_frame;

	CPimpomToolBar() { p_frame = NULL; }
	virtual BOOL OnUserToolTip(CMFCToolBarButton* pButton, CString& strTTText) const;
	virtual BOOL AllowShowOnPaneMenu() const { return FALSE; }
};


class PIMPOM_EXT CMainFrame : public CFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();
	virtual ~CMainFrame();

	CString		cmdLine;//コマンドライン（起動時に読み込む画像ファイル）

	CPIMPOMDlg*	GetMainDialog() const { return p_main_dlg; }
	bool		IsInvisibleMode() const;//pimpom.ini の INVISIBLE（画面を出さない）が指定されているか
	void		OnDialogCreated(int idd, CDialog *p_dlg);//モードレスダイアログが生成された（ドッキング対象ならパネルに収める）
	void		OnDialogShown(int idd, CDialog *p_dlg);//モードレスダイアログが表示された（パネルを前面に出す）
	void		SyncPanes();//パネルの表示状態を中のダイアログの表示状態に合わせる
	HWND		GetMovedItemOwner(UINT id) const;//メイン画面からパネルに移した部品を持つウィンドウ
	CString		GetToolTip(UINT id) const;//ツールバーのボタンのツールチップ

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	struct PANE_ENTRY
	{
		int			idd;//中に収めるダイアログのリソースID
		CDialogPane	*pane;
	};

	CPIMPOMDlg				*p_main_dlg;//中央に置くメイン画面
	CForwardDlg				*p_info_dlg;//画素ルーペ（周辺拡大・画素値・座標）
	CForwardDlg				*p_output_dlg;//出力（メッセージ）
	HWND					h_receiver;//外部プログラムからのメッセージを受ける非表示ウィンドウ
	std::vector<PANE_ENTRY>	panes;//ドッキングパネルの一覧
	std::vector<int>		placed_windows;//初めて開くときに置き場所を決めた別ウィンドウ（リソースID）
	CPimpomToolBar			toolbar;
	CMFCStatusBar			statusbar;
	std::map<UINT, int>		res_images;//アイコンのリソースID → ツールバーの画像番号
	std::map<HICON, int>	icon_images;//アイコン → ツールバーの画像番号
	std::map<UINT, CString>	tool_tips;//ツールバーのコマンド → ツールチップ
	CString					status_texts[6];//ステータスバーに表示中の文字列

	CDialogPane*	find_pane(int idd);
	void			show_pane(CDialogPane *pane, bool activate);
	void			place_window(int idd, CDialog *p_dlg);
	void			fit_data_pane(CDialogPane *pane);
	CDialogPane*	create_pane(int idd, UINT pane_id, LPCTSTR caption, CSize size, DWORD align, bool closable);
	void			create_panes();
	void			create_receiver();
	void			create_toolbar();
	void			create_statusbar();
	void			sync_toolbar();
	void			sync_statusbar();
	int				toolbar_image_from_resource(UINT icon_id);
	int				toolbar_image_from_icon(HICON h_icon);
	bool			is_main_button(UINT id, bool *p_auto_check) const;

	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnDestroy();
	afx_msg void	OnTimer(UINT_PTR nIDEvent);
	afx_msg void	OnInitMenu(CMenu* pMenu);
	afx_msg void	OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg LRESULT	OnExtInput(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnExtOutput(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnExtProcess(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
