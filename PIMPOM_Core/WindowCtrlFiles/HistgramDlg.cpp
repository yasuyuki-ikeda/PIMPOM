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
//CHistgramDlg
//ヒストグラムウィンドウ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "HistgramDlg.h"

// *************************************
//         マ  ク  ロ   定   義         
// *************************************
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define		COLOR_BAR_W		(256)//カラーバーの幅
#define		COLOR_BAR_H		(10)//カラーバーの高さ
#define		DATA_FIELD_X	(0)//ヒストグラムデータ表示領域の左上のX座標
#define		DATA_FIELD_Y	(195)//ヒストグラムデータ表示領域の左上のY座標
#define		DATA_FIELD_W	(256)//ヒストグラムデータ表示領域の幅
#define		DATA_FIELD_H	(60)//ヒストグラムデータ表示領域の高さ

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

//////////////////////////////////////////////////////////////////////////////////////////
//  メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////

CHistgramDlg::CHistgramDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHistgramDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistgramDlg)
	//}}AFX_DATA_INIT
}


void CHistgramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistgramDlg)
	//}}AFX_DATA_MAP
}


//ダイアログアイテムの配置
void CHistgramDlg::show_dlg_item()
{
	//ボタンに関連付けるビットマップのロード
	bmp_button_inc.LoadBitmap( IDB_BUTTON_INCLIMENT );
	bmp_button_dec.LoadBitmap( IDB_BUTTON_DECLIMENT );

	//ヒストグラム領域
	CWnd	*p_histgram_field = (CWnd*)GetDlgItem( IDC_HISTGRAM_FIELD  );
	p_histgram_field->MoveWindow( 0, 0, HISTGRAM_W , HISTGRAM_H );

	//ヒストグラムデータの表示領域
	CWnd	*p_histgram_data_field = (CWnd*)GetDlgItem( IDC_HISTGRAM_DATA_FIELD  );
	p_histgram_data_field->MoveWindow( DATA_FIELD_X , DATA_FIELD_Y , DATA_FIELD_W , DATA_FIELD_H );

	//設定ボタン
	CWnd	*p_button_set = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_SET  );
	p_button_set->MoveWindow( HISTGRAM_W, HISTGRAM_H + COLOR_BAR_H - 25, 35, 25);

	//度数表示の最大値を表すエディットボックス
	CWnd	*p_edit_histgram_max = (CWnd*)GetDlgItem( IDC_EDIT_HISTGRAM_MAX  );
	p_edit_histgram_max->MoveWindow( HISTGRAM_W, 0, 40, 15); 

	CWnd	*p_button_histgram_max_inc = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_MAX_INC  );//値を1ずつ増やすボタン
	p_button_histgram_max_inc->MoveWindow( HISTGRAM_W + 40, 0, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_inc, IDC_BUTTON_HISTGRAM_MAX_INC ); 

	CWnd	*p_button_histgram_max_dec = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_MAX_DEC  );//値を1ずつ減らすボタン
	p_button_histgram_max_dec->MoveWindow( HISTGRAM_W + 40, 12, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_dec, IDC_BUTTON_HISTGRAM_MAX_DEC ); 

	CWnd	*p_button_histgram_max_hlf = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_MAX_HLF  );//値を半分にするボタン
	p_button_histgram_max_hlf->MoveWindow( HISTGRAM_W, 15, 20, 15);
	
	CWnd	*p_button_histgram_max_dbl = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_MAX_DBL  );//値を2倍にするボタン
	p_button_histgram_max_dbl->MoveWindow( HISTGRAM_W + 20, 15, 20, 15);

	API.SetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , 3000);//初期値を設定

	//表示レンジの最小値を表すエディットボックス
	CWnd	*p_edit_range_min = (CWnd*)GetDlgItem( IDC_EIDT_HISTGRAM_RANGE_MIN  );
	p_edit_range_min->MoveWindow( 0 , HISTGRAM_H + COLOR_BAR_H , 40 , 15); 

	CWnd	*p_button_range_min_inc = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MIN_INC  );//値を1ずつ増やすボタン
	p_button_range_min_inc->MoveWindow( 40, HISTGRAM_H + COLOR_BAR_H, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_inc, IDC_BUTTON_RANGE_MIN_INC ); 

	CWnd	*p_button_range_min_dec = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MIN_DEC  );//値を1ずつ減らすボタン
	p_button_range_min_dec->MoveWindow( 40, HISTGRAM_H + COLOR_BAR_H + 12, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_dec, IDC_BUTTON_RANGE_MIN_DEC );

	CWnd	*p_button_range_min_hlf = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MIN_HLF  );//値を半分にするボタン
	p_button_range_min_hlf->MoveWindow( 0, HISTGRAM_H + COLOR_BAR_H + 15, 20, 15);
	
	CWnd	*p_button_range_min_dbl = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MIN_DBL  );//値を2倍にするボタン
	p_button_range_min_dbl->MoveWindow( 20, HISTGRAM_H + COLOR_BAR_H + 15, 20, 15);


	//表示レンジの最大値を表すエディットボックス
	CWnd	*p_edit_range_max = (CWnd*)GetDlgItem( IDC_EIDT_HISTGRAM_RANGE_MAX  );
	p_edit_range_max->MoveWindow( HISTGRAM_W - 40 , HISTGRAM_H + COLOR_BAR_H , 40, 15); 

	CWnd	*p_button_range_max_inc = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MAX_INC  );//値を1ずつ増やすボタン
	p_button_range_max_inc->MoveWindow( HISTGRAM_W , HISTGRAM_H + COLOR_BAR_H, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_inc, IDC_BUTTON_RANGE_MAX_INC ); 

	CWnd	*p_button_range_max_dec = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MAX_DEC  );//値を1ずつ減らすボタン
	p_button_range_max_dec->MoveWindow( HISTGRAM_W , HISTGRAM_H + COLOR_BAR_H + 12, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_dec, IDC_BUTTON_RANGE_MAX_DEC );

	CWnd	*p_button_range_max_hlf = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MAX_HLF  );//値を半分にするボタン
	p_button_range_max_hlf->MoveWindow( HISTGRAM_W - 40, HISTGRAM_H + COLOR_BAR_H + 15, 20, 15);
	
	CWnd	*p_button_range_max_dbl = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MAX_DBL  );//値を2倍にするボタン
	p_button_range_max_dbl->MoveWindow( HISTGRAM_W - 20, HISTGRAM_H + COLOR_BAR_H + 15, 20, 15);


	//表示レンジの中間値を表すエディットボックス
	CWnd	*p_edit_range_mid = (CWnd*)GetDlgItem( IDC_EIDT_HISTGRAM_RANGE_MID  );
	p_edit_range_mid->MoveWindow(80 , HISTGRAM_H + COLOR_BAR_H , 40, 15); 

	CWnd	*p_edit_static_range_mid = (CWnd*)GetDlgItem( IDC_HISTGRAM_DLG_STATIC_RANGE_MID  );//エディットボックスの説明の文字（static）
	p_edit_static_range_mid->MoveWindow(60 , HISTGRAM_H + COLOR_BAR_H + 2, 20, 15); 

	CWnd	*p_button_range_mid_inc = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MID_INC  );//値を1ずつ増やすボタン
	p_button_range_mid_inc->MoveWindow( 120 , HISTGRAM_H + COLOR_BAR_H, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_inc, IDC_BUTTON_RANGE_MID_INC ); 

	CWnd	*p_button_range_mid_dec = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MID_DEC  );//値を1ずつ減らすボタン
	p_button_range_mid_dec->MoveWindow( 120 , HISTGRAM_H + COLOR_BAR_H + 12, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_dec, IDC_BUTTON_RANGE_MID_DEC );

	CWnd	*p_button_range_mid_hlf = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MID_HLF  );//値を半分にするボタン
	p_button_range_mid_hlf->MoveWindow( 80, HISTGRAM_H + COLOR_BAR_H + 15, 20, 15);
	
	CWnd	*p_button_range_mid_dbl = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MID_DBL  );//値を2倍にするボタン
	p_button_range_mid_dbl->MoveWindow( 100, HISTGRAM_H + COLOR_BAR_H + 15, 20, 15);


	//表示レンジの幅を表すエディットボックス
	CWnd	*p_edit_range_width = (CWnd*)GetDlgItem( IDC_EIDT_HISTGRAM_RANGE_WIDTH  );
	p_edit_range_width->MoveWindow( 150 , HISTGRAM_H + COLOR_BAR_H , 40, 15); 

	CWnd	*p_edit_static_range_width = (CWnd*)GetDlgItem( IDC_HISTGRAM_DLG_STATIC_RANGE_WIDTH  );//エディットボックスの説明の文字（static）
	p_edit_static_range_width->MoveWindow(140 , HISTGRAM_H + COLOR_BAR_H + 2, 10, 15); 

	CWnd	*p_button_range_width_inc = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_WIDTH_INC  );//値を1ずつ増やすボタン
	p_button_range_width_inc->MoveWindow( 190 , HISTGRAM_H + COLOR_BAR_H, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_inc, IDC_BUTTON_RANGE_WIDTH_INC ); 

	CWnd	*p_button_range_width_dec = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_WIDTH_DEC  );//値を1ずつ減らすボタン
	p_button_range_width_dec->MoveWindow( 190 , HISTGRAM_H + COLOR_BAR_H + 12, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_dec, IDC_BUTTON_RANGE_WIDTH_DEC );

	CWnd	*p_button_range_width_hlf = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_WIDTH_HLF  );//値を半分にするボタン
	p_button_range_width_hlf->MoveWindow( 150, HISTGRAM_H + COLOR_BAR_H + 15, 20, 15);
	
	CWnd	*p_button_range_width_dbl = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_WIDTH_DBL  );//値を2倍にするボタン
	p_button_range_width_dbl->MoveWindow( 170, HISTGRAM_H + COLOR_BAR_H + 15, 20, 15);


	//平均値を表すエディットボックス
	CWnd	*p_edit_average = (CWnd*)GetDlgItem( IDC_EDIT_HISTGRAM_AVERAGE  );
	p_edit_average->MoveWindow( DATA_FIELD_X + 35 , DATA_FIELD_Y + 5 , 50, 15); 

	CWnd	*p_edit_static_average = (CWnd*)GetDlgItem( IDC_HISTGRAM_DLG_STATIC_AVERAGE  );//エディットボックスの説明の文字（static）
	p_edit_static_average->MoveWindow( DATA_FIELD_X + 5 , DATA_FIELD_Y + 5, 30, 15); 


	//標準偏差を表すエディットボックス
	CWnd	*p_edit_stdiv = (CWnd*)GetDlgItem( IDC_EDIT_HISTGRAM_STDIV  );
	p_edit_stdiv->MoveWindow( DATA_FIELD_X + 160 , DATA_FIELD_Y + 5 , 50, 15 ); 

	CWnd	*p_edit_static_stdiv = (CWnd*)GetDlgItem( IDC_HISTGRAM_DLG_STATIC_STDIV  );//エディットボックスの説明の文字（static）
	p_edit_static_stdiv->MoveWindow( DATA_FIELD_X + 110 , DATA_FIELD_Y + 5 , 50 , 15 ); 

	//マウスでポイントされた濃度を表すエディットボックス
	CWnd	*p_edit_pointed_brightness = (CWnd*)GetDlgItem( IDC_EDIT_HISTGRAM_POINTED_BRIGHTNESS  );
	p_edit_pointed_brightness->MoveWindow( DATA_FIELD_X + 5 , DATA_FIELD_Y + 25 , 100, 15 ); 

	//マウスでポイントされた濃度の度数を表すエディットボックス
	CWnd	*p_edit_pointed_hist = (CWnd*)GetDlgItem( IDC_EDIT_HISTGRAM_POINTED_HIST  );
	p_edit_pointed_hist->MoveWindow( DATA_FIELD_X + 5 , DATA_FIELD_Y + 42 , 100, 15 ); 

	//表示濃度レンジを調節するボタン
	CWnd	*p_button_min_max = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_SET_MIN_MAX  );//min-max
	p_button_min_max->MoveWindow( DATA_FIELD_W + 2 , DATA_FIELD_Y , 55, 15 ); 

	CWnd	*p_button_ave = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_SET_AVE  );
	p_button_ave->MoveWindow( DATA_FIELD_W + 2 , DATA_FIELD_Y + 15 , 55, 15 ); //ave±sdv

	CWnd	*p_button_0_256 = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_SET_0_256  );
	p_button_0_256->MoveWindow( DATA_FIELD_W + 2 , DATA_FIELD_Y + 15*2 , 55, 15 ); //0-256

	CWnd	*p_button_pi = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_SET_PI_PI  );
	p_button_pi->MoveWindow( DATA_FIELD_W + 2 , DATA_FIELD_Y + 15*3 , 55, 15 ); //-pi - pi
}


