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
//幾何変換
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "opencv_headers.h"
#include "PIMPOM_API.h"
#include "filter\template\filter_ma.h"
#include "resize_data.h"

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

//マスクのサブピクセル値を計算
BYTE	sub_pix_mask_value(BYTE *p_mask_data, CSize src_size, double x, double y)
{
	//画面端で収まらない場合はマスクON
	if (x<-1 || x>src_size.cx || y<-1 || y>src_size.cy)	return MASKED_PIXEL;

	double mask_interm = sub_pix_value(p_mask_data, src_size, x, y);
	if ((MASKED_PIXEL + UNMASKED_PIXEL) / 2 < mask_interm)	return MASKED_PIXEL;
	else	return UNMASKED_PIXEL;
}


//リサイズ実行
template <class DATA>
bool	resize_data_unit(CPimpomAPI		*pAPI,					//(in)CPimpomAPIクラスオブジェクトへのポインタ
	int				target_data_number,		//(in)変換対象の画像メモリ番号
	int				dst_data_number,		//(in)変換後の画像を入れる画像メモリ番号
	bool			do_clear_dst,			//(in)変換前に画像メモリをクリアするかどうか
	DATA			*p_src,				//(in)変換対象の画像データ（target_data_numberで示す番号のデータ列戦闘ポインタ）
	CSize			dst_size,				//(in)変換後のサイズ
	bool			do_stretch,				//(in)変換後のサイズに合わせて画像を拡大/縮小するか(true:拡大する false:拡大しない)
	CPoint			offset,					//(in)オフセット量
	int				reduction_type			//(in)画像縮小のアルゴ（0:area-integration 1:max 2:min）
	)
{
	CDataUnit	*p_du, *p_new_dst_du;
	int			page, channel;
	CSize		src_size;
	DATA		*p_dst, *p_src_buffer;
	BYTE		*p_src_mask_buffer;
	CWorkArea	src_workarea;
	bool		do_use_mask;
	bool		do_use_workarea;


	//元データ取得
	if ((p_du = pAPI->GetDataUnit(target_data_number)) == NULL)	return false;

	src_size = p_du->DataSize;
	page = p_du->PageNumber;
	channel = p_du->ChannelNumber;



	//元データの一時退避バッファ確保
	p_src_buffer = new DATA[src_size.cx * src_size.cy * page * channel];//データ格納バッファ
	if (p_src_buffer == NULL)	return false;

	p_src_mask_buffer = new BYTE[src_size.cx * src_size.cy];//マスク格納バッファ
	if (p_src_mask_buffer == NULL){
		delete[]	p_src_buffer;
		return false;
	}



	//元データ退避＆リサイズ後のメモリ取得

	memcpy(p_src_mask_buffer, p_du->pMaskData, src_size.cx * src_size.cy);//マスク退避
	src_workarea.Copy(&p_du->WorkArea);//作業領域退避
	do_use_mask = p_du->DoUseMask;
	do_use_workarea = p_du->DoUseWorkArea;

	switch (p_du->DataType)
	{
	case BYTE_FORMAT:
		memcpy(p_src_buffer, p_du->pByteData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//データ退避
		p_dst = (DATA*)pAPI->GetByteMemory(dst_data_number, dst_size, do_clear_dst);//新メモリ確保
		break;

	case SHORT16_FORMAT:
		memcpy(p_src_buffer, p_du->pShortData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//データ退避
		p_dst = (DATA*)pAPI->GetShortMemory(dst_data_number, dst_size, do_clear_dst);//新メモリ確保
		break;

	case WORD_FORMAT:
		memcpy(p_src_buffer, p_du->pWordData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//データ退避
		p_dst = (DATA*)pAPI->GetWordMemory(dst_data_number, dst_size, do_clear_dst);//新メモリ確保
		break;

	case LONG32_FORMAT:
		memcpy(p_src_buffer, p_du->pLongData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//データ退避
		p_dst = (DATA*)pAPI->GetLongMemory(dst_data_number, dst_size, do_clear_dst);//新メモリ確保
		break;

	case DWORD_FORMAT:
		memcpy(p_src_buffer, p_du->pDwordData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//データ退避
		p_dst = (DATA*)pAPI->GetDwordMemory(dst_data_number, dst_size, do_clear_dst);//新メモリ確保
		break;

	case RGB_FORMAT:
		memcpy(p_src_buffer, p_du->pByteData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//データ退避
		p_dst = (DATA*)pAPI->GetRGBMemory(dst_data_number, dst_size, do_clear_dst);//新メモリ確保
		break;

	case FLOAT_FORMAT:
		memcpy(p_src_buffer, p_du->pFloatData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//データ退避
		p_dst = (DATA*)pAPI->GetFloatMemory(dst_data_number, dst_size, do_clear_dst);//新メモリ確保
		break;

	case XYZ_FORMAT:
		memcpy(p_src_buffer, p_du->pFloatData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//データ退避
		p_dst = (DATA*)pAPI->GetXYZMemory(dst_data_number, dst_size, do_clear_dst);//新メモリ確保
		break;

	case COMPLEX_FORMAT:
		memcpy(p_src_buffer, p_du->pFloatData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//データ退避
		p_dst = (DATA*)pAPI->GetComplexMemory(dst_data_number, dst_size, do_clear_dst);//新メモリ確保
		break;

	case THREE_D_FORMAT:
		memcpy(p_src_buffer, p_du->pByteData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//データ退避
		p_dst = (DATA*)pAPI->Get3DMemory(dst_data_number, dst_size, page, do_clear_dst);//新メモリ確保
		break;

	case FLOAT_3D_FORMAT:
		memcpy(p_src_buffer, p_du->pFloatData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//データ退避
		p_dst = (DATA*)pAPI->GetF3DMemory(dst_data_number, dst_size, page, do_clear_dst);//新メモリ確保
		break;

	case RGB_3D_FORMAT:
		memcpy(p_src_buffer, p_du->pByteData, src_size.cx * src_size.cy * page * channel * sizeof(DATA));//データ退避
		p_dst = (DATA*)pAPI->GetRGB3DMemory(dst_data_number, dst_size, page, do_clear_dst);//新メモリ確保
		break;
	}

	if (p_dst == NULL)
	{//新メモリ取得に失敗したらエラー終了
		delete[]	p_src_buffer;
		delete[]	p_src_mask_buffer;

		return false;
	}

	p_new_dst_du = pAPI->GetDataUnit(dst_data_number);//リサイズ後のデータ


	//データリサイズ実行
	if (!resize_image(p_src_buffer, p_dst, src_size, page, channel, dst_size, do_stretch, offset, reduction_type)){
		delete[]	p_src_buffer;
		delete[]	p_src_mask_buffer;

		return false;
	}

	//マスク・作業領域のリサイズ実行
	if (do_stretch)
	{
		//倍率
		double scale_x = (double)dst_size.cx / (double)src_size.cx;
		double scale_y = (double)dst_size.cy / (double)src_size.cy;


		//マスクリサイズ(bi-linear)
		for (int j = 0; j<dst_size.cy; j++)
		{
			for (int i = 0; i<dst_size.cx; i++)
			{
				float x = ((float)i - 0.5) / scale_x;
				float y = ((float)j - 0.5) / scale_y;

				if (x < 0 || y < 0){//四捨五入して負数になってしまう場合の対応
					*(p_new_dst_du->pMaskData + i + j*dst_size.cx) = UNMASKED_PIXEL;
				}
				else{
					*(p_new_dst_du->pMaskData + i + j*dst_size.cx) = sub_pix_mask_value(p_src_mask_buffer, src_size, x, y);
				}
			}
		}

		//作業領域リサイズ
		for (int n = 0; n < WORKAREA_FIG_NUM; n++) {
			src_workarea.Scale(n, scale_x, scale_y, CPoint(0, 0));
		}
		p_new_dst_du->WorkArea.Copy(&src_workarea);


		//delete[]	p_buffer;
	}
	else//変換前の縮尺のままコピー（はみ出し部分は切り取られる）
	{
		//作業領域コピー
		for (int n = 0; n < WORKAREA_FIG_NUM; n++) {
			src_workarea.Scale(n, 1, 1, offset);
		}
		p_new_dst_du->WorkArea.Copy(&src_workarea);
	}

	p_new_dst_du->DoUseMask = do_use_mask;//マスクの使用状態を復元
	p_new_dst_du->DoUseWorkArea = do_use_workarea;//作業領域の使用状態を復元


	delete[]	p_src_buffer;
	delete[]	p_src_mask_buffer;

	return true;
}



//パースペクティブ変換
template <class DATA>
bool	perspective_data(CPimpomAPI		*pAPI,				//(in)CPimpomAPIクラスオブジェクトへのポインタ
						int				target_data_number, //(in)変換対象の画像メモリ番号
						int				dst_data_number,	//(in)変換後の画像を入れる画像メモリ番号
						bool			do_clear_dst,		//(in)変換前に画像メモリをクリアするかどうか
						DATA			*p_src ,			//(in)変換対象の画像データ（target_data_numberで示す番号のデータ列戦闘ポインタ）
						double			p_param[8],			//(in)射影変換のパラメタ
						int				outRgnMode			//(in)画像からはみ出した部分の処理の方法(0:そのまま 1:最近傍画素の値で補間)
						)
{
	int i,j,k,c;
	int page, channel;
	CDataUnit	*p_du;
	CSize		src_size;
	DATA		*p_buffer, *p_dst;
	BYTE		*p_mask_buffer, *p_dst_mask;
	CWorkArea	src_workarea;
	bool		do_use_mask;
	bool		do_use_workarea;

	if( ( p_du = pAPI->GetDataUnit(target_data_number) ) == NULL )	return false;

	src_size = p_du->DataSize;
	page = p_du->PageNumber;
	channel = p_du->ChannelNumber;
	


	//現在のデータを一旦格納する
	p_buffer = new DATA[ src_size.cx * src_size.cy * page * channel ];
		if(p_buffer==NULL)	return false;

	p_mask_buffer = new BYTE[ src_size.cx * src_size.cy ];
	if(p_mask_buffer==NULL){
		delete[]	p_buffer;
		return false;
	}

	memcpy(p_buffer, p_src, src_size.cx * src_size.cy * page * channel * sizeof(DATA) );
	memcpy(p_mask_buffer, p_du->pMaskData, src_size.cx * src_size.cy );
	src_workarea.Copy(&p_du->WorkArea);//作業領域
	do_use_mask = p_du->DoUseMask;
	do_use_workarea = p_du->DoUseWorkArea;

	//メモリ取得
	switch(p_du->DataType)
	{
		case BYTE_FORMAT:
			p_dst = (DATA*)pAPI->GetByteMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case SHORT16_FORMAT:
			p_dst = (DATA*)pAPI->GetShortMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case WORD_FORMAT:
			p_dst = (DATA*)pAPI->GetWordMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case LONG32_FORMAT:
			p_dst = (DATA*)pAPI->GetLongMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case DWORD_FORMAT:
			p_dst = (DATA*)pAPI->GetDwordMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case RGB_FORMAT:
			p_dst = (DATA*)pAPI->GetRGBMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case FLOAT_FORMAT:
			p_dst = (DATA*)pAPI->GetFloatMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case XYZ_FORMAT:
			p_dst = (DATA*)pAPI->GetXYZMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case COMPLEX_FORMAT:
			p_dst = (DATA*)pAPI->GetComplexMemory(dst_data_number, src_size, do_clear_dst);
			break;

		case THREE_D_FORMAT:
			p_dst = (DATA*)pAPI->Get3DMemory(dst_data_number, src_size, page, do_clear_dst);
			break;

		case FLOAT_3D_FORMAT:
			p_dst = (DATA*)pAPI->GetF3DMemory(dst_data_number, src_size, page, do_clear_dst);
			break;

		case RGB_3D_FORMAT:
			p_dst = (DATA*)pAPI->GetRGB3DMemory(dst_data_number, src_size, page, do_clear_dst);
			break;
	}

	if(p_dst == NULL)
	{
		delete[]	p_buffer;
		delete[]	p_mask_buffer;
		return false;
	}
	p_dst_mask = pAPI->GetMaskMemory(dst_data_number);

	//変換実行
	double x,y,D,Nx,Ny;
	for(j=0 ; j<src_size.cy ; j++)
	{
		for(i=0 ; i<src_size.cx ; i++)
		{
			if(p_param[6]!=0 || p_param[7]!=0)//パースペクティブ変換
			{
				D = p_param[6]*i + p_param[7]*j + 1;
				if(D!=0){
					Nx = p_param[0]*i + p_param[1]*j + p_param[2];
					Ny = p_param[3]*i + p_param[4]*j + p_param[5];
					x = Nx / D;
					y = Ny / D;
				}else{
					x = y = -9999;
				}
			}
			else{//アフィン変換
					x = p_param[0]*i + p_param[1]*j + p_param[2];
					y = p_param[3]*i + p_param[4]*j + p_param[5];
			}

			if(x>=0 && y>=0 && x<src_size.cx-1 && y<src_size.cy-1)
			{
				if(!do_use_workarea || src_workarea.PtInWorkArea(CPoint((int)x, (int)y)))//作業領域内部
				{
					for(k=0 ; k<page ; k++){
						for(c=0 ; c<channel ; c++){
							*(p_dst + i + j*src_size.cx + (k*channel + c)*src_size.cx*src_size.cy) 
									= (DATA)sub_pix_value(p_buffer + (k*channel + c)*src_size.cx*src_size.cy, src_size, x, y);//データコピー
						}
					}

					*(p_dst_mask + i + j*src_size.cx) = sub_pix_mask_value(p_mask_buffer, src_size, x, y);//マスクコピー
				}
			}
			else if(outRgnMode==1)
			{
				//最近傍画素の指定
				if(x<0){
					x = 0;
				}
				else if (x >= src_size.cx - 1) {
					x = src_size.cx - 1;
				}

				if (y<0) {
					y = 0;
				}
				else if (y >= src_size.cy - 1) {
					y = src_size.cy - 1;
				}

				for(k=0 ; k<page ; k++){
					for(c=0 ; c<channel ; c++){
						*(p_dst + i + j*src_size.cx + (k*channel + c)*src_size.cx*src_size.cy)
							= (DATA)sub_pix_value(p_buffer + (k*channel + c)*src_size.cx*src_size.cy, src_size, x, y);//データコピー
					}
				}
			}
		}
	}


	delete[]	p_buffer;
	delete[]	p_mask_buffer;

	return true;
}




//クリッピング
template <class DATA>
bool	clip_data(CPimpomAPI *pAPI, int target_data_number, int dst_data_number, bool do_clear_dst,  DATA *p_src , CRect clip_rect)
{
	int i,j,k,c;
	int page, channel;
	CDataUnit	*p_du;
	CSize		src_size, dst_size;
	DATA		*p_buffer, *p_dst;
	BYTE		*p_mask_buffer, *p_dst_mask;


	if( ( p_du = pAPI->GetDataUnit(target_data_number) ) == NULL )	return false;

	src_size = p_du->DataSize;
	page = p_du->PageNumber;
	channel = p_du->ChannelNumber;

	clip_rect.NormalizeRect();
	dst_size = CSize(clip_rect.right - clip_rect.left + 1, clip_rect.bottom - clip_rect.top + 1);
	

	//現在のデータを一旦格納する
	p_buffer = new DATA[ src_size.cx * src_size.cy * page * channel ];//データ格納バッファ
	if(p_buffer==NULL)	return false;

	p_mask_buffer = new BYTE[ src_size.cx * src_size.cy ];//マスク格納バッファ
	if(p_mask_buffer==NULL){
		delete[]	p_buffer;
		return false;
	}

	memcpy(p_buffer, p_src, src_size.cx * src_size.cy * page * channel * sizeof(DATA) );//データ
	memcpy(p_mask_buffer, p_du->pMaskData, src_size.cx * src_size.cy );//マスク

		
	//変換後のメモリ取得
	switch(p_du->DataType)
	{
		case BYTE_FORMAT:
			p_dst = (DATA*)pAPI->GetByteMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case SHORT16_FORMAT:
			p_dst = (DATA*)pAPI->GetShortMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case WORD_FORMAT:
			p_dst = (DATA*)pAPI->GetWordMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case LONG32_FORMAT:
			p_dst = (DATA*)pAPI->GetLongMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case DWORD_FORMAT:
			p_dst = (DATA*)pAPI->GetDwordMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case RGB_FORMAT:
			p_dst = (DATA*)pAPI->GetRGBMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case FLOAT_FORMAT:
			p_dst = (DATA*)pAPI->GetFloatMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case XYZ_FORMAT:
			p_dst = (DATA*)pAPI->GetXYZMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case COMPLEX_FORMAT:
			p_dst = (DATA*)pAPI->GetComplexMemory(dst_data_number, dst_size, do_clear_dst);
			break;

		case THREE_D_FORMAT:
			p_dst = (DATA*)pAPI->Get3DMemory(dst_data_number, dst_size, page, do_clear_dst);
			break;

		case FLOAT_3D_FORMAT:
			p_dst = (DATA*)pAPI->GetF3DMemory(dst_data_number, dst_size, page, do_clear_dst);
			break;

		case RGB_3D_FORMAT:
			p_dst = (DATA*)pAPI->GetRGB3DMemory(dst_data_number, dst_size, page, do_clear_dst);
			break;
	}

	if(p_dst == NULL)
	{
		delete[]	p_buffer;
		delete[]	p_mask_buffer;
		return false;
	}
	p_dst_mask = pAPI->GetMaskMemory(dst_data_number);

	//クリップ実行
	int x,y;
	for(j=0 ; j<dst_size.cy ; j++)
	{
		for(i=0 ; i<dst_size.cx ; i++)
		{
			x = i + clip_rect.left;
			y = j + clip_rect.top;
			if(x>=0 && y>=0 && x<src_size.cx && j<src_size.cy)
			{
				for(k=0 ; k<page ; k++)
				{
					for(c=0 ; c<channel ; c++)
					{
						*(p_dst + i + j*dst_size.cx + (k*channel + c)*dst_size.cx*dst_size.cy) 
										= *(p_buffer + x + y*src_size.cx + (k*channel + c)*src_size.cx*src_size.cy) ;//データコピー
					}
				}
				*(p_dst_mask + i + j*dst_size.cx) = *(p_mask_buffer + x + y*src_size.cx) ;//マスクコピー
			}
		}
	}

	delete[]	p_buffer;
	delete[]	p_mask_buffer;

	return true;
}

/********************************************************************
機  能  名  称 : 画像データのサイズ変換
関    数    名 : ResizeDataUnit
引          数 : long			number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				CSize			new_size		(in)変換後のサイズ
				bool			do_stretch		(in)変換後のサイズに合わせて画像を拡大/縮小するか
				int				offset_x		(in)x方向オフセット量
				int				offset_y		(in)y方向オフセット量
				bool			show_message	(in)変換前に問い合わせダイアログを表示するか
				int				reduction_type,	(in)画像縮小のアルゴ（0:area-integration 1:max 2:min）
戻    り    値 : 成功すれば true
機          能 : 変換結果は、同画像メモリに上書きされる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::ResizeDataUnit(long number, CSize new_size, bool do_stretch, int offset_x, int offset_y, bool show_message, int reduction_type)
{
	return ResizeDataUnit(number,number,true,new_size,do_stretch,offset_x,offset_y,show_message,reduction_type);
}

/********************************************************************
機  能  名  称 : 画像データのサイズ変換
関    数    名 : ResizeDataUnit
引          数 : long			number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long			dst_data_number	(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
				 bool			do_clear_dst	(in)格納先画像メモリを一度クリアするかどうか(true:クリアする false:そのまま )
				CSize			new_size		(in)変換後のサイズ
				bool			do_stretch		(in)変換後のサイズに合わせて画像を拡大/縮小するか(true:拡大する false:拡大しない)
				int				offset_x		(in)x方向オフセット量
				int				offset_y		(in)y方向オフセット量
				bool			show_message	(in)変換前に問い合わせダイアログを表示するか
				int				reduction_type,	(in)画像縮小のアルゴ（0:area-integration 1:max 2:min）
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::ResizeDataUnit(long number, long dst_data_number, bool do_clear_dst, 
								   CSize new_size, bool do_stretch, int offset_x, int offset_y, bool show_message, int reduction_type)
{
	bool	ret;
	CDataUnit	*p_du, *p_du_after;
	CString		data_name;
	float		bright_max, bright_min;
	int			disp_page, disp_channel;


	//引数チェック
	if(new_size.cx<1 || new_size.cy<1)	return false;

	if( (p_du = GetDataUnit(number) )==NULL)	return false;

	if(dst_data_number==CURRENT_IMAGE){
		dst_data_number = GetCurrentImageNumber();//現在選択中の画像を指定する
	}
	else if(dst_data_number<0 || dst_data_number>= GetDataUnitNumber())
	{
		return false;
	}


	data_name = p_du->DataName;
	bright_max = p_du->BrightMax;
	bright_min = p_du->BrightMin;
	disp_page = p_du->DispPage;
	disp_channel = p_du->DispChannel;

	if(show_message){
		if( GetDataUnit(dst_data_number) ){
			CString str;
			str.Format("画像メモリ%dは上書きされます",dst_data_number);
			if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONINFORMATION ) != IDOK ){
				return false;
			}
		}
	}


	//変換実行
	ret = false;
	switch(p_du->DataType)
	{
		case BYTE_FORMAT:
			//not break
		case THREE_D_FORMAT:
			//not break
		case RGB_FORMAT:
			//not break
		case RGB_3D_FORMAT:
			ret= resize_data_unit(this, number, dst_data_number, do_clear_dst, p_du->pByteData, new_size, do_stretch, CPoint(offset_x,offset_y),reduction_type);
			break;

		case SHORT16_FORMAT:
			ret = resize_data_unit(this, number, dst_data_number, do_clear_dst, p_du->pShortData, new_size, do_stretch, CPoint(offset_x, offset_y), reduction_type);
			break;

		case WORD_FORMAT:
			ret = resize_data_unit(this, number, dst_data_number, do_clear_dst, p_du->pWordData, new_size, do_stretch, CPoint(offset_x, offset_y), reduction_type);
			break;

		case LONG32_FORMAT:
			ret = resize_data_unit(this, number, dst_data_number, do_clear_dst, p_du->pLongData, new_size, do_stretch, CPoint(offset_x, offset_y), reduction_type);
			break;

		case DWORD_FORMAT:
			ret = resize_data_unit(this, number, dst_data_number, do_clear_dst, p_du->pDwordData, new_size, do_stretch, CPoint(offset_x, offset_y), reduction_type);
			break;

		case FLOAT_FORMAT:
			//not break
		case COMPLEX_FORMAT:
			//not break
		case XYZ_FORMAT:
			//not break
		case XY_FORMAT:
			//not break
		case FLOAT_3D_FORMAT:
			ret = resize_data_unit(this, number, dst_data_number, do_clear_dst, p_du->pFloatData, new_size, do_stretch, CPoint(offset_x, offset_y), reduction_type);
			break;
	}
	if(!ret)	return false;

	//表示などを元に戻す
	p_du_after = GetDataUnit(dst_data_number);
	if(!p_du_after)		return false;
	
	p_du_after->DataName = data_name;
	p_du_after->BrightMax = bright_max;
	p_du_after->BrightMin = bright_min;
	p_du_after->DispPage = disp_page;
	p_du_after->DispChannel = disp_channel;

	return true;
}

/********************************************************************
機  能  名  称 : 画像データのパースペクティブ変換
関    数    名 : PerspectiveTransDataUnit
引          数 : long			number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				double			p_param[8]		(in)変換パラメタ（ホモグラフィ行列）
				bool			show_message	(in)変換前に問い合わせダイアログを表示するか
				int				outRgnMode		(in)画像からはみ出した部分の処理の方法(0:そのまま 1:最近傍画素の値で補間)
戻    り    値 : 成功すれば true
機          能 : 変換結果は、同画像メモリに上書きされる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::PerspectiveTransDataUnit(long number, double p_param[8], bool show_message, int outRgnMode)
{
	return PerspectiveTransDataUnit(number,number,true,p_param,show_message, outRgnMode);
}

/********************************************************************
機  能  名  称 : 画像データのパースペクティブ変換
関    数    名 : PerspectiveTransDataUnit
引          数 : long			number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long			dst_data_number	(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
				 bool			do_clear_dst	(in)格納先画像メモリを一度クリアするかどうか
				double			p_param[8]		(in)変換パラメタ（ホモグラフィ行列）
				bool			show_message	(in)変換前に問い合わせダイアログを表示するか
				int				outRgnMode		(in)画像からはみ出した部分の処理の方法(0:そのまま 1:最近傍画素の値で補間)
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::PerspectiveTransDataUnit(long number, long dst_data_number, bool do_clear_dst, double p_param[8], bool show_message, int outRgnMode)
{
	bool	ret;
	CDataUnit	*p_du, *p_du_after;
	CString		data_name;
	float		bright_max, bright_min;
	int			disp_page, disp_channel;


	if( (p_du = GetDataUnit(number) )==NULL)	return false;

	if(dst_data_number==CURRENT_IMAGE){
		dst_data_number = GetCurrentImageNumber();//現在選択中の画像を指定する
	}
	else if(dst_data_number<0 || dst_data_number>= GetDataUnitNumber())
	{
		return false;
	}


	data_name = p_du->DataName;
	bright_max = p_du->BrightMax;
	bright_min = p_du->BrightMin;
	disp_page = p_du->DispPage;
	disp_channel = p_du->DispChannel;


	if(show_message){
		if( GetDataUnit(dst_data_number) ){
			CString str;
			str.Format("画像メモリ%dは上書きされます",dst_data_number);
			if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONINFORMATION ) != IDOK ){
				return false;
			}
		}
	}


	//変換実行
	ret = false;
	switch(p_du->DataType)
	{
		case BYTE_FORMAT:
			//not break
		case THREE_D_FORMAT:
			//not break
		case RGB_FORMAT:
			//not break
		case RGB_3D_FORMAT:
			ret= perspective_data(this, number, dst_data_number, do_clear_dst, p_du->pByteData, p_param, outRgnMode);
			break;

		case SHORT16_FORMAT:
			ret= perspective_data(this, number, dst_data_number, do_clear_dst, p_du->pShortData, p_param, outRgnMode);
			break;

		case WORD_FORMAT:
			ret= perspective_data(this, number, dst_data_number, do_clear_dst, p_du->pWordData, p_param, outRgnMode);
			break;

		case LONG32_FORMAT:
			ret= perspective_data(this, number, dst_data_number, do_clear_dst, p_du->pLongData, p_param, outRgnMode);
			break;

		case DWORD_FORMAT:
			ret= perspective_data(this, number, dst_data_number, do_clear_dst, p_du->pDwordData, p_param, outRgnMode);
			break;

		case FLOAT_FORMAT:
			//not break
		case COMPLEX_FORMAT:
			//not break
		case XYZ_FORMAT:
			//not break
		case XY_FORMAT:
			//not break
		case FLOAT_3D_FORMAT:
			ret= perspective_data(this, number, dst_data_number, do_clear_dst, p_du->pFloatData, p_param, outRgnMode);
			break;
	}
	if(!ret)	return false;

	//表示などを元に戻す
	p_du_after = GetDataUnit(dst_data_number);
	if(!p_du_after)		return false;
	
	p_du_after->DataName = data_name;
	p_du_after->BrightMax = bright_max;
	p_du_after->BrightMin = bright_min;
	p_du_after->DispPage = disp_page;
	p_du_after->DispChannel = disp_channel;
	
	return true;
}

/********************************************************************
機  能  名  称 : 画像データのパースペクティブ変換
関    数    名 : PerspectiveTransDataUnit
引          数 : long			number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				double			ti,tj 			(in)画像内平行移動
				double			tX,tY 			(in)撮像面内平行移動
				double			theta 			(in)撮像面内回転 [deg]
				double			phai  			(in)カメラ横方向傾斜[deg]
				double			psi   			(in)カメラ縦方向傾斜[deg]
				double			fl_phisical		(in)レンズ焦点距離[mm]
				double			ccd_pix_size	(in)ccd1画素サイズ[μm]
				bool			show_message	(in)変換後にメッセージを表示するか
				double			*perspective_param	(out)変換パラメタ（ホモグラフィ行列）
				double			magnify			(in)拡大率
				int				outRgnMode		(in)画像からはみ出した部分の処理の方法(0:そのまま 1:最近傍画素の値で補間)
戻    り    値 : 成功すれば true
機          能 : 変換結果は、同画像メモリに上書きされる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::PerspectiveTransDataUnit(long number,
											 double ti, double tj, double tX, double tY, 
											 double theta, double phai, double psi, 
											 double fl_phisical, double ccd_pix_size, bool show_message, double *perspective_param, double magnify,int outRgnMode)
{
	return PerspectiveTransDataUnit(number,number,true,ti,tj,tX,tY,theta,phai,psi,fl_phisical,ccd_pix_size,show_message, perspective_param, magnify, outRgnMode);
}

/********************************************************************
機  能  名  称 : 画像データのパースペクティブ変換
関    数    名 : PerspectiveTransDataUnit
引          数 : long			number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long			dst_data_number	(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
				 bool			do_clear_dst	(in)格納先画像メモリを一度クリアするかどうか
				double			ti,tj 			(in)画像内平行移動
				double			tX,tY 			(in)撮像面内平行移動
				double			theta 			(in)撮像面内回転 [deg]
				double			phai  			(in)カメラ横方向傾斜[deg]
				double			psi   			(in)カメラ縦方向傾斜[deg]
				double			fl_phisical		(in)レンズ焦点距離[mm]
				double			ccd_pix_size	(in)ccd1画素サイズ[μm]
				bool			show_message	(in)変換後にメッセージを表示するか
				double			*perspective_param	(out)変換パラメタ（ホモグラフィ行列）
				double			magnify			(in)拡大率
				int				outRgnMode		(in)画像からはみ出した部分の処理の方法(0:そのまま 1:最近傍画素の値で補間)
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
2015.11.11   Y.Ikeda         バグ修正
2016.05.02   Y.Ikeda         拡大率のパラメタを追加
********************************************************************/
bool	CPimpomAPI::PerspectiveTransDataUnit(long number, long dst_data_number, bool do_clear_dst,
	double ti, double tj, double tX, double tY,
	double theta, double phai, double psi,
	double fl_phisical, double ccd_pix_size, bool show_message, double *perspective_param, double magnify, int outRgnMode)
{
	CDataUnit	*p_du;
	if ((p_du = GetDataUnit(number)) == NULL)	return false;

	double fl;
	double p_param[8];
	double theta_rad, phai_rad, psi_rad;
	theta_rad = theta / 180 * PI;
	phai_rad = phai / 180 * PI;
	psi_rad = psi / 180 * PI;

	fl = fl_phisical / (ccd_pix_size / 1000 * sqrt(2.0));//焦点距離


														 //レンズ中心座標 [C]
	double  C[3];
	C[0] = 0;
	C[1] = 0;
	C[2] = fl;


	//移動前（垂直視）カメラパラメタ作成***********************

	//内部パラメタ [A0]
	cv::Mat A0(3, 3, CV_64F);
	A0.at<double>(0, 0) = fl*magnify;		A0.at<double>(0, 1) = 0;				A0.at<double>(0, 2) = (double)p_du->DataSize.cx / 2.0 + ti;
	A0.at<double>(1, 0) = 0;				A0.at<double>(1, 1) = fl*magnify;		A0.at<double>(1, 2) = (double)p_du->DataSize.cy / 2.0 + tj;
	A0.at<double>(2, 0) = 0;				A0.at<double>(2, 1) = 0;				A0.at<double>(2, 2) = 1.;




	//外部パラメタ  [RT0] = [ E | C ]
	cv::Mat RT0(3, 4, CV_64F);
	RT0.at<double>(0, 0) = 1;	RT0.at<double>(0, 1) = 0;	RT0.at<double>(0, 2) = 0;	RT0.at<double>(0, 3) = C[0];
	RT0.at<double>(1, 0) = 0;	RT0.at<double>(1, 1) = 1;	RT0.at<double>(1, 2) = 0;	RT0.at<double>(1, 3) = C[1];
	RT0.at<double>(2, 0) = 0;	RT0.at<double>(2, 1) = 0;	RT0.at<double>(2, 2) = 1;	RT0.at<double>(2, 3) = C[2];

	//プロジェクション行列 [P0]=[A0][RT0]
	cv::Mat P0 = A0*RT0;


	//移動後カメラパラメタ作成***********************

	//内部パラメタ [A]
	cv::Mat A(3, 3, CV_64F);
	A.at<double>(0, 0) = fl;	A.at<double>(0, 1) = 0;		A.at<double>(0, 2) = (double)p_du->DataSize.cx / 2.0 + tX;
	A.at<double>(1, 0) = 0;		A.at<double>(1, 1) = fl;	A.at<double>(1, 2) = (double)p_du->DataSize.cy / 2.0 + tY;
	A.at<double>(2, 0) = 0;		A.at<double>(2, 1) = 0;		A.at<double>(2, 2) = 1.0;

	//回転行列 [R]=[R1][R2][R3]
	cv::Mat R1(3, 3, CV_64F);
	R1.at<double>(0, 0) = cos(theta_rad);	R1.at<double>(0, 1) = -sin(theta_rad);	R1.at<double>(0, 2) = 0;
	R1.at<double>(1, 0) = sin(theta_rad);	R1.at<double>(1, 1) = cos(theta_rad);	R1.at<double>(1, 2) = 0;
	R1.at<double>(2, 0) = 0;				R1.at<double>(2, 1) = 0;				R1.at<double>(2, 2) = 1.0;

	cv::Mat R2(3, 3, CV_64F);
	R2.at<double>(0, 0) = cos(phai_rad);	R2.at<double>(0, 1) = 0;				R2.at<double>(0, 2) = -sin(phai_rad);
	R2.at<double>(1, 0) = 0;				R2.at<double>(1, 1) = 1;				R2.at<double>(1, 2) = 0;
	R2.at<double>(2, 0) = sin(phai_rad);	R2.at<double>(2, 1) = 0;				R2.at<double>(2, 2) = cos(phai_rad);

	cv::Mat R3(3, 3, CV_64F);
	R3.at<double>(0, 0) = 1;				R3.at<double>(0, 1) = 0;				R3.at<double>(0, 2) = 0;
	R3.at<double>(1, 0) = 0;				R3.at<double>(1, 1) = cos(psi_rad); 	R3.at<double>(1, 2) = -sin(psi_rad);
	R3.at<double>(2, 0) = 0;				R3.at<double>(2, 1) = sin(psi_rad);		R3.at<double>(2, 2) = cos(psi_rad);

	cv::Mat TMP1 = R1*R2;
	cv::Mat R = TMP1*R3;


	//外部パラメタ [RT] = [R | C]
	cv::Mat RT(3, 4, CV_64F);
	RT.at<double>(0, 0) = R.at<double>(0, 0);		RT.at<double>(0, 1) = R.at<double>(0, 1);		RT.at<double>(0, 2) = R.at<double>(0, 2);		RT.at<double>(0, 3) = C[0];
	RT.at<double>(1, 0) = R.at<double>(1, 0);		RT.at<double>(1, 1) = R.at<double>(1, 1);		RT.at<double>(1, 2) = R.at<double>(1, 2);		RT.at<double>(1, 3) = C[1];
	RT.at<double>(2, 0) = R.at<double>(2, 0);		RT.at<double>(2, 1) = R.at<double>(2, 1);		RT.at<double>(2, 2) = R.at<double>(2, 2);		RT.at<double>(2, 3) = C[2];

	//プロジェクション行列 [P] = [A][RT]
	cv::Mat P = A*RT;


	//ホモグラフィ作成***********************
	cv::Mat P_T = P.t();
	cv::Mat P_ = P_T*((P*P.t()).inv(cv::DECOMP_SVD));//移動後カメラプロジェクション行列の一般化逆行列 [P_] = [P_T] [ [P][P_T] ]-1
	cv::Mat H = (P0*P_).inv(cv::DECOMP_SVD);//ホモグラフィ行列


	double den = H.at<double>(2, 2);
	p_param[0] = H.at<double>(0, 0) / den;
	p_param[1] = H.at<double>(0, 1) / den;
	p_param[2] = H.at<double>(0, 2) / den;
	p_param[3] = H.at<double>(1, 0) / den;
	p_param[4] = H.at<double>(1, 1) / den;
	p_param[5] = H.at<double>(1, 2) / den;
	p_param[6] = H.at<double>(2, 0) / den;
	p_param[7] = H.at<double>(2, 1) / den;


	if (perspective_param) {
		memcpy(perspective_param, p_param, 8 * sizeof(double));
	}


	return PerspectiveTransDataUnit(number, dst_data_number, do_clear_dst, p_param, show_message, outRgnMode);
}


/********************************************************************
機  能  名  称 : 画像データの等方変換(回転、平行移動、拡大)
関    数    名 : IsoTarnsDataUnit
引          数 : long			number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				double			ti, tj			(in)平行移動
				double			theta   		(in)回転移動[deg]
				double			magnify 		(in)拡大率
				bool			show_message	(in)変換前に問い合わせダイアログを表示するか
				double			magnify			(in)拡大率
				int				outRgnMode		(in)画像からはみ出した部分の処理の方法(0:そのまま 1:最近傍画素の値で補間)
戻    り    値 : 成功すれば true
機          能 : 変換結果は、同画像メモリに上書きされる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::IsoTransDataUnit(long number, double ti, double tj, double theta, double magnify, bool show_message, double *perspective_param, int outRgnMode)
{
	if(magnify==0)	return false;

	return PerspectiveTransDataUnit(number, ti, tj, 0,0, theta, 0,0, 16, 7.4 ,show_message, perspective_param, magnify, outRgnMode);
}

/********************************************************************
機  能  名  称 : 画像データの等方変換(回転、平行移動、拡大)
関    数    名 : IsoTarnsDataUnit
引          数 : long			number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long			dst_data_number	(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
				 bool			do_clear_dst	(in)格納先画像メモリを一度クリアするかどうか
				double			ti, tj			(in)平行移動
				double			theta   		(in)回転移動[deg]
				double			magnify 		(in)拡大率
				bool			show_message	(in)変換前に問い合わせダイアログを表示するか
				int				outRgnMode		(in)画像からはみ出した部分の処理の方法(0:そのまま 1:最近傍画素の値で補間)
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::IsoTransDataUnit(long number, long dst_data_number, bool do_clear_dst, 
									 double ti, double tj, double theta, double magnify, bool show_message, double *perspective_param,int outRgnMode)
{
	return PerspectiveTransDataUnit(number, dst_data_number, do_clear_dst, ti, tj, 0,0, theta, 0,0, 16, 7.4,show_message, perspective_param, magnify, outRgnMode);


}

/********************************************************************
機  能  名  称 : //パースペクティブ座標変換
関    数    名 : PerspectiveTransCoor
引          数 : double			p_param[8]		(in)変換パラメタ（ホモグラフィ行列）
				 double			srcX			(in)変換前座標
				 double			srcY			(in)変換前座標
				 double			pDstX			(out)変換後座標
				 double			pDstY			(out)変換後座標
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::PerspectiveTransCoor(double perspective_param[8], double srcX, double srcY, double *pDstX, double *pDstY)
{

	double p_param[8] = { 0 };

	cv::Mat H(3, 3, CV_64F);
	H.at<double>(0, 0) = perspective_param[0];	H.at<double>(0, 1) = perspective_param[1];	H.at<double>(0, 2) = perspective_param[2];
	H.at<double>(1, 0) = perspective_param[3];	H.at<double>(1, 1) = perspective_param[4];	H.at<double>(1, 2) = perspective_param[5];
	H.at<double>(2, 0) = perspective_param[6];	H.at<double>(2, 1) = perspective_param[7];	H.at<double>(2, 2) = 1.0;

	cv::Mat H_ = H.inv(cv::DECOMP_SVD);//逆行列

	double den = H_.at<double>(2, 2);
	p_param[0] = H_.at<double>(0, 0) / den;
	p_param[1] = H_.at<double>(0, 1) / den;
	p_param[2] = H_.at<double>(0, 2) / den;
	p_param[3] = H_.at<double>(1, 0) / den;
	p_param[4] = H_.at<double>(1, 1) / den;
	p_param[5] = H_.at<double>(1, 2) / den;
	p_param[6] = H_.at<double>(2, 0) / den;
	p_param[7] = H_.at<double>(2, 1) / den;


	double denom = p_param[6] * srcX + p_param[7] * srcY + 1.0;
	*pDstX = (p_param[0] * srcX + p_param[1] * srcY + p_param[2]) / denom;
	*pDstY = (p_param[3] * srcX + p_param[4] * srcY + p_param[5]) / denom;


	return true;
}


/********************************************************************
機  能  名  称 : 画像データのクリッピング
関    数    名 : ClipDataUnit
引          数 : long			number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				CRect			clip_rect		(in)クリッピング範囲
				bool			show_message	(in)変換前に問い合わせダイアログを表示するか
戻    り    値 : 成功すれば true
機          能 : 変換結果は、同画像メモリに上書きされる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::ClipDataUnit(long number, CRect clip_rect, bool show_message)
{
	return ClipDataUnit(number, number, true, clip_rect, show_message);
}

/********************************************************************
機  能  名  称 : 画像データのクリッピング
関    数    名 : ResizeDataUnit
引          数 : long			number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 long			dst_data_number	(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
				 bool			do_clear_dst	(in)格納先画像メモリを一度クリアするかどうか
				CRect			clip_rect		(in)クリッピング範囲
				bool			show_message	(in)変換前に問い合わせダイアログを表示するか
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::ClipDataUnit(long number, long dst_data_number, bool do_clear_dst, CRect clip_rect, bool show_message)
{
	bool	ret;
	CDataUnit	*p_du, *p_du_after;
	CString		data_name;
	float		bright_max, bright_min;
	int			disp_page, disp_channel;
	bool		use_workarea;
	CWorkArea	work_area;


	if( (p_du = GetDataUnit(number) )==NULL)	return false;

	if(dst_data_number==CURRENT_IMAGE){
		dst_data_number = GetCurrentImageNumber();//現在選択中の画像を指定する
	}
	else if(dst_data_number<0 || dst_data_number>= GetDataUnitNumber())
	{
		return false;
	}


	data_name = p_du->DataName;
	bright_max = p_du->BrightMax;
	bright_min = p_du->BrightMin;
	disp_page = p_du->DispPage;
	disp_channel = p_du->DispChannel;
	work_area.Copy(&p_du->WorkArea);
	use_workarea = p_du->DoUseWorkArea;

	if(show_message){
		if( GetDataUnit(dst_data_number) ){
			CString str;
			str.Format("画像メモリ%dは上書きされます",dst_data_number);
			if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONINFORMATION ) != IDOK ){
				return false;
			}
		}
	}


	//変換実行
	ret=false;
	switch(p_du->DataType)
	{
		case BYTE_FORMAT:
			//not break
		case THREE_D_FORMAT:
			//not break
		case RGB_FORMAT:
			//not break
		case RGB_3D_FORMAT:
			ret= clip_data(this, number, dst_data_number, do_clear_dst, p_du->pByteData, clip_rect);
			break;

		case SHORT16_FORMAT:
			ret= clip_data(this, number, dst_data_number, do_clear_dst, p_du->pShortData, clip_rect);
			break;

		case WORD_FORMAT:
			ret= clip_data(this, number, dst_data_number, do_clear_dst, p_du->pWordData, clip_rect);
			break;

		case LONG32_FORMAT:
			ret= clip_data(this, number, dst_data_number, do_clear_dst, p_du->pLongData, clip_rect);
			break;

		case DWORD_FORMAT:
			ret= clip_data(this, number, dst_data_number, do_clear_dst, p_du->pDwordData, clip_rect);
			break;

		case FLOAT_FORMAT:
			//not break
		case COMPLEX_FORMAT:
			//not break
		case XYZ_FORMAT:
			//not break
		case XY_FORMAT:
			//not break
		case FLOAT_3D_FORMAT:
			ret= clip_data(this, number, dst_data_number, do_clear_dst, p_du->pFloatData, clip_rect);
			break;
	}
	if(!ret)	return false;

	//表示などを元に戻す
	p_du_after = GetDataUnit(dst_data_number);
	if(!p_du_after)		return false;
	
	p_du_after->DataName = data_name;
	p_du_after->BrightMax = bright_max;
	p_du_after->BrightMin = bright_min;
	p_du_after->DispPage = disp_page;
	p_du_after->DispChannel = disp_channel;

	//作業領域をコピー
	work_area.Move(-clip_rect.left, -clip_rect.top);
	p_du_after->WorkArea.Copy(&work_area);
	p_du_after->DoUseWorkArea = use_workarea;
	
	return true;
}