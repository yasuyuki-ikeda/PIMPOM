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
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"

// *************************************
//         マ  ク  ロ   定   義         
// *************************************
#pragma comment(lib, "version.lib")
#define numof(array) (sizeof(array)/sizeof(array[0]))


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

///////////////////////////////////////////////////////////////////////////////////
//初期化処理・終了処理		
///////////////////////////////////////////////////////////////////////////////////

//タイトルにkeyを含むウィンドウハンドルを検索する
static HWND find_window_from_key(HWND hWndParent, TCHAR key[])
{
	TCHAR szBuff[512];

	SendMessage(hWndParent, WM_GETTEXT, 255, (LPARAM)szBuff);
	if (_tcsstr(szBuff, key)){
		return hWndParent;
	}
	else{
		HWND hWndChild = ::FindWindowEx(hWndParent, NULL, _T("#32770"), NULL);
		if (hWndChild == NULL)	return NULL;

		do {
			HWND wnd = find_window_from_key(hWndChild, key);
			if (wnd)	return wnd;

		} while ((hWndChild = ::FindWindowEx(hWndParent, hWndChild, _T("#32770"), NULL)) != NULL);
	}
	return NULL;
}





CPimpomAPI::CPimpomAPI()
{
	//モードレスダイアログの初期化******************************************:
	//標準のウインドウ
	pPIMMOMDlg=NULL;//メインウインドウのポインタ
	pDataDlg=NULL;//データウインドウのポインタ
	pHistgramDlg=NULL;//ヒストグラムウインドウのポインタ
	pFlterCtrlDlg=NULL;//汎用フィルタウィンドウのポインタ
	pCalcDlg=NULL;//計算用
	pBinalizeDlg=NULL;//2値化
	pBatchDlg=NULL;//バッチ処理
	pWorkAreaDlg=NULL;
	pMarkDlg=NULL;//マーク表示用
	pDataPickDlg=NULL;//データのピックアップ
	pSocketCtrlDlg=NULL;//ソケット通信
	pChartDlg=NULL;//グラフ表示
	pCmdPrptDlg=NULL;//コマンドプロンプト
	pPythonBatchDlg = NULL;//Pythonバッチ処理


	//ユーザダイアログ
	for(int i=0 ; i<CUSTOM_FUNC_MAX ; i++){
		custom_func[i].dllname.Empty();
		custom_func[i].caption.Empty();
		custom_func[i].hInst = NULL;
	}

	//環境設定を初期化******************************************:
	data_number = DATA_NUMBER_DEFAULT;

	//デフォルトフォント
	default_font.lfHeight = 11;//高さ
	default_font.lfWidth = 0;//平均幅（自動）
	default_font.lfEscapement =0;//基線角度
	default_font.lfOrientation =0;//文字角度	
	default_font.lfWeight =FW_THIN;//厚み
	default_font.lfItalic = false;//斜体
	default_font.lfUnderline = false;//下線
	default_font.lfStrikeOut =false;//打消し線	
	default_font.lfCharSet = SHIFTJIS_CHARSET;//文字セット	
	default_font.lfOutPrecision = OUT_DEFAULT_PRECIS;//出力精度	
	default_font.lfClipPrecision = CLIP_DEFAULT_PRECIS;//クリッピング精度	
	default_font.lfQuality = DEFAULT_QUALITY;//出力品質	
	default_font.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;//ピッチ・ファミリ	
	sprintf( default_font.lfFaceName, "ＭＳゴシック");//書体名	
	
	default_bg_color = RGB(190,190,190);//標準の背景色
	default_fg_color = RGB(0,255,0);//標準のグラフィック色
	//Y←0.299×R+0.587×G+0.114×B
	rgb_gain[0] = 299;//299;//Rゲイン
	rgb_gain[1] = 587;//587;//Gゲイン
	rgb_gain[2] = 114;//114;//Bゲイン
	socket_delimiter =0;//デリミタ

	invisible = false;//画面不可視化

	batch_execute_rule = 0;//コマンドプロンプトをバッチ実行するときのルール(0:カスタムファンクションの前 1：カスタムファンクションの後 0,1以外:実行しない)



	//画像メモリ一覧表示ウィンドウに表示する画像サイズ	
	small_image_size = CSize(SMALL_W, SMALL_H);


	//画像メモリ******************************************:
	//DataUnitへのポインタ配列を作る
	pp_data_unit = (CDataUnit**)malloc(DATA_NUMBER_MAX * sizeof(CDataUnit**));
	for(int n=0 ; n<DATA_NUMBER_MAX ; n++){
		pp_data_unit[n] = NULL;
	}

	current_image_number = 0;//現在選択中の画像番号を0で初期化
	


	//バックグラウンド処理を初期化******************
	background_proc_id = BGPROC_NOTHING;//
	

	backup_data = NULL;//バックアップ用画像メモリを初期化
	//capture=NULL;
	pSockCtrl = NULL;


	//複数起動時の対応(id付与)
	for (int i = 0; i < 10; i++)
	{
		TCHAR key[256];

		if (i == 0){
			_stprintf(key, " - PIMPOM  ");
		}
		else{
			_stprintf(key, " - PIMPOM %d", i);
		}

		HWND wnd = find_window_from_key(::GetDesktopWindow(), key);
		if (wnd == NULL){
			pimpom_id = i;
			break;
		}
	}
}

