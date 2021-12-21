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
//2値化
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "filter\template\filter_gauss.h"
#include "filter\template\filter_log.h"
#include "opencv_headers.h"

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



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//テンプレートで型なし(全ての型の画像に対応する)関数をつくる
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//グローバル閾値による2値化
template <class DATA, class DATA2>
bool	binalize_global(
				 DATA			*p_src_data,	//(in)入力画像配列の先頭ポインタ
				 DATA2			*p_dst_data,	//(out)出力画像配列の先頭ポインタ
				 CSize			image_size,		//(in)画像サイズ
				 double			min_thresh,		//(in)最小閾値
				 double			max_thresh,		//(in)最大閾値
				 bool			inverse,		//(in)２値反転(true)
				 double			fg_val,			//(in)前景値
				 double			bg_val,			//(in)背景値
				 CRect			work_area		//(in)処理領域
				 )
{
	double fg, bg;

	if(!inverse){
		fg = fg_val;
		bg = bg_val;
	}else{
		fg = bg_val;
		bg = fg_val;
	}

	for(int j = work_area.top ; j<=work_area.bottom ; j++)
	{
		for(int i = work_area.left ; i<=work_area.right ; i++)
		{
			double val = *(p_src_data + i + j*image_size.cx);

			if(val >= min_thresh && val<= max_thresh){
				*(p_dst_data + i + j*image_size.cx) = fg;
			}else{
				*(p_dst_data + i + j*image_size.cx) = bg;
			}
		}
	}

	return true;
}


//ガウシアン自己差分による2値化
template <class DATA, class DATA2>
bool	binalize_gauss(
				 DATA			*p_src_data,	//(in)入力画像配列の先頭ポインタ
				 DATA2			*p_dst_data,	//(out)出力画像配列の先頭ポインタ
				 CSize			image_size,		//(in)画像サイズ
				 int			filter_size,	//(in)
				 int			offset,			//(in)
				 int			inverse,		//(in)２値反転(true)
				 double			fg_val,			//(in)前景値
				 double			bg_val,			//(in)背景値
				 CRect			work_area,		//(in)処理領域
				 int			pad_type		//(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
				 )
{
	bool ret=true;

	float *p_buffer = new float[image_size.cx * image_size.cy ];//バッファメモリ確保
	if(!p_buffer)	return false;

	memset(p_buffer, 0, image_size.cx * image_size.cy * sizeof(float));//バッファ初期化

	ret = filter_gauss_unshade(p_src_data, p_buffer, image_size, filter_size, -1, offset, 1, work_area, FLT_MAX, -FLT_MAX, pad_type,NULL);//ガウシアンフィルタをかける

	if(ret){
		binalize_global(p_buffer, p_dst_data, image_size, 0 ,FLT_MAX, inverse, fg_val, bg_val, work_area);
	}

	delete p_buffer;

	return ret;
}



//ラプラシアンオフガウシアンによる2値化
template <class DATA, class DATA2>
bool	binalize_laplace_gauss(
				 DATA			*p_src_data,	//(in)入力画像配列の先頭ポインタ
				 DATA2			*p_dst_data,	//(out)出力画像配列の先頭ポインタ
				 CSize			image_size,		//(in)画像サイズ
				 int			filter_size,	//(in)
				 int			offset,			//(in)
				 int			inverse,		//(in)２値反転(true)
				 double			fg_val,			//(in)前景値
				 double			bg_val,			//(in)背景値
				 CRect			work_area,		//(in)処理領域
				 int			pad_type		//(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
				 )
{
	bool ret=true;

	float *p_buffer = new float[image_size.cx * image_size.cy ];//バッファメモリ確保
	if(!p_buffer)	return false;

	memset(p_buffer, 0, image_size.cx * image_size.cy * sizeof(float));//バッファ初期化

	ret = filter_lapcace_gauss(p_src_data, p_buffer, image_size, filter_size, -1, work_area, 0,1, FLT_MAX, -FLT_MAX , pad_type, NULL);//ガウシアンフィルタをかける

	if(ret){
		binalize_global(p_buffer, p_dst_data, image_size, offset ,FLT_MAX, inverse, fg_val, bg_val, work_area);
	}

	delete[] p_buffer;

	return ret;
}


