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
//CDataDlg
//画像メモリ一覧表示ウィンドウ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "DataDlg.h"

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
extern	CPimpomAPI	API;

// *************************************
//         関    数    参    照         
// *************************************

//////////////////////////////////////////////////////////////////////////////////////////
//  メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////

CDataDlg::CDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}

/********************************************************************
機  能  名  称 : 画像メモリにデータが存在しないかどうかを確かめる
関    数    名 : DataExistMessage
引          数 : int		image_num	(in)画像メモリ番号 
				 CString	mes			(in)image_numにデータが存在する場合に表示するメッセージ
戻    り    値 : 存在しない、もしくは上書きする場合は、falseを返す
機          能 : 画像メモリにデータを書き込む前に内容確認するときに使う
				 image_numにデータが存在する場合は、上書き問い合わせダイアログを出し、ここでOKを押すとfalseを返す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CDataDlg::DataExistMessage(CString mes, int image_num)
{
	CDataUnit *pDU;
	CString  str ;

	pDU = API.GetDataUnit(image_num);//現在選択中の画像データを取得
	if( !pDU )		return	false;//指定したメモリにデータが存在しない

	switch( pDU->DataType ){
	case BYTE_FORMAT:
		str.LoadString(IDS_DATATYPE_BYTE);
		break;
	case FLOAT_FORMAT:
		str.LoadString(IDS_DATATYPE_FLOAT);
		break;
	case COMPLEX_FORMAT:
		str.LoadString(IDS_DATATYPE_COMPLEX);
		break;
	case THREE_D_FORMAT:
		str.LoadString(IDS_DATATYPE_3D);
		break;
	case XY_FORMAT:
		str.LoadString(IDS_DATATYPE_XY);
		break;
	case XYZ_FORMAT:
		str.LoadString(IDS_DATATYPE_XYZ);
		break;
	case FLOAT_3D_FORMAT:
		str.LoadString(IDS_DATATYPE_F3D);
		break;
	case SHORT16_FORMAT:
		str.LoadString(IDS_DATATYPE_SHORT);
		break;
	case WORD_FORMAT:
		str.LoadString(IDS_DATATYPE_WORD);
		break;
	case LONG32_FORMAT:
		str.LoadString(IDS_DATATYPE_LONG);
		break;
	case DWORD_FORMAT:
		str.LoadString(IDS_DATATYPE_DWORD);
		break;
	case RGB_FORMAT:
		str.LoadString(IDS_DATATYPE_RGB);
		break;
	case RGB_3D_FORMAT:
		str.LoadString(IDS_DATATYPE_RGB3D);
		break;
	}

	str = "この画像メモリには" + str + pDU->DataName + "が入っています。\n" + mes;
	if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONQUESTION ) == IDOK ){//上書きを選択
		//backup_stock_image();
		return	false;
	}else{
		return	true;
	}
}

/********************************************************************
機  能  名  称 : 現在選択中の画像メモリ番号を変更する
関    数    名 : ChangeCurrentDataNumber
引          数 : long number　(in)新たに選択する画像メモリ番号
戻    り    値 : 以前に選択していた画像メモリ番号を返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
long CDataDlg::ChangeCurrentDataNumber(long number)
{
	long	old_number = API.GetCurrentImageNumber();
	API.SetCurrentImageNumber(number);

		if( old_number != number ){//選択している画像番号が変わったときの処理
			if(API.pPIMMOMDlg){
				API.pPIMMOMDlg->StopMovie();//動画停止
			}
		}

	draw_current_rect( old_number, API.default_bg_color );//以前に選択していた画像の周りを背景色で塗る（矩形を上書きする）
	draw_current_rect( API.GetCurrentImageNumber(), RGB(0,0,0) );//新しく選択した画像の周りに矩形を描く
	
	return	old_number;
}



///////////////////////////////////////////////////////////////////////////
//サムネイル描画に関するメンバ関数

/********************************************************************
機  能  名  称 : 背景色で矩形描画
関    数    名 : draw_background_rect
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::draw_background_rect(CRect rct)
{
	HDC hdc = ::GetDC(m_hWnd);
	HPEN hpen = ::CreatePen(PS_SOLID, 1, API.default_bg_color);
	HBRUSH hbrush = ::CreateSolidBrush(API.default_bg_color);
	::SelectObject(hdc, hpen);
	::SelectObject(hdc, hbrush);
	::Rectangle(hdc, rct.left, rct.top, rct.right, rct.bottom);
	::DeleteObject(hbrush);
	::DeleteObject(hpen);
	::ReleaseDC(m_hWnd, hdc);
}


/********************************************************************
機  能  名  称 : 全ての画像データのサムネイルを描画する
関    数    名 : DrawAllImages
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::DrawAllImages()
{
	//背景描画
	RECT clientRct;
	::GetClientRect(m_hWnd, &clientRct);
	draw_background_rect(clientRct);

	for(int n=0 ; n<API.GetDataUnitNumber() ; n++){
		DrawImage(n);
	}
}

/********************************************************************
機  能  名  称 : 指定した画像データのサムネイルを描画する
関    数    名 : DrawImage
引          数 : long number	(in)画像メモリ番号
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::DrawImage(long number)
{
	if (number >= API.GetDataUnitNumber())	return;

	pp_image_disp_small[number]->DrawAdjustedImage( API.GetDataUnit(number) , false);
	draw_image_info( number );
}


/********************************************************************
機  能  名  称 : 文字列を表示長さでクリップする
関    数    名 : clip_string
引          数 : CDC *pdc       (in)デバイスコンテキスト（フォントは事前に設定しておくこと）
                 CString str	(in)文字列
				 int max_width  (in)表示長さ
戻    り    値 : クリップされた文字列
機          能 : 
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
CString CDataDlg::clip_string(CDC *pdc, CString str, int max_width)
{
	CString clippedStr;

	for (int len = str.GetLength(); len > 0; len--)
	{
		clippedStr = str.Left(len);
		CSize strSize = pdc->GetTextExtent(clippedStr);
		if (strSize.cx < max_width)
		{
			break;
		}
	}

	return clippedStr;
}

/********************************************************************
機  能  名  称 : 指定した画像データの情報（メモリ番号、データ形式、データ名）をサムネイル横に表示する
関    数    名 : draw_image_info
引          数 : long number	(in)画像メモリ番号
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::draw_image_info(long number)
{	
	CString		type_name, str_image_number;
	CDC			*p_dc;
	CFont		font, *p_old_font;
	CRect		image_area;

	//小イメージのImageFieldを表す矩形領域を取得する
	image_area = pp_image_disp_small[number]->GetFieldRect();
	//デバイスコンテキストの取得とフォントの指定する
	p_dc = GetDC();
	font.CreateFontIndirect(&API.default_font);
	p_old_font= (CFont*)p_dc->SelectObject( &font );

	//画像番号を表示
		if(number<=9)	str_image_number.Format(" %d", number);//1桁
		else		str_image_number.Format("%d", number);//2桁

	p_dc->TextOut(	image_area.right - 13 , image_area.top , str_image_number );


	//背景色で矩形を描画（データ名の下地）
	CRect name_rct(image_area.left, image_area.bottom + 5, image_area.right, image_area.bottom + 5 + API.default_font.lfHeight);
	draw_background_rect(name_rct);

		if( API.GetDataUnit(number) )
		{//データが無ければデータ形式,データ名は表示できない

			//データ名の表示
			CString dispName = clip_string(p_dc, API.GetDataUnit(number)->DataName, image_area.Width() );
			p_dc->SetBkColor(API.default_bg_color);//テキストの背景色を決定
			p_dc->TextOut(image_area.left, image_area.bottom + 5, dispName);//データ名を描画

			

			//データタイプ名の表示
				switch( API.GetDataUnit(number)->DataType ){
					case BYTE_FORMAT:
						p_dc->SetBkColor( RGB(255,100,100) );
						type_name = _T("BYTE");
						break;

					case FLOAT_FORMAT:
						p_dc->SetBkColor( RGB(100,100,255) );
						type_name = _T("FLOAT" );
						break;

					case THREE_D_FORMAT:
						p_dc->SetBkColor( RGB(100,255,100) );
						type_name = _T("3D");
						break;

					case FLOAT_3D_FORMAT:
						p_dc->SetBkColor( RGB(100,255,250) );
						type_name = _T("F3D");
						break;

					case COMPLEX_FORMAT:
						p_dc->SetBkColor( RGB(255,255,100) );
						type_name = _T("COMP" );
						break;

					case XY_FORMAT:
						p_dc->SetBkColor( RGB(255,100,255) );
						type_name = _T("XY");
						break;

					case XYZ_FORMAT:
						p_dc->SetBkColor( RGB(255,255,255) );
						type_name = _T("XYZ");
						break;

					case SHORT16_FORMAT:
						p_dc->SetBkColor( RGB(255,100,100) );
						type_name = _T("SHORT");
						break;

					case WORD_FORMAT:
						p_dc->SetBkColor( RGB(255,100,100) );
						type_name = _T("WORD");
						break;

					case LONG32_FORMAT:
						p_dc->SetBkColor( RGB(255,100,100) );
						type_name = _T("LONG");
						break;

					case DWORD_FORMAT:
						p_dc->SetBkColor( RGB(255,100,100) );
						type_name = _T("DWORD");
						break;

					case RGB_FORMAT:
						p_dc->SetBkColor( RGB(100,100,100) );
						type_name = _T("RGB");
						break;

					case RGB_3D_FORMAT:
						p_dc->SetBkColor( RGB(100,100,100) );
						type_name = _T("RGB3D");
						break;
				}
			p_dc->TextOut(	image_area.left , image_area.top , type_name );//データタイプを描画
		}

	//オブジェクト（フォント）の開放とデバイスコンテキストの開放
	p_dc->SelectObject( p_old_font );
	ReleaseDC( p_dc );
	font.DeleteObject();
}

/********************************************************************
機  能  名  称 : 指定した画像データのサムネイル横の周りに矩形描画
関    数    名 : draw_current_rect
引          数 : long		number	(in)画像メモリ番号
				 COLORREF	color	(in)描画色
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::draw_current_rect(long number, COLORREF color)
{
	CDC			*p_dc;
	CPen		pen(PS_SOLID , 2 , color );
	CPen		*p_old_pen;
	CBrush		*p_old_brush;
	CRect		ext_area;
	
	p_dc = GetDC();//デバイスコンテキストを取得する
	if(!p_dc)	return;

	p_old_pen = (CPen*)p_dc->SelectObject( &pen );//ペンを取得する
	p_old_brush = (CBrush*)p_dc->SelectStockObject( NULL_BRUSH );//ブラシを取得する（ヌルブラシ）

	ext_area = pp_image_disp_small[number]->GetFieldRect()  +  CRect(3,3,3,3);//矩形を描く領域を設定する
	p_dc->Rectangle( ext_area );//矩形を描く

	//ペンとブラシを開放
	p_dc->SelectObject( p_old_pen );
	p_dc->SelectObject( p_old_brush );
	pen.DeleteObject();

	ReleaseDC( p_dc );//デバイスコンテキストを開放
}

/********************************************************************
機  能  名  称 : 指定した位置に表示されている画像データの番号を返す
関    数    名 : get_image_number_from_coordinate
引          数 : CPoint		coor	(in)座標
戻    り    値 : 指定した座標がどのImageFieldにも入っていない場合は-1を返す
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CDataDlg::get_image_number_from_coordinate(CPoint coor)
{
		for(int n=0 ; n<API.GetDataUnitNumber() ; n++){//表示画像番号の変更
			if( pp_image_disp_small[n]->GetFieldRect().PtInRect(coor) ){
				return	n;
			}
		}
	return	-1;
}



/////////////////////////////////////////////////////////////////////////////
// メッセージ ハンドラ
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDataDlg, CDialog)
	//{{AFX_MSG_MAP(CDataDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_DATAMENU_RENAME, &CDataDlg::OnDatamenuRename)
	ON_COMMAND(ID_DATAMENU_CONVERT_TYPE, &CDataDlg::OnDatamenuConvertType)
	ON_COMMAND(ID_DATAMENU_RESIZE, &CDataDlg::OnDatamenuResize)
	ON_COMMAND(ID_DATAMENU_DELETE, &CDataDlg::OnDatamenuDelete)
	ON_COMMAND(ID_DATAMENU_SAVEBITMAP, &CDataDlg::OnDatamenuSavebitmap)
	ON_COMMAND(ID_DATAMENU_UNDO, &CDataDlg::OnDatamenuUndo)
	ON_COMMAND(ID_DATAMENU_DISP3D, &CDataDlg::OnDatamenuDisp3d)
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
BOOL CDataDlg::OnInitDialog() 
{
	int n;
	CDialog::OnInitDialog();

	//ドラッグ＆ドロップ対応
	DragAcceptFiles();

	//背景色のブラシをつくる
	bg_brush = ::CreateSolidBrush(API.default_bg_color);

	//小イメージの配列をつくる
	pp_image_disp_small = (CImageField**)malloc(API.GetDataUnitNumber() * sizeof(CImageField**));
	if(!pp_image_disp_small)	return FALSE;


	CPoint  origin_coor(5,5);//最初の画像領域の座標
	CPoint	disp_small_coor = origin_coor;//画像領域の座標

	for(n=0 ; n<API.GetDataUnitNumber() ; n++)
	{
		//画像領域の取得
		pp_image_disp_small[n] = new CImageField( m_hWnd, API.small_image_size, disp_small_coor );
		if(!pp_image_disp_small[n])	return FALSE;

		if(n==API.GetDataUnitNumber()-1)
		{
			//このウィンドウの高さを決定
			RECT wndRct, clientRct;
			GetWindowRect(&wndRct);
			GetClientRect(&clientRct);
			int non_client_height = (wndRct.bottom - wndRct.top) - (clientRct.bottom - clientRct.top);

			WindowHeight = disp_small_coor.y + ( API.small_image_size.cy + SMALL_MARGN_Y) + non_client_height;
		}
		else
		{
			//次の画像領域の座標の算出
			disp_small_coor.x += (API.small_image_size.cx + SMALL_MARGN_X);


			//折り返し処理
			if (API.thumbneil_colum > 0 &&
				API.thumbneil_colum * (API.small_image_size.cx + SMALL_MARGN_X) < API.data_window_width )
			{//サムネイル行数が指定されている場合

				if (n%API.thumbneil_colum == API.thumbneil_colum-1) {//所定行数で折り返す
					disp_small_coor.x = origin_coor.x;
					disp_small_coor.y += (API.small_image_size.cy + SMALL_MARGN_Y);
				}
			}
			else
			{//サムネイル行数が指定されていない、またはウィンドウ内に収まらない場合

				if (disp_small_coor.x + API.small_image_size.cx >= API.data_window_width) {//画面内に収まらなくなるたびに折り返す
					disp_small_coor.x = origin_coor.x;
					disp_small_coor.y += (API.small_image_size.cy + SMALL_MARGN_Y);
				}
			}
		}
	}


	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


/********************************************************************
機  能  名  称 : ドラッグアンドドロップ
関    数    名 : OnDropFiles
引          数 : 
戻    り    値 : 
機          能 : サムネイル上に画像ファイルドロップされたら、ドロップされた画像メモリにファイルを読み込む
				　複数のファイルがドロップされたら、複数の画像メモリに順番に読み込む
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::OnDropFiles(HDROP hDropInfo)
{
	CString		strFileName;
	int			dropped_image_num;
	int		file_num = (int)DragQueryFileA( hDropInfo, -1, NULL, 0);
	POINT	drop_point;

	if (DragQueryPoint(hDropInfo, &drop_point) == 0){
		return;
	}

	dropped_image_num = get_image_number_from_coordinate(CPoint(drop_point));//ドロップされた画像メモリ番号
	if(dropped_image_num==-1){
		return;
	}

	for(int i=0; i+dropped_image_num<API.GetDataUnitNumber() && i<file_num ; i++)
	{
		UINT size = DragQueryFile( hDropInfo, i, NULL, 0) + 1;//ファイル名の長さを取得

		DragQueryFile( hDropInfo, i , strFileName.GetBuffer(size),size );
		strFileName.ReleaseBuffer();
			if( DataExistMessage("上書きしてもよろしいですか？",i + dropped_image_num) )		return;

			API.BackupDataUnit(i + dropped_image_num);//現在のデータバックアップ


		//ファイルをロードする
		if( !API.SelectAndLoadImageFile( i + dropped_image_num, strFileName) ){
			return;
		}

		API.DrawImage(i + dropped_image_num,true);
	}

	DragFinish(hDropInfo);
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
void CDataDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	for(int n=0 ; n<API.GetDataUnitNumber() ; n++){
		delete	pp_image_disp_small[n];
	}
	free( pp_image_disp_small);

	DeleteObject( bg_brush );
}

/********************************************************************
機  能  名  称 :キーボードからの入力
関    数    名 : PreTranslateMessage
引          数 : 
戻    り    値 : 
機          能 : カーソルキー：カーソルの移動
				 Delキー：選択中の画像メモリクリア
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
BOOL CDataDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN ){
		CPoint	point;
		CString	str;

		switch(pMsg->wParam){
			case VK_RETURN://Enter
				return  true;

			case VK_ESCAPE://Esc
				return  true;

			case VK_UP://↑
				GetCursorPos( &point ) ;
				SetCursorPos(point.x , point.y - 1);
				break;

			case VK_DOWN://↓
				GetCursorPos( &point ) ;
				SetCursorPos(point.x , point.y + 1);
				break;

			case VK_LEFT://←
				GetCursorPos( &point ) ;
				SetCursorPos(point.x - 1 , point.y);
				break;

			case VK_RIGHT://→
				GetCursorPos( &point ) ;
				SetCursorPos(point.x + 1, point.y);
				break;

			case VK_DELETE://Del
				str.Format("画像メモリ %d のデータを削除します．よろしいですか？" , API.GetCurrentImageNumber());
				if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONQUESTION ) == IDOK ){
					API.DeleteDataUnit(CURRENT_IMAGE);
				}
				break;

			case 'C'://Ctrl + C
				if ((GetKeyState(VK_CONTROL) & 0x8000) != 0) {
					API.CopyToClipbord();
				}
				break;

			case 'V'://Ctrl + V
				if ((GetKeyState(VK_CONTROL) & 0x8000) != 0) 
				{
					if (!DataExistMessage("上書きしてもよろしいですか？", CURRENT_IMAGE))
					{
						API.BackupDataUnit(CURRENT_IMAGE);//現在のデータバックアップ
						API.PasteFromClipbord(API.GetCurrentImageNumber());//クリップボードからペースト
					}
				}
				break;
		}
	}		
	return CDialog::PreTranslateMessage(pMsg);
}

/********************************************************************
機  能  名  称 :ダイアログの背景色を設定
関    数    名 : OnCtlColor
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
HBRUSH CDataDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);//ダイアログの背景色を設定
	
	// TODO: この位置で DC のアトリビュートを変更してください
	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください

	if( nCtlColor == CTLCOLOR_DLG )
		return   bg_brush ;

	return hbr;
}

/********************************************************************
機  能  名  称 :再描画
関    数    名 : OnCtlColor
引          数 : 
戻    り    値 : 
機          能 : すべてのサムネイルを再描画
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	
	DrawAllImages();//全ての画像を再描画する
	// 描画用メッセージとして CDialog::OnPaint() を呼び出してはいけません
}

/********************************************************************
機  能  名  称 :マウス左ボタンが押された
関    数    名 : OnLButtonDown
引          数 : 
戻    り    値 : 
機          能 : ①画像メモリ選択
				 ②画像データのメモリ間移動／コピーの開始
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int n;

	GetCapture();//マウスキャプチャー
	is_draging = true;//ドラッグ中を表すフラグを立てる

		if( (n = get_image_number_from_coordinate(point)) != -1 )//画像の上でマウスの左ボタンが押された
			API.DrawImage(n, false);
	
	CDialog::OnLButtonDown(nFlags, point);
}



/********************************************************************
機  能  名  称 :マウスカーソルの移動
関    数    名 : OnMouseMove
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
		if(is_draging){//ドラッグ中
			HCURSOR csr = AfxGetApp()->LoadCursor( IDC_POINTER_COPY );//カーソルをドラッグ中の物に替える
			SetCursor( csr );
		}	
	mouse_point = point;
	CDialog::OnMouseMove(nFlags, point);
}

/********************************************************************
機  能  名  称 :マウス左ボタンが離された
関    数    名 : OnLButtonUp
引          数 : 
戻    り    値 : 
機          能 : 画像データのメモリ間移動／コピーの実行
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int n;

	ReleaseCapture();//マウスのキャプチャーを開放
	is_draging = false;//ドラッグ中を表すフラグを倒す

		if( (n = get_image_number_from_coordinate(point)) != -1 ){//画像の上でマウスの左ボタンが離された
			if( n != API.GetCurrentImageNumber() ){//今まで選択していたものとは異なる画像の上でマウスの左ボタンが離された
				if( API.CopyDataUnit( API.GetCurrentImageNumber() , n, true) ){//コピー
					/*ChangeCurrentDataNumber(n);*/
					API.DrawImage(n);
				}
			}
		}

	CDialog::OnLButtonUp(nFlags, point);
}