CPimpomAPI::~CPimpomAPI(void)
{
	//各種ウィンドウの開放
	DialogDelete(IDD_DATA_DLG);
	DialogDelete(IDD_FILTER_CTRL_DLG);
	DialogDelete(IDD_CALC_DLG);
	DialogDelete(IDD_BINALIZE_DLG);
	DialogDelete(IDD_BATCH_DLG);
	DialogDelete(IDD_MULTI_IMAGE_DISP_DLG); 
	DialogDelete(IDD_3D_IMAGE_DISP_DLG);
	DialogDelete(IDD_WORKAREA_DLG);
	DialogDelete(IDD_MARK_DLG);
	DialogDelete(IDD_HISTGRAM_DLG);
	DialogDelete(IDD_DATAPICK_DLG);
	DialogDelete(IDD_CHART_DLG);
	DialogDelete(IDD_SOCKET_CTRL_DLG);
	DialogDelete(IDD_COMMAND_RPOMPT_DLG);

	
	//ユーザダイアログの開放
	for(int i=0 ; i<CUSTOM_FUNC_MAX ; i++){	
		UserDialogDelete(i);
	}


	//バックアップデータの開放
	if(backup_data){
		delete	backup_data;
	}

	//画像メモリの開放
	for(int n=0 ; n<data_number ; n++){
		if(pp_data_unit[n]){
			delete	pp_data_unit[n];
		}
	}
	free(pp_data_unit);

	//ビデオキャプチャの開放
	CaptureDeInit();

	//ソケットの開放
	if(pSockCtrl){
		CloseSocket();
		delete pSockCtrl;
	}
}

