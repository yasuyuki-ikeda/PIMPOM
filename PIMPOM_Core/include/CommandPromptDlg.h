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
//CCommandPromptDlg
//スクリプトウィンドウ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#pragma once
#include "afxcmn.h"
#include "CalcDlg1.h"
#include "afxwin.h"



// *************************************
//         マ  ク  ロ   定   義         
// *************************************
#define COMMAND_LINE_MAX	5000
#define CONDITION_STACK_MAX	1000
#define UNDO_MAX	50


// *************************************
//         構  造  体   定   義         
// *************************************

//コマンドラインのインデックス
typedef struct COMMAND_LINE_STRUCT_{
	int		lineStartIndex;				//行開始インデックス
	int		lineLength;					//行の長さ
	int		lineStartIndexMultiChar;	//2バイト文字も1文字とカウントした場合の行開始インデックス
	int		lineLengthMultiChar;		//2バイト文字も1文字とカウントした場合の行の長さ
}COMMAND_LINE_STRUCT;


///*************************************
//         クラス   定   義         
///*************************************

class CCommandPromptDlg : public CDialog
{
	DECLARE_DYNAMIC(CCommandPromptDlg)

	enum { IDD = IDD_COMMAND_RPOMPT_DLG };// ダイアログ データ

public:
	CCommandPromptDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CCommandPromptDlg();
	bool OnBatchExecute(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param);

	

protected:
	CRichEditCtrl m_rich_edit;//スクリプトを記入するエディットボックス
	CRichEditCtrl m_rich_edit2;//メッセージを表示するエディットボックス
	CRichEditCtrl m_rich_edit3;//計算結果を表示するエディットボックス
	CRichEditCtrl m_rich_edit4;//行番号を表示するエディットボックス
	CListBox m_list_macro;
	CListCtrl m_var_list;
	CBitmap m_bmp_find;
	BATCH_INPUT			batchInBuffer;//バッチ実行パラメタ保存先(BatchExecuteから実行すると、上書きされる)
	BATCH_OUTPUT		batchOutBuffer;//バッチ実行パラメタ保存先(BatchExecuteから実行すると、上書きされる)
	CCalculate	m_calclate;//コマンドライン実行のクラス
	CString		m_filename;//スクリプトファイル名
	CString		m_macro_description[CALC_MACRO_CNT_MAX];//マクロの解説文
	bool		rich_edit_change_event_active_flg;//リッチエディットの変更イベントをアクティブにするかどうか
	bool		blockMap[CALC_COMMAND_LINE_MAX]; //blockMap  通過中の処理ブロック（if～endif, for～endfor）の状態を表すマップ。　行番号でアクセスするbool型配列の先頭ポインタ
												 //			当該ブロックの中を実行できる場合はtrue。スキップする場合はfalse
	char		*pUndoTbl[UNDO_MAX];//スクリプトの編集履歴を退避する場所
	int			undoID;//undo回数
	bool		brakePoints[CALC_COMMAND_LINE_MAX];//ブレークポイント

	virtual  void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	int		 get_command_lines(CString textBuffer, COMMAND_LINE_STRUCT commandLines[]);//全体を表す文字列から行ごとのインデックスを調べる
	void	 color_command_lines(bool isFirstRead=false);//マクロ／文字列／制御コードを色分けする
	void	 set_text_color(CRichEditCtrl	*pedit,int startMultiChar, int lenMultiChar, COLORREF color);//m_rich_editのテキスト色設定
	void	 show_line_number(int index=-1, bool enhanceColor = false);
	bool	 comp_string_value(CString textBuffer, int index, int *pLength, int *pLengthMultiChar);//textBufferが"で囲まれた文字列かどうか判定
	bool	 comp_variable_definition(CString textBuffer, int index, int *pLength);
	void	 show_macro_discription(int macroid);//マクロの説明表示
	bool	 step_execute(int *p_program_counter, BATCH_INPUT *p_batch_in = NULL, BATCH_OUTPUT *p_batch_out = NULL, bool fast = false, bool *pJumped=NULL);//ステップ実行
	void	 show_all_result();//全結果表示
	void	 arrange_dlg_item();
	bool	 load_macro_description();
	bool	 open_script_file(char *folder_path=NULL, char *file_name=NULL);
	void	 init_variable_watch();
	void	 show_variables();
	void	 log_text_data();
	void     change_break_points(char *pPrevTxt=NULL, char *pPrsentTxt=NULL);



//メッセージハンドラ
	DECLARE_MESSAGE_MAP()
public:
	virtual  BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonCommandPromptDo();
	afx_msg void OnEnChangeRicheditCommandPrompt();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEditCommandPromptProgramCounter();
	afx_msg void OnBnClickedButtonCommandPromptResultClear();
	afx_msg void OnBnClickedCheckCommandPromptDo();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCommandPromptMenuNew();
	afx_msg void OnCommandPromptMenuOpen();
	afx_msg void OnCommandPromptMenuSave();
	afx_msg void OnCommandPromptMenuSaveas();
	afx_msg void OnCommandPromptMenuStepExe();
	afx_msg void OnCommandPromptMenuSeqExe();
	afx_msg void OnCommandPromptMenuClear();
	afx_msg void OnLbnSelchangeListCommandPromptMacro();
	afx_msg void OnLbnDblclkListCommandPromptMacro();
	afx_msg void OnCommandPromptMenuSample();
	afx_msg void OnEnUpdateRicheditCommandPrompt();
	afx_msg void OnEnChangeEditCommandPromptMacro();
	afx_msg void OnBnClickedCheckCommandPromptFind();
	afx_msg void OnBnClickedButtonCommandPromptTestDo();
	afx_msg void OnCommandPromptMenuReference();
	afx_msg void OnBnClickedButtonCommandPromptUndo();
	afx_msg void OnBnClickedButtonCommandPromptRedo();
	afx_msg void OnDestroy();
};
