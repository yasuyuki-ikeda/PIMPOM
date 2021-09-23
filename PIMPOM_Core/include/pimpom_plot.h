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

#define  PIMPOM_PLOT_FORMAT_BYTE_IMAGE		(0)
#define  PIMPOM_PLOT_FORMAT_SHORT_IMAGE		(1)
#define  PIMPOM_PLOT_FORMAT_WORD_IMAGE		(2)
#define  PIMPOM_PLOT_FORMAT_LONG_IMAGE		(3)
#define  PIMPOM_PLOT_FORMAT_DWORD_IMAGE		(4)
#define  PIMPOM_PLOT_FORMAT_FLOAT_IMAGE		(5)
#define  PIMPOM_PLOT_FORMAT_3D_IMAGE		(10)
#define  PIMPOM_PLOT_FORMAT_F3D_IMAGE		(11)
#define  PIMPOM_PLOT_FORMAT_RGB_IMAGE		(20)
#define  PIMPOM_PLOT_FORMAT_MASK_IMAGE		(21)
#define  PIMPOM_PLOT_FORMAT_FLOAT_1D_CHART		(30)
#define  PIMPOM_PLOT_FORMAT_DOUBLE_1D_CHART		(31)
#define  PIMPOM_PLOT_FORMAT_INT_1D_CHART		(32)
#define  PIMPOM_PLOT_FORMAT_BYTE_1D_CHART		(33)
#define  PIMPOM_PLOT_FORMAT_FLOAT_2D_CHART		(40)
#define  PIMPOM_PLOT_FORMAT_DOUBLE_2D_CHART		(41)
#define  PIMPOM_PLOT_FORMAT_INT_2D_CHART		(42)
#define  PIMPOM_PLOT_FORMAT_BYTE_2D_CHART		(43)

#define  PIMPOM_PLOT_COMMAND_PREPAIR_DRAWING	(0)
#define  PIMPOM_PLOT_COMMAND_DRAW_IMAGE	(1)
#define  PIMPOM_PLOT_COMMAND_DRAW_POINT_ON_IMAGE	(2)
#define  PIMPOM_PLOT_COMMAND_DRAW_LINE_ON_IMAGE	(3)
#define  PIMPOM_PLOT_COMMAND_DRAW_RECT_ON_IMAGE	(4)
#define  PIMPOM_PLOT_COMMAND_DRAW_CIRCLE_ON_IMAGE	(5)
#define  PIMPOM_PLOT_COMMAND_DRAW_CURSOR_ON_IMAGE	(6)
#define  PIMPOM_PLOT_COMMAND_DRAW_TEXT_ON_IMAGE	(10)
#define  PIMPOM_PLOT_COMMAND_SHOW_MESSAGE	(11)
#define  PIMPOM_PLOT_COMMAND_SHOW_NEW_MESSAGE	(12)
#define  PIMPOM_PLOT_COMMAND_ADD_MARK	(13)
#define  PIMPOM_PLOT_COMMAND_ADD_DATA_TO_CAHRT	(20)
#define  PIMPOM_PLOT_COMMAND_ADD_DATA_TO_CAHRT2	(21)
#define  PIMPOM_PLOT_COMMAND_CLEAR_CAHRT	(22)
#define  PIMPOM_PLOT_COMMAND_BATCH_EXECUTE	(30)
#define  PIMPOM_PLOT_COMMAND_EXECUTE	(100)

#define  PIMPOM_PLOT_ASYNC_PARAM_LEN  (1000)
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
				   //	0:BYTE 1:short 2:WORD 3:long 4:DWORD 5:FLOAT 10:3D 11:FLOAT3D: 20 RGB: 21 MASK
				   //	30～33:折れ線グラフ表示用データ  40～43:散布図表示用データ
	int	image_num;//表示先の画像メモリ番号
	int		width;//幅
	int		height;//高さ
	int		page;//奥行
	int		channel;//チャネル数
	int		size;//共有メモリのサイズ
	BYTE	pData[];//画像データ
}SHEAREMEM_PLOT_IMAGE;

#pragma	pack(pop)
#pragma	warning(pop)

typedef struct {
	int		asyncFlg;//1 : PIMPOMから外部プログラムへの非同期出力  2:外部プログラムからPIMPOMへの非同期入力  0:入出力不可
	float	params[PIMPOM_PLOT_ASYNC_PARAM_LEN];//非同期通信パラメータ
	SHEAREMEM_PLOT_IMAGE  memory;
}SHEAREMEM_PLOT_IMAGE_ASYNC;