/********************************************************************
機  能  名  称 :iniデータを読み出す
関    数    名 : GetProfile
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
CString		CPimpomAPI::GetProfile(CString key1, CString key2)
{
	CWinApp *pApp=AfxGetApp();
	return pApp->GetProfileString(key1, key2, NULL);
}


/********************************************************************
機  能  名  称 :iniファイルに書き込む
関    数    名 : WriteProfile
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void		CPimpomAPI::WriteProfile(CString key1, CString key2, CString str)
{
	CWinApp *pApp=AfxGetApp();
	pApp->WriteProfileString(key1, key2, str);
}

/********************************************************************
機  能  名  称 :/INIファイルから設定を読み込む
関    数    名 : InitSettingFromProfile
引          数 :
戻    り    値 :
機          能 :pimpom.iniファイルから以下の値を読み込んで設定する
　　　　　　　　 ・画像メモリ数
		 　　　　・最初に表示する画像メモリ番号
			 　　・メイン画面のサイズ（幅／高さ）
			　   ・データウィンドウの幅
			   　・サムネイル画像のサイズ（幅／高さ）
			　　 ・ユーザ定義ダイアログ
				（・画像データ）
				（・サムネイル画像の行数）

日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::InitSettingFromProfile()
{	
	CString key_path, key_w, key_h, key_page, buf;
	CString path;
	short	width, height, page;

	data_number =  _ttoi( GetProfile("SETTING", "IMG_NUM"));//画像メモリの数
	if (data_number < 1) 
	{
		data_number = DATA_NUMBER_DEFAULT;
	}


	//画像メモリにファイルを入力
	for(int i=0 ; i<GetDataUnitNumber() ; i++ ){
		key_path.Format("IMG%d",i);
		key_w.Format("IMG%dW",i);
		key_h.Format("IMG%dH",i);
		key_page.Format("IMG%dPage",i);

		path = GetProfile("DEFAULT_IMAGE", key_path);
		if(!path.IsEmpty())
		{	
			buf = GetProfile("DEFAULT_IMAGE", key_w);
			if(buf.IsEmpty()){		width = 0;				}
			else{					width = _ttoi( buf );	}

			buf = GetProfile("DEFAULT_IMAGE", key_h);
			if(buf.IsEmpty()){		height = 0;				}
			else{					height = _ttoi( buf );	}

			buf = GetProfile("DEFAULT_IMAGE", key_page);
			if(buf.IsEmpty()){		page = 1;				}
			else{					page = _ttoi( buf );	}

			if( SelectAndLoadImageFile(i,path,width,height,page) )
			{ 
				GetDataUnit(i)->UpdateDispBuffer();
			}
		}
	}

	//初期選択画像番号
	current_image_number = _ttoi( GetProfile("SETTING", "DEFAULT_CURRENT_IMAGE"));
	if (current_image_number < 0 || current_image_number >= data_number) 
	{
		current_image_number = 0;
	}


	//メイン画像のサイズ
	main_image_size.cx = _ttoi( GetProfile("SETTING", "MAIN_IMG_W"));
	if (main_image_size.cx < 1) 
	{
		main_image_size.cx = MAIN_W;
	}

	main_image_size.cy = _ttoi( GetProfile("SETTING", "MAIN_IMG_H"));
	if (main_image_size.cy < 1)
	{
		main_image_size.cy = MAIN_H;
	}


	//メインウィンドウのサイズ
	main_window_size.cx = MAIN_WIN_LEFT_MARGIN + main_image_size.cx + MAIN_WIN_RIGHT_MARGIN;
	main_window_size.cy = MAIN_WIN_TOP_MARGIN + main_image_size.cy + MAIN_WIN_BOTTOM_MARGIN;

	//データウィンドウの幅
	data_window_width = _ttoi( GetProfile("SETTING", "DATA_WIN_W"));
	if (data_window_width < 1)
	{
		data_window_width = DATA_WIN_W;
	}

	//１行に表示するサムネイル数
	thumbneil_colum = _ttoi(GetProfile("SETTING", "THUMBNEIL_COLMN"));
	if (thumbneil_colum < 1)
	{
		thumbneil_colum = SMALL_COLUM;
	}


	//サムネイル画像サイズ	
	small_image_size.cx = _ttoi( GetProfile("SETTING", "SMALL_IMG_W"));
	if (small_image_size.cx < 1 || small_image_size.cx >= data_window_width) 
	{
		small_image_size.cx = SMALL_W;
	}

	small_image_size.cy = _ttoi( GetProfile("SETTING", "SMALL_IMG_H"));
	if (small_image_size.cy < 1)
	{
		small_image_size.cy = SMALL_H;
	}

	//フォントサイズ
	default_font.lfHeight = _ttoi( GetProfile("SETTING", "FONT_SIZE"));
	if (default_font.lfHeight  < 1)
	{
		default_font.lfHeight = FONT_SIZE;
	}


	CString  defDlg = GetProfile("SETTING", "DEFEULT_USER_DLG");
	invisible = _ttoi(GetProfile("SETTING", "INVISIBLE"));

	//バッチ実行するpythonのファイルパス
	python_batch_file = GetProfile("SETTING", "PYTHON_BATCH_FILE");


	//ユーザダイアログの読み込み
	{
		FILE *fp;
		char buffer[1024];
		CString line, dllname, caption;
		HINSTANCE	hInst=NULL;
		int		count=0;

		//iniファイルから使用するdll入力
		if( (fp = fopen(GetDefaultPath() + "\\custom_func.ini","rt")) != NULL){
			count=0;
			for(int i=0 ; i<CUSTOM_FUNC_MAX ; i++)
			{
				if( fgets(buffer, 1024, fp) != NULL )
				{
					line = CString(buffer);//1ライン読み込む
					int sprit = line.Find("=");
					if(sprit>0){
						dllname = line.Left(sprit) + ".dll";//dll名
						caption = line.Mid(sprit+1, line.GetLength());//キャプション

						hInst = AfxLoadLibrary( dllname );//DLLロードする
						if(hInst != NULL){
							custom_func[count].dllname = dllname;
							custom_func[count].caption = caption;
							custom_func[count].hInst = hInst;

							if (defDlg == caption.Left(defDlg.GetLength()) && !defDlg.IsEmpty()){
								UserDialogOpen(count);
							}

							count++;
						}else{
							AfxMessageBox(dllname + _T("が見つかりません"));
						}
					}
				}else{
					break;
				}
			}

			fclose(fp);
		}
	}
}




///////////////////////////////////////////////////////////////////////////////////
//バックグラウンド処理		
///////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : バックグラウンド処理のIDを返す
関    数    名 : GetBGProcID
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int	CPimpomAPI::GetBGProcID()
{
	return  background_proc_id;
}

/********************************************************************
機  能  名  称 : バックグラウンド処理のIDを設定
関    数    名 : SetBGProcID
引          数 : int id(in)
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::SetBGProcID(int id)
{
	if(background_proc_id==BGPROC_NOTHING){
		background_proc_id=id;
	}
}

/********************************************************************
機  能  名  称 : バックグラウンド処理の終了
関    数    名 : StopBGProc
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::StopBGProc()
{
	background_proc_id=BGPROC_NOTHING;
}



//*********************************************************
// 説明:
//   この関数は、バージョンリソースのコピーへのポインタ返す。
//   バージョンリソースのコピーは、使用後に解放しなくてはならない。
//
// 戻り値:
//     void * …… 関数が成功すると バージョンリソースのコピーへのポインタ を
//            　　 関数が失敗すると NULL を返す。
//
// 引数:
//     char   *FileName // ファイル名
//     DWORD **dwTrans  // バージョンリソースの言語情報
//*********************************************************
static void *GetVersionInfo( char *FileName, DWORD **dwTrans )
{
	void  *pVersionInfo; // バージョンリソースを指すポインタ
	UINT   uDumy;        //
	DWORD  dwDumy;       //
	DWORD  dwSize;       // バージョンリソースのサイズ

	// バージョンリソースのサイズを取得
	dwSize = GetFileVersionInfoSize( FileName, &dwDumy );
	if ( dwSize <= 0 )
	{
		// 失敗
		return NULL;
	}

	pVersionInfo = malloc( dwSize );
	if ( !pVersionInfo )
	{
		// 失敗
		return NULL;
	}

	// バージョンリソースを取得
	if ( GetFileVersionInfo( FileName, 0, dwSize, pVersionInfo ) )
	{
		// バージョンリソースの言語情報を取得
		if ( VerQueryValue( pVersionInfo, "\\VarFileInfo\\Translation", (void **)dwTrans, &uDumy ) )
		{
			return pVersionInfo;
		}
	}
	free( pVersionInfo );

	return NULL; // 失敗
}//GetVersionInfo

//*********************************************************
// 説明:
//   バージョンリソースから、バージョン情報のコピーを取得する。
//
// 戻り値:
//     BOOL …… 関数が成功すると TRUE  を、
//          　　 関数が失敗すると FALSE を返す。
//
// 引数:
//           char  *Buffer       …… 取得するバージョン情報を受け取るバッファ
//           void  *pVersionInfo …… 関数GetVersionInfo() の戻り値
//           DWORD  dwTrans      …… バージョンリソースの言語情報
//     const char  *KeyWord      …… 取得したいバージョン情報の名前
//       "Comments"         // コメント
//       "CompanyName"      // 会社名
//       "FileDescription"  // 説明
//       "FileVersion"      // ファイルバージョン
//       "InternalName"     // 内部名
//       "LegalCopyright"   // 著作権
//       "LegalTrademarks"  // 商標
//       "OriginalFilename" // 正式ファイル名
//       "PrivateBuild"     // プライベートビルド情報
//       "ProductName"      // 製品名
//       "ProductVersion"   // 製品バージョン
//       "SpecialBuild"     // スペシャルビルド情報
//*********************************************************
static BOOL GetVersionValue( char *Buffer, void *pVersionInfo, DWORD dwTrans, const char *KeyWord )
{
	char *pValue; // バージョンリソース中のバージョン情報を指すポインタ
	char  Path[MAX_PATH+1];
	UINT  uDumy;
	WORD  wCodePageID[] = { 0, 932, 949, 950, 1200, 1250, 1251, 1252, 1253, 1254, 1255, 1256 };
	WORD  wLanguageID[] =
		{
			0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0406, 0x0407, 0x0408,
			0x0409, 0x040A, 0x040B, 0x040C, 0x040D, 0x040E, 0x040F, 0x0410,
			0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417, 0x0418,
			0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F, 0x0420,
			0x0421, 0x0804, 0x0807, 0x0809, 0x080A, 0x080C, 0x0810,
			0x0813, 0x0814, 0x0816, 0x081A, 0x0C0C, 0x100C
		};

	sprintf( Path, "\\StringFileInfo\\%04x%04x\\%s", LOWORD(dwTrans), HIWORD(dwTrans), KeyWord );
	if ( VerQueryValue( pVersionInfo, Path, (void **)&pValue, &uDumy ) )
	{
		strcpy( Buffer, pValue );
		return TRUE; // 成功
	}

	// バージョンリソースに言語情報が記録されていない場合
	for( int i = 0; i < ( numof(wCodePageID) ); i++ )
	{
		for( int j = 0; j < numof(wLanguageID); j++ )
		{
			sprintf
			(
				Path,
				"\\StringFileInfo\\%04x%04x\\%s",
				wLanguageID[j],
				wCodePageID[i],
				KeyWord
			);
			if ( VerQueryValue( pVersionInfo, Path, (void **)&pValue, &uDumy ) )
			{
				strcpy( Buffer, pValue );
				return TRUE; // 成功
			}
		}
	}

	Buffer[0] = '\0';
	return FALSE; // 失敗
}//GetVersionValue

 

/********************************************************************
機  能  名  称 : バージョンリソース情報
関    数    名 : GetVersion
引          数 : 
//     const char  *KeyWord      …… 取得したいバージョン情報の名前
//       "Comments"         // コメント
//       "CompanyName"      // 会社名
//       "FileDescription"  // 説明
//       "FileVersion"      // ファイルバージョン
//       "InternalName"     // 内部名
//       "LegalCopyright"   // 著作権
//       "LegalTrademarks"  // 商標
//       "OriginalFilename" // 正式ファイル名
//       "PrivateBuild"     // プライベートビルド情報
//       "ProductName"      // 製品名
//       "ProductVersion"   // 製品バージョン
//       "SpecialBuild"     // スペシャルビルド情報
戻    り    値 : バージョンリソース情報
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
CString CPimpomAPI::GetVersion(const char  *KeyWord)
{
	CString verStr;
	char   strVersion[100];
	void  *pVersionInfo;
	DWORD *dwTrans;
	CString filepath = GetDefaultPath() + "pimpom_core.dll";

	pVersionInfo = GetVersionInfo( filepath.GetBuffer(), &dwTrans );
	if ( pVersionInfo )
	{
		GetVersionValue( strVersion,  pVersionInfo, *dwTrans, KeyWord  );

		verStr.Format("%s", strVersion);

		free( pVersionInfo );
	}

	filepath.ReleaseBuffer();
	
	return verStr; 
}




/********************************************************************
機  能  名  称 : 数値を文字列に変換する
関    数    名 : ToString
引          数 : float				value				(in)数値
				 int				ordered_length		(in)長さ指定(-1の場合は指定しない)
				 char				fillchar			(in)文字列長さがordered_lengthに満たない場合に埋める文字
				 char				filldir				(in)文字列長さがordered_lengthに満たない場合にどちらから埋めるか 1以外:左から 1:右から
戻    り    値 : 変換された文字列
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
CString		CPimpomAPI::ToString(float value, int ordered_length, char fillchar, char filldir)
{
	CString		str;

	str.Format("%f", value);

	long len = str.GetLength();
	for(int i = len - 1  ;  i>0 ; i--){//文字列の右端の0を消した文字列をつくる
		if( str.GetAt(i) != '0' ){
			str = str.Left(i+1);
			break;
		}
	}

	//再右端が.ならばこれも消す
	if (str.GetAt(str.GetLength() - 1) == '.') {
		str = str.Left(str.GetLength() - 1);
	}



	if(ordered_length>0)//文字列の長さを指定する場合
	{
		if( ordered_length > str.GetLength() ){//文字列が指定された長さより短い場合
			for (int i = str.GetLength(); i < ordered_length; i++) {
				if (filldir == 1) {//右端から埋める
					str += CString(fillchar);
				}else {//左端からうめる
					str = CString(fillchar) + str;
				}
			}
								
		}else if( ordered_length >0  &&  ordered_length < str.GetLength()  ){//文字列が指定された長さより長い場合
			str = str.Left( ordered_length );
		}
	}

	return	str;
}

/********************************************************************
機  能  名  称 : 文字列を数値に変換する
関    数    名 : ToString
引          数 : CString			str				(in)文字列
				 double				*presult		(out)変換された数値
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::ToNumber(CString str, double *presult)
{
	double result = (float)atof( str );//文字列を数値にする（エラーの場合 atof は　0.0を返す）

	if(result == 0.0){//0.0が数値として返された場合
		//値が本当に0.0のとき（エラーでない場合）は文字列は '0' , '.' , ' 'のみで構成されている
		for(int i=0 ; i<str.GetLength() ; i++){
			if( str[i] != '0'   &&    str[i] != '.'   &&   str[i] != ' ')		return	false;
		}
	}

	*presult = result;

	return true;
}


/********************************************************************
機  能  名  称 : 高精度タイマー取得
関    数    名 : Tick
引          数 : 
戻    り    値 : PIMPOM起動時からの経過時間[ms]
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
DWORD	CPimpomAPI::Tick()
{
	LARGE_INTEGER nFreq, nTimeCnt;

	//変数の初期化
	memset(&nFreq, 0x00, sizeof nFreq);
	memset(&nTimeCnt, 0x00, sizeof nTimeCnt);

	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nTimeCnt);


	return (DWORD)((nTimeCnt.QuadPart) * 1000 / nFreq.QuadPart);
}