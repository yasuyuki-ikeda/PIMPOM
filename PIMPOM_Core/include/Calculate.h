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
#pragma once
#include "global_define.h"
#include "global_typedef.h"

///*************************************
//         マ  ク  ロ   定   義         
///*************************************

#define		CALC_MACRO_CNT_MAX			(400)	//マクロの最大数
#define		CALC_MACRO_NAME_LEN_MAX		(256)	//マクロ名の最大長
#define		CALC_MACRO_ARGS_MAX			(15)	//マクロ引数の最大数
#define		CALC_MACRO_OVERLOAD_MAX		(10)		//マクロ変数をオーバーロードできる数
#define		CALC_COMMAND_LIST_MAX		(150)	//コマンドリストの最大要素数
#define		CALC_STRING_BUFFER_LEN		(30)	//1行のコマンドあたりで扱える文字列型データの最大数
#define		CALC_STRING_LEN_MAX			(1024)	//文字列の最大長
#define		CALC_COMMAND_LINE_MAX		(5000)	//コマンド行数の最大
#define		CALC_VARIABLE_NAME_MAX		(256)	//変数名の最大数
#define		CALC_VARIABLE_BUFFER_LEN	(150)	//変数最大数
#define		CALC_FILE_FINDER_MAX		(3)	//ファイル列挙の最大数


//コマンドリストの要素の種類
#define		COM_TYPE_NUMBER			0	//数値
#define		COM_TYPE_OPERATOR		1	//演算式
#define		COM_TYPE_PARENT			2	//括弧
#define		COM_TYPE_MACRO			3	//マクロ
#define		COM_TYPE_COMMA			4	//カンマ
#define		COM_TYPE_COOR_X			5	//x座標
#define		COM_TYPE_COOR_Y			6	//y座標
#define		COM_TYPE_STRING			7	//文字列
#define		COM_TYPE_CTRL			8	//コマンド実行制御文
#define		COM_TYPE_VARIABLE		9	//変数

#define		COM_PARENT_OPEN			0	//(	
#define		COM_PARENT_CLOSE		1	//)	



//コマンド実行制御コード
#define		CALC_CTRL_BREAK					1000	//break
#define		CALC_CTRL_GOTO					1001	//goto()
#define		CALC_CTRL_IF					1002	//if()
#define		CALC_CTRL_ELSE					1003	//else
#define		CALC_CTRL_ENDIF					1004	//endif
#define		CALC_CTRL_CLEAR					1005	//clear
#define		CALC_CTRL_WHILE					1006	//while()
#define		CALC_CTRL_ENDWHILE				1007	//endwhile
#define		CALC_CTRL_ELSEIF				1008	//elseif
#define		CALC_CTRL_FOR					1009	//for
#define		CALC_CTRL_ENDFOR				1010	//endfor

// *************************************
//         構  造  体   定   義         
// *************************************

//マクロ定義の構造体
typedef struct MACRO_DEF_STRUCT_{
	short	overloadCnt;	//オーバーロード数
	char	name[CALC_MACRO_NAME_LEN_MAX];		//マクロ名
	unsigned char	args[CALC_MACRO_OVERLOAD_MAX];								//引数の数
	unsigned char	arg_type[CALC_MACRO_OVERLOAD_MAX][CALC_MACRO_ARGS_MAX];		//引数の種類
	int		ret_type;//戻り値の型（マクロによっては実行時に変更する場合あり）
}MACRO_DEF_STRUCT;


//マクロの引数,戻り値の構造体
typedef struct MACRO_IO_STRUCT_{
	short	type;	//引数の種類 0:数値  7:文字列
	double  number;	//数値引数
	short	str_id;	//文字列引数(文字列バッファのID)
}MACRO_IO_STRUCT;




//コマンドリストの構造体
typedef struct COMMAND{
	COMMAND		*prev;//リストの一つ前のアドレス
	COMMAND		*next;//リストの一つ後のアドレス

	//コマンドの種類
	short		type;	//0: 定数  1:オペレータ  2:括弧   3:マクロ		4:,(マクロとともに使う)	
						//5:i or x	6:j or y	7:文字列	8:制御構文　9:変数
	//中身
	double		number;	//type が 0 のとき数値
	short		ope;	//type が 1 のとき		0:+   1:-   2:*   3:/  4:==  5:<  6:>  7:<= 8:>= 9:!=　10: = 11:&&  12: ||  13:+=  14:-=  15:*=  16/=
	short		pare;	//type が 2 のとき     0:(   1:)
	short		mac;	//type が 3 のとき マクロID
	short		str_id;	//typeが7のとき文字列バッファのID
	short		var_id; //typeが9のときの変数バッファのID
}COMMAND;


