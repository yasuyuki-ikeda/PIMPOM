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
//CImageField
//画像の画面表示を扱うクラス
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#pragma once
#include "DataUnit.h"

///*************************************
//         マ  ク  ロ   定   義         
///*************************************
#define		NO_SIZE_DATA	( CSize(-1,-1) )
#define		NO_POINT_DATA	( CPoint(-1,-1) )
#define		NO_RECT_DATA	( CRect(-1,-1,-1,-1) )

///*************************************
//         クラス   定   義         
///*************************************
class PIMPOM_EXT CImageField
{
public:
	CImageField();
	CImageField(HWND wnd, CSize field_size, CPoint field_coor);//コンストラクタ
	virtual ~CImageField();//デストラクタ
	void DrawImage(CDataUnit *p_du, bool redraw_background);//描画する
	void DrawPiece(CDataUnit *p_du, CPoint src_coor, CSize src_size, bool do_adjust_scale_as_field_size , bool do_update_scale, bool do_redraw_background);//画像データの一部分を描画する
	void DrawAdjustedImage(CDataUnit *p_du , bool do_update_scale);//ImageFiledの大きさに合わせて拡大・縮小して描画する
	void ScaleImage(CDataUnit *p_du , float scale);//画像転送時の拡大率を変更する
	void ScrollImage(CDataUnit *p_du , CPoint poi);//画像転送元の矩形領域をスクロールする
	bool GetSrcOffset(CDataUnit *p_du, CPoint *p_offset);//画像転送元のオフセット（矩形領域の左上の座標）を得る
	bool GetSrcSize(CDataUnit *p_du, CSize *p_src_size);//画像転送元の矩形領域のサイズを取得する
	bool GetSrcRect(CDataUnit *p_du, CRect *p_src_rect);//画像転送元の矩形領域を取得する
	bool GetDestSize(CDataUnit *p_du, CSize *p_dest_size);//画像転送先の矩形領域のサイズを取得する
	bool GetDestRect(CDataUnit *p_du, CRect *p_dest_rect);//画像転送先の矩形領域を取得する
	CRect GetFieldRect();//このImageFieldをあらわす矩形を得る
	bool GetSrcCoor(CDataUnit *p_du ,  double dest_coor_x, double dest_coor_y , double *p_src_coor_x, double *p_src_coor_y);//画像転送先（ウインドウ内）の座標から転送元（画像データ内）の座標に変換する
	bool GetDestCoor(CDataUnit *p_du , double src_coor_x ,double src_coor_y , double *p_dest_coor_x, double *p_dest_coor_y);//画像転送元（画像データ内）の座標から転送先（ウインドウ内）の座標に変換する
	bool Resize(CSize size);
	bool Move(CPoint point);

	HWND			hImageWnd;//画像を表示するウインドウのハンドル
	CSize			FieldSize;//表示領域のサイズ
	CPoint			FieldCoordinate;//表示領域の左上の座標

private:
	void fill_field(COLORREF color);//ImageFiled内を単一色で塗りつぶす

};

