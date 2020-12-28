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
//PIMPOM全体で使用可能なマクロを定義する
/*******************************************************************/
#pragma once

//**************************************
//         インクルードファイル         
//**************************************
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <LIMITS.H>
#include <FLOAT.H>
#include <winuser.h>


///*************************************
//         マ  ク  ロ   定   義         
///*************************************

//ＩＤの定義（いじるべからず）//////////////////////////////////////////////////
//パレットのＩＤ
#define		PALETTE_GRAYSCALE			(0)
#define		PALETTE_NEGATIVE			(1)
#define		PALETTE_RAINBOW				(2)
#define		PALETTE_BLACK_RAINBOW		(3)
#define		PALETTE_COSINE				(4)
#define		PALETTE_BLUE_YELLOW			(5)
#define		PALETTE_GREEN_RED			(6)
#define		PALETTE_LABEL				(7)


//データの種類
#define		BYTE_FORMAT			(0)
#define		FLOAT_FORMAT		(1)
#define		COMPLEX_FORMAT		(2)
#define		THREE_D_FORMAT		(3)
#define		FLOAT_3D_FORMAT		(4)
#define		XY_FORMAT			(5)
#define		XYZ_FORMAT			(6)
#define		RGB_FORMAT			(7)
#define		WORD_FORMAT			(8)
#define		LONG32_FORMAT		(9)		//Halconと競合しないように
#define		RGB_3D_FORMAT		(10)
#define		SHORT16_FORMAT		(11)
#define		DWORD_FORMAT		(12)

//データコンバート
#define		COMP_TO_REAL	(0)
#define		COMP_TO_IMAG	(1)
#define		RGB_TO_ALL	(0)
#define		RGB_TO_R	(1)
#define		RGB_TO_G	(2)
#define		RGB_TO_B	(3)

//現在選択中の画像メモリを表す
#define		CURRENT_IMAGE		(-1)

//マスク
#define		MASKED_PIXEL				(1)
#define		UNMASKED_PIXEL				(0)		

#define		CUSTOM_MENU_ID				(9999)
#define		FILE_IO_LOG_MENU_ID			(9900)

//作業領域の種類
#define		WORK_AREA_TYPE_NOTHING		(-1)
#define		WORK_AREA_TYPE_RECT			(0)
#define		WORK_AREA_TYPE_LINE			(1)
#define		WORK_AREA_TYPE_ELLIPSE		(2)
#define		WORK_AREA_TYPE_CIRCLE		(3)
#define		WORK_AREA_TYPE_ARC			(4)
#define		WORK_AREA_TYPE_POLYGON		(5)

//マークのタイプ
#define		MARK_TYPE_NOTHING		(-1)
#define		MARK_TYPE_POINT			(0)
#define		MARK_TYPE_LINE			(1)



//バッチ実行結果
#define		BATCH_EXEC_STATE_NORMAL		(0)//通常実行された
#define		BATCH_EXEC_STATE_SKIPPED	(1)//処理がスキップされた
#define		BATCH_EXEC_STATE_RETRY		(2)//リトライ要求
#define		BATCH_EXEC_STATE_STOP		(3)//中断要求

//通信の種類
#define		SOCKET_SERVER		(0)	//サーバソケット
#define		SOCKET_CLIENT		(1)	//クライアントソケット
#define		SOCKET_SERVER_SUB	(2)

//バックグラウンド処理（別スレッド）のID
#define		BGPROC_NOTHING		(0)
#define		BGPROC_CAPTURING	(1)
#define		BGPROC_MOVIE		(2)
#define		BGPROC_BATCH_EXC	(3)

//共有メモリを使ったデータの入力
#define	 SHEAREMEM_NAME		"PIMPOM_SHAREDMEM"
#define  SHEAREMEM_HEADER_SIZE		(20)


#define		FILE_PATH_LEN_MAX			(2048)	//ファイルパスの最大長
#define		MULTI_FILE_PATH_BUFFER		(500000)	//CFileDialogのバッファ

//文字定数の設定////////////////////////////////////////////////////////////////


#define		MAIN_W		(640)					//メインイメージの幅のデフォルト値
#define		MAIN_H		(480)					//メインイメージの高さのデフォルト値


//メインウィンドウ(MainDlg)
#define		MAIN_WIN_LEFT_MARGIN	(0)		//メインイメージ表示領域からメインウィンドウ左端までのマージン
#define		MAIN_WIN_TOP_MARGIN		(0)		//メインイメージ表示領域からメインウィンドウ上端までのマージン
#define		MAIN_WIN_RIGHT_MARGIN	(50)	//メインイメージ表示領域からメインウィンドウ右端までのマージン
#define		MAIN_WIN_BOTTOM_MARGIN	(160)	//メインイメージ表示領域からメインウィンドウ下端までのマージン

