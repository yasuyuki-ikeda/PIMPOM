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
//CommandPromptDlg.cpp
//
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "CommandPromptDlg.h"
#include "shlwapi.h"
#include "Windowsx.h"

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

bool Is2ByteCode(CString str, int index)
{
	if(str.GetLength()<2)	return false;	

	char buf[2];
	buf[0] = str[index];
	buf[1] = str[index+1];

	if( _mbclen( (BYTE*)buf ) ==2){
		return true;
	}
	
	return false;
}

// CCommandPromptDlg ダイアログ

//////////////////////////////////////////////////////////////////////////////////////////
//  メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////

//バッチ実行用
bool CCommandPromptDlg::OnBatchExecute(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param)
{
	if( ::IsWindowVisible(m_hWnd) == false)	return false;

	KillTimer(0);//一旦タイマーを切る

	API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, 0);//プログラムカウンタ初期化

	m_calclate.ClearAll();//結果クリア
	show_all_result();



	memset(blockMap, 0, sizeof(bool)*CALC_COMMAND_LINE_MAX);//blockMapの初期化
	for(;;)
	{
		int program_counter=0;
		API.GetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, &program_counter);//プログラムカウンタ取得
		
		if (step_execute(&program_counter, in_param, out_param, true, blockMap)){//実行
			API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, program_counter);//プログラムカウンタ更新	
		}else{
			break;
		}
	}



	SetTimer(0,10,NULL);//タイマー再開

	return true;
}

