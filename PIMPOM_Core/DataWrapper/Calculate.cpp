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
//CCalculate
//演算式の解釈と実行
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
extern	CPimpomAPI	API;

// *************************************
//         関    数    参    照         
// *************************************

void CCalculate::debug_out(COMMAND	*command_dbg)
{
#ifdef _DEBUG
	char buffer[1024] = { 0 };

	if (command_dbg->type == COM_TYPE_NUMBER)
	{
		sprintf(buffer, "%lf ", command_dbg->number);
	}
	else if (command_dbg->type == COM_TYPE_OPERATOR)
	{
		char op[][5] = { "+", "-", "*", "/", "==", "<", ">", "<=", ">=", "!=", "=", "&&", "||", "+=", "-=", "*=", "/=" };
		sprintf(buffer, "%s ", op[command_dbg->ope]);
	}
	else if (command_dbg->type == COM_TYPE_PARENT)
	{
		char pa[] = { '(', ')' };
		sprintf(buffer, "%c ", pa[command_dbg->pare]);
	}
	else if (command_dbg->type == COM_TYPE_MACRO)
	{
		sprintf(buffer, "%s ", pMacroDefinision[command_dbg->mac].name);
	}
	else if (command_dbg->type == COM_TYPE_COMMA)
	{
		sprintf(buffer, ", ");
	}
	else if (command_dbg->type == COM_TYPE_COOR_X)
	{
		sprintf(buffer, "x ");
	}
	else if (command_dbg->type == COM_TYPE_COOR_Y)
	{
		sprintf(buffer, "y ");
	}
	else if (command_dbg->type == COM_TYPE_STRING)
	{
		sprintf(buffer, "str ");
	}
	else if (command_dbg->type == COM_TYPE_VARIABLE)
	{
		sprintf(buffer, "var ");
	}
	else
	{

	}

	OutputDebugString(buffer);
#endif
}

void CCalculate::debug_out_ln(COMMAND	*start)
{
#ifdef _DEBUG
	//デバッグ出力
	for (COMMAND *command_dbg = start->next; command_dbg != NULL; command_dbg = command_dbg->next)//コマンドリストを順次検索
	{
		debug_out(command_dbg);
	}
	OutputDebugString("\n");
#endif
}




CCalculate::CCalculate()
{
	//マクロ定義
	pMacroDefinision = new MACRO_DEF_STRUCT[CALC_MACRO_CNT_MAX];//メモリ確保
	memset( pMacroDefinision, 0, sizeof(MACRO_DEF_STRUCT)*CALC_MACRO_CNT_MAX );//メモリ初期化
	define_macros();//マクロ定義の実施


	//コマンド先頭ポインタマップ初期化
	for(int i=0 ; i<CALC_COMMAND_LINE_MAX ; i++){
		pCalcDataStructMap[i] = NULL;
	}

	//変数先頭ポインタマップ初期化
	for(int i=0 ; i<CALC_VARIABLE_BUFFER_LEN ; i++){
		pCalcVariableMap[i] = NULL;
	}

	//ファイル一覧作成のためのファインダ
	for (int i = 0; i < CALC_FILE_FINDER_MAX; i++){
		pFileFinder[i] = NULL;
	}
}


CCalculate::~CCalculate()
{
	ClearAll();

	delete[]	pMacroDefinision;

	for (int i = 0; i < CALC_FILE_FINDER_MAX; i++){
		if (pFileFinder[i]){
			pFileFinder[i]->Close();
			delete	pFileFinder[i];
		}
	}
}



