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
//画像メモリへ直接描画
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "opencv_headers.h"
#include "PIMPOM_API.h"
#include "Labeling.h"


// *************************************
//         マ  ク  ロ   定   義         
// *************************************
#define		BIGGER(A,B)			( ((A)>(B)) ? (A):(B) )
#define		SMALLER(A,B)		( ((A)<(B)) ? (A):(B) )

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//テンプレートで型なし(全ての型の画像に対応する)関数をつくる
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//画像メモリ上に直線描画
template <class DATA>
void	draw_line_on_image(CPimpomAPI *pAPI, long image_num, double x0, double y0, double x1, double y1, int pen_size, DATA dat)
{
	int			r,g,b;
	double		dir[2], len;
	double xc,yc,x,y;
	CDataUnit *p_du;

	if( (p_du = pAPI->GetDataUnit(image_num))==NULL)	return;

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		//R/G/Bの各値
		r = (COLORREF)dat % 0x100;
		g = ((COLORREF)dat / 0x100) % 0x100;
		b = ((COLORREF)dat / 0x100 / 0x100) % 0x100;

		//グレー画像が存在するメモリに対して、DATA=RGBREFでこの関数を呼んだ場合の対応
		if (!p_du->DispRGB) {
			dat = (DATA)r;//rの値をグレー値として使う
		}
	}



	//始点・終点間距離
	len = sqrt((double)((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0)));
	dir[0] = (double)(x1 - x0)/len;
	dir[1] = (double)(y1 - y0)/len;

	//描画
	for(double d=0 ; d<=len ; d+=0.5)
	{
		xc = x0 + dir[0]*d ;
		yc = y0 + dir[1]*d ;
		for(double w=-pen_size/2 ; w<=pen_size/2 ; w++)
		{
			x = xc - dir[1]*w;
			y = yc + dir[0]*w;
			int i = (int)(x + 0.5);
			int j = (int)(y + 0.5);

			if(i>=0 && j>=0 && i<p_du->DataSize.cx && j<p_du->DataSize.cy)
			{
				if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
				{//RGB
					p_du->SetDataValue(i, j, r, -1, 0);
					p_du->SetDataValue(i, j, g, -1, 1);
					p_du->SetDataValue(i, j, b, -1, 2);
				}else{//モノクロ
					p_du->SetDataValue(i, j, dat, -1, -1);
				}
			}
		}
	}
}


//画像メモリ上に円描画
template <class DATA>
void	draw_circle_on_image(CPimpomAPI *pAPI, long image_num, double cx, double cy, double rad, DATA dat)
{
	int			r,g,b;
	double dt;
	CDataUnit *p_du;

	if( (p_du = pAPI->GetDataUnit(image_num))==NULL)	return;
	if( rad<=0 )	return;

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		//R/G/Bの各値
		r = (COLORREF)dat % 0x100;
		g = ((COLORREF)dat / 0x100) % 0x100;
		b = ((COLORREF)dat / 0x100 / 0x100) % 0x100;

		//グレー画像が存在するメモリに対して、DATA=RGBREFでこの関数を呼んだ場合の対応
		if (!p_du->DispRGB) {
			dat = (DATA)r;//rの値をグレー値として使う
		}
	}

	dt = 0.5/rad;//円弧刻み角度(円周上で0.5画素刻み)

	//描画
	for(double t=0 ; t<=2*PI+dt ; t+=dt)
	{
		double x = rad*cos(t) + cx;
		double y = rad*sin(t) + cy;
		int i= (int)(x+0.5);
		int j= (int)(y+0.5);

		if(i>=0 && j>=0 && i<p_du->DataSize.cx && j<p_du->DataSize.cy)
		{
			if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
			{//RGB

				p_du->SetDataValue(i, j, r, -1, 0);
				p_du->SetDataValue(i, j, g, -1, 1);
				p_du->SetDataValue(i, j, b, -1, 2);
			}else{//モノクロ
				p_du->SetDataValue(i, j, dat, -1, -1);
			}
		}
	}
}


//画像メモリ上に塗りつぶし円描画
template <class DATA>
void	fill_circle_on_image(CPimpomAPI *pAPI, long image_num, double cx, double cy, double rad, DATA dat)
{
	int			r,g,b;
	CDataUnit *p_du;

	if( (p_du = pAPI->GetDataUnit(image_num))==NULL)	return;

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		//R/G/Bの各値
		r = (COLORREF)dat % 0x100;
		g = ((COLORREF)dat / 0x100) % 0x100;
		b = ((COLORREF)dat / 0x100 / 0x100) % 0x100;

		//グレー画像が存在するメモリに対して、DATA=RGBREFでこの関数を呼んだ場合の対応
		if (!p_du->DispRGB) {
			dat = (DATA)r;//rの値をグレー値として使う
		}
	}



	//描画
	for(int j=(int)(cy-rad-1.0) ; j<=(int)(cy+rad+1.0) ; j++)
	{
		for(int i=(int)(cx-rad-1.0) ; i<=(int)(cx+rad+1.0) ; i++)
		{
			if(i>=0 && j>=0 && i<p_du->DataSize.cx && j<p_du->DataSize.cy)
			{
				double dx = (double)i - cx;
				double dy = (double)j - cy;

				if( fabs(dx) < rad+1 && fabs(dy) < rad+1)
				{
					//円に外接する矩形領域内の画素
					double d = dx*dx + dy*dy;
					double radBorderThr = (rad+0.75)*(rad+0.75);
					double radInnerThr = (rad-0.75)*(rad-0.75);

					if( d < radBorderThr )
					{
						if( d < radInnerThr)
						{
							//円内部の画素 = 指定色で塗りつぶす
							if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
							{//RGB
								p_du->SetDataValue(i, j, r, -1, 0);
								p_du->SetDataValue(i, j, g, -1, 1);
								p_du->SetDataValue(i, j, b, -1, 2);
							}else{//モノクロ
								p_du->SetDataValue(i, j, dat, -1, -1);
							}
						}
						else
						{
							//円周境界線上の画素 = 中間色で塗りつぶす
							int innersubPixCnt=0, subPixCnt=0;
							double innerRatio;
							for(double sy=dy-0.5 ; sy<=dy+0.5 ; sy+=0.2){
								for(double sx=dx-0.5 ; sx<=dx+0.5 ; sx+=0.2){
									subPixCnt++;
									if(sx*sx + sy*sy < rad*rad){
										innersubPixCnt++;
									}
								}
							}
							innerRatio = (double)innersubPixCnt/(double)subPixCnt;//画素の円内部率


							if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
							{//RGB
								double bgr = p_du->GetDataValue(i, j, -1, 0);
								double bgg = p_du->GetDataValue(i, j, -1, 1);
								double bgb = p_du->GetDataValue(i, j, -1, 2);

								double valr = (double)r*innerRatio +bgr*(1.0-innerRatio);
								double valg = (double)g*innerRatio +bgg*(1.0-innerRatio);
								double valb = (double)b*innerRatio +bgb*(1.0-innerRatio);

								p_du->SetDataValue(i, j, valr, -1, 0);
								p_du->SetDataValue(i, j, valg, -1, 1);
								p_du->SetDataValue(i, j, valb, -1, 2);
							}else{//モノクロ
								double bg = p_du->GetDataValue(i, j, -1, -1);
								double val = (double)dat*innerRatio +bg*(1.0-innerRatio);
								p_du->SetDataValue(i, j, val, -1, -1);
							}
						}
					}
				}
			}
		}
	}
}


