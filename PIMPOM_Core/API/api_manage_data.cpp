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
//画像メモリの操作
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "Shlwapi.h"

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
機  能  名  称 : 現在選択中の画像メモリの番号を取得する
関    数    名 : GetCurrentImageNumber
引          数 : 
戻    り    値 : 現在選択中の画像データの番号
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
long		CPimpomAPI::GetDataUnitNumber()
{
	return		data_number;
}

/********************************************************************
機  能  名  称 : 現在選択中の画像メモリの番号を取得する
関    数    名 : GetCurrentImageNumber
引          数 : 
戻    り    値 : 現在選択中の画像データの番号
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
long		CPimpomAPI::GetCurrentImageNumber()
{
	return		current_image_number;
}

/********************************************************************
機  能  名  称 : //現在選択中の画像データの番号を変更する（再描画はしない）
関    数    名 : SetCurrentImageNumber
引          数 : 
戻    り    値 : 成功するとtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool		CPimpomAPI::SetCurrentImageNumber(long number)
{
	//引数チェック
	if(number<0 || number>= GetDataUnitNumber())	return false;

	current_image_number = number;

	return true;
}

/********************************************************************
機  能  名  称 : DataUnitクラスへのポインタをを取得する
関    数    名 : GetDataUnit
引          数 : long		number		(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
戻    り    値 : DataUnitクラスへのポインタ。データが無い場合はNULL
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
CDataUnit	*CPimpomAPI::GetDataUnit(long number)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>= GetDataUnitNumber())	return NULL;

	return		pp_data_unit[number];
}

/********************************************************************
機  能  名  称 : 画像メモリの型・サイズ・奥行等取得
関    数    名 : GetDataTypeSize
引          数 : long		number		(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 int		*ptype		(out)画像メモリの型
				 int		*pwidth		(out)画像幅
				 int		*pheight	(out)画像高さ
				 int		*ppages		(out)画像の奥行
戻    り    値 : データが無い場合はfalse それ以外はtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::GetDataTypeSize(long number, int *ptype, int *pwidth, int *pheight, int *ppages)
{
	CDataUnit *pdu = GetDataUnit(number);
	if(!pdu)	return false;

	*ptype = pdu->DataType;
	*pwidth = pdu->DataSize.cx;
	*pheight = pdu->DataSize.cy;
	*ppages = pdu->PageNumber;

	return true;
}

/********************************************************************
機  能  名  称 : 画像メモリの最大・最小を返す
関    数    名 : GetDataTypeSize
引          数 : long		number		(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 float		*pMin		(out)最小値
				 float		*pMax		(out)最大値
戻    り    値 : データが無い場合はfalse それ以外はtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::GetDataMinMax(long number, float *pMin, float *pMax)
{
	CDataUnit *pdu = GetDataUnit(number);
	if(!pdu)	return false;

	pdu->GetDataMinMax(pMin, pMax);

	return true;
}


/********************************************************************
機  能  名  称 : 表示データのページを設定する
関    数    名 : SetDispPage
引          数 : long		number		(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long		page		(in)ページ
戻    り    値 : データが無い場合はfalse それ以外はtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::SetDispPage(long number, long page)
{
	CDataUnit *pdu = GetDataUnit(number);
	if(!pdu)	return false;

	pdu->SetDispPage(page);

	return true;
}

/********************************************************************
機  能  名  称 : 表示データのチャネルを設定する
関    数    名 : GetDataTypeSize
引          数 : long		number		(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long		channel		(in)チャネル
戻    り    値 : データが無い場合はfalse それ以外はtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CPimpomAPI::SetDispChannel(long number, long channel)
{
	CDataUnit *pdu = GetDataUnit(number);
	if(!pdu)	return false;

	pdu->SetDispChannel(channel);

	return true;
}


/********************************************************************
機  能  名  称 : 指定した形式の画像メモリ（DataUnit）を取得する
関    数    名 : FormatDataUnit
引          数 : long		number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long		type			(in)画像データのタイプ
				 CSize		size			(in)画像データのサイズ
				 long		page			(in)画像データの奥行き
				 bool		do_clear_memory	(in)既に同タイプ・サイズのメモリが確保されている場合、現在のデータをクリアするかどうか
戻    り    値 : DataUnitクラスへのポインタ。失敗した場合はNULL
機          能 : numberで指定された番号にDataUnitのメモリ未確保の場合は、新たにメモリ確保する
				指定さえたタイプ・サイズと異なるメモリがある場合は、内部データのみメモリ取り直す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
2013.05.06   Y.Ikeda         GetDataUnitで得たポインタがリンク切れを起こさないように修正
********************************************************************/
CDataUnit	*CPimpomAPI::FormatDataUnit(long number, long type, CSize size, long page, bool do_clear_memory)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>= GetDataUnitNumber())	return NULL;


	if(size.cx<=0 || size.cy<=0)	return NULL;//画像サイズが不正
	
	if(page<=0 )	return NULL;



	if(!do_clear_memory){//再利用
		CDataUnit *p_du;
		if( p_du = GetDataUnit(number),  p_du != NULL){//すでにメモリが存在する	
			if( p_du->Compare(type, size, page) ){//メモリタイプ・サイズがおなじ
				return	p_du;
			}
		}
	}
			
	if( pp_data_unit[number] == NULL )
	{
		pp_data_unit[number] = new CDataUnit(type, size, page);//メモリを新規確保する
	}
	else
	{
		pp_data_unit[number]->Destroy();//現在入っているデータを削除する

		if(!pp_data_unit[number]->Format(type, size, page)){//DataUnitのメモリをとりなおす
			delete	pp_data_unit[number];
			pp_data_unit[number]=NULL;
		}
	}

	return	pp_data_unit[number];
}

