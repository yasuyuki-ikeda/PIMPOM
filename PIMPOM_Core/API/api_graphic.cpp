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
//メイン画面への描画
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

//領域の水平方向の辺 (線分 s1s2) と 線分 p1p2 の交点を求める
//交点が無ければ　falseを返す
static bool	get_closs_secion_with_h_side(double s1x, //(in)点s1のx座標
										 double s1y, //(in)点s1のy座標
										double s2x, //(in)点s2のx座標
										double s2y, //(in)点s2のy座標
										double p1x, //(in)点p1のx座標
										double p1y, //(in)点p1のy座標
										double p2x, //(in)点p2のx座標
										double p2y, //(in)点p2のy座標
										double *pCx, //(out)交点のx座標
										double *pCy  //(out)交点のy座標
										)
{
	double	cross_section_xf;//*交点のx座標
	double	h_side_yf;//水平方向の辺 (線分 s1s2) のy座標 ＝ 交点のy座標;

	h_side_yf = s1y;//水平方向の辺 (線分 s1s2) のy座標

	//p1,p2が辺を延長した直線に対して同じ側にある　＝　水平方向の辺(線分 s1s2)と線分p1p2は交わらない
	if(  (p1y - h_side_yf) * (p2y - h_side_yf) > 0 )
	{//交点なし
		return false;
	}
		
	//交点のx座標を求める
	cross_section_xf = ( p2x - p1x ) * ( h_side_yf -  p1y ) / ( p2y - p1y ) + p1x;

	//s1,s2がp1p2を延長した直線に対し、同じ側にる　＝　水平方向の辺(線分 s1s2)と線分p1p2は交わらない
	if( (s1x - cross_section_xf) * (s2x - cross_section_xf) > 0)
	{
		return false;
	}


	*pCx = cross_section_xf;
	*pCy = h_side_yf;

	return true;
}