//画像メモリ上に矩形描画
template <class DATA>
void	draw_rect_on_image(CPimpomAPI *pAPI, long image_num, double x0, double y0, double x1, double y1, DATA dat, bool fill, DATA fill_dat)
{
	int			r,g,b;
	double		tmp;
	CDataUnit *p_du;

	if( (p_du = pAPI->GetDataUnit(image_num))==NULL)	return;

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		//R/G/Bの各値
		r = (COLORREF)fill_dat % 0x100;
		g = ((COLORREF)fill_dat / 0x100) % 0x100;
		b = ((COLORREF)fill_dat / 0x100 / 0x100) % 0x100;

		//グレー画像が存在するメモリに対して、DATA=RGBREFでこの関数を呼んだ場合の対応
		if (!p_du->DispRGB) {
			dat = (DATA)r;//rの値をグレー値として使う
			fill_dat = (DATA)r;
		}
	}



	//始点終点座標の大小修正
	if(x0>x1){
		tmp = x0;	x0=x1;	x1=tmp;
	}

	if(y0>y1){
		tmp = y0;	y0=y1;	y1=tmp;
	}






	//内部塗りつぶし
	if(fill)
	{
		int			x0i, x1i, y0i, y1i;//内部の座標
		x0i = (int)x0+1;
		y0i = (int)y0+1;
		x1i = (int)x1;
		y1i = (int)y1;


		//内部
		for(int j=y0i ; j<=y1i ; j++)
		{
			for(int i=x0i ; i<=x1i ; i++)
			{
				if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
				{//RGB
					p_du->SetDataValue(i, j, r, -1, 0);
					p_du->SetDataValue(i, j, g, -1, 1);
					p_du->SetDataValue(i, j, b, -1, 2);
				}else{//モノクロ
					p_du->SetDataValue(i, j, fill_dat, -1, -1);
				}
			}
		}

		//上辺近傍(左上、右上角含む)
		{
			double bg,fg;
			bg = y0-(int)y0;
			fg = 1.0 - bg;

			for(int i=x0i-1 ; i<=x1i+1 ; i++)
			{
				double bg2, fg2;
				if(i==x0i-1){//左上角
					fg2 = fg*( (int)x0 + 1.0 - x0 );
					bg2 = 1.0 - fg2;
				}else if(i==x1i+1){//右上角
					fg2 = fg*( x1 - (int)x1 );
					bg2 = 1.0 - fg2;
				}else{
					fg2 = fg;
					bg2 = bg;
				}

				if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
				{//RGB
					double bg_r = p_du->GetDataValue(i, (int)y0, -1, 0);
					double bg_g = p_du->GetDataValue(i, (int)y0, -1, 1);
					double bg_b = p_du->GetDataValue(i, (int)y0, -1, 2);

					p_du->SetDataValue(i, (int)y0, r*fg2 + bg_r*bg2 , -1, 0);
					p_du->SetDataValue(i, (int)y0, g*fg2 + bg_g*bg2 , -1, 1);
					p_du->SetDataValue(i, (int)y0, b*fg2 + bg_b*bg2 , -1, 2);
				}else{//モノクロ
					double bg_val = p_du->GetDataValue(i, (int)y0, -1, -1);
					p_du->SetDataValue(i, (int)y0, fill_dat*fg2 + bg_val*bg2, -1, -1);
				}
			}
		}
		
		//左辺近傍
		{
			double bg,fg;
			bg = x0-(int)x0;
			fg = 1.0 - bg;

			for(int j=y0i ; j<=y1i ; j++)
			{
				if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
				{//RGB
					double bg_r = p_du->GetDataValue((int)x0, j, -1, 0);
					double bg_g = p_du->GetDataValue((int)x0, j, -1, 1);
					double bg_b = p_du->GetDataValue((int)x0, j, -1, 2);

					p_du->SetDataValue((int)x0, j, r*fg + bg_r*bg , -1, 0);
					p_du->SetDataValue((int)x0, j, g*fg + bg_g*bg , -1, 1);
					p_du->SetDataValue((int)x0, j, b*fg + bg_b*bg , -1, 2);
				}else{//モノクロ
					double bg_val = p_du->GetDataValue((int)x0, j, -1, -1);
					p_du->SetDataValue((int)x0, j, fill_dat*fg + bg_val*bg , -1, -1);
				}
			}
		}

		//下辺近傍(左下、右下角含む)
		{
			double bg,fg;
			fg = y1-(int)y1;
			bg = 1.0 - fg;

			for(int i=x0i-1 ; i<=x1i+1 ; i++)
			{
				double bg2, fg2;
				if(i==x0i-1){//左上角
					fg2 = fg*( (int)x0 + 1.0 - x0 );
					bg2 = 1.0 - fg2;
				}else if(i==x1i+1){//右上角
					fg2 = fg*( x1 - (int)x1 );
					bg2 = 1.0 - fg2;
				}else{
					fg2 = fg;
					bg2 = bg;
				}

				if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
				{//RGB
					double bg_r = p_du->GetDataValue(i, (int)y1+1, -1, 0);
					double bg_g = p_du->GetDataValue(i, (int)y1+1, -1, 1);
					double bg_b = p_du->GetDataValue(i, (int)y1+1, -1, 2);

					p_du->SetDataValue(i, (int)y1+1, r*fg2 + bg_r*bg2 , -1, 0);
					p_du->SetDataValue(i, (int)y1+1, g*fg2 + bg_g*bg2 , -1, 1);
					p_du->SetDataValue(i, (int)y1+1, b*fg2 + bg_b*bg2 , -1, 2);
				}else{//モノクロ
					double bg_val = p_du->GetDataValue(i, (int)y1+1, -1, -1);
					p_du->SetDataValue(i, (int)y1+1, fill_dat*fg2 + bg_val*bg2, -1, -1);
				}
			}
		}

		//右辺近傍
		{
			double bg,fg;
			fg = x1-(int)x1;
			bg = 1.0 - fg;

			for(int j=y0i ; j<=y1i ; j++)
			{
				if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
				{//RGB
					double bg_r = p_du->GetDataValue((int)x1+1, j, -1, 0);
					double bg_g = p_du->GetDataValue((int)x1+1, j, -1, 1);
					double bg_b = p_du->GetDataValue((int)x1+1, j, -1, 2);

					p_du->SetDataValue((int)x1+1, j, r*fg + bg_r*bg , -1, 0);
					p_du->SetDataValue((int)x1+1, j, g*fg + bg_g*bg , -1, 1);
					p_du->SetDataValue((int)x1+1, j, b*fg + bg_b*bg , -1, 2);
				}else{//モノクロ
					double bg_val = p_du->GetDataValue((int)x1+1, j, -1, -1);
					p_du->SetDataValue((int)x1+1, j, fill_dat*fg + bg_val*bg , -1, -1);
				}
			}
		}

		//左上

	}
	else
	{
		//外枠のみ
		draw_line_on_image(pAPI, image_num, x0, y0, x1, y0, 1, dat);//上
		draw_line_on_image(pAPI, image_num, x1, y0, x1, y1, 1, dat);//右
		draw_line_on_image(pAPI, image_num, x1, y1, x0, y1, 1, dat);//下
		draw_line_on_image(pAPI, image_num, x0, y1, x0, y0, 1, dat);//左
	}
}