/********************************************************************
機  能  名  称 : 画像を2値化してべつの画像メモリに入れる
関    数    名 : Binalize
引          数 : int			src_number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 int			dst_number			(in)2値化後の画像を入れるメモリ
				 double			 min_thresh			(in)最小閾値
				 double			 max_thresh			(in)最大閾値
				 bool			inverse				(in)２値反転(true)
				 BYTE			fg_val				(in)前景値
				 BYTE			bg_val				(in)背景値
				 bool			show_message		(in)変換前に問い合わせダイアログを表示するか

戻    り    値 : 成功するとtrue
機          能 : 新しくBYTE型メモリを取得し、閾値以内の画素をon_label/それ以外をoff_labelにする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::Binalize(long src_number, long dst_number, double min_thresh, double max_thresh, bool inverse, BYTE fg_val, BYTE bg_val, bool show_message, bool toMultiImage)
{
	bool ret=true;
	CDataUnit	*p_du=NULL, *p_dst_du=NULL;
	CRect		calc_area;


	//引数チェック
	if( (p_du = GetDataUnit(src_number))==NULL)	return false;

	if(dst_number >= GetDataUnitNumber() || dst_number<0)	return false;

	if(src_number == dst_number)	return false;//同じ番号の指定はできない

	if(GetDataUnit(dst_number)){//結果の格納先として指定された画像メモリに、すでに別のデータが入っている場合は問い合わせ
		if(show_message){
			CString str;
			str.Format("画像メモリ%dは上書きされます",dst_number);
			if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONINFORMATION ) != IDOK ){
				return false;
			}
		}
	}


	
	int pageStart, pageEnd, channelStart, channelEnd, dstPageNum;
	if (toMultiImage)//複数ページ・チャネル対応
	{
		pageStart = 0;
		pageEnd = p_du->PageNumber-1;
		channelStart = 0;
		channelEnd = p_du->ChannelNumber-1;
		dstPageNum = p_du->PageNumber * p_du->ChannelNumber;
	}
	else{
		pageStart = pageEnd = p_du->DispPage;
		channelStart = channelEnd = p_du->DispChannel;
		dstPageNum = 1;
	}



	if (Get3DMemory(dst_number, p_du->DataSize, dstPageNum, true) == NULL)	return false;//新しくBYTE型メモリを取得

	CopyDataUnit(src_number, dst_number, false, false, true, false, true,true);//表示位置・マスク・処理領域・マークを受け継ぐ

	p_dst_du = GetDataUnit(dst_number);



	int imsize = p_du->DataSize.cx * p_du->DataSize.cy;//画像のサイズ（画素数）
	calc_area = p_du->GetRectArea();//処理対象の矩形範囲


	if (p_du->DispRGB == false || (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) || toMultiImage){//グレー画像に対する処理

		int dstPageCnt = 0;
		for (int page = pageStart; page <= pageEnd; page++)
		{
			for (int channel = channelStart; channel <= channelEnd; channel++)
			{
				if (p_du->pByteData){	//8bit画像
					ret = binalize_global(
						p_du->pByteData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, min_thresh, max_thresh, inverse, fg_val, bg_val, calc_area);

				}
				else if (p_du->pShortData){	//16bit画像
					ret = binalize_global(
						p_du->pShortData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, min_thresh, max_thresh, inverse, fg_val, bg_val, calc_area);

				}
				else if (p_du->pWordData){	//16bit画像
					ret = binalize_global(
						p_du->pDwordData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, min_thresh, max_thresh, inverse, fg_val, bg_val, calc_area);

				}
				else if (p_du->pLongData){	//32bit画像
					ret = binalize_global(
						p_du->pLongData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, min_thresh, max_thresh, inverse, fg_val, bg_val, calc_area);

				}
				else if (p_du->pDwordData){	//32bit画像
					ret = binalize_global(
						p_du->pDwordData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, min_thresh, max_thresh, inverse, fg_val, bg_val, calc_area);

				}
				else if (p_du->pFloatData){//浮動少数点
					ret = binalize_global(
						p_du->pFloatData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, min_thresh, max_thresh, inverse, fg_val, bg_val, calc_area);

				}

				dstPageCnt++;
			}
		}

		if (toMultiImage){//複数ページ・チャネル対応
			p_dst_du->DispPage = p_du->DispPage;
			p_dst_du->DispChannel = p_du->DispChannel;
		}

	}else{//RGB画像に対する処理
		unsigned char*pBrightness = new unsigned char[imsize];
		if(pBrightness==NULL)	return false;

		//いったん明度値のみの画像にする
		for(int j=calc_area.top; j<=calc_area.bottom ; j++){
			for(int i=calc_area.left ; i<=calc_area.right ; i++){
				*(pBrightness + i + j*p_du->DataSize.cx) = p_du->GetDataValue(CPoint(i,j),-1,-1);
			}
		}

		ret = binalize_global(
				pBrightness,
				p_dst_du->pByteData,
				p_du->DataSize, min_thresh, max_thresh, inverse, fg_val, bg_val, calc_area);

		delete[]	pBrightness;
	}


	return ret;
}

/********************************************************************
機  能  名  称 : 画像を2値化してマスクに入れる
関    数    名 : Binalize
引          数 : int			src_number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 double			 min_thresh			(in)最小閾値
				 double			 max_thresh			(in)最大閾値
				 bool			inverse				(in)２値反転
戻    り    値 : 成功するとtrue
機          能 : 閾値以内の画素のマスクをOn/それ以外をOffにする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::Binalize(long src_number, double min_thresh, double max_thresh, bool inverse)
{
	CDataUnit	*p_du=NULL;
	CRect		calc_area;

	if( (p_du = GetDataUnit(src_number))==NULL)	return false;

	calc_area = p_du->GetRectArea();//処理対象の矩形範囲
	
	p_du->ClearMask();//マスクを一旦クリア

	for(int j=calc_area.top; j<=calc_area.bottom ; j++)
	{
		for(int i=calc_area.left ; i<=calc_area.right ; i++)
		{
			CPoint pt(i,j);

			if( !p_du->DoUseWorkArea || p_du->WorkArea.PtInWorkArea(pt) )//作業領域に入っているかどうかﾁｪｯｸ
			{
				double	val;
				bool	onoff;

				//2値化
				val= p_du->GetDataValue(pt,-1,-1);
				if(val < min_thresh || max_thresh < val){	onoff=false;	}
				else{	onoff=true;	}

				if(inverse)	onoff = !onoff;

				if(onoff)	p_du->SetMaskValue(i,j,UNMASKED_PIXEL);
				else		p_du->SetMaskValue(i,j,MASKED_PIXEL);
			}
		}
	}

	return true;
}

/********************************************************************
機  能  名  称 : 大津の手法で２値化閾値を求める
関    数    名 : BinalizeThresholdOtsu
引          数 : int			src_number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
戻    り    値 : 閾値を返す　失敗したら0になる
機          能 : グローバル２値化閾値の決定
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
double	CPimpomAPI::BinalizeThresholdOtsu(long src_number)
{
	CDataUnit	*p_du=NULL;
	CRect		calc_area;
	float		min_val, max_val;
	int			histgram[256] = {0};//ヒストグラム
	double		sum = 0;
	int			pixcount=0;
	int			max_hist_id;
	double		max_hist = 0.0;
	double		average, average1, average2;



	if( (p_du = GetDataUnit(src_number))==NULL)	return false;
	calc_area = p_du->GetRectArea();//処理対象の矩形範囲


	//ヒストグラム生成
	p_du->GetDataMinMax(&min_val, &max_val);//データの最大・最小
	pixcount=0;
	for(int j=calc_area.top; j<=calc_area.bottom ; j++)
	{
		for(int i=calc_area.left ; i<=calc_area.right ; i++)
		{
			CPoint pt(i,j);
			if( !p_du->DoUseWorkArea || p_du->WorkArea.PtInWorkArea(pt) )//作業領域に入っているかどうかﾁｪｯｸ
			{
				double val = p_du->GetDataValue(pt,-1,-1);

				//256段階でﾋｽﾄｸﾞﾗﾑ作成
				double idx = (double)((val - min_val)/(max_val-min_val)*255.0);
				histgram[(int)idx]++;	
				sum += val;
				pixcount++;
			}
		}
	}
	if(pixcount==0)	return 0;

	average = sum / pixcount;
	
	for(int i=0; i<256; i++)
	{
		int count1 = 0, count2 = 0;
		long data = 0;
		double breakup1 = 0.0, breakup2 = 0.0;
		double tmp = 0;
		double sigma1, sigma2;
		
		//クラス1
		data = 0;
		for(int j=0;j<i;j++){
			count1 += histgram[j];
			data += histgram[j] * j;
		}
		if(count1 != 0){
			average1 = (double)data / (double)count1;/*平均 =(データの総和 / 個数)*/

			for(int j=0;j<i;j++){
				breakup1 += pow( (j- average1), 2 ) * histgram[j];/*分散=(データ - 平均値)^2の総和/個数 */
			}
			breakup1 /= (double)count1; 
		}
		
		//クラス2
		data = 0;
		for(int j=i;j<256;j++){
			count2 += histgram[j];
			data += histgram[j] * j;
		}
		if(count2 != 0){
			average2 = (double)data / (double)count2;/*平均 =(データの総和 / 個数)*/

			for(int j=i;j<256;j++){
				breakup2 += pow( (j - average2), 2 ) * histgram[j];/*分散=(データ - 平均値)^2の総和/個数 */
			}
			breakup2 /= (double)count2;
		}
		
		
		sigma1 = (count1 * breakup1 + count2 * breakup2);/*クラス内分散*/
		sigma2 = count1 * pow( (average1 - average), 2 ) + count2 * pow( (average2 - average), 2 );/*クラス間分散*/
		
		tmp = sigma2 / sigma1;
		if(max_hist < tmp){
			max_hist = tmp;
			max_hist_id = i;
		}
	}
	
	return (double)max_hist_id / 255.0*(max_val-min_val) + min_val;
}


