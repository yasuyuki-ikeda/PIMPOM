/*******************************************************************
Copyright (c) 2020, yasuyuki ikeda All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.
* Neither the name of the <organization> nor the names of its contributors 
  may be used to endorse or promote products derived from this software 
  without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************/
//

#include "stdafx.h"
#include <global_define.h>

void pimpom_api_def();


void _InitAysnc()
{
	InitAysnc(0);
}

int _EnableAysnc()
{
	return EnableAysnc();
}

int _WaitAysnc(int wait)
{
	return WaitAysnc(wait);
}

void _ReturnAysnc()
{
	ReturnAysnc();
}



/********************************************************************
機  能  名  称 : PIMPOMに8bitモノクロ画像データを出力する
関    数    名 : _PlotByteImage
引          数 : int imgNo　	(in)画像メモリ番号
				np::ndarray img (in)画像データ配列(2次元のnumpy配列 [row][col])
				np::ndarray params (in)パラメータ
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void plot_byte_image_internal(int imgNo, np::ndarray img, bool async, np::ndarray *pParams)
{
	//入力データのチェック
	if (img.get_nd() != 2)
	{
		throw std::runtime_error("second argument must be 2-dimensional");
	}
	if (img.get_dtype() != np::dtype::get_builtin<unsigned char>())
	{
		throw std::runtime_error("second argument must be uint8 array");
	}

	if (pParams)
	{//パラメタのチェック
		if (pParams->get_nd() != 1) {
			throw std::runtime_error("parameter must be 1-dimensional");
		}
		if (pParams->get_dtype() != np::dtype::get_builtin<float>())
		{
			throw std::runtime_error("parameter must be float32 array");
		}
	}

	//入力データのコピー
	auto shape = img.get_shape();
	int h = shape[0];
	int w = shape[1];
	unsigned char *tmp = new unsigned char[w * h];
	unsigned char *ptr = reinterpret_cast<unsigned char *>(img.get_data());

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			tmp[ j*w + i] = *ptr;
			ptr++;
		}
	}

	if (!async)
	{
		PlotByteImage(imgNo, w, h, tmp);
	}
	else 
	{
		if (pParams)
		{//パラメタのコピー
			float tmpParam[PIMPOM_PLOT_ASYNC_PARAM_LEN] = { 0 };
			auto param_shape = pParams->get_shape();
			float *ptrParam = reinterpret_cast<float *>(pParams->get_data());
			for (int i = 0; i < PIMPOM_PLOT_ASYNC_PARAM_LEN && i < param_shape[0]; i++)
			{
				tmpParam[i] = ptrParam[i];
			}

			PlotByteImageAsync(imgNo, w, h, tmp, tmpParam);
		}
		else
		{
			PlotByteImageAsync(imgNo, w, h, tmp, NULL);
		}
	}

	delete[] tmp;
}

void _PlotByteImage(int imgNo, np::ndarray img)
{
	plot_byte_image_internal(imgNo, img, false, NULL);
}

void _PlotByteImageAsync(int imgNo, np::ndarray img)
{
	plot_byte_image_internal(imgNo, img, true, NULL);
}

void _PlotByteImageAsync(int imgNo, np::ndarray img, np::ndarray params)
{
	plot_byte_image_internal(imgNo, img, true, &params);
}

/********************************************************************
機  能  名  称 : PIMPOMに24bitカラー画像データを出力する
関    数    名 : _PlotByteImage
引          数 : int imgNo　	(in)画像メモリ番号
				　np::ndarray img (in)画像データ配列(3次元のnumpy配列 [row][col][channel])
				 int order_rgb   (in)カラーの並び (0以外：RGB  0:BGR)
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void plot_rgb_image_internal(int imgNo, np::ndarray img, int order_rgb, bool async, np::ndarray *pParams)
{
	//入力データのチェック
	if (img.get_nd() != 3)
	{
		throw std::runtime_error("second argument must be 3-dimensional");
	}

	if (img.get_dtype() != np::dtype::get_builtin<unsigned char>())
	{
		throw std::runtime_error("a must be uint8 array");
	}

	if (pParams)
	{//パラメタのチェック
		if (pParams->get_nd() != 1) {
			throw std::runtime_error("parameter must be 1-dimensional");
		}
		if (pParams->get_dtype() != np::dtype::get_builtin<float>())
		{
			throw std::runtime_error("parameter must be float32 array");
		}
	}

	auto shape = img.get_shape();
	int h = shape[0];
	int w = shape[1];
	int ch = shape[2];

	if (ch != 3) {
		throw std::runtime_error("last channel length must be 3");
	}

	//入力データのコピー
	unsigned char *tmp = new unsigned char[ch * w * h];
	unsigned char *ptr = reinterpret_cast<unsigned char *>(img.get_data());

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			for (int c = 0; c < ch; c++)
			{
				if (order_rgb) {
					tmp[c*w*h + j*w + i] = *ptr;
				}
				else {
					tmp[(ch - 1 - c)*w*h + j*w + i] = *ptr;
				}

				ptr++;
			}
		}
	}

	if (!async)
	{
		PlotRGBImage(imgNo, w, h, tmp);
	}
	else 
	{
		if (pParams)
		{//パラメタのコピー
			float tmpParam[PIMPOM_PLOT_ASYNC_PARAM_LEN] = { 0 };
			auto param_shape = pParams->get_shape();
			float *ptrParam = reinterpret_cast<float *>(pParams->get_data());
			for (int i = 0; i < PIMPOM_PLOT_ASYNC_PARAM_LEN && i < param_shape[0]; i++)
			{
				tmpParam[i] = ptrParam[i];
			}

			PlotRGBImageAsync(imgNo, w, h, tmp, tmpParam);
		}
		else {
			PlotRGBImageAsync(imgNo, w, h, tmp, NULL);
		}
	}

	delete[] tmp;
}

void _PlotRGBImage(int imgNo, np::ndarray img, int order_rgb)
{
	plot_rgb_image_internal(imgNo, img, order_rgb, false, NULL);
}

void _PlotRGBImageAsync(int imgNo, np::ndarray img, int order_rgb)
{
	plot_rgb_image_internal(imgNo, img, order_rgb, true, NULL);
}

void _PlotRGBImageAsync(int imgNo, np::ndarray img, int order_rgb, np::ndarray params)
{
	plot_rgb_image_internal(imgNo, img, order_rgb, true, &params);
}

/********************************************************************
機  能  名  称 : PIMPOMに8bitモノクロのマルチチャネル画像データを出力する
関    数    名 : _PlotByteImage
引          数 : int imgNo　	(in)画像メモリ番号
               　np::ndarray img (in)画像データ配列(3次元のnumpy配列 [row][col][channel])
 戻    り    値 :
 機          能 :
 日付         作成者          内容
 ------------ --------------- ---------------------------------------
 Y.Ikeda         新規作成
 ********************************************************************/