//idで示されるボタンにビットマップを関連づける
HBITMAP CHistgramDlg::set_bitmap_button(HBITMAP h_bmp, UINT id)
{
	CButton	*p_button = (CButton*)GetDlgItem(id);
	return	p_button->SetBitmap( h_bmp );
}


//カラーバーを表すビットマップの作成
void CHistgramDlg::create_color_bar()
{
	p_color_bar_data = new BYTE[ COLOR_BAR_W * COLOR_BAR_H ];//カラーバーの濃度データの作成
		for(int j=0   ;   j< COLOR_BAR_H   ;   j++)
			for(int i=0   ;   i< COLOR_BAR_W   ;   i++)
				*( p_color_bar_data + i + j * COLOR_BAR_W ) = (BYTE)i;

	p_color_bar_data_info = (BITMAPINFO*)new char[ sizeof(BITMAPINFOHEADER) + 1024];//BITMAPINFOHEADERとカラーパレットのメモリを取得する
																		
	p_color_bar_data_info->bmiHeader.biSize = 40;//ヘッダのサイズ
	p_color_bar_data_info->bmiHeader.biWidth = COLOR_BAR_W;//カラーバーの幅
	p_color_bar_data_info->bmiHeader.biHeight = COLOR_BAR_H;//カラーバーの高さ
	p_color_bar_data_info->bmiHeader.biPlanes = 1;
	p_color_bar_data_info->bmiHeader.biBitCount = 8;//カラーバーの1画素あたりのサイズ
	p_color_bar_data_info->bmiHeader.biCompression = 0;//カラーバーの圧縮形式
	p_color_bar_data_info->bmiHeader.biSizeImage   = 0;
	p_color_bar_data_info->bmiHeader.biXPelsPerMeter = 0;
	p_color_bar_data_info->bmiHeader.biYPelsPerMeter = 0;
	p_color_bar_data_info->bmiHeader.biClrUsed = 0;
	p_color_bar_data_info->bmiHeader.biClrImportant = 0;

	ZeroMemory( p_color_bar_data_info->bmiColors , 1024);//カラーパレットをゼロクリア（全て黒）
}


