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
//CPimpomAPI
//ダイアログリソースとのデータのやり取り
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"

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

// *************************************
//         関    数    参    照         
// *************************************


///////////////////////////////////////////////////////////////////////////////////
//ダイアログリソースから値をやりとりする		
///////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : ダイアログアイテムの領域を取得する
関    数    名 : GetEditValue
引          数 : HWND			h_wnd		(in)ダイアログアイテムが存在するウィンドウのハンドル
                 UINT			id			(in)ダイアログアイテムのリソースＩＤ
                 RECT			*pRct		(out)取得した領域（クライアント座標）
戻    り    値 : 成功したらtrue
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::GetDialogItemRect(HWND h_wnd, UINT id, CRect *pRct)
{
	HWND		h_item;
	CRect		rgn;
	CPoint		pnt;

	h_item = (HWND)::GetDlgItem(h_wnd, id);//ダイアログアイテムがのポインタを取得する
	if (h_item == NULL)		return	false;//ポインタ取得に失敗

	
	::GetWindowRect(h_item, &rgn);

	pnt.x = rgn.left;
	pnt.y = rgn.top;
	::ScreenToClient(h_wnd, &pnt);

	pRct->left = pnt.x;
	pRct->top = pnt.y;
	pRct->right = rgn.right - rgn.left + pnt.x;
	pRct->bottom = rgn.bottom - rgn.top + pnt.y;

	return true;
}