//領域の垂直方向の辺 (線分 s1s2) と 線分 p1p2 の交点を求める
//交点が無ければ　falseを返す
static bool	get_closs_secion_with_v_side(double s1x, //(in)点s1のx座標
										 double s1y, //(in)点s1のy座標
										double s2x, //(in)点s2のx座標
										double s2y, //(in)点s2のy座標
										double p1x, //(in)点p1のx座標
										double p1y, //(in)点p1のy座標
										double p2x, //(in)点p2のx座標
										double p2y, //(in)点p2のy座標
										double *pCx, //(out)交点のx座標
										double *pCy  //(out)交点のy座標
										)
{
	double	cross_section_yf;//*交点のy座標
	double	h_side_xf;//垂直方向の辺 (線分 s1s2) のx座標 ＝ 交点のy座標;

	h_side_xf = s1x;//垂直方向の辺 (線分 s1s2) のx座標

	//p1,p2が辺を延長した直線に対して同じ側にある　＝　垂直方向の辺(線分 s1s2)と線分p1p2は交わらない
	if(  (p1x - h_side_xf) * (p2x - h_side_xf) > 0 )
	{
		return false;
	}
		
	//交点のy座標を求める
	cross_section_yf = ( p2y - p1y ) * ( h_side_xf -  p1x ) / ( p2x - p1x ) + p1y;

	//s1,s2がp1p2を延長した直線に対し、同じ側にる　＝　水平方向の辺(線分 s1s2)と線分p1p2は交わらない
	if( (s1y - cross_section_yf) * (s2y - cross_section_yf) > 0)
	{
		return false;
	}

	*pCx = h_side_xf;
	*pCy = cross_section_yf;

	return true;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//画像表示の座標に関する動作
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
機  能  名  称 : メイン画面の座標系から画像座標に変換する
関    数    名 : GetSrcCoor
引          数 : double		dest_coor_x, dest_coor_y	(in)メイン画面の座標系
				 double		*p_src_coor_x, *p_src_coor_y	(out)画像座標
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::GetSrcCoor( double dest_coor_x, double dest_coor_y , double *p_src_coor_x, double *p_src_coor_y)
{
	CDataUnit	*p_du = GetDataUnit( CURRENT_IMAGE );
	CImageField	*p_image = pPIMMOMDlg->GetMainImage();

		if(!p_du)		return	false;
		if(!p_image)	return	false;

	return	p_image->GetSrcCoor( p_du, dest_coor_x, dest_coor_y, p_src_coor_y, p_src_coor_y);
}


/********************************************************************
機  能  名  称 : 画像座標からメイン画面の座標系に変換する
関    数    名 : GetDestCoor
引          数 : double		src_coor_x, src_coor_y		(in)画像座標
				 double		*p_dest_coor_x, *p_dest_coor_y	(out)メイン画面の座標系
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
//画像転送前の座標（画像内）から画像転送後（ウインドウ内）の座標に変換する
bool	CPimpomAPI::GetDestCoor(double src_coor_x , double src_coor_y , double *p_dest_coor_x, double *p_dest_coor_y)
{
	CDataUnit	*p_du = GetDataUnit( CURRENT_IMAGE );
	CImageField	*p_image = pPIMMOMDlg->GetMainImage();

		if(!p_du)		return	false;
		if(!p_image)	return	false;

	return	p_image->GetDestCoor( p_du, src_coor_x, src_coor_y, p_dest_coor_x, p_dest_coor_y);
}


///////////////////////////////////////////////////////////////////////////////////////
//画像表示関連
///////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
機  能  名  称 : 表示画像の濃度のレンジを設定してから画像表示
関    数    名 : SetDispRange
引          数 : long		number		(in)画像メモリ番号
				float		min			(in)濃度最小値
				float		max			(in)濃度最大値
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool		CPimpomAPI::SetDispRange(long number, float min , float max)
{
		if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合

	CDataUnit	*p_du = GetDataUnit(number);
		if(p_du == NULL)	return	false;//データが無いときはなにもしない

		if( !p_du->SetDispRange( min, max) )	return	false;//表示レンジの変更に失敗

	DrawImage( number, true);
	return	true;
}


/********************************************************************
機  能  名  称 : 表示画像の濃度のレンジを自動で設定してから画像表示
関    数    名 : SetDispRangeAuto
引          数 : long		number		(in)画像メモリ番号
				int			type		(in)自動設定タイプ   0:最大/最小 1:平均値±標準偏差×2
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool		CPimpomAPI::SetDispRangeAuto(long number, int type)
{
		if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合

	CDataUnit	*p_du = GetDataUnit(number);
		if(p_du == NULL)	return	false;//データが無いときはなにもしない

	float min, max;
	if(type==0){
		p_du->GetDataMinMax(&min,&max);
	}else{
		if(!pHistgramDlg->set_min_max_from_mid_width( p_du->Average , p_du->Stdiv * 2 , &min, &max) )	return false;
	}
		if( !p_du->SetDispRange( min, max) )	return	false;//表示レンジの変更に失敗

	DrawImage( number, true);
	return	true;
}


/********************************************************************
機  能  名  称 : 画像表示
関    数    名 : DrawImage
引          数 : long		number		(in)画像メモリ番号
				bool		do_update	(in)表示データの更新するか
				bool		do_adjust	(in)表示サイズをメイン画面サイズにあわせるか
				bool redraw_background  (in)背景再描画するか
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawImage(long number, bool do_update , bool do_adjust, bool redraw_background)
{
		if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	pDataDlg->ChangeCurrentDataNumber( number );//選択中の画像を変更する 

	CDataUnit	*p_du = GetDataUnit(number);
	if(p_du)
	{
		if(do_update){	
			p_du->UpdateDispBuffer();//表示画像の更新をおこなう
		}

		if( do_adjust){
			pPIMMOMDlg->p_image_disp_main->DrawAdjustedImage(p_du,do_adjust);//縮尺変更して描画する
		}
	}
	
	pPIMMOMDlg->DrawImage(redraw_background);
	pDataDlg->DrawImage(number);
	pHistgramDlg->DrawHistgram();//ヒストグラムを描画する
	pMarkDlg->ShowMarkVal( number );//マークを表示する

	if(pMultiImageDispDlg){//複数画像表示
		pMultiImageDispDlg->DispMultiImage();
	}

	if (p3DImgDispDlg){//3D画像一覧表示
		p3DImgDispDlg->DispImage();
	}
}

void	CPimpomAPI::DrawImage(long number, CString name, bool do_update, bool do_adjust)
{
	CDataUnit	*p_du = GetDataUnit(number);
	if(p_du)	p_du->DataName = name;

	DrawImage(number, do_update, do_adjust);
}

/********************************************************************
機  能  名  称 : メイン画面表示画像スケール変更
関    数    名 : ScaleImage
引          数 : long		number		(in)画像メモリ番号
				　float scale			(in)スケール　（1/100～100の範囲）
戻    り    値 : 成功すれば true
機          能 : スケール変更ののち再描画する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::ScaleImage(long number, float scale)
{
	CDataUnit *p_du = GetDataUnit( number );
	if(p_du == NULL)	return false;//現在選択中の画像メモリにデータが無ければなにもしない

	if(scale < 0.01 || scale > 100)	return false;//倍率は1/100～100

	pPIMMOMDlg->p_image_disp_main->ScaleImage(p_du,scale);

	DrawImage(number);//再描画

	return true;
}

/********************************************************************
機  能  名  称 : 画像表示（メイン画面のみ）
関    数    名 : DrawCurrentImageFast
引          数 : bool		do_update	(in)表示データの更新するか
戻    り    値 : 成功すれば true
機          能 : 現在選択中の画像を表示する。メイン画面のみであるため,少し高速になる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void		CPimpomAPI::DrawCurrentImageFast(bool do_update )
{
	long	number = GetCurrentImageNumber();//現在選択中の画像を指定する場合

		if(do_update){	//表示画像の更新をおこなう場合
			CDataUnit	*p_du = GetDataUnit(number);
				if(p_du)	p_du->UpdateDispBuffer();//データが無いときは更新しない
		}
	pPIMMOMDlg->DrawImage(false);//メイン画面を描画する
}


/********************************************************************
機  能  名  称 : 表示画像のカラーパレットを変更する
関    数    名 : ChangePalette
引          数 : long		number		(in)画像メモリ番号
				short			palette_id		(in)カラーパレットのID
戻    り    値 : 成功したらtrue
機          能 : RGB,RGB3D以外の画像データ表示時の、擬似カラー表示方法を変更する
				設定できる種類は下記
					PALETTE_GRAYSCALE：		グレースケール
					PALETTE_NEGATIVE：		グレースケール反転
					PALETTE_RAINBOW：		Ｂ→Ｇ→Ｒに滑らかに変化
					PALETTE_BLACK_RAINBOW：	Ｂ→Ｇ→Ｒに滑らかに変化。0未満は黒
					PALETTE_COSINE：		正弦波状の濃度変化
					PALETTE_BLUE_YELLOW：	Ｂ→Ｙに滑らかに変化
					PALETTE_GREEN_RED：		Ｇ→Ｒに滑らかに変化
					PALETTE_LABEL:			濃度１ごとに表示色が大きく変化する(ラベル用)
日付         作成者          内容
------------ --------------- ---------------------------------------
			Y.Ikeda         新規作成
********************************************************************/
bool		CPimpomAPI::ChangePalette(int number, short palette_id)
{
	CDataUnit *pdu = GetDataUnit(number);

	if (!pdu)	return false;

	pdu->ChangePalette(palette_id);

	return true;
}


///////////////////////////////////////////////////////////////////////////////////
//図形描画に関する処理
/////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : 直線ax + by + c=0を矩形領域でクリップする
関    数    名 : ClipSegmentF
引          数 : CRect		region				(in)矩形領域
				 double		a,b,c				(in)直線パラメタ
				 double		p0x, p0y, p1x, p1y	(out)直線線分始点
戻    り    値 : 成功すれば true
機          能 : 直線線分が領域内に含まれない場合はfalse を返す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::ClipSegmentF( CRect region , double a, double b, double c, double *p0x, double *p0y, double *p1x, double *p1y)
{
	//領域内に直線が入っているかどうかチェック
	POINT ptLU, ptRU, ptRB, ptLB;
	ptLU.x = region.left;	ptLU.y = region.top;//領域左上
	ptRU.x = region.right;	ptRU.y = region.top;//領域右上
	ptRB.x = region.right;	ptRB.y = region.bottom;//領域右下
	ptLB.x = region.left;	ptLB.y = region.bottom;//領域左下


	double clossPointX[2], clossPointY[2];
	int crossCnt=0;//交点数

	if( (a * (double)ptLU.x + b * (double)ptLU.y + c) * (a * (double)ptRU.x + b * (double)ptRU.y + c) < 0 )		//直線が領域端 上端（左上-右上）と交わる
	{
		clossPointY[crossCnt] = ptLU.y;
		clossPointX[crossCnt] = (-c - b*(double)ptLU.y )/a;
		crossCnt++;
	}

	if( (a * (double)ptRB.x + b * (double)ptRB.y + c) * (a * (double)ptLB.x + b * (double)ptLB.y + c) < 0 )		//直線が領域端 左下-右下と交わる
	{
		clossPointY[crossCnt] = ptLB.y;
		clossPointX[crossCnt] = (-c - b*(double)ptLB.y )/a;
		crossCnt++;
	}

	if( (a * (double)ptLU.x + b * (double)ptLU.y + c) * (a * (double)ptLB.x + b * (double)ptLB.y + c) < 0 )		//直線が領域端 左上-左下と交わる
	{
		clossPointX[crossCnt] = ptLU.x;
		clossPointY[crossCnt] = (-c - a*(double)ptLU.x )/b;
		crossCnt++;
	}

	if( (a * (double)ptRU.x + b * (double)ptRU.y + c) * (a * (double)ptRB.x + b * (double)ptRB.y + c) < 0 )		//直線が領域端 右上-右下と交わる
	{
		clossPointX[crossCnt] = ptRB.x;
		clossPointY[crossCnt] = (-c - a*(double)ptRB.x )/b;
		crossCnt++;
	}

	if(crossCnt!=2)	return false;

	*p0x = clossPointX[0];
	*p0y = clossPointY[0];
	*p1x = clossPointX[1];
	*p1y = clossPointY[1];

	return true;
}




/********************************************************************
機  能  名  称 : 直線線分を矩形領域でクリッピング
関    数    名 : ClipSegment
引          数 : CRect		region		(in)矩形領域
				 double		p0x, p0y	(in/out)直線線分始点
				 CPoint		p1x, p1y	(in/out)直線線分終点
戻    り    値 : 成功すれば true
機          能 : 直線線分が領域内に含まれない場合はfalse を返す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::ClipSegment( CRect region , double *p0x, double *p0y, double *p1x, double *p1y )
{
	double  cross_section_x, cross_section_y, vertex1x, vertex1y, vertex2x, vertex2y, vertex3x, vertex3y, vertex4x, vertex4y;//矩形領域の各頂点
	double  clipped_vertex_x[2], clipped_vertex_y[2];//線分を描画する頂点

	int		vertex_counter = 0;//決定した頂点の数（2になったら描画開始）

		if(*p0x == *p1x && *p0y == *p1y)		return	false;//2点が同じ座標の場合は切り取りをおこなわない

	//領域内に入っている点を探す
		if(	*p0x > region.left  &&		
			*p0x < region.right  &&
			*p0y > region.top  &&
			*p0y < region.bottom  )//p0が領域内である
		{
			clipped_vertex_x[vertex_counter] = *p0x;
			clipped_vertex_y[vertex_counter] = *p0y;
			vertex_counter++;
		}

		if(	*p1x > region.left  &&		
			*p1x < region.right  &&
			*p1y > region.top  &&
			*p1y < region.bottom  )//p1が領域内である
		{
			clipped_vertex_x[vertex_counter] = *p1x;
			clipped_vertex_y[vertex_counter] = *p1y;
			vertex_counter++;
		}

				if(vertex_counter==2)	goto	L_DET_VERT;//描画する線分の2頂点が決まった


	//線分切り取り
	vertex1x = region.left ;
	vertex2x = region.right;
	vertex3x = region.right;
	vertex4x = region.left ;
	vertex1y = region.top ;
	vertex2y = region.top ;
	vertex3y = region.bottom ;
	vertex4y = region.bottom ;

	//上の辺と2点を結ぶ直線との交点を調べる
	if( get_closs_secion_with_h_side( vertex1x, vertex1y, vertex2x, vertex2y, *p0x, *p0y, *p1x, *p1y, &cross_section_x, &cross_section_y)  ){
		clipped_vertex_x[ vertex_counter ] = cross_section_x;
		clipped_vertex_y[ vertex_counter ] = cross_section_y;
		vertex_counter++;
				
		if(vertex_counter==2)	goto	L_DET_VERT;//描画する線分の2頂点が決まった
	}

	//右の辺と2点を結ぶ直線との交点を調べる
	if( get_closs_secion_with_v_side( vertex2x, vertex2y, vertex3x, vertex3y, *p0x, *p0y, *p1x, *p1y, &cross_section_x, &cross_section_y)  ){
		clipped_vertex_x[ vertex_counter ] = cross_section_x;
		clipped_vertex_y[ vertex_counter ] = cross_section_y;
		vertex_counter++;
				
		if(vertex_counter==2)	goto	L_DET_VERT;//描画する線分の2頂点が決まった
	}

	//下の辺と2点を結ぶ直線との交点を調べる
	if( get_closs_secion_with_h_side( vertex3x, vertex3y, vertex4x, vertex4y, *p0x, *p0y, *p1x, *p1y, &cross_section_x, &cross_section_y)  ){
		clipped_vertex_x[ vertex_counter ] = cross_section_x;
		clipped_vertex_y[ vertex_counter ] = cross_section_y;
		vertex_counter++;
				
		if(vertex_counter==2)	goto	L_DET_VERT;//描画する線分の2頂点が決まった
	}

	//左の辺と2点を結ぶ直線との交点を求める
	if( get_closs_secion_with_v_side( vertex4x, vertex4y, vertex1x, vertex1y, *p0x, *p0y, *p1x, *p1y, &cross_section_x, &cross_section_y)  ){
		clipped_vertex_x[ vertex_counter ] = cross_section_x;
		clipped_vertex_y[ vertex_counter ] = cross_section_y;
		vertex_counter++;
				
		if(vertex_counter==2)	goto	L_DET_VERT;//描画する線分の2頂点が決まった
	}


	//最後まで2つの頂点が決まらなかった場合
	if( vertex_counter < 2 )	return	false;//線分が矩形領域内に含まれない

	//2頂点が決定した場合
L_DET_VERT:
		*p0x = clipped_vertex_x[ 0 ];
		*p0y = clipped_vertex_y[ 0 ];
		*p1x = clipped_vertex_x[ 1 ];
		*p1y = clipped_vertex_y[ 1 ];

		return	true;
}


/********************************************************************
機  能  名  称 : 矩形領域内に直線線分を描画する（領域外はクリッピング）
関    数    名 : DrawClippedSegment
引          数 : CDC		*p_dc		(in)
				 CRect		region		(in)矩形領域
				 double		p0x,p0y		(in)直線線分始点
				 double		p1x,p1y		(in)直線線分終点
				 int		size		(in)線幅
				 COLORREF	col			(in)描画色
戻    り    値 : 成功すれば true
機          能 : 直線線分が領域内に含まれない場合はfalse を返す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::DrawClippedSegment(CDC *p_dc, CRect region , double p0x, double p0y, double p1x, double p1y, int size, COLORREF col)
{
	CPen	pen, *poldpen;
		if( !ClipSegment(region, &p0x, &p0y, &p1x, &p1y) )		return	false;//直線線分が矩形領域内に含まれない

	pen.CreatePen(PS_SOLID , size , col );
	poldpen = p_dc->SelectObject(&pen);
	p_dc->MoveTo( (int)(p0x+0.5), (int)(p0y+0.5) );
	p_dc->LineTo( (int)(p1x+0.5), (int)(p1y+0.5) );
	p_dc->SelectObject(poldpen);
	pen.DeleteObject();

	return	true;
}

bool	CPimpomAPI::DrawClippedSegment(CDC *p_dc, CRect region , double p0x, double p0y, double p1x, double p1y)
{
	return DrawClippedSegment(p_dc, region ,p0x, p0y, p1x ,p1y, 1, default_fg_color);//標準色で描画する
}





/********************************************************************
機  能  名  称 : 矩形領域内に矩形を描画する（領域外はクリッピング）
関    数    名 : DrawClippedRect
引          数 : CDC		*p_dc		(in)
				 CRect		region		(in)矩形領域
				 CRect		draw_rect	(in)描画する矩形
				 int		size		(in)線幅
				 COLORREF	col			(in)描画色
戻    り    値 : 成功すれば true
機          能 : 描画矩形が領域内に含まれない場合はfalse を返す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::DrawClippedRect(CDC *p_dc, CRect region , CRect draw_rect, int size, COLORREF col)
{
	bool	result = false;
	CPoint	cross_section, vertex1, vertex2, vertex3, vertex4;//矩形領域の各頂点

	vertex1 = CPoint( draw_rect.left , draw_rect.top );
	vertex2 = CPoint( draw_rect.right, draw_rect.top );
	vertex3 = CPoint( draw_rect.right, draw_rect.bottom );
	vertex4 = CPoint( draw_rect.left , draw_rect.bottom );

		if(DrawClippedSegment(p_dc, region, vertex1.x , vertex1.y , vertex2.x , vertex2.y ,size, col) )	result = true;
		if(DrawClippedSegment(p_dc, region, vertex2.x , vertex2.y , vertex3.x , vertex3.y ,size, col) )	result = true;
		if(DrawClippedSegment(p_dc, region, vertex3.x , vertex3.y , vertex4.x , vertex4.y ,size, col) )	result = true;
		if(DrawClippedSegment(p_dc, region, vertex4.x , vertex4.y , vertex1.x , vertex1.y ,size, col) )	result = true;

	return	result;
}

bool	CPimpomAPI::DrawClippedRect(CDC *p_dc, CRect region , CRect draw_rect)
{
	return DrawClippedRect(p_dc, region , draw_rect, 1, default_fg_color);//標準色で描画する
}


/********************************************************************
機  能  名  称 : メイン画面上に直線描画
関    数    名 : DrawLine
引          数 : double		x0,y0		(in)始点座標（画像座標で指定）
				 double		x1,y1		(in)終点座標（画像座標で指定）
				 int		pen_size	(in)描画するペンサイズ
				 COLORREF	col			(in)描画色
				 int		width		(in)線幅
戻    り    値 : 
機          能 : 線幅に1より大きい値を指定すると、幅つき直線領域になる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawLine(double x0, double y0, double x1, double y1, int pen_size, COLORREF col, int width)
{
	CImageField  *p_if;
	CDC			 *p_dc;
	CDataUnit	 *p_du;
	CRect		dest_rect;
	double		dest_point1x, dest_point1y, dest_point2x, dest_point2y;

	if( (p_du = GetDataUnit(CURRENT_IMAGE))==NULL)	return;//選択中の画像メモリにデータが無ければここから後は何もしない
	p_dc = pPIMMOMDlg->GetDC();//メインウインドウのデバイスコンテキスト取得
	p_if = pPIMMOMDlg->GetMainImage();//メイン画面の画像表示情報を取得
	p_if->GetDestRect(p_du, &dest_rect);//メイン画面の画像表示矩形領域を取得
	p_if->GetDestCoor( p_du , x0, y0 , &dest_point1x, &dest_point1y );//メイン画面での座標を取得
	p_if->GetDestCoor( p_du , x1, y1 , &dest_point2x, &dest_point2y );//メイン画面での座標を取得
	DrawClippedSegment(p_dc, dest_rect, dest_point1x, dest_point1y, dest_point2x, dest_point2y, pen_size, col);//線を描く
	pPIMMOMDlg->ReleaseDC(p_dc);

	if(width>1){//幅つき
		CPoint point[4];
		FIGURE_LINE line;

		line.x1=(int)(x0+0.5);
		line.y1=(int)(y0+0.5);
		line.x2=(int)(x1+0.5);
		line.y2=(int)(y1+0.5);
		line.w = width;
		CWorkArea::GetLineVertxes(line, point);
		
		DrawLine(point[0].x, point[0].y, point[1].x, point[1].y, pen_size, col, 1);
		DrawLine(point[1].x, point[1].y, point[2].x, point[2].y, pen_size, col, 1);
		DrawLine(point[2].x, point[2].y, point[3].x, point[3].y, pen_size, col, 1);
		DrawLine(point[3].x, point[3].y, point[0].x, point[0].y, pen_size, col, 1);
	}
}


void	CPimpomAPI::DrawLine(double x0, double y0, double x1, double y1, int width)
{
	DrawLine(x0, y0, x1, y1, 1, default_fg_color, width);
}





/********************************************************************
機  能  名  称 : メイン画面上に十字カーソル描画
関    数    名 : DrawCursor
引          数 : double		x,y			(in)座標（画像座標で指定）
				 int		pen_size	(in)描画するペンサイズ
				 COLORREF	col			(in)描画色
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawCursor(double x, double y, int pen_size, COLORREF col)
{
	long		mark_length = 20;//マークのラインの長さ

	DrawLine((double)(x - mark_length/2) , y , (double)(x + mark_length/2) , y, pen_size, col);
	DrawLine(x , (double)(y - mark_length/2) , x , (double)(y + mark_length/2), pen_size, col);
}

void	CPimpomAPI::DrawCursor(double x, double y)
{
	DrawCursor(x, y, 1, default_fg_color);
}


/********************************************************************
機  能  名  称 : メイン画面上に矩形描画
関    数    名 : DrawRect
引          数 : double		left		(in)左辺座標（画像座標で指定）
				double			top			(in)右辺座標（画像座標で指定）
				double			right		(in)上辺座標（画像座標で指定）
				double			bottom		(in)下辺座標（画像座標で指定）
				 int		pen_size	(in)描画するペンサイズ
				 COLORREF	col			(in)描画色
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawRect(double left, double top, double right, double bottom, int pen_size, COLORREF col)
{
	DrawLine(left, top, right, top, pen_size, col);
	DrawLine(left, bottom, right, bottom, pen_size, col);
	DrawLine(left, top, left, bottom, pen_size, col);	
	DrawLine(right, top, right, bottom, pen_size, col);
}

void	CPimpomAPI::DrawRect(double left, double top, double right, double bottom)
{
	DrawRect(left, top, right, bottom, 1, default_fg_color);
}


/********************************************************************
機  能  名  称 : メイン画面上に円描画
関    数    名 : DrawCircle
引          数 : double		x,y			(in)中心座標（画像座標で指定）
				double		r			(in)半径（画像座標で指定）
				 int		pen_size	(in)描画するペンサイズ
				 COLORREF	col			(in)描画色
				 int		width		(in)線幅
戻    り    値 : 
機          能 : 線幅に1より大きい値を指定すると、幅つき円周領域になる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawCircle(double x, double y, double r, int pen_size, COLORREF col, int width)
{
	DrawEllipse(x,y,r,r,pen_size,col,width);
}

void	CPimpomAPI::DrawCircle(double x, double y, double r, int width)
{
	DrawCircle( x, y, r, 1, default_fg_color, width);
}


/********************************************************************
機  能  名  称 : メイン画面上に楕円描画
関    数    名 : DrawEllipse
引          数 : double		x,y			(in)中心座標（画像座標で指定）
				double		rx,yx		(in)X,Y方向の半径（画像座標で指定）
				 int		pen_size	(in)描画するペンサイズ
				 COLORREF	col			(in)描画色
				 int		width		(in)線幅
戻    り    値 : 
機          能 : 線幅に1より大きい値を指定すると、幅つき楕円周領域になる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawEllipse(double x, double y, double rx, double ry, int pen_size, COLORREF col, int width)
{
	CImageField  *p_if;
	CDataUnit	 *p_du;
	CDC			 *p_dc;
	double		dest_rx, dest_ry;
	CRect		dest_rect;
	double		dest_point_x, dest_point_y;
	CPen	pen, *poldpen;
	CBrush	 *poldbrush;
			
			if( (p_du = GetDataUnit(CURRENT_IMAGE))==NULL)	return;//選択中の画像メモリにデータが無ければここから後は何もしない

	p_dc = pPIMMOMDlg->GetDC();//メインウインドウのデバイスコンテキスト取得
	p_if = pPIMMOMDlg->GetMainImage();//メイン画面の画像表示情報を取得
	pen.CreatePen(PS_SOLID , pen_size , col);
	poldpen = (CPen*)p_dc->SelectObject(&pen);
	poldbrush = (CBrush*)p_dc->SelectStockObject(NULL_BRUSH);

	p_if->GetDestRect(p_du, &dest_rect);//メイン画面の画像表示矩形領域を取得
	p_if->GetDestCoor( p_du , x, y , &dest_point_x, &dest_point_y );//メイン画面での座標を取得
	dest_rx = p_du->DispScale * rx;
	dest_ry = p_du->DispScale * ry;

	p_dc->Ellipse((int)(dest_point_x-dest_rx+0.5), (int)(dest_point_y-dest_ry+0.5), (int)(dest_point_x+dest_rx+0.5), (int)(dest_point_y+dest_ry+0.5));
	p_dc->SelectObject(poldpen);
	p_dc->SelectObject(poldbrush);
	pPIMMOMDlg->ReleaseDC(p_dc);
	pen.DeleteObject();

	if(width>1){
		if( ((double)width/2.0 < rx)  &&  ((double)width/2.0 < ry)  )
		{
			DrawEllipse(x, y, rx-(double)width/2.0, ry-(double)width/2.0);
		}
	
		DrawEllipse(x, y, rx+(double)width/2.0, ry+(double)width/2.0);
	}
}

void	CPimpomAPI::DrawEllipse(double x, double y, double rx, double ry)
{
	DrawEllipse( x, y, rx, ry, 1, default_fg_color);
}


/********************************************************************
機  能  名  称 : メイン画面上にテキスト描画
関    数    名 : DrawText
引          数 : double		x,y			(in)描画座標（画像座標で指定）
				 CString	str,		(in)テキスト
				 COLORREF	col			(in)描画色
				 int		size		(in)文字サイズ
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawText(double x, double y, CString str, COLORREF col, int size)
{
	CImageField  *p_if;
	CDataUnit	 *p_du;
	CRect		dest_rect;
	double		dest_point_x, dest_point_y;
	CDC			*p_dc;
	CFont		font, *poldfont;
	LOGFONT		lf;
	COLORREF	oldcol;

			if( (p_du = GetDataUnit(CURRENT_IMAGE))==NULL)	return;//選択中の画像メモリにデータが無ければここから後は何もしない

	memcpy(&lf,&default_font,sizeof(LOGFONT));
	if(size>0){		lf.lfHeight = size;	}/*lf.lfHeight=80;*/
	font.CreateFontIndirect(&lf);

	p_dc = pPIMMOMDlg->GetDC();//メインウインドウのデバイスコンテキスト取得
	p_if = pPIMMOMDlg->GetMainImage();//メイン画面の画像表示情報を取得
	oldcol = p_dc->SetTextColor(col);
	poldfont = (CFont*)p_dc->SelectObject(&font);
	p_if->GetDestRect(p_du, &dest_rect);//メイン画面の画像表示矩形領域を取得
	p_if->GetDestCoor( p_du , x, y , &dest_point_x, &dest_point_y );//メイン画面での座標を取得
	
	//メイン画面からはみ出さないようにする
	if(	(int)(dest_point_x+0.5) >= MAIN_WIN_LEFT_MARGIN && 
		(int)(dest_point_x+0.5) + lf.lfHeight < MAIN_WIN_LEFT_MARGIN + main_image_size.cx && 
		(int)(dest_point_y+0.5) >= MAIN_WIN_TOP_MARGIN && 
		(int)(dest_point_y+0.5) + lf.lfWidth < MAIN_WIN_TOP_MARGIN + main_image_size.cy)
	{
		p_dc->TextOut((int)(dest_point_x+0.5), (int)(dest_point_y+0.5), str);
	}
	
	p_dc->SetTextColor(oldcol);
	p_dc->SelectObject(poldfont);
	pPIMMOMDlg->ReleaseDC(p_dc);//デバイスコンテキストを開放する
	font.DeleteObject();
}

void	CPimpomAPI::DrawText(double x, double y, CString str)
{
	DrawText(x, y, str, default_fg_color, NULL);
}


/********************************************************************
機  能  名  称 : メイン画面上にテキスト描画（フォーマット指定）
関    数    名 : DrawText
引          数 : int		x,y			(in)描画座標（画像座標で指定）
				 COLORREF	col			(in)描画色
				 int		size		(in)文字サイズ
				 char		*format, ...(in)テキスト
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPimpomAPI::DrawText(double x, double y, COLORREF col, int size, char *format, ...)
{
	va_list list;
	char	buf[5000];

	va_start( list, format );
	vsprintf(buf,format, list);
	va_end( list );

	DrawText(x, y, CString(buf), col, size);
}