#define		DATA_SELECT_FIELD_Y		(0)		//データの表示タイプを決めるボタンを配置する領域の左上のｙ座標
#define		DATA_SELECT_FIELD_W		(25)		//データの表示タイプを決めるボタンを配置する領域の幅
#define		DATA_SELECT_FIELD_H		(100)		//データの表示タイプを決めるボタンを配置する領域の高さ

#define		LAYER_SELECT_FIELD_Y		(130)		//レイヤーの表示タイプを決めるボタンを配置する領域の左上のｙ座標
#define		LAYER_SELECT_FIELD_W		(25)		//レイヤーの表示タイプを決めるボタンを配置する領域の幅
#define		LAYER_SELECT_FIELD_H		(125)		//レイヤーの表示タイプを決めるボタンを配置する領域の高さ

#define		DATA_VALUE_DISP_FIELD_X		(0)			//データの値を表示する領域の左上のｘ座標
#define		DATA_VALUE_DISP_FIELD_W		(400)		//データの値を表示する領域の幅
#define		DATA_VALUE_DISP_FIELD_H		(80)		//データの値を表示する領域の高さ

#define		MESSAGE_DISP_FIELD_X		(270)		//メッセージを表示する領域の左上のｘ座標
#define		MESSAGE_DISP_FIELD_H		(150)		//メッセージを表示する領域の高さ

#define		DATA_NUMBER_DEFAULT		(100)	//画像メモリ数のデフォルト値
#define		DATA_NUMBER_MAX			(1000)	//画像メモリ数の最大値

#define		MARK_NUMBER_MAX_OLD		(256)	//マーカー最大値
#define		MARK_NUMBER_MAX			(2000)	//マーカー最大値

#define		CUSTOM_FUNC_MAX			(10)	//カスタムファンクション最大数

#define		FILE_IO_LOG_MAX	(10)

#define		WORKAREA_EDIT_MARGIN	(10)	//作業領域編集自につまんで操作する領域の幅

//データウィンドウ(DataDlg)
#define		DATA_WIN_W		(1150)				//データウィンドウの幅
#define		DATA_WIN_H		(200)				//データウィンドウの高さ

#define		SMALL_W		(40)					//小イメージの幅のデフォルト値
#define		SMALL_H		(30)					//小イメージの高さのデフォルト値
#define		SMALL_MARGN_X	(15)				//小イメージの横間隔
#define		SMALL_MARGN_Y	(20)				//小イメージの縦間隔
#define     SMALL_COLUM  (20)					//小イメージの表示行数

//ヒストグラムウインドウ(HistgramDlg)
#define		HISTGRAM_WIN_W		(340)			//ヒストグラムウインドウの幅
#define		HISTGRAM_WIN_H		(320)			//ヒストグラムウインドウの高さ

#define		HISTGRAM_W		(256)				//ヒストグラムの幅
#define		HISTGRAM_H		(150)				//ヒストグラムの高さ

//マークウインドウ(MarkDlg)
#define		MARK_WIN_W		(330)			//マークウインドウの幅
#define		MARK_WIN_H		(280)			//マークウインドウの高さ
#define		MARK_ATTRIB_NUM	(3)				//マークの属性数

//作業領域ウインドウ(WorkAreaDlg)
#define		WORKAREA_WIN_W		(330)			//ウインドウの幅
#define		WORKAREA_WIN_H		(280)			//ウインドウの高さ
#define		WORKAREA_FIG_NUM	(10)			//作業領図形数

//点郡表示
#define		POINT_CLOUD_W	(1000)		//点郡表示するスクリーンの幅
#define		POINT_CLOUD_H	(740)		//点郡表示するスクリーンの高さ


//フォントのサイズ
#define     FONT_SIZE       (11)

//通信
#define		RECIVE_BUFFER_SIZE		(10000)	//受信バッファサイズ
#define		CALLBACK_FUNC_NUM		(2)	//登録できるコールバック関数の最大数

//バッチ実行
#define		BATCH_IN_PARAM_NUM		(5)	//バッチ実行の入力パラメタ数
#define		BATCH_OUT_PARAM_NUM		(15)	//バッチ実行の出力パラメタ数

//グラフ表示
#define		CHART_NUM			(10)//グラフの数
#define		CHART_DATA_NUM		(10000)//1つのグラフ表示するデータ点の数
#define		CHART_HISTGRAM_SPLIT_MAX	(500)//ヒストグラム度数分割数

//数学的定数の設定//////////////////////////////////////////////////////////////

#define		PI			(3.141592653589793238)				//円周率

/////////////////////////////////////////////////////////////////////////////////
#define		CR		(0x0d)
#define		LF		(0x0a)

//フィルタ関連///////////////////////////////////////////////////////////////////
#define		COMB_FILTER_SIZE_MAX		(61)	//畳みこみフィルタの最大サイズ
#define		LANK_FILTER_SIZE_MAX		(21)	//順位付けフィルタの最大サイズ


#define		BINALIZE_LABEL_MAX			(9999)	//最大ラベル数