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
//CDataUnit
//画像データを統合的に扱うクラス
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


CMark::CMark()
{
}


CMark::CMark(float x, float y)
{
	type = MARK_TYPE_POINT;
	figure.point.x = x;
	figure.point.y = y;

	memset(attrib, 0, sizeof(attrib));
}

CMark::CMark(float x1, float y1, float x2, float y2)
{
	type = MARK_TYPE_LINE;
	figure.line.x1 = x1;
	figure.line.y1 = y1;
	figure.line.x2 = x2;
	figure.line.y2 = y2;

	memset(attrib, 0, sizeof(attrib));
}

CMark::~CMark()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//テンプレートで型なし(全ての型の画像に対応する)関数をつくる
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//データ→表示濃度を計算
template <class DATA>
void	calc_bright( CDataUnit *p_du , DATA *p_src_data)
{
	int				i, j;
	DATA			*p_data;
	BYTE			*p_mask, *p_disp_buf,*p_disp_buf_rgb;
	double			bri_tilt, bri_offset, temp_bri, temp_data;
	CSize			data_size;

	data_size = p_du->DataSize;//画像データのサイズ

	if (p_du->BrightMax - p_du->BrightMin > 0)
	{	
		bri_tilt = 256.0 / (p_du->BrightMax - p_du->BrightMin);//データ　→　表示濃度　の変換割合
		bri_offset = p_du->BrightMin;//表示最低濃度
	}
	else{
		bri_tilt = 0;
		bri_offset = 0;
	}
	



	if( (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT ) || p_du->DispRGB == false  )//グレー表示
	{
		p_data = p_src_data + data_size.cx * data_size.cy * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel ) ;//データを表すポインタの初期化
		p_mask = p_du->pMaskData;//マスクを表すポインタを初期化

		for( j = 0   ;   j < data_size.cy   ;   j ++)
		{
			p_disp_buf = p_du->pDispBuffer + (data_size.cy - 1 - j) * p_du->DispBufferRow;//表示画像バッファを表すポインタの設定

			for( i = 0   ;   i < data_size.cx   ;   i++ )
			{
				if( p_du->DoUseMask  &&  (*p_mask == MASKED_PIXEL) )//マスクをかける場合の処理
				{
					*p_disp_buf = 0;//マスクがかかっている画素は黒にする
				}
				else
				{//マスクをかけない場合の処理
					temp_data = (double)(*p_data);

					//画像データから表示画像の濃度を計算する	
					temp_bri = bri_tilt * ( temp_data  -  bri_offset );
					if( temp_bri < 0.0)				temp_bri = 0.0;
					if( temp_bri > 255.0 )			temp_bri = 255.0;

					*p_disp_buf = (BYTE)temp_bri;
				}

			//各データのポインタを次の画素にずらす
			p_data++;
			p_disp_buf++;
			p_mask++;
			}
		}
	}
	else//RGB表示
	{
		p_data = p_src_data + data_size.cx * data_size.cy * (p_du->DispPage * p_du->ChannelNumber );//データを表すポインタの初期化
		p_mask = p_du->pMaskData;//マスクを表すポインタを初期化

		for( j = 0   ;   j < data_size.cy   ;   j ++)
		{
			p_disp_buf_rgb = p_du->pDispBufferRGB + (data_size.cy - 1 - j) * p_du->DispBufferRowRGB;//表示画像バッファを表すポインタの設定

			for( i = 0   ;   i < data_size.cx   ;   i++ )
			{
				if( p_du->DoUseMask  &&  (*p_mask == MASKED_PIXEL) )//マスクをかける場合の処理
				{
					//マスクがかかっている画素は黒にする
					*p_disp_buf_rgb = 0;//r
					*(p_disp_buf_rgb+1) = 0;//g
					*(p_disp_buf_rgb+2) = 0;//b
				}
				else//マスクをかけない場合の処理
				{
					//r
					temp_data = (double)(*p_data);	
					temp_bri = bri_tilt * ( temp_data  -  bri_offset );
					if( temp_bri < 0.0)				temp_bri = 0.0;
					if( temp_bri > 255.0 )			temp_bri = 255.0;

					*(p_disp_buf_rgb + 2) = (BYTE)temp_bri;

					//g
					temp_data = (double)*(p_data + data_size.cx * data_size.cy);
					temp_bri = bri_tilt * ( temp_data  -  bri_offset );
					if( temp_bri < 0.0)				temp_bri = 0.0;
					if( temp_bri > 255.0 )			temp_bri = 255.0;

					*(p_disp_buf_rgb + 1) = (BYTE)temp_bri;
			
					//b
					temp_data = (double)*(p_data + data_size.cx * data_size.cy*2);
					temp_bri = bri_tilt * ( temp_data  -  bri_offset );
					if( temp_bri < 0.0)				temp_bri = 0.0;
					if( temp_bri > 255.0 )			temp_bri = 255.0;

					*(p_disp_buf_rgb + 0) = (BYTE)temp_bri;
				}

			//各データのポインタを次の画素にずらす
			p_data++;
			p_disp_buf_rgb+=3;
			p_mask++;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//作業領域の中の平均と標準偏差を計算し，ヒストグラムをつくる
template <class DATA>
void	calc_hist_and_stat( CDataUnit *p_du , 
							DATA *p_src_data , 
							CRect work_area, 
							int rgb_gain[]
			)
{
	int				i, j, pix_counter, data_addr_offset, disp_buf_addr_offset , data_addr_page_offset;
	DATA			*p_data;
	BYTE			*p_mask, *p_disp_buf, *p_disp_buf_rgb;
	double			sum, sum_2, temp_data;
	int				intensity;
	CSize			data_size;


	data_size = p_du->DataSize;//画像データのサイズ
	

	//各変数の初期化
	sum = 0;
	sum_2 = 0;
	ZeroMemory( p_du->BrightLevel, sizeof(long)*256 );
	pix_counter = 0;
	
	if( (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) || p_du->DispRGB == false  )//グレー表示
	{
		data_addr_page_offset = data_size.cx * data_size.cy * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel );//表示ページ番号によるアドレスのオフセット値

		for( j= work_area.top   ;   j<=work_area.bottom   ;   j++)
		{
			//画素のアドレスの計算
			data_addr_offset = j * data_size.cx  +  work_area.left;//画素によるデータのアドレスのオフセット値の決定
			disp_buf_addr_offset = (data_size.cy - 1 - j) * p_du->DispBufferRow + work_area.left;//画素による表示画像バッファのオフセット値の決定

			p_data = p_src_data + data_addr_offset + data_addr_page_offset;//データ
			p_disp_buf = p_du->pDispBuffer + disp_buf_addr_offset;//表示画像バッファ
			p_mask = p_du->pMaskData + data_addr_offset;//マスク

			for( i=work_area.left   ;   i<=work_area.right   ;   i++ )
			{
				if( !p_du->DoUseMask  ||  (*p_mask == UNMASKED_PIXEL) )//マスクの処理
				{
					if( !p_du->DoUseWorkArea || p_du->WorkArea.PtInWorkArea(CPoint(i,j)) )//作業領域内の処理
					{
						temp_data = (double)(*p_data);

						//表示画像の濃度分布の度数分布を作る
						p_du->BrightLevel[ *p_disp_buf ]++;

						//平均と標準偏差を得るための計算（和）をする
						sum += temp_data;
						sum_2 += temp_data * temp_data;
						
						//計算に使う画素の数を数える
						pix_counter++;
					}
				}
			p_data++;
			p_disp_buf++;
			p_mask++;
			}
		}
	}
	else//RGB表示
	{
		data_addr_page_offset = data_size.cx * data_size.cy * (p_du->DispPage * p_du->ChannelNumber);//表示ページ番号によるアドレスのオフセット値

		for( j= work_area.top   ;   j<=work_area.bottom   ;   j++)
		{
			//画素のアドレスの計算
			data_addr_offset = j * data_size.cx  +  work_area.left;//画素によるデータのアドレスのオフセット値の決定
			disp_buf_addr_offset = (data_size.cy - 1 - j) * p_du->DispBufferRowRGB + work_area.left*3;//画素による表示画像バッファのオフセット値の決定

			p_data = p_src_data + data_addr_offset + data_addr_page_offset;//データ
			p_disp_buf_rgb = p_du->pDispBufferRGB + disp_buf_addr_offset;//表示画像バッファ
			p_mask = p_du->pMaskData + data_addr_offset;//マスク

			for( i=work_area.left   ;   i<=work_area.right   ;   i++ )
			{
				if( !p_du->DoUseMask  ||  (*p_mask == UNMASKED_PIXEL) )//マスクをかけない場合の処理
				{
					intensity = ( ((int)*p_data ) * rgb_gain[0] 
							    + ((int)*(p_data + data_size.cx * data_size.cy )) * rgb_gain[1]
							    + ((int)*(p_data + data_size.cx * data_size.cy*2 )) * rgb_gain[2]  )/1000;
					temp_data = (double)intensity;

					//表示画像の濃度分布の度数分布を作る
					intensity = ( ((int)*(p_disp_buf_rgb))    * rgb_gain[0] 
								+ ((int)*(p_disp_buf_rgb +1)) * rgb_gain[1]
								+ ((int)*(p_disp_buf_rgb +2)) * rgb_gain[2]  )/1000;
					p_du->BrightLevel[ intensity ]++;

					//平均と標準偏差を得るための計算（和）をする
					sum += temp_data;
					sum_2 += temp_data * temp_data;
					
					//計算に使う画素の数を数える
					pix_counter++;
				}
			p_data++;
			p_disp_buf_rgb+=3;
			p_mask++;
			}
		}
	}

	p_du->Average = (float)(sum / (double)pix_counter);//平均
	p_du->Stdiv = (float)sqrt( fabs( p_du->Average * p_du->Average
										- sum_2 / (double)pix_counter ) );//標準偏差
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//作業領域の中の最大値を計算する
template <class DATA>
void	calc_min_max( CDataUnit *p_du , 
					 DATA *p_src_data , 
					 CRect work_area, 
					 float *p_min, 
					 float *p_max, 
					 int rgb_gain[])
{
	int				i, j, data_addr_offset , data_addr_page_offset;
	DATA			*p_data;
	BYTE			*p_mask;
	double			temp_max, temp_min;
	int				intensity;
	CSize			data_size;


	data_size = p_du->DataSize;//画像データのサイズ
	
	if( (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) || p_du->DispRGB == false  )//グレー表示
	{
		data_addr_page_offset = data_size.cx * data_size.cy * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel );//表示ページ番号によるアドレスのオフセット値

		//変数の初期化
		temp_max = (double)*(p_src_data + data_addr_page_offset);
		temp_min = (double)*(p_src_data + data_addr_page_offset);

		for( j= work_area.top   ;   j<=work_area.bottom   ;   j++)
		{
			//画素のアドレスの計算
			data_addr_offset = j * data_size.cx  +  work_area.left;//画素によるデータのアドレスのオフセット値の決定

			p_data = p_src_data + data_addr_offset + data_addr_page_offset;//データ
			p_mask = p_du->pMaskData + data_addr_offset;//マスク

			for( i=work_area.left   ;   i<=work_area.right   ;   i++ )
			{
					if( !p_du->DoUseMask  ||  (*p_mask == UNMASKED_PIXEL) )//マスクをかけない場合
					{
						if( temp_min > (double)(*p_data) )		temp_min = (double)(*p_data);//最小値
						if( temp_max < (double)(*p_data) )		temp_max = (double)(*p_data);//最大値
					}
				p_data++;
				p_mask++;
			}
		}
	}
	else//RGB表示
	{
		data_addr_page_offset = data_size.cx * data_size.cy * (p_du->DispPage * p_du->ChannelNumber );//表示ページ番号によるアドレスのオフセット値

		//変数の初期化
		intensity = ( ((int)*(p_src_data + data_addr_page_offset)) * rgb_gain[0]
					 +((int)*(p_src_data + data_addr_page_offset  + data_size.cx * data_size.cy)) * rgb_gain[1]
					 +((int)*(p_src_data + data_addr_page_offset  + data_size.cx * data_size.cy*2)) * rgb_gain[2] )/1000;
		temp_max = (double)intensity;
		temp_min = (double)intensity;

		for( j= work_area.top   ;   j<=work_area.bottom   ;   j++)
		{
			//画素のアドレスの計算
			data_addr_offset = j * data_size.cx  +  work_area.left;//画素によるデータのアドレスのオフセット値の決定

			p_data = p_src_data + data_addr_offset + data_addr_page_offset;//データ
			p_mask = p_du->pMaskData + data_addr_offset;//マスク

			for( i=work_area.left   ;   i<=work_area.right   ;   i++ )
			{
					if( !p_du->DoUseMask  ||  (*p_mask == UNMASKED_PIXEL) )//マスクをかけない場合
					{
						intensity = ( ((int)*p_data) * rgb_gain[0]
									 +((int)*(p_data  + data_size.cx * data_size.cy)) * rgb_gain[1]
									 +((int)*(p_data  + data_size.cx * data_size.cy*2)) * rgb_gain[2] )/1000;

						if( temp_min > (double)intensity )		temp_min = (double)intensity;//最小値
						if( temp_max < (double)intensity )		temp_max = (double)intensity;//最大値
					}
				p_data++;
				p_mask++;
			}
		}
	}

	*p_min = (float)temp_min;
	*p_max = (float)temp_max;
}


//サブピクセル値を計算
template <class DATA>
double	calc_sub_pix_internal( CDataUnit *p_du , 
								DATA *p_src_data, 
								double x, 
								double y
								)
{
	double	w0, w1;
	double	v1,v2,v3,v4;

	w0 = x - (int)x;
	w1 = y - (int)y;

	if(w0==0.0 && w1==0.0)
	{
		v1 = (double)*(p_src_data);
		v2 = 0;	v3=0;	v4=0;
	}
	else
	{
		v1 = (double)*(p_src_data                        ) * (1.0 - w0)*(1.0 - w1);
		v2 = (double)*(p_src_data                     + 1) * (      w0)*(1.0 - w1);
		v3 = (double)*(p_src_data + p_du->DataSize.cx    ) * (1.0 - w0)*(      w1);
		v4 = (double)*(p_src_data + p_du->DataSize.cx + 1) * (      w0)*(      w1);
	}

	return v1+v2+v3+v4;
}


//サブピクセル値を計算
template <class DATA>
double	calc_sub_pix( CDataUnit *p_du , 
					  DATA		*p_src_data, 
					  double x, 
					  double y, 
					  long page, 
					  int channel, 
					  int rgb_gain[]
						)
{
	int		addr;
	int imsize = p_du->DataSize.cx * p_du->DataSize.cy;//画像のサイズ（画素数）


	if(page<0)	page=p_du->DispPage;
	if(channel<0)
	{
		if( (p_du->DataType==RGB_FORMAT || p_du->DataType==RGB_3D_FORMAT) && p_du->DispRGB)//RGB表示の場合は明度
		{
			addr = (int)x + ((int)y)*p_du->DataSize.cx + imsize * (page * p_du->ChannelNumber );
			return  (  calc_sub_pix_internal(p_du, p_src_data + addr, x, y) * rgb_gain[0]
					 + calc_sub_pix_internal(p_du, p_src_data + addr + imsize, x, y) * rgb_gain[1]
					 + calc_sub_pix_internal(p_du, p_src_data + addr + imsize*2, x, y)  * rgb_gain[2] ) /1000;
		}

		channel=p_du->DispChannel;
	}

	addr = (int)x + ((int)y)*p_du->DataSize.cx + imsize * (page * p_du->ChannelNumber + channel);
	return calc_sub_pix_internal(p_du, p_src_data + addr, x, y);

}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//標準のコンストラクタ
CDataUnit::CDataUnit()
{

}


/********************************************************************
機  能  名  称 : コンストラクタ
関    数    名 : CDataUnit
引          数 : int			type		(in)画像メモリのタイプ
				 CSize			size		(in)画像サイズ
				 long			page		(in)画像の奥行き
戻    り    値 : 
機          能 : CDataUnitクラスの構築
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
CDataUnit::CDataUnit(long type, CSize size, long page)
{
	Format(type,size,page);
}

/********************************************************************
機  能  名  称 : 内部データ初期化
関    数    名 : Format
引          数 : int			type		(in)画像メモリのタイプ
				 CSize			size		(in)画像サイズ
				 long			page		(in)画像の奥行き
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataUnit::Format(long type, CSize size, long page)
{
	//引数チェック
	if(size.cx<=0 || size.cy<=0)	return false;
	if(page<=0)		return false;


	//データに関する変数を初期化する
	complex_image_power_log = false;
	DataType = type;
	DataSize = size;
	DataName.Empty();
	SrcFilePath.Empty();
	DispPage = 0;
	DispChannel = 0;
	DispRGB = true;
	pByteData	= NULL;
	pFloatData	= NULL;
	pShortData	= NULL;
	pWordData	= NULL;
	pLongData	= NULL;
	pDwordData	= NULL;
	Average = 0.0;
	Stdiv = 0.0;
	ZeroMemory( BrightLevel, sizeof(long)*256 );//表示濃度の度数分布を全て0で初期化する
	MarkNum = 0;
	memset(pMarkMap, 0, sizeof(pMarkMap));


	//作業領域に関する変数を初期化する
	WorkArea.Initialize(size);//作業領域を画像のサイズにあわす
	DoUseWorkArea = false;//作業領域を使わない状態で初期化する

	//画像データを初期化する
		switch( DataType )
		{
			case BYTE_FORMAT:
				PageNumber = 1;
				ChannelNumber = 1;
				pByteData = new BYTE[ DataSize.cx * DataSize.cy ];
				if(!pByteData)	return false;
				memset(pByteData, 0, DataSize.cx * DataSize.cy * sizeof(BYTE));
				DataName = "BYTE";
				break;

			case FLOAT_FORMAT:
				PageNumber = 1;
				ChannelNumber = 1;
				pFloatData = new float[ DataSize.cx * DataSize.cy];
				if(!pFloatData)	return false;
				memset(pFloatData, 0, DataSize.cx * DataSize.cy * sizeof(float));
				DataName = "FLOAT";
				break;

			case COMPLEX_FORMAT:
				PageNumber = 1;
				ChannelNumber = 4;
				pFloatData = new float[ DataSize.cx * DataSize.cy * ChannelNumber ];
				if(!pFloatData)	return false;
				memset(pFloatData, 0, DataSize.cx * DataSize.cy * ChannelNumber * sizeof(float));
				DataName = "COMPLEX";
				break;

			case THREE_D_FORMAT:
				PageNumber = page;
				ChannelNumber = 1;
				pByteData = new BYTE[ DataSize.cx * DataSize.cy * PageNumber * ChannelNumber ];
				if(!pByteData)	return false;
				memset(pByteData, 0, DataSize.cx * DataSize.cy * PageNumber * ChannelNumber * sizeof(BYTE));
				DataName = "3D";
				break;

			case XY_FORMAT:
				PageNumber = 1;
				ChannelNumber = 2;
				pFloatData = new float[ DataSize.cx * DataSize.cy * ChannelNumber ];
				if(!pFloatData)	return false;
				memset(pFloatData, 0, DataSize.cx * DataSize.cy * ChannelNumber * sizeof(float));
				DataName = "PHASE XY";
				break;

			case XYZ_FORMAT:
				PageNumber = 1;
				ChannelNumber = 3;
				pFloatData = new float[ DataSize.cx * DataSize.cy * ChannelNumber ];
				if(!pFloatData)	return false;
				memset(pFloatData, 0, DataSize.cx * DataSize.cy * ChannelNumber * sizeof(float));
				DataName = "3D SHAPE";
				break;

			case FLOAT_3D_FORMAT:
				PageNumber = page;
				ChannelNumber = 1;
				pFloatData = new float[ DataSize.cx * DataSize.cy * PageNumber ];
				if(!pFloatData)	return false;
				memset(pFloatData, 0, DataSize.cx * DataSize.cy * PageNumber * sizeof(float));
				DataName = "FLOAT 3D";
				break;

			case RGB_FORMAT:
				PageNumber = 1;
				ChannelNumber = 3;
				pByteData = new BYTE[ DataSize.cx * DataSize.cy * ChannelNumber ];
				if(!pByteData)	return false;
				memset(pByteData, 0, DataSize.cx * DataSize.cy * ChannelNumber * sizeof(BYTE));
				DataName = "RGB COLOR";
				break;

			case SHORT16_FORMAT:
				PageNumber = 1;
				ChannelNumber = 1;
				pShortData = new short[ DataSize.cx * DataSize.cy ];
				if(!pShortData)	return false;
				memset(pShortData, 0, DataSize.cx * DataSize.cy * sizeof(short));
				DataName = "SHORT";
				break;

			case WORD_FORMAT:
				PageNumber = 1;
				ChannelNumber = 1;
				pWordData = new WORD[ DataSize.cx * DataSize.cy ];
				if(!pWordData)	return false;
				memset(pWordData, 0, DataSize.cx * DataSize.cy * sizeof(WORD));
				DataName = "WORD";
				break;

			case LONG32_FORMAT:
				PageNumber = 1;
				ChannelNumber = 1;
				pLongData = new long[ DataSize.cx * DataSize.cy ];
				if(!pLongData)	return false;
				memset(pLongData, 0, DataSize.cx * DataSize.cy * sizeof(long));
				DataName = "LONG";
				break;

			case DWORD_FORMAT:
				PageNumber = 1;
				ChannelNumber = 1;
				pDwordData = new DWORD[ DataSize.cx * DataSize.cy ];
				if(!pDwordData)	return false;
				memset(pDwordData, 0, DataSize.cx * DataSize.cy * sizeof(DWORD));
				DataName = "DWORD";
				break;

			case RGB_3D_FORMAT:
				PageNumber = page;
				ChannelNumber = 3;
				pByteData = new BYTE[ DataSize.cx * DataSize.cy * PageNumber * ChannelNumber ];
				if(!pByteData)	return false;
				memset(pByteData, 0, DataSize.cx * DataSize.cy * PageNumber * ChannelNumber * sizeof(BYTE));
				DataName = "RGB 3D";
				break;

			default:
				return false;
		}

	///表示画像バッファを初期化する
	if( DataSize.cx % 4 == 0)		DispBufferRow = DataSize.cx;
	else							DispBufferRow = DataSize.cx + (4 - DataSize.cx % 4);

	pDispBuffer = new BYTE[ DispBufferRow * DataSize.cy ];//表示画像バッファのメモリ取得する
	if(!pDispBuffer){
		Destroy();
		return false;
	}
	memset( pDispBuffer , 0, DispBufferRow * DataSize.cy  );//表示画像の濃度をゼロクリアする

	BrightMax = 256.0;//表示する最大濃度を256で初期化する
	BrightMin = 0.0;//表示する最低濃度を0で初期化する
	DispScale = 1.0;//スケールを1で初期化する
	DataDispOffset = CPoint(0,0);//オフセット（表示する矩形領域の左上の座標）を画像の原点で初期化する
	set_bmp_info(size);//ビットマップ情報を設定する
	ChangePalette( PALETTE_GRAYSCALE );//グレースケールでカラーパレットを初期化する

	if(DataType==RGB_FORMAT || DataType==RGB_3D_FORMAT)
	{
		///RGB用画像バッファを初期化する 2006.05.19
		if( (DataSize.cx*3) % 4 == 0)		DispBufferRowRGB = DataSize.cx*3;
		else								DispBufferRowRGB = DataSize.cx*3 + (4 - (DataSize.cx*3) % 4);

		pDispBufferRGB = new BYTE[ DispBufferRowRGB * DataSize.cy ];//表示画像バッファのメモリ取得する
		if(!pDispBufferRGB){
			Destroy();
			return false;
		}
		ZeroMemory( pDispBufferRGB , DispBufferRowRGB * DataSize.cy  );//表示画像の濃度をゼロクリアする
	}
	else
	{
		pDispBufferRGB = NULL;
		DispBufferRowRGB = 0;
	}

	///マスクデータを初期化する
	pMaskData = new BYTE[ DataSize.cx * DataSize.cy ];//マスクデータのメモリ取得する
	if(!pMaskData){
		Destroy();
		return false;
	}

	ClearMask();//マスクデータをゼロクリア（全ての画素をマスクがかかっていない状態で初期化する）
	DoUseMask = true;


	//マークを初期化する
	//pMark = new CMark[MARK_NUMBER_MAX];//マークのメモリ取得する
	//if(!pMark){
	//	Destroy();
	//	return false;
	//}

	//for(int i=0 ; i<MARK_NUMBER_MAX ; i++){
	//	pMark[i].x = pMark[i].y = 0;
	//	memset(pMark[i].attrib , 0, MARK_ATTRIB_NUM*sizeof(double));
	//}
	/*MarkNum = 0;*/


	return true;
}


/********************************************************************
機  能  名  称 : BITMAPINFOHEADERの初期化（8ビットビットマップで初期化する）
関    数    名 : CDataUnit
引          数 : CSize			size		(in)画像サイズ
戻    り    値 : 
機          能 : pDispBufferBmpInfoを8ビットビットマップで初期化する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataUnit::set_bmp_info(CSize size)
{
	pDispBufferBmpInfo = (BITMAPINFO*)new char[ sizeof(BITMAPINFOHEADER) + 1024 ];//BITMAPINFOHEADER + カラーパレットのメモリを取得する
	pDispBufferBmpInfo->bmiHeader.biSize = 40;//ヘッダのサイズ
	pDispBufferBmpInfo->bmiHeader.biWidth = size.cx;//画像データの幅
	pDispBufferBmpInfo->bmiHeader.biHeight = size.cy;//画像データの高さ
	pDispBufferBmpInfo->bmiHeader.biPlanes = 1;
	pDispBufferBmpInfo->bmiHeader.biBitCount = 8;//表示画像バッファの1画素あたりのサイズ
	pDispBufferBmpInfo->bmiHeader.biCompression = 0;//表示画像バッファの圧縮形式
	pDispBufferBmpInfo->bmiHeader.biSizeImage   = 0;
	pDispBufferBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	pDispBufferBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	pDispBufferBmpInfo->bmiHeader.biClrUsed = 0;
	pDispBufferBmpInfo->bmiHeader.biClrImportant = 0;

	pDispBufferBmpInfoRGB = (BITMAPINFO*)new char[ sizeof(BITMAPINFOHEADER) ];//BITMAPINFOHEADERのメモリを取得する
	pDispBufferBmpInfoRGB->bmiHeader.biSize = sizeof(BITMAPINFOHEADER) ;//ヘッダのサイズ
	pDispBufferBmpInfoRGB->bmiHeader.biWidth = size.cx;//画像データの幅
	pDispBufferBmpInfoRGB->bmiHeader.biHeight = size.cy;//画像データの高さ
	pDispBufferBmpInfoRGB->bmiHeader.biPlanes = 1;
	pDispBufferBmpInfoRGB->bmiHeader.biBitCount = 24;//表示画像バッファの1画素あたりのサイズ
	pDispBufferBmpInfoRGB->bmiHeader.biCompression = 0;//表示画像バッファの圧縮形式
	pDispBufferBmpInfoRGB->bmiHeader.biSizeImage   = 0;
	pDispBufferBmpInfoRGB->bmiHeader.biXPelsPerMeter = 0;
	pDispBufferBmpInfoRGB->bmiHeader.biYPelsPerMeter = 0;
	pDispBufferBmpInfoRGB->bmiHeader.biClrUsed = 0;
	pDispBufferBmpInfoRGB->bmiHeader.biClrImportant = 0;
}

/********************************************************************
機  能  名  称 : デストラクタ
関    数    名 : ~CDataUnit
引          数 : 
戻    り    値 : 
機          能 : CDataUnitの破棄
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
CDataUnit::~CDataUnit()
{
	Destroy();
}

/********************************************************************
機  能  名  称 : 内部データ解放
関    数    名 : Destroy
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataUnit::Destroy()
{
	if(pByteData)			delete[]	pByteData;
	if(pFloatData)			delete[]	pFloatData;
	if(pShortData)			delete[]	pShortData;
	if(pWordData)			delete[]	pWordData;
	if(pLongData)			delete[]	pLongData;
	if(pDwordData)			delete[]	pDwordData;
	if(pMaskData)			delete[]	pMaskData;
	if(pDispBuffer)			delete[]	pDispBuffer;
	if(pDispBufferBmpInfo)	delete[]	pDispBufferBmpInfo;
	if(pDispBufferRGB)			delete[]	pDispBufferRGB;
	if(pDispBufferBmpInfoRGB)	delete[]	pDispBufferBmpInfoRGB;
	
	for(int i=0 ; i<MARK_NUMBER_MAX ; i++){
		if(pMarkMap[i]){
			delete pMarkMap[i];
			pMarkMap[i]=NULL;
		}
	}


	complex_image_power_log = false;
	DataType = -1;
	DataSize = CSize(0,0);
	DataName.Empty();
	SrcFilePath.Empty();
	DispPage = 0;
	DispChannel = 0;
	DispRGB = true;
	pByteData	= NULL;
	pFloatData	= NULL;
	pShortData	= NULL;
	pWordData	= NULL;
	pLongData	= NULL;
	pDwordData	= NULL;
	Average = 0.0;
	Stdiv = 0.0;
	MarkNum = 0;
	ZeroMemory( BrightLevel, sizeof(long)*256 );//表示濃度の度数分布を全て0で初期化する
	BrightMax = 256.0;
	BrightMin = 0.0;
	DispScale = 1.0;
	DataDispOffset = CPoint(0,0);

}

/********************************************************************
機  能  名  称 : 画像データ　→　表示画像　に変換
関    数    名 : UpdateDispBuffer
引          数 : 
戻    り    値 : 
機          能 : 明度変換、マスクかけ
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CDataUnit::UpdateDispBuffer()
{
	CRect			calc_area = GetRectArea();//処理対象の矩形範囲	

	//表示濃度変換
	if( pByteData )
	{				
		calc_bright( this , pByteData);
		calc_hist_and_stat( this , pByteData,  calc_area, API.rgb_gain);
	}
	else if( pFloatData )
	{		
		calc_bright( this , pFloatData );
		calc_hist_and_stat( this , pFloatData,  calc_area, API.rgb_gain);
	}
	else if( pShortData )
	{		
		calc_bright( this , pShortData );
		calc_hist_and_stat( this , pShortData,  calc_area, API.rgb_gain);
	}
	else if( pWordData )
	{		
		calc_bright( this , pWordData );
		calc_hist_and_stat( this , pWordData,  calc_area, API.rgb_gain);
	}
	else if( pLongData )
	{		
		calc_bright( this , pLongData );
		calc_hist_and_stat( this , pLongData,  calc_area, API.rgb_gain);
	}
	else if( pDwordData )
	{		
		calc_bright( this , pDwordData );
		calc_hist_and_stat( this , pDwordData,  calc_area, API.rgb_gain);
	}

	//TRACE("UPDATE!\n");
}

/********************************************************************
機  能  名  称 : 表示濃度レンジを変更する
関    数    名 : SetDispRange
引          数 : float			min			(in)最小濃度
				 float			max			(in)最大濃度
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataUnit::SetDispRange(float min, float max)
{
	if(max < min)	return	false;//表示レンジとして指定した2値の大小関係が逆のときはなにもしない

	BrightMax = max;
	BrightMin = min;

	return	true;
}

/********************************************************************
機  能  名  称 : 表示の拡大率を変更する
関    数    名 : SetDispScale
引          数 : float			scale		(in)拡大率
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataUnit::SetDispScale(float scale)
{
	DispScale = scale;
	DataDispOffset = CPoint(0,0);
}

/********************************************************************
機  能  名  称 : 表示ページ番号（動画の奥行き番号）を変更する
関    数    名 : MoveDispPage
引          数 : int			offset		(in) 表示ページ変更数
戻    り    値 : 成功したらtrue
機          能 : 3D,F3D,RGB3Dタイプにおいて、表示画像の奥行き番号を変更する
				 現在表示中の番号からの相対位置で指定する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataUnit::MoveDispPage(int offset)
{
	int	dest_page = DispPage + offset;
	if( dest_page < 0	||	dest_page >= PageNumber)	return  false;

	DispPage = dest_page;
	return	true;
}

/********************************************************************
機  能  名  称 : 表示ページ番号（動画の奥行き番号）を変更する
関    数    名 : SetDispPage
引          数 : int			page		(in)表示ページ番号
戻    り    値 : 成功したらtrue
機          能 : 3D,F3D,RGB3Dタイプにおいて、表示画像の奥行き番号を変更する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataUnit::SetDispPage(int page)
{
	if( page < 0	||	page >= PageNumber)	return false;

	DispPage = page;
	return	true;
}

/********************************************************************
機  能  名  称 : 表示チャネルを変更する
関    数    名 : SetDispPage
引          数 : int	channel		(in)表示channel番号 RGB表示の場合は-1
戻    り    値 : 成功したらtrue
機          能 : RGB,RGB3Dタイプにおいて、表示画像のチャネルを変更する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataUnit::SetDispChannel(int channel)
{

	if(channel==-1){
		DispRGB=true;
	}

	if( channel < 0	||	channel >= ChannelNumber)	return false;

	DispRGB = false;
	DispChannel = channel;

	return	true;
}


/********************************************************************
機  能  名  称 : 表示画像のカラーパレットを変更する
関    数    名 : ChangePalette
引          数 : short			palette_id		(in)カラーパレットのID					
戻    り    値 : 成功したらtrue
機          能 : RGB,RGB3D以外の画像データ表示時の、擬似カラー表示方法を変更する
				 設定できる種類は下記
					PALETTE_GRAYSCALE：		グレースケール
					PALETTE_NEGATIVE：		グレースケール反転
					PALETTE_RAINBOW：		Ｂ→Ｇ→Ｒに滑らかに変化
					PALETTE_BLACK_RAINBOW：	Ｂ→Ｇ→Ｒに滑らかに変化。0未満は黒
					PALETTE_COSINE：		正弦波状の濃度変化
					PALETTE_BLUE_YELLOW：	Ｂ→Ｙに滑らかに変化
					PALETTE_GREEN_RED：		Ｇ→Ｒに滑らかに変化
					PALETTE_LABEL:			濃度１ごとに表示色が大きく変化する(ラベル用)
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataUnit::ChangePalette(short palette_id)
{
	int i;

	switch( palette_id )
	{
		case	PALETTE_GRAYSCALE:
			for(i=0 ; i<256 ; i++)
			{
				pDispBufferBmpInfo->bmiColors[i].rgbRed = i;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = i;
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = i;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}
			break;

		case	PALETTE_NEGATIVE:
			for(i=0 ; i<256 ; i++)
			{
				pDispBufferBmpInfo->bmiColors[i].rgbRed = 255-i;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = 255-i;
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = 255-i;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}
			break;

		case	PALETTE_BLACK_RAINBOW:
			//no break
		case	PALETTE_RAINBOW:
			for(i=0 ; i<64 ; i++)
			{
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = 255;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = i*4;
				pDispBufferBmpInfo->bmiColors[i].rgbRed = 0;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}

			for(i=64 ; i<128 ; i++)
			{
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = 255 - (i-64)*4;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = 255;
				pDispBufferBmpInfo->bmiColors[i].rgbRed = 0;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}

			for(i=128 ; i<192 ; i++)
			{
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = 0;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = 255;
				pDispBufferBmpInfo->bmiColors[i].rgbRed = (i-128)*4;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}

			for(i=192 ; i<256 ; i++)
			{
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = 0;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = 255 - (i-192)*4;
				pDispBufferBmpInfo->bmiColors[i].rgbRed = 255;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}

			if( palette_id == PALETTE_BLACK_RAINBOW )//0のときだけ黒にする
			{
				pDispBufferBmpInfo->bmiColors[0].rgbBlue = 0;
				pDispBufferBmpInfo->bmiColors[0].rgbGreen = 0;
				pDispBufferBmpInfo->bmiColors[0].rgbRed = 0;
			}
			break;

		case	PALETTE_COSINE:
			for(i=0 ; i<256 ; i++)
			{
				double temp_kido = 127.5 * cos (i*2.0*PI/255.0) + 127.5;
				pDispBufferBmpInfo->bmiColors[i].rgbRed = (unsigned char)temp_kido;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = (unsigned char)temp_kido;
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = (unsigned char)temp_kido;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}
			break;

		case	PALETTE_BLUE_YELLOW:
			for(i=0 ; i<256 ; i++)
			{
				pDispBufferBmpInfo->bmiColors[i].rgbRed = i;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = i;
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = 255-i;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}
			break;

		case	PALETTE_GREEN_RED:
			for(i=0 ; i<256 ; i++)
			{
				pDispBufferBmpInfo->bmiColors[i].rgbRed = i;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = 255-i;
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = 0;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}
			break;

		case	PALETTE_LABEL:
			{
				BYTE	r[]={0,		255,	255,	255,	0,		0};
				BYTE	g[]={255,	0,		255,	0,		255,	0};
				BYTE	b[]={255,	255,	0,		0,		0,		255};

				for(i=1 ; i<256 ; i++)
				{
					pDispBufferBmpInfo->bmiColors[i].rgbRed = r[i%6];
					pDispBufferBmpInfo->bmiColors[i].rgbGreen = g[i%6];
					pDispBufferBmpInfo->bmiColors[i].rgbBlue = b[i%6];
					pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
				}
				//0のときだけ黒にする
				pDispBufferBmpInfo->bmiColors[0].rgbBlue = 0;
				pDispBufferBmpInfo->bmiColors[0].rgbGreen = 0;
				pDispBufferBmpInfo->bmiColors[0].rgbRed = 0;
			}
			break;

		default:
			break;
	}
}

/********************************************************************
機  能  名  称 : マスクをクリア
関    数    名 : ClearMask
引          数 : 
戻    り    値 : 
機          能 : マスクの値をすべてUNMASKED_PIXELにする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataUnit::ClearMask()
{
	ZeroMemory( pMaskData, DataSize.cx * DataSize.cy  );
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//データ作成
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
機  能  名  称 : 複素数画像の実部・虚部からパワースペクトル・位相分布をつくる
関    数    名 : CreateComplexPolarComponents
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataUnit::CreateComplexPolarComponents()
{
	if( DataType!=COMPLEX_FORMAT )		return;
	if( !pFloatData )	return;

	float	*real_data , *imag_data , *power_data , *phase_data ;

	real_data = pFloatData;
	imag_data = pFloatData + DataSize.cx * DataSize.cy;
	power_data = pFloatData + DataSize.cx * DataSize.cy * 2;
	phase_data = pFloatData + DataSize.cx * DataSize.cy * 3;

	for(int i=0 ; i<DataSize.cx * DataSize.cy ; i++)
	{
		if( complex_image_power_log )//パワースペクトルで対数をとる
		{
			*power_data = (float)log( sqrt((*real_data) * (*real_data) + (*imag_data) * (*imag_data))  );
		}
		else
		{
			*power_data = (float) sqrt((*real_data) * (*real_data) + (*imag_data) * (*imag_data))  ;
		}

		*phase_data = (float)atan2( *imag_data , *real_data);

		real_data ++ ;
		imag_data ++ ;
		power_data ++ ;
		phase_data ++ ;
	}
}

/********************************************************************
機  能  名  称 : 複素数画像のパワースペクトル・位相分布から実部・虚部を作る
関    数    名 : CreateComplexDescartesComponents
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataUnit::CreateComplexDescartesComponents()
{
	if( DataType != COMPLEX_FORMAT )		return;
	if( !pFloatData )						return;	

	float	*real_data , *imag_data , *power_data , *phase_data ;
	int		i;

	real_data = pFloatData;
	imag_data = pFloatData + DataSize.cx * DataSize.cy;
	power_data = pFloatData + DataSize.cx * DataSize.cy * 2;
	phase_data = pFloatData + DataSize.cx * DataSize.cy * 3;


		for(i=0 ; i<DataSize.cx * DataSize.cy ; i++)
		{
			if( complex_image_power_log )//パワーの対数をとる
			{
				float	temp = (float) exp( *power_data );	
				*real_data = temp * cos( *phase_data );
				*imag_data = temp * sin( *phase_data );
			}
			else
			{
				*real_data = ( *power_data ) * cos( *phase_data );
				*imag_data = ( *power_data ) * sin( *phase_data );
			}		

			power_data++;
			phase_data++;
			real_data++;
			imag_data++;
		}

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//データ取得
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
機  能  名  称 : 矩形領域を取得する
関    数    名 : GetRectArea
引          数 : 
戻    り    値 : 作業領域を使う場合は、外接する矩形を返す。
				 使わない場合は画像全体サイズの矩形を返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
CRect CDataUnit::GetRectArea()
{
	if( DoUseWorkArea )//作業領域を使う場合
	{
		CRect rect = WorkArea.GetCircumRect();

		if(rect.left<0)	rect.left=0;
		if(rect.right>=DataSize.cx)	rect.right = DataSize.cx-1;
		if(rect.top<0)	rect.top=0;
		if(rect.bottom>=DataSize.cy)	rect.bottom = DataSize.cy-1;

		return rect;
	}
	else//作業領域を使わない場合
	{
		return  CRect(0, 0, DataSize.cx-1, DataSize.cy-1);
	}
}

/********************************************************************
機  能  名  称 : 画像メモリのタイプ・サイズ・奥行き数をチェックする
関    数    名 : Compare
引          数 : long		type		(in)画像メモリのタイプ
				 CSize		size		(in)画像メモリのサイズ
				 long		page		(in)画像メモリの奥行き数
戻    り    値 : 引数で指定されたものと同じならtrueを返す　異なればfalseを返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataUnit::Compare(long type, CSize size, long page)
{
	if( DataType == type   &&   DataSize == size   &&   page == page )		return	true;
	else																	return	false;
}

/********************************************************************
機  能  名  称 : 画像メモリのタイプ・サイズ・奥行き数を比較する
関    数    名 : Compare
引          数 : CDataUnit		*p_du		(in)比較対象のCDataUnitへのポインタ
戻    り    値 : 引数で指定されたものと同じならtrueを返す　異なればfalseを返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataUnit::Compare(CDataUnit *p_du)
{
	if( !p_du )		return	false;
	return		Compare(p_du->DataType , p_du->DataSize , p_du->PageNumber);
}

/********************************************************************
機  能  名  称 : 画像メモリの最大値・最小値を求める
関    数    名 : GetDataMinMax
引          数 : float			*p_min		(out)画像メモリの最小値
				 float			*p_max		(out)画像メモリの最大値
戻    り    値 : 
機          能 : チャネル・奥行きが複数ある画像の場合は、表示中の画像の最大・最小を求める
				 RGB表示の場合は、グレー化した値を求める
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataUnit::GetDataMinMax(float *p_min, float *p_max)
{
	CRect			calc_area = GetRectArea();//処理対象の矩形範囲

	////計算
		if( pByteData )
		{				
			calc_min_max( this , pByteData,  calc_area, p_min, p_max, API.rgb_gain);
		}
		else if( pFloatData )
		{		
			calc_min_max( this , pFloatData,  calc_area, p_min, p_max, API.rgb_gain);
		}
		else if( pShortData )
		{	
			calc_min_max( this , pShortData,  calc_area, p_min, p_max, API.rgb_gain);
		}
		else if( pWordData )
		{	
			calc_min_max( this , pWordData,  calc_area, p_min, p_max, API.rgb_gain);
		}
		else if( pLongData )
		{	
			calc_min_max( this , pLongData,  calc_area, p_min, p_max, API.rgb_gain);
		}
		else if( pDwordData )
		{	
			calc_min_max( this , pDwordData,  calc_area, p_min, p_max, API.rgb_gain);
		}
}

/********************************************************************
機  能  名  称 : 指定された座標のデータ値を求める
関    数    名 : GetDataValue
引          数 : CPoint			point		(in)座標
				 int			page		(in)奥行き（-1の場合は現在表示中のもの）
				 int			channel		(in)チャネル（-1の場合は現在表示中のもの）
戻    り    値 : 
機          能 : はみ出した場合は0を返す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
double CDataUnit::GetDataValue(CPoint point, long page, int channel)
{
#if 1 //処理速度短縮版
	if(point.x<0  ||  point.x>=DataSize.cx)		return	0;
	if(point.y<0  ||  point.y>=DataSize.cy)		return	0;


	if(page<0)	page=DispPage;
	if(channel<0)
	{
		if( (DataType==RGB_FORMAT || DataType==RGB_3D_FORMAT) && DispRGB)//RGB表示の場合は明度
		{
			return GetDataValue(point.x, point.y, page, channel);
		}

		channel=DispChannel;
	}
	
	long	addr_page_offset = DataSize.cx * DataSize.cy * (page * ChannelNumber + channel);

	if( pByteData )				
		return	(double)*(pByteData + point.x + point.y * DataSize.cx + addr_page_offset);

	else if( pFloatData )		
		return	(double)*(pFloatData + point.x + point.y * DataSize.cx + addr_page_offset);	

	else if( pShortData )		
		return	(double)*(pShortData + point.x + point.y * DataSize.cx + addr_page_offset);

	else if( pWordData )		
		return	(double)*(pWordData + point.x + point.y * DataSize.cx + addr_page_offset);	

	else if( pLongData )		
		return	(double)*(pLongData + point.x + point.y * DataSize.cx + addr_page_offset);	

	else if( pDwordData )		
		return	(double)*(pDwordData + point.x + point.y * DataSize.cx + addr_page_offset);	


	return 0;
#else
	return GetDataValue(point.x, point.y, rgb_type,page,channel);
#endif
}

/********************************************************************
機  能  名  称 : 指定された座標のデータ値を求める（サブピクセルで指定）
関    数    名 : GetDataValue
引          数 : double			x			(in)座標x
				 double			y			(in)座標y
				 int			page		(in)奥行き（-1の場合は現在表示中のもの）
				 int			channel		(in)チャネル（-1の場合は現在表示中のもの）
戻    り    値 : 指定された座標のデータ値（サブピクセル） はみ出した場合は0を返す
機          能 : サブピクセルでの値を求める
				 はみ出した場合は0を返す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
double CDataUnit::GetDataValue(double x, double y, long page, int channel)
{
	if(x<0  ||  x>DataSize.cx-1)		return	0;
	if(y<0  ||  y>DataSize.cy-1)		return	0;
	if ( page>PageNumber - 1)		return 0;
	if ( channel>ChannelNumber - 1)		return 0;


	if( pByteData )	
	{
		return	calc_sub_pix( this , pByteData, x, y, page, channel, API.rgb_gain);
	}
	else if( pFloatData )
	{
		return	calc_sub_pix( this  ,pFloatData, x, y, page, channel, API.rgb_gain);	
	}
	else if( pShortData )
	{
		return	calc_sub_pix( this , pShortData, x, y, page, channel, API.rgb_gain);
	}
	else if( pWordData )
	{
		return	calc_sub_pix( this , pWordData, x, y, page, channel, API.rgb_gain);
	}
	else if( pLongData )
	{
		return	calc_sub_pix( this , pLongData, x, y, page, channel, API.rgb_gain);	
	}
	else if( pDwordData )
	{
		return	calc_sub_pix( this , pDwordData, x, y, page, channel, API.rgb_gain);	
	}

	return 0;
}

/********************************************************************
機  能  名  称 : 指定された座標のマスク値を取得する
関    数    名 : GetMaskValue
引          数 : CPoint			point		(in)座標
戻    り    値 : 指定された座標のマスク値  0:UNMASKED_PIXEL 1:MASKED_PIXEL
機          能 : はみ出した場合は0を返す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
BYTE CDataUnit::GetMaskValue(CPoint point)
{
		if(point.x<0  ||  point.x>=DataSize.cx)		return	0;
		if(point.y<0  ||  point.y>=DataSize.cy)		return	0;

		return pMaskData[point.x + point.y*DataSize.cx];
}

/********************************************************************
機  能  名  称 : 指定された座標のデータ値を設定する
関    数    名 : SetDataValue
引          数 : int			x			(in)座標x
				 int			y			(in)座標y
				 double			data		(in)データ値
				 int			page		(in)奥行き（-1の場合は現在表示中のもの）
				 int			channel		(in)チャネル（-1の場合は現在表示中のもの （RGB表示の場合はすべてのチャネルに同じ値が設定））
				 bool			satulate	(in)データ上下限で飽和させるかどうか(falseの場合はオーバーフローする)
戻    り    値 : 成功するとtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataUnit::SetDataValue(int x, int y, double data, long page, int channel, bool satulate)
{
	//引数チェック
	if(x<0  ||  x>DataSize.cx-1)		return	false;
	if(y<0  ||  y>DataSize.cy-1)		return	false;

	if(page<0)	page=DispPage;

	if(channel<0)
	{
		if( (DataType==RGB_FORMAT || DataType==RGB_3D_FORMAT) && DispRGB)//RGB表示の場合はすべてのチャネルに同じ値を入れる
		{
			for(int c=0 ; c<3 ; c++)
			{
				int addr = x + y*DataSize.cx + DataSize.cx*DataSize.cy * (page * ChannelNumber + c)	;
				if(data<0)			*(pByteData + addr) = 0;
				else if(data>255)	*(pByteData + addr) = 255;
				else				*(pByteData + addr) = (BYTE)data;
			}		
		}

		channel=DispChannel;
	}


	int addr = x + y*DataSize.cx + DataSize.cx*DataSize.cy * (page * ChannelNumber + channel)	;

	if( pByteData )	
	{
		if(satulate){
			if(data<0.0)			*(pByteData + addr) = 0;
			else if(data>255.0)	*(pByteData + addr) = 255;
			else				*(pByteData + addr) = (BYTE)data;
		}else{
			*(pByteData + addr) = (BYTE)data;
		}
	}
	else if( pFloatData )
	{
		*(pFloatData + addr) = (float)data;
	}
	else if( pShortData )
	{
		if(satulate){
			if(data<-32768.0)		*(pShortData + addr) = (short)-32768;
			else if(data>32767.0)	*(pShortData + addr) = (short)32767;
			else				*(pShortData + addr) = (short)data;
		}else{
			*(pShortData + addr) = (short)data;
		}
	}
	else if( pWordData )
	{
		if(satulate){
			if(data<0.0)			*(pWordData + addr) = (WORD)0;
			else if(data>65535.0)	*(pWordData + addr) = (WORD)65535;
			else				*(pWordData + addr) = (WORD)data;
		}else{
			*(pWordData + addr) = (WORD)data;
		}
	}
	else if( pLongData )
	{
		if(satulate){
			if(data<-2147483648.0)		*(pLongData + addr) = (long)-2147483648;
			else if(data>2147483647.0)	*(pLongData + addr) = (long)2147483647;
			else						*(pLongData + addr) = (long)data;
		}else{
			*(pLongData + addr) = (long)data;
		}
	}
	else if( pDwordData )
	{
		if(satulate){
			if(data<0.0)					*(pDwordData + addr) = (DWORD)0;
			else if(data>4294967295.0)	*(pDwordData + addr) = (DWORD)4294967295;
			else						*(pDwordData + addr) = (DWORD)data;
		}else{
			*(pDwordData + addr) = (DWORD)data;
		}
	}

	return true;
}

/********************************************************************
機  能  名  称 : 指定された座標のデータ値を設定する
関    数    名 : SetMaskValue
引          数 : int			x			(in)座標x
				 int			y			(in)座標y
				 BYTE			mask		(in)マスク値 0:UNMASKED_PIXEL 1:MASKED_PIXEL
戻    り    値 : 成功するとtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataUnit::SetMaskValue(int x, int y, BYTE mask)
{
		if(x<0  ||  x>DataSize.cx-1)		return	false;
		if(y<0  ||  y>DataSize.cy-1)		return	false;

		pMaskData[x + y*DataSize.cx] = mask;
		return true;
}

/********************************************************************
機  能  名  称 : 指定された座標の表示画像の濃度を返す
関    数    名 : GetDisplayIntensity
引          数 : CPoint			point		(in)座標
戻    り    値 : 濃度値　はみ出した場合は0を返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
unsigned char CDataUnit::GetDisplayIntensity(CPoint point)
{
	int		intensity;

	if(point.x<0  ||  point.x>=DataSize.cx)		return	0;
	if(point.y<0  ||  point.y>=DataSize.cy)		return	0;

	if( (DataType != RGB_FORMAT && DataType != RGB_3D_FORMAT)  ||  DispRGB == false )//グレースケール
	{
		return	*(pDispBuffer + point.x  + (DataSize.cy - 1 - point.y) * DispBufferRow);
	}
	else//RGBカラー
	{
		intensity = ( API.rgb_gain[0] * (int)*(pDispBufferRGB + point.x*3  + (DataSize.cy - 1 - point.y) * DispBufferRowRGB)		//r
				    + API.rgb_gain[1] * (int)*(pDispBufferRGB + point.x*3  + (DataSize.cy - 1 - point.y) * DispBufferRowRGB + 1)	//g
				    + API.rgb_gain[2] * (int)*(pDispBufferRGB + point.x*3  + (DataSize.cy - 1 - point.y) * DispBufferRowRGB + 2)  )/1000;//b
		return (unsigned char)intensity;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//データの変更
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : マークを追加する
関    数    名 : AddMark
引          数 : 
戻    り    値 : 成功したら追加されたマーク番号を返す失敗したら-1を返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int	CDataUnit::AddMark(CMark mark)
{
	int idx = MarkNum;
	if( idx +1>= MARK_NUMBER_MAX )	return -1;//マークの数チェック

	if(pMarkMap[idx]==NULL){//新たにマーク用のメモリをとる
		pMarkMap[idx] = new CMark();
		if(pMarkMap[idx]==NULL)	return -1;
	}

	//マークをコピーする
	pMarkMap[idx]->type = mark.type;
	memcpy( &pMarkMap[idx]->figure, &mark.figure, sizeof(FIGURE_UNION));
	for(int i=0 ; i<MARK_ATTRIB_NUM ; i++){
		pMarkMap[idx]->attrib[i] = mark.attrib[i];
	}


	MarkNum++;//マークの数を一つ増やす

	return idx;
}

/********************************************************************
機  能  名  称 : マークを消去する
関    数    名 : DeleteMark
引          数 : long				id			(in)マーク番号
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataUnit::DeleteMark(long idx)
{
	if( idx>= MarkNum || idx<0)	return false;//引数チェック

	if(pMarkMap[idx])
	{
		delete pMarkMap[idx];//idx番目のメモリを削除

		for(int i=idx ; i<MARK_NUMBER_MAX-1 ; i++){//ポインタをずらす
			pMarkMap[i] = pMarkMap[i+1];
		}

		pMarkMap[MARK_NUMBER_MAX-1]=NULL;//最後はNULLにする

		MarkNum--;//マーク数を１つ減らす

		return true;
	}else{
		return false;
	}
}

/********************************************************************
機  能  名  称 : すべてのマーククリア
関    数    名 : ClearMark
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataUnit::ClearMark()
{
	for(int i=0 ; i<MARK_NUMBER_MAX ; i++){
		if(pMarkMap[i]){
			delete pMarkMap[i];
			pMarkMap[i]=NULL;
		}
		MarkNum=0;
	}
}

/********************************************************************
機  能  名  称 : マークのデータ値を取得する
関    数    名 : GetMark
引          数 : 
				 long				idx			(in)マーク番号

戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataUnit::GetMark(long idx, CMark *pmark)
{
	if( idx>= MarkNum || idx<0)	return false;//引数チェック

	if(pMarkMap[idx])
	{
		pmark->type = pMarkMap[idx]->type;
		memcpy( &pmark->figure, &pMarkMap[idx]->figure, sizeof(FIGURE_UNION));
		for(int i=0 ; i<MARK_ATTRIB_NUM ; i++){
			pmark->attrib[i]  = pMarkMap[idx]->attrib[i];
		}
		return true;
	}else{
		return false;
	}
}

/********************************************************************
機  能  名  称 : マークのデータ値を設定する
関    数    名 : SetMark
引          数 : 
				 long				idx			(in)マーク番号

戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataUnit::SetMark(long idx, CMark mark)
{
	if( idx>= MarkNum || idx<0)	return false;//引数チェック

	if(pMarkMap[idx])
	{
		pMarkMap[idx]->type = mark.type;
		memcpy( &pMarkMap[idx]->figure, &mark.figure, sizeof(FIGURE_UNION));
		for(int i=0 ; i<MARK_ATTRIB_NUM ; i++){
			pMarkMap[idx]->attrib[i] = mark.attrib[i];
		}
		return true;
	}
	else
	{
		return false;
	}

}


long CDataUnit::GetMarkNumber()
{
	return MarkNum;
}


/********************************************************************
機  能  名  称 : CDataUnitのメンバをコピー
関    数    名 : Copy
引          数 : CDataUnit		*p_src_du			(in)コピー元の画像メモリ
				 bool			do_copy_data		(in)画像メモリの中をコピー（フォーマット/サイズ/ページ/チャネルが同じ場合のみ）
				 bool			do_copy_mask		(in)マスクをコピー（サイズが同じ場合のみ）
				 bool			do_copy_workarea	(in)作業領域をコピー
				 bool			do_copy_range		(in)表示画像の濃度レンジをコピー
				 bool			do_copy_mark		(in)マークをコピーするかどうか
				 bool			do_copy_pos_scale	(in)表示位置とスケールをコピー（サイズが同じ場合のみ）
戻    り    値 : 成功するとtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
//データを指定してコピー
bool CDataUnit::Copy(CDataUnit *p_src_du, 
					 bool do_copy_data, 
					 bool do_copy_mask, 
					 bool do_copy_workarea,
					 bool do_copy_range, 
					 bool do_copy_mark, 
					 bool do_copy_pos_scale)
{

	if( !p_src_du )		return	false;//コピー元にデータが無ければ何もしない



	//画像データのコピー
	if(do_copy_data)
	{
		if( p_src_du->DataType == DataType && 
			p_src_du->DataSize == DataSize && 
			p_src_du->PageNumber == PageNumber && 
			p_src_du->ChannelNumber == ChannelNumber)
		{ 
			//データに関する変数のコピー
			if (do_copy_data){
				DataName = p_src_du->DataName;
			}

			DispPage = p_src_du->DispPage;
			DispChannel = p_src_du->DispChannel;
			DispRGB  = p_src_du->DispRGB ;
			Average = p_src_du->Average;
			Stdiv = p_src_du->Stdiv;
			memcpy( BrightLevel , p_src_du->BrightLevel , sizeof(long)*256 );
			SrcFilePath = p_src_du->SrcFilePath;


			////画像データのコピー
			if( pByteData )
			{
				memcpy( pByteData , p_src_du->pByteData , DataSize.cx * DataSize.cy * sizeof(BYTE) *PageNumber * ChannelNumber);
			}
			else if( pFloatData )
			{
				memcpy( pFloatData , p_src_du->pFloatData , DataSize.cx * DataSize.cy * sizeof(float) *PageNumber * ChannelNumber);
			}
			else if( pShortData )
			{
				memcpy( pShortData , p_src_du->pShortData , DataSize.cx * DataSize.cy * sizeof(short) *PageNumber * ChannelNumber);
			}
			else if( pWordData )
			{
				memcpy( pWordData , p_src_du->pWordData , DataSize.cx * DataSize.cy * sizeof(WORD) *PageNumber * ChannelNumber);
			}
			else if( pLongData )
			{
				memcpy( pLongData , p_src_du->pLongData , DataSize.cx * DataSize.cy * sizeof(long) *PageNumber * ChannelNumber);
			}
			else if( pDwordData )
			{
				memcpy( pDwordData , p_src_du->pDwordData , DataSize.cx * DataSize.cy * sizeof(DWORD) *PageNumber * ChannelNumber);
			}

			///	表示画像のコピー
			memcpy( pDispBuffer , p_src_du->pDispBuffer , DispBufferRow * DataSize.cy );

			///	RGB表示画像のコピー
			if(p_src_du->pDispBufferRGB != NULL)
			{
				memcpy( pDispBufferRGB , p_src_du->pDispBufferRGB , DispBufferRowRGB * DataSize.cy );
			}


			//擬似カラーパレットのコピー
			memcpy( pDispBufferBmpInfo->bmiColors , 
				p_src_du->pDispBufferBmpInfo->bmiColors , 1024 );
		}
	}

	//マスクデータのコピー
	if(do_copy_mask)
	{
		if( p_src_du->DataSize == DataSize )
		{
			memcpy( pMaskData , p_src_du->pMaskData , DataSize.cx * DataSize.cy );
			DoUseMask = p_src_du->DoUseMask;
		}
	}

	//作業領域のコピー
	if(do_copy_workarea)
	{
		//if( p_src_du->DataSize == DataSize )
		{
			WorkArea.Copy( &p_src_du->WorkArea );
			DoUseWorkArea = p_src_du->DoUseWorkArea;
		}
	}

	//表示濃度レンジのコピー
	if(do_copy_range)
	{
		BrightMax = p_src_du->BrightMax;
		BrightMin = p_src_du->BrightMin;
	}

	//マークのコピー
	if(do_copy_mark)
	{
		ClearMark();
		for(int i=0 ; i<p_src_du->GetMarkNumber() ; i++)
		{
			CMark mark;
			p_src_du->GetMark(i, &mark);
			AddMark(mark);
		}
	}

	//表示位置とスケールのコピー
	if(do_copy_pos_scale)
	{
		if(p_src_du->DataSize == DataSize)
		{
			DispScale = p_src_du->DispScale;
			DataDispOffset = p_src_du->DataDispOffset;
		}
	}

	return	true;
}

//すべてのメンバをコピーする
bool CDataUnit::Copy(CDataUnit *p_src_du)
{
	return Copy(p_src_du, true, true, true, true, true, true);
}

/********************************************************************
機  能  名  称 : 作業領域からマスクを作る
関    数    名 : SetMaskByWorkArea
引          数 : 
戻    り    値 : 成功するとtrue
機          能 : 作業領域外にすべてマスクをかける
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataUnit::SetMaskByWorkArea()
{
	for(int j=0 ; j<DataSize.cy ; j++)
	{
		for(int i=0 ; i<DataSize.cx ; i++)
		{
			if(WorkArea.PtInWorkArea(CPoint(i,j)))
			{
				*(pMaskData + i + j*DataSize.cx) = UNMASKED_PIXEL;
			}else{
				*(pMaskData + i + j*DataSize.cx) = MASKED_PIXEL;
			}
		}
	}
	return true;
}

/********************************************************************
機  能  名  称 : 作業領域の初期化
関    数    名 : InitWorkArea
引          数 : 
戻    り    値 : 
機          能 : 作業領域を画像全体を現す矩形のみにする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataUnit::InitWorkArea()
{
	WorkArea.Initialize(DataSize);
}



/********************************************************************
機  能  名  称 : 
関    数    名 : GetMinLimitValue
引          数 : 
戻    り    値 : データ値の下限値
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
double CDataUnit::GetMinLimitValue()
{
	//画像データを初期化する
		switch( DataType )
		{
			case BYTE_FORMAT:
				return 0;

			case FLOAT_FORMAT:
				return -FLT_MAX;

			case COMPLEX_FORMAT:
				return -FLT_MAX;

			case THREE_D_FORMAT:
				return 0;

			case XY_FORMAT:
				return -FLT_MAX;

			case XYZ_FORMAT:
				return -FLT_MAX;

			case FLOAT_3D_FORMAT:
				return -FLT_MAX;

			case RGB_FORMAT:
				return 0;

			case SHORT16_FORMAT:
				return SHRT_MIN;

			case WORD_FORMAT:
				return 0;

			case LONG32_FORMAT:
				return LONG_MIN;

			case DWORD_FORMAT:
				return 0;

			case RGB_3D_FORMAT:
				return 0;

		}
	return 0;
}



/********************************************************************
機  能  名  称 : 
関    数    名 : GetMaxLimitValue
引          数 : 
戻    り    値 : データ値の上限値
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
double CDataUnit::GetMaxLimitValue()
{
	//画像データを初期化する
		switch( DataType )
		{
			case BYTE_FORMAT:
				return UCHAR_MAX;

			case FLOAT_FORMAT:
				return FLT_MAX;

			case COMPLEX_FORMAT:
				return FLT_MAX;

			case THREE_D_FORMAT:
				return UCHAR_MAX;

			case XY_FORMAT:
				return FLT_MAX;

			case XYZ_FORMAT:
				return FLT_MAX;

			case FLOAT_3D_FORMAT:
				return FLT_MAX;

			case RGB_FORMAT:
				return UCHAR_MAX;

			case SHORT16_FORMAT:
				return SHRT_MAX;

			case WORD_FORMAT:
				return USHRT_MAX;

			case LONG32_FORMAT:
				return LONG_MAX;

			case DWORD_FORMAT:
				return ULONG_MAX;

			case RGB_3D_FORMAT:
				return UCHAR_MAX;

		}
	return 0;
}