/********************************************************************
機  能  名  称 : Pタイルで２値化閾値を求める
関    数    名 : BinalizeThresholdPTile
引          数 : int			src_number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 int			param				(in)0～100 %
戻    り    値 : 閾値を返す失敗したら0になる
機          能 : グローバル２値化閾値の決定
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
double	CPimpomAPI::BinalizeThresholdPTile(long src_number, int param)
{
	CDataUnit	*p_du=NULL;
	CRect		calc_area;
	float		min_val, max_val;
	int			histgram[256] = {0};//ヒストグラム
	double		sum = 0;
	int			pixcount=0;
	double		max_hist = 0.0;




	if( (p_du = GetDataUnit(src_number))==NULL)	return false;
	calc_area = p_du->GetRectArea();//処理対象の矩形範囲


	//ヒストグラム生成
	p_du->GetDataMinMax(&min_val, &max_val);//データの最大・最小
	pixcount=0;
	for(int j=calc_area.top; j<=calc_area.bottom ; j++)
	{
		for(int i=calc_area.left ; i<=calc_area.right ; i++)
		{
			CPoint pt(i,j);
			if( !p_du->DoUseWorkArea || p_du->WorkArea.PtInWorkArea(pt) )//作業領域に入っているかどうかﾁｪｯｸ
			{
				double val = p_du->GetDataValue(pt,-1,-1);

				//256段階でﾋｽﾄｸﾞﾗﾑ作成
				double idx = (double)((val - min_val)/(max_val-min_val)*255.0);
				histgram[(int)idx]++;	
				sum += val;
				pixcount++;
			}
		}
	}
	if(pixcount==0)	return 0;


	int thresh=0;
	int cnt=0;
	for(int i=0; i<256; i++)
	{
		cnt += histgram[i];

		if( (double)cnt/(double)pixcount >= (double)param/100){
			thresh = i;
			break;
		}
	}
	
	return thresh;
}


/********************************************************************
機  能  名  称 : 画像を局所2値化してべつの画像メモリに入れる
関    数    名 : BinalizeLocalGauss
引          数 : int			src_number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 int			dst_number			(in)2値化後の画像を入れるメモリ
				 int			filter_size,		(in)ガウシアンフィルタサイズ(フィルタサイズの半値が3σ)
				 int			 offset				(in)オフセット
				 bool			inverse				(in)２値反転(true)
				 BYTE			fg_val				(in)前景値
				 BYTE			bg_val				(in)背景値 
				 int			pad_type			(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
				 bool			show_message		(in)変換前に問い合わせダイアログを表示するか
戻    り    値 : 成功するとtrue
機          能 : ガウシアンフィルタをかけた画像と、元画像の差分の符号で2値化
			　　　新しくBYTE型メモリを取得し、閾値以内の画素をfg_val/それ以外をbg_valにする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::BinalizeLocalGauss(long src_number, long dst_number, int filter_size, int offset, bool inverse, BYTE fg_val, BYTE bg_val, int pad_type, bool show_message, bool toMultiImage)
{
	bool ret=true;
	CDataUnit	*p_du=NULL, *p_dst_du=NULL;
	CRect		calc_area;

	//引数チェック
	if( (p_du = GetDataUnit(src_number))==NULL)	return false;

	if(dst_number >= GetDataUnitNumber() || dst_number<0)	return false;

	if(src_number == dst_number)	return false;//同じ番号の指定はできない

	if(GetDataUnit(dst_number)){//結果の格納先として指定された画像メモリに、すでに別のデータが入っている場合は問い合わせ
		if(show_message){
			CString str;
			str.Format("画像メモリ%dは上書きされます",dst_number);
			if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONINFORMATION ) != IDOK ){
				return false;
			}
		}
	}


	int pageStart, pageEnd, channelStart, channelEnd, dstPageNum;
	if (toMultiImage)//複数ページ・チャネル対応
	{
		pageStart = 0;
		pageEnd = p_du->PageNumber - 1;
		channelStart = 0;
		channelEnd = p_du->ChannelNumber - 1;
		dstPageNum = p_du->PageNumber * p_du->ChannelNumber;
	}
	else{
		pageStart = pageEnd = p_du->DispPage;
		channelStart = channelEnd = p_du->DispChannel;
		dstPageNum = 1;
	}



	if (Get3DMemory(dst_number, p_du->DataSize, dstPageNum, true) == NULL)	return false;//新しくBYTE型メモリを取得

	CopyDataUnit(src_number, dst_number, false, false, true, false, true,true);//表示位置・マスク・処理領域・マークを受け継ぐ
	
	p_dst_du = GetDataUnit(dst_number);


	int imsize = p_du->DataSize.cx * p_du->DataSize.cy;//画像のサイズ（画素数）
	calc_area = p_du->GetRectArea();//処理対象の矩形範囲


	

	if (p_du->DispRGB == false || (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) || toMultiImage){//グレー画像に対する処理

		int dstPageCnt = 0;
		for (int page = pageStart; page <= pageEnd; page++)
		{
			for (int channel = channelStart; channel <= channelEnd; channel++)
			{
				if (p_du->pByteData){	//8bit画像
					ret = binalize_gauss(
						p_du->pByteData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}
				else if (p_du->pShortData){	//16bit画像
					ret = binalize_gauss(
						p_du->pShortData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}
				else if (p_du->pWordData){	//16bit画像
					ret = binalize_gauss(
						p_du->pDwordData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}
				else if (p_du->pLongData){	//32bit画像
					ret = binalize_gauss(
						p_du->pLongData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}
				else if (p_du->pDwordData){	//32bit画像
					ret = binalize_gauss(
						p_du->pDwordData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}
				else if (p_du->pFloatData){//浮動少数点
					ret = binalize_gauss(
						p_du->pFloatData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}


				dstPageCnt++;
			}
		}

		if (toMultiImage){//複数ページ・チャネル対応
			p_dst_du->DispPage = p_du->DispPage;
			p_dst_du->DispChannel = p_du->DispChannel;
		}


	}else{//RGB画像に対する処理
		unsigned char*pBrightness = new unsigned char[imsize];
		if(pBrightness==NULL)	return false;

		//いったん明度値のみの画像にする
		for(int j=calc_area.top; j<=calc_area.bottom ; j++){
			for(int i=calc_area.left ; i<=calc_area.right ; i++){
				*(pBrightness + i + j*p_du->DataSize.cx) = p_du->GetDataValue(CPoint(i,j),-1,-1);
			}
		}

		ret = binalize_gauss(
				pBrightness,
				p_dst_du->pByteData,
				p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

		delete[]	pBrightness;
	}


	return ret;
}


/********************************************************************
機  能  名  称 : 画像を局所2値化してべつの画像メモリに入れる
関    数    名 : BinalizeLocalLaplaceGauss
引          数 : int			src_number			(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
				 int			dst_number			(in)2値化後の画像を入れるメモリ
				 int			filter_size,		(in)ガウシアンフィルタサイズ(フィルタサイズの半値が3σ)
				 int			 offset				(in)オフセット
				 bool			inverse				(in)２値反転(true)
				 BYTE			fg_val				(in)前景値
				 BYTE			bg_val				(in)背景値
				 int			pad_type			(in)paddingのタイプ  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ
				 bool			show_message		(in)変換前に問い合わせダイアログを表示するか
戻    り    値 : 成功するとtrue
機          能 : ガウシアンフィルタをかけた画像と、元画像の差分の符号で2値化
			　　　新しくBYTE型メモリを取得し、閾値以内の画素をfg_val/それ以外をbg_valにする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::BinalizeLocalLaplaceGauss(long src_number, long dst_number, int filter_size, int offset, bool inverse, BYTE fg_val, BYTE bg_val, int pad_type, bool show_message, bool toMultiImage)
{
	bool ret=true;
	CDataUnit	*p_du=NULL, *p_dst_du=NULL;
	CRect		calc_area;

	//引数チェック
	if( (p_du = GetDataUnit(src_number))==NULL)	return false;

	if(dst_number >= GetDataUnitNumber() || dst_number<0)	return false;

	if(src_number == dst_number)	return false;//同じ番号の指定はできない

	if(GetDataUnit(dst_number)){//結果の格納先として指定された画像メモリに、すでに別のデータが入っている場合は問い合わせ
		if(show_message){
			CString str;
			str.Format("画像メモリ%dは上書きされます",dst_number);
			if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONINFORMATION ) != IDOK ){
				return false;
			}
		}
	}


	int pageStart, pageEnd, channelStart, channelEnd, dstPageNum;
	if (toMultiImage)//複数ページ・チャネル対応
	{
		pageStart = 0;
		pageEnd = p_du->PageNumber - 1;
		channelStart = 0;
		channelEnd = p_du->ChannelNumber - 1;
		dstPageNum = p_du->PageNumber * p_du->ChannelNumber;
	}
	else{
		pageStart = pageEnd = p_du->DispPage;
		channelStart = channelEnd = p_du->DispChannel;
		dstPageNum = 1;
	}


	if (Get3DMemory(dst_number, p_du->DataSize, dstPageNum, true) == NULL)	return false;//新しくBYTE型メモリを取得
	
	CopyDataUnit(src_number, dst_number, false, false, true, false, true,true);//表示位置・マスク・処理領域・マークを受け継ぐ

	p_dst_du = GetDataUnit(dst_number);


	int imsize = p_du->DataSize.cx * p_du->DataSize.cy;//画像のサイズ（画素数）
	calc_area = p_du->GetRectArea();//処理対象の矩形範囲


	

	if(p_du->DispRGB == false || (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT)){//グレー画像に対する処理

		int dstPageCnt = 0;
		for (int page = pageStart; page <= pageEnd; page++)
		{
			for (int channel = channelStart; channel <= channelEnd; channel++)
			{
				if(p_du->pByteData){	//8bit画像
					ret = binalize_laplace_gauss(
						p_du->pByteData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
							p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}else if(p_du->pShortData){	//16bit画像
					ret = binalize_laplace_gauss(
						p_du->pShortData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
							p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}else if(p_du->pWordData){	//16bit画像
					ret =binalize_laplace_gauss(
						p_du->pDwordData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
							p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}else if(p_du->pLongData){	//32bit画像
					ret = binalize_laplace_gauss(
						p_du->pLongData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
							p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}else if(p_du->pDwordData){	//32bit画像
					ret = binalize_laplace_gauss(
						p_du->pDwordData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
							p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}else if(p_du->pFloatData){//浮動少数点
					ret =binalize_laplace_gauss(
						p_du->pFloatData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
							p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}

				dstPageCnt++;
			}
		}


		if (toMultiImage){//複数ページ・チャネル対応
			p_dst_du->DispPage = p_du->DispPage;
			p_dst_du->DispChannel = p_du->DispChannel;
		}

	}else{//RGB画像に対する処理
		unsigned char*pBrightness = new unsigned char[imsize];
		if(pBrightness==NULL)	return false;

		//いったん明度値のみの画像にする
		for(int j=calc_area.top; j<=calc_area.bottom ; j++){
			for(int i=calc_area.left ; i<=calc_area.right ; i++){
				*(pBrightness + i + j*p_du->DataSize.cx) = p_du->GetDataValue(CPoint(i,j),-1,-1);
			}
		}

		ret = binalize_laplace_gauss(
				pBrightness,
				p_dst_du->pByteData,
				p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

		delete[]	pBrightness;
	}


	return ret;
}

/********************************************************************
機  能  名  称 : 2値化画像をラベリングしてべつの画像メモリに入れる
関    数    名 : Label
引          数 : long			src_number			(in)2値化後の画像を入れるメモリ(-1にした場合は現在選択中の画像)
				 long			dst_number			(in)ラベル画像(ラベル番号)を入れるメモリ
				 double			 fg_val				(in)前景値
				 double			 bg_val				(in)背景値
				 bool			sort_label			(in)ラベル番号をラベル面積でソートするか 
				 int			min_area			(in)ラベル最小面積
				 int			area[],				(out)ラベル面積配列
				 int			gx[],				(out)ラベル重心xの配列
				 int			gy[],				(out)ラベル重心yの配列
				 bool			show_message		(in)変換前に問い合わせダイアログを表示するか
戻    り    値 : ラベル数　処理が失敗した場合は -1
機          能 : ２値化画像に対してラベリングする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int	CPimpomAPI::Label(
				long	src_number, 
				long	dst_number, 
				bool	sort_label, 
				int		min_area,  
				int		*area,		
				float	*gx,
				float	*gy,
				BYTE	fg_val,
				BYTE	bg_val,
				bool	show_message, 
				bool	toMultiImage)
{
	CDataUnit	*p_du=NULL, *p_dst_du=NULL;
	CRect		calc_area;
	int			labelCnt=0;


	if( (p_du = GetDataUnit(src_number))==NULL)	return -1;
	if (p_du->DataType != BYTE_FORMAT && p_du->DataType != THREE_D_FORMAT)	return -1;//8bit画像でないと処理しない
	if(src_number == dst_number)	return -1;//同じ番号の指定はできない




	int pageStart, pageEnd, channelStart, channelEnd, dstPageNum;
	if (toMultiImage)//複数ページ・チャネル対応
	{
		pageStart = 0;
		pageEnd = p_du->PageNumber - 1;
		channelStart = 0;
		channelEnd = p_du->ChannelNumber - 1;
		dstPageNum = p_du->PageNumber * p_du->ChannelNumber;
	}
	else{
		pageStart = pageEnd = p_du->DispPage;
		channelStart = channelEnd = p_du->DispChannel;
		dstPageNum = 1;
	}

	if (GetF3DMemory(dst_number, p_du->DataSize, dstPageNum, true) == NULL)	return false;//新しくF3D型メモリを取得

	CopyDataUnit(src_number, dst_number, false, false, true, false, true,true);//表示位置・マスク・処理領域・マークを受け継ぐ

	p_dst_du = GetDataUnit(dst_number);
	
	
	int imsize = p_du->DataSize.cx * p_du->DataSize.cy;//画像のサイズ（画素数）


	cv::Mat src;
	ConvertToCvImage2(src_number, &src);

	int dstPageCnt = 0;
	for (int page = pageStart; page <= pageEnd; page++)
	{
		for (int channel = channelStart; channel <= channelEnd; channel++)
		{

			//ラべリング処理
			cv::Mat LabelImg;
			cv::Mat stats;
			cv::Mat centroids;
			int nLab = cv::connectedComponentsWithStats(src, LabelImg, stats, centroids);

			labelCnt = 0;
			for (int i = 1; i < nLab && labelCnt < BINALIZE_LABEL_MAX ; i++)
			{
				int *param = stats.ptr<int>(i);
				int labelarea = param[cv::ConnectedComponentsTypes::CC_STAT_AREA];
				if (labelarea >= min_area)
				{
						if (!toMultiImage)
						{//ラベル情報
							if (gx != NULL && gy != NULL)
							{
								gx[labelCnt] = centroids.at<double>(i * 2);
								gy[labelCnt] = centroids.at<double>(i * 2 + 1);
							}

							if (area != NULL) {
								area[labelCnt] = labelarea;
							}
						}
						labelCnt++;
				}
			}

			//結果格納
			for (int i = 0; i < imsize; i++) {
				*(p_dst_du->pFloatData + imsize*dstPageCnt + i) = ((int*)LabelImg.data)[i];
			}


			dstPageCnt++;
		}
	}

	if (toMultiImage){//複数ページ・チャネル対応
		p_dst_du->DispPage = p_du->DispPage;
		p_dst_du->DispChannel = p_du->DispChannel;
	}

	return labelCnt;
}