/********************************************************************
機  能  名  称 : ダイアログアイテムの領域を設定する
関    数    名 : GetEditValue
引          数 : HWND			h_wnd		(in)ダイアログアイテムが存在するウィンドウのハンドル
                 UINT			id			(in)ダイアログアイテムのリソースＩＤ
                 RECT			*pRct		(in)設定する領域（クライアント座標）
戻    り    値 : 成功したらtrue
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SetDialogItemRect(HWND h_wnd, UINT id, CRect Rct)
{
	HWND		h_item;
	CPoint		pnt;

	h_item = (HWND)::GetDlgItem(h_wnd, id);//ダイアログアイテムがのポインタを取得する
	if (h_item == NULL)		return	false;//ポインタ取得に失敗

	pnt.x = Rct.left;
	pnt.y = Rct.top;
	::ClientToScreen(h_wnd, &pnt);

	::MoveWindow(h_item, pnt.x, pnt.y, Rct.Width(), Rct.Height(),true);

	return true;
}



/********************************************************************
機  能  名  称 : エディットボックスから数値を取得する(double)
関    数    名 : GetEditValue
引          数 : HWND			h_wnd		(in)エディットボックスが存在するウィンドウのハンドル
				 UINT			id			(in)エディットボックスのリソースＩＤ
				 double			*value		(out)取得した数値
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::GetEditValue(HWND h_wnd, UINT id , double *value)
{
	char		str[100];
	long		len;
	HWND		h_edit; 
	
	h_edit = (HWND)::GetDlgItem( h_wnd , id );//エディットボックスのポインタを取得する
		if(h_edit == NULL)		return	false;//ポインタ取得に失敗

	len = ::GetWindowText(h_edit, str, 100);//エディットボックス内の文字列を取得する
		if(len == 0)	return	false;//エディットボックスになにも入ってない場合は値をかえさない

	return ToNumber(str, value);

}

/********************************************************************
機  能  名  称 : エディットボックスから数値を取得する(float)
関    数    名 : GetEditValue
引          数 : HWND			h_wnd		(in)エディットボックスが存在するウィンドウのハンドル
				 UINT			id			(in)エディットボックスのリソースＩＤ
				 float			*value		(out)取得した数値
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::GetEditValue(HWND h_wnd, UINT id , float *value)
{
	double  tmp;
		if( !GetEditValue(h_wnd, id, &tmp) )	return false;
	*value = (float)tmp;
	return true;
}

/********************************************************************
機  能  名  称 : エディットボックスから数値を取得する(short)
関    数    名 : GetEditValue
引          数 : HWND			h_wnd		(in)エディットボックスが存在するウィンドウのハンドル
				 UINT			id			(in)エディットボックスのリソースＩＤ
				 short			*value		(out)取得した数値
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::GetEditValue(HWND h_wnd, UINT id , short *value)
{
	double  tmp;
		if( !GetEditValue(h_wnd, id, &tmp) )	return false;
	*value = (short)tmp;
	return	true;
}

/********************************************************************
機  能  名  称 : エディットボックスから数値を取得する(int)
関    数    名 : GetEditValue
引          数 : HWND			h_wnd		(in)エディットボックスが存在するウィンドウのハンドル
				 UINT			id			(in)エディットボックスのリソースＩＤ
				 int			*value		(out)取得した数値
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::GetEditValue(HWND h_wnd, UINT id , int *value)
{
	double  tmp;
		if( !GetEditValue(h_wnd, id, &tmp) )	return false;
	*value = (int)tmp;
	return	true;
}

/********************************************************************
機  能  名  称 : エディットボックスからデータ(CString)を取り出す
関    数    名 : GetEditValue
引          数 : HWND			h_wnd		(in)エディットボックスが存在するウィンドウのハンドル
				 UINT			id			(in)エディットボックスのリソースＩＤ
				 CString		*str		(out)取得した文字列
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::GetEditValue(HWND h_wnd, UINT id , CString *str)
{
	HWND		h_edit; 
	CWnd		*p_wnd;
	
	h_edit = (HWND)::GetDlgItem( h_wnd , id );//エディットボックスのポインタを取得する
		if(h_edit == NULL)		return	false;//ポインタ取得に失敗

	p_wnd = CWnd::FromHandle( h_edit );
	p_wnd->GetWindowText(*str);//エディットボックス内の文字列を取得する
	return	true;
}

/********************************************************************
機  能  名  称 : チェックボックスからチェック状態を取得する
関    数    名 : GetCheck
引          数 : HWND			h_wnd		(in)チェックボックスが存在するウィンドウのハンドル
				 UINT			id			(in)チェックボックスのリソースＩＤ
戻    り    値 : チェックされていたらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::GetCheck(HWND h_wnd, UINT id )
{
	HWND	h_button;
	CButton	*p_button;

	h_button = (HWND)::GetDlgItem( h_wnd, id );//ボタンのポインタを取得する
		if(h_button == NULL)		return false;

	p_button = (CButton*)CWnd::FromHandle(h_button);
	return	(p_button->GetCheck() == 1) ? true:false;
}

/********************************************************************
機  能  名  称 : エディットボックスに数値を設定する
関    数    名 : SetEditValue
引          数 : HWND			h_wnd		(in)エディットボックスが存在するウィンドウのハンドル
				 UINT			id			(in)エディットボックスのリソースＩＤ
				 double			value		(in)設定する数値
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SetEditValue(HWND h_wnd, UINT id , double value)
{
	int i, len;
	char		str[100];
	HWND		h_edit;
	
	h_edit = (HWND)::GetDlgItem( h_wnd, id );//エディットボックスのポインタを取得する
		if(h_edit == NULL)		return false;

	len = sprintf(str, "%f", value);
		for( i = len - 1  ;  i>0 ; i--){//文字列の右端の0を消す
			if( str[i] != '0' ){
				str[i+1] = '\0';
				break;
			}
		}
	
	::SetWindowText(h_edit, str);//エディットボックス内の文字列を設定する
	return	true;
}

/********************************************************************
機  能  名  称 : エディットボックスにデータ(CString)を設定する
関    数    名 : SetEditValue
引          数 : HWND			h_wnd		(in)エディットボックスが存在するウィンドウのハンドル
				 UINT			id			(in)エディットボックスのリソースＩＤ
				 CString		str			(in)設定する文字列
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SetEditValue(HWND h_wnd, UINT id , CString str)
{
	HWND		h_edit;
	
	h_edit = (HWND)::GetDlgItem( h_wnd, id );//エディットボックスのポインタを取得する
		if(h_edit == NULL)		return false;

	::SetWindowText(h_edit, str);//エディットボックス内の文字列を設定する
	return	true;
}

/********************************************************************
機  能  名  称 : チェックボックスにチェックの状態を設定する
関    数    名 : SetCheck
引          数 : HWND			h_wnd		(in)チェックボックスが存在するウィンドウのハンドル
				 UINT			id			(in)チェックボックスのリソースＩＤ
				 bool			value		(in)チェックの状態（チェックされていればtrue）
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SetCheck(HWND h_wnd, UINT id ,bool value)
{
	HWND	h_button;
	CButton	*p_button;

	h_button = (HWND)::GetDlgItem( h_wnd, id );//ボタンのポインタを取得する
		if(h_button == NULL)		return false;

	p_button = (CButton*)CWnd::FromHandle(h_button);
	p_button->SetCheck(value);
	return	true;
}



///////////////////////////////////////////////////////////////////////////////////
//メッセージ表示用のダイアログを出す
///////////////////////////////////////////////////////////////////////////////////
/********************************************************************
機  能  名  称 : メッセージ表示用のダイアログを出す
関    数    名 : MessageBox
引          数 : char *format, ...		(in)表示文字フォーマット
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void		CPimpomAPI::MessageBox(char *format, ...)
{
	va_list list;
	char	buf[5000];

	va_start( list, format );
	vsprintf(buf,format, list);
	va_end( list );

	AfxMessageBox(CString(buf));
}




///////////////////////////////////////////////////////////////////////////////////
//PIMPOM標準のモードレスダイアログを扱う
///////////////////////////////////////////////////////////////////////////////////
/********************************************************************
機  能  名  称 : 標準のモードレスダイアログの生成
関    数    名 : DialogOpen
引          数 : int	id		(in)ダイアログのリソースID
				 bool	show	(in)表示／非表示
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
			 Y.Ikeda         CMultiImageDispDlg追加
********************************************************************/
bool	CPimpomAPI::DialogOpen(int id, bool show)
{
	CDialog *pdlg=NULL;

	switch(id){
		case IDD_DATA_DLG:
			if(!pDataDlg){
				pDataDlg = new CDataDlg();
				if(pDataDlg)	pDataDlg->Create(IDD_DATA_DLG, NULL);
			}
			pdlg = (CDialog*)pDataDlg;
			break;

		case IDD_HISTGRAM_DLG:
			if(!pHistgramDlg){
				pHistgramDlg = new CHistgramDlg();
				if(pHistgramDlg)	pHistgramDlg->Create(IDD_HISTGRAM_DLG, NULL);
			}
			pdlg = (CDialog*)pHistgramDlg;
			break;


		case IDD_FILTER_CTRL_DLG:
			if(!pFlterCtrlDlg){
				pFlterCtrlDlg = new CFilterCtrlDlg();
				if(pFlterCtrlDlg)	pFlterCtrlDlg->Create(IDD_FILTER_CTRL_DLG, NULL);
			}
			pdlg = (CDialog*)pFlterCtrlDlg;
			break;

		case IDD_CALC_DLG:
			if(!pCalcDlg){
				pCalcDlg = new CCalcDlg();
				if(pCalcDlg)	pCalcDlg->Create(IDD_CALC_DLG, NULL);
			}
			pdlg = (CDialog*)pCalcDlg;
			break;

		case IDD_BINALIZE_DLG:
			if(!pBinalizeDlg){
				pBinalizeDlg = new CBinalizeDlg();
				if(pBinalizeDlg)	pBinalizeDlg->Create(IDD_BINALIZE_DLG, NULL);
			}
			pdlg = (CDialog*)pBinalizeDlg;
			break;

		case IDD_BATCH_DLG:
			if(!pBatchDlg){
				pBatchDlg = new CBatchDlg();
				if(pBatchDlg)	pBatchDlg->Create(IDD_BATCH_DLG, NULL);
			}
			pdlg = (CDialog*)pBatchDlg;
			break;

		case IDD_MULTI_IMAGE_DISP_DLG:
			if(!pMultiImageDispDlg){
				pMultiImageDispDlg = new CMultiImageDispDlg();
				if(pMultiImageDispDlg)	pMultiImageDispDlg->Create(IDD_MULTI_IMAGE_DISP_DLG, NULL);
			}
			pdlg = (CDialog*)pMultiImageDispDlg;
			break;

		case IDD_3D_IMAGE_DISP_DLG:
			if (!p3DImgDispDlg){
				p3DImgDispDlg = new C3DImgDispDlg();
				if (p3DImgDispDlg)	p3DImgDispDlg->Create(IDD_3D_IMAGE_DISP_DLG, NULL);
			}
			pdlg = (CDialog*)p3DImgDispDlg;
			break;

		case IDD_WORKAREA_DLG:
			if(!pWorkAreaDlg){
				pWorkAreaDlg = new CWorkAreaDlg();
				if(pWorkAreaDlg)	pWorkAreaDlg->Create(IDD_WORKAREA_DLG, NULL);
			}
			pdlg = (CDialog*)pWorkAreaDlg;
			break;

		case IDD_MARK_DLG:
			if(!pMarkDlg){
				pMarkDlg = new CMarkDlg();
				if(pMarkDlg)	pMarkDlg->Create(IDD_MARK_DLG, NULL);
			}
			pdlg = (CDialog*)pMarkDlg;
			break;

		case IDD_DATAPICK_DLG:
			if(!pDataPickDlg){
				pDataPickDlg = new CDataPickDlg();
				if(pDataPickDlg)	pDataPickDlg->Create(IDD_DATAPICK_DLG, NULL);
			}
			pdlg = (CDialog*)pDataPickDlg;
			break;

		case IDD_SOCKET_CTRL_DLG:
			if(!pSocketCtrlDlg){
				pSocketCtrlDlg = new CSocketCtrlDlg();
				if(pSocketCtrlDlg)	pSocketCtrlDlg->Create(IDD_SOCKET_CTRL_DLG, NULL);
			}
			pdlg = (CDialog*)pSocketCtrlDlg;
			break;

		case IDD_CHART_DLG:
			if(!pChartDlg){
				pChartDlg = new CChartDlg();
				if(pChartDlg)	pChartDlg->Create(IDD_CHART_DLG, NULL);
			}
			pdlg = (CDialog*)pChartDlg;
			break;

		case IDD_COMMAND_RPOMPT_DLG:
			if(!pCmdPrptDlg){
				pCmdPrptDlg = new CCommandPromptDlg();
				if(pCmdPrptDlg)	pCmdPrptDlg->Create(IDD_COMMAND_RPOMPT_DLG, NULL);
			}
			pdlg = (CDialog*)pCmdPrptDlg;
			break;

		case IDD_PYTHON_BATCH_DLG:
			if (!pPythonBatchDlg) {
				pPythonBatchDlg = new CPythonBatchDlg();
				if (pPythonBatchDlg)	pPythonBatchDlg->Create(IDD_PYTHON_BATCH_DLG, NULL);
			}
			pdlg = (CDialog*)pPythonBatchDlg;
			break;
	}

	if(pdlg){
		if (show) {
			pdlg->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
			
			pdlg->ShowWindow(SW_SHOW);
		}
		else {
			pdlg->ShowWindow(SW_HIDE);
		}

	}else{
		return false;
	}
	
	return true;
}

