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
//CDataUnitConvert
//画像データを統合的に扱うクラス 
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
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

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
extern	CPimpomAPI	API;

// *************************************
//         関    数    参    照         
// *************************************


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//テンプレートで型なし(全ての型の画像に対応する)関数をつくる
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//表示画像をDATA型にコピー
template <class DATA>
void	disp_to_data(CDataUnit *p_du, DATA *p_buffer, int color_gray_method)
{
	CSize size = p_du->DataSize;

	if( (p_du->DataType==RGB_FORMAT || p_du->DataType==RGB_3D_FORMAT) && p_du->DispRGB)//RGB表示
	{
		if (color_gray_method == 1)//彩度
		{
			int satulation;
			BYTE	*p_disp_buf_rgb;

			for (int j = 0; j<size.cy; j++)
			{
				for (int i = 0; i<size.cx; i++)
				{
					p_disp_buf_rgb = p_du->pDispBufferRGB + (size.cy - 1 - j) * p_du->DispBufferRowRGB + i * 3;

					int max = MAX3(*(p_disp_buf_rgb), *(p_disp_buf_rgb + 1), *(p_disp_buf_rgb + 2));
					int min = MIN3(*(p_disp_buf_rgb), *(p_disp_buf_rgb + 1), *(p_disp_buf_rgb + 2));

					if (max == 0){
						*(p_buffer + i + j*size.cx) = 0;
					}
					else{
						satulation = (max - min) * 255 * 1024 / max;
						*(p_buffer + i + j*size.cx) = (BYTE)(satulation / 1024);
					}
				}
			}
		}
		else if (color_gray_method == 2)//色相
		{

		}
		else//明度
		{
			BYTE	*p_disp_buf_rgb;
			int		intensity;
			for (int j = 0; j < size.cy; j++)
			{
				for (int i = 0; i < size.cx; i++)
				{
					p_disp_buf_rgb = p_du->pDispBufferRGB + (size.cy - 1 - j) * p_du->DispBufferRowRGB + i * 3;

					intensity = (int)*(p_disp_buf_rgb)*API.rgb_gain[2]	//b
						+ (int)*(p_disp_buf_rgb + 1) *API.rgb_gain[1]	//g
						+ (int)*(p_disp_buf_rgb + 2) *API.rgb_gain[0];	//r

					*(p_buffer + i + j*size.cx) = (BYTE)( intensity / 1000);
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


//マスク画像をDATA型にコピー
template <class DATA>
void	mask_to_data(CDataUnit *p_du, DATA *p_buffer)
{
	CSize size = p_du->DataSize;

	for (int j = 0; j<size.cy; j++)
	{
		for (int i = 0; i<size.cx; i++)
		{
			*(p_buffer + i + j*size.cx) = *(p_du->pMaskData + i + size.cx*j)*255;
			*(p_du->pMaskData + i + size.cx*j) = UNMASKED_PIXEL;
		}
	}
}


//DATA_SRC型データをDATA_DST型データにコピー
template <class DATA_SRC, class DATA_DST>
void	data_to_data(	CDataUnit	*p_src_du,		//(in)コピー元のCDataUnit
						DATA_SRC	*p_src_data,	//(in)p_src_du内部の画像データ先頭ポインタ
						DATA_DST	*p_dst_data,	//(out)コピー先画像データ先頭ポインタ
						double		dst_data_min,	//(in)p_dst_dataの下限値
						double		dst_data_max,	//(in)p_dst_dataの上限値
						int			color_gray_method	//(in)RGB->グレー変換方法 (1:彩度 2:色相  これら以外:明度)
						)
{
	CSize size = p_src_du->DataSize;
	
	if((p_src_du->DataType==RGB_FORMAT || p_src_du->DataType==RGB_3D_FORMAT) && p_src_du->DispRGB)//RGB表示
	{
		if (color_gray_method == 1)//彩度
		{
			int satulation;
			BYTE *pr = p_src_du->pByteData + (p_src_du->DispPage * p_src_du->ChannelNumber) * size.cx * size.cy;
			BYTE *pg = p_src_du->pByteData + (p_src_du->DispPage * p_src_du->ChannelNumber) * size.cx * size.cy + size.cx*size.cy;
			BYTE *pb = p_src_du->pByteData + (p_src_du->DispPage * p_src_du->ChannelNumber) * size.cx * size.cy + size.cx*size.cy * 2;
			DATA_DST *pdst = p_dst_data;

			for (int i = 0; i < size.cx*size.cy; i++)
			{
				int max = MAX3(*pr, *pg, *pb);
				int min = MIN3(*pr, *pg, *pb);

				if (max == 0){
					*pdst = 0;
				}
				else{
					satulation = (max - min) * 255 * 1024 / max;
					*pdst = (DATA_DST)(satulation / 1024);
				}

				pr++;
				pg++;
				pb++;
				pdst++;
			}
		}
		else if (color_gray_method == 2)//色相
		{

		}
		else//明度
		{
			int intensity;
			BYTE *pr = p_src_du->pByteData + (p_src_du->DispPage * p_src_du->ChannelNumber) * size.cx * size.cy;
			BYTE *pg = p_src_du->pByteData + (p_src_du->DispPage * p_src_du->ChannelNumber) * size.cx * size.cy + size.cx*size.cy;
			BYTE *pb = p_src_du->pByteData + (p_src_du->DispPage * p_src_du->ChannelNumber) * size.cx * size.cy + size.cx*size.cy * 2;
			DATA_DST *pdst = p_dst_data;

			for (int i = 0; i < size.cx*size.cy; i++)
			{
				intensity = (int)*pr * API.rgb_gain[0]	//r
					+ (int)*pg * API.rgb_gain[1]		//g
					+ (int)*pb * API.rgb_gain[2];		//b
				*pdst = (DATA_DST)(intensity / 1000.0);

				pr++;
				pg++;
				pb++;
				pdst++;
			}
		}

	}
	else
	{
		DATA_SRC *psrc = p_src_data +  (p_src_du->DispPage * p_src_du->ChannelNumber + p_src_du->DispChannel) * size.cx * size.cy;
		DATA_DST *pdst = p_dst_data;
		double tmp;
		

		if(dst_data_min<dst_data_max)//上下限で飽和させる
		{
			for(int i=0 ; i<size.cx*size.cy  ; i++)
			{
				tmp = *psrc;
				if(tmp<dst_data_min)		tmp = dst_data_min;
				if(tmp>dst_data_max)		tmp = dst_data_max;

				*pdst = (DATA_DST)tmp;
		
				psrc++;
				pdst++;
			}
		}else{//そのままつかう
			for(int i=0 ; i<size.cx*size.cy  ; i++)
			{
				*pdst = (DATA_DST)*psrc;
		
				psrc++;
				pdst++;
			}
		}
	}
}



//コンバート実行
template <class DATA>
bool	data_convert(CDataUnit				*pSrcDU,	//(in)変換前のCDataUnitへのポインタ
					 DATA					*pSrcData,	//(in)pSrcDU内部の画像データ先頭ポインタ（型をくべつするため）
					 CDataUnit				*pDstDU,	//(out)変換後のCDataUnitへのポインタ
					 CONVERT_DATA_OPTION	option		//(in)変換オプション
					 )
{
	CSize		size;
	double		dst_data_min=0, dst_data_max=0;

	if(option.satulate)//データ上下限超えたとき飽和させるかどうか
	{
		dst_data_min = pDstDU->GetMinLimitValue();
		dst_data_max = pDstDU->GetMaxLimitValue();
	}

	size = pSrcDU->DataSize;

	//データを変換
		switch( option.type_to )
		{
			case BYTE_FORMAT:
				if(option.byte_from==1)
				{
					disp_to_data(pSrcDU, pDstDU->pByteData, option.color_gray_method);//disp->BYTE
				}
				else if (option.byte_from==2)
				{
					mask_to_data(pSrcDU, pDstDU->pByteData);//mask->BYTE
				}
				else{
					data_to_data(pSrcDU, pSrcData, pDstDU->pByteData, dst_data_min, dst_data_max, option.color_gray_method);//data->BYTE
				}
				break;

			case SHORT16_FORMAT://data->short
				data_to_data(pSrcDU, pSrcData, pDstDU->pShortData, dst_data_min, dst_data_max, option.color_gray_method);
				break;

			case WORD_FORMAT://data->WORD
				data_to_data(pSrcDU, pSrcData, pDstDU->pWordData, dst_data_min, dst_data_max, option.color_gray_method);
				break;

			case LONG32_FORMAT://data->LONG
				data_to_data(pSrcDU, pSrcData, pDstDU->pLongData, dst_data_min, dst_data_max, option.color_gray_method);
				break;

			case DWORD_FORMAT://data->DWORD
				data_to_data(pSrcDU, pSrcData, pDstDU->pDwordData, dst_data_min, dst_data_max, option.color_gray_method);
				break;

			case RGB_FORMAT://data->RGB
				if(pSrcDU->DataType == RGB_FORMAT || pSrcDU->DataType == RGB_3D_FORMAT)//RGB同士の変換
				{
					//そのままの値をコピー
					memcpy(pDstDU->pByteData, pSrcDU->pByteData + pSrcDU->DispPage * pSrcDU->ChannelNumber * size.cx * size.cy , 
														size.cx * size.cy * pSrcDU->ChannelNumber * sizeof(BYTE) );
				}
				else
				{
					if( option.rgb_to == RGB_TO_B )//Bに入れる
					{
						data_to_data(pSrcDU, pSrcData, pDstDU->pByteData + size.cx * size.cy * 2, dst_data_min, dst_data_max, 0);
					}
					else if( option.rgb_to == RGB_TO_G )//Gに入れる
					{
						data_to_data(pSrcDU, pSrcData, pDstDU->pByteData + size.cx * size.cy * 1, dst_data_min, dst_data_max, 0);
					}
					else if( option.rgb_to == RGB_TO_R )//Rに入れる
					{
						data_to_data(pSrcDU, pSrcData, pDstDU->pByteData + size.cx * size.cy * 0, dst_data_min, dst_data_max, 0);
					}
					else//RGB全てに入れる
					{
						data_to_data(pSrcDU, pSrcData, pDstDU->pByteData + size.cx * size.cy * 2, dst_data_min, dst_data_max, 0);
						data_to_data(pSrcDU, pSrcData, pDstDU->pByteData + size.cx * size.cy * 1, dst_data_min, dst_data_max, 0);
						data_to_data(pSrcDU, pSrcData, pDstDU->pByteData + size.cx * size.cy * 0, dst_data_min, dst_data_max, 0);
					}
				}
				break;

			case FLOAT_FORMAT://data->FLOAT
				data_to_data(pSrcDU, pSrcData, pDstDU->pFloatData, 0, 0, option.color_gray_method);//データをバッファに入れる
				break;

			case XYZ_FORMAT://data->XYZ
				{
					data_to_data(pSrcDU, pSrcData, pDstDU->pFloatData + size.cx * size.cy * 2, 0, 0, option.color_gray_method);//zに入れる

					//データx,yデータつくる
					float		*px, *py ;
					for(int j=0 ; j<size.cy ; j++)
					{
						for(int i=0 ; i<size.cx ; i++)
						{
							px = pDstDU->pFloatData + i + j*size.cx;
							py = px + size.cx * size.cy ;
							//pz = px + size.cx * size.cy * 2;

							*px = (float)(i - size.cx/2);
							*py = (float)(-j + size.cy/2);
							//*pz = (float)*(p_buffer + i + j*size.cx);
						}
					}
				}
				break;

			case COMPLEX_FORMAT://data->COMPLLEX
				if( option.comp_to == COMP_TO_REAL )
				{//実部に入れる
					data_to_data(pSrcDU, pSrcData, pDstDU->pFloatData, 0, 0, option.color_gray_method);
				}
				else
				{//虚部に入れる
					data_to_data(pSrcDU, pSrcData, pDstDU->pFloatData + size.cx * size.cy, 0, 0, option.color_gray_method);
				}
				pDstDU->CreateComplexPolarComponents();
				break;

			case THREE_D_FORMAT://data->3D
				for (int i = 0, j = option.threeD_offset; i<pSrcDU->PageNumber && j<pDstDU->PageNumber; i++, j++)
				{
					if (j < 0)	continue;

					pSrcDU->DispPage = i;
					data_to_data(pSrcDU, pSrcData, pDstDU->pByteData + size.cx * size.cy  * j, dst_data_min, dst_data_max, option.color_gray_method);
				}
				break;

			case FLOAT_3D_FORMAT://data->F3D
				for (int i = 0, j = option.threeD_offset ; i<pSrcDU->PageNumber && j<pDstDU->PageNumber; i++, j++)
				{
					if (j < 0)	continue;

					pSrcDU->DispPage = i;
					data_to_data(pSrcDU, pSrcData, pDstDU->pFloatData + size.cx * size.cy * j, dst_data_min, dst_data_max, option.color_gray_method);
				}
				break;
		}


	return true;
}



/********************************************************************
機  能  名  称 : データの型を変換する
関    数    名 : Convert
引          数 : CONVERT_DATA_OPTION option  (in)
戻    り    値 : 成功したらtrueを返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataUnit::Convert(CONVERT_DATA_OPTION option)
{
	//同データ同士の変換の場合は処理をスキップ
	//if( DataType==option.type_to)
	//{
	//	if( option.type_to != BYTE_FORMAT  ||  option.byte_from_disp==false ){//表示データから変換する場合のみ有効にする
	//		return true;
	//	}
	//}


	//
	CDataUnit *pSrcDU = new CDataUnit(DataType, DataSize, PageNumber);
	if(!pSrcDU)	return false;

	if(!pSrcDU->Copy(this)){
		delete pSrcDU;
		return false;
	}


	Destroy();

	int page = pSrcDU->PageNumber;
	if (option.type_to == THREE_D_FORMAT || option.type_to == FLOAT_3D_FORMAT || option.type_to == RGB_3D_FORMAT)
	{
		if (option.threeD_page > 0 && option.threeD_page<pSrcDU->PageNumber)
		{
			page = option.threeD_page;
		}
	}
	if (!Format(option.type_to, pSrcDU->DataSize, page)){
		delete pSrcDU;
		return false;
	}


	
	//画像データコピー

	bool ret = true;
	switch(pSrcDU->DataType)
	{
		case BYTE_FORMAT://BYTEから変換
			ret = data_convert(pSrcDU, pSrcDU->pByteData, this, option);
			break;

		case THREE_D_FORMAT://3Dから変換
			ret = data_convert(pSrcDU, pSrcDU->pByteData, this, option);
			break;

		case RGB_FORMAT://RGBから変換
			ret = data_convert(pSrcDU, pSrcDU->pByteData, this, option);
			break;

		case RGB_3D_FORMAT://RGB3Dから変換
			ret = data_convert(pSrcDU, pSrcDU->pByteData, this, option);
			break;

		case SHORT16_FORMAT://SHORTから変換
			ret = data_convert(pSrcDU, pSrcDU->pShortData, this, option);
			break;

		case WORD_FORMAT://WORDから変換
			ret = data_convert(pSrcDU, pSrcDU->pWordData, this, option);
			break;

		case LONG32_FORMAT://LONGから変換
			ret = data_convert(pSrcDU, pSrcDU->pLongData, this, option);
			break;

		case DWORD_FORMAT://DWORDから変換
			ret = data_convert(pSrcDU, pSrcDU->pDwordData, this, option);
			break;

		case FLOAT_FORMAT://FLOATから変換
			ret = data_convert(pSrcDU, pSrcDU->pFloatData, this, option);
			break;

		case COMPLEX_FORMAT://複素数から変換
			ret = data_convert(pSrcDU, pSrcDU->pFloatData, this, option);
			break;

		case XYZ_FORMAT://XYZから変換
			ret = data_convert(pSrcDU, pSrcDU->pFloatData, this, option);
			break;

		case XY_FORMAT://XYから変換
			ret = data_convert(pSrcDU, pSrcDU->pFloatData, this, option);
			break;

		case FLOAT_3D_FORMAT://F3Dから変換
			ret = data_convert(pSrcDU, pSrcDU->pFloatData, this, option);
			break;

		default:
			delete pSrcDU;
			return false;
	}

	if(!ret){
		delete pSrcDU;
		return false;
	}


	//その他データコピー
	
	DataName = pSrcDU->DataName;//データ名
	SrcFilePath = pSrcDU->SrcFilePath;//読み込んだファイルパス
	WorkArea.Copy(&pSrcDU->WorkArea);//作業領域
	DataDispOffset = pSrcDU->DataDispOffset;//表示位置
	DispScale = pSrcDU->DispScale;//表示倍率

	for(int i=0 ; i< pSrcDU->MarkNum ; i++){//マーク座標の復帰
		CMark mark;
		pSrcDU->GetMark(i, &mark);
		AddMark( mark);
	}

	delete pSrcDU;

	return true;
}