/********************************************************************
機  能  名  称 : 画像メモリ（DataUnit）を削除する
関    数    名 : DeleteDataUnit
引          数 : long		number		(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::DeleteDataUnit(long number)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>= GetDataUnitNumber())	return false;

	if( pp_data_unit[number] != NULL ){

		delete	pp_data_unit[number];//現在入っているデータを削除する
		pp_data_unit[number] = NULL;
		DrawImage(number);

		return true;
	}else{
		return false;
	}
}


/********************************************************************
機  能  名  称 : 画像メモリの内容をコピーする
関    数    名 : CopyDataUnit
引          数 : long		src_number		(in)コピー元画像メモリ番号
				 long		dest_number		(in)コピー先画像メモリ番号(src_numberと同じ値は指定不可)
				 bool		show_confirm	(in)コピー実行時に警告メッセージボックスを出すかどうか
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::CopyDataUnit(long src_number, 
								 long dest_number, 
								 bool show_confirm)
{
	//引数チェック
	if(src_number<0 || src_number>=GetDataUnitNumber() )	return false;//src_numberが不正
	if(dest_number<0 || dest_number>=GetDataUnitNumber() )	return false;//dest_numberが不正
	if( src_number == dest_number )		return	false;//同じ画像メモリ番号
	if( pp_data_unit[ src_number ] == NULL)		return	false;//コピー元に何もない


	CDataUnit	*p_dest_du;
	CConfirmDlg confirm;
	bool		do_copy_data=true;
	bool		do_copy_mask=true;
	bool		do_copy_workarea=true;
	bool		do_copy_range=true;
	bool		do_copy_mark=true;
	bool		do_copy_pos_scale=true;

	p_dest_du = pp_data_unit[ dest_number ];
	if(p_dest_du != NULL && show_confirm)
	{
		confirm.SetWidth(100, 0);
		confirm.SetTitle("コピーするデータ");
		confirm.RegistVar("画像データ", &do_copy_data);
		confirm.RegistVar("マスク", &do_copy_mask);
		confirm.RegistVar("マーク", &do_copy_mark);
		confirm.RegistVar("表示濃度レンジ", &do_copy_range);
		confirm.RegistVar("作業領域", &do_copy_workarea);
		confirm.RegistVar("表示位置", &do_copy_pos_scale);

		if(confirm.DoModal()!=IDOK)		return	false;//コピーするデータ種類の問い合わせ
	}
	
	return CopyDataUnit(src_number, dest_number, do_copy_data, do_copy_mask, do_copy_workarea, do_copy_range, do_copy_mark, do_copy_pos_scale);
}

/********************************************************************
機  能  名  称 : 画像メモリの内容をコピーする
関    数    名 : CopyDataUnit
引          数 : long		src_number			(in)コピー元画像メモリ番号
				 long		dest_number			(in)コピー先画像メモリ番号(src_numberと同じ値は指定不可)
				 bool		do_copy_data		(in)画像メモリの中をコピー（画像メモリの型/サイズ/ページ/チャネルが違う場合はメモリ再取得）
				 bool		do_copy_mask		(in)マスクをコピー（サイズが同じ場合のみ）
				 bool		do_copy_workarea	(in)作業領域をコピー
				 bool		do_copy_range		(in)表示画像の濃度レンジをコピー
				 bool		do_copy_mark		(in)マークをコピーするかどうか
				 bool		do_copy_pos_scale	(in)表示位置とスケールをコピー（サイズが同じ場合のみ）
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::CopyDataUnit(long src_number, 
								 long dest_number, 
								 bool do_copy_data, 
								 bool do_copy_mask, 
								 bool do_copy_workarea,
								 bool do_copy_range,
								 bool do_copy_mark,
								 bool do_copy_pos_scale)
{
	CDataUnit	*p_src_du, *p_dest_du;
	bool		data_rearoc_flg = false;//コピー先に画メモリ(再)取得するかどうか


	//引数チェック
	if(src_number<0 || src_number>=GetDataUnitNumber() )	return false;//src_numberが不正
	if(dest_number<0 || dest_number>=GetDataUnitNumber() )	return false;//dest_numberが不正
	if( src_number == dest_number )		return	false;//同じ画像メモリ番号
	if( ( p_src_du = pp_data_unit[ src_number ] ) == NULL)		return	false;//コピー元のデータがない
		


	p_dest_du = pp_data_unit[ dest_number ];

		if( p_dest_du == NULL)//コピー先に画像メモリがない場合はメモリ取得する
		{
			data_rearoc_flg = true;
		}
		else
		{	
			if(do_copy_data)//画像データをコピーする場合
			{
				if(p_dest_du->DataSize != p_src_du->DataSize ||
					p_dest_du->DataType != p_src_du->DataType ||
					p_dest_du->PageNumber != p_src_du->PageNumber ||
					p_dest_du->ChannelNumber != p_src_du->ChannelNumber 
					)
				{
						data_rearoc_flg = true;//コピー先と画像メモリの型/サイズ/ページ/チャネルが違う場合はメモリ再取得する
				}
			}
		}

		if(data_rearoc_flg)
		{//コピー先に画像メモリ取得
			p_dest_du = FormatDataUnit(dest_number, p_src_du->DataType, p_src_du->DataSize, p_src_du->PageNumber, true);//コピー先にあたらしいメモリを取得する
			if( p_dest_du == NULL)		return	false;//メモリ取得失敗
		}


		//画像データをコピーする
		if( !p_dest_du->Copy( p_src_du, do_copy_data,
					do_copy_mask,
					do_copy_workarea,
					do_copy_range,
					do_copy_mark,
					do_copy_pos_scale))	return	false;




	return	true;//成功したらtrueを返す
}



/********************************************************************
機  能  名  称 : BYTE型の画像メモリを取得
関    数    名 : GetByteMemory
引          数 : long		number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 CSize		size			(in)画像サイズ
				 bool		do_clear_memory	(in)既に同タイプ・サイズのメモリが確保されている場合、現在のデータをクリアするかどうか
戻    り    値 : 画像メモリへのポインタ。取得失敗した場合はNULL
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
unsigned char	*CPimpomAPI::GetByteMemory(long number, CSize size, bool do_clear_memory)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_numberが不正

	if(size.cx<=0 || size.cy<=0)	return NULL;//画像サイズが不正


	//画像メモリを取得
	CDataUnit	*p_du = FormatDataUnit(number, BYTE_FORMAT, size, 1, do_clear_memory);

		if(p_du != NULL)		return	p_du->pByteData;
		else					return	NULL;//メモリ取得に失敗した場合
}


/********************************************************************
機  能  名  称 : FLOAT型の画像メモリを取得
関    数    名 : GetFloatMemory
引          数 : long		number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 CSize		size			(in)画像サイズ
				 bool		do_clear_memory	(in)既に同タイプ・サイズのメモリが確保されている場合、現在のデータをクリアするかどうか
戻    り    値 : 画像メモリへのポインタ。取得失敗した場合はNULL
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
float			*CPimpomAPI::GetFloatMemory(long number, CSize size, bool do_clear_memory)
{	
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_numberが不正

	if(size.cx<=0 || size.cy<=0)	return NULL;//画像サイズが不正
	

	//画像メモリを取得
	CDataUnit *p_du = FormatDataUnit(number, FLOAT_FORMAT, size, 1, do_clear_memory);

		if(p_du != NULL)		return	p_du->pFloatData;
		else					return	NULL;//メモリ取得に失敗した場合
}


/********************************************************************
機  能  名  称 : 8bit動画型の画像メモリを取得
関    数    名 : Get3DMemory
引          数 : long		number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 CSize		size			(in)画像サイズ
				 long		page			(in)画像奥行き
				 bool		do_clear_memory	(in)既に同タイプ・サイズのメモリが確保されている場合、現在のデータをクリアするかどうか
戻    り    値 : 画像メモリへのポインタ。取得失敗した場合はNULL
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
unsigned char	*CPimpomAPI::Get3DMemory(long number, CSize size, long page, bool do_clear_memory)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_numberが不正

	if(size.cx<=0 || size.cy<=0)	return NULL;//画像サイズが不正

	//画像メモリを取得
	CDataUnit *p_du = FormatDataUnit(number, THREE_D_FORMAT, size, page, do_clear_memory);

		if(p_du != NULL)		return	p_du->pByteData;
		else					return	NULL;//メモリ取得に失敗した場合
}

/********************************************************************
機  能  名  称 : 32bit動画型の画像メモリを取得
関    数    名 : GetF3DMemory
引          数 : long		number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 CSize		size			(in)画像サイズ
				 long		page			(in)画像奥行き
				 bool		do_clear_memory	(in)既に同タイプ・サイズのメモリが確保されている場合、現在のデータをクリアするかどうか
戻    り    値 : 画像メモリへのポインタ。取得失敗した場合はNULL
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
float			*CPimpomAPI::GetF3DMemory(long number, CSize size, long page, bool do_clear_memory)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_numberが不正

	if(size.cx<=0 || size.cy<=0)	return NULL;//画像サイズが不正


	//画像メモリを取得
	CDataUnit *p_du = FormatDataUnit(number, FLOAT_3D_FORMAT, size, page, do_clear_memory);

		if(p_du != NULL)		return	p_du->pFloatData;
		else					return	NULL;//メモリ取得に失敗した場合
}

/********************************************************************
機  能  名  称 : 複素数型の画像メモリを取得
関    数    名 : GetComplexMemory
引          数 : long		number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 CSize		size			(in)画像サイズ
				 bool		do_clear_memory	(in)既に同タイプ・サイズのメモリが確保されている場合、現在のデータをクリアするかどうか
戻    り    値 : 画像メモリへのポインタ。取得失敗した場合はNULL
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
float			*CPimpomAPI::GetComplexMemory(long number, CSize size, bool do_clear_memory)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_numberが不正

	if(size.cx<=0 || size.cy<=0)	return NULL;//画像サイズが不正

	
	//画像メモリを取得
	CDataUnit *p_du = FormatDataUnit(number, COMPLEX_FORMAT, size, 4, do_clear_memory);

		if(p_du != NULL)		return	p_du->pFloatData;
		else					return	NULL;//メモリ取得に失敗した場合
}

/********************************************************************
機  能  名  称 : XY型の画像メモリを取得
関    数    名 : GetXYMemory
引          数 : long		number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 CSize		size			(in)画像サイズ
				 bool		do_clear_memory	(in)既に同タイプ・サイズのメモリが確保されている場合、現在のデータをクリアするかどうか
戻    り    値 : 画像メモリへのポインタ。取得失敗した場合はNULL
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
float			*CPimpomAPI::GetXYMemory(long number, CSize size, bool do_clear_memory)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>=GetDataUnitNumber() )	return false;//src_numberが不正

	if(size.cx<=0 || size.cy<=0)	return false;//画像サイズが不正


	//画像メモリを取得
	CDataUnit *p_du = FormatDataUnit(number, XY_FORMAT, size, 2, do_clear_memory);

		if(p_du != NULL)		return	p_du->pFloatData;
		else					return	NULL;//メモリ取得に失敗した場合
}

/********************************************************************
機  能  名  称 : XYZ型の画像メモリを取得
関    数    名 : GetXYZMemory
引          数 : long		number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 CSize		size			(in)画像サイズ
				 bool		do_clear_memory	(in)既に同タイプ・サイズのメモリが確保されている場合、現在のデータをクリアするかどうか
戻    り    値 : 画像メモリへのポインタ。取得失敗した場合はNULL
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
float			*CPimpomAPI::GetXYZMemory(long number, CSize size, bool do_clear_memory)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>=GetDataUnitNumber() )	return false;//src_numberが不正

	if(size.cx<=0 || size.cy<=0)	return false;//画像サイズが不正


	//画像メモリを取得
	CDataUnit *p_du = FormatDataUnit(number, XYZ_FORMAT, size, 3, do_clear_memory);

		if(p_du != NULL)		return	p_du->pFloatData;
		else					return	NULL;//メモリ取得に失敗した場合
}

/********************************************************************
機  能  名  称 : RGBｶﾗｰ型の画像メモリを取得
関    数    名 : GetRGBMemory
引          数 : long		number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 CSize		size			(in)画像サイズ
				 bool		do_clear_memory	(in)既に同タイプ・サイズのメモリが確保されている場合、現在のデータをクリアするかどうか
戻    り    値 : 画像メモリへのポインタ。取得失敗した場合はNULL
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
unsigned char	*CPimpomAPI::GetRGBMemory(long number, CSize size, bool do_clear_memory)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_numberが不正

	if(size.cx<=0 || size.cy<=0)	return NULL;//画像サイズが不正
		

	//画像メモリを取得
	CDataUnit *p_du = FormatDataUnit(number, RGB_FORMAT, size, 3, do_clear_memory);

		if(p_du != NULL)		return	p_du->pByteData;
		else					return	NULL;//メモリ取得に失敗した場合
}

/********************************************************************
機  能  名  称 : RGBｶﾗｰ動画型の画像メモリを取得
関    数    名 : GetRGBMemory
引          数 : long		number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 CSize		size			(in)画像サイズ
				 long		page			(in)画像奥行き
				 bool		do_clear_memory	(in)既に同タイプ・サイズのメモリが確保されている場合、現在のデータをクリアするかどうか
戻    り    値 : 画像メモリへのポインタ。取得失敗した場合はNULL
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
unsigned char	*CPimpomAPI::GetRGB3DMemory(long number, CSize size, long page, bool do_clear_memory)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_numberが不正

	if(size.cx<=0 || size.cy<=0)	return NULL;//画像サイズが不正
		

	//画像メモリを取得
	CDataUnit *p_du = FormatDataUnit(number, RGB_3D_FORMAT, size, page, do_clear_memory);

		if(p_du != NULL)		return	p_du->pByteData;
		else					return	NULL;//メモリ取得に失敗した場合
}

/********************************************************************
機  能  名  称 : 16bit符号付き型の画像メモリを取得
関    数    名 : GetShortMemory
引          数 : long		number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 CSize		size			(in)画像サイズ
				 bool		do_clear_memory	(in)既に同タイプ・サイズのメモリが確保されている場合、現在のデータをクリアするかどうか
戻    り    値 : 画像メモリへのポインタ。取得失敗した場合はNULL
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
short			*CPimpomAPI::GetShortMemory(long number, CSize size, bool do_clear_memory)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_numberが不正

	if(size.cx<=0 || size.cy<=0)	return NULL;//画像サイズが不正

	
	//画像メモリを取得
	CDataUnit *p_du = FormatDataUnit(number, SHORT16_FORMAT, size, 1, do_clear_memory);

		if(p_du != NULL)		return	p_du->pShortData;
		else					return	NULL;//メモリ取得に失敗した場合
}

/********************************************************************
機  能  名  称 : 16bit型の画像メモリを取得
関    数    名 : GetWordMemory
引          数 : long		number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 CSize		size			(in)画像サイズ
				 bool		do_clear_memory	(in)既に同タイプ・サイズのメモリが確保されている場合、現在のデータをクリアするかどうか
戻    り    値 : 画像メモリへのポインタ。取得失敗した場合はNULL
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
WORD			*CPimpomAPI::GetWordMemory(long number, CSize size, bool do_clear_memory)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_numberが不正

	if(size.cx<=0 || size.cy<=0)	return NULL;//画像サイズが不正

	
	//画像メモリを取得
	CDataUnit *p_du = FormatDataUnit(number, WORD_FORMAT, size, 1, do_clear_memory);

		if(p_du != NULL)		return	p_du->pWordData;
		else					return	NULL;//メモリ取得に失敗した場合
}

/********************************************************************
機  能  名  称 : 32bit符号つき型の画像メモリを取得
関    数    名 : GetLongMemory
引          数 : long		number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 CSize		size			(in)画像サイズ
				 bool		do_clear_memory	(in)既に同タイプ・サイズのメモリが確保されている場合、現在のデータをクリアするかどうか
戻    り    値 : 画像メモリへのポインタ。取得失敗した場合はNULL
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
long			*CPimpomAPI::GetLongMemory(long number, CSize size, bool do_clear_memory)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_numberが不正

	if(size.cx<=0 || size.cy<=0)	return NULL;//画像サイズが不正
	

	//画像メモリを取得
	CDataUnit *p_du = FormatDataUnit(number, LONG32_FORMAT, size, 1, do_clear_memory);

		if(p_du != NULL)		return	p_du->pLongData;
		else					return	NULL;//メモリ取得に失敗した場合
}

/********************************************************************
機  能  名  称 : 32bit型の画像メモリを取得
関    数    名 : GetDwordMemory
引          数 : long		number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 CSize		size			(in)画像サイズ
				 bool		do_clear_memory	(in)既に同タイプ・サイズのメモリが確保されている場合、現在のデータをクリアするかどうか
戻    り    値 : 画像メモリへのポインタ。取得失敗した場合はNULL
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
DWORD			*CPimpomAPI::GetDwordMemory(long number, CSize size, bool do_clear_memory)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_numberが不正

	if(size.cx<=0 || size.cy<=0)	return NULL;//画像サイズが不正
	

	//画像メモリを取得
	CDataUnit *p_du = FormatDataUnit(number, DWORD_FORMAT, size, 1, do_clear_memory);

		if(p_du != NULL)		return	p_du->pDwordData;
		else					return	NULL;//メモリ取得に失敗した場合
}

/********************************************************************
機  能  名  称 : マスクメモリを取得
関    数    名 : GetLongMemory
引          数 : long		number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
戻    り    値 : マスクメモリへのポインタ
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
BYTE			*CPimpomAPI::GetMaskMemory(long number)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_numberが不正

	//画像メモリを取得
	CDataUnit *p_du = GetDataUnit(number);

		if(p_du != NULL)		return	p_du->pMaskData;
		else					return	NULL;//メモリが無い場合
}


/********************************************************************
機  能  名  称 : 読み込み元のファイルの絶対パス取得
関    数    名 : GetSrcFilePathName
引          数 : long		number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
戻    り    値 : ファイルから読み込まれた画像メモリに対し、読み込み元のファイルのフルパスを返す
				 ファイルから読み込まれていない画像メモリでは、ヌル文字が帰る
				 画像メモリが存在しない場合はヌル文字が帰る
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
CString			CPimpomAPI::GetSrcFilePathName(long number)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>=GetDataUnitNumber() )	return CString("");//src_numberが不正

	//画像メモリを取得
	CDataUnit *p_du = GetDataUnit(number);

		if(p_du != NULL)		return	p_du->SrcFilePath;
		else					return	CString("");//メモリが無い場合
}


CString			CPimpomAPI::GetSrcFileName(long number)
{
	CString full_path = GetSrcFilePathName(number);

	return CString( PathFindFileName(full_path) );
}

CString			CPimpomAPI::GetSrcFilePath(long number)
{
	CString full_path = GetSrcFilePathName(number);
	CString name = GetSrcFileName(number);

	return full_path.Left(full_path.GetLength()-name.GetLength());
}


CString			CPimpomAPI::GetSrcFileTitle(long number)
{
	CString filename = GetSrcFileName(number);
	CString filetitle = filename;

	for(int i=filename.GetLength()-1 ; i>1 ; i--){
		if(filename[i]=='.'){
			filetitle = filename.Left(i);
			break;
		}
	}

	return filetitle;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//作業領域
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : 作業領域を設定(矩形)
関    数    名 : SetWorkArea
引          数 : RECT		rect			(in)矩形
				 long		number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long		figure_number	(in)図形番号
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool		CPimpomAPI::SetWorkArea(long image_number, long figure_number, RECT rect, bool ornot )
{
	CDataUnit *pdu;
	if( !(pdu = GetDataUnit(image_number)) )		return false;

	pdu->WorkArea.SetWorkArea(figure_number, rect, ornot);
	return true;
}

/********************************************************************
機  能  名  称 : 作業領域を設定(幅つき直線)
関    数    名 : SetWorkArea
引          数 : FIGURE_LINE		line			(in)幅つき直線
				 long				number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long				figure_number	(in)図形番号
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool		CPimpomAPI::SetWorkArea(long image_number, long figure_number, FIGURE_LINE line, bool ornot )
{
	CDataUnit *pdu;
	if( !(pdu = GetDataUnit(image_number)) )		return false;

	pdu->WorkArea.SetWorkArea(figure_number, line, ornot);
	return true;
}

/********************************************************************
機  能  名  称 : 作業領域を設定(楕円)
関    数    名 : SetWorkArea
引          数 : FIGURE_ELLIPSE	ellipse			(in)楕円
				 long				number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long				figure_number	(in)図形番号
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool		CPimpomAPI::SetWorkArea(long image_number, long figure_number, FIGURE_ELLIPSE ellipse, bool ornot)
{
	CDataUnit *pdu;
	if( !(pdu = GetDataUnit(image_number)) )		return false;

	return pdu->WorkArea.SetWorkArea(figure_number, ellipse, ornot);
}

/********************************************************************
機  能  名  称 : 作業領域を設定(幅つき円周)
関    数    名 : SetWorkArea
引          数 : FIGURE_CIRCLE	circle			(in)幅つき円周
				 long				number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long				figure_number	(in)図形番号
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool		CPimpomAPI::SetWorkArea(long image_number, long figure_number, FIGURE_CIRCLE circle, bool ornot )
{
	CDataUnit *pdu;
	if( !(pdu = GetDataUnit(image_number)) )		return false;

	return pdu->WorkArea.SetWorkArea(figure_number, circle, ornot);
}

/********************************************************************
機  能  名  称 : 作業領域を設定(幅つき円弧)
関    数    名 : SetWorkArea
引          数 : FIGURE_ARC		arc				(in)幅つき円弧
				 long				number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long				figure_number	(in)図形番号
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool		CPimpomAPI::SetWorkArea(long image_number, long figure_number, FIGURE_ARC arc, bool ornot )
{
	CDataUnit *pdu;
	if( !(pdu = GetDataUnit(image_number)) )		return false;

	return pdu->WorkArea.SetWorkArea(figure_number, arc, ornot);
}

/********************************************************************
機  能  名  称 : 作業領域を取得
関    数    名 : GetWorkArea
引          数 : FIGURE_UNION	*area			(out)作業領域の図形
				 bool				*ornot			(out)true:OR領域　false:NOT領域
				 long				image_number	(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long				figure_number	(in)図形番号
戻    り    値 : 作業領域の種類。失敗したら-1
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int		CPimpomAPI::GetWorkArea(long image_number, long figure_number, FIGURE_UNION *area, bool *ornot )
{
	CDataUnit *pdu;
	if( !(pdu = GetDataUnit(image_number)) )		return -1;

	if(figure_number<0 || figure_number>=WORKAREA_FIG_NUM)	return -1;
	
	memcpy(area, &pdu->WorkArea.Figure[figure_number], sizeof(FIGURE_UNION));
	*ornot = pdu->WorkArea.OrNot[figure_number];
	return pdu->WorkArea.Type[figure_number];
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//マーク
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
機  能  名  称 : マークを追加する
関    数    名 : AddMark
引          数 : long				image_number	(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 float				x,y				(in)マーク座標
				 double				*p_attrib		(in)属性配列
戻    り    値 : 成功したら追加されたマーク番号を返す失敗したら-1を返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
long	CPimpomAPI::AddMark(long image_number, float x, float y, double *p_attrib)
{
	CMark mark = CMark(x,y);
	if(p_attrib){
		memcpy(mark.attrib, p_attrib, sizeof(double)*MARK_ATTRIB_NUM);
	}else{
		memset(mark.attrib,0,sizeof(double)*MARK_ATTRIB_NUM);
	}

	return AddMark(image_number,mark);
}


long	CPimpomAPI::AddLineMark(long image_number, float x1, float y1, float x2, float y2, double *p_attrib)
{
	CMark mark = CMark(x1,y1,x2,y2);
	if(p_attrib){
		memcpy(mark.attrib, p_attrib, sizeof(double)*MARK_ATTRIB_NUM);
	}else{
		memset(mark.attrib,0,sizeof(double)*MARK_ATTRIB_NUM);
	}

	return AddMark(image_number,mark);
}


long	CPimpomAPI::AddMark(long image_number, CMark mark)
{
	CDataUnit	*pdu= GetDataUnit(image_number);
	if( pdu==NULL )	return -1;//データがないとき

	int num = pdu->AddMark(mark);
	if(num<0)		return -1;
			
	SetCheck( pPIMMOMDlg->m_hWnd , IDC_BUTTON_PIMPON_DLG_MARK, true);//メインウィンドウのマーク表示ON

	//マークウィンドウに反映
	if(pMarkDlg){
		pMarkDlg->ShowWindow(SW_SHOW);
		pMarkDlg->ShowMarkVal(image_number);
	}

	return num;
}

/********************************************************************
機  能  名  称 : マークを消去する
関    数    名 : DeleteMark
引          数 : long				image_number	(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long				number			(in)マーク番号
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::DeleteMark(long image_number, long mark_number)
{
	CDataUnit	*pdu = GetDataUnit(image_number);

	if( pdu==NULL )		return	false;//データが無いとき

	return pdu->DeleteMark(mark_number);
}


/********************************************************************
機  能  名  称 : すべてのマークを消去する
関    数    名 : DeleteMark
引          数 : long				image_number	(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::ClearMark(long number)
{
	CDataUnit	*pdu = GetDataUnit(number);

	if( pdu==NULL )		return	false;//データが無いとき

	pdu->ClearMark();

	return true;
}

/********************************************************************
機  能  名  称 : マークの座標とデータ値を取得する
関    数    名 : GetMarkVal
引          数 : long				image_number	(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long				number			(in)マーク番号
				 float				*px,*py			(out)マーク座標
				 double				*p_val			(out)マーク座標のデータ値
				 double				*p_attrib		(out)マークの属性(配列)
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::GetMarkVal(long image_number, long mark_number, float *px, float *py, double *p_val, double *p_attrib)
{
	CDataUnit	*pdu = GetDataUnit(image_number);
	if( pdu==NULL )		return	false;//データが無いとき
	
	CMark mark;
	if( !pdu->GetMark(mark_number, &mark))	return false;

	*px = mark.figure.point.x;//マークの座標を取得
	*py = mark.figure.point.y;
	*p_val = pdu->GetDataValue( *px, *py ,-1,-1);//マーク座標の値を取得

	if(p_attrib){
		memcpy( p_attrib, mark.attrib, MARK_ATTRIB_NUM*sizeof(double));//マークの属性を取得
	}

	return	true;
}

/********************************************************************
機  能  名  称 : マークの数を取得する
関    数    名 : GetMarkNumber
引          数 : long				image_number	(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
戻    り    値 : マークの数 失敗したら-1を返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
long	CPimpomAPI::GetMarkNumber(long image_number)
{
	CDataUnit	*pdu;

	pdu = GetDataUnit(image_number);
		if( pdu==NULL )		return	-1;//データが無いとき

	return		pdu->GetMarkNumber();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//バックアップ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
機  能  名  称 : 画像データをバックアップする
関    数    名 : BackupDataUnit
引          数 : long				image_number	(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::BackupDataUnit(long number)
{
	//引数チェック
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>=GetDataUnitNumber() )	return false;//src_numberが不正

	CDataUnit *psrc_du = GetDataUnit(number);
		if( !psrc_du )		return	false;//バックアップ元の画像が無ければなにもしない
		
		if( backup_data!=NULL )	delete	backup_data;

	backup_data = new CDataUnit(	psrc_du->DataType, psrc_du->DataSize, psrc_du->PageNumber);
	if(!backup_data)	return false;

	return	backup_data->Copy(psrc_du);
}

/********************************************************************
機  能  名  称 : バックアップしている画像データを復活させる
関    数    名 : RecoverDataUnit
引          数 : long				image_number	(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::RecoverDataUnit(long number)
{
	//引数チェック
	if(number == CURRENT_IMAGE)	number = GetCurrentImageNumber();//現在選択中の画像を指定する場合
	else if(number<0 || number>=GetDataUnitNumber() )	return false;//src_numberが不正

	if( !backup_data )	return	false;//バックアップデータがない

	CDataUnit *pdst_du = FormatDataUnit(number, backup_data->DataType,backup_data->DataSize, backup_data->PageNumber,true);
		if(!pdst_du)	return	false;
		if( !pdst_du->Copy(backup_data))	return	false;

	DrawImage(number);

	return	true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//演算式で画像生成
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
機  能  名  称 : 演算式で画像生成
関    数    名 : CalcDataValue
引          数 : long				number			(in)結果格納する画像メモリ番号(-1にした場合は現在選択中の画像)
				 CString			com_str[]		(in)演算式
				 int				com_str_cnt		(in)演算式
				 bool				satulate		(in)上下限を超えた場合に飽和させる(falseにするとオーバーフローする)
				 CString			*pErr_str		(out)エラー
				 CProgressCtrl		*pProg			(in)プログレスバーへのポインタ(プログレスバーを使わない場合はNULL)
戻    り    値 : 成功したらtrue
機          能 : com_strで指定された演算式を解釈して、画素ごとに実行し、
				 結果をimage_numberで指定された画像メモリに入れる。
				 画像メモリが未取得ならばエラーになる
				 pProgを指定すると処理途中経過が表示される
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::CalcDataValue(long number, CString com_str, bool satulate, CString *pErr_str, CProgressCtrl *pProg)
{
	return CalcDataValue(number, -1, CSize(1,1),com_str,satulate,pErr_str,pProg);
}

/********************************************************************
機  能  名  称 : 演算式で画像生成
関    数    名 : CalcDataValue
引          数 : long				number			(in)結果格納する画像メモリ番号(-1にした場合は現在選択中の画像)
				 long				type			(in)画像メモリのタイプ
				 CSize				size			(in)画像メモリのサイズ
				 CString			com_str			(in)演算式
				 bool				satulate		(in)上下限を超えた場合に飽和させる(falseにするとオーバーフローする)
				 CString			*pErr_str		(out)エラー
				 CProgressCtrl		*pProg			(in)プログレスバーへのポインタ(プログレスバーを使わない場合はNULL)
戻    り    値 : 成功したらtrue
機          能 : com_strで指定された演算式を解釈して、画素ごとに実行し、
				 結果をimage_numberで指定された画像メモリに入れる。
				 メモリはtype,sizeで指定されたものが新しく取得される
				 pProgを指定すると処理途中経過が表示される
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::CalcDataValue(long number, long type, CSize size, CString com_str, bool satulate, CString *pErr_str, CProgressCtrl *pProg)
{
	int	i,j;
	CCalculate		calc;
	double			result;
	CDataUnit		*pdu=NULL;
	CString			err_str, com_strs[3];
	bool			ret=false;
	CRect			calc_area;
	char			strbuf[CALC_STRING_LEN_MAX];



	if(type<0){//現在あるメモリを使う
		if( !(pdu = GetDataUnit(number)) ){
			err_str = "このメモリには画像が存在しません";
			goto CalcDataValue_post_proc;	
		}
	}else{//新しくメモリを確保する
		if(!(pdu = FormatDataUnit(number, type, size, 1, true))){
			goto CalcDataValue_post_proc;	
		}
	}
	calc_area = pdu->GetRectArea();//計測領域


	//コマンド登録
	com_strs[0] = com_str;
	if( !calc.RegisterCommandString(com_strs, 1, &err_str) )	goto CalcDataValue_post_proc;

	//コマンドテスト実行
	if( !calc.Execute(0, 1,1, &err_str) )	goto CalcDataValue_post_proc;
	if( calc.GetResult(0,&result, strbuf, &err_str ) != 0 )	goto CalcDataValue_post_proc;


	if(pProg)	pProg->SetRange(0, (short)pdu->DataSize.cy-1);//プログレスバー準備

	//コマンドを画素ごとに実行
	for(j=calc_area.top ; j<=calc_area.bottom ; j++)
	{
		for(i=calc_area.left ; i<=calc_area.right ; i++)
		{
			//マスク判定
			if (pdu->DoUseMask) {
				if (pdu->GetMaskValue(CPoint(i, j)) == MASKED_PIXEL)		continue;
			}


			//ワークエリア判定
			if (pdu->DoUseWorkArea)
			{
				if( !pdu->WorkArea.PtInWorkArea(CPoint(i, j)))		continue;
			}



			if ((pdu->DataType == RGB_FORMAT || pdu->DataType == RGB_3D_FORMAT) && pdu->DispRGB)
			{//RGBそれぞれに対する処理
					for (int c = 0; c < 3; c++)
					{
						if (!calc.Execute(0, i, j, &err_str, c))		goto CalcDataValue_post_proc;
						if (calc.GetResult(0, &result, strbuf, &err_str) != 0)	goto CalcDataValue_post_proc;

						pdu->SetDataValue(i, j, result, -1, c);
					}
			}
			else//グレー値に対する処理
			{
					if (!calc.Execute(0, i, j, &err_str))			goto CalcDataValue_post_proc;
					if (calc.GetResult(0, &result, strbuf, &err_str) != 0)	goto CalcDataValue_post_proc;

					pdu->SetDataValue(i, j, result, -1, -1, satulate);
			}
		}
		if(pProg)	pProg->SetPos(j);//プログレスバー進める
	}
	if(pProg)	pProg->SetPos(0);//プログレスバー終了

	
	//DrawImage( number, true);//再描画
	ret = true;

CalcDataValue_post_proc:
	if(pErr_str){	*pErr_str = err_str;}
	return ret;
}

/********************************************************************
機  能  名  称 : 演算式で画像生成（マスク用）
関    数    名 : CalcMaskValue
引          数 : long				number			(in)結果格納する画像メモリ番号(-1にした場合は現在選択中の画像)
				 CString			com_str			(in)演算式
				 double				mask_max		(in)マスクOFFとする最大値
				 double				mask_min		(in)マスクOFFとする最小値
				 CString			*pErr_str		(out)エラー
				 CProgressCtrl		*pProg			(in)プログレスバーへのポインタ(プログレスバーを使わない場合はNULL)
戻    り    値 : 成功したらtrue
機          能 : com_strで指定された演算式を解釈して、画素ごとに実行し、
				 結果からimage_numberで指定された画像メモリのマスクを作る
				 画像メモリが未取得ならばエラーになる
				 pProgを指定すると処理途中経過が表示される
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::CalcMaskValue(long number, CString com_str, double mask_max, double mask_min, CString *pErr_str, CProgressCtrl *pProg)
{
	int	i,j;
	CCalculate		calc;
	double			result;
	CDataUnit		*pdu=NULL;
	CString			err_str, com_strs[3];;
	bool			ret=false;
	CRect			calc_area;
	char			strbuf[CALC_STRING_LEN_MAX];




		if( !(pdu = GetDataUnit(number)) ){
			err_str = "このメモリには画像が存在しません";
			goto CalcMaskValue_post_proc;	
		}
	calc_area = pdu->GetRectArea();//計測領域


	//コマンド登録
	com_strs[0] = com_str;
	if( !calc.RegisterCommandString(com_strs,1, &err_str) )	goto CalcMaskValue_post_proc;


	//コマンドテスト実行
	if( !calc.Execute(0, 1,1, &err_str) )	goto CalcMaskValue_post_proc;
	if( calc.GetResult(0,&result, strbuf, &err_str ) != 0 )	goto CalcMaskValue_post_proc;


	if(pProg)	pProg->SetRange(0, (short)pdu->DataSize.cy-1);//プログレスバー準備

	//コマンドを画素ごとに実行
	for(j=calc_area.top ; j<=calc_area.bottom ; j++)
	{
		for(i=calc_area.left ; i<=calc_area.right ; i++)
		{
			int val;
			if((pdu->DataType==RGB_FORMAT || pdu->DataType==RGB_3D_FORMAT) && pdu->DispRGB )
			{//RGBそれぞれに対する処理
				double color_reslt[3]={0};
				for(int c=0 ; c<3 ; c++)
				{
					if(!calc.Execute(0,i,j,&err_str,c))				goto CalcMaskValue_post_proc;
					if( calc.GetResult(0,&color_reslt[c], strbuf, &err_str ) != 0 )	goto CalcMaskValue_post_proc;
				}
				result = ( color_reslt[0]*rgb_gain[0] + color_reslt[1]*rgb_gain[1] + color_reslt[2]*rgb_gain[2] )/1000;
			}
			else//グレー値に対する処理
			{
				if(!calc.Execute(0, i,j,&err_str))			goto CalcMaskValue_post_proc;
				if( calc.GetResult(0, &result, strbuf, &err_str ) != 0 )	goto CalcMaskValue_post_proc;
			}

			if( mask_min <= result && mask_max>=result ){	val = UNMASKED_PIXEL;	}
			else										{	val = MASKED_PIXEL;		}
			pdu->SetMaskValue(i,j, val);
		}
		if(pProg)	pProg->SetPos(j);//プログレスバー進める
	}
	if(pProg)	pProg->SetPos(0);//プログレスバー終了

	
	//DrawImage( number, true);//再描画
	ret = true;

CalcMaskValue_post_proc:
	if(pErr_str){	*pErr_str = err_str;}
	return ret;
}