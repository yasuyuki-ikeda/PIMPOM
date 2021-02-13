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
//ファイルIO
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "opencv_headers.h"
#include "PIMPOM_API.h"
#include "atlimage.h"


#include <Shlobj.h>
#pragma comment(lib, "Ole32.lib")

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

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED){
		SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
	}
	return 0;
}

CString BrowseForFolder(CString path, CString root)
{
	BOOL		bRes;
	char		chPutFolder[MAX_PATH];
	LPITEMIDLIST	pidlRetFolder;
	BROWSEINFO	stBInfo;
	CString		cRetStr;
	char		c_path[MAX_PATH];


	if (root != _T("")){
		path = root + path;
	}

	strcpy(c_path, path.GetBuffer());
	path.ReleaseBuffer();

	//　構造体を初期化します。
	stBInfo.pidlRoot = NULL;	//ルートフォルダです。
	stBInfo.hwndOwner = NULL;	//表示するダイアログの親ウィンドウのハンドルです。
	stBInfo.pszDisplayName = chPutFolder;	//選択されているフォルダ名を受けます。
	stBInfo.lpszTitle = "";	//説明の文字列です。
	stBInfo.ulFlags = 0;	//表示フラグです。
	stBInfo.lpfn = &BrowseCallbackProc;	//ダイアログプロシージャへのポインタです。
	stBInfo.lParam = (LPARAM)c_path;	//プロシージャに送るパラメーターです。

	//　ダイアログボックスを表示します。
	pidlRetFolder = ::SHBrowseForFolder(&stBInfo);

	//　pidlRetFolderにはそのフォルダを表すアイテムＩＤリストへのポインタが
	//　入っています。chPutFolderには選択されたフォルダ名（非フルパス）だけ
	//　しか入っていないので、このポインタを利用します。

	if (pidlRetFolder != NULL)
	{
		bRes = ::SHGetPathFromIDList(pidlRetFolder, chPutFolder);//　フルパスを取得
		if (bRes != FALSE)
		{
			cRetStr = chPutFolder;

			if (root != ""){//ルートからの相対パス
				cRetStr = cRetStr.Right(cRetStr.GetLength() - root.GetLength());
			}

		}

		::CoTaskMemFree(pidlRetFolder);
	}

	return cRetStr;
}



