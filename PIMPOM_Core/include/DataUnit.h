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
//CDataUnit
//画像データを統合的に扱うクラス
/*******************************************************************/
#pragma once

//**************************************
//         インクルードファイル         
//**************************************
#include "global_define.h"
#include "global_typedef.h"

///*************************************
//         クラス   定   義         
///*************************************
class AFX_EXT_CLASS CWorkArea
{
public:
	int					Type[WORKAREA_FIG_NUM];//図形のタイプ
	bool				OrNot[WORKAREA_FIG_NUM];
	FIGURE_UNION		Figure[WORKAREA_FIG_NUM];//図形
	int					FigureEditMode[WORKAREA_FIG_NUM];//図形編集モード 0:見初期化 1:全体移動  2以降:頂点移動

	void			Initialize(CSize image_size);
	void			Initialize(CSize image_size, int no, int type, bool ornot);//作業領域を初期化する
	bool			PtInWorkArea(CPoint point);//指定された座標が作業領域内に入っているかチェックする
	bool			PtInWorkArea(CPoint point, int no);//指定された座標が指定された番号の作業領域内に入っているかチェックする
	void			Copy(CWorkArea *area);//作業領域をコピーする
	CRect			GetCircumRect();//全領域に外接する矩形を求める
	CRect			GetCircumRect(int no);//指定された番号の作用領域に外接する矩形を求める
	bool			Move(int dx, int dy);//すべての作業領域を平行移動させる
	bool			Move(int no, int dx, int dy);//指定された番号の作業領域を平行移動させる
	bool			Scale(int no, float scalex, float scaley, CPoint offset);//スケーリングする
	static void		GetLineVertxes(FIGURE_LINE line, CPoint point[4]);//幅つき直線の４頂点を求める

	int				GetWorkArea(int no, FIGURE_UNION *fig, bool *ornot);
	bool			SetWorkArea(int no, RECT rect, bool ornot);
	bool			SetWorkArea(int no, FIGURE_LINE line, bool ornot);
	bool			SetWorkArea(int no, FIGURE_ELLIPSE ellipse, bool ornot);
	bool			SetWorkArea(int no, FIGURE_CIRCLE circle, bool ornot);
	bool			SetWorkArea(int no, FIGURE_ARC arc, bool ornot);


};


class AFX_EXT_CLASS CMark
{
public:
	short			type;
	FIGURE_UNION	figure;
	//float	x;//X座標
	//float	y;//Y座標
	double	attrib[MARK_ATTRIB_NUM];//属性


	//コンストラクタ
	CMark();
	CMark(float x, float y);
	CMark(float x1, float y1, float x2, float y2);
	virtual ~CMark();//デストラクタ
};


class AFX_EXT_CLASS CDataUnit  
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//変数
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	///データに関する変数
	long			DataType;//画像データの型
	CString			DataName;//データ名
	CString			SrcFilePath;//ファイルパス(ファイルから読み込まれた画像に対して)
	CSize			DataSize;//画像データの高さ／幅
	BYTE			*pByteData;//画像データ（8ビット整数型）の先頭ポインタ
	short			*pShortData;//画像データ（符号あり16ビット整数）の先頭ポインタ
	WORD			*pWordData;//画像データ（符号なし16ビット整数）の先頭ポインタ
	long			*pLongData;//画像データ（符号あり32ビット整数)の先頭ポインタ
	DWORD			*pDwordData;//画像データ（符号なし32ビット整数)の先頭ポインタ
	float			*pFloatData;//画像データ（32ビット浮動小数点）の先頭ポインタ
	long			PageNumber;//画像データのページ数
	long			ChannelNumber;//画像データのチャネル数
	float			Average;//画像データの平均
	float			Stdiv;//画像データの標準偏差
	bool			complex_image_power_log;//複素数型データのパワースペクトルにlogをとる

	///表示画像に関する変数
	BYTE			*pDispBuffer;//表示画像（8bitモノクロ）
	BYTE			*pDispBufferRGB;//表示画像（24bitカラー）
	BITMAPINFO		*pDispBufferBmpInfo;//表示画像(8bit)のビットマップ情報
	BITMAPINFO		*pDispBufferBmpInfoRGB;//表示画像(24bit)のビットマップの情報
	long			DispPage;//表示ページ
	long			DispChannel;//表示チャネル
	bool			DispRGB;//RGBカラーの表示
	long			DispBufferRow;//表示画像(8bit)のビット列幅（必ず４の倍数でなくてはならない）
	long			DispBufferRowRGB;//表示画像(24bit)のビット列幅（必ず４の倍数でなくてはならない）
	float			BrightMax;//表示濃度の最大値
	float			BrightMin;//表示濃度の最小値
	float			DispScale;//拡大率
	CPoint			DataDispOffset;//画像の左上の座標
	long			BrightLevel[256];//表示濃度レベルの分布

	///作業領域に関する変数
	CWorkArea		WorkArea;//作業領域
	bool			DoUseWorkArea;//作業領域使用フラグ

	///マスクに関する変数
	BYTE			*pMaskData;	//マスクデータ			マスクをかける : MASKED_PIXEL	 		マスクをかけない : UNMASKED_PIXEL
	bool			DoUseMask;//マスク使用フラグ

	//マークに関する変数
	long			MarkNum;//マーク数
	CMark			*pMarkMap[MARK_NUMBER_MAX];//マークのポインタ


