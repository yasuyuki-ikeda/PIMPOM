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
//CBinalizeDlg
//２値化設定ウィンドウ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "C3DImgDispDlg.h"

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
extern	CPimpomAPI	API;

// *************************************
//         関    数    参    照         
// *************************************

/********************************************************************
機  能  名  称 : 
関    数    名 : clear_buffer
引          数 :
戻    り    値 : 
機          能 :　

日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void C3DImgDispDlg::clear_buffer()
{
	for (int i = 0; i < TD_IMAGE_DISP_MAX; i++)
	{
		DispStruct[i].pDispBuffers = NULL;
		DispStruct[i].pageNumber = -1;
	}

	delete[]	pMemories;
	pMemories = NULL;
}



/********************************************************************
機  能  名  称 : 3D画像データ一覧表示の位置／サイズを設定する
関    数    名 : arrange_images
引          数 : 
戻    り    値 : ウィンドウ内に表示できるデータ数
機          能 :　DispStruct[]のメンバdispOffsetX,dispOffsetY,dispWidth,dispHeightを設定する
                  ウィンドウ上に表示できる場合は、それぞれに正値が入る
                  ウィンドウ上に表示できない場合は、それぞれに負値が入る
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
int C3DImgDispDlg::arrange_images()
{
	RECT windowRect;//このウィンドウのクライアント領域を表す矩形
	::GetClientRect(m_hWnd, &windowRect);


	float scale;
	API.GetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_SCALE, &scale);

	int offsetX = TD_IMAGE_DISP_OFFSET_X;
	int offsetY = TD_IMAGE_DISP_OFFSET_Y;
	int width = (int)( (float)API.main_image_size.cx * scale);
	int height = (int)((float)API.main_image_size.cy * scale);


	//ウィンドウ内に入る限り座標配置する
	
	int bufferCnt = 0;
	for (; bufferCnt < TD_IMAGE_DISP_MAX; bufferCnt++)
	{
		//折り返し
		if (offsetX + width >= windowRect.right - windowRect.left){
			offsetX = TD_IMAGE_DISP_OFFSET_X;
			offsetY += height + SMALL_MARGN_Y;

			if (offsetY + height + SMALL_MARGN_Y> windowRect.bottom - windowRect.top){
				break;
			}
		}

		//表示位置とサイズを設定
		DispStruct[bufferCnt].dispOffsetX = offsetX;
		DispStruct[bufferCnt].dispOffsetY = offsetY;
		DispStruct[bufferCnt].dispWidth = width;
		DispStruct[bufferCnt].dispHeight = height;


		offsetX += width + 5/*SMALL_MARGN_X*/;//表示位置を進める
	}

	//ウィンドウ内に入りきらないものは非表示とする(サイズを-1にする)
	for (int n = bufferCnt; n < TD_IMAGE_DISP_MAX; n++)
	{
		DispStruct[bufferCnt].dispOffsetX = -1;
		DispStruct[bufferCnt].dispOffsetY = -1;
		DispStruct[n].dispWidth = -1;
		DispStruct[n].dispHeight = -1;
	}

	return bufferCnt;
}


