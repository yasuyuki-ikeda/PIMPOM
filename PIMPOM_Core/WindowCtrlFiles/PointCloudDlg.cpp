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
//CPointCloudDlg
//三次元点群表示ウィンドウ
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
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
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

extern CPimpomAPI	API;

// *************************************
//         関    数    参    照         
// *************************************


//////////////////////////////////////////////////////////////////////////////////////////
//  メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////

CPointCloudDlg::CPointCloudDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPointCloudDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPointCloudDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CPointCloudDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPointCloudDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


//////////////////////////////////////////////////////////////////////////////////////////
//  メッセージ ハンドラ
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPointCloudDlg, CDialog)
	//{{AFX_MSG_MAP(CPointCloudDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_BN_CLICKED(IDC_CHECK_POINT_CLOUD_CTRL_ROTATE, OnPointCloudCtrlRotate)
	ON_BN_CLICKED(IDC_CHECK_POINT_CLOUD_CTRL_SCALE, OnPointCloudCtrlScale)
	ON_BN_CLICKED(IDC_CHECK_POINT_CLOUD_CTRL_TRANSLATE, OnPointCloudCtrlTranslate)
	//}}AFX_MSG_MAP
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
BOOL CPointCloudDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	MoveWindow( 0,0, POINT_CLOUD_W + 5, POINT_CLOUD_H + 30 );
	sampling_size = CSize(1,1);
	Rendering( sampling_size );
	is_druging = false;
	is_draging_mbutton = false;
	API.SetCheck( m_hWnd , IDC_CHECK_POINT_CLOUD_CTRL_ROTATE , true );

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/********************************************************************
機  能  名  称 : スクリーン用ビットマップの初期化
関    数    名 : init_bmp
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPointCloudDlg::init_bmp()
{
	p_screen_bmp_info = (LPBITMAPINFO)new char[ sizeof(BITMAPINFOHEADER) + 1024 ];

    p_screen_bmp_info->bmiHeader.biSize = 40;
    p_screen_bmp_info->bmiHeader.biWidth = POINT_CLOUD_W;
    p_screen_bmp_info->bmiHeader.biHeight = -POINT_CLOUD_H;
    p_screen_bmp_info->bmiHeader.biPlanes = 1;
    p_screen_bmp_info->bmiHeader.biBitCount = 8;
    p_screen_bmp_info->bmiHeader.biCompression = 0;
    p_screen_bmp_info->bmiHeader.biSizeImage   = 0;
    p_screen_bmp_info->bmiHeader.biXPelsPerMeter = 0;
    p_screen_bmp_info->bmiHeader.biYPelsPerMeter = 0;
    p_screen_bmp_info->bmiHeader.biClrUsed = 0;
    p_screen_bmp_info->bmiHeader.biClrImportant = 0;

	p_screen_bmp_data = new unsigned char[ POINT_CLOUD_W*POINT_CLOUD_H ];
	ZeroMemory( p_screen_bmp_data , POINT_CLOUD_W*POINT_CLOUD_H );
}

/********************************************************************
機  能  名  称 : ウインドウの破棄
関    数    名 : OnDestroy
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPointCloudDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	CDialog::OnDestroy();

	delete[]	p_transformed_x;
	delete[]	p_transformed_y;
	delete[]	p_transformed_z;
	delete[]	p_original_z;
	delete[]	pZbuffer;
	delete[]	p_screen_bmp_data;
	delete[]	p_screen_bmp_info;
	delete[]	p_mask;
	delete[]	p_pixel_flg;
	delete[]	p_brightness;	
}

/********************************************************************
機  能  名  称 : 再描画
関    数    名 : OnPaint
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPointCloudDlg::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	
	draw_screen();
}

/********************************************************************
機  能  名  称 : スクリーン用ビットマップを描画
関    数    名 : draw_screen
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPointCloudDlg::draw_screen()
{
	HDC hdc = ::GetDC(m_hWnd);
	::StretchDIBits(hdc, 0, 25, POINT_CLOUD_W, POINT_CLOUD_H,
						0, 0, POINT_CLOUD_W, POINT_CLOUD_H, 
						p_screen_bmp_data, p_screen_bmp_info, DIB_RGB_COLORS, SRCCOPY);
	::ReleaseDC(m_hWnd, hdc);	

	draw_scale();
}

/********************************************************************
機  能  名  称 : 点群データ作成
関    数    名 : CreatePointCloud
引          数 : CDataUnit *p_du	(in)元となるXYZ型データ
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CPointCloudDlg::CreatePointCloud(CDataUnit *p_du)
{
		if(p_du == NULL)	return	IDCANCEL;
		if(p_du->DataType != XYZ_FORMAT)	return	IDCANCEL;

	data_size = p_du->DataSize;
	bmp_data_width = p_du->DispBufferRow;
	p_transformed_x = new float[ data_size.cx*data_size.cy ];
	p_transformed_y = new float[ data_size.cx*data_size.cy ];
	p_transformed_z = new float[ data_size.cx*data_size.cy ];
	p_original_z = new float[ data_size.cx*data_size.cy ];
	pZbuffer = new float[ POINT_CLOUD_W*POINT_CLOUD_H ];
	p_pixel_flg = new unsigned char[ POINT_CLOUD_W*POINT_CLOUD_H ];
	p_mask = new unsigned char[ data_size.cx*data_size.cy ];
	p_brightness = new unsigned char[ bmp_data_width*data_size.cy ];

	init_bmp();
	memcpy( p_screen_bmp_info->bmiColors , p_du->pDispBufferBmpInfo->bmiColors , sizeof(RGBQUAD)*256 );
	memcpy( p_brightness , p_du->pDispBuffer , bmp_data_width*data_size.cy );

	scale = (float)0.7;
	translate = CPoint(0,0);

	memcpy( p_transformed_x, p_du->pFloatData , data_size.cx*data_size.cy*sizeof(float) ); 
	memcpy( p_transformed_y, p_du->pFloatData + data_size.cx*data_size.cy	,	data_size.cx*data_size.cy*sizeof(float) ); 
	memcpy( p_transformed_z, p_du->pFloatData + data_size.cx*data_size.cy*2,	data_size.cx*data_size.cy*sizeof(float) ); 
	memcpy( p_original_z , p_transformed_z, data_size.cx*data_size.cy*sizeof(float) );
	memcpy( p_mask , p_du->pMaskData , data_size.cx*data_size.cy );

	return	DoModal();
}

/********************************************************************
機  能  名  称 : 平行透視法で点群をレンダリング
関    数    名 : Rendering
引          数 : CSize sampling	(in)点群データのサンプリング間隔
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPointCloudDlg::Rendering(CSize sampling)
{
	float			*px, *py, *pz , *pz0;
	unsigned char	*p_ma, *p_bri;
	long			i, j, addr, x, y;


	ZeroMemory( p_screen_bmp_data , POINT_CLOUD_W * POINT_CLOUD_H);//スクリーンを表すビットマップの中身をゼロクリア
	ZeroMemory( p_pixel_flg , POINT_CLOUD_W * POINT_CLOUD_H);//スクリーンの画素にすでに値が書き込まれていることを表すフラグをゼロクリア

		for(j=0 ; j<data_size.cy ; j+= sampling.cy){//スクリーンの縦方向のループ（一点間隔でサンプリング）
			px = p_transformed_x + j*data_size.cx;
			py = p_transformed_y + j*data_size.cx;
			pz = p_transformed_z + j*data_size.cx;
			pz0 = p_original_z + j*data_size.cx;
			p_ma = p_mask + j*data_size.cx;
			p_bri = p_brightness + (data_size.cy -1 -j) * bmp_data_width;

				for( i=0 ; i<data_size.cx ; i+=sampling.cx){//スクリーンの横方向のループ（一点間隔でサンプリング）
					x = (long)((*px) * scale + POINT_CLOUD_W/2 + translate.x);
					y = (long)((*py) * scale + POINT_CLOUD_H/2 + translate.y);

						if( *p_ma == 0 )
						if( ( x>=0  &&  x<POINT_CLOUD_W)   &&   ( y>=0  &&  y<POINT_CLOUD_H) ){//座標が画面内に入っているかどうか
							addr =  x + (POINT_CLOUD_H -1 - y  ) * POINT_CLOUD_W;
								
								//陰点消去
								if( *(p_pixel_flg + addr) == 0 ){//スクリーンの画素にはじめて値を描きこむ場合
									*( p_screen_bmp_data + addr ) = *p_bri ;
									*( p_pixel_flg + addr) = 1;
									*( pZbuffer + addr) = *pz;//zバッファに奥行き情報をたくわえる

								}else{ //スクリーンの画素に上書きする場合（陰点消去が必要）

									if( *(pZbuffer + addr) < *pz ){//どちらの値を表示するかZバッファをしらべて判断する
										*( p_screen_bmp_data + addr ) = *p_bri ;
										*( p_pixel_flg + addr) = 1;
										*( pZbuffer + addr) = *pz;
									}
								}
						}

					px +=sampling.cx;
					py +=sampling.cx;
					pz +=sampling.cx;
					pz0 +=sampling.cx;
					p_ma +=sampling.cx;
					p_bri +=sampling.cx;
				}
		}
	draw_screen();
}

/********************************************************************
機  能  名  称 : スケール(縮尺)を描く
関    数    名 : draw_scale
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPointCloudDlg::draw_scale()
{
	CPoint	scale_origin( POINT_CLOUD_W - 300 , POINT_CLOUD_H - 20);
	CSize	scale_size( 300, 25 );
	CDC		*pDC = GetDC();
	float	n, scale_length;
	CString	str;
	CFont	font,*p_old_font;

	n = (float)log10( ( 300 - 20 ) / scale );
	scale_length = (float)pow( (double)10, (int)n );
	str.Format( "%f mm" , scale_length );

	scale_origin.x = (long)(POINT_CLOUD_W - (scale_length*scale + 20) - 10);
	scale_size.cx = (long)(scale_length*scale + 20);

	font.CreateFontIndirect(&API.default_font);
	p_old_font = pDC->SelectObject(&font);

	pDC->Rectangle( scale_origin.x , scale_origin.y , scale_origin.x + scale_size.cx , scale_origin.y + scale_size.cy );
	pDC->TextOut( (int)(scale_origin.x + 10 + scale_length*scale/2 - 15), scale_origin.y + 5 , str, 6);
	pDC->TextOut( (int)(scale_origin.x + 10 + scale_length*scale/2 + 16), scale_origin.y + 5 , CString("mm") );
	pDC->MoveTo( scale_origin.x + 10 , scale_origin.y + 20 );
	pDC->LineTo( (int)(scale_origin.x + 10 + scale_length*scale) , scale_origin.y + 20 );
	pDC->MoveTo( scale_origin.x + 10 , scale_origin.y + 20 );
	pDC->LineTo( scale_origin.x + 10 , scale_origin.y + 17 );
	pDC->MoveTo( (int)(scale_origin.x + 10 + scale_length*scale) , scale_origin.y + 20 );
	pDC->LineTo( (int)(scale_origin.x + 10 + scale_length*scale) , scale_origin.y + 17 );
	
	pDC->SelectObject(p_old_font);
	ReleaseDC(pDC);
	font.DeleteObject();
}

/********************************************************************
機  能  名  称 : 点群をY軸周りに回転する
関    数    名 : RotationPhai
引          数 : float	phai	(in)回転角度[rad]
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPointCloudDlg::RotationPhai(float phai)
{
	int i;
	double	temp;
	double	sin_phai, cos_phai;
	float	*px, *pz;

	sin_phai = sin( phai );
	cos_phai = cos( phai );

	px = p_transformed_x;
	pz = p_transformed_z;
		for(i=0  ;  i<data_size.cx*data_size.cy  ;  i++){
			//回転させる
			temp = cos_phai * (*px) - sin_phai * (*pz);
			*pz = (float)(sin_phai * (*px) + cos_phai * (*pz));
			*px = (float)temp;

			px++;
			pz++;
		}	
}

/********************************************************************
機  能  名  称 : 点群をX軸周りに回転する
関    数    名 : RotationTheta
引          数 : float	theta	(in)回転角度[rad]
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPointCloudDlg::RotationTheta(float theta)
{
	int i;
	double	temp;
	double	sin_theta, cos_theta;
	float	*py, *pz;

	sin_theta = sin( theta );
	cos_theta = cos( theta );

	py = p_transformed_y ;
	pz = p_transformed_z ;
		for(i=0  ;  i<data_size.cx*data_size.cy  ;  i++){
			//回転させる
			temp = cos_theta * (*py) - sin_theta * (*pz);
			*pz = (float)(sin_theta * (*py) + cos_theta * (*pz));
			*py = (float)temp;

			py++;
			pz++;
		}
}

/********************************************************************
機  能  名  称 : 3Dコントロール方法の取得
関    数    名 : get_ctrl_type
引          数 : 
戻    り    値 : 0:回転   1:拡大/縮小   2:平行移動
機          能 : ラジオボタンの値を取得
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
short CPointCloudDlg::get_ctrl_type()
{
	if (is_druging)
	{//左ボタンドラッグ
		if (API.GetCheck(m_hWnd, IDC_CHECK_POINT_CLOUD_CTRL_ROTATE)) {//回転
			return	0;

		}
		else if (API.GetCheck(m_hWnd, IDC_CHECK_POINT_CLOUD_CTRL_SCALE)) {//拡大/縮小
			return	1;

		}
		else if (API.GetCheck(m_hWnd, IDC_CHECK_POINT_CLOUD_CTRL_TRANSLATE)) {//平行移動
			return	2;
		}
	}
	else if(is_draging_mbutton)
	{//中央ボタンドラッグ
		return	2;
	}


	return 0;
}

/********************************************************************
機  能  名  称 : 3Dコントロール方法の設定
関    数    名 : set_ctrl_type
引          数 : short type		(in)0:回転   1:拡大/縮小   2:平行移動
戻    り    値 : 
機          能 : ラジオボタンの値を設定
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPointCloudDlg::set_ctrl_type(short type)
{
	bool	ctrl_flg[3];//コントロールタイプ選択ボタンの状態を決めるフラグ

		for(int n=0 ; n<3 ; n++){//type の値によってフラグをかえる
			if(n == type)		ctrl_flg[n] = true;
			else				ctrl_flg[n] = false;
		}

	//フラグの値でコントロールタイプ選択ボタンの状態を決める
	API.SetCheck( m_hWnd , IDC_CHECK_POINT_CLOUD_CTRL_ROTATE , ctrl_flg[0] );
	API.SetCheck( m_hWnd , IDC_CHECK_POINT_CLOUD_CTRL_SCALE  , ctrl_flg[1] );
	API.SetCheck( m_hWnd , IDC_CHECK_POINT_CLOUD_CTRL_TRANSLATE , ctrl_flg[2] );
}

/********************************************************************
機  能  名  称 : マウスの左ボタンが押された
関    数    名 : OnLButtonDown
引          数 : 
戻    り    値 : 
機          能 : マウスドラッグの開始
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPointCloudDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	GetCapture();//マウスキャプチャー
	is_druging = true;	//ドラッグ中を表すフラグを立てる
	mouse_down_point = point;//マウスを押した座標を記憶する
}

/********************************************************************
機  能  名  称 : マウスの左ボタンが離された
関    数    名 : OnLButtonUp
引          数 : 
戻    り    値 : 
機          能 : マウスドラッグの終了
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPointCloudDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();//マウスキャプチャーを開放する
	is_druging = false;//ドラッグ中のフラグを倒す
	Rendering( sampling_size );	//スクリーンに描画する
}

/********************************************************************
機  能  名  称 :マウスの中央ボタンが押された
関    数    名 : OnMButtonDown
引          数 :
戻    り    値 :
機          能 : 画像表示位置の移動
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPointCloudDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	GetCapture();//マウスをキャプチャーする
	is_draging_mbutton = true;//ドラッグ中を表すフラグを立てる
	mouse_down_point = point;//マウスを押した座標を記憶する
}

/********************************************************************
機  能  名  称 :マウスの中央ボタンが離された
関    数    名 : OnMButtonUp
引          数 :
戻    り    値 :
機          能 : マウスドラッグ終了
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPointCloudDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();//マウスキャプチャーを開放する
	is_draging_mbutton = false;
	Rendering(sampling_size);	//スクリーンに描画する
}

/********************************************************************
機  能  名  称 :マウスホイールの回転
関    数    名 : OnMouseWheel
引          数 :
戻    り    値 :
機          能 : 拡大・縮小
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
BOOL CPointCloudDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//スケールを変更して描画
	if (zDelta>0) {
		scale *= 1.1;//10%拡大
	}
	else {
		scale *= 0.9;//10%縮小
	}

	Rendering(sampling_size);	//スクリーンに描画する

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


/********************************************************************
機  能  名  称 : マウスポインタが移動した
関    数    名 : OnMouseMove
引          数 : 
戻    り    値 : 
機          能 : マウスドラッグ中
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPointCloudDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	int		dx,dy;
	double	temp;

		if( !is_druging && !is_draging_mbutton)	return;//ドラッグ中でない場合は何もしない
		if( point.y<20 )	return;//マウスポインタがy座標が20より小さい（コントロールタイプ選択ボタンの）領域ではなにもしない

	//ドラッグによる移動量計算
	dx = mouse_down_point.x - point.x;
	dy = -( mouse_down_point.y - point.y );

		switch( get_ctrl_type() ){
			case 0:	//回転
				RotationPhai( (float)(dx * 0.02) );
				RotationTheta( (float)(dy * 0.02) );
				break;

			case 1://拡大・縮小
					if( (temp = scale + dy*0.01) > 0 )		scale = (float)temp;//縮尺が小さくなり過ぎないようにする
				break;

			case 2://水平移動
				translate += CPoint( -dx, -dy );
				break;
		}
	Rendering( CSize(6,6) );

	mouse_down_point = point;
	
	CDialog::OnLButtonUp(nFlags, point);
}

/********************************************************************
機  能  名  称 : 「回転」ボタンが押された
関    数    名 : OnPointCloudCtrlRotate
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPointCloudDlg::OnPointCloudCtrlRotate() 
{
	set_ctrl_type(0);
}

/********************************************************************
機  能  名  称 : 「拡大/縮小」ボタンが押された
関    数    名 : OnPointCloudCtrlScale
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPointCloudDlg::OnPointCloudCtrlScale() 
{
	set_ctrl_type(1);
}

/********************************************************************
機  能  名  称 : 「平行移動」ボタンが押された
関    数    名 : OnPointCloudCtrlTranslate
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPointCloudDlg::OnPointCloudCtrlTranslate() 
{
	set_ctrl_type(2);
}


