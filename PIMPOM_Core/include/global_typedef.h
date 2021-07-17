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
//PIMPOM全体で使用可能な構造体を定義する
/*******************************************************************/
#pragma once

// *************************************
//         構  造  体   定   義         
// *************************************

typedef struct {			/*  (x - x0)/dx = (y - y0)/dy = (z - z0)/dz  */
	double dx;
	double dy;
	double dz;
	double x0;
	double y0;
	double z0;
} Line1;


typedef struct{
	double	x;
	double	y;
	double	z;
}COOR_XYZ;

typedef struct{
	double	i;
	double	j;
}COOR_IJ;


typedef struct{
	int type_to;//変換後の画像メモリの型
	int rgb_to;//RGB型に変換する場合の入力先 （1:R成分 2:G成分 3:B成分 1～3以外:RGB全成分） 
	int comp_to;//複素数型の挿入先（0:実部　0以外:虚部）
	bool satulate;//型式の上下限を超える値を変換するとき飽和させるかどうか（0以外：飽和させる　0:オーバーフローさせる）
	int byte_from;//type_toがBYTE型のとき、表示メモリorマスクメモリから変換するかどうか（0:データから 1:表示メモリから 2:マスクから 3:疑似カラーから）
	int color_gray_method;//RGB->グレー変換方法 (1:彩度 2:色相  これら以外:明度)
	int  threeD_page;//3Dに変換するときのページ数
	int  threeD_offset;//3Dに変換するときのページオフセット
}CONVERT_DATA_OPTION;


typedef struct{
	float		x;				// 第１点Ｘ座標
	float		y;				// 第１点Ｙ座標
}FIGURE_POINT;

typedef struct{
	float		left;				// 第１点Ｘ座標
	float		top;				// 第１点Ｙ座標
	float		right;				// 第２点Ｘ座標
	float		bottom;				// 第２点Ｙ座標
}FIGURE_RECT;

typedef struct{
	float		x1;					// 第１点Ｘ座標
	float		y1;					// 第１点Ｙ座標
	float		x2;					// 第２点Ｘ座標
	float		y2;					// 第２点Ｙ座標
	float		w;					// 幅
}FIGURE_LINE;

typedef struct{
	float		cx;					// 楕円の中心点Ｘ座標
	float		cy;					// 楕円の中心点Ｙ座標
	float		rx;					// 楕円のＸ方向半径
	float		ry;					// 楕円のＸ方向半径
}FIGURE_ELLIPSE;


typedef struct {
	float		cx;					// 円の中心点Ｘ座標
	float		cy;					// 円の中心点Ｙ座標
	float		r;					// 円の半径
	float		w;					// 幅
}FIGURE_CIRCLE;

typedef struct{
	float		cx;					// 円弧の中心点Ｘ座標
	float		cy;					// 円弧の中心点Ｙ座標
	float		r;					// 円弧の半径
	float		start;				// 円弧の開始角度
	float		end;				// 円弧の開始角度
	float		w;					// 幅
}FIGURE_ARC;

typedef struct{
	int				count;				// 頂点の数
	FIGURE_POINT	point[10];			// 頂点の座標配列
}FIGURE_POLYGON;


typedef union{
	FIGURE_POINT	point;
	FIGURE_RECT		rect;//矩形作業領域
	FIGURE_LINE		line;//線状領域
	FIGURE_ELLIPSE	ellipse;//楕円状領域
	FIGURE_CIRCLE	circle;//円周状領域
	FIGURE_ARC		arc;//円弧状領域
	FIGURE_POLYGON	polygon;//多角形状領域
}FIGURE_UNION;



typedef struct {

	long			DataType;//データのタイプ
	char			DataName[1024];//データ名
	long			DataSizeX;//データのサイズ
	long			DataSizeY;//データのサイズ
	long			PageNumber;//データの奥行き
	long			ChannelNumber;//1画素のデータ数
	long			DispPage;//表示ページ
	long			DispChannel;//表示チャネル
	bool			DispRGB;//RGBカラー情報の表示

	long			MarkNum;//マーク数 ： MARK_NUMBER_MAXを入れること
	long			MarkX[MARK_NUMBER_MAX_OLD];//マークの座標：この設定は古いから使わないこと
	long			MarkY[MARK_NUMBER_MAX_OLD];//マークの座標：この設定は古いから使わないこと
	

	float			BrightMax;//表示濃度の最大値
	float			BrightMin;//表示濃度の最小値

	float			DispScale;//拡大率
	long			DataDispOffsetX;//画像転送元の左上の座標
	long			DataDispOffsetY;//画像転送元の左上の座標

	int				WorkAreaType[WORKAREA_FIG_NUM];//作業領域の図形タイプ
	bool			WorkAreaOrNot[WORKAREA_FIG_NUM];//作業領域OR/NOT種別
	FIGURE_UNION		WorkAreaFig[WORKAREA_FIG_NUM];//作業領域の図形
	//RECT			WorkArea;//作業領域
	//FIGURE_LINE		WorkAreaLine;
	//FIGURE_ELLIPSE	WorkAreaEllipse;
	//FIGURE_CIRCLE		WorkAreaCircle;
	//FIGURE_ARC		WorkAreaArc;
	//WORKAREA_POLYGON	WorkAreaPolygon;
	bool			DoUseWorkArea;//作業領域を使うかどうかを表すフラグ

	bool			DoUseMask;//マスクをかけるかどうかを表すフラグ


} DATA_UNIT_INFO;


//バッチ実行時の入力パラメタ
typedef struct{
	char	param[BATCH_IN_PARAM_NUM][1024];//入力パラメタ
	char	imae_file_path[FILE_PATH_LEN_MAX];//入力画像のファイル名
	int		image_number;//入力画像が入っているメモリ番号
	bool	AutoExecFlg;//連続実行中であることを示す
}BATCH_INPUT;


//バッチ実行時の出力パラメタ
typedef struct{
	char	result[BATCH_OUT_PARAM_NUM][1024];//出力パラメタ
	int		state; //0:通常処理 1:処理を飛ばした 2:処理をリトライ 3:強制終了
}BATCH_OUTPUT;