/********************************************************************
機  能  名  称 : 画像データ表示用メモリの設定
関    数    名 : copy_disp_buffers
引          数 : int image_number	(in)対象画像メモリ番号(-1指定の場合は、現在選択中の画像メモリ)
				 int page_offset	(in)表示開始するページ番号
				 int disp_cnt		(in)表示するページ番号
戻    り    値 : 成功したらtrue
機          能 : pMemoriesに表示用メモリを確保する
                 DispStruct[]のメンバ pDispBuffers, pageNumberを設定する
                  
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool C3DImgDispDlg::copy_disp_buffers(int image_number, int page_offset, int disp_cnt)
{
	//表示データの取得
	CDataUnit *pDU = API.GetDataUnit(image_number);
	if (pDU == NULL){//表示データがない場合はエラー終了
		return false;
	}

	
	//バッファいったんクリア
	clear_buffer();

	//表示バッファのメモリ確保
	int bufferLen = pDU->PageNumber - page_offset;
	if (bufferLen > TD_IMAGE_DISP_MAX){
		bufferLen = TD_IMAGE_DISP_MAX;
	}

	if (pDU->pDispBufferRGB){//RGB
		pMemories = new BYTE[pDU->DispBufferRowRGB * pDU->DataSize.cy * bufferLen];
	}
	else{//RGB以外
		pMemories = new BYTE[pDU->DispBufferRow * pDU->DataSize.cy * bufferLen];
	}

	if (pMemories == NULL){
		return false;//メモリ確保失敗時はエラー終了
	}



	int orgDispPage = pDU->DispPage;//メイン画面に表示されていたページ番号退避

	//表示画像作成＆バッファにコピー
	for (int bufferCnt = 0, dispPage = page_offset; 
		dispPage < pDU->PageNumber && bufferCnt<TD_IMAGE_DISP_MAX && bufferCnt<disp_cnt;
		dispPage++, bufferCnt++)
	{

		if (DispStruct[bufferCnt].dispWidth>0 && DispStruct[bufferCnt].dispHeight>0)
		{//画像が表示できる場合のみ

			//メイン画面表示画像の作成
			pDU->DispPage = dispPage;
			pDU->UpdateDispBuffer();

			if (pDU->Average == 0.0 && pDU->Stdiv == 0 && API.GetCheck(m_hWnd, IDC_CHECK_MASK))
			{
				DispStruct[bufferCnt].mask = 1;
			}
			else{
				DispStruct[bufferCnt].mask = 0;
			}

			//表示画像をバッファにコピー
			DispStruct[bufferCnt].pageNumber = dispPage;
			if (pDU->pDispBufferRGB)
			{//RGB
				DispStruct[bufferCnt].pDispBuffers = &pMemories[pDU->DispBufferRowRGB * pDU->DataSize.cy * bufferCnt];

				memcpy(DispStruct[bufferCnt].pDispBuffers, pDU->pDispBufferRGB, pDU->DispBufferRowRGB * pDU->DataSize.cy);
			}
			else{//RGB以外
				DispStruct[bufferCnt].pDispBuffers = &pMemories[pDU->DispBufferRow * pDU->DataSize.cy * bufferCnt];

				memcpy(DispStruct[bufferCnt].pDispBuffers, pDU->pDispBuffer, pDU->DispBufferRow * pDU->DataSize.cy);
			}
		}
	}

	//メイン画面表示ページをもとにもどす
	pDU->DispPage = orgDispPage;
	pDU->UpdateDispBuffer();


	return true;
}


/********************************************************************
機  能  名  称 : 画像データ一覧描画
関    数    名 : disp_images
引          数 : int image_number	(in)対象画像メモリ番号(-1指定の場合は、現在選択中の画像メモリ)
戻    り    値 : 成功したらtrue
機          能 : DispStruct[]の内容を描画する

日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool C3DImgDispDlg::draw_images(int image_number)
{
	//表示データの取得
	CDataUnit *pDU = API.GetDataUnit(image_number);
	if (pDU == NULL){//表示データがない場合はエラー終了
		return false;
	}

	//背景描画
	{
		RECT windowRect;
		::GetClientRect(m_hWnd, &windowRect);

		HDC hdc = ::GetDC(m_hWnd);
		HPEN hpen = ::CreatePen(PS_SOLID, 1, API.default_bg_color);
		HBRUSH hbrush = ::CreateSolidBrush(API.default_bg_color);
		::SelectObject(hdc, hpen);
		::SelectObject(hdc, hbrush);
		::Rectangle(hdc, windowRect.left, windowRect.top + 25, windowRect.right, windowRect.bottom);
		::DeleteObject(hbrush);
		::DeleteObject(hpen);
		::ReleaseDC(m_hWnd, hdc);
	}

	//画像描画
	for (int bufferCnt = 0; bufferCnt < TD_IMAGE_DISP_MAX; bufferCnt++)
	{
		if (DispStruct[bufferCnt].pDispBuffers != NULL && 
			DispStruct[bufferCnt].pageNumber >= 0 &&
			DispStruct[bufferCnt].dispWidth>0 && DispStruct[bufferCnt].dispHeight>0)
		{
			//画像を描画する
			HDC hdc = ::GetDC(m_hWnd);

			::SetStretchBltMode(hdc, HALFTONE);//中間色で補間する

			if ((pDU->DataType != RGB_FORMAT && pDU->DataType != RGB_3D_FORMAT))
			{//RGB以外
				::StretchDIBits(hdc,
					DispStruct[bufferCnt].dispOffsetX, DispStruct[bufferCnt].dispOffsetY,
					DispStruct[bufferCnt].dispWidth, DispStruct[bufferCnt].dispHeight,
					0, 0, pDU->DataSize.cx, pDU->DataSize.cy,
					DispStruct[bufferCnt].pDispBuffers,
					pDU->pDispBufferBmpInfo, DIB_RGB_COLORS, SRCCOPY);

			}
			else
			{//RGB
				::StretchDIBits(hdc,
					DispStruct[bufferCnt].dispOffsetX, DispStruct[bufferCnt].dispOffsetY,
					DispStruct[bufferCnt].dispWidth, DispStruct[bufferCnt].dispHeight,
					0, 0, pDU->DataSize.cx, pDU->DataSize.cy,
					DispStruct[bufferCnt].pDispBuffers,
					pDU->pDispBufferBmpInfoRGB, DIB_RGB_COLORS, SRCCOPY);
			}


			//表示データのページ番号を描画
			HFONT hfont = ::CreateFontIndirect(&API.default_font);
			::SelectObject(hdc, hfont);

			char strbuf[256] = { 0 };
			sprintf(strbuf, "%d", DispStruct[bufferCnt].pageNumber);
			::TextOut(hdc, DispStruct[bufferCnt].dispOffsetX, DispStruct[bufferCnt].dispOffsetY + DispStruct[bufferCnt].dispHeight + 5, strbuf, 256);

			::DeleteObject(hfont);


			//マスクつきは赤枠
			if (DispStruct[bufferCnt].mask)
			{
				HPEN hpen = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				::SelectObject(hdc, hpen);
				::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));
				::Rectangle(hdc, DispStruct[bufferCnt].dispOffsetX - 1,
					DispStruct[bufferCnt].dispOffsetY - 1,
					DispStruct[bufferCnt].dispOffsetX + DispStruct[bufferCnt].dispWidth + 1,
					DispStruct[bufferCnt].dispOffsetY + DispStruct[bufferCnt].dispHeight + 1);
				::DeleteObject(hpen);
			}


			//メイン画面で表示中のページの場合は枠を表示
			if (DispStruct[bufferCnt].pageNumber == pDU->DispPage)
			{
				HPEN hpen = ::CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
				::SelectObject(hdc, hpen);
				::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));
				::Rectangle(hdc, DispStruct[bufferCnt].dispOffsetX - 4, 
								 DispStruct[bufferCnt].dispOffsetY - 4, 
								 DispStruct[bufferCnt].dispOffsetX + DispStruct[bufferCnt].dispWidth + 4,
								 DispStruct[bufferCnt].dispOffsetY + DispStruct[bufferCnt].dispHeight + 4);
				::DeleteObject(hpen);
			}


			::ReleaseDC(m_hWnd, hdc);
		}
	}

	//ウィンドウタイトルにデータ名表示
	CString str;
	str.Format(" (%d x %d x %d)", pDU->DataSize.cx, pDU->DataSize.cy, pDU->PageNumber);
	::SetWindowText(m_hWnd, pDU->DataName + str);


	//表示濃度	
	API.SetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_MIN, pDU->BrightMin);
	API.SetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_MAX, pDU->BrightMax);
	

	return true;
}

/********************************************************************
機  能  名  称 : スライダーを設定する
関    数    名 : set_slider
引          数 :
戻    り    値 : スライダーの値（＝表示開始ページ番号）
機          能 : ウィンドウ上に表示できる画像数が、表示データのページ数より少ない場合、
　　　　　　　　　スライダーを用いて、表示する区間（表示開始ページ番号）を決める
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
int C3DImgDispDlg::set_slider(int image_number)
{
	if (pMemories == NULL){//表示用メモリが準備されていなければ何もしない
		return 0;
	}

	//表示データ取得
	CDataUnit *pDU = API.GetDataUnit(image_number);
	if (pDU == NULL){
		return 0;
	}

	//ウィンドウ上に表示できる画像数を取得
	int dispImgMax = arrange_images();

	int prevPos = m_slider.GetPos();
	int newPos;

	if (pDU->PageNumber > dispImgMax)
	{//表示できる画像数が、表示データのページ数より少ない場合は、スライダーを使用して表示範囲を選択する
		
		m_slider.SetRange(0, pDU->PageNumber - dispImgMax);

		if (prevPos >= 0 && prevPos <= pDU->PageNumber - dispImgMax){
			newPos = prevPos;
		}
		else{
			newPos = 0;
			
		}
		m_slider.SetPos(newPos);
		m_slider.ShowWindow(SW_SHOW);
	}
	else
	{//表示できる画像数が、表示データのページ数以上の場合はスライダーは使用しない
		newPos = 0;
		m_slider.SetPos(newPos);
		m_slider.ShowWindow(SW_HIDE);
	}

	return newPos;
}




/********************************************************************
機  能  名  称 : 3D画像データ一覧表示
関    数    名 : DispImage
引          数 :(in)対象の画像メモリ番号(-1の場合は現在選択中の画像メモリ番号)
戻    り    値 :成功したらtrue
機          能 :

日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool C3DImgDispDlg::DispImage(int image_number)
{
	int pos = set_slider(image_number);

	if (!copy_disp_buffers(image_number, pos))	return false;

	if (!draw_images(image_number))	return false;

	if (image_number < 0)		image_number = API.GetCurrentImageNumber();
	imageNumber = image_number;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(C3DImgDispDlg, CDialog)

C3DImgDispDlg::C3DImgDispDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3DImgDispDlg::IDD, pParent)
{
	for (int i = 0; i < TD_IMAGE_DISP_MAX; i++)
	{
		memset(&DispStruct[i], 0, sizeof(TD_IMAGE_DISP_STRUCT));
		//DispStruct[i].pDispBuffers = NULL;
		DispStruct[i].pageNumber = -1;
	}
	pMemories = NULL;

	imageNumber = -1;
}

C3DImgDispDlg::~C3DImgDispDlg()
{
	clear_buffer();
}

void C3DImgDispDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_DISP_PAGE, m_slider);
}


BEGIN_MESSAGE_MAP(C3DImgDispDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_3D_IMG_DISP_SCALE, &C3DImgDispDlg::OnBnClickedButton3dImgDispScale)
	ON_BN_CLICKED(IDC_BUTTON_3D_IMG_DISP_SCALE2, &C3DImgDispDlg::OnBnClickedButton3dImgDispScale2)
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_DISP_MINMAX, &C3DImgDispDlg::OnBnClickedButtonDispMinmax)
	ON_BN_CLICKED(IDC_CHECK_MASK, &C3DImgDispDlg::OnBnClickedCheckMask)
END_MESSAGE_MAP()


// C3DImgDispDlg メッセージ ハンドラー

/********************************************************************
機  能  名  称 : ダイアログの初期化
関    数    名 : OnInitDialog
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ -------------- - -------------------------------------- -
Y.Ikeda         新規作成
********************************************************************/
BOOL C3DImgDispDlg::OnInitDialog()
{
	CDialog::OnInitDialog();



	API.SetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_SCALE, (float)API.small_image_size.cx / (float)API.main_image_size.cx);
	imageNumber = -1;

	DispImage(-1);//現在選択中の画像を表示対象とする

	return true;
}