/********************************************************************
機  能  名  称 : 初期化する
関    数    名 : init_calc_structure
引          数 :
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::init_calc_structure(int id,					//(in)行番号
							  CString *p_err_message	//(out)エラーメッセージ
							  )
{
	if(!pCalcDataStructMap[id])//データメモリがなければ新規確保
	{
		pCalcDataStructMap[id] = new CALC_DATA_STRUCT;
		if(pCalcDataStructMap[id]==NULL){
			p_err_message->Format("Error :lackness of memory (init_calc_structure)");
			return false;
		}
		
		memset(pCalcDataStructMap[id], 0, sizeof(CALC_DATA_STRUCT));//初期化
	}
	else 
	{	//データメモリがある場合

		//中間データのメモリを削除(計測結果とエラーメッセージは残す)

		if( pCalcDataStructMap[id]->pCommandList ){//コマンドリストクリア
			delete[]	pCalcDataStructMap[id]->pCommandList;
			pCalcDataStructMap[id]->pCommandList = NULL;
		}

		if( pCalcDataStructMap[id]->pStrignBuffer ){//文字列バッファクリア
			delete		pCalcDataStructMap[id]->pStrignBuffer ;
			pCalcDataStructMap[id]->pStrignBuffer = NULL;
		}

		memset(&pCalcDataStructMap[id]->ctrlData, 0, sizeof(CALC_CTRL_DATA));//コマンドライン制御コードクリア
	}

	return true;
}

/********************************************************************
機  能  名  称 : 
関    数    名 : deinit_calc_structure
引          数 :
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCalculate::deinit_calc_structure(int id)
{
	if(id<0 || id>=CALC_COMMAND_LINE_MAX){
		return;
	}

	if(pCalcDataStructMap[id])
	{
		if(pCalcDataStructMap[id]->pStrignBuffer)
		{
			delete	pCalcDataStructMap[id]->pStrignBuffer;
			pCalcDataStructMap[id]->pStrignBuffer = NULL;
		}

		if(pCalcDataStructMap[id]->pCommandList)
		{
			delete[]	pCalcDataStructMap[id]->pCommandList;
			pCalcDataStructMap[id]->pCommandList = NULL;
		}
	}

	delete	pCalcDataStructMap[id];
	pCalcDataStructMap[id] = NULL;
}

/********************************************************************
機  能  名  称 : 登録されているコマンドラインの長さを返す
関    数    名 : GetCommandLineLen
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CCalculate::GetCommandLineLen()
{
	for(int i=0 ; i<CALC_COMMAND_LINE_MAX ; i++)
	{
		if(pCalcDataStructMap[i]==NULL){
			return i;
		}
	}
	return CALC_COMMAND_LINE_MAX-1;
}

/********************************************************************
機  能  名  称 : コマンド文字列／文字列バッファ／コマンドリスト／計算結果をすべてクリア
関    数    名 : ClearAll
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCalculate::ClearAll()
{
	for(int i=0 ; i<CALC_COMMAND_LINE_MAX ; i++)
	{
		deinit_calc_structure(i);
	}

	clear_variable();
}


/********************************************************************
機  能  名  称 : 新規コマンドリストの挿入
関    数    名 : set_command_list
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CCalculate::set_command_list(int		id ,				//(in)
									CString	commandStr,				//(in)コマンド文字列
								   COMMAND	*pNewCommandCodeList,	//(in)挿入するコマンドリスト
								   int		ctrl_code				//(in)挿入するコマンドライン制御コード
								   )
{
	if(id<0 || id >= CALC_COMMAND_LINE_MAX)	return false;//idが不正

	if(pCalcDataStructMap[id]==NULL)	return false;//メモリ初期化ができてない


	//指定されたコマンドリストがあればメモリを一旦削除
	if(pCalcDataStructMap[id]->pCommandList){
		delete[]	pCalcDataStructMap[id]->pCommandList;
		pCalcDataStructMap[id]->pCommandList=NULL;
	}


	int len = get_command_listLen(pNewCommandCodeList);//新しいコマンドリストの長さが0でなければメモリ確保
	if(len>0){
		pCalcDataStructMap[id]->pCommandList = new COMMAND[len];
		if(pCalcDataStructMap[id]->pCommandList==NULL)	return false;
		
		copy_command_list(pCalcDataStructMap[id]->pCommandList, pNewCommandCodeList, len);//コマンドリストをコピー
	}

	strcpy( pCalcDataStructMap[id]->commandString, commandStr.GetBuffer());//コマンドライン文字列をコピー
	commandStr.ReleaseBuffer();

	pCalcDataStructMap[id]->ctrlData.controlCode = ctrl_code;//コマンドライン制御コード

	return true;
}

/********************************************************************
機  能  名  称 : コマンドリスト長さ取得
関    数    名 : get_command_listLen
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CCalculate::get_command_listLen(COMMAND *command)
{
	int cnt=0;
	COMMAND *com = command;

	while(com!=NULL){
		com = com->next;
		cnt++;
	}

	return cnt;
}

/********************************************************************
機  能  名  称 : コマンドリストの先頭ポインタ取得
関    数    名 : get_command_list
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
COMMAND *CCalculate::get_command_list(int id, CALC_CTRL_DATA* p_ctrl_data)
{
	if(id<0 || id >= CALC_COMMAND_LINE_MAX)	return NULL;//idが不正

	if(pCalcDataStructMap[id]==NULL)	return NULL;//メモリ初期化ができてない


	memcpy( p_ctrl_data,  &pCalcDataStructMap[id]->ctrlData, sizeof(CALC_CTRL_DATA));

	return pCalcDataStructMap[id]->pCommandList;
}

/********************************************************************
機  能  名  称 : コマンドリストをコピーする
関    数    名 : copy_command_list
引          数 : 
戻    り    値 : 正常に演算できた場合は true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CCalculate::copy_command_list(	COMMAND *dst,	//(out)コピー先コマンドリスト先頭ポインタ
										COMMAND *src,	//(in)コピー元コマンドリスト先頭ポインタ
										int		size	//(in)リスト長
									)
{
	memcpy(dst, src, sizeof(COMMAND)*size);
	for(int i=0 ; i<size ; i++){
		if(src[i].prev!=0){
			int offset_prev = src[i].prev - src;
			dst[i].prev = dst + offset_prev;
		}

		if(src[i].next!=0){
			int offset_next = src[i].next - src;
			dst[i].next = dst + offset_next;
		}
	}
}




/********************************************************************
機  能  名  称 : 文字列バッファへ文字列を新たに挿入する
関    数    名 : push_string_to_buffer
引          数 :
戻    り    値 : 成功した場合はバッファIDが帰る
				失敗した場合は、-1:ID番号不正  -2:ID番号で指定された行が未実行
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CCalculate::push_string_to_buffer(
								 int	 id,
								 CString str,				//(in)挿入する文字列
								 CString *p_err_message,	//(out)エラーメッセージ
								 bool	 isStatic			//(in)コマンドラインで指定される固定文字列かどうか
								 )
{
	if(id<0 || id>=CALC_COMMAND_LINE_MAX){
		return -1;//IDが不正
	}

	if(pCalcDataStructMap[id]==NULL){
		return -2;//メモリ初期化ができてない
	}

	if(str.GetLength() >= CALC_STRING_LEN_MAX )//挿入する文字列が長すぎる
	{
		p_err_message->Format("Error: command string is too long (max %d)", CALC_STRING_LEN_MAX-1);
		set_error_message(id, *p_err_message);
		return -1;
	}

	if(pCalcDataStructMap[id]->pStrignBuffer==NULL)//メモリがなければ新たにとりなおす
	{
		pCalcDataStructMap[id]->pStrignBuffer = new CALC_STRING_BUFFER_STRUCT;
		if(!pCalcDataStructMap[id]->pStrignBuffer )
		{
			p_err_message->Format("Error: lackness of mempry (push_string_to_buffer)");
			set_error_message(id, *p_err_message);
			return -1;
		}

		memset(pCalcDataStructMap[id]->pStrignBuffer, 0, sizeof(CALC_STRING_BUFFER_STRUCT));
	}

	if( pCalcDataStructMap[id]->pStrignBuffer->count + 1 >= CALC_STRING_BUFFER_LEN)//文字列バッファあふれ
	{
		p_err_message->Format("Error: too many string value(max %d)", CALC_STRING_BUFFER_LEN-1);
		set_error_message(id, *p_err_message);
		return -1;
	}


	int newcnt = pCalcDataStructMap[id]->pStrignBuffer->count;

	//文字列コピー
	strcpy( pCalcDataStructMap[id]->pStrignBuffer->strbuffer[newcnt], str.GetBuffer());
	str.ReleaseBuffer();

	pCalcDataStructMap[id]->pStrignBuffer->count++;//文字列バッファカウント進める

	if(isStatic){
		pCalcDataStructMap[id]->pStrignBuffer->staticCount++;
	}

	return newcnt;
}

/********************************************************************
機  能  名  称 : バッファから文字列取得
関    数    名 : get_string_from_buffer
引          数 : 
戻    り    値 : 文字列バッファの先頭アドレスが帰る
				　失敗するとNULL
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
char *CCalculate::get_string_from_buffer(int id, int str_id)
{
	if(id<0 || id>=CALC_COMMAND_LINE_MAX)	return NULL;//idが不正

	if(pCalcDataStructMap[id]==NULL)	return NULL;//メモリ初期化ができてない

	if(pCalcDataStructMap[id]->pStrignBuffer==NULL)	return NULL;//文字列バッファに何もない

	if(str_id<0 || str_id>=pCalcDataStructMap[id]->pStrignBuffer->count)	return NULL;//str_idが不正


	return pCalcDataStructMap[id]->pStrignBuffer->strbuffer[str_id];
}


/********************************************************************
機  能  名  称 : 文字列バッファから固定文字でないものを削除
関    数    名 : delete_non_static_string_buffer
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCalculate::delete_non_static_string_buffer(int id)
{
	if(id<0 || id>=CALC_COMMAND_LINE_MAX)	return ;//idが不正

	if(pCalcDataStructMap[id]==NULL)	return ;//メモリ初期化ができてない

	if(pCalcDataStructMap[id]->pStrignBuffer==NULL)	return ;//文字列バッファに何もない

	pCalcDataStructMap[id]->pStrignBuffer->count = pCalcDataStructMap[id]->pStrignBuffer->staticCount;
}


/********************************************************************
機  能  名  称 : 変数のメモリを確保する
関    数    名 : new_variable
引          数 :
戻    り    値 : 成功した場合は変数のIDが帰る。失敗したときは負数が帰る
機          能 : 変数のメモリを確保する
　　　　　　　　　初期値は数値0
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CCalculate::new_variable(
						CString var_name,			//(in)変数名
						CString *p_err_message		//(out)エラーメッセージ
						)
{
	//変数名が長すぎる
	if(var_name.GetLength() > CALC_VARIABLE_NAME_MAX){
		p_err_message->Format("Error: too long variable name(max %d)", CALC_VARIABLE_NAME_MAX-1);
		return -1;
	}

	//変数バッファがいっぱい
	if(pCalcVariableMap[CALC_VARIABLE_BUFFER_LEN-1] != NULL ){
		p_err_message->Format("Error: too mary varialbe definission (max %d)", CALC_VARIABLE_BUFFER_LEN);
		return -1;
	}

	//同じ名前の変数宣言
	int id=0;
	while( pCalcVariableMap[id]!=NULL){
		if( var_name == CString(pCalcVariableMap[id]->name) ){
			//p_err_message->Format("Error: varialbe " + var_name + " is already defined");
			return id;
		}
		id++;
	}

	//バッファに変数追加
	pCalcVariableMap[id] = new CALC_VARIABLE_STRUCT;
	if(pCalcVariableMap[id]==NULL)	return -1;

	memset(pCalcVariableMap[id], 0, sizeof(CALC_VARIABLE_STRUCT) );

	sprintf( pCalcVariableMap[id]->name, var_name.GetBuffer() );	var_name.ReleaseBuffer();

	return id;
	
}

/********************************************************************
機  能  名  称 : ID指定で変数を取得する
関    数    名 : GetVariable
引          数 : 
戻    り    値 : 成功したらtrue
				 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::GetVariable(int id,					//(in)変数ID
							  CALC_RESULT_STRUCT *pvar, //(out)変数の値
							  char varname[]			//(out)変数名
								)
{
	if(CALC_VARIABLE_BUFFER_LEN <= id || id<0){//idが不正
		return false;
	}

	if(pCalcVariableMap[id]==NULL){//idでしていされた変数がメモリ確保されていない
		return false;
	}

	memcpy(pvar , &pCalcVariableMap[id]->data, sizeof(CALC_RESULT_STRUCT));//データ取得
	strcpy(varname, pCalcVariableMap[id]->name);//変数名取得

	return true;
}

/********************************************************************
機  能  名  称 : ID指定で変数を設定する
関    数    名 : SetVariable
引          数 : 
戻    り    値 : 成功したらtrue
				 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::SetVariable(int id,					//(in)変数ID
							  CALC_RESULT_STRUCT *pvar //(in)変数の値
								)
{
	if(CALC_VARIABLE_BUFFER_LEN <= id || id<0){//idが不正
		return false;
	}

	if(pCalcVariableMap[id]==NULL){//idでしていされた変数がメモリ確保されていない
		return false;
	}

	memcpy( &pCalcVariableMap[id]->data, pvar, sizeof(CALC_RESULT_STRUCT));//データ設定
	return true;
}


/********************************************************************
機  能  名  称 : 変数宣言ををクリア
関    数    名 : clear_variable
引          数 : 
戻    り    値 : 
				 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCalculate::clear_variable()
{
	for(int i=0 ; i<CALC_VARIABLE_BUFFER_LEN ; i++)
	{
		delete	pCalcVariableMap[i];
		pCalcVariableMap[i] = NULL;
	}

}

/********************************************************************
機  能  名  称 : 計算結果取得
関    数    名 : GetResult
引          数 : 
戻    り    値 : 計測結果のタイプ　0:数値  7:文字列   -1:ID番号不正  -2:ID番号で指定された行が未実行
				 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CCalculate::GetResult(int		id,			//(in)
						  double	*number,	//(out)
						  char		str[],		//(out)
						  CString	*p_err_message,	//(out)
						  DWORD		*p_elapsed_time	//(out)
						)
{
	if(id<0 || id>=GetCommandLineLen())
	{
		p_err_message->Format("Error: Res() argument must be within 0-%d", GetCommandLineLen()-1);
		*number=0;
		str[0]=0;
		return -1;//idが不正
	}

	if(pCalcDataStructMap[id]==NULL)
	{
		p_err_message->Format("---");
		*number=0;
		str[0]=0;
		return -2;//メモリ初期化ができてない
	}


	if(pCalcDataStructMap[id]->result.type == COM_TYPE_NUMBER)
	{//数値
		*number = pCalcDataStructMap[id]->result.number;
		str[0]=0;
	}
	else if(pCalcDataStructMap[id]->result.type == COM_TYPE_STRING)
	{//文字列
		strcpy(str, pCalcDataStructMap[id]->result.str);
		*number=0;
	}


	*p_err_message  = CString(pCalcDataStructMap[id]->errorMessage);//エラーメッセージ出力

	if (p_elapsed_time){
		*p_elapsed_time = pCalcDataStructMap[id]->result.elapsedTime;//経過時間
	}
	

	return pCalcDataStructMap[id]->result.type;
}

/********************************************************************
機  能  名  称 : 
関    数    名 : set_result
引          数 : 
戻    り    値 : 失敗したらfalse
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CCalculate::set_result(int id, COMMAND *com, DWORD elapsed_time)
{

	if(com->type == COM_TYPE_NUMBER)
	{
		pCalcDataStructMap[id]->result.type = COM_TYPE_NUMBER;
		pCalcDataStructMap[id]->result.number = com->number;
	}
	else if(com->type == COM_TYPE_STRING)
	{
		pCalcDataStructMap[id]->result.type = COM_TYPE_STRING;
		strcpy( pCalcDataStructMap[id]->result.str, get_string_from_buffer(id, com->str_id) );
	}

	pCalcDataStructMap[id]->result.elapsedTime = elapsed_time;


	return true;
}




/********************************************************************
機  能  名  称 : 
関    数    名 : set_error_message
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CCalculate::set_error_message(int id, CString err )
{
	if(id<0 || id>=CALC_COMMAND_LINE_MAX)	return ;//idが不正

	if(pCalcDataStructMap[id]==NULL)	return ;//メモリ初期化ができてない

	if(err.GetLength()>=CALC_STRING_LEN_MAX)	return;//エラーメッセージが長すぎる

	strcpy(pCalcDataStructMap[id]->errorMessage, err.GetBuffer() );
	err.ReleaseBuffer();
}


/********************************************************************
機  能  名  称 : パラメタで関連づいた複数コマンド文字列を静的に展開して１つの文字列にまとめる
関    数    名 : ExtendSubCommands
引          数 : 
戻    り    値 : 正常に展開できた場合は true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::ExtendSubCommands(int		sub_com_num,		//(in)サブコマンド文字列数
								   CString	sub_com_str[],		//(in)サブコマンド文字列
								   CString	sub_com_id[],		//(in)サブコマンドを表す識別文字列
								   CString	*p_com_str,			//(in/out)メインコマンド文字列
								   CString	*p_err_message,		//(out)エラーメッセージ
								   bool		*sub_com_enable		//(in)サブコマンドの有効／無効（NULL可）
								   )
{
	p_err_message->Empty();

	//パラメタ文字列をすべて展開する
	for(int j=0 ; j<sub_com_num ; j++){
		bool sub_com_remain_flg;
		do{
			sub_com_remain_flg=false;
			for(int i=0 ; i<sub_com_num ; i++)
			{
				if( sub_com_str[j].Find(sub_com_id[i]) != -1){//パラメタ文字列の中でさらにパラメタ識別文字が見つかった
					sub_com_remain_flg=true;

					if(sub_com_enable != NULL){
						if(!sub_com_enable[i]){//無効なパラメタを使った場合
							p_err_message->Format("Error: パラメタ" + sub_com_id[i] + "は有効ではありません");
							return false;
						}
					}

					if(i==j){
						p_err_message->Format("Error: パラメタの自己参照があります");
						return false;
					}else {
						sub_com_str[j].Replace(sub_com_id[i], CString("(") + sub_com_str[i] +  CString(")"));
					}
				}
			}
		}while(sub_com_remain_flg);//パラメタ参照がネストしている場合は、展開を繰り返す
	}

	//コマンド文字列に反映
	for(int i=0 ; i<sub_com_num ; i++){
		p_com_str->Replace(sub_com_id[i], CString("(") + sub_com_str[i] +  CString(")"));
	}
	return true;
}




/********************************************************************
機  能  名  称 : すべてのコマンド文字列を翻訳して実行準備する
関    数    名 : RegisterCommandString
引          数 : 
戻    り    値 : 失敗した場合はtrueを返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::RegisterCommandString(	
										CString		com_strs[],		//(in)コマンド文字列
										int			com_str_cnt,	//(in)文字列数
										CString		*p_err_message	//(out)エラーメッセージ
										)
{
	for(int id=0 ; id<com_str_cnt ; id++)
	{
		int	com_num=0, ctrl_code=0; 
		COMMAND	CommandCode[CALC_COMMAND_LINE_MAX]={0};


		if(com_strs[id].GetLength() >= CALC_STRING_LEN_MAX){
			p_err_message->Format("Error: command line%d is too long (max=%d)", id, CALC_STRING_LEN_MAX-1);
			return false;//コマンド文字列が長すぎる
		}


		if(pCalcDataStructMap[id]){
			if(pCalcDataStructMap[id]->commandString == com_strs[id]){//過去に実行したコマンド文字列と変わっていない場合は、コマンドリストを再利用する
				continue;
			}
		}


		
		if( init_calc_structure(id, p_err_message ))//コマンド文字列をデータメモリに登録
		{
			//コマンド文字列⇒コマンドリストに翻訳
			com_num = command_string_to_command_list(id, com_strs[id], CommandCode, p_err_message, &ctrl_code);
			if(com_num>0)
			{//成功
				if(!set_command_list(id, com_strs[id], CommandCode, ctrl_code) )//コマンドリストをデータメモリに登録
				{
					set_error_message(id, *p_err_message);
					return false;
				}
			}
			else
			{//失敗
				set_error_message(id, *p_err_message);
				return false;
			}
		}
		else
		{
			set_error_message(id, *p_err_message);
			return false;
		}
	}

	

	//if-else-endifの設定
	if( !set_branch_property(com_str_cnt,p_err_message) )
	{
		return false;
	}

	return true;
}





/********************************************************************
機  能  名  称 : コマンド文字列を行単位で翻訳してコマンドリストに変える
関    数    名 : command_string_to_command_list
引          数 :
戻    り    値 : 正確にコマンドを読み取れたコマンド数		コマンドの読み取りに失敗した場合は負数
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CCalculate::command_string_to_command_list(
									    int			id,				//(in)行番号
										CString		str,			//(in)コマンド文字列
										COMMAND		commands[],		//(out)コマンドリスト
										CString		*p_err_message,	//(out)エラーメッセージ
										int			*p_ctrl_code	//(out)
										)
{
	int index_start=0;//コマンド文字探索開始位置
	int	com_cnt=0;//コマンドの数


	


	//この行が変数宣言であるかどうかしらべる
	int varDefCnt = FindVariableDefinission(str, p_err_message);
	if(  varDefCnt>0 ){
		index_start = str.GetLength();//変数宣言がある場合は、この行にはその他の記述は無効とする
	}else if(varDefCnt<0){
		set_error_message(id, *p_err_message);//誤った変数宣言があったら、エラー
		return -1;
	}


	//コマンド実行制御文を探す
	int incliment=0;
	if( FindCtrlCode(str, index_start, p_ctrl_code , &incliment) ){
		index_start += incliment;
	}
	


	//コマンドの最初に括弧をつける
	commands[ 0 ].type   = COM_TYPE_PARENT;
	commands[ 0 ].pare   = COM_PARENT_OPEN;
	commands[ 0 ].prev   = NULL;
	commands[ 0 ].next   = commands + 1;
	com_cnt = 1;



	for(int index=index_start ; index<str.GetLength() ; )//コマンド文字列をしらべる
	{
		int			temp_int;
		double		temp_double;
		CString		temp_str;
		int			indexIncliment=0;

			if(com_cnt >= CALC_COMMAND_LIST_MAX){//コマンド数が多すぎる
				p_err_message->Format("Error: line%d has too meny commands", id);
				set_error_message(id, *p_err_message);
				return -1;
			}


			if( IsStringValue( str , index , &temp_str, &indexIncliment) )//"で囲まれた固定文字列が見つかった
			{
				commands[ com_cnt ].type   = COM_TYPE_STRING;
				if( (temp_int = push_string_to_buffer(id, temp_str , p_err_message, true) ) < 0){

					return -1;
				}else{
					commands[ com_cnt ].str_id = temp_int;//検索された文字列をバッファに入れる
					commands[ com_cnt ].prev   = commands + com_cnt - 1;
					commands[ com_cnt ].next   = commands + com_cnt + 1;
					com_cnt++;
				}
			}
			else if( IsComment(str, index) )
			{
				//コメントが見つかったら、これ以上先は見ない
				break;
			}
			else if( IsNumberValue( str , index , &temp_double, &indexIncliment) )//数値が見つかった
			{
				commands[ com_cnt ].type   = COM_TYPE_NUMBER;
				commands[ com_cnt ].number = temp_double;
				commands[ com_cnt ].prev   = commands + com_cnt - 1;
				commands[ com_cnt ].next   = commands + com_cnt + 1;
				com_cnt++;

			}
			else if( IsOperator( str , index , &temp_int , &indexIncliment) )//演算子が見つかった
			{
				commands[ com_cnt ].type   = COM_TYPE_OPERATOR;
				commands[ com_cnt ].ope    = temp_int;
				commands[ com_cnt ].prev   = commands + com_cnt - 1;
				commands[ com_cnt ].next   = commands + com_cnt + 1;
				com_cnt++;

			}
			else if( IsParenthesis( str , index , &temp_int) )//括弧が見つかった
			{
				commands[ com_cnt ].type   = COM_TYPE_PARENT;
				commands[ com_cnt ].pare   = temp_int;
				commands[ com_cnt ].prev   = commands + com_cnt - 1;
				commands[ com_cnt ].next   = commands + com_cnt + 1;
				com_cnt++;

				indexIncliment=1;

			}
			else if( IsMacroName( str , index , &temp_int, &indexIncliment) )//マクロが見つかった
			{
				commands[ com_cnt ].type   = COM_TYPE_MACRO;
				commands[ com_cnt ].mac    = temp_int;
				commands[ com_cnt ].prev   = commands + com_cnt - 1;
				commands[ com_cnt ].next   = commands + com_cnt + 1;
				com_cnt++;
			}
			else if( IsDefinedVariable( str, index, &temp_int , &indexIncliment) )// 宣言済み変数が見つかった
			{
				commands[ com_cnt ].type   = COM_TYPE_VARIABLE;
				commands[ com_cnt ].var_id    = temp_int;
				commands[ com_cnt ].prev   = commands + com_cnt - 1;
				commands[ com_cnt ].next   = commands + com_cnt + 1;
				com_cnt++;
			}
			else if (str[index] == ',' || str[index] == ';')// カンマが見つかった
			{
				commands[ com_cnt ].type   = COM_TYPE_COMMA;
				commands[ com_cnt ].prev   = commands + com_cnt - 1;
				commands[ com_cnt ].next   = commands + com_cnt + 1;
				com_cnt++;

				indexIncliment=1;

			}
			else if( str[ index ] == 'i' ||  str[ index ] == 'j' || str[ index ] == 'x' || str[ index ] == 'y')// 座標を表す i, j,x,y が見つかった
			{
				if( str[ index ] == 'i' || str[ index ] == 'x')		commands[ com_cnt ].type   = COM_TYPE_COOR_X;
				else												commands[ com_cnt ].type   = COM_TYPE_COOR_Y;
					
				commands[ com_cnt ].prev   = commands + com_cnt - 1;
				commands[ com_cnt ].next   = commands + com_cnt + 1;
				com_cnt++;

				indexIncliment=1;
			}
			else if( str[ index ] != ' ' && str[index] != '\t')
			{
				//不正な文字が含まれている
				p_err_message->Format("Error: line%d has invarid character (%c)", id, str[ index ]);
				return  -1;
			}

			index += indexIncliment;
	}

	//コマンドの括弧を閉じる
	commands[ com_cnt ].type = COM_TYPE_PARENT;
	commands[ com_cnt ].pare = COM_PARENT_CLOSE;
	commands[ com_cnt ].prev = commands + com_cnt - 1 ;
	commands[ com_cnt ].next = NULL;
	com_cnt++;


	//（ と　）の数が同じかどうかチェック
	if(  !check_parenthesis(commands, p_err_message) )	return -1;


	return	com_cnt;
}


/********************************************************************
機  能  名  称 : 文字列がコメントかどうか
関    数    名 : IsComment
引          数 :
戻    り    値 : コメントならばtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::IsComment(
							 CString	comstr,		//(in)検索対象の文字列
							 int		index		//(in)comstr内の検索を開始する位置 
							 )
{
	if( index + 1 >= comstr.GetLength() )	return false;

	if(comstr[index]=='/' && comstr[index+1]=='/')	return true;

	return false;
}


/********************************************************************
機  能  名  称 : 文字列から文字列(""で囲まれた領域)を検索する
関    数    名 : IsStringValue
引          数 :
戻    り    値 : 文字列が見つかったらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::IsStringValue(
							 CString	comstr,				//(in)検索対象の文字列
							 int		index ,				//(in)comstr内の検索を開始する位置
							 CString	*p_str,				//(out)検索された文字列
							 int		*pindexIncliment	//(out)インデックスインクリメント量
							 )
{
	if(comstr[index]!='"')	return	false;//indexで示す文字が「"」ではない場合は何もしない

	CString buffer("");//チェックした文字をここにためていく
	int temp_index;//注目文字インデックス

	//文字列が""で囲まれているかどうか順次チェック
	for(temp_index = index + 1 ; temp_index < comstr.GetLength() ; temp_index++)
	{
		if(comstr[temp_index]=='"')//文字列終わりの「"」が見つかった
		{
			*p_str = buffer;
			*pindexIncliment = temp_index  - index + 1;
			return true;
		}
		else
		{
			buffer += CString(comstr[temp_index]);
		}
	}

	return false;//文字列終わりの「"」が見つからなかった
}






/********************************************************************
機  能  名  称 :文字列が数値データかどうか判断
関    数    名 : IsNumberValue
引          数 :
戻    り    値 : 数値が見つかったらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/

bool CCalculate::IsNumberValue(
							 CString	comstr,				//(in)文字列
							 int		index,				//(in)comstr内の検索を開始する位置 
							 double		*p_number,			//(out)数値
							 int		*pIndexIncliment	//(out)インデックスインクリメント量
							 )
{
	if( comstr.GetLength()<=index )	return false;
	if( !isdigit( comstr[index] ) )	return	false;//indexで示す文字が数値ではない場合は何もしない
	

	CString		num_str("");//数値としてチェックした文字をここに入れていく
	BOOL		under_1_flg = false;//小数点があったかどうかを示すフラグ
	int			temp_index = index;//注目文字インデックス


	//文字列が数値かどうか順次チェック
	while( temp_index < comstr.GetLength() ){

			if( isdigit( comstr[temp_index] ) )//注目文字が数値の場合
			{
					num_str += (CString)comstr[ temp_index++ ];
			}
			else if( comstr[temp_index]=='.' )//注目文字が「.」 の場合
			{
					if( under_1_flg == false   &&   !num_str.IsEmpty() )//「.」は小数点である
					{
							num_str += (CString)comstr[ temp_index++ ];
							under_1_flg = true;
					}
					else//不正な「.」(小数点が２つある)
					{
						return false;
					}

			}else{//注目文字が数値でない場合
				break;
			}
	}

	if( !num_str.IsEmpty() ){//数値を表す文字列が見つかった
		*p_number = atof( num_str );

		*pIndexIncliment = temp_index - index;
		
		return	true;

	}else{//数値を表す文字列が見つからなかった
		return false;
	}
}


/********************************************************************
機  能  名  称 : 文字列が演算子かどうか判断
関    数    名 : IsOperator
引          数 :
戻    り    値 : 演算子が見つかったらtrue
機          能 : 四則演算
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::IsOperator(	CString	comstr,				//(in)文字列
							   int		index,				//(in)comstr内の検索を開始する位置
							   int		*p_ope,				//(out)0:+     1:-      2:*      3:/   
							   int		*pIndexIncliment	//(out)インデックスインクリメント量
							   )
{
	if(index + 1 < comstr.GetLength() )//2文字演算子
	{
		*pIndexIncliment=2;

		if(comstr[index]=='=' && comstr[index+1]=='='){
			*p_ope = 4;
			return true;
		}else if(comstr[index]=='<' && comstr[index+1]=='='){
			*p_ope = 7;
			return true;
		}else if(comstr[index]=='>' && comstr[index+1]=='='){
			*p_ope = 8;
			return true;
		}else if(comstr[index]=='!' && comstr[index+1]=='='){
			*p_ope = 9;
			return true;
		}
		else if (comstr[index] == '&' && comstr[index + 1] == '&') {
			*p_ope = 11;
			return true;
		}
		else if (comstr[index] == '|' && comstr[index + 1] == '|') {
			*p_ope = 12;
			return true;
		}
		else if (comstr[index] == '+' && comstr[index + 1] == '=') {
			*p_ope = 13;
			return true;
		}
		else if (comstr[index] == '-' && comstr[index + 1] == '=') {
			*p_ope = 14;
			return true;
		}
		else if (comstr[index] == '*' && comstr[index + 1] == '=') {
			*p_ope = 15;
			return true;
		}
		else if (comstr[index] == '/' && comstr[index + 1] == '=') {
			*p_ope = 16;
			return true;
		}
	}

	//1文字演算子
	*pIndexIncliment=1;

	switch( comstr[index] ){
		case '+':
			*p_ope = 0;
			return true;
		case '-':
			*p_ope = 1;
			return true;
		case '*':
			*p_ope = 2;
			return true;
		case '/':
			*p_ope = 3;
			return true;
		case '<':
			*p_ope = 5;
			return true;
		case '>':
			*p_ope = 6;
			return true;
		case '=':
			*p_ope = 10;
			return true;
	}

	return false;
}


/********************************************************************
機  能  名  称 : 文字列が括弧かどうか判断
関    数    名 : IsParenthesis
引          数 :
戻    り    値 : 見つかったらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::IsParenthesis(
								   CString	comstr,		//(in)文字列
								   int		index,	//(in/out)comstr内の検索を開始する位置 　括弧が見つかったらその分インデックスを進める
								   int		*p_pare		//(out)0:(     1:) 
								   )
{
	if( comstr[ index ] == '(' ){
		*p_pare = 0;
		return true;
	}else if( comstr[ index ] == ')' ){
		*p_pare = 1;
		return true;
	}

	return false;
}


/********************************************************************
機  能  名  称 : 文字列がマクロ名かどうか判断
関    数    名 : IsMacroName
引          数 :
戻    り    値 : 見つかったらtrue
機          能 : マクロ名および直後の「(」を探す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::IsMacroName(
							 CString	comstr,				//(in)文字列
							int			index,				//(in)comstr内の検索を開始する位置 
							int			*p_macro_id,		//(out)マクロ番号
							int			*pIndexIncliment	//(out)インデックスインクリメント量
							)
{

	for(int id=0 ; id<CALC_MACRO_CNT_MAX ; id++)//すべてのマクロ名を検索
	{
		if( pMacroDefinision[id].name[0]==0)	continue;//未定義

		int		temp_index = index;//注目文字インデックス

		int mac_nam_len = (int)strlen( pMacroDefinision[id].name );//マクロ名長

		//マクロ名とcomstrを1文字ずつ比較
		bool isMcrName=true;
		for(int i=0 ; i<mac_nam_len; i++)
		{
			if( comstr[i+temp_index] != pMacroDefinision[id].name[i])
			{
				isMcrName=false;//1文字でも異なればcomstrはマクロ名ではない
				break;
			}
		}
		
		if(isMcrName)//注目文字がマクロ名と同じであるため、その直後に(があれば、マクロである
		{
			if( comstr[temp_index + mac_nam_len] == '(' )//マクロ名の直後の「 ( 」が見つかった＝マクロである
			{
				temp_index += mac_nam_len -1;

				*p_macro_id = id;

				*pIndexIncliment = temp_index - index + 1;

				return	true;
			}
		}
	}

	return false;//マクロが見つからなかった
}


/********************************************************************
機  能  名  称 : 文字列が宣言済み変数名かどうか
関    数    名 : IsDefinedVariable
引          数 :
戻    り    値 : 文字列が見つかったらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::IsDefinedVariable(
							 CString	comstr,		//(in)検索対象の文字列
							 int		index ,		//(in)comstr内の検索を開始する位置
							 int		*p_var_id,	//(out)変数名
							 int		*pIndexIncliment	//(out)インデックスインクリメント量
							 )
{
	//登録されているすべての変数名と比較し、同じ物を探す
	int varID=0;
	int varNameLenMax=0;
	while( pCalcVariableMap[varID]!=NULL )
	{
		CALC_RESULT_STRUCT varres={0};
		char	varname[CALC_VARIABLE_NAME_MAX]={0};

		GetVariable(varID, &varres, varname);//変数名を取得

		bool is_var_name=true;

		for(int  idx=0; idx < strlen(varname) ; idx++)//１文字ずつ比較
		{
			if(varname[idx] != comstr[idx + index] ){//異なる文字があった
				is_var_name=false;
				break;
			}
		}

		if(is_var_name)
		{//すべての文字が一致した
			if(varNameLenMax < strlen(varname))//最も長い変数名を探す（部分的一致を避けるため）
			{
				varNameLenMax = (int)strlen(varname);
				*p_var_id = varID;
				*pIndexIncliment = (int)strlen(varname);
			}
			//return true;
		}

		varID++;
	}

	if(varNameLenMax>0)
		return true;
	else
		return false;
}



/********************************************************************
機  能  名  称 : 変数宣言を探す
関    数    名 : FindVariableDefinission
引          数 :
戻    り    値 : 変数宣言が見つかったら変数の数を返す　エラーのばあいは-1を返す
機          能 : 変数宣言を探して、変数バッファを確保する
				変数宣言：var [変数名1], [変数名2], ...
				変数名には記号（+-*=/|&'^~:.@#$!?()[]<>{}"\)およびスペースは入れてはいけない


日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CCalculate::FindVariableDefinission(
							 CString	comstr,		//(in)検索対象の文字列
							 CString	*p_err_message	//(out)エラーメッセージ
							 )
{
	if( comstr.GetLength()<5 ){
		return 0;
	}

	//最初が「var 」からはじまる
	int comstr_def_begin = comstr.Find("var ");
	if(comstr_def_begin<0){
		return 0;
	}

	//変数名1, 変数名2, 変数名3,,, とカンマで区切られた文字列を切り分ける
	CString substr[CALC_VARIABLE_BUFFER_LEN];

	int var_cand_cnt=0;
	substr[var_cand_cnt].Empty();
	for(int i=comstr_def_begin+4; i<comstr.GetLength() ; i++)
	{
		if(IsComment(comstr,i)){
			break;//コメント
		}

		if(comstr[i]==',')
		{
			if(var_cand_cnt+1 >=  CALC_VARIABLE_BUFFER_LEN){
				p_err_message->Format("Error:variable can be defined less than %d", CALC_VARIABLE_BUFFER_LEN);
				return -1;
			}

			var_cand_cnt++;
			substr[var_cand_cnt].Empty();
		}
		else
		{
			substr[var_cand_cnt] += CString(comstr[i]);
		}
	}

	//変数名の前後のスペースを切り詰める
	int var_cnt=0;
	for(int i=0 ; i<=var_cand_cnt ; i++)
	{
		if(substr[i].GetLength()==0)	continue;

		int startPos=0;
		for(int j=0 ; j<substr[i].GetLength()  ; j++){
			if( substr[i][j]==' ' || substr[i][j] == '\t'){
				startPos++;
			}else{
				break;
			}
		}

		int endPos = substr[i].GetLength()-1;
		for(int j=substr[i].GetLength()-1 ; j>=0 ; j++){
			if( substr[i][j]==' ' || substr[i][j] == '\t'){
				endPos--;
			}else{
				break;
			}
		}

		if(startPos<=endPos)
		{
			CString var_str = substr[i].Mid(startPos, endPos-startPos+1);

			if(var_str.FindOneOf("+-*/%=|&'^~:.@#$!?()[]<>{}\"\\ ")<0 && //禁止文字が含まれていないか
				isalpha(var_str[0])  ){//最初の文字はアルファベットであるか

				if( new_variable(var_str, p_err_message) < 0 ){
					return -1;
				}

				var_cnt++;
			}else{
				p_err_message->Format("Error:invarild variable difinission");
				return -1;

			}
		}
	}

	return var_cnt;
}