//ヒストグラムを描画する
//do_show_values を true にするとヒストグラムの設定値を再表示する
void CHistgramDlg::DrawHistgram()
{
	int	i;
	float	disp_hist, histgram_max;
	CDC		*p_dc;
	CDataUnit	*p_du;


	p_du = API.GetDataUnit( CURRENT_IMAGE );//現在選択中のデータを取得する
		if(p_du == NULL)	return;//データが無ければなにもしない

	p_dc = GetDC();//デバイスコンテキストを取得

		if( !API.GetEditValue(m_hWnd, IDC_EDIT_HISTGRAM_MAX , &histgram_max ))		return;//ヒストグラム最大値を取得する

	p_dc->Rectangle( 0 , 0 , HISTGRAM_W , HISTGRAM_H );//背景の矩形を描画する

		for(i=0 ; i<256 ; i++){
			disp_hist = HISTGRAM_H / histgram_max * (float)p_du->BrightLevel[i];//度数を表示するときの高さ

			//度数を表示する（縦ライン）
			p_dc->MoveTo(i, HISTGRAM_H);
			p_dc->LineTo(i, HISTGRAM_H - (long)disp_hist);
		}
	
	ReleaseDC(p_dc);//デバイスコンテキストを開放

	draw_color_bar();//カラーバーを描画する
	
	//ヒストグラムの設定値を再表示する
	API.SetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_AVERAGE , p_du->Average);
	API.SetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_STDIV , p_du->Stdiv);
	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , p_du->BrightMin);
	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , p_du->BrightMax);
	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , ( p_du->BrightMax   +   p_du->BrightMin ) / 2 );
	API.SetEditValue( m_hWnd,  IDC_EIDT_HISTGRAM_RANGE_WIDTH , ( p_du->BrightMax   -   p_du->BrightMin ) / 2 );

}


