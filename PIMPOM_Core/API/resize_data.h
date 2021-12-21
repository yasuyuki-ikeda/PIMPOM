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
//
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "global_define.h"

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

//サブピクセル値を計算（bi-linear interporation）
template <class DATA>
double	sub_pix_value(DATA *p_src_data, CSize src_size, double x, double y)
{
	double	wx, wy, w0, w1, w2, w3;
	int		addr;
	double  sum = 0;

	//画像から1pix以上はみ出す場合は0を返す
	if (x<-1 || x>src_size.cx || y<-1 || y>src_size.cy)	return 0;

	//画像の隅
	if (x < 0) {
		wx = 1.0;
	}
	else if (x > src_size.cx - 1) {
		wx = 0.0;
	}
	else {
		wx = x - (int)x;
	}

	if (y < 0) {
		wy = 1.0;
	}
	else if (y > src_size.cy - 1) {
		wy = 0.0;
	}
	else {
		wy = y - (int)y;
	}



	addr = floor(x) + floor(y)*src_size.cx;

	w0 = (1.0 - wx)*(1.0 - wy);
	w1 = (wx)*(1.0 - wy);
	w2 = (1.0 - wx)*(wy);
	w3 = (wx)*(wy);

	if (w0 > 0) {
		sum += (double)*(p_src_data + addr)*w0;
	}
	if (w1 > 0) {
		sum += (double)*(p_src_data + addr + 1)*w1;
	}
	if (w2 > 0) {
		sum += (double)*(p_src_data + addr + src_size.cx) *w2;
	}
	if (w3 > 0) {
		sum += (double)*(p_src_data + addr + src_size.cx + 1) *w3;
	}


	return sum;
}