/********************************************************************
機  能  名  称 : 文字列からコマンド実行制御構文を検索する
関    数    名 : FindCtrlCode
引          数 :
戻    り    値 : 見つかったらtrue
機          能 : 
				break/goto()/if()/else/endif
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::FindCtrlCode(
							 CString	comstr,				//(in)検索対象の文字列
							 int		start_index,		//(in)comstr内の検索を開始する位置
							 int		*p_ctrl_id,			//(out)制御構文番号
							 int		*pIndexIncliment	//(out)インデックスインクリメント量
							)
{

	*p_ctrl_id = 0;

	comstr.MakeUpper();

	for(int index=start_index ; index<comstr.GetLength() ; index++)
	{
		if(comstr[index]==' ' || comstr[index] == '\t'){
			continue;//制御構文の前のスペースは無視する
		}
		else
		{
			if( comstr.Find("BREAK",index) == index )
			{	
				int next_index = index + CString("BREAK").GetLength();
				if( comstr[next_index] != ' '  && comstr[next_index] != '\t'  && comstr[next_index] != '\r' && comstr[next_index] != '\0'){//breakの直後はスペースかか終端改行のみ
					return false;
				}

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_BREAK;
				return true;
			}
			else if( comstr.Find("CLEAR",index) == index  )
			{
				int next_index = index + CString("CLEAR").GetLength();
				if( comstr[next_index] != ' '  && comstr[next_index] != '\t'  && comstr[next_index] != '\r' && comstr[next_index] != '\0'){//clearの直後はスペースか改行か終端のみ
					return false;
				}

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_CLEAR;
				return true;
			}
			else if( comstr.Find("GOTO(",index) == index  )
			{
				int next_index = index + CString("GOTO").GetLength();

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_GOTO;
				return true;
			}
			else if( comstr.Find("IF(",index) == index  )
			{
				int next_index = index + CString("IF").GetLength();

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_IF;
				return true;
			}
			else if (comstr.Find("ELSEIF", index) == index)
			{
				int next_index = index + CString("ELSEIF").GetLength();

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_ELSEIF;
				return true;
			}
			else if( comstr.Find("ELSE",index) == index  )
			{
				int next_index = index + CString("ELSE").GetLength();
				if( comstr[next_index] != ' '  && comstr[next_index] != '\t'  && comstr[next_index] != '\r' && comstr[next_index] != '\0'){//elseの直後はスペースか改行か終端のみ
					return false;
				}

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_ELSE;
				return true;
			}
			else if( comstr.Find("ENDIF",index) == index  )
			{
				int next_index = index + CString("ENDIF").GetLength();
				if( comstr[next_index] != ' '  && comstr[next_index] != '\t'  &&comstr[next_index] != '\r' && comstr[next_index] != '\0'){//endifの直後はスペースか改行か終端のみ
					return false;
				}

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_ENDIF;
				return true;
			}
			else if( comstr.Find("WHILE(",index) == index  )
			{
				int next_index = index + CString("WHILE").GetLength();

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_WHILE;
				return true;
			}
			else if (comstr.Find("ENDWHILE", index) == index)
			{
				int next_index = index + CString("WNDWHILE").GetLength();
				if (comstr[next_index] != ' '  && comstr[next_index] != '\t'  && comstr[next_index] != '\r' && comstr[next_index] != '\0'){//endwhileの直後はスペースか改行か終端のみ
					return false;
				}

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_ENDWHILE;
				return true;
			}
			else if (comstr.Find("FOR(", index) == index)
			{
					int next_index = index + CString("FOR").GetLength();

					*pIndexIncliment = next_index - start_index;
					*p_ctrl_id = CALC_CTRL_FOR;
					return true;
			}
			else if (comstr.Find("ENDFOR", index) == index)
			{
				int next_index = index + CString("ENDFOR").GetLength();
				if (comstr[next_index] != ' '  && comstr[next_index] != '\t'  && comstr[next_index] != '\r' && comstr[next_index] != '\0'){//endforの直後はスペースか改行か終端のみ
					return false;
				}

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_ENDFOR;
				return true;
			}
			else{
				return false;
			}
		}
	}


	return false;
}



/********************************************************************
機  能  名  称 : //コマンドライン制御コードと条件分岐のジャンプ先を調べる
関    数    名 : set_branch_property
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::set_branch_property(int com_str_cnt, CString *p_err_message)	
{
	for(int id=0 ; id<com_str_cnt ; id++){
		pCalcDataStructMap[id]->ctrlData.branchNestCount=0;//ネストの深さ初期化
		pCalcDataStructMap[id]->ctrlData.branchToLine[0]=0;//ジャンプ先初期化
		pCalcDataStructMap[id]->ctrlData.branchToLine[1]=0;
		pCalcDataStructMap[id]->ctrlData.branchFrmLink = -1;//else/endif⇒if endwhile⇒while逆リンク
	}


	//if～endifのネストの状態調査

	int nestCnt=0;
	for(int id=0 ; id<com_str_cnt ; id++)
	{
		if(pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_IF){//ifならばここからネストが一つ深くなる
			nestCnt++;
		}

		if(pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_WHILE){//whileならばここからネストが一つ深くなる
			nestCnt++;
		}

		if (pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_FOR){//forならばここからネストが一つ深くなる
			nestCnt++;
		}

		pCalcDataStructMap[id]->ctrlData.branchNestCount = nestCnt;

		if(pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_ENDIF){//endifならば次からネストが一つ浅くなる
			nestCnt--;
		}

		if(pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_ENDWHILE){//endwhileならば次からネストが一つ浅くなる
			nestCnt--;
		}

		if (pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_ENDFOR){//endforならば次からネストが一つ浅くなる
			nestCnt--;
		}
	}



	//if,elseif,else,while,forのジャンプ先調査
	for(int id=0 ; id<com_str_cnt ; id++)
	{
		if( pCalcDataStructMap[id]->ctrlData.controlCode  == CALC_CTRL_IF)//注目するif
		{
			pCalcDataStructMap[id]->ctrlData.branchToLine[0] = id + 1;//ここ（elseif(true)のとき）からのジャンプ先->この直後の行へ


			//注目するifと対応するelse/elseif/endifを探して、ジャンプ元・ジャンプ先を設定する

			bool endifFoundFlg = false;//endifが見つかったことを表すフラグ
			int  previfid = id;//ひとつ前のifまたはelseifのID
			int  elseID = -1;//ひとつ前のelseのID
			for(int j=id+1 ; j<com_str_cnt ; j++)
			{	
				if (pCalcDataStructMap[previfid]->ctrlData.branchNestCount == pCalcDataStructMap[j]->ctrlData.branchNestCount)
				{
					 if (pCalcDataStructMap[j]->ctrlData.controlCode == CALC_CTRL_ELSEIF)//elseifが見つかった
					{
						if (elseID >= 0){
							//elseifよりelseが先に見つかってしまっていたらエラー
							p_err_message->Format("invarid IF/ENDIF block");
							return false;
						}

						//対応するif
						pCalcDataStructMap[j]->ctrlData.branchFrmLink = id;

						//ジャンプ元( if(false) / elseif(false) )からのジャンプ->この行へ
						pCalcDataStructMap[previfid]->ctrlData.branchToLine[1] = j;

						//ここ（elseif(true)のとき）からのジャンプ先->この直後の行へ
						pCalcDataStructMap[j]->ctrlData.branchToLine[0] = j + 1;


						previfid = j;
					}
					else if( pCalcDataStructMap[j]->ctrlData.controlCode == CALC_CTRL_ELSE )//elseが見つかった
					{
						//対応するif
						pCalcDataStructMap[j]->ctrlData.branchFrmLink = id;

						//ジャンプ元( if(false) / elseif(false) )からのジャンプ->この行へ
						pCalcDataStructMap[previfid]->ctrlData.branchToLine[1] = j;

						//ここからのジャンプ先(if～endif内にまだTrueの条件がない場合)->この直後の行へ
						pCalcDataStructMap[j]->ctrlData.branchToLine[0] = j + 1;


						elseID = j;
					}
					else if(pCalcDataStructMap[j]->ctrlData.controlCode == CALC_CTRL_ENDIF)//endifが見つかった
					{
						//対応するif
						pCalcDataStructMap[j]->ctrlData.branchFrmLink = id;

						if(elseID>=0)//ジャンプ元がelse
						{
							//ジャンプ元( else )からのジャンプ->この行へ
							pCalcDataStructMap[elseID]->ctrlData.branchToLine[1] = j;
						}
						else//ジャンプ元がif・elseif
						{
							//ジャンプ元( if(false) / elseif(false) )からのジャンプ先->この行へ
							pCalcDataStructMap[previfid]->ctrlData.branchToLine[1] = j;
						}

						endifFoundFlg = true;
						break;
					}

				}
				else if(pCalcDataStructMap[id]->ctrlData.branchNestCount > pCalcDataStructMap[j]->ctrlData.branchNestCount)
				{
					//注目するifよりネストが浅くなったら探索終了
					break;
				}
			}

			if(!endifFoundFlg){
				//対応するendifが見つからなかったらエラー
				p_err_message->Format("invarid IF/ENDIF block");
				return false;
			}
		}
		else
		if( pCalcDataStructMap[id]->ctrlData.controlCode  == CALC_CTRL_WHILE)//注目するwhile
		{
			pCalcDataStructMap[id]->ctrlData.branchToLine[0] = id + 1;//while(true)のジャンプ先：whileの直後の行

			//注目するwhileと対応するendwhileを探す
			bool endwhileFoundFlg=false;
			for(int j=id+1 ; j<com_str_cnt ; j++)
			{	
				if(pCalcDataStructMap[id]->ctrlData.branchNestCount== pCalcDataStructMap[j]->ctrlData.branchNestCount)
				{
					if(pCalcDataStructMap[j]->ctrlData.controlCode == CALC_CTRL_ENDWHILE)
					{
						//対応するendwhileが見つかった
						endwhileFoundFlg = true;
						pCalcDataStructMap[id]->ctrlData.branchToLine[1] = j+1;//while(false)のジャンプ先：同じネストのendwhileの直後の行
						pCalcDataStructMap[j]->ctrlData.branchToLine[0] = id;//endwhileのジャンプ先： 注目するwhile()の行
						pCalcDataStructMap[j]->ctrlData.branchFrmLink = id;
						break;
					}
				}
				else if(pCalcDataStructMap[id]->ctrlData.branchNestCount > pCalcDataStructMap[j]->ctrlData.branchNestCount)
				{
					//注目するwhileよりネストが浅くなったら探索終了
					break;
				}
			}

			if(!endwhileFoundFlg){
				//対応するendifが見つからなかったらエラー
				p_err_message->Format("invarid WHILE/ENDWHILE block");
				return false;
			}
		}
		else
			if (pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_FOR)//注目するfor
			{
				pCalcDataStructMap[id]->ctrlData.branchToLine[0] = id + 1;//for(,true,)のジャンプ先：whileの直後の行

				//注目するforと対応するendforを探す
				bool endforFoundFlg = false;
				for (int j = id + 1; j<com_str_cnt; j++)
				{
					if (pCalcDataStructMap[id]->ctrlData.branchNestCount == pCalcDataStructMap[j]->ctrlData.branchNestCount)
					{
						if (pCalcDataStructMap[j]->ctrlData.controlCode == CALC_CTRL_ENDFOR)
						{
							//対応するendforが見つかった
							endforFoundFlg = true;
							pCalcDataStructMap[id]->ctrlData.branchToLine[1] = j + 1;//for(false)のジャンプ先：同じネストのendforの直後の行
							pCalcDataStructMap[j]->ctrlData.branchToLine[0] = id;//endforのジャンプ先： 注目するfor()の行
							pCalcDataStructMap[j]->ctrlData.branchFrmLink = id;
							break;
						}
					}
					else if (pCalcDataStructMap[id]->ctrlData.branchNestCount > pCalcDataStructMap[j]->ctrlData.branchNestCount)
					{
						//注目するforよりネストが浅くなったら探索終了
						break;
					}
				}

				if (!endforFoundFlg){
					//対応するendifが見つからなかったらエラー
					p_err_message->Format("invarid FOR/ENDFOR block");
					return false;
				}
			}
	}



	//どことも対応しないelseif,else,endifおよびendwhileおよびforがないかチェック

	for(int id=0 ; id<com_str_cnt ; id++)
	{
		if (pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_ELSEIF){
			if (pCalcDataStructMap[id]->ctrlData.branchFrmLink<0){
				p_err_message->Format("invarid if/elseif/else/endif block");
				return false;
			}
		}

		if(pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_ELSE){
			if(pCalcDataStructMap[id]->ctrlData.branchFrmLink<0){
				p_err_message->Format("invarid if/elseif/else/endif block");
				return false;
			}
		}

		if(pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_ENDIF){
			if(pCalcDataStructMap[id]->ctrlData.branchFrmLink<0){
				p_err_message->Format("invarid if/endif block");
				return false;
			}
		}

		if(pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_ENDWHILE){
			if(pCalcDataStructMap[id]->ctrlData.branchFrmLink<0){
				p_err_message->Format("invarid while/endwhile block");
				return false;
			}
		}

		if (pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_ENDFOR){
			if (pCalcDataStructMap[id]->ctrlData.branchFrmLink<0){
				p_err_message->Format("invarid for/endfor block");
				return false;
			}
		}
	}




	return true;
}


/********************************************************************
機  能  名  称 : コマンドを実行する
関    数    名 : ExecuteAPI
引          数 :
	pBlockMap  通過中の処理ブロック（if～endif, for～endfor）の状態を表すマップ。　行番号でアクセスするbool型配列の先頭ポインタ
	　　　　　当該ブロックの中を実行する場合はtrue。スキップする場合はfalse
戻    り    値 : エラーまたは、次に進めない条件ならばfalse
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CCalculate::ExecuteAPI(
						    int			id,					//(in)実行する行番号
							CString		*p_err_message,		//(out)エラーメッセージ
							int			*pNextID,			//(out)次に実行する行番号(NULLを設定することも可)
							BATCH_INPUT *p_batch_in,		//(in)
							BATCH_OUTPUT *p_batch_out,		//(out)
							bool		autoDrawImage,		//(in)処理後に画像描画を実行する
							bool		*pBlockMap		//(in/out)
							)
{
	return Execute(id, 1, 1, p_err_message, -1, true, pNextID, p_batch_in, p_batch_out, autoDrawImage, pBlockMap);
}


/********************************************************************
機  能  名  称 : 画素単位でコマンドを実行する
関    数    名 : Execute
引          数 :
	pBlockMap  通過中の処理ブロック（if～endif, for～endfor）の状態を表すマップ。　行番号でアクセスするbool型配列の先頭ポインタ
　　　　	　当該ブロックの中を実行する場合はtrue。スキップする場合はfalse
戻    り    値 : エラーまたは、次に進めない条件ならばfalse
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CCalculate::Execute(
	int			id,					//(in)実行する行番号
	int			x,					//(in)画像上の座標x
	int			y,					//(in)画像上の座標y
	CString		*p_err_message,		//(out)エラーメッセージ
	int			rgb,				//(in)
	bool		ApiMode,			//(in)PIMPOM APIのコマンドを実行するモード
	int			*pNextID,			//(out)次に実行する行番号(NULLを設定することも可)
	BATCH_INPUT *p_batch_in,		//(in)
	BATCH_OUTPUT *p_batch_out,		//(out)
	bool		autoDrawImage,		//(in)処理後に画像描画を実行する
	bool		*pBlockMap			//(in/out)
	)
{
	//引数チェック
	if (id < 0 || id >= GetCommandLineLen()){
		//p_err_message->Format("Error: line number must be within 0-%d", GetCommandLineLen()-1);
		//set_error_message(id, *p_err_message);
		return false;
	}



	COMMAND		CommandMemoryTmp[CALC_COMMAND_LIST_MAX] = { 0 };//作業用
	COMMAND		AdditionalCommandMemory[CALC_COMMAND_LIST_MAX] = { 0 };//作業用
	int			AdditionalCommandMemoryIndex = 0;//作業用


	CALC_CTRL_DATA	ctrlData = { 0 };
	COMMAND		*pCommandList = get_command_list(id, &ctrlData);//コマンドリスト取得
	int			commandListLen = get_command_listLen(pCommandList);//コマンドリストの長さ

	if (commandListLen <= 2 && ctrlData.controlCode == 0)//コマンド文字なし、 コマンドライン制御コードもなし
	{
		//この行は無視して行番号を進める
		p_err_message->Format("---");
		set_error_message(id, *p_err_message);

		if (pNextID){
			*pNextID = id + 1;
		}
	}
	else
	{
		if(ctrlData.controlCode==CALC_CTRL_CLEAR){//コマンドライン制御コードがクリア命令
			ClearAll();
			*pNextID = 0;
			return false;
		}

		copy_command_list(CommandMemoryTmp, pCommandList, commandListLen);//コマンドリストのコピーをとる（実行すると内容が変わるため）


		//入力パラメタ設定
		CALC_PARAM_STRUCT param={0};
		param.x = x;
		param.y = y;
		param.rgbType = rgb;
		param.apiMode = ApiMode;
		param.pbatchIn = p_batch_in;
		param.pbatchOut = p_batch_out;

		DWORD start_tick = API.Tick(); //処理開始時刻

		bool ret;
		if (ctrlData.controlCode == CALC_CTRL_FOR)//for文
		{
			bool mode;
			if (pBlockMap[id] == false){//for文の中に初めてはいる
				mode = 0;//初期化->ループ条件計算
			}
			else{//for文のループ中である
				mode = 1;//カウンタ更新->ループ条件計算
			}
			ret = calculate_for_command_list(id, mode, &param, CommandMemoryTmp, AdditionalCommandMemory, &AdditionalCommandMemoryIndex, p_err_message, autoDrawImage);//計算実行
		}
		else//for文以外
		{
			ret = calculate_command_list(id, &param, CommandMemoryTmp, AdditionalCommandMemory, &AdditionalCommandMemoryIndex, p_err_message, autoDrawImage);//計算実行
		}

		if( ret )//計算実行
		{
			set_result(id, CommandMemoryTmp, API.Tick() - start_tick );//計算結果格納


			if(pNextID){
				//コマンドライン制御コードごとに、次に実行する行番号を決める
				if (!set_next_id(id, ctrlData, CommandMemoryTmp, p_err_message, pNextID, pBlockMap))
				{
					set_error_message(id, *p_err_message);
					delete_non_static_string_buffer(id);//コマンド実行で使った文字列バッファをクリア
					return false;
				}
			}
		}
		else
		{
			set_error_message(id, *p_err_message);
			delete_non_static_string_buffer(id);//コマンド実行で使った文字列バッファをクリア
			return false;
		}

		p_err_message->Format("OK");
		set_error_message(id, *p_err_message);
		delete_non_static_string_buffer(id);//コマンド実行で使った文字列バッファをクリア
	
	}

	return true;
}


/********************************************************************
機  能  名  称 : コマンドライン制御コードと計算結果の整合性のチェック
関    数    名 : CheckCtrlCode
引          数 : 
		pBlockMap  通過中の処理ブロック（if～endif, for～endfor）の状態を表すマップ。　コマンドと同じ並びのbool型配列の先頭ポインタ
　　　　　当該ブロックの中を実行する場合はtrue。スキップする場合はfalse
戻    り    値 : エラーまたは、次に進めない条件ならばfalse
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/

bool	CCalculate::set_next_id(int id, CALC_CTRL_DATA ctrlData, COMMAND *com, CString *p_err_message, int *pNextID, bool *pBlockMap)
{
	if(ctrlData.controlCode==CALC_CTRL_BREAK)//break
	{
		//先にすすまない
		*pNextID = id;
		return false;
	}

	if(ctrlData.controlCode == CALC_CTRL_GOTO)//goto
	{
		//実行行番号を所望の位置に移動させる
		if(com->type==COM_TYPE_NUMBER){
			if(com->number>=0 && com->number<GetCommandLineLen())
			{	
				*pNextID = (int)com->number;

			}else{
				//移動できない場所を指している
				p_err_message->Format("Error: goto() argument must be within 0-%d", GetCommandLineLen()-1);
				return false;				
			}
		}else{
			//goto()の引数が数値でない
			p_err_message->Format("Error: goto() argument must be number");
			return false;
		}
	}
	else if(ctrlData.controlCode == CALC_CTRL_IF)//if
	{
		//実行行番号を所望の位置に移動させる
		if(com->type==COM_TYPE_NUMBER)
		{
			if(com->number!=0)
			{//if(true)
				*pNextID = ctrlData.branchToLine[0];//このifの中を実行

				if (pBlockMap){
					pBlockMap[id] = true;
				}
			}
			else
			{//if(false)
				*pNextID = ctrlData.branchToLine[1];//このifの中はスキップ
			}
		}
		else
		{
			//if()の引数が数値でない
			p_err_message->Format("Error: if() argument must be number");
			return false;
		}
	}
	else if (ctrlData.controlCode == CALC_CTRL_ELSEIF)//elseif
	{
		if (com->type == COM_TYPE_NUMBER)
		{
			bool passed_block_flg = true;
			if (pBlockMap){
				passed_block_flg = pBlockMap[ctrlData.branchFrmLink];
			}

	
			if (com->number != 0 && passed_block_flg==false)
			{	//elseif(true)である
				//かつ当該If～endifブロックを通過中でまだtrueの条件が見つかっていない

				*pNextID = ctrlData.branchToLine[0];//このelseifの中を実行

				if (pBlockMap){
					pBlockMap[ctrlData.branchFrmLink] = true;
				}
			}
			else
			{//elseif(false)または当該If～endifブロックですでにtrueの条件が見つかっている

				*pNextID = ctrlData.branchToLine[1];//このelseifの中はスキップ
			}
		}
		else
		{
			//if()の引数が数値でない
			p_err_message->Format("Error: elseif() argument must be number");
			return false;
		}
	}
	else if(ctrlData.controlCode == CALC_CTRL_ELSE)//else
	{
		bool passed_block_flg = true;
		if (pBlockMap){
			passed_block_flg = pBlockMap[ctrlData.branchFrmLink];
		}


		if (passed_block_flg==false)//当該If～endifブロックを通過中でまだtrueの条件が見つかっていない
		{
			*pNextID = ctrlData.branchToLine[0];//else中を実行

			if (pBlockMap){
				pBlockMap[ctrlData.branchFrmLink] = true;
			}
		}
		else{
			*pNextID = ctrlData.branchToLine[1];//endifまでスキップ
		}


	}
	else if (ctrlData.controlCode == CALC_CTRL_ENDIF)//endif
	{
		*pNextID = id + 1;

		if (pBlockMap){
			pBlockMap[ctrlData.branchFrmLink] = false;
		}
	}
	else if(ctrlData.controlCode == CALC_CTRL_WHILE)//while
	{
		//実行行番号を所望の位置に移動させる
		if(com->type==COM_TYPE_NUMBER)
		{
			if(com->number!=0)
			{//while(true)
				*pNextID = ctrlData.branchToLine[0];//このブロック内を実行

				if (pBlockMap){
					pBlockMap[id] = true;
				}
			}
			else{//while(false)
				*pNextID = ctrlData.branchToLine[1];//このブロック内をスキップ


				if (pBlockMap){
					pBlockMap[id] = false;
				}
			}
		}
		else
		{
			//if()の引数が数値でない
			p_err_message->Format("Error: white() argument must be number");
			return false;
		}
	}
	else if(ctrlData.controlCode == CALC_CTRL_ENDWHILE)//endwhile
	{
		//実行行番号を所望の位置に移動させる
		*pNextID  = ctrlData.branchToLine[0];
	}
	else if (ctrlData.controlCode == CALC_CTRL_FOR)//for
	{
		//実行行番号を所望の位置に移動させる
		if (com->type == COM_TYPE_NUMBER)
		{
			if (com->number != 0)
			{	//if(true)かつ
				*pNextID = ctrlData.branchToLine[0];//このブロック内を実行

				if (pBlockMap){
					pBlockMap[id] = true;
				}
			}
			else{//if(false)
				*pNextID = ctrlData.branchToLine[1];//このブロック内をスキップ

				if (pBlockMap){
					pBlockMap[id] = false;
				}
			}
		}
		else
		{
			//if()の引数が数値でない
			p_err_message->Format("Error: for() argument must be number");
			return false;
		}
	}
	else if (ctrlData.controlCode == CALC_CTRL_ENDFOR)//endfor
	{
		//実行行番号を所望の位置に移動させる
		*pNextID = ctrlData.branchToLine[0];
	}
	else
	{
		*pNextID = id + 1;
	}

	return true;
}


/********************************************************************
機  能  名  称 : コマンドリストを順次実行する
関    数    名 : Calclate
引          数 :
戻    り    値 : 正常に計算されたらtrue
機          能 : 
			コマンドリスト内において、「(」  から  「)」 までの区間を計算して数値または文字列におきかえる。	
			ネストされた ( があれば再起呼び出しをする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::calculate_command_list( 
						  int				id,
						 CALC_PARAM_STRUCT	*p_calc_param,			//(in/out)計算の入出力パラメタ
						 COMMAND			*start ,				//(in/out)計算を開始する、コマンドリスト内のポインタ。→　関数終了後には数値or文字列が入っている
						 COMMAND			*additonal_com,			//(work)
						 int				*additional_com_id,		//(work)
						 CString			*p_err_message,			//(out)エラーメッセージ
						 bool				autoDrawImage			//(in)処理後に画像描画を実行する
						 )
{
	COMMAND		*command ;//注目する演算子


	if(start == NULL){//最初のコマンドがない
		p_err_message->Format("Syntex Error (Calclate0)");
		return	false;
	}

	if(start->type != COM_TYPE_PARENT  || start->pare != COM_PARENT_OPEN){//最初のコマンドが「(」でない
		p_err_message->Format("Syntex Error (Calclate1)");
		return	false;
	}

	


	//マクロおよび()のネストを解消

	for( command = start->next ; command != NULL ; command = command->next )//コマンドリストを順次検索
	{
		if( command->type == COM_TYPE_MACRO )
		{//マクロを処理する
			if( !calculate_command_list_with_macro(id, p_calc_param, command , additonal_com, additional_com_id, p_err_message, autoDrawImage) )	return false;
		}
		else if( command->type == COM_TYPE_PARENT )
		{
			if( command->pare == COM_PARENT_OPEN)
			{// ネストされた()内を処理する
				if( !calculate_command_list(id, p_calc_param, command , additonal_com, additional_com_id, p_err_message,autoDrawImage) ) return  false;
			}else{
				break;// 「 ) 」 が見つかったため検索終了
			}
		}
		else if( command->type ==  COM_TYPE_COMMA)
		{
			break;// 「 , 」 が見つかったため検索終了
		}

	}


	

	//変数を値（数値／文字列）に置き換える(ただし、代入演算子の直前の変数は置き換えない)
	for( command = start->next ; command != NULL ; command = command->next)//コマンドリストを順次検索
	{
		if(command->type == COM_TYPE_PARENT && command->pare == COM_PARENT_CLOSE){// 「 ) 」 が見つかったため検索終了
			
			
			break;
		}

		if( command->type == COM_TYPE_VARIABLE )
		{
			if (command->next->type == COM_TYPE_OPERATOR &&
				(command->next->ope == 10 || command->next->ope == 13 || command->next->ope == 14 || command->next->ope == 15 || command->next->ope == 16))
			{
				continue;	//代入演算子の直前の変数の場合は処理をスキップ
			}

			CALC_RESULT_STRUCT varres={0};
			char	varname[CALC_VARIABLE_NAME_MAX]={0};

			GetVariable(command->var_id, &varres, varname);//変数を取得

			if( varres.type == COM_TYPE_NUMBER )//変数の中身が数値の場合
			{
				command->type = COM_TYPE_NUMBER;
				command->number = varres.number;
			}
			else if( varres.type == COM_TYPE_STRING )//変数の中身が文字列の場合
			{
				int ret = push_string_to_buffer(id, CString(varres.str), p_err_message) ;
				if(ret>=0){
					command->type = COM_TYPE_STRING;
					command->str_id = ret;
				}else{
					return false;
				}
			}
		}
	}


	//i,j,x,yを数値に置き換える
	for( command = start->next ; command != NULL ; command = command->next)//コマンドリストを順次検索
	{
		if(command->type == COM_TYPE_PARENT && command->pare == COM_PARENT_CLOSE){// 「 ) 」 が見つかったため検索終了
			break;
		}

		if( command->type == COM_TYPE_COOR_X ){//i,x
			command->type = COM_TYPE_NUMBER;
			command->number = (double)p_calc_param->x;
		}else if( command->type == COM_TYPE_COOR_Y ){//j,y
			command->type = COM_TYPE_NUMBER;
			command->number = (double)p_calc_param->y;
		}
	}


	//この段階で命令が正常な場合は [(][数値][演算子][数値][演算子]...[数値][)] の順で並んでいる。

	//例外:負の符号つき数値の対応１
	if( start->next->type == COM_TYPE_OPERATOR && start->next->ope == 1)//「 ( 」の直後が「-」である
	{
			//[(][-][数値]→[(][0][-][数値]
			additonal_com[ *additional_com_id ].type = COM_TYPE_NUMBER;
			additonal_com[ *additional_com_id ].number = 0.0;
			additonal_com[ *additional_com_id ].prev = start;
			additonal_com[ *additional_com_id ].next = start->next;
			start->next->prev = additonal_com + *additional_com_id;
			start->next = additonal_com + *additional_com_id;
			(*additional_com_id) ++;
	}

	//例外:負の符号つき数値の対応２
	for( command = start->next ; command != NULL ; command = command->next)
	{
		if(command->type==COM_TYPE_OPERATOR && command->ope == 1 &&
			command->prev->type==COM_TYPE_OPERATOR && 
			(command->prev->ope == 10 || command->prev->ope == 13 || command->prev->ope == 14 || command->prev->ope == 15 || command->prev->ope == 16))//代入演算子の直後が「-」である
		{
			//[=][-][数値]→[=][0][-][数値]
			additonal_com[ *additional_com_id ].type = COM_TYPE_NUMBER;
			additonal_com[ *additional_com_id ].number = 0.0;
			additonal_com[ *additional_com_id ].prev = command->prev;
			additonal_com[ *additional_com_id ].next = command;
			command->prev->next = additonal_com + *additional_com_id;
			command->prev = additonal_com + *additional_com_id;
			(*additional_com_id) ++;
		}
	}



	//剰余算を処理する
	for( command = start->next ; command != NULL ; command = command->next)//コマンドリストを順次検索
	{
		if( command->type == COM_TYPE_PARENT  && command->pare == COM_PARENT_CLOSE){// 「 ) 」 が見つかったため検索終了
			break;
		}

		if( command->type == COM_TYPE_OPERATOR )//演算子
		{		
			if( command->ope == 2  ||  command->ope == 3 )	//剰余算
			{		

				if( command->prev->type != COM_TYPE_NUMBER  ||  command->next->type != COM_TYPE_NUMBER){//演算子の前後が数値ではない
					p_err_message->Format("Syntax Error (Calclate2)");
					return false;
				}


				double	 result;
					if( command->ope == 2 ){	//掛け算
						result = command->prev->number * command->next->number;
					}else{						//割り算
						result = command->prev->number / command->next->number;
					}

				//使った命令をリストから削除して代わりに計算結果を入れる
				COMMAND	*temp_next = command->next;
				command = command->prev;
				command->type = COM_TYPE_NUMBER;
				command->number = result;
				command->next = temp_next->next;
				temp_next->next->prev = command;
			}
		}
	}


	//加減算を処理する

	for( command = start->next ; command != NULL ; command = command->next)//コマンドリストを順次検索
	{
		if( command->type == COM_TYPE_PARENT  && command->pare == COM_PARENT_CLOSE){// 「 ) 」 が見つかったため検索終了
			break;
		}

		if( command->type == COM_TYPE_OPERATOR )//演算子
		{		
			if( command->ope == 0  ||  command->ope == 1 )	//加減算
			{		
				if( command->prev->type == COM_TYPE_NUMBER  &&  command->next->type == COM_TYPE_NUMBER)//演算子の前後が数値である
				{
					double	 result;
						if( command->ope == 0 ){	//足し算
							result = command->prev->number + command->next->number;
						}else{						//引き算
							result = command->prev->number - command->next->number;
						}

					//加減算に使った命令をリストから削除して代わりに計算結果を入れる
					COMMAND	*temp_next = command->next;
					command = command->prev;
					command->type = COM_TYPE_NUMBER;
					command->number = result;
					command->next = temp_next->next;
					temp_next->next->prev = command;


				}
				else if( command->ope == 0 && (command->prev->type == COM_TYPE_STRING &&  command->next->type == COM_TYPE_STRING ) )//+の両方が文字列である
				{
					//文字列合成
					CString str = CString( get_string_from_buffer(id, command->prev->str_id) ) + CString( get_string_from_buffer(id, command->next->str_id) );
				
					int str_id = push_string_to_buffer(id, str, p_err_message);
					if(str_id<0){
						return false;
					}
					
					//使った命令をリストから削除して代わりに計算結果を入れる
					COMMAND	*temp_next = command->next;
					command = command->prev;
					command->type = COM_TYPE_STRING;
					command->str_id = str_id;
					command->next = temp_next->next;
					temp_next->next->prev = command;
				}
				else
				{
					p_err_message->Format("Syntax Error (Calclate3)");
					return false;
				}



			}
		}
	}




	//比較演算子を処理する

	for( command = start->next ; command != NULL ; command = command->next)//コマンドリストを順次検索
	{
		if( command->type == COM_TYPE_PARENT  && command->pare == COM_PARENT_CLOSE){// 「 ) 」 が見つかったため検索終了
			break;
		}

		if( command->type == COM_TYPE_OPERATOR )//演算子
		{		
			if( command->ope == 4  ||  command->ope == 5 || command->ope == 6 ||command->ope == 7 || command->ope == 8 || command->ope == 9 )	//比較演算子
			{		
				if( command->prev->type == COM_TYPE_NUMBER  &&  command->next->type == COM_TYPE_NUMBER)//演算子の前後が数値である
				{
					double	 result;
						if( command->ope == 4 ){	//==
							result = (command->prev->number == command->next->number ) ? 1 :0 ;
						}else if( command->ope == 5 ){	//<
							result = (command->prev->number < command->next->number ) ? 1 : 0;
						}else if( command->ope == 6 ){	//>
							result = (command->prev->number > command->next->number ) ? 1 : 0;
						}else if( command->ope == 7 ){	//<=
							result = (command->prev->number <= command->next->number ) ? 1 : 0;
						}else if( command->ope == 8 ){	//>=
							result = (command->prev->number >= command->next->number ) ? 1 : 0;
						}else if( command->ope == 9 ){	//!=
							result = (command->prev->number != command->next->number ) ? 1 : 0;
						}
			

					//使った命令をリストから削除して代わりに計算結果を入れる
					COMMAND	*temp_next = command->next;
					command = command->prev;
					command->type = COM_TYPE_NUMBER;
					command->number = result;
					command->next = temp_next->next;
					temp_next->next->prev = command;
				}
				else if( (command->ope == 4 || command->ope == 9) && command->prev->type == COM_TYPE_STRING  &&  command->next->type == COM_TYPE_STRING)//演算子(==,!=)の前後が文字列である ⇒　文字列比較し、同じならば0,違えば1
				{
					int result=0; 
					if( strcmp( get_string_from_buffer(id, command->prev->str_id), get_string_from_buffer(id, command->next->str_id) )==0 ){//文字列が同じ
						if( command->ope == 4 ){//==
							result=1;
						}else{//!=
							result=0;
						}
					}else{
						if( command->ope == 4 ){//==
							result=0;
						}else{//!=
							result=1;
						}
					}

					

					//使った命令をリストから削除して代わりに計算結果を入れる
					COMMAND	*temp_next = command->next;
					command = command->prev;
					command->type = COM_TYPE_NUMBER;
					command->number = result;
					command->next = temp_next->next;
					temp_next->next->prev = command;
				}else{
					p_err_message->Format("Syntax Error (Calclate4)");
					return false;
				}
			}
		}
	}

	//論理比較演算子を処理する

	for (command = start->next; command != NULL; command = command->next)//コマンドリストを順次検索
	{
		if (command->type == COM_TYPE_PARENT  && command->pare == COM_PARENT_CLOSE) {// 「 ) 」 が見つかったため検索終了
			break;
		}

		if (command->type == COM_TYPE_OPERATOR)//演算子
		{
			if ( command->ope == 11 || command->ope == 12)	//比較演算子
			{
				if (command->prev->type == COM_TYPE_NUMBER  &&  command->next->type == COM_TYPE_NUMBER)//演算子の前後が数値である
				{
					double	 result;
					 if (command->ope == 11) {	//&&
						result = (command->prev->number != 0) && (command->next->number != 0);
					}
					else if (command->ope == 12) {	//||
						result = (command->prev->number != 0) || (command->next->number != 0);
					}


					//使った命令をリストから削除して代わりに計算結果を入れる
					COMMAND	*temp_next = command->next;
					command = command->prev;
					command->type = COM_TYPE_NUMBER;
					command->number = result;
					command->next = temp_next->next;
					temp_next->next->prev = command;
				}
				else {
					p_err_message->Format("Syntax Error (Calclate4)");
					return false;
				}
			}
		}
	}


	//代入演算子を処理する

	for( command = start->next ; command != NULL ; command = command->next)//コマンドリストを順次検索
	{
		if( command->type == COM_TYPE_PARENT  && command->pare == COM_PARENT_CLOSE){// 「 ) 」 が見つかったため検索終了
			break;
		}

		if( command->type == COM_TYPE_OPERATOR )//演算子
		{		
			if (command->ope == 10 || command->ope == 13 || command->ope == 14 || command->ope == 15 || command->ope == 16)
			{
				if (command->prev->type == COM_TYPE_VARIABLE  &&  command->next->type == COM_TYPE_NUMBER)//演算子の前が変数、後が数値である
				{

					//変数に数値を代入する
					CALC_RESULT_STRUCT varres = { 0 };
					char varname[CALC_VARIABLE_NAME_MAX];

					GetVariable(command->prev->var_id, &varres, varname);

					if (command->ope == 10){ // =
						varres.type = COM_TYPE_NUMBER;
						varres.number = command->next->number;
					}
					else if (varres.type == COM_TYPE_NUMBER)
					{
						if (command->ope == 13){// += 
							varres.number += command->next->number;
						}
						else if (command->ope == 14){// -= 
							varres.number -= command->next->number;
						}
						else if (command->ope == 15){// *= 
							varres.number *= command->next->number;
						}
						else if (command->ope == 16){// /= 
							varres.number /= command->next->number;
						}
						else{
							p_err_message->Format("Syntax Error (Calclate4)");
							return false;
						}
					}
					else{
						p_err_message->Format("Syntax Error (Calclate4)");
						return false;
					}

					SetVariable(command->prev->var_id, &varres);

					//使った命令をリストから削除して代わりに計算結果(数値)を入れる
					COMMAND	*temp_next = command->next;
					command = command->prev;
					command->type = COM_TYPE_NUMBER;
					command->number = varres.number;
					command->next = temp_next->next;
					temp_next->next->prev = command;

				}
				else if (command->ope == 10 && command->prev->type == COM_TYPE_VARIABLE  &&  command->next->type == COM_TYPE_STRING)//演算子の前が変数、後が文字列である
				{

					//変数に文字列を代入する
					CALC_RESULT_STRUCT varres = { 0 };
					char varname[CALC_VARIABLE_NAME_MAX];

					GetVariable(command->prev->var_id, &varres, varname);

					if (command->ope == 10){ // =
						varres.type = COM_TYPE_STRING;
						strcpy(varres.str, get_string_from_buffer(id, command->next->str_id));
					}
					else if (varres.type == COM_TYPE_STRING)
					{
						if (command->ope == 13)//+=
						{
							if (strlen(varres.str) + strlen(get_string_from_buffer(id, command->next->str_id)) >= CALC_STRING_LEN_MAX)
							{
								p_err_message->Format("too much long strings!");
								return false;
							}

							strcat(varres.str, get_string_from_buffer(id, command->next->str_id));
						}
						else{
							p_err_message->Format("Syntax Error (Calclate4)");
							return false;
						}
					}
					else{
						p_err_message->Format("Syntax Error (Calclate4)");
						return false;
					}

					SetVariable(command->prev->var_id, &varres);

					//使った命令をリストから削除して代わりに計算結果を入れる
					COMMAND	*temp_next = command->next;
					command = command->prev;
					command->type = COM_TYPE_STRING;
					command->str_id = temp_next->str_id;
					command->next = temp_next->next;
					temp_next->next->prev = command;

				}
				else{
					p_err_message->Format("Syntax Error (Calclate4)");
					return false;
				}
			}
		}
	}




	//後処理

	//この段階では () または ( 数値 or文字列 )の順に並んでいる
	//また、commandは最後の)を表している

	if(start->next->type == COM_TYPE_NUMBER)//(数値)
	{
		start->type = COM_TYPE_NUMBER;
		start->number = start->next->number;
		start->next = command->next;
			if( start->next != NULL ){//次の命令語がある場合はアドレスを渡す
				start->next->prev = start;
			}
	}
	else if(start->next->type == COM_TYPE_STRING)//(文字列)
	{
		start->type = COM_TYPE_STRING;
		start->str_id = start->next->str_id;
		start->next = command->next;
			if( start->next != NULL ){//次の命令語がある場合はアドレスを渡す
				start->next->prev = start;
			}
	}else{//()
		start->type = COM_TYPE_NUMBER;
		start->number = 0;
		start->next = command->next;
			if( start->next != NULL ){//次の命令語がある場合はアドレスを渡す
				start->next->prev = start;
			}
	}


	return true;
}


/********************************************************************
機  能  名  称 : for文を実行する
関    数    名 : calculate_command_list_with_macro
引          数 :
戻    り    値 : 正常に計算されたらtrue
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::calculate_for_command_list(
	int				id,						//(in)
	int				mode,					//(in)0:初期化->ループ継続条件計算 　1:カウンタ更新->ループ継続条件計算
	CALC_PARAM_STRUCT	*p_calc_param,		//(in/out)計算の入出力パラメタ
	COMMAND			*start,					//(in/out)コマンドリスト内のfor文へのポインタ。→　関数終了後には数値or文字列が入っている
	COMMAND			*additonal_com,			//(work)
	int				*additional_com_id,		//(work)
	CString			*p_err_message,			//(out)エラーメッセージ
	bool			autoDrawImage		//(in)処理後に画像描画を実行する
	)
{
	COMMAND		*command;
	MACRO_IO_STRUCT	arg_val[CALC_MACRO_ARGS_MAX] = { 0 };//マクロ引数
	MACRO_IO_STRUCT	result_val = { 0 };//マクロ実行結果
	int			overloadID = 0;//マクロ引数のオーバーライド番号
	


	//入力時点ではコマンドリストは以下の並びになっているはずである
	//[for][(].....[,].....[,].....[)]


	if (start->next == NULL){//forの次のコマンドがない
		p_err_message->Format("Syntex Error (calculate_for_command_list)");
		return	false;
	}
	if (start->next->type != COM_TYPE_PARENT || start->next->pare != 0){//forの次のコマンドが「(」でない
		p_err_message->Format("Syntex Error (calculate_for_command_list)");
		return	false;
	}

	if (start->next->next == NULL){//forの次の次のコマンドがない
		p_err_message->Format("Syntex Error (calculate_for_command_list)");
		return	false;
	}




		int			arg_counter = 0;//「,」でつながった構文の数

		//初期化,ループ継続条件計算,カウンタ更新の構文を探す

		COMMAND	*p_arg_com[3] = { 0 };//命令語の先頭アドレスの格納先

		p_arg_com[arg_counter++] = start->next;//初期化となる命令語の先頭アドレス

		for (command = start->next; command != NULL; command = command->next)
		{
			if (command->type == COM_TYPE_COMMA)
			{//「,」が見つかった

				if (arg_counter == 3){
					p_err_message->Format("Syntax Error (calculate_for_command_list)");
					return false;
				}

				//「,」をコマンドから削除してその位置に「 ) 」と「 ( 」を挿入する
				// for( a , b , c) → for( a )( b )( c )
				command->type = COM_TYPE_PARENT;
				command->pare = COM_PARENT_CLOSE;
				additonal_com[*additional_com_id].type = COM_TYPE_PARENT;
				additonal_com[*additional_com_id].pare = COM_PARENT_OPEN;
				additonal_com[*additional_com_id].prev = command;
				additonal_com[*additional_com_id].next = command->next;
				command->next = &additonal_com[*additional_com_id];
				(*additional_com_id)++;

				p_arg_com[arg_counter++] = command->next;//引数となる命令語の先頭アドレス
			}
			else if (command->type == COM_TYPE_PARENT && command->pare == COM_PARENT_CLOSE)
			{
				//「)」がみつかったら引数探索終了
				break;
			}
		}

		if (arg_counter != 3)
		{
			p_err_message->Format("Syntax Error (calculate_for_command_list)");
			return false;
		}


		//命令後を実行する

		if (mode == 0)
		{
			if (!calculate_command_list(id, p_calc_param, p_arg_com[0], additonal_com, additional_com_id, p_err_message, autoDrawImage))	return	false;
			if (!calculate_command_list(id, p_calc_param, p_arg_com[1], additonal_com, additional_com_id, p_err_message, autoDrawImage))	return	false;
		}
		else{
			if (!calculate_command_list(id, p_calc_param, p_arg_com[2], additonal_com, additional_com_id, p_err_message, autoDrawImage))	return	false;
			if (!calculate_command_list(id, p_calc_param, p_arg_com[1], additonal_com, additional_com_id, p_err_message, autoDrawImage))	return	false;
		}


		//この段階ではp_arg_com[1]の位置には文字列か数値がきているはずである

		if (p_arg_com[1]->type == COM_TYPE_NUMBER)
		{
			//この行の結果を入れ替える
			start->type = p_arg_com[1]->type;
			start->number = p_arg_com[1]->number;
		}
		else
		{
			p_err_message->Format("Error: 2nd argument of for() must be number");
			return false;
		}


	return true;
}



/********************************************************************
機  能  名  称 : コマンドリストのマクロ部分を順次実行する
関    数    名 : calculate_command_list_with_macro
引          数 :
戻    り    値 : 正常に計算されたらtrue
機          能 : 
			コマンドリスト内において、マクロ区間を計算して数値または文字列におきかえる。	
			ネストされた ( があれば再起呼び出しをする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
2020.04.29			 「()」のネストに対応
********************************************************************/
bool CCalculate::calculate_command_list_with_macro(
						     int				id,						//(in)
							 CALC_PARAM_STRUCT	*p_calc_param,			//(in/out)計算の入出力パラメタ
							 COMMAND			*start,					//(in/out)コマンドリスト内のマクロへのポインタ。→　関数終了後には数値or文字列が入っている
							 COMMAND			*additonal_com,			//(work)
							 int				*additional_com_id,		//(work)
							 CString			*p_err_message,			//(out)エラーメッセージ
							 bool				autoDrawImage			//(in)処理後に画像描画を実行する
							 )
{
	COMMAND		*command;
	MACRO_IO_STRUCT	arg_val[CALC_MACRO_ARGS_MAX]={0};//マクロ引数
	MACRO_IO_STRUCT	result_val={0};//マクロ実行結果
	int			overloadID = 0;//マクロ引数のオーバーライド番号
	int			arg_counter = 0;//引数の数




	//入力時点ではコマンドリストは以下の並びになっているはずである
	//[マクロ][(].....[,].....[)]


	if(start->next == NULL){//マクロの次のコマンドがない
		p_err_message->Format("Syntex Error (calculate_command_list_with_macro0)");
		return	false;
	}
	if(start->next->type != COM_TYPE_PARENT  || start->next->pare != 0){//マクロの次のコマンドが「(」でない
		p_err_message->Format("Syntex Error (calculate_command_list_with_macro1)");
		return	false;
	}

	if(start->next->next==NULL ){//マクロの次の次のコマンドがない
		p_err_message->Format("Syntex Error (calculate_command_list_with_macro2)");
		return	false;
	}


			

	if (start->next->next->type == COM_TYPE_PARENT &&	//マクロの次の次のコマンドが「)」である
		start->next->next->pare == COM_PARENT_CLOSE)
	{
		//マクロ引数がない場合

		//マクロ定義に引数なしのものがあるか探す
		bool varidArgsFlg = false;
		for (overloadID = 0; overloadID < pMacroDefinision[start->mac].overloadCnt; overloadID++) {
			if (pMacroDefinision[start->mac].args[overloadID] == 0) {
				varidArgsFlg = true;
				break;
			}
		}
		if (!varidArgsFlg) {//なければエラー終了
			p_err_message->Format("Error: invarid argument count for %s", pMacroDefinision[start->mac].name);
			return false;
		}

		//マクロ実行
		if( !execute_macro(id, p_calc_param, &result_val, start->mac, overloadID, arg_val,  p_err_message, autoDrawImage) ){
			return false;
		}

		//後処理
		//マクロの計算に用いた命令語を削除して計算結果を挿入する
		start->type = result_val.type;
		if(result_val.type == COM_TYPE_NUMBER){
			start->number = result_val.number;
		}else{
			start->str_id = result_val.str_id;
		}

		start->next = start->next->next->next;
		if (start->next != NULL) {//次の命令語があればアドレスを渡す
			start->next->prev = start;
		}
	}
	else
	{
		//マクロ引数がある場合
		

		//引数となる命令語を探す

		COMMAND	*p_arg_com[CALC_MACRO_ARGS_MAX] = { 0 };//命令語の先頭アドレスの格納先

		p_arg_com[arg_counter++] = start->next;//最初の引数となる命令語の先頭アドレス


		int pearNestCnt = 0;//「()」のネスト回数

		//2番目以降の引数を探す
		for (command = start->next; command != NULL; command = command->next)
		{

			if (command->type == COM_TYPE_COMMA)
			{//「,」が見つかった

				if (arg_counter + 1 >= CALC_MACRO_ARGS_MAX) {//引数の数が多すぎる
					p_err_message->Format("Syntax Error (calculate_command_list_with_macro3)");
					return false;
				}

				//if (pearNestCnt>1)
				//{//「()」のネスト位置がおかしい
				//	p_err_message->Format("Syntax Error () nest?");
				//	return false;
				//}

				//「,」をコマンドから削除してその位置に「 ) 」と「 ( 」を挿入する
				// macro( a , b ) → macro( a )( b )
				command->type = COM_TYPE_PARENT;
				command->pare = COM_PARENT_CLOSE;
				additonal_com[*additional_com_id].type = COM_TYPE_PARENT;
				additonal_com[*additional_com_id].pare = COM_PARENT_OPEN;
				additonal_com[*additional_com_id].prev = command;
				additonal_com[*additional_com_id].next = command->next;
				command->next = &additonal_com[*additional_com_id];
				(*additional_com_id)++;

				p_arg_com[arg_counter++] = command->next;//引数となる命令語の先頭アドレス
					
				if (pearNestCnt > 0) {//次の命令語アドレスは、この場所に新しく入った「(」なので、ネストの数を調整する
					pearNestCnt--;
				}
			}
			else if (command->type == COM_TYPE_MACRO)
			{//マクロの引数内にさらにマクロがある場合は深さ優先で処理する

				if (!calculate_command_list_with_macro(id, p_calc_param, command, additonal_com, additional_com_id, p_err_message, autoDrawImage))	return false;
			}
			else if (command->type == COM_TYPE_PARENT )
			{
				if (command->pare == COM_PARENT_OPEN)
				{
					pearNestCnt++;//「()」のネスト回数を1増やす
				}
				else{
					if (pearNestCnt > 1){//「()」のネストが深い場合
						pearNestCnt--;//「()」のネスト回数を1減らす
					}
					else{
						//このマクロと対応する「)」がみつかったら引数探索終了
						break;
					}

				}
			}
		}


		//マクロの引数となる命令後を先にすべて実行する

		for (int c = 0; c < arg_counter; c++) {
			if (!calculate_command_list(id, p_calc_param, p_arg_com[c], additonal_com, additional_com_id, p_err_message, autoDrawImage))	return	false;
		}


		//この段階では　[マクロ][数値/文字列][数値/文字列]...の順　に並んでいるはずである


		//引数の並びが正しいかチェックする
		bool varidArgsFlg = false;
		COMMAND *pLastCommand;

		for (overloadID = 0; overloadID < pMacroDefinision[start->mac].overloadCnt; overloadID++)//すべてのオーバーロードに対してチェック
		{
			if (pMacroDefinision[start->mac].args[overloadID] == arg_counter)
			{
				bool invaridArgCorderFlg = true;

				COMMAND *com = start->next;
				for (int c = 0; c < arg_counter; c++, com = com->next)
				{
					if (com->type == pMacroDefinision[start->mac].arg_type[overloadID][c])//引数の種類（数値/文字列）が正しいか
					{
						if (com->type == COM_TYPE_NUMBER)
						{//数値引数
							arg_val[c].type = COM_TYPE_NUMBER;
							arg_val[c].number = com->number;
						}
						else {//文字列引数
							arg_val[c].type = COM_TYPE_STRING;
							arg_val[c].str_id = com->str_id;
						}
					}
					else
					{
						invaridArgCorderFlg = false;
						break;
					}
				}

				if (invaridArgCorderFlg) //正しい
				{
					pLastCommand = com;
					varidArgsFlg = true;
					break;
				}
			}
		}

		if (!varidArgsFlg) {//なけらばエラー終了
			p_err_message->Format("Error: invarid argument for %s", pMacroDefinision[start->mac].name);
			return false;
		}

		//マクロ実行
		if (!execute_macro(id, p_calc_param, &result_val, start->mac, overloadID, arg_val, p_err_message, autoDrawImage))	return false;


		//後処理
		//マクロの計算に用いた命令語を削除して計算結果を挿入する
		start->type = result_val.type;
		if (result_val.type == COM_TYPE_NUMBER) {
			start->number = result_val.number;
		}
		else {
			start->str_id = result_val.str_id;
		}

		start->next = pLastCommand;
		if (start->next != NULL) {//次の命令語があればアドレスを渡す
			start->next->prev = start;
		}
	}

	return true;
}