//カラーバーを描画する
void CHistgramDlg::draw_color_bar()
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//選択中の画像データを取得する

		//カラーパレットの設定
		if(p_du != NULL)
			memcpy( p_color_bar_data_info->bmiColors , p_du->pDispBufferBmpInfo->bmiColors, 1024);//選択中のデータのカラーパレットをコピー

		else
			ZeroMemory( p_color_bar_data_info->bmiColors , 1024);//カラーパレットをゼロクリア（全て黒）

	HDC	hdc = ::GetDC( m_hWnd );//デバイスコンテキストを取得する
	::StretchDIBits( hdc,	0 , HISTGRAM_H	 , COLOR_BAR_W , COLOR_BAR_H ,//カラーバーのビットマップ（DIB）を貼り付ける
							0 ,	0			 , COLOR_BAR_W , COLOR_BAR_H ,
							p_color_bar_data ,  p_color_bar_data_info ,  DIB_RGB_COLORS , SRCCOPY);
	::ReleaseDC(m_hWnd, hdc);	//デバイスコンテキストを開放する
}

//中央値のレンジ幅から表示レンジの最大と最小を求める
bool CHistgramDlg::set_min_max_from_mid_width(float mid, float width, float *min, float *max)
{
	float temp_max, temp_min;
	
	temp_max = mid + width;
	temp_min = mid - width;

		if(temp_min >= temp_max)	return	false;//大小関係が逆になったらしっぱい

	*max = temp_max;
	*min = temp_min;
	return	true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  メッセージ ハンドラ
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CHistgramDlg, CDialog)
	//{{AFX_MSG_MAP(CHistgramDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_MAX_INC, OnButtonHistgramMaxInc)
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_MAX_DEC, OnButtonHistgramMaxDec)
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_MAX_HLF, OnButtonHistgramMaxHlf)
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_MAX_DBL, OnButtonHistgramMaxDbl)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MIN_INC, OnButtonRangeMinInc)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MIN_DEC, OnButtonRangeMinDec)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MIN_HLF, OnButtonRangeMinHlf)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MIN_DBL, OnButtonRangeMinDbl)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MID_INC, OnButtonRangeMidInc)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MID_DEC, OnButtonRangeMidDec)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MID_HLF, OnButtonRangeMidHlf)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MID_DBL, OnButtonRangeMidDbl)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_WIDTH_INC, OnButtonRangeWidthInc)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_WIDTH_DEC, OnButtonRangeWidthDec)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_WIDTH_HLF, OnButtonRangeWidthHlf)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_WIDTH_DBL, OnButtonRangeWidthDbl)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MAX_INC, OnButtonRangeMaxInc)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MAX_DEC, OnButtonRangeMaxDec)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MAX_HLF, OnButtonRangeMaxHlf)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MAX_DBL, OnButtonRangeMaxDbl)
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_SET, OnHistgramSet)
	ON_EN_CHANGE(IDC_EIDT_HISTGRAM_RANGE_MIN, OnChangeHistgramRangeMin)
	ON_EN_CHANGE(IDC_EIDT_HISTGRAM_RANGE_MAX, OnChangeHistgramRangeMax)
	ON_EN_CHANGE(IDC_EIDT_HISTGRAM_RANGE_MID, OnChangeHistgramRangeMid)
	ON_EN_CHANGE(IDC_EIDT_HISTGRAM_RANGE_WIDTH, OnChangeHistgramRangeWidth)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_SET_MIN_MAX, OnHistgramSetMinMax)
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_SET_AVE, OnHistgramSetAve)
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_SET_0_256, OnHistgramSet0256)
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_SET_PI_PI, OnHistgramSetPiPi)
	//}}AFX_MSG_MAP
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