void _Plot3DImage(int imgNo, np::ndarray img)
{
	int nd = img.get_nd();

	if (nd != 3)
	{
		throw std::runtime_error("second argument must be 3-dimensional");
	}

	if (img.get_dtype() != np::dtype::get_builtin<unsigned char>())
	{
		throw std::runtime_error("a must be uint8 array");
	}

	auto shape = img.get_shape();
	auto strides = img.get_strides();

	int h = shape[0];
	int w = shape[1];
	int ch = shape[2];

	//printf("w=%d, h=%d, ch=%d",w,h,ch);

	unsigned char *tmp = new unsigned char[ch * w * h];
	unsigned char *ptr = reinterpret_cast<unsigned char *>(img.get_data());

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			for (int c = 0; c < ch; c++)
			{
				tmp[(ch - 1 - c)*w*h + j*w + i] = *ptr;
				ptr++;
			}
		}
	}


	Plot3DImage(imgNo, w, h, ch, tmp);

	delete[] tmp;
}


/********************************************************************
機  能  名  称 : PIMPOMに32bit浮動小数点画像データを出力する
関    数    名 : _PlotByteImage
引          数 : int imgNo　	(in)画像メモリ番号
　               np::ndarray img (in)画像データ配列(2次元のnumpy配列 [row][col])
 戻    り    値 :
 機          能 :
 日付         作成者          内容
 ------------ --------------- ---------------------------------------
 Y.Ikeda         新規作成
 ********************************************************************/