//文字列バッファの構造体
typedef struct CALC_STRING_BUFFER_STRUCT_{
	char	strbuffer[CALC_STRING_BUFFER_LEN][CALC_STRING_LEN_MAX+1];//CALC_STRING_BUFFER_LEN個の文字列のバッファ
	short	count;//文字列バッファを何個まで使用しているか
	short	staticCount;//固定文字列(コマンドライン上で""で囲まれるもの)だけで文字列バッファを何個まで使用しているか
}CALC_STRING_BUFFER_STRUCT;



//コマンドライン制御関連の構造体
typedef struct CALC_CTRL_DATA_{
	short				controlCode;						//コマンドライン制御コード
	short				branchNestCount;					//if～endif/while～endwhileまでのネスト
	int					branchToLine[2];					//if/else/whileの場合に次にジャンプする行番号
	int					branchFrmLink;						//else/endif/endwhileからif/whileへの逆リンク
}CALC_CTRL_DATA;


//計算の入力パラメタの構造体
typedef struct CALC_PARAM_STRUCT_{
	int					x;//画像座標x
	int					y;//画像座標y
	int					rgbType;//RGBのどこに入れるか
	bool				apiMode;//APIのマクロを実行するか
	BATCH_INPUT			*pbatchIn;
	BATCH_OUTPUT		*pbatchOut;
}CALC_PARAM_STRUCT;



//計算結果の構造体
typedef struct CALC_RESULT_STRUCT_{
	short	type;//タイプ(数値 or 文字列)
	double	number;//数値
	char	str[CALC_STRING_LEN_MAX];//文字列
	DWORD	elapsedTime;//経過時間

}CALC_RESULT_STRUCT;



//１行のコマンド実行に必要な全データをまとめた構造体
typedef struct CALC_DATA_STRUCT_{
	char						commandString[CALC_STRING_LEN_MAX];	//コマンド文字列
	CALC_STRING_BUFFER_STRUCT	*pStrignBuffer;						//文字列バッファの先頭ポインタ
	COMMAND						*pCommandList;						//コマンドリストの先頭ポインタ
	CALC_CTRL_DATA				ctrlData;
	CALC_RESULT_STRUCT			result;								//計測結果
	char						errorMessage[CALC_STRING_LEN_MAX];	//エラーメッセージ
}CALC_DATA_STRUCT;



//変数の構造体
typedef struct CALC_VARIABLE_BUFFER_STRUCT_{
	char				name[CALC_VARIABLE_NAME_MAX];//変数名
	CALC_RESULT_STRUCT	data;//変数の中身
}CALC_VARIABLE_STRUCT;



///*************************************
//         クラス   定   義         
///*************************************
class PIMPOM_EXT CCalculate
{
public:
	CCalculate();
	virtual ~CCalculate();

	void	ClearAll();//すべてのコマンドを削除する
	bool	RegisterCommandString(CString com_str[], int com_str_cnt, CString *p_err_message);//コマンド文字の解釈と登録
	bool	ExecuteAPI(int id, CString *p_err_message, int *pNextID = NULL, BATCH_INPUT *p_batch_in = NULL, BATCH_OUTPUT *p_batch_out = NULL, bool autoDrawImage = true, bool *pBlockMap = NULL);//実行
	bool	Execute(int id, int i, int j, CString *p_err_message, int rgb = -1, bool ApiMode = false, int *pNextID = NULL, BATCH_INPUT *p_batch_in = NULL, BATCH_OUTPUT *p_batch_out = NULL, bool autoDrawImage = true, bool *pBlockMap = NULL);//画像座標ごとに実行
	int		GetResult(int id, double *number, char str[], CString *p_err_message,  DWORD *p_elapsed_time=NULL);//実行結果取得
	
	bool	FindCtrlCode(CString str, int index ,int *p_ctrl_id, int *pIndexIncliment);//文字列からコマンド実行制御構文を探す
	bool	IsMacroName(CString str, int index, int *p_macro_id, int *pIndexIncliment);//文字列がマクロ名かどうか判断
	bool	IsParenthesis( CString str , int index , int *p_pare);//文字列が括弧かどうか判断
	bool	IsOperator(CString comstr, int index, int *p_ope , int *pIndexIncliment);//文字列が演算子かどうか判断
	bool	IsNumberValue(CString comstr, int index , double *p_number,int *pIndexIncliment);//文字列が数値かどうか判断
	bool	IsStringValue(CString comstr, int index , CString *p_str ,int *pIndexIncliment);//文字列が""で囲まれているかどうか判断
	bool	IsDefinedVariable(CString comstr, int index ,int *p_var_id, int *pIndexIncliment);//文字列が宣言済み変数かどうか判断
	bool	IsComment(CString	comstr,	int index);//文字列がコメントかどうか判断
	int		GetCommandLineLen();//登録されているコマンドライン数を返す