/********************************************************************
機  能  名  称 : スクリプトの編集履歴を退避する
関    数    名 : log_text_data
引          数 : 
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::log_text_data()
{
	CString str;
	m_rich_edit.GetWindowText(str);

	if (pUndoTbl[UNDO_MAX - 1] != NULL){
		delete[] pUndoTbl[UNDO_MAX - 1];
	}

	for (int n = UNDO_MAX-1; n > 0; n--)
	{
		pUndoTbl[n] = pUndoTbl[n - 1];
	}

	pUndoTbl[0] = new char[str.GetLength()+1 ];
	if (pUndoTbl[0])
	{
		strcpy(pUndoTbl[0], str.GetBuffer());
		str.ReleaseBuffer();
	}

	undoID = 0;
}


/********************************************************************
機  能  名  称 :ある区間のテキストの色決め
関    数    名 : SetTextColor
引          数 : 文字インデックス／長さは2バイト文字も1文字とカウントしたときの順序で設定すること
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::set_text_color(CRichEditCtrl	*pedit,
									 int			startMultiChar,		//(in)色決めする区間の開始文字インデックス
									 int			lenMultiChar,		//(in)色決めする区間の長さ
									 COLORREF		color				//(in)文字色
									 )
{
	//return;

	long old_sel_start, old_sel_end ;
	pedit->GetSel(old_sel_start,old_sel_end);//現在の選択範囲を取得する


	//色設定
	CHARFORMAT2 cf;

	pedit->SetSel( startMultiChar, startMultiChar + lenMultiChar);//選択範囲を変える

	pedit->GetDefaultCharFormat(cf);

	cf.crTextColor = color;//色指定

	pedit->SetSelectionCharFormat(cf);

	pedit->SetSel(old_sel_start,old_sel_end);//選択範囲をもとにもどす

}

/********************************************************************
機  能  名  称 : 行番号表示の設定
関    数    名 : set_line_index
引          数 : 行番号 -1の場合はすべて
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void	 CCommandPromptDlg::show_line_number(int index, bool enhanceColor)
{
	if (index == -1)
	{
		//すべて
		CString strbuf(_T("")), str;
		for (int i = 0; i<CALC_COMMAND_LINE_MAX; i++) 
		{
			if (brakePoints[i])
			{
				str.Format("%04d@\r\n", i);
			}
			else {
				str.Format("%04d \r\n", i);
			}
			strbuf += str;
		}
		m_rich_edit4.SetWindowText(strbuf);
	}
	else if(0<= index && index<CALC_COMMAND_LINE_MAX)
	{
		//指定した番号のみ
		CString strbuf(_T(""));
		m_rich_edit4.GetWindowText(strbuf);

		if (brakePoints[index])
		{
			strbuf.SetAt(index * 7 + 4, '@');
		}
		else {
			strbuf.SetAt(index * 7 + 4, ' ');
		}
		m_rich_edit4.SetWindowText(strbuf);

		if (enhanceColor) {
			set_text_color(&m_rich_edit4, index * 6, 6, RGB(255, 0, 0));//色付け
		}
	}

}

/********************************************************************
機  能  名  称 : マクロの説明表示
関    数    名 : show_macro_discription
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::show_macro_discription(int macroid)
{
	if(macroid<0 || macroid>= CALC_MACRO_CNT_MAX)	return ;

	MACRO_DEF_STRUCT mcr_def={0};

	if( !m_calclate.GetMacroDefinision(macroid, &mcr_def))	return ;//マクロ定義を取得

	API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_MACRO_DISCRIPTION, m_macro_description[macroid]);//マクロの説明を表示する


	//マクロリストから当該マクロを選択する
	for(int i=0 ; i<m_list_macro.GetCount() ; i++){
		CString str;
		m_list_macro.GetText(i,str);
		if(str==CString(mcr_def.name)){
			m_list_macro.SetCurSel(i);
			break;
		}
	}
}



/********************************************************************
機  能  名  称 : 変数宣言
関    数    名 : comp_variable_definition
引          数 : 
戻    り    値 : 文字列が見つかった場合はtrue
機          能 : "var "を探す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCommandPromptDlg::comp_variable_definition(CString  textBuffer,		//(in)比較対象文字列
												 int	  index,			//(in)文字列のインデックス(文字列が見つかった場合はその分進める)
												 int	  *pLength			//(out)
													)
{
	CString target=_T("var ");//変数宣言のキーワード


	for(int i=0 ; i<target.GetLength() ; i++){
		if( target[i] != textBuffer[index+i] ){
			return false;
		}
	}

	if(index>0){//変数宣言の前にはなにも文字がきてはいけない
		if( isalnum((int)textBuffer[index-1]) ){
			return false;
		}
	}

	*pLength = target.GetLength();
	return true;
}


/********************************************************************
機  能  名  称 : ""で囲まれる文字列かどうかしらべる
関    数    名 : comp_string_value
引          数 : 
戻    り    値 : 文字列が見つかった場合はtrue
機          能 : textBufferが""で囲まれる文字列かどうかしらべる
                 最初の文字が"ならば、そこから改行までの範囲を文字列とする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCommandPromptDlg::comp_string_value(	CString textBuffer,			//(in)比較対象文字列
											int		intex,				//(in)文字列のインデックス
											int		*pLength,			//(out)文字列の長さ
											int		*pLengthMultiChar	//(out)2バイト文字も1文字とカウントしたときの文字列の長さ
									   )
{
	//1文字目を調べる
	if(textBuffer[intex]!='"'){
		return false;//最初の文字が"でなければ文字列ではない
	}
	

	//2文字目以降を調べる
	int tmp_index = intex + 1;
	int count_multi_char = 1;

	while(textBuffer[tmp_index]!=0 && textBuffer[tmp_index]!='\r')//改行か終端がくるまで調べる
	{
		if(textBuffer[tmp_index]=='"'){//"が見つかった
			tmp_index++;
			count_multi_char++;
			break;
		}else{
			if(Is2ByteCode(textBuffer,tmp_index )){//2バイト文字
				tmp_index+=2;
				count_multi_char++;
			}else{//1バイト文字
				tmp_index++;
				count_multi_char++;
			}
		}
	}

	*pLength = tmp_index - intex;
	*pLengthMultiChar = count_multi_char;
	return true;
}


/********************************************************************
機  能  名  称 : 行ごとのインデックスを調べる
関    数    名 : get_command_lines
引          数 : 
戻    り    値 : 行数
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CCommandPromptDlg::get_command_lines(CString				textString,			//(in)エディットコントロールから取得したテキスト全体
									   COMMAND_LINE_STRUCT	commandLines[])		//(out)コマンドライン
{
	int commandLineCount=0;//行数
	
	
	//出力引数初期化
	memset(commandLines, 0, sizeof(COMMAND_LINE_STRUCT)*COMMAND_LINE_MAX );


	//行数および各行あたりの文字数を数える

	int index=0;//textStringのインデックス
	int indexMultiChar=0;//2バイト文字も1文字とカウントした場合のtextStringのインデックス

	while( index < textString.GetLength())
	{
		if(index+1 <textString.GetLength() && textString[index]=='\r' && textString[index+1]=='\n')//改行コード
		{
			if(commandLineCount+1 > COMMAND_LINE_MAX)
			{//行数が多すぎる
				API.MessageBox("too match lines (%d)", COMMAND_LINE_MAX);
				return -1;
			}

			commandLineCount++;//コマンドライン進める

			//行の開始位置を設定
			commandLines[ commandLineCount ].lineStartIndex = index+2;
			commandLines[ commandLineCount ].lineStartIndexMultiChar = indexMultiChar+1;

			//行の長さを初期化
			commandLines[ commandLineCount ].lineLength =0;
			commandLines[ commandLineCount ].lineLengthMultiChar = 0;

			index+=2;
			indexMultiChar++;
		}
		else{
			
			if( Is2ByteCode(textString,index ) )
			{//2バイト文字
				commandLines[ commandLineCount ].lineLength +=2;
				commandLines[ commandLineCount ].lineLengthMultiChar ++;

				index+=2;
				indexMultiChar++;
				
			}else{//1バイト文字
				commandLines[ commandLineCount ].lineLength ++;
				commandLines[ commandLineCount ].lineLengthMultiChar ++;

				index++;
				indexMultiChar++;
			}
		}
	}

	commandLineCount++;//最後に1行進める


	return commandLineCount;
}

/********************************************************************
機  能  名  称 : コメント／マクロ名／テキストに応じて色つけする
関    数    名 : color_command_lines
引          数 : bool isFirstRead      (in)初めて読み込むテキストかどうか
戻    り    値 : 行数
機          能 : ・isFirstReadがtrueのときは、すべての行について色付けする
　　　　　　　　 ・isFirstReadがfalseのとき、前回この関数が呼ばれた時とコマンド行数が2以上変化していれば、すべての行について色付けする
		 　　　　・上記以外の場合は、カーソルがある行のみ色付けする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::color_command_lines(bool isFirstRead)	
{
	COMMAND_LINE_STRUCT commandLines[COMMAND_LINE_MAX]={0};//コマンドラインのインデックス
	int commandLineCount=0;//行数
	static int prevCmmandLineCount=0;//前回この関数が呼ばれたときの行数
	CString commandString;
	long CurrentSelEnd, CurrentSelStart;



	m_rich_edit.GetSel(CurrentSelStart, CurrentSelEnd);//現在エディットコントロールで選択中の位置取得

	m_rich_edit.GetWindowText(commandString);//エディットコントロールからテキスト取得

	commandLineCount = get_command_lines(commandString, commandLines);//行番号を取得

	if(isFirstRead){
		set_text_color(&m_rich_edit, 0, m_rich_edit.GetTextLength() , RGB(0,0,0));//最初の読み込み時は全行を一旦黒にする
	}


	for(int i=0 ; i<commandLineCount ; i++)//すべての行について実施
	{
		if(commandLines[i].lineLength==0)
		{
			//空白行
		}
		else
		{//空白でない行

			if(!isFirstRead){//最初の読み込み時以外で、行数変化行数が1行以下の場合、カーソルのある行でのみ色付け処理をする
				if(abs(prevCmmandLineCount - commandLineCount)<=1){
					if(CurrentSelEnd < commandLines[i].lineStartIndexMultiChar-1   || CurrentSelStart >  commandLines[i].lineStartIndexMultiChar + commandLines[i].lineLengthMultiChar + 1)	continue;
				}

				set_text_color(&m_rich_edit, commandLines[i].lineStartIndexMultiChar, commandLines[i].lineLengthMultiChar, RGB(0,0,0));//行を黒にする
			}

			CString commandStr = commandString.Mid(commandLines[i].lineStartIndex, commandLines[i].lineLength);//注目行

				//コマンドライン制御文字列を探す
				int ctrlID, ctrlCodeLen=0;
				if( m_calclate.FindCtrlCode( commandStr, 0, &ctrlID ,&ctrlCodeLen) )
				{
					set_text_color(&m_rich_edit, commandLines[i].lineStartIndexMultiChar, ctrlCodeLen, RGB(0,0,255));//コマンドライン制御文字列を青にする
				}
				

				//マクロ、文字列を探す

				int idx = commandLines[i].lineStartIndex;
				int idxMultiChar  =commandLines[i].lineStartIndexMultiChar; 

				while( idx < commandLines[i].lineStartIndex + commandLines[i].lineLength )
				{
					int macroId;
					int len, multiLen;

					if( comp_variable_definition(commandString, idx, &len) )//変数宣言かどうかチェック
					{
						set_text_color(&m_rich_edit, idxMultiChar, 3, RGB(0,0,255));//青にする

						idx += len;
						idxMultiChar += len;
					}
					else 
					if( comp_string_value(commandString, idx, &len, &multiLen)  )//文字列引数かどうかチェック
					{	
						set_text_color(&m_rich_edit, idxMultiChar, multiLen, RGB(128,0,0));//文字列をワインレッドにする

						idx += len;
						idxMultiChar += multiLen;
					}
					else if(m_calclate.IsComment(commandString,idx))//コメントかどうかチェック
					{
						int commentLenMultiChar = commandLines[i].lineLengthMultiChar - (idxMultiChar - commandLines[i].lineStartIndexMultiChar );
						
						set_text_color(&m_rich_edit, idxMultiChar, commentLenMultiChar, RGB(0,200,0));//行を緑にする

						break;//コメントならばブレーク
					}
					else 
					if(m_calclate.IsMacroName( commandString, idx, &macroId, &len) )//マクロかどうかチェック
					{
						set_text_color(&m_rich_edit, idxMultiChar, len, RGB(255,100,100));//マクロを赤にする

						if(idxMultiChar <= CurrentSelEnd && idxMultiChar+len+1 >= CurrentSelEnd)
						{//カーソルがマクロ位置にあれば、説明を出す
							show_macro_discription(macroId);
						}

						idx += len;
						idxMultiChar += len;
					}
				    else
					{
						if(Is2ByteCode(commandString,idx ))
						{//2バイト文字
							idx+=2;
							idxMultiChar++;
						}else{//1バイト文字
							idx++;
							idxMultiChar++;
						}
					}
						
			}
		}
	}

	prevCmmandLineCount = commandLineCount;
}




/********************************************************************
機  能  名  称 : ステップ実行
関    数    名 : OnBnClickedButtonCommandPromptDo
引          数 : 
戻    り    値 : 正常実行終了後、次に進むことがれば true
				 エラー当の原因で処理が止まる場合はfalse
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCommandPromptDlg::step_execute(int			*p_program_counter,			//(in/out)プログラムカウンタ
									BATCH_INPUT		*p_batch_in,		//(in)
									BATCH_OUTPUT	*p_batch_out,		//(out)
									bool			fast,
									bool			*pJumped				//(in/out)
									)
{
	CString				comStrBuffer(""), tmpErr("");
	COMMAND_LINE_STRUCT commandLines[COMMAND_LINE_MAX]={0};//コマンドラインのインデックス
	int					commandLineCount=0;	//行数
	bool				ret=true;

	
	m_rich_edit.GetWindowText(comStrBuffer);//エディットコントロールから全テキストを取得

	commandLineCount = get_command_lines(comStrBuffer, commandLines);//全コマンド行取得

	//プログラムカウンタの行番号を赤色、それ以外の行番号をグレーにする
	show_line_number();
	show_line_number(*p_program_counter, true);

	//すべてのコマンド行を登録
	CString comStr[COMMAND_LINE_MAX];
	for(int i=0 ; i<commandLineCount ; i++){
		comStr[i] = comStrBuffer.Mid(commandLines[i].lineStartIndex, commandLines[i].lineLength);
	}

	if( ret = m_calclate.RegisterCommandString(comStr, commandLineCount, &tmpErr) )//コマンド解析
	{
		ret = m_calclate.ExecuteAPI(*p_program_counter , &tmpErr, p_program_counter, p_batch_in, p_batch_out, 
			API.GetCheck(m_hWnd, IDC_CHECK_COMMAND_PROMPT_AUTO_DRAWIMAGE),
			pJumped
			) ;//1行実行
	}else{
		//コマンド解析時のエラーメッセージは表示する
		if(!tmpErr.IsEmpty()){
			p_batch_out->state = BATCH_EXEC_STATE_STOP;
			AfxMessageBox(tmpErr);
		}
	}


	//バッチ実行のパラメタを保存しておく
	if (p_batch_in != &batchInBuffer){
		memcpy(&batchInBuffer, p_batch_in, sizeof(BATCH_INPUT));
	}
	if (p_batch_out != &batchOutBuffer){
		memcpy(&batchOutBuffer, p_batch_out, sizeof(BATCH_OUTPUT));
	}

	if (!fast)
	{
		//全結果表示
		show_all_result();

		//変数一覧表示
		show_variables();
	}

	return ret;

}

/********************************************************************
機  能  名  称 : 全結果表示
関    数    名 : show_all_result
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::show_all_result()
{
	CString	 comStrBuffer(""), resultStrBuffer(""), errStrBuffer(""), tmpStr, tmpErr;

	//全結果表示
	for(int i=0 ; i<m_calclate.GetCommandLineLen() ; i++)
	{
		double		number;
		char		strbuf[CALC_STRING_LEN_MAX];
		DWORD		elapsed_time = 0;
		int			result_type = m_calclate.GetResult(i, &number, strbuf, &tmpErr, &elapsed_time);//結果取得	
		
		//エラーメッセージを画面表示に反映
		errStrBuffer += tmpErr + CString("\r\n");
		
		//結果を画面表示に反映
		if(result_type>=0){//結果取得成功
			if (result_type == COM_TYPE_NUMBER)		tmpStr.Format("Res(%d)=%lf (%d[ms])", i, number, elapsed_time);
			else									tmpStr.Format("Res(%d)=\"%s\" (%d[ms])", i, strbuf, elapsed_time);
		}else{//結果取得失敗
			tmpStr.Empty();
		}
		resultStrBuffer += tmpStr + CString("\r\n");

	}

	m_rich_edit2.SetWindowText(errStrBuffer);//エラーメッセージ一覧
	m_rich_edit3.SetWindowText(resultStrBuffer);//結果一覧
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
void CCommandPromptDlg::arrange_dlg_item()
{
	RECT clientRct={0};//クライアント領域を表す矩形
	int clientWidth, clientHeight, offsetx, offsety;

	::GetClientRect(m_hWnd, &clientRct);//クライアント領域を表す矩形を取得

	clientWidth = (clientRct.right - clientRct.left);
	clientHeight = (clientRct.bottom - clientRct.top);
	offsetx = 2;
	offsety = 0;



	//ダイアログアイテム
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_RICHEDIT_COMMAND_PROMPT),					offsetx + clientWidth*5/100,	offsety,					clientWidth*55/100-2, clientHeight*70/100-4, true);//コマンドプロンプト
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_RICHEDIT_COMMAND_PROMPT2),				offsetx + clientWidth*60/100,	offsety,					clientWidth*20/100-2, clientHeight*70/100-4, true);//エラーメッセージ
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_RICHEDIT_COMMAND_PROMPT3),				offsetx + clientWidth*80/100,	offsety,					clientWidth*20/100-2, clientHeight*70/100-4, true);//結果の値
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_RICHEDIT_COMMAND_PROMPT4),				offsetx,					offsety,					clientWidth*5/100-2,  clientHeight*70/100-4, true);//行番号
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHECK_COMMAND_PROMPT_FIND),				offsetx,					offsety+clientHeight*70/100,   20, 20, true);//マクロ検索
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_COMMAND_PROMPT_MACRO),				offsetx+20,					offsety+clientHeight*70/100,   clientWidth*30/100-2-20, 20, true);//マクロ検索
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_LIST_COMMAND_PROMPT_MACRO),				offsetx,					offsety+clientHeight*70/100+20,	clientWidth*30/100-2, clientHeight*25/100-20, true);//マクロ一覧
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_EDIT_COMMAND_PROMPT_MACRO_DISCRIPTION),	offsetx + clientWidth*30/100,	offsety+clientHeight*70/100,	clientWidth*50/100-2, clientHeight*25/100, true);//マクロ説明
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_LIST_VARIABLE),							offsetx + clientWidth*80/100,	offsety+clientHeight*70/100,	clientWidth*20/100-2, clientHeight*25/100, true);//変数ウォッチ


	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_STATIC_COMMAND_PROMPT_PROGRAM_COUNTER),	offsetx,	offsety+clientHeight*95/100, 80, 20, true);//プログラムカウンタのキャプション
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER),	offsetx + 80,   offsety+clientHeight*95/100, 40, 20, true);//プログラムカウンタの値
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_BUTTON_COMMAND_PROMPT_DO),	offsetx + 120,   offsety+clientHeight*95/100, 80, 20, true);//ステップ実行」ボタン
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_CHECK_COMMAND_PROMPT_DO),	offsetx + 200,   offsety+clientHeight*95/100, 80, 20, true);//「連続実行」チェックボックス
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_BUTTON_COMMAND_PROMPT_RESULT_CLEAR),	offsetx + 280,   offsety+clientHeight*95/100, 80, 20, true);//「結果クリア」ボタン
	//::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHECK_COMMAND_PROMPT_AUTO_DRAWIMAGE), offsetx + 360, offsety + clientHeight * 95 / 100, 200, 20, true);//自動再描画ボタン
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_COMMAND_PROMPT_UNDO), offsetx + 360, offsety + clientHeight * 95 / 100, 60, 20, true);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_COMMAND_PROMPT_REDO), offsetx + 360 + 60, offsety + clientHeight * 95 / 100, 60, 20, true);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_COMMAND_PROMPT_TEST_DO), offsetx + 560, offsety + clientHeight * 95 / 100, 60, 20, true);//テスト実行ボタン
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_COMMAND_PROMPT_TEST), offsetx + 620, offsety + clientHeight * 95 / 100, 500, 20, true);//テスト実行用コマンドプロンプト

	if(API.GetCheck(m_hWnd,IDC_CHECK_COMMAND_PROMPT_DO))
	{
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_RICHEDIT_COMMAND_PROMPT),false);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_COMMAND_PROMPT_DO),false);//「ステップ実行」ボタンを押せなくする
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_COMMAND_PROMPT_RESULT_CLEAR),false);//「結果クリア」ボタンを押せなくする
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER),false);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_LIST_COMMAND_PROMPT_MACRO),false);//マクロ一覧リストを使えなくする
	}else{
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_RICHEDIT_COMMAND_PROMPT),true);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_COMMAND_PROMPT_DO),true);//「ステップ実行」ボタンを使用可能にする
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_COMMAND_PROMPT_RESULT_CLEAR),true);//「結果クリア」ボタンを使用可能にする
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER),true);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_LIST_COMMAND_PROMPT_MACRO),true);//マクロ一覧リストを使えるようにする
	}
}


/********************************************************************
機  能  名  称 : 変数ウォッチリスト
関    数    名 : init_variable_watch
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::init_variable_watch()
{
    LVCOLUMN    lvc;
    char       caption[32];

	m_var_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

    lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // 有効フラグ


	//変数名
	sprintf(caption, "変数");
    lvc.iSubItem    = 0;            // サブアイテム番号
    lvc.pszText     = caption;   // 見出しテキスト
    lvc.cx          = 80;          // 横幅
    m_var_list.InsertColumn(0, &lvc);

	//値
	sprintf(caption, "値");
    lvc.iSubItem    = 1;            // サブアイテム番号
    lvc.pszText     = caption;   // 見出しテキスト
    lvc.cx          = 80;          // 横幅
    m_var_list.InsertColumn(1, &lvc);
   
}



/********************************************************************
機  能  名  称 : 変数のリスト表示
関    数    名 : show_variables
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::show_variables()
{
	LVITEM       lvi={0};
	lvi.mask = LVIF_TEXT;
	char        buf[1024];
	

	m_var_list.DeleteAllItems();//一旦すべてのリスト要素をクリア


	CALC_RESULT_STRUCT varres={0};
	char	varname[CALC_VARIABLE_NAME_MAX]={0};

	int varID=0;
	while( m_calclate.GetVariable(varID,&varres,varname) )
	{
			//変数名
			lvi.iItem = varID;
			lvi.iSubItem = 0;
			sprintf(buf, "%s", varname);
			lvi.pszText = buf;
			m_var_list.InsertItem(&lvi);//リスト挿入

			//値
			lvi.iItem = varID;
			lvi.iSubItem = 1;
			if(varres.type == COM_TYPE_NUMBER){
				sprintf(buf, "%lf", varres.number);
			}else if(varres.type = COM_TYPE_STRING){
				sprintf(buf, "%s", varres.str);
			}
			lvi.pszText = buf;
			m_var_list.SetItem(&lvi);

		varID++;
	}

	for (int i = 0; i < BATCH_IN_PARAM_NUM; i++)
	{
		//変数名
		lvi.iItem = varID;
		lvi.iSubItem = 0;
		sprintf(buf, "BATCH_IN[%d]", i);
		lvi.pszText = buf;
		m_var_list.InsertItem(&lvi);//リスト挿入

		//値
		lvi.iItem = varID;
		lvi.iSubItem = 1;
		sprintf(buf, "%s", batchInBuffer.param[i]);
		lvi.pszText = buf;
		m_var_list.SetItem(&lvi);

		varID++;
	}

	for (int i = 0; i < BATCH_OUT_PARAM_NUM; i++)
	{
		//変数名
		lvi.iItem = varID;
		lvi.iSubItem = 0;
		sprintf(buf, "BATCH_OUT[%d]", i);
		lvi.pszText = buf;
		m_var_list.InsertItem(&lvi);//リスト挿入

		//値
		lvi.iItem = varID;
		lvi.iSubItem = 1;
		sprintf(buf, "%s", batchOutBuffer.result[i]);
		lvi.pszText = buf;
		m_var_list.SetItem(&lvi);

		varID++;
	}
}


/********************************************************************
機  能  名  称 : マクロの解説文書を読み込む
関    数    名 : load_macro_description
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCommandPromptDlg::load_macro_description()
{
	FILE *fp = fopen(API.GetDefaultPath() + "..\\..\\..\\Doc\\macro_description.txt", "rt");//ファイルを開く
	if(fp)
	{
		char tmp[CALC_STRING_LEN_MAX]={0};
		CString lineBuffer[CALC_COMMAND_LINE_MAX];
		int line_number=0;

		//ファイルから全行読み込み
		for(line_number=0; line_number<CALC_COMMAND_LINE_MAX; line_number++){
			if(fgets(tmp, CALC_STRING_LEN_MAX, fp) == NULL){
				break;
			}else{
				lineBuffer[line_number].Format("%s\r\n", tmp);
			}
		}

		fclose(fp);



		//マクロ要素ごとに説明文を探す
		for(int i=0 ; i<CALC_MACRO_CNT_MAX ; i++)
		{
			m_macro_description[i].Empty();

			MACRO_DEF_STRUCT mac={0};
			m_calclate.GetMacroDefinision(i, &mac);//マクロ名取得

			if(strlen(mac.name)<=0)	continue;

			int start_line=line_number;
			for(int j=0 ; j<line_number ; j++){
				if( lineBuffer[j].Find( CString(mac.name) + CString("("), 0) >=0 ){//マクロ名が書かれた行を探す
					start_line = j;
					break;
				}
			}

			for(int j=start_line ; j<line_number ; j++){
				if( lineBuffer[j].Find( CString("======"), 0) >=0  ){//マクロ説明の終端を探す
					break;
				}else{
					m_macro_description[i] += lineBuffer[j];
				}
			}
		}

		return true;
	}else{
		return false;
	}
}

/********************************************************************
機  能  名  称 : スクリプトファイルを開く
関    数    名 : open_script_file
引          数 : (in)folder_path
                 (in)file_name
戻    り    値 : 成功したら true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCommandPromptDlg::open_script_file(char folder_path[], char file_name[])
{
	int	file_number;
	CString filetype[2], path_str, file_name_str;
	FILE *fp;

	if (file_name == NULL)
	{
		filetype[0].LoadString(IDS_FILETYPE_TXT);
		filetype[1].LoadString(IDS_FILETYPE_ANY);
		if (!API.OpenFileDialog(1, filetype[0] + filetype[1] + _T("|"), &path_str, &file_name_str, &file_number, folder_path))	return false;//ファイルダイアログを開いてファイルパスを取得
	}
	else
	{
		file_name_str = CString(file_name);
		path_str = CString(folder_path);
	}


	fp = fopen(path_str + file_name_str, "rt");//ファイルを開く
	if(fp)
	{
		CString allStr("");
		char tmp[CALC_STRING_LEN_MAX]={0};

		for(int i=0 ; i<CALC_COMMAND_LINE_MAX ; i++)
		{
			if( fgets(tmp, CALC_STRING_LEN_MAX-1, fp) != NULL ){//テキストを1行読み込む
				allStr += CString(tmp);//
			}
			else
			{
				rich_edit_change_event_active_flg = false;//リッチエディットの編集イベントを切る

				OnBnClickedButtonCommandPromptResultClear();//結果クリア

				m_rich_edit.SetWindowText(allStr);//読み込んだテキストをエディットに反映

				log_text_data();

				color_command_lines(true);//文字色つけ

				rich_edit_change_event_active_flg=true;//リッチエディットの編集イベントをアクティベート

				m_filename = path_str + file_name_str;//読み込んだファイル名を残す

				::SetWindowText(m_hWnd, m_filename + CString(" -スクリプト"));//ファイル名をダイアログのキャプションに反映する
				break;
			}
		}

		fclose(fp);

		return true;
	}else{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//イベントハンドラ
//////////////////////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC(CCommandPromptDlg, CDialog)

CCommandPromptDlg::CCommandPromptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommandPromptDlg::IDD, pParent)
{
	m_filename = _T("");
	AfxInitRichEdit2();
}

CCommandPromptDlg::~CCommandPromptDlg()
{
	//メモリ解放
	for (int i = 0; i < UNDO_MAX; i++){
		if (pUndoTbl[i] != NULL){
			delete[] pUndoTbl[i];
		}
	}

	m_bmp_find.DeleteObject();
}

void CCommandPromptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_COMMAND_PROMPT, m_rich_edit);
	DDX_Control(pDX, IDC_RICHEDIT_COMMAND_PROMPT2, m_rich_edit2);
	DDX_Control(pDX, IDC_RICHEDIT_COMMAND_PROMPT3, m_rich_edit3);
	DDX_Control(pDX, IDC_RICHEDIT_COMMAND_PROMPT4, m_rich_edit4);
	DDX_Control(pDX, IDC_LIST_COMMAND_PROMPT_MACRO, m_list_macro);
	DDX_Control(pDX, IDC_LIST_VARIABLE, m_var_list);
}


BEGIN_MESSAGE_MAP(CCommandPromptDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_COMMAND_PROMPT_DO, &CCommandPromptDlg::OnBnClickedButtonCommandPromptDo)
	ON_EN_CHANGE(IDC_RICHEDIT_COMMAND_PROMPT, &CCommandPromptDlg::OnEnChangeRicheditCommandPrompt)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, &CCommandPromptDlg::OnEnChangeEditCommandPromptProgramCounter)
	ON_BN_CLICKED(IDC_BUTTON_COMMAND_PROMPT_RESULT_CLEAR, &CCommandPromptDlg::OnBnClickedButtonCommandPromptResultClear)
	ON_BN_CLICKED(IDC_CHECK_COMMAND_PROMPT_DO, &CCommandPromptDlg::OnBnClickedCheckCommandPromptDo)
	ON_WM_SIZE()
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_NEW, &CCommandPromptDlg::OnCommandPromptMenuNew)
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_OPEN, &CCommandPromptDlg::OnCommandPromptMenuOpen)
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_SAVE, &CCommandPromptDlg::OnCommandPromptMenuSave)
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_SAVEAS, &CCommandPromptDlg::OnCommandPromptMenuSaveas)
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_STEP_EXE, &CCommandPromptDlg::OnCommandPromptMenuStepExe)
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_SEQ_EXE, &CCommandPromptDlg::OnCommandPromptMenuSeqExe)
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_CLEAR, &CCommandPromptDlg::OnCommandPromptMenuClear)
	ON_LBN_SELCHANGE(IDC_LIST_COMMAND_PROMPT_MACRO, &CCommandPromptDlg::OnLbnSelchangeListCommandPromptMacro)
	ON_LBN_DBLCLK(IDC_LIST_COMMAND_PROMPT_MACRO, &CCommandPromptDlg::OnLbnDblclkListCommandPromptMacro)
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_SAMPLE, &CCommandPromptDlg::OnCommandPromptMenuSample)
	ON_EN_UPDATE(IDC_RICHEDIT_COMMAND_PROMPT, &CCommandPromptDlg::OnEnUpdateRicheditCommandPrompt)
	ON_EN_CHANGE(IDC_EDIT_COMMAND_PROMPT_MACRO, &CCommandPromptDlg::OnEnChangeEditCommandPromptMacro)
	ON_BN_CLICKED(IDC_CHECK_COMMAND_PROMPT_FIND, &CCommandPromptDlg::OnBnClickedCheckCommandPromptFind)
	ON_BN_CLICKED(IDC_BUTTON_COMMAND_PROMPT_TEST_DO, &CCommandPromptDlg::OnBnClickedButtonCommandPromptTestDo)
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_REFERENCE, &CCommandPromptDlg::OnCommandPromptMenuReference)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_COMMAND_PROMPT_UNDO, &CCommandPromptDlg::OnBnClickedButtonCommandPromptUndo)
	ON_BN_CLICKED(IDC_BUTTON_COMMAND_PROMPT_REDO, &CCommandPromptDlg::OnBnClickedButtonCommandPromptRedo)
	ON_WM_DESTROY()
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
BOOL CCommandPromptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(AfxGetApp()->LoadIcon(IDI_RUN), TRUE);			// アイコンを設定
	
	m_bmp_find.LoadBitmap(IDB_FIND);
	CButton	*p_button = (CButton*)GetDlgItem(IDC_CHECK_COMMAND_PROMPT_FIND);
	p_button->SetBitmap((HBITMAP)m_bmp_find);
	

	arrange_dlg_item();

	init_variable_watch();

	load_macro_description();

	//イベントマスク設定
	m_rich_edit.SetEventMask(ENM_CHANGE);

	//デフォルトフォーマット設定
	CHARFORMAT2 cf;
	m_rich_edit.GetDefaultCharFormat(cf);
	cf.dwMask = CFM_COLOR | CFM_SIZE | CFM_FACE |CFM_CHARSET ;
	cf.yHeight = 15 * 12;//文字サイズ
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	cf.crTextColor = RGB(0,0,0);//文字色(黒)
	cf.bCharSet = SHIFTJIS_CHARSET;//文字コードセット
	lstrcpy(cf.szFaceName, TEXT("ＭＳ ゴシック"));//フォント

	m_rich_edit.SetDefaultCharFormat(cf);
	m_rich_edit2.SetDefaultCharFormat(cf);
	m_rich_edit3.SetDefaultCharFormat(cf);

	cf.crTextColor = RGB(128,128,128);//グレー
	m_rich_edit4.SetDefaultCharFormat(cf);



	//マクロ一覧初期化
	for(int i=0 ; i<CALC_MACRO_CNT_MAX ; i++){
		MACRO_DEF_STRUCT mac;
		m_calclate.GetMacroDefinision(i, &mac);
		if(strlen(mac.name)>0){
			m_list_macro.AddString( CString(mac.name));
		}
	}

	memset(&batchInBuffer, 0, sizeof(BATCH_INPUT));
	memset(&batchOutBuffer, 0, sizeof(BATCH_OUTPUT));

	m_calclate.ClearAll();//コマンド初期化

	memset(blockMap, 0, sizeof(bool)*CALC_COMMAND_LINE_MAX);//PassedIfIDMapの初期化

	memset(brakePoints, 0, sizeof(bool)*CALC_COMMAND_LINE_MAX);//brakePointsの初期化


	//スクリプトの編集履歴を退避する場所の確保
	for (int i = 0; i < UNDO_MAX; i++){
		pUndoTbl[i] = NULL;
	}
	undoID = 0;
	log_text_data();

	API.SetCheck(m_hWnd, IDC_CHECK_COMMAND_PROMPT_AUTO_DRAWIMAGE, true);//自動再描画ON

	API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, 0);//プログラムカウンタ初期化

	show_line_number();//行番号表示初期化

	rich_edit_change_event_active_flg=true;//リッチエディットの編集をアクティベーと

	SetTimer(0,10,NULL);//タイマー開始
	
						
	//バックアップファイルを読み込む
	CString path = API.GetDefaultPath();
	open_script_file(path.GetBuffer(), "script_backup.txt");
	path.ReleaseBuffer();
	m_rich_edit.SetSel(0, 0);



	return true;
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
void CCommandPromptDlg::OnDestroy()
{
	//テキストバックアップ
	CString str;
	m_rich_edit.GetWindowText(str);
	FILE *fp = fopen(API.GetDefaultPath() + "\\script_backup.txt", "wt");
	if (fp) {
		fwrite(str.GetBuffer(), 1, str.GetLength(), fp);
		str.ReleaseBuffer();
		fclose(fp);
	}

	CDialog::OnDestroy();
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
BOOL CCommandPromptDlg::PreTranslateMessage(MSG* pMsg) 
{

	if(pMsg->message == WM_KEYDOWN )
	{//キー操作

		switch(pMsg->wParam){
			case VK_ESCAPE://Esc
				return  true;

			case VK_TAB://tab
				if (pMsg->hwnd == ::GetDlgItem(m_hWnd, IDC_RICHEDIT_COMMAND_PROMPT))
				{
					//スクリプトを記入するエディットボックスでtabを押された場合
					//ダイアログアイテム選択を変えるのではなく、エディットボックスにtabを挿入する
					m_rich_edit.ReplaceSel(_T("\t"));
					return true;
				}
				break;
			default:
				break;
		}
	}

	else if (pMsg->message == WM_LBUTTONDOWN)
	{//マウス左ボタンクリック
		if (pMsg->hwnd == ::GetDlgItem(m_hWnd, IDC_RICHEDIT_COMMAND_PROMPT4))
		{//行番号
			CString comStrBuffer;
			RECT win_rct;

			m_rich_edit4.GetWindowText(comStrBuffer);//エディットコントロールから全テキストを取得
			m_rich_edit4.GetWindowRect(&win_rct);//エディットコントロールの座標取得
		
			int charStart = 0, lineCnt=0;
			int prevCharPos = m_rich_edit4.GetCharPos(0).y + win_rct.top;
			for (int n = 1; n < comStrBuffer.GetLength()  ; n++)
			{
				int charPos = m_rich_edit4.GetCharPos(n).y + win_rct.top;

				if (pMsg->pt.y < charPos)
				{//行位置がクリック位置を超えた場合

					brakePoints[lineCnt] = !brakePoints[lineCnt];//ブレークポイント反転
					show_line_number(lineCnt);//行番号表示に反映
					break;
				}
				else if (prevCharPos < charPos)
				{//改行時
					charStart = n;
					lineCnt++;
				}

				prevCharPos = charPos;
			}
		}
	}





	return CDialog::PreTranslateMessage(pMsg);
}


/********************************************************************
機  能  名  称 : エディット内容変更に伴うブレークポイント変更
関    数    名 : change_break_points
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::change_break_points(char *pPrevStr, char *pPresentStr)
{
	bool		tmpbrakePoints[CALC_COMMAND_LINE_MAX] = {0};

	CString str;
	m_rich_edit.GetWindowText(str);
	char *pBuffer = str.GetBuffer();


	if (pPrevStr == NULL)
	{
		pPrevStr = pUndoTbl[0];
	}

	if (pPresentStr == NULL)
	{
		pPresentStr = pBuffer;
	}

	if (pUndoTbl[0])
	{//ひとつ前の履歴と比較

		int prevStrLen = (int)strlen(pPrevStr);
		int presentStrLen = (int)strlen(pPresentStr);

		//前方一致比較
		int lineCnt = 0;
		for (int i = 0; i<prevStrLen && i<presentStrLen; i++)
		{
			if (pPrevStr[i] != pPresentStr[i])
			{
				break;
			}
			else if(pPresentStr[i] =='\n')
			{//改行
				tmpbrakePoints[lineCnt] = brakePoints[lineCnt];//この行のブレークポイントをコピー
				lineCnt++;//行番号を進める
			}
		}

		if (strcmp(pPrevStr, pPresentStr) != 0 && presentStrLen>0)
		{
			//行番号を数える
			int prevLines = 0;
			int presentLines = 0;
			for (int i = 0; i < prevStrLen ; i++)
			{
				if (pPrevStr[i] == '\n') { prevLines++; }
			}

			
			for (int i = 0; i < presentStrLen; i++)
			{
				if (pPresentStr[i] == '\n') { presentLines++; }
			}

			//後方一致比較
			int prevLineCnt = prevLines-1;
			int presentLineCnt = presentLines-1;
			for (int i = prevStrLen - 1, j = presentStrLen - 1; i>=0 && j>=0; i--, j--)
			{
				if (pPrevStr[i] != pPresentStr[j])
				{
					break;
				}
				else if (pPresentStr[j] == '\n')
				{//改行
					tmpbrakePoints[presentLineCnt] = brakePoints[prevLineCnt];//この行のブレークポイントをコピー
					prevLineCnt--;//行番号を進める
					presentLineCnt--;
				}
			}
		}

		memcpy(brakePoints, tmpbrakePoints, sizeof(bool)*CALC_COMMAND_LINE_MAX);
		show_line_number();//行番号表示

	}

	str.ReleaseBuffer();
}



/********************************************************************
機  能  名  称 : エディット内容変更
関    数    名 : OnEnChangeRicheditCommandPrompt
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnEnChangeRicheditCommandPrompt()
{
	if(rich_edit_change_event_active_flg){
		color_command_lines();	//コマンドラインの色分け
		//API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, 0);//プログラムカウンタ初期化

		change_break_points();//ブレークポイントの変更

		log_text_data();
	}
}





/********************************************************************
機  能  名  称 :
関    数    名 : OnTimer
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/

void CCommandPromptDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(0);//一旦タイマーを切る
	
	//連続実行
	if(API.GetCheck(m_hWnd,IDC_CHECK_COMMAND_PROMPT_DO))
	{
		int program_counter=0;
		API.GetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, &program_counter);//プログラムカウンタ取得

		if (brakePoints[program_counter])
		{//ブレークポイント
			API.SetCheck(m_hWnd, IDC_CHECK_COMMAND_PROMPT_DO, false);
			arrange_dlg_item();

			//プログラムカウンタの行番号を赤色、それ以外の行番号をグレーにする
			show_line_number();
			show_line_number(program_counter, true);

			API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, program_counter+1);//プログラムカウンタをひとつ進めておく	
			return;
		}

		if (step_execute(&program_counter, &batchInBuffer, &batchOutBuffer, false, blockMap))
		{//実行
			API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, program_counter);//プログラムカウンタ更新	
		}else{
			API.SetCheck(m_hWnd,IDC_CHECK_COMMAND_PROMPT_DO,false);
			arrange_dlg_item();
		}
	}
	else
	{
		//各エディットのスクロール量をそろえる
		int line = m_rich_edit.GetFirstVisibleLine();
		int line2 = m_rich_edit2.GetFirstVisibleLine();
		int line3 = m_rich_edit3.GetFirstVisibleLine();
		int line4 = m_rich_edit4.GetFirstVisibleLine();
		
		m_rich_edit2.LineScroll(line-line2);
		m_rich_edit3.LineScroll(line-line3);
		m_rich_edit4.LineScroll(line-line4);
	}

	SetTimer(0,10,NULL);//タイマー再開

	CDialog::OnTimer(nIDEvent);
}



void CCommandPromptDlg::OnEnChangeEditCommandPromptProgramCounter()
{
	// TODO:  これが RICHEDIT コントロールの場合、
	// まず、CDialog::OnInitDialog() 関数をオーバーライドして、OR 状態の ENM_CHANGE
	// フラグをマスクに入れて、CRichEditCtrl().SetEventMask() を呼び出さない限り、
	// コントロールは、この通知を送信しません。

	// TODO:  ここにコントロール通知ハンドラ コードを追加してください。
}


/********************************************************************
機  能  名  称 : 「ステップ実行」ボタン押下
関    数    名 : OnBnClickedButtonCommandPromptDo
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnBnClickedButtonCommandPromptDo()
{
	int	program_counter=0;//プログラムカウンタ

	API.GetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, &program_counter);//プログラムカウンタ取得

	step_execute(&program_counter, &batchInBuffer, &batchOutBuffer, false, blockMap);//ステップ実行

	API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, program_counter);//プログラムカウンタ更新

}

/********************************************************************
機  能  名  称 : 「結果クリア」ボタン押下
関    数    名 : OnBnClickedButtonCommandPromptResultClear
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnBnClickedButtonCommandPromptResultClear()
{
	m_calclate.ClearAll();

	memset(&batchInBuffer, 0, sizeof(BATCH_INPUT));
	memset(&batchOutBuffer, 0, sizeof(BATCH_OUTPUT));

	show_all_result();

	//変数一覧表示
	show_variables();

	API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, 0);//プログラムカウンタ初期化

	set_text_color(&m_rich_edit4, 0 , m_rich_edit4.GetTextLength(), RGB(128,128,128));//行番号をすべてグレーにする

	memset(blockMap, 0, sizeof(bool)*CALC_COMMAND_LINE_MAX);//PassedIfIDMapの初期化
}

/********************************************************************
機  能  名  称 : 「連続実行」チェックボックス押下
関    数    名 : OnBnClickedButtonCommandPromptResultClear
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnBnClickedCheckCommandPromptDo()
{
	if(API.GetCheck(m_hWnd,IDC_CHECK_COMMAND_PROMPT_DO))
	{
		arrange_dlg_item();

		KillTimer(0);//一旦タイマーを切る

		//OnBnClickedButtonCommandPromptResultClear();//結果クリア

		SetTimer(0,10,NULL);//タイマー再開
	}else{
		arrange_dlg_item();
	}
}


/********************************************************************
機  能  名  称 : 「テスト実行」ボタン押下
関    数    名 : OnBnClickedButtonCommandPromptTestDo
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnBnClickedButtonCommandPromptTestDo()
{
	CString command_str, err_str;
	CCalculate		calc;


	API.GetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_TEST, &command_str);//コマンド取得

	if (calc.RegisterCommandString(&command_str, 1, &err_str)){
		calc.ExecuteAPI(0, &err_str, NULL, NULL, NULL,
			API.GetCheck(m_hWnd, IDC_CHECK_COMMAND_PROMPT_AUTO_DRAWIMAGE)
			);//1行実行
	}
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
void CCommandPromptDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	arrange_dlg_item();
}


/********************************************************************
機  能  名  称 : 「ファイル／新規作成」メニュー
関    数    名 : OnCommandPromptMenuNew
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuNew()
{
	OnBnClickedButtonCommandPromptResultClear();

	

	CString str(" ");
	m_rich_edit.SetWindowText(str);//エディットクリア

	memset(brakePoints, 0, sizeof(bool)*CALC_COMMAND_LINE_MAX);//brakePointsの初期化
	show_line_number();//行番号表示
}


/********************************************************************
機  能  名  称 : 「ファイル／開く」メニュー
関    数    名 : OnCommandPromptMenuNew
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuOpen()
{
	if (open_script_file())
	{
		memset(brakePoints, 0, sizeof(bool)*CALC_COMMAND_LINE_MAX);//brakePointsの初期化
		show_line_number();//行番号表示
	}
}


/********************************************************************
機  能  名  称 : 「ファイル／上書き保存」メニュー
関    数    名 : OnCommandPromptMenuSave
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuSave()
{
	if(!m_filename.IsEmpty())
	{	
		//過去に開いたor保存したファイル名がある
		if( PathFileExists( m_filename) )
		{
			//ファイルが存在する
			FILE *fp = fopen(m_filename, "wb");//ファイルを開く
			if(fp)
			{
				CString allStr("");
				
				m_rich_edit.GetWindowText(allStr);//コマンドライン文字列をすべて取得

				if( fwrite(allStr.GetBuffer(), allStr.GetLength(), 1, fp) != 1){ //ファイルを保存
					API.MessageBox("%s file saving failed", m_filename);
				}

				fclose(fp);
			}
		}else{//ファイルが存在しなければ、ファイル名をつけて保存しなおし
			OnCommandPromptMenuSaveas();
		}
	}else{//初めてならば、ファイル名をつけて保存
		OnCommandPromptMenuSaveas();
	}
}

/********************************************************************
機  能  名  称 : 「ファイル／名前をつけて保存」メニュー
関    数    名 : OnCommandPromptMenuSaveas
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuSaveas()
{
	CString filetype, pathname;
	FILE *fp;

	filetype.LoadString(IDS_FILETYPE_TXT);
	CFileDialog myDLG(false , "*.txt", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,  filetype);

	if(myDLG.DoModal() != IDOK)  return ;
	pathname = myDLG.GetPathName();

	fp = fopen(pathname, "wb");//ファイルを開く
	if(fp)
	{
		CString allStr("");
		
		m_rich_edit.GetWindowText(allStr);//コマンドライン文字列をすべて取得

		if( fwrite(allStr.GetBuffer(), allStr.GetLength(), 1, fp) != 1){ //ファイルを保存
			API.MessageBox("%s file saving failed", pathname);
		}else{
			m_filename = pathname;//保存したファイル名を残す
			::SetWindowText(m_hWnd, m_filename + CString(" -スクリプト"));//ファイル名をダイアログのキャプションに反映する
		}

		fclose(fp);
	}
	else{
		API.MessageBox("%s file opening failed", pathname);
	}
}

/********************************************************************
機  能  名  称 : 「実行／ステップ実行」メニュー
関    数    名 : OnCommandPromptMenuStepExe
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuStepExe()
{
	OnBnClickedButtonCommandPromptDo();
}

/********************************************************************
機  能  名  称 : 「実行／連続実行」メニュー
関    数    名 : OnCommandPromptMenuSeqExe
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuSeqExe()
{
	if(!API.GetCheck(m_hWnd,IDC_CHECK_COMMAND_PROMPT_DO)){
		API.SetCheck(m_hWnd,IDC_CHECK_COMMAND_PROMPT_DO,true);
	}
}

/********************************************************************
機  能  名  称 : 「実行／結果クリア」メニュー
関    数    名 : OnCommandPromptMenuClear
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuClear()
{
	OnBnClickedButtonCommandPromptResultClear();
}


/********************************************************************
機  能  名  称 : 「ヘルプ／コマンドプロンプトリファレンス」メニュー
関    数    名 : OnCommandPromptMenuClear
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuReference()
{
	ShellExecute(NULL, "open", API.GetDefaultPath() + "..\\..\\..\\Doc\\PIMPOM_スクリプトマニュアル.doc", NULL, NULL, SW_SHOWNORMAL);
}



/********************************************************************
機  能  名  称 : マクロ名リスト選択変更
関    数    名 : OnLbnSelchangeListCommandPromptMacro
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnLbnSelchangeListCommandPromptMacro()
{
	CString macro_name;
	int idx = m_list_macro.GetCurSel();
	m_list_macro.GetText(idx,macro_name);

	for(int i=0 ; i<CALC_MACRO_CNT_MAX ; i++)
	{
		MACRO_DEF_STRUCT mcr_def={0};
		if( m_calclate.GetMacroDefinision(i, &mcr_def))//マクロ定義を取得
		{
			if( macro_name == CString(mcr_def.name))
			{
				API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_MACRO_DISCRIPTION, m_macro_description[i]);//マクロの説明を表示する
				break;
			}
		}
	}
}

/********************************************************************
機  能  名  称 : マクロ名リストダブルクリック
関    数    名 : OnLbnSelchangeListCommandPromptMacro
引          数 : 
戻    り    値 : 
機          能 : コマンドプロンプトに選択マクロが挿入される
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnLbnDblclkListCommandPromptMacro()
{
	CString macro_name;
	int idx = m_list_macro.GetCurSel();
	m_list_macro.GetText(idx,macro_name);

	m_rich_edit.ReplaceSel(macro_name + CString("(") );
}

/********************************************************************
機  能  名  称 : サンプルスクリプトを開く
関    数    名 : OnCommandPromptMenuSample
引          数 :
戻    り    値 :
機          能 : 
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuSample()
{
	CString path = API.GetDefaultPath() + CString("..\\..\\..\\Doc\\ScriptSpl");
	char fillpathbuf[1024];
	_fullpath( fillpathbuf, path.GetBuffer() , 1024);
	path.ReleaseBuffer();
	
	open_script_file(fillpathbuf);
}


void CCommandPromptDlg::OnEnUpdateRicheditCommandPrompt()
{
	// TODO:  これが RICHEDIT コントロールの場合、
	// まず、CDialog::OnInitDialog() 関数をオーバーライドして、OR 状態の ENM_CHANGE
	// OR 状態の ENM_SCROLL フラグを IParam マスクに入れて、
	// OR 状態の ENM_UPDATE フラグを lParam マスクに入れて、

	// TODO:  ここにコントロール通知ハンドラ コードを追加してください。

	
}

/********************************************************************
機  能  名  称 : マクロ名検索エディットボックスの値が変更された
関    数    名 : OnEnChangeEditCommandPromptMacro
引          数 :
戻    り    値 :
機          能 :　マクロ名検索エディットボックスの値を含むマクロのみを表示
　　　　　　　　　(マクロ名検索チェックボックスONの間のみ)
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnEnChangeEditCommandPromptMacro()
{
	CString targetStr;//キーワード

	API.GetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_MACRO, &targetStr);

	ListBox_ResetContent(::GetDlgItem(m_hWnd, IDC_LIST_COMMAND_PROMPT_MACRO));//リストボックスクリア


	//マクロ一覧のうちキーワードと適合するもののみリストに入れる

		for (int i = 0; i < CALC_MACRO_CNT_MAX; i++)
		{
			MACRO_DEF_STRUCT mac;
			m_calclate.GetMacroDefinision(i, &mac);

			if (strlen(mac.name) > 0) {
				CString str = CString(mac.name);

				if (str.MakeUpper().Find(targetStr.MakeUpper()) >= 0) {//大文字・小文字不問
					m_list_macro.AddString(CString(mac.name));
				}
			}
		}



	API.SetCheck(m_hWnd, IDC_CHECK_COMMAND_PROMPT_FIND, true);
}


/********************************************************************
機  能  名  称 : マクロ名検索チェックボックスの状態変更
関    数    名 : OnEnChangeEditCommandPromptMacro
引          数 :
戻    り    値 :
機          能 :　マクロ名リストボックス内からキーワード検索
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnBnClickedCheckCommandPromptFind()
{
	if (API.GetCheck(m_hWnd, IDC_CHECK_COMMAND_PROMPT_FIND))
	{
		OnEnChangeEditCommandPromptMacro();
	}
	else
	{
		ListBox_ResetContent(::GetDlgItem(m_hWnd, IDC_LIST_COMMAND_PROMPT_MACRO));//リストボックスクリア

		//マクロ一覧を作り直す
		for (int i = 0; i<CALC_MACRO_CNT_MAX; i++)
		{
			MACRO_DEF_STRUCT mac;
			m_calclate.GetMacroDefinision(i, &mac);

			if (strlen(mac.name)>0) {
				m_list_macro.AddString(CString(mac.name));
			}
		}
	}
}


/********************************************************************
機  能  名  称 : Undo
関    数    名 : OnBnClickedButtonCommandPromptUndo
引          数 :
戻    り    値 :
機          能 :　
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnBnClickedButtonCommandPromptUndo()
{
	rich_edit_change_event_active_flg = false;

	if (undoID + 1 > 0 && undoID + 1 < UNDO_MAX)
	{
		if (pUndoTbl[undoID + 1])
		{
			m_rich_edit.SetWindowText(CString(pUndoTbl[undoID + 1]));
			color_command_lines(true);//文字色つけ
			change_break_points(pUndoTbl[undoID], pUndoTbl[undoID + 1]);//ブレークポイントの変更
			undoID++;
		}
	}


	rich_edit_change_event_active_flg = true;

	
}

/********************************************************************
機  能  名  称 : Redo
関    数    名 : OnBnClickedButtonCommandPromptRedo
引          数 :
戻    り    値 :
機          能 :　
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CCommandPromptDlg::OnBnClickedButtonCommandPromptRedo()
{
	rich_edit_change_event_active_flg = false;

	if (undoID - 1 >= 0 && undoID - 1 < UNDO_MAX)
	{
		if (pUndoTbl[undoID -1])
		{
			m_rich_edit.SetWindowText(CString(pUndoTbl[undoID- 1]));
			color_command_lines(true);//文字色つけ
			change_break_points(pUndoTbl[undoID], pUndoTbl[undoID-1]);//ブレークポイントの変更
			undoID--;
		}
	}


	rich_edit_change_event_active_flg = true;
}
