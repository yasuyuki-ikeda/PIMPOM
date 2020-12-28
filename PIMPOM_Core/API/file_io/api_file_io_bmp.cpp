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
//ファイルIO(Bitmap)
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "atlimage.h"

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


/********************************************************************
機  能  名  称 :  ビットマップファイルを読み込む
関    数    名 : LoadBitmapPart
引          数 : long		image_number	(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		pathname	(in)ファイルのフルパス
				int		partX,partY		(in)部分読み込み位置
				int		partW,partH		(in)部分読み込みサイズ
戻    り    値 : 成功すれば true
機          能 : 8bitﾓﾉｸﾛ、24bitｶﾗｰに対応
部分読み込み対応
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LoadBitmap(long image_number, CString	pathname, int partX, int partY, int partW, int partH)
{
	CFile  file;
	BITMAPFILEHEADER	bmp_file_header;
	BITMAPINFOHEADER	bmp_info_header;
	long				bmp_width_gap;
	CDataUnit			*p_du;
	CSize				size;
	unsigned char		*ptmp_rgb;
	int			i,j;

		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//現在選択中の画像を指定する場合

		if( ! file.Open(pathname,CFile::modeRead|CFile::typeBinary) )	return false;

	//BITMAPFILEHEADERの読みこみ
	file.Read( &bmp_file_header , sizeof(BITMAPFILEHEADER) );
		if( bmp_file_header.bfType != 0x4d42){	
			AfxMessageBox(_T("このファイルはビットマップではありません"));
			file.Close();
			return false;
		}

	//BITMAPINFOHEADERの読みこみ
	file.Read( &bmp_info_header, sizeof(BITMAPINFOHEADER) );
	size.cx = bmp_info_header.biWidth;//サイズ決定
	size.cy = bmp_info_header.biHeight;

	//
	if (bmp_info_header.biSize > sizeof(BITMAPINFOHEADER)){
		file.Seek(bmp_info_header.biSize - sizeof(BITMAPINFOHEADER), CFile::current);
	}

		if(bmp_info_header.biBitCount == 8){//8ビットビットマップ
			//新しいメモリを取得
			p_du = FormatDataUnit( image_number , BYTE_FORMAT, size, 1, true);
			if( p_du == NULL )	return	false;//メモリ取得に失敗したとき

			////DIBデータ幅とRAWデータ幅の差
			if( size.cx % 4 == 0)	bmp_width_gap = 0;	
			else			bmp_width_gap = 4 - size.cx % 4;

			////カラーパレットの読み込み
			file.Read( p_du->pDispBufferBmpInfo->bmiColors, 1024);



			//データ読み込み
			if (partX >= 0 && partY >= 0 && partX < size.cx - 1 && partY < size.cy-1 && partW > 0 && partH > 0 )
			{//部分読み込み

				if (partX + partW > size.cx ){
					partW = size.cx - partX;
				}

				if (partY + partH > size.cy ){
					partH = size.cy - partY;
				}

				for (j = size.cy - 1; j >= 0; j--)
				{//上下反転

					if (j >= partY && j < partY + partH)
					{
						file.Seek(partX, CFile::current);
						file.Read(p_du->pByteData + size.cx * j + partX, partW);
						file.Seek(size.cx - (partX + partW), CFile::current);
					}
					else{
						file.Seek(size.cx , CFile::current);
					}

					file.Seek(bmp_width_gap, CFile::current);
				}
			}
			else
			{//全体読み込み
				for (j = size.cy - 1; j >= 0; j--)
				{//上下反転
					file.Read(p_du->pByteData + size.cx * j, size.cx);
					file.Seek(bmp_width_gap, CFile::current);
				}
			}
		}
		else if(bmp_info_header.biBitCount == 24)//24ビットビットマップ
		{
			//新しいメモリを取得
			p_du = FormatDataUnit( image_number , RGB_FORMAT, size, 3, true);
			if( p_du == NULL )	return	false;//メモリ取得に失敗したとき

			////DIBデータ幅とRAWデータ幅の差
			if( (size.cx*3) % 4 == 0)	bmp_width_gap = 0;	
			else				bmp_width_gap = 4 - (size.cx*3) % 4;

			////データの一時保管場所の作成
			ptmp_rgb = (unsigned char*)malloc( (size.cx*3)*size.cy );
			if(!ptmp_rgb)	return	false;
			memset(ptmp_rgb, 0, (size.cx * 3)*size.cy);

			////データ読み込み
			if (partX >= 0 && partY >= 0 && partX < size.cx - 1 && partY < size.cy - 1 && partW > 0 && partH > 0)
			{//部分読み込み

				if (partX + partW > size.cx ){
					partW = size.cx - partX;
				}

				if (partY + partH > size.cy ){
					partH = size.cy - partY;
				}


				for (j = size.cy - 1; j >= 0; j--)
				{
					if (j >= partY && j < partY + partH)
					{
						file.Seek(partX * 3, CFile::current);
						file.Read(ptmp_rgb + (size.cx * 3) * j + partX * 3, (partW * 3));
						file.Seek((size.cx - (partX + partW)) * 3, CFile::current);
					}
					else{
						file.Seek(size.cx * 3, CFile::current);
					}

					file.Seek(bmp_width_gap, CFile::current);
				}
			}
			else
			{//全体読み込み
				for(j = size.cy-1  ;  j>=0  ;  j-- ){//上下反転
					file.Read( ptmp_rgb  +	(size.cx*3) * j  , (size.cx*3)	);
					file.Seek( bmp_width_gap , CFile::current);
				}
			}



			////データの変換
			for(j=0 ; j<size.cy ; j++){
				for(i=0 ; i<size.cx ; i++){
					*(p_du->pByteData + i + j*size.cx )					   = *(ptmp_rgb + i*3 + j*(size.cx*3)+2);//r
					*(p_du->pByteData + i + j*size.cx + size.cx*size.cy)   = *(ptmp_rgb + i*3 + j*(size.cx*3)+1);//g
					*(p_du->pByteData + i + j*size.cx + size.cx*size.cy*2) = *(ptmp_rgb + i*3 + j*(size.cx*3)+0);//b
				}
			}
			free(ptmp_rgb);//データの一時保管場所の開放

		}else if(bmp_info_header.biBitCount == 32){//32ビットビットマップ
			//新しいメモリを取得
			p_du = FormatDataUnit( image_number , RGB_FORMAT, size, 3, true);
			if( p_du == NULL )	return	false;//メモリ取得に失敗したとき

			////データの一時保管場所の作成
			ptmp_rgb = (unsigned char*)malloc( (size.cx*4)*size.cy );
			if(!ptmp_rgb)	return	false;

			////データ読み込み
			for(j = size.cy-1  ;  j>=0  ;  j-- ){//上下反転
				file.Read( ptmp_rgb  +	(size.cx*4) * j  , (size.cx*4)	);
			}
			////データの変換
			for(j=0 ; j<size.cy ; j++){
				for(i=0 ; i<size.cx ; i++){
					*(p_du->pByteData + i + j*size.cx )					   = *(ptmp_rgb + i*4 + j*(size.cx*4)+2);//r
					*(p_du->pByteData + i + j*size.cx + size.cx*size.cy)   = *(ptmp_rgb + i*4 + j*(size.cx*4)+1);//g
					*(p_du->pByteData + i + j*size.cx + size.cx*size.cy*2) = *(ptmp_rgb + i*4 + j*(size.cx*4)+0);//b
				}
			}
			free(ptmp_rgb);//データの一時保管場所の開放

		}else{//8bit,24bit以外
			if( !LoadAnyImage(image_number, pathname) ){
				file.Close();
				return false;
			}
		}



	p_du->DataName = file.GetFileTitle();//名前を付ける
	p_du->SrcFilePath = pathname;//読み込んだファイルのパスを残す
	file.Close();

	SetFileIOLog(pathname);//ファイル名のログを残す

	return	true;
}


/********************************************************************
機  能  名  称 :  サイズ/フォーマット限定でビットマップファイルを読み込む
関    数    名 : LoadSizedBitmap
引          数 : BYTE		*p_data		(out)データ格納先メモリ領域の先頭ポインタ
				CSize		size		(in)画像データのサイズ
				bool		is_rgb		(in)フォーマット（true:24bitｶﾗｰ  false:8bitﾓﾉｸﾛ）
				CString		pathname	(in)ファイルのフルパス
戻    り    値 : 成功すれば true
機          能 : サイズ/ﾌｫｰﾏｯﾄが異なる場合は読み込まない
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::LoadSizedBitmap(BYTE *p_data , CSize size, bool is_rgb, CString	pathname)
{
	CFile  file;
	BITMAPFILEHEADER	bmp_file_header;
	BITMAPINFOHEADER	bmp_info_header;
	long				bmp_width_gap;


		if( ! file.Open(pathname,CFile::modeRead|CFile::typeBinary) )	return false;

		if(p_data == NULL)	return	false;//メモリが確保されていない

	//BITMAPFILEHEADERの読みこみ
	file.Read( &bmp_file_header , sizeof(BITMAPFILEHEADER) );
		if( bmp_file_header.bfType != 0x4d42){	
			//AfxMessageBox(_T("このファイルはビットマップではありません"));
			file.Close();
			return false;
		}

	//BITMAPINFOHEADERの読みこみ
	file.Read( &bmp_info_header, sizeof(BITMAPINFOHEADER) );
		//サイズがあわない
		if(	bmp_info_header.biWidth != size.cx   ||   bmp_info_header.biHeight != size.cy )
		{
			AfxMessageBox(pathname + "はサイズが合いません");
			file.Close();
			return false;
		}

		//カラー/モノクロが合わない
		if( (is_rgb==false && bmp_info_header.biBitCount != 8) ||
			(is_rgb==true  && (bmp_info_header.biBitCount != 24  &&  bmp_info_header.biBitCount != 32)) )
		{
			AfxMessageBox(pathname + "はビットマップ形式が合いません");
			file.Close();
			return false;
		}

		if(bmp_info_header.biBitCount == 8)//モノクロ
		{
			////DIBデータ幅とRAWデータ幅の差
			if( size.cx % 4 == 0)	bmp_width_gap = 0;	
			else					bmp_width_gap = 4 - size.cx % 4;

			file.Seek( 1024 , CFile::current );//カラーパレット分を飛ばす

			////データ読み込み
			for(int j = size.cy-1  ;  j>=0  ;  j-- ){//上下反転
				file.Read( p_data  +  size.cx * j  , size.cx  );
				file.Seek( bmp_width_gap , CFile::current);
			}
		}
		else if(bmp_info_header.biBitCount == 24)//24カラー
		{
			////DIBデータ幅とRAWデータ幅の差
			if( size.cx % 4 == 0)	bmp_width_gap = 0;	
			else					bmp_width_gap = 4 - (size.cx*3) % 4;

			
			////データ読み込み
			BYTE *buffer = new BYTE[size.cx*3];
			if(!buffer)	return false;

			for(int j = size.cy-1  ;  j>=0  ;  j-- ){//上下反転
				file.Read( buffer , size.cx*3  );
				for(int i=0 ; i<size.cx ; i++){
					*(p_data  +  size.cx * j + i)					  = *(buffer + i*3 + 2);//r
					*(p_data  +  size.cx * j + i + size.cx*size.cy)   = *(buffer + i*3 + 1);//g
					*(p_data  +  size.cx * j + i + size.cx*size.cy*2) = *(buffer + i*3 + 0);//b
				}
				file.Seek( bmp_width_gap , CFile::current);
			}

			delete[] buffer;

		}
		else if(bmp_info_header.biBitCount == 32)//32カラー
		{
			////データ読み込み
			BYTE *buffer = new BYTE[size.cx*4];
			if(!buffer)	return false;

			for(int j = size.cy-1  ;  j>=0  ;  j-- ){//上下反転
				file.Read( buffer , size.cx*4  );
				for(int i=0 ; i<size.cx ; i++){
					*(p_data  +  size.cx * j + i)					  = *(buffer + i*4 + 2);//r
					*(p_data  +  size.cx * j + i + size.cx*size.cy)   = *(buffer + i*4 + 1);//g
					*(p_data  +  size.cx * j + i + size.cx*size.cy*2) = *(buffer + i*4 + 0);//b
				}
			}

			delete[] buffer;
		}


	file.Close();

	SetFileIOLog(pathname);//ファイル名のログを残す

	return	true;
}


/********************************************************************
機  能  名  称 :  表示データをビットマップとして保存する
関    数    名 : SaveViewAsBitmap
引          数 : long		image_number	(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		pathname		(in)ファイルのフルパス
戻    り    値 : 成功すれば true
機          能 : 保存されるビットマップのフォーマットは以下のとおり
					RGB型、RGB 3D型⇒ 24bitｶﾗｰ
					それ以外⇒ 8bitﾓﾉｸﾛ
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SaveViewAsBitmap(long image_number, CString	pathname)
{
	CFile		file;
	CDataUnit	*p_du;
	BITMAPFILEHEADER	bmp_file_header;
	CSize	size;

		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//現在選択中の画像を指定する場合

	p_du = GetDataUnit( image_number );//指定された画像メモリを取得する
		if(p_du == NULL)	return	false;//指定された画像メモリにデータが無ければなにもしない
	size = p_du->DataSize;

		if( ! file.Open(pathname , CFile::modeWrite | CFile::modeCreate | CFile::typeBinary ) )	return false; //ファイルを開く
	

	if( (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) || p_du->DispRGB == false){//モノクロ
		//BITMAPFILEHEADER の作成と保存
		bmp_file_header.bfType = 0x4d42;
		bmp_file_header.bfSize = 14+40+1024+(p_du->DispBufferRow * size.cy);
		bmp_file_header.bfReserved1 = 0;
		bmp_file_header.bfReserved2 = 0;
		bmp_file_header.bfOffBits = 54+1024 ;
		file.Write( &bmp_file_header , sizeof(BITMAPFILEHEADER) );

		//BITMAPINFOの保存
		file.Write( p_du->pDispBufferBmpInfo , sizeof(BITMAPINFOHEADER) + 1024 );
		//データの保存
		file.Write( p_du->pDispBuffer , p_du->DispBufferRow * size.cy);

	}else{//RGB
		//BITMAPFILEHEADER の作成と保存
		bmp_file_header.bfType = 0x4d42;
		bmp_file_header.bfSize = 14+40+( p_du->DispBufferRowRGB * size.cy);
		bmp_file_header.bfReserved1 = 0;
		bmp_file_header.bfReserved2 = 0;
		bmp_file_header.bfOffBits = 54;
		file.Write( &bmp_file_header , sizeof(BITMAPFILEHEADER) );

		//BITMAPINFOの保存
		file.Write( p_du->pDispBufferBmpInfoRGB , sizeof(BITMAPINFOHEADER) );
		//データの保存
		file.Write( p_du->pDispBufferRGB , p_du->DispBufferRowRGB * size.cy);
	}


	file.Close();

	SetFileIOLog(pathname);//ファイル名のログを残す

	return	true;
}


/********************************************************************
機  能  名  称 :  表示データをビットマップとして保存する
関    数    名 : SaveViewAsBitmap
引          数 : long		image_number	(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
戻    り    値 : 成功すれば true
機          能 : 保存するファイルを選択するためのファイルダイアログを出す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SaveViewAsBitmap(long image_number)
{
	CDataUnit	*p_du;

	if (image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//現在選択中の画像を指定する場合

	p_du = GetDataUnit(image_number);//していされた画像メモリを取得する
	if (p_du == NULL)	return	false;////指定された画像メモリにデータが無ければなにもしない


	CString pathname,filename;
	filename = GetSrcFileName(image_number);
	CFileDialog myDLG(false , _T("bmp") , filename, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
							"ビットマップ(*.bmp)|*.BMP|すべてのファイル(*.*)|*.*||");


		if( myDLG.DoModal() != IDOK)	return	false;
	pathname = myDLG.GetPathName();

	return	SaveViewAsBitmap( image_number , pathname);
}


/********************************************************************
機  能  名  称 :  8bitのデータをビットマップとして保存する
関    数    名 : SaveByteAsBitmap
引          数 : BYTE			*p_data			(in)データ格納先メモリ領域の先頭ポインタ
				BITMAPINFO		*pBmpInfo		(in)ビットマップ情報
				CSize			size			(in)画像サイズ
				CString			pathname		(in)ファイルのフルパス
戻    り    値 : 成功すれば true
機          能 : 8bitﾓﾉｸﾛビットマップとして保存
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SaveByteAsBitmap(BYTE *p_data , BITMAPINFO *pBmpInfo, CSize	size, CString pathname)
{
	CFile		file;
	BITMAPFILEHEADER	bmp_file_header;
	int j, gap;
	char	blank[4];

		if(p_data ==NULL)	return	false;
		if( ! file.Open(pathname , CFile::modeWrite | CFile::modeCreate | CFile::typeBinary ) )	return false; //ファイルを開く

	//BITMAPFILEHEADER の作成と保存
    bmp_file_header.bfType = 0x4d42;
    bmp_file_header.bfSize = 14+40+1024+( size.cx*size.cy);
    bmp_file_header.bfReserved1 = 0;
    bmp_file_header.bfReserved2 = 0;
    bmp_file_header.bfOffBits = 54+1024 ;
	file.Write( &bmp_file_header , sizeof(BITMAPFILEHEADER) );

	//BITMAPINFOの保存
	file.Write( pBmpInfo , sizeof(BITMAPINFOHEADER) + 1024 );
	size.cx = pBmpInfo->bmiHeader.biWidth;
	size.cy = pBmpInfo->bmiHeader.biHeight;

	//データの保存
	if(size.cx%4 == 0)		gap = 0;
	else					gap = 4 - size.cx%4;

		for(j=size.cy - 1  ;  j>=0  ;  j--){//上下反転させてファイル入力する
			file.Write( p_data + j*size.cx , size.cx);
			file.Write( blank , gap );
		}

	file.Close();

	SetFileIOLog(pathname);//ファイル名のログを残す

	return	true;
}




