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
//画像メモリの型変更
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include <limits>

// *************************************
//         マ  ク  ロ   定   義         
// *************************************
#define MAX3(X,Y,Z)		((X)>(Y) ? (X):(Y)) > (Z) ?  ((X)>(Y) ? (X):(Y)) : (Z)
#define MIN3(X,Y,Z)		((X)<(Y) ? (X):(Y)) < (Z) ?  ((X)<(Y) ? (X):(Y)) : (Z)

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


#if 0 //これらの処理は CDataUnitのメンバ関数に移行
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//テンプレートで型なし(全ての型の画像に対応する)関数をつくる
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//表示画像をDATA型のバッファにコピー
template <class DATA>
void	disp_to_buffer( CDataUnit *p_du, DATA *p_buffer, int rgb_gain[])
{
	CSize size = p_du->DataSize;

	if( (p_du->DataType==RGB_FORMAT || p_du->DataType==RGB_3D_FORMAT) && p_du->DispRGB, int colorGrayType)//RGB表示
	{
		if (colorGrayType == 0)//明度
		{
			for(int j=0 ; j<size.cy ; j++)
			{
				for(int i=0 ; i<size.cx ; i++)
				{	
					int		intensity;
					BYTE	*p_disp_buf_rgb;

					p_disp_buf_rgb = p_du->pDispBufferRGB + (size.cy - 1 - j) * p_du->DispBufferRowRGB + i*3;
					intensity = ( *(p_disp_buf_rgb)   *rgb_gain[0] 
						+ *(p_disp_buf_rgb+1) *rgb_gain[1] 
						+ *(p_disp_buf_rgb+2) *rgb_gain[2] )/1000;
					*(p_buffer + i + j*size.cx ) = (BYTE)intensity;
				}
			}
		}		
		else if (colorGrayType == 1)
		{//彩度
			for(int j=0 ; j<size.cy ; j++)
			{
				for(int i=0 ; i<size.cx ; i++)
				{	
					int satulation;
					BYTE	*p_disp_buf_rgb;
					p_disp_buf_rgb = p_du->pDispBufferRGB + (size.cy - 1 - j) * p_du->DispBufferRowRGB + i*3;

					int max = MAX3( *(p_disp_buf_rgb), *(p_disp_buf_rgb+1) , *(p_disp_buf_rgb+2) );
					int min = MIN3( *(p_disp_buf_rgb), *(p_disp_buf_rgb+1) , *(p_disp_buf_rgb+2) );

					if (max == 0){
						*(p_buffer + i + j*size.cx ) = 0;
					}
					else{
						satulation = (max - min) * 1024 / max;
						*(p_buffer + i + j*size.cx ) = (BYTE)(brightness / 1024);
					}
				}
			}
		}
	}
	else
	{
		for(int j=0 ; j<size.cy ; j++)
		{
			for(int i=0 ; i<size.cx ; i++)
			{
				*(p_buffer + i + j*size.cx ) = *(p_du->pDispBuffer + i + (size.cy - j -1)*p_du->DispBufferRow );
			}
		}
	}
}

//データをDATA型のバッファにコピー
template <class DATA>
void	data_to_buffer( CDataUnit *p_du, DATA *p_data, DATA *p_buffer, int rgb_gain[], int colorGrayType)
{
	CSize size = p_du->DataSize;
	
	if((p_du->DataType==RGB_FORMAT || p_du->DataType==RGB_3D_FORMAT) && p_du->DispRGB)//RGB表示
	{
	
		if (colorGrayType == 0)//明度
		{
			int brightness;
			for (int i = 0; i < size.cx*size.cy; i++)
			{
				brightness = (int)*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i) * rgb_gain[0]
							+ (int)*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i + size.cx*size.cy) * rgb_gain[1]
							+ (int)*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i + size.cx*size.cy * 2) * rgb_gain[2];
				*(p_buffer + i) = (BYTE)(brightness / 1000.0);
			}
		}
		else if (colorGrayType == 1)
		{//彩度
			int satulation;
			for(int i = 0; i < size.cx*size.cy; i++)
			{
				int max = MAX3(*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i),
					*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i + size.cx*size.cy),
					*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i + size.cx*size.cy * 2));

				int min = MIN3(*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i),
					*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i + size.cx*size.cy),
					*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i + size.cx*size.cy * 2));

				if (max == 0){
					*(p_buffer + i) = 0;
				}
				else{
					satulation = (max - min) * 1024 / max;
					*(p_buffer + i) = (BYTE)(brightness / 1024);
				}

			}

		}
	}
	else
	{
		int memsize = size.cx * size.cy * sizeof(DATA);
		memcpy( p_buffer, p_data + (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel) * size.cx * size.cy , memsize);//現在のデータを別メモリに確保
	}
}