//画像メモリ上にテキスト描画
template <class DATA>
void draw_text_on_image(CPimpomAPI *pAPI, long image_num, int x, int y, char str[], DATA dat )
{
	int			imgx,imgy;
	int			r,g,b;
	CDataUnit *p_du;

	if( (p_du = pAPI->GetDataUnit(image_num))==NULL)	return;

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		//R/G/Bの各値
		r = (COLORREF)dat % 0x100;
		g = ((COLORREF)dat / 0x100) % 0x100;
		b = ((COLORREF)dat / 0x100 / 0x100) % 0x100;

		//グレー画像が存在するメモリに対して、DATA=RGBREFでこの関数を呼んだ場合の対応
		if (!p_du->DispRGB) {
			dat = (DATA)r;//rの値をグレー値として使う
		}
	}





	//キャンバスに文字を書き込む
	int fontFace = cv::FONT_HERSHEY_SIMPLEX;
	float scale = (float)pAPI->default_font.lfHeight / (float)22;
	int thickness = 1;
	int baseline = 0;
	cv::Size textSize = getTextSize(cv::String(str), fontFace, scale, thickness, &baseline);
	cv::Mat canvas = cv::Mat::zeros(textSize.height, textSize.width, CV_8UC1);
	cv::putText(canvas, cv::String(str), cv::Point(0, textSize.height-1), fontFace, scale, cv::Scalar(255), thickness, cv::LINE_8);

	//画像データにキャンバスをコピーする
	for (int j = 0; j<canvas.rows; j++)
	{
		for (int i = 0; i<canvas.cols; i++)
		{
			imgx = x + i;
			imgy = y + j;

			if (canvas.at<unsigned char>(j,i) > 0)
			{
				if ((p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT) && p_du->DispRGB)
				{//RGB
					p_du->SetDataValue(imgx, imgy, r, -1, 0);
					p_du->SetDataValue(imgx, imgy, g, -1, 1);
					p_du->SetDataValue(imgx, imgy, b, -1, 2);
				}
				else {//モノクロ
					p_du->SetDataValue(imgx, imgy, dat, -1, -1);
				}
			}
		}
	}



}


//画像メモリ上にドット描画
template <class DATA>
void draw_point_on_image(CPimpomAPI *pAPI, long image_num, CPoint point, int dot_size, DATA dat, bool tomask)
{
	int			r,g,b;
	int			dot_size_half = dot_size/2;
	CDataUnit *p_du;

	if( (p_du = pAPI->GetDataUnit(image_num))==NULL)	return;

	//ドット位置が画像上にあるかチェック
	if(point.x<0 || point.y<0 || point.x >= p_du->DataSize.cx || point.y >= p_du->DataSize.cy)	return ;

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		//R/G/Bの各値
		r = (COLORREF)dat % 0x100;
		g = ((COLORREF)dat / 0x100) % 0x100;
		b = ((COLORREF)dat / 0x100 / 0x100) % 0x100;

		//RGBに対して、DATA=RGBREFでこの関数を呼んだ場合の対応
		if (!p_du->DispRGB) {
			dat = (DATA)r;//rの値をグレー値として使う
		}
	}



	////グレー画像が存在するメモリに対して、この関数を呼んだ場合の対応
	//if( !( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB) ){
	//	dat = (DATA)r;//rの値をグレー値として使う
	//}


	for(int j=-dot_size_half ; j<=dot_size_half ; j++)
	{
		for(int i=-dot_size_half ; i<=dot_size_half ; i++)
		{
			if(i*i+j*j<=dot_size_half*dot_size_half)//円形ドット
			{
				CPoint pos(point.x+i, point.y+j);

				if( !p_du->DoUseWorkArea || p_du->WorkArea.PtInWorkArea(pos) )//作業領域に入っているかどうかﾁｪｯｸ
				{
					if(!tomask)
					{
						if( !p_du->DoUseMask || p_du->GetMaskValue(pos)==UNMASKED_PIXEL )//マスクがかかっているかどうかﾁｪｯｸ
						if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
						{//RGB
							p_du->SetDataValue(pos.x, pos.y, r, -1,0);
							p_du->SetDataValue(pos.x, pos.y, g, -1,1);
							p_du->SetDataValue(pos.x, pos.y, b, -1,2);
						}else{//モノクロ
							p_du->SetDataValue(pos.x, pos.y, (double)dat, -1, -1);
						}
					}else{//マスク
						p_du->SetMaskValue(pos.x, pos.y, (BYTE)dat);
					}
				}
			}
		}
	}
}



