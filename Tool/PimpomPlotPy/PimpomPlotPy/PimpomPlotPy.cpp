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





/********************************************************************
機  能  名  称 : PIMPOMに8bitモノクロ画像データを出力する
関    数    名 : _PlotByteImage
引          数 : int imgNo　	(in)画像メモリ番号
				np::ndarray img (in)画像データ配列(2次元のnumpy配列 [row][col])
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void _PlotByteImage(int imgNo, np::ndarray img)
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


	//printf("w=%d, h=%d, ch=%d",w,h,ch);

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

	PlotByteImage(imgNo, w, h, tmp);

	delete[] tmp;
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
void _PlotRGBImage(int imgNo, np::ndarray img, int order_rgb)
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

	if (ch != 3){
		throw std::runtime_error("last channel length must be 3");
	}

	//printf("w=%d, h=%d, ch=%d",w,h,ch);

	unsigned char *tmp = new unsigned char[ch * w * h];
	unsigned char *ptr = reinterpret_cast<unsigned char *>(img.get_data());

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			for (int c = 0; c < ch; c++)
			{
				if (order_rgb){
					tmp[c*w*h + j*w + i] = *ptr;
				}
				else{
					tmp[(ch-1-c)*w*h + j*w + i] = *ptr;
				}
				
				ptr++;
			}
		}
	}


	PlotRGBImage(imgNo, w, h, tmp);

	delete[] tmp;
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
　               np::ndarray img (in)画像データ配列(3次元のnumpy配列 [row][col][channel])
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

	int h = shape[0];
	int w = shape[1];
	int ch = shape[2];



	//printf("w=%d, h=%d, ch=%d",w,h,ch);

	float *tmp = new float[ch * w * h];
	float *ptr = reinterpret_cast<float *>(img.get_data());

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

	PlotF3DImage(imgNo, w, h, ch, tmp);

	delete[] tmp;
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


	p::def("PlotByteImage", _PlotByteImage);
	p::def("PlotRGBImage", _PlotRGBImage);
	p::def("PlotFloatImage", _PlotFloatImage);
	p::def("Plot3DImage", _Plot3DImage);
	p::def("PlotF3DImage", _PlotF3DImage);
	p::def("SetMaskImage", _SetMaskImage);
	p::def("ExecuteCommand", _ExecuteCommand);
	p::def("GetByteImage", _GetByteImage);
	p::def("GetRGBImage", _GetRGBImage);
	p::def("Get3DImage", _Get3DImage);
	p::def("GetFloatImage", _GetFloatImage);
	p::def("GetF3DImage", _GetF3DImage);
	p::def("GetMaskImage", _GetMaskImage); 
	p::def("GetWorkArea", _GetWorkArea);

	pimpom_api_def();
}