//画像転送を伴わないメッセージを受け渡しするための共有メモリのヘッダ
typedef struct {
	int		type;//メッセージタイプ　
                  //0:画像上への描画の背景となる画像のコピー  1:画像描画  2:画像上に点描画  3:画像上に直線描画 4:画像上に矩形描画 5:画像上に円描画 6:画像上に十字カーソル描画 10:画像上にテキスト描画 
                  //11:メッセージエリアに追加表示   12:メッセージエリアに新規表示  13:マーク追加  20:折れ線グラフ要素追加  21:散布図要素追加  22:グラフクリア 
                  //30:バッチ実行  100:スクリプトコマンド実行
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
static int pimpom_async_ids[2] = {2,1};//非同期入力のID [0]このプロセスのID  [1]相手のプロセスのID

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
		else if (id > 0) {
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
		case PIMPOM_PLOT_FORMAT_BYTE_IMAGE://BYTE型メモリ
			pix_size = sizeof(BYTE);
			break;

		case PIMPOM_PLOT_FORMAT_SHORT_IMAGE://SHORT型メモリ
			pix_size = sizeof(short);
			break;

		case PIMPOM_PLOT_FORMAT_WORD_IMAGE://WORD型メモリ
			pix_size = sizeof(WORD);
			break;

		case PIMPOM_PLOT_FORMAT_LONG_IMAGE://LONG型メモリ
			pix_size = sizeof(long);
			break;

		case PIMPOM_PLOT_FORMAT_DWORD_IMAGE://DWORD型メモリ
			pix_size = sizeof(DWORD);
			break;

		case PIMPOM_PLOT_FORMAT_FLOAT_IMAGE://float型メモリ
			pix_size = sizeof(float);
			break;

		case PIMPOM_PLOT_FORMAT_3D_IMAGE://3D型メモリ
			pix_size = sizeof(BYTE);
			break;

		case PIMPOM_PLOT_FORMAT_F3D_IMAGE://F3D型メモリ
			pix_size = sizeof(float);
			break;

		case PIMPOM_PLOT_FORMAT_RGB_IMAGE://RGB型メモリ
			pix_size = sizeof(BYTE);
			break;

		case PIMPOM_PLOT_FORMAT_MASK_IMAGE://マスクメモリ
			pix_size = sizeof(BYTE);
			break;

		case PIMPOM_PLOT_FORMAT_FLOAT_1D_CHART://1Dグラフ(float)
		case PIMPOM_PLOT_FORMAT_FLOAT_2D_CHART://2Dグラフ(float)
			pix_size = sizeof(float);
			break;

		case PIMPOM_PLOT_FORMAT_DOUBLE_1D_CHART://1Dグラフ(double)
		case PIMPOM_PLOT_FORMAT_DOUBLE_2D_CHART://2Dグラフ(double)
			pix_size = sizeof(double);
			break;

		case PIMPOM_PLOT_FORMAT_INT_1D_CHART://1Dグラフ(int)
		case PIMPOM_PLOT_FORMAT_INT_2D_CHART://2Dグラフ(int)
			pix_size = sizeof(int);
			break;

		case PIMPOM_PLOT_FORMAT_BYTE_1D_CHART://1Dグラフ(BYTE)
		case PIMPOM_PLOT_FORMAT_BYTE_2D_CHART://2Dグラフ(BYTE)
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

		pimpom_plot_send_data(WM_USER + 100, 0, pimpom_target_id);//共有メモリからPIMPOMに読み出すようにメッセージを送信
		pimpom_plot_free_shere_mem(pShereMem, hShare);//共有メモリを解放
	}


	//PIMPOMから画像データを入力する
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


		pimpom_plot_send_data(WM_USER + 102, 0, pimpom_target_id);//PIMPOMから共有メモリにデータ格納するようにメッセージを送信
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
	機  能  名  称 : PIMPOMとの共有メモリから画像データを非同期読み込みする
	関    数    名 : GetImageAsync
	引          数 :
	int num　	(in)画像メモリ番号
	int format (out)データ型式
	int *height (out)高さ
	int *page   (out)奥行
	int *channel   (out)チャネル
	void** ppData (out)画像データのポインタへのポインタ（ここに新しいメモリが確保される）
	int checkAsyncFlg  (in)非同期参照フラグのチェックをする。所望のものであればtrueを返す。
					(負数の場合はチェックせずに読み出す)
	float* pAsyncParams (out)パラメタ
	戻    り    値 :成功したらtrue
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static int pimpom_get_image_async(int format, int *pImageNum, int *pWidth, int *pHeight, int *pPage, int *pChannel, void** ppData, int checkAsyncFlg, float* pAsyncParams)
	{
		SHEAREMEM_PLOT_IMAGE_ASYNC	*pShmem;//共有メモリ領域
		SHEAREMEM_PLOT_IMAGE_ASYNC	shmem_header = { 0 };
		HANDLE	hShare;//共有メモリのハンドル
		int pix_size;//画像データのピクセルサイズ
		int data_size;

		//共有メモリ領域のヘッダ部分を読む
		pShmem = (SHEAREMEM_PLOT_IMAGE_ASYNC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_PLOT_IMAGE_ASYNC), &hShare, 0);
		if (!pShmem)	return 0;

		memcpy(&shmem_header, pShmem, sizeof(SHEAREMEM_PLOT_IMAGE_ASYNC));
		pimpom_plot_free_shere_mem(pShmem, hShare);//共有メモリ解放

		if (pAsyncParams) {
			memcpy(pAsyncParams, shmem_header.params, PIMPOM_PLOT_ASYNC_PARAM_LEN * sizeof(float));
		}

		if (checkAsyncFlg >= 0) 
		{//非同期読み込み可能かチェック
			if (shmem_header.asyncFlg != checkAsyncFlg) {
				return 0;//非同期参照フラグが所望のものではない
			}
			else {
				return 1;//非同期参照フラグが所望のものである
			}
		}
		else 
		{//非同期読み込み実行

		 //画像メモリヘッダの異常チェック
			if (shmem_header.memory.format != format || shmem_header.memory.width <= 0 || shmem_header.memory.height <= 0
				|| shmem_header.memory.channel <= 0 || shmem_header.memory.page <= 0 || shmem_header.memory.size <= 0)	return 0;

			//共有メモリ領域の全体読み直す
			pShmem = (SHEAREMEM_PLOT_IMAGE_ASYNC*)pimpom_plot_get_shere_mem(shmem_header.memory.size, &hShare, 0);
			if (!pShmem)	return 0;

			pix_size = get_pix_size(shmem_header.memory.format);
			data_size = pix_size*shmem_header.memory.width*shmem_header.memory.height*shmem_header.memory.page*shmem_header.memory.channel;

			*ppData = (void**)malloc(data_size);//データ用のメモリ領域確保
			if ((*ppData) == NULL) {
				pimpom_plot_free_shere_mem(pShmem, hShare);//共有メモリ解放
				return 0;
			}

			memcpy(*ppData, pShmem->memory.pData, data_size);
			pimpom_plot_free_shere_mem(pShmem, hShare);//共有メモリ解放

			*pImageNum = shmem_header.memory.image_num;
			*pWidth = shmem_header.memory.width;
			*pHeight = shmem_header.memory.height;
			*pChannel = shmem_header.memory.channel;
			*pPage = shmem_header.memory.page;
		}

		return 1;
	}

	/********************************************************************
	機  能  名  称 : PIMPOMとの共有メモリへ画像データを非同期書き込みする
	関    数    名 : PlotImageAsync
	引          数 :
	int num　	(in)画像メモリ番号
	int format (in)データ型式
	int width	(in)幅
	int height (in)高さ
	int page   (in)奥行
	int channel   (in)チャネル
	void* pData (in)画像データの先頭ポインタ
	int asyncFlg (in)非同期参照フラグのセット(負数の場合は同期終了)
	float* pAsyncParams (in)パラメタ
	戻    り    値 :
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void pimpom_plot_image_async(int format, int num, int width, int height, int page, int channel, void* pData, int asyncFlg, float* pAsyncParams)
	{
		static HANDLE	hShare = NULL;
		static SHEAREMEM_PLOT_IMAGE_ASYNC	*pShereMem = NULL;
		int		pix_size, sheremem_size;


		if (pShereMem) {
			pimpom_plot_free_shere_mem(pShereMem, hShare);//共有メモリを解放
		}

	
		pix_size = get_pix_size(format);//画像データのピクセルサイズ
		sheremem_size = sizeof(SHEAREMEM_PLOT_IMAGE_ASYNC) + width*height*pix_size*page*channel;//画像データのサイズ
		pShereMem = (SHEAREMEM_PLOT_IMAGE_ASYNC*)pimpom_plot_get_shere_mem(sheremem_size, &hShare, 1);//共有メモリ取得
		if (!pShereMem)	return;


		pShereMem->memory.format = format;
		pShereMem->memory.width = width;
		pShereMem->memory.height = height;
		pShereMem->memory.page = page;
		pShereMem->memory.channel = channel;
		pShereMem->memory.size = sheremem_size;
		memcpy(pShereMem->memory.pData, pData, width*height*pix_size*page*channel);

		pShereMem->asyncFlg = asyncFlg;
		if (pAsyncParams) {
			memcpy(pShereMem->params, pAsyncParams, PIMPOM_PLOT_ASYNC_PARAM_LEN*sizeof(float));
		}
	}


	/********************************************************************
	機  能  名  称 : 非同期参照フラグの値を初期値0に戻す
	関    数    名 : InitAysnc
	引          数 :
	戻    り    値 : 
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void InitAysnc(int thisIsPIMPOM)
	{
		if (thisIsPIMPOM) {
			pimpom_async_ids[0] = 1;
			pimpom_async_ids[1] = 2;
		}
	}

	/********************************************************************
	機  能  名  称 : 非同期参照可能かチェックする
	関    数    名 : EnableAysnc
	引          数 :　
	float* pAsyncParams (out)パラメタ
	戻    り    値 : 相手のプロセスのIDならば1を返すそれ以外は0を返す
	機          能 :
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static int EnableAysnc(float* pAsyncParams)
	{
		return pimpom_get_image_async(0, NULL, NULL, NULL, NULL, NULL, NULL, pimpom_async_ids[1], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_BYTE_IMAGE, num, width, height, 1, 1, pData);
	}

	static void PlotByteImageAsync(int num, int width, int height, BYTE* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_BYTE_IMAGE, num, width, height, 1, 1, pData, pimpom_async_ids[0], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_SHORT_IMAGE, num, width, height, 1, 1, pData);
	}

	static void PlotShortImageAsync(int num, int width, int height, short* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_SHORT_IMAGE, num, width, height, 1, 1, pData, pimpom_async_ids[0], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_WORD_IMAGE, num, width, height, 1, 1, pData);
	}

	static void PlotWordImageAsync(int num, int width, int height, WORD* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_WORD_IMAGE, num, width, height, 1, 1, pData, pimpom_async_ids[0], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_LONG_IMAGE, num, width, height, 1, 1, pData);
	}

	static void PlotLongImageAsync(int num, int width, int height, long* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_LONG_IMAGE, num, width, height, 1, 1, pData, pimpom_async_ids[0], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_DWORD_IMAGE, num, width, height, 1, 1, pData);
	}

	static void PlotDwordImageAsync(int num, int width, int height, DWORD* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_DWORD_IMAGE, num, width, height, 1, 1, pData, pimpom_async_ids[0], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_FLOAT_IMAGE, num, width, height, 1, 1, pData);
	}

	static void PlotFloatImageAsync(int num, int width, int height, float* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_FLOAT_IMAGE, num, width, height, 1, 1, pData,  pimpom_async_ids[0], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_3D_IMAGE, num, width, height, page, 1, pData);
	}

	static void Plot3DImageAsync(int num, int width, int height, int page, BYTE* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_3D_IMAGE, num, width, height, page, 1, pData,  pimpom_async_ids[0], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_F3D_IMAGE, num, width, height, page, 1, pData);
	}

	static void PlotF3DImageAsync(int num, int width, int height, int page, float* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_F3D_IMAGE, num, width, height, page, 1, pData, pimpom_async_ids[0], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_RGB_IMAGE, num, width, height, 1, 3, pData);
	}

	static void PlotRGBImageAsync(int num, int width, int height, BYTE* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_RGB_IMAGE, num, width, height, 1, 3, pData,  pimpom_async_ids[0], pAsyncParams);
	}


	/********************************************************************
	機  能  名  称 : 1Dデータを折れ線表示
	関    数    名 : PlotDataToChart
	引          数 :
	int	num　			(in)グラフ番号
	int			dataNum		(in)データの数
	float		*pData				(in)値
	戻    り    値 :
	機          能 : 折れ線グラフに値を描画する
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void PlotDataToChartF(int num, int dataNum, float *pData)
	{
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_FLOAT_1D_CHART, num, dataNum, 1, 1, 1, pData);
	}

	static void PlotDataToChartD(int num, int dataNum, double *pData)
	{
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_DOUBLE_1D_CHART, num, dataNum, 1, 1, 1, pData);
	}

	static void PlotDataToChartI(int num, int dataNum, int *pData)
	{
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_INT_1D_CHART, num, dataNum, 1, 1, 1, pData);
	}

	static void PlotDataToChartB(int num, int dataNum, BYTE *pData)
	{
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_BYTE_1D_CHART, num, dataNum, 1, 1, 1, pData);
	}

	/********************************************************************
	機  能  名  称 : 2Dデータを散布図表示
	関    数    名 : PlotDataToChart
	引          数 :
	int			num　			(in)グラフ番号
	int			dataNum		(in)データの数
	float		*pDataX,*pDataY		(in)値
	戻    り    値 :
	機          能 : 2次元散布図に値を描画する
	日付         作成者          内容
	------------ --------------- ---------------------------------------
	Y.Ikeda         新規作成
	********************************************************************/
	static void PlotDataToChart2F(int num, int dataNum, float *pDataX, float *pDataY)
	{
		float *pdataBuf = new float[dataNum * 2];
		memcpy(pdataBuf, pDataX, dataNum * sizeof(float));
		memcpy(pdataBuf + dataNum, pDataY, dataNum * sizeof(float));

		pimpom_plot_image(PIMPOM_PLOT_FORMAT_FLOAT_2D_CHART, num, dataNum, 2, 1, 1, pdataBuf);

		delete[] pdataBuf;
	}

	static void PlotDataToChart2D(int num, int dataNum, double *pDataX, double *pDataY)
	{
		double *pdataBuf = new double[dataNum * 2];
		memcpy(pdataBuf, pDataX, dataNum * sizeof(double));
		memcpy(pdataBuf + dataNum, pDataY, dataNum * sizeof(double));

		pimpom_plot_image(PIMPOM_PLOT_FORMAT_DOUBLE_2D_CHART, num, dataNum, 2, 1, 1, pdataBuf);

		delete[] pdataBuf;
	}

	static void PlotDataToChart2I(int num, int dataNum, int *pDataX, int *pDataY)
	{
		int *pdataBuf = new int[dataNum * 2];
		memcpy(pdataBuf, pDataX, dataNum * sizeof(int));
		memcpy(pdataBuf + dataNum, pDataY, dataNum * sizeof(int));

		pimpom_plot_image(PIMPOM_PLOT_FORMAT_INT_2D_CHART, num, dataNum, 2, 1, 1, pdataBuf);

		delete[] pdataBuf;
	}

	static void PlotDataToChart2B(int num, int dataNum, BYTE *pDataX, BYTE *pDataY)
	{
		BYTE *pdataBuf = new BYTE[dataNum * 2];
		memcpy(pdataBuf, pDataX, dataNum * sizeof(BYTE));
		memcpy(pdataBuf + dataNum, pDataY, dataNum * sizeof(BYTE));

		pimpom_plot_image(PIMPOM_PLOT_FORMAT_BYTE_2D_CHART, num, dataNum, 2, 1, 1, pdataBuf);

		delete[] pdataBuf;
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_BYTE_IMAGE, num, width, height, 1, 1, pData);
	}

	static int GetByteImageAsync(int *num, int *width, int *height, BYTE** ppData)
	{
		int page, channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_BYTE_IMAGE, num, width, height, &page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_SHORT_IMAGE, num, width, height, 1, 1, pData);
	}

	static int GetShortImageAsync(int *num, int *width, int *height, short** ppData)
	{
		int page, channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_SHORT_IMAGE, num, width, height, &page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_WORD_IMAGE, num, width, height, 1, 1, pData);
	}

	static int GetWordImageAsync(int *num, int *width, int *height, WORD** ppData)
	{
		int page, channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_WORD_IMAGE, num, width, height, &page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_LONG_IMAGE, num, width, height, 1, 1, pData);
	}

	static int GetLongImageAsync(int *num, int *width, int *height, long** ppData)
	{
		int page, channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_LONG_IMAGE, num, width, height, &page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_DWORD_IMAGE, num, width, height, 1, 1, pData);
	}

	static int GetDwordImageAsync(int *num, int *width, int *height, DWORD** ppData)
	{
		int page, channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_DWORD_IMAGE, num, width, height, &page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_FLOAT_IMAGE, num, width, height, 1, 1, pData);
	}

	static int GetFloatImageAsync(int *num, int *width, int *height, float** ppData)
	{
		int page, channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_FLOAT_IMAGE, num, width, height, &page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_3D_IMAGE, num, width, height, page, 1, pData);
	}

	static int Get3DImageAsync(int *num, int *width, int *height, int *page, BYTE** ppData)
	{
		int channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_3D_IMAGE, num, width, height, page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_F3D_IMAGE, num, width, height, page, 1, pData);
	}

	static int GetF3DImageAsync(int *num, int *width, int *height, int *page, float** ppData)
	{
		int channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_F3D_IMAGE, num, width, height, page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_RGB_IMAGE, num, width, height, 1, 3, pData);
	}

	static int GetRGBImageAsync(int *num, int *width, int *height, BYTE** ppData)
	{
		int page, channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_RGB_IMAGE, num, width, height, &page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_draw_figure(PIMPOM_PLOT_COMMAND_PREPAIR_DRAWING, num, 0, 0, 0, 0, 0, 0, 0, 0, copy_src_num);
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

		pShereMem->type = PIMPOM_PLOT_COMMAND_DRAW_IMAGE;
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
		pimpom_draw_figure(PIMPOM_PLOT_COMMAND_DRAW_POINT_ON_IMAGE, num, x, y, 0, 0, 1, r, g, b, -1);
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
		pimpom_draw_figure(PIMPOM_PLOT_COMMAND_DRAW_LINE_ON_IMAGE, num, x0, y0, x1, y1, 1, r, g, b, -1);
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
		pimpom_draw_figure(PIMPOM_PLOT_COMMAND_DRAW_RECT_ON_IMAGE, num, x0, y0, x1, y1, 1, r, g, b, -1);
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
		pimpom_draw_figure(PIMPOM_PLOT_COMMAND_DRAW_CIRCLE_ON_IMAGE, num, cx, cy, rad, fill, 1, r, g, b, -1);
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
		pimpom_draw_figure(PIMPOM_PLOT_COMMAND_DRAW_CURSOR_ON_IMAGE, num, x, y, 0, 0, 1, r, g, b, -1);
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

		pimpom_draw_text(PIMPOM_PLOT_COMMAND_DRAW_TEXT_ON_IMAGE, num, x, y, buf, r, g, b, -1);
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

		pimpom_draw_text(PIMPOM_PLOT_COMMAND_SHOW_MESSAGE, 0, 0, 0, buf, 0, 0, 0, -1);
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

		pimpom_draw_text(PIMPOM_PLOT_COMMAND_SHOW_NEW_MESSAGE, 0, 0, 0, buf, 0, 0, 0, -1);
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

		pShereMem->type = PIMPOM_PLOT_COMMAND_ADD_MARK;
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

		pShereMem->type = PIMPOM_PLOT_COMMAND_ADD_DATA_TO_CAHRT;
		pShereMem->image_num = num;
		sprintf(pShereMem->message, "%f\n", val);

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);
		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}



	/********************************************************************
	機  能  名  称 : グラフに値追加
	関    数    名 : AddDataToChart2
	引          数 : 
	int	num　			(in)グラフ番号
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

		pShereMem->type = PIMPOM_PLOT_COMMAND_ADD_DATA_TO_CAHRT2;
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

		pShereMem->type = PIMPOM_PLOT_COMMAND_CLEAR_CAHRT;
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

		pShereMem->type = PIMPOM_PLOT_COMMAND_BATCH_EXECUTE;
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
	機  能  名  称 : スクリプトコマンド実行
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

		pShereMem->type = PIMPOM_PLOT_COMMAND_EXECUTE;

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

		pimpom_get_image(PIMPOM_PLOT_FORMAT_MASK_IMAGE, num, width, height, 1, 1, pData);
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
			pimpom_plot_image(PIMPOM_PLOT_FORMAT_MASK_IMAGE, num, width, height, 1, 1, pData);
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