//画像メモリ上に塗りつぶし描画
template <class DATA>
void flood_fill_on_image(CPimpomAPI *pAPI, long image_num, CPoint point, DATA dat, bool tomask)
{
	int			r,g,b;
	CDataUnit	*p_du;
	CRect		calc_area;

	if( (p_du = pAPI->GetDataUnit(image_num))==NULL)	return;
	calc_area = p_du->GetRectArea();//処理対象の矩形範囲

	//塗りつぶし開始位置が画像上にあるかチェック
	if(point.x<0 || point.y<0 || point.x >= p_du->DataSize.cx || point.y >= p_du->DataSize.cy)	return ;

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		//R/G/Bの各値
		r = (COLORREF)dat % 0x100;
		g = ((COLORREF)dat / 0x100) % 0x100;
		b = ((COLORREF)dat / 0x100 / 0x100) % 0x100;

		//グレー画像が存在するメモリに対して、DATA=RGBREFでこの関数を呼んだ場合の対応
		if (!p_du->DispRGB) {
			dat = (DATA)r;//rの値をグレー値として使う
		}
	}



	//塗りつぶし領域をラベリングで決める/////////////////////////////////////
	LabelingBS	labeling;
	unsigned char *pbinary;
	short *plabel;
	short fill_label_no;

	pbinary = (unsigned char*)malloc( p_du->DataSize.cx * p_du->DataSize.cy );//２値化画像
	if(pbinary==NULL)	return ;
	memset(pbinary,0,p_du->DataSize.cx * p_du->DataSize.cy);

	plabel = (short*)malloc( p_du->DataSize.cx * p_du->DataSize.cy * sizeof(short));//ラベル画像
	if(plabel==NULL){
		free(plabel);
		return ;
	}


	if(!tomask)
	{
		if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
		{//RGB
			//選択画素の色をピックして、同色だけの２値化画像をつくる
			double valr,valg,valb;
			valr = p_du->GetDataValue(point, -1, 0);
			valg = p_du->GetDataValue(point, -1, 1);
			valb = p_du->GetDataValue(point, -1, 2);
			for(int j=calc_area.top ; j<=calc_area.bottom ; j++)
			{
				for(int i=calc_area.left ; i<=calc_area.right ; i++)
				{
					if( !p_du->DoUseWorkArea || p_du->WorkArea.PtInWorkArea(CPoint(i,j)) )//作業領域に入っているかどうかﾁｪｯｸ
					if( !p_du->DoUseMask || p_du->GetMaskValue(CPoint(i,j))==UNMASKED_PIXEL )//マスクがかかっているかどうかﾁｪｯｸ
					if( p_du->GetDataValue(CPoint(i,j),-1,0) == valr && //同じ色であるかどうか確認
						p_du->GetDataValue(CPoint(i,j),-1,1) == valg && 
						p_du->GetDataValue(CPoint(i,j),-1,2) == valb 
						){
							*(pbinary + i + j*p_du->DataSize.cx) = 1;
						}
				}
			}

			//ラベリングして、塗りつぶすラベル番号をとる
			labeling.Exec( pbinary, plabel, p_du->DataSize.cx, p_du->DataSize.cy, true, 0 );
			fill_label_no = *(plabel + point.x + point.y*p_du->DataSize.cx);

			//ぬりつぶす
			for(int j=calc_area.top ; j<=calc_area.bottom ; j++)
			{
				for(int i=calc_area.left ; i<=calc_area.right ; i++)
				{
					if( *(plabel + i + j*p_du->DataSize.cx) == fill_label_no){
						p_du->SetDataValue(i, j, r, -1,0);
						p_du->SetDataValue(i, j, g, -1,1);
						p_du->SetDataValue(i, j, b, -1,2);
					}
				}
			}
		}
		else
		{//モノクロ
			//選択画素の色をピックして、同色だけの２値化画像をつくる
			DATA val = (DATA)p_du->GetDataValue(point,-1,-1);
			for(int j=calc_area.top ; j<=calc_area.bottom ; j++)
			{
				for(int i=calc_area.left ; i<=calc_area.right ; i++)
				{
					if( !p_du->DoUseWorkArea || p_du->WorkArea.PtInWorkArea(CPoint(i,j)) )//作業領域に入っているかどうかﾁｪｯｸ
					if( !p_du->DoUseMask || p_du->GetMaskValue(CPoint(i,j))==UNMASKED_PIXEL )//マスクがかかっているかどうかﾁｪｯｸ
					if( p_du->GetDataValue(CPoint(i,j),-1,-1) == val){//同じ値であるかどうか確認
						*(pbinary + i + j*p_du->DataSize.cx) = 1;
					}else{
						*(pbinary + i + j*p_du->DataSize.cx) = 0;
					}
				}
			}

			//ラベリングして、塗りつぶすラベル番号をとる
			labeling.Exec( pbinary, plabel, p_du->DataSize.cx, p_du->DataSize.cy, true, 0 );
			fill_label_no = *(plabel + point.x + point.y*p_du->DataSize.cx);

			//ぬりつぶす
			for(int j=calc_area.top ; j<=calc_area.bottom ; j++)
			{
				for(int i=calc_area.left ; i<=calc_area.right ; i++)
				{
					if( *(plabel + i + j*p_du->DataSize.cx) == fill_label_no){
						p_du->SetDataValue(i, j, dat, -1, -1);
					}
				}
			}
		}
	}
	else
	{//マスク画像
			//選択画素の色をピックして、同色だけの２値化画像をつくる
			BYTE val = p_du->GetMaskValue(point);
			for(int j=calc_area.top ; j<=calc_area.bottom ; j++)
			{
				for(int i=calc_area.left ; i<=calc_area.right ; i++)
				{
					if( !p_du->DoUseWorkArea || p_du->WorkArea.PtInWorkArea(CPoint(i,j)) )//作業領域に入っているかどうかﾁｪｯｸ
					if( p_du->GetMaskValue(CPoint(i,j)) == val){//同じ値であるかどうか確認
						*(pbinary + i + j*p_du->DataSize.cx) = 1;
					}else{
						*(pbinary + i + j*p_du->DataSize.cx) = 0;
					}
				}
			}

			//ラベリングして、塗りつぶすラベル番号をとる
			labeling.Exec( pbinary, plabel, p_du->DataSize.cx, p_du->DataSize.cy, true, 0 );
			fill_label_no = *(plabel + point.x + point.y*p_du->DataSize.cx);

			//ぬりつぶす
			for(int j=calc_area.top ; j<=calc_area.bottom ; j++)
			{
				for(int i=calc_area.left ; i<=calc_area.right ; i++)
				{
					if( *(plabel + i + j*p_du->DataSize.cx) == fill_label_no){
						p_du->SetMaskValue(i, j, (BYTE)dat);
					}
				}
			}
	}

	free(plabel);
	free(pbinary);

}

//作業領域内部塗りつぶし
template <class DATA>
void fill_workarea_on_image(CPimpomAPI *pAPI, long image_num, DATA dat, bool tomask)
{
	int			r,g,b;
	CDataUnit	*p_du;
	CRect		calc_area;

	if( (p_du = pAPI->GetDataUnit(image_num))==NULL)	return;
	if( !p_du->DoUseWorkArea )	return;//作業領域非表示の場合はなにもしない

	calc_area = p_du->GetRectArea();//処理対象の矩形範囲

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		//R/G/Bの各値
		r = (COLORREF)dat % 0x100;
		g = ((COLORREF)dat / 0x100) % 0x100;
		b = ((COLORREF)dat / 0x100 / 0x100) % 0x100;

		//グレー画像が存在するメモリに対して、DATA=RGBREFでこの関数を呼んだ場合の対応
		if (!p_du->DispRGB) {
			dat = (DATA)r;//rの値をグレー値として使う
		}
	}



	for(int j=calc_area.top ; j<=calc_area.bottom ; j++)
	{
		for(int i=calc_area.left ; i<=calc_area.right ; i++)
		{
			if( p_du->WorkArea.PtInWorkArea(CPoint(i,j)) )//作業領域に入っているかどうかﾁｪｯｸ
			{
				if(!tomask)
				{
					if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
					{//RGB
						p_du->SetDataValue(i, j, r, -1,0);
						p_du->SetDataValue(i, j, g, -1,1);
						p_du->SetDataValue(i, j, b, -1,2);
					}else{//モノクロ
						p_du->SetDataValue(i, j, dat, -1, -1);
					}
				}
				else//マスク
				{
					p_du->SetMaskValue(i, j, (BYTE)dat);
				}
			}
		}
	}
}




