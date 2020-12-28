/*******************************************************************
                   License Agreement
    Ploaform for IMage Procession Obserber and Modifier

Copyright(c) 2020, Yasuyuki Ikeda All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met :

*Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and / or other materials provided with the distribution.
* The name of the copyright holders may not be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.IN NO EVENT SHALL CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************/
//外部プログラムからPIMPOMにデータを出力するための関数群
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include <windows.h>

// *************************************
//         マ  ク  ロ   定   義         
// *************************************
#ifndef _PIMPOM_PLOT
#define _PIMPOM_PLOT

#define  PINPOM_WND_KEY		" - PIMPOM"
#define	 SHEAREMEM_NAME		"PIMPOM_SHAREDMEM"
#define  SHEAREMEM_EXPAND_MESSAGE_SIZE		50000
#define  PIMPOM_WND_CNT_MAX		10

// *************************************
//         外    部    宣    言         
// *************************************

// *************************************
//         構  造  体   定   義         
// *************************************
#pragma	warning(push)
#pragma	warning(disable:4200)
#pragma	pack(push, 8)

//画像データを受け渡しするための共有メモリのヘッダ
typedef struct {
	int		format;//画像データのタイプ
				   //	0:BYTE  
				   //	1:short
				   //	2:WORD
				   //	3:long
				   //	4:DWORD
	int		image_num;//表示先の画像メモリ番号
	int		width;//幅
	int		height;//高さ
	int		page;//奥行
	int		channel;//チャネル数
	int		size;//共有メモリのサイズ
	BYTE	pData[];//画像データ
}SHEAREMEM_PLOT_IMAGE;

#pragma	pack(pop)
#pragma	warning(pop)

//図形描画データを受け渡しするための共有メモリのヘッダ
typedef struct {
	int		type;//描画タイプ　0:背景となる画像のコピー  1:画像描画  2:点描画  3:直線描画 4:矩形描画
	int		image_num;////表示先の画像メモリ番号
	int		x0;
	int		y0;
	int		x1;
	int		y1;
	int		size;
	BYTE	red;//描画色(赤）
	BYTE	green;//描画色(緑）
	BYTE	blue;//描画色(青）
	int		copy_src;//背景となる画像メモリ番号
	char	message[5000];//メッセージ
}SHEAREMEM_EXT_PROC;

//

// *************************************
//         静    的    変    数         
// *************************************
static int pimpom_target_id = 0;

// *************************************
//         外    部    変    数         
// *************************************