void _PlotFloatImage(int imgNo, np::ndarray img)
{
	int nd = img.get_nd();

	if (nd != 2)
	{
		throw std::runtime_error("second argument must be two-dimensional");
	}
	if (img.get_dtype() != np::dtype::get_builtin<float>())
	{
		throw std::runtime_error("a must be float32 array");
	}

	auto shape = img.get_shape();
	auto strides = img.get_strides();

	int h = shape[0];
	int w = shape[1];

	float *tmp = new float[w * h];
	float *ptr = reinterpret_cast<float *>(img.get_data());

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			tmp[j*w + i] = *ptr;
			ptr++;
		}
	}

	PlotFloatImage(imgNo, shape[1], shape[0], tmp);

	delete[] tmp;
}




/********************************************************************
機  能  名  称 : PIMPOMに32bit浮動小数点のマルチチャネル画像データを出力する
関    数    名 : _PlotByteImage
引          数 : int imgNo　	(in)画像メモリ番号
　               np::ndarray img (in)画像データ配列(3次元のnumpy配列 [channel][row][col])
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void _PlotF3DImage(int imgNo, np::ndarray img)
{
	int nd = img.get_nd();

	if (nd != 3)
	{
		throw std::runtime_error("second argument must be 3-dimensional");
	}

	if (img.get_dtype() != np::dtype::get_builtin<float>())
	{
		throw std::runtime_error("a must be float32 array");
	}

	auto shape = img.get_shape();
	auto strides = img.get_strides();

	int ch = shape[0];
	int h = shape[1];
	int w = shape[2];



	//printf("w=%d, h=%d, ch=%d",w,h,ch);

	float *tmp = new float[ch * w * h];
	float *ptr = reinterpret_cast<float *>(img.get_data());

	for (int c = 0; c < ch; c++)
	{
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{

				tmp[(c)*w*h + j*w + i] = *ptr;
				ptr++;
			}
		}
	}

	PlotF3DImage(imgNo, w, h, ch, tmp);

	delete[] tmp;
}


/********************************************************************
機  能  名  称 : 32bit浮動小数点 1Dデータを折れ線グラフに出力する
関    数    名 : _PlotDataToChart
引          数 : 
int imgNo　	(in)画像メモリ番号
 np::ndarray data (in)画像データ配列(numpy配列 [row])
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void _PlotDataToChart(int imgNo, np::ndarray data)
{
	int nd = data.get_nd();

	if (nd != 1)
	{
		throw std::runtime_error("second argument must be 1-dimensional array");
	}

	auto shape = data.get_shape();
	auto strides = data.get_strides();
	int len = shape[0];

	if (data.get_dtype() == np::dtype::get_builtin<float>())
	{
		float *tmp = new float[len];
		float *ptr = reinterpret_cast<float *>(data.get_data());
		memcpy(tmp, ptr, len * sizeof(float));

		PlotDataToChartF(imgNo, len, tmp);

		delete[] tmp;
	}
	else if (data.get_dtype() == np::dtype::get_builtin<double>())
	{
		double *tmp = new double[len];
		double *ptr = reinterpret_cast<double *>(data.get_data());
		memcpy(tmp, ptr, len * sizeof(double));

		PlotDataToChartD(imgNo, len, tmp);

		delete[] tmp;
	}
	else if (data.get_dtype() == np::dtype::get_builtin<int>())
	{
		int *tmp = new int[len];
		int *ptr = reinterpret_cast<int *>(data.get_data());
		memcpy(tmp, ptr, len * sizeof(int));

		PlotDataToChartI(imgNo, len, tmp);

		delete[] tmp;
	}
	else if (data.get_dtype() == np::dtype::get_builtin<BYTE>())
	{
		BYTE *tmp = new BYTE[len];
		BYTE *ptr = reinterpret_cast<BYTE *>(data.get_data());
		memcpy(tmp, ptr, len * sizeof(BYTE));

		PlotDataToChartB(imgNo, len, tmp);

		delete[] tmp;
	}
	else
	{
		throw std::runtime_error("second must be float32/float64/int32/uint8 array");
	}
}


/********************************************************************
機  能  名  称 : 32bit浮動小数点 2Dデータを散布図に出力する
関    数    名 : _PlotDataToChart
引          数 :
int imgNo　	(in)画像メモリ番号
np::ndarray dataX, dataY (in)画像データ配列(numpy配列 [row])
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void _PlotDataToChart2(int imgNo, np::ndarray dataX, np::ndarray dataY)
{
	int ndX = dataX.get_nd();
	int ndY = dataY.get_nd();

	if (ndX != 1 || ndY != 1)
	{
		throw std::runtime_error("second and third argument must be 1-dimensional array");
	}

	auto shapeX = dataX.get_shape();
	auto shapeY = dataY.get_shape();
	if (shapeX[0] != shapeY[0])
	{
		throw std::runtime_error("length of second and third argument must be same");
	}


	int len = shapeX[0];

	if (dataX.get_dtype() == np::dtype::get_builtin<float>() && dataY.get_dtype() == np::dtype::get_builtin<float>())
	{
		float *tmpX = new float[len*2];
		float *tmpY = tmpX + len;
		float *ptrX = reinterpret_cast<float *>(dataX.get_data());
		float *ptrY = reinterpret_cast<float *>(dataY.get_data());
		memcpy(tmpX, ptrX, len * sizeof(float));
		memcpy(tmpY, ptrY, len * sizeof(float));

		PlotDataToChart2F(imgNo, len, tmpX, tmpY);

		delete[] tmpX;
	}
	else if (dataX.get_dtype() == np::dtype::get_builtin<double>() && dataY.get_dtype() == np::dtype::get_builtin<double>())
	{
		double *tmpX = new double[len * 2];
		double *tmpY = tmpX + len;
		double *ptrX = reinterpret_cast<double *>(dataX.get_data());
		double *ptrY = reinterpret_cast<double *>(dataY.get_data());
		memcpy(tmpX, ptrX, len * sizeof(double));
		memcpy(tmpY, ptrY, len * sizeof(double));

		PlotDataToChart2D(imgNo, len, tmpX, tmpY);

		delete[] tmpX;
	}
	else if (dataX.get_dtype() == np::dtype::get_builtin<int>() && dataY.get_dtype() == np::dtype::get_builtin<int>())
	{
		int *tmpX = new int[len * 2];
		int *tmpY = tmpX + len;
		int *ptrX = reinterpret_cast<int *>(dataX.get_data());
		int *ptrY = reinterpret_cast<int *>(dataY.get_data());
		memcpy(tmpX, ptrX, len * sizeof(int));
		memcpy(tmpY, ptrY, len * sizeof(int));

		PlotDataToChart2I(imgNo, len, tmpX, tmpY);

		delete[] tmpX;
	}
	else if (dataX.get_dtype() == np::dtype::get_builtin<BYTE>() && dataY.get_dtype() == np::dtype::get_builtin<BYTE>())
	{
		BYTE *tmpX = new BYTE[len * 2];
		BYTE *tmpY = tmpX + len;
		BYTE *ptrX = reinterpret_cast<BYTE *>(dataX.get_data());
		BYTE *ptrY = reinterpret_cast<BYTE *>(dataY.get_data());
		memcpy(tmpX, ptrX, len * sizeof(BYTE));
		memcpy(tmpY, ptrY, len * sizeof(BYTE));

		PlotDataToChart2B(imgNo, len, tmpX, tmpY);

		delete[] tmpX;
	}
	else
	{
		throw std::runtime_error("second and third argument  must be float32/float64/int32/uint8 array");
	}
}


/********************************************************************
機  能  名  称 : PIMPOMにマスク画像データを出力する
関    数    名 : _PlotMaskImage
引          数 : int imgNo　	(in)画像メモリ番号
np::ndarray img (in)画像データ配列(2次元のnumpy配列 [row][col])
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void _SetMaskImage(int imgNo, np::ndarray img)
{
	int nd = img.get_nd();

	if (nd != 2)
	{
		throw std::runtime_error("second argument must be 2-dimensional");
	}
	if (img.get_dtype() != np::dtype::get_builtin<unsigned char>())
	{
		throw std::runtime_error("a must be uint8 array");
	}

	auto shape = img.get_shape();
	auto strides = img.get_strides();

	int h = shape[0];
	int w = shape[1];


	unsigned char *tmp = new unsigned char[w * h];
	unsigned char *ptr = reinterpret_cast<unsigned char *>(img.get_data());

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			tmp[j*w + i] = *ptr;
			ptr++;
		}
	}

	SetMaskImage(imgNo, tmp);

	delete[] tmp;
}

/********************************************************************
機  能  名  称 : PIMPOMから非同期参照パラメタを取得する
関    数    名 : _GetAysncParams
引          数 : 
戻    り    値 :非同期参照パラメタ(1次元のnumpy配列)
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
np::ndarray _GetAysncParams()
{
	Py_intptr_t shape[1] = { PIMPOM_PLOT_ASYNC_PARAM_LEN };
	np::ndarray params = np::zeros(1, shape, np::dtype::get_builtin<float>());
	float *ptr = reinterpret_cast<float *>(params.get_data());

	GetAysncParams(ptr);
	return params;
}

/********************************************************************
機  能  名  称 : PIMPOMから8bitモノクロ画像データを入力する
関    数    名 : _GetByteImage
引          数 : int imgNo　	(in)画像メモリ番号
戻    り    値 :画像データ配列(2次元のnumpy配列 [row][col])
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
np::ndarray  _GetByteImage(int imgNo)
{
	int type, w, h, page;
	GetDataTypeSize(imgNo, &type, &w, &h, &page);

	if (type != BYTE_FORMAT) {
		throw std::runtime_error("not BYTE image");
	}
	

	Py_intptr_t shape[2] = { h, w };
	np::ndarray img = np::zeros(2, shape, np::dtype::get_builtin<unsigned char>());

	unsigned char *tmp = new unsigned char[w * h];
	unsigned char *ptr = reinterpret_cast<unsigned char *>(img.get_data());

	GetByteImage(imgNo, w, h, tmp);

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			*ptr = tmp[j*w + i];
			ptr++;
		}
	}

	delete[] tmp;

	return img;
}

/********************************************************************
機  能  名  称 : PIMPOMから8bitモノクロ画像データを非同期入力する
関    数    名 : _GetByteImage
引          数 : int imgNo　	(in)画像メモリ番号
戻    り    値 :画像データ配列(2次元のnumpy配列 [row][col])
失敗したらサイズ1x1の行列が入る
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
np::ndarray  _GetByteImageAsync()
{
	int w, h, num;
	unsigned char *tmp;

	int ret = GetByteImageAsync(&num, &w, &h, &tmp);
	if (!ret) 
	{
		Py_intptr_t shape[2] = { 1, 1 };
		np::ndarray img = np::zeros(2, shape, np::dtype::get_builtin<unsigned char>());
		return img;
	}

	Py_intptr_t shape[2] = { h, w };
	np::ndarray img = np::zeros(2, shape, np::dtype::get_builtin<unsigned char>());
	unsigned char *ptr = reinterpret_cast<unsigned char *>(img.get_data());

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			*ptr = tmp[j*w + i];
			ptr++;
		}
	}

	free(tmp);

	return img;
}

/********************************************************************
機  能  名  称 : PIMPOMから24bitカラー画像データを入力する
関    数    名 : _GetRGBImage
引          数 : int imgNo　	(in)画像メモリ番号
                 int order_rgb   (out)カラーの並び (0以外：RGB  0:BGR)
 戻    り    値 :画像データ配列(3次元のnumpy配列 [row][col][channel])
 機          能 :
 日付         作成者          内容
 ------------ --------------- ---------------------------------------
 Y.Ikeda         新規作成
 ********************************************************************/