/********************************************************************
機  能  名  称 : 画像メモリの参照
関    数    名 : refer_image
引          数 :
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::refer_image( 
						   double	*p_result,		//(out)参照結果
						   int		image_no,		//(in)画像メモリ番号
						   double	x,				//(in)画像座標
						   double	y,				//(in)画像座標
						   int		page,			//(int)
						   int		channel,		//(int)
						   CString	*p_err_message,	//(out)エラーメッセージ
						   bool		to_mask			//(in)マスク参照するかどうか
						   )
{
	CDataUnit		*pDU;
	CString			str;
	CSize			size;
							

	if( 0<=image_no  &&  image_no < API.GetDataUnitNumber() )
	{
		if( API.GetDataUnit( (int)image_no ) == NULL){
			p_err_message->Format("Error: image%d do not have data",image_no);
			return false;
		}
	}
	else
	{
		p_err_message->Format("Error: image id must be within 0 ～ %d ", API.GetDataUnitNumber() -1);
		return false;
	}


	pDU = API.GetDataUnit( image_no);
	size = pDU->DataSize;

	if(to_mask){
		*p_result = (double)*(pDU->pMaskData + (int)x + (int)y * pDU->DataSize.cx);
	}else{
		*p_result  = pDU->GetDataValue( x, y, page ,channel);
	}

	return	true;
}


/********************************************************************
機  能  名  称 : コマンドリストのチェック
関    数    名 : refer_image
引          数 :
戻    り    値 : チェックが通ればtrue
機          能 : (と)の数が同じかチェック
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::check_parenthesis(
								COMMAND		*com,			//(in)コマンドリスト先頭ポインタ
								CString		*p_err_message	//(out)エラーメッセージ
								)
{
	COMMAND		*command;
	int			start_num, end_num;
	start_num = 0;	// 「 ( 」 の数
	end_num = 0;	// 「 ) 」 の数

	for(command = com ; command != NULL ; command = command->next){
		if( command->type == 2 ){
			if( command->pare == 0 )	start_num++;
			else						end_num++;
		}
	}

	if( start_num != end_num ){
		p_err_message->Format("Error: count ( and ) must be same");
		return false;

	}else{
		return true;
	}
}