/********************************************************************
機  能  名  称 :マウス右ボタンが押された
関    数    名 : OnRButtonDown
引          数 : 
戻    り    値 : 
機          能 : メニュー表示
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	int n;
	
	if( (n = get_image_number_from_coordinate(point)) != -1 ){//画像の上でマウスの左ボタンが押された
		if(API.GetDataUnit(n)){
			API.DrawImage(n, false);

			CMenu    *popupMenuP = NULL;
			CMenu    cMenu;
			int      err = 0;
		    
			// メニューをロード
			if (!err) if (!cMenu.LoadMenu(IDR_DATA_MENU)) err = 1;

			// サブメニューを取得
			if (!err) if ((popupMenuP = cMenu.GetSubMenu(0)) == NULL) err = 1;

			// メニューをポップアップ
			if (!err){
				ClientToScreen(&point);
				if (!popupMenuP->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this)) err = 1;
			}
			
			cMenu.DestroyMenu();// メニューを破棄
		}

	}
		
	CDialog::OnRButtonDown(nFlags, point);
}


/********************************************************************
機  能  名  称 :マウス右ボタンメニュー：画像メモリの名前変更
関    数    名 :OnDatamenuRename
引          数 : 
戻    り    値 : 
機          能 : 画像メモリの名前変更
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::OnDatamenuRename()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(pdu){
		CConfirmDlg dlg;
		dlg.SetWidth(70,200);
		dlg.RegistVar("画像データ名", &pdu->DataName);
		if(dlg.DoModal()==IDOK){
			API.DrawImage(CURRENT_IMAGE, false);
		}
	}
}

/********************************************************************
機  能  名  称 :マウス右ボタンメニュー：画像メモリの型変換
関    数    名 :OnDatamenuConvertType
引          数 : 
戻    り    値 : 
機          能 : 画像メモリの型変換
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::OnDatamenuConvertType()
{
	CConvertDataDlg  cvt;

		if(!API.GetDataUnit(CURRENT_IMAGE))	return;
	cvt.DoModal();
}

/********************************************************************
機  能  名  称 :マウス右ボタンメニュー：画像メモリのサイズ変換
関    数    名 :OnDatamenuResize
引          数 : 
戻    り    値 : 
機          能 : 画像メモリのサイズ変換
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::OnDatamenuResize()
{
	CResizeDataDlg  res;

		if(!API.GetDataUnit(CURRENT_IMAGE))	return;
	res.DoModal();
}

/********************************************************************
機  能  名  称 :マウス右ボタンメニュー：画像メモリの削除
関    数    名 :OnDatamenuDelete
引          数 : 
戻    り    値 : 
機          能 : 画像メモリの削除
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::OnDatamenuDelete()
{
	CString	str;

	str.Format("画像メモリ %d のデータを削除します．よろしいですか？" , API.GetCurrentImageNumber());
		if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONQUESTION ) != IDOK )	return;

	API.BackupDataUnit(CURRENT_IMAGE);
	API.DeleteDataUnit(CURRENT_IMAGE);
}


/********************************************************************
機  能  名  称 :マウス右ボタンメニュー：表示画像をビットマップとして保存
関    数    名 :OnDatamenuSavebitmap
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::OnDatamenuSavebitmap()
{
	API.SaveViewAsBitmap( API.GetCurrentImageNumber() );
}

/********************************************************************
機  能  名  称 :マウス右ボタンメニュー：画像データを復元
関    数    名 :OnDatamenuUndo
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::OnDatamenuUndo()
{
	CString	str;

	str.Format("画像メモリ %d にデータを復元します．よろしいですか？" , API.GetCurrentImageNumber());
		if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONQUESTION ) != IDOK )	return;

	API.RecoverDataUnit(CURRENT_IMAGE);
}

/********************************************************************
機  能  名  称 :3次元画像一覧表示
関    数    名 :OnDatamenuDisp3d
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CDataDlg::OnDatamenuDisp3d()
{
	API.DialogOpen(IDD_3D_IMAGE_DISP_DLG, true);
}