np::ndarray   _GetRGBImage(int imgNo, int order_rgb)
{
	int type, w, h, page, ch=3;
	GetDataTypeSize(imgNo, &type, &w, &h, &page);

	if (type != RGB_FORMAT) {
		throw std::runtime_error("not RGB image");
	}


	Py_intptr_t shape[3] = { h, w , ch};
	np::ndarray img = np::zeros(3, shape, np::dtype::get_builtin<unsigned char>());

	unsigned char *tmp = new unsigned char[ch * w * h];
	unsigned char *ptr = reinterpret_cast<unsigned char *>(img.get_data());

	GetRGBImage(imgNo, w, h, tmp);

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			for (int c = 0; c < ch; c++)
			{
				if (order_rgb) {
					*ptr = tmp[c*w*h + j*w + i];
				}
				else {
					*ptr = tmp[(ch - 1 - c)*w*h + j*w + i];
				}

				ptr++;
			}
		}
	}

	delete[] tmp;

	return img;
}

/********************************************************************
機  能  名  称 : PIMPOMから24bitカラー画像データを非同期入力する
関    数    名 : _GetRGBImage
引          数 : int imgNo　	(in)画像メモリ番号
int order_rgb   (out)カラーの並び (0以外：RGB  0:BGR)
戻    り    値 :画像データ配列(3次元のnumpy配列 [row][col][channel])
失敗したらサイズ1x1x1の行列が入る
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
np::ndarray  _GetRGBImageAsync(int order_rgb)
{
	int w, h, num;
	unsigned char *tmp;

	int ret = GetRGBImageAsync(&num, &w, &h, &tmp);
	if (!ret)
	{
		Py_intptr_t shape[3] = { 1, 1 , 1 };
		np::ndarray img = np::zeros(3, shape, np::dtype::get_builtin<unsigned char>());
		return img;
	}

	Py_intptr_t shape[3] = { h, w , 3 };
	np::ndarray img = np::zeros(3, shape, np::dtype::get_builtin<unsigned char>());
	unsigned char *ptr = reinterpret_cast<unsigned char *>(img.get_data());

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			for (int c = 0; c < 3; c++)
			{
				if (order_rgb) {
					*ptr = tmp[c*w*h + j*w + i];
				}
				else {
					*ptr = tmp[(3 - 1 - c)*w*h + j*w + i];
				}

				ptr++;
			}
		}
	}

	free(tmp);

	return img;
}

/********************************************************************
機  能  名  称 : PIMPOMに8bitモノクロのマルチチャネル画像データを出力する
関    数    名 : _Get3DImage
引          数 : int imgNo　	(in)画像メモリ番号
 戻    り    値 :画像データ配列(3次元のnumpy配列 [row][col][channel])
 機          能 :
 日付         作成者          内容
 ------------ --------------- ---------------------------------------
 Y.Ikeda         新規作成
 ********************************************************************/