protected:
	void set_bmp_info( CSize size );//BITMAPINFOを初期化する



public:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//関数
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//生成・破棄
	CDataUnit();
	CDataUnit(long type, CSize size, long page);//コンストラクタ
	virtual ~CDataUnit();//デストラクタ
	void Destroy();
	bool Format(long type, CSize size, long page);


	//コピー・比較
	bool Copy(CDataUnit *p_src_du);
	bool Copy(CDataUnit *p_src_du, bool do_copy_data, bool do_copy_mask, bool do_copy_workarea,
					 bool do_copy_range, bool do_copy_mark, bool do_copy_pos_scale);//画像データをコピーする
	bool Compare( CDataUnit *p_du );//画像データの型とサイズを比較する (同じならtrueを返す　異なればfalseを返す)
	bool Compare(long type, CSize size, long page);
	


	//データの取得・設定
	unsigned char	GetDisplayIntensity(CPoint point);//指定した画素の表示濃度を取得する
	double			GetDataValue(CPoint point, long page=-1, int channel=-1);//指定した座標の画像データ値を取得する
	double			GetDataValue(double x, double y, long page=-1, int channel=-1);//指定した座標の画像データ値を取得する
	BYTE			GetMaskValue(CPoint point);////指定した座標のマスク値を取得する
	bool			SetDataValue(int x, int y, double data, long page, int channel, bool satulate=true);//指定された座標の画像データ値を設定する
	bool			SetMaskValue(int x, int y, BYTE mask);//指定された座標のマスク値を設定する
	void			ClearMask();//マスククリア
	void			InitWorkArea();//作業領域の初期化
	bool			SetMaskByWorkArea();//作業領域外部にマスクをかける
	CRect			GetRectArea();//作業領域に外接する矩形領域を取得する
	void			CreateComplexPolarComponents();//直交座標成分（実部，虚部）から極座標成分の（パワー，位相）を求める（複素数型データのための処理）
	void			CreateComplexDescartesComponents();//極座標成分から直交座標成分を求める（複素数型データのための処理）
	double			GetMaxLimitValue();//データの上限値
	double			GetMinLimitValue();//データの下限値
	bool			Convert(CONVERT_DATA_OPTION option);//データの型を変換する
	int				AddMark(CMark mark);
	bool			DeleteMark(long idx);//マークを削除する
	void			ClearMark();//マークを全削除する
	bool			GetMark(long idx, CMark *pmark);//マークの座標を取得する
	long			GetMarkNumber();//マークの数を取得する
	bool			SetMark(long idx, CMark mark);//

	//画像表示
	void			UpdateDispBuffer();//表示画像を更新する
	void			GetDataMinMax(float *min, float *max);//画像データの最大／最小を取得
	bool			SetDispRange(float min, float max);//表示濃度の最大／最少を指定する
	void			SetDispScale(float scale);//拡大率を変更する
	bool			MoveDispPage(int offset);//表示ページを現在から指定量だけ移動させる
	bool			SetDispPage(int page);//表示ページを設定する
	bool			SetDispChannel(int channel);//表示チャネルを設定する
	void			ChangePalette(short palette_id);//カラーパレットを変更する


};