/********************************************************************
機  能  名  称 : 画像メモリ上に直線描画（RGB型、RGB 3D型の画像メモリに対応）
関    数    名 : DrawLineOnImage
引          数 : int		image_num		(in)画像メモリ番号
				 double		x0,y0			(in)始点座標
				 double		x1,y1			(in)終点座標
				 int		pen_size		(in)描画するペンサイズ
				 COLORREF	col				(in)描画色
				 int		copy_image_num	(in)コピーする画像番号
戻    り    値 : 
機          能 : 複数ページ/チャネルがある場合は表示中のものに描画
                 copy_image_numに画像番号を指定すると、背景として画像をコピーしてからその上に描画する
				 グレー画像が存在するメモリに対してこの関数を呼んだ場合は, 赤成分を画素値として使う
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/

void	CPimpomAPI::DrawLineOnImage(long image_num, double x0, double y0, double x1, double y1, int pen_size, COLORREF col, int copy_image_num)
{
	if(copy_image_num>=0 && copy_image_num<GetDataUnitNumber())
	{
		CopyDataUnit(copy_image_num, image_num);
		CONVERT_DATA_OPTION	option;
		option.type_to = RGB_FORMAT;
		option.rgb_to = RGB_TO_ALL;
		ConvertDataUnit(image_num,option);	
	}

	draw_line_on_image(this, image_num,x0,y0,x1,y1,pen_size,col);
}


/********************************************************************
機  能  名  称 : 画像メモリ上に直線描画（グレースケールの画像メモリに対応）
関    数    名 : DrawLineOnImage
引          数 : int		image_num		(in)画像メモリ番号
				 double		x0,y0			(in)始点座標
				 double		x1,y1			(in)終点座標
				 int		pen_size		(in)描画するペンサイズ
				 double		val				(in)描画値
				 int		copy_image_num	(in)コピーする画像番号
戻    り    値 : 
機          能 : 複数ページ/チャネルがある場合は表示中のものに描画

日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawLineOnImage(long image_num, double x0, double y0, double x1, double y1, int pen_size, double val)
{
	draw_line_on_image(this, image_num,x0,y0,x1,y1,pen_size,val);
}


/********************************************************************
機  能  名  称 : 画像メモリ上に直線描画（RGB型、RGB 3D型の画像メモリに対応）
関    数    名 : DrawLineOnImage
引          数 : int		image_num		(in)画像メモリ番号
				 double		a,b,c			(in)直線方程式(ax+by+c=0)
				 int		pen_size		(in)描画するペンサイズ
				 COLORREF	col				(in)描画色
				 int		copy_image_num	(in)コピーする画像番号
戻    り    値 : 
機          能 : 複数ページ/チャネルがある場合は表示中のものに描画
                 copy_image_numに画像番号を指定すると、背景として画像をコピーしてからその上に描画する
				 グレー画像が存在するメモリに対してこの関数を呼んだ場合は, 赤成分を画素値として使う
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/

void	CPimpomAPI::DrawLineOnImageF(long image_num, double a, double b, double c, int pen_size, COLORREF col, int copy_image_num)
{
	if(copy_image_num>=0 && copy_image_num<GetDataUnitNumber())
	{
		CopyDataUnit(copy_image_num, image_num);
		CONVERT_DATA_OPTION	option;
		option.type_to = RGB_FORMAT;
		option.rgb_to = RGB_TO_ALL;
		ConvertDataUnit(image_num,option);	
	}

	CDataUnit *pdu = GetDataUnit(image_num);
	if(!pdu)	return;

	double x0,y0,x1,y1;
	if( !ClipSegmentF( CRect(0,0,pdu->DataSize.cx-1,pdu->DataSize.cy-1), a, b, c, &x0, &y0, &x1, &y1) )	return;

	draw_line_on_image(this, image_num,x0,y0,x1,y1,pen_size,col);
}



/********************************************************************
機  能  名  称 : 画像メモリ上に直線描画（グレースケールの画像メモリに対応）
関    数    名 : DrawLineOnImage
引          数 : int		image_num		(in)画像メモリ番号
				 double		a,b,c			(in)直線方程式(ax+by+c=0)
				 int		pen_size		(in)描画するペンサイズ
				 double		val				(in)描画値
				 int		copy_image_num	(in)コピーする画像番号
戻    り    値 : 
機          能 : 複数ページ/チャネルがある場合は表示中のものに描画

日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawLineOnImageF(long image_num, double a, double b, double c, int pen_size, double val)
{
	CDataUnit *pdu = GetDataUnit(image_num);
	if(!pdu)	return;

	double x0,y0,x1,y1;
	if( !ClipSegmentF( CRect(0,0,pdu->DataSize.cx-1,pdu->DataSize.cy-1), a, b, c, &x0, &y0, &x1, &y1) )	return;

	draw_line_on_image(this, image_num,x0,y0,x1,y1,pen_size,val);
}


/********************************************************************
機  能  名  称 : 画像メモリ上に十字カーソル描画（RGB型、RGB 3D型の画像メモリに対応）
関    数    名 : DrawCursorOnImage
引          数 : int		image_num		(in)画像メモリ番号
				 double		x,y				(in)座標
				 int		pen_size		(in)描画するペンサイズ
				 COLORREF	col				(in)描画色
				 int		copy_image_num	(in)コピーする画像番号
戻    り    値 : 
機          能 : 複数ページ/チャネルがある場合は表示中のものに描画
                 copy_image_numに画像番号を指定すると、背景として画像をコピーしてからその上に描画する
				 グレー画像が存在するメモリに対してこの関数を呼んだ場合は, 赤成分を画素値として使う
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawCursorOnImage(long image_num, double x, double y, int pen_size, COLORREF col, int copy_image_num)
{
	long		mark_length = 20;//マークのラインの長さ

	DrawLineOnImage(image_num, (double)(x - mark_length/2) , y , (double)(x + mark_length/2) , y, pen_size, col, copy_image_num);
	DrawLineOnImage(image_num, x , (double)(y - mark_length/2) , x , (double)(y + mark_length/2), pen_size, col);
}


/********************************************************************
機  能  名  称 : 画像メモリ上に十字カーソル描画（グレースケールの画像メモリに対応）
関    数    名 : DrawCursorOnImage
引          数 : int		image_num		(in)画像メモリ番号
				 double		x,y			(in)座標
				 int		pen_size		(in)描画するペンサイズ
				 double		val				(in)描画値
				 int		copy_image_num	(in)コピーする画像番号
戻    り    値 : 
機          能 : 複数ページ/チャネルがある場合は表示中のものに描画

日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawCursorOnImage(long image_num, double x, double y, int pen_size, double val)
{
	long		mark_length = 20;//マークのラインの長さ

	DrawLineOnImage(image_num, (double)(x - mark_length/2) , y , (double)(x + mark_length/2) , y, pen_size, val);
	DrawLineOnImage(image_num, x , (double)(y - mark_length/2) , x , (double)(y + mark_length/2), pen_size, val);
}


/********************************************************************
機  能  名  称 : 画像メモリ上に円描画（RGB型、RGB 3D型の画像メモリに対応）
関    数    名 : DrawCircleOnImage
引          数 : int		image_num		(in)画像メモリ番号
				 double		cx,cy			(in)中心座標
				 double		rad				(in)半径
				 COLORREF	col				(in)描画色
				 bool		fill			(in)中を塗りつぶすかどうか
				 int		copy_image_num	(in)コピーする画像番号
戻    り    値 : 
機          能 : 複数ページ/チャネルがある場合は表示中のものに描画
                 copy_image_numに画像番号を指定すると、背景として画像をコピーしてからその上に描画する
				 グレー画像が存在するメモリに対してこの関数を呼んだ場合は, 赤成分を画素値として使う
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
//RGB型、RGB 3D型の画像メモリに対応
void	CPimpomAPI::DrawCircleOnImage(long image_num, double cx, double cy, double rad, COLORREF col, bool fill, int copy_image_num)
{
	if(copy_image_num>=0 && copy_image_num<GetDataUnitNumber()){
		CopyDataUnit(copy_image_num, image_num);
		CONVERT_DATA_OPTION	option;
		option.type_to = RGB_FORMAT;
		option.rgb_to = RGB_TO_ALL;
		ConvertDataUnit(image_num,option);	
	}

	if(fill){
		fill_circle_on_image(this, image_num,cx,cy,rad,col);
	}else{
		draw_circle_on_image(this, image_num,cx,cy,rad,col);
	}
}

/********************************************************************
機  能  名  称 : 画像メモリ上に円描画（グレースケールの画像メモリに対応）
関    数    名 : DrawCircleOnImage
引          数 : int		image_num		(in)画像メモリ番号
				 double		cx,cy			(in)中心座標
				 double		rad				(in)半径
				 double		val				(in)描画値
				 bool		fill			(in)中を塗りつぶすかどうか
				 int		copy_image_num	(in)コピーする画像番号
戻    り    値 : 
機          能 : 複数ページ/チャネルがある場合は表示中のものに描画
      
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawCircleOnImage(long image_num, double cx, double cy, double rad, double val, bool fill)
{
	if(fill){
		fill_circle_on_image(this, image_num,cx,cy,rad,val);
	}else{
		draw_circle_on_image(this, image_num,cx,cy,rad,val);
	}
}


/********************************************************************
機  能  名  称 : 画像メモリ上に矩形描画(RGB型、RGB 3D型の画像メモリに対応)
関    数    名 : DrawRectOnImage
引          数 : int		image_num	(in)画像メモリ番号
				double		x0			(in)左辺座標
				double		y0			(in)右辺座標
				double		x1			(in)上辺座標
				double		y1			(in)下辺座標
				int			pen_size	(in)描画するペンサイズ
				COLORREF	col			(in)描画色
				bool		fill		(in)中を塗りつぶすかどうか
				COLORREF	fill_col	(in)中を塗りつぶす色
				int		copy_image_num	(in)コピーする画像番号
戻    り    値 : 
機          能 : 複数ページ/チャネルがある場合は表示中のものに描画
                 copy_image_numに画像番号を指定すると、背景として画像をコピーしてからその上に描画する
				 グレー画像が存在するメモリに対してこの関数を呼んだ場合は, 赤成分を画素値として使う
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawRectOnImage(long image_num, double x0, double y0, double x1, double y1, COLORREF col, bool fill, COLORREF fill_col, int copy_image_num)
{
	if(copy_image_num>=0 && copy_image_num<GetDataUnitNumber()){
		CopyDataUnit(copy_image_num, image_num);
		CONVERT_DATA_OPTION	option;
		option.type_to = RGB_FORMAT;
		option.rgb_to = RGB_TO_ALL;
		ConvertDataUnit(image_num,option);	
	}

	draw_rect_on_image(this, image_num, x0,y0,x1,y1,col, fill, fill_col);
}


/********************************************************************
機  能  名  称 : 画像メモリ上に矩形描画（グレースケールの画像メモリに対応）
関    数    名 : DrawRectOnImage
引          数 : int		image_num	(in)画像メモリ番号
				double		x0			(in)左辺座標
				double		y0			(in)右辺座標
				double		x1			(in)上辺座標
				double		y1			(in)下辺座標
				int			pen_size	(in)描画するペンサイズ
				double		val			(in)描画値
				bool		fill		(in)中を塗りつぶすかどうか
				double		fill_val	(in)中を塗りつぶす値
				int		copy_image_num	(in)コピーする画像番号
戻    り    値 : 
機          能 : 複数ページ/チャネルがある場合は表示中のものに描画

日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
//RGB型、RGB 3D型以外の画像メモリに対応
void	CPimpomAPI::DrawRectOnImage(long image_num, double x0, double y0, double x1, double y1, double val, bool fill, double fill_val)
{
	draw_rect_on_image(this, image_num, x0,y0,x1,y1,val, fill, fill_val);
}


/********************************************************************
機  能  名  称 : 画像メモリ上にテキスト描画(RGB型、RGB 3D型の画像メモリに対応)
関    数    名 : DrawTextOnImage
引          数 : int		image_num		(in)画像メモリ番号
				 int		x,y				(in)描画座標
				 char		str[],			(in)テキスト
				 COLORREF	col				(in)描画色
				 int		copy_image_num	(in)コピーする画像番号
戻    り    値 : 
機          能 : ASCIIコードのみ対応
				 複数ページ/チャネルがある場合は表示中のものに描画
                 copy_image_numに画像番号を指定すると、背景として画像をコピーしてからその上に描画する
				 グレー画像が存在するメモリに対してこの関数を呼んだ場合は, 赤成分を画素値として使う
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
//RGB型、RGB 3D型の画像メモリに対応
void	CPimpomAPI::DrawTextOnImage(long image_num, int x, int y, char str[], COLORREF col, int copy_image_num)
{
	if(str==NULL)	return;

	if(copy_image_num>=0 && copy_image_num<GetDataUnitNumber()){
		CopyDataUnit(copy_image_num, image_num);
		CONVERT_DATA_OPTION	option;
		option.type_to = RGB_FORMAT;
		option.rgb_to = RGB_TO_ALL;
		ConvertDataUnit(image_num,option);	
	}

	draw_text_on_image(this, image_num, x, y, str, col);
}


/********************************************************************
機  能  名  称 : 画像メモリ上にテキスト描画（グレースケールの画像メモリに対応）
関    数    名 : DrawTextOnImage
引          数 : int		image_num		(in)画像メモリ番号
				 int		x,y				(in)描画座標
				 char		str[],			(in)テキスト
				 double		val				(in)描画値
				 int		copy_image_num	(in)コピーする画像番号
戻    り    値 : 
機          能 : ASCIIコードのみ対応
				複数ページ/チャネルがある場合は表示中のものに描画
 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawTextOnImage(long image_num, int x, int y, char str[], double val)
{
	draw_text_on_image(this, image_num, x, y, str, val);
}


/********************************************************************
機  能  名  称 : フォーマット指定で画像メモリ上にテキスト描画（RGB型、RGB 3D型の画像メモリに対応）
関    数    名 : DrawTextOnImage
引          数 : int		image_num	(in)画像メモリ番号
				 int		x,y			(in)描画座標
				 COLORREF	col			(in)描画色
				 char		*format, ...(in)テキスト
戻    り    値 : 
機          能 : ASCIIコードのみ対応
				複数ページ/チャネルがある場合は表示中のものに描画
				グレー画像が存在するメモリに対してこの関数を呼んだ場合は, 赤成分を画素値として使う
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawTextOnImage(long image_num, int x, int y, COLORREF col, char *format, ...)
{
	va_list list;
	char	buf[5000];

	va_start( list, format );
	vsprintf(buf,format, list);
	va_end( list );

	DrawTextOnImage(image_num,x,y,buf,col);
}


/********************************************************************
機  能  名  称 : フォーマット指定で画像メモリ上にテキスト描画（グレースケールの画像メモリに対応）
関    数    名 : DrawTextOnImage
引          数 : int		image_num	(in)画像メモリ番号
				 int		x,y			(in)描画座標
				 double		val			(in)描画値
				 char		*format, ...(in)テキスト
戻    り    値 : 
機          能 : ASCIIコードのみ対応
				複数ページ/チャネルがある場合は表示中のものに描画
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawTextOnImage(long image_num, int x, int y, double val, char *format, ...)
{
	va_list list;
	char	buf[5000];

	va_start( list, format );
	vsprintf(buf,format, list);
	va_end( list );

	DrawTextOnImage(image_num,x,y,buf,val);
}




/********************************************************************
機  能  名  称 : 画像メモリ上にドット描画（RGB型、RGB 3D型の画像メモリに対応）
関    数    名 : DrawPointOnImage
引          数 : int		image_num	(in)画像メモリ番号
				 int		x,y			(in)描画座標
				 int		dot_size	(in)ドットサイズ(直径)
				 COLORREF	col			(in)描画色
				 int		copy_image_num	(in)コピーする画像番号
戻    り    値 : 
機          能 : 複数ページ/チャネルがある場合は表示中のものに描画
				 ドット形状は円
                 copy_image_numに画像番号を指定すると、背景として画像をコピーしてからその上に描画する
				 グレー画像が存在するメモリに対してこの関数を呼んだ場合は, 赤成分を画素値として使う
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawPointOnImage(long image_num, int x, int y, int dot_size, COLORREF col, int copy_image_num)
{
	//カラー背景画像を新たにつくる場合の対応
	if(copy_image_num>=0 && copy_image_num<GetDataUnitNumber()){
		CopyDataUnit(copy_image_num, image_num);
		CONVERT_DATA_OPTION	option;
		option.type_to = RGB_FORMAT;
		option.rgb_to = RGB_TO_ALL;
		ConvertDataUnit(image_num,option);	
	}

	draw_point_on_image(this, image_num, CPoint(x,y), dot_size, col, false);
}


/********************************************************************
機  能  名  称 : 画像メモリ上にドット描画（グレースケールの画像メモリに対応）
関    数    名 : DrawPointOnImage
引          数 : int		image_num	(in)画像メモリ番号
				 int		x,y			(in)描画座標
				 int		dot_size	(in)ドットサイズ(直径)
				 double		val			(in)描画値
				 int		copy_image_num	(in)コピーする画像番号
戻    り    値 : 
機          能 : 複数ページ/チャネルがある場合は表示中のものに描画
					ドット形状は円
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawPointOnImage(long image_num, int x, int y, int dot_size, double val)
{
	draw_point_on_image(this, image_num, CPoint(x,y), dot_size, val, false);
}


/********************************************************************
機  能  名  称 : マスク上にドット描画
関    数    名 : DrawPointOnImage
引          数 : int		image_num	(in)画像メモリ番号
				 int		x,y			(in)描画座標
				 int		dot_size	(in)ドットサイズ
				 BYTE		val			(in)描画値　1:マスクあり 0:マスクなし それ以外の値：マスクなし
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawPointOnMask(long image_num, int x, int y, int dot_size, BYTE val)
{
	if(val!=MASKED_PIXEL && val!=UNMASKED_PIXEL)	val=UNMASKED_PIXEL;

	draw_point_on_image(this, image_num, CPoint(x,y), dot_size, val, true);
}


/********************************************************************
機  能  名  称 : 画像メモリ上で領域塗りつぶしをおこなう（RGB型、RGB 3D型の画像メモリに対応）
関    数    名 : DrawPointOnImage
引          数 : int		image_num	(in)画像メモリ番号
				 int		x,y			(in)始点座標
				 COLORREF	col			(in)塗りつぶし色
戻    り    値 : 
機          能 :始点座標と同じ色である領域を指定色で塗りつぶす 
				複数ページ/チャネルがある場合は表示中のもので実施
				グレー画像が存在するメモリに対してこの関数を呼んだ場合は, 赤成分を画素値として使う
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
//RGB型、RGB 3D型の画像メモリに対応
void	CPimpomAPI::FloodFillOnImage(long image_num, int x, int y, COLORREF col)
{
	flood_fill_on_image(this, image_num, CPoint(x,y), col, false);
}


/********************************************************************
機  能  名  称 : 画像メモリ上で領域塗りつぶしをおこなう（グレースケールの画像メモリに対応）
関    数    名 : DrawPointOnImage
引          数 : int		image_num	(in)画像メモリ番号
				 int		x,y			(in)始点座標
				 double		val			(in)塗りつぶし値
戻    り    値 : 
機          能 :始点座標と同じ色である領域を指定色で塗りつぶす 
				複数ページ/チャネルがある場合は表示中のもので実施
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::FloodFillOnImage(long image_num, int x, int y, double val)
{
	flood_fill_on_image(this, image_num, CPoint(x,y), val, false);
}

/********************************************************************
機  能  名  称 : マスクメモリ上で領域塗りつぶしをおこなう
関    数    名 : DrawPointOnImage
引          数 : int		image_num	(in)画像メモリ番号
				 int		x,y			(in)始点座標
				 BYTE		val			(in)描画値　1:マスクあり 0:マスクなし それ以外の値：マスクなし
戻    り    値 : 
機          能 :
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::FloodFillOnMask(long image_num, int x, int y, BYTE val)
{
	flood_fill_on_image(this, image_num, CPoint(x,y), val, true);
}

/********************************************************************
機  能  名  称 : 作業領域の画像データを指定色に塗りつぶす（RGB型、RGB 3D型の画像メモリに対応）
関    数    名 : DrawPointOnImage
引          数 : int		image_num	(in)画像メモリ番号
				 COLORREF	col			(in)塗りつぶし色
戻    り    値 : 
機          能 :始点座標と同じ色である領域を指定色で塗りつぶす 
				複数ページ/チャネルがある場合は表示中のもので実施
				グレー画像が存在するメモリに対してこの関数を呼んだ場合は, 赤成分を画素値として使う
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
//RGB型、RGB 3D型の画像メモリに対応
void	CPimpomAPI::FillWorkAreaOnImage(long image_num, COLORREF col)
{
	fill_workarea_on_image(this, image_num, col, false);
}


/********************************************************************
機  能  名  称 : 作業領域の画像データを指定色に塗りつぶす（グレースケールの画像メモリに対応）
関    数    名 : DrawPointOnImage
引          数 : int		image_num	(in)画像メモリ番号
				 double		val			(in)塗りつぶし値
戻    り    値 : 
機          能 :始点座標と同じ色である領域を指定色で塗りつぶす 
				複数ページ/チャネルがある場合は表示中のもので実施
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
//RGB型、RGB 3D型以外の画像メモリに対応
void	CPimpomAPI::FillWorkAreaOnImage(long image_num, double val)
{
	fill_workarea_on_image(this, image_num, val, false);
}

/********************************************************************
機  能  名  称 : 作業領域のマスクデータを塗りつぶす
関    数    名 : DrawPointOnImage
引          数 : int		image_num	(in)画像メモリ番号
				 BYTE		val			(in)描画値　1:マスクあり 0:マスクなし それ以外の値：マスクなし
戻    り    値 : 
機          能 :
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::FillWorkAreaOnMask(long image_num, BYTE val)
{
	fill_workarea_on_image(this, image_num, val, true);
}


/********************************************************************
機  能  名  称 : モノクロ画像をカラー画像にする
関    数    名 : PrepeareDrawing
引          数 : int num　			(in)描画する画像メモリ番号
				 int copy_src_num	(in)背景画像の画像メモリ番号
戻    り    値 : 
機          能 : copy_src_numのメモリに入っている画像を背景としてnumに描画用のRGB画像を新規生成する
				 図形描画前にこの関数を呼び出しておく
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::PrepeareDrawing(int num, int copy_src_num)
{
	if( !CopyDataUnit(copy_src_num, num) )	return false;

	if( !ConvertDataUnit(num, RGB_FORMAT, 0, 0, true, false, 0 ))	return false;

	return true;
}



/********************************************************************
機  能  名  称 : 画像メモリ上に別の画像を描画する
関    数    名 : DrawImageOnImage
引          数 : long			image_num		(in)下地とする画像メモリ番号
				 long			src_image_num	(in)描画する画像データが入った画像メモリ番号
				 int			offsetX,		(in)描画オフセットx座標
				 int			offsetY,		(in)描画オフセットy座標
				 int			mixType			(in)データの合成方法 1:max, 2:min, other:平均
戻    り    値 : 
機          能 : image_numのメモリに入っている画像を背景としてsrc_image_numにに入っている画像を書き込む
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::DrawImageOnImage(long number, long src_image_num, int offsetX, int offsetY, int mixType)
{
	CDataUnit *p_canvas_du;
	CDataUnit *p_draw_du;

	if( (p_canvas_du = GetDataUnit(number))==NULL)	return;
	if( (p_draw_du = GetDataUnit(src_image_num))==NULL)	return;


	if(  p_canvas_du->DataType == RGB_FORMAT || p_canvas_du->DataType == RGB_3D_FORMAT  )
	{
		p_canvas_du->DispRGB = true;

		if(p_draw_du->DataType == RGB_FORMAT || p_draw_du->DataType == RGB_3D_FORMAT)
		{//RGB->RGB
			p_draw_du->DispRGB = true;
			for(int src_y=0 ; src_y<p_draw_du->DataSize.cy ; src_y++)
			{
				for(int src_x=0 ; src_x<p_draw_du->DataSize.cx ; src_x++)
				{
					int dst_x = src_x + offsetX;
					int dst_y = src_y + offsetY;

					if(p_canvas_du->WorkArea.PtInWorkArea(CPoint(dst_x,dst_y)))
					{
						for(int c=0 ; c<3 ; c++){
							double baseVal = p_canvas_du->GetDataValue(dst_x,dst_y,-1,c);
							double drawVal = p_draw_du->GetDataValue(src_x,src_y,-1,c);

							if(mixType==1){//max
								p_canvas_du->SetDataValue(dst_x, dst_y, BIGGER(baseVal,drawVal), -1, c, true);
							}else if(mixType==2){//min
								p_canvas_du->SetDataValue(dst_x, dst_y, SMALLER(baseVal,drawVal), -1, c, true);
							}else{//average
								p_canvas_du->SetDataValue(dst_x, dst_y, (baseVal+drawVal)/2.0, -1, c, true);
							}
						}
					}
				}
			}
		}
		else
		{//RGB以外->RGB
			for(double src_y=0 ; src_y<p_draw_du->DataSize.cy ; src_y++)
			{
				for(double src_x=0 ; src_x<p_draw_du->DataSize.cx ; src_x++)
				{
					double dst_x = src_x + offsetX;
					double dst_y = src_y + offsetY;

					if(p_canvas_du->WorkArea.PtInWorkArea(CPoint(dst_x,dst_y)))
					{
						double baseVal = p_canvas_du->GetDataValue(dst_x,dst_y,-1,0);
						double drawVal = p_draw_du->GetDataValue(src_x,src_y,-1,0);
						double val;

						if(mixType==1){//max
							val = BIGGER(baseVal,drawVal);
						}else if(mixType==2){//min
							val = SMALLER(baseVal,drawVal);
						}else{//average
							val = (baseVal+drawVal)/2.0;
						}
					
						p_canvas_du->SetDataValue(dst_x, dst_y, val, -1, 0, true);//r
						p_canvas_du->SetDataValue(dst_x, dst_y, val, -1, 1, true);//g
						p_canvas_du->SetDataValue(dst_x, dst_y, val, -1, 2, true);//b
					}
				}
			}
		}
	}
	else
	{
		if(p_draw_du->DataType == RGB_FORMAT || p_draw_du->DataType == RGB_3D_FORMAT)
		{//RGB->RGB以外
			p_draw_du->DispRGB = true;
		}
			
		for(double src_y=0 ; src_y<p_draw_du->DataSize.cy ; src_y++)
		{
			for(double src_x=0 ; src_x<p_draw_du->DataSize.cx ; src_x++)
			{
				double dst_x = src_x + offsetX;
				double dst_y = src_y + offsetY;

				if(p_canvas_du->WorkArea.PtInWorkArea(CPoint(dst_x,dst_y)))
				{
					double baseVal = p_canvas_du->GetDataValue(dst_x,dst_y,-1,0);
					double drawVal = p_draw_du->GetDataValue(src_x,src_y,-1,0);
					double val;

					if(mixType==1){//max
						val = BIGGER(baseVal,drawVal);
					}else if(mixType==2){//min
						val = SMALLER(baseVal,drawVal);
					}else{//average
						val = (baseVal+drawVal)/2.0;
					}
					p_canvas_du->SetDataValue(dst_x,dst_y, val, -1, -1, true);
				}
			}
		}
	}
}