np::ndarray  _Get3DImage(int imgNo )
{
	int type, w, h, page;
	GetDataTypeSize(imgNo, &type, &w, &h, &page);

	if (type != THREE_D_FORMAT) {
		throw std::runtime_error("not 3D image");
	}


	Py_intptr_t shape[3] = { h, w , page };
	np::ndarray img = np::zeros(3, shape, np::dtype::get_builtin<unsigned char>());

	unsigned char *tmp = new unsigned char[page * w * h];
	unsigned char *ptr = reinterpret_cast<unsigned char *>(img.get_data());

	Get3DImage(imgNo, w, h, page, tmp);

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			for (int c = 0; c < page; c++)
			{
				*ptr = tmp[(page - 1 - c)*w*h + j*w + i] ;
				ptr++;
			}
		}
	}

	delete[] tmp;

	return img;
}

/********************************************************************
機  能  名  称 : PIMPOMに32bit浮動小数点画像データを出力する
関    数    名 : _GetFloatImage
引          数 : int imgNo　	(in)画像メモリ番号
戻    り    値 :画像データ配列(2次元のnumpy配列 [row][col])
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
np::ndarray _GetFloatImage(int imgNo)
{
	int type, w, h, page;
	GetDataTypeSize(imgNo, &type, &w, &h, &page);

	if (type != FLOAT_FORMAT) {
		throw std::runtime_error("not FLOAT image");
	}


	Py_intptr_t shape[2] = { h, w };
	np::ndarray img = np::zeros(2, shape, np::dtype::get_builtin<float>());

	float *tmp = new float[w * h];
	float *ptr = reinterpret_cast<float *>(img.get_data());

	GetFloatImage(imgNo, w, h, tmp);

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			*ptr = tmp[j*w + i] ;
			ptr++;
		}
	}

	delete[] tmp;

	return img;
}

/********************************************************************
機  能  名  称 : PIMPOMから32bit浮動小数点のマルチチャネル画像データを入力する
関    数    名 : _GetF3DImage
引          数 : int imgNo　	(in)画像メモリ番号
戻    り    値 :画像データ配列(3次元のnumpy配列 [row][col][channel])
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
np::ndarray _GetF3DImage(int imgNo)
{
	int type, w, h, page;
	GetDataTypeSize(imgNo, &type, &w, &h, &page);

	if (type != FLOAT_3D_FORMAT) {
		throw std::runtime_error("not FLOAT 3D image");
	}


	Py_intptr_t shape[3] = { h, w , page};
	np::ndarray img = np::zeros(3, shape, np::dtype::get_builtin<float>());

	float *tmp = new float[w * h * page];
	float *ptr = reinterpret_cast<float *>(img.get_data());

	GetF3DImage(imgNo, w, h, page, tmp);

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			for (int c = 0; c < page; c++)
			{
				tmp[(page - 1 - c)*w*h + j*w + i] = *ptr;
				ptr++;
			}
		}
	}


	delete[] tmp;

	return img;
}


/********************************************************************
機  能  名  称 : PIMPOMからマスク画像データを入力する
関    数    名 : _GetMaskImage
引          数 : int imgNo　	(in)画像メモリ番号
戻    り    値 :画像データ配列(2次元のnumpy配列 [row][col])
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
np::ndarray  _GetMaskImage(int imgNo)
{
	int type, w, h, page;
	GetDataTypeSize(imgNo, &type, &w, &h, &page);

	if (type <0) {
		throw std::runtime_error("no image");
	}


	Py_intptr_t shape[2] = { h, w };
	np::ndarray img = np::zeros(2, shape, np::dtype::get_builtin<unsigned char>());

	unsigned char *tmp = new unsigned char[w * h];
	unsigned char *ptr = reinterpret_cast<unsigned char *>(img.get_data());

	GetMaskImage(imgNo, tmp);

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			*ptr = tmp[j*w + i];
			ptr++;
		}
	}

	delete[] tmp;

	return img;
}


/********************************************************************
機  能  名  称 : 作業領域を取得
関    数    名 : _GetWorkArea
引          数 : int num　	(in)画像メモリ番号
                 int fig_n  (in)図形番号
戻    り    値 : 4隅座標(長さ4のnumpy配列)
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
np::ndarray  _GetWorkArea(int imgNo, int fig_n)
{
	int type, w, h, page;
	GetDataTypeSize(imgNo, &type, &w, &h, &page);

	if (type <0) {
		throw std::runtime_error("no image");
	}

	RECT rct;
	rct = GetWorkArea(imgNo, fig_n);

	Py_intptr_t shape = 4;
	np::ndarray img = np::zeros(1, &shape, np::dtype::get_builtin<int>());
	int *ptr = reinterpret_cast<int *>(img.get_data());

	ptr[0] = rct.left;
	ptr[1] = rct.top;
	ptr[2] = rct.right;
	ptr[3] = rct.bottom;

	return img;
}




//文字コード変換（Shift-JIS -> UTF8）
// ShiftJISコード文字列を 一度 Unicode に変換してから UFT-8 へと変換し、そのデータを返す
// 内部でメモリ確保しているため、使用後は外部でメモリを解放する必要がある
LPSTR SJIStoUTF8(LPCSTR lpText)
{
	if (lpText == NULL || *lpText == '\0'){
		return NULL;
	}

	// (1) ShiftJIS 文字列を Unicode に変換
	//     ワイド文字列変換に必要な文字数分のバッファを確保
	const int cchWideChar = ::MultiByteToWideChar(CP_ACP, 0, lpText, -1, NULL, 0);
	LPWSTR lpw = new WCHAR[cchWideChar];
	if (lpw == NULL){
		return NULL;
	}
	*lpw = L'\0';

	//     上記で求めたワイド文字列バッファを用いて Unicode に変換
	const int nUnicodeCount = ::MultiByteToWideChar(CP_ACP, 0, lpText, -1, lpw, cchWideChar);
	if (nUnicodeCount <= 0){
		delete[] lpw;
		return NULL;
	}

	// (2) Unicode 文字列を UTF-8 に変換
	//     マルチバイト文字列変換に必要な文字数分のバッファを確保
	const int cchMultiByte = ::WideCharToMultiByte(CP_UTF8, 0, lpw, -1, NULL, 0, NULL, NULL);
	LPSTR lpa = new CHAR[cchMultiByte];
	if (lpa == NULL){
		delete[] lpw;
		return NULL;
	}
	*lpa = '\0';

	//     上記で求めたマルチバイト文字列バッファを用いて UTF-8 に変換
	const int nMultiCount = ::WideCharToMultiByte(CP_UTF8, 0, lpw, -1, lpa, cchMultiByte, NULL, NULL);
	if (nMultiCount <= 0){
		delete[] lpw;
		delete[] lpa;
		return NULL;
	}

	// (3) 変換成功。変換に使った一時バッファを解放
	delete[] lpw;

	return lpa;
}

/********************************************************************
機  能  名  称 : コマンド実行
関    数    名 : _execute_command
引          数 : std::string command　	(in)コマンド
                 std::string &result    (out)コマンド実行結果
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
int _execute_command(std::string command, std::string &result)
{
	char ccom[5000]; // コマンド
	char cres[5000]; // 結果
	char cerr[5000]; // エラー

	std::strcpy(ccom, command.c_str());   // コマンドコピー

	int ret = ExecuteCommand(ccom, cres, cerr);
	if (ret)
	{
		char *pTmp = SJIStoUTF8(cres);//文字コード変換

		result = std::string(pTmp);// 結果コピー

		delete[] pTmp;
	} else{
		std::cout << cerr;
		result = std::string("0");
	}


	return ret;
}

std::string _ExecuteCommand(std::string command)
{
	std::string result;

	int ret = _execute_command(command, result);

	return result;
}




BOOST_PYTHON_MODULE(Pimpom) {
	Py_Initialize();
	np::initialize();

	p::def("InitAysnc", _InitAysnc);
	p::def("EnableAysnc", _EnableAysnc);
	p::def("WaitAysnc", _WaitAysnc);
	p::def("ReturnAysnc", _ReturnAysnc);
	p::def("PlotByteImage", _PlotByteImage);
	p::def("PlotByteImageAsync", (void(*)(int, np::ndarray))&_PlotByteImageAsync);
	p::def("PlotByteImageAsync", (void(*)(int, np::ndarray, np::ndarray))&_PlotByteImageAsync);
	p::def("PlotRGBImage", _PlotRGBImage);
	p::def("PlotRGBImageAsync", (void(*)(int, np::ndarray, int))&_PlotRGBImageAsync);
	p::def("PlotRGBImageAsync", (void(*)(int, np::ndarray, int, np::ndarray))&_PlotRGBImageAsync);
	p::def("PlotFloatImage", _PlotFloatImage);
	p::def("Plot3DImage", _Plot3DImage);
	p::def("PlotF3DImage", _PlotF3DImage);
	p::def("SetMaskImage", _SetMaskImage);
	p::def("ExecuteCommand", _ExecuteCommand);
	p::def("GetAysncParams", _GetAysncParams);
	p::def("GetByteImage", _GetByteImage);
	p::def("GetRGBImage", _GetRGBImage);
	p::def("GetByteImageAsync", _GetByteImageAsync);
	p::def("GetRGBImageAsync", _GetRGBImageAsync);
	p::def("Get3DImage", _Get3DImage);
	p::def("GetFloatImage", _GetFloatImage);
	p::def("GetF3DImage", _GetF3DImage);
	p::def("GetMaskImage", _GetMaskImage); 
	p::def("GetWorkArea", _GetWorkArea);
	p::def("PlotDataToChart", _PlotDataToChart);
	p::def("PlotDataToChart2", _PlotDataToChart2);

	pimpom_api_def();
}