//データ間コピー
template <class DATA_SRC, class DATA_DST>
void	data_copy(DATA_SRC *p_src, DATA_DST *p_dst, CSize size, CONVERT_DATA_OPTION option)
{
	double	min,max;

	//DATA_DST型の最大値・最小値
	min = (double)(std::numeric_limits<DATA_DST>::min)();
	max = (double)(std::numeric_limits<DATA_DST>::max)();

	for(int i=0 ; i<size.cx*size.cy ; i++)
	{
		if(option.satulate)
		{
			if(min > (double)*p_src){			*p_dst = (DATA_DST)min;		}
			else if( max < (double)*p_src ){	*p_dst = (DATA_DST)max;		}
			else{								*p_dst = (DATA_DST)*p_src; 	}
		}
		else
		{
			*p_dst = (DATA_DST)*p_src; 
		}

		p_src++;
		p_dst++;
	}
}

//コンバート実行
template <class DATA>
bool	data_convert(CPimpomAPI *pAPI, int target_data_number, DATA *p_src , CONVERT_DATA_OPTION option, int rgb_gain[])
{
	int i,j;
	CDataUnit	*p_du;
	CSize		size;
	DATA		*p_buffer;
	BYTE		*p_dest_byte;
	float		*p_dest_float, *px, *py, *pz;
	short		*p_dest_short;
	WORD		*p_dest_word;
	long		*p_dest_long;
	DWORD		*p_dest_dword;
	BYTE		*p_mask_buffer;


	if( ( p_du = pAPI->GetDataUnit(target_data_number) ) == NULL )	return false;
	size = p_du->DataSize;
	

	//現在のデータを一旦格納するメモリを確保
	p_buffer = new DATA[ size.cx * size.cy * p_du->PageNumber * p_du->ChannelNumber];
	if(p_buffer==NULL)	return false;

	p_mask_buffer = new BYTE[size.cx * size.cy];
	if(p_mask_buffer==NULL){
		delete[]	p_buffer;
		return false;
	}

	memcpy(p_mask_buffer, p_du->pMaskData, size.cx * size.cy);//マスクデータをバッファへコピー

	//データを変換
		switch( option.type_to )
		{
			case BYTE_FORMAT:
					if(option.byte_from_disp)
					{
						disp_to_buffer(p_du, p_buffer, rgb_gain, option.color_gray_mathod);//disp->BYTE
					}
					else
					{
						data_to_buffer(p_du, p_src, p_buffer, rgb_gain, option.color_gray_mathod);//data->BYTE
					}
					p_dest_byte = pAPI->GetByteMemory( target_data_number, size, true);//BYTE型のメモリ取得
					data_copy(p_buffer, p_dest_byte, size, option);//バッファからデータを戻す
				break;

			case SHORT16_FORMAT://data->short
					data_to_buffer(p_du, p_src, p_buffer, rgb_gain, option.color_gray_mathod);//データをバッファに入れる
					p_dest_short = pAPI->GetShortMemory( target_data_number , size, true);//新しいメモリを取得
					data_copy(p_buffer, p_dest_short, size, option);//バッファからデータを戻す
				break;

			case WORD_FORMAT://data->WORD
					data_to_buffer(p_du, p_src, p_buffer, rgb_gain, option.color_gray_mathod);//データをバッファに入れる
					p_dest_word = pAPI->GetWordMemory( target_data_number , size, true);//新しいメモリを取得
					data_copy(p_buffer, p_dest_word, size, option);//バッファからデータを戻す
				break;

			case LONG32_FORMAT://data->LONG
					data_to_buffer(p_du, p_src, p_buffer, rgb_gain, option.color_gray_mathod);//データをバッファに入れる
					p_dest_long = pAPI->GetLongMemory( target_data_number , size, true);//新しいメモリを取得
					data_copy(p_buffer, p_dest_long, size, option);//バッファからデータを戻す
				break;

			case DWORD_FORMAT://data->DWORD
					data_to_buffer(p_du, p_src, p_buffer, rgb_gain, option.color_gray_mathod);//データをバッファに入れる
					p_dest_dword = pAPI->GetDwordMemory( target_data_number , size, true);//新しいメモリを取得
					data_copy(p_buffer, p_dest_dword, size, option);//バッファからデータを戻す
				break;

			case RGB_FORMAT://data->RGB
				if(p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)//RGB同士の変換
				{
					BYTE	*p_buffer_rgb = new BYTE[ size.cx * size.cy * p_du->ChannelNumber ];
					if(!p_buffer_rgb)	return false;

					memcpy(p_buffer_rgb, p_du->pByteData + p_du->DispPage * p_du->ChannelNumber * size.cx * size.cy , 
														size.cx * size.cy * p_du->ChannelNumber * sizeof(BYTE) );//現在のデータをバッファへ格納
					p_dest_byte = pAPI->GetRGBMemory( target_data_number , size, true);//新しいメモリを取得

					memcpy(p_dest_byte, p_buffer_rgb, size.cx * size.cy * p_du->ChannelNumber * sizeof(BYTE) );//バッファコピー

					delete[]	p_buffer_rgb;
				}
				else
				{
					data_to_buffer(p_du, p_src, p_buffer, rgb_gain);//データをバッファに入れる
					p_dest_byte = pAPI->GetRGBMemory( target_data_number , size, true);//新しいメモリを取得

					if( option.rgb_to == RGB_TO_B )//Bに入れる
					{
						data_copy(p_buffer, p_dest_byte + size.cx * size.cy * 2, size, option);
					}
					else if( option.rgb_to == RGB_TO_G )//Gに入れる
					{
						data_copy(p_buffer, p_dest_byte + size.cx * size.cy, size, option);
					}
					else if( option.rgb_to == RGB_TO_R )//Rに入れる
					{
						data_copy(p_buffer, p_dest_byte, size, option);
					}
					else//RGB全てに入れる
					{
						data_copy(p_buffer, p_dest_byte, size, option);
						data_copy(p_buffer, p_dest_byte + size.cx * size.cy, size, option);
						data_copy(p_buffer, p_dest_byte + size.cx * size.cy * 2, size, option);
					}
				}
				break;

			case FLOAT_FORMAT://data->FLOAT
					data_to_buffer(p_du, p_src, p_buffer, rgb_gain, option.color_gray_mathod);//データをバッファに入れる
					p_dest_float = pAPI->GetFloatMemory( target_data_number , size, true);//新しいメモリを取得

					//データ入れ替え
					for(j=0 ; j<size.cy ; j++)
					{
						for(i=0 ; i<size.cx ; i++)
						{
							px = p_dest_float + i + j*size.cx;
							*px = (float)*(p_buffer + i + j*size.cx);
						}
					}
				break;

			case XYZ_FORMAT://data->XYZ
					data_to_buffer(p_du, p_src, p_buffer, rgb_gain, option.color_gray_mathod);//データをバッファに入れる
					p_dest_float = pAPI->GetXYZMemory( target_data_number , size, true);//新しいメモリを取得

					//データ入れ替え
					for(j=0 ; j<size.cy ; j++)
					{
						for(i=0 ; i<size.cx ; i++)
						{
							px = p_dest_float + i + j*size.cx;
							py = px + size.cx * size.cy ;
							pz = px + size.cx * size.cy * 2;

							*px = (float)(i - size.cx/2);
							*py = (float)(-j + size.cy/2);
							*pz = (float)*(p_buffer + i + j*size.cx);
						}
					}
				break;

			case COMPLEX_FORMAT://data->COMP
				data_to_buffer(p_du, p_src, p_buffer, rgb_gain, option.color_gray_mathod);//データをバッファに入れる
				p_dest_float = pAPI->GetComplexMemory( target_data_number , size, true);//新しいメモリを取得

				if( option.comp_to == COMP_TO_REAL ){//実部に入れる
				}else{//虚部に入れる
					p_dest_float += size.cx * size.cy;
				}

				//データ入れ替え
				for(j=0 ; j<size.cy ; j++)
				{
					for(i=0 ; i<size.cx ; i++)
					{
						px = p_dest_float + i + j*size.cx;
						*px = (float)*(p_buffer + i + j*size.cx);
					}
				}
				p_du->CreateComplexPolarComponents();
				break;

			case THREE_D_FORMAT://data->3D
				for(int i=0 ; i<p_du->PageNumber ; i++)//データをバッファに入れる
				{
					p_du->DispPage = i;
					data_to_buffer(p_du, p_src, p_buffer + size.cx * size.cy * i, rgb_gain, option.color_gray_mathod);
				}
				p_dest_byte = pAPI->Get3DMemory( target_data_number , size, p_du->PageNumber, true);//新しいメモリを取得

				if( (p_du = pAPI->GetDataUnit(target_data_number)) != NULL)
				{
					for(int i=0 ; i<p_du->PageNumber ; i++)//バッファからデータを戻す
					{
						p_du->DispPage = i;
						data_copy(p_buffer + size.cx * size.cy * i, p_dest_byte + size.cx * size.cy * i, size, option);
					}
				}

		}

	memcpy(pAPI->GetMaskMemory(target_data_number), p_mask_buffer,  size.cx * size.cy);//マスクデータを戻す


	delete[]	p_buffer;
	delete[]	p_mask_buffer;


	

	return true;
}
#endif