	static bool	ExtendSubCommands(int sub_com_num, CString sub_com_str[], CString sub_com_id[], CString *p_com_str, CString *p_err_message, bool *sub_com_enable=NULL);

	
	bool	GetMacroDefinision(int id, MACRO_DEF_STRUCT *pMcr);//マクロの定義を返す
	int		FindVariableDefinission(CString	comstr,	CString	*p_err_message);//変数宣言を探す
	bool	GetVariable(int id, CALC_RESULT_STRUCT *pvar, char varname[]);//変数を取得する
	bool	SetVariable(int id, CALC_RESULT_STRUCT *pvar);//変数を設定する



private:
	void debug_out(COMMAND	*start);
	void debug_out_ln(COMMAND	*start);

	//マクロ定義
	bool	define_macro( int ret_type, int id, char name[], 
						  int argtype0=-1, int argtype1=-1, int argtype2=-1, int argtype3=-1, int argtype4=-1, 
						  int argtype5=-1, int argtype6=-1, int argtype7=-1, int argtype8=-1, int argtype9=-1, 
						  int argtype10=-1, int argtype11=-1,int argtype12=-1, int argtype13=-1, int argtype14=-1);
	void	define_macros();

	//コマンド文字列の解析
	int		command_string_to_command_list(int id, CString  comstr, COMMAND commands[], CString *p_err_message, int *p_ctrl_code);//コマンド文字列を解析してコマンドリストを作る							 
	bool	check_parenthesis(COMMAND *com, CString *p_err_message);
	bool	set_branch_property(int com_str_cnt, CString *p_err_message)	;


	//コマンドリストの実行
	bool	calculate_for_command_list(int id, int mode, CALC_PARAM_STRUCT	*p_param, COMMAND  *com, COMMAND *additonal_com, int *additional_com_id, CString *p_err_message, bool autoDrawImage);//for文の実行
	bool	calculate_command_list(int id,  CALC_PARAM_STRUCT	*p_param, COMMAND  *com, COMMAND *additonal_com,int *additional_com_id, CString *p_err_message, bool autoDrawImage);//コマンドリストの実行
	bool	calculate_command_list_with_macro(int id, CALC_PARAM_STRUCT	*p_param, COMMAND *com, COMMAND *additonal_com,int *additional_com_id, CString *p_err_message, bool autoDrawImage);//マクロ関連のコマンドリストの実行
	bool	refer_image(double *p_result, int image_no, double x, double y, int page, int channel, CString *p_err_message, bool to_mask=false);//画像参照
	bool	execute_macro(int id, CALC_PARAM_STRUCT	*p_calc_param, MACRO_IO_STRUCT *p_result, int macro_no, int overwrideID, MACRO_IO_STRUCT params[], CString *p_err_message, bool autoDrawImage);//マクロの実行（実装はCalculateMacro.cpp）
	bool	set_next_id(int id, CALC_CTRL_DATA ctrlData, COMMAND *com, CString *p_err_message, int *pNextID, bool *blockMap);


	//データ管理
	bool	init_calc_structure(int id, CString *p_err_message);//データメモリ初期化
	void	deinit_calc_structure(int id);//メモリ解放
	int		push_string_to_buffer(int id, CString str, CString *p_err_message, bool isStatic=false);//文字列バッファに文字列を１つ挿入
	char	*get_string_from_buffer(int id, int str_id);//文字列バッファから文字列を取得
	void	delete_non_static_string_buffer(int id);//文字列バッファから固定文字でないものを削除
	bool	set_command_list(int id, CString commandStr, COMMAND *pNewCommandCodeList, int ctrl_code);//コマンドリストを設定
	COMMAND *get_command_list(int id, CALC_CTRL_DATA* p_ctrl_code);//コマンドリストの先頭ポインタ取得
	int		get_command_listLen(COMMAND *command);//コマンドリストの長さ取得
	void    copy_command_list(COMMAND *dst, COMMAND *src, int size);//コマンドリストをコピー
	bool	set_result(int id, COMMAND *com, DWORD elapsed_time);//計測結果を設定
	void	set_error_message(int id, CString err );
	int		new_variable( CString str, CString *p_err_message);//新変数
	void	clear_variable();//変数宣言すべて削除



	MACRO_DEF_STRUCT		*pMacroDefinision;//マクロの定義の配列先頭ポインタ
	
	CALC_DATA_STRUCT		*pCalcDataStructMap[CALC_COMMAND_LINE_MAX];//コマンド実行に必要なデータの先頭ポインタマップ

	CALC_VARIABLE_STRUCT	*pCalcVariableMap[CALC_VARIABLE_BUFFER_LEN];//変数の先頭ポインタマップ

	CFileFind	*pFileFinder[CALC_FILE_FINDER_MAX];//ファイル一覧作成のためのファインダ
};