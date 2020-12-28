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
//CIO3DDlg
//3次元画像の入出力ダイアログ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "opencv_headers.h"
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
extern	CPimpomAPI	API;

// *************************************
//         関    数    参    照         
// *************************************

bool get_file_footer(CString pathname, CString ext, short *pwidth, short *pheight, short *ppage, short *pchannel);

//////////////////////////////////////////////////////////////////////////////////////////
//  メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////

CIO3DDlg::CIO3DDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIO3DDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIO3DDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CIO3DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIO3DDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}

/********************************************************************
機  能  名  称 : ファイルタイプの変更
関    数    名 : change_filetype_radiobutton
引          数 : int type　(in)
戻    り    値 : 
機          能 : ラジオボタン選択状態の変更
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CIO3DDlg::change_filetype_radiobutton(int type)
{
	bool	radio_button_check[3];
		for(int i=0 ; i<3 ; i++)	radio_button_check[i] = false;

	radio_button_check[type] = true;

	API.SetCheck( m_hWnd , IDC_RADIO_IO3D_RAW , radio_button_check[FILE_TYPE_RAW] );
	API.SetCheck( m_hWnd , IDC_RADIO_IO3D_BMP , radio_button_check[FILE_TYPE_BMP] );
	API.SetCheck( m_hWnd , IDC_RADIO_IO3D_FLOAT , radio_button_check[FILE_TYPE_FLOAT] );
}

/********************************************************************
機  能  名  称 : このダイアログを保存モードで起動する
関    数    名 : DoModalStoreType
引          数 : int data_number　(in)保存元のデータが入った画像メモリ番号
戻    り    値 : DoModalと同じ
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CIO3DDlg::DoModalStoreType(long data_number)
{
	CDataUnit *p_du;

	do_store = true;//保存タイプのフラグを立てる
	target_image_number = data_number;//ターゲットとなる画像メモリを指定する

	p_du = API.GetDataUnit( target_image_number );
		if( p_du == NULL )	return	IDCANCEL;//ターゲットの画像が無ければなにもしない
		if( p_du->PageNumber<=1 )	return	IDCANCEL;//ページが1以下の場合には何もしない

	return	DoModal();//ダイアログをモーダルとして初期化する
}

/********************************************************************
機  能  名  称 : このダイアログを読み込みモードで起動する
関    数    名 : DoModalLoadType
引          数 : int data_number　(in)読み込み先の画像メモリ番号
戻    り    値 : DoModalと同じ
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CIO3DDlg::DoModalLoadType(long data_number)
{
	do_store = false;//入力タイプのフラグを立てる
	target_image_number = data_number;//ターゲットとなる画像メモリを指定する

	return	DoModal();//ダイアログをモーダルとして初期化する
}

/********************************************************************
機  能  名  称 : 3次元データを複数枚のファイルとして保存
関    数    名 : store
引          数 : 
戻    り    値 : 正常に保存できればtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CIO3DDlg::store()
{
	CDataUnit	*p_du;
	CListBox	*p_list;
	float		width, height;
	CSize		size;
	CString		file_path, file_name, mes;
	int			n, file_number;
	bool		ret;


	p_du = API.GetDataUnit( target_image_number );//ターゲットの画像データを取得する
		if( p_du == NULL )	return	false;//ターゲットの画像が無ければなにもしない

	//ファイル名のリストを作る
	p_list = (CListBox*)GetDlgItem( IDC_LISTBOX_IO3D_FILENAME );//リストボックスのポインタを取得する
	
	disp_all_save_files();//リストボックスに全てのファイル名を表示する

	//各値を取得
	API.GetEditValue( m_hWnd , IDC_EDIT_IO3D_FILE_PATH , &file_path );//ファイルのパス
	API.GetEditValue( m_hWnd , IDC_EDIT_IO3D_WIDTH , &width );//画像の幅
	API.GetEditValue( m_hWnd , IDC_EDIT_IO3D_HEIGHT , &height );//画像の高さ
	size.cx = (long)width;
	size.cy = (long)height;
	file_number = p_list->GetCount();//ファイルの数

	//保存する
	for(n=0  ;  n<file_number  ;  n++)//全ての画像を分割して保存する
	{
		//保存中の画像を表示する
		p_du->SetDispPage(n);//ページを指定
		API.DrawImage(target_image_number , true);//現在保存中の画像を更新
		p_list->GetText(n, file_name);//ファイル名をリストから取得する

			 if( p_du->DataType == THREE_D_FORMAT )//8bit3Dファイルの場合//////////////////////////
			 {
					if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_RAW) )//RAWファイルとして保存
					{
						ret = API.SaveByteImage( p_du->pByteData + size.cx * size.cy * n ,
															file_path + file_name , size , 1); 
					}
					else if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_BMP) )//ビットマップとして保存
					{
						ret = API.SaveByteAsBitmap(p_du->pByteData + size.cx * size.cy * n ,
												 p_du->pDispBufferBmpInfo, size, file_path + file_name );
					}
					else
					{
						AfxMessageBox("この形式では保存できません");
						return	false;
					}

			}
			 else if(p_du->DataType == FLOAT_3D_FORMAT)//32bit3Dファイルの場合//////////////////////////////
			 {

					if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_RAW) )//RAWファイルとして保存
					{
						ret = API.SaveViewAsRaw(  target_image_number , file_path + file_name );
					}
					else if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_BMP) )//ビットマップとして保存
					{
						ret = API.SaveViewAsBitmap( target_image_number , file_path + file_name );

					}
					else if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_FLOAT) )//Float型として保存
					{
						ret = API.SaveFloatImage( p_du->pFloatData + size.cx * size.cy * n ,
																file_path + file_name , size, 1);
					}
			}
			else  if( p_du->DataType == RGB_3D_FORMAT )//24bitカラー3Dファイルの場合//////////////////////////
			{
					if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_RAW) )//RAWファイルとして保存
					{
						AfxMessageBox("この形式では保存できません");
						return	false;

					}
					else if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_BMP) )//ビットマップとして保存
					{
						p_du->SetDispPage(n);
						API.SetDispRange(target_image_number,0,256);//表示画像の濃度をデータにあわせる
						ret = API.SaveViewAsBitmap(target_image_number,file_path + file_name);//表示画像を保存
					}
					else
					{
						AfxMessageBox("この形式では保存できません");
						return	false;
					}
			}else{
				return	false;
			}

			if(ret == false)//画像の保存に失敗した時
			{
				mes.Format("%d枚目の画像保存に失敗");
				AfxMessageBox(mes);
			}

		API.ShowMessage( true, 0, false, "%d/%d終了" , n+1 , file_number);
	}
	return	true;
}

/********************************************************************
機  能  名  称 : 保存モードで「参照」ボタンが押された
関    数    名 : browse_for_store
引          数 : 
戻    り    値 : 
機          能 : １ファイル選択できるファイルダイアログを出す
				 選択したファイルのタイトルに連続番号をつけて、保存ファイルのリストに表示する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CIO3DDlg::browse_for_store()
{
	CString path_name, file_name, path, title;


	//ファイルダイアログを表示
	CString filestr_any;
	filestr_any.LoadString(IDS_FILETYPE_ANY);
	CFileDialog myDLG(false , NULL , NULL , OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filestr_any + _T("|"));	

		if( myDLG.DoModal() != IDOK )	return;

	//ファイルダイアログからパスとタイトルを取得する
	path_name = myDLG.GetPathName();
	file_name = myDLG.GetFileName();
	path = path_name.Left( path_name.GetLength()  -  file_name.GetLength() );
	title = myDLG.GetFileTitle();

	//パスと連続名を表示
	API.SetEditValue( m_hWnd , IDC_EDIT_IO3D_FILE_PATH , path );
	API.SetEditValue( m_hWnd , IDC_EDIT_IO3D_SERIAL_NAME , title );
}

/********************************************************************
機  能  名  称 : ビットマップファイルのBITMAPINFOHEADERを読み込む
関    数    名 : check_bitmap_header
引          数 : CString			filename	(in)ファイル名
				 BITMAPINFOHEADER	&bmp_header	(out)読み込まれたBMPファイルのBITMAPINFOHEADER
戻    り    値 : 正常に読み込めればtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CIO3DDlg::check_bitmap_header(CString filename, BITMAPINFOHEADER &bmp_header)
{
	CFile  file;
	BITMAPFILEHEADER	bmp_file_header;

		if( ! file.Open(filename,CFile::modeRead|CFile::typeBinary) )	return false;

	//BITMAPFILEHEADERの読みこみ
	file.Read( &bmp_file_header , sizeof(BITMAPFILEHEADER) );
		if( bmp_file_header.bfType != 0x4d42){	
			AfxMessageBox(_T("このファイルはビットマップではありません"));
			file.Close();
			return false;
		}

	//BITMAPINFOHEADERの読みこみ
	file.Read( &bmp_header, sizeof(BITMAPINFOHEADER) );

	file.Close();
	return true;
}

/********************************************************************
機  能  名  称 : 3次元データをファイルから読み込む
関    数    名 : load
引          数 : 
戻    り    値 : 正常に読み込めたらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CIO3DDlg::load()
{
	CListBox	*p_list;
	float		width, height, channel, *p_float_data;
	BYTE		*p_byte_data;
	CSize		size;
	BITMAPINFOHEADER bmp_header = {0};
	CString		file_path, file_name, mes;	
	int			n, file_number;
	bool		ret = true;
	CDataUnit	*pdu;

	p_list = (CListBox*)GetDlgItem( IDC_LISTBOX_IO3D_FILENAME );//リストボックスのポインタを取得する
	if( p_list ==NULL )	return false;

	//各値を取得
	API.GetEditValue( m_hWnd , IDC_EDIT_IO3D_FILE_PATH , &file_path );//ファイルのパス
	API.GetEditValue( m_hWnd , IDC_EDIT_IO3D_WIDTH , &width );//画像の幅
	API.GetEditValue( m_hWnd , IDC_EDIT_IO3D_HEIGHT , &height );//画像の高さ
	API.GetEditValue(m_hWnd, IDC_EDIT_IO3D_CHANNEL, &channel);//画像のチャネル
	size.cx = (long)width;
	size.cy = (long)height;
	file_number = p_list->GetCount();//ファイルの数

	if( file_number ==0 )	return false;

	//メモリを取得
	if (API.GetCheck(m_hWnd, IDC_RADIO_IO3D_LOADBYTE))
	{
		if (channel == 3){
			p_byte_data = API.GetRGB3DMemory(target_image_number, size, file_number, true);
		}
		else{
			p_byte_data = API.Get3DMemory(target_image_number, size, file_number, true);
		}
	}
	else{
		p_float_data = API.GetF3DMemory(target_image_number, size, file_number, true);
	}


	pdu = API.GetDataUnit(target_image_number);

	//ファイルの読み込み
	for(n=0 ; n<file_number ; n++)
	{
		p_list->GetText( n , file_name);
		if (API.GetCheck(m_hWnd, IDC_RADIO_IO3D_LOADBYTE))//RAW
		{
			cv::Mat img = cv::imread((LPCSTR)(file_path + file_name), -1);

			if (img.cols != width || img.rows != height || img.channels() != channel)	return false;

			if (channel==3)
			{
				BYTE *pdstR = p_byte_data + n * size.cx * size.cy * pdu->ChannelNumber;
				BYTE *pdstG = pdstR + size.cx * size.cy;
				BYTE *pdstB = pdstG + size.cx * size.cy;
				for (int j = 0; j <height; j++)
				{
					for (int i = 0; i < width; i++)
					{
						*pdstR = img.at<Vec3b>(j, i)[2];
						*pdstG = img.at<Vec3b>(j, i)[1];
						*pdstB = img.at<Vec3b>(j, i)[0];

						pdstR++;
						pdstG++;
						pdstB++;
					}
				}
			}
			else if (channel == 1)
			{
				BYTE *pdst = p_byte_data + n * size.cx * size.cy * pdu->ChannelNumber;
				BYTE *psrc = img.data;
				for (int j = 0; j <height; j++)
				{
					for (int i = 0; i < width; i++)
					{
						*pdst = *psrc;
						pdst++;
						psrc++;
					}
				}
			}
			else{
				return false;
			}
		}
		else
		{
			ret = API.LoadFloatImage(p_float_data + n * size.cx * size.cy * pdu->ChannelNumber, file_path + file_name, size);
		}

		if(!ret)	return	false;
	}

	API.DrawImage( target_image_number , true);//読み込んだ画像データを再描画する
	return	true;
}

/********************************************************************
機  能  名  称 : 読み込みモードで「参照」ボタンが押された
関    数    名 : browse_for_load
引          数 : 
戻    り    値 : 
機          能 : 複数ファイルを選択できるファイルダイアログを出す
　　　　　　　　 選択されたファイルをリストに表示する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CIO3DDlg::browse_for_load()
{
	int			file_number;
	CString			path_name, file_name[1000], path;

	//ファイルダイアログを表示する
	CString		filestr_any, filestr_raw, filestr_bmp, filestr_float;
	filestr_any.LoadString(IDS_FILETYPE_ANY);
	filestr_raw.LoadString(IDS_FILETYPE_RAW);
	filestr_bmp.LoadString(IDS_FILETYPE_BITMAP);
	filestr_float.LoadString(IDS_FILETYPE_FLOAT);
	if (API.OpenFileDialog(1000, _T("すべての画像ファイル|*.*|") + filestr_bmp + filestr_raw + filestr_float + _T(" | "),
		&path, file_name, &file_number) != IDOK)	return;

	if (file_number > 0){
		setlist_for_load(path, file_name, file_number);
	}

}

void CIO3DDlg::setlist_for_load(CString path, CString file_name[], int file_number)
{
	//最初の１枚読み込んで、幅・高さ・チャネルを決める
	CString fname0 = file_name[0];
	fname0.MakeUpper();
	if(fname0.Right(3) == _T("RAW") )//raw
	{
		API.SetCheck(m_hWnd, IDC_RADIO_IO3D_LOADBYTE, true);

		short w, h, c, p;
		if (get_file_footer(path + file_name[0], _T("RAW"), &w, &h, &p, &c))
		{
			API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_WIDTH, w);//幅を表示する
			API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_HEIGHT, h);//高さを表示する
		}
	}
	else if(fname0.Right(5) == _T("FLOAT") )//float
	{
		API.SetCheck(m_hWnd, IDC_RADIO_IO3D_LOADFLOAT, true);

		short w, h, c, p;
		if (get_file_footer(path + file_name[0], _T("RAW"), &w, &h, &p, &c))
		{
			API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_WIDTH, w);//幅を表示する
			API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_HEIGHT, h);//高さを表示する
		}
	}
	else
	{
		API.SetCheck(m_hWnd, IDC_RADIO_IO3D_LOADBYTE, true);

		cv::Mat img = cv::imread((LPCSTR)(path + file_name[0]), -1);

		if (img.channels() != 1 && img.channels() != 3)	return ;

		API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_WIDTH, img.cols);//幅を表示する
		API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_HEIGHT, img.rows);//高さを表示する
		API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_CHANNEL, img.channels());//チャネルを表示する		
	}


	//リストにファイル名を入れる
	CListBox		*p_list = (CListBox*)GetDlgItem(IDC_LISTBOX_IO3D_FILENAME);
	p_list->ResetContent();//リストをクリアする

	for(int i=0 ; i<file_number ; i++){
			p_list->AddString( file_name[i] );
	}

	API.SetEditValue( m_hWnd , IDC_EDIT_IO3D_FILE_PATH , path );//パスを表示する
	API.SetEditValue( m_hWnd , IDC_IO_3D_PAGE_NUMBER , file_number );//ファイル数を表示する


}

/********************************************************************
機  能  名  称 : 指定されたファイルが存在するかどうかチェックする
関    数    名 : DoFileExist
引          数 : CString	file_name	(in)ファイルパス
戻    り    値 : 存在すればtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CIO3DDlg::DoFileExist(CString file_name)
{
	CFileStatus status;

		if( CFile::GetStatus(file_name , status) )		return  true;
		else											return  false; 
}


//////////////////////////////////////////////////////////////////////////////////////////
//  メッセージ ハンドラ
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CIO3DDlg, CDialog)
	//{{AFX_MSG_MAP(CIO3DDlg)
	ON_BN_CLICKED(IDC_BUTTON_IO3D_REFER, OnIo3dRefer)
	//ON_BN_CLICKED(IDC_BUTTON_IO3D_DISP_ALL_FILES, OnIo3dDispAllFiles)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CIO3DDlg::OnBnClickedOk)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_RADIO_IO3D_RAW, &CIO3DDlg::OnBnClickedRadioIo3dRaw)
	ON_BN_CLICKED(IDC_RADIO_IO3D_BMP, &CIO3DDlg::OnBnClickedRadioIo3dBmp)
	ON_BN_CLICKED(IDC_RADIO_IO3D_FLOAT, &CIO3DDlg::OnBnClickedRadioIo3dFloat)
	ON_EN_CHANGE(IDC_EDIT_IO3D_SERIAL_NAME, &CIO3DDlg::OnEnChangeEditIo3dSerialName)
	ON_BN_CLICKED(IDC_IO_3D_DLG_FOLDER_BROWSE, &CIO3DDlg::OnBnClickedIo3dDlgFolderBrowse)
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
BOOL CIO3DDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString		str;
	CSize		init_size;
	CDataUnit	*p_du;




	if( do_store )//セーブ
	{

		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_RAW), SW_SHOW);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_BMP), SW_SHOW);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_FLOAT), SW_SHOW);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_EDIT_IO3D_SERIAL_NAME), SW_SHOW);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_LOADBYTE), SW_HIDE);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_LOADFLOAT), SW_HIDE);

		p_du = API.GetDataUnit( target_image_number );
		if(p_du == NULL)	return false;
		if(p_du->DataType != THREE_D_FORMAT && p_du->DataType != FLOAT_3D_FORMAT && p_du->DataType != RGB_3D_FORMAT)	return false;

		//ファイルのサイズ
		API.SetEditValue( m_hWnd , IDC_EDIT_IO3D_WIDTH , p_du->DataSize.cx );
		API.SetEditValue( m_hWnd , IDC_EDIT_IO3D_HEIGHT , p_du->DataSize.cy );
		API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_CHANNEL, p_du->ChannelNumber);
	
		::SetWindowText(m_hWnd, p_du->DataName + "の保存");//保存する画像データの名前

		//保存するデータの形式にあわせて選択可能なラジオボタンを変える
		switch( p_du->DataType )
		{
			case THREE_D_FORMAT:
				change_filetype_radiobutton(FILE_TYPE_RAW);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_RAW), true);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_BMP), true);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_FLOAT), false);
				API.SetEditValue( m_hWnd , IDC_IO_3D_PAGE_NUMBER , p_du->PageNumber );
				break;

			case FLOAT_3D_FORMAT:
				change_filetype_radiobutton(FILE_TYPE_FLOAT);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_RAW), false);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_BMP), false);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_FLOAT), false);
				API.SetEditValue( m_hWnd , IDC_IO_3D_PAGE_NUMBER , p_du->PageNumber );
				break;

			case RGB_3D_FORMAT:
				change_filetype_radiobutton(FILE_TYPE_BMP);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_RAW), false);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_BMP), false);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_FLOAT), false);
				API.SetEditValue( m_hWnd , IDC_IO_3D_PAGE_NUMBER , p_du->PageNumber );
				break;
		}

		disp_all_save_files();//ファイル一覧
	}
	else//ロード
	{
		DragAcceptFiles();//ドラッグ&ドロップ

		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_RAW), SW_HIDE);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_BMP), SW_HIDE);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_FLOAT), SW_HIDE);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_EDIT_IO3D_SERIAL_NAME), SW_HIDE);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_LOADBYTE), SW_SHOW);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_LOADFLOAT), SW_SHOW);

		API.SetEditValue( m_hWnd , IDC_EDIT_IO3D_WIDTH , 640 );
		API.SetEditValue( m_hWnd , IDC_EDIT_IO3D_HEIGHT , 480 );
		API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_CHANNEL, 1);
		API.SetEditValue( m_hWnd , IDC_IO_3D_PAGE_NUMBER , 1 );
		API.SetCheck(m_hWnd, IDC_RADIO_IO3D_LOADBYTE, true);

		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_IO3D_FILE_PATH),false);
	}


	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
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
void CIO3DDlg::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
		
	// 描画用メッセージとして CDialog::OnPaint() を呼び出してはいけません
}

/********************************************************************
機  能  名  称 : 「OK」ボタンが押された
関    数    名 : OnOK
引          数 : 
戻    り    値 : 
機          能 : 保存／読み込み実行してダイアログを閉じる
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CIO3DDlg::OnOK() 
{
	if( do_store ){//保存
		if( !store() ){
			AfxMessageBox("保存失敗");
			return;
		}
	}else{//入力
		if( !load() ){
			AfxMessageBox("読み込み失敗");
			return;
		}
	}
	
	CDialog::OnOK();
}


//保存する全てのファイルを表示
void CIO3DDlg::disp_all_save_files() 
{
	CDataUnit	*p_du;
	CListBox	*p_list;
	int		n;
	float	page_number;
	CString	serial_name, serial_number, file_name, extern_name;
	
	p_du = API.GetDataUnit( target_image_number );//ターゲットの画像データを取得する
	if( p_du == NULL )	return;//ターゲットの画像が無ければなにもしない

	//リストボックスのポインタを取得
	p_list = (CListBox*)GetDlgItem( IDC_LISTBOX_IO3D_FILENAME );
	p_list->ResetContent();//リストボックスの中身をリセット

	//各値を取得
	API.GetEditValue( m_hWnd , IDC_EDIT_IO3D_SERIAL_NAME , &serial_name );//ファイルの連続名
	API.GetEditValue( m_hWnd , IDC_IO_3D_PAGE_NUMBER , &page_number );//画像の奥行き	

	//指定したページが画像データの奥行きよりも大きい時は画像データの奥行きにあわせる
	if(page_number > p_du->PageNumber)
	{
		page_number = (float)p_du->PageNumber;
		API.SetEditValue( m_hWnd , IDC_IO_3D_PAGE_NUMBER , page_number );
	}

	for(n=0 ; n<page_number ; n++)
	{
		serial_number.Format("%03d", n);//続き番号をきめる	

		//拡張子を決める
		if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_RAW) )				extern_name = _T(".RAW");
		else if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_BMP))			extern_name = _T(".BMP");
		else if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_FLOAT))		extern_name = _T(".FLOAT");

		file_name = serial_name + serial_number + extern_name;//ファイル名 = ファイルの連続名 + 続き番号 + 拡張子
			if(!do_store)//読み込みの時
			{
				if(!DoFileExist(file_name))//ファイルが存在しないとき
				{
					AfxMessageBox( file_name + _T("はこのディレクトリには存在しません") );
					break;
				}
			}
		p_list->AddString( file_name );//リストに追加する
	}	
}

/********************************************************************
機  能  名  称 : 「参照」ボタンが押された
関    数    名 : OnIo3dRefer
引          数 : 
戻    り    値 : 
機          能 : 保存モード：保存する一連のファイルのファイル名を既存ファイル名を参照して作る
				 読み込みモード：読み込む複数の画像ファイルを選択する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
//「参照」ボタンが押された
void CIO3DDlg::OnIo3dRefer() 
{
	if( do_store ){//保存
		browse_for_store();
	}else{//入力
		browse_for_load();
	}	
}

//OKボタン押下
void CIO3DDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OnOK();
}



/********************************************************************
機  能  名  称 : ドラッグアンドドロップ
関    数    名 : OnDropFiles
引          数 :
戻    り    値 :
機          能 : 画像がドロップされたらリストボックスに追加する
日付         作成者          内容
------------ --------------- ---------------------------------------
			Y.Ikeda         新規作成
********************************************************************/
void CIO3DDlg::OnDropFiles(HDROP hDropInfo)
{
	if (do_store){//保存
		
	}
	else//入力
	{
		int		file_num = (int)DragQueryFileA(hDropInfo, -1, NULL, 0);//ドロップされたファイル数を取得
		CString		file_name[10000], path;

		for (int i = 0; i<file_num; i++)
		{
			UINT size = DragQueryFile(hDropInfo, i, NULL, 0) + 1;//ファイル名の長さを取得

			if (size>3)
			{
				CString pathname;
				DragQueryFile(hDropInfo, i, pathname.GetBuffer(size), size);
				pathname.ReleaseBuffer();

				file_name[i] = pathname.Right(pathname.GetLength() - pathname.ReverseFind(_T('\\')) - 1);//ファイル名分離

				if (i == 0){//ファイルパス分離
					path = pathname.Left(pathname.ReverseFind(_T('\\')) +1);
				}
			}
		}
		DragFinish(hDropInfo);


		setlist_for_load(path, file_name, file_num);

	}
	CDialog::OnDropFiles(hDropInfo);
}






void CIO3DDlg::OnBnClickedRadioIo3dRaw()
{
	if (do_store){//保存
		disp_all_save_files();
	}
	else
	{//入力
	}
}


void CIO3DDlg::OnBnClickedRadioIo3dBmp()
{
	if (do_store){//保存
		disp_all_save_files();
	}
	else
	{//入力
	}
}


void CIO3DDlg::OnBnClickedRadioIo3dFloat()
{
	if (do_store){//保存
		disp_all_save_files();
	}
	else
	{//入力
	}
}


void CIO3DDlg::OnEnChangeEditIo3dSerialName()
{
	if (do_store){//保存
		disp_all_save_files();
	}
	else
	{//入力
	}
}




void CIO3DDlg::OnBnClickedIo3dDlgFolderBrowse()
{
	CString path;
	if (API.FolderDialog(&path))
	{
		API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_FILE_PATH, path + _T("\\"));//パスを表示する
	}
}