template <class DATA>
bool	resize_image(
	DATA			*p_src,					//(in)変換前の画像データ
	DATA			*p_dst,					//(out)変換後の画像データ
	SIZE			src_size,				//(in)変換前の画像サイズ
	int				page,					//(in)
	int				channel,				//(in)
	SIZE			dst_size,				//(in)変換後の画像サイズ
	bool			do_stretch,				//(in)変換後のサイズに合わせて画像を拡大/縮小するか(true:拡大する false:拡大しない)
	POINT			offset,					//(in)オフセット量
	int				reduction_type			//(in)画像縮小のアルゴ（0:area-integration 1:max 2:min）
	)
{
	if (do_stretch)//変換後のサイズに合わせて画像を引き伸ばす
	{
		//倍率
		double scale_x = (double)dst_size.cx / (double)src_size.cx;
		double scale_y = (double)dst_size.cy / (double)src_size.cy;

		//画像縮小(面積積分)用データ格納バッファ確保
		int buffersize = BIGGER(src_size.cx, dst_size.cx) * BIGGER(src_size.cy, dst_size.cy) + src_size.cx + src_size.cy;
		float *p_buffer = new float[buffersize];
		if (p_buffer == NULL)
		{//バッファ確保できなければエラー終了
			return false;
		}

		float *p_x_integralval = p_buffer + BIGGER(src_size.cx, dst_size.cx) * BIGGER(src_size.cy, dst_size.cy);
		float *p_y_integralval = p_x_integralval + src_size.cx;


		//データリサイズ
		for (int k = 0; k<page; k++)
		{
			for (int c = 0; c<channel; c++)
			{
				int src_offs = (k*channel + c)*src_size.cx*src_size.cy;
				int dst_offs = (k*channel + c)*dst_size.cx*dst_size.cy;

				//x方向
				if (scale_x<1)
				{//縮小

					for (int j = 0; j<src_size.cy; j++)
					{
						for (int i = 0; i<dst_size.cx; i++)
						{
							float startx = (float)i / scale_x;//同一pixに集約されるx座標範囲の始点
							float endx = (float)(i + 1) / scale_x;//同一pixに集約されるx座標範囲の終点
							float weight;
							int cnt = 0;//同一pixに集約される画素数
							float sum = 0, denom = 0;//同一pixに集約される画素値合計


							//領域始点側：部分的に領域に入る画素
							weight = (ceil(startx) - startx);
							if (weight>0){
								p_x_integralval[cnt] = *(p_src + (int)startx + j*src_size.cx + src_offs);
								sum += p_x_integralval[cnt] * weight;
								denom += weight;
								cnt++;
							}

							//領域中央：すべてが領域に入る画素
							for (int pix = ceil(startx); pix< floor(endx); pix++)
							{
								p_x_integralval[cnt] = *(p_src + pix + j*src_size.cx + src_offs);
								sum += p_x_integralval[cnt];
								denom++;
								cnt++;
							}

							//領域終点側：部分的に領域に入る画素
							weight = (endx - floor(endx));
							if (weight>0 && endx<src_size.cx){
								p_x_integralval[cnt] = *(p_src + (int)endx + j*src_size.cx + src_offs);
								sum += p_x_integralval[cnt] * weight;
								denom += weight;
								cnt++;
							}


							if (reduction_type == 1)
							{//max
								float maxval = p_x_integralval[0];
								for (int idx = 0; idx<cnt; idx++){
									if (maxval < p_x_integralval[idx]){
										maxval = p_x_integralval[idx];
									}
								}
								*(p_buffer + i + j*dst_size.cx) = maxval;
							}
							else if (reduction_type == 2)
							{//max
								float minval = p_x_integralval[0];
								for (int idx = 0; idx<cnt; idx++){
									if (minval > p_x_integralval[idx]){
										minval = p_x_integralval[idx];
									}
								}
								*(p_buffer + i + j*dst_size.cx) = minval;
							}
							else
							{//area integration
								*(p_buffer + i + j*dst_size.cx) = sum / denom;
							}
						}
					}
				}
				else
				{//拡大→bi-linear

					for (int j = 0; j<src_size.cy; j++)
					{
						for (int i = 0; i<dst_size.cx; i++)
						{
							float x = ((float)i - 0.5) / scale_x;
							*(p_buffer + i + j*dst_size.cx) = (DATA)sub_pix_value(p_src + src_offs, src_size, x, j);
						}
					}
				}


				//y方向
				if (scale_y<1)
				{//縮小

					for (int i = 0; i<dst_size.cx; i++)
					{
						for (int j = 0; j<dst_size.cy; j++)
						{
							float starty = (float)j / scale_y;//同一pixに集約されるy座標範囲の始点
							float endy = (float)(j + 1) / scale_y;//同一pixに集約されるy座標範囲の終点
							float weight;
							int cnt = 0;//同一pixに集約される画素数
							float sum = 0, denom = 0;//同一pixに集約される画素値合計


							//領域始点側：部分的に領域に入る画素
							weight = (ceil(starty) - starty);
							if (weight>0){
								p_y_integralval[cnt] = *(p_buffer + i + ((int)starty)*dst_size.cx);
								sum += p_y_integralval[cnt] * weight;
								denom += (ceil(starty) - starty);
								cnt++;
							}

							//領域中央：すべてが領域に入る画素
							for (int pix = ceil(starty); pix< floor(endy); pix++)
							{
								p_y_integralval[cnt] = *(p_buffer + i + pix*dst_size.cx);
								sum += p_y_integralval[cnt];
								denom++;
								cnt++;
							}

							//領域終点側：部分的に領域に入る画素
							weight = (endy - floor(endy));
							if (weight>0 && endy<src_size.cy){
								p_y_integralval[cnt] = *(p_buffer + i + ((int)endy)*dst_size.cx);
								sum += p_y_integralval[cnt] * weight;
								denom += weight;
								cnt++;
							}

							if (reduction_type == 1)
							{//max
								float maxval = p_y_integralval[0];
								for (int idx = 0; idx<cnt; idx++){
									if (maxval < p_y_integralval[idx]){
										maxval = p_y_integralval[idx];
									}
								}
								*(p_dst + i + j*dst_size.cx + dst_offs) = maxval;
							}
							else if (reduction_type == 2)
							{//max
								float minval = p_y_integralval[0];
								for (int idx = 0; idx<cnt; idx++){
									if (minval > p_y_integralval[idx]){
										minval = p_y_integralval[idx];
									}
								}
								*(p_dst + i + j*dst_size.cx + dst_offs) = minval;
							}
							else
							{//area integration
								*(p_dst + i + j*dst_size.cx + dst_offs) = sum / denom;
							}
						}
					}
				}
				else
				{//拡大
					CSize buffseSize(dst_size.cx, src_size.cy);
					for (int j = 0; j<dst_size.cy; j++)
					{
						for (int i = 0; i<dst_size.cx; i++)
						{
							float y = (float)(j - 0.5) / scale_y;
							*(p_dst + i + j*dst_size.cx + dst_offs) = (DATA)sub_pix_value(p_buffer, buffseSize, i, y);
						}
					}
				}
			}
		}


		delete[]	p_buffer;
	}
	else//変換前の縮尺のままコピー（はみ出し部分は切り取られる）
	{
		for (int j = 0; j<dst_size.cy; j++)
		{
			for (int i = 0; i<dst_size.cx; i++)
			{
				int im = i - offset.x;
				int jm = j - offset.y;

				if (im >= 0 && jm >= 0 && im<src_size.cx && jm<src_size.cy)
				{
					for (int k = 0; k<page; k++){
						for (int c = 0; c<channel; c++){
							*(p_dst + i + j*dst_size.cx + (k*channel + c)*dst_size.cx*dst_size.cy)
								= *(p_src + im + jm*src_size.cx + (k*channel + c)*src_size.cx*src_size.cy);//データコピー
						}
					}
				}
			}
		}
	}
	return true;
}


