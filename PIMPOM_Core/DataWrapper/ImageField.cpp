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
#include "stdafx.h"
#include "PIMPOM_API.h"

// *************************************
//         マ  ク  ロ   定   義         
// *************************************
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

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

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
//標準のコンストラクタ
CImageField::CImageField()
{

}

/********************************************************************
機  能  名  称 : コンストラクタ
関    数    名 : CImageField
引          数 : HWND			wnd			(in)画像表示するウィンドウのハンドル
				 CSize			field_size	(in)ImageFieldのサイズ
				 CPoint			field_coor	(in)ImageFieldのウィンドウ内の位置
戻    り    値 : 
機          能 : CImageFieldクラスの構築
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
CImageField::CImageField(HWND wnd, CSize field_size, CPoint field_coor)
{
	hImageWnd = wnd;
	FieldSize = field_size;
	FieldCoordinate = field_coor;
}

//デストラクタ
CImageField::~CImageField()
{

}


/********************************************************************
機  能  名  称 : リサイズ
関    数    名 : Resize
引          数 : CSize size					新しいサイズ
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool CImageField::Resize(CSize size)
{
	FieldSize = size;

	return true;
}


/********************************************************************
機  能  名  称 : 移動
関    数    名 : Move
引          数 : CPoint point					新しい左上座標
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool CImageField::Move(CPoint point)
{
	FieldCoordinate	= point;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//描画関数
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
機  能  名  称 : 画像を表示する
関    数    名 : DrawImage
引          数 : CDataUnit		*p_du					(in)画像メモリ
				 bool			do_redraw_background	(in)背景を再描画するかどうか
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CImageField::DrawImage(CDataUnit *p_du, bool do_redraw_background)
{
	CSize	src_size;

		if( !p_du ){
			fill_field( RGB(0,0,0) );
			return;//データがないときは領域を黒く塗りつぶす
		}

		if( GetSrcSize(p_du, &src_size) ){
			DrawPiece(p_du, p_du->DataDispOffset, src_size , false , false , do_redraw_background);
		}
}

/********************************************************************
機  能  名  称 : 画像の一部分を表示する
関    数    名 : DrawPiece
引          数 : CDataUnit		*p_du							(in)画像メモリ
				 CPoint			src_coor						(in)画像メモリの描画領域の左上座標
				 CSize			src_size						(in)画像メモリの描画領域の高さと幅
				 bool			do_adjust_scale_as_field_size	(in) true　にすると，縦横比をImageFiledの大きさにあわして表示する
																	false にすると，指定したDataUnit (*p_du) のスケーリング情報で表示する
				 bool			do_update_scale					(in)do_adjust_scale_as_field_sizeがtrueのとき、
																	p_duのスケーリング情報を更新するかどうか
				 bool			do_redraw_background			(in)背景を再描画するかどうか
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CImageField::DrawPiece(CDataUnit *p_du, CPoint src_coor, CSize src_size, bool do_adjust_scale_as_field_size, bool do_update_scale, bool do_redraw_background)
{
	float	src_aspect_ratio, dest_aspect_ratio, scale;
	CSize	dest_size;
	CPoint	src_coor_dib;//DIB形式にした時の画像転送元の座標

		if(p_du == NULL){
			fill_field( RGB(0,0,0) );
			return;//データがないときはImageFiled内を黒く塗りつぶす
		}

	//画像の縦横比調整
		if( do_adjust_scale_as_field_size ){//ImageFieldの大きさにあわす
			src_aspect_ratio  = (float)src_size.cx / (float)src_size.cy;//画像転送元矩形領域の	　幅/高さ
			dest_aspect_ratio = (float)FieldSize.cx / (float)FieldSize.cy;//ImageFieldの　幅/高さ

				if(  src_aspect_ratio == dest_aspect_ratio ){//画像転送元矩形領域とImageFieldの　幅/高さ が同じ場合
					scale = (float)FieldSize.cx / (float)src_size.cx ;
					dest_size = FieldSize;

				}else  if(  src_aspect_ratio > dest_aspect_ratio  ){//画像転送元領域のほうがImageFieldよりも横長である→拡大後のデータの幅をImageFieldの幅にあわせる
					scale = (float)FieldSize.cx / (float)src_size.cx ;
					dest_size = CSize( FieldSize.cx , (int)(FieldSize.cx / src_aspect_ratio) );

				}else{//画像転送元領域のほうがImageFieldよりも縦長である→拡大後のデータの高さをImageFiledの高さにあわせる
					scale = (float)FieldSize.cy / (float)src_size.cy ;
					dest_size = CSize( (int)(FieldSize.cy * src_aspect_ratio) , FieldSize.cy );
				}

				if(do_update_scale)		ScaleImage(p_du,scale);//p_du->SetDispScale(scale);

		}else{//DataUnit (*p_du) のスケーリング情報にあわす
			 GetDestSize(p_du, &dest_size);
		}

	//DIBの座標に変換
	src_coor_dib = CPoint( src_coor.x ,  p_du->DataSize.cy - src_size.cy - src_coor.y);

	//背景表示
		if( do_redraw_background )
			if( dest_size.cx < FieldSize.cx   ||   dest_size.cy < FieldSize.cy  ){
				//画像転送先の矩形領域がImageFiledより狭いとき，背景を塗りつぶす
				fill_field( API.default_bg_color , &dest_size);
			}

	//画像転送
	HDC	hdc = ::GetDC( hImageWnd );
//		if(scale < 1)//縮小するとき
			::SetStretchBltMode( hdc, HALFTONE);//中間色で補間する
	
			if((p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) || p_du->DispRGB == false){
				::StretchDIBits( hdc,	FieldCoordinate.x , FieldCoordinate.y , dest_size.cx , dest_size.cy ,
							src_coor_dib.x ,	 src_coor_dib.y , src_size.cx , src_size.cy,
							p_du->pDispBuffer,  p_du->pDispBufferBmpInfo,  DIB_RGB_COLORS,  SRCCOPY);

			}else{//RGB
				::StretchDIBits( hdc,	FieldCoordinate.x , FieldCoordinate.y , dest_size.cx , dest_size.cy ,
							src_coor_dib.x ,	 src_coor_dib.y , src_size.cx , src_size.cy,
							p_du->pDispBufferRGB,  p_du->pDispBufferBmpInfoRGB,  DIB_RGB_COLORS,  SRCCOPY);
			}
	::ReleaseDC(hImageWnd, hdc);
}

/********************************************************************
機  能  名  称 : ImageFiledの大きさに合わせて画像を表示する
関    数    名 : DrawAdjustedImage
引          数 : CDataUnit		*p_du					(in)画像メモリ
				 bool			do_update_scale			(in) p_duのスケーリング情報を更新するかどうか
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CImageField::DrawAdjustedImage(CDataUnit *p_du, bool do_update_scale)
{
		if(p_du == NULL){
			fill_field( RGB(0,0,0) );
			return;//データがないときは領域を黒く塗りつぶす
		}

	DrawPiece( p_du, CPoint(0,0), p_du->DataSize, true, do_update_scale, true);
}

/********************************************************************
機  能  名  称 : ImageFiled内を単一色で塗りつぶす
関    数    名 : fill_field
引          数 : COLORREF		color 					(in)塗りつぶす色
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CImageField::fill_field( COLORREF color ,CSize* pDrawImgSize)
{
	CBrush	brush( color );
	CPen	pen(PS_SOLID,1,color);
	HDC		hdc = ::GetDC( hImageWnd );
	if(hdc==NULL)	return;

	HBRUSH	old_brush = (HBRUSH)::SelectObject(hdc, (HBRUSH)brush );
	HPEN old_pen = (HPEN)::SelectObject(hdc, (HPEN)pen);
	CRect	area = GetFieldRect();
	if (pDrawImgSize)
	{// ImageFiled内から描画する画像サイズ分を除いたもの
		if (area.Height() > pDrawImgSize->cy) 
		{
			::Rectangle(hdc, area.left, area.top + pDrawImgSize->cy, area.right, area.bottom);

			if (area.Width() > pDrawImgSize->cx)
			{
				::Rectangle(hdc, area.left + pDrawImgSize->cx, area.top, area.right, area.top + pDrawImgSize->cy);
			}
		}
		else {
			if (area.Width() > pDrawImgSize->cx) 
			{
				::Rectangle(hdc, area.left + pDrawImgSize->cx, area.top, area.right, area.bottom);
			}
		}
	}
	else
	{// ImageFiled内全域
		::Rectangle(hdc, area.left, area.top, area.right, area.bottom);
	}
	::SelectObject(hdc, old_brush );
	::SelectObject(hdc, old_pen);
	ReleaseDC( hImageWnd, hdc );
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//表示方式の変更
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
機  能  名  称 : 画像表示の拡大率を変更する
関    数    名 : ScaleImage
引          数 : CDataUnit		*p_du			(in)画像メモリ
				 float			scale			(in)拡大率
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
			 Y.Ikeda		 現在表示されている中心座標を保ったまま拡大率変更
********************************************************************/
void CImageField::ScaleImage(CDataUnit *p_du , float scale)
{
		if( !p_du )	return;//データが無ければなにもしない


	//現在の表示中心座標＝新しく表示する中心座標でもある
	double dispCenterX, dispCenterY;
	GetSrcCoor(p_du, FieldSize.cx/2 + FieldCoordinate.x, FieldSize.cy/2 + FieldCoordinate.y, &dispCenterX, &dispCenterY);

	//新しく表示する4済座標
	double newDispLeft = dispCenterX - fabs(dispCenterX - p_du->DataDispOffset.x)*p_du->DispScale /scale ;
	double newDispTop = dispCenterY - fabs(dispCenterY - p_du->DataDispOffset.y)*p_du->DispScale /scale ;
	double newDispRight = dispCenterX + fabs(dispCenterX - p_du->DataDispOffset.x)*p_du->DispScale /scale ;
	double newDispBottom = dispCenterY + fabs(dispCenterY - p_du->DataDispOffset.y)*p_du->DispScale /scale ;

	//表示オフセット座標の設定
	if( newDispLeft>=0 && newDispRight < p_du->DataSize.cx){
		p_du->DataDispOffset.x = newDispLeft;
	}else{
		p_du->DataDispOffset.x = 0;
	}

	if( newDispTop>=0 && newDispBottom < p_du->DataSize.cy){
		p_du->DataDispOffset.y = newDispTop;
	}else{
		p_du->DataDispOffset.y = 0;
	}

	p_du->DispScale = scale;
}