/********************************************************************
機  能  名  称 : 画像データのタイプ変換
関    数    名 : ConvertDataUnit
引          数 : long					number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 int					type_to			(in)変換後の画像メモリの型
				 int					rgb_to			(in)RGB型に変換する場合の入力先 （1:R成分 2:G成分 3:B成分 0:RGB全成分）
				 int					comp_to			(in)複素数型の挿入先（0:実部　1:虚部）
				 bool					satulate		(in)型式の上下限を超える値を変換するとき飽和させるかどうか
				 int					byte_from	　　(in)表示メモリまたはマスクから変換するかどうか（type_toがBYTE型のときのみ有効）（0:データから 1:表示画像から 2:マスクから）
				bool					show_message	(in)変換後にメッセージを表示するか
戻    り    値 : 成功すれば true
機          能 : 変換結果は、同画像メモリに上書きされる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::ConvertDataUnit(long number, int type_to, int rgb_to, int comp_to, bool satulate, int byte_from, int color_gray_method, bool show_message)
{
	CONVERT_DATA_OPTION option = {0};
	option.type_to = type_to;
	option.rgb_to = rgb_to;
	option.comp_to = comp_to;
	option.satulate = satulate;
	option.byte_from = byte_from;
	option.color_gray_method = color_gray_method;

	return ConvertDataUnit(number,option,show_message );
}

/********************************************************************
機  能  名  称 : 画像データのタイプ変換
関    数    名 : ConvertDataUnit
引          数 : long					number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				CONVERT_DATA_OPTION		option			(in)変換オプション
				bool					show_message	(in)変換後にメッセージを表示するか
戻    り    値 : 成功すれば true
機          能 : 変換結果は、同画像メモリに上書きされる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::ConvertDataUnit(long number, CONVERT_DATA_OPTION option, bool show_message)
{
	CDataUnit	*p_du;

	if( (p_du = GetDataUnit(number) )==NULL)	return false;

	if(show_message){
		if( AfxMessageBox( "データタイプの変更後はもとのデータは消去されます" , MB_OKCANCEL | MB_ICONINFORMATION ) != IDOK ){
			return false;
		}
	}

	return p_du->Convert(option);
}



/********************************************************************
機  能  名  称 : 表示データをBYTE型にコンバート
関    数    名 : ConvertDispToByte
引          数 : long					number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
戻    り    値 : 成功すれば true
機          能 : 変換結果は、同画像メモリに上書きされる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::ConvertDispToByte(long number)
{
	CONVERT_DATA_OPTION option={0};
	option.type_to = BYTE_FORMAT;
	option.byte_from = 1;

	return ConvertDataUnit(number, option, false);
}


/********************************************************************
機  能  名  称 : マスクをBYTE型にコンバート
関    数    名 : ConvertMaskToByte
引          数 : long					number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
戻    り    値 : 成功すれば true
機          能 : 変換結果は、同画像メモリに上書きされる
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::ConvertMaskToByte(long number)
{
	CONVERT_DATA_OPTION option = { 0 };
	option.type_to = BYTE_FORMAT;
	option.byte_from = 2;

	return ConvertDataUnit(number, option, false);
}