/********************************************************************
機  能  名  称 : 画像データをファイルから読み込む
関    数    名 : template <class DATA>　load_image_template
引          数 : DATA		*p_data		(out)画像データの配列
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
				long		page		(in)画像データの奥行き数
				long		channel		(in)画像データのチャネル数
戻    り    値 : 成功すれば true
機          能 : 全ての型の画像に対応するためのテンプレート
				ファイルの内容はDATAで指定される型のバイナリのベタデータ

日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
bool	load_image_template(DATA *p_data, CString pathname, CSize size, long page, long channel)
{
	CFile		file;
	CString		str;
	int n;
	DATA *addr;

		//ファイルを開く
		if( ! file.Open(pathname ,  CFile::modeRead | CFile::typeBinary ) ){
			str = pathname + _T(" が開けません");
			AfxMessageBox(str);
			return	false;
		}

		//データを読み込む
		for(n=0 ; n<page*channel ; n++){
			addr = p_data + size.cx * size.cy * n;
			if( file.Read( addr, size.cx * size.cy * sizeof(DATA) ) != size.cx * size.cy * sizeof(DATA) ){
				AfxMessageBox("読み込みに失敗しました");
				file.Close();
				return	false;
			}
		}
	file.Close();//ファイルを閉じる
	
	return	true;
}


/********************************************************************
機  能  名  称 : 画像データをファイルに保存する
関    数    名 : template <class DATA>　save_image_template
引          数 : DATA		*p_data		(in)画像データの配列
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
				long		page		(in)画像データの奥行き数
				long		channel		(in)画像データのチャネル数
戻    り    値 : 成功すれば true
機          能 : 全ての型の画像に対応するためのテンプレート
				ファイルの内容はDATAで指定される型のバイナリのベタデータ

日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
template <class DATA>
bool	save_image_template(DATA *p_data , CString	pathname, CSize size, long page, long channel)
{
	CFile	file;
	CString	str;
	int n;
	DATA *addr;
	long	footer[4]={0};

	if(p_data ==NULL)	return	false;
	if( ! file.Open(pathname , CFile::modeWrite | CFile::modeCreate | CFile::typeBinary ) )	return false; //ファイルを開く

	//データ保存
	for(n=0 ; n<page*channel ; n++){
		addr = p_data + size.cx * size.cy * n;
		file.Write( addr ,  size.cx * size.cy * sizeof(DATA) );
	}

	//フッタを書き込む
	footer[0]=size.cx;
	footer[1]=size.cy;
	footer[2]=page;
	footer[3]=channel;
	file.Write(footer, sizeof(footer) );

	file.Close();//ファイルを閉じる

	return	true;
}



static bool	load_image_internal(CPimpomAPI *pAPI, int data_type, long image_number, CString pathname, CSize size, int page)
{
	CDataUnit	*p_du;
	CString		str;
	bool ret;

		if(image_number == CURRENT_IMAGE)		image_number = pAPI->GetCurrentImageNumber();//現在選択中の画像を指定する場合

	p_du = pAPI->FormatDataUnit( image_number , data_type, size, page, true);//新しいメモリを取得
		if(p_du==NULL)	return	false;//メモリ取得に失敗したとき

		switch(data_type){
			case BYTE_FORMAT:
				ret = load_image_template(p_du->pByteData, pathname, size, 1, 1);
				break;

			case THREE_D_FORMAT:
				ret = load_image_template(p_du->pByteData, pathname, size, page, 1);
				break;

			case SHORT16_FORMAT:
				ret = load_image_template(p_du->pShortData, pathname, size, 1, 1);
				break;

			case WORD_FORMAT:
				ret = load_image_template(p_du->pWordData, pathname, size, 1, 1);
				break;

			case LONG32_FORMAT:
				ret = load_image_template(p_du->pLongData, pathname, size, 1, 1);
				break;

			case DWORD_FORMAT:
				ret = load_image_template(p_du->pDwordData, pathname, size, 1, 1);
				break;

			case FLOAT_FORMAT:
				ret = load_image_template(p_du->pFloatData, pathname, size, 1, 1);
				break;

			case XY_FORMAT:
				ret = load_image_template(p_du->pFloatData, pathname, size, 1, 2);
				break;

			case XYZ_FORMAT:
				ret = load_image_template(p_du->pFloatData, pathname, size, 1, 3);
				break;

			case COMPLEX_FORMAT:
				ret = load_image_template(p_du->pFloatData, pathname, size, 1, 4);
				break;

			case FLOAT_3D_FORMAT:
				ret = load_image_template(p_du->pFloatData, pathname, size, page, 1);
				break;

			case RGB_FORMAT:
				ret = load_image_template(p_du->pByteData, pathname, size, 1, 3);
				break;

			case RGB_3D_FORMAT:
				ret = load_image_template(p_du->pByteData, pathname, size, page, 3);
				break;
		}

	p_du->DataName = ::PathFindFileNameA(pathname);//名前を付ける
	p_du->SrcFilePath = pathname;//読み込んだファイルのパスを残す
	return	ret;
}

/********************************************************************
機  能  名  称 : ファイルIOのログをとる
関    数    名 : SetFileIOLog
引          数 : CString		pathname	(in)ファイルのフルパス
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SetFileIOLog(CString pathname)
{
	CString log[FILE_IO_LOG_MAX], key;

	//現在のログを読み出す
	for(int i=0 ; i<FILE_IO_LOG_MAX ; i++){
		key.Format("LOG%d",i);
		log[i] = GetProfile("FILE_LOG", key);
		if(log[i] == pathname){
			return true;
		}
	}

	//ログ番号をすすめる
	for(int i=FILE_IO_LOG_MAX-1 ; i>0 ; i--){
		log[i]=log[i-1];
	}
	log[0]=pathname;

	//ファイルに書き出す
	for(int i=0 ; i<FILE_IO_LOG_MAX ; i++){
		key.Format("LOG%d",i);
		WriteProfile("FILE_LOG", key, log[i]);
	}

	return true;
}

/********************************************************************
機  能  名  称 : ファイルIOのログを取得する
関    数    名 : GetFileIOLog
引          数 : int	 log_number	(in)ログ番号　番号が小さいほど新しい
戻    り    値 : ファイルパス
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
CString	CPimpomAPI::GetFileIOLog(int log_number)
{
	//現在のログを読み出す
	CString key;
	key.Format("LOG%d",log_number);
	return GetProfile("FILE_LOG", key);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//読み込み
/////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : 8bit型のデータをファイルから読み込む
関    数    名 : LoadByteImage
引          数 : BYTE		*p_data		(out)データ格納先メモリ領域の先頭ポインタ
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LoadByteImage(BYTE *p_data, CString pathname, CSize size)
{
	bool ret= load_image_template(p_data, pathname, size, 1,1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
機  能  名  称 : 8bit型のデータをファイルから読み込む
関    数    名 : LoadByteImage
引          数 : long image_number		(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LoadByteImage(long image_number, CString pathname, CSize size)
{
	bool ret = load_image_internal(this, BYTE_FORMAT, image_number, pathname, size, 1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}

/********************************************************************
機  能  名  称 : 16bit符号つき型のデータをファイルから読み込む
関    数    名 : LoadShortImage
引          数 : short		*p_data		(out)データ格納先メモリ領域の先頭ポインタ
				CString		filename	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LoadShortImage(short *p_data, CString pathname, CSize size)
{
	bool ret =  load_image_template(p_data, pathname, size, 1,1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}

/********************************************************************
機  能  名  称 : 16bit符号なし型のデータをファイルから読み込む
関    数    名 : LoadWordImage
引          数 : WORD		*p_data		(out)データ格納先メモリ領域の先頭ポインタ
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LoadWordImage(WORD *p_data, CString pathname, CSize size)
{
	bool ret =  load_image_template(p_data, pathname, size, 1,1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
機  能  名  称 : 16bit符号つき型データをファイルから読み込む
関    数    名 : LoadShortImage
引          数 : long image_number		(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LoadShortImage(long image_number, CString pathname, CSize size)
{
	bool ret =  load_image_internal(this, SHORT16_FORMAT, image_number, pathname, size, 1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}

/********************************************************************
機  能  名  称 : 16bit符号なし型のデータをファイルから読み込む
関    数    名 : LoadWordImage
引          数 : long image_number		(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LoadWordImage(long image_number, CString pathname, CSize size)
{
	bool ret =  load_image_internal(this, WORD_FORMAT, image_number, pathname, size, 1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
機  能  名  称 : 32bit符号付き型のデータをファイルから読み込む
関    数    名 : LoadLongImage
引          数 : long		*p_data		(out)データ格納先メモリ領域の先頭ポインタ
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
戻    り    値 : 成功すれば true
機          能 : 

日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LoadLongImage(long *p_data, CString pathname, CSize size)
{
	bool ret = load_image_template(p_data, pathname, size, 1, 1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}

/********************************************************************
機  能  名  称 : 32bit符号なし型のデータをファイルから読み込む
関    数    名 : LoadDWordImage
引          数 : DWORD		*p_data		(out)データ格納先メモリ領域の先頭ポインタ
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
戻    り    値 : 成功すれば true
機          能 : 

日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LoadDWordImage(DWORD *p_data, CString pathname, CSize size)
{
	bool ret = load_image_template(p_data, pathname, size, 1, 1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}

/********************************************************************
機  能  名  称 : 32bit符号つき型のデータをファイルから読み込む
関    数    名 : LoadLongImage
引          数 : long image_number		(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LoadLongImage(long image_number, CString pathname, CSize size)
{
	bool ret =  load_image_internal(this, LONG32_FORMAT, image_number, pathname, size, 1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}

/********************************************************************
機  能  名  称 : 32bit符号なし型のデータをファイルから読み込む
関    数    名 : LoadDwordImage
引          数 : long image_number		(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LoadDwordImage(long image_number, CString pathname, CSize size)
{
	bool ret =  load_image_internal(this, DWORD_FORMAT, image_number, pathname, size, 1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
機  能  名  称 : float(32浮動小数点)型のデータをファイルから読み込む
関    数    名 : LoadFloatImage
引          数 : float		*p_data		(out)データ格納先メモリ領域の先頭ポインタ
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
戻    り    値 : 成功すれば true
機          能 : 

日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LoadFloatImage(float *p_data, CString pathname, CSize size)
{
	bool ret = load_image_template(p_data, pathname, size, 1,1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
機  能  名  称 : float(32浮動小数点)型のデータをファイルから読み込む
関    数    名 : LoadFloatImage
引          数 : long image_number		(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LoadFloatImage(long image_number, CString	pathname, CSize size)
{
	bool ret = load_image_internal(this, FLOAT_FORMAT, image_number, pathname, size, 1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
機  能  名  称 : 複素数(32浮動小数点×4)型のデータをファイルから読み込む
関    数    名 : LoadComplexImage
引          数 : long image_number		(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LoadComplexImage(long image_number, CString	pathname, CSize size)
{
	bool ret = load_image_internal(this, COMPLEX_FORMAT, image_number, pathname, size, 4);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
機  能  名  称 : XY(32浮動小数点×2)型のデータをファイルから読み込む
関    数    名 : LoadXYImage
引          数 : long image_number		(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LoadXYImage(long image_number, CString	pathname, CSize size)
{
	bool ret = load_image_internal(this, XY_FORMAT, image_number, pathname, size, 2);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
機  能  名  称 : XYZ(32浮動小数点×3)型のデータをファイルから読み込む
関    数    名 : LoadXYZImage
引          数 : long image_number		(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
//画像メモリ番号をCURRENT_IMAGEにした場合は現在選択中の画像に対して処理をおこなう
bool	CPimpomAPI::LoadXYZImage(long image_number, CString	pathname, CSize size)
{
	bool ret = load_image_internal(this, XYZ_FORMAT, image_number, pathname, size, 3);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
機  能  名  称 : 3D(8bitの連続画像)型のデータをファイルから読み込む
関    数    名 : Load3DImage
引          数 : long image_number		(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::Load3DImage(long image_number, CString	pathname, CSize size, long page)
{
	bool ret = load_image_internal(this, THREE_D_FORMAT, image_number, pathname, size, page);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
機  能  名  称 : Float3D(32bit浮動小数点の連続画像)型のデータをファイルから読み込む
関    数    名 : LoadF3DImage
引          数 : long image_number		(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像データのサイズ
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LoadF3DImage(long image_number, CString	pathname, CSize size, long page)
{
	bool ret = load_image_internal(this, FLOAT_3D_FORMAT, image_number, pathname, size, page);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
機  能  名  称 : 画像ファイルを読み込む
関    数    名 : SelectAndLoadImageFile
引          数 : long image_number		(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
戻    り    値 : 成功すれば true
機          能 : 保存するファイルを選ぶためのファイルダイアログを出し、
				選ばれたファイルの拡張子からタイプを判断してファイルを開く
				 対応している拡張子は以下のとおり
				 *.bmp: 8bitﾓﾉｸﾛ、24bitｶﾗｰ
				 *.raw: 8bitﾓﾉｸﾛ
				 *.jpg: 24bitｶﾗｰ
				 *.word: 16bit(PIMPOM専用ﾌｫｰﾏｯﾄ)
				 *.long: 32bit(PIMPOM専用ﾌｫｰﾏｯﾄ)
				 *.float: 32bit浮動少数点(PIMPOM専用ﾌｫｰﾏｯﾄ)
				 *.comp: 複素数(PIMPOM専用ﾌｫｰﾏｯﾄ)
				 *.xy: 2次元分布(PIMPOM専用ﾌｫｰﾏｯﾄ)
				 *.xyz: 3次元分布(PIMPOM専用ﾌｫｰﾏｯﾄ)
				 *.3d: 8bit動画(PIMPOM専用ﾌｫｰﾏｯﾄ)
				 *.f3d: 32bit浮動少数点動画(PIMPOM専用ﾌｫｰﾏｯﾄ)
				 *.pim: 画像メモリのアーカイブデータ(PIMPOM専用ﾌｫｰﾏｯﾄ)

日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SelectAndLoadImageFile(long image_number)
{
	int i;
	CString		file_type_name[15], all_file_type;

		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//現在選択中の画像を指定する場合

	//入力可能なファイル名
	file_type_name[0].LoadString(IDS_FILETYPE_ANY);//*.*
	file_type_name[1].LoadString(IDS_FILETYPE_BITMAP);//*.bmp
	file_type_name[2].LoadString(IDS_FILETYPE_RAW);//*.raw
	file_type_name[3].LoadString(IDS_FILETYPE_JPEG);//*.jpg
	file_type_name[4].LoadString(IDS_FILETYPE_WORD);//*.word
	file_type_name[5].LoadString(IDS_FILETYPE_SHORT);//*.short
	file_type_name[6].LoadString(IDS_FILETYPE_LONG);//*.long
	file_type_name[7].LoadString(IDS_FILETYPE_DWORD);//*.dword
	file_type_name[8].LoadString(IDS_FILETYPE_FLOAT);//*.float
	file_type_name[9].LoadString(IDS_FILETYPE_COMPLEX);//*.comp
	file_type_name[10].LoadString(IDS_FILETYPE_XY);//*.xy
	file_type_name[11].LoadString(IDS_FILETYPE_XYZ);//*.xyz
	file_type_name[12].LoadString(IDS_FILETYPE_3D);//*.3d
	file_type_name[13].LoadString(IDS_FILETYPE_F3D);//*.f3d
	file_type_name[14].LoadString(IDS_FILETYPE_ARCHIVED);//*.pim
	

	all_file_type.Empty();
		for(i=0 ; i<13 ; i++)	all_file_type += file_type_name[i];
	all_file_type += _T("|");

	//複数枚入力を可能にする
	CFileDialog myDLG(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT , all_file_type );
	char	name[ MULTI_FILE_PATH_BUFFER ];
	name[0] = '\0';
	myDLG.m_ofn.lpstrFile = name;
	myDLG.m_ofn.nMaxFile  = MULTI_FILE_PATH_BUFFER-1;

		if(myDLG.DoModal() != IDOK)  return	false;

	POSITION	pos = myDLG.GetStartPosition();

		//選択したファイルを順番に読み込む
		while( ( pos != NULL )   &&   ( image_number < data_number ) )
		{
			if (SelectAndLoadImageFile(image_number, myDLG.GetNextPathName(pos)))
			{
				DrawImage( image_number, true,true);
			}

			image_number ++;
		}
	return	true;
}



/********************************************************************
機  能  名  称 : バイナリ画像ファイルのフッタ部から画像の幅・高さ・奥行・チャネルを読みだす
関    数    名 : get_file_footer
引          数 : CString	pathname						(in)ファイルのフルパス
				 CString	ext								(in)ファイルの拡張子
				 short		*pwidth,*pheight,*ppage	,*pchannel		(out)画像の幅/高さ/奥行/チャネル（フッタから読み取ったもの）読み取り失敗時は-1
戻    り    値 : 幅・高さ・奥行が画像から読み取れればtrueを返す
機          能 : バイナリ画像ファイルのフッタ部を読み出し、ファイルサイズと比較する
				比較結果が妥当ならば、幅・高さ・奥行・チャネルを出力する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool get_file_footer(CString pathname, CString ext, short *pwidth, short *pheight, short *ppage, short *pchannel)
{
	CFile		file;
	CFileStatus status;
	int			pixsize=1, expected_filesize;
	long	footer[4]={0};


	//ファイルを開く
	if( ! file.Open(pathname ,  CFile::modeRead | CFile::typeBinary ) ){
		return	false;
	}
	
	file.GetStatus(status);//ファイルサイズを取得する
	file.Seek(status.m_size-sizeof(footer), CFile::begin );
	file.Read(footer, sizeof(footer));//フッタ部を取得する
	file.Close();

	//ビットサイズ
	ext.MakeUpper();
	if( ext == _T("RAW") )		{	pixsize=1;		}
	else if( ext == _T("WORD") ){	pixsize=2;		}
	else if( ext == _T("SHORT") ){	pixsize=2;		}
	else if( ext == _T("DWORD") ){	pixsize=4;		}
	else if( ext == _T("LONG") ){	pixsize=4;		}
	else if( ext == _T("FLOAT") ){	pixsize=4;		}
	else if( ext == _T("COMP") ){	pixsize=4;		}
	else if( ext == _T("XY") ){		pixsize=4;		}
	else if( ext == _T("XYZ") ){	pixsize=4;		}
	else if( ext == _T("3D") ){		pixsize=1;		}
	else if( ext == _T("F3D") ){	pixsize=4;		}
	else{	
		return false;	
	}

	//フッタからファイルサイズを予測
	expected_filesize = footer[0]*footer[1]*footer[2]*footer[3]*pixsize + sizeof(footer);
	if(expected_filesize==status.m_size){
		*pwidth = (short)footer[0];
		*pheight = (short)footer[1];
		*ppage = (short)footer[2];
		*pchannel = (short)footer[3];
		return true;	
	}else{
		*pwidth = -1;
		*pheight = -1;
		*ppage = -1;
		*pchannel = -1;
		return false;
	}

}

/********************************************************************
機  能  名  称 : 画像ファイルを読み込む
関    数    名 : SelectAndLoadImageFile
引          数 : long		image_number		(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				 CString	pathname			(in)ファイルのフルパス
				 short		width				(in)画像の幅
				 short		height				(in)画像の高さ
				 short		page				(in)画像の奥行き
戻    り    値 : 成功すれば true
機          能 : 保存するファイルを選ぶためのファイルダイアログを出し、
				選ばれたファイルの拡張子からタイプを判断してファイルを開く

日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SelectAndLoadImageFile(long image_number, CString pathname, short width, short height, short page)
{
	long		len, i;
	CString		ext, mes;
	CConfirmDlg	confirm;
	bool		need_confirm=false;
	short		channel;

		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//現在選択中の画像を指定する場合

	//ファイル名から拡張子を抜き出す
	len = pathname.GetLength();
		for(i = len-1 ; i>=0  ; i-- ){
			if( pathname.GetAt(i) == '.' ){
				ext = pathname.Right( len-1-i ); 
				break;
			}
		}

	//フッタを読みだす
	get_file_footer(pathname, ext, &width, &height, &page, &channel);

	
	//サイズ指定が無い場合は、入力UIを出す
	if(width<=0 || height<=0 || page<=0){
		need_confirm = true;
		width = 640;
		height = 480;
		page = 1;
		confirm.SetWidth(50, 50);
		confirm.SetTitle("ファイルサイズ");
		confirm.RegistVar("幅", &width);
		confirm.RegistVar("高さ", &height);
		confirm.RegistVar("ページ", &page);
	}

	//拡張子によって入力するタイプを判断する
	ext.MakeUpper();
		if( ext == _T("RAW") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadByteImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("BMP") ){
			return	LoadBitmap( image_number, pathname);

		}else if( ext == _T("SHORT") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadShortImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("WORD") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadWordImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("LONG") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadLongImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("DWORD") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadDwordImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("FLOAT") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadFloatImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("COMP") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadComplexImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("XY") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadXYImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("XYZ") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadXYZImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("3D") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	Load3DImage( image_number , pathname , CSize(width, height), page );

		}else if( ext == _T("F3D") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadF3DImage( image_number , pathname , CSize(width, height), page );

		}else if( ext == _T("PIM") ){
			return	LoadArchivedImage( image_number , pathname);

		}
		else if (ext == _T("IFZ")) {
			return LoadIFZ(image_number, pathname);
		}
		else if (ext == _T("BFZ")) {
			return LoadBFZ(image_number, pathname);
		}else{
			return  LoadAnyImage( image_number, pathname);
		}
}


/********************************************************************
機  能  名  称 : 画像ファイルを読み込む
関    数    名 : LoadAnyImage
引          数 : long		image_number		(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				 CString	pathname			(in)ファイルのフルパス
				 short		width				(in)画像の幅
				 short		height				(in)画像の高さ
				 short		page				(in)画像の奥行き
戻    り    値 : 成功すれば true
機          能 : 保存するファイルを選ぶためのファイルダイアログを出し、
				選ばれたファイルの拡張子からタイプを判断してファイルを開く

日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
			 Y.Ikeda         OpenCV活用（http://opencv.jp/opencv-2svn/cpp/highgui_reading_and_writing_images_and_video.html）
********************************************************************/
bool CPimpomAPI::LoadAnyImage(long image_number, CString pathname)
{
	cv::Mat img = cv::imread((LPCTSTR)pathname, -1);

	if (!ConvertFromCvImage2(image_number, &img))
	{
		MessageBox("%s ファイルが開けません", pathname.GetBuffer()); 
		pathname.ReleaseBuffer();
		return false;
	}


	///読み込んだファイルのパスを残す
	CDataUnit *p_du = GetDataUnit(image_number);
	p_du->DataName = ::PathFindFileNameA(pathname);
	p_du->SrcFilePath = pathname;

	DrawImage();


	SetFileIOLog(pathname);

	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//保存
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : 表示画像(8bit)をファイルに保存する
関    数    名 : SaveViewAsRaw
引          数 : long		image_number(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		pathname	(in)ファイルのフルパス
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SaveViewAsRaw(long image_number, CString	pathname)
{
	CFile		file;
	CDataUnit	*p_du;
	CSize	size;
	long	footer[4]={0};

		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//現在選択中の画像を指定する場合

	p_du = GetDataUnit( image_number );//指定された画像メモリを取得する
		if(p_du == NULL)	return	false;//指定された画像メモリにデータが無ければなにもしない
		if(p_du->pDispBuffer == NULL)	return	false;//8bit表示画像がなければなにもしない
	
	size = p_du->DataSize;

		if( ! file.Open(pathname , CFile::modeWrite | CFile::modeCreate | CFile::typeBinary ) )	return false; //ファイルを開く

	file.Write(p_du->pDispBuffer , p_du->DispBufferRow * size.cy );//データを書き込む

	//フッタを書き込む
	footer[0]=p_du->DispBufferRow;
	footer[1]=size.cy;
	footer[2]=1;
	footer[3]=1;
	file.Write(footer, sizeof(footer) );

	file.Close();//ファイルを閉じる

	SetFileIOLog(pathname);//ファイル名のログを残す

	return	true;
}




/********************************************************************
機  能  名  称 : 8bit型のデータをファイルに保存する
関    数    名 : SaveByteImage
引          数 : BYTE		*p_data		(out)データ格納先メモリ領域の先頭ポインタ
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像のサイズ
				long		page		(in)画像の奥行き数
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SaveByteImage(BYTE *p_data , CString	pathname, CSize size, long page)
{
	bool ret = save_image_template(p_data, pathname, size, page,1);
	if(ret){
		SetFileIOLog(pathname);//ファイル名のログを残す
	}
	return ret;
}

/********************************************************************
機  能  名  称 : 16bit符号付き型のデータをファイルに保存する
関    数    名 : SaveShortImage
引          数 : short		*p_data		(out)データ格納先メモリ領域の先頭ポインタ
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像のサイズ
				long		page		(in)画像の奥行き数
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SaveShortImage(short *p_data , CString	pathname, CSize size, long page)
{
	bool ret = save_image_template(p_data, pathname, size, page,1);
	if(ret){
		SetFileIOLog(pathname);//ファイル名のログを残す
	}
	return ret;
}

/********************************************************************
機  能  名  称 : 16bit型のデータをファイルに保存する
関    数    名 : SaveWordImage
引          数 : WORD		*p_data		(out)データ格納先メモリ領域の先頭ポインタ
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像のサイズ
				long		page		(in)画像の奥行き数
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SaveWordImage(WORD *p_data , CString	pathname, CSize size, long page)
{
	bool ret = save_image_template(p_data, pathname, size, page,1);
	if(ret){
		SetFileIOLog(pathname);//ファイル名のログを残す
	}
	return ret;
}

/********************************************************************
機  能  名  称 : 32bit符号つき型のデータをファイルに保存する
関    数    名 : SaveLongImage
引          数 : long		*p_data		(out)データ格納先メモリ領域の先頭ポインタ
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像のサイズ
				long		page		(in)画像の奥行き数
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SaveLongImage(long *p_data , CString	pathname, CSize size, long page)
{
	bool ret = save_image_template(p_data, pathname, size, page,1);
	if(ret){
		SetFileIOLog(pathname);//ファイル名のログを残す
	}
	return ret;
}

/********************************************************************
機  能  名  称 : 32bit型のデータをファイルに保存する
関    数    名 : SaveDWORDImage
引          数 : DWORD		*p_data		(out)データ格納先メモリ領域の先頭ポインタ
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像のサイズ
				long		page		(in)画像の奥行き数
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SaveDwordImage(DWORD *p_data , CString	pathname, CSize size, long page)
{
	bool ret = save_image_template(p_data, pathname, size, page,1);
	if(ret){
		SetFileIOLog(pathname);//ファイル名のログを残す
	}
	return ret;
}

/********************************************************************
機  能  名  称 : float(32浮動小数点)型のデータをファイルに保存する
関    数    名 : SaveFloatImage
引          数 : float		*p_data		(out)データ格納先メモリ領域の先頭ポインタ
				CString		pathname	(in)ファイルのフルパス
				CSize		size		(in)画像のサイズ
				long		page		(in)画像の奥行き数
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SaveFloatImage(float *p_data, CString	pathname, CSize size, long page)
{
	bool ret = save_image_template(p_data, pathname, size, page,1);
	if(ret){
		SetFileIOLog(pathname);//ファイル名のログを残す
	}
	return ret;
}


/********************************************************************
機  能  名  称 : 画像データをファイルを保存する
関    数    名 : SelectAndSaveImageFile
引          数 : long image_number	(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
戻    り    値 : 成功すれば true
機          能 : 画像データのタイプ別にファイルを保存する
				保存するファイルを選ぶためのファイルダイアログを出す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SelectAndSaveImageFile(long image_number)
{
	CDataUnit	*p_du;
	CString		filename, fileext, file_type_name;

		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//現在選択中の画像を指定する場合

	p_du = GetDataUnit( image_number );//現在選択中の画像メモリを取得する
		if(p_du == NULL)	return	false;//現在選択中の画像メモリにデータが無ければなにもしない

	//ファイルの種類をきめる
		switch( p_du->DataType ){
			case BYTE_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_RAW);//*.raw
				fileext = _T("raw");
				break;

			case FLOAT_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_FLOAT);//*.float
				fileext = _T("float");
				break;

			case COMPLEX_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_COMPLEX);//*.comp
				fileext = _T("comp");
				break;

			case XY_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_XY);//*.xy
				fileext = _T("xy");
				break;

			case XYZ_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_XYZ);//*.xyz
				fileext = _T("xyz");
				break;

			case THREE_D_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_3D);//*.3d
				fileext = _T("3d");
				break;

			case FLOAT_3D_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_F3D);//*.f3d
				fileext = _T("f3d");
				break;

			case RGB_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_RAW);//*.bmp
				fileext = _T("bmp");
				break;

			case SHORT16_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_SHORT);//*.short
				fileext = _T("short");
				break;

			case WORD_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_WORD);//*.word
				fileext = _T("word");
				break;

			case LONG32_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_LONG);//*.long
				fileext = _T("long");
				break;

			case DWORD_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_DWORD);//*.dword
				fileext = _T("dword");
				break;

			//case RGB_3D_FORMAT:
			//	file_type_name.LoadString(IDS_FILETYPE_BITMAP);//*.rgb3d
			//	fileext = _T("rgb3d");
			//	break;

			default:
				return false;
		}

	CFileDialog myDLG(FALSE, fileext, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, file_type_name);
		if( myDLG.DoModal() != IDOK )	return	false;

	filename = myDLG.GetPathName();
	return	SelectAndSaveImageFile( image_number, filename );
}


/********************************************************************
機  能  名  称 : 画像データをファイルを保存する
関    数    名 : SelectAndSaveImageFile
引          数 : long		image_number	(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		filename		(in)ファイルのフルパス
戻    り    値 : 成功すれば true
機          能 : 画像データのタイプ別にファイルを保存する
				保存するファイルを選ぶためのファイルダイアログを出す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SelectAndSaveImageFile(long image_number, CString pathname)
{
	CDataUnit	*p_du;
	CSize	size;

		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//現在選択中の画像を指定する場合

	p_du = GetDataUnit( image_number );//指定された画像メモリを取得する
		if(p_du == NULL)	return	false;//指定された画像メモリにデータが無ければなにもしない

	if( p_du->DataType!= RGB_FORMAT && p_du->DataType!= RGB_3D_FORMAT )//グレー
	{
		if( p_du->pByteData ){
				return	SaveByteImage( p_du->pByteData , pathname, p_du->DataSize, p_du->PageNumber );

		}else if( p_du->pShortData ){
				return	SaveShortImage( p_du->pShortData , pathname, p_du->DataSize, p_du->PageNumber );

		}else if( p_du->pWordData ){
				return	SaveWordImage( p_du->pWordData , pathname, p_du->DataSize, p_du->PageNumber );

		}else if( p_du->pLongData ){
				return	SaveLongImage( p_du->pLongData , pathname, p_du->DataSize, p_du->PageNumber );

		}else if( p_du->pDwordData ){
				return	SaveDwordImage( p_du->pDwordData , pathname, p_du->DataSize, p_du->PageNumber );

		}else if( p_du->pFloatData ){
				return	SaveFloatImage( p_du->pFloatData , pathname, p_du->DataSize, p_du->PageNumber );
		}else{
			return false;
		}
	}else{//RGB
		SetDispRange(image_number,0,256);
		return SaveViewAsBitmap(image_number,pathname);
	}
}


//ファイルパスからディレクトリパスを抜き出す
CString dirpathFromFilepath(CString path)
{
	while (path.GetLength()>0)
	{
		if (PathIsDirectoryA(path))
		{
			return path;
		}
		else
		{
			int i = path.ReverseFind('\\');
			if (i > 0)
			{
				path = path.Left(i);
			}
			else
			{
				return "";
			}
		}
	}

	return "";
}

/********************************************************************
機  能  名  称 : 読み込むファイルを選ぶためのファイルダイアログを出す
関    数    名 : OpenFileDialog
引          数 : int		file_number_max	(in)ファイル最大数
				 CString	file_kinds,		(in)ファイルの種類
				CString		*pfile_path,	(out)ファイルまでのパス
				CString		pfile_names[],	(out)ファイル名（名順でソート）
				int			*pfile_number	(out)ファイル数
				 char		def_path[]		(in)最初に開くディレクトリパスへのポインタ(NULLならばカレントディレクトリをつかう)
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::OpenFileDialog(int		file_number_max, //ファイル最大数(IN)
						 CString	file_kinds,		//ファイルの種類（ファイルダイアログ呼び出し時に使用）(IN)
						 CString	*pfile_path,	//パス(OUT)
						 CString	pfile_names[],	//ファイル名(OUT)
						 int		*pfile_number,	//ファイル数(OUT)
						 char		def_path[]		//最初に開くディレクトリパス
						)
{
	short			n, i;
	char			name[ MULTI_FILE_PATH_BUFFER ];
	POSITION		pos;
	CString			path_name, file_name, path, title, ext, tmp;
	CFileDialog		myDLG(true , NULL , NULL , OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_ALLOWMULTISELECT ,//ファイルの複数枚選択を可能にする
							file_kinds);	
	TCHAR			t_def_path[MAX_PATH];

		if(file_number_max <=0)		return false;

	//ファイルダイアログを表示する
	name[0] = '\0';
	myDLG.m_ofn.lpstrFile = name;
	myDLG.m_ofn.nMaxFile  = MULTI_FILE_PATH_BUFFER-1;

	if(def_path){
		CString dirpath = dirpathFromFilepath(CString(def_path));
		_tcscpy( t_def_path, dirpath);
		myDLG.m_ofn.lpstrInitialDir = t_def_path;
	}

		if( myDLG.DoModal() != IDOK )	return false;	

	//ファイルダイアログから読み込むファイルのデータを取得する
	title = myDLG.GetFileTitle();//ファイルのタイトルを取得する
	
		if( !title.IsEmpty() ){//１ファイルのみ選択時
			ext = myDLG.GetFileExt();//ファイルの拡張子を取得する
			path_name = myDLG.GetPathName();
			file_name = path_name.Right(path_name.GetLength() - path_name.ReverseFind('\\') -1);//myDLG.GetFileName();//ファイル名を取得する
			path = path_name.Left( path_name.GetLength()  -  file_name.GetLength() );//ファイルのパスを取得する

			*pfile_path   = path;
			*pfile_names  = file_name;
			*pfile_number = 1;

		}else{//複数のファイルが選択されたときは，全てのファイル名を読み込むファイルのリストに追加する
			pos	= myDLG.GetStartPosition();
			n = 0;

				while( pos != NULL  &&  n < file_number_max ){
					path_name = myDLG.GetNextPathName(pos);
					path	 = myDLG.GetPathName() + _T("\\");
					file_name = path_name.Right( path_name.GetLength() - path.GetLength());

					pfile_names[n] = file_name;

					n ++;

					//ファイル名をアルファベット順にソート
					for(i=n-1 ; i>0 ; i-- ){
						if(  strcmp(pfile_names[i-1], pfile_names[i]) > 0 ){
							tmp = pfile_names[i-1];
							pfile_names[i-1] = pfile_names[i];
							pfile_names[i]=tmp;
						}
					}
				}
			*pfile_path = path;
			*pfile_number = n;
		}

	return true;
}

/********************************************************************
機  能  名  称 : 読み込むファイルを選ぶためのダイアログを出す
関    数    名 : OpenFileDialog
引          数 : int		file_number_max	(in)ファイル最大数
				CString		file_kind_ext	(in)ファイルの拡張子
				CString		file_kind_disp	(in)ファイルの種類説明
				CString		*pfile_path,	(out)ファイルまでのパス
				CString		pfile_names[],	(out)ファイル名（名順でソート）
				int			*pfile_number	(out)ファイル数
				 char		def_path[]		(in)最初に開くディレクトリパスへのポインタ(NULLならばカレントディレクトリをつかう)
戻    り    値 : 成功すれば true
機          能 : ファイルの拡張子は１種類のみ

日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::OpenFileDialog(int		file_number_max,	//ファイル最大数(IN)
									CString	file_kind_ext,		//ファイルの拡張子(IN)
									CString	file_kind_disp,		//ファイルの種類説明(IN)
									CString	*pfile_path,		//パス(OUT)
									CString	pfile_names[],		//ファイル名(OUT)
									int		*pfile_number,		//ファイル数(OUT)
									 char		def_path[]	//最初に開くディレクトリパス
									)
{
	CString file_kind;

	file_kind = file_kind_disp + _T("(*.") + file_kind_ext + _T(")|*.") + file_kind_ext.MakeUpper() + _T("|すべてのファイル(*.*)|*.*||");

	return OpenFileDialog(file_number_max, file_kind, pfile_path, pfile_names, pfile_number, def_path);
}



/********************************************************************
機  能  名  称 : 書き込むファイルを選ぶためのファイルダイアログを出す
関    数    名 : SaveFileDialog
引          数 : 
                 CString	file_kinds    (IN)ファイルの種類（ファイルダイアログ呼び出し時に使用）
				 CString	*pfile_path,  (IN/OUT)パス
戻    り    値 : 成功すれば true
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SaveFileDialog(
			CString	file_kinds,		//ファイルの種類（ファイルダイアログ呼び出し時に使用）(IN)
			CString	*pfile_path		//パス(IN/OUT)
)
{
	CFileDialog myDLG(false, _T("pim"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, file_kinds);
	TCHAR			t_def_path[MAX_PATH];

	if (pfile_path) {
		CString dirpath = dirpathFromFilepath(*pfile_path);
		_tcscpy(t_def_path, dirpath);
		myDLG.m_ofn.lpstrInitialDir = t_def_path;
	}

	if (myDLG.DoModal() != IDOK)  return false;
	*pfile_path = myDLG.GetPathName();

	return true;
}


/********************************************************************
機  能  名  称 : PIMPOM.exeのファイルパスを取得する
関    数    名 : GetDefaultPath
引          数 : 
戻    り    値 : PIMPOM.exeのファイルパス
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
CString CPimpomAPI::GetDefaultPath()
{
	static CString strAppPath;
	if(strAppPath.IsEmpty())
	{
		// アプリケーションパスを取得
		if(GetModuleFileName(NULL, strAppPath.GetBuffer(_MAX_PATH), _MAX_PATH))
		{
			strAppPath.ReleaseBuffer();
			char szDrive[_MAX_PATH]; // D:
			char szDir[_MAX_PATH]; // \\folders\\ 
			char szFName[_MAX_PATH]; // filename
			char szExt[_MAX_PATH]; // .ext

			// フルパスを分解
			_tsplitpath(strAppPath, szDrive, szDir, szFName, szExt);

			// パスを作成
			strAppPath.Format(_T("%s%s"), szDrive, szDir); 
		} 
		else		
		{// エラーの場合は空のCStringを返す
			strAppPath.ReleaseBuffer();
			strAppPath.Empty();
		}
	}
	return strAppPath;
}


/********************************************************************
機  能  名  称 : フォルダを選ぶダイアログを出す
関    数    名 : GetDefaultPath
引          数 : 
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::FolderDialog(CString *pfolder)
{
	*pfolder = BrowseForFolder(*pfolder, "");

	if (*pfolder == ""){
		return false;
	}

	return true;
}

/********************************************************************
機  能  名  称 : フォルダ直下のファイル一覧を作成する
関    数    名 : GetDefaultPath
引          数 :
戻    り    値 : ファイル数を返す
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
int	CPimpomAPI::GetFileList(
					CString		folderpath,			//(in)フォルダパス
					CString		ext,				//(in)ファイル拡張子
					int			file_number_max,	//(in)ファイル数上限
					CString		pfile_names[]		//(out)ファイル名一覧
					)
{
	CFileFind    finder;
	BOOL         bWorking = finder.FindFile(folderpath + _T("\\*.") + ext);
	int cnt = 0;

	//find all *.bmp file names and add them to the list
	while (bWorking && cnt<file_number_max)
	{
		bWorking = finder.FindNextFile();//find a file name

		if (finder.IsDots()) continue;
		if (finder.IsDirectory())continue;

		CString filename = finder.GetFileName();

		if (filename.Right(ext.GetLength()) == ext)
		{
			pfile_names[cnt] = filename;
			cnt++;
		}	
	}


	return cnt;
}