#ifdef __cplusplus
extern "C" {
#endif


	//タイトルにkeyを含むウィンドウハンドルを検索する
	static HWND pimpom_plot_find_window_from_key(HWND hWndParent, TCHAR key[])
	{
		TCHAR szBuff[512];

		SendMessage(hWndParent, WM_GETTEXT, 255, (LPARAM)szBuff);
		if (_tcsstr(szBuff, key)) {
			return hWndParent;
		}
		else {
			HWND hWndChild = ::FindWindowEx(hWndParent, NULL, _T("#32770"), NULL);
			if (hWndChild == NULL)	return NULL;

			do {
				HWND wnd = pimpom_plot_find_window_from_key(hWndChild, key);
				if (wnd)	return wnd;

			} while ((hWndChild = ::FindWindowEx(hWndParent, hWndChild, _T("#32770"), NULL)) != NULL);
		}
		return NULL;
	}


	static void makeWindowKey(int id, TCHAR key[])
	{
		if (id == 0) {
			_stprintf(key, _T(" - PIMPOM  "));
		}
		else if (id>0) {
			_stprintf(key, _T(" - PIMPOM %d"), id);
		}
		else {
			_stprintf(key, _T(" - PIMPOM"));
		}
	}

	//PIMPOMにメッセージ送信する
	static void pimpom_plot_send_data(UINT Msg, UINT wParam, int id)
	{
		HWND hWnd;
		TCHAR key[256];

		makeWindowKey(id, key);

		//hWnd = pimpom_plot_find_window_from_key(::GetDesktopWindow(), PINPOM_WND_KEY);
		hWnd = pimpom_plot_find_window_from_key(::GetDesktopWindow(), key);
		if (hWnd == NULL)	return;

		SendMessage(hWnd, Msg, wParam, (LPARAM)NULL);
	}

	//共有メモリを取得する
	static void *pimpom_plot_get_shere_mem(int size, HANDLE *hShare, int doWrite)
	{
		if (doWrite)
		{//書き込み用
#ifdef _M_AMD64
			*hShare = CreateFileMapping((HANDLE)0xffffffffffffffff, NULL, PAGE_READWRITE, 0, size, _T(SHEAREMEM_NAME));
#else
			*hShare = CreateFileMapping((HANDLE)0xffffffff, NULL, PAGE_READWRITE, 0, size, _T(SHEAREMEM_NAME));
#endif
			if (!*hShare)	return NULL;

			return ::MapViewOfFile(*hShare, FILE_MAP_WRITE, 0, 0, size);
		}
		else
		{//読み出し用
			*hShare = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, FILE_MAP_READ, 0, size, _T(SHEAREMEM_NAME));
			if (!*hShare)	return NULL;

			return ::MapViewOfFile(*hShare, FILE_MAP_READ, 0, 0, size);
		}
	}

	//共有メモリを解放する
	static void pimpom_plot_free_shere_mem(void *pMem, HANDLE hShare)
	{
		::UnmapViewOfFile(pMem);
		::CloseHandle(hShare);
	}

	//画像データのピクセルサイズ
	static int get_pix_size(int format)
	{
		int pix_size = sizeof(BYTE);
		switch (format)
		{
		case 0://BYTE型メモリ
			pix_size = sizeof(BYTE);
			break;

		case 1://SHORT型メモリ
			pix_size = sizeof(short);
			break;

		case 2://WORD型メモリ
			pix_size = sizeof(WORD);
			break;

		case 3://LONG型メモリ
			pix_size = sizeof(long);
			break;

		case 4://DWORD型メモリ
			pix_size = sizeof(DWORD);
			break;

		case 5://float型メモリ
			pix_size = sizeof(float);
			break;

		case 10://3D型メモリ
			pix_size = sizeof(BYTE);
			break;

		case 11://F3D型メモリ
			pix_size = sizeof(float);
			break;

		case 20://RGB型メモリ
			pix_size = sizeof(BYTE);
			break;

		case 21://マスクメモリ
			pix_size = sizeof(BYTE);
			break;

		default:
			return 0;
		}

		return pix_size;
	}



	//PIMPOMに画像データを出力する
	static void pimpom_plot_image(int format, int num, int width, int height, int page, int channel, void* pData)
	{
		HANDLE	hShare;
		SHEAREMEM_PLOT_IMAGE	*pShereMem;
		int		pix_size, sheremem_size;

		pix_size = get_pix_size(format);//画像データのピクセルサイズ
		sheremem_size = sizeof(SHEAREMEM_PLOT_IMAGE) + width*height*pix_size*page*channel;//画像データのサイズ
		pShereMem = (SHEAREMEM_PLOT_IMAGE*)pimpom_plot_get_shere_mem(sheremem_size, &hShare, 1);//共有メモリ取得
		if (!pShereMem)	return;

		//共有メモリにプロットする画像データの情報を入れる
		pShereMem->format = format;
		pShereMem->image_num = num;
		pShereMem->width = width;
		pShereMem->height = height;
		pShereMem->page = page;
		pShereMem->channel = channel;
		pShereMem->size = sheremem_size;
		memcpy(pShereMem->pData, pData, width*height*pix_size*page*channel);

		pimpom_plot_send_data(WM_USER + 100, 0, pimpom_target_id);//プロットのメッセージを送信
		pimpom_plot_free_shere_mem(pShereMem, hShare);//共有メモリを解放
	}


	//PIMPOMから画像データを出力する
	static void pimpom_get_image(int format, int num, int width, int height, int page, int channel, void* pData)
	{
		HANDLE	hShare;
		SHEAREMEM_PLOT_IMAGE	*pShereMem;
		int		pix_size, sheremem_size;

		pix_size = get_pix_size(format);//画像データのピクセルサイズ
		sheremem_size = sizeof(SHEAREMEM_PLOT_IMAGE) + width*height*pix_size*page*channel;//画像データのサイズ
		pShereMem = (SHEAREMEM_PLOT_IMAGE*)pimpom_plot_get_shere_mem(sheremem_size, &hShare, 1);//共有メモリ取得
		if (!pShereMem)	return;

		//共有メモリにプロットする画像データの情報を入れる
		pShereMem->format = format;
		pShereMem->image_num = num;
		pShereMem->width = width;
		pShereMem->height = height;
		pShereMem->page = page;
		pShereMem->channel = channel;
		pShereMem->size = sheremem_size;


		pimpom_plot_send_data(WM_USER + 102, 0, pimpom_target_id);//メッセージを送信
		memcpy(pData, pShereMem->pData, width*height*pix_size*page*channel);


		pimpom_plot_free_shere_mem(pShereMem, hShare);//共有メモリを解放
	}




	//PIMPOMのウィンドウIDを列挙する
	static int getTargetIDs(int IDs[])
	{
		int wndCnt = 0;

		for (int id = 0; id < PIMPOM_WND_CNT_MAX; id++)
		{
			HWND hWnd;
			TCHAR key[256];

			makeWindowKey(id, key);
			hWnd = pimpom_plot_find_window_from_key(::GetDesktopWindow(), key);
			if (hWnd) {
				IDs[wndCnt] = id;
				wndCnt++;
			}
		}

		return wndCnt;
	}



	//データを送るPIMPOMのIDを指定する
	static void setTargetID(int target)
	{
		pimpom_target_id = target;
	}


	/********************************************************************
	機  能  名  称 : PIMPOMにBYTE画像データを出力する
	関    数    名 : PlotByteImage
	引          数 : int num　	(in)画像メモリ番号
	int width	(in)幅
	int height (in)高さ
	BYTE* pData (in)画像データの先頭ポインタ
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void PlotByteImage(int num, int width, int height, BYTE* pData)
	{
		pimpom_plot_image(0, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	機  能  名  称 : PIMPOMにshort画像データを出力する
	関    数    名 : PlotShortImage
	引          数 : int num　	(in)画像メモリ番号
	int width	(in)幅
	int height (in)高さ
	short* pData (in)画像データの先頭ポインタ
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void PlotShortImage(int num, int width, int height, short* pData)
	{
		pimpom_plot_image(1, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	機  能  名  称 : PIMPOMにunsigned short画像データを出力する
	関    数    名 : PlotWordImage
	引          数 : int num　	(in)画像メモリ番号
	int width	(in)幅
	int height (in)高さ
	unsigned short* pData (in)画像データの先頭ポインタ
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void PlotWordImage(int num, int width, int height, WORD* pData)
	{
		pimpom_plot_image(2, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	機  能  名  称 : PIMPOMにlong画像データを出力する
	関    数    名 : PlotLongImage
	引          数 : int num　	(in)画像メモリ番号
	int width	(in)幅
	int height (in)高さ
	long* pData (in)画像データの先頭ポインタ
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void PlotLongImage(int num, int width, int height, long* pData)
	{
		pimpom_plot_image(3, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	機  能  名  称 : PIMPOMにunsigned long画像データを出力する
	関    数    名 : PlotDwordImage
	引          数 : int num　	(in)画像メモリ番号
	int width	(in)幅
	int height (in)高さ
	unsigned long* pData (in)画像データの先頭ポインタ
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void PlotDwordImage(int num, int width, int height, DWORD* pData)
	{
		pimpom_plot_image(4, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	機  能  名  称 : PIMPOMにfloat画像データを出力する
	関    数    名 : PlotFloatImage
	引          数 : int num　	(in)画像メモリ番号
	int width	(in)幅
	int height (in)高さ
	float* pData (in)画像データの先頭ポインタ
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void PlotFloatImage(int num, int width, int height, float* pData)
	{
		pimpom_plot_image(5, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	機  能  名  称 : PIMPOMに3D画像データを出力する
	関    数    名 : Plot3DImage
	引          数 : int num　	(in)画像メモリ番号
	int width	(in)幅
	int height (in)高さ
	int page   (in)奥行
	BYTE* pData (in)画像データの先頭ポインタ
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void Plot3DImage(int num, int width, int height, int page, BYTE* pData)
	{
		pimpom_plot_image(10, num, width, height, page, 1, pData);
	}


	/********************************************************************
	機  能  名  称 : PIMPOMにFloat3D画像データを出力する
	関    数    名 : PlotF3DImage
	引          数 : int num　	(in)画像メモリ番号
	int width	(in)幅
	int height (in)高さ
	int page   (in)奥行
	BYTE* pData (in)画像データの先頭ポインタ
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void PlotF3DImage(int num, int width, int height, int page, float* pData)
	{
		pimpom_plot_image(11, num, width, height, page, 1, pData);
	}

	/********************************************************************
	機  能  名  称 : PIMPOMにRGB画像データを出力する
	関    数    名 : PlotRGBImage
	引          数 : int num　	(in)画像メモリ番号
	int width	(in)幅
	int height (in)高さ
	BYTE* pData (in)画像データの先頭ポインタ
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void PlotRGBImage(int num, int width, int height, BYTE* pData)
	{
		pimpom_plot_image(20, num, width, height, 1, 3, pData);
	}

	/********************************************************************
	機  能  名  称 : PIMPOMからBYTE画像データを入力する
	関    数    名 : GetByteImage
	引          数 : int num　	(in)画像メモリ番号
	int width	(in)幅
	int height (in)高さ
	BYTE* pData (out)画像データの先頭ポインタ width*heightのメモリ領域が確保されていること
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void GetByteImage(int num, int width, int height, BYTE* pData)
	{
		pimpom_get_image(0, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	機  能  名  称 : PIMPOMからshort画像データを入力する
	関    数    名 : GethortImage
	引          数 : int num　	(in)画像メモリ番号
	int width	(in)幅
	int height (in)高さ
	short* pData (out)画像データの先頭ポインタ width*heightのメモリ領域が確保されていること
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void GetShortImage(int num, int width, int height, short* pData)
	{
		pimpom_get_image(1, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	機  能  名  称 : PIMPOMからunsigned short画像データを入力する
	関    数    名 : GetWordImage
	引          数 : int num　	(in)画像メモリ番号
	int width	(in)幅
	int height (in)高さ
	unsigned short* pData (out)画像データの先頭ポインタ width*heightのメモリ領域が確保されていること
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void GetWordImage(int num, int width, int height, WORD* pData)
	{
		pimpom_get_image(2, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	機  能  名  称 : PIMPOMからlong画像データを入力する
	関    数    名 : GetLongImage
	引          数 : int num　	(in)画像メモリ番号
	int width	(in)幅
	int height (in)高さ
	long* pData (out)画像データの先頭ポインタ width*heightのメモリ領域が確保されていること
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void GetLongImage(int num, int width, int height, long* pData)
	{
		pimpom_get_image(3, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	機  能  名  称 : PIMPOMからunsigned long画像データを入力する
	関    数    名 : PlotDwordImage
	引          数 : int num　	(in)画像メモリ番号
	int width	(in)幅
	int height (in)高さ
	unsigned long* pData (out)画像データの先頭ポインタ width*heightのメモリ領域が確保されていること
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void GetDwordImage(int num, int width, int height, DWORD* pData)
	{
		pimpom_get_image(4, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	機  能  名  称 : PIMPOMからfloat画像データを入力する
	関    数    名 : GetFloatImage
	引          数 : int num　	(in)画像メモリ番号
	 int width	(in)幅
	int height (in)高さ
	float* pData (out)画像データの先頭ポインタ width*heightのメモリ領域が確保されていること
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void GetFloatImage(int num, int width, int height, float* pData)
	{
		pimpom_get_image(5, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	機  能  名  称 : PIMPOMから3D画像データを入力する
	関    数    名 : Get3DImage
	引          数 : int num　	(in)画像メモリ番号
	int width	(in)幅
	int height (in)高さ
	int page   (in)奥行
	BYTE* pData (out)画像データの先頭ポインタ width*height*pageのメモリ領域が確保されていること
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void Get3DImage(int num, int width, int height, int page, BYTE* pData)
	{
		pimpom_get_image(10, num, width, height, page, 1, pData);
	}

	/********************************************************************
	機  能  名  称 : PIMPOMからFloat3D画像データを入力する
	関    数    名 : GetF3DImage
	引          数 : int num　	(in)画像メモリ番号
	int width	(in)幅
	int height (in)高さ
	int page   (in)奥行
	float* pData (out)画像データの先頭ポインタ width*height*pageのメモリ領域が確保されていること
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void GetF3DImage(int num, int width, int height, int page, float* pData)
	{
		pimpom_get_image(11, num, width, height, page, 1, pData);
	}


	/********************************************************************
	機  能  名  称 : PIMPOMからRGB画像データを入力する
	関    数    名 : GetRGBImage
	引          数 : int num　	(in)画像メモリ番号
	int width	(in)幅
	int height (in)高さ
	BYTE* pData (out)画像データの先頭ポインタ  width*height*3のメモリ領域が確保されていること
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void GetRGBImage(int num, int width, int height, BYTE* pData)
	{
		pimpom_get_image(20, num, width, height, 1, 3, pData);
	}


	//PIMPOMに図形データを出力する
	static void pimpom_draw_figure(int type, int num, int x0, int y0, int x1, int y1, int size, BYTE r, BYTE g, BYTE b, int copy_src)
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;

		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC), &hShare, 1);
		if (!pShereMem)	return;

		pShereMem->type = type;
		pShereMem->image_num = num;
		pShereMem->x0 = x0;
		pShereMem->y0 = y0;
		pShereMem->x1 = x1;
		pShereMem->y1 = y1;
		pShereMem->size = size;
		pShereMem->red = r;
		pShereMem->green = g;
		pShereMem->blue = b;
		pShereMem->copy_src = copy_src;

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);
		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}

	//PIMPOMにテキストデータを出力する
	static void pimpom_draw_text(int type, int num, int x, int y, char mes[], BYTE r, BYTE g, BYTE b, int copy_src)
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;

		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC), &hShare, 1);
		if (!pShereMem)	return;

		pShereMem->type = type;
		pShereMem->image_num = num;
		pShereMem->x0 = x;
		pShereMem->y0 = y;
		pShereMem->x1 = 0;
		pShereMem->y1 = 0;
		pShereMem->size = 0;
		pShereMem->red = r;
		pShereMem->green = g;
		pShereMem->blue = b;
		pShereMem->copy_src = copy_src;
		strcpy(pShereMem->message, mes);

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);
		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}

	/********************************************************************
	機  能  名  称 : 図形描画準備
	関    数    名 : PrepeareDrawing
	引          数 : int num　			(in)描画する画像メモリ番号
	int copy_src_num	(in)背景画像の画像メモリ番号
	戻    り    値 :
	機          能 : copy_src_numのメモリに入っている画像を背景としてnumに描画用のRGB画像を新規生成する
	図形描画前にこの関数を呼び出しておく
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void PrepeareDrawing(int num, int copy_src_num)
	{
		pimpom_draw_figure(0, num, 0, 0, 0, 0, 0, 0, 0, 0, copy_src_num);
	}

	/********************************************************************
	機  能  名  称 : 画像描画
	関    数    名 : DrawImage
	引          数 : int num　			(in)描画する画像メモリ番号
	bool doAdjust		(in)画面サイズに合わせるかどうか
	char *format, ...  (in)画像データ名
	戻    り    値 :
	機          能 : 画像メモリ上に図形描画したあと、表示のためにこの関数を呼び出す
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void DrawImage2(int num, bool doAdjust, char *format, ...)
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;
		va_list list;
		char	buf[5000] = { 0 };

		//データ名を取得
		va_start(list, format);
		vsprintf(buf, format, list);
		va_end(list);

		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC), &hShare, 1);
		if (!pShereMem)	return;

		pShereMem->type = 1;
		pShereMem->image_num = num;
		sprintf(pShereMem->message, "%d,%s\n", doAdjust, buf);

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);
		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}


	static void DrawImage(int num)
	{
		DrawImage2(num, false, "");
	}

	/********************************************************************
	機  能  名  称 : 画像メモリ上に点を描画する
	関    数    名 : DrawPointOnImage
	引          数 : int	num　		(in)描画する画像メモリ番号
	int	x,y			(in)点の座標
	BYTE	r,g,b		(in)点の描画色
	戻    り    値 :
	機          能 : 画像メモリ上に点を描画する
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void DrawPointOnImage(int num, int x, int y, BYTE r, BYTE g, BYTE b)
	{
		pimpom_draw_figure(2, num, x, y, 0, 0, 1, r, g, b, -1);
	}

	/********************************************************************
	機  能  名  称 : 画像メモリ上に直線を描画する
	関    数    名 : DrawLineOnImage
	引          数 : int	num　			(in)描画する画像メモリ番号
	int	x0,y0,x1,y1		(in)直線の始点終点座標
	BYTE	r,g,b			(in)点の描画色
	戻    り    値 :
	機          能 : 画像メモリ上に点を描画する
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void DrawLineOnImage(int num, int x0, int y0, int x1, int y1, BYTE r, BYTE g, BYTE b)
	{
		pimpom_draw_figure(3, num, x0, y0, x1, y1, 1, r, g, b, -1);
	}

	/********************************************************************
	機  能  名  称 : 画像メモリ上に矩形を描画する
	関    数    名 : DrawRectOnImage
	引          数 : int	num　			(in)描画する画像メモリ番号
	int	x0,y0,x1,y1		(in)矩形の４隅座標
	BYTE	r,g,b			(in)点の描画色
	戻    り    値 :
	機          能 : 画像メモリ上に点を描画する
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void DrawRectOnImage(int num, int x0, int y0, int x1, int y1, BYTE r, BYTE g, BYTE b)
	{
		pimpom_draw_figure(4, num, x0, y0, x1, y1, 1, r, g, b, -1);
	}

	/********************************************************************
	機  能  名  称 : 画像メモリ上に円形を描画する
	関    数    名 : DrawCircleOnImage
	引          数 : int	num　			(in)描画する画像メモリ番号
	int	cx,cy			(in)中心座標
	int	rad				(in)半径
	bool	fill			(in)中を塗りつぶすかどうか
	BYTE	r,g,b			(in)点の描画色
	戻    り    値 :
	機          能 : 画像メモリ上に点を描画する
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void DrawCircleOnImage(int num, int cx, int cy, int rad, bool fill, BYTE r, BYTE g, BYTE b)
	{
		pimpom_draw_figure(5, num, cx, cy, rad, fill, 1, r, g, b, -1);
	}


	/********************************************************************
	機  能  名  称 : 画像メモリ上に十字カーソルを描画する
	関    数    名 : DrawCursorOnImage
	引          数 : int	num　			(in)描画する画像メモリ番号
	int	x,y				(in)座標
	BYTE	r,g,b			(in)点の描画色
	戻    り    値 :
	機          能 : 画像メモリ上に点を描画する
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void DrawCursorOnImage(int num, int x, int y, BYTE r, BYTE g, BYTE b)
	{
		pimpom_draw_figure(6, num, x, y, 0, 0, 1, r, g, b, -1);
	}


	/********************************************************************
	機  能  名  称 : 画像メモリ上に矩形を描画する
	関    数    名 : DrawRectOnImage
	引          数 : int	num　			(in)描画する画像メモリ番号
	int	x0,y0,x1,y1		(in)矩形の４隅座標
	BYTE	r,g,b			(in)点の描画色
	戻    り    値 :
	機          能 : 画像メモリ上に点を描画する
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void DrawTextOnImage(int num, int x, int y, BYTE r, BYTE g, BYTE b, char *format, ...)
	{
		va_list list;
		char	buf[5000];

		va_start(list, format);
		vsprintf(buf, format, list);
		va_end(list);

		pimpom_draw_text(10, num, x, y, buf, r, g, b, -1);
	}

	/********************************************************************
	機  能  名  称 : メッセージを出す
	関    数    名 : ShowMessage
	引          数 :
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void ShowMessage(char *format, ...)
	{
		va_list list;
		char	buf[5000];

		va_start(list, format);
		vsprintf(buf, format, list);
		va_end(list);

		pimpom_draw_text(11, 0, 0, 0, buf, 0, 0, 0, -1);
	}


	/********************************************************************
	機  能  名  称 : あたらしくメッセージを出す(過去のものはクリア)
	関    数    名 : ShowMessage
	引          数 :
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void ShowNewMessage(char *format, ...)
	{
		va_list list;
		char	buf[5000];

		va_start(list, format);
		vsprintf(buf, format, list);
		va_end(list);

		pimpom_draw_text(12, 0, 0, 0, buf, 0, 0, 0, -1);
	}

	/********************************************************************
	機  能  名  称 : マーカー追加
	関    数    名 : AddMark
	引          数 : int	num　			(in)画像メモリ番号
	float x,y				(in)座標
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void AddMark(int num, float x, float y, double atrrib0 = 0, double atrrib1 = 0, double atrrib2 = 0)
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;

		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC), &hShare, 1);
		if (!pShereMem)	return;

		pShereMem->type = 13;
		pShereMem->image_num = num;
		sprintf(pShereMem->message, "%f,%f,%lf,%lf,%lf\n", x, y, atrrib0, atrrib1, atrrib2);

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);
		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}


	/********************************************************************
	機  能  名  称 : グラフに値追加
	関    数    名 : AddDataToChart
	引          数 : int	num　			(in)グラフ番号
	float val				(in)値
	戻    り    値 :
	機          能 : 折れ線グラフに値を追加して描画する
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void AddDataToChart(int num, float val)
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;

		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC), &hShare, 1);
		if (!pShereMem)	return;

		pShereMem->type = 20;
		pShereMem->image_num = num;
		sprintf(pShereMem->message, "%f\n", val);

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);
		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}

	/********************************************************************
	機  能  名  称 : グラフに値追加
	関    数    名 : AddDataToChart2
	引          数 : int	num　			(in)グラフ番号
	float valx,valy		(in)値
	戻    り    値 :
	機          能 : 2次元散布図に値を追加して描画する
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void AddDataToChart2(int num, float valx, float valy)
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;

		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC), &hShare, 1);
		if (!pShereMem)	return;

		pShereMem->type = 21;
		pShereMem->image_num = num;
		sprintf(pShereMem->message, "%f,%f\n", valx, valy);

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);
		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}

	/********************************************************************
	機  能  名  称 : グラフをクリア
	関    数    名 : ClearChart
	引          数 : int	num　			(in)グラフ番号
	戻    り    値 :
	機          能 : 2次元散布図に値を追加して描画する
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void ClearChart(int num)
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;

		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC), &hShare, 1);
		if (!pShereMem)	return;

		pShereMem->type = 22;
		pShereMem->image_num = num;

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);
		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}


	/********************************************************************
	機  能  名  称 : ユーザダイアログのバッチ実行関数を呼ぶ
	関    数    名 : BatchExecute
	引          数 : int	num　				(in)ユーザダイアログの番号
	char	param[5][1024]		(in)バッチ実行の入力パラメタ
	char	result[15][1024]	(out)バッチ実行の出力パラメタ
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void BatchExecute(
		int	num,
		char	imae_file_path[2048],//入力画像ファイル
		char	param[5][1024],//入力パラメタ
		char	result[15][1024],//出力パラメタ
		int	*state//実行結果
	)
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;
		char *ptr;

		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC) + SHEAREMEM_EXPAND_MESSAGE_SIZE, &hShare, 1);
		if (!pShereMem)	return;

		pShereMem->type = 30;
		pShereMem->image_num = num;

		//パラメタ書き込み
		ptr = (char*)pShereMem + sizeof(SHEAREMEM_EXT_PROC);
		sprintf(ptr, "%s", imae_file_path);
		ptr += 2048;

		for (int i = 0; i<5; i++) {
			sprintf(ptr, "%s", param[i]);
			ptr += 1024;
		}

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);//転送

																  //結果読み出し
		ptr = (char*)pShereMem + sizeof(SHEAREMEM_EXT_PROC) + SHEAREMEM_EXPAND_MESSAGE_SIZE / 2;
		for (int i = 0; i<15; i++) {
			strcpy(result[i], ptr);
			ptr += 1024;
		}

		*state = (int)(*ptr);



		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}


	/********************************************************************
	機  能  名  称 : コマンド実行
	関    数    名 : ExecuteCommand
	引          数 : (in)command
	(out)result
	(out)err
	戻    り    値 : 実行結果 (成功: 1   失敗:0)
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static int ExecuteCommand(char command[], char result[], char err[])
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;
		char *ptr;
		int ret;



		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC) + SHEAREMEM_EXPAND_MESSAGE_SIZE, &hShare, 1);
		if (!pShereMem)	return 0;

		pShereMem->type = 100;

		//コマンド書き込み
		ptr = (char*)pShereMem + sizeof(SHEAREMEM_EXT_PROC);
		sprintf(ptr, command);


		//転送
		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);

		//結果読み出し
		ptr = (char*)pShereMem + sizeof(SHEAREMEM_EXT_PROC) + SHEAREMEM_EXPAND_MESSAGE_SIZE / 2;
		ret = ptr[0];				//先頭Byte : 実行結果 (成功: 1   失敗:0)
		strcpy(result, &ptr[5000]);	//先頭から5000～10000Byte : 実行結果
		strcpy(err, &ptr[10000]);//先頭から10000～15000Byte : エラーメッセージ

		pimpom_plot_free_shere_mem(pShereMem, hShare);

		return ret;
	}


	/********************************************************************
	機  能  名  称 : PIMPOMから画像データの型式とサイズを出力する
	関    数    名 : GetDataTypeSize
	引          数 : 
					int		*ptype		(out)画像メモリの型
					int		*pwidth		(out)画像幅
					int		*pheight	(out)画像高さ
					int		*ppages		(out)画像の奥行
	戻    り    値 : 実行結果 (成功: 1   失敗:0)
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	//PIMPOMから画像データの型式とサイズを出力する
	static void GetDataTypeSize(int num, int *ptype, int *pwidth, int *pheight, int *ppages)
	{
		int  tmp[4] = { -1 };

		for (int i = 0; i < 4; i++)
		{
			char command[1024] = { 0 };
			char result[1024] = { 0 };
			char err[1024] = { 0 };

			sprintf(command, "GetDataTypeSize(%d,%d)", num,i);
			if (ExecuteCommand(command, result, err)) {
				tmp[i] = atoi(result);
			}
		}

		*ptype = tmp[0];
		*pwidth = tmp[1];
		*pheight = tmp[2];
		*ppages = tmp[3];
	}

	/********************************************************************
	機  能  名  称 : PIMPOMからマスク画像データを入力する
	関    数    名 : GetMaskImage
	引          数 : int num　	(in)画像メモリ番号
	BYTE* pData (out)画像データの先頭ポインタ メモリ領域が確保されていること
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void GetMaskImage(int num, BYTE* pData)
	{
		int width = 0, height = 0, type = 0, page = 0;
		GetDataTypeSize(num, &type, &width, &height, &page);//画像サイズ取得

		pimpom_get_image(21, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	機  能  名  称 : PIMPOMにマスク画像データを出力する
	関    数    名 : SetMaskImage
	引          数 : int num　	(in)画像メモリ番号
	BYTE* pData (in)マスク画像データの先頭ポインタ
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void SetMaskImage(int num, BYTE* pData)
	{
		int width=0, height =0, type = 0, page = 0;
		GetDataTypeSize(num, &type, &width, &height, &page);//画像サイズ取得

		if (width > 0 && height > 0) {
			pimpom_plot_image(21, num, width, height, 1, 1, pData);
		}
	}

	/********************************************************************
	機  能  名  称 : 作業領域を取得
	関    数    名 : GetWorkArea
	引          数 : int num　	(in)画像メモリ番号
	                 int fig_n  (in)図形番号
	戻    り    値 :作業領域(矩形のみ)
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static RECT GetWorkArea(int num, int fig_n)
	{
		int  tmp[4] = { 0 };
		RECT rct = {0};

		for (int i = 0; i < 4; i++)
		{
			char command[1024] = { 0 };
			char result[1024] = { 0 };
			char err[1024] = { 0 };

			sprintf(command, "GetWorkAreaVtx(%d,%d,%d)", num, fig_n, i);
			if (!ExecuteCommand(command, result, err))		return rct;

			tmp[i] = atoi(result);
		}

		rct.left = tmp[0];
		rct.top = tmp[1];
		rct.right = tmp[2];
		rct.bottom = tmp[3];

		return rct;
	}

	/********************************************************************
	機  能  名  称 : 作業領域を設定
	関    数    名 : SetWorkArea
	引          数 : int num　	(in)画像メモリ番号
					int fig_n  (in)図形番号
					RECT area  (in)矩形
					bool *ornot			(out)true:OR領域　false:NOT領域
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void SetWorkArea(int num, int fig_n, int left, int top, int right, int bottom, bool ornot)
	{
		char command[1024] = { 0 };
		char result[1024] = { 0 };
		char err[1024] = { 0 };

		sprintf(command, "SetWorkArea(%d,%d,%d,%d,%d,%d,%d)", num, fig_n, left, top, right, bottom, ornot);
		if (!ExecuteCommand(command, result, err))		return ;
	}


#ifdef __cplusplus
}
#endif

#endif