/********************************************************************
機  能  名  称 : 画像の表示範囲を移動する
関    数    名 : ScrollImage
引          数 : CDataUnit		*p_du			(in)画像メモリ
				 CPoint			poi				(in)画像の表示範囲の左上座標
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CImageField::ScrollImage(CDataUnit *p_du , CPoint poi)
{
	CSize	src_size;
		if( !p_du )	return;//データが無ければなにもしない
		
	GetSrcSize(p_du , &src_size);
		if(poi.x < 0   ||   poi.x > p_du->DataSize.cx - src_size.cx )			return;
		if(poi.y < 0   ||   poi.y > p_du->DataSize.cy - src_size.cy )			return;

	p_du->DataDispOffset = poi;
	//DrawImage(p_du, false);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//表示に関するデータ取得のための関数
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
機  能  名  称 : 画像の表示範囲の左上座標を得る
関    数    名 : GetSrcOffset
引          数 : CDataUnit		*p_du			(in)画像メモリ
				 CPoint			*p_offset		(out)画像の表示範囲の左上座標
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CImageField::GetSrcOffset(CDataUnit *p_du, CPoint *p_offset)
{
		if( !p_du )	return false;//データが無ければ false が返る

	*p_offset = p_du->DataDispOffset;
	return	true;
}

/********************************************************************
機  能  名  称 : 画像の表示範囲のサイズを得る
関    数    名 : GetSrcSize
引          数 : CDataUnit		*p_du			(in)画像メモリ
				 CSize			*p_src_size		(out)画像の表示範囲のサイズ
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CImageField::GetSrcSize(CDataUnit *p_du, CSize *p_src_size)
{
	CSize	result;

		if( !p_du )		return false;//データが無ければfalseをかえす

		if( p_du->DataSize.cx * p_du->DispScale  <  FieldSize.cx ){//拡大後の画像データの幅がImageFieldの幅より小さい→画像全体の幅を表示できる
			result.cx = p_du->DataSize.cx;

		}else{//拡大後のデータの幅が表示領域の幅より大きい→画像の一部を表示
			result.cx = (long)(FieldSize.cx / p_du->DispScale);
		}

		if( p_du->DataSize.cy * p_du->DispScale  <  FieldSize.cy ){//拡大後の画像データの高さがImageFieldの高さより小さい→画像全体の高さを表示できる
			result.cy = p_du->DataSize.cy;

		}else{//拡大後のデータの高さが表示領域の高さより大きい→画像の一部を表示
			result.cy = (long)(FieldSize.cy / p_du->DispScale);
		}

	*p_src_size = result;
	return	true;
}

/********************************************************************
機  能  名  称 : 画像の表示範囲の矩形を得る
関    数    名 : GetSrcSize
引          数 : CDataUnit		*p_du			(in)画像メモリ
				 CRect			*p_src_rect		(out)画像の表示範囲の矩形
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CImageField::GetSrcRect(CDataUnit *p_du, CRect *p_src_rect)
{
	CRect	result;
	CSize	src_size;
		if( !p_du )		return	false;//データが無ければfalseをかえす

	GetSrcSize(p_du, &src_size);
	result.left = p_du->DataDispOffset.x;
	result.top  = p_du->DataDispOffset.y;
	result.right = p_du->DataDispOffset.x + src_size.cx - 1;
	result.bottom = p_du->DataDispOffset.y + src_size.cy - 1;

	*p_src_rect = result;
	return	true;
}

/********************************************************************
機  能  名  称 : ImageField内の画像表示領域のサイズを得る
関    数    名 : GetDestSize
引          数 : CDataUnit		*p_du				(in)画像メモリ
				 CSize			*p_dest_size		(out)ImageFieldの画像表示領域のサイズ
戻    り    値 : 成功したらtrue
機          能 : 「画像メモリのサイズ×拡大率＞＝ImageFieldのサイズ」のときは、ImageFieldのサイズを返す
				 それ以外は、「画像メモリのサイズ×拡大率のサイズ」を返す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CImageField::GetDestSize(CDataUnit *p_du, CSize *p_dest_size)
{
	CSize	result, scaled_src_size;

		if( !p_du )		return false;//データが無ければfalseをかえす

	scaled_src_size.cx = (long)(p_du->DataSize.cx * p_du->DispScale);
		if(  scaled_src_size.cx <  FieldSize.cx ){//拡大後の画像データの幅が表示領域の幅より小さい→ImageFieldの一部に表示
			result.cx = scaled_src_size.cx;

		}else{//拡大後の画像データの幅が表示領域の幅より大きい→ImageFieldの幅全体に表示
			result.cx = FieldSize.cx;
		}

	scaled_src_size.cy = (long)(p_du->DataSize.cy * p_du->DispScale);
		if(  scaled_src_size.cy <  FieldSize.cy ){//拡大後の画像データの高さが表示領域の高さより小さい→ImageFieldの一部に表示
			result.cy = scaled_src_size.cy;

		}else{//拡大後の画像データの高さが表示領域の高さより大きい→ImageFieldの高さ全体に表示
			result.cy = FieldSize.cy;
		}

	*p_dest_size = result;
	return	true;
}

/********************************************************************
機  能  名  称 : ImageField内の画像表示領域の矩形を得る
関    数    名 : GetDestRect
引          数 : CDataUnit		*p_du				(in)画像メモリ
				 CRect			*p_dest_rect		(out)ImageField内の画像表示領域の矩形
戻    り    値 : 成功したらtrue
機          能 :「 画像メモリのサイズ×拡大率＞＝ImageFieldのサイズ」のときは、ImageFieldの矩形を返す
				それ以外のときは、左上座標がImageFieldと同じでサイズが「画像メモリのサイズ×拡大率」である矩形を返す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CImageField::GetDestRect(CDataUnit *p_du, CRect *p_dest_rect)
{
	CRect	dest_rect;
	CSize	dest_size;

		if( !p_du )		return	false;//データが無ければfalseをかえす

	GetDestSize(p_du, &dest_size);//画像転送先の矩形領域のサイズを取得する
	dest_rect.left = FieldCoordinate.x;
	dest_rect.top = FieldCoordinate.y;
	dest_rect.right = FieldCoordinate.x + dest_size.cx - 1;
	dest_rect.bottom  = FieldCoordinate.y + dest_size.cy - 1;

	*p_dest_rect = dest_rect;
	return	true;
}

/********************************************************************
機  能  名  称 : ImageFieldの矩形を得る
関    数    名 : GetFieldRect
引          数 : 
戻    り    値 : ImageFieldの矩形
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
CRect	CImageField::GetFieldRect()
{
	return	CRect(	FieldCoordinate.x ,						FieldCoordinate.y ,
					FieldCoordinate.x + FieldSize.cx -1 ,	FieldCoordinate.y + FieldSize.cy - 1 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//画像転送前後の座標変換に関する関数
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
機  能  名  称 : ImageField内の座標から画像内の座標に変換する
関    数    名 : GetSrcCoor
引          数 : CDataUnit		*p_du				(in)画像メモリ
				 double		dest_coor_x, dest_coor_y	(in)メイン画面の座標系
				 double		*p_src_coor_x, *p_src_coor_y	(out)画像座標
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CImageField::GetSrcCoor(CDataUnit *p_du,  double dest_coor_x, double dest_coor_y , double *p_src_coor_x, double *p_src_coor_y)
{
		if( !p_du )	return false;//データが無ければfalseをかえす

	*p_src_coor_x = (double)( dest_coor_x  -  FieldCoordinate.x ) / p_du->DispScale  + p_du->DataDispOffset.x - 0.5;
	*p_src_coor_y = (double)( dest_coor_y  -  FieldCoordinate.y ) / p_du->DispScale  + p_du->DataDispOffset.y - 0.5;


		//画像転送元の領域内ではない
		if(*p_src_coor_x<-0.5  ||  *p_src_coor_x>(double)p_du->DataSize.cx-0.5)		return	false;
		if(*p_src_coor_y<-0.5  ||  *p_src_coor_y>(double)p_du->DataSize.cy-0.5)		return	false;
	
	return	true;
}

/********************************************************************
機  能  名  称 : 画像内の座標からImageField内の座標に変換する
関    数    名 : GetDestCoor
引          数 : CDataUnit		*p_du				(in)画像メモリ
				 double		src_coor_x, src_coor_y		(in)画像座標
				 double		*p_dest_coor_x, *p_dest_coor_y	(out)メイン画面の座標系
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CImageField::GetDestCoor(CDataUnit *p_du, double src_coor_x , double src_coor_y , double *p_dest_coor_x, double *p_dest_coor_y)
{
		if( !p_du )	return false;//データが無ければfalseをかえす

	*p_dest_coor_x = (double)(src_coor_x + 0.5 - p_du->DataDispOffset.x) * p_du->DispScale  +  FieldCoordinate.x;
	*p_dest_coor_y = (double)(src_coor_y + 0.5 - p_du->DataDispOffset.y) * p_du->DispScale  +  FieldCoordinate.y;

		//画像転送先の領域内ではない
		if(*p_dest_coor_x< (double)FieldCoordinate.x-0.5  ||  *p_dest_coor_x > (double)(FieldCoordinate.x + FieldSize.cx)-0.5 )		return	false;
		if(*p_dest_coor_y< (double)FieldCoordinate.y-0.5  ||  *p_dest_coor_y > (double)(FieldCoordinate.y + FieldSize.cy)-0.5 )		return	false;
	
	return	true;
}

