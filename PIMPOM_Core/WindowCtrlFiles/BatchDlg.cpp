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
//CBatchDlg
//
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "BatchDlg.h"
#include "pimpom_plot.h"
#include <process.h>

// *************************************
//         マ  ク  ロ   定   義         
// *************************************
#define EXEC_MODE_WAIT		(0)
#define EXEC_MODE_AUTO_EXEC		(1)
#define EXEC_MODE_SINGLE_EXEC		(2)

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


unsigned __stdcall BatchThread(void* pArg)
{
	CBatchDlg* pDlg = (CBatchDlg*)pArg;

	pDlg->BackGroundProc();

	return 0;
}



/********************************************************************
機  能  名  称 : 文字列を分割する
関    数    名 : split
引          数 : CString		str		(in)分割対象文字
                 CString		delim	(in)デリミタ
				 CStringArray	&result	(out)分割結果文字配列
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
static void split(CString str, CString delim, CStringArray &result)
{
  result.RemoveAll();
 
    int indexback = 0;
 
  CString st;
 
  int i;
  for(i = 0; i < str.GetLength(); i++)
  {
    if (str.GetAt(i) == delim.GetAt(0))
    {
      if ((i - indexback) == 0)
      {
        result.Add("");
      }
      else
      {
        st = str.Mid(indexback, i - indexback);
        result.Add(st);
      }
      indexback = i + 1;
    }
  }
 
  if ((i - indexback) != 0)
  {
    st = str.Mid(indexback, i - indexback);
    result.Add(st);
  }
 
}


//////////////////////////////////////////////////////////////////////////////////////////
//  メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CBatchDlg, CDialog)

CBatchDlg::CBatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBatchDlg::IDD, pParent)
{
	hThread = NULL;
}

CBatchDlg::~CBatchDlg()
{
	if(hThread)
		TerminateThread(hThread,0);
}

void CBatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BATCH, m_list_contol);
	DDX_Control(pDX, IDC_PROGRESS_BATCH, m_progress);
}




/********************************************************************
機  能  名  称 : １回バッチ実行する
関    数    名 : single_execute
引          数 : bool auto_exec_flg (in)実行後、リスト次の要素を選択するようにする
戻    り    値 : auto_exec_flg=trueのときは、リストの最後まで到達したらtrueを返す.それ以外はfalseを返す
                 バッチ実行のステータスがリトライを要求したらfalseを返す
				 バッチ実行のステータスが強制終了を要求したらfalseを返す
機          能 : リストで現在選択中の要素（画像、入力パラメタ）にて１回バッチ実行を行う
                 リスト要素が選択されていなければ最初の要素で実行する

				 バッチ実行とは、開かれているカスタムダイアログのバッチ実行イベントハンドラを呼び出すことである
				 バッチ実行イベントハンドラは出力値およびステータスを返すことができる。 
				 複数のカスタムダイアログが開かれていれば、各ダイアログのバッチ実行イベントハンドラを順次実行する
　　　　　　　　 複数のカスタムダイアログから出てきた出力値は、つなぎ合わせて表示される
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CBatchDlg::single_execute(bool auto_exec_flg)
{
	int file_no=0;
	bool retryState=false;
	bool stopState=false;
	BATCH_INPUT		in_param = { 0 };//入力パラメタ
	BATCH_OUTPUT	out_param = { 0 };//出力パラメタ：　複数のカスタムダイアログがある場合は、出力値を統合


	//入力パラメタをセットする
	in_param.AutoExecFlg = auto_exec_flg;
	GetBatchInput(&in_param, &file_no);


	//バッチ実行
	if (API.BatchExecute(&in_param, &out_param))
	{
		//出力パラメタをリストに表示する
		set_list_item(file_no, &in_param, &out_param);

		if (out_param.state == BATCH_EXEC_STATE_STOP)//中断要求
		{
			return true;
		}
		else if (out_param.state == BATCH_EXEC_STATE_RETRY)//リトライ要求
		{
			return false;
		}
	}



	if(auto_exec_flg )//一斉実行時
	{
		int next_num = file_no + 1;
		if(m_list_contol.GetItemCount()<=next_num)//リストの最後まで到達した
		{
			select_list_item(0);//リスト選択番号を最初に戻す
			return true;
		}
		else//リストの最後まで到達していない
		{
			select_list_item(next_num);//リスト選択番号をひとつ進める
			return false;
		}
	}
	else//単発実行時
	{
		return true;
	}
}

/********************************************************************
機  能  名  称 : ダイアログアイテムの表示状態変更
関    数    名 : show_dlg_item
引          数 : int executing_mode (in)連続実行中かどうか   0:待機状態 1:連続実行中  2:一回実行中
戻    り    値 : 
機          能 : 連続実行中かどうかで表示状態を変える
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CBatchDlg::show_dlg_item(int  executing_mode)
{
	if(executing_mode== EXEC_MODE_AUTO_EXEC){
		API.SetCheck(m_hWnd,IDC_CHECK_BATCH_EXEC,true);//連続実行ボタンのチェックを入れる
		::SetWindowText(::GetDlgItem(m_hWnd,IDC_CHECK_BATCH_EXEC), CString("中断"));
	}else {
		API.SetCheck(m_hWnd,IDC_CHECK_BATCH_EXEC,false);//連続実行ボタンのチェックをはずす
		::SetWindowText(::GetDlgItem(m_hWnd,IDC_CHECK_BATCH_EXEC), CString("一斉実行"));
	}


	::EnableWindow(::GetDlgItem(m_hWnd,IDC_LIST_BATCH), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_BATCH_SRC_IMAGE_NO), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_FILE_OPEN), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_FILE_DELETE), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_FILE_CLEAR), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_EXEC_SINGLE), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_EXEC_SINGLE_PREV), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_EXEC_SINGLE_NEXT), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_FILE_OUT), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_FILE_IN), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDCANCEL), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_BATCH_LOG_FILE_NAME), !executing_mode);

}

/********************************************************************
機  能  名  称 : リスト初期化
関    数    名 : init_list
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::init_list()
{
    LVCOLUMN    lvc;
    char       caption[32];

	m_list_contol.SetExtendedStyle(LVS_EX_FULLROWSELECT);

    lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // 有効フラグ

	//ID
	sprintf(caption, "ID");
    lvc.iSubItem    = 0;            // サブアイテム番号
    lvc.pszText     = caption;   // 見出しテキスト
    lvc.cx          = 40;          // 横幅
    m_list_contol.InsertColumn(0, &lvc);	

	//ファイル名
	sprintf(caption, "画像ファイル");
    lvc.iSubItem    = 1;            // サブアイテム番号
    lvc.pszText     = caption;   // 見出しテキスト
    lvc.cx          = 500;          // 横幅
    m_list_contol.InsertColumn(1, &lvc);

	//入力パラメタ
    for (int i = 0; i < BATCH_IN_PARAM_NUM ; i++)
    {
		sprintf(caption, "IN%d",i);
        lvc.iSubItem    = i + 2;            // サブアイテム番号
        lvc.pszText     = caption;   // 見出しテキスト
        lvc.cx          = 30;          // 横幅
        m_list_contol.InsertColumn(i + 2, &lvc);
    }

	//出力パラメタ
    for (int i = 0; i < BATCH_OUT_PARAM_NUM; i++)
    {
		sprintf(caption, "OUT%d",i);
        lvc.iSubItem    = i + 2 + BATCH_IN_PARAM_NUM;            // サブアイテム番号
        lvc.pszText     = caption;   // 見出しテキスト
        lvc.cx          = 30;          // 横幅
        m_list_contol.InsertColumn(i + 2 + BATCH_IN_PARAM_NUM, &lvc);
    }
}

/********************************************************************
機  能  名  称 : リストに要素追加
関    数    名 : add_list_item
引          数 : BATCH_INPUT *in_param	  (in)バッチ実行の入力パラメタ
				 BATCH_OUTPUT *out_param  (in)バッチ実行の出力パラメタ
                 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::add_list_item(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param )
{
	LVITEM       lvi={0};
	int			index;
	char		str[1024]={0};
	
	lvi.mask = LVIF_TEXT;

	//ID
	lvi.iItem = m_list_contol.GetItemCount();
	lvi.iSubItem = 0;
	sprintf_s(str, 1024, "%05d", lvi.iItem);
	lvi.pszText = str;
	index = m_list_contol.InsertItem(&lvi);
	

	//画像ファイル名
	//lvi.iItem = n;
	lvi.iSubItem = 1;
	lvi.pszText = in_param->imae_file_path;
	m_list_contol.SetItem(&lvi);

	//パラメタ
	set_list_item(index, in_param, out_param);
}

/********************************************************************
機  能  名  称 : リストの値変更
関    数    名 : set_list_item
引          数 : int n (in)リストの要素番号
				 BATCH_INPUT *in_param	  (in)バッチ実行の入力パラメタ
				 BATCH_OUTPUT *out_param  (in)バッチ実行の出力パラメタ
                 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::set_list_item(int n, BATCH_INPUT *in_param, BATCH_OUTPUT *out_param)
{
	LVITEM       lvi={0};
	lvi.mask = LVIF_TEXT;


	//入力パラメタ
    for (int i = 0; i < BATCH_IN_PARAM_NUM; i++)
    {
		lvi.iItem = n;
		lvi.iSubItem = i+2;
		lvi.pszText = in_param->param[i];
		m_list_contol.SetItem(&lvi);
	}

	//出力パラメタ
	if(out_param!=NULL){
		for (int i = 0; i < BATCH_OUT_PARAM_NUM; i++)
		{
			lvi.iItem = n;
			lvi.iSubItem = i+2+BATCH_IN_PARAM_NUM;
			
			lvi.pszText = out_param->result[i];
			m_list_contol.SetItem(&lvi);
		}
	}
}

/********************************************************************
機  能  名  称 : リストの選択中の要素番号を返す
関    数    名 : get_selected_list_item
引          数 : 
戻    り    値 : リストの選択中の要素番号
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int	CBatchDlg::get_selected_list_item()
{
	return m_list_contol.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
}

/********************************************************************
機  能  名  称 : リストの要素を選択する
関    数    名 : select_list_item
引          数 : int n (in)リストの要素番号
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::select_list_item(int n)
{
	m_list_contol.SetItemState(n, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_list_contol.SetSelectionMark(n);
	m_list_contol.EnsureVisible(n,true);
}

/********************************************************************
機  能  名  称 : csvファイルから入出力パラメタ一覧を読み込む
関    数    名 : load_image_list
引          数 : CString filename (in)csvファイルのパス
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CBatchDlg::load_image_list(CString filename, char splitter)
{
	bool	ret=true;
	FILE *fp = fopen(filename,"rt");
	if(fp){
		for(;;){
			char buffer[5000]={0};
			if(fgets(buffer, 5000, fp)!=NULL)//txtファイルから１行読み取り
			{
				BATCH_INPUT in_param={0};
				BATCH_OUTPUT out_param={0};
				CStringArray params;

				if(buffer[strlen(buffer)-1]=='\n'){//fgetsで入力された改行文字を消す
					buffer[strlen(buffer)-1]=0;
				}

				//csvファイルから読み取った行を分割
				split(CString(buffer), CString(splitter), params);

				//画像ファイル名
				{
					CString str = params.GetAt(0);
					strcpy(in_param.imae_file_path, str.GetBuffer());
					str.ReleaseBuffer();
				}

				//入力パラメタ
				for(int n=1 ; n<params.GetCount() && n-1<BATCH_IN_PARAM_NUM ; n++){
					CString str = params.GetAt(n);
					strcpy(in_param.param[n-1], str.GetBuffer());
					str.ReleaseBuffer();
				}

				//出力パラメタ
				for(int n=1+BATCH_IN_PARAM_NUM ; n<params.GetCount() && n-1-BATCH_IN_PARAM_NUM<BATCH_OUT_PARAM_NUM ; n++){
					CString str = params.GetAt(n);
					strcpy(out_param.result[n-1-BATCH_IN_PARAM_NUM], str.GetBuffer());
					str.ReleaseBuffer();
				}

				add_list_item(&in_param,&out_param);//リストに追加
			}else{
				ret = false;
				break;
			}
		}
		fclose(fp);
	}	

	return ret;
}

/********************************************************************
機  能  名  称 : リスト要素ソートのための比較関数
関    数    名 : compare_func
引          数 : 
戻    り    値 : 
機          能 : 比較対象が両方とも数値ならば、大きさで比較
　　　　　　　　それ以外の場合は文字列の順で比較　　　　　
		 日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CALLBACK CBatchDlg::compare_func(LPARAM param1, LPARAM param2, LPARAM param3)
{
	CBatchDlg* pDlg = (CBatchDlg*)param3;
	int nSubItem  = pDlg->m_iSubItem;
	CString    strItem1 = pDlg->m_list_contol.GetItemText(param1, nSubItem);
	CString    strItem2 = pDlg->m_list_contol.GetItemText(param2, nSubItem);

	double numItem1 = atof(strItem1);
	double numItem2 = atof(strItem2);
	if(numItem1!=0.0 && numItem2!=0.0)//数値どうしの比較
	{
		if(pDlg->m_sort_dir_flg)	
			return (numItem1>numItem2);
		else
			return (numItem1<numItem2);
	}
	else//文字列どうしの比較
	{
		if(pDlg->m_sort_dir_flg)
			return strcmp(strItem1, strItem2);
		else
			return strcmp(strItem2, strItem1);
	}
}


/********************************************************************
機  能  名  称 : ダイアログアイテムの移動
関    数    名 : arrange_dlg_item
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::arrange_dlg_item()
{
	CRect clientRct={0}, listRct = { 0 };

	::GetClientRect(m_hWnd, &clientRct);//クライアント領域を表す矩形を取得

	
	API.GetDialogItemRect(m_hWnd, IDC_LIST_BATCH, &listRct);
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_LIST_BATCH)  , listRct.left, listRct.top, clientRct.right-clientRct.left-10, clientRct.bottom-clientRct.top-120, true);//リストコントロール
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_STATIC_BATCH),    0, 0, clientRct.right-clientRct.left-2, clientRct.bottom-clientRct.top-60, true);//ｸﾞﾙｰﾌﾟボックス

	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_STATIC_BATCH_LOGFILEPATH) , 5   , clientRct.bottom-clientRct.top-60 , 70, 20, true);//「ルートパス」キャプション
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_EDIT_BATCH_LOG_FILE_NAME) , 90 , clientRct.bottom-clientRct.top-60 , 350, 20, true);//ルートパスを入れるｴﾃﾞｨｯﾄﾎﾞｯｸｽ
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_BATCH_ROOT), 440, clientRct.bottom - clientRct.top - 60, 20, 20, true);//ルートパスブラウズボタン
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_CHANGE_EXECUTE_RULE), 480, clientRct.bottom - clientRct.top - 60, 60, 20, true);//「実行ルール」ボタン
	
	



	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_EXEC_SINGLE)     , 5  , clientRct.bottom-clientRct.top-30, 70, 20, true);
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_EXEC_SINGLE_PREV), 75 , clientRct.bottom-clientRct.top-30, 30, 20, true);
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_EXEC_SINGLE_NEXT), 105, clientRct.bottom-clientRct.top-30, 30, 20, true);
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_CHECK_BATCH_EXEC)             , 135, clientRct.bottom-clientRct.top-30, 70, 20, true);
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_CHECK_BATCH_EXEC)             , 135, clientRct.bottom-clientRct.top-30, 70, 20, true);
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_PROGRESS_BATCH)               , 210, clientRct.bottom-clientRct.top-30, 330, 20, true);
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_EDIT_BATCH_PROGRESS)          , 540, clientRct.bottom-clientRct.top-30, 20, 20, true);
	::MoveWindow( ::GetDlgItem(m_hWnd,IDCANCEL)                         , clientRct.right-clientRct.left-80, clientRct.bottom-clientRct.top-30, 70, 20, true);
}

//////////////////////////////////////////////////////////////////////////////////////////
//  メッセージ ハンドラ
//////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CBatchDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_FILE_OPEN, &CBatchDlg::OnBnClickedButtonBatchFileOpen)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_FILE_DELETE, &CBatchDlg::OnBnClickedButtonBatchFileDelete)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_FILE_CLEAR, &CBatchDlg::OnBnClickedButtonBatchFileClear)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_EXEC_SINGLE, &CBatchDlg::OnBnClickedBatchExecSingle)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_BATCH_EXEC_SINGLE_PREV, &CBatchDlg::OnBnClickedBatchExecSinglePrev)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_EXEC_SINGLE_NEXT, &CBatchDlg::OnBnClickedBatchExecSingleNext)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_FILE_OUT, &CBatchDlg::OnBnClickedButtonBatchFileOut)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_FILE_IN, &CBatchDlg::OnBnClickedButtonBatchFileIn)
	ON_WM_DROPFILES()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BATCH, &CBatchDlg::OnNMDblclkListBatch)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_BATCH, &CBatchDlg::OnLvnColumnclickListBatch)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_BATCH, &CBatchDlg::OnNMRClickListBatch)
	ON_BN_CLICKED(IDC_CHECK_BATCH_EXEC, &CBatchDlg::OnBnClickedCheckBatchExec)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_BATCH_ROOT, &CBatchDlg::OnBnClickedButtonBatchRoot)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_EXECUTE_RULE, &CBatchDlg::OnBnClickedButtonChangeExecuteRule)
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
BOOL CBatchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//ドラッグ＆ドロップ対応
	DragAcceptFiles();

	//ダイアログアイテムの初期化
    init_list();        // リストコントロール初期化
	API.SetEditValue(m_hWnd, IDC_EDIT_BATCH_SRC_IMAGE_NO, 0);
	API.SetEditValue(m_hWnd, IDC_EDIT_BATCH_ROOT, "");

	arrange_dlg_item();


	UINT thrID = 0;
	hThread = (HANDLE)::_beginthreadex(NULL, 0, &BatchThread, this, 0, &thrID);


	show_dlg_item(EXEC_MODE_WAIT);//ダイアログアイテムの表示
	m_sort_dir_flg = true;

	return true;
}

/********************************************************************
機  能  名  称 : このウィンドウを表示状態にする
関    数    名 : OnShowWindow
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);


}

/********************************************************************
機  能  名  称 : ドラッグアンドドロップ
関    数    名 : OnDropFiles
引          数 : 
戻    り    値 : 
機          能 : 画像またはcsvがドロップされたらアクション
　　　　　　　　画像：画像ファイルのパスをリストに追加
		　　　　csv：入力アイテム一覧として読み込む
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::OnDropFiles(HDROP hDropInfo)
{
	int		file_num = (int)DragQueryFileA( hDropInfo, -1, NULL, 0);//ドロップされたファイル数を取得

	for(int i=0; i<file_num ; i++)
	{
		UINT size = DragQueryFile( hDropInfo, i, NULL, 0) + 1;//ファイル名の長さを取得

		if(size>3)
		{
			CString filename;
			DragQueryFile( hDropInfo, i , filename.GetBuffer(size) ,size );
			filename.ReleaseBuffer();

			if(filename.Right(3).MakeLower()==_T("csv") )//csvの場合はファイル内に書かれている画像一覧を読み出してリストに入れる
			{
				load_image_list(filename);
			}
			else if (filename.Right(3).MakeLower() == _T("txt"))//txtの場合は、区切り文字を選択してリストに入れる
			{
				CConfirmDlg dlg;
				bool splitterType[] = { true, false , false};
				dlg.SetTitle("区切り文字");
				dlg.RegistRadioVar("カンマ(,)", &splitterType[0]);
				dlg.RegistRadioVar("スペース", &splitterType[1]);
				dlg.RegistRadioVar("コロン(:)", &splitterType[2]);
				if (dlg.DoModal() == IDOK){
					if (splitterType[0]){
						load_image_list(filename, ',');
					}
					else if (splitterType[1])
					{
						load_image_list(filename, ' ');
					}
					else{
						load_image_list(filename, ':');
					}
				}
			}
			else//それ以外の場合はファイル名をそのままリストに入れる
			{
				BATCH_INPUT in_param={0};
				strcpy(in_param.imae_file_path, filename.GetBuffer(size) );
				filename.ReleaseBuffer();
				add_list_item(&in_param);
			}
		}	
	}
	DragFinish(hDropInfo);
}


/********************************************************************
機  能  名  称 : 「一斉実行」ボタン押下
関    数    名 : OnBnClickedCheckBatchExec
引          数 : 
戻    り    値 : 
機          能 : 連続実行中でなければ、連続実行を開始する。
　　　　　　　　　連続実行中ならば、連続実行を中断する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::OnBnClickedCheckBatchExec()
{
	if(m_list_contol.GetItemCount()==0)	return;

	if(API.GetBGProcID()==BGPROC_BATCH_EXC)//一斉実行中断
	{
		API.StopBGProc();
		show_dlg_item(EXEC_MODE_WAIT);//ダイアログアイテムの表示状態変更
		m_progress.SetPos(0);//プログレスバーをゼロにもどす
		API.SetEditValue(m_hWnd, IDC_EDIT_BATCH_PROGRESS,_T(""));
	}
	else if(API.GetBGProcID()==BGPROC_NOTHING)//一斉実行開始
	{
		select_list_item(0);//最初の画像ファイルを選択
		API.SetBGProcID(BGPROC_BATCH_EXC);
		show_dlg_item(EXEC_MODE_AUTO_EXEC);//ダイアログアイテムの表示状態変更
		m_progress.SetRange32(0, m_list_contol.GetItemCount()-1);//プログレスバーをセットする
	}
}

/********************************************************************
機  能  名  称 : 「選択実行」ボタン押下
関    数    名 : OnBnClickedBatchExecSingle
引          数 : 
戻    り    値 : 
機          能 : リストで現在選択中のパラメタで１回バッチ実行する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::OnBnClickedBatchExecSingle()
{
	if(m_list_contol.GetItemCount()==0)	return;
	
	show_dlg_item(EXEC_MODE_SINGLE_EXEC);
	API.SetBGProcID(BGPROC_BATCH_EXC);
}

/********************************************************************
機  能  名  称 : 「<」ボタン押下
関    数    名 : OnBnClickedBatchExecSinglePrev
引          数 : 
戻    り    値 : 
機          能 : リストで現在選択中の要素を一つ戻し、そのパラメタで１回バッチ実行する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::OnBnClickedBatchExecSinglePrev()
{
	if(m_list_contol.GetItemCount()==0)	return;

	int sel = get_selected_list_item();
	if(sel<=0){
		select_list_item(0);
	}else{
		select_list_item(sel-1);
	}

	show_dlg_item(EXEC_MODE_SINGLE_EXEC);
	API.SetBGProcID(BGPROC_BATCH_EXC);
}

/********************************************************************
機  能  名  称 : 「>」ボタン押下
関    数    名 : OnBnClickedBatchExecSingleNext
引          数 : 
戻    り    値 : 
機          能 : リストで現在選択中の要素を一つ進め、そのパラメタで１回バッチ実行する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::OnBnClickedBatchExecSingleNext()
{
	if(m_list_contol.GetItemCount()==0)	return;

	int sel = get_selected_list_item();
	if(sel<0 || sel==m_list_contol.GetItemCount()-1){
		select_list_item(0);
	}else{
		select_list_item(sel+1);
	}

	show_dlg_item(EXEC_MODE_SINGLE_EXEC);
	API.SetBGProcID(BGPROC_BATCH_EXC);
}

/********************************************************************
機  能  名  称 : 「画像ファイルを開く」ボタン押下
関    数    名 : OnBnClickedButtonBatchFileOpen
引          数 : 
戻    り    値 : 
機          能 : ダイアログを開き、そこで選択された画像ファイルのパスをリストへ追加する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::OnBnClickedButtonBatchFileOpen()
{
	int			file_number;
	CString		path_name, file_name[10000], path;
	CString		file_type_name[13], all_file_type;


	//入力可能なファイル名
	file_type_name[12].LoadString(IDS_FILETYPE_ANY);//*.*
	file_type_name[0].LoadString(IDS_FILETYPE_BITMAP);//*.bmp
	file_type_name[1].LoadString(IDS_FILETYPE_RAW);//*.raw
	file_type_name[2].LoadString(IDS_FILETYPE_JPEG);//*.jpg
	file_type_name[3].LoadString(IDS_FILETYPE_WORD);//*.word
	file_type_name[4].LoadString(IDS_FILETYPE_LONG);//*.long
	file_type_name[5].LoadString(IDS_FILETYPE_FLOAT);//*.float
	file_type_name[6].LoadString(IDS_FILETYPE_COMPLEX);//*.comp
	file_type_name[7].LoadString(IDS_FILETYPE_XY);//*.xy
	file_type_name[8].LoadString(IDS_FILETYPE_XYZ);//*.xyz
	file_type_name[9].LoadString(IDS_FILETYPE_3D);//*.3d
	file_type_name[10].LoadString(IDS_FILETYPE_F3D);//*.f3d
	file_type_name[11].LoadString(IDS_FILETYPE_ARCHIVED);//*.pim
	
	all_file_type.Empty();
	for(int i=0 ; i<13 ; i++){
		all_file_type += file_type_name[i];
	}
	all_file_type += _T("|");

	if( !API.OpenFileDialog(10000,all_file_type, &path, file_name, &file_number) )	return;

	for(int i=0 ; i<file_number ; i++)
	{
		if(file_name[i].GetLength()<3)	return;//ファイル名短すぎ

		BATCH_INPUT in_param={0};
		CString str = path + file_name[i];
		strcpy(in_param.imae_file_path, str.GetBuffer());
		str.ReleaseBuffer();
		add_list_item(&in_param);
	}
}

/********************************************************************
機  能  名  称 : 「削除」ボタン押下
関    数    名 : OnBnClickedButtonBatchFileDelete
引          数 : 
戻    り    値 : 
機          能 : リストで現在選択中の要素を削除する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::OnBnClickedButtonBatchFileDelete()
{
	m_list_contol.DeleteItem( get_selected_list_item());
}

/********************************************************************
機  能  名  称 : 「クリア」ボタン押下
関    数    名 : OnBnClickedButtonBatchFileClear
引          数 : 
戻    り    値 : 
機          能 : リストクリア
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::OnBnClickedButtonBatchFileClear()
{
	m_list_contol.DeleteAllItems();
}



/********************************************************************
機  能  名  称 : 入力パラメタ取得
関    数    名 : GetBatchInput
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::GetBatchInput(BATCH_INPUT *p_in_param, int *p_file_no)
{
	int file_no = 0;
	int image_num = 0;
	CString path = "";
	CString rootpath = "";

	//現在選択中のファイルリスト番号を取得
	file_no = get_selected_list_item();
	if (file_no<0) {//選択してなければ最初の要素を選択
		file_no = 0;
		select_list_item(file_no);
	}


	API.GetEditValue(m_hWnd, IDC_EDIT_BATCH_ROOT, &rootpath);

	path = rootpath + m_list_contol.GetItemText(file_no, 1);//対象画像ファイル
	API.GetEditValue(m_hWnd, IDC_EDIT_BATCH_SRC_IMAGE_NO, &image_num);//対象画像格納先の画像メモリ番号

	strcpy(p_in_param->imae_file_path, path.GetBuffer());	path.ReleaseBuffer();
	p_in_param->image_number = image_num;

	for (int n = 0; n < BATCH_IN_PARAM_NUM; n++) {
		CString str = m_list_contol.GetItemText(file_no, n + 2);
		strcpy(p_in_param->param[n], str.GetBuffer());	str.ReleaseBuffer();
	}

	if (p_file_no)
	{
		*p_file_no = file_no;
	}
}


/********************************************************************
機  能  名  称 : 入力・出力パラメタ設定
関    数    名 : SetBatchInOut
引          数 :
戻    り    値 :
機          能 : 
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::SetBatchInOut(BATCH_INPUT *p_in_param, BATCH_OUTPUT *p_out_param)
{
	//現在選択中のファイルリスト番号を取得
	int file_no = get_selected_list_item();
	if (file_no<0) {//選択してなければ最初の要素を選択
		file_no = 0;
		select_list_item(file_no);
	}

	set_list_item(file_no, p_in_param, p_out_param);
}

/********************************************************************
機  能  名  称 : バックグラウンド処理
関    数    名 : BackGroundProc
引          数 : 
戻    り    値 : 
機          能 : コマンドをバックグラウンドで実行
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::BackGroundProc()
{
	for (;;)
	{
		switch (API.GetBGProcID())
		{
		case BGPROC_CAPTURING://カメラスルー中
			break;

		case BGPROC_MOVIE://動画表示中
			break;

		case BGPROC_BATCH_EXC://バッチ処理中

			if (API.GetCheck(m_hWnd, IDC_CHECK_BATCH_EXEC))
			{
				//連続実行
				if (single_execute(true))
				{
					//リストの最後まで到達した→連続実行終了

					API.StopBGProc();
					show_dlg_item(EXEC_MODE_WAIT);//ダイアログアイテムの表示状態変更
					m_progress.SetPos(0);//プログレスバーをゼロにもどす
					API.SetEditValue(m_hWnd, IDC_EDIT_BATCH_PROGRESS, _T(""));
				}
				else
				{
					//リストの最後まで到達していない→連続実行継続

					m_progress.SetPos(get_selected_list_item());//プログレスバーをすすめる
					API.SetEditValue(m_hWnd, IDC_EDIT_BATCH_PROGRESS, (get_selected_list_item() + 1) * 100 / m_list_contol.GetItemCount());
				}
			}
			else
			{
				//単発実行
				if (single_execute(false)) {
					//リトライ要求なし→終了
					API.StopBGProc();
					show_dlg_item(EXEC_MODE_WAIT);//ダイアログアイテムの表示状態変更
				}
			}

			break;
		}

		Sleep(10);
	}

}



/********************************************************************
機  能  名  称 : 「csvに出力」ボタン押下
関    数    名 : OnBnClickedButtonBatchFileOut
引          数 : 
戻    り    値 : 
機          能 : リストの内容（入出力パラメタ一覧）をcsvファイルに出力する
　　　　　　　　　出力先を選ぶためのファイルダイアログを出す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::OnBnClickedButtonBatchFileOut()
{
	CFileDialog myDLG(false, _T("csv"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
							"画像ファイルリスト(*.csv)|*.CSV|テキスト(*.txt)|*.txt||");
	if( myDLG.DoModal() != IDOK )	return	;

	FILE *fp = fopen(myDLG.GetPathName(),"wt");
	if(fp){
		for(int i=0 ; i<m_list_contol.GetItemCount() ; i++)
		{
			//画像ファイル名
			CString path = m_list_contol.GetItemText(i, 1);
			fprintf(fp, "%s",path.GetBuffer());
			path.ReleaseBuffer();

			//入力パラメタ
			for (int n = 0; n < BATCH_IN_PARAM_NUM; n++){
				CString str = m_list_contol.GetItemText(i, n+2);
				fprintf(fp, ",%s",str.GetBuffer());
				str.ReleaseBuffer();
			}

			//出力パラメタ
			for (int n = 0; n < BATCH_OUT_PARAM_NUM; n++){
				CString str = m_list_contol.GetItemText(i, n+2+BATCH_IN_PARAM_NUM);
				fprintf(fp, ",%s",str.GetBuffer());
				str.ReleaseBuffer();
			}
			fprintf(fp, "\n");
		}
		fclose(fp);
	}
}

/********************************************************************
機  能  名  称 : 「csvから入力」ボタン押下
関    数    名 : OnBnClickedButtonBatchFileIn
引          数 : 
戻    り    値 : 
機          能 : リストの内容（入出力パラメタ一覧）をcsvファイルから入力する
　　　　　　　　　出力先を選ぶためのファイルダイアログを出す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::OnBnClickedButtonBatchFileIn()
{
	CFileDialog myDLG(true, _T("csv"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
							"画像ファイルリスト(*.csv)|*.CSV|テキスト(*.txt)|*.txt||");
	if( myDLG.DoModal() != IDOK )	return	;

	load_image_list(myDLG.GetPathName());
}

/********************************************************************
機  能  名  称 : リスト項目をダブルクリック
関    数    名 : OnNMDblclkListBatch
引          数 : 
戻    り    値 : 
機          能 : 実行
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::OnNMDblclkListBatch(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	if(m_list_contol.GetItemCount()==0)	return;

	show_dlg_item(EXEC_MODE_SINGLE_EXEC);
	API.SetBGProcID(BGPROC_BATCH_EXC);


	*pResult = 0;
}

/********************************************************************
機  能  名  称 : リストの見出し部を押下
関    数    名 : OnLvnColumnclickListBatch
引          数 : 
戻    り    値 : 
機          能 : その項目でリスト要素をソートする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::OnLvnColumnclickListBatch(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	m_sort_dir_flg = !m_sort_dir_flg;//ソート方向を切り替える
	m_iSubItem = pNMLV->iSubItem;//リストの項目番号
	m_list_contol.SortItems( compare_func, (LPARAM)this );

	for ( int i = 0; i < m_list_contol.GetItemCount(); i++ ){
			m_list_contol.SetItemData( i, (DWORD)i );
	}

	*pResult = 0;
}


/********************************************************************
機  能  名  称 : リスト項目を右クリック
関    数    名 : OnNMRClickListBatch
引          数 : 
戻    り    値 : 
機          能 : 入力パラメタの編集
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::OnNMRClickListBatch(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	
	BATCH_INPUT in_param={0};
	CString param_val[BATCH_IN_PARAM_NUM];
	CConfirmDlg dlg;
	int selected_num = get_selected_list_item();
	if(selected_num<0)	return;

	//入力パラメタを設定するダイアログを出す
	dlg.SetTitle(_T("入力パラメタ"));
	dlg.SetWidth(50,400);
	for(int i=0 ; i<BATCH_IN_PARAM_NUM ; i++)
	{
		param_val[i] = m_list_contol.GetItemText(selected_num, i+2);	

		CString title;
		title.Format("入力%d", i);
		dlg.RegistVar(title, &param_val[i]);
	}

	if(dlg.DoModal()!=IDOK)	return;

	//パラメタをリストに設定する
	for(int i=0 ; i<BATCH_IN_PARAM_NUM ; i++)
	{
		strcpy( in_param.param[i] , param_val[i].GetBuffer() );
		param_val[i].ReleaseBuffer();
	}
	set_list_item(selected_num, &in_param);


	*pResult = 0;
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
void CBatchDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	
	arrange_dlg_item();
}



/********************************************************************
機  能  名  称 : ルートパスブラウズボタン押下
関    数    名 : OnBnClickedButtonBatchRoot
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::OnBnClickedButtonBatchRoot()
{
	CString rootFolder;
	if (API.FolderDialog(&rootFolder)){
		API.SetEditValue(m_hWnd, IDC_EDIT_BATCH_ROOT, rootFolder);
	}
}

/********************************************************************
機  能  名  称 : 「実行ルール」ボタン押下
関    数    名 : OnBnClickedButtonChangeExecuteRule
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CBatchDlg::OnBnClickedButtonChangeExecuteRule()
{
	CConfirmDlg dlg;
	bool rule[3] = {true,false,false};

	for (int i = 0; i < 3; i++){
		rule[i] = (i == API.batch_execute_rule);
	}

	dlg.SetTitle("バッチ実行ルール");
	dlg.SetWidth(300, 30);
	dlg.RegistRadioVar("スクリプト -> カスタムファンクション(0,1,2,,,)", &rule[0]);
	dlg.RegistRadioVar("カスタムファンクション(0,1,2,,,) -> スクリプト ", &rule[1]);
	dlg.RegistRadioVar("スクリプト のみ", &rule[2]);

	if (dlg.DoModal() == IDOK){
		for (int i = 0; i < 3; i++){
			if (rule[i]){
				API.batch_execute_rule = i;
				break;
			}
		}
	}
}