/********************************************************************
機  能  名  称 : 標準のモードレスダイアログの破棄
関    数    名 : DialogDelete
引          数 : int id		(in)ダイアログのリソースID
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DialogDelete(int id)
{
	CDialog *pdlg=NULL;

	switch(id){
		case IDD_DATA_DLG:
			if(pDataDlg){
				DestroyWindow(pDataDlg->m_hWnd);
				delete	pDataDlg;	pDataDlg=NULL;
			}
			break;

		case IDD_HISTGRAM_DLG:
			if(pHistgramDlg){
				DestroyWindow(pHistgramDlg->m_hWnd);
				delete	pHistgramDlg;	pHistgramDlg=NULL;
			}
			break;

		case IDD_FILTER_CTRL_DLG:
			if(pFlterCtrlDlg){
				DestroyWindow(pFlterCtrlDlg->m_hWnd);
				delete	pFlterCtrlDlg;	pFlterCtrlDlg=NULL;
			}
			break;

		case IDD_CALC_DLG:
			if(pCalcDlg){
				DestroyWindow(pCalcDlg->m_hWnd);
				delete	pCalcDlg;	pCalcDlg=NULL;
			}
			break;

		case IDD_BINALIZE_DLG:
			if(pBinalizeDlg){
				DestroyWindow(pBinalizeDlg->m_hWnd);
				delete	pBinalizeDlg;	pBinalizeDlg=NULL;
			}
			break;

		case IDD_BATCH_DLG:
			if(pBatchDlg){
				DestroyWindow(pBatchDlg->m_hWnd);
				delete	pBatchDlg;	pBatchDlg=NULL;
			}
			break;

		case IDD_MULTI_IMAGE_DISP_DLG:
			if(pMultiImageDispDlg){
				DestroyWindow(pMultiImageDispDlg->m_hWnd);
				delete	pMultiImageDispDlg;	pMultiImageDispDlg=NULL;
			}
			break;

		case IDD_3D_IMAGE_DISP_DLG:
			if (p3DImgDispDlg){
				DestroyWindow(p3DImgDispDlg->m_hWnd);
				delete	p3DImgDispDlg;	p3DImgDispDlg = NULL;
			}
			break;

		case IDD_WORKAREA_DLG:
			if(pWorkAreaDlg){
				DestroyWindow(pWorkAreaDlg->m_hWnd);
				delete	pWorkAreaDlg;	pWorkAreaDlg=NULL;
			}
			break;

		case IDD_MARK_DLG:
			if(pMarkDlg){
				DestroyWindow(pMarkDlg->m_hWnd);
				delete	pMarkDlg;	pMarkDlg=NULL;
			}
			break;

		case IDD_DATAPICK_DLG:
			if(pDataPickDlg){
				DestroyWindow(pDataPickDlg->m_hWnd);
				delete	pDataPickDlg;	pDataPickDlg=NULL;
			}
			break;

		case IDD_SOCKET_CTRL_DLG:
			if(pSocketCtrlDlg){
				DestroyWindow(pSocketCtrlDlg->m_hWnd);
				delete	pSocketCtrlDlg;	pSocketCtrlDlg=NULL;
			}
			break;

		case IDD_CHART_DLG:
			if(pChartDlg){
				DestroyWindow(pChartDlg->m_hWnd);
				delete	pChartDlg;	pChartDlg=NULL;
			}
			break;

		case IDD_COMMAND_RPOMPT_DLG:
			if(pCmdPrptDlg){
				DestroyWindow(pCmdPrptDlg->m_hWnd);
				delete	pCmdPrptDlg;	pCmdPrptDlg=NULL;
			}
			break;

		case IDD_PYTHON_BATCH_DLG:
			if (pPythonBatchDlg) {
				DestroyWindow(pPythonBatchDlg->m_hWnd);
				delete	pPythonBatchDlg;	pPythonBatchDlg = NULL;
			}
			break;
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////////
//メインウィンドウにメッセージを表示する
//////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
機  能  名  称 : メインウィンドウにメッセージを表示する
関    数    名 : ShowMessage
引          数 : CString str			(in)表示文字
				bool do_clear			(in)過去のメッセージをクリアするかどうか
				int num					(in)メッセージのタブ番号
				bool force				(in)強制表示（他のタブが開いていても切り替える）
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::ShowMessage(bool do_clear, int num, bool force, CString str)
{
	pPIMMOMDlg->ShowMessage( do_clear , num, force, str );
}

/********************************************************************
機  能  名  称 : メインウィンドウにメッセージを表示する
関    数    名 : ShowMessage
引          数 : char *format, ...		(in)表示文字フォーマット
				bool do_clear			(in)過去のメッセージをクリアするかどうか
				int num					(in)メッセージのタブ番号
				bool force				(in)強制表示（他のタブが開いていても切り替える）
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::ShowMessage(bool do_clear,int num, bool force, char *format, ...)
{
	va_list list;
	char	buf[5000];

	va_start( list, format );
	vsprintf(buf,format, list);
	va_end( list );

	pPIMMOMDlg->ShowMessage( do_clear, num, force ,CString(buf));
}

void	CPimpomAPI::ShowMessage(bool do_clear, char *format, ...)//メッセージ1に強制表示
{
	va_list list;
	char	buf[5000];

	va_start( list, format );
	vsprintf(buf,format, list);
	va_end( list );

	pPIMMOMDlg->ShowMessage( do_clear, 1,true ,CString(buf));
}

/********************************************************************
機  能  名  称 : マーク編集モードの状態取得
関    数    名 : GetMarkEditMode
引          数 : 
戻    り    値 : 1:表示のみ 2:表示+編集(点) 3:表示+編集(線) 0:表示なし
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int	CPimpomAPI::GetMarkEditMode()
{
	if (GetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_MARK))
	{
		if (pMarkDlg)
		{
			if (pMarkDlg->GetMarkEditType() == MARK_TYPE_POINT )
			{
				return 2;
			}
			else if (pMarkDlg->GetMarkEditType() == MARK_TYPE_LINE)
			{
				return 3;
			}
			else {
				return 1;
			}
		}
		else {
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

/********************************************************************
機  能  名  称 : マーク編集モードの状態設定
関    数    名 : SetMarkEditMode
引          数 :int mode  (in)1:表示のみ 2:表示+編集(点) 3:表示+編集(線) 0:表示なし
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::SetMarkEditMode(int mode)
{
	CDataUnit	*p_du = GetDataUnit( CURRENT_IMAGE );//現在選択中のDataUnitを取得
	if(p_du == NULL)		return;//データが無ければなにもしない

	if( mode==2 || mode == 3)//マークの編集をはじめる
	{
		SetCheck( pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_MARK , true);//マーク表示ボタンをチェック



		//マークウィンドウ表示
		if(pMarkDlg)
		{
			if (mode == 2)
			{
				pMarkDlg->SetMarkEditType(MARK_TYPE_POINT);
			}
			else {
				pMarkDlg->SetMarkEditType(MARK_TYPE_LINE);
			}
			
			pMarkDlg->ShowWindow(SW_SHOW);
			pMarkDlg->ShowMarkVal( CURRENT_IMAGE );//十字カーソルを表示
		}

		ShowMessage(false, 0, true, "クリックでマーク追加");//メッセージを出す

		//ほかの編集ボタンのチェックをはずす
		SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE, false);
		if (pWorkAreaDlg) {
			pWorkAreaDlg->SetWorkAreaEditMode(false);
		}
	}
	else if (mode == 1)//表示のみ
	{
		SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_MARK, true);//マーク表示ボタンをチェック

		//マークウィンドウを表示
		if (pMarkDlg) {
			pMarkDlg->SetMarkEditType(-1);
			pMarkDlg->ShowWindow(SW_SHOW);
			pMarkDlg->ShowMarkVal(CURRENT_IMAGE);//十字カーソルを表示
		}
	}
	else
	{//マーク表示しない状態にする

		SetCheck( pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_MARK , false);//マーク表示ボタンのチェックをはずす

		//マークウィンドウを非表示
		if(pMarkDlg){
			pMarkDlg->SetMarkEditType(-1);
			pMarkDlg->ShowWindow(SW_HIDE);
		}
	}

	DrawImage();//再描画
}



/********************************************************************
機  能  名  称 : 作業領域編集モードの状態取得
関    数    名 : GetWorkAreaEditMode
引          数 : 
戻    り    値 : 作業領域編集モードならばtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::GetWorkAreaEditMode()
{
	if (pWorkAreaDlg)
	{
		if (GetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_WORK_AREA)) {
			return pWorkAreaDlg->GetWorkAreaEditMode();
		}
		else {
			return false;
		}
	}
	else 
	{
		return false;
	}
}

/********************************************************************
機  能  名  称 : 作業領域編集モードの状態設定
関    数    名 : SetWorkAreaEditMode
引          数 : bool onoff  (in)true:作業領域編集モードに入る	(in)false:作業領域編集モードを中止
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::SetWorkAreaEditMode(bool onoff)
{
	CDataUnit	*p_du = GetDataUnit( CURRENT_IMAGE );//現在選択中のDataUnitを取得
	if(p_du == NULL)		return;//データが無ければなにもしない

	if( onoff )
	{//作業領域の編集をする

		if( ! GetCheck( pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_WORK_AREA ) )
		{//作業領域が使われていない場合は使うようにする
			SetCheck( pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_WORK_AREA , true);
			p_du->DoUseWorkArea = true;
		}

		//作業領域ウィンドウを表示
		if(pWorkAreaDlg){
			pWorkAreaDlg->SetWorkAreaEditMode(true);
			pWorkAreaDlg->ShowWindow(SW_SHOW);
		}

		ShowMessage(false, 0, true, "ドラッグで作業領域作成");//メッセージを出す

		//ほかの編集ボタンのチェックをはずす
		SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE, false);
		if (pMarkDlg) 
		{
			if (pMarkDlg->GetMarkEditType() == MARK_TYPE_POINT || pMarkDlg->GetMarkEditType() == MARK_TYPE_LINE) {
				pMarkDlg->SetMarkEditType(-1);
			}
		}
	}
	else {
		if (pWorkAreaDlg) {
			pWorkAreaDlg->SetWorkAreaEditMode(false);
		}
	}

	
	DrawImage();//再描画
}

/********************************************************************
機  能  名  称 : 2点間の距離測定ツールの状態取得
関    数    名 : GetRulerToolMode
引          数 : 
戻    り    値 : 使用中ならばtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::GetRulerToolMode()
{
	return GetCheck( pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE);
}

/********************************************************************
機  能  名  称 : 2点間の距離測定ツールの状態設定
関    数    名 : SetRulerToolMode
引          数 : bool onoff  (in)true:使用開始	(in)false:使用中止
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::SetRulerToolMode(bool onoff)
{
	CDataUnit	*p_du = GetDataUnit( CURRENT_IMAGE );//現在選択中のDataUnitを取得
	if(p_du == NULL)		return;//データが無ければなにもしない

	if( GetCheck( pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE ) ){//距離の計算をできるようにする
		//ほかの編集ボタンのチェックをはずす
		
		if (pWorkAreaDlg) {
			pWorkAreaDlg->SetWorkAreaEditMode(false);
		}

		if (pMarkDlg) {
			if (pMarkDlg->GetMarkEditType() == MARK_TYPE_POINT || pMarkDlg->GetMarkEditType() == MARK_TYPE_LINE) {
				pMarkDlg->SetMarkEditType(-1);
			}
		}

		ShowMessage(false, 0, true, "ドラッグした２点の距離と値の差を計算する");//メッセージを出す
	}
	DrawImage();//再描画
}

/********************************************************************
機  能  名  称 : フリーハンド編集モードの状態取得
関    数    名 : GetDataPickEditType
引          数 : 
戻    り    値 : 0:非表示  1:ピック  2:ペイント 3:フィル
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
int		CPimpomAPI::GetDataPickEditType()
{
	if (pDataPickDlg)
	{
		if (pDataPickDlg->IsWindowVisible())
		{
			if (pDataPickDlg->GetDataPickMode()== DATAPICK_PICK) {
				return 1;
			}
			else if (pDataPickDlg->GetDataPickMode() == DATAPICK_PAINT) {
				return 2;
			}
			else {
				return 3;
			}
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

/********************************************************************
機  能  名  称 : フリーハンド編集モードの状態設定
関    数    名 : SetDataPickEditType
引          数 :
戻    り    値 : 0:非表示  1:ピック  2:ペイント　3:フィル
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::SetDataPickEditType(int mode)
{
	if (pDataPickDlg)
	{
		if (mode == 0)
		{
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_PAINT, false);
			pDataPickDlg->ShowWindow(SW_HIDE);
		}
		else if (mode == 1)
		{
			pDataPickDlg->SetDataPickMode(DATAPICK_PICK);
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_PAINT, true);
			pDataPickDlg->ShowWindow(SW_SHOW);
		}
		else if (mode == 2)
		{
			pDataPickDlg->SetDataPickMode(DATAPICK_PAINT);
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_PAINT, true);
			pDataPickDlg->ShowWindow(SW_SHOW);
		}
		else {
			pDataPickDlg->SetDataPickMode(DATAPICK_FILL);
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_PAINT, true);
			pDataPickDlg->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		if (mode == 0)
		{

		}
		else if (mode == 1)
		{
			DialogOpen(IDD_DATAPICK_DLG, true);
			pDataPickDlg->SetDataPickMode(DATAPICK_PICK);
		}
		else if (mode == 2) {
			DialogOpen(IDD_DATAPICK_DLG, true);
			pDataPickDlg->SetDataPickMode(DATAPICK_PAINT);
		}
		else {
			DialogOpen(IDD_DATAPICK_DLG, true);
			pDataPickDlg->SetDataPickMode(DATAPICK_FILL);
		}
	}
}


/********************************************************************
機  能  名  称 : 画像生成ウィンドウの状態取得
関    数    名 : GetCalcToolMode
引          数 :
戻    り    値 : true:表示 false:非表示
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::GetCalcToolMode()
{
	if (pCalcDlg)
	{
		return pCalcDlg->IsWindowVisible();
	}
	else {
		return false;
	}
}

/********************************************************************
機  能  名  称 : 画像生成ウィンドウの状態設定
関    数    名 : GetCalcToolMode
引          数 : true:表示 false:非表示
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPimpomAPI::SetCalcToolMode(bool onoff)
{
	if (pCalcDlg)
	{
		if (onoff) {
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC, true);
			pCalcDlg->ShowWindow(SW_SHOW);
		}
		else {
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC, false);
			pCalcDlg->ShowWindow(SW_HIDE);
		}
	}
	else {
		if (onoff) {
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC, true);
			DialogOpen(IDD_CALC_DLG, true);
		}
	}
}

/********************************************************************
機  能  名  称 : ヒストグラムウィンドウのの状態取得
関    数    名 : GetHistgramToolMode
引          数 :
戻    り    値 : true:表示 false:非表示
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::GetHistgramToolMode()
{
	if (pHistgramDlg)
	{
		return pHistgramDlg->IsWindowVisible();
	}
	else {
		return false;
	}
}

/********************************************************************
機  能  名  称 : ヒストグラムウィンドウの状態設定
関    数    名 : SetHistgramToolMode
引          数 : true:表示 false:非表示
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPimpomAPI::SetHistgramToolMode(bool onoff)
{
	if (pHistgramDlg)
	{
		pHistgramDlg->ShowWindow(onoff ? SW_SHOW : SW_HIDE);
	}
	else {
		if (onoff) {
			DialogOpen(IDD_HISTGRAM_DLG, true);
		}
	}
}

/********************************************************************
機  能  名  称 : ビデオキャプチャのスルー表示状態の取得
関    数    名 : GetVideoCaptureThroughMode
引          数 : 
戻    り    値 : 1:スルー表示中 2:バッチ実行つき連続撮影中 0:スルー表示なし
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int	CPimpomAPI::GetVideoCaptureThroughMode()
{
	if( GetCheck( pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE) ){
		return 1;
	}else if(GetCheck( pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2)){
		return 2;
	}else{
		return 0;
	}
}

/********************************************************************
機  能  名  称 : ビデオキャプチャのスルー表示状態の設定
関    数    名 : SetVideoCaptureThroughMode
引          数 : int mode  (in)1:スルー表示 2:バッチ実行つき連続撮影 0:スルー表示なし
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::SetVideoCaptureThroughMode(int mode)
{
	if(mode==1)
	{
		if(GetCheck(pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2)){
			StopBGProc();
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2, false);
		}

		SetCheck(pPIMMOMDlg->m_hWnd,IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE,true);
		SetBGProcID(BGPROC_CAPTURING);
	}
	else if(mode==2)
	{
		if(GetCheck(pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE)){
			StopBGProc();
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE,false);
		}

		SetCheck(pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2, true);
		SetBGProcID(BGPROC_CAPTURING);
	}
	else
	{
		StopBGProc();
		SetCheck(pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2, false);
		SetCheck(pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE,false);
	}
}

/********************************************************************
機  能  名  称 : 表示画像をクリップボードにコピーする
関    数    名 : CopyToClipbord
引          数 : int image_num
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::CopyToClipbord(int image_num)
{
	bool ret=false;
	CDC memDC;
	CBitmap bm;
	int copySizeX, copySizeY;

	CDC* pDC = pPIMMOMDlg->GetDC();//メインウィンドウのデバイスコンテキスト取得
	if(pDC){

		CDataUnit *pDU = GetDataUnit(image_num);
		if(pDU)
		{
			DrawImage(image_num);

			double imageRight,imageBottom;
			GetDestCoor( (double)pDU->DataSize.cx -0.5 , (double)pDU->DataSize.cy -0.5 , &imageRight, &imageBottom);

			//表示画像の右端でコピーする領域をクリップ
			if( imageRight > MAIN_WIN_LEFT_MARGIN  &&  imageRight < MAIN_WIN_LEFT_MARGIN + main_image_size.cx){
				copySizeX = (int)imageRight - MAIN_WIN_LEFT_MARGIN;
			}else{
				copySizeX = (int)main_image_size.cx;
			}

			//表示画像の下端でコピーする領域をクリップ
			if( imageBottom > MAIN_WIN_TOP_MARGIN  &&  imageBottom < MAIN_WIN_TOP_MARGIN + main_image_size.cy){
				copySizeY = (int)imageBottom - MAIN_WIN_TOP_MARGIN;
			}else{
				copySizeY = (int)main_image_size.cy;
			}

			if(memDC.CreateCompatibleDC(pDC))//pDC と互換性のあるメモリデバイスコンテキストを作成
			{
				if(bm.CreateCompatibleBitmap(pDC, copySizeX, copySizeY))//pDCと互換性のあるビットマップを作成
				{
					CBitmap* pOldBmp = memDC.SelectObject(&bm);//メモリデバイスコンテキストにビットマップを選択
					if( memDC.StretchBlt(MAIN_WIN_LEFT_MARGIN , MAIN_WIN_TOP_MARGIN, copySizeX , copySizeY ,pDC, 0 , 0 , copySizeX , copySizeY ,SRCCOPY) )//画面イメージをビットマップに転送
					{
						if(pPIMMOMDlg->OpenClipboard()){//クリップボードを開く
							::EmptyClipboard();
							if(::SetClipboardData(CF_BITMAP, bm.GetSafeHandle())!=NULL){//クリップボードにビットマップ貼り付け
								ret = true;
							}
							::CloseClipboard();//クリップボードを閉じる
						}
					}
					bm.Detach();
					memDC.SelectObject(pOldBmp);//ビットマップ解放
				}
				memDC.DeleteDC();//メモリデバイスコンテキスト解放
			}
		}
		pPIMMOMDlg->ReleaseDC( pDC );//メインウィンドウのデバイスコンテキスト解放
	}

	return ret;
}




/********************************************************************
機  能  名  称 : クリップボードにある画像をメモリに入れる
関    数    名 : CopyFromClipbord
引          数 : int image_num
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::PasteFromClipbord(int image_num)
{
	bool ret=true;

	if (IsClipboardFormatAvailable(CF_BITMAP)) 
	{
		if( ::OpenClipboard(FindWindow(NULL,NULL)))//クリップボードを開く
		{
			//DIBハンドルの取得 
			HANDLE hBitmap = GetClipboardData(CF_DIB); 
			if(hBitmap == NULL)		return false;

			char* szBuffer = (char *)GlobalLock(hBitmap); //DIB先頭ポインタの取得
			if(szBuffer)
			{
				BITMAPINFOHEADER bmpInfo;//DIBヘッダの取得
				memcpy( &bmpInfo, szBuffer, sizeof(BITMAPINFOHEADER) ); 
			


				//書き込む画像メモリの確保
				BYTE *pRGBData = GetRGBMemory(image_num, CSize(bmpInfo.biWidth, bmpInfo.biHeight), true);
				if(pRGBData)
				{
					if(bmpInfo.biBitCount==32)//32ビット
					{
						int nWidth = bmpInfo.biWidth*4;
						unsigned char* pData = (unsigned char*)(szBuffer + sizeof(BITMAPINFOHEADER) + 12);//画像データ先頭ポインタ取得

						//データコピー
						for(int j=0 ; j<bmpInfo.biHeight ; j++){
							for(int i=0 ; i<bmpInfo.biWidth ; i++){
								int addr = i + (bmpInfo.biHeight-1-j)*bmpInfo.biWidth;
								*(pRGBData + addr)										=  *(pData + nWidth*j + i*4 + 2);//R
								*(pRGBData + addr + bmpInfo.biWidth*bmpInfo.biHeight)	=  *(pData + nWidth*j + i*4 + 1);//G
								*(pRGBData + addr + bmpInfo.biWidth*bmpInfo.biHeight*2)	=  *(pData + nWidth*j + i*4 + 0);//B
							}
						}
						

					}
					else if(bmpInfo.biBitCount==24)//24ビット
					{
						unsigned char* pData = (unsigned char*)(szBuffer + sizeof(BITMAPINFOHEADER) );//画像データ先頭ポインタ取得
						
						//DIBデータ幅とRGBデータ幅の差
						int nWidth, bmp_width_gap;
						if( (bmpInfo.biWidth*3) % 4 == 0)	bmp_width_gap = 0;	
						else								bmp_width_gap = 4 - (bmpInfo.biWidth*3) % 4;
						nWidth = bmpInfo.biWidth*3 + bmp_width_gap;

						//データコピー
						for(int j=0 ; j<bmpInfo.biHeight ; j++){
							for(int i=0 ; i<bmpInfo.biWidth ; i++){
								int addr = i + (bmpInfo.biHeight-1-j)*bmpInfo.biWidth;
								*(pRGBData + addr)										=  *(pData + nWidth*j + i*3 + 2);//R
								*(pRGBData + addr + bmpInfo.biWidth*bmpInfo.biHeight)	=  *(pData + nWidth*j + i*3 + 1);//G
								*(pRGBData + addr + bmpInfo.biWidth*bmpInfo.biHeight*2)	=  *(pData + nWidth*j + i*3 + 0);//B
							}
						}

					}
					else if(bmpInfo.biBitCount==8)//8ビット
					{

						COLORREF	colTbl[256];
						memcpy(colTbl, szBuffer, 1024);//カラーパレット取得
						unsigned char* pData = (unsigned char*)(szBuffer + sizeof(BITMAPINFOHEADER) + 1024);//画像データ先頭ポインタ取得

						//DIBデータ幅とRGBデータ幅の差
						int nWidth, bmp_width_gap;
						if( (bmpInfo.biWidth) % 4 == 0)	bmp_width_gap = 0;	
						else							bmp_width_gap = 4 - bmpInfo.biWidth % 4;
						nWidth = bmpInfo.biWidth + bmp_width_gap;

						//データコピー
						for(int j=0 ; j<bmpInfo.biHeight ; j++){
							for(int i=0 ; i<bmpInfo.biWidth ; i++){
								int addr = i + (bmpInfo.biHeight-1-j)*bmpInfo.biWidth;
								COLORREF rgb = colTbl[*(pData + nWidth*j + i)];
								*(pRGBData + addr)										=  (BYTE)rgb;//R
								*(pRGBData + addr + bmpInfo.biWidth*bmpInfo.biHeight)	=  (BYTE)(((WORD)(rgb)) >> 8);//G
								*(pRGBData + addr + bmpInfo.biWidth*bmpInfo.biHeight*2)	=  (BYTE)((rgb)>>16);//B
							}
						}
					}
					else
					{
						ret = false;
					}
					
					DrawImage(image_num,true,true);//描画
				}
				GlobalUnlock(hBitmap); 
			}
			::CloseClipboard();//クリップボードを閉じる
		}
	}

	return ret;
}


////////////////////////////////////////////////////////////////////////////////////////////
//ユーザダイアログの使用
////////////////////////////////////////////////////////////////////////////////////////////
typedef void (OpenUserDlg) (CPimpomAPI*);
typedef void (DestroyUserDlg) (void);
typedef void (OnDrawMainImage_UserDlg)(int image_number);
typedef void (OnMouseDown_UserDlg) (POINT point);
typedef void (OnMouseDrug_UserDlg) (POINT start_point, POINT currnet_point);
typedef void (OnFinishMouseDrug_UserDlg) (POINT start_point, POINT currnet_point);
typedef void (OnBatchExecute_UserDlg) (BATCH_INPUT *in_param, BATCH_OUTPUT *out_param);
typedef void (OnReceive_UserDlg) (BYTE *data, int *ndata);

/********************************************************************
機  能  名  称 : ユーザダイアログのキャプションを取得
関    数    名 : GetUserDlgCaptions
引          数 : CString caption[]  (out)キャプション
戻    り    値 : 登録されているユーザダイアログの数
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
//ユーザダイアログのキャプションを取得
int		CPimpomAPI::GetUserDlgCaptions(CString caption[])
{
	int cnt=0;
	for(int i=0 ; i<CUSTOM_FUNC_MAX ; i++){
		if(custom_func[i].hInst != NULL){
			caption[i] = custom_func[i].caption;
			cnt++;
		}else{
			break;
		}
	}

	return cnt;
}

/********************************************************************
機  能  名  称 : ユーザダイアログの起動
関    数    名 : UserDialogOpen
引          数 : int n  (in)ダイアログ番号
戻    り    値 : 正常実行できたらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::UserDialogOpen(int n)
{
	OpenUserDlg		*func;

	if( custom_func[n].hInst != NULL ){
		func = (OpenUserDlg *)GetProcAddress( custom_func[n].hInst, "OpenUserDlg" );
		if( func != NULL ){
			func(this);
			return true;
		}
	}
	return false;
}

/********************************************************************
機  能  名  称 : ユーザダイアログの破壊
関    数    名 : UserDialogDelete
引          数 : int n		(in)ユーザダイアログの番号
戻    り    値 : 正常実行できたらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::UserDialogDelete(int n)
{
	DestroyUserDlg		*func;

	if( custom_func[n].hInst == NULL )	return false;

	func = (DestroyUserDlg *)GetProcAddress( custom_func[n].hInst, "DestroyUserDlg" );
	if( func != NULL ){
		func();
	}else{
		return false;
	}
	if(!AfxFreeLibrary( custom_func[n].hInst )){// ロードしたDLLを解放
		return false;
	}
	custom_func[n].hInst=NULL;

	return true;
}


/********************************************************************
機  能  名  称 : メイン画面描画
関    数    名 : UserDialogOnMouseDown
引          数 : int		n		(in)ユーザダイアログの番号
				 int		image_number	(in)画像メモリ番号
戻    り    値 : 正常実行できたらtrue
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::UserDialogOnDrawMainImage(int n, int image_number)
{
	OnDrawMainImage_UserDlg		*func;

	if (custom_func[n].hInst != NULL){
		func = (OnDrawMainImage_UserDlg *)GetProcAddress(custom_func[n].hInst, "OnDrawMainImage_UserDlg");
		if (func != NULL){
			func(image_number);
			return true;
		}
	}
	return false;
}

/********************************************************************
機  能  名  称 : メイン画面上でのマウス押下
関    数    名 : UserDialogOnMouseDown
引          数 : int		n		(in)ユーザダイアログの番号
				 POINT		point	(in)マウス押下座標（画像座標）
戻    り    値 : 正常実行できたらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::UserDialogOnMouseDown(int n, POINT point)
{
	OnMouseDown_UserDlg		*func;

	if( custom_func[n].hInst != NULL ){
		func = (OnMouseDown_UserDlg *)GetProcAddress( custom_func[n].hInst, "OnMouseDown_UserDlg" );
		if( func != NULL ){
			func(point );
			return true;
		}
	}
	return false;
}

/********************************************************************
機  能  名  称 : メイン画面上でのマウス右ボタン押下
関    数    名 : UserDialogOnMouseRDown
引          数 : int		n		(in)ユーザダイアログの番号
				 POINT		point	(in)マウス押下座標（画像座標）
戻    り    値 : 正常実行できたらtrue
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
			 Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::UserDialogOnMouseRDown(int n, POINT point)
{
	OnMouseDown_UserDlg* func;

	if (custom_func[n].hInst != NULL) {
		func = (OnMouseDown_UserDlg*)GetProcAddress(custom_func[n].hInst, "OnMouseRDown_UserDlg");
		if (func != NULL) {
			func(point);
			return true;
		}
	}
	return false;
}

/********************************************************************
機  能  名  称 : メイン画面上でのマウスドラッグ
関    数    名 : UserDialogOnMouseDrug
引          数 : int n		(in)ユーザダイアログの番号
				 POINT		start_point	(in)マウスドラッグ開始（画像座標）
				 POINT		currnet_point	(in)現在のマウス座標（画像座標）
戻    り    値 : 正常実行できたらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::UserDialogOnMouseDrug(int n, POINT start_point, POINT currnet_point)
{
	OnMouseDrug_UserDlg		*func;

	if( custom_func[n].hInst != NULL ){
		func = (OnMouseDrug_UserDlg *)GetProcAddress( custom_func[n].hInst, "OnMouseDrug_UserDlg" );
		if( func != NULL ){
			func(start_point,currnet_point );
			return true;
		}
	}
	return false;
}

/********************************************************************
機  能  名  称 : メイン画面上でのマウスドラッグ終了
関    数    名 : UserDialogOnFinishMouseDrug
引          数 : int n		(in)ユーザダイアログの番号
				 POINT		start_point	(in)マウスドラッグ開始座標（画像座標）
				 POINT		currnet_point	(in)マウスドラッグ終了座標（画像座標）
戻    り    値 : 正常実行できたらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::UserDialogOnFinishMouseDrug(int n, POINT start_point, POINT currnet_point)
{
	OnFinishMouseDrug_UserDlg		*func;

	if( custom_func[n].hInst != NULL ){
		func = (OnFinishMouseDrug_UserDlg *)GetProcAddress( custom_func[n].hInst, "OnFinishMouseDrug_UserDlg" );
		if( func != NULL ){
			func(start_point,currnet_point );
			return true;
		}
	}
	return false;
}

/********************************************************************
機  能  名  称 : ユーザダイアログのバッチ実行
関    数    名 : UserDialogBatchExecute
引          数 : int	n				(in)ユーザダイアログの番号
				 BATCH_INPUT *in_param	(in)入力パラメタ
				 BATCH_OUTPUT *out_param (out)出力パラメタ
戻    り    値 : 正常実行できたらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::UserDialogBatchExecute(int n, BATCH_INPUT *in_param, BATCH_OUTPUT *out_param)
{
	OnBatchExecute_UserDlg		*func;

	if( custom_func[n].hInst != NULL ){
		func = (OnBatchExecute_UserDlg *)GetProcAddress( custom_func[n].hInst, "OnBatchExecute_UserDlg" );
		if( func != NULL ){
			func(in_param, out_param);
			return true;
		}
	}
	return false;
}

/********************************************************************
機  能  名  称 : バッチ実行
関    数    名 : UserDialogBatchExecute
引          数 : 
			BATCH_INPUT *in_param	(in)入力パラメタ
			BATCH_OUTPUT *out_param (out)出力パラメタ
戻    り    値 : 正常実行できたらtrue
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::BatchExecute(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param)
{
	//画像ファイルをロード
	if (strlen(in_param->imae_file_path) > 0)
	{
		if (SelectAndLoadImageFile(in_param->image_number, CString(in_param->imae_file_path))) {
			DrawImage(in_param->image_number, true, true);
		}
	}
	

	if (batch_execute_rule != 1)
	{
		if (pCmdPrptDlg)//コマンドラインのバッチ実行(カスタムファンクションの前)
		{
			if (pCmdPrptDlg->OnBatchExecute(in_param, out_param))//コマンドラインのバッチ実行イベントハンドラを呼び出す
			{

			}
		}

		if (pPythonBatchDlg)//pyhtonのバッチ実行
		{
			if (pPythonBatchDlg->OnBatchExecute())
			{

			}
		}
	}




	//カスタムダイアログのバッチ実行
	if (batch_execute_rule == 0 || batch_execute_rule == 1)
	{
		for (int i = 0; i < CUSTOM_FUNC_MAX; i++)
		{
			BATCH_OUTPUT out_param_tmp = { 0 };

			if (UserDialogBatchExecute(i, in_param, &out_param_tmp))//カスタムダイアログのバッチ実行イベントハンドラを呼び出す
			{
				for (int n = 0; n < BATCH_OUT_PARAM_NUM; n++)//複数のカスタムダイアログがある場合は、出力値を統合
				{
					CString reslog = CString(out_param->result[n]) + CString(out_param_tmp.result[n]);
					strcpy(out_param->result[n], reslog.GetBuffer());
					reslog.ReleaseBuffer();
				}
			}
		}
	}


	if (batch_execute_rule == 1)
	{
		if (pCmdPrptDlg)//コマンドラインのバッチ実行(カスタムファンクションの後)
		{
			if (pCmdPrptDlg->OnBatchExecute(in_param, out_param))//コマンドラインのバッチ実行イベントハンドラを呼び出す
			{

			}
		}

		if (pPythonBatchDlg)//pyhtonのバッチ実行
		{
			if (pPythonBatchDlg->OnBatchExecute())
			{

			}
		}
	}


	return true;
}

/********************************************************************
機  能  名  称 : ユーザダイアログの通信受信
関    数    名 : UserDialogBatchExecute
引          数 : int	n		(in)ユーザダイアログの番号
				 BATCH_INPUT *in_param	(in)入力パラメタ
				 BATCH_OUTPUT *out_param (out)出力パラメタ
戻    り    値 : 正常実行できたらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::UserDialogReceive(int n, BYTE *data, int *ndata)
{
	OnReceive_UserDlg		*func;

	if( custom_func[n].hInst != NULL ){
		func = (OnReceive_UserDlg *)GetProcAddress( custom_func[n].hInst, "OnReceive_UserDlg" );
		if( func != NULL ){
			func(data, ndata);
			return true;
		}
	}
	return false;
}


/********************************************************************
機  能  名  称 : 折れ線グラフに値を追加して描画する
関    数    名 : AddDataToChart
引          数 : int	chart_id		(in)グラフの番号
				 float	value			(in)値
戻    り    値 : 正常実行できたら追加したデータ番号を返す。失敗したら負数を返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CPimpomAPI::AddDataToChart(int chart_id, float value)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return -1;

	return pChartDlg->AddDataToChart(chart_id, value);
}

/********************************************************************
機  能  名  称 : 2次元散布図に値を追加して描画する
関    数    名 : AddDataToChart2
引          数 : int	chart_id		(in)グラフの番号
				 float	valueX			(in)値X
				 float	valueY			(in)値Y
戻    り    値 : 正常実行できたら追加したデータ番号を返す。失敗したら負数を返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CPimpomAPI::AddDataToChart2(int chart_id, float valueX, float valueY)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return -1;

	return pChartDlg->AddDataToChart(chart_id, valueX, valueY);
}

/********************************************************************
機  能  名  称 : 折れ線グラフにデータ列を追加して描画する
関    数    名 : PlotDataToChart
引          数 : int	chart_id		(in)グラフの番号
				 int	data_num		(in)データ数
				    	*pValArr		(in)データ列
				 bool	doClear			(in)プロット前に現在のグラフをクリア
戻    り    値 : 正常実行できたらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::PlotDataToChart(int chart_id, int data_num, float *pValArr, bool doClear)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return false;
	return pChartDlg->PlotDataToChart(chart_id, data_num, pValArr, doClear);
}

bool CPimpomAPI::PlotDataToChart(int chart_id, int data_num, double *pValArr, bool doClear)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return false;
	return pChartDlg->PlotDataToChart(chart_id, data_num, pValArr, doClear);
}

bool CPimpomAPI::PlotDataToChart(int chart_id, int data_num, int *pValArr, bool doClear)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return false;
	return pChartDlg->PlotDataToChart(chart_id, data_num, pValArr, doClear);
}

bool CPimpomAPI::PlotDataToChart(int chart_id, int data_num, BYTE *pValArr, bool doClear)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return false;
	return pChartDlg->PlotDataToChart(chart_id, data_num, pValArr, doClear);
}

/********************************************************************
機  能  名  称 : 2次元散布図にデータ列を追加して描画する
関    数    名 : PlotDataToChart2
引          数 : int	chart_id		(in)グラフの番号
				 int	data_num		(in)データ数
				    	*pValArrX		(in)データ列X
						*pValArrY		(in)データ列Y
				 bool	doClear			(in)プロット前に現在のグラフをクリア
戻    り    値 : 正常実行できたらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::PlotDataToChart2(int chart_id, int data_num, float *pValArrX, float *pValArrY, bool doClear)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return false;
	return pChartDlg->PlotDataToChart(chart_id, data_num, pValArrX, pValArrY, doClear);
}

bool CPimpomAPI::PlotDataToChart2(int chart_id, int data_num, double *pValArrX, double *pValArrY, bool doClear)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return false;
	return pChartDlg->PlotDataToChart(chart_id, data_num, pValArrX, pValArrY, doClear);
}

bool CPimpomAPI::PlotDataToChart2(int chart_id, int data_num, int *pValArrX, int *pValArrY, bool doClear)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return false;
	return pChartDlg->PlotDataToChart(chart_id, data_num, pValArrX, pValArrY, doClear);
}

bool CPimpomAPI::PlotDataToChart2(int chart_id, int data_num, BYTE *pValArrX, BYTE *pValArrY, bool doClear)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return false;
	return pChartDlg->PlotDataToChart(chart_id, data_num, pValArrX, pValArrY, doClear);
}

void CPimpomAPI::ClearChart(int chart_id)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return ;
	pChartDlg->ClearChart(chart_id);
}