/********************************************************************
機  能  名  称 : ダイアログの初期化
関    数    名 : OnInitDialog
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
BOOL CHistgramDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	create_color_bar();//カラーバーの作成
	show_dlg_item();//ダイアログアイテムの配置

	MoveWindow(API.main_window_size.cx, 0, HISTGRAM_WIN_W,  HISTGRAM_WIN_H);//ウインドウの大きさ設定
	flg_str_from_key = false;
	is_draging_r=false;

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/********************************************************************
機  能  名  称 : このウィンドウを表示状態にする
関    数    名 : OnShowWindow
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);


}

/********************************************************************
機  能  名  称 :キーボードからの入力
関    数    名 : PreTranslateMessage
引          数 : 
戻    り    値 : 
機          能 : リターン：ヒストグラム設定変更の反映
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
BOOL CHistgramDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN ){
		CPoint	point;

		switch(pMsg->wParam){
			case VK_RETURN://Enter
				OnHistgramSet() ;
				return  true;

			case VK_ESCAPE://Esc
				return  true;

			case VK_UP://↑
				return  true;

			case VK_DOWN://↓
				return  true;

			case VK_LEFT://←
				return  true;

			case VK_RIGHT://→
				return true;

			case VK_DELETE://Del
				break;

			default:
				flg_str_from_key = true;
				break;
		}
	}	
	
	return CDialog::PreTranslateMessage(pMsg);
}

/********************************************************************
機  能  名  称 :ウインドウの破棄
関    数    名 : OnDestroy
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnDestroy() 
{
	CDialog::OnDestroy();
		
	delete[]	p_color_bar_data_info;
	delete[]	p_color_bar_data;
}

/********************************************************************
機  能  名  称 :再描画
関    数    名 : OnPaint
引          数 : 
戻    り    値 : 
機          能 : ヒストグラムを再描画
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	
	DrawHistgram();
	// 描画用メッセージとして CDialog::OnPaint() を呼び出してはいけません
}

/********************************************************************
機  能  名  称 :マウスカーソルが通過した
関    数    名 : OnMouseMove
引          数 : 
戻    り    値 : 
機          能 : 通過した位置のヒストグラムの要素を表示する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	static int oldpoint_x = -1;//前回マウスでポイントした点のx座標
	CDC		*p_dc;
	float	max, min, coef;
	CRect	histgram_field( 0,0, HISTGRAM_W, HISTGRAM_H );//ヒストグラムを描く領域を表す矩形
	CString	bri, hist;
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );

		if( p_du == NULL )		return;//現在選択中の画像メモリにデータが無ければ何もしない

	p_dc = GetDC();//デバイスコンテキストの取得
	p_dc->SetROP2(R2_NOT);//色反転させるペン
		//if( oldpoint_x != -1 ){//前回に描いた線がある場合は，塗りなおしをする
		//	p_dc->MoveTo( oldpoint_x , HISTGRAM_H -1 );
		//	p_dc->LineTo( oldpoint_x , 0 );	
		//}
	DrawHistgram();//ヒストグラム描画

		if( histgram_field.PtInRect(point) )
		{//現在のマウスポインタがヒストグラム上にある場合の処理

			

			//ヒストグラム上でマウスでポイントした濃度の度数を表す線を反転ペンで描く
			p_dc->MoveTo( point.x , HISTGRAM_H -1 );
			p_dc->LineTo( point.x , 0 );
			oldpoint_x = point.x;

			//ヒストグラム上でマウスでポイントした濃度の変換前の値とその度数を表示する
			coef = (float)((p_du->BrightMax - p_du->BrightMin)/256.0);
			min = point.x * coef  +  p_du->BrightMin;
			max = (point.x + 1) * coef  +  p_du->BrightMin;
	
				if(point.x == 255)
					bri = API.ToString( min ) + _T("～");
				else if(point.x == 0)	
					bri = _T("～") + API.ToString( max );
				else
					bri = API.ToString( min ) + _T("～") + API.ToString( max );

			hist.Format("度数：%d" , p_du->BrightLevel[ point.x ] );


			if(is_draging_r)
			{//右ドラッグ中
				int sx=0,ex=0;
				if(hist_value_drag_start > point.x){
					sx = point.x+1;	ex = hist_value_drag_start;
				}else if(hist_value_drag_start < point.x){
					ex = point.x-1;	sx = hist_value_drag_start;
				}

				for(int x=sx ; x<=ex ; x++){//ドラッグ範囲を反転
					p_dc->MoveTo( x , HISTGRAM_H -1 );
					p_dc->LineTo( x , 0 );
				}
			}


		}else{
			bri = _T("");
			hist = _T("");
			oldpoint_x = -1;
			is_draging_r=false;

		}
	API.SetEditValue( m_hWnd , IDC_EDIT_HISTGRAM_POINTED_BRIGHTNESS , bri );
	API.SetEditValue( m_hWnd , IDC_EDIT_HISTGRAM_POINTED_HIST , hist );

	ReleaseDC(p_dc);//デバイスコンテキストの開放
	CDialog::OnMouseMove(nFlags, point);
}

/********************************************************************
機  能  名  称 :度数の最大値の値変更ボタン押下
関    数    名 : OnButtonHistgramMaxInc
引          数 : 
戻    り    値 : 
機          能 : 1増加
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonHistgramMaxInc() 
{
	float	histgram_max;
		if( !API.GetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , &histgram_max ))		return;
		if( histgram_max >= FLT_MAX )		return;
	API.SetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , histgram_max + 1);
	DrawHistgram();
}

/********************************************************************
機  能  名  称 :度数の最大値の値変更ボタン押下
関    数    名 : OnButtonHistgramMaxDec
引          数 : 
戻    り    値 : 
機          能 : 1減少
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonHistgramMaxDec() 
{
	float	histgram_max;
		if( !API.GetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , &histgram_max ))		return;
		if( histgram_max <= 1 )		return;
	API.SetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , histgram_max + 1);
	DrawHistgram();
}

/********************************************************************
機  能  名  称 :度数の最大値の値変更ボタン押下
関    数    名 : OnButtonHistgramMaxDec
引          数 : 
戻    り    値 : 
機          能 : 値を半分にする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonHistgramMaxHlf() 
{
	float	histgram_max;
		if( !API.GetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , &histgram_max ))		return;
		if( histgram_max <= 2 )		return;
	API.SetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , histgram_max / 2);
	DrawHistgram();
}

/********************************************************************
機  能  名  称 :度数の最大値の値変更ボタン押下
関    数    名 : OnButtonHistgramMaxDec
引          数 : 
戻    り    値 : 
機          能 : 値を２倍にする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonHistgramMaxDbl() 
{
	float	histgram_max;
		if( !API.GetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , &histgram_max ))		return;
		if( histgram_max >= FLT_MAX )		return;
	API.SetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , histgram_max * 2);
	DrawHistgram();
}

/********************************************************************
機  能  名  称 :表示最小値の値変更ボタン押下
関    数    名 : OnButtonRangeMinInc
引          数 : 
戻    り    値 : 
機          能 : １増加
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonRangeMinInc() 
{
	float	range_min, range_max;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ))		return;

	API.SetDispRange( CURRENT_IMAGE, range_min + 1, range_max );
}

/********************************************************************
機  能  名  称 :表示最小値の値変更ボタン押下
関    数    名 : OnButtonRangeMinDec
引          数 : 
戻    り    値 : 
機          能 : １減少
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonRangeMinDec() 
{
	float	range_min, range_max;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ))		return;

	API.SetDispRange( CURRENT_IMAGE, range_min - 1, range_max );
}

/********************************************************************
機  能  名  称 :表示最小値の値変更ボタン押下
関    数    名 : OnButtonRangeMinHlf
引          数 : 
戻    り    値 : 
機          能 : 値を半分にする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonRangeMinHlf() 
{
	float	range_min, range_max;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ))		return;

	API.SetDispRange( CURRENT_IMAGE, range_min / 2 , range_max );
}

/********************************************************************
機  能  名  称 :表示最小値の値変更ボタン押下
関    数    名 : OnButtonRangeMinDbl
引          数 : 
戻    り    値 : 
機          能 : 値を２倍にする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonRangeMinDbl() 
{
	float	range_min, range_max;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ))		return;

	API.SetDispRange( CURRENT_IMAGE, range_min * 2 , range_max );
}

/********************************************************************
機  能  名  称 :表示最大値の値変更ボタン押下
関    数    名 : OnButtonRangeMaxInc
引          数 : 
戻    り    値 : 
機          能 : １増加
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonRangeMaxInc() 
{
	float	range_min, range_max;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ))		return;

	API.SetDispRange( CURRENT_IMAGE, range_min, range_max + 1 );	
}

/********************************************************************
機  能  名  称 :表示最大値の値変更ボタン押下
関    数    名 : OnButtonRangeMaxDec
引          数 : 
戻    り    値 : 
機          能 : １減少
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonRangeMaxDec() 
{
	float	range_min, range_max;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ))		return;

	API.SetDispRange( CURRENT_IMAGE, range_min, range_max - 1 );		
}

/********************************************************************
機  能  名  称 :表示最大値の値変更ボタン押下
関    数    名 : OnButtonRangeMaxHlf
引          数 : 
戻    り    値 : 
機          能 : 値を半分にする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonRangeMaxHlf() 
{
	float	range_min, range_max;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ))		return;

	API.SetDispRange( CURRENT_IMAGE, range_min, range_max / 2 );		
}

/********************************************************************
機  能  名  称 :表示最大値の値変更ボタン押下
関    数    名 : OnButtonRangeMaxDbl
引          数 : 
戻    り    値 : 
機          能 : 値を２倍にする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonRangeMaxDbl() 
{
	float	range_min, range_max;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ))		return;

	API.SetDispRange( CURRENT_IMAGE, range_min, range_max * 2 );		
}

/********************************************************************
機  能  名  称 :表示中央値の値変更ボタン押下
関    数    名 : OnButtonRangeMidInc
引          数 : 
戻    り    値 : 
機          能 : １増加
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonRangeMidInc() 
{
	float	range_max, range_min, range_mid, range_width;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , &range_mid ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ))		return;	
		if(!set_min_max_from_mid_width( range_mid + 1, range_width, &range_min, &range_max) )	return;
	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );	
}

/********************************************************************
機  能  名  称 :表示中央値の値変更ボタン押下
関    数    名 : OnButtonRangeMidDec
引          数 : 
戻    り    値 : 
機          能 : １減少
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonRangeMidDec() 
{
	float	range_max, range_min, range_mid, range_width;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , &range_mid ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ))		return;	
		if(!set_min_max_from_mid_width( range_mid - 1, range_width, &range_min, &range_max) )	return;
	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );		
}

/********************************************************************
機  能  名  称 :表示中央値の値変更ボタン押下
関    数    名 : OnButtonRangeMidHlf
引          数 : 
戻    り    値 : 
機          能 : 値を半分にする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonRangeMidHlf() 
{
	float	range_max, range_min, range_mid, range_width;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , &range_mid ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ))		return;	
		if(!set_min_max_from_mid_width( range_mid / 2, range_width, &range_min, &range_max) )	return;
	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );			
}

/********************************************************************
機  能  名  称 :表示中央値の値変更ボタン押下
関    数    名 : OnButtonRangeMidDbl
引          数 : 
戻    り    値 : 
機          能 : 値を２倍にする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonRangeMidDbl() 
{
	float	range_max, range_min, range_mid, range_width;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , &range_mid ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ))		return;	
		if(!set_min_max_from_mid_width( range_mid * 2, range_width, &range_min, &range_max) )	return;
	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );		
}

/********************************************************************
機  能  名  称 :表示幅の値変更ボタン押下
関    数    名 : OnButtonRangeWidthInc
引          数 : 
戻    り    値 : 
機          能 : １増加
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonRangeWidthInc() 
{
	float	range_max, range_min, range_mid, range_width;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , &range_mid ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ))		return;	
		if(!set_min_max_from_mid_width( range_mid, range_width + 1, &range_min, &range_max) )	return;
	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );		
}

/********************************************************************
機  能  名  称 :表示幅の値変更ボタン押下
関    数    名 : OnButtonRangeWidthDec
引          数 : 
戻    り    値 : 
機          能 : １減少
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonRangeWidthDec() 
{
	float	range_max, range_min, range_mid, range_width;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , &range_mid ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ))		return;	
		if(!set_min_max_from_mid_width( range_mid, range_width - 1, &range_min, &range_max) )	return;
	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );			
}

/********************************************************************
機  能  名  称 :表示幅の値変更ボタン押下
関    数    名 : OnButtonRangeWidthHlf
引          数 : 
戻    り    値 : 
機          能 : 値を半分にする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonRangeWidthHlf() 
{
	float	range_max, range_min, range_mid, range_width;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , &range_mid ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ))		return;	
		if(!set_min_max_from_mid_width( range_mid, range_width / 2 , &range_min, &range_max) )	return;
	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );		
}

/********************************************************************
機  能  名  称 :表示幅の値変更ボタン押下
関    数    名 : OnButtonRangeWidthDbl
引          数 : 
戻    り    値 : 
機          能 : 値を２倍にする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnButtonRangeWidthDbl() 
{
	float	range_max, range_min, range_mid, range_width;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , &range_mid ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ))		return;	
		if(!set_min_max_from_mid_width( range_mid, range_width * 2 , &range_min, &range_max) )	return;
	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );		
}

/********************************************************************
機  能  名  称 :「SET」ボタンが押された
関    数    名 : OnHistgramSet
引          数 : 
戻    り    値 : 
機          能 : 表示レンジを反映して表示画像データを更新しヒストグラム再描画
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnHistgramSet() 
{
		float	range_min, range_max;

		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ) )	return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ) )	return;

	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );
}

/********************************************************************
機  能  名  称 :度数最大値を表すエディットボックスの値変更
関    数    名 : OnChangeHistgramRangeMin
引          数 : 
戻    り    値 : 
機          能 : エディットボックスの直接編集
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnChangeHistgramRangeMin() 
{
	float	range_max, range_min, range_mid, range_width;

		if( !flg_str_from_key )		return;//キーボードからの文字入力以外には応答しない
	flg_str_from_key = false;

		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ) )	return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ) )	return;

		if( range_max < range_min)		return;

	range_mid   = (float)( (range_max + range_min)/2.0 );
	range_width = (float)( (range_max - range_min)/2.0 );

	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID   , range_mid );
	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH   , range_width );
}

/********************************************************************
機  能  名  称 :表示最大値を表すエディットボックスの値変更
関    数    名 : OnChangeHistgramRangeMax
引          数 : 
戻    り    値 : 
機          能 : エディットボックスの直接編集
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnChangeHistgramRangeMax() 
{
	float	range_max, range_min, range_mid, range_width;

		if( !flg_str_from_key )		return;//キーボードからの文字入力以外には応答しない
	flg_str_from_key = false;

		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ) )	return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ) )	return;

		if( range_max < range_min)		return;

	range_mid   = (float)( (range_max + range_min)/2.0 );
	range_width = (float)( (range_max - range_min)/2.0 );

	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID   , range_mid );
	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH   , range_width );	
}

/********************************************************************
機  能  名  称 :表示中央値を表すエディットボックスの値変更
関    数    名 : OnChangeHistgramRangeMid
引          数 : 
戻    り    値 : 
機          能 : エディットボックスの直接編集
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnChangeHistgramRangeMid() 
{
	float	range_max, range_min, range_mid, range_width;

		if( !flg_str_from_key )		return;//キーボードからの文字入力以外には応答しない
	flg_str_from_key = false;

		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID   , &range_mid ) )	return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ) )	return;
		if(!set_min_max_from_mid_width( range_mid, range_width , &range_min, &range_max) )	return;

	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX   , range_max );
	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN   , range_min );
}

/********************************************************************
機  能  名  称 :表示幅を表すエディットボックスの値変更
関    数    名 : OnChangeHistgramRangeWidth
引          数 : 
戻    り    値 : 
機          能 : エディットボックスの直接編集
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnChangeHistgramRangeWidth() 
{
	float	range_max, range_min, range_mid, range_width;

		if( !flg_str_from_key )		return;//キーボードからの文字入力以外には応答しない
	flg_str_from_key = false;

		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID   , &range_mid ) )	return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ) )	return;
		if( !set_min_max_from_mid_width( range_mid, range_width , &range_min, &range_max) )	return;

	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX   , range_max );
	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN   , range_min );
}

/********************************************************************
機  能  名  称 :「Max-Min」ボタン押下
関    数    名 : OnHistgramSetMinMax
引          数 : 
戻    り    値 : 
機          能 : 表示範囲を「最大　－　最小」に設定して表示画像データを更新しヒストグラム再描画
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnHistgramSetMinMax() 
{
	float min, max;
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du == NULL)	return;

	p_du->GetDataMinMax(&min, &max);
	API.SetDispRange( CURRENT_IMAGE, min, max);
}

/********************************************************************
機  能  名  称 :「ave±sdv2」ボタン押下
関    数    名 : OnHistgramSetMinMax
引          数 : 
戻    り    値 : 
機          能 : 表示範囲を「平均値　±　標準偏差×2」に設定して表示画像データを更新しヒストグラム再描画
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnHistgramSetAve() 
{
	float min, max;
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du == NULL)	return;

		if(!set_min_max_from_mid_width( p_du->Average , p_du->Stdiv * 2 , &min, &max) )	return;
	API.SetDispRange( CURRENT_IMAGE, min, max);	
}

/********************************************************************
機  能  名  称 :「0～256」ボタン押下
関    数    名 : OnHistgramSetMinMax
引          数 : 
戻    り    値 : 
機          能 : 表示範囲を「0～256」に設定して表示画像データを更新しヒストグラム再描画
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnHistgramSet0256() 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du == NULL)	return;

	API.SetDispRange( CURRENT_IMAGE, 0, 256);	
}

/********************************************************************
機  能  名  称 :「-π～π」ボタン押下
関    数    名 : OnHistgramSetMinMax
引          数 : 
戻    り    値 : 
機          能 : 表示範囲を「-π～π」に設定して表示画像データを更新しヒストグラム再描画
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnHistgramSetPiPi() 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du == NULL)	return;

	API.SetDispRange( CURRENT_IMAGE, -(float)PI, (float)PI);	
}


/********************************************************************
機  能  名  称 : マウス右ボタン押下
関    数    名 : OnHistgramSetMinMax
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	CRect	histgram_field( 0,0, HISTGRAM_W, HISTGRAM_H );//ヒストグラムを描く領域を表す矩形

	if( histgram_field.PtInRect(point) )
	{
		is_draging_r=true;//右ドラッグ中のフラグを立てる

		hist_value_drag_start = point.x;//右ドラッグ開始座標
	}

	CDialog::OnRButtonDown(nFlags, point);
}


/********************************************************************
機  能  名  称 : マウス右ボタン押し上げ
関    数    名 : OnRButtonUp
引          数 : 
戻    り    値 : 
機          能 : ドラッグ終了。表示範囲をドラッグ区間にあわせる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CHistgramDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	CRect	histgram_field( 0,0, HISTGRAM_W, HISTGRAM_H );//ヒストグラムを描く領域を表す矩形

	if( histgram_field.PtInRect(point) && is_draging_r==true)
	{
		CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du)
		{
			int sx=0, ex=0;
			if(hist_value_drag_start > point.x){
				sx = point.x+1;	ex = hist_value_drag_start;
			}else if(hist_value_drag_start < point.x){
				ex = point.x-1;	sx = hist_value_drag_start;
			}

			if(sx<ex)
			{
				float coef = (float)((p_du->BrightMax - p_du->BrightMin)/256.0);
				float new_min = sx * coef;
				float new_max = ex * coef;

				CString str;
				str.Format("%f～%fの濃度範囲で表示します", new_min, new_max);
				if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONINFORMATION ) == IDOK ){
					API.SetDispRange( CURRENT_IMAGE, new_min, new_max);
				}

				
			}
		}

	}

	is_draging_r=false;//右ドラッグ中のフラグを寝かす
	DrawHistgram();//ヒストグラム再描画

	CDialog::OnRButtonUp(nFlags, point);
}
