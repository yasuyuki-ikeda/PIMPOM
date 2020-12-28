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
//CWorkArea
//作業領域を扱うクラス
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

/********************************************************************
機  能  名  称 : 図形番号を指定して作業領域を初期化する
関    数    名 : Initialize
引          数 : CSize		image_size				(in)画像メモリサイズ
				 int		no						(in)図形番号
				 int		t						(in)図形タイプ
				 bool		ornot					(in)OR領域／NOT領域  true:OR  false:NOT
戻    り    値 : 
機          能 : 矩形タイプで初期化する場合、サイズは画像メモリのサイズになる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkArea::Initialize(CSize image_size, int no, int t, bool ornot)
{
	//引数チェック
	if(image_size.cx<=0 || image_size.cy<=0)	return;
	if(no<0 || no>=WORKAREA_FIG_NUM)return;


	memset(&Figure[no], 0, sizeof(FIGURE_UNION));

	FigureEditMode[no] = 0;
	Type[no] = t;
	OrNot[no] = ornot;
	switch(Type[no])
	{
	case WORK_AREA_TYPE_RECT://矩形
		Figure[no].rect.left = 0;
		Figure[no].rect.top = 0;
		Figure[no].rect.right = image_size.cx -1;
		Figure[no].rect.bottom = image_size.cy -1;
		break;

	case WORK_AREA_TYPE_LINE://幅つき直線
		memset(&Figure[no].line, 0, sizeof(FIGURE_LINE));
		Figure[no].line.w = 1;
		break;

	case WORK_AREA_TYPE_ELLIPSE://楕円
		memset(&Figure[no].ellipse, 0, sizeof(FIGURE_ELLIPSE));
		break;

	case WORK_AREA_TYPE_CIRCLE://幅つき円周
		memset(&Figure[no].circle, 0, sizeof(FIGURE_CIRCLE));
		Figure[no].circle.w=1;
		break;

	case WORK_AREA_TYPE_ARC://幅つき円弧
		memset(&Figure[no].arc, 0, sizeof(FIGURE_ARC));
		Figure[no].arc.w=1;
		break;

	case WORK_AREA_TYPE_POLYGON://多角形
		memset(&Figure[no].polygon, 0, sizeof(FIGURE_POLYGON));
		break;
	}
}

/********************************************************************
機  能  名  称 : 全ての作業領域を初期化する
関    数    名 : Initialize
引          数 : CSize		image_size				(in)画像メモリサイズ
戻    り    値 : 
機          能 : 作業領域0は画像サイズの矩形に初期化。それ以外は無効
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkArea::Initialize(CSize image_size)
{
	//引数チェック
	if(image_size.cx<=0 || image_size.cy<=0)	return;


	Initialize(image_size, 0,  WORK_AREA_TYPE_RECT, true);//作業領域0は画像サイズの矩形に初期化。それ以外は無効
	for(int i=1 ; i<WORKAREA_FIG_NUM ; i++){
		Initialize(CSize(1,1), i,  WORK_AREA_TYPE_NOTHING, true);
	}
}

/********************************************************************
機  能  名  称 : 作業領域をコピーする
関    数    名 : Copy
引          数 : CWorkArea		*area		(in)コピー元の作業領域
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkArea::Copy(CWorkArea *area)
{
	//引数チェック
	if(!area)	return;


	memcpy(Type, area->Type, sizeof(int)*WORKAREA_FIG_NUM);
	memcpy(Figure, area->Figure, sizeof(FIGURE_UNION)*WORKAREA_FIG_NUM);
	memcpy(OrNot, area->OrNot, sizeof(bool)*WORKAREA_FIG_NUM);
}

/********************************************************************
機  能  名  称 : 点が作業領域内に入っているかチェックする
関    数    名 : PtInWorkArea
引          数 : CPoint		point		(in)
戻    り    値 : 全ての作業領域をチェックし、
				どれかのORに入っているかつどのNOT領域にも入っていないならばtrueを返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CWorkArea::PtInWorkArea(CPoint point)
{
	//どれかのNOT図形内ならば領域外とする
	for(int i=0 ; i<WORKAREA_FIG_NUM ; i++){
		if(!OrNot[i]){
			if( !PtInWorkArea(point,i) )	return false;
		}
	}

	//すべてのNOT図形外かつ
	//どれかのOR図形内ならば領域内とする
	for(int i=0 ; i<WORKAREA_FIG_NUM ; i++){
		if(OrNot[i]){
			if( PtInWorkArea(point,i) )	return true;
		}
	}

	return false;
}

/********************************************************************
機  能  名  称 : 点が作業領域内に入っているかチェックする（図形番号指定）
関    数    名 : PtInWorkArea
引          数 : CPoint		point		(in)
				 int		no			(in)図形番号
戻    り    値 : noで指定された図形番号が
				 OR領域：　　図形内に入っていればtrue
				 NOT領域：　　図形内に入っていなければtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CWorkArea::PtInWorkArea(CPoint point, int no)
{
	//引数チェック
	if(no<0 || no>=WORKAREA_FIG_NUM)return false;


	bool res=false;

	switch(Type[no])
	{
	case WORK_AREA_TYPE_RECT://矩形
		if( Figure[no].rect.left<=point.x && Figure[no].rect.top<=point.y && 
			Figure[no].rect.right>=point.x && Figure[no].rect.bottom>=point.y)
		{
			res = OrNot[no];
		}else{
			res = !OrNot[no];
		}
		break;



	case WORK_AREA_TYPE_LINE://幅つき直線
		if(Figure[no].line.x1==Figure[no].line.x2 || Figure[no].line.y1==Figure[no].line.y2 || Figure[no].line.w==0){
			res = !OrNot[no];//面積がないとき
		}else{
			CPoint vertex[4];
			double outer_prod[4];
			GetLineVertxes(Figure[no].line, vertex);

			outer_prod[0] = (vertex[0].x - point.x)*(vertex[1].y - point.y) - (vertex[0].y - point.y)*(vertex[1].x - point.x) ;
			outer_prod[1] = (vertex[1].x - point.x)*(vertex[2].y - point.y) - (vertex[1].y - point.y)*(vertex[2].x - point.x) ;
			outer_prod[2] = (vertex[2].x - point.x)*(vertex[3].y - point.y) - (vertex[2].y - point.y)*(vertex[3].x - point.x) ;
			outer_prod[3] = (vertex[3].x - point.x)*(vertex[0].y - point.y) - (vertex[3].y - point.y)*(vertex[0].x - point.x) ;

			if( outer_prod[0]*outer_prod[1]<0 ||
				outer_prod[1]*outer_prod[2]<0 ||
				outer_prod[2]*outer_prod[3]<0 ||
				outer_prod[3]*outer_prod[0]<0)
			{
				res = !OrNot[no];
			}else{
				res = OrNot[no];
			}
		}
		break;


	case WORK_AREA_TYPE_ELLIPSE://楕円
		if(Figure[no].ellipse.rx==0 || Figure[no].ellipse.ry==0){
			res = !OrNot[no];//面積がないとき
		}else{
			double dis;
			dis = (double)(Figure[no].ellipse.cx - point.x)*(Figure[no].ellipse.cx - point.x)/(double)(Figure[no].ellipse.rx*Figure[no].ellipse.rx)
				+ (double)(Figure[no].ellipse.cy - point.y)*(Figure[no].ellipse.cy - point.y)/(double)(Figure[no].ellipse.ry*Figure[no].ellipse.ry);

			if(dis<=1.0)	res = OrNot[no];
			else		res = !OrNot[no];
		}
		break;

	case WORK_AREA_TYPE_CIRCLE://幅つき円周
		if(Figure[no].circle.r==0 || Figure[no].circle.w==0){
			res = !OrNot[no];//面積がないとき
		}else{
			double radius2;
			radius2 = (double)(Figure[no].circle.cx - point.x)*(Figure[no].circle.cx - point.x)
				+ (double)(Figure[no].circle.cy - point.y)*(Figure[no].circle.cy - point.y);

			if( radius2> (Figure[no].circle.r - Figure[no].circle.w/2)*(Figure[no].circle.r - Figure[no].circle.w/2) &&
				radius2< (Figure[no].circle.r + Figure[no].circle.w/2)*(Figure[no].circle.r + Figure[no].circle.w/2))
			{
				res = OrNot[no];
			}else{
				res = !OrNot[no];
			}
		}
		break;

	//幅つき円弧
	case WORK_AREA_TYPE_ARC://幅つき円弧
		res = !OrNot[no];
		break;

	//多角形
	case WORK_AREA_TYPE_POLYGON://多角形
		res = !OrNot[no];
		break;

	//領域なし
	default:
		res = false;
		break;
	}

	return res;
}

/********************************************************************
機  能  名  称 : 幅つき直線の４頂点を求める
関    数    名 : GetLineVertxes
引          数 : FIGURE_LINE		line			(in)幅つき直線
				 CPoint				point[4]		(out)頂点
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CWorkArea::GetLineVertxes(FIGURE_LINE line, CPoint point[4])
{
		double len, tx, ty;
		CPoint	p1, p2, p3, p4;

		len = sqrt( (double)(line.x1 - line.x2)*(line.x1 - line.x2) + (double)(line.y1 - line.y2)*(line.y1 - line.y2)  );
		if(len==0){
			point[0] = CPoint(line.x1,line.y1);
			point[1] = CPoint(line.x1,line.y1);
			point[2] = CPoint(line.x2,line.y2);
			point[3] = CPoint(line.x2,line.y2);
			return;
		}

		tx =  (double)(line.y1 - line.y2) / len * (double)line.w/2;
		ty = -(double)(line.x1 - line.x2) / len * (double)line.w/2;
		
		point[0].x = line.x1-(long)tx;	point[0].y = line.y1-(long)ty;
		point[1].x = line.x1+(long)tx;	point[1].y = line.y1+(long)ty;
		point[2].x = line.x2+(long)tx;	point[2].y = line.y2+(long)ty;
		point[3].x = line.x2-(long)tx;	point[3].y = line.y2-(long)ty;
}

/********************************************************************
機  能  名  称 : 作業領域を取得する
関    数    名 : GetWorkArea
引          数 : int				no			(in)図形番号
				 FIGURE_UNION	*fig		(out)図形
				 bool				*ornot		(out)OR領域／NOT領域  true:OR  false:NOT
戻    り    値 : 作業領域の図形のタイプ   (失敗したら-1を返す)
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CWorkArea::GetWorkArea(int no, FIGURE_UNION *fig, bool *ornot)
{
	//引数チェック
	if(no<0 || no>=WORKAREA_FIG_NUM)	return -1;

	memcpy(fig, &Figure[no], sizeof(FIGURE_UNION));
	*ornot = OrNot[no];
	return Type[no];
}

/********************************************************************
機  能  名  称 : 矩形の作業領域を設定する
関    数    名 : SetWorkArea
引          数 : int				no			(in)図形番号
				 RECT				rect		(in)矩形
				 bool				ornot		(in)OR領域／NOT領域  true:OR  false:NOT
戻    り    値 : 成功したらtrue
機          能 : RECT内の上下左右関係が反転していた場合は、正規化する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CWorkArea::SetWorkArea(int no, RECT rect, bool ornot)
{
	//引数チェック
	if(no<0 || no>=WORKAREA_FIG_NUM)	return false;


	//RECT内の上下左右関係が反転していた場合は、正規化する
	int tmp;
	if(rect.left>rect.right){
		tmp = rect.left;
		rect.left = rect.right;
		rect.right = tmp;
	}

	if(rect.top>rect.bottom){
		tmp=rect.top;
		rect.top=rect.bottom;
		rect.bottom = tmp;
	}

	Type[no]= WORK_AREA_TYPE_RECT;
	OrNot[no] = ornot;
	Figure[no].rect.left=rect.left;
	Figure[no].rect.top=rect.top;
	Figure[no].rect.right=rect.right;
	Figure[no].rect.bottom=rect.bottom;

	return true;
}

/********************************************************************
機  能  名  称 : 幅つき直線の作業領域を設定する
関    数    名 : SetWorkArea
引          数 : int				no			(in)図形番号
				 FIGURE_LINE		line		(in)幅つき直線
				 bool				ornot		(in)OR領域／NOT領域  true:OR  false:NOT
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CWorkArea::SetWorkArea(int no, FIGURE_LINE line, bool ornot)
{
	//引数チェック
	if(no<0 || no>=WORKAREA_FIG_NUM)	return false;

	Type[no] = WORK_AREA_TYPE_LINE;
	OrNot[no] = ornot;
	Figure[no].line.x1=line.x1;
	Figure[no].line.y1=line.y1;
	Figure[no].line.x2=line.x2;
	Figure[no].line.y2=line.y2;
	Figure[no].line.w=line.w;

	return true;
}

/********************************************************************
機  能  名  称 : 楕円の作業領域を設定する
関    数    名 : SetWorkArea
引          数 : int				no			(in)図形番号
				 FIGURE_ELLIPSE	ellipse		(in)楕円
				 bool				ornot		(in)OR領域／NOT領域  true:OR  false:NOT
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CWorkArea::SetWorkArea(int no, FIGURE_ELLIPSE ellipse, bool ornot)
{
	//引数チェック
	if(no<0 || no>=WORKAREA_FIG_NUM)	return false;

	Type[no] = WORK_AREA_TYPE_ELLIPSE;
	OrNot[no] = ornot;
	Figure[no].ellipse.cx=ellipse.cx;
	Figure[no].ellipse.cy=ellipse.cy;
	Figure[no].ellipse.rx=ellipse.rx;
	Figure[no].ellipse.ry=ellipse.ry;

	return true;
}

/********************************************************************
機  能  名  称 : 幅つき円周の作業領域を設定する
関    数    名 : SetWorkArea
引          数 : int				no			(in)図形番号
				 FIGURE_CIRCLE	circle		(in)幅つき円周
				 bool				ornot		(in)OR領域／NOT領域  true:OR  false:NOT
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CWorkArea::SetWorkArea(int no, FIGURE_CIRCLE circle, bool ornot)
{
	//引数チェック
	if(no<0 || no>=WORKAREA_FIG_NUM)	return false;

	Type[no] = WORK_AREA_TYPE_CIRCLE;
	OrNot[no] = ornot;
	Figure[no].circle.cx=circle.cx;
	Figure[no].circle.cy=circle.cy;
	Figure[no].circle.r=circle.r;
	Figure[no].circle.w=circle.w;

	return true;
}

/********************************************************************
機  能  名  称 : 幅つき円弧の作業領域を設定する
関    数    名 : SetWorkArea
引          数 : int				no			(in)図形番号
				 FIGURE_ARC		arc			(in)幅つき円弧
				 bool				ornot		(in)OR領域／NOT領域  true:OR  false:NOT
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CWorkArea::SetWorkArea(int no, FIGURE_ARC arc, bool ornot)
{
	//引数チェック
	if(no<0 || no>=WORKAREA_FIG_NUM)	return false;

	Type[no] = WORK_AREA_TYPE_ARC;
	OrNot[no] = ornot;
	Figure[no].arc.cx=arc.cx;
	Figure[no].arc.cy=arc.cy;
	Figure[no].arc.r=arc.r;
	Figure[no].arc.start=arc.start;
	Figure[no].arc.end=arc.end;
	Figure[no].arc.w=arc.w;

	return true;
}

/********************************************************************
機  能  名  称 : すべての図形に外接する矩形を求める
関    数    名 : GetCircumRect
引          数 : 
戻    り    値 : 外接矩形
機          能 : 処理に失敗した場合は(0,0,0,0)が帰る
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
CRect CWorkArea::GetCircumRect()
{
	CRect	rect;
	int		xmin,ymin,xmax,ymax;

	xmin = ymin = 99999;
	xmax = ymax = -99999;
	for(int i=0 ; i<WORKAREA_FIG_NUM ; i++)
	{
		if(Type[i] != WORK_AREA_TYPE_NOTHING)
		{
			rect = GetCircumRect(i);
			if(xmin > rect.left){		xmin=rect.left;	}
			if(xmax < rect.right){		xmax=rect.right;}
			if(ymin > rect.top){		ymin=rect.top;	}
			if(ymax < rect.bottom){		ymax=rect.bottom;}
		}
	}

	return CRect(xmin,ymin,xmax,ymax);
}

/********************************************************************
機  能  名  称 : 指定された図形に外接する矩形を求める
関    数    名 : GetCircumRect
引          数 : int				no			(in)図形番号
戻    り    値 : 外接矩形
機          能 : 処理に失敗した場合は(0,0,0,0)が帰る
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
CRect CWorkArea::GetCircumRect(int no)
{
	CRect	rect(0,0,0,0);

	//引数チェック
	if(no<0 || no>=WORKAREA_FIG_NUM)	return rect;

	switch(Type[no])
	{
	case WORK_AREA_TYPE_RECT://矩形
		rect.left = Figure[no].rect.left;
		rect.top = Figure[no].rect.top;
		rect.right = Figure[no].rect.right;
		rect.bottom = Figure[no].rect.bottom;
		break;

	case WORK_AREA_TYPE_LINE://幅つき直線
		{
			CPoint	vertex[4];
			int		xmin,ymin,xmax,ymax;
			GetLineVertxes(Figure[no].line, vertex);

			xmin = ymin = 99999;
			xmax = ymax = -99999;
			for(int n=0 ; n<4 ; n++)
			{
				if(xmin > vertex[n].x){			xmin=vertex[n].x;	}
				else if(xmax < vertex[n].x){	xmax=vertex[n].x;	}

				if(ymin > vertex[n].y){			ymin=vertex[n].y;	}
				else if(ymax < vertex[n].y){	ymax=vertex[n].y;	}
			}

			rect.left = xmin;
			rect.right = xmax;
			rect.top = ymin;
			rect.bottom = ymax;
		}
		break;


	case WORK_AREA_TYPE_ELLIPSE://楕円
		{
			rect.left = Figure[no].ellipse.cx - Figure[no].ellipse.rx;
			rect.right = Figure[no].ellipse.cx + Figure[no].ellipse.rx;
			rect.top = Figure[no].ellipse.cy - Figure[no].ellipse.ry;
			rect.bottom = Figure[no].ellipse.cy + Figure[no].ellipse.ry;
		}
		break;

	case WORK_AREA_TYPE_CIRCLE://幅つき円周
		{
			rect.left = Figure[no].circle.cx - (Figure[no].circle.r + Figure[no].circle.w/2) ;
			rect.right = Figure[no].circle.cx + (Figure[no].circle.r + Figure[no].circle.w/2) ;
			rect.top = Figure[no].circle.cy - (Figure[no].circle.r + Figure[no].circle.w/2) ;
			rect.bottom = Figure[no].circle.cy + (Figure[no].circle.r + Figure[no].circle.w/2) ;
		}
		break;

	//幅つき円弧
	case WORK_AREA_TYPE_ARC://幅つき円弧
		break;

	//多角形
	case WORK_AREA_TYPE_POLYGON://多角形
		break;

	//領域なし
	default:
		break;
	}

	rect.NormalizeRect();
	return rect;
}

/********************************************************************
機  能  名  称 : 作業領域を平行移動
関    数    名 : Move
引          数 : int				dx,dy		(in)移動量
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CWorkArea::Move(int dx, int dy)
{
	for(int i=0 ; i<WORKAREA_FIG_NUM ; i++)
	{
		if( !Move(i,dx,dy) )	return false;
	}
	return true;
}

/********************************************************************
機  能  名  称 : 作業領域を平行移動
関    数    名 : Move
引          数 :int				no			(in)図形番号 
				int				dx,dy		(in)移動量
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CWorkArea::Move(int no, int dx, int dy)
{
	//引数チェック
	if(no<0 || no>=WORKAREA_FIG_NUM)	return false;

	switch(Type[no])
	{
	case WORK_AREA_TYPE_RECT://矩形
		Figure[no].rect.left += dx;
		Figure[no].rect.right += dx;
		Figure[no].rect.top += dy;
		Figure[no].rect.bottom += dy;
		break;

	case WORK_AREA_TYPE_LINE://幅つき直線
		Figure[no].line.x1 += dx;
		Figure[no].line.x2 += dx;
		Figure[no].line.y1 += dy;
		Figure[no].line.y2 += dy;
		break;


	case WORK_AREA_TYPE_ELLIPSE://楕円
		Figure[no].ellipse.cx += dx;
		Figure[no].ellipse.cy += dy;
		break;

	case WORK_AREA_TYPE_CIRCLE://幅つき円周
		Figure[no].circle.cx += dx;
		Figure[no].circle.cy += dy;
		break;

	//幅つき円弧
	case WORK_AREA_TYPE_ARC://幅つき円弧
		break;

	//多角形
	case WORK_AREA_TYPE_POLYGON://多角形
		break;

	//領域なし
	default:
		return false;
	}

	return true;
}



/********************************************************************
機  能  名  称 : 作業領域をスケーリングする
関    数    名 : Scale
引          数 : 
				 double scalex　　(in)xスケール
				 double scaley    (in)yスケール
				 CPoint offset    (in)x,yオフセット
戻    り    値 : 
機          能 : 幅つき直線、円弧のときはx/yスケールが同じ場合のみ実行可能
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool CWorkArea::Scale(int no, float scalex, float scaley, CPoint offset)
{
	//引数チェック
	if (no<0 || no >= WORKAREA_FIG_NUM)	return false;

	switch (Type[no])
	{
	case WORK_AREA_TYPE_RECT://矩形
		Figure[no].rect.left = scalex * Figure[no].rect.left + offset.x;
		Figure[no].rect.right = scalex * Figure[no].rect.right + offset.x;
		Figure[no].rect.top = scaley * Figure[no].rect.top + offset.y;
		Figure[no].rect.bottom = scaley * Figure[no].rect.bottom + offset.y;
		break;

	case WORK_AREA_TYPE_LINE://幅つき直線
		if (scalex == scaley)
		{
			Figure[no].line.x1 = scalex * Figure[no].line.x1 + offset.x;
			Figure[no].line.x2 = scalex * Figure[no].line.x2 + offset.x;
			Figure[no].line.y1 = scaley * Figure[no].line.y1 + offset.y;
			Figure[no].line.y2 = scaley * Figure[no].line.y2 + offset.y;
			Figure[no].line.w = scalex * Figure[no].line.w;
		}
		else {
				return false;
			}
		break;


	case WORK_AREA_TYPE_ELLIPSE://楕円
		Figure[no].ellipse.cx = scalex * Figure[no].ellipse.cx + offset.x;
		Figure[no].ellipse.cy = scaley * Figure[no].ellipse.cy + offset.y;
		Figure[no].ellipse.rx = scalex * Figure[no].ellipse.rx;
		Figure[no].ellipse.ry = scaley * Figure[no].ellipse.ry;
		break;

	case WORK_AREA_TYPE_CIRCLE://幅つき円周
		if (scalex == scaley)
		{
			Figure[no].circle.cx = scalex * Figure[no].circle.cx + offset.x;
			Figure[no].circle.cy = scalex * Figure[no].circle.cy + offset.y;
			Figure[no].circle.r = scalex * Figure[no].circle.r;
			Figure[no].circle.w = scalex * Figure[no].circle.w;
		}
		else {
			return false;
		}
		break;

		//幅つき円弧
	case WORK_AREA_TYPE_ARC://幅つき円弧
		return false;
		//break;

		//多角形
	case WORK_AREA_TYPE_POLYGON://多角形
		break;

		//領域なし
	default:
		return false;
	}

	return true;
}