/********************************************************************
機  能  名  称 : ウィンドウ再描画
関    数    名 : OnPaint
引          数 :
戻    り    値 :
機          能 :

日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void C3DImgDispDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// 描画メッセージで CDialog::OnPaint() を呼び出さないでください。

	draw_images(imageNumber);
}


/********************************************************************
機  能  名  称 : ウィンドウリサイズ
関    数    名 : DispImage
引          数 :
戻    り    値 :
機          能 :

日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void C3DImgDispDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	DispImage(imageNumber);
}


/********************************************************************
機  能  名  称 : 表示倍率拡大ボタン押下
関    数    名 : OnBnClickedButton3dImgDispScale
引          数 :
戻    り    値 :
機          能 :

日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void C3DImgDispDlg::OnBnClickedButton3dImgDispScale()
{
	float scale;
	API.GetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_SCALE, &scale);
	scale *= 2;
	API.SetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_SCALE, scale);


	DispImage(imageNumber);
}

/********************************************************************
機  能  名  称 : 表示倍率縮小ボタン押下
関    数    名 : OnBnClickedButton3dImgDispScale2
引          数 :
戻    り    値 :
機          能 :

日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void C3DImgDispDlg::OnBnClickedButton3dImgDispScale2()
{
	float scale;
	API.GetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_SCALE, &scale);
	scale /= 2;
	API.SetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_SCALE, scale);

	DispImage(imageNumber);
}

/********************************************************************
機  能  名  称 : スライダー操作
関    数    名 : OnHScroll
引          数 :
戻    り    値 :
機          能 :

日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void C3DImgDispDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar*)&m_slider)//グローバル閾値設定
	{
		DispImage(imageNumber);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


/********************************************************************
機  能  名  称 : マウス右クリック
関    数    名 : OnLButtonDown
引          数 :
戻    り    値 :
機          能 :

日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void C3DImgDispDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//表示データ取得
	CDataUnit *pDU = API.GetDataUnit(imageNumber);
	if (pDU)
	{
		for (int bufferCnt = 0; bufferCnt < TD_IMAGE_DISP_MAX; bufferCnt++)
		{
			if (DispStruct[bufferCnt].pDispBuffers != NULL &&
				DispStruct[bufferCnt].pageNumber >= 0 &&
				DispStruct[bufferCnt].dispWidth>0 && DispStruct[bufferCnt].dispHeight > 0)
			{
				if (DispStruct[bufferCnt].dispOffsetX < point.x &&
					DispStruct[bufferCnt].dispOffsetX + DispStruct[bufferCnt].dispWidth > point.x &&
					DispStruct[bufferCnt].dispOffsetY < point.y &&
					DispStruct[bufferCnt].dispOffsetY + DispStruct[bufferCnt].dispHeight > point.y)
				{
					//表示ページを変えて再描画
					pDU->DispPage = DispStruct[bufferCnt].pageNumber;
					API.DrawImage(imageNumber);
					DispImage(imageNumber);
					break;
				}
			}
		}
			
	}


	CDialog::OnLButtonDown(nFlags, point);
}




/********************************************************************
機  能  名  称 : min-maxボタン押下
関    数    名 : OnBnClickedButtonDispMinmax
引          数 :
戻    り    値 :
機          能 :

日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void C3DImgDispDlg::OnBnClickedButtonDispMinmax()
{
	//表示データ取得
	CDataUnit *pDU = API.GetDataUnit(imageNumber);
	if (pDU)
	{
		//濃度上下限値の初期
		float maxAll, minAll, min, max;
		pDU->GetDataMinMax(&maxAll, &minAll);

		int orgDispPage = pDU->DispPage;//メイン画面に表示されていたページ番号退避
		
		//全ページで濃度上下限値を探す
		for (int n = 0; n < pDU->PageNumber; n++)
		{
			pDU->DispPage = n;
			pDU->GetDataMinMax(&min, &max);

			if (minAll > min){
				minAll = min;
			}

			if (maxAll < max){
				maxAll = max;
			}
		}

		pDU->DispPage = orgDispPage;//表示ページ番号復帰

		pDU->SetDispRange(minAll, maxAll);//濃度上下限設定
		API.SetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_MIN, minAll);
		API.SetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_MAX, maxAll);

		DispImage(imageNumber);//画像描画

		API.DrawImage();
	}
}



void C3DImgDispDlg::OnBnClickedCheckMask()
{
	DispImage(imageNumber);
}